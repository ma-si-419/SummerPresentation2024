#include "PlayerStateAttack.h"
#include "PlayerStateIdle.h"
#include "PlayerStateHitAttack.h"
#include "PlayerStateDodge.h"
#include "Player.h"
#include "SceneGame.h"
#include "CommandIdList.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"


namespace
{
	//�C�e���o���Ă���Ƃ��̈ړ����x
	constexpr float kEnergyAttackMoveSpeed = 0.8f;
	//�U�����o���Ă���Ƃ��̈ړ����x
	constexpr float kPhysicalAttackMoveSpeed = 3;
	//�G���߂��ɂ��锻��ɂȂ鋗��
	constexpr float kNearEnemyLength = 15;
	//�G�ɋ߂Â����Ԃ̍ő�
	constexpr int kGoEnemyTime = 60;
	//�g���K�[����������l
	constexpr int kTriggerReaction = 200;
}

void PlayerStateAttack::Init(std::string button, bool isSpecial)
{
	//�K�E�Z�̏ꍇ
	if (isSpecial)
	{
		auto attackDataId = m_pPlayer->GetSetSpecialAttackId();

		m_attackId = attackDataId[button];
		m_isNormalAttack = false;
		m_isGoTarget = !m_pPlayer->GetAttackData(m_attackId).isEnergy;
	}
	//�ʏ�U���̏ꍇ
	else
	{
		//�C�e�U��
		if (button == Game::InputId::kX)
		{
			m_attackId = CommandId::kEnergyAttack1;
			m_isGoTarget = false;
		}
		//�i���U��
		else if (button == Game::InputId::kB)
		{
			m_attackId = CommandId::kPhysicalAttack1;
			m_isGoTarget = true;
		}
		m_isNormalAttack = true;
	}
	//�A�j���[�V������ύX
	m_pPlayer->ChangeAnim(m_pPlayer->GetAttackData(m_attackId).animationName);
	//�v���C���[��MP�����炷
	m_pPlayer->SubMp(static_cast<float>(m_pPlayer->GetAttackData(m_attackId).cost));
}

