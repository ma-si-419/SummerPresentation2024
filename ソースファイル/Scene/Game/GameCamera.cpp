#include "GameCamera.h"
#include <cmath>
namespace
{
	//�J�����ƃv���C���[�̋���
	constexpr float kPlayerDistance = 24.0f;
	//�J�����̈ړ����x(�����̔{��)
	constexpr float kCameraSpeed = 0.001f;
	//�J�����̍���
	constexpr float kCameraHeight = 5.0f;
	//��]�����Ȃ��P�\�͈�
	constexpr float kRotaMargin = 60.0f;
	//�J�����̏����ʒu
	MyEngine::Vector3 kInitPos(0, -50, -100);
	//��{�I�ȃJ�����̉�]�
	constexpr float kCameraRotaY = 0.5f;
	//�J�����̉�]��������ɂ���ĉ�
	constexpr float kDistanceRotaYDecayRate = 0.1f;
	//�J�����̏�������p(60�x)
	constexpr float kInitCameraFov = 1.046f;
	//�J�������L�������̎���p(80�x)
	constexpr float kWideCameraFov = 1.395f;
	//�J�����̎���p���g�k���鑬�x
	constexpr float kFovScalingSpeed = (kWideCameraFov - kInitCameraFov) * 0.05f;
	//�J������������Ƃ��̏㏸���x
	constexpr float kCameraUpSpeed = 0.1f;
	//�J�����̍ő�㏸�l
	constexpr float kCameraMaxUpPos = 1.0f;
	//�X�J�C�h�[���̑傫��
	constexpr float kSkyDomeScale = 8;
	//�v���C���[�ƓG�̊Ԃ̂ǂ̓���������_�̐^�񒆂ɂ��邩
	constexpr float kCameraTargetPosRate = 0.5f;
	//�J�����̃j�A
	constexpr float kCameraNear = 1.0f;
	//�J�����̃t�@�[
	constexpr float kCameraFar = 7500.0f;
}

GameCamera::GameCamera() :
	m_playerRota(0),
	m_fov(kInitCameraFov),
	m_cameraUpPos(0)
{
	SetCameraNearFar(kCameraNear, kCameraFar);
	m_domeHandle = MV1LoadModel("data/model/Dome.mv1");
	MV1SetScale(m_domeHandle, VGet(kSkyDomeScale, kSkyDomeScale, kSkyDomeScale));
}

GameCamera::~GameCamera()
{
	MV1DeleteModel(m_domeHandle);
}

void GameCamera::Init()
{
	//���C�g�̍쐬
	m_lightHandle = CreateDirLightHandle(VGet(0, 0, 1));

	m_playerPos.y += kCameraHeight + m_cameraUpPos;

	MyEngine::Vector3 enemyToPlayer = m_playerPos - m_targetPos;

	float vecSize = enemyToPlayer.Length();

	MyEngine::Vector3 unitVec = enemyToPlayer.Normalize();

	m_cameraPos.x = enemyToPlayer.x + kPlayerDistance * unitVec.x;
	m_cameraPos.y = enemyToPlayer.y + kPlayerDistance * unitVec.y;
	m_cameraPos.z = enemyToPlayer.z + kPlayerDistance * unitVec.z;

	MATRIX mat = MGetRotY(kCameraRotaY / (vecSize * kDistanceRotaYDecayRate));

	m_cameraPos = m_cameraPos.MatTransform(mat);

	m_cameraPos += m_targetPos;

	//�J�����̃^�[�Q�b�g���W���쐬
	MyEngine::Vector3 cameraTarget = m_playerPos + (m_targetPos - m_playerPos) * kCameraTargetPosRate;

	//�����_�̕\���̂���
	m_playerVelo = cameraTarget;

	//�J�����̍��W��ݒ�
	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), cameraTarget.CastVECTOR());

}

void GameCamera::Update()
{

	if (m_isUpFov)
	{
		m_fov += kFovScalingSpeed;
		m_cameraUpPos += kCameraUpSpeed;

		if (m_fov > kWideCameraFov)
		{
			m_fov = kWideCameraFov;
		}
		if (m_cameraUpPos > kCameraMaxUpPos)
		{
			m_cameraUpPos = kCameraMaxUpPos;
		}
	}
	else
	{
		m_fov -= kFovScalingSpeed;
		m_cameraUpPos -= kCameraUpSpeed;
		if (m_fov < kInitCameraFov)
		{
			m_fov = kInitCameraFov;
		}
		if (m_cameraUpPos < 0)
		{
			m_cameraUpPos = 0;
		}
	}

	SetupCamera_Perspective(m_fov);

	m_playerPos.y += kCameraHeight + m_cameraUpPos;

	MyEngine::Vector3 enemyToPlayer = m_playerPos - m_targetPos;

	float vecSize = enemyToPlayer.Length();

	MyEngine::Vector3 unitVec = enemyToPlayer.Normalize();

	m_cameraPos.x = enemyToPlayer.x + kPlayerDistance * unitVec.x;
	m_cameraPos.y = enemyToPlayer.y + kPlayerDistance * unitVec.y;
	m_cameraPos.z = enemyToPlayer.z + kPlayerDistance * unitVec.z;

	MATRIX mat = MGetRotY(kCameraRotaY / (vecSize * kDistanceRotaYDecayRate));

	m_cameraPos = m_cameraPos.MatTransform(mat);

	m_cameraPos += m_targetPos;

	//�J�����̃^�[�Q�b�g���W���쐬
	MyEngine::Vector3 cameraTarget = m_playerPos + (m_targetPos - m_playerPos) * kCameraTargetPosRate;

	//�����_�̕\���̂���
	m_playerVelo = cameraTarget;

	//�J�����̍��W��ݒ�
	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), cameraTarget.CastVECTOR());
	//����p���L���邩�ǂ�����false�ɂ���
	m_isUpFov = false;
	//�J�����̍��W�ɃX�J�C�h�[����ݒ�
	MV1SetPosition(m_domeHandle,VGet(0,0,0));

	//�J�����̌��Ă�������Ƀf�B���N�V�������C�g��ݒ�
	SetLightDirectionHandle(m_lightHandle, (m_targetPos - m_cameraPos).Normalize().CastVECTOR());
}