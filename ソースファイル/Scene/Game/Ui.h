#pragma once
#include <map>
#include <vector>
#include "Vector2.h"
#include "Vector3.h"
#include <string>
#include "DataManager.h"
#include <memory>


class Player;
class Enemy;
class Ui
{
public:
	enum class GameOverItem
	{
		kRetry,
		kEnd,
		kItemNum
	};

	struct SpecialAttackCommandInfo
	{
		std::string name = "empty";
		int cost = -1;
	};
public:
	Ui();
	virtual ~Ui();

	void Init();
	
	void RetryInit();

	void DrawStateBar(std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy);

	void SetUiPos(std::string name, MyEngine::Vector2 pos);

	void SetComboCount(int count) { m_comboCount = count; }
	
	bool GetIsCountCombo() { return m_isCountCombo; }

	void LoadUiHandle(std::vector<DataManager::UiInfo> data);

	void AddShowDamage(MyEngine::Vector3 pos, int damage,bool isPlayer);

	/// <summary>
	/// ダメージを表示する
	/// </summary>
	void DrawDamage();

	/// <summary>
	/// ゲームオーバー時のUIを表示する
	/// </summary>
	/// <param name="arrowPos">どの項目を選択しているか</param>
	void DrawGameOver(int arrowPos);

	void DrawGameClear();

	/// <summary>
	/// バトル開始前の合図を表示する
	/// </summary>
	/// <param name="startFlag">バトルが開始したかどうか
	void DrawStartSign(bool startFlag);

	/// <summary>
	/// 攻撃関係の操作を表示する
	/// </summary>
	/// <param name="showSpecialAttack">必殺技パレットを開いているかどうか</param>
	void DrawCommand(bool showSpecialAttack,std::map<std::string, SpecialAttackCommandInfo> attackInfo);

	/// <summary>
	/// コンボ数を表示する
	/// </summary>
	void DrawComboCount();
private:
	enum class ButtonSort
	{
		kA,kLb = kA,
		kB,kRb = kB,
		kX,kLt = kX,
		kY,kRt = kY
	};

	enum class CommandSort
	{
		kPhysical,kSpecialA = kPhysical,
		kEnergy, kSpecialB = kEnergy,
		kDodge, kSpecialX = kDodge,
		kCharge,kSpecialY = kCharge
	};

	struct UiStatus
	{
		//画像ハンドル
		int handle = -1;
		//表示座標
		MyEngine::Vector2 drawPos;
	};
	//エネミーに与えたダメージを表示するときに必要な構造体
	struct DamageShowStatus
	{
		//表示するダメージ
		std::string damage = "empty";
		//表示時間
		float time = 0;
		//表示座標
		MyEngine::Vector2 pos;
	};

	enum class ParticlePos
	{
		kLeftUp,
		kRightUp,
		kLeftDown,
		kRightDown,
		kPosKindNum
	};

	struct Particle
	{
		MyEngine::Vector2 pos;
		MyEngine::Vector2 vec;
		int color = 0;
		int lifeTime = 0;
	};

	//前のフレーム表示していたHpバーの状態を残しておく
	int m_lastPlayerHpBarEndPosX;
	int m_lastEnemyHpBarEndPosX;

	//前のフレームのHP
	float m_lastPlayerHp;
	float m_lastEnemyHp;

	//HPがなくなっていくまでの時間カウント
	int m_playerLostHpBarLifeTime;
	int m_enemyLostHpBarLifeTime;

	//fightの拡大率
	float m_fightExRate;
	//fightのアルファ値
	int m_fightAlpha;
	//fightと何フレーム表示したか
	int m_showFightTime;
	//何フレームfightを揺らしたか
	int m_fightShakeTime;
	//fightの縮小を行っているかどうか
	bool m_isFightChangeExRate;

	//Koの拡大率
	float m_koExRate;


	//基本コマンドUIのX座標
	int m_normalCommandPosX;
	//基本コマンドUIのアルファ値
	int m_normalCommandAlpha;
	//必殺技コマンドのX座標
	int m_specialCommandPosX;
	//必殺技コマンドUIのアルファ値
	int m_specialCommandAlpha;

	//コンボ数
	int m_comboCount;
	//前のフレーム表示していたフレーム数
	int m_lastComboCount;
	//コンボを表示する文字のalpha値
	int m_comboUiAlpha;
	//コンボをカウントするタイミングかどうか
	bool m_isCountCombo;

	//体力を表示するときのフォントハンドル
	int m_hpNumFontHandle;
	//魔力を表示するときのフォントハンドル
	int m_mpNumFontHandle;
	//ダメージを表示するときのフォントハンドル
	int m_damageFontHandle;
	//ゲームオーバーの時の選択肢のフォントハンドル
	int m_gameOverFontHandle;
	//コマンドを表示するときのフォントハンドル
	int m_commandFontHandle;
	//コンボを表示するときのフォントハンドル
	int m_comboCountFontHandle;
	//コンボが途切れてからのフレーム数
	int m_comboEndTime;
	//コンボをほめるコメントのX座標
	int m_comboPraiseCommentPosX;

	//表示するダメージの配列
	std::vector<DamageShowStatus> m_showEnemyDamage;
	std::vector<DamageShowStatus> m_showPlayerDamage;
	//選択している矢印を動かす際に使用する変数
	float m_shakeArrowNum;
	//表示しているUiのデータ
	std::map<std::string, UiStatus> m_showUi;
	//クリア時に表示するパーティクルの配列
	std::vector<Particle> m_particles;
};