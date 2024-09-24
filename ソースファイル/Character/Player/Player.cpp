#include "Player.h"
#include "Game.h"
#include "SceneGame.h"
#include "AttackBase.h"
#include "CommandIdList.h"
#include "CapsuleColliderData.h"
#include "PlayerStateIdle.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"
namespace
{
	//移動速度
	constexpr float kMoveSpeed = 1.0f;
	//攻撃を出す座標
	constexpr float kAttackPos = 5.0f;
	//格闘攻撃を出す時の敵との距離(当たり判定の大きさの倍率)
	constexpr float kPhysicalAttackLange = 0.9f;
	//当たり判定の大きさ
	constexpr float kColScale = 2.0f;
	//トリガーが反応する
	constexpr int kTriggerReaction = 200;
	//初期回転度
	constexpr int kInitRota = 10;
	//受けるダメージの最低値
	constexpr int kMinDamage = 3;
	//モデルの拡大率
	constexpr float kModelScale = 0.03f;
}
Player::Player() :
	CharacterBase("data/model/Player.mv1", ObjectTag::kPlayer),
	m_rota(0),
	m_lastAttackTime(0),
	m_isOpenSpecialPallet(false)
{

}

Player::~Player()
{
	MV1DeleteModel(m_modelHandle);
}

void Player::Init(std::shared_ptr<Physics> physics)
{
	m_pState = std::make_shared<PlayerStateIdle>(std::dynamic_pointer_cast<Player>(shared_from_this()), m_pScene);
	m_pState->m_nextState = m_pState;
	auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_pState);
	state->Init();

	SetSpecialAttack();

	MV1SetScale(m_modelHandle, VGet(kModelScale, kModelScale, kModelScale));
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	m_rota = kInitRota;

	m_pPhysics = physics;
	Collidable::Init(physics);

	//Y軸を中心とした回転をするので
	MyEngine::Vector3 rotationShaftPos = m_targetPos;
	//Y座標が関係しないようにプレイヤーと同じ座標にする
	rotationShaftPos.y = m_rigidbody.GetPos().y;

	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_rigidbody.GetPos();

	MyEngine::Vector3 pos;

	pos.x = rotationShaftPos.x + cosf(m_rota) * toShaftPosVec.Length();
	pos.y = 0;
	pos.z = rotationShaftPos.z + sinf(m_rota) * toShaftPosVec.Length();

	m_rigidbody.SetPos(pos);

	//当たり判定
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	colData->m_radius = kColScale;
	//当たり判定の縦幅
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();
	colPos.y += kColScale;
	//当たり判定の座標調整
	colData->m_startPos = colPos;
	//ハンドルの座標を設定する
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
	//敵の方向を向くようにする
	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);

}

void Player::RetryInit()
{
	m_nowHp = m_status.hp;
	m_nowMp = m_status.mp;

	m_rota = kInitRota;


	//Y軸を中心とした回転をするので
	MyEngine::Vector3 rotationShaftPos = m_targetPos;
	//Y座標が関係しないようにプレイヤーと同じ座標にする
	rotationShaftPos.y = m_rigidbody.GetPos().y;

	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_rigidbody.GetPos();

	MyEngine::Vector3 pos;

	pos.x = rotationShaftPos.x + cosf(m_rota) * toShaftPosVec.Length();
	pos.y = 0;
	pos.z = rotationShaftPos.z + sinf(m_rota) * toShaftPosVec.Length();

	//当たり判定
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);

	//当たり判定の縦幅
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();
	colPos.y += kColScale;
	//当たり判定の座標調整
	colData->m_startPos = colPos;

	m_rigidbody.SetPos(pos);

	//Stateの初期化
	m_pState = std::make_shared<PlayerStateIdle>(std::dynamic_pointer_cast<Player>(shared_from_this()), m_pScene);
	m_pState->m_nextState = m_pState;
	auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_pState);
	state->Init();

	//ハンドルの座標を設定する
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
	//敵の方向を向くようにする
	MV1SetRotationZYAxis(m_modelHandle, (m_rigidbody.GetPos() - m_targetPos).CastVECTOR(), VGet(0.0f, 1.0f, 0.0f), 0.0f);
}

