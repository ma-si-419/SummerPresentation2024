#include "EnemyStateDodge.h"
#include "EffekseerManager.h"

namespace
{
	//最低何フレームこのStateでいるか
	constexpr int kShortestTime = 8;
	//移動速度
	constexpr float kMoveSpeed = 4.0f;
	//動きの方向の数
	constexpr int kMoveDirNum = 8;
	//動きの方向の数の半分
	constexpr int kMoveDirNumHalf = static_cast<int>(kMoveDirNum * 0.5);
}
void EnemyStateDodge::Init()
{
	MyEngine::Vector3 moveDir = MyEngine::Vector3(GetRand(kMoveDirNum) - kMoveDirNumHalf, 0, GetRand(kMoveDirNum) - kMoveDirNumHalf);
	//移動方向のランダムがどちらも0で出たら
	if (moveDir.sqLength() == 0)
	{
		moveDir = MyEngine::Vector3(0, 0, 1);
	}
	moveDir = moveDir.Normalize();
	m_velo = moveDir * kMoveSpeed;
	m_pEnemy->ChangeAnim("Move");
	MyEngine::Vector3 pos = m_pEnemy->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Dodge"), pos, false);
	EffekseerManager::GetInstance().Entry(effect);
	m_pEnemy->SetEffectData(effect);
}
void EnemyStateDodge::Update()
{
	//経過時間
	m_time++;
	//移動
	m_pEnemy->SetVelo(m_velo);

	m_pEnemy->SetModelFront(m_velo + m_pEnemy->GetPos());
	m_isLookPlayer = false;

	m_pEnemy->PlayAnim();

	//時間になったらすぐにこのStateを抜ける
	if (m_time > kShortestTime)
	{
		m_isChangeState = true;
		m_pEnemy->EndEffect();
	}
}

int EnemyStateDodge::OnDamage(std::shared_ptr<Collidable> collider)
{
	//すべて0で返す
	return 0;
}
