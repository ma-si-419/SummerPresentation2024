#include "AttackBase.h"
#include "CapsuleColliderData.h"
#include "EffekseerForDXLib.h"
#include "EffekseerData.h"
#include "EffekseerManager.h"

namespace
{
	//�G�Ƃ̋����̂����ǂ��܂Œǔ����邩
	constexpr float kTrackLengthRate = 0.9f;
	//�ǂ�����ǔ����邩
	constexpr float kTrackStartPosRate = 0.3f;
	//�ǂ̂��炢�e���U��΂点�邩
	constexpr int kScatterPower = 50;
	constexpr int kScatterPowerHalf = kScatterPower * 0.5;
	//���[�U�[�̃G�t�F�N�g���o�����o
	constexpr int kLaserEffectPopTime = 1;
}

AttackBase::AttackBase(ObjectTag tag) :
	Collidable(tag, ColliderData::Kind::kCapsule),
	m_status(),
	m_dir(),
	m_isExist(true),
	m_lifeTime(0),
	m_moveLength(0),
	m_targetLength(0),
	m_isLeaveEffect(false),
	m_playSoundHandle(-1),
	m_isPopEffect(true),
	m_isPlaySound(false)
{
}

AttackBase::~AttackBase()
{
}

void AttackBase::Init(std::shared_ptr<Physics> physics, MyEngine::Vector3 pos, std::string effekseerId, int soundHandle)
{
	//�����蔻��̏��������o�^
	Collidable::Init(physics);
	//���W��ݒ肷��
	m_rigidbody.SetPos(pos);
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(Collidable::m_pColData);
	colData->m_startPos = m_rigidbody.GetPos() + (m_dir * m_status.radius);

	//�n���h����ۑ����Ă���
	m_playSoundHandle = soundHandle;


	//�G�t�F�N�g�̐ݒ�
	if (m_isPopEffect)
	{
		MyEngine::Vector3 effectPos = colData->m_startPos;
		//�G�t�F�N�g�̃|�C���^���쐬
		std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData(effekseerId), effectPos, false);
		//�G�t�F�N�g���}�l�[�W���[�ɒǉ�
		EffekseerManager::GetInstance().Entry(effect);
		if (m_status.isLaser)
		{
			//�o���Ă���G�t�F�N�g�̔z��ɒǉ�
			m_pLaserEffekseerData.push_back(effect);
		}
		else
		{
			m_pEffekseerData = effect;
		}
		m_effectId = effekseerId;
	}
}

void AttackBase::SetStatus(DataManager::AttackInfo status, MyEngine::Vector3 target, MyEngine::Vector3 playerPos, float power)
{
	//�R���C�_�[�f�[�^���_�E���L���X�g���f�[�^��ݒ肷��
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(Collidable::m_pColData);
	//�X�e�[�^�X������
	m_status = status;
	//�_���[�W�͏o�����L�����̍U���͂ɔ{���������Čv�Z
	m_status.damage = static_cast<int>(m_status.damageRate * power);
	//�v���C���[����^�[�Q�b�g�Ɍ������Ă̕���������
	m_dir = (target - playerPos).Normalize();
	//�ł��o������������΂点��Z�ł����
	if (status.isScatter)
	{
		//X���Ƀ����_���ŉ�]������
		MATRIX randomX = MGetRotX(static_cast<float>(GetRand(kScatterPower) - kScatterPowerHalf) * 0.01f);
		//Y���Ƀ����_���ŉ�]������
		MATRIX randomY = MGetRotY(static_cast<float>(GetRand(kScatterPower) - kScatterPowerHalf) * 0.01f);

		MATRIX mat = MMult(randomX, randomY);

		m_dir = m_dir.MatTransform(mat);
	}
	//�����蔻��̏�������
	colData->m_radius = m_status.radius;
	colData->m_isMoveStartPos = status.isLaser;
	m_targetLength = (target - playerPos).Length();
}

