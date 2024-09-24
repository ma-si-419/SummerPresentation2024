#pragma once
#include "EnemyStateBase.h"
class EnemyStateHitAttack : public EnemyStateBase
{
public:
	enum class HitEffect
	{
		kLightHit,
		kStun,
		kBurst,
		kGuard
	};
	EnemyStateHitAttack(std::shared_ptr<Enemy> enemy, std::shared_ptr<SceneGame> scene) : EnemyStateBase(enemy, scene) {}
	
	//����������
	void Init(int effect);
	
	//�X�V����
	virtual void Update() override;
	//�_���[�W���󂯂����̏������s��
	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;
private:
	//�o�ߎ���
	int m_time = 0;
	//�󂯂��U���̎��
	HitEffect m_effect = HitEffect::kLightHit;
	
	//�m�b�N�o�b�N�̕���
	MyEngine::Vector3 m_burstDir;



};

