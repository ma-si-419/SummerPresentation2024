#include "EnemyStateMove.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"

namespace
{
	//�Œች�t���[������State�ł��邩
	constexpr int kShortestTime = 120;
	//MoveBack�����t���[������State�ł��邩
	constexpr int kMoveBackTime = 60;
	//�㉺�ړ���D�悷��Y���W�̂���̑傫��
	constexpr float kYGapScale = 3;
	//�߂Â����Ƃ�D�悵�n�߂鋗��
	constexpr float kMoveFrontDistance = 50;
	//����邱�Ƃ�D�悵�n�߂鋗��
	constexpr float kMoveBackDistance = 8;
	//����Ă��鋗���̂܂ܗD��x���グ�Ȃ��悤�ɁA�����Ɋ����������ėD��x�ɕϊ�����
	constexpr float kDistanceRate = 0.005f;
	//��{�I�Ȉړ������̊���
	constexpr int kMoveDirRate[3] = { 30,20,20 };
	//�ړ����x
	constexpr float kMoveSpeed = 0.5f;
	//�����̕����̐�
	constexpr int kMoveDirNum = 8;
	//�����̕����̐��̔���
	constexpr int kMoveDirNumHalf = static_cast<int>(kMoveDirNum * 0.5);
	//�v���C���[�ɋ߂Â������Ȃ��悤��
	constexpr float kPlayerDistance = 15.0f;

}

void EnemyStateMove::Init(MyEngine::Vector3 playerPos)
{
	//player�̍��W�ɍ��킹�ē������������肷��

	//�ړ������̊���
	int moveRate[static_cast<int>(MoveKind::kMoveKindNum)] = {};
	//����������������
	moveRate[static_cast<int>(MoveKind::kFront)] = kMoveDirRate[static_cast<int>(MoveKind::kFront)];
	moveRate[static_cast<int>(MoveKind::kBack)] = kMoveDirRate[static_cast<int>(MoveKind::kBack)];
	moveRate[static_cast<int>(MoveKind::kRandom)] = kMoveDirRate[static_cast<int>(MoveKind::kRandom)];


	//Y���W������Ă�����ł��邾�����킹��悤�ɓ���
	float gapPosY = abs(m_pEnemy->GetPos().y - playerPos.y);
	//Y���W�̂��ꂪ�傫��������㉺�ړ�����悤�ɂ���
	if (gapPosY > kYGapScale)
	{
		m_isMoveVertical = true;
	}
	//�v���C���[�Ƃ̋�����������Ή����قǃv���C���[�Ɍ������Ă����m�����グ��
	float distance = (m_pEnemy->GetPos() - playerPos).Length();
	//������������������΋߂Â��D��x���グ��
	if (distance > kMoveFrontDistance)
	{
		//�D��x�̑�����
		int addRate = static_cast<int>((distance - kMoveFrontDistance) * kDistanceRate);

		moveRate[static_cast<int>(MoveKind::kFront)] += addRate;
	}
	//�������������߂���Η����悤�ɂ���
	else if (distance < kMoveBackDistance)
	{
		//�ق��̑I����������
		moveRate[static_cast<int>(MoveKind::kFront)] = 0;
		moveRate[static_cast<int>(MoveKind::kRandom)] = 0;
	}

	//���������ׂđ������l
	int total = 0;

	for (auto item : moveRate)
	{
		total += item;
	}

	//���ׂđ������l�������_���͈̔͂ɂ���
	int ans = GetRand(total);
	int moveKind = 0;

	//�����������Ă����A0�ȉ��ɂȂ������̍s���Ɍ��肷��
	for (auto item : moveRate)
	{
		ans -= item;
		if (ans <= 0)
		{
			break;
		}
		moveKind++;
	}
	if (moveKind > static_cast<int>(MoveKind::kRandom))
	{
		moveKind = static_cast<int>(MoveKind::kRandom);
	}
	//�ړ�����
	MyEngine::Vector3 moveDir;

	//�v���C���[�Ɍ������Ă���
	if (moveKind == static_cast<int>(MoveKind::kFront))
	{
		moveDir = (playerPos - m_pEnemy->GetPos()).Normalize();

		m_moveKind = MoveKind::kFront;
	}
	//�v���C���[���痣���
	else if (moveKind == static_cast<int>(MoveKind::kBack))
	{
		moveDir = (m_pEnemy->GetPos() - playerPos).Normalize();
		//�㉺�ړ��t���O�������Ă�����
		if (m_isMoveVertical)
		{
			//�v���C���[��Y���W�����킹�ɍs��
			moveDir.y *= -1;
		}
		m_moveKind = MoveKind::kBack;
	}
	//�����_���ȕ����Ɉړ�����
	else if (moveKind == static_cast<int>(MoveKind::kRandom))
	{
		moveDir = MyEngine::Vector3(GetRand(kMoveDirNum) - kMoveDirNumHalf, 0, GetRand(kMoveDirNum) - kMoveDirNumHalf).Normalize();
	
		//�㉺�ړ��t���O�������Ă�����
		if (m_isMoveVertical)
		{
			//�O�㉡�ړ��Ȃ��ŏ㉺�ړ�����
			MyEngine::Vector3 targetPos = playerPos;
			//�O�㉡�������G�l�~�[�̌��ݒn�Ɠ����ɂ���
			targetPos.x = m_pEnemy->GetPos().x;
			targetPos.z = m_pEnemy->GetPos().z;

			//�㉺�ړ������݂̂̕����x�N�g�����쐬
			moveDir = (targetPos - m_pEnemy->GetPos()).Normalize();
		}

		m_moveKind = MoveKind::kRandom;	
	}

	//�ŏ��̍��W��ۑ�����
	m_initPos = m_pEnemy->GetPos();
	//�^�[�Q�b�g���W��ۑ�����
	m_targetPos = playerPos;


	//�ړ��x�N�g��
	m_velo = moveDir * kMoveSpeed;

	m_pEnemy->ChangeAnim("Move");

}

