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
	//格闘攻撃を連続で出したかどうかを判断する時間
	constexpr int kPhysicalAttackConnectTime = 30;
	//気弾攻撃を連続で出したかどうかを判断する時間
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
		//入れるデータ
		AnimationInfo pushData;
		//プレイヤーかどうかが一致していればデータを入れる
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
	//前のアニメーションをデタッチする
	MV1DetachAnim(m_modelHandle, m_playAnim);
	//アニメの再生速度を設定
	m_animPlaySpeed = m_animData[animName].playSpeed;
	//アニメの再生時間をリセット
	m_animTime = 0;
	//アニメーションのループするフレームを設定
	m_animLoopStartTime = static_cast<float>(m_animData[animName].loopFrame);
	//ループが終わるフレームを設定
	m_animLoopEndTime = static_cast<float>(m_animData[animName].endFrame);
	//ループ終了フレームが0じゃなければループフラグを立てる
	if (m_animLoopEndTime > 0)
	{
		m_isLoopAnim = true;
	}
	//新しいアニメーションをアタッチする
	m_playAnim = MV1AttachAnim(m_modelHandle, m_animData[animName].number);
	//アニメーションの総再生時間を設定する
	m_totalAnimTime = MV1GetAnimTotalTime(m_modelHandle, m_animData[animName].number);
}

bool CharacterBase::PlayAnim()
{
	//アニメーションの再生フレームを進める
	m_animTime += m_animPlaySpeed;
	//アニメーションの繰り返しが行われるのなら
	if (m_isLoopAnim)
	{
		//アニメーションの繰り返し終了時間が過ぎたら
		if (m_animTime > m_animLoopEndTime)
		{
			//アニメーションの繰り返し開始時間まで戻す
			m_animTime = m_animLoopStartTime;
		}
	}
	//アニメーションの総再生フレームを超えたら
	if (m_animTime > m_totalAnimTime)
	{
		//再生フレームを0に戻す
		m_animTime = 0;
		//再生を止める
		m_animPlaySpeed = 0;
		return true;
	}
	//再生フレームを反映させる
	MV1SetAttachAnimTime(m_modelHandle, m_playAnim, m_animTime);
	return false;
}

void CharacterBase::StopAnimLoop()
{
	m_isLoopAnim = false;
}

void CharacterBase::SetAttackEndAnim(float attackEndTime)
{
	//前のアニメーションをデタッチする
	MV1DetachAnim(m_modelHandle, m_playAnim);
	//アニメの再生時間をリセット
	m_animTime = 0;
	//アニメーションのループするフレームを設定
	m_animLoopStartTime = static_cast<float>(m_animData["SpAttackEnd"].loopFrame);
	//ループが終わるフレームを設定
	m_animLoopEndTime = static_cast<float>(m_animData["SpAttackEnd"].endFrame);
	//ループ終了フレームが0じゃなければループフラグを立てる
	m_isLoopAnim = m_animLoopEndTime > 0;
	//新しいアニメーションをアタッチする
	m_playAnim = MV1AttachAnim(m_modelHandle, m_animData["SpAttackEnd"].number);
	//アニメーションの総再生時間を設定する
	m_totalAnimTime = MV1GetAnimTotalTime(m_modelHandle, m_animData["SpAttackEnd"].number);
	//アニメの再生速度を設定
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
