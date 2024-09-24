#include "PlayerStateMove.h"
#include "PlayerStateIdle.h"
#include "PlayerStateGuard.h"
#include "PlayerStateDodge.h"
#include "PlayerStateHitAttack.h"
#include "PlayerStateCharge.h"
#include "PlayerStateAttack.h"
#include "Player.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"


namespace
{
	//�ړ����x
	constexpr float kMoveSpeed = 1.0f;
	//�G�Ƃ̈�ԋ߂�����(�^��ɂ����Ȃ��悤��)
	constexpr int kShortestEnemyDistance = 15;
	//�_�b�V������Ƃ��̈ړ����x�̔{��
	constexpr float kDashSpeedRate = 1.8f;
	//�g���K�[����������l
	constexpr int kTriggerReaction = 200;
}

void PlayerStateMove::Init()
{
	m_pPlayer->ChangeAnim("MoveFront");
}

void PlayerStateMove::Update(MyEngine::Input input)
{
	//State�̕ω�
	//�K�E�Z�p���b�g���J���Ă��Ȃ��ꍇ
	if (!input.IsPress(Game::InputId::kLb))
	{
		//�C�e�U���������ꍇ
		if (input.IsTrigger(Game::InputId::kX))
		{
			//�ꉞ�G�t�F�N�g�������Ă���
			m_pPlayer->EndEffect();
			//��Ԃ�ω�������
			m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
			state->Init(Game::InputId::kX, false);
			return;
		}
		//�i���U���������ꍇ
		if (input.IsTrigger(Game::InputId::kB))
		{
			//�ꉞ�G�t�F�N�g�������Ă���
			m_pPlayer->EndEffect();
			//��Ԃ�ω�������
			m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
			state->Init(Game::InputId::kB, false);
			return;
		}
		//�K�[�h���͂�����Ă�����
		if (input.IsPress(Game::InputId::kRb))
		{
			//�ꉞ�G�t�F�N�g�������Ă���
			m_pPlayer->EndEffect();
			//State��Guard�ɕύX����
			m_nextState = std::make_shared<PlayerStateGuard>(m_pPlayer, m_pScene);
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
				//�ꉞ�G�t�F�N�g�������Ă���
				m_pPlayer->EndEffect();
				//��Ԃ�ω�������
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kY, true);
				return;
			}
		}
		else if (input.IsTrigger(Game::InputId::kB))
		{
			//MP���\���ɂ�������
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kB]))
			{
				//�ꉞ�G�t�F�N�g�������Ă���
				m_pPlayer->EndEffect();
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
				//�ꉞ�G�t�F�N�g�������Ă���
				m_pPlayer->EndEffect();
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
				//�ꉞ�G�t�F�N�g�������Ă���
				m_pPlayer->EndEffect();
				//��Ԃ�ω�������
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kA, true);
				return;
			}
		}
	}

	//��L�̏�ԑJ�ڂɓ��Ă͂܂�Ȃ�������
	//���̈ړ��������s��

	//�ړ��x�N�g��
	MyEngine::Vector3 velo;

	MyEngine::Input::StickInfo stick = input.GetStickInfo();

	MyEngine::Vector3 dir(stick.leftStickX, 0, -stick.leftStickY);

	//�ړ����͂�����Ă���Ƃ�
	if (dir.sqLength() != 0)
	{
		dir = dir.Normalize();
	}

	//�G�l�~�[�̍��W
	MyEngine::Vector3 targetPos = m_pPlayer->GetTargetPos();
	//Y���𒆐S�Ƃ�����]������̂�
	//Y���W���֌W���Ȃ��悤�Ƀv���C���[�Ɠ������W�ɂ���
	MyEngine::Vector3 rotationShaftPos = targetPos;
	rotationShaftPos.y = m_pPlayer->GetPos().y;

	//�v���C���[�����]�̒��S�ւ̃x�N�g��
	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_pPlayer->GetPos();

	//�_�b�V���{�^����������Ă��邩
	bool isDash = input.IsPress(Game::InputId::kA);

	//�ړ����x
	float speed = kMoveSpeed;

	//�_�b�V���{�^����������Ă����王��p���L����
	m_pPlayer->SetUpFov(isDash);

	//�_�b�V���{�^����������Ă�����
	if (isDash)
	{
		speed *= kDashSpeedRate;

		if (!m_pPlayer->GetEffectData())
		{
			MyEngine::Vector3 pos = m_pPlayer->GetPos();
			std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Dash"), pos, true);
			EffekseerManager::GetInstance().Entry(effect);
			m_pPlayer->SetEffectData(effect);
		}

		auto effect = m_pPlayer->GetEffectData();
		effect->SetPos(m_pPlayer->GetPos());
	}
	else
	{
		m_pPlayer->EndEffect();
	}
	//����̉�]�x���擾����
	float x = m_pPlayer->GetPos().x - rotationShaftPos.x;
	float z = m_pPlayer->GetPos().z - rotationShaftPos.z;

	float angle = std::atan2(z, x);

	//��]�̑傫��
	float hMoveScale = 0;

	//���Ɍ��������W
	MyEngine::Vector3 nextPos;

	//���ړ�
	if (dir.x != 0.0f)
	{
		//�����ɂ���ĉ�]����傫����ω�������
		hMoveScale = (dir.x * kMoveSpeed) / toShaftPosVec.Length();
		//�_�b�V���{�^���������Ă����牡�ړ��̑����Ƀ_�b�V���̔{����������
		if (isDash)
		{
			hMoveScale *= kDashSpeedRate;
		}
	}

	//�G�ɋ߂������ꍇ
	if (toShaftPosVec.Length() < kShortestEnemyDistance)
	{
		//�O���͂���Ă���ꍇ
		if (dir.z > 0)
		{
			//�����͂̒l�ŉ����������߂�
			if (dir.x > 0)
			{
				//�O���͂ŉ�]����
				hMoveScale += (dir.z * speed) / toShaftPosVec.Length();
			}
			else
			{
				//�O���͂ŉ�]����
				hMoveScale -= (dir.z * speed) / toShaftPosVec.Length();
			}
		}
		//�����͂���Ă���ꍇ
		else
		{
			nextPos += nextPos + toShaftPosVec.Normalize() * dir.z * speed;
		}
	}
	//�G�����苗������Ă���ꍇ
	else
	{
		nextPos += nextPos + toShaftPosVec.Normalize() * dir.z * speed;
	}

	//�㉺�ړ�
	if (input.GetTriggerInfo().left > kTriggerReaction)
	{
		nextPos.y += kMoveSpeed;
	}
	else if (input.GetTriggerInfo().right > kTriggerReaction)
	{
		nextPos.y -= kMoveSpeed;
	}


	//���݂̊p�x�ɉ��ړ��̑傫���𑫂�
	angle += hMoveScale;

	nextPos.x += cosf(angle) * toShaftPosVec.Length() + rotationShaftPos.x;
	nextPos.y += m_pPlayer->GetPos().y;
	nextPos.z += sinf(angle) * toShaftPosVec.Length() + rotationShaftPos.z;



	velo = nextPos - m_pPlayer->GetPos();

	//�K�E�Z�p���b�g���J����Ă��Ȃ�
	if (!input.IsPress(Game::InputId::kLb))
	{
		//����s���̓��͂����ꂽ��
		if (input.IsTrigger(Game::InputId::kA))
		{
			//�ꉞ�G�t�F�N�g�������Ă���
			m_pPlayer->EndEffect();
			//State��Dodge�ɕύX����
			m_nextState = std::make_shared<PlayerStateDodge>(m_pPlayer, m_pScene);
			//����̕�����ݒ肷��
			auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
			state->Init(dir);
			return;
		}
	}

	m_pPlayer->PlayAnim();
	m_pPlayer->SetVelo(velo);

	//���ʂ̍��W
	MyEngine::Vector3 frontPos = m_pPlayer->GetPos() + velo;
	//�㉺�ړ����s���ۂ͓G�̕��������Ȃ��瓮���悤�ɂ���
	if (frontPos.y != m_pPlayer->GetPos().y)
	{
		frontPos = m_pPlayer->GetTargetPos();
	}



	//�ړ����ĂȂ����
	if (velo.sqLength() < 0.001f)
	{
		//�^�[�Q�b�g�̍��W�������悤�ɂ���
		m_pPlayer->SetModelFront(m_pPlayer->GetTargetPos());
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		return;
	}
	//�ړ����Ă����
	else
	{
		//�ړ���̍��W�������悤�ɂ���
		m_pPlayer->SetModelFront(frontPos);
		//�����̃|�C���^��Ԃ�
		m_nextState = shared_from_this();
		return;
	}


}

int PlayerStateMove::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage();
	//��Ԃ�ω�������
	m_nextState = std::make_shared<PlayerStateHitAttack>(m_pPlayer, m_pScene);
	//�󂯂��U���̎�ނ�ݒ肷��
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->Init(collider);
	//�q�b�g�G�t�F�N�g��\������
	m_pPlayer->EndEffect();
	MyEngine::Vector3 pos = m_pPlayer->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Hit"), pos, false);
	EffekseerManager::GetInstance().Entry(effect);

	return damage;
}