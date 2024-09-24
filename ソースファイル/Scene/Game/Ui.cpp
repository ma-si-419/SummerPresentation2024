#include "Ui.h"
#include "DxLib.h"
#include "Game.h"
#include "Player.h"
#include "Enemy.h"

namespace
{
	//HP�̎c�ʂ�\������Ƃ��̃t�H���g�̑傫��
	constexpr int kHpNumFontSize = 20;
	//MP�̎c�ʂ�\������Ƃ��̃t�H���g�̑傫��
	constexpr int kMpNumFontSize = 15;
	//HP�o�[�̑傫��
	constexpr int kHpBarHeight = 15;
	constexpr int kHpBarWidth = 188;
	//MP�o�[�̑傫��
	constexpr int kMpBarHeight = 8;
	constexpr int kMpBarWidth = 188;
	//�X�^���o�[�̑傫��
	constexpr int kStanBarHeight = 8;
	constexpr int kStanBarWidth = 128;
	//�̗͂����������̐Ԃ��Q�[�W���������Ă����܂ł̎���
	constexpr int kLostHpBarLifeTime = 30;
	//�Ԃ��Q�[�W���������鑬�x
	constexpr int kLostHpBarDecreaseSpeed = 3;
	//�_���[�W���󂯂����ɗh���傫��
	constexpr int kShakeScale = 15;
	//�_���[�W���󂯂����ɗh���傫���̔���
	constexpr int kShakeHalfScale = static_cast<int>(kShakeScale * 0.5);
	//HP�o�[�̍��W(�摜�̍��W�ɑ΂��Ă�HP�o�[�̍��W�̂���𒼂�)
	constexpr int kHpBarPosX = -52;
	constexpr int kHpBarPosY = 8;
	//�v���C���[��MP�o�[�̍��W(�摜�̍��W�ɑ΂��Ă�MP�o�[�̍��W�̂���𒼂�)
	constexpr int kMpBarPosX = -36;
	constexpr int kMpBarPosY = 40;
	//�v���C���[��HP�̗ʂ�\��������W
	constexpr int kHpNumPosX = 42;
	constexpr int kHpNumPosY = -3;
	//�v���C���[��MP�̗ʂ�\��������W
	constexpr int kMpNumPosX = 100;
	constexpr int kMpNumPosY = 32;
	//�G�l�~�[�̃X�^���o�[�̍��W(�摜�̍��W�ɑ΂��ẴX�^���o�[�̍��W�̂���𒼂�)
	constexpr int kStanBarPosX = 23;
	constexpr int kStanBarPosY = 40;
	//�_���[�W��\������t�H���g�̑傫��
	constexpr int kDamageFontSize = 48;
	//�_���[�W��\�����鎞��
	constexpr float kDamageShowTime = 30;
	//�_���[�W�\���������n�߂鎞��
	constexpr float kDamageVanishTime = 5;
	//�_���[�W��\��������W���������炷
	constexpr int kDamageShowPosShakeScale = 80;
	constexpr int kDamageShowPosShakeScaleHalf = static_cast<int>(kDamageShowPosShakeScale * 0.5);
	constexpr int kDamageShowPosShiftY = -50;
	//����h�炷�傫��
	constexpr float kShakeArrowScale = 10.0f;
	//����h�炷�X�s�[�h
	constexpr float kShakeArrowSpeed = 0.5f;
	//�Q�[���I�[�o�[�̕�����\��������W
	constexpr int kGameOverStringPosX[2] = { 637,660 };
	constexpr int kGameOverStringPosY[2] = { 490,650 };
	//�Q�[���I�[�o�[�̕����̑傫��
	constexpr int kGameOverFontSize = 96;
	//���ƕ����̋���
	constexpr int kArrowDistance = 75;
	//�o�g���J�n����Ready�̊g�嗦
	constexpr float kReadyExRate = 1.0f;
	//�o�g���J�n����fight�̏����鑬�x
	constexpr int kFightFadeOutSpeed = 15;
	//�o�g���J�n����fight��������܂ł̎���
	constexpr int kFightShowTime = 60;
	//�o�g���J�n����fight�̏����g�嗦
	constexpr float kFightInitExRate = 3.0f;
	//�o�g���J�n����fight�̍ŏI�g�嗦
	constexpr float kFightFinalExRate = 1.0f;
	//�o�g���J�n����fight��h�炷����
	constexpr int kFightShakeTime = 20;
	//�o�g���J�n����fight��h�炷�傫��
	constexpr int kFightShakeScale = 20;
	//�o�g���J�n����fight��h�炷�傫���̔���
	constexpr int kFightShakeScaleHalf = static_cast<int>(kFightShakeScale * 0.5);
	//�o�g���J�n����fight�̊g�嗦�̃T�C�Y�ύX�X�s�[�h
	constexpr float kFightExRateChangeSpeed = 0.3f;
	//�o�g���J�n����fight�̏����A���t�@�l
	constexpr int kFightInitAlpha = 255;
	//�{�^���̉摜�̑傫��
	constexpr int kButtonGraphSize = 56;
	//�V�����_�[�{�^���̉摜�̑傫��
	constexpr int kShoulderButtonGraphSize = 59;
	//�V�����_�[�{�^����UI�ƃt���[����UI�̂���
	constexpr int kShoulderButtonGraphGapX = 60;
	constexpr int kShoulderButtonGraphGapY = 3;
	//�{�^����UI�ƃt���[����UI�̂���
	constexpr int kButtonGraphGapX = 15;
	constexpr int kButtonGraphGapY = 6;
	//������ƃt���[����UI�̂���
	constexpr int kCommandStringGapX = 80;
	constexpr int kCommandStringGapY = 10;
	//�R�X�g�ƃt���[����UI�̂���
	constexpr int kCostStringGapX = 350;
	constexpr int kCostStringGapY = 10;
	//�R�}���h��\������X���W�̊�{�I�Ȉʒu
	constexpr int kCommandPosX = 80;
	//�R�}���h��\������Y���W
	constexpr int kCommandPosY[4] = { 580,645,710,775 };
	//�R�}���h�̖��O��\������Ƃ��̃t�H���g�̑傫��
	constexpr int kCommandFontSize = 48;
	//�R�}���h���؂�ւ��Ƃ��̓�������
	constexpr int kCommandChangeMoveSpeed = 5;
	//�R�}���h���؂�ւ��Ƃ��̃t�F�[�h�̑���
	constexpr int kCommandChangeFadeSpeed = 30;
	//�R���{����\������Ƃ��̃t�H���g�̑傫��
	constexpr int kComboCountFontSize = 64;
	//�R���{����\��������W
	constexpr int kComboCountDrawPosX = 1200;
	constexpr int kComboCountDrawPosY = 400;
	//�R���{�����t�F�[�h�A�E�g������܂ł̎���
	constexpr int kEndComboFadeOutStartTime = 120;
	//�t�F�[�h�A�E�g���鑬��
	constexpr int kComboUiFadeOutSpeed = 15;
	//�R���{���ق߂�R�����g�������Ă��鑬��
	constexpr int kComboPraiseCommentMoveSpeed = 30;
	//�R���{���ق߂�R�����g�̒�ʒu
	constexpr int kComboPraiseCommentPosX = 1280;
	constexpr int kComboPraiseCommentPosY = 330;
	//�R���{���ق߂�R�����g�̏���X���W(��ʊO)
	constexpr int kComboPraiseCommentInitPosX = 2000;
	//�ق߂�R�����g��Good�ɂȂ�R���{��
	constexpr int kGoodComboNum = 10;
	//�ق߂�R�����g��Great�ɂȂ�R���{��
	constexpr int kGreatComboNum = 30;
	//�ق߂�R�����g��Excellent�ɂȂ�R���{��
	constexpr int kExcellentComboNum = 50;
	//�Q�[���N���A����KO�̏����g�嗦
	constexpr float kInitKoExRate = 20.0f;
	//�Q�[���N���A����KO�̊g�k���x
	constexpr float kKoScalingSpeed = 1.0f;
	//�Q�[���N���A����KO�̍ŏI�g�嗦
	constexpr float kFinalKoExRate = 2.0f;
	//�Q�[���N���A���̕����̕\�����W
	constexpr int kGameClearStringPosX = 600;
	constexpr int kGameClearStringPosY = 600;
	//�K�[�h�R�}���h��\��������W
	constexpr int kGuardCommandPosX = 380;
	constexpr int kGuardCommandPosY = 520;
	//SP�R�}���h��\������R�}���h��\��������W
	constexpr int kSpCommandControlPosX = 90;
	constexpr int kSpCommandControlPosY = 520;
	//���~�R�}���h��\��������W
	constexpr int kDownCommandPosX = 300;
	constexpr int kDownCommandPosY = 430;
	//�㏸�R�}���h��\��������W
	constexpr int kUpCommandPosX = 90;
	constexpr int kUpCommandPosY = 430;
	//��t���[���ɑ��₷�p�[�e�B�N���̍ő吔
	constexpr int kAddParticleMaxNum = 20;
	//�p�[�e�B�N���̍ő呬�x
	constexpr int kParticleSpeed = 10;
	//�p�[�e�B�N���̑傫��
	constexpr int kParticleRadius = 2;
	//�p�[�e�B�N���ɂ�����d�̗͂�
	constexpr float kParticleGravity = 0.05f;
	//�p�[�e�B�N���̃��C�t�^�C��
	constexpr int kParticleLifeTime = 300;
	//�Q�[���I�[�o�[���̔w�i�ɕ\�����鍕����`�̃A���t�@�l
	constexpr int kGameOverBackGroundAlpha = 128;
	//HP�o�[��\������F
	const int kHpBarColor = GetColor(64, 255, 64);
	//MP�o�[��\������F
	const int kMpBarColor = GetColor(160, 192, 255);
	//�X�^���o�[��\������F
	const int kStanBarColor = GetColor(192, 32, 192);
	//HP����������������ɂ��Ԃ���o�[�̐F
	const int kHpDecreasedBoxColor = GetColor(64, 64, 64);
	//��
	const int kRedColor = GetColor(255, 0, 0);
	//��
	const int kWhiteColor = GetColor(255,255,255);
	//��
	const int kBlackColor = GetColor(0,0,0);
	//�_���[�W���󂯂�����UI�̐F
	const int kDamageFontColor = GetColor(200, 0, 0);
	//�R���{����\������Ƃ��̐F
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
	m_damageFontHandle = CreateFontToHandle("�A���j�����g���}��", kDamageFontSize, 9, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_gameOverFontHandle = CreateFontToHandle("�A���j�����g���}��", kGameOverFontSize, 9, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_commandFontHandle = CreateFontToHandle("�A���j�����g���}��", kCommandFontSize, 9, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_comboCountFontHandle = CreateFontToHandle("�A���j�����g���}��", kComboCountFontSize, 9, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_hpNumFontHandle = CreateFontToHandle("�A���j�����g���}��", kHpNumFontSize, 9, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
	m_mpNumFontHandle = CreateFontToHandle("�A���j�����g���}��", kMpNumFontSize, 9, DX_FONTTYPE_ANTIALIASING_EDGE_8X8);
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
	//�_���[�W�\�������ׂď���
	for (int i = 0; i < m_showEnemyDamage.size(); i++)
	{
		m_showEnemyDamage.erase(m_showEnemyDamage.begin() + i);
		i--;
	}
	//�R���{�����ׂď���
	m_comboCount = 0;
	m_lastComboCount = 0;
	//�R���{�̃J�E���g���ĊJ����
	m_isCountCombo = true;
	//�r�؂ꂽ���ԃJ�E���g������������
	m_comboEndTime = 0;
	m_comboUiAlpha = 255;
}

void Ui::DrawStateBar(std::shared_ptr<Player> player, std::shared_ptr<Enemy> enemy)
{
	//�_���[�W���󂯂��ۃv���C���[�̃X�e�[�g�o�[��h�炷�傫��
	MyEngine::Vector2 playerStateBarShakeSize(0, 0);
	//�_���[�W���󂯂��ۃG�l�~�[�̃X�e�[�g�o�[��h�炷�傫��
	MyEngine::Vector2 enemyStateBarShakeSize(0, 0);

	//�O�̃t���[�����v���C���[��HP�������Ă�����
	if (m_lastPlayerHp > player->GetNowHp())
	{
		//����������\���Ԃ��o�[�������Ȃ�܂ł̎��Ԃ�ݒ肷��
		m_playerLostHpBarLifeTime = kLostHpBarLifeTime;

		//�v���C���[�̃X�e�[�g�o�[�̍��W��h�炷
		playerStateBarShakeSize = MyEngine::Vector2(GetRand(kShakeScale) - kShakeHalfScale, GetRand(kShakeScale) - kShakeHalfScale);
	}
	//�O�̃t���[�����G�l�~�[��HP�������Ă�����
	if (m_lastEnemyHp > enemy->GetNowHp())
	{
		//����������\���Ԃ��o�[�������Ȃ�܂ł̎��Ԃ�ݒ肷��
		m_enemyLostHpBarLifeTime = kLostHpBarLifeTime;
		//�G�l�~�[�̃X�e�[�g�o�[�̍��W��h�炷
		enemyStateBarShakeSize = MyEngine::Vector2(GetRand(kShakeScale) - kShakeHalfScale, GetRand(kShakeScale) - kShakeHalfScale);
	}

	//�̗͗ʂ��o���Ă���
	m_lastPlayerHp = player->GetNowHp();
	m_lastEnemyHp = enemy->GetNowHp();

	//�摜�̖��O
	std::string playerStateBar = "PlayerStateBar";
	std::string enemyStateBar = "EnemyStateBar";

	//�v���C���[HP�o�[�̊J�n�ʒu
	MyEngine::Vector2 playerHpBarStartPos;
	playerHpBarStartPos.x = m_showUi[playerStateBar].drawPos.x + kHpBarWidth + kHpBarPosX;
	playerHpBarStartPos.y = m_showUi[playerStateBar].drawPos.y + kHpBarHeight + kHpBarPosY;

	//�v���C���[HP�o�[�̏I���ʒu
	MyEngine::Vector2 playerHpBarEndPos;

	playerHpBarEndPos.x = m_showUi[playerStateBar].drawPos.x - kHpBarWidth + kHpBarPosX;
	playerHpBarEndPos.y = m_showUi[playerStateBar].drawPos.y - kHpBarHeight + kHpBarPosY;

	//�v���C���[HP�o�[�̒���
	float playerHpBarLength = playerHpBarEndPos.x - playerHpBarStartPos.x;

	//�v���C���[MP�o�[�̊J�n�ʒu
	MyEngine::Vector2 playerMpBarStartPos;
	playerMpBarStartPos.x = m_showUi[playerStateBar].drawPos.x + kMpBarWidth + kMpBarPosX;
	playerMpBarStartPos.y = m_showUi[playerStateBar].drawPos.y + kMpBarHeight + kMpBarPosY;


	//�v���C���[MP�o�[�̏I���ʒu
	MyEngine::Vector2 playerMpBarEndPos;
	playerMpBarEndPos.x = m_showUi[playerStateBar].drawPos.x - kMpBarWidth + kMpBarPosX;
	playerMpBarEndPos.y = m_showUi[playerStateBar].drawPos.y - kMpBarHeight + kMpBarPosY;

	//�v���C���[MP�o�[�̒���
	float playerMpBarLength = playerMpBarEndPos.x - playerMpBarStartPos.x;

	//�G�l�~�[HP�o�[�̊J�n�ʒu
	MyEngine::Vector2 enemyHpBarStartPos;

	enemyHpBarStartPos.x = m_showUi[enemyStateBar].drawPos.x + kHpBarWidth + kHpBarPosX;
	enemyHpBarStartPos.y = m_showUi[enemyStateBar].drawPos.y + kHpBarHeight + kHpBarPosY;

	//�G�l�~�[HP�o�[�̏I���ʒu
	MyEngine::Vector2 enemyHpBarEndPos;

	enemyHpBarEndPos.x = m_showUi[enemyStateBar].drawPos.x - kHpBarWidth + kHpBarPosX;
	enemyHpBarEndPos.y = m_showUi[enemyStateBar].drawPos.y - kHpBarHeight + kHpBarPosY;

	float enemyHpBarLength = enemyHpBarEndPos.x - enemyHpBarStartPos.x;

	//�G�l�~�[�X�^���o�[�̊J�n�ʒu
	MyEngine::Vector2 enemyStanBarStartPos;

	enemyStanBarStartPos.x = m_showUi[enemyStateBar].drawPos.x + kStanBarWidth + kStanBarPosX;
	enemyStanBarStartPos.y = m_showUi[enemyStateBar].drawPos.y + kStanBarHeight + kStanBarPosY;

	//�G�l�~�[�X�^���o�[�̏I���ʒu
	MyEngine::Vector2 enemyStanBarEndPos;

	enemyStanBarEndPos.x = m_showUi[enemyStateBar].drawPos.x - kStanBarWidth + kStanBarPosX;
	enemyStanBarEndPos.y = m_showUi[enemyStateBar].drawPos.y - kStanBarHeight + kStanBarPosY;

	float enemyStanBarLength = enemyStanBarEndPos.x - enemyStanBarStartPos.x;

	//�v���C���[�̃X�e�[�g�o�[�̍��W��h�炷
	playerHpBarStartPos += playerStateBarShakeSize;
	playerHpBarEndPos += playerStateBarShakeSize;
	playerMpBarStartPos += playerStateBarShakeSize;
	playerMpBarEndPos += playerStateBarShakeSize;

	//�G�l�~�[�̃X�e�[�g�o�[�̍��W��h�炷
	enemyHpBarStartPos += enemyStateBarShakeSize;
	enemyHpBarEndPos += enemyStateBarShakeSize;
	enemyStanBarStartPos += enemyStateBarShakeSize;
	enemyStanBarEndPos += enemyStateBarShakeSize;

	//�v���C���[��HP�o�[�\��
	DrawBox(static_cast<int>(playerHpBarStartPos.x), static_cast<int>(playerHpBarStartPos.y),
		static_cast<int>(playerHpBarEndPos.x), static_cast<int>(playerHpBarEndPos.y),
		kHpBarColor, true);
	//�v���C���[��MP�o�[�\��
	DrawBox(static_cast<int>(playerMpBarStartPos.x), static_cast<int>(playerMpBarStartPos.y),
		static_cast<int>(playerMpBarEndPos.x), static_cast<int>(playerMpBarEndPos.y),
		kMpBarColor, true);
	//�G�l�~�[��HP�o�[�̕\��
	DrawBox(static_cast<int>(enemyHpBarStartPos.x), static_cast<int>(enemyHpBarStartPos.y),
		static_cast<int>(enemyHpBarEndPos.x), static_cast<int>(enemyHpBarEndPos.y),
		kHpBarColor, true);
	//�G�l�~�[�̃X�^���o�[�\��
	DrawBox(static_cast<int>(enemyStanBarStartPos.x), static_cast<int>(enemyStanBarStartPos.y),
		static_cast<int>(enemyStanBarEndPos.x), static_cast<int>(enemyStanBarEndPos.y),
		kStanBarColor, true);


	//�̗͂ɉ����ď�ɂ��Ԃ���{�b�N�X�̒�����ω�������
	int playerLostHpBoxPosX = static_cast<int>(playerHpBarStartPos.x + playerHpBarLength * (player->GetNowHp() / player->GetStatus().hp));
	int enemyLostHpBoxPosX = static_cast<int>(enemyHpBarStartPos.x + enemyHpBarLength * (enemy->GetNowHp() / enemy->GetStatus().hp));

	//�v���C���[�̋C�͂ɉ����ď�ɂ��Ԃ���{�b�N�X�̒�����ω�������
	int playerLostMpBoxPosX = static_cast<int>(playerMpBarStartPos.x + playerMpBarLength * (player->GetNowMp() / player->GetStatus().mp));

	//�G�l�~�[�̃X�^���Q�[�W�ɉ����ď�ɂ��Ԃ���{�b�N�X�̒�����ω�������
	int enemyLostStanBoxPosX = static_cast<int>(enemyStanBarStartPos.x + enemyStanBarLength * enemy->GetStanPointRate());

	//�v���C���[��HP�o�[�̏�ɂ��Ԃ���{�b�N�X�\��
	DrawBox(playerLostHpBoxPosX, static_cast<int>(playerHpBarStartPos.y),
		static_cast<int>(playerHpBarEndPos.x), static_cast<int>(playerHpBarEndPos.y),
		kHpDecreasedBoxColor, true);
	//�G�l�~�[��HP�o�[�̏�ɂ��Ԃ���{�b�N�X�\��
	DrawBox(enemyLostHpBoxPosX, static_cast<int>(enemyHpBarStartPos.y),
		static_cast<int>(enemyHpBarEndPos.x), static_cast<int>(enemyHpBarEndPos.y),
		kHpDecreasedBoxColor, true);

	//�v���C���[��MP�o�[�̏�ɂ��Ԃ���{�b�N�X�\��
	DrawBox(playerLostMpBoxPosX, static_cast<int>(playerMpBarStartPos.y),
		static_cast<int>(playerMpBarEndPos.x), static_cast<int>(playerMpBarEndPos.y),
		kHpDecreasedBoxColor, true);

	//�G�l�~�[�̃X�^���o�[�̏�ɂ��Ԃ���{�b�N�X��\��
	DrawBox(enemyLostStanBoxPosX, static_cast<int>(enemyStanBarStartPos.y),
		static_cast<int>(enemyStanBarEndPos.x), static_cast<int>(enemyStanBarEndPos.y),
		kHpDecreasedBoxColor, true);

	//�Ԃ��o�[�̏�����
	if (m_lastPlayerHpBarEndPosX == 0 && m_lastEnemyHpBarEndPosX == 0)
	{
		m_lastPlayerHpBarEndPosX = playerLostHpBoxPosX;
		m_lastEnemyHpBarEndPosX = enemyLostHpBoxPosX;
	}

	//�Ԃ��o�[��������܂ł̎��Ԃ��J�E���g����
	m_playerLostHpBarLifeTime--;
	m_enemyLostHpBarLifeTime--;

	//�v���C���[�̗̑͂�����������\���Ԃ��o�[��\��
	DrawBox(playerLostHpBoxPosX, static_cast<int>(playerHpBarStartPos.y),
		m_lastPlayerHpBarEndPosX, static_cast<int>(playerHpBarEndPos.y),
		kRedColor, true);
	//�G�l�~�[�̗̑͂�����������\���Ԃ��o�[��\��
	DrawBox(enemyLostHpBoxPosX, static_cast<int>(enemyHpBarStartPos.y),
		m_lastEnemyHpBarEndPosX, static_cast<int>(enemyHpBarEndPos.y),
		kRedColor, true);

	//�Ԃ��Q�[�W���\������āA��莞�ԃ_���[�W���󂯂Ȃ�������
	//�v���C���[���̏���
	if (m_playerLostHpBarLifeTime < 0)
	{
		//�Ԃ��Q�[�W��Z�����Ă���
		m_lastPlayerHpBarEndPosX += kLostHpBarDecreaseSpeed;
		//���݂̗̑͂����Z���Ȃ�����
		if (m_lastPlayerHpBarEndPosX > playerLostHpBoxPosX)
		{
			//���݂̗̑͂̍��W�ɍ��킹��
			m_lastPlayerHpBarEndPosX = playerLostHpBoxPosX;
		}
	}
	//�G�l�~�[���̏���
	if (m_enemyLostHpBarLifeTime < 0)
	{
		//�Ԃ��Q�[�W��Z�����Ă���
		m_lastEnemyHpBarEndPosX += kLostHpBarDecreaseSpeed;
		//���݂̗̑͂����Z���Ȃ�����
		if (m_lastEnemyHpBarEndPosX > enemyLostHpBoxPosX)
		{
			//���݂̗̑͂̍��W�ɍ��킹��
			m_lastEnemyHpBarEndPosX = enemyLostHpBoxPosX;
		}
	}
	//�v���C���[�̃X�e�[�g�o�[�\��
	DrawRotaGraph(static_cast<int>(m_showUi[playerStateBar].drawPos.x + playerStateBarShakeSize.x),
		static_cast<int>(m_showUi[playerStateBar].drawPos.y + playerStateBarShakeSize.y),
		1.0, 0.0, m_showUi[playerStateBar].handle, true);
	//�G�l�~�[�̃X�e�[�g�o�[�\��
	DrawRotaGraph(static_cast<int>(m_showUi[enemyStateBar].drawPos.x + enemyStateBarShakeSize.x),
		static_cast<int>(m_showUi[enemyStateBar].drawPos.y + +enemyStateBarShakeSize.y),
		1.0, 0.0, m_showUi[enemyStateBar].handle, true);
	//�̗͒l�\��
	std::string playerHp = std::to_string(static_cast<int>(player->GetNowHp()));
	std::string enemyHp = std::to_string(static_cast<int>(enemy->GetNowHp()));

	//�v���C���[�̗̑͒l�\��
	DrawStringToHandle(static_cast<int>(m_showUi[playerStateBar].drawPos.x + playerStateBarShakeSize.x + kHpNumPosX),
		static_cast<int>(m_showUi[playerStateBar].drawPos.y + playerStateBarShakeSize.y + kHpNumPosY),
		playerHp.c_str(), kWhiteColor, m_hpNumFontHandle);
	//�G�l�~�[�̗̑͒l�\��
	DrawStringToHandle(static_cast<int>(m_showUi[enemyStateBar].drawPos.x + enemyStateBarShakeSize.x + kHpNumPosX),
		static_cast<int>(m_showUi[enemyStateBar].drawPos.y + enemyStateBarShakeSize.y + kHpNumPosY),
		enemyHp.c_str(), kWhiteColor, m_hpNumFontHandle);
	//�v���C���[�̋C�͒l�\��
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
		//�Q�[���V�[����UI�����[�h����
		UiStatus pushData;
		//�f�[�^������
		pushData.drawPos = MyEngine::Vector2(item.posX, item.posY);
		//�摜�̃��[�h
		pushData.handle = LoadGraph(("data/image/" + item.path + ".png").c_str());
		//�摜�̖��O�Ń}�b�v�ɓo�^
		m_showUi[item.path] = pushData;
	}
}

void Ui::AddShowDamage(MyEngine::Vector3 pos, int damage, bool isPlayer)
{
	DamageShowStatus pushData;
	VECTOR screenPos = ConvWorldPosToScreenPos(pos.CastVECTOR());
	pushData.pos = screenPos;
	//�\�����W���������炷
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
	//�\�����Ԃ��I����Ă�����̂��폜����
	//�G�l�~�[�_���[�W�z��
	for (int i = 0; i < m_showEnemyDamage.size(); i++)
	{
		if (m_showEnemyDamage[i].time < 0)
		{
			m_showEnemyDamage.erase(m_showEnemyDamage.begin() + i);
			i--;
		}
	}

	//�v���C���[�_���[�W�z��
	for (int i = 0; i < m_showPlayerDamage.size(); i++)
	{
		if (m_showPlayerDamage[i].time < 0)
		{
			m_showPlayerDamage.erase(m_showPlayerDamage.begin() + i);
			i--;
		}
	}

	//�`�揈��
	//�G�l�~�[
	for (auto& item : m_showEnemyDamage)
	{
		int alpha = 0;
		//�\������c�莞�Ԃ����ȏ�̎��̓A���t�@�l�����炳�Ȃ�
		if (item.time > kDamageVanishTime)
		{
			alpha = 255;
		}
		//�c�莞�Ԃ����ȉ��ɂȂ�����
		else
		{
			//�c�莞�Ԃ���A���t�@�l���v�Z����
			alpha = static_cast <int>(255 / kDamageVanishTime * item.time);
		}
		//������̒������擾
		int length = GetStringLength(item.damage.c_str());
		//������𒆉����낦�ɂ���
		int shiftSize = static_cast<int>(length * kDamageFontSize * 0.5);
		//�u�����h���[�h��ύX
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		//�_���[�W�̕\��
		DrawFormatStringToHandle(static_cast<int>(item.pos.x - shiftSize), static_cast<int>(item.pos.y + kDamageShowPosShiftY),
			kWhiteColor, m_damageFontHandle, item.damage.c_str());
		//�u�����h���[�h�����ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//�\�����Ԃ����炷
		item.time--;
	}

	//�v���C���[
	for (auto& item : m_showPlayerDamage)
	{
		int alpha = 0;
		//�\������c�莞�Ԃ����ȏ�̎��̓A���t�@�l�����炳�Ȃ�
		if (item.time > kDamageVanishTime)
		{
			alpha = 255;
		}
		//�c�莞�Ԃ����ȉ��ɂȂ�����
		else
		{
			//�c�莞�Ԃ���A���t�@�l���v�Z����
			alpha = static_cast <int>(255 / kDamageVanishTime * item.time);
		}
		//������̒������擾
		int length = GetStringLength(item.damage.c_str());
		//������𒆉����낦�ɂ���
		int shiftSize = static_cast<int>(length * kDamageFontSize * 0.5);
		//�u�����h���[�h��ύX
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, alpha);
		//�_���[�W�̕\��
		DrawFormatStringToHandle(static_cast<int>(item.pos.x - shiftSize), static_cast<int>(item.pos.y + kDamageShowPosShiftY),
			kDamageFontColor, m_damageFontHandle, item.damage.c_str());
		//�u�����h���[�h�����ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		//�\�����Ԃ����炷
		item.time--;
	}
}

void Ui::DrawGameOver(int arrowPos)
{
	//���̃o�g���V�[�����Â�����
	SetDrawBlendMode(DX_BLENDMODE_ALPHA, kGameOverBackGroundAlpha);
	DrawBox(0, 0, Game::kWindowWidth, Game::kWindowHeight, kBlackColor, true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	//�Q�[���I�[�o�[�̉摜�f�[�^
	std::string logo = "GameOverLogo";
	DrawGraph(static_cast<int>(m_showUi[logo].drawPos.x), static_cast<int>(m_showUi[logo].drawPos.y),
		m_showUi[logo].handle, true);

	DrawStringToHandle(kGameOverStringPosX[static_cast<int>(GameOverItem::kRetry)], kGameOverStringPosY[static_cast<int>(GameOverItem::kRetry)], "���g���C", kBlackColor, m_gameOverFontHandle, kWhiteColor);
	DrawStringToHandle(kGameOverStringPosX[static_cast<int>(GameOverItem::kEnd)], kGameOverStringPosY[static_cast<int>(GameOverItem::kEnd)], "��߂�", kBlackColor, m_gameOverFontHandle, kWhiteColor);

	m_shakeArrowNum += kShakeArrowSpeed;

	//���̕\��
	if (arrowPos == 0)
	{
		MyEngine::Vector2 pos(static_cast<int>(kGameOverStringPosX[static_cast<int>(GameOverItem::kRetry)] + sinf(m_shakeArrowNum) * kShakeArrowScale - kArrowDistance), static_cast<int>(kGameOverStringPosY[static_cast<int>(GameOverItem::kRetry)]));
		DrawStringToHandle(static_cast<int>(pos.x), static_cast<int>(pos.y), "��", kBlackColor, m_gameOverFontHandle, kWhiteColor);
	}
	else
	{
		MyEngine::Vector2 pos(static_cast<int>(kGameOverStringPosX[static_cast<int>(GameOverItem::kEnd)] + sinf(m_shakeArrowNum) * kShakeArrowScale - kArrowDistance), static_cast<int>(kGameOverStringPosY[static_cast<int>(GameOverItem::kEnd)]));
		DrawStringToHandle(static_cast<int>(pos.x), static_cast<int>(pos.y), "��", kBlackColor, m_gameOverFontHandle, kWhiteColor);
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
			//�E��ɍs���悤�Ƀx�N�g���쐬
			addParticle.vec = MyEngine::Vector2(1, -1);
		}
		else if (posKind == static_cast<int>(ParticlePos::kLeftUp))
		{
			addParticle.pos.x = 0;
			addParticle.pos.y = 0;
			//�E���ɍs���悤�Ƀx�N�g���쐬
			addParticle.vec = MyEngine::Vector2(1, 1);
		}
		else if (posKind == static_cast<int>(ParticlePos::kRightUp))
		{
			addParticle.pos.x = Game::kWindowWidth;
			addParticle.pos.y = 0;
			//�����ɍs���悤�Ƀx�N�g���쐬
			addParticle.vec = MyEngine::Vector2(-1, 1);
		}
		else if (posKind == static_cast<int>(ParticlePos::kRightDown))
		{
			addParticle.pos.x = Game::kWindowWidth;
			addParticle.pos.y = Game::kWindowHeight;
			//����ɍs���悤�Ƀx�N�g���쐬
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
		//�p�[�e�B�N����`�悷��
		DrawCircle(static_cast<int>(item.pos.x), static_cast<int>(item.pos.y), kParticleRadius, item.color, true);
		//���W�̍X�V
		item.pos += item.vec;
		//�x�N�g�����������������ɂ��Ă���
		item.vec.y += kParticleGravity;
		//���C�t�^�C�������炵�Ă���
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

	//KO�̊g�嗦�𒲐�����
	if (m_koExRate > kFinalKoExRate)
	{
		m_koExRate -= kKoScalingSpeed;
	}
	else
	{
		m_koExRate = kFinalKoExRate;
	}
	//�Q�[���N���A�̉摜�f�[�^
	std::string logo = "Ko";
	DrawRotaGraph(static_cast<int>(m_showUi[logo].drawPos.x), static_cast<int>(m_showUi[logo].drawPos.y),
		m_koExRate, 0.0, m_showUi[logo].handle, true);

	DrawStringToHandle(kGameClearStringPosX, kGameClearStringPosY, "Press A Button", kWhiteColor, m_gameOverFontHandle);;

}

void Ui::DrawStartSign(bool startFlag)
{
	//Ready�̉摜�f�[�^�̖��O
	std::string ready = "Ready";
	//Fight�̉摜�f�[�^�̖��O
	std::string fight = "Fight";
	//�܂��o�g�����n�܂��Ă��Ȃ��ꍇ
	if (!startFlag)
	{
		//Ready�ƕ\������
		DrawRotaGraph(static_cast<int>(m_showUi[ready].drawPos.x), static_cast<int>(m_showUi[ready].drawPos.y), kReadyExRate, 0, m_showUi[ready].handle, true);
	}
	//�o�g�����J�n�����ꍇ
	else
	{
		//fight�̕\�����W
		MyEngine::Vector2 drawPos(m_showUi[fight].drawPos.x, m_showUi[fight].drawPos.y);

		//�\�����ĉ��t���[�������������v�����Ă���
		m_showFightTime++;
		//fight�̊g�嗦�𒲐�����
		if (m_isFightChangeExRate)
		{
			//��{�I�ɏk�����Ă���
			m_fightExRate -= kFightExRateChangeSpeed;
			//�T�C�Y�����l�����������Ȃ�����
			if (m_fightExRate < kFightFinalExRate)
			{
				//�T�C�Y�����l�ɂ���
				m_fightExRate = kFightFinalExRate;
				//�k�����~�߂�
				m_isFightChangeExRate = false;
			}
		}
		//�g�嗦�̒������I������ꍇ
		else
		{
			//�\�����Ă����莞�Ԃ����Ă�����
			if (m_showFightTime > kFightShowTime)
			{
				//fight�̃A���t�@�l�������Ă���
				m_fightAlpha -= kFightFadeOutSpeed;
			}
			//fight�������h�炷
			if (m_fightShakeTime < kFightShakeTime)
			{
				//�\�����W�����炷
				drawPos.x += GetRand(kFightShakeScale) - kFightShakeScaleHalf;
				drawPos.y += GetRand(kFightShakeScale) - kFightShakeScaleHalf;
				//�h�炵�����Ԃ��v������
				m_fightShakeTime++;
			}

		}

		//fight�̃A���t�@�l��0�ȏゾ������`�悷��
		if (m_fightAlpha > 0)
		{
			//�A���t�@�l��ݒ肷��
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fightAlpha);
			//Fight�ƕ\������
			DrawRotaGraph(static_cast<int>(drawPos.x), static_cast<int>(drawPos.y), m_fightExRate, 0, m_showUi[fight].handle, true);
			//�u�����h���[�h�����ɖ߂�
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
		}

	}
}

void Ui::DrawCommand(bool showSpecialAttack, std::map<std::string, SpecialAttackCommandInfo> attackInfo)
{
	//�R�}���h��\������t���[��
	std::string frame = "CommandFrame";
	//�{�^���̉摜
	std::string button = "ButtonABXY";
	//�R���g���[���[�̏㑤�ɂ���{�^���摜
	std::string shoulderButton = "ButtonShoulder";

	//�K�E�Z�p���b�g���J���Ă��Ȃ��Ƃ�
	if (!showSpecialAttack)
	{
		//��{�R�}���h�̃A���t�@�l��������
		if (m_normalCommandAlpha < 255)
		{
			//���l�܂ŃA���t�@�l��������
			m_normalCommandAlpha += kCommandChangeFadeSpeed;
		}
		else
		{
			//��������ő�l�ɂ���
			m_normalCommandAlpha = 255;
		}
		//�K�E�Z�R�}���h�̃A���t�@�l��������
		if (m_specialCommandAlpha > 0)
		{
			//0�܂ŉ�����
			m_specialCommandAlpha -= kCommandChangeFadeSpeed;
		}
		else
		{
			//0�����������Ȃ�Ȃ��悤�ɂ���
			m_specialCommandAlpha = 0;
		}

		//��{�R�}���h�̍��W���E�ɓ�����
		if (m_normalCommandPosX < kCommandPosX)
		{
			//���̈ʒu�܂ňړ�������
			m_normalCommandPosX += kCommandChangeMoveSpeed;
		}
		else
		{
			//����������̈ʒu�ɖ߂�
			m_normalCommandPosX = kCommandPosX;
		}
		//�K�E�Z�R�}���h�̍��W�����ɓ�����
		if (m_specialCommandPosX > 0)
		{
			//0�܂ňړ�������
			m_specialCommandPosX -= kCommandChangeMoveSpeed;
		}
		else
		{
			//0�������������Ȃ�
			m_specialCommandPosX = 0;
		}

	}
	//�K�E�Z�p���b�g���J���Ă���Ƃ�
	else
	{
		//�K�E�Z�R�}���h�̃A���t�@�l��������
		if (m_specialCommandAlpha < 255)
		{
			//���l�܂ŃA���t�@�l��������
			m_specialCommandAlpha += kCommandChangeFadeSpeed;
		}
		else
		{
			//��������ő�l�ɂ���
			m_specialCommandAlpha = 255;
		}
		//��{�R�}���h�̃A���t�@�l��������
		if (m_normalCommandAlpha > 0)
		{
			//0�܂ŉ�����
			m_normalCommandAlpha -= kCommandChangeFadeSpeed;
		}
		else
		{
			//0�����������Ȃ�Ȃ��悤�ɂ���
			m_normalCommandAlpha = 0;
		}


		//�K�E�Z�R�}���h�̍��W���E�ɓ�����
		if (m_specialCommandPosX < kCommandPosX)
		{
			//���̈ʒu�܂ňړ�������
			m_specialCommandPosX += kCommandChangeMoveSpeed;
		}
		else
		{
			//����������̈ʒu�ɖ߂�
			m_specialCommandPosX = kCommandPosX;
		}
		//��{�R�}���h�̍��W�����ɓ�����
		if (m_normalCommandPosX > 0)
		{
			//0�܂ňړ�������
			m_normalCommandPosX -= kCommandChangeMoveSpeed;
		}
		else
		{
			//0�������������Ȃ�
			m_normalCommandPosX = 0;
		}
	}


	//��{�R�}���h�̕\��

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_normalCommandAlpha);
	//�t���[����\������
	DrawGraph(m_normalCommandPosX, kCommandPosY[static_cast<int>(CommandSort::kPhysical)],
		m_showUi[frame].handle, true);
	DrawGraph(m_normalCommandPosX, kCommandPosY[static_cast<int>(CommandSort::kEnergy)],
		m_showUi[frame].handle, true);
	DrawGraph(m_normalCommandPosX, kCommandPosY[static_cast<int>(CommandSort::kDodge)],
		m_showUi[frame].handle, true);
	DrawGraph(m_normalCommandPosX, kCommandPosY[static_cast<int>(CommandSort::kCharge)],
		m_showUi[frame].handle, true);

	//�e����ɑΉ������{�^����\������
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

	//�e����̖��O��\������
	DrawStringToHandle(m_normalCommandPosX + kCommandStringGapX, kCommandPosY[static_cast<int>(CommandSort::kPhysical)] + kCommandStringGapY,
		"���񂹂܂ق�", kBlackColor, m_commandFontHandle,kWhiteColor);
	DrawStringToHandle(m_normalCommandPosX + kCommandStringGapX, kCommandPosY[static_cast<int>(CommandSort::kEnergy)] + kCommandStringGapY,
		"�܂���", kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawStringToHandle(m_normalCommandPosX + kCommandStringGapX, kCommandPosY[static_cast<int>(CommandSort::kDodge)] + kCommandStringGapY,
		"������/�_�b�V��", kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawStringToHandle(m_normalCommandPosX + kCommandStringGapX, kCommandPosY[static_cast<int>(CommandSort::kCharge)] + kCommandStringGapY,
		"�`���[�W", kBlackColor, m_commandFontHandle, kWhiteColor);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�K�E�Z�R�}���h�̕\��

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_specialCommandAlpha);
	//�t���[����\������
	DrawGraph(m_specialCommandPosX, kCommandPosY[static_cast<int>(CommandSort::kSpecialA)],
		m_showUi[frame].handle, true);
	DrawGraph(m_specialCommandPosX, kCommandPosY[static_cast<int>(CommandSort::kSpecialB)],
		m_showUi[frame].handle, true);
	DrawGraph(m_specialCommandPosX, kCommandPosY[static_cast<int>(CommandSort::kSpecialX)],
		m_showUi[frame].handle, true);
	DrawGraph(m_specialCommandPosX, kCommandPosY[static_cast<int>(CommandSort::kSpecialY)],
		m_showUi[frame].handle, true);

	//�e����ɑΉ������{�^����\������
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

	//�e����̖��O��\������

	DrawStringToHandle(m_specialCommandPosX + kCommandStringGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialA)] + kCommandStringGapY,
		attackInfo[Game::InputId::kA].name.c_str(), kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawStringToHandle(m_specialCommandPosX + kCommandStringGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialB)] + kCommandStringGapY,
		attackInfo[Game::InputId::kB].name.c_str(), kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawStringToHandle(m_specialCommandPosX + kCommandStringGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialX)] + kCommandStringGapY,
		attackInfo[Game::InputId::kX].name.c_str(), kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawStringToHandle(m_specialCommandPosX + kCommandStringGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialY)] + kCommandStringGapY,
		attackInfo[Game::InputId::kY].name.c_str(), kBlackColor, m_commandFontHandle, kWhiteColor);

	//�e����̃R�X�g��\������
	DrawStringToHandle(m_specialCommandPosX + kCostStringGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialA)] + kCostStringGapY,
		std::to_string(attackInfo[Game::InputId::kA].cost).c_str(), kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawStringToHandle(m_specialCommandPosX + kCostStringGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialB)] + kCostStringGapY,
		std::to_string(attackInfo[Game::InputId::kB].cost).c_str(), kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawStringToHandle(m_specialCommandPosX + kCostStringGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialX)] + kCostStringGapY,
		std::to_string(attackInfo[Game::InputId::kX].cost).c_str(), kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawStringToHandle(m_specialCommandPosX + kCostStringGapX, kCommandPosY[static_cast<int>(CommandSort::kSpecialY)] + kCostStringGapY,
		std::to_string(attackInfo[Game::InputId::kY].cost).c_str(), kBlackColor, m_commandFontHandle, kWhiteColor);

	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);

	//�K�[�h�R�}���h��\������
	DrawRectGraph(kGuardCommandPosX, kGuardCommandPosY,
		kShoulderButtonGraphSize * static_cast<int>(ButtonSort::kRb),
		0, kShoulderButtonGraphSize, kShoulderButtonGraphSize,
		m_showUi[shoulderButton].handle, true);
	DrawStringToHandle(kGuardCommandPosX + kShoulderButtonGraphGapX, kGuardCommandPosY + kShoulderButtonGraphGapY,
		"�K�[�h", kBlackColor, m_commandFontHandle, kWhiteColor);
	//�K�E�Z�R�}���h��\������R�}���h��\������
	DrawRectGraph(kSpCommandControlPosX, kSpCommandControlPosY,
		kShoulderButtonGraphSize * static_cast<int>(ButtonSort::kLb),
		0, kShoulderButtonGraphSize, kShoulderButtonGraphSize,
		m_showUi[shoulderButton].handle, true);
	DrawStringToHandle(kSpCommandControlPosX + kShoulderButtonGraphGapX, kSpCommandControlPosY + kShoulderButtonGraphGapY,
		"Sp�R�}���h", kBlackColor, m_commandFontHandle, kWhiteColor);
	//�㉺�ړ��R�}���h��\������R�}���h��\������
	DrawRectGraph(kUpCommandPosX, kUpCommandPosY,
		kShoulderButtonGraphSize * static_cast<int>(ButtonSort::kLt),
		0, kShoulderButtonGraphSize, kShoulderButtonGraphSize,
		m_showUi[shoulderButton].handle, true);
	DrawStringToHandle(kUpCommandPosX + kShoulderButtonGraphGapX, kUpCommandPosY + kShoulderButtonGraphGapY,
		"�A�b�v", kBlackColor, m_commandFontHandle, kWhiteColor);
	DrawRectGraph(kDownCommandPosX, kDownCommandPosY,
		kShoulderButtonGraphSize * static_cast<int>(ButtonSort::kRt),
		0, kShoulderButtonGraphSize, kShoulderButtonGraphSize,
		m_showUi[shoulderButton].handle, true);
	DrawStringToHandle(kDownCommandPosX + kShoulderButtonGraphGapX, kDownCommandPosY + kShoulderButtonGraphGapY,
		"�_�E��", kBlackColor, m_commandFontHandle, kWhiteColor);
}

