#include "EnemyStateAttack.h"
#include "CommandIdList.h"
#include "EffekseerForDXLib.h"
#include "EffekseerManager.h"

namespace
{
	//Ši“¬UŒ‚‚ğ—Dæ‚·‚é‹——£
	constexpr float kPrioritizePhysialAttackDistance = 30.0f;

	//‹C’eUŒ‚‚ğ—Dæ‚·‚é‹——£
	constexpr float kPrioritizeEnergyAttackDistance = 100.0f;

	//Ši“¬UŒ‚‚ğo‚·‹——£
	constexpr float kPhysicalAttackDistance = 15.0f;

	//Ši“¬UŒ‚‚ğo‚·‚Æ‚«‚ÌˆÚ“®‘¬“x
	constexpr float kMoveSpeed = 1.5f;

	//Ši“¬UŒ‚‚ğ—£‚ê‚Ä‚¢‚Ä‚ào‚·ŠÔ
	constexpr int kMaxMoveTime = 120;

	//Šî–{“I‚ÈUŒ‚•û–@‚ÌŠ„‡
	constexpr int kAttackKindRate[4] = { 30,30,20,20 };
	//Ši“¬UŒ‚‚ğ—Dæ‚·‚é‚Æ‚«‚ÌŠ„‡
	constexpr int kPrioritizePhysialAttackKindRate[4] = { 20,20,30,30 };
	//‹C’eUŒ‚‚ğ—Dæ‚·‚é‚Æ‚«‚ÌŠ„‡
	constexpr int kPrioritizeEnergyAttackKindRate[4] = { 50,50,0,0 };

}

void EnemyStateAttack::Init(std::shared_ptr<Player> player)
{
	//‰Šú‰»
	m_time = 0;
	m_isNearPlayer = true;
	m_pPlayer = player;


	//s“®‚ğ‚È‚É‚É‚·‚é‚©
	int attackKind = 0;

	//‹——£‚É‚æ‚Á‚Äo‚·‹Z‚Ì—Dæ“x‚ğ•Ï‰»‚³‚¹‚é
	if ((m_pPlayer->GetPos() - m_pEnemy->GetPos()).Length() > kPrioritizeEnergyAttackDistance)
	{
		//‹C’eUŒ‚‚ª—Dæ‚³‚ê‚éê‡
		int totalRate = 0;
		//Šm—¦‚ğ‚·‚×‚Ä‘«‚·
		for (auto item : kPrioritizeEnergyAttackKindRate)
		{
			totalRate += item;
		}
		int rand = GetRand(totalRate);

		for (auto item : kPrioritizeEnergyAttackKindRate)
		{
			rand -= item;
			if (rand <= 0)
			{
				break;
			}
			attackKind++;
		}
	}
	else if ((m_pPlayer->GetPos() - m_pEnemy->GetPos()).Length() > kPrioritizePhysialAttackDistance)
	{
		//Ši“¬UŒ‚‚ª—Dæ‚³‚ê‚éê‡
		int totalRate = 0;
		//Šm—¦‚ğ‚·‚×‚Ä‘«‚·
		for (auto item : kPrioritizePhysialAttackKindRate)
		{
			totalRate += item;
		}
		int rand = GetRand(totalRate);

		for (auto item : kPrioritizePhysialAttackKindRate)
		{
			rand -= item;
			if (rand <= 0)
			{
				break;
			}
			attackKind++;
		}
	}
	else
	{
		//‚Ç‚¿‚ç‚à—Dæ‚µ‚È‚¢ê‡
		int totalRate = 0;
		//Šm—¦‚ğ‚·‚×‚Ä‘«‚·
		for (auto item : kAttackKindRate)
		{
			totalRate += item;
		}
		int rand = GetRand(totalRate);

		for (auto item : kAttackKindRate)
		{
			rand -= item;
			if (rand <= 0)
			{
				break;
			}
			attackKind++;
		}
	}

	//UŒ‚‚Ìí—Ş‚ğŒ©‚ÄUŒ‚‚ğŒˆ’è‚·‚é
	if (attackKind == static_cast<int>(AttackKind::kRepeatedlyEnergy))
	{
		m_attackId = CommandId::kEnemyEnergyAttack;
		m_isNearPlayer = false;
		m_isStartAttack = true;
	}
	else if (attackKind == static_cast<int>(AttackKind::kLaser))
	{
		m_attackId = CommandId::kEnemyLaserAttack;
		m_isNearPlayer = false;
		m_isStartAttack = true;
	}
	else if (attackKind == static_cast<int>(AttackKind::kBreakAttack))
	{
		m_attackId = CommandId::kEnemySlamAttack;
		m_isNearPlayer = true;
		m_isStartAttack = false;
	}
	else if (attackKind == static_cast<int>(AttackKind::kStanAttack))
	{
		m_attackId = CommandId::kEnemyStanAttack;
		m_isNearPlayer = true;
		m_isStartAttack = false;
	}

	m_pEnemy->ChangeAnim(m_pEnemy->GetAttackData()[m_attackId].animationName);
}

