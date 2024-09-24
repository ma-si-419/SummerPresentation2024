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
	//�G�t�F�N�V�A�̃n���h���������Ă���
	m_effekseerHandle = handleData.first;
	//�v���C�n���h����ۑ�����
	m_playHandle = PlayEffekseer3DEffect(handleData.first);
	//���W�̐ݒ�
	m_pos = pos;
	SetPosPlayingEffekseer3DEffect(m_playHandle, pos.x, pos.y, pos.z);
	//���[�v�̐ݒ�
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
	//�v���C���Ԃ����C�t�^�C���𒴂�����
	if (m_playTime > m_lifeTime)
	{
		//�G�t�F�N�g���~�߂�
		StopEffekseer3DEffect(m_playHandle);
		//���[�v�t���O�������Ă�����
		if (m_isLoop)
		{
			//�Đ����Ȃ���
			m_playHandle = PlayEffekseer3DEffect(m_effekseerHandle);
			m_playTime = 0;
		}
		//�t���O�������Ă��Ȃ����
		else
		{
			//�폜�t���O�𗧂Ă�
			m_isDeleteFlag = true;
		}
	}
	//���W�̐ݒ�
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
