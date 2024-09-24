#include "EnemyStateHitAttack.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"

namespace
{
	//�y���U�����󂯂��Ƃ��̃m�b�N�o�b�N�̑���
	constexpr float kLightKnockBackSpeed = 0.3f;
	//������΂��U�����󂯂����̃m�b�N�o�b�N�̑���
	constexpr float kBurstKnockBackSpeed = 1.8f;
	//�K�[�h�������̃m�b�N�o�b�N�̑���
	constexpr float kGuardKnockBackSpeed = 0.2f;
	//�y���U�����󂯂����̓����Ȃ�����
	constexpr int kLightHitStunTime = 8;
	//������΂��U�����󂯂����̓����Ȃ�����
	constexpr int kBurstHitStunTime = 90;
	//�X�^���U�����󂯂����̓����Ȃ�����
	constexpr int kStunHitStunTime = 40;
	//�K�[�h�������̓����Ȃ�����
	constexpr int kGuardStunTime = 3;

}

void EnemyStateHitAttack::Init(int effect)
{
	if (effect == static_cast<int>(HitEffect::kBurst))
	{
		m_effect = HitEffect::kBurst;
		m_pEnemy->ChangeAnim("Burst");
	}
	else if (effect == static_cast<int>(HitEffect::kLightHit))
	{
		m_effect = HitEffect::kLightHit;
		m_pEnemy->ChangeAnim("LightHit");
		m_pEnemy->PlaySE("LightHit", DX_PLAYTYPE_BACK);
	}
	else if (effect == static_cast<int>(HitEffect::kStun))
	{
		m_effect = HitEffect::kStun;
		m_pEnemy->ChangeAnim("Stun");
	}
	else if (effect == static_cast<int>(HitEffect::kGuard))
	{
		m_effect = HitEffect::kGuard;
		m_pEnemy->PlaySE("Guard", DX_PLAYTYPE_BACK);
	}

	m_burstDir = (m_pEnemy->GetPos() - m_pEnemy->GetTargetPos()).Normalize();

}

void EnemyStateHitAttack::Update()
{
	//�o�ߎ��Ԃ��v������
	m_time++;
	//�ړ��x�N�g��
	MyEngine::Vector3 velo;

	//�y���U�����󂯂���
	if (m_effect == HitEffect::kLightHit)
	{
		velo = m_burstDir * kLightKnockBackSpeed;
		//�y���U�����󂯂����̃X�^�����Ԃ��o�߂�����
		if (m_time > kLightHitStunTime)
		{
			//��Ԃ�ω�������
			m_isChangeState = true;
		}
	}
	//������΂��U�����󂯂���
	else if (m_effect == HitEffect::kBurst)
	{
		velo = m_burstDir * kBurstKnockBackSpeed;
		//������΂��U�����󂯂����̃X�^�����Ԃ��o�߂�����
		if (m_time > kBurstHitStunTime)
		{
			//��Ԃ�ω�������
			m_isChangeState = true;
		}
	}
	//�X�^���U�����󂯂���
	else if (m_effect == HitEffect::kStun)
	{
		velo = MyEngine::Vector3(0, 0, 0);
		//�X�^���U�����󂯂����̃X�^�����Ԃ��o�߂�����
		if (m_time > kStunHitStunTime)
		{
			//��Ԃ�ω�������
			m_isChangeState = true;
		}
	}
	//�K�[�h���Ă�����
	else
	{
		velo = m_burstDir * kGuardKnockBackSpeed;
		//�K�[�h�������̃X�^�����Ԃ��o�߂�����
		if (m_time > kGuardStunTime)
		{
			//��Ԃ�ω�������
			m_isChangeState = true;
		}
	}
	m_pEnemy->PlayAnim();
	m_pEnemy->SetVelo(velo);
}

int EnemyStateHitAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage() - GetRand(static_cast<int>(m_pEnemy->GetStatus().def));
	//�󂯂��U���̎�ނ�ݒ肷��
	m_hitEffect = attack->GetHitEffect();
	MyEngine::Vector3 pos = m_pEnemy->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Hit"), pos, false);
	EffekseerManager::GetInstance().Entry(effect);
	return damage;
}
