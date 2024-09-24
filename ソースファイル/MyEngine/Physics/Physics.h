#pragma once
#include <list>
#include <vector>
#include <memory>
#include "Collidable.h"

class Physics final
{
public:
	//���������Փ˕���o�^�E�폜����
	void Entry(std::shared_ptr<Collidable> col);
	void Exit(std::shared_ptr<Collidable> col);
	//�o�^�����Փ˕��̕����ړ��A�Փ˒ʒm���s��
	void Update();
	//�����蔻��̕\�����s��
	void DebugDraw();
private:
	std::list<std::shared_ptr<Collidable>> m_collidables; //�o�^���ꂽcollidable�̃��X�g
	void FixPosition();

	bool CheckCollide(std::shared_ptr<Collidable> first, std::shared_ptr<Collidable> second);

	// OnCollide�̏Փ˒ʒm�̂��߂̃f�[�^
	struct OnCollideInfo
	{
		std::shared_ptr<Collidable> owner;
		std::shared_ptr<Collidable> colider;
		void OnCollide() { owner->OnCollide(colider); }
	};
};

