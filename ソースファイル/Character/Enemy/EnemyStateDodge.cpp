#include "EnemyStateDodge.h"
#include "EffekseerManager.h"

namespace
{
	//�Œች�t���[������State�ł��邩
	constexpr int kShortestTime = 8;
	//�ړ����x
	constexpr float kMoveSpeed = 4.0f;
	//�����̕����̐�
	constexpr int kMoveDirNum = 8;
	//�����̕����̐��̔���
	constexpr int kMoveDirNumHalf = static_cast<int>(kMoveDirNum * 0.5);
}
void EnemyStateDodge::Init()
{
	MyEngine::Vector3 moveDir = MyEngine::Vector3(GetRand(kMoveDirNum) - kMoveDirNumHalf, 0, GetRand(kMoveDirNum) - kMoveDirNumHalf);
	//�ړ������̃����_�����ǂ����0�ŏo����
	if (moveDir.sqLength() == 0)
	{
		moveDir = MyEngine::Vector3(0, 0, 1);
	}
	moveDir = moveDir.Normalize();
	m_velo = moveDir * kMoveSpeed;
	m_pEnemy->ChangeAnim("Move");
	MyEngine::Vector3 pos = m_pEnemy->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Dodge"), pos, false);
	EffekseerManager::GetInstance().Entry(effect);
	m_pEnemy->SetEffectData(effect);
}
void EnemyStateDodge::Update()
{
	//�o�ߎ���
	m_time++;
	//�ړ�
	m_pEnemy->SetVelo(m_velo);

	m_pEnemy->SetModelFront(m_velo + m_pEnemy->GetPos());
	m_isLookPlayer = false;

	m_pEnemy->PlayAnim();

	//���ԂɂȂ����炷���ɂ���State�𔲂���
	if (m_time > kShortestTime)
	{
		m_isChangeState = true;
		m_pEnemy->EndEffect();
	}
}

int EnemyStateDodge::OnDamage(std::shared_ptr<Collidable> collider)
{
	//���ׂ�0�ŕԂ�
	return 0;
}