void Ui::DrawComboCount()
{
	//�R���{���Ă���Ε\������(�R���{��2�񂩂�J�E���g)
	if (m_comboCount > 1)
	{
		//��������������ɂ��邽�߂̏���
		std::string comboString = std::to_string(m_comboCount);
		//�����̕��������擾�����̕����ɍ��W���炷(�����͔��p�Ȃ̂Ńt�H���g�T�C�Y��0.5��������)
		int length = static_cast<int>(GetStringLength(comboString.c_str()) * kComboCountFontSize * 0.5);

		//�R���{����\������
		DrawFormatStringToHandle(kComboCountDrawPosX - length, kComboCountDrawPosY, kComboFontColor, m_comboCountFontHandle, "%dCOMBO", m_comboCount);
		//�\�������R���{����ۑ�����
		m_lastComboCount = m_comboCount;
		//�A���t�@�l���ő�l�ɂ���
		m_comboUiAlpha = 255;
		//�R���{���ق߂�R�����g�������ʒu�ɖ߂�
		m_comboPraiseCommentPosX = kComboPraiseCommentInitPosX;
	}
	//�R���{���r�؂�Ă�����
	if (m_comboCount == 0 && m_lastComboCount > 1)
	{
		//��莞�ԃR���{�𐔂��Ȃ��悤�ɂ���
		m_isCountCombo = false;
	}
	//�R���{�𐔂��Ȃ����Ԃ̊�
	if (!m_isCountCombo)
	{
		//�R���{���r�؂�Ă���̎��Ԃ�ۑ�����
		m_comboEndTime++;

		//�\������R�����g
		std::string praiseComment = "empty";

		//�R���{���ق߂�R�����g��ݒ肷��
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
		//�R���{���ق߂�R�����g�̍��W�𒲐�����
		if (m_comboPraiseCommentPosX > kComboPraiseCommentPosX)
		{
			m_comboPraiseCommentPosX -= kComboPraiseCommentMoveSpeed;
		}
		//�R���{���r�؂�Ă����莞�Ԃ�������
		if (m_comboEndTime > kEndComboFadeOutStartTime)
		{
			//�R���{���ƃR�����g���t�F�[�h�A�E�g�����Ă���
			m_comboUiAlpha -= kComboUiFadeOutSpeed;
			//���S�Ƀt�F�[�h�A�E�g������
			if (m_comboUiAlpha < 0)
			{
				//�O�̃t���[���̃R���{�������ɖ߂�
				m_lastComboCount = 0;
				//�R���{�̃J�E���g���ĊJ����
				m_isCountCombo = true;
				//�r�؂ꂽ���ԃJ�E���g������������
				m_comboEndTime = 0;
			}
		}
		//�A���t�@�l�̐ݒ������
		SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_comboUiAlpha);
		//��������������ɂ��邽�߂̏���
		std::string comboString = std::to_string(m_lastComboCount);
		//�����̕��������擾�����̕����ɍ��W���炷(�����͔��p�Ȃ̂Ńt�H���g�T�C�Y��0.5��������)
		int length = static_cast<int>(GetStringLength(comboString.c_str()) * kComboCountFontSize * 0.5);
		//�ł����R���{����\����������
		DrawFormatStringToHandle(kComboCountDrawPosX - length, kComboCountDrawPosY, kComboFontColor, m_comboCountFontHandle, "%dCOMBO", m_lastComboCount);
		//�R�����g��\������
		DrawRotaGraph(m_comboPraiseCommentPosX, kComboPraiseCommentPosY, 1, 0, m_showUi[praiseComment].handle, true);
		//�u�����h���[�h�����ɖ߂�
		SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
	}

}
