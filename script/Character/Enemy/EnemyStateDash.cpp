#include "EnemyStateDash.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"

namespace
{
	//最低何フレームこのStateでいるか
	constexpr int kShortestTime = 120;
	//MoveBack時何フレームこのStateでいるか
	constexpr int kMoveBackTime = 60;
	//上下移動を優先するY座標のずれの大きさ
	constexpr float kYGapScale = 3;
	//近づくことを優先し始める距離
	constexpr float kMoveFrontDistance = 50;
	//離れることを優先し始める距離
	constexpr float kMoveBackDistance = 15;
	//離れている距離のまま優先度を上げないように、距離に割合をかけて優先度に変換する
	constexpr float kDistanceRate = 0.005f;
	//基本的な移動方向の割合
	constexpr int kMoveDirRate[3] = { 35,10,30 };
	//移動速度
	constexpr float kMoveSpeed = 2.0f;
	//動きの方向の数
	constexpr int kMoveDirNum = 8;
	//動きの方向の数の半分
	constexpr int kMoveDirNumHalf = kMoveDirNum * 0.5;
	//プレイヤーに近づきすぎないように
	constexpr float kPlayerDistance = 15.0f;
}

void EnemyStateDash::Init(MyEngine::Vector3 playerPos)
{
	//playerの座標に合わせて動く方向を決定する

	//移動方向の割合
	int moveRate[static_cast<int>(MoveKind::kMoveKindNum)] = {};
	//割合を初期化する
	moveRate[static_cast<int>(MoveKind::kFront)] = kMoveDirRate[static_cast<int>(MoveKind::kFront)];
	moveRate[static_cast<int>(MoveKind::kBack)] = kMoveDirRate[static_cast<int>(MoveKind::kBack)];
	moveRate[static_cast<int>(MoveKind::kRandom)] = kMoveDirRate[static_cast<int>(MoveKind::kRandom)];


	//Y座標がずれていたらできるだけ合わせるように動く
	float gapPosY = abs(m_pEnemy->GetPos().y - playerPos.y);
	//Y座標のずれが大きかったら上下移動するようにする
	if (gapPosY > kYGapScale)
	{
		m_isMoveVertical = true;
	}
	//プレイヤーとの距離が遠ければ遠いほどプレイヤーに向かっていく確率を上げる
	float distance = (m_pEnemy->GetPos() - playerPos).Length();
	//一定よりも距離が遠ければ近づく優先度を上げる
	if (distance > kMoveFrontDistance)
	{
		//優先度の増加量
		int addRate = static_cast<int>((distance - kMoveFrontDistance) * kDistanceRate);

		moveRate[static_cast<int>(MoveKind::kFront)] += addRate;
	}
	//一定よりも距離が近ければ離れるようにする
	else if (distance < kMoveBackDistance)
	{
		//ほかの選択肢を消す
		moveRate[static_cast<int>(MoveKind::kFront)] = 0;
		moveRate[static_cast<int>(MoveKind::kRandom)] = 0;
	}

	//割合をすべて足した値
	int total = 0;

	for (auto item : moveRate)
	{
		total += item;
	}

	//すべて足した値をランダムの範囲にする
	int ans = GetRand(total);
	int moveKind = 0;

	//割合を引いていき、0以下になった時の行動に決定する
	for (auto item : moveRate)
	{
		ans -= item;
		if (ans <= 0)
		{
			break;
		}
		moveKind++;
	}


	//移動方向
	MyEngine::Vector3 moveDir;

	//プレイヤーに向かっていく
	if (moveKind == static_cast<int>(MoveKind::kFront))
	{
		moveDir = (playerPos - m_pEnemy->GetPos()).Normalize();

		m_moveKind = MoveKind::kFront;
	}
	//プレイヤーから離れる
	else if (moveKind == static_cast<int>(MoveKind::kBack))
	{
		moveDir = (m_pEnemy->GetPos() - playerPos).Normalize();
		//上下移動フラグが立っていたら
		if (m_isMoveVertical)
		{
			//プレイヤーとY座標を合わせに行く
			moveDir.y *= -1;
		}
		m_moveKind = MoveKind::kBack;
	}
	//ランダムな方向に移動する
	else if (moveKind == static_cast<int>(MoveKind::kRandom))
	{
		moveDir = MyEngine::Vector3(GetRand(kMoveDirNum) - kMoveDirNumHalf, 0, GetRand(kMoveDirNum) - kMoveDirNumHalf);
		//移動方向のランダムがどちらも0で出たら
		if (moveDir.sqLength() == 0)
		{
			moveDir = MyEngine::Vector3(0, 0, 1);
		}
		moveDir = moveDir.Normalize();
		//上下移動フラグが立っていたら
		if (m_isMoveVertical)
		{
			//前後横移動なしで上下移動する
			MyEngine::Vector3 targetPos = playerPos;
			//前後横成分をエネミーの現在地と同じにする
			targetPos.x = m_pEnemy->GetPos().x;
			targetPos.z = m_pEnemy->GetPos().z;

			//上下移動成分のみの方向ベクトルを作成
			moveDir = (targetPos - m_pEnemy->GetPos()).Normalize();
		}
		m_moveKind = MoveKind::kRandom;
	}

	//最初の座標を保存する
	m_initPos = m_pEnemy->GetPos();
	//ターゲット座標を保存する
	m_targetPos = playerPos;

	//移動ベクトル
	m_velo = moveDir * kMoveSpeed;

	m_pEnemy->ChangeAnim("Move");
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Dash"), m_pEnemy->GetPos(), true);
	EffekseerManager::GetInstance().Entry(effect);
	m_pEnemy->SetEffectData(effect);

}

