#include "Box2dFactory.h"
#include "cocos2d.h"
#include "VisibleRect.h"
#include "SquareNode.h"

Box2dFactory::Box2dFactory()
{

}

Box2dFactory::~Box2dFactory()
{

}

Box2dFactory *Box2dFactory::getInstance()
{
	static Box2dFactory instance;
	return &instance;
}

void Box2dFactory::initPhysics()
{
	b2Vec2 gravity;
	gravity.Set(0.0f, 0.0f);
	m_world = new b2World(gravity);

	// Do we want to let bodies sleep?
	m_world->SetAllowSleeping(true);

	m_world->SetContinuousPhysics(true);

	m_debugDraw = new GLESDebugDraw(PTM_RATIO);
	m_world->SetDebugDraw(m_debugDraw);

	m_contactListener = new ContactListener();
	m_world->SetContactListener(m_contactListener);


	uint32 flags = 0;
	flags += b2Draw::e_shapeBit;
	/* flags += b2Draw::e_jointBit;
	flags += b2Draw::e_aabbBit;
	flags += b2Draw::e_pairBit;
	flags += b2Draw::e_centerOfMassBit;*/
	m_debugDraw->SetFlags(flags);
}

void Box2dFactory::debugDraw()
{
	// IMPORTANT:
	// This is only for debug purposes
	ccGLEnableVertexAttribs(kCCVertexAttribFlag_Position);

	kmGLPushMatrix();

	m_world->DrawDebugData();

	kmGLPopMatrix();

	CHECK_GL_ERROR_DEBUG();
}

