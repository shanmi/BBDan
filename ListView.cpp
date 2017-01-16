#include "ListView.h"

USING_NS_CC;
using namespace std;
using namespace std::placeholders;


ListView *ListView::create(const CCSize &size)
{
	ListView *view = new ListView(size);
	view->init();
	view->autorelease();
	return view;
}


ListView::ListView(const CCSize &size)
: m_size(size)
, m_touchPriority(0)
, m_spacingX(0)
, m_spacingY(0)
, m_columnSize(1)
, m_platHeight(0)
, m_isTouching(false)
{
	
}

bool ListView::init()
{
	setContentSize(m_size);

	/*CCLayerColor *maskNode = CCLayerColor::create(ccc4(0, 255, 0, 75));
	maskNode->setContentSize(m_size);
	addChild(maskNode);*/

	CCLayerColor *back = CCLayerColor::create(ccc4(125, 0, 0, 255));
	back->setContentSize(m_size);
	m_clippingNode = CCClippingNode::create();
	m_clippingNode->setInverted(false);
	m_clippingNode->setAlphaThreshold(1.0f);
	m_clippingNode->setStencil(back);
	addChild(m_clippingNode);

	m_container = CCNode::create();
	m_clippingNode->addChild(m_container);

	return true;
}

int ListView::addNode(cocos2d::CCNode *node)
{
	float contentHeight = m_container->getContentSize().height;
	float contentWidth = m_container->getContentSize().width;

	m_container->addChild(node);
	int height = contentHeight;
	auto nodeSize = node->getContentSize();
	if (m_platHeight == 0)
	{
		m_platHeight = nodeSize.height;
	}

	int rate = 0;
	int startX = (m_size.width - m_columnSize*nodeSize.width - (m_columnSize - 1)*m_spacingX) / 2;
	int newWidth = startX + (nodeSize.width + m_spacingX) * (m_nodes.size() % m_columnSize);
	int newHeight = -(nodeSize.height + m_spacingY)*(m_nodes.size() / m_columnSize);
	if (m_nodes.size() % m_columnSize == 0)
	{
		rate = 1;
	}
	
	node->setPosition(ccp(newWidth, newHeight));
	
	m_nodes.push_back(node);

	float maxWidth = max(contentWidth, nodeSize.width * m_columnSize);
	float curHeight = height + rate*nodeSize.height;
	m_container->setContentSize(CCSize(maxWidth, curHeight));
	m_container->setPosition(ccp(0, m_size.height - nodeSize.height - m_spacingY));

	return m_nodes.size() - 1;
}

void ListView::removeNode(int index)
{
	auto node = getNode(index);
	if (node)
	{
		node->removeFromParent();
	}
}

cocos2d::CCNode *ListView::getNode(int index)
{
	int nodeAmount = count();
	if (index < 0 || index > nodeAmount - 1) return NULL;

	return m_nodes[index];
}

void ListView::onEnter()
{
	CCNode::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, m_touchPriority, true);
}

void ListView::onExit()
{
	CCNode::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
}

void ListView::clear()
{
	m_container->removeAllChildren();	
	m_nodes.clear();
}

void ListView::doMove(cocos2d::CCPoint offset)
{
	CCPoint contentPos = m_container->getPosition();
	
	float newY = contentPos.y + offset.y;
	auto tarPos = ccp(0, newY);
	m_container->setPosition(tarPos);

}

bool ListView::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	auto localPos = convertToNodeSpace(pTouch->getLocation());
	auto size = getContentSize();

	CCRect rect(0, 0, size.width, size.height);
	if (!rect.containsPoint(localPos)) return false;

	m_isTouching = true;
	onTouchBegan(pTouch);
	return true;
}	

void ListView::ccTouchMoved(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	CCSize contentSize = m_container->getContentSize();
	if (contentSize.height < m_size.height) return;

	auto curPos = pTouch->getLocation();
	auto prePos = pTouch->getPreviousLocation();

	doMove(ccpSub(curPos, prePos));
	onTouchMoved(pTouch);
}

void ListView::ccTouchEnded(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	auto startPos = pTouch->getStartLocation();
	auto prePos = pTouch->getPreviousLocation();
	auto curPos = pTouch->getLocation();
	m_isTouching = false;

	dragback();
	onTouchEnded(pTouch);
}

bool ListView::isOutOfRange()
{
	auto containerSize = m_container->getContentSize();
	auto curPos = m_container->getPosition();

	float minY = m_size.height - m_platHeight - m_spacingY;
	float maxY = ((m_nodes.size() - 1) / m_columnSize) * (m_platHeight + m_spacingY) + m_spacingY;

	return curPos.y < minY || curPos.y > maxY;
}

void ListView::dragback()
{
	auto containerSize = m_container->getContentSize();
	auto curPos = m_container->getPosition();

	if (containerSize.height <= m_size.height)
	{
		return;
	}
	float minY = m_size.height - m_platHeight - m_spacingY;
	float maxY = ((m_nodes.size() - 1) / m_columnSize) * (m_platHeight + m_spacingY) + m_spacingY;

	float newY = min(max(curPos.y, minY), maxY);
	m_container->setPositionY(newY);

	/*float posY = getSliderPosByListPos(newY);
	m_slider->setPositionY(posY);*/

}
