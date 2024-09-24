#pragma once
#include "EnemyStateBase.h"
class EnemyStateIdle : public EnemyStateBase
{
public:
	EnemyStateIdle(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) : EnemyStateBase(enemy, scene) {}
	//����������
	void Init();
	//�X�V����
	virtual void Update() override;
	//�_���[�W���󂯂����̏������s��
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
};