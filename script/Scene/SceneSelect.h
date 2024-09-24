#pragma once
#include "SceneBase.h"
class SceneSelect : public SceneBase
{
private:
	enum class LevelKind
	{
		kTutorial,
		kEasy,
		kNormal,
		kHard
	};
public:
	SceneSelect(SceneManager& sceneManager, DataManager& dataManager, SoundManager& soundManager);
	~SceneSelect();
	//����������
	void Init();
	//�X�V����
	void Update(MyEngine::Input input);
	//�`�揈��
	void Draw();
	//�I������
	void End();
private:
	//�I�����Ă����Փx
	int m_selectLevel;
	//�߂邩�ǂ���
	bool m_isBack;
};

