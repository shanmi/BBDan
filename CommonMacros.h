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
	kZOrder_Layout,
	KZOrder_GuideLayer,
	KZOrder_PauseLayer,
	KZOrder_GameOver,
	KZOrder_LibaoLayer
};

enum SquareType
{
	kType_Empty = 26,
	kType_Square = 40,
	kType_Triangle = 10,
	kType_AddMarble = 25,
	kType_AddCoin = 1,
	kType_Rebound = 3,
	kType_EliminateRow = 4,
	kType_EliminateCol = 5,
	kType_BossEatMarble = 2,
};

enum CollisionType
{
	kCollision_Square,
	kCollision_Triangle,
	kCollision_AddMarble,
	kCollision_AddCoin,
	kCollision_Rebound,
	kCollision_EliminateRow,
	kCollision_EliminateCol,
	kCollision_BossEatMarble
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
	kTag_Wall = 100,
	kTag_Marble = 200,
	kTag_Pause,
	kTag_GuideLayer,
	kTag_GameOver
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