#include "SceneManager.h"
#include "SceneTitle.h"
#include "DxLib.h"

namespace
{
	constexpr int kFadeSpeed = 3;
}

SceneManager::SceneManager() :
	m_isEnd(false),
	m_isFadeOut(false),
	m_fadeAlpha(0),
	m_isChangeScene(false)
{
}

SceneManager::~SceneManager()
{
}

void SceneManager::Init(std::shared_ptr<SceneBase> firstScene)
{
	m_pScene = firstScene;
	m_pScene->Init();
}

void SceneManager::Update(MyEngine::Input input)
{

	//�V�[���؂�ւ��t���O�������Ă�����
	if (m_isChangeScene)
	{
		//�t�F�[�h�A�E�g����
		if (m_isFadeOut)
		{
			m_fadeAlpha += kFadeSpeed;
			if (m_fadeAlpha > 255)
			{
				m_pScene->End();
				m_pScene = m_pNextScene;
				m_pScene->Init();
				m_isFadeOut = false;
			}
		}
		//�t�F�[�h�C������
		else
		{
			m_fadeAlpha -= kFadeSpeed;
			if (m_fadeAlpha < 0)
			{
				m_fadeAlpha = 0;
				m_isChangeScene = false;
			}
		}
		//�V�[���؂�ւ����n�܂�������͂������Ȃ��悤�ɂ���
		MyEngine::Input none;
		m_pScene->Update(none);
	}
	//�V�[���؂�ւ����Ă��Ȃ��Ƃ��̏���
	else
	{
		m_pScene->Update(input);
	}

}

void SceneManager::Draw()
{
	m_pScene->Draw();

	SetDrawBlendMode(DX_BLENDMODE_ALPHA, m_fadeAlpha);
	DrawBox(0, 0, Game::kWindowWidth, Game::kWindowHeight, GetColor(0, 0, 0), true);
	SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
}
void SceneManager::ChangeScene(std::shared_ptr<SceneBase> next)
{
	if (m_pScene)
	{
		m_isChangeScene = true;
	}
	m_isFadeOut = true;
	m_pNextScene = next;

}
