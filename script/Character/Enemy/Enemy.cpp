#include "Enemy.h"
#include "SceneGame.h"
#include "AttackBase.h"
#include "CapsuleColliderData.h"
#include "Ui.h"
#include "EnemyStateIdle.h"
namespace
{
	//�����蔻��̑傫��
	constexpr float kColScale = 2.0f;
	//�����ʒu
	const MyEngine::Vector3 kInitPos(30, 0, 30);
	//�X�^���Q�[�W�̃}�b�N�X
	constexpr float kMaxStanPoint = 500;
	//�X�^���Q�[�W���񕜂���܂ł̎���
	constexpr int kHealStanPointTime = 200;
	//�R���{���Ȃ���Ȃ��Ȃ�܂ł̎���
	constexpr int kComboTime = 180;
	//�G�l�~�[�̗̑�
	constexpr int kEnemyHp = 50000;
	//�G�l�~�[�̍U����
	constexpr int kEnemyAtk = 150;

}
Enemy::Enemy() :
	CharacterBase("data/model/Enemy.mv1", ObjectTag::kEnemy),
	m_stanPoint(kMaxStanPoint),
	m_lastHitDamageTime(0),
	m_comboCount(0)
{
	//m_status.hp = kEnemyHp;
	//m_status.atk = kEnemyAtk;
}

Enemy::~Enemy()
{
	MV1DeleteModel(m_modelHandle);
}

void Enemy::Init(std::shared_ptr<Physics> physics)
{
	ChangeAnim("Idle");

	MV1SetScale(m_modelHandle, VGet(3, 3, 3));
	Collidable::Init(physics);
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	colData->m_radius = kColScale;
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	m_pPhysics = physics;

	m_rigidbody.SetPos(kInitPos);
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);

}

void Enemy::RetryInit()
{
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	m_comboCount = 0;

	m_lastHitDamageTime = kComboTime;

	MyEngine::Vector3 pos = kInitPos;

	m_rigidbody.SetPos(pos);
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	//�����蔻��̏c��
	pos.y += kColScale;
	//�����蔻��̍��W����
	colData->m_startPos = pos;
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
}

void Enemy::Update(std::shared_ptr<SceneGame> scene)
{
	//State��Ai�̏����킽��
	m_pState->m_aiData = m_aiStateData;

	//�_���[�W���󂯂Ă���̎��Ԃ��J�E���g����
	m_lastHitDamageTime++;

	//�_���[�W���󂯂Ă����莞�Ԃ�������X�^���l���񕜂��Ă���
	if (m_lastHitDamageTime > kHealStanPointTime)
	{
		m_stanPoint++;
		//����l�𒴂��Ȃ��悤��
		if (m_stanPoint > kMaxStanPoint)
		{
			m_stanPoint = kMaxStanPoint;
		}
	}
	//�_���[�W���󂯂Ă����莞�Ԃ�������R���{�������炷
	if (m_lastHitDamageTime > kComboTime)
	{
		m_comboCount = 0;
	}
	//�R���{����UI�ɓn��������
	m_pUi->SetComboCount(m_comboCount);

	//State�ύX�t���O�������Ă�����State��ύX����
	if (m_pState->m_isChangeState)
	{
		m_pState = m_pState->m_nextState;
	}

	//State�̍X�V����
	m_pState->Update();

	//���W�̍X�V
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());

	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();

	//�����蔻��̏c��
	colPos.y += kColScale;
	//�����蔻��̍��W����
	colData->m_startPos = colPos;
	//�v���C���[�̕�������State���ǂ������f����
	if (m_pState->m_isLookPlayer)
	{
		MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
	}

}

void Enemy::Draw()
{
	MV1DrawModel(m_modelHandle);
}

void Enemy::OnCollide(std::shared_ptr<Collidable> collider)
{
	//�v���C���[�̍U���ɓ���������
	if (collider->GetTag() == ObjectTag::kPlayerAttack)
	{
		auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
		int damage = m_pState->OnDamage(attack);
		if (damage < 0)
		{
			damage = 2;
		}
		m_nowHp -= damage;
		if (m_nowHp < 0)
		{
			m_nowHp = 0;
		}
		//�X�^���_���[�W���󂯂�
		m_stanPoint -= attack->GetStanDamage();
		//�����肷���Ȃ��悤��
		if (m_stanPoint < 0)
		{
			m_stanPoint = 0;
		}
		//�R���{���J�E���g����^�C�~���O��������
		if (m_pUi->GetIsCountCombo())
		{
			//�R���{���𑝂₷
			m_comboCount++;
		}
		//�_���[�W���󂯂Ă���̎��Ԃ𐔂���
		m_lastHitDamageTime = 0;
		//UI�Ɏ󂯂��_���[�W�𑗂�
		m_pUi->AddShowDamage(m_rigidbody.GetPos(), damage,false);
	}
}

void Enemy::InitAiState(std::shared_ptr<SceneGame> scene)
{
	m_pState = std::make_shared<EnemyStateIdle>(std::dynamic_pointer_cast<Enemy>(shared_from_this()), scene);
}

float Enemy::GetStanPointRate()
{
	return m_stanPoint / kMaxStanPoint;
}

void Enemy::StateUpdate(std::shared_ptr<Player> player)
{
	m_pState->CheckSituation(player);
}

std::shared_ptr<AttackBase> Enemy::CreateAttack(std::string id, MyEngine::Vector3 laserTargetPos)
{
	//�G�l�~�[�̍U�����쐬
	std::shared_ptr<AttackBase> ans = std::make_shared<AttackBase>(ObjectTag::kEnemyAttack);

	//�U�����o�����W���쐬

	MyEngine::Vector3 toTargetVec = m_targetPos - m_rigidbody.GetPos();

	MyEngine::Vector3 attackPos = m_rigidbody.GetPos() + toTargetVec.Normalize() * m_attackData[id].radius;

	//�T�E���h�n���h��
	int soundHandle = m_pScene->GetSEHandle(m_attackData[id].soundName);

	//�X�e�[�^�X�ݒ�
	if (m_attackData[id].isLaser)
	{
		ans->SetStatus(m_attackData[id], laserTargetPos, m_rigidbody.GetPos(), m_status.atk);
	}
	else
	{
		ans->SetStatus(m_attackData[id], m_targetPos, m_rigidbody.GetPos(), m_status.atk);
	}
	ans->Init(m_pPhysics, attackPos, m_attackData[id].effekseerName, soundHandle);

	return ans;
}

void Enemy::InitPos()
{
	m_rigidbody.SetPos(kInitPos);
}
