#include "SoundManager.h"
#include "DxLib.h"

void SoundManager::SetHandle(std::vector<std::string> datas)
{
	//���ɂ���f�[�^�����ׂď���
	for (auto item : m_soundHandles)
	{
		DeleteSoundMem(item.second);
	}
	m_soundHandles.clear();
	for (auto item : datas)
	{
		//�V�[���Ŏg���T�E���h�t�@�C�������ׂă��[�h����
		std::string name = "data/sound/" + item + ".mp3";

		m_soundHandles[item] = LoadSoundMem(name.c_str());
	}
}

void SoundManager::Play(std::string name, int playType)
{
	PlaySoundMem(m_soundHandles[name], playType);
}

void SoundManager::Stop(std::string name)
{
	StopSoundMem(m_soundHandles[name]);
}
