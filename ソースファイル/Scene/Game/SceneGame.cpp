#include "Game.h"
#include "SceneGame.h"
#include "SceneTitle.h"
#include "Physics.h"
#include "Player.h"
#include "GameCamera.h"
#include "Enemy.h"
#include "Ui.h"
#include "AttackBase.h"
#include "LoadCsv.h"
#include "ObjectManager.h"
#include "EffekseerManager.h"
#include "EffekseerForDXLib.h"

namespace
{
	//ゲームオーバー時の選択肢の数
	constexpr int kGameOverItemNum = 1;
	//バトル開始までの時間
	constexpr int kStandByTime = 120;
	//クリア時のエフェクトを出す間隔
	constexpr int kClearEffectPopTime = 10;
	//クリア時のエフェクトの座標の範囲
	constexpr int kClearEffectPopRange = 20;
	constexpr int kClearEffectPopRangeHalf = 10;
	//音楽が変わる体力の割合
	constexpr float kMusicChangeHpRate = 0.3f;
}

SceneGame::SceneGame(SceneManager& sceneManager, DataManager& dataManager, SoundManager& soundManager) :
	SceneBase(sceneManager, dataManager, soundManager),
	m_isGameOver(false),
	m_gameOverSelectItemNum(0),
	m_isStartBattle(false),
	m_time(0),
	m_isGameClear(false),
	m_isFinalStage(false),
	m_isClearSe(true),
	m_isClearBgm(true)
{

	//当たり判定管理クラスのポインタ
	m_pPhysics = std::make_shared<Physics>();
	//プレイヤーのポインタ
	m_pPlayer = std::make_shared<Player>();
	//カメラのポインタ
	m_pGameCamera = std::make_shared<GameCamera>();
	//エネミーのポインタ
	m_pEnemy = std::make_shared<Enemy>();
	//Uiのポインタ
	m_pUi = std::make_shared<Ui>();

#ifdef _DEBUG

	//ステージのモデルハンドル
	m_handle =  MV1LoadModel("data/model/Stage.mv1");

#endif // !_DEBUG
}

SceneGame::~SceneGame()
{
}

void SceneGame::Init()
{
	m_dataManager.LoadAnimationFile();

	ObjectManager::GetInstance().SetObject(Game::SceneNum::kGame);

	m_pPlayer->SetAnimationData(m_dataManager.GetAnimationData(), true);
	m_pEnemy->SetAnimationData(m_dataManager.GetAnimationData(), false);

	m_soundManager.SetHandle(m_dataManager.GetSoundData(Game::SceneNum::kGame));

	m_soundManager.Play("BattleStartBgm", DX_PLAYTYPE_LOOP);

	//エネミーの初期化(当たり判定を登録する)
	m_pEnemy->Init(m_pPhysics);
	//エネミーのStateパターンの初期化
	m_pEnemy->InitAiState(shared_from_this());
	//UIのポインタを渡す
	m_pPlayer->SetUiManager(m_pUi);
	m_pEnemy->SetUiManager(m_pUi);

	//エネミーの座標をプレイヤーに渡す
	m_pPlayer->SetTargetPos(m_pEnemy->GetPos());
	//プレイヤーにシーンのポインタを渡す
	m_pPlayer->SetGameScene(shared_from_this());
	//プレイヤーの初期化(当たり判定を登録する)
	m_pPlayer->Init(m_pPhysics);
	//エネミーにプレイヤーの座標を渡す
	m_pEnemy->SetTargetPos(m_pPlayer->GetPos());
	//エネミーにシーンのポインタを渡す
	m_pEnemy->SetGameScene(shared_from_this());
	//カメラにプレイヤーの座標を渡す
	m_pGameCamera->SetPlayerPos(m_pPlayer->GetPos());
	//カメラにエネミーの座標を渡す
	m_pGameCamera->SetTargetPos(m_pEnemy->GetPos());
	//カメラにプレイヤーの移動量を渡す
//	m_pGameCamera->SetPlayerVelo(m_pPlayer->GetVelo());
	//カメラの初期化
	m_pGameCamera->Init();

	//プレイヤーに必殺技のデータを入れる
	m_pPlayer->SetAttackData(m_dataManager.GetAttackData());
	//エネミーに必殺技のデータを入れる
	m_pEnemy->SetAttackData(m_dataManager.GetAttackData());
	//エネミーにAIのデータを入れる
	m_pEnemy->SetAiData(m_dataManager.GetAiData());

	//UIに画像のデータを入れる
	m_pUi->LoadUiHandle(m_dataManager.GetUiData(Game::SceneNum::kGame));

#ifdef _DEBUG

	//ステージのモデルハンドル
	MV1SetScale(m_handle,VGet(2,2,2));
	MV1SetPosition(m_handle,VGet(0,-1000,0));

#endif // !_DEBUG
}

