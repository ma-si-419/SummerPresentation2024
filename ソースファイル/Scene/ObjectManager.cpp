#include "ObjectManager.h"
#include "Object.h"
namespace
{
	const std::map<std::string, Game::SceneNum> kSceneNum =
	{
		{"Game", Game::SceneNum::kGame},
		{"Title", Game::SceneNum::kTitle}
	};
}

ObjectManager::~ObjectManager()
{
	//オブジェクトの削除
	for (auto& item : m_pObjects)
	{
		item->Final();
	}
	m_pObjects.clear();
}

void ObjectManager::LoadData()
{
	LoadCsv loadCsv;

	std::vector<std::vector<std::string>> data = loadCsv.LoadFile("data/csv/objectData.csv");

	for (auto item : data)
	{
		ObjectData pushData;

		pushData.path = item[static_cast<int>(ObjectDataInfo::kPath)];
		pushData.pos.x = std::stof(item[static_cast<int>(ObjectDataInfo::kPosX)]);
		pushData.pos.y = std::stof(item[static_cast<int>(ObjectDataInfo::kPosY)]);
		pushData.pos.z = std::stof(item[static_cast<int>(ObjectDataInfo::kPosZ)]);
		pushData.rota.x = std::stof(item[static_cast<int>(ObjectDataInfo::kRotaX)]);
		pushData.rota.y = std::stof(item[static_cast<int>(ObjectDataInfo::kRotaY)]);
		pushData.rota.z = std::stof(item[static_cast<int>(ObjectDataInfo::kRotaZ)]);
		pushData.scale.x = std::stof(item[static_cast<int>(ObjectDataInfo::kScaleX)]);
		pushData.scale.y = std::stof(item[static_cast<int>(ObjectDataInfo::kScaleY)]);
		pushData.scale.z = std::stof(item[static_cast<int>(ObjectDataInfo::kScaleZ)]);
		pushData.scene = kSceneNum.at(item[static_cast<int>(ObjectDataInfo::kScene)]);
		
		m_objectDatas[item[static_cast<int>(ObjectDataInfo::kName)]] = pushData;
	}
	
}

void ObjectManager::SetObject(Game::SceneNum scene)
{
	//オブジェクトの削除
	for (auto& item : m_pObjects)
	{
		item->Final();
	}
	m_pObjects.clear();
	
	//シーンのオブジェクトの作成
	for (auto& item : m_objectDatas)
	{
		if (item.second.scene == scene)
		{
			std::string path = "data/model/" + item.second.path + ".mv1";

			std::shared_ptr<Object> pushData = std::make_shared<Object>(path.c_str());
			
			pushData->SetStatus(item.second.pos, item.second.rota, item.second.scale);

			m_pObjects.push_back(pushData);
		}
	}

}

void ObjectManager::Draw()
{
	for (auto& item : m_pObjects)
	{
		item->Draw();
	}
}
