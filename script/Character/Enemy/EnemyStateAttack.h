#pragma once
#include "EnemyStateBase.h"

class Player;
class EnemyStateAttack : public EnemyStateBase
{
public:
	EnemyStateAttack(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) : EnemyStateBase(enemy,scene) {}
	//�o���Z�����肷��
	void Init(std::shared_ptr<Player> player);
	//�X�V����
	virtual void Update() override;
	//�_���[�W���󂯂����̏������s��
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
private:
	//����State�ɓ����Ă���̌o�ߎ���
	int m_time = 0;
	//�U�����o���n�߂Ă���̌o�ߎ���
	int m_attackTime = 0;

	std::string m_attackId = "empty";

	enum class AttackKind
	{
		kRepeatedlyEnergy,
		kLaser,
		kBreakAttack,
		kStanAttack,
		kAttackKindNum
	};

	//�v���C���[�ɋ߂Â����ǂ���
	bool m_isNearPlayer = false;
	//�U�����o���Ă��邩�t���O
	bool m_isStartAttack = false;
	//�U�����I��������ǂ����t���O
	bool m_isAttackEnd = false;
	//�v���C���[�̍��W���擾���邽�߂Ƀ|�C���^�[������
	std::shared_ptr<Player> m_pPlayer;
	//�U�����o������
	int m_popAttackNum = 0;
	//���[�U�[���o���U���̍��W
	MyEngine::Vector3 m_laserTargetPos;
	//�U����̍d�����Ԃł��邩�ǂ���
	bool m_isAttackEndStanAnim = false;
};