void SceneGame::RetryInit()
{
	m_pPlayer->EndEffect();
	m_pEnemy->EndEffect();
	m_pEnemy->InitAiState(shared_from_this());
	m_pEnemy->InitPos();
	m_pPlayer->SetTargetPos(m_pEnemy->GetPos());
	m_pPlayer->RetryInit();
	m_pEnemy->SetTargetPos(m_pPlayer->GetPos());
	m_pEnemy->RetryInit();
	m_pUi->RetryInit();
	//カメラにプレイヤーの座標を渡す
	m_pGameCamera->SetPlayerPos(m_pPlayer->GetPos());
	//カメラにエネミーの座標を渡す
	m_pGameCamera->SetTargetPos(m_pEnemy->GetPos());
	//残っている攻撃をすべてけす
	for (auto item : m_pAttacks)
	{
		item->Final(m_pPhysics);
	}
	m_pAttacks.clear();
	//カメラの初期化
	m_pGameCamera->Init();
	m_isGameOver = false;
	m_isStartBattle = false;
	m_time = 0;
}

void SceneGame::Update(MyEngine::Input input)
{
	//時間を計測する
	m_time++;
	//バトルが開始していないときの処理
	if (!m_isStartBattle)
	{
		m_pPlayer->PlayAnim();
		m_pEnemy->PlayAnim();
		//一定時間待機したら
		if (m_time > kStandByTime)
		{
			//バトルを開始する
			m_isStartBattle = true;
		}

	}
	//バトル開始後の処理
	else
	{
		//敵の体力が0になったら
		if (m_pEnemy->GetNowHp() <= 0)
		{
			m_isGameClear = true;
		}

		//ゲームオーバー時の処理
		if (m_isGameOver)
		{
			//上入力
			if (input.IsTrigger(Game::InputId::kUp))
			{
				m_gameOverSelectItemNum--;
				if (m_gameOverSelectItemNum < 0)
				{
					m_gameOverSelectItemNum = 0;
				}
			}
			//下入力
			if (input.IsTrigger(Game::InputId::kDown))
			{
				m_gameOverSelectItemNum++;
				if (m_gameOverSelectItemNum > static_cast<int>(Ui::GameOverItem::kItemNum))
				{
					m_gameOverSelectItemNum = static_cast<int>(Ui::GameOverItem::kItemNum);
				}
			}
			//決定入力
			if (input.IsTrigger(Game::InputId::kOk))
			{
				if (m_gameOverSelectItemNum == static_cast<int>(Ui::GameOverItem::kRetry))
				{
					RetryInit();
				}
				else if (m_gameOverSelectItemNum == static_cast<int>(Ui::GameOverItem::kEnd))
				{
					m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager, m_dataManager, m_soundManager));
					return;
				}
			}
		}
		//ゲームクリア時の処理
		else if (m_isGameClear)
		{
			if (m_isClearSe)
			{
				m_soundManager.Stop("BattleStartBgm");
				m_soundManager.Stop("BattleEndBgm");
				m_soundManager.Play("ClearFanfare", DX_PLAYTYPE_BACK);
				m_isClearSe = false;
			}
			//SEがなり終わったBGMを鳴らす
			if (CheckSoundMem(m_soundManager.GetHandle("ClearFanfare")))
			{
				if (m_isClearBgm)
				{
					m_soundManager.Play("ClearBgm", DX_PLAYTYPE_LOOP);
					m_isClearBgm = false;
				}
			}
			//一定時間ごとにエフェクトを出す
			if (m_time % kClearEffectPopTime == 0)
			{
				MyEngine::Vector3 effectPos = m_pPlayer->GetPos();
				effectPos.x += GetRand(kClearEffectPopRange) - kClearEffectPopRangeHalf;
				effectPos.y += GetRand(kClearEffectPopRange) - kClearEffectPopRangeHalf;
				effectPos.z += GetRand(kClearEffectPopRange) - kClearEffectPopRangeHalf;


				std::shared_ptr<EffekseerData> clearEffect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("clear"),effectPos,false);
				
				EffekseerManager::GetInstance().Entry(clearEffect);
			}

			if (input.IsTrigger(Game::InputId::kA))
			{
				m_soundManager.Stop("ClearBgm");
				m_soundManager.Play("OK", DX_PLAYTYPE_BACK);
				m_sceneManager.ChangeScene(std::make_shared<SceneTitle>(m_sceneManager, m_dataManager, m_soundManager));
				return;
			}
		}
		//ゲームオーバーもゲームクリアしていないときの処理
		else
		{
			//当たり判定の更新
			m_pPhysics->Update();
			//プレイヤーの更新
			m_pPlayer->Update(shared_from_this(), input);
			//エネミーの更新
			m_pEnemy->StateUpdate(m_pPlayer);
			m_pEnemy->Update(shared_from_this());
		}
		//プレイヤーにエネミーの座標を渡す
		m_pPlayer->SetTargetPos(m_pEnemy->GetPos());
		//エネミーにプレイヤーの座標を渡す
		m_pEnemy->SetTargetPos(m_pPlayer->GetPos());
		//カメラにプレイヤーの座標を渡す
		m_pGameCamera->SetPlayerPos(m_pPlayer->GetPos());
		//カメラにエネミーの座標を渡す
		m_pGameCamera->SetTargetPos(m_pEnemy->GetPos());
		//カメラの視野角を広げるかどうか
		if (m_pPlayer->GetIsUpFov())
		{
			m_pGameCamera->UpFov();
		}

		//カメラの更新
		m_pGameCamera->Update();
		for (auto& attack : m_pAttacks)
		{
			//攻撃の更新
			if (attack->GetTag() == ObjectTag::kPlayerAttack)
			{
				attack->Update(m_pEnemy->GetPos());
			}
			else if (attack->GetTag() == ObjectTag::kEnemyAttack)
			{
				attack->Update(m_pPlayer->GetPos());
			}
			//処理をしない攻撃だったら
			if (!attack->GetIsExist())
			{
				//当たり判定を消す
				attack->Final(m_pPhysics);
			}
		}

		for (int i = 0; i < m_pAttacks.size(); i++)
		{
			//攻撃が消えていたら
			if (!m_pAttacks[i]->GetIsExist())
			{
				//配列から消す
				m_pAttacks.erase(m_pAttacks.begin() + i);
				i--;
			}
		}

	}
	//体力が一定量以下になったら音楽を変える
	if (m_pEnemy->GetNowHp() < m_pEnemy->GetStatus().hp * kMusicChangeHpRate ||
		m_pPlayer->GetNowHp() < m_pPlayer->GetStatus().hp * kMusicChangeHpRate)
	{
		if (!m_isFinalStage)
		{
			m_soundManager.Stop("BattleStartBgm");

			m_soundManager.Play("BattleEndBgm", DX_PLAYTYPE_LOOP);
			m_isFinalStage = true;
		}
	}

	//エフェクトを更新する
	UpdateEffekseer3D();

}

