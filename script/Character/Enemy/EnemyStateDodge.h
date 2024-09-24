#pragma once
#include "EnemyStateBase.h"
class EnemyStateDodge : public EnemyStateBase
{
public:
	EnemyStateDodge(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) : EnemyStateBase(enemy, scene) {}
	//どの行動をするかを決める
	void Init();
	//更新処理
	virtual void Update() override;
	//ダメージを受けた時の処理を行う
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
private:
	//移動ベクトル
	MyEngine::Vector3 m_velo;
};