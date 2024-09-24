#pragma once
#include <memory>
#include <vector>
#include "LoadCsv.h"
#include "Game.h"
#include <map>
#include "Vector3.h"


class Object;
// シングルトンクラス
class ObjectManager
{
private:
	ObjectManager() = default;
	~ObjectManager();
	ObjectManager(const ObjectManager&) = delete;
	ObjectManager& operator = (const ObjectManager&) = delete;
	ObjectManager(ObjectManager&&) = delete;
	ObjectManager& operator = (ObjectManager&&) = delete;
public:

	static ObjectManager& GetInstance()
	{
		static ObjectManager instance;
		return instance;
	}

	void LoadData();

	void SetObject(Game::SceneNum scene);

	void Draw();

private:
	struct ObjectData
	{
		std::string path;
		MyEngine::Vector3 pos;
		MyEngine::Vector3 rota;
		MyEngine::Vector3 scale;
		Game::SceneNum scene;
	};

	enum class ObjectDataInfo
	{
		kName,
		kPath,
		kPosX,
		kPosY,
		kPosZ,
		kRotaX,
		kRotaY,
		kRotaZ,
		kScaleX,
		kScaleY,
		kScaleZ,
		kScene
	};

	std::map<std::string, ObjectData> m_objectDatas;

	std::vector<std::shared_ptr<Object>> m_pObjects;
};

