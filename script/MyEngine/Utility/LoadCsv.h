#pragma once
#include "Game.h"
#include <vector>
#include <string>
#include <map>

class LoadCsv
{
public:
	std::vector<std::vector<std::string>> LoadFile(std::string path);
private:


	//�����ŋ�؂�֐�
	std::vector<std::string> Split(const std::string& str, const char cutChar);
};

