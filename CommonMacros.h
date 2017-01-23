#ifndef __COMMONMACROS_H__
#define __COMMONMACROS_H__

#define LABEL_FONT "fonts/SF Square Head Bold.ttf"
#define FONT_WHITE "numbers/number1.png"
#define FONT_YELLOW "numbers/number2.png"

enum TouchPriority
{
	kPriority_Main = -1,
	kPriority_Game = -10,
	kPriority_Guide = -20,
	kPriority_Pause = -30,
	kPriority_Shop = -40,
	kPriority_Libao = -50
};

enum GameZOrder
{
	kZOrder_Main,
	kZOrder_Square,
	kZOrder_Marble,
	kZOrder_Character,
	kZOrder_Layout,
	KZOrder_GuideLayer,
	KZOrder_PauseLayer,
	KZOrder_GameOver,
	kZOrder_Shop,
	KZOrder_LibaoLayer
};

enum SquareType
{
	kType_Empty,
	kType_Square,
	kType_Triangle,
	kType_AddMarble,
	kType_AddCoin,
	kType_Rebound,
	kType_EliminateRow,
	kType_EliminateCol,
	kType_EliminateCross,
	kType_BossEatMarble,
	kType_Protect,
};

enum MarbleType
{
	kMarble_Normal,
	kMarble_Faster,
	kMarble_Biger,
	kMarble_Dispersed,
	kMarble_Bomb,
	kMarble_Across,
};
enum ViewTag
{
	kTag_Streak = 100,
	kTag_Wall = 200,
	kTag_Marble = 300,
	kTag_Pause,
	kTag_Shop,
	kTag_Libao,
	kTag_GuideLayer,
	kTag_GameOver,
	kTag_Protect,
};

enum PropType
{
	kProp_Clear,
	kProp_DoubleAttact,
	kProp_Freezing
};

#define NOTIFY_VIEWS(_FUNC_ ,...)						\
for (size_t index = 0; index < m_views.size(); ++index) \
{														\
	m_views[index]->_FUNC_(__VA_ARGS__);				\
}

#endif