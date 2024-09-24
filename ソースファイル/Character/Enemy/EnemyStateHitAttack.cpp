#include "EnemyStateHitAttack.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"

namespace
{
	//軽い攻撃を受けたときのノックバックの速さ
	constexpr float kLightKnockBackSpeed = 0.3f;
	//吹き飛ばし攻撃を受けた時のノックバックの速さ
	constexpr float kBurstKnockBackSpeed = 1.8f;
	//ガードした時のノックバックの速さ
	constexpr float kGuardKnockBackSpeed = 0.2f;
	//軽い攻撃を受けた時の動けない時間
	constexpr int kLightHitStunTime = 8;
	//吹き飛ばし攻撃を受けた時の動けない時間
	constexpr int kBurstHitStunTime = 90;
	//スタン攻撃を受けた時の動けない時間
	constexpr int kStunHitStunTime = 40;
	//ガードした時の動けない時間
	constexpr int kGuardStunTime = 3;

}

void EnemyStateHitAttack::Init(int effect)
{
	if (effect == static_cast<int>(HitEffect::kBurst))
	{
		m_effect = HitEffect::kBurst;
		m_pEnemy->ChangeAnim("Burst");
	}
	else if (effect == static_cast<int>(HitEffect::kLightHit))
	{
		m_effect = HitEffect::kLightHit;
		m_pEnemy->ChangeAnim("LightHit");
		m_pEnemy->PlaySE("LightHit", DX_PLAYTYPE_BACK);
	}
	else if (effect == static_cast<int>(HitEffect::kStun))
	{
		m_effect = HitEffect::kStun;
		m_pEnemy->ChangeAnim("Stun");
	}
	else if (effect == static_cast<int>(HitEffect::kGuard))
	{
		m_effect = HitEffect::kGuard;
		m_pEnemy->PlaySE("Guard", DX_PLAYTYPE_BACK);
	}

	m_burstDir = (m_pEnemy->GetPos() - m_pEnemy->GetTargetPos()).Normalize();

}

void EnemyStateHitAttack::Update()
{
	//経過時間を計測する
	m_time++;
	//移動ベクトル
	MyEngine::Vector3 velo;

	//軽い攻撃を受けた時
	if (m_effect == HitEffect::kLightHit)
	{
		velo = m_burstDir * kLightKnockBackSpeed;
		//軽い攻撃を受けた時のスタン時間が経過したら
		if (m_time > kLightHitStunTime)
		{
			//状態を変化させる
			m_isChangeState = true;
		}
	}
	//吹き飛ばし攻撃を受けた時
	else if (m_effect == HitEffect::kBurst)
	{
		velo = m_burstDir * kBurstKnockBackSpeed;
		//吹き飛ばし攻撃を受けた時のスタン時間が経過したら
		if (m_time > kBurstHitStunTime)
		{
			//状態を変化させる
			m_isChangeState = true;
		}
	}
	//スタン攻撃を受けた時
	else if (m_effect == HitEffect::kStun)
	{
		velo = MyEngine::Vector3(0, 0, 0);
		//スタン攻撃を受けた時のスタン時間が経過したら
		if (m_time > kStunHitStunTime)
		{
			//状態を変化させる
			m_isChangeState = true;
		}
	}
	//ガードしていた時
	else
	{
		velo = m_burstDir * kGuardKnockBackSpeed;
		//ガードした時のスタン時間が経過したら
		if (m_time > kGuardStunTime)
		{
			//状態を変化させる
			m_isChangeState = true;
		}
	}
	m_pEnemy->PlayAnim();
	m_pEnemy->SetVelo(velo);
}

int EnemyStateHitAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそのまま渡す
	damage = attack->GetDamage() - GetRand(static_cast<int>(m_pEnemy->GetStatus().def));
	//受けた攻撃の種類を設定する
	m_hitEffect = attack->GetHitEffect();
	MyEngine::Vector3 pos = m_pEnemy->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Hit"), pos, false);
	EffekseerManager::GetInstance().Entry(effect);
	return damage;
}
