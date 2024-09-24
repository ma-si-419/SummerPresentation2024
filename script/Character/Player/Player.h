#pragma once
#include "CharacterBase.h"
#include "Input.h"
#include "PlayerStateBase.h"
#include "Ui.h"
class SceneGame;
class Player : public CharacterBase 
{
public:
	Player();
	~Player();

	void Init(std::shared_ptr<Physics> physics);
	void RetryInit();
	void Update(std::shared_ptr<SceneGame> scene, MyEngine::Input input);
	void Draw();

	float GetRota() { return m_rota; }

	MyEngine::Vector3 GetVelo() { return m_rigidbody.GetVelo(); }

	void SetRota(float rota) { m_rota = rota; }

	void SetVelo(MyEngine::Vector3 velo) { m_rigidbody.SetVelo(velo); }

	MyEngine::Vector3 GetPos() { return m_rigidbody.GetPos(); }
	
	MyEngine::Vector3 GetTargetPos() { return m_targetPos; }

	DataManager::AttackInfo GetAttackData(std::string id) { return m_attackData[id]; }
	
	bool GetIsOpenSpecialPallet() { return m_isOpenSpecialPallet; }

	PlayerStateBase::PlayerStateKind GetStateKind() { return m_pState->GetKind(); }

	void AddMp(float addPoint) { if(m_nowMp < m_status.mp)m_nowMp += addPoint; }
	void SubMp(float subPoint) { m_nowMp -= subPoint; if (m_nowMp < 0)m_nowMp = 0; }

	/// <summary>
	/// 攻撃の種類を取得する
	/// </summary>
	/// <param name="attackId">調べたい攻撃のID</param>
	/// <returns>true:Energy,false:Physical</returns>
	bool GetAttackKind(std::string attackId);

	virtual void OnCollide(std::shared_ptr<Collidable> collider) override;

	//セットしてある必殺技のIdを返す
	std::map<std::string, std::string> GetSetSpecialAttackId() { return m_setSpecialAttackId; };
	//セットしてある必殺技の名前を返す
	std::map<std::string, Ui::SpecialAttackCommandInfo> GetSetSpecialAttackInfo();
	//攻撃を作成する
	std::shared_ptr<AttackBase> CreateAttack(std::string id,MyEngine::Vector3 laserTargetPos);
	//視野角を広げるかどうかを取得する
	bool GetIsUpFov() { return m_isUpFov; }
	//視野角を広げる
	void SetUpFov(bool up) { m_isUpFov = up; }

private:

	float m_rota;

	bool m_isUpFov;

	//最後に攻撃を出してからの時間をカウントする
	int m_lastAttackTime;
	//前のフレームの入力を保存しておく
	MyEngine::Vector3 m_lastInput;
	//必殺技パレットを開いているかどうか
	bool m_isOpenSpecialPallet;

	void ChangeState(std::shared_ptr<PlayerStateBase> state);

	void SetSpecialAttack();

	//設定している技のId
	std::map<std::string, std::string> m_setSpecialAttackId;

	//Stateパターン
	std::shared_ptr<PlayerStateBase> m_pState;
};

