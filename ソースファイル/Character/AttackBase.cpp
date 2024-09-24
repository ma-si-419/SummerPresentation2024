#include "AttackBase.h"
#include "CapsuleColliderData.h"
#include "EffekseerForDXLib.h"
#include "EffekseerData.h"
#include "EffekseerManager.h"

namespace
{
	//敵との距離のうちどこまで追尾するか
	constexpr float kTrackLengthRate = 0.9f;
	//どこから追尾するか
	constexpr float kTrackStartPosRate = 0.3f;
	//どのくらい弾を散らばらせるか
	constexpr int kScatterPower = 50;
	constexpr int kScatterPowerHalf = kScatterPower * 0.5;
	//レーザーのエフェクトを出す感覚
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
	//当たり判定の初期化＆登録
	Collidable::Init(physics);
	//座標を設定する
	m_rigidbody.SetPos(pos);
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(Collidable::m_pColData);
	colData->m_startPos = m_rigidbody.GetPos() + (m_dir * m_status.radius);

	//ハンドルを保存しておく
	m_playSoundHandle = soundHandle;


	//エフェクトの設定
	if (m_isPopEffect)
	{
		MyEngine::Vector3 effectPos = colData->m_startPos;
		//エフェクトのポインタを作成
		std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData(effekseerId), effectPos, false);
		//エフェクトをマネージャーに追加
		EffekseerManager::GetInstance().Entry(effect);
		if (m_status.isLaser)
		{
			//出しているエフェクトの配列に追加
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
	//コライダーデータをダウンキャストしデータを設定する
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(Collidable::m_pColData);
	//ステータスを入れる
	m_status = status;
	//ダメージは出したキャラの攻撃力に倍率をかけて計算
	m_status.damage = static_cast<int>(m_status.damageRate * power);
	//プレイヤーからターゲットに向かっての方向を入れる
	m_dir = (target - playerPos).Normalize();
	//打ち出す方向をちらばらせる技であれば
	if (status.isScatter)
	{
		//X軸にランダムで回転させる
		MATRIX randomX = MGetRotX(static_cast<float>(GetRand(kScatterPower) - kScatterPowerHalf) * 0.01f);
		//Y軸にランダムで回転させる
		MATRIX randomY = MGetRotY(static_cast<float>(GetRand(kScatterPower) - kScatterPowerHalf) * 0.01f);

		MATRIX mat = MMult(randomX, randomY);

		m_dir = m_dir.MatTransform(mat);
	}
	//当たり判定の情報を入れる
	colData->m_radius = m_status.radius;
	colData->m_isMoveStartPos = status.isLaser;
	m_targetLength = (target - playerPos).Length();
}

void AttackBase::Update(MyEngine::Vector3 targetPos)
{
	//レーザー以外のものはすべてサウンドを鳴らす
	if (!m_isPlaySound)
	{
		if (!m_status.isLaser)
		{
			m_playSoundHandle = PlaySoundMem(m_playSoundHandle, DX_PLAYTYPE_BACK);
			m_isPlaySound = true;
		}
		else
		{
			//レーザーは一段目に出したものだけ効果音を鳴らす
			if (m_isPopEffect)
			{
				PlaySoundMem(m_playSoundHandle, DX_PLAYTYPE_BACK);
				m_isPlaySound = true;
			}
		}
	}

	//ライフタイムをカウントする
	m_lifeTime++;
	//当たり判定のデータをダウンキャスト
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(Collidable::m_pColData);
	//移動ベクトルを作成
	MyEngine::Vector3 velo = m_dir * m_status.speed;

	//当たり判定に移動ベクトルを設定
	m_rigidbody.SetVelo(velo);

	m_moveLength += velo.Length();

	//エフェクトを出す設定のものだったら
	if (m_isPopEffect)
	{
		//エフェクトの更新
		MyEngine::Vector3 effectPos = colData->m_startPos;

		//レーザーじゃない場合
		if (!m_status.isLaser)
		{
			//座標を更新し続ける
			m_pEffekseerData->SetPos(effectPos);
		}
		//レーザーの場合
		else
		{
			if (m_lifeTime % kLaserEffectPopTime == 0)
			{
				MyEngine::Vector3 pos = m_rigidbody.GetPos();

				//一定時間ごとにエフェクトを出す
				std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData(m_effectId), pos, false);

				EffekseerManager::GetInstance().Entry(effect);

				m_pLaserEffekseerData.push_back(effect);
			}
		}
	}


	//敵を追尾する攻撃だったら
	if (m_status.isTrack)
	{
		//出したばかりの時は追尾しないようにする
		if (m_moveLength > m_targetLength * kTrackStartPosRate)
		{
			//移動量が初期化時の敵との距離よりも短ければ
			if (m_moveLength < m_targetLength * kTrackLengthRate)
			{
				//追尾するようにする
				m_dir = (targetPos - m_rigidbody.GetPos()).Normalize();
			}
		}
	}

	//ライフタイムが上限を超えたら処理しないようにする
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

			//エフェクトを残さないと決められていたら
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
			//エフェクトを残さないと決められていたら
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
	//エフェクトを残さないと決められていたら
	if (!m_isLeaveEffect)
	{
		//再生中のエフェクトを消す
		for (auto& item : m_pLaserEffekseerData)
		{
			item->Final();
		}
		if (m_pEffekseerData)
		{
			m_pEffekseerData->Final();
		}
	}

	//当たり判定をけす
	Collidable::Final(physics);
}
