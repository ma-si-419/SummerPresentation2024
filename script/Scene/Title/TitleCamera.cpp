#include "TitleCamera.h"
#include <cmath>
namespace
{
	//��]����~�̑傫��
	constexpr float kLoopRadius = 20.0f;
	//�J�����̉�]���x
	constexpr float kCameraSpeed = 0.01f;
}
TitleCamera::TitleCamera():
	m_loopCount(0),
	m_lightHandle(-1)
{
	SetCameraNearFar(1.5f, 1000.0f);
}

TitleCamera::~TitleCamera()
{
}

void TitleCamera::Init()
{
	//���C�g�̍쐬
	m_lightHandle = CreateDirLightHandle(VGet(0, 0, 1));

	m_cameraPos = MyEngine::Vector3(kLoopRadius, 0.0f, kLoopRadius);
}

void TitleCamera::Update()
{
	m_loopCount += kCameraSpeed;
	m_cameraPos = MyEngine::Vector3(sinf(m_loopCount) * kLoopRadius, 0.0f, cosf(m_loopCount) * kLoopRadius);
	
	MyEngine::Vector3 targetPos(0, 0, 0);

	SetCameraPositionAndTarget_UpVecY(m_cameraPos.CastVECTOR(), targetPos.CastVECTOR());

	MyEngine::Vector3 cameraDir = (targetPos - m_cameraPos).Normalize();

	//�J�����̌��Ă�������Ƀf�B���N�V�������C�g��ݒ�
	SetLightDirectionHandle(m_lightHandle, cameraDir.CastVECTOR());
}
