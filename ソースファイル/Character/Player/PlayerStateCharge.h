#pragma once
#include "PlayerStateBase.h"
class PlayerStateCharge : public PlayerStateBase
{
public:
	PlayerStateCharge(std::shared_ptr<Player> player, std::shared_ptr<SceneGame> scene) : PlayerStateBase(player, scene) {};
	
	void Init();

	void Update(MyEngine::Input input) override;

	PlayerStateKind GetKind() override { return PlayerStateKind::kCharge; }

	int OnDamage(std::shared_ptr<Collidable> collider) override;
};

