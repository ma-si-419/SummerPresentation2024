#include "PlayerStateMove.h"
#include "PlayerStateIdle.h"
#include "PlayerStateGuard.h"
#include "PlayerStateDodge.h"
#include "PlayerStateHitAttack.h"
#include "PlayerStateCharge.h"
#include "PlayerStateAttack.h"
#include "Player.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"


namespace
{
	//移動速度
	constexpr float kMoveSpeed = 1.0f;
	//敵との一番近い距離(真上にいかないように)
	constexpr int kShortestEnemyDistance = 15;
	//ダッシュするときの移動速度の倍率
	constexpr float kDashSpeedRate = 1.8f;
	//トリガーが反応する値
	constexpr int kTriggerReaction = 200;
}

void PlayerStateMove::Init()
{
	m_pPlayer->ChangeAnim("MoveFront");
}

void PlayerStateMove::Update(MyEngine::Input input)
{
	//Stateの変化
	//必殺技パレットを開いていない場合
	if (!input.IsPress(Game::InputId::kLb))
	{
		//気弾攻撃をした場合
		if (input.IsTrigger(Game::InputId::kX))
		{
			//一応エフェクトを消しておく
			m_pPlayer->EndEffect();
			//状態を変化させる
			m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
			state->Init(Game::InputId::kX, false);
			return;
		}
		//格闘攻撃をした場合
		if (input.IsTrigger(Game::InputId::kB))
		{
			//一応エフェクトを消しておく
			m_pPlayer->EndEffect();
			//状態を変化させる
			m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
			state->Init(Game::InputId::kB, false);
			return;
		}
		//ガード入力がされていたら
		if (input.IsPress(Game::InputId::kRb))
		{
			//一応エフェクトを消しておく
			m_pPlayer->EndEffect();
			//StateをGuardに変更する
			m_nextState = std::make_shared<PlayerStateGuard>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateGuard>(m_nextState);
			state->Init();
			return;
		}
		//チャージ入力されていたら
		if (input.IsPress(Game::InputId::kY))
		{
			//StateをChargeに変更する
			m_nextState = std::make_shared<PlayerStateCharge>(m_pPlayer, m_pScene);
			auto state = std::dynamic_pointer_cast<PlayerStateCharge>(m_nextState);
			state->Init();
			return;
		}
	}
	//必殺技パレットを開いている場合
	else
	{
		std::map<std::string, std::string> setSpecialAttack = m_pPlayer->GetSetSpecialAttackId();
		if (input.IsTrigger(Game::InputId::kY))
		{
			//MPが十分にあったら
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kY]))
			{
				//一応エフェクトを消しておく
				m_pPlayer->EndEffect();
				//状態を変化させる
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kY, true);
				return;
			}
		}
		else if (input.IsTrigger(Game::InputId::kB))
		{
			//MPが十分にあったら
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kB]))
			{
				//一応エフェクトを消しておく
				m_pPlayer->EndEffect();
				//状態を変化させる
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kB, true);
				return;
			}

		}
		else if (input.IsTrigger(Game::InputId::kX))
		{
			//MPが十分にあったら
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kX]))
			{
				//一応エフェクトを消しておく
				m_pPlayer->EndEffect();
				//状態を変化させる
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kX, true);
				return;
			}
		}
		else if (input.IsTrigger(Game::InputId::kA))
		{
			//MPが十分にあったら
			if (m_pPlayer->GetNowMp() >= m_pPlayer->GetAttackCost(setSpecialAttack[Game::InputId::kA]))
			{
				//一応エフェクトを消しておく
				m_pPlayer->EndEffect();
				//状態を変化させる
				m_nextState = std::make_shared<PlayerStateAttack>(m_pPlayer, m_pScene);
				auto state = std::dynamic_pointer_cast<PlayerStateAttack>(m_nextState);
				state->Init(Game::InputId::kA, true);
				return;
			}
		}
	}

	//上記の状態遷移に当てはまらなかったら
	//下の移動処理を行う

	//移動ベクトル
	MyEngine::Vector3 velo;

	MyEngine::Input::StickInfo stick = input.GetStickInfo();

	MyEngine::Vector3 dir(stick.leftStickX, 0, -stick.leftStickY);

	//移動入力がされているとき
	if (dir.sqLength() != 0)
	{
		dir = dir.Normalize();
	}

	//エネミーの座標
	MyEngine::Vector3 targetPos = m_pPlayer->GetTargetPos();
	//Y軸を中心とした回転をするので
	//Y座標が関係しないようにプレイヤーと同じ座標にする
	MyEngine::Vector3 rotationShaftPos = targetPos;
	rotationShaftPos.y = m_pPlayer->GetPos().y;

	//プレイヤーから回転の中心へのベクトル
	MyEngine::Vector3 toShaftPosVec = rotationShaftPos - m_pPlayer->GetPos();

	//ダッシュボタンが押されているか
	bool isDash = input.IsPress(Game::InputId::kA);

	//移動速度
	float speed = kMoveSpeed;

	//ダッシュボタンが押されていたら視野角を広げる
	m_pPlayer->SetUpFov(isDash);

	//ダッシュボタンが押されていたら
	if (isDash)
	{
		speed *= kDashSpeedRate;

		if (!m_pPlayer->GetEffectData())
		{
			MyEngine::Vector3 pos = m_pPlayer->GetPos();
			std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Dash"), pos, true);
			EffekseerManager::GetInstance().Entry(effect);
			m_pPlayer->SetEffectData(effect);
		}

		auto effect = m_pPlayer->GetEffectData();
		effect->SetPos(m_pPlayer->GetPos());
	}
	else
	{
		m_pPlayer->EndEffect();
	}
	//現状の回転度を取得する
	float x = m_pPlayer->GetPos().x - rotationShaftPos.x;
	float z = m_pPlayer->GetPos().z - rotationShaftPos.z;

	float angle = std::atan2(z, x);

	//回転の大きさ
	float hMoveScale = 0;

	//次に向かう座標
	MyEngine::Vector3 nextPos;

	//横移動
	if (dir.x != 0.0f)
	{
		//距離によって回転する大きさを変化させる
		hMoveScale = (dir.x * kMoveSpeed) / toShaftPosVec.Length();
		//ダッシュボタンを押していたら横移動の速さにダッシュの倍率をかける
		if (isDash)
		{
			hMoveScale *= kDashSpeedRate;
		}
	}

	//敵に近すぎた場合
	if (toShaftPosVec.Length() < kShortestEnemyDistance)
	{
		//前入力されている場合
		if (dir.z > 0)
		{
			//横入力の値で回る方向を決める
			if (dir.x > 0)
			{
				//前入力で回転する
				hMoveScale += (dir.z * speed) / toShaftPosVec.Length();
			}
			else
			{
				//前入力で回転する
				hMoveScale -= (dir.z * speed) / toShaftPosVec.Length();
			}
		}
		//後ろ入力されている場合
		else
		{
			nextPos += nextPos + toShaftPosVec.Normalize() * dir.z * speed;
		}
	}
	//敵から一定距離離れている場合
	else
	{
		nextPos += nextPos + toShaftPosVec.Normalize() * dir.z * speed;
	}

	//上下移動
	if (input.GetTriggerInfo().left > kTriggerReaction)
	{
		nextPos.y += kMoveSpeed;
	}
	else if (input.GetTriggerInfo().right > kTriggerReaction)
	{
		nextPos.y -= kMoveSpeed;
	}


	//現在の角度に横移動の大きさを足す
	angle += hMoveScale;

	nextPos.x += cosf(angle) * toShaftPosVec.Length() + rotationShaftPos.x;
	nextPos.y += m_pPlayer->GetPos().y;
	nextPos.z += sinf(angle) * toShaftPosVec.Length() + rotationShaftPos.z;



	velo = nextPos - m_pPlayer->GetPos();

	//必殺技パレットが開かれていなく
	if (!input.IsPress(Game::InputId::kLb))
	{
		//回避行動の入力がされたら
		if (input.IsTrigger(Game::InputId::kA))
		{
			//一応エフェクトを消しておく
			m_pPlayer->EndEffect();
			//StateをDodgeに変更する
			m_nextState = std::make_shared<PlayerStateDodge>(m_pPlayer, m_pScene);
			//回避の方向を設定する
			auto state = std::dynamic_pointer_cast<PlayerStateDodge>(m_nextState);
			state->Init(dir);
			return;
		}
	}

	m_pPlayer->PlayAnim();
	m_pPlayer->SetVelo(velo);

	//正面の座標
	MyEngine::Vector3 frontPos = m_pPlayer->GetPos() + velo;
	//上下移動を行う際は敵の方を向きながら動くようにする
	if (frontPos.y != m_pPlayer->GetPos().y)
	{
		frontPos = m_pPlayer->GetTargetPos();
	}



	//移動してなければ
	if (velo.sqLength() < 0.001f)
	{
		//ターゲットの座標を向くようにする
		m_pPlayer->SetModelFront(m_pPlayer->GetTargetPos());
		m_nextState = std::make_shared<PlayerStateIdle>(m_pPlayer, m_pScene);
		auto state = std::dynamic_pointer_cast<PlayerStateIdle>(m_nextState);
		state->Init();
		return;
	}
	//移動していれば
	else
	{
		//移動先の座標を向くようにする
		m_pPlayer->SetModelFront(frontPos);
		//自分のポインタを返す
		m_nextState = shared_from_this();
		return;
	}


}

int PlayerStateMove::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそのまま渡す
	damage = attack->GetDamage();
	//状態を変化させる
	m_nextState = std::make_shared<PlayerStateHitAttack>(m_pPlayer, m_pScene);
	//受けた攻撃の種類を設定する
	auto state = std::dynamic_pointer_cast<PlayerStateHitAttack>(m_nextState);
	state->Init(collider);
	//ヒットエフェクトを表示する
	m_pPlayer->EndEffect();
	MyEngine::Vector3 pos = m_pPlayer->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Hit"), pos, false);
	EffekseerManager::GetInstance().Entry(effect);

	return damage;
}