#include "EffekseerManager.h"
#include "LoadCsv.h"
#include <string>
#include "EffekseerForDXLib.h"

EffekseerManager::~EffekseerManager()
{
	for (auto& item : m_pEffekseerDatas)
	{
		item->Final();
	}
	m_pEffekseerDatas.clear();
}

void EffekseerManager::LoadData()
{
	LoadCsv load;

	//�G�t�F�N�g�̃f�[�^�ǂݍ���
	std::vector<std::vector<std::string>> loadEffectData = load.LoadFile("data/csv/effekseerHandleData.csv");

	for (auto& item : loadEffectData)
	{
		std::string dataName = "data/effekseer/" + item[static_cast<int>(Game::EffeckInfoSort::kFileName)] + ".efk";

		//�n���h�������[�h���ă}�b�v�ɓo�^
		m_effekseerHandles[item[static_cast<int>(Game::EffeckInfoSort::kName)]].first = LoadEffekseerEffect(dataName.c_str(), stof(item[static_cast<int>(Game::EffeckInfoSort::kMag)]));
		m_effekseerHandles[item[static_cast<int>(Game::EffeckInfoSort::kName)]].second = stoi(item[static_cast<int>(Game::EffeckInfoSort::kLoopFrame)]);
	}
}

void EffekseerManager::Update()
{
	std::vector<int> deleteDatas;
	int count = 0;
	//�G�t�F�N�g�̃A�b�v�f�[�g
	for (auto& item : m_pEffekseerDatas)
	{
		item->Update();
		if (item->GetDeleteFlag())
		{
			deleteDatas.push_back(count);
		}
		count++;
	}
	//�폜�t���O�������Ă�����폜����
	auto ret = std::remove_if(m_pEffekseerDatas.begin(), m_pEffekseerDatas.end(),
		[](const auto& item)
		{
			bool isDelete = item->GetDeleteFlag();
			if (isDelete)
			{
				item->Final();
			}
			return isDelete;
		});
	m_pEffekseerDatas.erase(ret, m_pEffekseerDatas.end());
}

void EffekseerManager::Draw()
{
	DrawEffekseer3D();
}
