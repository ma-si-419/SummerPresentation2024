#include "EnemyStateCharge.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"

namespace
{
	//�Œች�t���[������State�ł��邩
	constexpr int kShortestTime = 120;
}

void EnemyStateCharge::Init()
{
	//�G�t�F�N�g�̐ݒ�
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Chrage"),m_pEnemy->GetPos(),true);
	EffekseerManager::GetInstance().Entry(effect);
	m_pEnemy->SetEffectData(effect);

	//�A�j���[�V�����̐ݒ�
	m_pEnemy->ChangeAnim("Charge");
}

void EnemyStateCharge::Update()
{
	//���Ԃ��v������
	m_time++;

	//Charge��Ԃ͒��g�I�ɂ͉������Ȃ�
	m_pEnemy->SetVelo(MyEngine::Vector3(0, 0, 0));

	//�A�j���[�V�����̍X�V
	m_pEnemy->PlayAnim();

	//���̃t���[���ɂ���Œ᎞�Ԃ𒴂�����m���ŕʂ̃t���[���ɍs��
	int random = GetRand(m_time) - kShortestTime;

	if (random > 0)
	{
		m_isChangeState = true;
		m_pEnemy->EndEffect();
	}
}

int EnemyStateCharge::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage() - GetRand(static_cast<int>(m_pEnemy->GetStatus().def));
	//�󂯂��U���̎�ނ�ݒ肷��
	m_hitEffect = attack->GetHitEffect();
	m_pEnemy->EndEffect();
	m_isChangeState = true;
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Hit"), m_pEnemy->GetPos(), false);
	EffekseerManager::GetInstance().Entry(effect);
	m_pEnemy->SetEffectData(effect);
	return damage;
}
