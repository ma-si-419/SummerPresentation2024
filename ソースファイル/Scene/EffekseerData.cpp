#include "EffekseerData.h"
#include "EffekseerForDXLib.h"

EffekseerData::EffekseerData(std::pair<int, int> handleData, MyEngine::Vector3 pos, bool isLoop) :
	m_pos(),
	m_effekseerHandle(-1),
	m_playHandle(-1),
	m_playTime(0),
	m_lifeTime(-1),
	m_isLoop(false),
	m_isDraw(true),
	m_isDeleteFlag(false)
{
	//エフェクシアのハンドルを持っておく
	m_effekseerHandle = handleData.first;
	//プレイハンドルを保存する
	m_playHandle = PlayEffekseer3DEffect(handleData.first);
	//座標の設定
	m_pos = pos;
	SetPosPlayingEffekseer3DEffect(m_playHandle, pos.x, pos.y, pos.z);
	//ループの設定
	m_lifeTime = handleData.second;
	m_isLoop = isLoop;
}

EffekseerData::~EffekseerData()
{
	StopEffekseer3DEffect(m_playHandle);
}

void EffekseerData::Update()
{
	m_playTime++;
	//プレイ時間がライフタイムを超えたら
	if (m_playTime > m_lifeTime)
	{
		//エフェクトを止める
		StopEffekseer3DEffect(m_playHandle);
		//ループフラグが立っていたら
		if (m_isLoop)
		{
			//再生しなおす
			m_playHandle = PlayEffekseer3DEffect(m_effekseerHandle);
			m_playTime = 0;
		}
		//フラグが立っていなければ
		else
		{
			//削除フラグを立てる
			m_isDeleteFlag = true;
		}
	}
	//座標の設定
	SetPosPlayingEffekseer3DEffect(m_playHandle, m_pos.x, m_pos.y, m_pos.z);
}

void EffekseerData::Draw()
{
	if (m_isDraw)
	{
		DrawEffekseer3D_Draw(m_playHandle);
	}
}

void EffekseerData::Final()
{
	StopEffekseer3DEffect(m_playHandle);
	m_isDraw = false;
	m_isDeleteFlag = true;
}
