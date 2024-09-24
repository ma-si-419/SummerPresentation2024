#include "PlayerStateGuard.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHitAttack.h"
#include "Player.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"
namespace
{
	//ダメージの軽減率
	constexpr float kDamageCutRate = 0.2f;
}
void PlayerStateGuard::Init()
{
	MyEngine::Vector3 pos = m_pPlayer->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Guard"), pos, true);
	EffekseerManager::GetInstance().Entry(effect);
	m_pPlayer->SetEffectData(effect);
	m_pPlayer->ChangeAnim("Guard");
}
void PlayerStateGuard::Update(MyEngine::Input input)
{
	//敵の方向を向くようにする
	m_pPlayer->SetModelFront(m_pPlayer->GetTargetPos());
	//移動しないようにする
	m_pPlayer->SetVelo(MyEngine::Vector3(0,0,0));
	//アニメーションを動かす
	m_pPlayer->PlayAnim();
	//エフェクトの位置を合わせる
	auto effect = m_pPlayer->GetEffectData();
	effect->SetPos(m_pPlayer->GetPos());

	//状態の変化
	if (input.IsPress(Game::InputId::kRb))
	{
		m_nextState = shared_from_this();
		return;
	}
	//ガード入力がされていなければ
	else
	{
		//状態を変化させる
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		m_pPlayer->EndEffect();
		return;
	}
}

int PlayerStateGuard::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそカットして返す
	damage = attack->GetDamage() * kDamageCutRate;
	m_pPlayer->PlaySE("Guard", DX_PLAYTYPE_BACK);

	return damage;
}
