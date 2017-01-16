#ifndef __PAGE_NODE_H__
#define __PAGE_NODE_H__

#include "cocos2d.h"
#include "cocos-ext.h"
#include "MarbleAttr.h"

struct ShopConfig
{
	int id;
	int price;
	int diamond;
	int present;
	int iconId;
};

class UiLayout;
class ShopBallNode
	:public cocos2d::CCNode
{
public:
	static ShopBallNode *create(const MarbleAttr attr);
	~ShopBallNode(){}
	virtual bool init();

	void initLayout();
	void buyItem(cocos2d::CCObject *pSender);

private:
	ShopBallNode(const MarbleAttr attr);

private:
	UiLayout *m_mainLayout;
	MarbleAttr m_attr;
};

class ShopCoinNode : public cocos2d::CCNode
{
public:
	static ShopCoinNode *create(const ShopConfig &config);
	void setCallbackListener(std::function <void()> callback){ m_callback = callback; }
private:
	ShopCoinNode(const ShopConfig &config){ m_config = config; }
	virtual bool init();

	void buyItem(CCObject *pSender);
	void payCallBack(CCNode* nouse, int payType, int payResult, int payIntNoUse);

private:
	ShopConfig m_config;
	std::function <void()> m_callback;

public:
	void initMainLayout();
	void closePannel(cocos2d::CCObject *pSender);

private:
	UiLayout *m_mainLayout;
};
#endif