#include "CharacterBase.h"
#include <cassert>
#include "AttackBase.h"
#include "Physics.h"
#include "CommandIdList.h"
#include "LoadCsv.h"
#include "EffekseerForDXLib.h"
#include "SceneGame.h"
#include "EffekseerData.h"

namespace
{
	//�i���U����A���ŏo�������ǂ����𔻒f���鎞��
	constexpr int kPhysicalAttackConnectTime = 30;
	//�C�e�U����A���ŏo�������ǂ����𔻒f���鎞��
	constexpr int kEnergyAttackConnectTime = 30;
}

CharacterBase::CharacterBase(const TCHAR* model, ObjectTag tag) :
	Collidable(tag, ColliderData::Kind::kCapsule),
	m_nowHp(0),
	m_nowMp(0),
	m_animTime(0),
	m_animPlaySpeed(0),
	m_totalAnimTime(0),
	m_isLoopAnim(false),
	m_animLoopEndTime(0),
	m_animLoopStartTime(0),
	m_playAnim(0)
{
	m_modelHandle = MV1LoadModel(model);
}

CharacterBase::~CharacterBase()
{
	MV1DeleteModel(m_modelHandle);
}

void CharacterBase::SetTargetPos(MyEngine::Vector3 pos)
{
	m_targetPos = pos;
}

void CharacterBase::SetAnimationData(std::vector<std::vector<std::string>> data, bool isPlayer)
{
	for (auto& item : data)
	{
		//�����f�[�^
		AnimationInfo pushData;
		//�v���C���[���ǂ�������v���Ă���΃f�[�^������
		if (isPlayer == static_cast<bool>(std::stoi(item[static_cast<int>(AnimationInfoSort::kIsPlayer)])))
		{
			pushData.number = std::stoi(item[static_cast<int>(AnimationInfoSort::kNumber)]);
			pushData.loopFrame = std::stoi(item[static_cast<int>(AnimationInfoSort::kLoopFrame)]);
			pushData.endFrame = std::stoi(item[static_cast<int>(AnimationInfoSort::kEndFrame)]);
			pushData.playSpeed = std::stof(item[static_cast<int>(AnimationInfoSort::kPlaySpeed)]);

			m_animData[item[static_cast<int>(AnimationInfoSort::kName)]] = pushData;
		}
	}
}

void CharacterBase::ChangeAnim(std::string animName)
{
	//�O�̃A�j���[�V�������f�^�b�`����
	MV1DetachAnim(m_modelHandle, m_playAnim);
	//�A�j���̍Đ����x��ݒ�
	m_animPlaySpeed = m_animData[animName].playSpeed;
	//�A�j���̍Đ����Ԃ����Z�b�g
	m_animTime = 0;
	//�A�j���[�V�����̃��[�v����t���[����ݒ�
	m_animLoopStartTime = static_cast<float>(m_animData[animName].loopFrame);
	//���[�v���I���t���[����ݒ�
	m_animLoopEndTime = static_cast<float>(m_animData[animName].endFrame);
	//���[�v�I���t���[����0����Ȃ���΃��[�v�t���O�𗧂Ă�
	if (m_animLoopEndTime > 0)
	{
		m_isLoopAnim = true;
	}
	//�V�����A�j���[�V�������A�^�b�`����
	m_playAnim = MV1AttachAnim(m_modelHandle, m_animData[animName].number);
	//�A�j���[�V�����̑��Đ����Ԃ�ݒ肷��
	m_totalAnimTime = MV1GetAnimTotalTime(m_modelHandle, m_animData[animName].number);
}

bool CharacterBase::PlayAnim()
{
	//�A�j���[�V�����̍Đ��t���[����i�߂�
	m_animTime += m_animPlaySpeed;
	//�A�j���[�V�����̌J��Ԃ����s����̂Ȃ�
	if (m_isLoopAnim)
	{
		//�A�j���[�V�����̌J��Ԃ��I�����Ԃ��߂�����
		if (m_animTime > m_animLoopEndTime)
		{
			//�A�j���[�V�����̌J��Ԃ��J�n���Ԃ܂Ŗ߂�
			m_animTime = m_animLoopStartTime;
		}
	}
	//�A�j���[�V�����̑��Đ��t���[���𒴂�����
	if (m_animTime > m_totalAnimTime)
	{
		//�Đ��t���[����0�ɖ߂�
		m_animTime = 0;
		//�Đ����~�߂�
		m_animPlaySpeed = 0;
		return true;
	}
	//�Đ��t���[���𔽉f������
	MV1SetAttachAnimTime(m_modelHandle, m_playAnim, m_animTime);
	return false;
}

void CharacterBase::StopAnimLoop()
{
	m_isLoopAnim = false;
}

void CharacterBase::SetAttackEndAnim(float attackEndTime)
{
	//�O�̃A�j���[�V�������f�^�b�`����
	MV1DetachAnim(m_modelHandle, m_playAnim);
	//�A�j���̍Đ����Ԃ����Z�b�g
	m_animTime = 0;
	//�A�j���[�V�����̃��[�v����t���[����ݒ�
	m_animLoopStartTime = static_cast<float>(m_animData["SpAttackEnd"].loopFrame);
	//���[�v���I���t���[����ݒ�
	m_animLoopEndTime = static_cast<float>(m_animData["SpAttackEnd"].endFrame);
	//���[�v�I���t���[����0����Ȃ���΃��[�v�t���O�𗧂Ă�
	m_isLoopAnim = m_animLoopEndTime > 0;
	//�V�����A�j���[�V�������A�^�b�`����
	m_playAnim = MV1AttachAnim(m_modelHandle, m_animData["SpAttackEnd"].number);
	//�A�j���[�V�����̑��Đ����Ԃ�ݒ肷��
	m_totalAnimTime = MV1GetAnimTotalTime(m_modelHandle, m_animData["SpAttackEnd"].number);
	//�A�j���̍Đ����x��ݒ�
	m_animPlaySpeed = m_totalAnimTime / attackEndTime;
}

void CharacterBase::PlaySE(std::string name, int playType)
{
	m_pScene->PlaySE(name, playType);
}

void CharacterBase::StopSE(std::string name)
{
	m_pScene->StopSE(name);
}

void CharacterBase::EndEffect()
{
	if (m_pEffectData)
	{
		m_pEffectData->Final();
		m_pEffectData = nullptr;
	}
}

void CharacterBase::SetModelFront(MyEngine::Vector3 pos)
{
	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - pos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
}
