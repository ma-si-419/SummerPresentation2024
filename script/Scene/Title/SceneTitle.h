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
	//初期化処理
	void Init();
	//更新処理
	void Update(MyEngine::Input input);
	//描画処理
	void Draw();
	//終了処理
	void End();
	//表示するUiのロードを行う
	void LoadUiHandle(std::vector<DataManager::UiInfo> data);

private:
	struct UiStatus
	{
		//画像ハンドル
		int handle = -1;
		//表示座標
		MyEngine::Vector2 drawPos;
	};
	//表示しているUiのデータ
	std::map<std::string, UiStatus> m_showUi;
	//選択している項目
	int m_selectItem;
	//オプションを開いているか
	bool m_isOpenOption;
	//エンドウィンドウを開いているか
	bool m_isOpenEndWindow;
	//本当にゲームを終わるかどうかの確認
	bool m_isEnd;
	//スカイドームのモデルハンドル
	int m_domeHandle;
	//プレイヤーのモデルハンドル
	int m_playerHandle;
	//エネミーのモデルハンドル
	int m_enemyHandle;
	//フォントハンドル
	int m_fontHandle;
	//選択している矢印を動かす際に使用する変数
	float m_shakeArrowNum;
	//カメラ
	std::shared_ptr<TitleCamera> m_pTitleCamera;
	
};