#include "Game.h"
#include "LoadCsv.h"
#include <fstream>
#include <cassert>

//SpringGame���݂Ď�������

std::vector<std::vector<std::string>> LoadCsv::LoadFile(std::string path)
{
	//�t�@�C���ǂݍ���
	std::ifstream ifs(path.c_str());
	//�ǂݍ��߂Ȃ�������~�߂�
	if (!ifs)
	{
		assert(false);
	}
	//��s�̃f�[�^������
	std::string strBuf;
	//�R���}��؂�̃f�[�^������z��
	std::vector<std::vector<std::string>> ans;

	//��s�ڂ̃f�[�^�͂���Ȃ��̂ň�x�ǂݍ���ł���
	std::getline(ifs, strBuf);

	//�f�[�^�����ׂēǂݍ���
	while (std::getline(ifs, strBuf))
	{
		//�؂������f�[�^
		std::vector<std::string> pushData = Split(strBuf, ',');
		ans.push_back(pushData);
		////�����f�[�^
		//Game::AttackInfo pushData;

		////�����f�[�^�ɏ�������
		//pushData.cost = std::stoi(ans[static_cast<int>(Game::AttackInfoSort::kCost)]);
		//pushData.damageRate = std::stof(ans[static_cast<int>(Game::AttackInfoSort::kDamageRate)]);
		//pushData.speed = std::stof(ans[static_cast<int>(Game::AttackInfoSort::kSpeed)]);
		//pushData.radius = std::stof(ans[static_cast<int>(Game::AttackInfoSort::kRadius)]);
		//pushData.lifeTime = std::stoi(ans[static_cast<int>(Game::AttackInfoSort::kLifeTime)]);
		//pushData.attackNum = std::stoi(ans[static_cast<int>(Game::AttackInfoSort::kAttackNum)]);
		//pushData.isLaser = static_cast<bool>(std::stoi(ans[static_cast<int>(Game::AttackInfoSort::kLaser)]));
		//pushData.isScatter = static_cast<bool>(std::stoi(ans[static_cast<int>(Game::AttackInfoSort::kScatter)]));
		//pushData.isTrack = static_cast<bool>(std::stoi(ans[static_cast<int>(Game::AttackInfoSort::kTrack)]));
		//pushData.attackTime = std::stoi(ans[static_cast<int>(Game::AttackInfoSort::kAttackTime)]);
		//pushData.actionTime = std::stoi(ans[static_cast<int>(Game::AttackInfoSort::kActionTime)]);
		//pushData.isEnergy = static_cast<bool>(std::stoi(ans[static_cast<int>(Game::AttackInfoSort::kEnergy)]));
		//

		////�f�[�^������
		//m_attackData[ans[static_cast<int>(Game::AttackInfoSort::kId)]] = pushData;
	}

	return ans;
}
std::vector<std::string> LoadCsv::Split(const std::string& str, const char cutChar)
{
	std::vector<std::string> ans;

	//��؂�J�n�ʒu
	int first = 0;

	//0����ǂ��܂ŋ�؂�̂�
	int last = static_cast<int>(str.find_first_of(cutChar));

	while (first < str.size())
	{
		//��؂������̂����ʂɓ���Ă���
		std::string subStr(str, first, last - first);

		ans.push_back(subStr);

		/*��؂�ʒu�X�V*/

		//��؂�J�n�ʒu����؂�ŏI�ʒu�ɍX�V����
		first = last + 1;

		//��؂�J�n�ʒu�����؂镶����T���n�߁A��؂�ŏI�ʒu�����肷��
		last = static_cast<int>(str.find_first_of(cutChar, first));

		//��؂镶���������Ȃ�������
		if (last == std::string::npos)
		{
			//��؂�ŏI�ʒu�𕶎���̍Ō�ɂ���
			last = static_cast<int>(str.size());
		}

	}

	return ans;
}
