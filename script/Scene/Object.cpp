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
	//�|�W�V������ݒ�
	MV1SetPosition(m_modelHandle,pos.CastVECTOR());
	//�p�x�����W�A���ɕϊ�
	float changeRadian = DX_PI_F / 180;
	MyEngine::Vector3 rotation(rota.x * changeRadian,rota.y * changeRadian,rota.z * changeRadian);
	//���[�e�[�V������ݒ�
	MV1SetRotationXYZ(m_modelHandle,rotation.CastVECTOR());
	//�X�P�[����ݒ�
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
