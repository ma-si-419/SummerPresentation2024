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
}

SceneTitle::SceneTitle(SceneManager& sceneManager, DataManager& dataManager, SoundManager& soundManager) :
	SceneBase(sceneManager, dataManager, soundManager),
	m_isEnd(false),
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

	m_soundManager.Play("TitleBgm",DX_PLAYTYPE_LOOP);

	//Uiのロード
	LoadUiHandle(m_dataManager.GetUiData(Game::SceneNum::kTitle));
	//モデルのポジション設定
	MV1SetPosition(m_domeHandle, VGet(0, 0, 0));
	MV1SetPosition(m_playerHandle, kPlayerInitPos.CastVECTOR());
	MV1SetPosition(m_enemyHandle, kEnemyInitPos.CastVECTOR());
	//モデルのスケール設定
	MV1SetScale(m_domeHandle, VGet(5, 5, 5));
	MV1SetScale(m_playerHandle, VGet(0.03, 0.03, 0.03));
	MV1SetScale(m_enemyHandle, VGet(3, 3, 3));
	//モデルのアニメーション設定
	MV1AttachAnim(m_playerHandle,kPlayerAnimNumber);
	MV1AttachAnim(m_enemyHandle,kEnemyAnimNumber);
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
				//セレクトシーンに飛ぶ
//				m_sceneManager.ChangeScene(std::make_shared<SceneSelect>(m_sceneManager, m_dataManager));

				//ゲームシーンに直接飛ぶ
				m_soundManager.Stop("TitleBgm");
				m_sceneManager.ChangeScene(std::make_shared<SceneGame>(m_sceneManager, m_dataManager, m_soundManager));
				return;
			}
			//else if (m_selectItem == static_cast<int>(ItemKind::kOption))
			//{
			//	//オプションウィンドウを開く
			//	m_isOpenOption = true;
			//}
			else if (m_selectItem == static_cast<int>(ItemKind::kEnd))
			{
				//エンドウィンドウを開く
				m_isOpenEndWindow = true;
				//本当に終了するかどうかを「いいえ」に入れておく
				m_isEnd = false;
			}
		}
	}
	//オプションウィンドウを開いているとき
	//else if (m_isOpenOption)
	//{
	//	//戻るボタンを押したとき
	//	if (input.IsTrigger(Game::InputId::kCancel))
	//	{
	//		m_isOpenOption = false;
	//	}
	//}
	//エンドウィンドウを開いているとき
	else if (m_isOpenEndWindow)
	{

		//本当に終了するかどうかを選択する
		if (input.IsTrigger(Game::InputId::kLeft))
		{
			m_soundManager.Play(cursorSE, DX_PLAYTYPE_BACK);
			m_isEnd = true;
		}
		else if (input.IsTrigger(Game::InputId::kRight))
		{
			m_soundManager.Play(cursorSE, DX_PLAYTYPE_BACK);
			m_isEnd = false;
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
			if (m_isEnd)
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
		DrawGraph(m_showUi[titleLogo].drawPos.x, m_showUi[titleLogo].drawPos.y, m_showUi[titleLogo].handle, true);;
		//文字列の表示
		DrawStringToHandle(500, 500, "バトルスタート", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		DrawStringToHandle(620, 650, "やめる", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		//矢印の表示
		if (m_selectItem == static_cast<int>(ItemKind::kStart))
		{
			DrawStringToHandle(400 + sinf(m_shakeArrowNum) * kShakeArrowScale, 500, "→", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		}
		else if (m_selectItem == static_cast<int>(ItemKind::kEnd))
		{
			DrawStringToHandle(520 + sinf(m_shakeArrowNum) * kShakeArrowScale, 650, "→", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		}
	}
	//エンドウィンドウを開いているとき
	if (m_isOpenEndWindow)
	{
		DrawStringToHandle(450, 300, "ほんとうにやめる？", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		DrawStringToHandle(380, 500, "はい", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		DrawStringToHandle(950, 500, "いいえ", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		if (m_isEnd)
		{
			DrawStringToHandle(280 + sinf(m_shakeArrowNum) * kShakeArrowScale, 500, "→", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
		}
		else
		{
			DrawStringToHandle(850 + sinf(m_shakeArrowNum) * kShakeArrowScale, 500, "→", GetColor(0, 0, 0), m_fontHandle, GetColor(255, 255, 255));
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
