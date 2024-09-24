#include "Ui.h"
#include "DxLib.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"

namespace
{
	//HPの残量を表示するときのフォントの大きさ
	constexpr int kHpNumFontSize = 20;
	//MPの残量を表示するときのフォントの大きさ
	constexpr int kMpNumFontSize = 15;
	//HPバーの大きさ
	constexpr int kHpBarHeight = 15;
	constexpr int kHpBarWidth = 188;
	//MPバーの大きさ
	constexpr int kMpBarHeight = 8;
	constexpr int kMpBarWidth = 188;
	//スタンバーの大きさ
	constexpr int kStanBarHeight = 8;
	constexpr int kStanBarWidth = 128;
	//体力が減った時の赤いゲージが減少していくまでの時間
	constexpr int kLostHpBarLifeTime = 30;
	//赤いゲージが減少する速度
	constexpr int kLostHpBarDecreaseSpeed = 3;
	//ダメージを受けた時に揺れる大きさ
	constexpr int kShakeScale = 15;
	//ダメージを受けた時に揺れる大きさの半分
	constexpr int kShakeHalfScale = static_cast<int>(kShakeScale * 0.5);
	//HPバーの座標(画像の座標に対してのHPバーの座標のずれを直す)
	constexpr int kHpBarPosX = -52;
	constexpr int kHpBarPosY = 8;
	//プレイヤーのMPバーの座標(画像の座標に対してのMPバーの座標のずれを直す)
	constexpr int kMpBarPosX = -36;
	constexpr int kMpBarPosY = 40;
	//プレイヤーのHPの量を表示する座標
	constexpr int kHpNumPosX = 42;
	constexpr int kHpNumPosY = -3;
	//プレイヤーのMPの量を表示する座標
	constexpr int kMpNumPosX = 100;
	constexpr int kMpNumPosY = 32;
	//エネミーのスタンバーの座標(画像の座標に対してのスタンバーの座標のずれを直す)
	constexpr int kStanBarPosX = 23;
	constexpr int kStanBarPosY = 40;
	//ダメージを表示するフォントの大きさ
	constexpr int kDamageFontSize = 48;
	//ダメージを表示する時間
	constexpr float kDamageShowTime = 30;
	//ダメージ表示が消え始める時間
	constexpr float kDamageVanishTime = 5;
	//ダメージを表示する座標を少しずらす
	constexpr int kDamageShowPosShakeScale = 80;
	constexpr int kDamageShowPosShakeScaleHalf = static_cast<int>(kDamageShowPosShakeScale * 0.5);
	constexpr int kDamageShowPosShiftY = -50;
	//矢印を揺らす大きさ
	constexpr float kShakeArrowScale = 10.0f;
	//矢印を揺らすスピード
	constexpr float kShakeArrowSpeed = 0.5f;
	//ゲームオーバーの文字を表示する座標
	constexpr int kGameOverStringPosX[2] = { 637,660 };
	constexpr int kGameOverStringPosY[2] = { 490,650 };
	//ゲームオーバーの文字の大きさ
	constexpr int kGameOverFontSize = 96;
	//矢印と文字の距離
	constexpr int kArrowDistance = 75;
	//バトル開始時のReadyの拡大率
	constexpr float kReadyExRate = 1.0f;
	//バトル開始時のfightの消える速度
	constexpr int kFightFadeOutSpeed = 15;
	//バトル開始時のfightが消えるまでの時間
	constexpr int kFightShowTime = 60;
	//バトル開始時のfightの初期拡大率
	constexpr float kFightInitExRate = 3.0f;
	//バトル開始時のfightの最終拡大率
	constexpr float kFightFinalExRate = 1.0f;
	//バトル開始時のfightを揺らす時間
	constexpr int kFightShakeTime = 20;
	//バトル開始時のfightを揺らす大きさ
	constexpr int kFightShakeScale = 20;
	//バトル開始時のfightを揺らす大きさの半分
	constexpr int kFightShakeScaleHalf = static_cast<int>(kFightShakeScale * 0.5);
	//バトル開始時のfightの拡大率のサイズ変更スピード
	constexpr float kFightExRateChangeSpeed = 0.3f;
	//バトル開始時のfightの初期アルファ値
	constexpr int kFightInitAlpha = 255;
	//ボタンの画像の大きさ
	constexpr int kButtonGraphSize = 56;
	//ショルダーボタンの画像の大きさ
	constexpr int kShoulderButtonGraphSize = 59;
	//ショルダーボタンのUIとフレームのUIのずれ
	constexpr int kShoulderButtonGraphGapX = 60;
	constexpr int kShoulderButtonGraphGapY = 3;
	//ボタンのUIとフレームのUIのずれ
	constexpr int kButtonGraphGapX = 15;
	constexpr int kButtonGraphGapY = 6;
	//文字列とフレームのUIのずれ
	constexpr int kCommandStringGapX = 80;
	constexpr int kCommandStringGapY = 10;
	//コストとフレームのUIのずれ
	constexpr int kCostStringGapX = 350;
	constexpr int kCostStringGapY = 10;
	//コマンドを表示するX座標の基本的な位置
	constexpr int kCommandPosX = 80;
	//コマンドを表示するY座標
	constexpr int kCommandPosY[4] = { 580,645,710,775 };
	//コマンドの名前を表示するときのフォントの大きさ
	constexpr int kCommandFontSize = 48;
	//コマンドが切り替わるときの動く速さ
	constexpr int kCommandChangeMoveSpeed = 5;
	//コマンドが切り替わるときのフェードの速さ
	constexpr int kCommandChangeFadeSpeed = 30;
	//コンボ数を表示するときのフォントの大きさ
	constexpr int kComboCountFontSize = 64;
	//コンボ数を表示する座標
	constexpr int kComboCountDrawPosX = 1200;
	constexpr int kComboCountDrawPosY = 400;
	//コンボ数をフェードアウトさせるまでの時間
	constexpr int kEndComboFadeOutStartTime = 120;
	//フェードアウトする速さ
	constexpr int kComboUiFadeOutSpeed = 15;
	//コンボをほめるコメントが入ってくる速さ
	constexpr int kComboPraiseCommentMoveSpeed = 30;
	//コンボをほめるコメントの定位置
	constexpr int kComboPraiseCommentPosX = 1280;
	constexpr int kComboPraiseCommentPosY = 330;
	//コンボをほめるコメントの初期X座標(画面外)
	constexpr int kComboPraiseCommentInitPosX = 2000;
	//ほめるコメントがGoodになるコンボ数
	constexpr int kGoodComboNum = 10;
	//ほめるコメントがGreatになるコンボ数
	constexpr int kGreatComboNum = 30;
	//ほめるコメントがExcellentになるコンボ数
	constexpr int kExcellentComboNum = 50;
	//ゲームクリア時のKOの初期拡大率
	constexpr float kInitKoExRate = 20.0f;
	//ゲームクリア時のKOの拡縮速度
	constexpr float kKoScalingSpeed = 1.0f;
	//ゲームクリア時のKOの最終拡大率
	constexpr float kFinalKoExRate = 2.0f;
	//ゲームクリア時の文字の表示座標
	constexpr int kGameClearStringPosX = 600;
	constexpr int kGameClearStringPosY = 600;
	//ガードコマンドを表示する座標
	constexpr int kGuardCommandPosX = 380;
	constexpr int kGuardCommandPosY = 520;
	//SPコマンドを表示するコマンドを表示する座標
	constexpr int kSpCommandControlPosX = 90;
	constexpr int kSpCommandControlPosY = 520;
	//下降コマンドを表示する座標
	constexpr int kDownCommandPosX = 300;
	constexpr int kDownCommandPosY = 430;
	//上昇コマンドを表示する座標
	constexpr int kUpCommandPosX = 90;
	constexpr int kUpCommandPosY = 430;
	//一フレームに増やすパーティクルの最大数
	constexpr int kAddParticleMaxNum = 20;
	//パーティクルの最大速度
	constexpr int kParticleSpeed = 10;
	//パーティクルの大きさ
	constexpr int kParticleRadius = 2;
	//パーティクルにかかる重力の力
	constexpr float kParticleGravity = 0.05f;
	//パーティクルのライフタイム
	constexpr int kParticleLifeTime = 300;
	//ゲームオーバー時の背景に表示する黒い矩形のアルファ値
	constexpr int kGameOverBackGroundAlpha = 128;
	//HPバーを表示する色
	const int kHpBarColor = GetColor(64, 255, 64);
	//MPバーを表示する色
	const int kMpBarColor = GetColor(160, 192, 255);
	//スタンバーを表示する色
	const int kStanBarColor = GetColor(192, 32, 192);
	//HPが減った分だけ上にかぶせるバーの色
	const int kHpDecreasedBoxColor = GetColor(64, 64, 64);
	//赤
	const int kRedColor = GetColor(255, 0, 0);
	//白
	const int kWhiteColor = GetColor(255,255,255);
	//黒
	const int kBlackColor = GetColor(0,0,0);
	//ダメージを受けた時のUIの色
	const int kDamageFontColor = GetColor(200, 0, 0);
	//コンボ数を表示するときの色
	const int kComboFontColor = GetColor(255, 255, 128);
}

