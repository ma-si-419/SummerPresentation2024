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
	//カメラの座標
	MyEngine::Vector3 m_cameraPos;
	//回転させるときの計算に使用する変数
	float m_loopCount;
	//ライトのハンドル
	int m_lightHandle;

};

