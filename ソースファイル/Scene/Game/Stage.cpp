#include "Stage.h"
#include "EffekseerManager.h"

Stage::Stage(ObjectTag tag):
	Collidable(tag,ColliderData::Kind::kSphere)
{
}

Stage::~Stage()
{
}

void Stage::OnCollide(std::shared_ptr<Collidable> collider)
{
	if (collider->GetTag() == ObjectTag::kPlayer || 
		collider->GetTag() == ObjectTag::kEnemy)
	{
		MyEngine::Vector3 pos = m_rigidbody.GetPos();
		std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("StageHit"),pos,false);
		EffekseerManager::GetInstance().Entry(effect);
	}
}
