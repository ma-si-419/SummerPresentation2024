#pragma once
#include <vector>
#include <memory>
#include <map>
#include <string>
#include "EffekseerData.h"

class EffekseerManager
{
private:
	EffekseerManager() = default;
	~EffekseerManager();
	EffekseerManager(const EffekseerManager&) = delete;
	EffekseerManager& operator = (const EffekseerManager&) = delete;
	EffekseerManager(EffekseerManager&&) = delete;
	EffekseerManager& operator = (EffekseerManager&&) = delete;
public:

	static EffekseerManager& GetInstance()
	{
		static EffekseerManager instance;
		return instance;
	}

	void LoadData();

	std::pair<int, int> GetEffekseerHandleData(std::string name) { return m_effekseerHandles[name]; }

	void Entry(std::shared_ptr<EffekseerData> effectData) { m_pEffekseerDatas.push_back(effectData); }

	void Update();

	void Draw();
private:

	std::map<std::string, std::pair<int, int>> m_effekseerHandles;

	std::vector<std::shared_ptr<EffekseerData>> m_pEffekseerDatas;

};

