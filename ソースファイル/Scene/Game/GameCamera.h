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
	//スカイドームのハンドル
	int m_domeHandle;
	//ライトのハンドル
	int m_lightHandle;

	//カメラの座標
	MyEngine::Vector3 m_cameraPos;
	//プレイヤーの座標
	MyEngine::Vector3 m_playerPos;
	//ターゲットの座標
	MyEngine::Vector3 m_targetPos;
	//プレイヤーの移動ベクトル
	MyEngine::Vector3 m_playerVelo;
	//プレイヤーの回転行列
	MATRIX m_playerRotaMat;
	//プレイヤーがターゲットを中心にどのくらい回転しているのか
	float m_playerRota;
	//視野角を広げるフラグ
	bool m_isUpFov;
	//視野角
	float m_fov;
	//視野角とともにカメラの座標を上げる
	float m_cameraUpPos;


};

