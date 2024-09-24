#pragma once
#include "SceneManager.h"
#include "DataManager.h"
#include "SoundManager.h"
class SceneBase
{
public:
	SceneBase(SceneManager& sceneManager,DataManager& dataManager,SoundManager& soundManager);
	virtual ~SceneBase() {}
	//����������
	virtual void Init() = 0;
	//�X�V����
	virtual void Update(MyEngine::Input input) = 0;
	//�`�揈��
	virtual void Draw() = 0;
	//�V�[���𔲂���Ƃ��̏���
	virtual void End() = 0;
protected:

	SceneManager& m_sceneManager;
	DataManager& m_dataManager;
	SoundManager& m_soundManager;
};