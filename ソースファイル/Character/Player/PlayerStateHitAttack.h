#pragma once
#include "PlayerStateBase.h"
class PlayerStateHitAttack : public PlayerStateBase
{
public:
	enum class HitEffect
	{
		kLightHit,
		kStun,
		kBurst,
		kGuard
	};

	PlayerStateHitAttack(std::shared_ptr<Player> player, std::shared_ptr<SceneGame> scene) : PlayerStateBase(player, scene) {};

	void Init(std::shared_ptr<Collidable> collider);

	virtual void Update(MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kHitAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

private:
	//経過時間
	int m_time = 0;
	//受けた攻撃の種類
	HitEffect m_hitEffect = HitEffect::kLightHit;
	//ノックバックの方向
	MyEngine::Vector3 m_burstDir;
};
