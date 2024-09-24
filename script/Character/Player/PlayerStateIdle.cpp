#include "PlayerStateIdle.h"
#include "PlayerStateMove.h"
#include "PlayerStateGuard.h"
#include "PlayerStateDodge.h"
#include "PlayerStateAttack.h"
#include "PlayerStateHitAttack.h"
#include "PlayerStateCharge.h"
#include "Player.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"

namespace
{
	//�g���K�[����������l
	constexpr int kTriggerReaction = 200;
}

void PlayerStateIdle::Init()
{
	m_pPlayer->ChangeAnim("Idle");
}

void PlayerStateIdle::Update(MyEngine::Input input)
{
	//�ړ����͂�����Ă�����
	if (input.GetStickInfo().leftStickX != 0 || input.GetStickInfo().leftStickY != 0 ||
		input.GetTriggerInfo().left > kTriggerReaction || input.GetTriggerInfo().right > kTriggerReaction)
	{
		//State��Move�ɕύX����
		m_nextState = std::make_shared<PlayerStateMove>(m_pPlayer,m_pScene);
		auto state = std::dynamic_pointer_cast<PlayerStateMove>(m_nextState);
		state->Init();
		return;
	}
	//�K�E�Z�p���b�g���J���Ă��Ȃ��ꍇ
	if (!input.IsPress(Game::InputId::kLb))
	{
		//�C�e�U���������ꍇ
		if (input.IsTrigger(Game::InputId::kX))
		{
			//State��NormalEnergyAttack�ɕύX����
			m_nextState =  std::make_shared<PlayerStateAttack>(m_pPlayer,m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
			state->Init(Game::InputId::kX,false);
			return;
		}
		//�i���U���������ꍇ
		if (input.IsTrigger(Game::InputId::kB))
		{
			//State��NormalPhysicalAttack�ɕύX����
			m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
			state->Init(Game::InputId::kB, false);
			return;
		}
		//����s���̓��͂����ꂽ��
		if (input.IsTrigger(Game::InputId::kA))
		{
			//State��Dodge�ɕύX����
			m_nextState =  std::make_shared<PlayerStateDodge>(m_pPlayer,m_pScene);
			//����̕�����ݒ肷��
			auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
			MyEngine::Vector3 dir(0,0,1);
			state->Init(dir);
			return;
		}
		//�K�[�h���͂�����Ă�����
		if (input.IsPress(Game::InputId::kRb))
		{
			//State��Guard�ɕύX����
			m_nextState =  std::make_shared<PlayerStateGuard>(m_pPlayer,m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateGuard>(m_nextState);
			state->Init();
			return;
		}
		//�`���[�W���͂���Ă�����
		if (input.IsPress(Game::InputId::kY))
		{
			//State��Charge�ɕύX����
			m_nextState = std::make_shared<PlayerStateCharge>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateCharge>(m_nextState);
			state->Init();
			return;
		}
	}
	//�K�E�Z�p���b�g���J���Ă���ꍇ
	else
	{
		std::map<std::string, std::string> setSpecialAttack = m_pPlayer->GetSetSpecialAttackId();
		if (input.IsTrigger(Game::InputId::kY))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kY]))
			{
				//��Ԃ�ω�������
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kY,true);
				return;
			}
		}
		else if (input.IsTrigger(Game::InputId::kB))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kB]))
			{
				//��Ԃ�ω�������
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kB, true);
				return;
			}
		}
		else if (input.IsTrigger(Game::InputId::kX))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kX]))
			{
				//��Ԃ�ω�������
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kX, true);
				return;
			}
		}
		else if (input.IsTrigger(Game::InputId::kA))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kA]))
			{
				//��Ԃ�ω�������
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kA, true);
				return;
			}
		}
	}

	m_pPlayer->SetVelo(MyEngine::Vector3(0,0,0));
	m_pPlayer->SetModelFront(m_pPlayer->GetTargetPos());
	m_pPlayer->PlayAnim();
	//��L�̓��͂�����Ă��Ȃ�������State��ύX���Ȃ�
	m_nextState =  shared_from_this();
}

int PlayerStateIdle::OnDamage(std::shared_ptr<Collidable> collider)
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
