#pragma once
#include "Vector3.h"
class GameCamera
{
public:
	GameCamera();
	~GameCamera();

	void Init();

	void Update();

	MyEngine::Vector3 GetPos() { return m_cameraPos; }

	void SetPlayerPos(MyEngine::Vector3 pos) { m_playerPos = pos; }

	void SetTargetPos(MyEngine::Vector3 pos) { m_targetPos = pos; }

	void DrawDome() {MV1DrawModel(m_domeHandle); }



	void UpFov() { m_isUpFov = true; }

	void SetPlayerRotaMat(MATRIX mat) { m_playerRotaMat = mat; }
private:
	//�X�J�C�h�[���̃n���h��
	int m_domeHandle;
	//���C�g�̃n���h��
	int m_lightHandle;

	//�J�����̍��W
	MyEngine::Vector3 m_cameraPos;
	//�v���C���[�̍��W
	MyEngine::Vector3 m_playerPos;
	//�^�[�Q�b�g�̍��W
	MyEngine::Vector3 m_targetPos;
	//�v���C���[�̈ړ��x�N�g��
	MyEngine::Vector3 m_playerVelo;
	//�v���C���[�̉�]�s��
	MATRIX m_playerRotaMat;
	//�v���C���[���^�[�Q�b�g�𒆐S�ɂǂ̂��炢��]���Ă���̂�
	float m_playerRota;
	//����p���L����t���O
	bool m_isUpFov;
	//����p
	float m_fov;
	//����p�ƂƂ��ɃJ�����̍��W���グ��
	float m_cameraUpPos;


};

