#pragma once
#include "Vector3.h"
class TitleCamera
{
public:
	TitleCamera();
	~TitleCamera();

	void Init();

	void Update();
private:
	//�J�����̍��W
	MyEngine::Vector3 m_cameraPos;
	//��]������Ƃ��̌v�Z�Ɏg�p����ϐ�
	float m_loopCount;
	//���C�g�̃n���h��
	int m_lightHandle;

};

