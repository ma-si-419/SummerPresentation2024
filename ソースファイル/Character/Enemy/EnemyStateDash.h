#pragma once
#include "EnemyStateBase.h"
class EnemyStateDash : public EnemyStateBase
{
public:
	EnemyStateDash(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) : EnemyStateBase(enemy, scene) {}
	//どの行動をするかを決める
	void Init(MyEngine::Vector3 playerPos);
	//更新処理
	virtual void Update() override;
	//ダメージを受けた時の処理を行う
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
private:
	enum class MoveKind
	{
		kFront,//プレイヤーに向かっていく
		kBack,//プレイヤーから離れる
		kRandom,//ランダムに移動
		kMoveKindNum
	};
	MoveKind m_moveKind = MoveKind::kFront;

	//上下移動するか
	bool m_isMoveVertical = false;
	//移動ベクトル
	MyEngine::Vector3 m_velo;
	//プレイヤーの座標
	MyEngine::Vector3 m_targetPos;
	//初期座標
	MyEngine::Vector3 m_initPos;
};

