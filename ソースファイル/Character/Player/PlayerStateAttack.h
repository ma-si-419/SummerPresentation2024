#pragma once
#include "PlayerStateBase.h"

class PlayerStateAttack : public PlayerStateBase
{
public:
	PlayerStateAttack(std::shared_ptr<Player> player , std::shared_ptr<SceneGame> scene) : PlayerStateBase(player,scene) {}

	/// <summary>
	/// ����������
	/// </summary>
	/// <param name="button">�������{�^��</param>
	/// <param name="isSpecial">�K�E�Z���ǂ���</param>
	void Init(std::string button,bool isSpecial);

	virtual void Update(MyEngine::Input input) override;

	virtual PlayerStateKind GetKind()override { return PlayerStateKind::kAttack; }

	virtual int OnDamage(std::shared_ptr<Collidable> collider) override;

private:

	/// <summary>
	/// ���̒ʏ�U����ID��Ԃ�
	/// </summary>
	/// <param name="id">���o���Ă���ʏ�U��</param>
	/// <returns>���ɏo���ʏ�U��</returns>
	std::string GetNextNormalAttack(std::string id);

	//�o�ߎ���
	int m_time = 0;
	//���b�ԂŌ��̏�Ԃɖ߂邩
	int m_actionTime = 0;
	//���̋Z���o���Ă��邩
	std::string m_attackId = "empty";
	//���ɏo���\���ID
	std::string m_nextAttackId = "empty";
	//�G���߂��ɂ��邩�ǂ���
	bool m_isGoTarget = false;
	//�U���̃��[�V�����ɓ����Ă��邩�ǂ����ǂ���
	bool m_isStartAttack = false;
	//�C�e�̕K�E�Z���g������̃A�j���[�V�����ɓ����Ă��邩�ǂ���
	bool m_isEndSpecialAttackAnim = false;
	//�����Ă���U�����ʏ�U�����ǂ���
	bool m_isNormalAttack = false;
	//�U�����I��������ǂ���
	bool m_isAttackEnd = false;
	//�o�����U���̐�
	int m_popAttackNum = 0;
	//�U�����o�����^�C�~���O�̓G�̈ʒu��ۑ�����
	MyEngine::Vector3 m_laserTargetPos;
};

