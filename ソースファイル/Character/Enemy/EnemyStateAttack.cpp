#include "EnemyStateAttack.h"
#include "CommandIdList.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"

namespace
{
	//�i���U����D�悷�鋗��
	constexpr float kPrioritizePhysialAttackDistance = 30.0f;

	//�C�e�U����D�悷�鋗��
	constexpr float kPrioritizeEnergyAttackDistance = 100.0f;

	//�i���U�����o������
	constexpr float kPhysicalAttackDistance = 15.0f;

	//�i���U�����o���Ƃ��̈ړ����x
	constexpr float kMoveSpeed = 1.5f;

	//�i���U���𗣂�Ă��Ă��o������
	constexpr int kMaxMoveTime = 120;

	//��{�I�ȍU�����@�̊���
	constexpr int kAttackKindRate[4] = { 30,30,20,20 };
	//�i���U����D�悷��Ƃ��̊���
	constexpr int kPrioritizePhysialAttackKindRate[4] = { 20,20,30,30 };
	//�C�e�U����D�悷��Ƃ��̊���
	constexpr int kPrioritizeEnergyAttackKindRate[4] = { 50,50,0,0 };

}

void EnemyStateAttack::Init(std::shared_ptr<Player> player)
{
	//������
	m_time = 0;
	m_isNearPlayer = true;
	m_pPlayer = player;


	//�s�����Ȃɂɂ��邩
	int attackKind = 0;

	//�����ɂ���ďo���Z�̗D��x��ω�������
	if ((m_pPlayer->GetPos() - m_pEnemy->GetPos()).Length() > kPrioritizeEnergyAttackDistance)
	{
		//�C�e�U�����D�悳���ꍇ
		int totalRate = 0;
		//�m�������ׂđ���
		for (auto item : kPrioritizeEnergyAttackKindRate)
		{
			totalRate += item;
		}
		int rand = GetRand(totalRate);

		for (auto item : kPrioritizeEnergyAttackKindRate)
		{
			rand -= item;
			if (rand <= 0)
			{
				break;
			}
			attackKind++;
		}
	}
	else if ((m_pPlayer->GetPos() - m_pEnemy->GetPos()).Length() > kPrioritizePhysialAttackDistance)
	{
		//�i���U�����D�悳���ꍇ
		int totalRate = 0;
		//�m�������ׂđ���
		for (auto item : kPrioritizePhysialAttackKindRate)
		{
			totalRate += item;
		}
		int rand = GetRand(totalRate);

		for (auto item : kPrioritizePhysialAttackKindRate)
		{
			rand -= item;
			if (rand <= 0)
			{
				break;
			}
			attackKind++;
		}
	}
	else
	{
		//�ǂ�����D�悵�Ȃ��ꍇ
		int totalRate = 0;
		//�m�������ׂđ���
		for (auto item : kAttackKindRate)
		{
			totalRate += item;
		}
		int rand = GetRand(totalRate);

		for (auto item : kAttackKindRate)
		{
			rand -= item;
			if (rand <= 0)
			{
				break;
			}
			attackKind++;
		}
	}

	//�U���̎�ނ����čU�������肷��
	if (attackKind == static_cast<int>(AttackKind::kRepeatedlyEnergy))
	{
		m_attackId = CommandId::kEnemyEnergyAttack;
		m_isNearPlayer = false;
		m_isStartAttack = true;
	}
	else if (attackKind == static_cast<int>(AttackKind::kLaser))
	{
		m_attackId = CommandId::kEnemyLaserAttack;
		m_isNearPlayer = false;
		m_isStartAttack = true;
	}
	else if (attackKind == static_cast<int>(AttackKind::kBreakAttack))
	{
		m_attackId = CommandId::kEnemySlamAttack;
		m_isNearPlayer = true;
		m_isStartAttack = false;
	}
	else if (attackKind == static_cast<int>(AttackKind::kStanAttack))
	{
		m_attackId = CommandId::kEnemyStanAttack;
		m_isNearPlayer = true;
		m_isStartAttack = false;
	}

	m_pEnemy->ChangeAnim(m_pEnemy->GetAttackData()[m_attackId].animationName);
}

