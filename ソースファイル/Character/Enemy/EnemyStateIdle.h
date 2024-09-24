#pragma once
#include "EnemyStateBase.h"
class EnemyStateIdle : public EnemyStateBase
{
public:
	EnemyStateIdle(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) : EnemyStateBase(enemy, scene) {}
	//初期化処理
	void Init();
	//更新処理
	virtual void Update() override;
	//ダメージを受けた時の処理を行う
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
};