void PlayerStateAttack::Update(MyEngine::Input input)
{
	DataManager::AttackInfo attackData = m_pPlayer->GetAttackData(m_attackId);
	//�o�ߎ��Ԃ��v������
	m_time++;
	//�ړ��x�N�g��
	MyEngine::Vector3 velo;
	//�X�e�B�b�N����
	MyEngine::Input::StickInfo stick = input.GetStickInfo();
	//�ړ�����
	MyEngine::Vector3 dir(stick.leftStickX, 0, -stick.leftStickY);

	//�C�e�U�����ǂ���
	bool isNormalEnergy =
		m_attackId == CommandId::kEnergyAttack1 ||
		m_attackId == CommandId::kEnergyAttack2 ||
		m_attackId == CommandId::kEnergyAttack3 ||
		m_attackId == CommandId::kEnergyAttack4;

	//�C�e�U���̂ݏo���Ȃ���ړ��ł���
	if (isNormalEnergy)
	{
		//�ړ����͂�����Ă���Ƃ�
		if (dir.sqLength() != 0)
		{
			dir = dir.Normalize();

			//�G�l�~�[�̍��W
			MyEngine::Vector3 targetPos = m_pPlayer->GetTargetPos();
			//Y���𒆐S�Ƃ�����]������̂�
			//Y���W���֌W���Ȃ��悤�Ƀv���C���[�Ɠ������W�ɂ���
			MyEngine::Vector3 rotationShaftPos = targetPos;
			rotationShaftPos.y = m_pPlayer->GetPos().y;

			//�v���C���[�����]�̒��S�ւ̃x�N�g��
			MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_pPlayer->GetPos();

			//�ړ����x
			float speed = kEnergyAttackMoveSpeed;

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
				hMoveScale = (dir.x * kEnergyAttackMoveSpeed) / toShaftPosVec.Length();
			}

			//�G�ɋ߂������ꍇ
			if (toShaftPosVec.Length() < kNearEnemyLength)
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
					//�G�l�~�[���痣��Ă����x�N�g��
					nextPos = nextPos + toShaftPosVec.Normalize() * dir.z * speed;
				}
			}
			//�G�����苗������Ă���ꍇ
			else
			{
				nextPos = nextPos + toShaftPosVec.Normalize() * dir.z * speed;
			}

			//���݂̊p�x�ɉ��ړ��̑傫���𑫂�
			angle += hMoveScale;

			//�㉺�ړ�
			if (input.GetTriggerInfo().left > kTriggerReaction)
			{
				nextPos.y += kEnergyAttackMoveSpeed;
			}
			else if (input.GetTriggerInfo().right > kTriggerReaction)
			{
				nextPos.y -= kEnergyAttackMoveSpeed;
			}


			nextPos.x += cosf(angle) * toShaftPosVec.Length() + rotationShaftPos.x;
			nextPos.y += m_pPlayer->GetPos().y;
			nextPos.z += sinf(angle) * toShaftPosVec.Length() + rotationShaftPos.z;

			velo = nextPos - m_pPlayer->GetPos();

			//�C�e�U�����s���Ă���Ƃ��͉���s���ɂ����Ɉڍs�ł���
			if (!input.IsPress(Game::InputId::kLb))
			{
				//����s���̓��͂����ꂽ��
				if (input.IsTrigger(Game::InputId::kA))
				{
					//State��Dodge�ɕύX����
					m_nextState = std::make_shared<PlayerStateDodge>(m_pPlayer, m_pScene);
					//����̕�����ݒ肷��
					auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
					state->Init(dir);
					return;
				}
			}
		}
		//�ړ����͂�����Ă��Ȃ��Ƃ���
		if (!input.IsPress(Game::InputId::kLb))
		{
			//����s���̓��͂����ꂽ��
			if (input.IsTrigger(Game::InputId::kA))
			{
				//State��Dodge�ɕύX����
				m_nextState = std::make_shared<PlayerStateDodge>(m_pPlayer, m_pScene);
				//����̕�����G�Ɍ������Ă��������ɕύX����
				auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
				MyEngine::Vector3 dodgeDir = (m_pPlayer->GetTargetPos() - m_pPlayer->GetPos()).Normalize();
				state->Init(dodgeDir);
				return;
			}
		}

	}
	//�i���U���������瑊��Ɍ������Ă���
	if (m_isGoTarget)
	{
		//�ړ��x�N�g���̐���
		MyEngine::Vector3 moveVec = m_pPlayer->GetTargetPos() - m_pPlayer->GetPos();
		velo = moveVec.Normalize() * kPhysicalAttackMoveSpeed;
	}
	//�G���߂��ɂ��邩�ǂ����𒲂ׂ�
	float length = (m_pPlayer->GetTargetPos() - m_pPlayer->GetPos()).Length();
	//�G���߂��ɂ��邩�A�o�ߎ��Ԃ���莞�Ԃ𒴂�����
	if (length <= kNearEnemyLength || m_time > kGoEnemyTime && !attackData.isEnergy)
	{
		//�G�Ɍ������Ă����̂���߂�
		m_isGoTarget = false;
		//�ړ������Ȃ�����
		velo = MyEngine::Vector3(0, 0, 0);
	}
	//�G�Ɍ������Ă����������I����Ă�����
	if (!m_isGoTarget)
	{
		//�U���̏��������鎞�Ԃ��J�E���g����
		m_actionTime++;
	}
	//�U�����I����Ă��Ȃ����
	if (!m_isAttackEnd)
	{
		//�U�����P���ȏꍇ
		if (attackData.attackNum == 1)
		{
			//�U�����o�����ԂɂȂ�����
			if (m_actionTime > attackData.attackStartTime)
			{
				std::shared_ptr<AttackBase> attack = m_pPlayer->CreateAttack(m_attackId,MyEngine::Vector3(0,0,0));
				//�i���U����������
				if (!attackData.isEnergy)
				{
					//�G�t�F�N�g���c���悤�ɐݒ������
					attack->SetLeaveEffect();
				}
				//�U�����o��
				m_pScene->AddAttack(attack);
				//�U�����I�������t���O�𗧂Ă�
				m_isAttackEnd = true;
				//�A�j���[�V�������[�v���~�߂�
				m_pPlayer->StopAnimLoop();
			}
		}
		//�U����������̏ꍇ
		else
		{
			//�U���̃X�p�����擾����
			int span = (attackData.attackEndTime - attackData.attackStartTime) / attackData.attackNum;
			//�U���̃^�C�~���O��������U�����o���悤�ɂ���
			if (m_actionTime % span == 0 && m_actionTime > attackData.attackStartTime)
			{
				//���[�U�[���o�����W���w�肳��Ă��Ȃ�������
				if (m_laserTargetPos.sqLength() == 0)
				{
					m_laserTargetPos = m_pPlayer->GetTargetPos();
				}
				//�U�����쐬
				std::shared_ptr<AttackBase> attack = m_pPlayer->CreateAttack(m_attackId,m_laserTargetPos);
				//���[�U�[��̍U���̐ݒ�
				if (attackData.isLaser)
				{
					//������܂ł̎���
					int lifeTime = attackData.lifeTime - m_actionTime;

					attack->SetAttackTime(lifeTime);
					//�G�t�F�N�g���c���悤�ɂ���
					attack->SetLeaveEffect();
					//��x�ڂ̍U���ȊO�G�t�F�N�g���o���Ȃ��悤�ɂ���
					if (m_popAttackNum > 0)
					{
						attack->SetNotPopEffect();
					}

					m_popAttackNum++;
				}
				//�U�����o��
				m_pScene->AddAttack(attack);
			}
			//�U�����o���^�C�~���O���I��������
			if (m_actionTime > attackData.attackEndTime)
			{
				m_isAttackEnd = true;
				m_pPlayer->StopAnimLoop();
			}
		}
	}
	//�U�����I��������
	else
	{
		//�ʏ�U�����o������̏���
		if (m_isNormalAttack)
		{
			//���̍U�������͂���Ă�����
			if (m_nextAttackId != "empty")
			{
				//�U���̃R�X�g��MP�������
				if (m_pPlayer->GetAttackCost(m_nextAttackId) < m_pPlayer->GetNowMp())
				{
					//MP�����炷
					m_pPlayer->SubMp(static_cast<float>(m_pPlayer->GetAttackCost(m_nextAttackId)));
					//���̍U���Ɉڍs����
					m_attackId = m_nextAttackId;
					//�U���̏�������������
					m_actionTime = 0;
					m_time = 0;
					m_isAttackEnd = false;
					auto attack = m_pPlayer->GetAttackData(m_nextAttackId);
					m_isNormalAttack = !attack.isSpecial;
					m_isGoTarget = !attack.isEnergy;
					m_pPlayer->ChangeAnim(attack.animationName);
					m_nextAttackId = "empty";
				}
				//�R�X�g������Ȃ����
				else
				{
					//�U���̑S�̃t���[�����I����Ă���Idle��Ԃɖ߂�
					if (m_actionTime > attackData.actionTotalTime)
					{
						m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
						auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
						state->Init();
						return;
					}
				}
			}
			//���̍U�������͂���Ă��Ȃ����
			else
			{
				//�U���̑S�̃t���[�����I����Ă���Idle��Ԃɖ߂�
				if (m_actionTime > attackData.actionTotalTime)
				{
					m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
					auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
					state->Init();
					return;
				}
			}
		}
		//�K�E�Z���o������̏���
		else
		{
			//�U���̑S�̃t���[�����I����Ă���Idle��Ԃɖ߂�
			if (m_actionTime > attackData.actionTotalTime)
			{
				m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
				state->Init();
				return;
			}
		}
	}

	//�U�����n�܂��Ă���R���{�̓��͂̎�t���s��
	if (m_actionTime > 0)
	{
		//�ʏ�U���̃R���{�̓���
		if (!input.IsPress(Game::InputId::kLb))
		{
			//�C�e�U���������ꍇ
			if (input.IsTrigger(Game::InputId::kX) && attackData.isEnergy)
			{
				//���ԓ��ɍU�����͂����Ă���Ύ��i�̍U���Ɉڍs����t���O�𗧂Ă�
				m_nextAttackId = GetNextNormalAttack(m_attackId);
			}
			//�i���U���������ꍇ 
			if (input.IsTrigger(Game::InputId::kB) && !attackData.isEnergy)
			{
				//���ԓ��ɍU�����͂����Ă���Ύ��i�̍U���Ɉڍs����t���O�𗧂Ă�
				m_nextAttackId = GetNextNormalAttack(m_attackId);
			}
		}
		else
		{
			//�Z�b�g���Ă���K�E�Z��Id
			auto attackDataId = m_pPlayer->GetSetSpecialAttackId();
			//�e��K�E�Z����͂����ꍇ
			//�ʏ�Z���o���Ă���Ȃ瑱���ĕK�E�Z���o��
			if (input.IsTrigger(Game::InputId::kA))
			{
				m_nextAttackId = attackDataId[Game::InputId::kA];
			}
			if (input.IsTrigger(Game::InputId::kB))
			{
				m_nextAttackId = attackDataId[Game::InputId::kB];
			}
			if (input.IsTrigger(Game::InputId::kX))
			{
				m_nextAttackId = attackDataId[Game::InputId::kX];
			}
			if (input.IsTrigger(Game::InputId::kY))
			{
				m_nextAttackId = attackDataId[Game::InputId::kY];
			}
		}
	}

	//�ړ��x�N�g��������
	m_pPlayer->SetVelo(velo);

	m_pPlayer->SetModelFront(m_pPlayer->GetTargetPos());

	bool endFlag = m_pPlayer->PlayAnim();

	//�A�j���[�V�������I�������^�C�~���O
	if (endFlag)
	{
		//�C�e�̕K�E�Z��ł������Ƃ�
		if (attackData.isSpecial && attackData.isEnergy)
		{
			//�C�e�̕K�E�Z��ł�����̃A�j���[�V�������s���Ă�����
			if (m_isEndSpecialAttackAnim)
			{
				//State��߂�
				m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
				state->Init();
				return;
			}
			//�s���Ă��Ȃ�������
			else
			{
				//�K�E�Z�̂��ƌ��̎��ԕ��A�j���[�V����������
				m_pPlayer->SetAttackEndAnim(static_cast<float>(attackData.actionTotalTime - attackData.attackEndTime));
				m_isEndSpecialAttackAnim = true;
			}
		}
		//�C�c�̕K�E�Z�ȊO�̂���
		else
		{
			//���̃A�j���[�V�������ݒ肳��Ă��Ȃ�������
			if (m_actionTime == 0)
			{
				//�A�j���[�V�������I�������
				//State��߂�
				m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
				state->Init();
				return;
			}
		}

	}

	m_nextState = shared_from_this();
}

int PlayerStateAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage();
	//�C�e��ł��Ă���Ƃ��̂ݏ�Ԃ�ω�������
	if (m_pPlayer->GetAttackData(m_attackId).isEnergy && !m_pPlayer->GetAttackData(m_attackId).isSpecial)
	{
		//HitAttack�ɕω�������
		m_nextState = std::make_shared<PlayerStateHitAttack>(m_pPlayer, m_pScene);
		//�󂯂��U���̎�ނ�ݒ肷��
		auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
		state->Init(collider);
	}
	//�q�b�g�G�t�F�N�g��\������
	MyEngine::Vector3 pos = m_pPlayer->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Hit"), pos, false);
	EffekseerManager::GetInstance().Entry(effect);
	return damage;
}

std::string PlayerStateAttack::GetNextNormalAttack(std::string id)
{
	if (id == CommandId::kEnergyAttack1)
	{
		return CommandId::kEnergyAttack2;
	}
	else if (id == CommandId::kEnergyAttack2)
	{
		return CommandId::kEnergyAttack3;
	}
	else if (id == CommandId::kEnergyAttack3)
	{
		return CommandId::kEnergyAttack4;
	}
	else if (id == CommandId::kPhysicalAttack1)
	{
		return CommandId::kPhysicalAttack2;
	}
	else if (id == CommandId::kPhysicalAttack2)
	{
		return CommandId::kPhysicalAttack3;
	}
	else
	{
		return "empty";
	}
}
