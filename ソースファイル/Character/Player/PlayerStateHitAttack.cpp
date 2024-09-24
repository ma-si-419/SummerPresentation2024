#include "PlayerStateHitAttack.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"

namespace
{
	//�y���U�����󂯂��Ƃ��̃m�b�N�o�b�N�̑���
	constexpr float kLightKnockBackSpeed = 0.3f;
	//������΂��U�����󂯂����̃m�b�N�o�b�N�̑���
	constexpr float kBurstKnockBackSpeed = 1.2f;
	//�y���U�����󂯂����̓����Ȃ�����
	constexpr int kLightHitStunTime = 5;
	//������΂��U�����󂯂����̓����Ȃ�����
	constexpr int kBurstHitStunTime = 30;
	//�X�^���U�����󂯂����̓����Ȃ�����
	constexpr int kStunHitStunTime = 40;
}
void PlayerStateHitAttack::Init(std::shared_ptr<Collidable> collider)
{
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	m_burstDir = (m_pPlayer->GetPos() - attack->GetPos()).Normalize();
	
	if (attack->GetHitEffect() == static_cast<int>(HitEffect::kLightHit))
	{
		m_hitEffect = HitEffect::kLightHit;
		m_pPlayer->ChangeAnim("LightHit");
		m_pPlayer->PlaySE("LightHit", DX_PLAYTYPE_BACK);
	}
	else if (attack->GetHitEffect() == static_cast<int>(HitEffect::kBurst))
	{
		m_hitEffect = HitEffect::kBurst;
		m_pPlayer->ChangeAnim("Burst");
	}
	else if (attack->GetHitEffect() == static_cast<int>(HitEffect::kStun))
	{
		m_hitEffect = HitEffect::kStun;
		m_pPlayer->ChangeAnim("Stun");
	}
}
void PlayerStateHitAttack::Update(MyEngine::Input input)
{
	//�o�ߎ��Ԃ��v������
	m_time++;
	//�ړ��x�N�g��
	MyEngine::Vector3 velo;

	//�y���U�����󂯂���
	if (m_hitEffect == HitEffect::kLightHit)
	{
		velo = m_burstDir * kLightKnockBackSpeed;
		//�y���U�����󂯂����̃X�^�����Ԃ��o�߂�����
		if (m_time > kLightHitStunTime)
		{
			//�A�C�h����Ԃɖ߂�
			m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer,m_pScene);
			return;
		}
	}
	//������΂��U�����󂯂���
	else if (m_hitEffect == HitEffect::kBurst)
	{
		velo = m_burstDir * kBurstKnockBackSpeed;
		//������΂��U�����󂯂����̃X�^�����Ԃ��o�߂�����
		if (m_time > kBurstHitStunTime)
		{
			//�A�C�h����Ԃɖ߂�
			m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
			return;
		}
	}
	//�X�^���U�����󂯂���
	else
	{
		velo = MyEngine::Vector3(0, 0, 0);
		//�X�^���U�����󂯂����̃X�^�����Ԃ��o�߂�����
		if (m_time > kStunHitStunTime)
		{
			//�A�C�h����Ԃɖ߂�
			m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
			return;
		}
	}
	m_pPlayer->PlayAnim();
	m_pPlayer->SetVelo(velo);
	//���݂̏�Ԃ�Ԃ�
	m_nextState = shared_from_this();
}

int PlayerStateHitAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage();
	//��Ԃ�ω�������
	m_nextState = std::make_shared<PlayerStateHitAttack>(m_pPlayer,m_pScene);
	//�󂯂��U���̎�ނ�ݒ肷��
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->Init(collider);
	//�q�b�g�G�t�F�N�g��\������
	MyEngine::Vector3 pos = m_pPlayer->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Hit"), pos, false);
	EffekseerManager::GetInstance().Entry(effect);
	

	return damage;
}