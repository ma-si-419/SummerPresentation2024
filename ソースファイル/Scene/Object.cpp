#include "Object.h"

Object::Object(const TCHAR* model):
	m_modelHandle(-1)
{
	m_modelHandle = MV1LoadModel(model);
	MV1SetPosition(m_modelHandle, VGet(0,0,0));
}

Object::~Object()
{
	MV1DeleteModel(m_modelHandle);
}

void Object::SetStatus(MyEngine::Vector3 pos, MyEngine::Vector3 rota, MyEngine::Vector3 scale)
{
	//ポジションを設定
	MV1SetPosition(m_modelHandle,pos.CastVECTOR());
	//角度をラジアンに変換
	float changeRadian = DX_PI_F / 180;
	MyEngine::Vector3 rotation(rota.x * changeRadian,rota.y * changeRadian,rota.z * changeRadian);
	//ローテーションを設定
	MV1SetRotationXYZ(m_modelHandle,rotation.CastVECTOR());
	//スケールを設定
	MV1SetScale(m_modelHandle, scale.CastVECTOR());
}

void Object::Draw()
{
	MV1DrawModel(m_modelHandle);
}


void Object::Final()
{
	MV1DeleteModel(m_modelHandle);
}