void EnemyStateDash::Update()
{
	//経過時間を計る
	m_time++;

	//最初の座標からターゲット座標まで移動したら
	if (m_moveKind == MoveKind::kFront && (m_targetPos - m_initPos).Length() - kPlayerDistance < (m_pEnemy->GetPos() - m_initPos).Length())
	{
		//移動をやめて別のStateに行く
		m_velo = MyEngine::Vector3(0, 0, 0);
		m_isChangeState = true;
		m_pEnemy->EndEffect();
	}
	//プレイヤーに近づきすぎないように一定距離まで来たら別のStateに行く
	if ((m_pEnemy->GetTargetPos() - m_pEnemy->GetPos()).Length() < kPlayerDistance)
	{
		//移動をやめて別のStateに行く
		m_velo = MyEngine::Vector3(0, 0, 0);
		m_isChangeState = true;
		m_pEnemy->EndEffect();
	}

	m_pEnemy->SetModelFront(m_velo + m_pEnemy->GetPos());
	m_isLookPlayer = false;

	m_pEnemy->PlayAnim();

	//Initで決定したベクトルで移動する
	m_pEnemy->SetVelo(m_velo);

	//エフェクトが設定されていたら
	if (m_pEnemy->GetEffectData())
	{
		auto effect = m_pEnemy->GetEffectData();

		effect->SetPos(m_pEnemy->GetPos());
	}

	int random = 0;

	//このフレームにいる最低時間を超えたら確率で別のフレームに行く

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
		m_pEnemy->EndEffect();
	}
}

int EnemyStateDash::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ダメージ
	int damage = 0;
	//攻撃のポインタ
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ダメージをそのまま渡す
	damage = attack->GetDamage() - GetRand(static_cast<int>(m_pEnemy->GetStatus().def));
	//受けた攻撃の種類を設定する
	m_hitEffect = attack->GetHitEffect();
	m_isChangeState = true;
	m_pEnemy->EndEffect();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Hit"), m_pEnemy->GetPos(), false);
	EffekseerManager::GetInstance().Entry(effect);
	m_pEnemy->SetEffectData(effect);
	return damage;
}
