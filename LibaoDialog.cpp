#include "LibaoDialog.h"
#include "CommonMacros.h"
#include "UiLayout.h"
#include "MyPurchase.h"
#include "GameController.h"
#include "GameConfig.h"

USING_NS_CC;

enum TagPanel
{
	Tag_CoinPanel,
	Tag_BallPanel,
};

void LibaoDialog::onEnter()
{
	CCLayer::onEnter();
	CCDirector::sharedDirector()->getTouchDispatcher()->addTargetedDelegate(this, kPriority_Libao, true);
	GameController::getInstance()->addView(this);
}

void LibaoDialog::onExit()
{
	CCLayer::onExit();
	CCDirector::sharedDirector()->getTouchDispatcher()->removeDelegate(this);
	GameController::getInstance()->removeView(this);
}

bool LibaoDialog::ccTouchBegan(cocos2d::CCTouch *pTouch, cocos2d::CCEvent *pEvent)
{
	return true;
}

LibaoDialog::LibaoDialog(int type)
:m_type(type)
{

}

LibaoDialog *LibaoDialog::create(int type)
{
	LibaoDialog *dialog = new LibaoDialog(type);
	dialog->init();
	dialog->autorelease();
	return dialog;
}

bool LibaoDialog::init()
{
	if (!CCLayer::init()){
		return false;
	}

	auto winSize = CCDirector::sharedDirector()->getWinSize();
	auto colorLayer = CCLayerColor::create(ccc4(0, 0, 0, 80));
	addChild(colorLayer);

	m_mainLayout = UiLayout::create("layout/libao_dialog.xml");
	m_mainLayout->setMenuTouchPriority(kPriority_Libao - 1);
	m_mainLayout->setAnchorPoint(ccp(0.5f, 0.5f));
	m_mainLayout->setPosition(ccpMult(winSize, 0.5f));
	addChild(m_mainLayout);

	initLayout();

	return true;
}

void LibaoDialog::initLayout()
{
	bool isBusinessMode = MyPurchase::sharedPurchase()->isBusinessMode();
	int isYijian = GameConfig::getInstance()->m_yijian;

	CCMenuItem *closeItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(6));
	closeItem->setTarget(this, menu_selector(LibaoDialog::closePanel));
	if (isBusinessMode && isYijian)
	{
		closeItem->setTarget(this, menu_selector(LibaoDialog::buyLibao));
	}

	CCMenuItem *buyItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(5));
	buyItem->setTarget(this, menu_selector(LibaoDialog::buyLibao));
	buyItem->setVisible(!isBusinessMode);

	CCMenuItem *sureItem = dynamic_cast<CCMenuItem*>(m_mainLayout->getChildById(8));
	sureItem->setTarget(this, menu_selector(LibaoDialog::buyLibao));
	sureItem->setVisible(isBusinessMode);

	CCSprite *libaoIcon = dynamic_cast<CCSprite*>(m_mainLayout->getChildById(4));
	std::string libaoImg;
	switch (m_type)
	{
	case PAY_TYPE_TIME_LIBAO:
		if (isYijian)
		{
			libaoImg = "libao/zujiemian_libao_huoqudaoju.png";
		}
		else
		{
			libaoImg = "libao/zujiemian_libao_xianshidalibao.png";
		}
		break;
	case PAY_TYPE_FISH_LIBAO:
		libaoImg = "libao/zujiemian_libao_xinshoulibao.png";
		break;
	case PAY_TYPE_COIN_LIBAO:
		libaoImg = "libao/zujiemian_libao_xinshoulibao.png";
		break;
	default:
		libaoImg = "libao/zujiemian_libao_xianshidalibao.png";
		break;
	}
	libaoIcon->initWithFile(libaoImg.c_str());


	CCLabelTTF *buyTip = dynamic_cast<CCLabelTTF*>(m_mainLayout->getChildById(7));
	if (isBusinessMode)
	{
		auto size = m_mainLayout->getContentSize();
		buyTip->setPosition(ccp(buyTip->getPositionX(), buyTip->getPositionY() - size.height * 0.8f));
		buyTip->setScale(0.5f);
		buyTip->setOpacity(120);

		if (isYijian)
		{
			buyTip->setVisible(false);
		}
	}
}

void LibaoDialog::closePanel(CCObject *pSender)
{
	removeFromParent();
}

void LibaoDialog::buyLibao(CCObject *pSender)
{
	MyPurchase::sharedPurchase()->payForProducts(m_type);
	bool isBusinessMode = MyPurchase::sharedPurchase()->isBusinessMode();
	int isYijian = GameConfig::getInstance()->m_yijian;
	if (isBusinessMode && isYijian)
	{
		removeFromParent();
	}
}

void LibaoDialog::updateCoins()
{
	removeFromParent();
}