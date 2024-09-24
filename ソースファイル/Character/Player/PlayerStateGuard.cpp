#include "PlayerStateGuard.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHitAttack.h"
#include "Player.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"
namespace
{
	//�_���[�W�̌y����
	constexpr float kDamageCutRate = 0.2f;
}
void PlayerStateGuard::Init()
{
	MyEngine::Vector3 pos = m_pPlayer->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Guard"), pos, true);
	EffekseerManager::GetInstance().Entry(effect);
	m_pPlayer->SetEffectData(effect);
	m_pPlayer->ChangeAnim("Guard");
}
void PlayerStateGuard::Update(MyEngine::Input input)
{
	//�G�̕����������悤�ɂ���
	m_pPlayer->SetModelFront(m_pPlayer->GetTargetPos());
	//�ړ����Ȃ��悤�ɂ���
	m_pPlayer->SetVelo(MyEngine::Vector3(0,0,0));
	//�A�j���[�V�����𓮂���
	m_pPlayer->PlayAnim();
	//�G�t�F�N�g�̈ʒu�����킹��
	auto effect = m_pPlayer->GetEffectData();
	effect->SetPos(m_pPlayer->GetPos());

	//��Ԃ̕ω�
	if (input.IsPress(Game::InputId::kRb))
	{
		m_nextState = shared_from_this();
		return;
	}
	//�K�[�h���͂�����Ă��Ȃ����
	else
	{
		//��Ԃ�ω�������
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		m_pPlayer->EndEffect();
		return;
	}
}

int PlayerStateGuard::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����J�b�g���ĕԂ�
	damage = attack->GetDamage() * kDamageCutRate;
	m_pPlayer->PlaySE("Guard", DX_PLAYTYPE_BACK);

	return damage;
}
