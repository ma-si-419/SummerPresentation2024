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
	/// �_���[�W��\������
	/// </summary>
	void DrawDamage();

	/// <summary>
	/// �Q�[���I�[�o�[����UI��\������
	/// </summary>
	/// <param name="arrowPos">�ǂ̍��ڂ�I�����Ă��邩</param>
	void DrawGameOver(int arrowPos);

	void DrawGameClear();

	/// <summary>
	/// �o�g���J�n�O�̍��}��\������
	/// </summary>
	/// <param name="startFlag">�o�g�����J�n�������ǂ���
	void DrawStartSign(bool startFlag);

	/// <summary>
	/// �U���֌W�̑����\������
	/// </summary>
	/// <param name="showSpecialAttack">�K�E�Z�p���b�g���J���Ă��邩�ǂ���</param>
	void DrawCommand(bool showSpecialAttack,std::map<std::string, SpecialAttackCommandInfo> attackInfo);

	/// <summary>
	/// �R���{����\������
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
		//�摜�n���h��
		int handle = -1;
		//�\�����W
		MyEngine::Vector2 drawPos;
	};
	//�G�l�~�[�ɗ^�����_���[�W��\������Ƃ��ɕK�v�ȍ\����
	struct DamageShowStatus
	{
		//�\������_���[�W
		std::string damage = "empty";
		//�\������
		float time = 0;
		//�\�����W
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

	//�O�̃t���[���\�����Ă���Hp�o�[�̏�Ԃ��c���Ă���
	int m_lastPlayerHpBarEndPosX;
	int m_lastEnemyHpBarEndPosX;

	//�O�̃t���[����HP
	float m_lastPlayerHp;
	float m_lastEnemyHp;

	//HP���Ȃ��Ȃ��Ă����܂ł̎��ԃJ�E���g
	int m_playerLostHpBarLifeTime;
	int m_enemyLostHpBarLifeTime;

	//fight�̊g�嗦
	float m_fightExRate;
	//fight�̃A���t�@�l
	int m_fightAlpha;
	//fight�Ɖ��t���[���\��������
	int m_showFightTime;
	//���t���[��fight��h�炵����
	int m_fightShakeTime;
	//fight�̏k�����s���Ă��邩�ǂ���
	bool m_isFightChangeExRate;

	//Ko�̊g�嗦
	float m_koExRate;


	//��{�R�}���hUI��X���W
	int m_normalCommandPosX;
	//��{�R�}���hUI�̃A���t�@�l
	int m_normalCommandAlpha;
	//�K�E�Z�R�}���h��X���W
	int m_specialCommandPosX;
	//�K�E�Z�R�}���hUI�̃A���t�@�l
	int m_specialCommandAlpha;

	//�R���{��
	int m_comboCount;
	//�O�̃t���[���\�����Ă����t���[����
	int m_lastComboCount;
	//�R���{��\�����镶����alpha�l
	int m_comboUiAlpha;
	//�R���{���J�E���g����^�C�~���O���ǂ���
	bool m_isCountCombo;

	//�̗͂�\������Ƃ��̃t�H���g�n���h��
	int m_hpNumFontHandle;
	//���͂�\������Ƃ��̃t�H���g�n���h��
	int m_mpNumFontHandle;
	//�_���[�W��\������Ƃ��̃t�H���g�n���h��
	int m_damageFontHandle;
	//�Q�[���I�[�o�[�̎��̑I�����̃t�H���g�n���h��
	int m_gameOverFontHandle;
	//�R�}���h��\������Ƃ��̃t�H���g�n���h��
	int m_commandFontHandle;
	//�R���{��\������Ƃ��̃t�H���g�n���h��
	int m_comboCountFontHandle;
	//�R���{���r�؂�Ă���̃t���[����
	int m_comboEndTime;
	//�R���{���ق߂�R�����g��X���W
	int m_comboPraiseCommentPosX;

	//�\������_���[�W�̔z��
	std::vector<DamageShowStatus> m_showEnemyDamage;
	std::vector<DamageShowStatus> m_showPlayerDamage;
	//�I�����Ă�����𓮂����ۂɎg�p����ϐ�
	float m_shakeArrowNum;
	//�\�����Ă���Ui�̃f�[�^
	std::map<std::string, UiStatus> m_showUi;
	//�N���A���ɕ\������p�[�e�B�N���̔z��
	std::vector<Particle> m_particles;
};