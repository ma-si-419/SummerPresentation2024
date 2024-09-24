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
	m_stanTime(0),
	m_nowHp(0),
	m_nowMp(0),
	m_isAttack(false),
	m_animTime(0),
	m_animPlaySpeed(0),
	m_totalAnimTime(0),
	m_isLoopAnim(false)
{
	m_modelHandle = MV1LoadModel(model);
	//auto& coldata = std::dynamic_pointer_cast<CapsuleColliderData>;
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

std::shared_ptr<AttackBase> CharacterBase::CreateAttack(std::shared_ptr<Physics> physics, std::string id, bool isPlayer)
{
	//std::shared_ptr<AttackBase> ans;
	////�U���f�[�^�̎�ނ��擾
	//if (isPlayer)
	//{
	//	ans = std::make_shared<AttackBase>(ObjectTag::kPlayerAttack);
	//}
	//else
	//{
	//	ans = std::make_shared<AttackBase>(ObjectTag::kEnemyAttack);
	//}
	////�U�����o�����W���쐬
	//MyEngine::Vector3 toTargetVec = m_attackTarget - m_rigidbody.GetPos();
	//MyEngine::Vector3 attackPos = m_rigidbody.GetPos() + toTargetVec.Normalize() * m_attackData[id].radius;

	//ans->SetStatus(m_attackData[id], m_attackTarget, m_rigidbody.GetPos(), m_status.atk);
	////�X�e�[�^�X�ݒ�
	//ans->Init(physics, attackPos, m_effekseerHandle);

	//return ans;
	return 0;
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
	m_animLoopStartTime = m_animData[animName].loopFrame;
	//���[�v���I���t���[����ݒ�
	m_animLoopEndTime = m_animData[animName].endFrame;
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
	m_animLoopStartTime = m_animData["SpAttackEnd"].loopFrame;
	//���[�v���I���t���[����ݒ�
	m_animLoopEndTime = m_animData["SpAttackEnd"].endFrame;
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

void CharacterBase::PlaySpecialAttack(std::string id)
{
	//�Z�œ����Ȃ����Ԃ�ݒ肷��
	m_stanTime = m_attackData[id].actionTotalTime;
	//�Z�̏���C�͕��A���݂̋C�͂����炷
	m_nowMp -= m_attackData[id].cost;
	//�U���̃^�[�Q�b�g���W�����b�N����
	m_attackTarget = m_targetPos;
	//�U�����o���Ă����Ԃɕς���
	m_isAttack = true;
	//�o���Ă���U���̎�ނ��擾����
	m_attackId = id;
	//�G���߂��ɂ��邩�ǂ�����false�ɂ���
	m_isNearTarget = false;
}

void CharacterBase::SetModelFront(MyEngine::Vector3 pos)
{
	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - pos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
}

void CharacterBase::SetNormalAttack(bool isPhysical, int time)
{
	//���o���Ă���U���̊m�F���ďo���U����ݒ肷��


	//�i���U���̏ꍇ
	if (isPhysical)
	{
		if (time < kPhysicalAttackConnectTime)
		{

			//�i���U���̈�i�ڂ��o���Ă�����
			if (m_attackId == CommandId::kPhysicalAttack1)
			{
				//�i���U���̓�i�ڂ��o��
				m_attackId = CommandId::kPhysicalAttack2;
			}
			//�i���U���̓�i�ڂ��o���Ă�����
			else if (m_attackId == CommandId::kPhysicalAttack2)
			{
				//�i���U���̎O�i�ڂ��o��
				m_attackId = CommandId::kPhysicalAttack3;
			}
			//�i���Z�̈�i�ځA��i�ڂ��o���Ă��Ȃ����
			else
			{
				m_attackId = CommandId::kPhysicalAttack1;
			}
		}
		//�i���U�����R���{�ɂȂ�^�C�~���O�łȂ����
		else
		{
			//�i���U���̈�i�ڂ��o��
			m_attackId = CommandId::kPhysicalAttack1;
		}
		//�G���߂��ɂ��邩�ǂ�����false�ɂ���
		m_isNearTarget = false;
	}
	//�C�e�U���̏ꍇ
	else
	{
		if (time < kEnergyAttackConnectTime)
		{

			//�C�e�U���̈�i�ڂ��o���Ă�����
			if (m_attackId == CommandId::kEnergyAttack1)
			{
				//�C�e�U���̓�i�ڂ��o��
				m_attackId = CommandId::kEnergyAttack2;
			}
			//�C�e�U���̓�i�ڂ��o���Ă�����
			else if (m_attackId == CommandId::kEnergyAttack2)
			{
				//�C�e�U���̎O�i�ڂ��o��
				m_attackId = CommandId::kEnergyAttack3;
			}
			//�C�e�U���̎O�i�ڂ��o���Ă�����
			else if (m_attackId == CommandId::kEnergyAttack3)
			{
				//�C�e�U���̎l�i�ڂ��o��
				m_attackId = CommandId::kEnergyAttack4;
			}
			//�C�e�U���̈�A��A�O�i�ڂłȂ����
			else
			{
				//�C�e�U���̈�i�ڂ��o��
				m_attackId = CommandId::kEnergyAttack1;
			}
		}
		//�C�e�U�����R���{�ɂȂ�^�C�~���O�o�Ȃ����
		else
		{
			//�C�e�U����1�i�ڂ��o��
			m_attackId = CommandId::kEnergyAttack1;
		}
	}
	//�U���̃^�[�Q�b�g���W�����b�N����
	m_attackTarget = m_targetPos;
	//�Z�œ����Ȃ����Ԃ�ݒ肷��
	m_stanTime = m_attackData[m_attackId].actionTotalTime;
	//�U�����o���Ă����Ԃɕς���
	m_isAttack = true;

}