void Box2dFactory::initBorders()
{
	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0); // bottom-left corner

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2EdgeShape groundBox;

	// bottom
	groundBox.Set(b2Vec2(VisibleRect::leftBottom().x / PTM_RATIO, VisibleRect::leftBottom().y / PTM_RATIO),
		b2Vec2(VisibleRect::rightBottom().x / PTM_RATIO, VisibleRect::rightBottom().y / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

	// top
	groundBox.Set(b2Vec2(VisibleRect::leftTop().x / PTM_RATIO, VisibleRect::leftTop().y / PTM_RATIO),
		b2Vec2(VisibleRect::rightTop().x / PTM_RATIO, VisibleRect::rightTop().y / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

	// left
	groundBox.Set(b2Vec2(VisibleRect::leftTop().x / PTM_RATIO, VisibleRect::leftTop().y / PTM_RATIO),
		b2Vec2(VisibleRect::leftBottom().x / PTM_RATIO, VisibleRect::leftBottom().y / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

	// right
	groundBox.Set(b2Vec2(VisibleRect::rightBottom().x / PTM_RATIO, VisibleRect::rightBottom().y / PTM_RATIO),
		b2Vec2(VisibleRect::rightTop().x / PTM_RATIO, VisibleRect::rightTop().y / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);
}

void Box2dFactory::initBorders(float topLine, float bottomLine)
{
	// Define the ground body.
	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0, 0); // bottom-left corner

	// Call the body factory which allocates memory for the ground body
	// from a pool and creates the ground box shape (also from a pool).
	// The body is also added to the world.
	b2Body* groundBody = m_world->CreateBody(&groundBodyDef);

	// Define the ground box shape.
	b2EdgeShape groundBox;

	//// bottom
	//groundBox.Set(b2Vec2(VisibleRect::leftBottom().x / PTM_RATIO, bottomLine / PTM_RATIO),
	//	b2Vec2(VisibleRect::rightBottom().x / PTM_RATIO, bottomLine / PTM_RATIO));
	//groundBody->CreateFixture(&groundBox, 0);

	// top
	groundBox.Set(b2Vec2(VisibleRect::leftTop().x / PTM_RATIO, topLine / PTM_RATIO),
		b2Vec2(VisibleRect::rightTop().x / PTM_RATIO, topLine / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

	// left
	groundBox.Set(b2Vec2(VisibleRect::leftTop().x / PTM_RATIO, VisibleRect::leftTop().y / PTM_RATIO),
		b2Vec2(VisibleRect::leftBottom().x / PTM_RATIO, VisibleRect::leftBottom().y / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);

	// right
	groundBox.Set(b2Vec2(VisibleRect::rightBottom().x / PTM_RATIO, VisibleRect::rightBottom().y / PTM_RATIO),
		b2Vec2(VisibleRect::rightTop().x / PTM_RATIO, VisibleRect::rightTop().y / PTM_RATIO));
	groundBody->CreateFixture(&groundBox, 0);
}

b2Body *Box2dFactory::createCircle(CCNode *node)
{
	b2BodyDef bodyDef;
	bodyDef.type = b2_dynamicBody; //dynatic status
	bodyDef.userData = node;    //save data for finding sprite by body
	bodyDef.linearDamping = 0.0f; //original speed 0
	bodyDef.position.Set(node->getPositionX() / PTM_RATIO, node->getPositionY() / PTM_RATIO); //mid point
	b2Body *pBody = m_world->CreateBody(&bodyDef); // create a body in Box2D world

	b2CircleShape bodyShape;
	bodyShape.m_radius = node->getContentSize().width * 0.5f / PTM_RATIO;

	b2FixtureDef bodyFixture; //attribute of the node
	bodyFixture.filter.groupIndex = -1; //check collision 
	bodyFixture.density = 0;
	bodyFixture.shape = &bodyShape;
	bodyFixture.friction = 0.0f;
	bodyFixture.restitution = 1.0f;
	pBody->CreateFixture(&bodyFixture);
	return pBody;
}

b2Body *Box2dFactory::createSquare(CCNode *node)
{
	CCPoint worldPos;
	if (node->getParent())
	{
		worldPos = node->getParent()->convertToWorldSpace(node->getPosition());
	}
	else
	{
		worldPos = node->getPosition();
	}
	b2BodyDef bodyDef;
	bodyDef.type = b2_staticBody; //dynatic status
	bodyDef.userData = node;    //save data for finding sprite by body
	bodyDef.linearDamping = 0.0f; //original speed 0
	bodyDef.position.Set(worldPos.x / PTM_RATIO, worldPos.y / PTM_RATIO); //mid point
	b2Body *pBody = m_world->CreateBody(&bodyDef); // create a body in Box2D world

	b2PolygonShape bodyShape;
	bodyShape.SetAsBox(node->getContentSize().width * 0.5f / PTM_RATIO, node->getContentSize().height * 0.5f / PTM_RATIO);//radius 

	b2FixtureDef bodyFixture; //attribute of the node
	bodyFixture.filter.groupIndex = 1; //check collision 
	bodyFixture.density = 0;
	bodyFixture.shape = &bodyShape;
	bodyFixture.friction = 0.0f;
	bodyFixture.restitution = 1.0f;
	pBody->CreateFixture(&bodyFixture);

	return pBody;
}

b2Body *Box2dFactory::createTriangle(cocos2d::CCNode *node)
{
	CCPoint worldPos;
	if (node->getParent())
	{
		worldPos = node->getParent()->convertToWorldSpace(node->getPosition());
	}
	else
	{
		worldPos = node->getPosition();
	}
	b2BodyDef bodyDef;
	bodyDef.userData = node;
	bodyDef.type = b2_staticBody;
	bodyDef.position.Set((worldPos.x - node->getContentSize().width / 2) / PTM_RATIO, (worldPos.y -node->getContentSize().height / 2) / PTM_RATIO);
	b2Body *pBody = m_world->CreateBody(&bodyDef);

	b2PolygonShape bodyShape;
	b2Vec2 vertices[3];

	int type = node->getTag();
	switch (type)
	{
	case 0:
		vertices[0].Set(0.0f, 0.0f);
		vertices[1].Set(node->getContentSize().width / PTM_RATIO, 0.0f);
		vertices[2].Set(0.0f, node->getContentSize().height / PTM_RATIO);
		break;
	case 1:
		vertices[0].Set(0.0f, 0.0f);
		vertices[1].Set(node->getContentSize().width / PTM_RATIO, node->getContentSize().height / PTM_RATIO);
		vertices[2].Set(0.0f, node->getContentSize().height / PTM_RATIO);
		break;
	case 2:
		vertices[0].Set(node->getContentSize().width / PTM_RATIO, node->getContentSize().height / PTM_RATIO);
		vertices[1].Set(0, node->getContentSize().height / PTM_RATIO);
		vertices[2].Set(node->getContentSize().width / PTM_RATIO, 0);
		break;
	case 3:
		vertices[0].Set(0.0f, 0.0f);
		vertices[1].Set(node->getContentSize().width / PTM_RATIO, 0.0f);
		vertices[2].Set(node->getContentSize().width / PTM_RATIO, node->getContentSize().height / PTM_RATIO);
		break;
	}
	bodyShape.Set(vertices, 3);

	b2FixtureDef bodyFixture; //attribute of the node
	bodyFixture.filter.groupIndex = 1; //check collision 
	bodyFixture.density = 0;
	bodyFixture.shape = &bodyShape;
	bodyFixture.friction = 0.0f;
	bodyFixture.restitution = 1.0f;
	pBody->CreateFixture(&bodyFixture);
	return pBody;
}

b2Body *Box2dFactory::createSquareBody(SquareNode *node)
{
	int type = node->getType();
	switch (type)
	{
	case TYPE_SQUARE:
		return createSquare(node);
	case TYPE_TRIANGLE:
		return createTriangle(node);
	}
	return createSquare(node);
}