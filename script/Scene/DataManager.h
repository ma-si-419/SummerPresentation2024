#pragma once
#include "Game.h"
#include <vector>
#include <string>
#include <map>
#include <memory>

class LoadCsv;
class DataManager
{
public:
	struct UiInfo
	{
		std::string path = "empty";
		float posX = 0;
		float posY = 0;
	};
	struct AttackInfo
	{
		std::string name = "empty";
		int cost = 0;
		float damageRate = 0;
		int stanDamage = 0;
		int damage = 0;
		float speed = 0;
		float radius = 0;
		int lifeTime = 0;
		int attackNum = 0;
		bool isLaser = false;
		bool isScatter = false;
		bool isTrack = false;
		int attackStartTime = 0;
		int attackEndTime = 0;
		int actionTotalTime = 0;
		bool isEnergy = false;
		int hitEffect = 0;
		bool isSpecial = false;
		std::string effekseerName = "empty";
		std::string animationName = "empty";
		std::string soundName = "empty";
	};

	DataManager();
	~DataManager();

	void Init();

	void LoadAttackFile();
	void LoadUiFile();
	void LoadAiFile();
	void LoadAnimationFile();
	void LoadSoundFile();

	//�U���̃f�[�^���擾
	std::map<std::string, AttackInfo> GetAttackData() { return m_attackData; };
	//���y�̃f�[�^���擾
	std::vector<std::string> GetSoundData(Game::SceneNum sceneNum);
	//�Q�[���V�[����UI�̃p�X�擾
	std::vector<UiInfo> GetUiData(Game::SceneNum sceneNum);
	//�G�l�~�[��AI�擾
	std::map<std::string, std::vector<int>> GetAiData() { return m_aiData; };
	//�A�j���[�V�����̃f�[�^�擾
	std::vector<std::vector<std::string>> GetAnimationData() { return m_LoadAnimationData; }
private:

	//�t�@�C������ǂݍ��񂾃A�j���[�V�����f�[�^�ۑ���
	std::vector<std::vector<std::string>> m_LoadAnimationData;

	//�U���̃f�[�^
	std::map<std::string, AttackInfo> m_attackData;
	//�����̃f�[�^
	std::vector<std::string> m_sceneTitleSoundData;
	std::vector<std::string> m_sceneGameSoundData;
	//�摜�̃f�[�^
	std::vector<UiInfo> m_sceneGameUiData;
	std::vector<UiInfo> m_sceneTitleUiData;
	std::vector<UiInfo> m_sceneSelectUiData;
	//Ai�̃f�[�^
	std::map<std::string, std::vector<int>> m_aiData;

	//�t�@�C�����[�h�ł���N���X
	std::shared_ptr<LoadCsv> m_pLoadCsv;
};

