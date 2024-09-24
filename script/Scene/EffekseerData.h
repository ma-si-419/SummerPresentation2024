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


	//���W
	MyEngine::Vector3 m_pos;
	//�G�t�F�N�g�̃n���h��
	int m_effekseerHandle;
	//�v���C���̃G�t�F�N�g�̃n���h��
	int m_playHandle;
	//�Đ����Ă��鎞��
	int m_playTime;
	//���C�t�^�C��
	int m_lifeTime;
	//���[�v���邩
	bool m_isLoop;
	//�`�悷�邩
	bool m_isDraw;
	//�폜���邩
	bool m_isDeleteFlag;
};

