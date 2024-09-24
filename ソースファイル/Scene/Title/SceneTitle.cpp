#include "Game.h"
#include "SceneTitle.h"
#include "SceneSelect.h"
#include "SceneGame.h"
#include "TitleCamera.h"

namespace
{
	//プレイヤーの初期位置
	MyEngine::Vector3 kPlayerInitPos(-5, -8, -5);
	//エネミーの初期位置
	MyEngine::Vector3 kEnemyInitPos(5, -5, 5);
	//矢印を揺らす大きさ
	constexpr float kShakeArrowScale = 10.0f;
	//矢印を揺らすスピード
	constexpr float kShakeArrowSpeed = 0.5f;
	//プレイヤーのアニメーションの番号
	constexpr int kPlayerAnimNumber = 20;
	//プレイヤーのアニメーションの番号
	constexpr int kEnemyAnimNumber = 19;
	//スカイドームの大きさ
	constexpr float kDomeScale = 5.0f;
	//エネミーの大きさ
	constexpr float kEnemyScale = 3.0f;
	//プレイヤ－の大きさ
	constexpr float kPlayerScale = 0.03f;
	//矢印を文字列の横に表示するときにずらす大きさ
	constexpr int kArrowGap = -100;
	//何も開いていないときの文字列
	const char* const kTitleString[static_cast<int>(SceneTitle::ItemKind::kItemKindNum)] =
	{
		"バトルスタート",
		"やめる"
	};
	const char* const kEndString[static_cast<int>(SceneTitle::EndKind::kEndKindNum)] =
	{
		"ほんとうにやめる?",
		"やめる",
		"やめない"
	};
	//何も開いていないときの文字列の座標
	constexpr int kTitleStringPosX[static_cast<int>(SceneTitle::ItemKind::kItemKindNum)] = { 500,620 };
	constexpr int kTitleStringPosY[static_cast<int>(SceneTitle::ItemKind::kItemKindNum)] = { 500,650 };
	//終了画面を開いているときの文字列の座標
	constexpr int kEndStringPosX[static_cast<int>(SceneTitle::EndKind::kEndKindNum)] = {450,380,950};
	constexpr int kEndStringPosY[static_cast<int>(SceneTitle::EndKind::kEndKindNum)] = {300,500,500};
}

