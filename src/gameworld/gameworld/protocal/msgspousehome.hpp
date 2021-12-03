#pragma once
#include "servercommon/userprotocal/msgheader.h"
#include "global/globalrole/spousehome/spousehome.hpp"
#include "servercommon/frienddef.hpp"
#include "servercommon/guilddef.hpp"

namespace Protocol
{
	enum CS_SPOUSE_HOME_TYPE
	{
		CS_SPOUSE_HOME_TYPE_ALL_INFO,				  // 请求所有消息
		CS_SPOUSE_HOME_TYPE_FURNITURE_EQUIT,          // 装备家具 p1 房间索引 p2 家具类型 p3 背包索引
		CS_SPOUSE_HOME_TYPE_FURNITURE_TAKE_OFF,       // 卸下家具 p1 房间索引   p2 道具索引
		CS_SPOUSE_HOME_TYPE_BUY_THEME,				  // 购买房间 p1 主题索引
		CS_SPOUSE_HOME_TYPE_VIEW_OTHER_PEOPLE_ROOM,	  // 查看房间 p1 对方uid
		CS_SPOUSE_HOME_TYPE_BUY_FURNITURE_ITEM,		  // 购买道具 p1 物品id     p2 物品数量
		CS_SPOUSE_HOME_TYPE_FRIEND_LIST_INFO,		  // 请求下发好友信息
		CS_SPOUSE_HOME_TYPE_GUILD_MEMBER_INFO,		  // 请求下发盟友信息
		CS_SPOUSE_HOME_TYPE_BUY_THEME_FOR_LOVER,		// 为伴侣购买房间 p1 uid p2 主题索引
		CS_SPOUSE_HOME_TYPE_FURNITURE_EQUIT_FOR_LOVER,  // 为伴侣装备家具 p1 uid p2 房间索引 p3 家具类型 p4 背包索引
	};

	//夫妻家园协议

	struct CSSpouseHomeOperaReq		// 8842
	{
		CSSpouseHomeOperaReq();
		MessageHeader header;

		int info_type;
		int param1;
		int param2;
		int param3;
		int param4;
	};

	struct SpouseHomeRoomParam
	{
		short room_index;						// 房间索引
		short theme_type;						// 主题类型
		ItemID furniture_slot_list[SPOUSE_HOME_FURNITURE_MAX_ITEM_SLOT];	// item_id
	};

	struct SCSpouseHomeRoomInfo					// 8843 房间信息
	{
		SCSpouseHomeRoomInfo();
		MessageHeader header;

		int uid;
		short room_cout;
		short pet_id;
		short room_furniture_limit;
		short reserve_sh;
		SpouseHomeRoomParam room_msg[SPOUSE_HOME_MAX_ROOM_NUM];
	};

	struct FriendAndGuildMemberInfo
	{
		int uid;
		char room_count;
		char sex;
		char prof;
		char reserve_ch;
		GameName name;
	};

	struct SCSpouseHomeFirendInfo	// 8844
	{
		SCSpouseHomeFirendInfo();
		MessageHeader header;

		short firend_count;
		short reserve_sh;
		FriendAndGuildMemberInfo firend_info_list[MAX_FRIEND_NUM];
	};

	struct SCSpouseHomeGuildMemberInfo	// 8845
	{
		SCSpouseHomeGuildMemberInfo();
		MessageHeader header;

		short guild_member_count;
		short reserve_sh;
		FriendAndGuildMemberInfo guild_member_info_list[MAX_MEMBER_COUNT];
	};

	struct SCSpouseHomeSingleRoomInfo					// 8846 单个房间信息
	{
		SCSpouseHomeSingleRoomInfo();
		MessageHeader header;

		short pet_id;
		short room_furniture_limit;
		SpouseHomeRoomParam room_msg;
	};
}