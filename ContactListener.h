#ifndef __CONTACT_LISTENER_H__
#define __CONTACT_LISTENER_H__

#include "cocos2d.h"  
#include "Box2D/Box2D.h"  

class ContactListener : public b2ContactListener
{
private:
	//�������忪ʼ�Ӵ�ʱ����Ӧ  
	virtual void BeginContact(b2Contact* contact);

	//�����Ӵ�ʱ��Ӧ  
	virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold);
	//�����Ӵ�ʱ��Ӧ��������preSolve�����  
	virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse);

	//����ʱ��Ӧ  
	virtual void EndContact(b2Contact* contact);

private:
	b2World *m_world;
};

#endif