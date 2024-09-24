#pragma once
#include "SceneBase.h"
#include "DxLib.h"
#include "Vector2.h"
class TitleCamera;
class SceneTitle : public SceneBase
{
private:
	enum class ItemKind
	{
		kStart,
		//kOption,
		kEnd
	};
	enum class OptionKind
	{
		kSE,
		kBGM,
		kWindowSize,
		kKindNum
	};
public:
	SceneTitle(SceneManager& sceneManager, DataManager& dataManager, SoundManager& soundManager);
	~SceneTitle();
	//����������
	void Init();
	//�X�V����
	void Update(MyEngine::Input input);
	//�`�揈��
	void Draw();
	//�I������
	void End();
	//�\������Ui�̃��[�h���s��
	void LoadUiHandle(std::vector<DataManager::UiInfo> data);

private:
	struct UiStatus
	{
		//�摜�n���h��
		int handle = -1;
		//�\�����W
		MyEngine::Vector2 drawPos;
	};
	//�\�����Ă���Ui�̃f�[�^
	std::map<std::string, UiStatus> m_showUi;
	//�I�����Ă��鍀��
	int m_selectItem;
	//�I�v�V�������J���Ă��邩
	bool m_isOpenOption;
	//�G���h�E�B���h�E���J���Ă��邩
	bool m_isOpenEndWindow;
	//�{���ɃQ�[�����I��邩�ǂ����̊m�F
	bool m_isEnd;
	//�X�J�C�h�[���̃��f���n���h��
	int m_domeHandle;
	//�v���C���[�̃��f���n���h��
	int m_playerHandle;
	//�G�l�~�[�̃��f���n���h��
	int m_enemyHandle;
	//�t�H���g�n���h��
	int m_fontHandle;
	//�I�����Ă�����𓮂����ۂɎg�p����ϐ�
	float m_shakeArrowNum;
	//�J����
	std::shared_ptr<TitleCamera> m_pTitleCamera;
	
};