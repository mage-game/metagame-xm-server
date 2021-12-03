#ifndef __CHINESE_ZODIAC_DEF_H__
#define __CHINESE_ZODIAC_DEF_H__

#include "servercommon.h"

static const int CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT = 12;				// 星座精魄类型数量限制
static const int CHINESE_ZODIAC_LEVEL_MAX_LIMIT = 100;					// 星座精魄等级上限

static const int CHINESE_ZODIAC_EQUIP_SLOT_MAX_LIMIT = 8;				// 星座装备槽数量上限  不能改，数据库
static const int CHINESE_ZODIAC_CUR_EQUIP_SLOT_MAX_LIMIT = 5;			// 当前星座装备槽数量
static const int CHINESE_ZODIAC_MAX_EQUIP_LEVEL = 100;					// 装备最高等级
static const int CHINESE_ZODIAC_MAX_EQUIP_SUIT = 6;						// 装备套装最大数

UNSTD_STATIC_CHECK(CHINESE_ZODIAC_CUR_EQUIP_SLOT_MAX_LIMIT <= CHINESE_ZODIAC_EQUIP_SLOT_MAX_LIMIT);

static const int CHINESE_ZODIAC_XINGHUN_LEVEL_MAX = 50;					// 星座星魂等级最大值
static const int CHINESE_ZODIAC_XINGHUN_TITLE_COUNT_MAX = 50;			// 星魂称号最大数量

static const int TIAN_XIANG_TABEL_MIDDLE_ROW = 3;							// 中间的那一行的行数
static const int TIAN_XIANG_TABEL_ROW_COUNT = 7;							// 行 y 最大行数
static const int TIAN_XIANG_TABEL_MIDDLE_GRIDS = 7;							// 列 x 最大列数
static const int TIAN_XIANG_ALL_CHAPTER_COMBINE = 3;						// 每个章节的组合数
static const int TIAN_XIANG_COMBINE_MEX_BEAD_NUM = 15;						// 每个组合最多的珠子数
static const int TIAN_XIANG_CHAPTER_NUM = 10;								// 章节最大数
static const int TIAN_XIANG_CURR_CHAPTER_NUM = 5;							// 当前章节数（星灵个数）

static const int GGL_COMBINE_TYPE_MAX_NUM = 10;								// 滚滚乐一次抽奖最大次数
static const int GGL_COMBINE_TYPE_NUM = 7;									// 滚滚乐牌子类型个数
static const int GGL_COMBINE_ITEM_NUM = 3;									// 滚滚乐牌子个数

static const int MIJI_KONG_NUM = 8;											// 星座秘籍的孔数
static const int MIJI_KONG_KILL_MAX_COUNT = 200;							// 星座秘籍最大数量
static const int MIJI_KONG_MAX_LEVEL = 3;									// 星座最大等级

static const int XING_LING_MAX_LEVEL = 40;									// 星灵最大等级

static const int MAX_CHINESEZODIAC_RECYCLE_NUM = 100;						// 单次分解最大格子数

enum TIAN_XIANG_BEAD_TYPE
{
	TIAN_XIANG_BEAD_TYPE_INVALID = 0,	//无效
	TIAN_XIANG_BEAD_TYPE_FIRE,			//火灵果
	TIAN_XIANG_BEAD_TYPE_METALS,		//金灵果
	TIAN_XIANG_BEAD_TYPE_WATER,			//水灵果
	TIAN_XIANG_BEAD_TYPE_WOOD,			//木灵果
	TIAN_XIANG_BEAD_TYPE_CHIJINLINGZHU,	//赤金灵珠
	TIAN_XIANG_BEAD_TYPE_SIX,			//灵珠6（扩展）

	TIAN_XIANG_BEAD_TYPE_MAX,			//灵果类型最大
	TIAN_XIANG_BEAD_TYPE_SAME = TIAN_XIANG_BEAD_TYPE_MAX,  //同种灵果即可
	TIAN_XIANG_BEAD_TYPE_ALL,			// 任意灵果即可
};

enum MIJI_ARRT_TYPE
{
	MIJI_ARRT_MAXHP = 0,				// 气血
	MIJI_ARRT_GONGJI,					// 攻击
	MIJI_ARRT_FANGYU,					// 防御
	MIJI_ARRT_BAOJI,					// 暴击
	MIJI_ARRT_KANGBAO,					// 抗暴
	MIJI_ARRT_MINGZHONG,				// 命中
	MIJI_ARRT_SHANBI,					// 闪避
	MIJI_ARRT_NVSHENGONGJI,				// 女神攻击
	MIJI_ARRT_GD_ZENGSHANG,				// 固定增伤
	MIJI_ARRT_GD_MIANFANG,				// 固定免伤
	
