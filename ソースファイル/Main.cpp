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

	//�f�o�b�O���[�h���̓E�B���h�E���[�h�ɂ���
	isWindow = true;

#endif // _DEBUG


	// �E�B���h�E���t���X�N���[�����ςɂ���
	ChangeWindowMode(isWindow);

	SetGraphMode(Game::kWindowWidth, Game::kWindowHeight, 16);

	SetMainWindowText("KOUMOTTO");

	SetChangeScreenModeGraphicsSystemResetFlag(false);
	if (DxLib_Init() == -1)		// �c�w���C�u��������������
	{
		return -1;			// �G���[���N�����璼���ɏI��
	}

	SetUseDirect3DVersion(DX_DIRECT3D_11);
	Effekseer_Init(kParticleMax);
	//Effekseer_InitDistortion();
	Effekseer_SetGraphicsDeviceLostCallbackFunctions();
	Effekseer_Sync3DSetting();
	//�t�H���g�̓ǂݍ���
	LPCSTR fontPath = "data/toroman.ttf";
	if (AddFontResourceEx(fontPath, FR_PRIVATE, NULL) > 0)
	{
	}
	else
	{
		// �t�H���g�Ǎ��G���[����
		assert(0 && "�t�H���g�ǂݍ��ݎ��s");
	}

	ChangeFont("�A���j�����g���}��", DX_CHARSET_DEFAULT);

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

	// �Q�[�����[�v
	while (ProcessMessage() != -1)
	{
		// ���̃t���[���̊J�n�������o���Ă���
		LONGLONG start = GetNowHiPerformanceCount();

		// �`����s���O�ɉ�ʂ��N���A����
		ClearDrawScreen();

		//���͏��̍X�V
		input.Update();
#ifdef _DEBUG
		{
			//�Z���N�g�{�^���������ꂽ��E�B���h�E���[�h��؂�ւ���
			if (input.IsTrigger("SELECT"))
			{
				isWindow = !isWindow;
				ChangeWindowMode(isWindow);
			}
		}
#endif
		//�V�[���̍X�V
		scene.Update(input);
		//�G�t�F�N�g�̍X�V
		EffekseerManager::GetInstance().Update();
		//�V�[���̕`��
		Effekseer_Sync3DSetting();
		scene.Draw();

		// ��ʂ��؂�ւ��̂�҂�
		ScreenFlip();

		// esc�L�[�ŃQ�[���I��
		if (scene.GetEnd())
		{
			break;
		}

		// FPS60�ɌŒ肷��
		while (GetNowHiPerformanceCount() - start < 16667)
		{
			// 16.66�~���b(16667�}�C�N���b)�o�߂���܂ő҂�
		}
	}

	Effkseer_End();

	DxLib_End();				// �c�w���C�u�����g�p�̏I������

	return 0;				// �\�t�g�̏I�� 
}