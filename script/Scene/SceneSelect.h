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
	//初期化処理
	void Init();
	//更新処理
	void Update(MyEngine::Input input);
	//描画処理
	void Draw();
	//終了処理
	void End();
private:
	//選択している難易度
	int m_selectLevel;
	//戻るかどうか
	bool m_isBack;
};