void EnemyStateAttack::Update()
{
	m_time++;
	//ˆÚ“®ƒxƒNƒgƒ‹
	MyEngine::Vector3 velo;

	//“G‚ÉŒü‚©‚Á‚Ä‚¢‚­
	if (m_isNearPlayer)
	{
		velo = (m_pPlayer->GetPos() - m_pEnemy->GetPos()).Normalize() * kMoveSpeed;
	}
	//ˆê’è‹——£‚Ü‚Å‹ß‚Ã‚­‚©ˆê’èŠÔ‚½‚Á‚½‚ç
	if ((m_pPlayer->GetPos() - m_pEnemy->GetPos()).Length() < kPhysicalAttackDistance || m_time > kMaxMoveTime)
	{
		//“®‚«‚ğ~‚ß‚é
		velo = MyEngine::Vector3(0, 0, 0);
		//UŒ‚‚ğo‚µn‚ß‚é
		m_isStartAttack = true;
		//“G‚É‹ß‚Ã‚­‚Ì‚ğ‚â‚ß‚é
		m_isNearPlayer = false;
	}
	//UŒ‚‚ğo‚µ‚Ä‚¢‚éŠÔ‚ğŒv‘ª‚·‚é
	if (m_isStartAttack)
	{
		m_attackTime++;
	}

	m_pEnemy->SetVelo(velo);

	//UŒ‚‚Ìî•ñ‚ğæ“¾‚·‚é
	std::map<std::string, DataManager::AttackInfo> attackData = m_pEnemy->GetAttackData();

	//UŒ‚‚ªI‚í‚Á‚Ä‚¢‚È‚¢‚É
	if (!m_isAttackEnd)
	{
		//UŒ‚‚ğo‚µn‚ß‚Ä‰½ƒtƒŒ[ƒ€‚©‚ğŒ©‚ÄUŒ‚‚Ìˆ—‚ğs‚¤
		if (m_attackTime > attackData[m_attackId].attackStartTime)
		{
			//UŒ‚‚ğ•¡”‰ño‚·‹Z‚Å‚ ‚ê‚Î
			if (attackData[m_attackId].attackNum > 1)
			{
				//UŒ‚‚ÌƒXƒpƒ“‚ğæ“¾‚·‚é
				int span = (attackData[m_attackId].attackEndTime - attackData[m_attackId].attackStartTime) / attackData[m_attackId].attackNum;
				//UŒ‚‚Ìƒ^ƒCƒ~ƒ“ƒO‚ª—ˆ‚½‚çUŒ‚‚ğo‚·‚æ‚¤‚É‚·‚é
				if (m_attackTime % span == 0 && m_attackTime < attackData[m_attackId].attackEndTime)
				{
					//ƒŒ[ƒU[‚ğo‚·À•W‚ªw’è‚³‚ê‚Ä‚¢‚È‚©‚Á‚½‚ç
					if (m_laserTargetPos.sqLength() == 0)
					{
						m_laserTargetPos = m_pEnemy->GetTargetPos();
					}
					//UŒ‚‚ğì¬
					std::shared_ptr<AttackBase> attack = m_pEnemy->CreateAttack(m_attackId, m_laserTargetPos);
					//ƒŒ[ƒU[ó‚ÌUŒ‚‚Ìİ’è
					if (attackData[m_attackId].isLaser)
					{
						//Á‚¦‚é‚Ü‚Å‚ÌŠÔ
						int lifeTime = attackData[m_attackId].lifeTime - m_attackTime;

						attack->SetAttackTime(lifeTime);
						//ƒGƒtƒFƒNƒg‚ğc‚·‚æ‚¤‚É‚·‚é
						attack->SetLeaveEffect();
						//ˆê“x–Ú‚ÌUŒ‚ˆÈŠOƒGƒtƒFƒNƒg‚ğo‚³‚È‚¢‚æ‚¤‚É‚·‚é
						if (m_popAttackNum > 0)
						{
							attack->SetNotPopEffect();
						}

						m_popAttackNum++;
					}
					//UŒ‚‚ğo‚·
					m_pScene->AddAttack(attack);
				}
			}
			//’P”­UŒ‚‚Å‚ ‚ê‚Î
			else
			{
				//UŒ‚‚ğo‚·ŠÔ‚É‚È‚Á‚½‚ç
				if (m_attackTime > attackData[m_attackId].attackStartTime)
				{
					//UŒ‚‚ğo‚·
					m_pScene->AddAttack(m_pEnemy->CreateAttack(m_attackId, MyEngine::Vector3(0, 0, 0)));
					m_isAttackEnd = true;
				}
			}
		}
	}
	//UŒ‚‚ÌŠÔ‚ªI‚í‚Á‚½‚ç
	if (m_attackTime > attackData[m_attackId].attackEndTime && !m_isAttackEndStanAnim)
	{
		m_isAttackEndStanAnim = true;
		m_isAttackEnd = true;
		m_pEnemy->SetAttackEndAnim(static_cast<float>(attackData[m_attackId].actionTotalTime - attackData[m_attackId].attackEndTime));
	}
	//s“®‘S‘Ì‚ÌŠÔ‚ªI‚í‚Á‚½‚ç
	if (m_attackTime > attackData[m_attackId].actionTotalTime)
	{
		m_isChangeState = true;
	}

	m_pEnemy->PlayAnim();

}
int EnemyStateAttack::OnDamage(std::shared_ptr<Collidable> collider)
{
	//ƒ_ƒ[ƒW
	int damage = 0;
	//UŒ‚‚Ìƒ|ƒCƒ“ƒ^
	auto attack = std::dynamic_pointer_cast<AttackBase>(collider);
	//ƒ_ƒ[ƒW‚ğ‚»‚Ì‚Ü‚Ü“n‚·
	damage = attack->GetDamage() - GetRand(static_cast<int>(m_pEnemy->GetStatus().def));
	//Šî–{State‚Í•ÏX‚µ‚È‚¢‚ªA‚«”ò‚Î‚µ‚¾‚¯•Ï‰»‚ğó‚¯‚é
	if (attack->GetHitEffect() == static_cast<int>(EnemyStateBase::HitEffectKind::kBurst))
	{
		m_hitEffect = attack->GetHitEffect();
		m_isChangeState = true;
	}
	MyEngine::Vector3 pos = m_pEnemy->GetPos();
	std::shared_ptr<EffekseerData> effect = std::make_shared<EffekseerData>(EffekseerManager::GetInstance().GetEffekseerHandleData("Hit"),pos,false);
	EffekseerManager::GetInstance().Entry(effect);
	return damage;
}
