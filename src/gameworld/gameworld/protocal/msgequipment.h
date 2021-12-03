
#ifndef MSGEQUIPMENT_H
#define MSGEQUIPMENT_H

#include "servercommon/userprotocal/msgheader.h"
#include "gamedef.h"
#include "servercommon/struct/itemlistparam.h"
#include "equipment/suitbase.hpp"
#include "servercommon/shenbingdef.hpp"
#include "servercommon/stonedef.hpp"
#include "servercommon/washdef.hpp"
#include "servercommon/equipsuitdef.hpp"
#include "globalconfig/equipconfig.h"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{
	class CSEquipmentInfoReq
	{
	public:
		CSEquipmentInfoReq();
		MessageHeader header;
	};

	class SCEquipmentInfoAck
	{
	public:
		SCEquipmentInfoAck();
		MessageHeader header;

		struct InfoType 
		{
			int	index;
			ItemDataWrapper	item_wrapper;
		};

		int total_dress_score;
		short is_ban_shizhuang_appe;
		ItemID cross_equip_item_id;
		unsigned int cross_equip_gain_timestamp;

		short min_strengthen_level;
		short count;
		InfoType equipment_list[ItemNamespace::MAX_EQUIPMENT_GRID_NUM];
	};

	class SCEquipmentGridInfo	// 所有装备格子
	{
	public:
		SCEquipmentGridInfo();
		MessageHeader header;
		
		struct InfoType
		{
			int	index;
			short strengthen_level;
			short shenzhu_level;
			short star_level;
			short eternity_level;
			int star_exp;
		};

		short use_eternity_level;
		short min_eternity_level;
		int count;
		InfoType equipment_grid_list[ItemNamespace::MAX_EQUIPMENT_GRID_NUM];
	};

	class SCNoticeWuqiColor
	{
	public:
		SCNoticeWuqiColor();
		MessageHeader header;

		ObjID obj_id;
		short reserve;
		int wuqi_color;
	};

	class SCEquipmentItemChange
	{
	public:
		enum
		{
			SEND_REASON_DEFAULT,
			SEND_REASON_COMPOUND,
		};

		SCEquipmentItemChange();
		MessageHeader header;

		short index;
		short send_reason;
		ItemDataWrapper	item_wrapper;
		int min_strengthen_level;
	};

	class SCGetOneEquipmentInfoAck
	{
	public:
		SCGetOneEquipmentInfoAck();
		MessageHeader	header;

		ItemDataWrapper	item_wrapper;
	};

	class SCEquipSuitInfo
	{
	public:
		SCEquipSuitInfo();
		MessageHeader header;

		EquipSuitInfoList suit_list;
	};

	class CSTakeOffEquipment
	{
	public:
		CSTakeOffEquipment();
		MessageHeader header;

		int index;
	};

	class CSEquipCrossEquipOpera
	{
	public:
		CSEquipCrossEquipOpera();
		MessageHeader header;

		char is_puton;
		char reserve_ch;
		short grid_idx;
	};

	// 装备强化
	class CSEquipStrengthen	// 1752
	{
	public:
		CSEquipStrengthen();
		MessageHeader header;

		short equip_index;
		short is_puton;
		short select_bind_first;
		short is_auto_buy;
		short use_lucky_item;
		short flag;
	};

	// 装备升星
	class CSEquipUpStar	// 1774
	{
	public:
		CSEquipUpStar();
		MessageHeader header;

		short equip_index;
		short reserve;
	};

	// 装备升级
	class CSEquipUpLevel
	{
	public:
		CSEquipUpLevel();
		MessageHeader header;

		short equip_index;
		short is_puton;
		short select_bind_first;
		//short use_protect_stuff;
	};

	//// 装备升级
	//class CSEquipUpLevel
	//{
	//public:
	//	CSEquipUpLevel();
	//	MessageHeader header;

	//	short equip_index;
	//	char is_puton;
	//	char select_bind_first;
	//	char is_auto_buy;
	//	char use_lucky_item;
	//	short reserve;
	//};
	/*class CSEquipUpLevel
	{
	public:
		CSEquipUpLevel();
		MessageHeader header;

		short equip_index1;
		short equip_index2;
		short equip_index3;
		short reserve;
	};*/

	// 装备传承
	class CSEquipInherit
	{
	public:
		CSEquipInherit();
		MessageHeader header;

		short equip_index1;
		short equip_index2;
		char is_puton1;
		char is_puton2;
		char inherit_type;
		char cost_type;
	};

	// 装备升品
	class CSEquipUpQuality
	{
	public:
		CSEquipUpQuality();
		MessageHeader header;

		char is_puton;
		char select_bind_first;
		short equip_index;
	};

	// 装备合成
	class CSEquipCompound			// 1788
	{
	public:
		CSEquipCompound();
		MessageHeader header;

		int equip_index;
	};

	// 请求装备提升永恒等级 1789
	class CSEquipUpEternity
	{
	public:
		CSEquipUpEternity();
		MessageHeader header;

		short equip_index;
		short is_auto_buy;
	};

	// 使用永恒等级 1792
	class CSEquipUseEternityLevel
	{
	public:
		CSEquipUseEternityLevel();
		MessageHeader header;

		int eternity_level;
	};

	// 屏蔽时装外观
	class CSBanDressAppe
	{
	public:
		CSBanDressAppe();
		MessageHeader header;

		int is_to_ban;
	};

	// 时装衣柜选择外观
	class CSEquipSelectDressAppe
	{
	public:
		CSEquipSelectDressAppe();
		MessageHeader header;

		short equip_type;
		UInt16 dress_id;
	};

	class CSGetEquipSpecialParamReq
	{
	public:
		CSGetEquipSpecialParamReq();
		MessageHeader	header;

		short is_puton;
		short equip_index;
	};

	// 装备神铸
	class CSEquipShenZhu
	{
	public:
		CSEquipShenZhu();
		MessageHeader header;

		short equip_index;
		short is_puton;
		short select_bind_first;
		short reserve;
	};

	//至宝升级
	class CSZhiBaoUplevel
	{
	public:
		CSZhiBaoUplevel();
		MessageHeader header;
	};

	// 至宝形象
	class CSZhiBaoUseImage
	{
	public:
		CSZhiBaoUseImage();
		MessageHeader header;

		int use_image;
	};

	class CSZhiBaoHuanhua
	{
	public:
		enum OperationType
		{
			OT_ACTIVE_AND_UPGRADE = 0,
			OT_USE_HUANHUA_XINGXIANG = 1,
		};

		CSZhiBaoHuanhua();
		MessageHeader header;

		int operation;
		int type;
	};

	// 至宝攻击  5852
	class SCZhiBaoAttack
	{
	public:
		SCZhiBaoAttack();
		MessageHeader header;

		ObjID attacker_id;			//攻击者id
		ObjID target_id;			//被攻击者id
		short skill_idx;			//技能索引
		short is_baoji;				//是否暴击
		int hurt;					//伤害
	};

	// 宝石镶嵌
	class CSStoneInlay
	{
	public:
		CSStoneInlay();
		MessageHeader header;

		int index_in_bag;
		char equip_part;
		char stone_slot;
		short is_inlay;
	};

	// 宝石升级
	class CSStoneUplevel
	{
	public:
		CSStoneUplevel();
		MessageHeader header;

		char equip_part;
		char stone_slot;
		char uplevel_type;
		char reserve;
	};

	// 请求宝石信息
	class CSReqStoneInfo
	{
	public:
		CSReqStoneInfo();
		MessageHeader header;
	};

	class SCStoneInfo
	{
	public:
		SCStoneInfo();
		MessageHeader header;

		struct StoneItem
		{
			int stone_id;	
			short reserve;	
			short reserve2;	
		};

		struct StonesOnPart
		{
			StoneItem stone_list[MAX_STONE_COUNT];
		};

		enum STONE_REASON
		{
			STONE_REASON_DEFAULT = 0,
			STONE_REASON_INLAY = 1,
			STONE_REASON_UNINLAY = 2,
			STONE_REASON_UPLEVEL = 3,
		};

		short reason;
		short total_stone_level;
		short stone_limit_flag[MAX_STONE_EQUIP_PART];
		short reserve;
		StonesOnPart stones_list[MAX_STONE_EQUIP_PART];
	};

	enum WASH_OPERATE_TYPE
	{
		WASH_OPERATE_TYPE_WASH = 0,
		WASH_OPERATE_TYPE_SAVE,
		WASH_OPERATE_TYPE_REQ,
	};

	// 宝石洗炼
	class CSWash
	{
	public:
		CSWash();
		MessageHeader header;
		
		int operate_type;

		short index;
		char reserve_ch;
		char is_autobuy;
		
		int lock_list[MAX_WASH_ATTR_COUNT];
	};

	class SCWashInfo
	{
	public:
		SCWashInfo();
		MessageHeader header;
		
		WashParam wash_param;
	};

	//////////////////////////////////////////////////////////////////////////
	class SCShengbingInfo				// 1713 下发神兵信息
	{
	public:
		SCShengbingInfo();
		MessageHeader header;

		char attach_status;											// 0：未附体，1：附体中，2：冻结，3：时间结束
		char reserve1;
		short reserve2;
		int attach_left_ms;											// 附体剩余时间 毫秒
		unsigned int expire_time_list[SHENBING_ID_MAX];				// 过期列表
	};

	class SCShenbingAppeChg				// 1714 神兵外观变化
	{
	public:
		SCShenbingAppeChg();
		MessageHeader header;

		ObjID obj_id;
		short shenbing_flag;			// flag & (1 << shenbing_id) 如果不等于0，则附身了神兵ID
	};

	class CSShenbingReq					// 1780 请求神兵信息
	{
	public:
		CSShenbingReq();
		MessageHeader header;
	};

	class CSShenbingCompose				// 1781 请求合成神兵
	{
	public:
		CSShenbingCompose();
		MessageHeader header;

		int shenbing_id;
	};

	class CSShenbingOP					// 1782 请求神兵操作
	{
	public:
		CSShenbingOP();
		MessageHeader header;

		short op_type;					// 1：附身，0：解体
		short reserve;
	};

	// 装备附灵  1771
	class CSEquipFuling
	{
	public:
		CSEquipFuling();
		MessageHeader header;
		
		short equip_index;				// 装备位置下标
		char is_puton;
		char reserve;
	};

	// 1757 锻造套装请求
	class CSDuanzaoSuitReq
	{
	public:
		CSDuanzaoSuitReq();
		MessageHeader header;

		enum EQUIPMENT_SUIT_OPERATE_TYPE
		{
			EQUIPMENT_SUIT_OPERATE_TYPE_INFO_REQ = 1,			// 信息请求
			EQUIPMENT_SUIT_OPERATE_TYPE_EQUIP_UP = 2,			// 升级请求
		};

		short operate_type;
		short equip_index;
	};

	// 1705 锻造套装等级信息
	class SCDuanzaoSuitInfo
	{
	public:
		SCDuanzaoSuitInfo();
		MessageHeader header;

		short suit_level_list[MAX_SUIT_EQUIP_PART];
	};
}

#pragma pack(pop)

#endif


