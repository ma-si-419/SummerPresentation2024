#pragma once
#include "Collidable.h"
class Stage : public Collidable
{
public:
	Stage(ObjectTag tag);
	virtual ~Stage();

	virtual void OnCollide(std::shared_ptr<Collidable> collider)override;
};