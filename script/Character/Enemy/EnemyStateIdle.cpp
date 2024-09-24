#include "EnemyStateIdle.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"

namespace
{
	//�Œች�t���[������State�ł��邩
	constexpr int kShortestTime = 30;
}

void EnemyStateIdle::Init()
{
	m_pEnemy->ChangeAnim("Idle");
}

void EnemyStateIdle::Update()
{
	//���Ԃ��v������
	m_time++;

	//Idle��Ԃ͉������Ȃ�
	m_pEnemy->SetVelo(MyEngine::Vector3(0,0,0));

	m_pEnemy->PlayAnim();


	//���̃t���[���ɂ���Œ᎞�Ԃ𒴂�����m���ŕʂ̃t���[���ɍs��
	int random = GetRand(m_time) - kShortestTime;

	if (random > 0)
	{
		m_isChangeState = true;
	}
}

int EnemyStateIdle::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage() - GetRand(static_cast<int>(m_pEnemy->GetStatus().def));
	//�󂯂��U���̎�ނ�ݒ肷��
	m_hitEffect = attack->GetHitEffect();
	m_isChangeState = true;
	MyEngine::Vector3 pos = m_pEnemy->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Hit"), pos, false);
	EffekseerManager::GetInstance().Entry(effect);
	return damage;
}