void Player::Update(std::shared_ptr<SceneGame> scene, MyEngine::Input input)
{
	//前のフレームとStateを比較して違うStateだったら
	if (m_pState->m_nextState->GetKind() != m_pState->GetKind())
	{
		m_isUpFov = false;
		//Stateを変更する
		m_pState = m_pState->m_nextState;
		m_pState->m_nextState = m_pState;
	}
#ifdef _DEBUG
	std::string stateKind = "empty";

	if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kAttack)
	{
		stateKind = "Attack\n";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kCharge)
	{
		stateKind = "Charge\n";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kDodge)
	{
		stateKind = "Dodge\n";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kGuard)
	{
		stateKind = "Guard\n";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kHitAttack)
	{
		stateKind = "HitAttack\n";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kIdle)
	{
		stateKind = "Idle\n";
	}
	else if (m_pState->GetKind() == PlayerStateBase::PlayerStateKind::kMove)
	{
		stateKind = "Move\n";
	}

	printfDx(stateKind.c_str());
#endif // _DEBUG

	m_isOpenSpecialPallet = input.IsPress(Game::InputId::kLb);

	//Stateの更新
	m_pState->Update(input);

	//当たり判定の更新
	auto colData = std::dynamic_pointer_cast<CapsuleColliderData>(m_pColData);
	//当たり判定の縦幅
	MyEngine::Vector3 colPos = m_rigidbody.GetPos();
	colPos.y += kColScale;
	//当たり判定の座標調整
	colData->m_startPos = colPos;
	//ハンドルの座標を設定する
	MV1SetPosition(m_modelHandle, m_rigidbody.GetPos().CastVECTOR());
	//体力が0以下になるとゲームオーバー
	if (m_nowHp <= 0)
	{
		scene->GameOver();
	}
}

void Player::Draw()
{
	MV1DrawModel(m_modelHandle);
}

bool Player::GetAttackKind(std::string attackId)
{
	return m_attackData[attackId].isEnergy;
}

void Player::OnCollide(std::shared_ptr<Collidable> collider)
{
	if (collider->GetTag() == ObjectTag::kEnemyAttack)
	{
		auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
		int damage = static_cast<int>(m_pState->OnDamage(collider) - GetRand(m_status.def));
		if (damage < 0)
		{
			damage = kMinDamage;
		}

		m_nowHp -= damage;
		if (m_nowHp < 0)
		{
			m_nowHp = 0;
		}
		//UIに受けたダメージを送る
		m_pUi->AddShowDamage(m_rigidbody.GetPos(), damage, true);
	}
}

std::map<std::string, Ui::SpecialAttackCommandInfo> Player::GetSetSpecialAttackInfo()
{
	std::map<std::string, Ui::SpecialAttackCommandInfo> ans;

	ans[Game::InputId::kA].name = m_attackData[m_setSpecialAttackId[Game::InputId::kA]].name;
	ans[Game::InputId::kA].cost = m_attackData[m_setSpecialAttackId[Game::InputId::kA]].cost;
	ans[Game::InputId::kB].name = m_attackData[m_setSpecialAttackId[Game::InputId::kB]].name;
	ans[Game::InputId::kB].cost = m_attackData[m_setSpecialAttackId[Game::InputId::kB]].cost;
	ans[Game::InputId::kX].name = m_attackData[m_setSpecialAttackId[Game::InputId::kX]].name;
	ans[Game::InputId::kX].cost = m_attackData[m_setSpecialAttackId[Game::InputId::kX]].cost;
	ans[Game::InputId::kY].name = m_attackData[m_setSpecialAttackId[Game::InputId::kY]].name;
	ans[Game::InputId::kY].cost = m_attackData[m_setSpecialAttackId[Game::InputId::kY]].cost;

	return ans;
}

std::shared_ptr<AttackBase> Player::CreateAttack(std::string id, MyEngine::Vector3 laserTargetPos)
{
	//エネミーの攻撃を作成
	std::shared_ptr<AttackBase> ans = std::make_shared<AttackBase>(ObjectTag::kPlayerAttack);

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

void Player::ChangeState(std::shared_ptr<PlayerStateBase> state)
{
	m_pState = state;
}

void Player::SetSpecialAttack()
{
	m_setSpecialAttackId["A"] = CommandId::kSpEnergyAttack;
	m_setSpecialAttackId["B"] = CommandId::kSpStanAttack;
	m_setSpecialAttackId["X"] = CommandId::kSpSlamAttack;
	m_setSpecialAttackId["Y"] = CommandId::kSpLaserAttack;

}
