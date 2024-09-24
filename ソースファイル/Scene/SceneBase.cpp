#include "SceneBase.h"

SceneBase::SceneBase(SceneManager& sceneManager,DataManager& dataManager, SoundManager& soundManager) :
	m_sceneManager(sceneManager),
	m_dataManager(dataManager),
	m_soundManager(soundManager)
{
}
