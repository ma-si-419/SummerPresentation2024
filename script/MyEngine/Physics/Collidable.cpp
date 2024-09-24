#include "Collidable.h"
#include "CapsuleColliderData.h"
#include "SphereColliderData.h"
#include "Physics.h"
#include <cassert>

Collidable::Collidable(ObjectTag tag, ColliderData::Kind colKind) :
	m_pColData(nullptr),
	m_tag(tag)
{
	m_pColData = CreateColliderData(colKind);
}

Collidable::~Collidable()
{
}

void Collidable::Init(std::shared_ptr<Physics> physics)
{
	//�������o�^
	physics->Entry(shared_from_this());
}

void Collidable::Final(std::shared_ptr<Physics> physics)
{
	//����������
	physics->Exit(shared_from_this());
}

void Collidable::OnCollide(std::shared_ptr<Collidable> collider)
{
}

std::shared_ptr<ColliderData> Collidable::CreateColliderData(ColliderData::Kind kind)
{
	if (m_pColData != nullptr)
	{
		assert(0 && "colliderData�͂��łɍ���Ă��܂�");
		return m_pColData;
	}
	if (kind == ColliderData::Kind::kCapsule)
	{
		//�J�v�Z���R���C�_�[�̏�������
		return std::make_shared<CapsuleColliderData>();
	}
	else if (kind == ColliderData::Kind::kSphere)
	{
		//�X�t�B�A�R���C�_�[�̏�������
		return std::make_shared<CapsuleColliderData>();
	}
	else
	{
		return nullptr;
	}
}
