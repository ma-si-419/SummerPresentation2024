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

std::shared_ptr<AttackBase> CharacterBase::CreateAttack(std::shared_ptr<Physics> physics, std::string id, bool isPlayer)
{
	//std::shared_ptr<AttackBase> ans;
	////攻撃データの種類を取得
	//if (isPlayer)
	//{
	//	ans = std::make_shared<AttackBase>(ObjectTag::kPlayerAttack);
	//}
	//else
	//{
	//	ans = std::make_shared<AttackBase>(ObjectTag::kEnemyAttack);
	//}
	////攻撃を出す座標を作成
	//MyEngine::Vector3 toTargetVec = m_attackTarget - m_rigidbody.GetPos();
	//MyEngine::Vector3 attackPos = m_rigidbody.GetPos() + toTargetVec.Normalize() * m_attackData[id].radius;

	//ans->SetStatus(m_attackData[id], m_attackTarget, m_rigidbody.GetPos(), m_status.atk);
	////ステータス設定
	//ans->Init(physics, attackPos, m_effekseerHandle);

	//return ans;
	return 0;
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
	m_animLoopStartTime = m_animData[animName].loopFrame;
	//ループが終わるフレームを設定
	m_animLoopEndTime = m_animData[animName].endFrame;
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
	m_animLoopStartTime = m_animData["SpAttackEnd"].loopFrame;
	//ループが終わるフレームを設定
	m_animLoopEndTime = m_animData["SpAttackEnd"].endFrame;
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

void CharacterBase::PlaySpecialAttack(std::string id)
{
	//技で動けない時間を設定する
	m_stanTime = m_attackData[id].actionTotalTime;
	//技の消費気力分、現在の気力を減らす
	m_nowMp -= m_attackData[id].cost;
	//攻撃のターゲット座標をロックする
	m_attackTarget = m_targetPos;
	//攻撃を出している状態に変える
	m_isAttack = true;
	//出している攻撃の種類を取得する
	m_attackId = id;
	//敵が近くにいるかどうかをfalseにする
	m_isNearTarget = false;
}

void CharacterBase::SetModelFront(MyEngine::Vector3 pos)
{
	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - pos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
}

void CharacterBase::SetNormalAttack(bool isPhysical, int time)
{
	//今出している攻撃の確認して出す攻撃を設定する


	//格闘攻撃の場合
	if (isPhysical)
	{
		if (time < kPhysicalAttackConnectTime)
		{

			//格闘攻撃の一段目を出していたら
			if (m_attackId == CommandId::kPhysicalAttack1)
			{
				//格闘攻撃の二段目を出す
				m_attackId = CommandId::kPhysicalAttack2;
			}
			//格闘攻撃の二段目を出していたら
			else if (m_attackId == CommandId::kPhysicalAttack2)
			{
				//格闘攻撃の三段目を出す
				m_attackId = CommandId::kPhysicalAttack3;
			}
			//格闘技の一段目、二段目を出していなければ
			else
			{
				m_attackId = CommandId::kPhysicalAttack1;
			}
		}
		//格闘攻撃がコンボになるタイミングでなければ
		else
		{
			//格闘攻撃の一段目を出す
			m_attackId = CommandId::kPhysicalAttack1;
		}
		//敵が近くにいるかどうかをfalseにする
		m_isNearTarget = false;
	}
	//気弾攻撃の場合
	else
	{
		if (time < kEnergyAttackConnectTime)
		{

			//気弾攻撃の一段目を出していたら
			if (m_attackId == CommandId::kEnergyAttack1)
			{
				//気弾攻撃の二段目を出す
				m_attackId = CommandId::kEnergyAttack2;
			}
			//気弾攻撃の二段目を出していたら
			else if (m_attackId == CommandId::kEnergyAttack2)
			{
				//気弾攻撃の三段目を出す
				m_attackId = CommandId::kEnergyAttack3;
			}
			//気弾攻撃の三段目を出していたら
			else if (m_attackId == CommandId::kEnergyAttack3)
			{
				//気弾攻撃の四段目を出す
				m_attackId = CommandId::kEnergyAttack4;
			}
			//気弾攻撃の一、二、三段目でなければ
			else
			{
				//気弾攻撃の一段目を出す
				m_attackId = CommandId::kEnergyAttack1;
			}
		}
		//気弾攻撃がコンボになるタイミング出なければ
		else
		{
			//気弾攻撃の1段目を出す
			m_attackId = CommandId::kEnergyAttack1;
		}
	}
	//攻撃のターゲット座標をロックする
	m_attackTarget = m_targetPos;
	//技で動けない時間を設定する
	m_stanTime = m_attackData[m_attackId].actionTotalTime;
	//攻撃を出している状態に変える
	m_isAttack = true;

}

