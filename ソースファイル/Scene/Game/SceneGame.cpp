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
	//�Q�[���I�[�o�[���̑I�����̐�
	constexpr int kGameOverItemNum = 1;
	//�o�g���J�n�܂ł̎���
	constexpr int kStandByTime = 120;
	//�N���A���̃G�t�F�N�g���o���Ԋu
	constexpr int kClearEffectPopTime = 10;
	//�N���A���̃G�t�F�N�g�̍��W�͈̔�
	constexpr int kClearEffectPopRange = 20;
	constexpr int kClearEffectPopRangeHalf = 10;
	//���y���ς��̗͂̊���
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

	//�����蔻��Ǘ��N���X�̃|�C���^
	m_pPhysics = std::make_shared<Physics>();
	//�v���C���[�̃|�C���^
	m_pPlayer = std::make_shared<Player>();
	//�J�����̃|�C���^
	m_pGameCamera = std::make_shared<GameCamera>();
	//�G�l�~�[�̃|�C���^
	m_pEnemy = std::make_shared<Enemy>();
	//Ui�̃|�C���^
	m_pUi = std::make_shared<Ui>();

#ifdef _DEBUG

	//�X�e�[�W�̃��f���n���h��
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

	//�G�l�~�[�̏�����(�����蔻���o�^����)
	m_pEnemy->Init(m_pPhysics);
	//�G�l�~�[��State�p�^�[���̏�����
	m_pEnemy->InitAiState(shared_from_this());
	//UI�̃|�C���^��n��
	m_pPlayer->SetUiManager(m_pUi);
	m_pEnemy->SetUiManager(m_pUi);

	//�G�l�~�[�̍��W���v���C���[�ɓn��
	m_pPlayer->SetTargetPos(m_pEnemy->GetPos());
	//�v���C���[�ɃV�[���̃|�C���^��n��
	m_pPlayer->SetGameScene(shared_from_this());
	//�v���C���[�̏�����(�����蔻���o�^����)
	m_pPlayer->Init(m_pPhysics);
	//�G�l�~�[�Ƀv���C���[�̍��W��n��
	m_pEnemy->SetTargetPos(m_pPlayer->GetPos());
	//�G�l�~�[�ɃV�[���̃|�C���^��n��
	m_pEnemy->SetGameScene(shared_from_this());
	//�J�����Ƀv���C���[�̍��W��n��
	m_pGameCamera->SetPlayerPos(m_pPlayer->GetPos());
	//�J�����ɃG�l�~�[�̍��W��n��
	m_pGameCamera->SetTargetPos(m_pEnemy->GetPos());
	//�J�����Ƀv���C���[�̈ړ��ʂ�n��
//	m_pGameCamera->SetPlayerVelo(m_pPlayer->GetVelo());
	//�J�����̏�����
	m_pGameCamera->Init();

	//�v���C���[�ɕK�E�Z�̃f�[�^������
	m_pPlayer->SetAttackData(m_dataManager.GetAttackData());
	//�G�l�~�[�ɕK�E�Z�̃f�[�^������
	m_pEnemy->SetAttackData(m_dataManager.GetAttackData());
	//�G�l�~�[��AI�̃f�[�^������
	m_pEnemy->SetAiData(m_dataManager.GetAiData());

	//UI�ɉ摜�̃f�[�^������
	m_pUi->LoadUiHandle(m_dataManager.GetUiData(Game::SceneNum::kGame));

#ifdef _DEBUG

	//�X�e�[�W�̃��f���n���h��
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
	//�J�����Ƀv���C���[�̍��W��n��
	m_pGameCamera->SetPlayerPos(m_pPlayer->GetPos());
	//�J�����ɃG�l�~�[�̍��W��n��
	m_pGameCamera->SetTargetPos(m_pEnemy->GetPos());
	//�c���Ă���U�������ׂĂ���
	for (auto item : m_pAttacks)
	{
		item->Final(m_pPhysics);
	}
	m_pAttacks.clear();
	//�J�����̏�����
	m_pGameCamera->Init();
	m_isGameOver = false;
	m_isStartBattle = false;
	m_time = 0;
}