	MIJI_SPECIAL_TYPE_MIN,
	MIJI_ARRT_HUIXUE_PER = MIJI_SPECIAL_TYPE_MIN,	// 回血调高X%
	MIJI_ARRT_JINENG_COOLCOWN_SUODUAN_PER,			// 技能冷却缩短X%
	MIJI_ARRT_JINENG_XISHU_PER,						// 技能系数提高X%
	MIJI_SPECIAL_TYPE_MAX,
};

#pragma pack(push, 4)

struct EquipLevelList
{
	EquipLevelList() { this->Reset(); }

	void Reset()
	{
		memset(equip_Item_list, 0, sizeof(equip_Item_list));
	}

	ItemID equip_Item_list[CHINESE_ZODIAC_EQUIP_SLOT_MAX_LIMIT];		// 每个部位对应装备等级
};

struct TianXiangTableGridParam
{
	TianXiangTableGridParam() { this->Reset(); }
	void Reset()
	{
		type = 0;
		reserve = 0;
	}

	bool IsEmpty() const { return 0 == type; }	//是否为空

	short type;
	short reserve;
};

struct ComPosi
{
	ComPosi() { this->Reset(); }
	ComPosi(int _x, int _y): x(_x), y(_y) {}

	void Reset()
	{
		x = -1;
		y = -1;
	}

	short x;
	short y;
};

struct MijiParam
{
	MijiParam() {}

	char single_miji_list[MIJI_KONG_NUM];
};

struct TianXiangTotalTableGridParam
{   
	TianXiangTotalTableGridParam() { this->Reset(); }

	void Reset()
	{
		memset(table_lsit, 0, sizeof(table_lsit));
		memset(bead_by_combine_lsit, -1, sizeof(bead_by_combine_lsit));
		
	}
	// 所有格子
	TianXiangTableGridParam table_lsit[TIAN_XIANG_TABEL_MIDDLE_GRIDS][TIAN_XIANG_TABEL_ROW_COUNT];

	// 保存珠子对应的组合
	ComPosi bead_by_combine_lsit[TIAN_XIANG_ALL_CHAPTER_COMBINE][TIAN_XIANG_COMBINE_MEX_BEAD_NUM];
};

struct XingLingParam
{   
	XingLingParam() { this->Reset(); }

	void Reset()
	{
		level = -1;
		bless = 0;
	}

	int level;
	int bless;
};

struct ChineseZodiacParam
{
	ChineseZodiacParam() { this->Reset(); }

	void Reset()
	{
		memset(zodiac_level_list, 0, sizeof(zodiac_level_list));
		memset(xinghun_level_list, 0, sizeof(xinghun_level_list));
		memset(xinghun_level_max_list, 0, sizeof(xinghun_level_max_list));
		memset(xinghun_baoji_value_list, 0, sizeof(xinghun_baoji_value_list));
 		memset(miji_list, -1, sizeof(miji_list));

		curr_chapter = 0;
		today_ggl_free_times = 0;
		zodiac_progress = 1;
		is_finish_all_chapter = 0;
		reserver_ch = 0;
		combine_system_msg_flag = 0;
		last_free_ggl_time = 1;
		xinghun_progress = 1;

		for (int i = 0; i < CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT; ++ i)
		{
			chinesezodiac_equip_list[i].Reset();
		}

		for (int i = 0; i < TIAN_XIANG_CHAPTER_NUM; ++ i)
		{
			chapter_lsit[i].Reset();
		}

		for (int i = 0; i < TIAN_XIANG_CHAPTER_NUM; ++ i)
		{
			xingling_lsit[i].Reset();
		}
	}

	short zodiac_level_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
	short xinghun_level_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
	short xinghun_level_max_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
	EquipLevelList chinesezodiac_equip_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
	
	TianXiangTotalTableGridParam chapter_lsit[TIAN_XIANG_CHAPTER_NUM];
	MijiParam miji_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];

	int curr_chapter;
	int today_ggl_free_times;		// 今天使用摇滚滚乐的免费次数
	int zodiac_progress;			// 星座开锁进度
	char is_finish_all_chapter;		// 是否完成了所有章节
	char reserver_ch;				
	short combine_system_msg_flag;	// 组合开启传闻标记
	unsigned int last_free_ggl_time;// 最后免费滚滚乐的时间

	XingLingParam xingling_lsit[TIAN_XIANG_CHAPTER_NUM];
	short xinghun_baoji_value_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
	int xinghun_progress;			// 星魂开锁进度
};

typedef char ChineseZodiacParamHex[sizeof(ChineseZodiacParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(ChineseZodiacParamHex) < 9216);

#pragma pack(pop)

#endif

