#include "Physics.h"
#include <cassert>
#include "Vector3.h"
#include "Collidable.h"
#include "Stage.h"
#include "CapsuleColliderData.h"
#include "SphereColliderData.h"
void Physics::Entry(std::shared_ptr<Collidable> col)
{
	// �o�^
	bool found = (std::find(m_collidables.begin(), m_collidables.end(), col) != m_collidables.end());
	if (!found)
	{
		m_collidables.emplace_back(col);
	}
	// ���ɓo�^����Ă���G���[
	else
	{
		assert(0 && "�w���collidable�͓o�^�ςł��B");
	}
}

void Physics::Exit(std::shared_ptr<Collidable> col)
{
	// �o�^����
	bool found = (std::find(m_collidables.begin(), m_collidables.end(), col) != m_collidables.end());
	if (found)
	{
		m_collidables.remove(col);
	}
	// �o�^����ĂȂ�������G���[
	else
	{
		assert(0 && "�w���collidable���o�^����Ă��܂���B");
	}
}

void Physics::Update()
{
	//������
	for (auto& item : m_collidables)
	{
		item->m_nextPos = (item->m_rigidbody.GetPos() + item->m_rigidbody.GetVelo());
	}
	//�������Ă�����̂�����z��
	std::vector<OnCollideInfo> pushData;
	for (auto& first : m_collidables)
	{
		for (auto& second : m_collidables)
		{
			//�����蔻��`�F�b�N
			if (CheckCollide(first, second))
			{
				//��x���ꂽ���̂��x����Ȃ��悤�Ƀ`�F�b�N
				bool hasFirstColData = false;
				bool hasSecondColData = false;
				for (auto& item : pushData)
				{
					//���łɓ���Ă�����e��
					if (item.owner == first)
					{
						hasFirstColData = true;
					}
					if (item.owner == second)
					{
						hasSecondColData = true;
					}
				}
				//�e����Ȃ������ꍇ�����������̃��X�g�ɓ����
				if (!hasFirstColData)
				{
					pushData.push_back({ first,second });
					//if (first->GetTag() == ObjectTag::kPlayer)
					//{
					//	printfDx("�v���C���[�ƂԂ�����");
					//}
					//if (first->GetTag() == ObjectTag::kEnemy)
					//{
					//	printfDx("�G�l�~�[�ƂԂ�����");
					//}
				}
				if (!hasSecondColData)
				{
					pushData.push_back({ second,first });
					//if (first->GetTag() == ObjectTag::kPlayer)
					//{
					//	printfDx("�v���C���[�ƂԂ�����");
					//}
					//if (first->GetTag() == ObjectTag::kEnemy)
					//{
					//	printfDx("�G�l�~�[�ƂԂ�����");
					//}
				}
			}
		}
	}
	//�������������蔻��̓����������̏������Ă�
	for (auto& hitCol : pushData)
	{
		hitCol.OnCollide();
	}
	//�ʒu�C��
	FixPosition();
}

void Physics::DebugDraw()
{
	//�����蔻��̕`����s��
	for (auto& item : m_collidables)
	{
		if (item->m_pColData->GetKind() == ColliderData::Kind::kCapsule)
		{
			auto capsuleData = std::dynamic_pointer_cast<CapsuleColliderData>(item->m_pColData);

			DrawCapsule3D(capsuleData->m_startPos.CastVECTOR(), item->m_rigidbody.GetPos().CastVECTOR(),
				capsuleData->m_radius, 3, GetColor(50, 50, 255), GetColor(255, 255, 255), false);
		}
		else if (item->m_pColData->GetKind() == ColliderData::Kind::kSphere)
		{
			auto sphereData = std::dynamic_pointer_cast<SphereColliderData>(item->m_pColData);

			DrawSphere3D(item->m_rigidbody.GetPos().CastVECTOR(), sphereData->m_radius, 3, GetColor(255, 255, 0), GetColor(255, 255, 255), false);
		}
	}
}

void Physics::FixPosition()
{
	for (auto& item : m_collidables)
	{
		// Pos���X�V����̂ŁAvelocity�������Ɉړ�����velocity�ɏC��
		MyEngine::Vector3 toFixedPos = item->m_nextPos - item->m_rigidbody.GetPos(); 
		MyEngine::Vector3 nextPos = item->m_rigidbody.GetPos() + toFixedPos;

		MyEngine::Vector3 centerPos(0, 0, 0);



		//TODO:�X�e�[�W�̓����蔻����쐬����
		//�ړ�������t����(������)
		if ((nextPos - centerPos).Length() > 500)
		{
			//�Ԃ������ꏊ��ۑ�����
			OnCollideInfo hitCollides;
			//�X�e�[�W�ƂԂ���������
			std::shared_ptr<Stage> stage = std::make_shared<Stage>(ObjectTag::kStage);
			//�Ԃ������ꏊ��␳�O�̍��W�ɐݒ�
			stage->m_rigidbody.SetPos(nextPos);
			hitCollides.owner = stage;
			hitCollides.colider = item;
			//�Ԃ��������̏������Ă�
			hitCollides.OnCollide();
			//���W��␳
			nextPos = (nextPos - centerPos).Normalize() * 500;
		}
		if(nextPos.y < -50)
		{
			nextPos.y = -50;
		}

		item->m_rigidbody.SetVelo(toFixedPos);


		// �ʒu�m��
		item->m_rigidbody.SetPos(nextPos);

		//�����蔻�肪�J�v�Z����������
		if (item->m_pColData->GetKind() == ColliderData::Kind::kCapsule)
		{
			auto capsuleCol = std::dynamic_pointer_cast<CapsuleColliderData>(item->m_pColData);
			//�L�т�J�v�Z�����ǂ����擾����
			if (!capsuleCol->m_isMoveStartPos)
			{
				//�L�тȂ��J�v�Z���������珉���ʒu���ꏏ�ɓ�����
				capsuleCol->m_startPos = item->m_nextPos;
			}
		}
	}
}

