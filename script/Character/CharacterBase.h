#pragma once
#include "DxLib.h"
#include "Collidable.h"
#include "DataManager.h"
#include "Game.h"
#include <map>
#include <string>

class EffekseerData;
class AttackBase;
class Physics;
class SceneGame;
class Ui;
class CharacterBase : public Collidable
{
public:
	struct Status
	{
		float hp = 100000;
		float mp = 250;
		float atk = 200;
		float def = 300;
	};
	/// <summary>
	/// �R���X�g���N�^
	/// </summary>
	CharacterBase(const TCHAR* graph, ObjectTag tag);
	/// <summary>
	/// �f�X�g���N�^
	/// </summary>
	virtual ~CharacterBase();

	/// <summary>
	/// ����������
	/// </summary>
	virtual void Init(std::shared_ptr<Physics> physics) abstract;

	virtual void RetryInit() abstract;

	/// <summary>
	/// �`�揈��
	/// </summary>
	virtual void Draw() abstract;

	//�ő�̗͂�Ԃ�
	Status GetStatus() { return m_status; }
	//���݂̗̑͂�Ԃ�
	float GetNowHp() { return m_nowHp; }
	//���݂̋C�͗ʂ�Ԃ�
	float GetNowMp() { return m_nowMp; }
	//���b�N�I�����Ă���G�̍��W��ݒ肷��
	void SetTargetPos(MyEngine::Vector3 pos);
	//�U���̏���ݒ肷��
	void SetAttackData(std::map<std::string, DataManager::AttackInfo> data) { m_attackData = data; }
	//�����Ȃ����Ԃ�ݒ肷��
	void SetStanTime(int time) { m_stanTime = time; }
	//�U���ɕK�v�ȋC�͗ʂ��擾����
	int GetAttackCost(std::string Id) { return m_attackData[Id].cost; }
	//�A�j���[�V�����̃f�[�^���擾����
	void SetAnimationData(std::vector<std::vector<std::string>> data,bool isPlayer);
	//���ʉ���炷�@
	void PlaySE(std::string name, int playType);
	//���ʉ����~�߂�
	void StopSE(std::string name);
	//�����ɂ��Ă���G�t�F�N�g��ݒ肷��
	void SetEffectData(std::shared_ptr<EffekseerData> effect) { m_pEffectData = effect; }
	//�Đ����Ă���|�C���^��Ԃ�
	std::shared_ptr<EffekseerData> GetEffectData() { return m_pEffectData; }
	//�����ɂ��Ă���G�t�F�N�g�̍Đ����~�߂�
	void EndEffect();
	//�K�E�Z���o���Ă����Ԃɕω�������
	void PlaySpecialAttack(std::string id);
	//����������ݒ肷��
	void SetModelFront(MyEngine::Vector3 pos);
	//�A�j���[�V������ύX����
	void ChangeAnim(std::string animName);
	//�A�j���[�V�������Đ�����
	bool PlayAnim();
	//�A�j���[�V�����̃��[�v����߂�
	void StopAnimLoop();
	//�U�����I���Ƃ��̃A�j���[�V����
	void SetAttackEndAnim(float attackEndTime);
	//Ui�Ǘ��N���X�̃|�C���^��ۑ�����
	void SetUiManager(std::shared_ptr<Ui> ui) { m_pUi = ui; }
	//�V�[���̃|�C���^��ۑ�����
	void SetGameScene(std::shared_ptr<SceneGame> scene) { m_pScene = scene; }
protected:
	enum class AnimationInfoSort
	{
		kName,
		kNumber,
		kLoopFrame,
		kEndFrame,
		kPlaySpeed,
		kIsPlayer
	};
	struct AnimationInfo
	{
		int number = 0;
		int loopFrame = 0;
		int endFrame = 0;
		float playSpeed = 0.0f;
	};
	//���f���n���h��
	int m_modelHandle;
	//�o�^����Physics�������Ă���
	std::shared_ptr<Physics> m_pPhysics;
	//�V�[���̃|�C���^�������Ă���
	std::shared_ptr<SceneGame> m_pScene;
	//Ui�Ǘ��N���X�̃|�C���^�������Ă���
	std::shared_ptr<Ui> m_pUi;
	//�U���̃f�[�^
	std::map<std::string, DataManager::AttackInfo> m_attackData;
	//�A�j���[�V�����̃f�[�^
	std::map<std::string, AnimationInfo> m_animData;
	//�����ɂ��Ă���G�t�F�N�g�̃|�C���^
	std::shared_ptr<EffekseerData> m_pEffectData;
	//��{�I�ȃX�e�[�^�X
	Status m_status;
	//���݂̗̑�
	float m_nowHp;
	//���݂̋C��
	float m_nowMp;
	//�����Ȃ�����
	int m_stanTime;
	//�U�����o���Ă��邩�ǂ���
	bool m_isAttack;
	//�^�[�Q�b�g���߂��ɂ��邩�ǂ���
	bool m_isNearTarget;
	//���Đ����Ă���A�j���[�V����
	int m_playAnim;
	//�A�j���[�V�����̍Đ����x
	float m_animPlaySpeed;
	//�A�j���[�V�����̑��Đ�����
	float m_totalAnimTime;
	//�A�j���[�V�����̍Đ�����
	float m_animTime;
	//�A�j���[�V���������[�v�������ɖ߂�t���[��
	float m_animLoopStartTime;
	//�A�j���[�V�����̃��[�v����Ƃ��̃t���[��
	float m_animLoopEndTime;
	//�A�j���[�V���������[�v�����邩�ǂ���
	bool m_isLoopAnim;
	//�o���Ă���U��
	std::string m_attackId;
	//�U�����o�����^�C�~���O�̓G�̍��W
	MyEngine::Vector3 m_attackTarget;
	//���b�N�I�����Ă��鑊��̍��W
	MyEngine::Vector3 m_targetPos;
	/// <summary>
	/// �U�����쐬����
	/// </summary>
	/// <param name="id">�U����ID</param>
	/// <returns>�U���̃|�C���^</returns>
	std::shared_ptr<AttackBase> CreateAttack(std::shared_ptr<Physics> physics, std::string id, bool isPlayer);




	//�ʏ�U�����o���Ă��Ԃɕω�������
	void SetNormalAttack(bool isPhysical, int time);
};