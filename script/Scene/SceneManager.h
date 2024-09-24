#pragma once
#include<memory>
#include "Input.h"
class SceneBase;
class SceneManager
{
public:
	//コンストラクタ
	SceneManager();
	//デストラクタ
	~SceneManager();

	//初期化処理
	void Init(std::shared_ptr<SceneBase> firstScene);

	void Update(MyEngine::Input input);
	//描画処理
	void Draw();
	//別のシーンに移動する際に使用する関数
	void ChangeScene(std::shared_ptr<SceneBase> next);
	//終了処理を呼ぶときに使う
	void GameEnd() { m_isEnd = true; }
	//終了処理が呼ばれたかどうか取得する
	bool GetEnd() { return m_isEnd; }
private:
	std::shared_ptr<SceneBase> m_pScene;

	std::shared_ptr<SceneBase> m_pNextScene;//次に行くシーンを保存する

	bool m_isEnd;//終了処理を呼ぶときに使う

	bool m_isFadeOut;//フェードアウトするか

	bool m_isChangeScene;//シーンを切り替えるときにtrueにする

	int m_fadeAlpha;
};