bool Physics::CheckCollide(std::shared_ptr<Collidable> first, std::shared_ptr<Collidable> second)
{
	//���̓����蔻��Ƒ��̓����蔻�肪���Ȃ����̂łȂ����
	if (first != second)
	{
		//�����蔻��̎�ނ��擾
		ColliderData::Kind firstKind = first->m_pColData->GetKind();
		ColliderData::Kind secondKind = second->m_pColData->GetKind();

		//���ǂ����̓����蔻��
		if (firstKind == ColliderData::Kind::kSphere && secondKind == ColliderData::Kind::kSphere)
		{
			//�����蔻������Ƀ_�E���L���X�g����
			auto firstCol = std::dynamic_pointer_cast<SphereColliderData>(first->m_pColData);
			auto secondCol = std::dynamic_pointer_cast<SphereColliderData>(second->m_pColData);

			//�����蔻��̋��������߂�
			float distance = (first->m_nextPos - second->m_nextPos).Length();

			//���̑傫�������킹�����̂����������Z����΂Ԃ����Ă���
			if (distance < firstCol->m_radius + secondCol->m_radius)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		//�J�v�Z�����m�̓����蔻��
		else if (firstKind == ColliderData::Kind::kCapsule && secondKind == ColliderData::Kind::kCapsule)
		{
			//�����蔻����J�v�Z���Ƀ_�E���L���X�g����
			auto firstCol = std::dynamic_pointer_cast<CapsuleColliderData>(first->m_pColData);
			auto secondCol = std::dynamic_pointer_cast<CapsuleColliderData>(second->m_pColData);

			//�J�v�Z�����m�̍ŒZ����
			float distance = Segment_Segment_MinLength(first->m_nextPos.CastVECTOR(), firstCol->m_startPos.CastVECTOR(),
				second->m_nextPos.CastVECTOR(), secondCol->m_startPos.CastVECTOR());

			if (distance < firstCol->m_radius + secondCol->m_radius)
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		//���ƃJ�v�Z���̓����蔻��
		else if (firstKind == ColliderData::Kind::kSphere && secondKind == ColliderData::Kind::kCapsule ||
			firstKind == ColliderData::Kind::kCapsule && secondKind == ColliderData::Kind::kSphere)
		{
			//���ƃJ�v�Z���̃R���C�_�[�f�[�^�쐬
			std::shared_ptr<ColliderData> sphereDataBase;
			std::shared_ptr<ColliderData> capsuleDataBase;
			float distance;
			//�ǂ��炪�J�v�Z���Ȃ̂����Ȃ̂����ʂ��Ă���f�[�^������
			if (firstKind == ColliderData::Kind::kSphere)
			{
				sphereDataBase = first->m_pColData;
				capsuleDataBase = second->m_pColData;

				//�J�v�Z���̐����̃f�[�^���g�����߂Ƀ_�E���L���X�g
				auto capsuleData = std::dynamic_pointer_cast<CapsuleColliderData>(capsuleDataBase);
				//�����Ɠ_�̍ŋߋ��������߂�
				distance = Segment_Point_MinLength(second->m_nextPos.CastVECTOR(),
					capsuleData->m_startPos.CastVECTOR(), first->m_nextPos.CastVECTOR());
			}
			else
			{
				capsuleDataBase = first->m_pColData;
				sphereDataBase = second->m_pColData;
				//�J�v�Z���̐����̃f�[�^���g�����߂Ƀ_�E���L���X�g
				auto capsuleData = std::dynamic_pointer_cast<CapsuleColliderData>(capsuleDataBase);
				//�����Ɠ_�̍ŋߋ��������߂�
				distance = Segment_Point_MinLength(first->m_nextPos.CastVECTOR(),
					capsuleData->m_startPos.CastVECTOR(), second->m_nextPos.CastVECTOR());
			}
			//�_�E���L���X�g
			auto sphereData = std::dynamic_pointer_cast<SphereColliderData>(sphereDataBase);
			auto capsuleData = std::dynamic_pointer_cast<CapsuleColliderData>(capsuleDataBase);

			//�~�ƃJ�v�Z���̔��a�����~�ƃJ�v�Z���̋������߂���Γ������Ă���
			if (distance < sphereData->m_radius + capsuleData->m_radius)
			{
				return true;
			}
			else
			{
				return false;
			}


		}
		//�ǂ�ɂ����Ă͂܂�Ȃ�������
		else
		{
			assert(false);
			return false;
		}
	}
	return false;
}
