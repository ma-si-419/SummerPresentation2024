#include "DxLib.h"
#include "EffekseerForDXLib.h"
#include "SceneTitle.h"
#include "SceneManager.h"
#include "DataManager.h"
#include "SoundManager.h"
#include "ObjectManager.h"
#include "EffekseerManager.h"
#include "Input.h"
#include "Game.h"
#include <cassert>
namespace
{
	constexpr int kParticleMax = 16000;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
	bool isWindow = false;

#ifdef _DEBUG

	//デバッグモード時はウィンドウモードにする
	isWindow = true;

#endif // _DEBUG


	// ウィンドウかフルスクリーンか可変にする
	ChangeWindowMode(isWindow);

	SetGraphMode(Game::kWindowWidth, Game::kWindowHeight, 16);

	SetMainWindowText("KOUMOTTO");

	SetChangeScreenModeGraphicsSystemResetFlag(false);
	if (DxLib_Init() == -1)		// ＤＸライブラリ初期化処理
	{
		return -1;			// エラーが起きたら直ちに終了
	}

	SetUseDirect3DVersion(DX_DIRECT3D_11);
	Effekseer_Init(kParticleMax);
	//Effekseer_InitDistortion();
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	Effekseer_Sync3DSetting();
	//フォントの読み込み
	LPCSTR fontPath = "data/toroman.ttf";
	if (AddFontResourceEx(fontPath, FR_PRIVATE, NULL) > 0)
	{
	}
	else
	{
		// フォント読込エラー処理
		assert(0 && "フォント読み込み失敗");
	}

	ChangeFont("アンニャントロマン", DX_CHARSET_DEFAULT);

	SetUseZBuffer3D(true);
	SetWriteZBuffer3D(true);
	SetDrawScreen(DX_SCREEN_BACK);

	SceneManager scene;
	DataManager data;
	SoundManager sound;

	data.LoadAttackFile();
	data.LoadUiFile();
	data.LoadAiFile();
	data.LoadSoundFile();
	ObjectManager::GetInstance().LoadData();
	EffekseerManager::GetInstance().LoadData();

	MyEngine::Input input;

	scene.Init(std::make_shared<SceneTitle>(scene, data, sound));

	// ゲームループ
	while (ProcessMessage() != -1)
	{
		// このフレームの開始時刻を覚えておく
		LONGLONG start = GetNowHiPerformanceCount();

		// 描画を行う前に画面をクリアする
		ClearDrawScreen();

		//入力情報の更新
		input.Update();
#ifdef _DEBUG
		{
			//セレクトボタンが押されたらウィンドウモードを切り替える
			if (input.IsTrigger("SELECT"))
			{
				isWindow = !isWindow;
				ChangeWindowMode(isWindow);
			}
		}
#endif
		//シーンの更新
		scene.Update(input);
		//エフェクトの更新
		EffekseerManager::GetInstance().Update();
		//シーンの描画
		Effekseer_Sync3DSetting();
		scene.Draw();

		// 画面が切り替わるのを待つ
		ScreenFlip();

		// escキーでゲーム終了
		if (scene.GetEnd())
		{
			break;
		}

		// FPS60に固定する
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16.66ミリ秒(16667マイクロ秒)経過するまで待つ
		}
	}

	Effkseer_End();

	DxLib_End();				// ＤＸライブラリ使用の終了処理

	return 0;				// ソフトの終了 
}