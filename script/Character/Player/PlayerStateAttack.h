#pragma once
#include "PlayerStateBase.h"

class PlayerStateAttack : public PlayerStateBase
{
public:
	PlayerStateAttack(std::shared_ptr<Player> player , std::shared_ptr<SceneGame> scene) : PlayerStateBase(player,scene) {}

	/// <summary>
	/// 初期化処理
	/// </summary>
	/// <param name="button">押したボタン</param>
	/// <param name="isSpecial">必殺技かどうか</param>
	void Init(std::string button,bool isSpecial);

	virtual void Update(MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

private:

	/// <summary>
	/// 次の通常攻撃のIDを返す
	/// </summary>
	/// <param name="id">今出している通常攻撃</param>
	/// <returns>次に出す通常攻撃</returns>
	std::string GetNextNormalAttack(std::string id);

	//経過時間
	int m_time = 0;
	//何秒間で元の状態に戻るか
	int m_actionTime = 0;
	//何の技を出しているか
	std::string m_attackId = "empty";
	//次に出す予定のID
	std::string m_nextAttackId = "empty";
	//敵が近くにいるかどうか
	bool m_isGoTarget = false;
	//攻撃のモーションに入っているかどうかどうか
	bool m_isStartAttack = false;
	//気弾の必殺技を使った後のアニメーションに入っているかどうか
	bool m_isEndSpecialAttackAnim = false;
	//抱いている攻撃が通常攻撃かどうか
	bool m_isNormalAttack = false;
	//攻撃が終わったかどうか
	bool m_isAttackEnd = false;
	//出した攻撃の数
	int m_popAttackNum = 0;
	//攻撃を出したタイミングの敵の位置を保存する
	MyEngine::Vector3 m_laserTargetPos;
};