Ui::Ui() :
	m_lastPlayerHp(0),
	m_lastEnemyHp(0),
	m_lastPlayerHpBarEndPosX(0),
	m_lastEnemyHpBarEndPosX(0),
	m_playerLostHpBarLifeTime(0),
	m_enemyLostHpBarLifeTime(0),
	m_showUi(),
	m_shakeArrowNum(0),
	m_fightExRate(kFightInitExRate),
	m_fightAlpha(kFightInitAlpha),
	m_showFightTime(0),
	m_fightShakeTime(0),
	m_isFightChangeExRate(true),
	m_normalCommandPosX(kCommandPosX),
	m_normalCommandAlpha(255),
	m_specialCommandPosX(0),
	m_specialCommandAlpha(0),
	m_comboEndTime(0),
	m_comboCount(0),
	m_comboPraiseCommentPosX(kComboPraiseCommentInitPosX),
	m_comboUiAlpha(255),
	m_isCountCombo(true),
	m_lastComboCount(0),
	m_koExRate(kInitKoExRate)
{
	m_damageFontHandle = CreateFontToHandle("アンニャントロマン", kDamageFontSize, 9, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_gameOverFontHandle = CreateFontToHandle("アンニャントロマン", kGameOverFontSize, 9, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_commandFontHandle = CreateFontToHandle("アンニャントロマン", kCommandFontSize, 9, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_comboCountFontHandle = CreateFontToHandle("アンニャントロマン", kComboCountFontSize, 9, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_hpNumFontHandle = CreateFontToHandle("アンニャントロマン", kHpNumFontSize, 9, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_mpNumFontHandle = CreateFontToHandle("アンニャントロマン", kMpNumFontSize, 9, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
}

Ui::~Ui()
{
	DeleteFontToHandle(m_damageFontHandle);
	DeleteFontToHandle(m_gameOverFontHandle);
	DeleteFontToHandle(m_commandFontHandle);
	DeleteFontToHandle(m_comboCountFontHandle);
	DeleteFontToHandle(m_hpNumFontHandle);
	for (auto& item : m_showUi)
	{
		DeleteGraph(item.second.handle);
	}
}

void Ui::Init()
{
	m_fightExRate = kFightInitExRate;
	m_fightAlpha = kFightInitAlpha;
	m_fightShakeTime = 0;
	m_showFightTime = 0;
	m_isFightChangeExRate = true;
	m_comboPraiseCommentPosX = kComboPraiseCommentInitPosX;
}

void Ui::RetryInit()
{
	m_fightExRate = kFightInitExRate;
	m_fightAlpha = kFightInitAlpha;
	m_fightShakeTime = 0;
	m_showFightTime = 0;
	m_isFightChangeExRate = true;
	//ダメージ表示をすべて消す
	for (int i = 0; i < m_showEnemyDamage.size(); i++)
	{
		m_showEnemyDamage.erase(m_showEnemyDamage.begin() + i);
		i--;
	}
	//コンボをすべて消す
	m_comboCount = 0;
	m_lastComboCount = 0;
	//コンボのカウントを再開する
	m_isCountCombo = true;
	//途切れた時間カウントを初期化する
	m_comboEndTime = 0;
	m_comboUiAlpha = 255;
}

void Ui::DrawStateBar(std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy)
{
	//ダメージを受けた際プレイヤーのステートバーを揺らす大きさ
	MyEngine::Vector2 playerStateBarShakeSize(0, 0);
	//ダメージを受けた際エネミーのステートバーを揺らす大きさ
	MyEngine::Vector2 enemyStateBarShakeSize(0, 0);

	//前のフレームよりプレイヤーのHPが減っていたら
	if (m_lastPlayerHp > player->GetNowHp())
	{
		//減った分を表す赤いバーが無くなるまでの時間を設定する
		m_playerLostHpBarLifeTime = kLostHpBarLifeTime;

		//プレイヤーのステートバーの座標を揺らす
		playerStateBarShakeSize = MyEngine::Vector2(GetRand(kShakeScale) - kShakeHalfScale, GetRand(kShakeScale) - kShakeHalfScale);
	}
	//前のフレームよりエネミーのHPが減っていたら
	if (m_lastEnemyHp > enemy->GetNowHp())
	{
		//減った分を表す赤いバーが無くなるまでの時間を設定する
		m_enemyLostHpBarLifeTime = kLostHpBarLifeTime;
		//エネミーのステートバーの座標を揺らす
		enemyStateBarShakeSize = MyEngine::Vector2(GetRand(kShakeScale) - kShakeHalfScale, GetRand(kShakeScale) - kShakeHalfScale);
	}

	//体力量を覚えておく
	m_lastPlayerHp = player->GetNowHp();
	m_lastEnemyHp = enemy->GetNowHp();

	//画像の名前
	std::string playerStateBar = "PlayerStateBar";
	std::string enemyStateBar = "EnemyStateBar";

	//プレイヤーHPバーの開始位置
	MyEngine::Vector2 playerHpBarStartPos;
	playerHpBarStartPos.x = m_showUi[playerStateBar].drawPos.x + kHpBarWidth + kHpBarPosX;
	playerHpBarStartPos.y = m_showUi[playerStateBar].drawPos.y + kHpBarHeight + kHpBarPosY;

	//プレイヤーHPバーの終了位置
	MyEngine::Vector2 playerHpBarEndPos;

	playerHpBarEndPos.x = m_showUi[playerStateBar].drawPos.x - kHpBarWidth + kHpBarPosX;
	playerHpBarEndPos.y = m_showUi[playerStateBar].drawPos.y - kHpBarHeight + kHpBarPosY;

	//プレイヤーHPバーの長さ
	float playerHpBarLength = playerHpBarEndPos.x - playerHpBarStartPos.x;

	//プレイヤーMPバーの開始位置
	MyEngine::Vector2 playerMpBarStartPos;
	playerMpBarStartPos.x = m_showUi[playerStateBar].drawPos.x + kMpBarWidth + kMpBarPosX;
	playerMpBarStartPos.y = m_showUi[playerStateBar].drawPos.y + kMpBarHeight + kMpBarPosY;


	//プレイヤーMPバーの終了位置
	MyEngine::Vector2 playerMpBarEndPos;
	playerMpBarEndPos.x = m_showUi[playerStateBar].drawPos.x - kMpBarWidth + kMpBarPosX;
	playerMpBarEndPos.y = m_showUi[playerStateBar].drawPos.y - kMpBarHeight + kMpBarPosY;

	//プレイヤーMPバーの長さ
	float playerMpBarLength = playerMpBarEndPos.x - playerMpBarStartPos.x;

	//エネミーHPバーの開始位置
	MyEngine::Vector2 enemyHpBarStartPos;

	enemyHpBarStartPos.x = m_showUi[enemyStateBar].drawPos.x + kHpBarWidth + kHpBarPosX;
	enemyHpBarStartPos.y = m_showUi[enemyStateBar].drawPos.y + kHpBarHeight + kHpBarPosY;

	//エネミーHPバーの終了位置
	MyEngine::Vector2 enemyHpBarEndPos;

	enemyHpBarEndPos.x = m_showUi[enemyStateBar].drawPos.x - kHpBarWidth + kHpBarPosX;
	enemyHpBarEndPos.y = m_showUi[enemyStateBar].drawPos.y - kHpBarHeight + kHpBarPosY;

	float enemyHpBarLength = enemyHpBarEndPos.x - enemyHpBarStartPos.x;

	//エネミースタンバーの開始位置
	MyEngine::Vector2 enemyStanBarStartPos;

	enemyStanBarStartPos.x = m_showUi[enemyStateBar].drawPos.x + kStanBarWidth + kStanBarPosX;
	enemyStanBarStartPos.y = m_showUi[enemyStateBar].drawPos.y + kStanBarHeight + kStanBarPosY;

	//エネミースタンバーの終了位置
	MyEngine::Vector2 enemyStanBarEndPos;

	enemyStanBarEndPos.x = m_showUi[enemyStateBar].drawPos.x - kStanBarWidth + kStanBarPosX;
	enemyStanBarEndPos.y = m_showUi[enemyStateBar].drawPos.y - kStanBarHeight + kStanBarPosY;

	float enemyStanBarLength = enemyStanBarEndPos.x - enemyStanBarStartPos.x;

	//プレイヤーのステートバーの座標を揺らす
	playerHpBarStartPos += playerStateBarShakeSize;
	playerHpBarEndPos += playerStateBarShakeSize;
	playerMpBarStartPos += playerStateBarShakeSize;
	playerMpBarEndPos += playerStateBarShakeSize;

	//エネミーのステートバーの座標を揺らす
	enemyHpBarStartPos += enemyStateBarShakeSize;
	enemyHpBarEndPos += enemyStateBarShakeSize;
	enemyStanBarStartPos += enemyStateBarShakeSize;
	enemyStanBarEndPos += enemyStateBarShakeSize;

	//プレイヤーのHPバー表示
	DrawBox(static_cast<int>(playerHpBarStartPos.x), static_cast<int>(playerHpBarStartPos.y),
		static_cast<int>(playerHpBarEndPos.x), static_cast<int>(playerHpBarEndPos.y),
		kHpBarColor, true);
	//プレイヤーのMPバー表示
	DrawBox(static_cast<int>(playerMpBarStartPos.x), static_cast<int>(playerMpBarStartPos.y),
		static_cast<int>(playerMpBarEndPos.x), static_cast<int>(playerMpBarEndPos.y),
		kMpBarColor, true);
	//エネミーのHPバーの表示
	DrawBox(static_cast<int>(enemyHpBarStartPos.x), static_cast<int>(enemyHpBarStartPos.y),
		static_cast<int>(enemyHpBarEndPos.x), static_cast<int>(enemyHpBarEndPos.y),
		kHpBarColor, true);
	//エネミーのスタンバー表示
	DrawBox(static_cast<int>(enemyStanBarStartPos.x), static_cast<int>(enemyStanBarStartPos.y),
		static_cast<int>(enemyStanBarEndPos.x), static_cast<int>(enemyStanBarEndPos.y),
		kStanBarColor, true);


	//体力に応じて上にかぶせるボックスの長さを変化させる
	int playerLostHpBoxPosX = static_cast<int>(playerHpBarStartPos.x + playerHpBarLength * (player->GetNowHp() / player->GetStatus().hp));
	int enemyLostHpBoxPosX = static_cast<int>(enemyHpBarStartPos.x + enemyHpBarLength * (enemy->GetNowHp() / enemy->GetStatus().hp));

	//プレイヤーの気力に応じて上にかぶせるボックスの長さを変化させる
	int playerLostMpBoxPosX = static_cast<int>(playerMpBarStartPos.x + playerMpBarLength * (player->GetNowMp() / player->GetStatus().mp));

	//エネミーのスタンゲージに応じて上にかぶせるボックスの長さを変化させる
	int enemyLostStanBoxPosX = static_cast<int>(enemyStanBarStartPos.x + enemyStanBarLength * enemy->GetStanPointRate());

	//プレイヤーのHPバーの上にかぶせるボックス表示
	DrawBox(playerLostHpBoxPosX, static_cast<int>(playerHpBarStartPos.y),
		static_cast<int>(playerHpBarEndPos.x), static_cast<int>(playerHpBarEndPos.y),
		kHpDecreasedBoxColor, true);
	//エネミーのHPバーの上にかぶせるボックス表示
	DrawBox(enemyLostHpBoxPosX, static_cast<int>(enemyHpBarStartPos.y),
		static_cast<int>(enemyHpBarEndPos.x), static_cast<int>(enemyHpBarEndPos.y),
		kHpDecreasedBoxColor, true);

	//プレイヤーのMPバーの上にかぶせるボックス表示
	DrawBox(playerLostMpBoxPosX, static_cast<int>(playerMpBarStartPos.y),
		static_cast<int>(playerMpBarEndPos.x), static_cast<int>(playerMpBarEndPos.y),
		kHpDecreasedBoxColor, true);

	//エネミーのスタンバーの上にかぶせるボックスを表示
	DrawBox(enemyLostStanBoxPosX, static_cast<int>(enemyStanBarStartPos.y),
		static_cast<int>(enemyStanBarEndPos.x), static_cast<int>(enemyStanBarEndPos.y),
		kHpDecreasedBoxColor, true);

	//赤いバーの初期化
	if (m_lastPlayerHpBarEndPosX == 0 && m_lastEnemyHpBarEndPosX == 0)
	{
		m_lastPlayerHpBarEndPosX = playerLostHpBoxPosX;
		m_lastEnemyHpBarEndPosX = enemyLostHpBoxPosX;
	}

	//赤いバーが消えるまでの時間をカウントする
	m_playerLostHpBarLifeTime--;
	m_enemyLostHpBarLifeTime--;

	//プレイヤーの体力が減った分を表す赤いバーを表示
	DrawBox(playerLostHpBoxPosX, static_cast<int>(playerHpBarStartPos.y),
		m_lastPlayerHpBarEndPosX, static_cast<int>(playerHpBarEndPos.y),
		kRedColor, true);
	//エネミーの体力が減った分を表す赤いバーを表示
	DrawBox(enemyLostHpBoxPosX, static_cast<int>(enemyHpBarStartPos.y),
		m_lastEnemyHpBarEndPosX, static_cast<int>(enemyHpBarEndPos.y),
		kRedColor, true);

	//赤いゲージが表示されて、一定時間ダメージを受けなかったら
	//プレイヤー側の処理
	if (m_playerLostHpBarLifeTime < 0)
	{
		//赤いゲージを短くしていく
		m_lastPlayerHpBarEndPosX += kLostHpBarDecreaseSpeed;
		//現在の体力よりも短くなったら
		if (m_lastPlayerHpBarEndPosX > playerLostHpBoxPosX)
		{
			//現在の体力の座標に合わせる
			m_lastPlayerHpBarEndPosX = playerLostHpBoxPosX;
		}
	}
	//エネミー側の処理
	if (m_enemyLostHpBarLifeTime < 0)
	{
		//赤いゲージを短くしていく
		m_lastEnemyHpBarEndPosX += kLostHpBarDecreaseSpeed;
		//現在の体力よりも短くなったら
		if (m_lastEnemyHpBarEndPosX > enemyLostHpBoxPosX)
		{
			//現在の体力の座標に合わせる
			m_lastEnemyHpBarEndPosX = enemyLostHpBoxPosX;
		}
	}
	//プレイヤーのステートバー表示
	DrawRotaGraph(static_cast<int>(m_showUi[playerStateBar].drawPos.x + playerStateBarShakeSize.x),
		static_cast<int>(m_showUi[playerStateBar].drawPos.y + playerStateBarShakeSize.y),
		1.0, 0.0, m_showUi[playerStateBar].handle, true);
	//エネミーのステートバー表示
	DrawRotaGraph(static_cast<int>(m_showUi[enemyStateBar].drawPos.x + enemyStateBarShakeSize.x),
		static_cast<int>(m_showUi[enemyStateBar].drawPos.y + +enemyStateBarShakeSize.y),
		1.0, 0.0, m_showUi[enemyStateBar].handle, true);
	//体力値表示
	std::string playerHp = std::to_string(static_cast<int>(player->GetNowHp()));
	std::string enemyHp = std::to_string(static_cast<int>(enemy->GetNowHp()));

	//プレイヤーの体力値表示
	DrawStringToHandle(static_cast<int>(m_showUi[playerStateBar].drawPos.x + playerStateBarShakeSize.x + kHpNumPosX),
		static_cast<int>(m_showUi[playerStateBar].drawPos.y + playerStateBarShakeSize.y + kHpNumPosY),
		playerHp.c_str(), kWhiteColor, m_hpNumFontHandle);
	//エネミーの体力値表示
	DrawStringToHandle(static_cast<int>(m_showUi[enemyStateBar].drawPos.x + enemyStateBarShakeSize.x + kHpNumPosX),
		static_cast<int>(m_showUi[enemyStateBar].drawPos.y + enemyStateBarShakeSize.y + kHpNumPosY),
		enemyHp.c_str(), kWhiteColor, m_hpNumFontHandle);
	//プレイヤーの気力値表示
	std::string playerMp = std::to_string(static_cast<int>(player->GetNowMp()));

	DrawStringToHandle(static_cast<int>(m_showUi[playerStateBar].drawPos.x + playerStateBarShakeSize.x + kMpNumPosX),
		static_cast<int>(m_showUi[playerStateBar].drawPos.y + playerStateBarShakeSize.y + kMpNumPosY),
		playerMp.c_str(), kWhiteColor, m_mpNumFontHandle);
}


void Ui::SetUiPos(std::string name, MyEngine::Vector2 pos)
{
	m_showUi[name].drawPos = pos;
}

void Ui::LoadUiHandle(std::vector<DataManager::UiInfo> data)
{
	for (auto& item : data)
	{
		//ゲームシーンのUIをロードする
		UiStatus pushData;
		//データを入れる
		pushData.drawPos = MyEngine::Vector2(item.posX, item.posY);
		//画像のロード
		pushData.handle = LoadGraph(("data/image/" + item.path + ".png").c_str());
		//画像の名前でマップに登録
		m_showUi[item.path] = pushData;
	}
}

void Ui::AddShowDamage(MyEngine::Vector3 pos, int damage, bool isPlayer)
{
	DamageShowStatus pushData;
	VECTOR screenPos = ConvWorldPosToScreenPos(pos.CastVECTOR());
	pushData.pos = screenPos;
	//表示座標を少しずらす
	pushData.pos.x += GetRand(kDamageShowPosShakeScale) - kDamageShowPosShakeScaleHalf;
	pushData.pos.y += GetRand(kDamageShowPosShakeScale) - kDamageShowPosShakeScaleHalf;
	pushData.damage = std::to_string(damage);
	pushData.time = kDamageShowTime;
	if (isPlayer)
	{
		m_showPlayerDamage.push_back(pushData);
	}
	else
	{
		m_showEnemyDamage.push_back(pushData);
	}
}

void Ui::DrawDamage()
{
	//表示時間が終わっているものを削除する
	//エネミーダメージ配列
	for (int i = 0; i < m_showEnemyDamage.size(); i++)
	{
		if (m_showEnemyDamage[i].time < 0)
		{
			m_showEnemyDamage.erase(m_showEnemyDamage.begin() + i);
			i--;
		}
	}

	//プレイヤーダメージ配列
	for (int i = 0; i < m_showPlayerDamage.size(); i++)
	{
		if (m_showPlayerDamage[i].time < 0)
		{
			m_showPlayerDamage.erase(m_showPlayerDamage.begin() + i);
			i--;
		}
	}

	//描画処理
	//エネミー
	for (auto& item : m_showEnemyDamage)
	{
		int alpha = 0;
		//表示する残り時間が一定以上の時はアルファ値を減らさない
		if (item.time > kDamageVanishTime)
		{
			alpha = 255;
		}
		//残り時間が一定以下になったら
		else
		{
			//残り時間からアルファ値を計算する
			alpha = static_cast <int>(255 / kDamageVanishTime * item.time);
		}
		//文字列の長さを取得
		int length = GetStringLength(item.damage.c_str());
		//文字列を中央ぞろえにする
		int shiftSize = static_cast<int>(length * kDamageFontSize * 0.5);
		//ブレンドモードを変更
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		//ダメージの表示
		DrawFormatStringToHandle(static_cast<int>(item.pos.x - shiftSize), static_cast<int>(item.pos.y + kDamageShowPosShiftY),
			kWhiteColor, m_damageFontHandle, item.damage.c_str());
		//ブレンドモードを元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//表示時間を減らす
		item.time--;
	}

	//プレイヤー
	for (auto& item : m_showPlayerDamage)
	{
		int alpha = 0;
		//表示する残り時間が一定以上の時はアルファ値を減らさない
		if (item.time > kDamageVanishTime)
		{
			alpha = 255;
		}
		//残り時間が一定以下になったら
		else
		{
			//残り時間からアルファ値を計算する
			alpha = static_cast <int>(255 / kDamageVanishTime * item.time);
		}
		//文字列の長さを取得
		int length = GetStringLength(item.damage.c_str());
		//文字列を中央ぞろえにする
		int shiftSize = static_cast<int>(length * kDamageFontSize * 0.5);
		//ブレンドモードを変更
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		//ダメージの表示
		DrawFormatStringToHandle(static_cast<int>(item.pos.x - shiftSize), static_cast<int>(item.pos.y + kDamageShowPosShiftY),
			kDamageFontColor, m_damageFontHandle, item.damage.c_str());
		//ブレンドモードを元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//表示時間を減らす
		item.time--;
	}
}

void Ui::DrawGameOver(int arrowPos)
{
	//後ろのバトルシーンを暗くする
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kGameOverBackGroundAlpha);
	DrawBox(0, 0, Game::kWindowWidth, Game::kWindowHeight, kBlackColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//ゲームオーバーの画像データ
	std::string logo = "GameOverLogo";
	DrawGraph(static_cast<int>(m_showUi[logo].drawPos.x), static_cast<int>(m_showUi[logo].drawPos.y),
		m_showUi[logo].handle, true);

	DrawStringToHandle(kGameOverStringPosX[static_cast<int>(GameOverItem::kRetry)], kGameOverStringPosY[static_cast<int>(GameOverItem::kRetry)], "リトライ", kBlackColor, m_gameOverFontHandle, kWhiteColor);
	DrawStringToHandle(kGameOverStringPosX[static_cast<int>(GameOverItem::kEnd)], kGameOverStringPosY[static_cast<int>(GameOverItem::kEnd)], "やめる", kBlackColor, m_gameOverFontHandle, kWhiteColor);

	m_shakeArrowNum += kShakeArrowSpeed;

	//矢印の表示
	if (arrowPos == 0)
	{
		MyEngine::Vector2 pos(static_cast<int>(kGameOverStringPosX[static_cast<int>(GameOverItem::kRetry)] + sinf(m_shakeArrowNum) * kShakeArrowScale - kArrowDistance), static_cast<int>(kGameOverStringPosY[static_cast<int>(GameOverItem::kRetry)]));
		DrawStringToHandle(static_cast<int>(pos.x), static_cast<int>(pos.y), "→", kBlackColor, m_gameOverFontHandle, kWhiteColor);
	}
	else
	{
		MyEngine::Vector2 pos(static_cast<int>(kGameOverStringPosX[static_cast<int>(GameOverItem::kEnd)] + sinf(m_shakeArrowNum) * kShakeArrowScale - kArrowDistance), static_cast<int>(kGameOverStringPosY[static_cast<int>(GameOverItem::kEnd)]));
		DrawStringToHandle(static_cast<int>(pos.x), static_cast<int>(pos.y), "→", kBlackColor, m_gameOverFontHandle, kWhiteColor);
	}
}

void Ui::DrawGameClear()
{
	int addParticleNum = GetRand(kAddParticleMaxNum);

	for (int i = 0; i < addParticleNum; i++)
	{
		Particle addParticle;
		addParticle.color = GetColor(GetRand(255), GetRand(255), GetRand(255));
		int posKind = GetRand(static_cast<int>(ParticlePos::kPosKindNum));
		if (posKind == static_cast<int>(ParticlePos::kLeftDown))
		{
			addParticle.pos.x = 0;
			addParticle.pos.y = Game::kWindowHeight;
			//右上に行くようにベクトル作成
			addParticle.vec = MyEngine::Vector2(1, -1);
		}
		else if (posKind == static_cast<int>(ParticlePos::kLeftUp))
		{
			addParticle.pos.x = 0;
			addParticle.pos.y = 0;
			//右下に行くようにベクトル作成
			addParticle.vec = MyEngine::Vector2(1, 1);
		}
		else if (posKind == static_cast<int>(ParticlePos::kRightUp))
		{
			addParticle.pos.x = Game::kWindowWidth;
			addParticle.pos.y = 0;
			//左下に行くようにベクトル作成
			addParticle.vec = MyEngine::Vector2(-1, 1);
		}
		else if (posKind == static_cast<int>(ParticlePos::kRightDown))
		{
			addParticle.pos.x = Game::kWindowWidth;
			addParticle.pos.y = Game::kWindowHeight;
			//左上に行くようにベクトル作成
			addParticle.vec = MyEngine::Vector2(-1, -1);
		}
		addParticle.vec.x *= GetRand(kParticleSpeed);
		addParticle.vec.y *= GetRand(kParticleSpeed);

		addParticle.lifeTime = kParticleLifeTime;

		m_particles.push_back(addParticle);
	}
	int number = 0;
	std::vector<int> deleteParticle;
	for (auto& item : m_particles)
	{
		//パーティクルを描画する
		DrawCircle(static_cast<int>(item.pos.x), static_cast<int>(item.pos.y), kParticleRadius, item.color, true);
		//座標の更新
		item.pos += item.vec;
		//ベクトルを少しずつ下向きにしていく
		item.vec.y += kParticleGravity;
		//ライフタイムを減らしていく
		item.lifeTime--;
		if (item.lifeTime < 0)
		{
			deleteParticle.push_back(number);
		}
		number++;
	}
	for (auto deleteItem : deleteParticle)
	{
		m_particles.erase(m_particles.begin() + deleteItem);
	}

	//KOの拡大率を調整する
	if (m_koExRate > kFinalKoExRate)
	{
		m_koExRate -= kKoScalingSpeed;
	}
	else
	{
		m_koExRate = kFinalKoExRate;
	}
	//ゲームクリアの画像データ
	std::string logo = "Ko";
	DrawRotaGraph(static_cast<int>(m_showUi[logo].drawPos.x), static_cast<int>(m_showUi[logo].drawPos.y),
		m_koExRate, 0.0, m_showUi[logo].handle, true);

	DrawStringToHandle(kGameClearStringPosX, kGameClearStringPosY, "Press A Button", kWhiteColor, m_gameOverFontHandle);;

}

void Ui::DrawStartSign(bool startFlag)
{
	//Readyの画像データの名前
	std::string ready = "Ready";
	//Fightの画像データの名前
	std::string fight = "Fight";
	//まだバトルが始まっていない場合
	if (!startFlag)
	{
		//Readyと表示する
		DrawRotaGraph(static_cast<int>(m_showUi[ready].drawPos.x), static_cast<int>(m_showUi[ready].drawPos.y), kReadyExRate, 0, m_showUi[ready].handle, true);
	}
	//バトルが開始した場合
	else
	{
		//fightの表示座標
		MyEngine::Vector2 drawPos(m_showUi[fight].drawPos.x, m_showUi[fight].drawPos.y);

		//表示して何フレーム立ったかを計測しておく
		m_showFightTime++;
		//fightの拡大率を調整する
		if (m_isFightChangeExRate)
		{
			//基本的に縮小していく
			m_fightExRate -= kFightExRateChangeSpeed;
			//サイズが一定値よりも小さくなったら
			if (m_fightExRate < kFightFinalExRate)
			{
				//サイズを一定値にする
				m_fightExRate = kFightFinalExRate;
				//縮小を止める
				m_isFightChangeExRate = false;
			}
		}
		//拡大率の調整が終わった場合
		else
		{
			//表示してから一定時間たっていたら
			if (m_showFightTime > kFightShowTime)
			{
				//fightのアルファ値を下げていく
				m_fightAlpha -= kFightFadeOutSpeed;
			}
			//fightを少し揺らす
			if (m_fightShakeTime < kFightShakeTime)
			{
				//表示座標をずらす
				drawPos.x += GetRand(kFightShakeScale) - kFightShakeScaleHalf;
				drawPos.y += GetRand(kFightShakeScale) - kFightShakeScaleHalf;
				//揺らした時間を計測する
				m_fightShakeTime++;
			}

		}

		//fightのアルファ値が0以上だったら描画する
		if (m_fightAlpha > 0)
		{
			//アルファ値を設定する
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fightAlpha);
			//Fightと表示する
			DrawRotaGraph(static_cast<int>(drawPos.x), static_cast<int>(drawPos.y), m_fightExRate, 0, m_showUi[fight].handle, true);
			//ブレンドモードを元に戻す
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

	}
}

void Ui::DrawCommand(bool showSpecialAttack, std::map<std::string, SpecialAttackCommandInfo> attackInfo)
{
	//コマンドを表示するフレーム
	std::string frame = "CommandFrame";
	//ボタンの画像
	std::string button = "ButtonABXY";
	//コントローラーの上側にあるボタン画像
	std::string shoulderButton = "ButtonShoulder";

	//必殺技パレットを開いていないとき
	if (!showSpecialAttack)
	{
		//基本コマンドのアルファ値をあげる
		if (m_normalCommandAlpha < 255)
		{
			//一定値までアルファ値をあげる
			m_normalCommandAlpha += kCommandChangeFadeSpeed;
		}
		else
		{
			//超えたら最大値にする
			m_normalCommandAlpha = 255;
		}
		//必殺技コマンドのアルファ値をさげる
		if (m_specialCommandAlpha > 0)
		{
			//0まで下げる
			m_specialCommandAlpha -= kCommandChangeFadeSpeed;
		}
		else
		{
			//0よりも小さくならないようにする
			m_specialCommandAlpha = 0;
		}

		//基本コマンドの座標を右に動かす
		if (m_normalCommandPosX < kCommandPosX)
		{
			//一定の位置まで移動させる
			m_normalCommandPosX += kCommandChangeMoveSpeed;
		}
		else
		{
			//超えたら一定の位置に戻す
			m_normalCommandPosX = kCommandPosX;
		}
		//必殺技コマンドの座標を左に動かす
		if (m_specialCommandPosX > 0)
		{
			//0まで移動させる
			m_specialCommandPosX -= kCommandChangeMoveSpeed;
		}
		else
		{
			//0よりも小さくしない
			m_specialCommandPosX = 0;
		}

	}
	//必殺技パレットを開いているとき
	else
	{
		//必殺技コマンドのアルファ値をあげる
		if (m_specialCommandAlpha < 255)
		{
			//一定値までアルファ値をあげる
			m_specialCommandAlpha += kCommandChangeFadeSpeed;
		}
		else
		{
			//超えたら最大値にする
			m_specialCommandAlpha = 255;
		}
		//基本コマンドのアルファ値をさげる
		if (m_normalCommandAlpha > 0)
		{
			//0まで下げる
			m_normalCommandAlpha -= kCommandChangeFadeSpeed;
		}
		else
		{
			//0よりも小さくならないようにする
			m_normalCommandAlpha = 0;
		}


		//必殺技コマンドの座標を右に動かす
		if (m_specialCommandPosX < kCommandPosX)
		{
			//一定の位置まで移動させる
			m_specialCommandPosX += kCommandChangeMoveSpeed;
		}
		else
		{
			//超えたら一定の位置に戻す
			m_specialCommandPosX = kCommandPosX;
		}
		//基本コマンドの座標を左に動かす
		if (m_normalCommandPosX > 0)
		{
			//0まで移動させる
			m_normalCommandPosX -= kCommandChangeMoveSpeed;
		}
		else
		{
			//0よりも小さくしない
			m_normalCommandPosX = 0;
		}
	}


	//基本コマンドの表示

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_normalCommandAlpha);
	//フレームを表示する
	DrawGraph(m_normalCommandPosX, kCommandPosY[static_cast<int>(CommandSort::kPhysical)],
		m_showUi[frame].handle, true);
	DrawGraph(m_normalCommandPosX, kCommandPosY[static_cast<int>(CommandSort::kEnergy)],
		m_showUi[frame].handle, true);
	DrawGraph(m_normalCommandPosX, kCommandPosY[static_cast<int>(CommandSort::kDodge)],
		m_showUi[frame].handle, true);
	DrawGraph(m_normalCommandPosX, kCommandPosY[static_cast<int>(CommandSort::kCharge)],
		m_showUi[frame].handle, true);

	//各操作に対応したボタンを表示する
	DrawRectGraph(m_normalCommandPosX + kButtonGraphGapX, kCommandPosY[static_cast<int>(CommandSort::kPhysical)] + kButtonGraphGapY,
		kButtonGraphSize * static_cast<int>(ButtonSort::kB), 0, kButtonGraphSize, kButtonGraphSize,
		m_showUi[button].handle, true);
	DrawRectGraph(m_normalCommandPosX + kButtonGraphGapX, kCommandPosY[static_cast<int>(CommandSort::kEnergy)] + kButtonGraphGapY,
		kButtonGraphSize * static_cast<int>(ButtonSort::kX), 0, kButtonGraphSize, kButtonGraphSize,
		m_showUi[button].handle, true);
	DrawRectGraph(m_normalCommandPosX + kButtonGraphGapX, kCommandPosY[static_cast<int>(CommandSort::kDodge)] + kButtonGraphGapY,
		kButtonGraphSize * static_cast<int>(ButtonSort::kA), 0, kButtonGraphSize, kButtonGraphSize,
		m_showUi[button].handle, true);
	DrawRectGraph(m_normalCommandPosX + kButtonGraphGapX, kCommandPosY[static_cast<int>(CommandSort::kCharge)] + kButtonGraphGapY,
		kButtonGraphSize * static_cast<int>(ButtonSort::kY), 0, kButtonGraphSize, kButtonGraphSize,
		m_showUi[button].handle, true);

	//各操作の名前を表示する
	DrawStringToHandle(m_normalCommandPosX + kCommandStringGapX, kCommandPosY[static_cast<int>(CommandSort::kPhysical)] + kCommandStringGapY,
		"きんせつまほう", kBlackColor, m_commandFontHandle,kWhiteColor);
	DrawStringToHandle(m_normalCommandPosX + kCommandStringGapX, kCommandPosY[static_cast<int>(CommandSort::kEnergy)] + kCommandStringGapY,
		"まだん", kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawStringToHandle(m_normalCommandPosX + kCommandStringGapX, kCommandPosY[static_cast<int>(CommandSort::kDodge)] + kCommandStringGapY,
		"かいひ/ダッシュ", kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawStringToHandle(m_normalCommandPosX + kCommandStringGapX, kCommandPosY[static_cast<int>(CommandSort::kCharge)] + kCommandStringGapY,
		"チャージ", kBlackColor, m_commandFontHandle, kWhiteColor);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//必殺技コマンドの表示

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_specialCommandAlpha);
	//フレームを表示する
	DrawGraph(m_specialCommandPosX, kCommandPosY[static_cast<int>(CommandSort::kSpecialA)],
		m_showUi[frame].handle, true);
	DrawGraph(m_specialCommandPosX, kCommandPosY[static_cast<int>(CommandSort::kSpecialB)],
		m_showUi[frame].handle, true);
	DrawGraph(m_specialCommandPosX, kCommandPosY[static_cast<int>(CommandSort::kSpecialX)],
		m_showUi[frame].handle, true);
	DrawGraph(m_specialCommandPosX, kCommandPosY[static_cast<int>(CommandSort::kSpecialY)],
		m_showUi[frame].handle, true);

	//各操作に対応したボタンを表示する
	DrawRectGraph(m_specialCommandPosX + kButtonGraphGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialA)] + kButtonGraphGapY,
		kButtonGraphSize * static_cast<int>(ButtonSort::kA), 0, kButtonGraphSize, kButtonGraphSize,
		m_showUi[button].handle, true);
	DrawRectGraph(m_specialCommandPosX + kButtonGraphGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialB)] + kButtonGraphGapY,
		kButtonGraphSize * static_cast<int>(ButtonSort::kB), 0, kButtonGraphSize, kButtonGraphSize,
		m_showUi[button].handle, true);
	DrawRectGraph(m_specialCommandPosX + kButtonGraphGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialX)] + kButtonGraphGapY,
		kButtonGraphSize * static_cast<int>(ButtonSort::kX), 0, kButtonGraphSize, kButtonGraphSize,
		m_showUi[button].handle, true);
	DrawRectGraph(m_specialCommandPosX + kButtonGraphGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialY)] + kButtonGraphGapY,
		kButtonGraphSize * static_cast<int>(ButtonSort::kY), 0, kButtonGraphSize, kButtonGraphSize,
		m_showUi[button].handle, true);

	//各操作の名前を表示する

	DrawStringToHandle(m_specialCommandPosX + kCommandStringGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialA)] + kCommandStringGapY,
		attackInfo[Game::InputId::kA].name.c_str(), kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawStringToHandle(m_specialCommandPosX + kCommandStringGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialB)] + kCommandStringGapY,
		attackInfo[Game::InputId::kB].name.c_str(), kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawStringToHandle(m_specialCommandPosX + kCommandStringGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialX)] + kCommandStringGapY,
		attackInfo[Game::InputId::kX].name.c_str(), kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawStringToHandle(m_specialCommandPosX + kCommandStringGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialY)] + kCommandStringGapY,
		attackInfo[Game::InputId::kY].name.c_str(), kBlackColor, m_commandFontHandle, kWhiteColor);

	//各操作のコストを表示する
	DrawStringToHandle(m_specialCommandPosX + kCostStringGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialA)] + kCostStringGapY,
		std::to_string(attackInfo[Game::InputId::kA].cost).c_str(), kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawStringToHandle(m_specialCommandPosX + kCostStringGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialB)] + kCostStringGapY,
		std::to_string(attackInfo[Game::InputId::kB].cost).c_str(), kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawStringToHandle(m_specialCommandPosX + kCostStringGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialX)] + kCostStringGapY,
		std::to_string(attackInfo[Game::InputId::kX].cost).c_str(), kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawStringToHandle(m_specialCommandPosX + kCostStringGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialY)] + kCostStringGapY,
		std::to_string(attackInfo[Game::InputId::kY].cost).c_str(), kBlackColor, m_commandFontHandle, kWhiteColor);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//ガードコマンドを表示する
	DrawRectGraph(kGuardCommandPosX, kGuardCommandPosY,
		kShoulderButtonGraphSize * static_cast<int>(ButtonSort::kRb),
		0, kShoulderButtonGraphSize, kShoulderButtonGraphSize,
		m_showUi[shoulderButton].handle, true);
	DrawStringToHandle(kGuardCommandPosX + kShoulderButtonGraphGapX, kGuardCommandPosY + kShoulderButtonGraphGapY,
		"ガード", kBlackColor, m_commandFontHandle, kWhiteColor);
	//必殺技コマンドを表示するコマンドを表示する
	DrawRectGraph(kSpCommandControlPosX, kSpCommandControlPosY,
		kShoulderButtonGraphSize * static_cast<int>(ButtonSort::kLb),
		0, kShoulderButtonGraphSize, kShoulderButtonGraphSize,
		m_showUi[shoulderButton].handle, true);
	DrawStringToHandle(kSpCommandControlPosX + kShoulderButtonGraphGapX, kSpCommandControlPosY + kShoulderButtonGraphGapY,
		"Spコマンド", kBlackColor, m_commandFontHandle, kWhiteColor);
	//上下移動コマンドを表示するコマンドを表示する
	DrawRectGraph(kUpCommandPosX, kUpCommandPosY,
		kShoulderButtonGraphSize * static_cast<int>(ButtonSort::kLt),
		0, kShoulderButtonGraphSize, kShoulderButtonGraphSize,
		m_showUi[shoulderButton].handle, true);
	DrawStringToHandle(kUpCommandPosX + kShoulderButtonGraphGapX, kUpCommandPosY + kShoulderButtonGraphGapY,
		"アップ", kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawRectGraph(kDownCommandPosX, kDownCommandPosY,
		kShoulderButtonGraphSize * static_cast<int>(ButtonSort::kRt),
		0, kShoulderButtonGraphSize, kShoulderButtonGraphSize,
		m_showUi[shoulderButton].handle, true);
	DrawStringToHandle(kDownCommandPosX + kShoulderButtonGraphGapX, kDownCommandPosY + kShoulderButtonGraphGapY,
		"ダウン", kBlackColor, m_commandFontHandle, kWhiteColor);
}

