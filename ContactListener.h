#ifndef __CONTACT_LISTENER_H__
#define __CONTACT_LISTENER_H__

#include "cocos2d.h"  
#include "Box2D/Box2D.h"  

class ContactListener : public b2ContactListener
{
private:
	//两个物体开始接触时会响应  
	virtual void BeginContact(b2Contact* contact);

	//持续接触时响应  
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	//持续接触时响应，调用完preSolve后调用  
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	//分离时响应  
	virtual void EndContact(b2Contact* contact);

private:
	b2World *m_world;
};

#endif