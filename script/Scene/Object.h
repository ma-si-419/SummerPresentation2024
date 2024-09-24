#pragma once
#include "Vector3.h"
class Object
{
public:
	Object(const TCHAR* model);
	virtual ~Object();

	void SetStatus(MyEngine::Vector3 pos,MyEngine::Vector3 rota,MyEngine::Vector3 scale);

	void Draw();

	void Final();
protected:

	int m_modelHandle;
};