void Ui::DrawComboCount()
{
	//コンボしていれば表示する(コンボは2回からカウント)
	if (m_comboCount > 1)
	{
		//文字列を左揃えにするための処理
		std::string comboString = std::to_string(m_comboCount);
		//数字の文字数を取得しその分左に座標ずらす(数字は半角なのでフォントサイズに0.5をかける)
		int length = static_cast<int>(GetStringLength(comboString.c_str()) * kComboCountFontSize * 0.5);

		//コンボ数を表示する
		DrawFormatStringToHandle(kComboCountDrawPosX - length, kComboCountDrawPosY, kComboFontColor, m_comboCountFontHandle, "%dCOMBO", m_comboCount);
		//表示したコンボ数を保存する
		m_lastComboCount = m_comboCount;
		//アルファ値を最大値にする
		m_comboUiAlpha = 255;
		//コンボをほめるコメントを初期位置に戻す
		m_comboPraiseCommentPosX = kComboPraiseCommentInitPosX;
	}
	//コンボが途切れていたら
	if (m_comboCount == 0 && m_lastComboCount > 1)
	{
		//一定時間コンボを数えないようにする
		m_isCountCombo = false;
	}
	//コンボを数えない時間の間
	if (!m_isCountCombo)
	{
		//コンボが途切れてからの時間を保存する
		m_comboEndTime++;

		//表示するコメント
		std::string praiseComment = "empty";

		//コンボをほめるコメントを設定する
		if (m_lastComboCount < kGoodComboNum)
		{
			//Good
			praiseComment = "Good";
		}
		else if (m_lastComboCount < kGreatComboNum)
		{
			//Great
			praiseComment = "Great";
		}
		else if (m_lastComboCount < kExcellentComboNum)
		{
			//Excellent
			praiseComment = "Excellent";
		}
		else
		{
			//Amaging
			praiseComment = "Amazing";
		}
		//コンボをほめるコメントの座標を調整する
		if (m_comboPraiseCommentPosX > kComboPraiseCommentPosX)
		{
			m_comboPraiseCommentPosX -= kComboPraiseCommentMoveSpeed;
		}
		//コンボが途切れてから一定時間たったら
		if (m_comboEndTime > kEndComboFadeOutStartTime)
		{
			//コンボ数とコメントをフェードアウトさせていく
			m_comboUiAlpha -= kComboUiFadeOutSpeed;
			//完全にフェードアウトしたら
			if (m_comboUiAlpha < 0)
			{
				//前のフレームのコンボ数を元に戻す
				m_lastComboCount = 0;
				//コンボのカウントを再開する
				m_isCountCombo = true;
				//途切れた時間カウントを初期化する
				m_comboEndTime = 0;
			}
		}
		//アルファ値の設定をする
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_comboUiAlpha);
		//文字列を左揃えにするための処理
		std::string comboString = std::to_string(m_lastComboCount);
		//数字の文字数を取得しその分左に座標ずらす(数字は半角なのでフォントサイズに0.5をかける)
		int length = static_cast<int>(GetStringLength(comboString.c_str()) * kComboCountFontSize * 0.5);
		//できたコンボ数を表示し続ける
		DrawFormatStringToHandle(kComboCountDrawPosX - length, kComboCountDrawPosY, kComboFontColor, m_comboCountFontHandle, "%dCOMBO", m_lastComboCount);
		//コメントを表示する
		DrawRotaGraph(m_comboPraiseCommentPosX, kComboPraiseCommentPosY, 1, 0, m_showUi[praiseComment].handle, true);
		//ブレンドモードを元に戻す
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

}
