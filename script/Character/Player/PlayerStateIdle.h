#pragma once
#include "PlayerStateBase.h"
class PlayerStateIdle : public PlayerStateBase
{
public:
	PlayerStateIdle(std::shared_ptr<Player> player, std::shared_ptr<SceneGame> scene) : PlayerStateBase(player, scene) {};

	void Init();

	virtual void Update(MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kIdle; }
	
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
};