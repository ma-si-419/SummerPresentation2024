#include "PlayerStateDodge.h"
#include "PlayerStateIdle.h"
#include "Player.h"
#include <cmath>
#include "EffekseerManager.h"

namespace
{
	constexpr float kDodgeSpeed = 2.0f;
	//����̎���
	constexpr int kDodgeTime = 10;
	//�G�Ƃ̍ŒZ����
	constexpr int kShortestEnemyDistance = 15;
}
void PlayerStateDodge::Init(MyEngine::Vector3 dir)
{
	m_moveDir = dir;
	m_pPlayer->PlaySE("Dodge",DX_PLAYTYPE_BACK);
	m_pPlayer->ChangeAnim("Move");
	MyEngine::Vector3 pos = m_pPlayer->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Dodge"), pos, false);
	EffekseerManager::GetInstance().Entry(effect);
	m_pPlayer->SetEffectData(effect);
	m_pPlayer->SetUpFov(true);
}
void PlayerStateDodge::Update(MyEngine::Input input)
{
	//����p���L����
	m_pPlayer->SetUpFov(true);
	//�G�l�~�[�̍��W
	MyEngine::Vector3 targetPos = m_pPlayer->GetTargetPos();
	//Y���𒆐S�Ƃ�����]������̂�
	//Y���W���֌W���Ȃ��悤�Ƀv���C���[�Ɠ������W�ɂ���
	MyEngine::Vector3 rotationShaftPos = targetPos;
	rotationShaftPos.y = m_pPlayer->GetPos().y;

	//�v���C���[�����]�̒��S�ւ̃x�N�g��
	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_pPlayer->GetPos();

	MyEngine::Vector3 nextPos;

	//��]�̑傫��
	float hMoveScale = 0;

	//����̉�]�x���擾����
	float x = m_pPlayer->GetPos().x - rotationShaftPos.x;
	float z = m_pPlayer->GetPos().z - rotationShaftPos.z;

	float angle = std::atan2(z, x);

	if (m_moveDir.x != 0.0f)
	{
		hMoveScale = (m_moveDir.x * kDodgeSpeed) / toShaftPosVec.Length();
	}

	//�G�ɋ߂���������͂����悤�ɂ���
	if (toShaftPosVec.Length() < kShortestEnemyDistance)
	{
		//�O���͂���Ă���ꍇ
		if (m_moveDir.z > 0)
		{
			//�����͂̒l�ŉ����������߂�
			if (m_moveDir.x > 0)
			{
				//�O���͂ŉ�]����
				hMoveScale += (m_moveDir.z * kDodgeSpeed) / toShaftPosVec.Length();
			}
			else
			{
				//�O���͂ŉ�]����
				hMoveScale -= (m_moveDir.z * kDodgeSpeed) / toShaftPosVec.Length();
			}
		}
		//�����͂���Ă���ꍇ
		else
		{
			nextPos = nextPos + toShaftPosVec.Normalize() * m_moveDir.z * kDodgeSpeed;
		}
	}
	//�G�����苗������Ă����ꍇ
	else
	{
		nextPos = nextPos + toShaftPosVec.Normalize() * m_moveDir.z * kDodgeSpeed;
	}

	//���݂̊p�x�ɉ��ړ��̑傫���𑫂�
	angle += hMoveScale;

	nextPos.x += cosf(angle) * toShaftPosVec.Length() + rotationShaftPos.x;
	nextPos.y += m_pPlayer->GetPos().y;
	nextPos.z += sinf(angle) * toShaftPosVec.Length() + rotationShaftPos.z;

	m_pPlayer->SetVelo(nextPos - m_pPlayer->GetPos());

	m_time++;

	//����̎��Ԃ�����
	if (m_time < kDodgeTime)
	{
		//�����̃|�C���^��Ԃ�
		m_nextState = shared_from_this();
	}
	//����̎��Ԃ��I�������
	else
	{
		//�A�C�h����Ԃɖ߂�
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer,m_pScene);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		m_pPlayer->EndEffect();
	}
	return;

}

int PlayerStateDodge::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�U���ɓ�����Ȃ��悤�ɂ���
	return 0;
}
