#ifndef __MSG_CHINESE_ZODIAC_HPP__
#define __MSG_CHINESE_ZODIAC_HPP__

#include "servercommon/chinesezodiacdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	enum CS_TIAN_XIANG_TYPE_OLD
	{
		CS_TIAN_XIANG_TYPE_OLD_ALL_INFO,			// 请求所有信息
		CS_TIAN_XIANG_TYPE_OLD_CHANGE_BEAD,			// 请求改变位置，p1 = x , p2 = y， p3 = 目标格子的x, p4 = 目标格子的y  , p5 = 章节
		CS_TIAN_XIANG_TYPE_OLD_XIE_BEAD,			// 卸载珠   p1 = x , p2 = y, p3 = 章节
		CS_TIAN_XIANG_TYPE_OLD_GUNGUN_LE_REQ,		// 滚滚乐抽奖  p1： 0是抽1次，1是抽10次
		CS_TIAN_XIANG_TYPE_OLD_UNLOCK,				// 点击开锁星座
		CS_TIAN_XIANG_TYPE_OLD_PUT_MIJI,			// 放秘籍  P1 = 星座类型， p2 = 秘籍index
		CS_TIAN_XIANG_TYPE_OLD_CALC_CAPACITY,		// 放置秘籍成功，重新计算战力
		CS_TIAN_XIANG_TYPE_OLD_MIJI_COMPOUND,		// 秘籍合成 p1：index1
		CS_TIAN_XIANG_TYPE_OLD_PUT_BEAD,			// 手动放珠子 p1:type, p2:章节
		CS_TIAN_XIANG_TYPE_OLD_UPGRADE_XINGLING,	// 升级星灵p1:星图id, p2:是否自动购买
		CS_TIAN_XIANG_TYPE_OLD_UPLEVEL_XINGHUN,		// 升级星魂 p1:生肖类型，p2:是否自动购买，p3:是否使用保护符
		CS_TIAN_XIANG_TYPE_OLD_XINGHUN_UNLOCK,		// 点击开锁星魂
	};
	
	// 秘籍单个修改 8210
	class SCMijiSingleChange
	{
	public:
		SCMijiSingleChange();
		MessageHeader header;

		int zodiac_type;
		int kong_index;
		int miji_index;
	};

	// 秘籍合成成功 8211
	class SCMijiCombineSucc
	{
	public:
		SCMijiCombineSucc();
		MessageHeader header;

		int miji_index;
	};
	
	/*
     * 请求 8205
     */
    class CSTianxiangReq
    {
    public:
		CSTianxiangReq();
		MessageHeader header;

        short info_type;
        short param1;
        short param2;
        short param3;
        short param4;
		short param5;
    };

	 /*
     * 天象单个珠子 8207
     */
    class SCTianXiangSignBead
    {
    public:
        SCTianXiangSignBead();
		MessageHeader header;

		int chapter;
		char x;
		char y;
		short type;
    };

	/*
     * 天象组合信息 8208
     */
    class SCTianXiangCombind
    {
    public:
        SCTianXiangCombind();
		MessageHeader header;

		char active_list[TIAN_XIANG_ALL_CHAPTER_COMBINE];
		char chapter;
		ComPosi bead_by_combine_list[TIAN_XIANG_ALL_CHAPTER_COMBINE][TIAN_XIANG_COMBINE_MEX_BEAD_NUM];
    };

	struct TianXiangChapter
	{
		short grid_list[TIAN_XIANG_TABEL_ROW_COUNT * TIAN_XIANG_TABEL_MIDDLE_GRIDS];
		ComPosi bead_by_combine_list[TIAN_XIANG_ALL_CHAPTER_COMBINE][TIAN_XIANG_COMBINE_MEX_BEAD_NUM];
	};

    /*
     * 全部天象信息列表 8206
     */
    class SCTianXiangAllInfo
    {
    public:
		SCTianXiangAllInfo();
		MessageHeader header;
		
		char curr_chapter;
		char active_list[TIAN_XIANG_CURR_CHAPTER_NUM][TIAN_XIANG_ALL_CHAPTER_COMBINE];

		TianXiangChapter chapter_list[TIAN_XIANG_CHAPTER_NUM];

		char is_finish_all_chapter;
		char reserve_ch;
		short reserve_sh;
    };

	/*
     * 滚滚乐信息 8209
     */
    class SCGunGunLeInfo
    {
    public:
        SCGunGunLeInfo();
		MessageHeader header;
		
		int today_ggl_free_times;
		int count;
		unsigned int last_free_ggl_time;
		int combine_type_list[GGL_COMBINE_TYPE_MAX_NUM];
    };

	/*
     * 星灵信息 8212
     */
    class SCXinglingInfo
    {
    public:
        SCXinglingInfo();
		MessageHeader header;
		
		XingLingParam xingling_lsit[TIAN_XIANG_CHAPTER_NUM];
    };

	class SCChineseZodiacAllInfo				// 8200,							// 星座系统所有信息						
	{
	public:
		SCChineseZodiacAllInfo();
		MessageHeader header;
		
		short zodiac_level_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
		short xinghun_level_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
		short xinghun_level_max_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
		short xinghun_baoji_value_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
		EquipLevelList chinesezodiac_equip_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];
		MijiParam miji_list[CHINESE_ZODIAC_SOUL_MAX_TYPE_LIMIT];

		int zodiac_progress;
		char upgrade_zodiac;
		char xinghun_progress;
		short reserve_sh;
	};

	class SCChineseZodiacEquipInfo				// 8201,							// 星座系统装备信息			
	{
	public:
		SCChineseZodiacEquipInfo();
		MessageHeader header;

		short zodiac_type;
		short equip_type;
		int equip_item;
	};

	class CSChineseZodiacPromoteXingHun			// 8202,							// 提升星座星魂等级
	{
	public:
		CSChineseZodiacPromoteXingHun();
		MessageHeader header;

		int zodiac_type;
		short is_auto_buy;
		short is_use_promote_item;
	};

	class CSChineseZodiacPromoteEquip			// 8203,							// 提升星座装备等级
	{
	public:
		CSChineseZodiacPromoteEquip();
		MessageHeader header;

		short zodiac_type;
		short equip_slot;
	};

	class CSChineseZodiacPromote				// 8204,							// 升星座		
	{
	public:
		CSChineseZodiacPromote();
		MessageHeader header;

		short zodiac_type;
		short is_auto_buy;
	};

	class CSChineseZodiacTakeOffEquip			// 8213,							// 卸下装备
	{
	public:
		CSChineseZodiacTakeOffEquip();
		MessageHeader header;

		short zodiac_type;   //生肖类型 
		short equip_type;	 //装备槽id	
	};

	class CSChineseZodiacRecycle		// 8214 生肖装备回收
	{
	public:
		CSChineseZodiacRecycle();
		MessageHeader header;

		int index_count;											// 格子数量
		short index_list[MAX_CHINESEZODIAC_RECYCLE_NUM];		    // 格子索引列表
	};
}

#pragma pack(pop)

#endif 


