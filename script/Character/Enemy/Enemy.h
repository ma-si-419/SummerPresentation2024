#pragma once
#include "CharacterBase.h"
#include "PlayerStateBase.h"
#include <memory>
class EnemyStateBase;
class SceneGame;
class Ui;
class Enemy : public CharacterBase
{
public:
	Enemy();
	~Enemy();

	void Init(std::shared_ptr<Physics> physics);
	void RetryInit();
	void Update(std::shared_ptr<SceneGame> scene);
	void Draw();

	virtual void OnCollide(std::shared_ptr<Collidable> collider) override;

	void InitAiState(std::shared_ptr<SceneGame> scene);

	void SetAiData(std::map<std::string, std::vector<int>> data) { m_aiStateData = data; }

	std::map<std::string, std::vector<int>> GetAiData() { return m_aiStateData; }

	//スタンゲージが今全体の何割あるかを返す
	float GetStanPointRate();

	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }

	MyEngine::Vector3 GetTargetPos() { return m_targetPos; }

	//攻撃の情報を返す
	std::map<std::string, DataManager::AttackInfo> GetAttackData() { return m_attackData; }

	void SetVelo(MyEngine::Vector3 velo) { m_rigidbody.SetVelo(velo); }

	//Stateの更新
	void StateUpdate(std::shared_ptr<Player> player);

	//攻撃を作成する
	std::shared_ptr<AttackBase> CreateAttack(std::string id, MyEngine::Vector3 laserTargetPos);

	//初期位置に戻す
	void InitPos();
private:
	//スタンゲージ
	float m_stanPoint;
	//最後に攻撃を受けてから立った時間
	int m_lastHitDamageTime;
	//間隔を開けずに攻撃を受けた数
	int m_comboCount;
	//Stateパターン
	std::shared_ptr<EnemyStateBase> m_pState;
	//敵のStateによって動きの確率を変化させる
	std::map<std::string, std::vector<int>> m_aiStateData;
};
