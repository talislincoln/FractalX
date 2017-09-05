#ifndef _ISYSTEM_H
#define _ISYSTEM_H

class ISystem
{
protected:
	virtual bool Init() = 0;
	virtual void Update(float dt) = 0;
	virtual void Render(float dt) = 0;
};

#endif
