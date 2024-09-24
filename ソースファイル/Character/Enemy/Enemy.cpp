#include "Enemy.h"
#include "SceneGame.h"
#include "AttackBase.h"
#include "CapsuleColliderData.h"
#include "Ui.h"
#include "EnemyStateIdle.h"
namespace
{
	//当たり判定の大きさ
	constexpr float kColScale = 2.0f;
	//初期位置
	const MyEngine::Vector3 kInitPos(30, 0, 30);
	//スタンゲージのマックス
	constexpr float kMaxStanPoint = 500;
	//スタンゲージが回復するまでの時間
	constexpr int kHealStanPointTime = 200;
	//コンボがつながらなくなるまでの時間
	constexpr int kComboTime = 180;
	//エネミーの体力
	constexpr int kEnemyHp = 50000;
	//エネミーの攻撃力
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
	//当たり判定の縦幅
	pos.y += kColScale;
	//当たり判定の座標調整
	colData->m_startPos = pos;
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
}

void Enemy::Update(std::shared_ptr<SceneGame> scene)
{
	//StateにAiの情報をわたす
	m_pState->m_aiData = m_aiStateData;

	//ダメージを受けてからの時間をカウントする
	m_lastHitDamageTime++;

	//ダメージを受けてから一定時間たったらスタン値を回復していく
	if (m_lastHitDamageTime > kHealStanPointTime)
	{
		m_stanPoint++;
		//上限値を超えないように
		if (m_stanPoint > kMaxStanPoint)
		{
			m_stanPoint = kMaxStanPoint;
		}
	}
	//ダメージを受けてから一定時間たったらコンボ数を減らす
	if (m_lastHitDamageTime > kComboTime)
	{
		m_comboCount = 0;
	}
	//コンボ数をUIに渡し続ける
	m_pUi->SetComboCount(m_comboCount);

	//State変更フラグが立っていたらStateを変更する
	if (m_pState->m_isChangeState)
	{
		m_pState = m_pState->m_nextState;
	}

	//Stateの更新処理
	m_pState->Update();

	//座標の更新
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());

	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();

	//当たり判定の縦幅
	colPos.y += kColScale;
	//当たり判定の座標調整
	colData->m_startPos = colPos;
	//プレイヤーの方を向くStateかどうか判断する
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
	//プレイヤーの攻撃に当たった時
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
		//スタンダメージを受ける
		m_stanPoint -= attack->GetStanDamage();
		//下がりすぎないように
		if (m_stanPoint < 0)
		{
			m_stanPoint = 0;
		}
		//コンボをカウントするタイミングだったら
		if (m_pUi->GetIsCountCombo())
		{
			//コンボ数を増やす
			m_comboCount++;
		}
		//ダメージを受けてからの時間を数える
		m_lastHitDamageTime = 0;
		//UIに受けたダメージを送る
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
	//エネミーの攻撃を作成
	std::shared_ptr<AttackBase> ans = std::make_shared<AttackBase>(ObjectTag::kEnemyAttack);

	//攻撃を出す座標を作成

	MyEngine::Vector3 toTargetVec = m_targetPos - m_rigidbody.GetPos();

	MyEngine::Vector3 attackPos = m_rigidbody.GetPos() + toTargetVec.Normalize() * m_attackData[id].radius;

	//サウンドハンドル
	int soundHandle = m_pScene->GetSEHandle(m_attackData[id].soundName);

	//ステータス設定
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
