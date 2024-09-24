#pragma once
#include "EnemyStateBase.h"

class Player;
class EnemyStateAttack : public EnemyStateBase
{
public:
	EnemyStateAttack(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) : EnemyStateBase(enemy,scene) {}
	//出す技を決定する
	void Init(std::shared_ptr<Player> player);
	//更新処理
	virtual void Update() override;
	//ダメージを受けた時の処理を行う
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
private:
	//このStateに入ってからの経過時間
	int m_time = 0;
	//攻撃を出し始めてからの経過時間
	int m_attackTime = 0;

	std::string m_attackId = "empty";

	enum class AttackKind
	{
		kRepeatedlyEnergy,
		kLaser,
		kBreakAttack,
		kStanAttack,
		kAttackKindNum
	};

	//プレイヤーに近づくかどうか
	bool m_isNearPlayer = false;
	//攻撃を出しているかフラグ
	bool m_isStartAttack = false;
	//攻撃が終わったかどうかフラグ
	bool m_isAttackEnd = false;
	//プレイヤーの座標を取得するためにポインターを持つ
	std::shared_ptr<Player> m_pPlayer;
	//攻撃を出した回数
	int m_popAttackNum = 0;
	//レーザーを出す攻撃の座標
	MyEngine::Vector3 m_laserTargetPos;
	//攻撃後の硬直時間であるかどうか
	bool m_isAttackEndStanAnim = false;
};

