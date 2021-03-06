#ifndef _SQUARE_NODE_H_
#define _SQUARE_NODE_H_

#include "cocos2d.h"
#include "Box2D/Box2D.h"
#include "CommonMacros.h"

struct Index
{
	int x;
	int y;
	Index(){}
	Index(int x, int y)
	{
		this->x = x;
		this->y = y;
	}
	inline bool operator==(const Index &index) const
	{

		if (this->x == index.x && this->y == index.y)
			return true;
		return false;
	}
};

struct CommonAction
{
	virtual void setBody(){}
	virtual void doCollisionAction(){}
	virtual void runRemoveAction(){}
};

class SquareNode : public cocos2d::CCNode
	, public CommonAction
{
public:
	static SquareNode *create();
	SquareNode();
	~SquareNode(){}

	virtual bool init();
	virtual void setPosition(const cocos2d::CCPoint &position);

	virtual void setBody();
	virtual void removeBody();
	virtual void doCollisionAction();
	virtual void runRemoveAction();
public:
	void setIndex(Index index){ m_index = index; }
	void setIndex(int x, int y){ m_index = Index(x, y); }
	Index getIndex() { return m_index; }

	virtual void addScore(int score);
	int getScore() { return m_score; }
	void setScore(int score){ m_score = score; addScore(0); }

	float getSpeed() { return m_speed; }
	void setSpeed(float speed){ m_speed = speed; }

	int getSquareType(){ return m_squareType; }
	b2Body *getBody(){ return m_body; }

	std::string getImage() { return m_image; }

	void setFreezing(bool isFreezing);

	void moveDown(bool isLastOne = false);
	void doScaleAction();
	bool shouldRemoveDirectly();
	bool canRemoveByProps();
	void showBombAction();
	void setPerfectScale();
	bool isSameColor(SquareNode *node);

protected:
	b2Body *m_body;
	cocos2d::CCSprite *m_imageSprite;
	cocos2d::CCLabelAtlas *m_scoreLabel;
	std::string m_image;
	int m_squareType;
	int m_score;
	Index m_index;
	int m_color;
	float m_speed;
};

class TriangleNode : public SquareNode
{
public:
	TriangleNode() : SquareNode(){ m_squareType = kType_Triangle; }
	static TriangleNode *create(int shap = -1);
	virtual bool init(int shap = -1);
	virtual void addScore(int score);
	virtual void setPosition(const cocos2d::CCPoint &position);
	virtual void setBody();
	virtual void doCollisionAction();
	void setPerfectScale();
};

class BossEatMarbleNode : public SquareNode
{
public:
	BossEatMarbleNode() : SquareNode(){ m_squareType = kType_BossEatMarble; }
	static BossEatMarbleNode *create();
	virtual bool init();
	virtual void setBody();
	virtual void doCollisionAction();
	virtual void runRemoveAction();
};

class IronNode : public SquareNode
{
public:
	IronNode() : SquareNode(){ m_squareType = kType_Iron; }
	static IronNode *create();
	virtual bool init();
	virtual void setBody();
	virtual void doCollisionAction();
	virtual void runRemoveAction();
};

#endif