#pragma once
#include "Enemy.h"
#include "SceneGame.h"
#include "Player.h"

class EnemyStateBase
{
public:
	enum class HitEffectKind
	{
		kLightAttack,
		kStan,
		kBurst,
		kGuard
	};

	enum class EnemyStateKind
	{
		kIdle,
		kMove,
		kDash,
		kDodge,
		kAttack,
		kGuard,
		kCharge,
		kHitAttack
	};
	EnemyStateBase(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) { m_pEnemy = enemy; m_pScene = scene; }
	//状況を把握し続ける更新
	void CheckSituation(std::shared_ptr<Player> player);
	// Stateそれぞれの処理を行い、つぎのStateに変わるタイミングでtrueを返す	
	virtual void Update() abstract;
	//ダメージを受けた時の処理を行う
	virtual int OnDamage(std::shared_ptr<Collidable> collider) abstract;
	//次移行するStateを保存する
	std::shared_ptr<EnemyStateBase> m_nextState;
	//エネミーのポインターを持っておく
	std::shared_ptr<Enemy> m_pEnemy;
	//ゲームシーンのポインターを持っておく
	std::shared_ptr<SceneGame> m_pScene;
	//次どのStateに行くかの確率のデータ
	std::map<std::string, std::vector<int>> m_aiData;
	//Stateの切り替えを行うflag
	bool m_isChangeState = false;
	//どんなダメージを受けたか
	int m_hitEffect = -1;
	//Stateが切り替わってから何フレーム立ったか計測する
	int m_time = 0;
	//プレイヤーの方を向くかどうか
	bool m_isLookPlayer = true;
};