void EnemyStateAttack::Update()
{
	m_time++;
	//�ړ��x�N�g��
	MyEngine::Vector3 velo;

	//�G�Ɍ������Ă���
	if (m_isNearPlayer)
	{
		velo = (m_pPlayer->GetPos() - m_pEnemy->GetPos()).Normalize() * kMoveSpeed;
	}
	//��苗���܂ŋ߂Â�����莞�Ԃ�������
	if ((m_pPlayer->GetPos() - m_pEnemy->GetPos()).Length() < kPhysicalAttackDistance || m_time > kMaxMoveTime)
	{
		//�������~�߂�
		velo = MyEngine::Vector3(0, 0, 0);
		//�U�����o���n�߂�
		m_isStartAttack = true;
		//�G�ɋ߂Â��̂���߂�
		m_isNearPlayer = false;
	}
	//�U�����o���Ă��鎞�Ԃ��v������
	if (m_isStartAttack)
	{
		m_attackTime++;
	}

	m_pEnemy->SetVelo(velo);

	//�U���̏����擾����
	std::map<std::string, DataManager::AttackInfo> attackData = m_pEnemy->GetAttackData();

	//�U�����I����Ă��Ȃ�����
	if (!m_isAttackEnd)
	{
		//�U�����o���n�߂ĉ��t���[���������čU���̏������s��
		if (m_attackTime > attackData[m_attackId].attackStartTime)
		{
			//�U���𕡐���o���Z�ł����
			if (attackData[m_attackId].attackNum > 1)
			{
				//�U���̃X�p�����擾����
				int span = (attackData[m_attackId].attackEndTime - attackData[m_attackId].attackStartTime) / attackData[m_attackId].attackNum;
				//�U���̃^�C�~���O��������U�����o���悤�ɂ���
				if (m_attackTime % span == 0 && m_attackTime < attackData[m_attackId].attackEndTime)
				{
					//���[�U�[���o�����W���w�肳��Ă��Ȃ�������
					if (m_laserTargetPos.sqLength() == 0)
					{
						m_laserTargetPos = m_pEnemy->GetTargetPos();
					}
					//�U�����쐬
					std::shared_ptr<AttackBase> attack = m_pEnemy->CreateAttack(m_attackId, m_laserTargetPos);
					//���[�U�[��̍U���̐ݒ�
					if (attackData[m_attackId].isLaser)
					{
						//������܂ł̎���
						int lifeTime = attackData[m_attackId].lifeTime - m_attackTime;

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
			}
			//�P���U���ł����
			else
			{
				//�U�����o�����ԂɂȂ�����
				if (m_attackTime > attackData[m_attackId].attackStartTime)
				{
					//�U�����o��
					m_pScene->AddAttack(m_pEnemy->CreateAttack(m_attackId, MyEngine::Vector3(0, 0, 0)));
					m_isAttackEnd = true;
				}
			}
		}
	}
	//�U���̎��Ԃ��I�������
	if (m_attackTime > attackData[m_attackId].attackEndTime && !m_isAttackEndStanAnim)
	{
		m_isAttackEndStanAnim = true;
		m_isAttackEnd = true;
		m_pEnemy->SetAttackEndAnim(static_cast<float>(attackData[m_attackId].actionTotalTime - attackData[m_attackId].attackEndTime));
	}
	//�s���S�̂̎��Ԃ��I�������
	if (m_attackTime > attackData[m_attackId].actionTotalTime)
	{
		m_isChangeState = true;
	}

	m_pEnemy->PlayAnim();

}
int EnemyStateAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage() - GetRand(static_cast<int>(m_pEnemy->GetStatus().def));
	//��{State�͕ύX���Ȃ����A������΂������ω����󂯂�
	if (attack->GetHitEffect() == static_cast<int>(EnemyStateBase::HitEffectKind::kBurst))
	{
		m_hitEffect = attack->GetHitEffect();
		m_isChangeState = true;
	}
	MyEngine::Vector3 pos = m_pEnemy->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Hit"),pos,false);
	EffekseerManager::GetInstance().Entry(effect);
	return damage;
}
