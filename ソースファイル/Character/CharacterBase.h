#pragma once
#include "DxLib.h"
#include "Collidable.h"
#include "DataManager.h"
#include "Game.h"
#include <map>
#include <string>

class EffekseerData;
class AttackBase;
class Physics;
class SceneGame;
class Ui;
class CharacterBase : public Collidable
{
public:
	struct Status
	{
		float hp = 100000;
		float mp = 250;
		float atk = 200;
		float def = 300;
	};
	/// <summary>
	/// コンストラクタ
	/// </summary>
	CharacterBase(const TCHAR* graph, ObjectTag tag);
	/// <summary>
	/// デストラクタ
	/// </summary>
	virtual ~CharacterBase();

	/// <summary>
	/// 初期化処理
	/// </summary>
	virtual void Init(std::shared_ptr<Physics> physics) abstract;

	virtual void RetryInit() abstract;

	/// <summary>
	/// 描画処理
	/// </summary>
	virtual void Draw() abstract;

	//最大体力を返す
	Status GetStatus() { return m_status; }
	//現在の体力を返す
	float GetNowHp() { return m_nowHp; }
	//現在の気力量を返す
	float GetNowMp() { return m_nowMp; }
	//ロックオンしている敵の座標を設定する
	void SetTargetPos(MyEngine::Vector3 pos);
	//攻撃の情報を設定する
	void SetAttackData(std::map<std::string, DataManager::AttackInfo> data) { m_attackData = data; }
	//動けない時間を設定する
	void SetStanTime(int time) { m_stanTime = time; }
	//攻撃に必要な気力量を取得する
	int GetAttackCost(std::string Id) { return m_attackData[Id].cost; }
	//アニメーションのデータを取得する
	void SetAnimationData(std::vector<std::vector<std::string>> data,bool isPlayer);
	//効果音を鳴らす　
	void PlaySE(std::string name, int playType);
	//効果音を止める
	void StopSE(std::string name);
	//自分についてくるエフェクトを設定する
	void SetEffectData(std::shared_ptr<EffekseerData> effect) { m_pEffectData = effect; }
	//再生しているポインタを返す
	std::shared_ptr<EffekseerData> GetEffectData() { return m_pEffectData; }
	//自分についてくるエフェクトの再生を止める
	void EndEffect();
	//必殺技を出している状態に変化させる
	void PlaySpecialAttack(std::string id);
	//向く方向を設定する
	void SetModelFront(MyEngine::Vector3 pos);
	//アニメーションを変更する
	void ChangeAnim(std::string animName);
	//アニメーションを再生する
	bool PlayAnim();
	//アニメーションのループをやめる
	void StopAnimLoop();
	//攻撃が終わるときのアニメーション
	void SetAttackEndAnim(float attackEndTime);
	//Ui管理クラスのポインタを保存する
	void SetUiManager(std::shared_ptr<Ui> ui) { m_pUi = ui; }
	//シーンのポインタを保存する
	void SetGameScene(std::shared_ptr<SceneGame> scene) { m_pScene = scene; }
protected:
	enum class AnimationInfoSort
	{
		kName,
		kNumber,
		kLoopFrame,
		kEndFrame,
		kPlaySpeed,
		kIsPlayer
	};
	struct AnimationInfo
	{
		int number = 0;
		int loopFrame = 0;
		int endFrame = 0;
		float playSpeed = 0.0f;
	};
	//モデルハンドル
	int m_modelHandle;
	//登録したPhysicsを持っておく
	std::shared_ptr<Physics> m_pPhysics;
	//シーンのポインタを持っておく
	std::shared_ptr<SceneGame> m_pScene;
	//Ui管理クラスのポインタを持っておく
	std::shared_ptr<Ui> m_pUi;
	//攻撃のデータ
	std::map<std::string, DataManager::AttackInfo> m_attackData;
	//アニメーションのデータ
	std::map<std::string, AnimationInfo> m_animData;
	//自分についているエフェクトのポインタ
	std::shared_ptr<EffekseerData> m_pEffectData;
	//基本的なステータス
	Status m_status;
	//現在の体力
	float m_nowHp;
	//現在の気力
	float m_nowMp;
	//動けない時間
	int m_stanTime;
	//攻撃を出しているかどうか
	bool m_isAttack;
	//ターゲットが近くにいるかどうか
	bool m_isNearTarget;
	//今再生しているアニメーション
	int m_playAnim;
	//アニメーションの再生速度
	float m_animPlaySpeed;
	//アニメーションの総再生時間
	float m_totalAnimTime;
	//アニメーションの再生時間
	float m_animTime;
	//アニメーションがループした時に戻るフレーム
	float m_animLoopStartTime;
	//アニメーションのループするときのフレーム
	float m_animLoopEndTime;
	//アニメーションをループさせるかどうか
	bool m_isLoopAnim;
	//出している攻撃
	std::string m_attackId;
	//攻撃を出したタイミングの敵の座標
	MyEngine::Vector3 m_attackTarget;
	//ロックオンしている相手の座標
	MyEngine::Vector3 m_targetPos;
	/// <summary>
	/// 攻撃を作成する
	/// </summary>
	/// <param name="id">攻撃のID</param>
	/// <returns>攻撃のポインタ</returns>
	std::shared_ptr<AttackBase> CreateAttack(std::shared_ptr<Physics> physics, std::string id, bool isPlayer);




	//通常攻撃を出してる状態に変化させる
	void SetNormalAttack(bool isPhysical, int time);
};