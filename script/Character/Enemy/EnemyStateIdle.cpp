#include "EnemyStateIdle.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"

namespace
{
	//最低何フレームこのStateでいるか
	constexpr int kShortestTime = 30;
}

void EnemyStateIdle::Init()
{
	m_pEnemy->ChangeAnim("Idle");
}

void EnemyStateIdle::Update()
{
	//時間を計測する
	m_time++;

	//Idle状態は何もしない
	m_pEnemy->SetVelo(MyEngine::Vector3(0,0,0));

	m_pEnemy->PlayAnim();


	//このフレームにいる最低時間を超えたら確率で別のフレームに行く
	int random = GetRand(m_time) - kShortestTime;

	if (random > 0)
	{
		m_isChangeState = true;
	}
}

int EnemyStateIdle::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそのまま渡す
	damage = attack->GetDamage() - GetRand(static_cast<int>(m_pEnemy->GetStatus().def));
	//受けた攻撃の種類を設定する
	m_hitEffect = attack->GetHitEffect();
	m_isChangeState = true;
	MyEngine::Vector3 pos = m_pEnemy->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Hit"), pos, false);
	EffekseerManager::GetInstance().Entry(effect);
	return damage;
}