void SceneGame::Update(MyEngine::Input input)
{
	//���Ԃ��v������
	m_time++;
	//�o�g�����J�n���Ă��Ȃ��Ƃ��̏���
	if (!m_isStartBattle)
	{
		m_pPlayer->PlayAnim();
		m_pEnemy->PlayAnim();
		//��莞�ԑҋ@������
		if (m_time > kStandByTime)
		{
			//�o�g�����J�n����
			m_isStartBattle = true;
		}

	}
	//�o�g���J�n��̏���
	else
	{
		//�G�̗̑͂�0�ɂȂ�����
		if (m_pEnemy->GetNowHp() <= 0)
		{
			m_isGameClear = true;
		}

		//�Q�[���I�[�o�[���̏���
		if (m_isGameOver)
		{
			//�����
			if (input.IsTrigger(Game::InputId::kUp))
			{
				m_gameOverSelectItemNum--;
				if (m_gameOverSelectItemNum < 0)
				{
					m_gameOverSelectItemNum = 0;
				}
			}
			//������
			if (input.IsTrigger(Game::InputId::kDown))
			{
				m_gameOverSelectItemNum++;
				if (m_gameOverSelectItemNum > static_cast<int>(Ui::GameOverItem::kItemNum))
				{
					m_gameOverSelectItemNum = static_cast<int>(Ui::GameOverItem::kItemNum);
				}
			}
			//�������
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
		//�Q�[���N���A���̏���
		else if (m_isGameClear)
		{
			if (m_isClearSe)
			{
				m_soundManager.Stop("BattleStartBgm");
				m_soundManager.Stop("BattleEndBgm");
				m_soundManager.Play("ClearFanfare", DX_PLAYTYPE_BACK);
				m_isClearSe = false;
			}
			//SE���Ȃ�I�����BGM��炷
			if (CheckSoundMem(m_soundManager.GetHandle("ClearFanfare")))
			{
				if (m_isClearBgm)
				{
					m_soundManager.Play("ClearBgm", DX_PLAYTYPE_LOOP);
					m_isClearBgm = false;
				}
			}
			//��莞�Ԃ��ƂɃG�t�F�N�g���o��
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
		//�Q�[���I�[�o�[���Q�[���N���A���Ă��Ȃ��Ƃ��̏���
		else
		{
			//�����蔻��̍X�V
			m_pPhysics->Update();
			//�v���C���[�̍X�V
			m_pPlayer->Update(shared_from_this(), input);
			//�G�l�~�[�̍X�V
			m_pEnemy->StateUpdate(m_pPlayer);
			m_pEnemy->Update(shared_from_this());
		}
		//�v���C���[�ɃG�l�~�[�̍��W��n��
		m_pPlayer->SetTargetPos(m_pEnemy->GetPos());
		//�G�l�~�[�Ƀv���C���[�̍��W��n��
		m_pEnemy->SetTargetPos(m_pPlayer->GetPos());
		//�J�����Ƀv���C���[�̍��W��n��
		m_pGameCamera->SetPlayerPos(m_pPlayer->GetPos());
		//�J�����ɃG�l�~�[�̍��W��n��
		m_pGameCamera->SetTargetPos(m_pEnemy->GetPos());
		//�J�����̎���p���L���邩�ǂ���
		if (m_pPlayer->GetIsUpFov())
		{
			m_pGameCamera->UpFov();
		}

		//�J�����̍X�V
		m_pGameCamera->Update();
		for (auto& attack : m_pAttacks)
		{
			//�U���̍X�V
			if (attack->GetTag() == ObjectTag::kPlayerAttack)
			{
				attack->Update(m_pEnemy->GetPos());
			}
			else if (attack->GetTag() == ObjectTag::kEnemyAttack)
			{
				attack->Update(m_pPlayer->GetPos());
			}
			//���������Ȃ��U����������
			if (!attack->GetIsExist())
			{
				//�����蔻�������
				attack->Final(m_pPhysics);
			}
		}

		for (int i = 0; i < m_pAttacks.size(); i++)
		{
			//�U���������Ă�����
			if (!m_pAttacks[i]->GetIsExist())
			{
				//�z�񂩂����
				m_pAttacks.erase(m_pAttacks.begin() + i);
				i--;
			}
		}

	}
	//�̗͂����ʈȉ��ɂȂ����特�y��ς���
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

	//�G�t�F�N�g���X�V����
	UpdateEffekseer3D();

}

void SceneGame::Draw()
{
	//�X�J�C�h�[���̕`��
	m_pGameCamera->DrawDome();
	//�v���C���[�̕`��
	m_pPlayer->Draw();
	//�G�l�~�[�̕`��
	m_pEnemy->Draw();

#ifdef _DEBUG
	//�����蔻��̕`��
	m_pPhysics->DebugDraw();
#endif
	//�I�u�W�F�N�g�̕`��
	ObjectManager::GetInstance().Draw();
	//�G�t�F�N�g��`�悷��
	EffekseerManager::GetInstance().Draw();
	//�v���C���[�ƃG�l�~�[�̗̑̓o�[��\������
	m_pUi->DrawStateBar(m_pPlayer, m_pEnemy);
	//�^�����_���[�W�̕\��
	m_pUi->DrawDamage();
	//�R�}���h�̕\��
	m_pUi->DrawCommand(m_pPlayer->GetIsOpenSpecialPallet(), m_pPlayer->GetSetSpecialAttackInfo());
	//�R���{���̕\��
	m_pUi->DrawComboCount();
	MyEngine::Vector3 pos = m_pGameCamera->GetPos();


	//�Q�[���I�[�o�[����UI�̕\��
	if (m_isGameOver)
	{
		m_pUi->DrawGameOver(m_gameOverSelectItemNum);
	}
	else if (m_isGameClear)
	{
		m_pUi->DrawGameClear();
	}

	//�Q�[���J�n����Ready��Fight�̕\��
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
