#pragma once
#include "PlayerStateBase.h"
class PlayerStateGuard : public PlayerStateBase
{
public:

	PlayerStateGuard(std::shared_ptr<Player> player, std::shared_ptr<SceneGame> scene) : PlayerStateBase(player, scene) {};

	void Init();

	virtual void Update(MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kGuard; }
	
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
	
};

