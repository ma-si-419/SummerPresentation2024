#include "PlayerStateHitAttack.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"

namespace
{
	//軽い攻撃を受けたときのノックバックの速さ
	constexpr float kLightKnockBackSpeed = 0.3f;
	//吹き飛ばし攻撃を受けた時のノックバックの速さ
	constexpr float kBurstKnockBackSpeed = 1.2f;
	//軽い攻撃を受けた時の動けない時間
	constexpr int kLightHitStunTime = 5;
	//吹き飛ばし攻撃を受けた時の動けない時間
	constexpr int kBurstHitStunTime = 30;
	//スタン攻撃を受けた時の動けない時間
	constexpr int kStunHitStunTime = 40;
}
void PlayerStateHitAttack::Init(std::shared_ptr<Collidable> collider)
{
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	m_burstDir = (m_pPlayer->GetPos() - attack->GetPos()).Normalize();
	
	if (attack->GetHitEffect() == static_cast<int>(HitEffect::kLightHit))
	{
		m_hitEffect = HitEffect::kLightHit;
		m_pPlayer->ChangeAnim("LightHit");
		m_pPlayer->PlaySE("LightHit", DX_PLAYTYPE_BACK);
	}
	else if (attack->GetHitEffect() == static_cast<int>(HitEffect::kBurst))
	{
		m_hitEffect = HitEffect::kBurst;
		m_pPlayer->ChangeAnim("Burst");
	}
	else if (attack->GetHitEffect() == static_cast<int>(HitEffect::kStun))
	{
		m_hitEffect = HitEffect::kStun;
		m_pPlayer->ChangeAnim("Stun");
	}
}
void PlayerStateHitAttack::Update(MyEngine::Input input)
{
	//経過時間を計測する
	m_time++;
	//移動ベクトル
	MyEngine::Vector3 velo;

	//軽い攻撃を受けた時
	if (m_hitEffect == HitEffect::kLightHit)
	{
		velo = m_burstDir * kLightKnockBackSpeed;
		//軽い攻撃を受けた時のスタン時間が経過したら
		if (m_time > kLightHitStunTime)
		{
			//アイドル状態に戻す
			m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer,m_pScene);
			return;
		}
	}
	//吹き飛ばし攻撃を受けた時
	else if (m_hitEffect == HitEffect::kBurst)
	{
		velo = m_burstDir * kBurstKnockBackSpeed;
		//吹き飛ばし攻撃を受けた時のスタン時間が経過したら
		if (m_time > kBurstHitStunTime)
		{
			//アイドル状態に戻す
			m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
			return;
		}
	}
	//スタン攻撃を受けた時
	else
	{
		velo = MyEngine::Vector3(0, 0, 0);
		//スタン攻撃を受けた時のスタン時間が経過したら
		if (m_time > kStunHitStunTime)
		{
			//アイドル状態に戻す
			m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
			return;
		}
	}
	m_pPlayer->PlayAnim();
	m_pPlayer->SetVelo(velo);
	//現在の状態を返す
	m_nextState = shared_from_this();
}

int PlayerStateHitAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそのまま渡す
	damage = attack->GetDamage();
	//状態を変化させる
	m_nextState = std::make_shared<PlayerStateHitAttack>(m_pPlayer,m_pScene);
	//受けた攻撃の種類を設定する
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->Init(collider);
	//ヒットエフェクトを表示する
	MyEngine::Vector3 pos = m_pPlayer->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Hit"), pos, false);
	EffekseerManager::GetInstance().Entry(effect);
	

	return damage;
}