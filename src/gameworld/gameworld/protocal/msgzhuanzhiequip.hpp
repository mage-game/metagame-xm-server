#ifndef __MSG_ZHUANZHI_EQUIP_H__
#define __MSG_ZHUANZHI_EQUIP_H__

#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/zhuanzhiequipdef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{

	// 转职装备操作类型
	enum ZHUANZHI_EQUIP_OPERATE_TYPE
	{
		ZHUANZHI_EQUIP_OPERATE_TYPE_EQUIP_INFO = 0,		// 请求装备信息
		ZHUANZHI_EQUIP_OPERATE_TYPE_STONE_INFO,			// 请求玉石信息
		ZHUANZHI_EQUIP_OPERATE_TYPE_SUIT_INFO,			// 请求套装信息

		ZHUANZHI_EQUIP_OPERATE_TYPE_TAKE_OFF,			// 脱下 p1: part_index
		ZHUANZHI_EQUIP_OPERATE_TYPE_UP_STAR,			// 升星 p1: part_index
		ZHUANZHI_EQUIP_OPERATE_TYPE_FULING,				// 附灵 p1: part_index

		ZHUANZHI_EQUIP_OPERATE_TYPE_INLAY_STONE,		// 镶嵌玉石 p1: part_index p2: slot_index p3: bag_index
		ZHUANZHI_EQUIP_OPERATE_TYPE_UNINLAY_STONE,		// 卸下玉石 p1: part_index p2: slot_index 
		ZHUANZHI_EQUIP_OPERATE_TYPE_UP_LEVEL,			// 升级玉石	p1: part_index p2: slot_index 
		ZHUANZHI_EQUIP_OPERATE_TYPE_REFINE_STONE,		// 精炼玉石 p1: part_index p2: seq p3: is_autobuy
		ZHUANZHI_EQUIP_OPERATE_TYPE_FORGE_SUIT,			// 锻造套装	p1: suit_index p2: part_index 
		ZHUANZHI_EQUIP_OPERATE_TYPE_ZHIZUN_COMPOSE,		// 至尊装备合成	p1: compose_id p2: best_attr_num p3: equip_index (身上装备索引,-1为背包） p4:bag_index1 p5:bag_index2

		ZHUANZHI_EQUIP_OPERATE_TYPE_STONE_RESOLVE,		// 玉石分解	p1: bag_index 
		ZHUANZHI_EQUIP_OPERATE_TYPE_STONE_CONVERT,		// 玉石兑换	p1: seq 

		ZHUANZHI_EQUIP_OPERATE_TYPE_AWAKENING_INFO,		// 请求觉醒信息
		ZHUANZHI_EQUIP_OPERATE_TYPE_AWAKENING_WAKE,		// 觉醒 p1: part_index p2: 自动购买
		ZHUANZHI_EQUIP_OPERATE_TYPE_AWAKENING_REPLACE,	// 替换 p1: part_index
		ZHUANZHI_EQUIP_OPERATE_TYPE_AWAKENING_LOCK,		// 加锁 p1: part_index, p2: lock_index
		ZHUANZHI_EQUIP_OPERATE_TYPE_AWAKENING_UNLOCK,	// 解锁 p1: part_index, p2: lock_index
	};

	class CSZhuanzhiEquipOpe		// 8770
	{
	public:
		CSZhuanzhiEquipOpe();
		MessageHeader header;						

		short operate;                                  // 操作类型（0 = 请求信息，1 = 脱下...）
		short param1;
		short param2;
		short param3;
		short param4;
		short param5;
	};

	class SCZhuanzhiEquipInfo			// 8771 转职装备信息
	{
	public:
		SCZhuanzhiEquipInfo();
		MessageHeader header;

		ZhuanzhiEquipItem zhuanzhi_equip_list[E_INDEX_MAX];
	};

	class SCZhuanzhiStoneInfo			// 8772 转职玉石信息
	{
	public:
		SCZhuanzhiStoneInfo();
		MessageHeader header;

		UInt32 stone_score;									// 玉石精粹
		ZhuanzhiStonesPartData stone_list[E_INDEX_MAX];
	};

	class SCZhuanzhiSuitInfo			// 8773 转职套装信息
	{
	public:
		SCZhuanzhiSuitInfo();
		MessageHeader header;

		char part_suit_type_list[E_INDEX_MAX];		// 套装类型列表
		char part_order_list[E_INDEX_MAX];			// 阶数列表
	};

	class SCZhuanzhiEquipAwakeningAllInfo			// 8928 所有转职装备觉醒信息
	{
	public:
		SCZhuanzhiEquipAwakeningAllInfo();
		MessageHeader header;

		unsigned char zhuanzhi_equip_awakening_lock_flag[MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_BYTE];
		ZhuanzhiEquipAwakening zhuanzhi_equip_awakening_list[E_INDEX_MAX];		// 觉醒信息
	};

	class SCZhuanzhiEquipAwakeningInfo				// 8929 一件转职装备觉醒信息
	{
	public:
		SCZhuanzhiEquipAwakeningInfo();
		MessageHeader header;

		int index;												// 索引
		unsigned char zhuanzhi_equip_awakening_lock_flag[MAX_ZHUANZHI_EQUIP_AWAKENING_LOCK_BYTE];
		ZhuanzhiEquipAwakening zhuanzhi_equip_awakening;		// 觉醒信息
	};

	class CSZhuanzhiEquipCompose		// 8952
	{
	public:
		CSZhuanzhiEquipCompose();
		MessageHeader header;

		ItemID item_id;												//合成的装备id
		short xianpin_num;											//仙品数量
		short bag_index_count;										//背包索引数量
		short bag_index_list[MAX_ZHUANZHI_EQUIP_COMPOSE_COUNT];		//背包索引列表
	};

	class SCZhuanzhiEquipComposeSucceed				// 8957 转职装备合成成功
	{
	public:
		SCZhuanzhiEquipComposeSucceed();
		MessageHeader header;

		short is_succeed;												// 是否成功
		short reverse_sh;
	};
}

#pragma pack(pop)

#endif

