#pragma once
#include "SceneManager.h"
#include "DataManager.h"
#include "SoundManager.h"
class SceneBase
{
public:
	SceneBase(SceneManager& sceneManager,DataManager& dataManager,SoundManager& soundManager);
	virtual ~SceneBase() {}
	//初期化処理
	virtual void Init() = 0;
	//更新処理
	virtual void Update(MyEngine::Input input) = 0;
	//描画処理
	virtual void Draw() = 0;
	//シーンを抜けるときの処理
	virtual void End() = 0;
protected:

	SceneManager& m_sceneManager;
	DataManager& m_dataManager;
	SoundManager& m_soundManager;
};