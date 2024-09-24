#pragma once
#include "SceneBase.h"
#include <memory>
#include <vector>
class Physics;
class Player;
class GameCamera;
class Enemy;
class AttackBase;
class Ui;
class SceneGame : public SceneBase, public std::enable_shared_from_this<SceneGame>
{
public:
	SceneGame(SceneManager& sceneManager, DataManager& dataManager, SoundManager& soundManager);
	~SceneGame();
	//����������
	void Init();
	//���g���C���̏���������
	void RetryInit();
	//�X�V����
	void Update(MyEngine::Input input);
	//�`�揈��
	void Draw();
	//�I������
	void End();
	//�U�����V�[���ɒǉ�
	void AddAttack(std::shared_ptr<AttackBase> pAttack);
	//�Q�[���I�[�o�[���ɌĂяo��
	void GameOver() { m_isGameOver = true; }
	//���ʉ����Đ�����
	void PlaySE(std::string name, int playType);
	//���ʉ����~�߂�
	void StopSE(std::string name) { m_soundManager.Stop(name); }

	//�T�E���h�̃n���h�����擾����
	int GetSEHandle(std::string name) { return m_soundManager.GetHandle(name); }

private:
	std::shared_ptr<Physics> m_pPhysics;
	std::shared_ptr<Player> m_pPlayer;
	std::shared_ptr<GameCamera> m_pGameCamera;
	std::shared_ptr<Enemy> m_pEnemy;
	std::shared_ptr<Ui> m_pUi;
	std::vector<std::shared_ptr<AttackBase>> m_pAttacks;

#ifdef _DEBUG

	//�X�e�[�W�̃��f���n���h��
	int m_handle;

#endif // !_DEBUG

	//�Q�[���I�[�o�[���ǂ���
	bool m_isGameOver;
	//�Q�[���N���A���ǂ���
	bool m_isGameClear;
	//�Q�[���I�[�o�[���̑I�����Ă���A�C�e��
	int m_gameOverSelectItemNum;
	//�o�g�����J�n���Ă��邩�ǂ���
	bool m_isStartBattle;
	//�V�[���ɗ��Ă���̎��Ԃ��v��
	int m_time;
	//�o�g���I�Ղ��ǂ���
	bool m_isFinalStage;
	//�Q�[���N���A�̉���炵�����ǂ���
	bool m_isClearSe;
	bool m_isClearBgm;
};