void AttackBase::Update(MyEngine::Vector3 targetPos)
{
	//���[�U�[�ȊO�̂��̂͂��ׂăT�E���h��炷
	if (!m_isPlaySound)
	{
		if (!m_status.isLaser)
		{
			m_playSoundHandle = PlaySoundMem(m_playSoundHandle, DX_PLAYTYPE_BACK);
			m_isPlaySound = true;
		}
		else
		{
			//���[�U�[�͈�i�ڂɏo�������̂������ʉ���炷
			if (m_isPopEffect)
			{
				PlaySoundMem(m_playSoundHandle, DX_PLAYTYPE_BACK);
				m_isPlaySound = true;
			}
		}
	}

	//���C�t�^�C�����J�E���g����
	m_lifeTime++;
	//�����蔻��̃f�[�^���_�E���L���X�g
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(Collidable::m_pColData);
	//�ړ��x�N�g�����쐬
	MyEngine::Vector3 velo = m_dir * m_status.speed;

	//�����蔻��Ɉړ��x�N�g����ݒ�
	m_rigidbody.SetVelo(velo);

	m_moveLength += velo.Length();

	//�G�t�F�N�g���o���ݒ�̂��̂�������
	if (m_isPopEffect)
	{
		//�G�t�F�N�g�̍X�V
		MyEngine::Vector3 effectPos = colData->m_startPos;

		//���[�U�[����Ȃ��ꍇ
		if (!m_status.isLaser)
		{
			//���W���X�V��������
			m_pEffekseerData->SetPos(effectPos);
		}
		//���[�U�[�̏ꍇ
		else
		{
			if (m_lifeTime % kLaserEffectPopTime == 0)
			{
				MyEngine::Vector3 pos = m_rigidbody.GetPos();

				//��莞�Ԃ��ƂɃG�t�F�N�g���o��
				std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData(m_effectId), pos, false);

				EffekseerManager::GetInstance().Entry(effect);

				m_pLaserEffekseerData.push_back(effect);
			}
		}
	}


	//�G��ǔ�����U����������
	if (m_status.isTrack)
	{
		//�o�����΂���̎��͒ǔ����Ȃ��悤�ɂ���
		if (m_moveLength > m_targetLength * kTrackStartPosRate)
		{
			//�ړ��ʂ����������̓G�Ƃ̋��������Z�����
			if (m_moveLength < m_targetLength * kTrackLengthRate)
			{
				//�ǔ�����悤�ɂ���
				m_dir = (targetPos - m_rigidbody.GetPos()).Normalize();
			}
		}
	}

	//���C�t�^�C��������𒴂����珈�����Ȃ��悤�ɂ���
	if (m_lifeTime > m_status.lifeTime)
	{
		m_isExist = false;
		for (auto& item : m_pLaserEffekseerData)
		{
			item->Final();
		}
		if (m_pEffekseerData)
		{
			m_pEffekseerData->Final();
		}
	}
}

void AttackBase::OnCollide(std::shared_ptr<Collidable> collider)
{
	if (collider->GetTag() == ObjectTag::kPlayer)
	{
		if (GetTag() == ObjectTag::kEnemyAttack)
		{
			m_isExist = false;

			//�G�t�F�N�g���c���Ȃ��ƌ��߂��Ă�����
			if (!m_isLeaveEffect)
			{
				for (auto& item : m_pLaserEffekseerData)
				{
					item->Final();
				}
				if (m_pEffekseerData)
				{
					m_pEffekseerData->Final();
				}
			}
		}
	}
	else if (collider->GetTag() == ObjectTag::kEnemy)
	{
		if (GetTag() == ObjectTag::kPlayerAttack)
		{
			m_isExist = false;
			//�G�t�F�N�g���c���Ȃ��ƌ��߂��Ă�����
			if (!m_isLeaveEffect)
			{
				for (auto& item : m_pLaserEffekseerData)
				{
					item->Final();
				}
				if (m_pEffekseerData)
				{
					m_pEffekseerData->Final();
				}
			}
		}
	}
}

void AttackBase::Final(std::shared_ptr<Physics> physics)
{
	//�G�t�F�N�g���c���Ȃ��ƌ��߂��Ă�����
	if (!m_isLeaveEffect)
	{
		//�Đ����̃G�t�F�N�g������
		for (auto& item : m_pLaserEffekseerData)
		{
			item->Final();
		}
		if (m_pEffekseerData)
		{
			m_pEffekseerData->Final();
		}
	}

	//�����蔻�������
	Collidable::Final(physics);
}
