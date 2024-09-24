#pragma once
#include "Vector3.h"
#include <vector>

class EffekseerData
{
public:
	EffekseerData(std::pair<int, int> handleData, MyEngine::Vector3 pos, bool isLoop);
	virtual ~EffekseerData();

	void SetPos(MyEngine::Vector3 pos) { m_pos = pos; }

	void Update();

	void Draw();

	bool GetDeleteFlag() { return m_isDeleteFlag; }

	void Final();
private:


	//座標
	MyEngine::Vector3 m_pos;
	//エフェクトのハンドル
	int m_effekseerHandle;
	//プレイ中のエフェクトのハンドル
	int m_playHandle;
	//再生している時間
	int m_playTime;
	//ライフタイム
	int m_lifeTime;
	//ループするか
	bool m_isLoop;
	//描画するか
	bool m_isDraw;
	//削除するか
	bool m_isDeleteFlag;
};

