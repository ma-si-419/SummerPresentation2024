#pragma once
#include "EnemyStateBase.h"
class EnemyStateDodge : public EnemyStateBase
{
public:
	EnemyStateDodge(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) : EnemyStateBase(enemy, scene) {}
	//�ǂ̍s�������邩�����߂�
	void Init();
	//�X�V����
	virtual void Update() override;
	//�_���[�W���󂯂����̏������s��
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
private:
	//�ړ��x�N�g��
	MyEngine::Vector3 m_velo;
};