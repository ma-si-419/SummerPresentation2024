#pragma once
#include "EnemyStateBase.h"
class EnemyStateHitAttack : public EnemyStateBase
{
public:
	enum class HitEffect
	{
		kLightHit,
		kStun,
		kBurst,
		kGuard
	};
	EnemyStateHitAttack(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) : EnemyStateBase(enemy, scene) {}
	
	//初期化処理
	void Init(int effect);
	
	//更新処理
	virtual void Update() override;
	//ダメージを受けた時の処理を行う
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
private:
	//経過時間
	int m_time = 0;
	//受けた攻撃の種類
	HitEffect m_effect = HitEffect::kLightHit;
	
	//ノックバックの方向
	MyEngine::Vector3 m_burstDir;



};