void SceneGame::Draw()
{
	//スカイドームの描画
	m_pGameCamera->DrawDome();
	//プレイヤーの描画
	m_pPlayer->Draw();
	//エネミーの描画
	m_pEnemy->Draw();

#ifdef _DEBUG
	//当たり判定の描画
	m_pPhysics->DebugDraw();
#endif
	//オブジェクトの描画
	ObjectManager::GetInstance().Draw();
	//エフェクトを描画する
	EffekseerManager::GetInstance().Draw();
	//プレイヤーとエネミーの体力バーを表示する
	m_pUi->DrawStateBar(m_pPlayer, m_pEnemy);
	//与えたダメージの表示
	m_pUi->DrawDamage();
	//コマンドの表示
	m_pUi->DrawCommand(m_pPlayer->GetIsOpenSpecialPallet(), m_pPlayer->GetSetSpecialAttackInfo());
	//コンボ数の表示
	m_pUi->DrawComboCount();
	MyEngine::Vector3 pos = m_pGameCamera->GetPos();


	//ゲームオーバー時のUIの表示
	if (m_isGameOver)
	{
		m_pUi->DrawGameOver(m_gameOverSelectItemNum);
	}
	else if (m_isGameClear)
	{
		m_pUi->DrawGameClear();
	}

	//ゲーム開始時のReadyやFightの表示
	m_pUi->DrawStartSign(m_isStartBattle);
}

void SceneGame::End()
{
	m_pPlayer->Final(m_pPhysics);
	m_pEnemy->Final(m_pPhysics);
}

void SceneGame::AddAttack(std::shared_ptr<AttackBase> pAttack)
{
	m_pAttacks.push_back(pAttack);
}

void SceneGame::PlaySE(std::string name, int playType)
{
	m_soundManager.Play(name, playType);
}
