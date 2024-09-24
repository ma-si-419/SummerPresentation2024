#pragma once
#include "Enemy.h"
#include "SceneGame.h"
#include "Player.h"

class EnemyStateBase
{
public:
	enum class HitEffectKind
	{
		kLightAttack,
		kStan,
		kBurst,
		kGuard
	};

	enum class EnemyStateKind
	{
		kIdle,
		kMove,
		kDash,
		kDodge,
		kAttack,
		kGuard,
		kCharge,
		kHitAttack
	};
	EnemyStateBase(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) { m_pEnemy = enemy; m_pScene = scene; }
	//�󋵂�c����������X�V
	void CheckSituation(std::shared_ptr<Player> player);
	// State���ꂼ��̏������s���A����State�ɕς��^�C�~���O��true��Ԃ�	
	virtual void Update() abstract;
	//�_���[�W���󂯂����̏������s��
	virtual int OnDamage(std::shared_ptr<Collidable> collider) abstract;
	//���ڍs����State��ۑ�����
	std::shared_ptr<EnemyStateBase> m_nextState;
	//�G�l�~�[�̃|�C���^�[�������Ă���
	std::shared_ptr<Enemy> m_pEnemy;
	//�Q�[���V�[���̃|�C���^�[�������Ă���
	std::shared_ptr<SceneGame> m_pScene;
	//���ǂ�State�ɍs�����̊m���̃f�[�^
	std::map<std::string, std::vector<int>> m_aiData;
	//State�̐؂�ւ����s��flag
	bool m_isChangeState = false;
	//�ǂ�ȃ_���[�W���󂯂���
	int m_hitEffect = -1;
	//State���؂�ւ���Ă��牽�t���[�����������v������
	int m_time = 0;
	//�v���C���[�̕����������ǂ���
	bool m_isLookPlayer = true;
};