SceneTitle::SceneTitle(SceneManager& sceneManager, DataManager& dataManager, SoundManager& soundManager) :
	SceneBase(sceneManager, dataManager, soundManager),
	m_isOpenEndWindow(false),
	m_isOpenOption(false),
	m_selectItem(0),
	m_domeHandle(-1),
	m_playerHandle(-1),
	m_enemyHandle(-1),
	m_shakeArrowNum(0)
{
	m_domeHandle = MV1LoadModel("data/model/Dome.mv1");
	m_playerHandle = MV1LoadModel("data/model/Player.mv1");
	m_enemyHandle = MV1LoadModel("data/model/Enemy.mv1");
	m_pTitleCamera = std::make_shared <TitleCamera>();
	m_fontHandle = CreateFontToHandle("アンニャントロマン", 96, 9, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
}

SceneTitle::~SceneTitle()
{
	MV1DeleteModel(m_domeHandle);
	MV1DeleteModel(m_playerHandle);
	MV1DeleteModel(m_enemyHandle);
	DeleteFontToHandle(m_fontHandle);
}

void SceneTitle::Init()
{

	m_soundManager.SetHandle(m_dataManager.GetSoundData(Game::SceneNum::kTitle));

	m_soundManager.Play("TitleBgm", DX_PLAYTYPE_LOOP);

	//Uiのロード
	LoadUiHandle(m_dataManager.GetUiData(Game::SceneNum::kTitle));
	//モデルのポジション設定
	MV1SetPosition(m_domeHandle, VGet(0, 0, 0));
	MV1SetPosition(m_playerHandle, kPlayerInitPos.CastVECTOR());
	MV1SetPosition(m_enemyHandle, kEnemyInitPos.CastVECTOR());
	//モデルのスケール設定
	MV1SetScale(m_domeHandle, VGet(kDomeScale, kDomeScale, kDomeScale));
	MV1SetScale(m_playerHandle, VGet(kPlayerScale, kPlayerScale, kPlayerScale));
	MV1SetScale(m_enemyHandle, VGet(kEnemyScale, kEnemyScale, kEnemyScale));
	//モデルのアニメーション設定
	MV1AttachAnim(m_playerHandle, kPlayerAnimNumber);
	MV1AttachAnim(m_enemyHandle, kEnemyAnimNumber);
	//プレイヤーとエネミーが向き合う形にする
	MV1SetRotationZYAxis(m_playerHandle, (kPlayerInitPos - kEnemyInitPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
	MV1SetRotationZYAxis(m_enemyHandle, (kEnemyInitPos - kPlayerInitPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
	//カメラの初期化
	m_pTitleCamera->Init();
}

void SceneTitle::Update(MyEngine::Input input)
{
	//選択している項目を指す矢印は常に揺らし続ける
	m_shakeArrowNum += kShakeArrowSpeed;

	//オプションもエンドウィンドウも開いていないとき
	bool isCloseWindow = !m_isOpenEndWindow && !m_isOpenOption;

	/*ウィンドウの状況で処理を変える*/

	std::string cursorSE = "Cursor";
	std::string OKSE = "OK";
	std::string cancelSE = "Cancel";

	//何も開いていないとき
	if (isCloseWindow)
	{
		if (input.IsTrigger(Game::InputId::kUp))
		{
			if (m_selectItem > 0)
			{
				m_selectItem--;
				m_soundManager.Play(cursorSE, DX_PLAYTYPE_BACK);
			}
		}
		else if (input.IsTrigger(Game::InputId::kDown))
		{
			if (m_selectItem < static_cast<int>(ItemKind::kEnd))
			{
				m_selectItem++;
				m_soundManager.Play(cursorSE, DX_PLAYTYPE_BACK);
			}
		}
		if (input.IsTrigger(Game::InputId::kOk))
		{
			//決定音を流す
			m_soundManager.Play(OKSE, DX_PLAYTYPE_BACK);
			if (m_selectItem == static_cast<int>(ItemKind::kStart))
			{
				//ゲームシーンに直接飛ぶ
				m_soundManager.Stop("TitleBgm");
				m_sceneManager.ChangeScene(std::make_shared<SceneGame>(m_sceneManager, m_dataManager, m_soundManager));
				return;
			}

			else if (m_selectItem == static_cast<int>(ItemKind::kEnd))
			{
				//エンドウィンドウを開く
				m_isOpenEndWindow = true;
				m_selectItem = static_cast<int>(EndKind::kBack);
			}
		}
	}
	//エンドウィンドウを開いているとき
	else if (m_isOpenEndWindow)
	{

		//本当に終了するかどうかを選択する
		if (input.IsTrigger(Game::InputId::kLeft))
		{
			m_soundManager.Play(cursorSE, DX_PLAYTYPE_BACK);
			m_selectItem = static_cast<int>(EndKind::kEnd);
		}
		else if (input.IsTrigger(Game::InputId::kRight))
		{
			m_soundManager.Play(cursorSE, DX_PLAYTYPE_BACK);
			m_selectItem = static_cast<int>(EndKind::kBack);
		}

		//戻るボタンを押したとき
		if (input.IsTrigger(Game::InputId::kCancel))
		{
			m_soundManager.Play(cancelSE, DX_PLAYTYPE_BACK);
			m_isOpenEndWindow = false;
		}

		//決定ボタンを押したとき
		if (input.IsTrigger(Game::InputId::kOk))
		{
			//本当に閉じるかどうかを確認
			if (m_selectItem == static_cast<int>(EndKind::kEnd))
			{
				m_soundManager.Play(cancelSE, DX_PLAYTYPE_BACK);
				//ゲームを終了する
				m_sceneManager.GameEnd();
			}
			else
			{
				m_soundManager.Play(cancelSE, DX_PLAYTYPE_BACK);
				//ウィンドウを閉じる
				m_isOpenEndWindow = false;
			}
		}
	}
	m_pTitleCamera->Update();
}

void SceneTitle::Draw()
{
	//背景の描画
	MV1DrawModel(m_domeHandle);
	//キャラクターの描画
	MV1DrawModel(m_playerHandle);
	MV1DrawModel(m_enemyHandle);



	//オプションもエンドウィンドウも開いていないとき
	bool isCloseWindow = !m_isOpenEndWindow && !m_isOpenOption;

	//ウィンドウを何も開いていないとき
	if (isCloseWindow)
	{
		//タイトルロゴの表示
		std::string titleLogo = "TitleLogo";
		DrawGraph(static_cast<int>(m_showUi[titleLogo].drawPos.x), static_cast<int>(m_showUi[titleLogo].drawPos.y), m_showUi[titleLogo].handle, true);;
		//文字列の表示
		DrawStringToHandle(kTitleStringPosX[static_cast<int>(ItemKind::kStart)], kTitleStringPosY[static_cast<int>(ItemKind::kStart)], kTitleString[static_cast<int>(ItemKind::kStart)], GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		DrawStringToHandle(kTitleStringPosX[static_cast<int>(ItemKind::kEnd)], kTitleStringPosY[static_cast<int>(ItemKind::kEnd)], kTitleString[static_cast<int>(ItemKind::kEnd)], GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		//矢印の表示
		if (m_selectItem == static_cast<int>(ItemKind::kStart))
		{
			DrawStringToHandle(kTitleStringPosX[static_cast<int>(ItemKind::kStart)] + kArrowGap + static_cast<int>(sinf(m_shakeArrowNum) * kShakeArrowScale), kTitleStringPosY[static_cast<int>(ItemKind::kStart)], "→", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		}
		else if (m_selectItem == static_cast<int>(ItemKind::kEnd))
		{
			DrawStringToHandle(kTitleStringPosX[static_cast<int>(ItemKind::kEnd)] + kArrowGap + static_cast<int>(sinf(m_shakeArrowNum) * kShakeArrowScale), kTitleStringPosY[static_cast<int>(ItemKind::kEnd)], "→", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		}
	}
	//エンドウィンドウを開いているとき
	if (m_isOpenEndWindow)
	{
		DrawStringToHandle(kEndStringPosX[static_cast<int>(EndKind::kConfirmation)], kEndStringPosY[static_cast<int>(EndKind::kConfirmation)], kEndString[static_cast<int>(EndKind::kConfirmation)], GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		DrawStringToHandle(kEndStringPosX[static_cast<int>(EndKind::kEnd)], kEndStringPosY[static_cast<int>(EndKind::kEnd)], kEndString[static_cast<int>(EndKind::kEnd)], GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		DrawStringToHandle(kEndStringPosX[static_cast<int>(EndKind::kBack)], kEndStringPosY[static_cast<int>(EndKind::kBack)], kEndString[static_cast<int>(EndKind::kBack)], GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		if (m_selectItem == static_cast<int>(EndKind::kEnd))
		{
			DrawStringToHandle(kEndStringPosX[static_cast<int>(EndKind::kEnd)] + kArrowGap + static_cast<int>(sinf(m_shakeArrowNum) * kShakeArrowScale), kEndStringPosY[static_cast<int>(EndKind::kEnd)], "→", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		}
		else
		{
			DrawStringToHandle(kEndStringPosX[static_cast<int>(EndKind::kBack)] + kArrowGap + static_cast<int>(sinf(m_shakeArrowNum) * kShakeArrowScale), kEndStringPosY[static_cast<int>(EndKind::kBack)], "→", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		}
	}
}

void SceneTitle::End()
{
	for (auto& item : m_showUi)
	{
		DeleteGraph(item.second.handle);
	}
}

void SceneTitle::LoadUiHandle(std::vector<DataManager::UiInfo> data)
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