void EnemyStateMove::Update()
{
	//�o�ߎ��Ԃ��v��
	m_time++;

	//�ŏ��̍��W����^�[�Q�b�g���W�܂ňړ�������
	if (m_moveKind == MoveKind::kFront && (m_targetPos - m_initPos).Length() - kPlayerDistance < (m_pEnemy->GetPos() - m_initPos).Length())
	{
		//�ړ�����߂ĕʂ�State�ɍs��
		m_velo = MyEngine::Vector3(0, 0, 0);
		m_isChangeState = true;
	}
	//�v���C���[�ɋ߂Â������Ȃ��悤�Ɉ�苗���܂ŗ�����ʂ�State�ɍs��
	if ((m_pEnemy->GetTargetPos() - m_pEnemy->GetPos()).Length() < kPlayerDistance)
	{
		//�ړ�����߂ĕʂ�State�ɍs��
		m_velo = MyEngine::Vector3(0, 0, 0);
		m_isChangeState = true;
	}

	//�v���C���[�Ƃ�Y���W�̍�
	float yGap = m_pEnemy->GetTargetPos().y - m_pEnemy->GetPos().y;
	//�㉺�ړ����Ă��ăv���C���[�Ƃ�Y���W���߂��Ȃ�����
	if (m_velo.y != 0 && yGap < kYGapScale)
	{
		//�ׂ�State�Ɉړ�����
		m_isChangeState = true;
	}


	m_pEnemy->SetModelFront(m_velo + m_pEnemy->GetPos());
	m_isLookPlayer = false;

	m_pEnemy->PlayAnim();

	//Init�Ō��肵���x�N�g���ňړ�����
	m_pEnemy->SetVelo(m_velo);

	int random = 0;

	//���̃t���[���ɂ���Œ᎞�Ԃ𒴂�����m���ŕʂ̃t���[���ɍs��

	if (m_moveKind == MoveKind::kBack)
	{
		random = GetRand(m_time) - kMoveBackTime;
	}
	else
	{
		random = GetRand(m_time) - kShortestTime;
	}
	if (random > 0)
	{
		m_isChangeState = true;
	}
}

int EnemyStateMove::OnDamage(std::shared_ptr<Collidable> collider)
{
	//�_���[�W
	int damage = 0;
	//�U���̃|�C���^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//�_���[�W�����̂܂ܓn��
	damage = attack->GetDamage() - GetRand(static_cast<int>(m_pEnemy->GetStatus().def));
	//�󂯂��U���̎�ނ�ݒ肷��
	m_hitEffect = attack->GetHitEffect();
	m_isChangeState = true;
	//�q�b�g�G�t�F�N�g��\������
	MyEngine::Vector3 pos = m_pEnemy->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Hit"), pos, false);
	EffekseerManager::GetInstance().Entry(effect);

	return damage;
}
