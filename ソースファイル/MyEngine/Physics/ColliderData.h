#pragma once
class ColliderData abstract
{
public:
	enum class Kind
	{
		kSphere,
		kCapsule
	};

	ColliderData(Kind kind) { m_kind = kind; }
	virtual ~ColliderData(){}


	//�����蔻���ʎ擾
	Kind GetKind()const { return m_kind; }

private:
	//�����蔻��̎��
	Kind m_kind;

};

