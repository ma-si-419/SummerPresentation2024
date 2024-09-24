#pragma once
#include "EnemyStateBase.h"
class EnemyStateCharge : public EnemyStateBase
{
public:
	EnemyStateCharge(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) : EnemyStateBase(enemy, scene) {}
	//初期化処理
	void Init();
	//プレイヤーのStateを見て次の動きを決定する
	virtual void Update() override;
	//ダメージを受けた時の処理を行う
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
};

