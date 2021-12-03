#pragma once
#include "servercommon/userprotocal/msgheader.h"
#include "global/globalrole/spousehome/spousehome.hpp"
#include "servercommon/frienddef.hpp"
#include "servercommon/guilddef.hpp"

namespace Protocol
{
	enum CS_SPOUSE_HOME_TYPE
	{
		CS_SPOUSE_HOME_TYPE_ALL_INFO,				  // ����������Ϣ
		CS_SPOUSE_HOME_TYPE_FURNITURE_EQUIT,          // װ���Ҿ� p1 �������� p2 �Ҿ����� p3 ��������
		CS_SPOUSE_HOME_TYPE_FURNITURE_TAKE_OFF,       // ж�¼Ҿ� p1 ��������   p2 ��������
		CS_SPOUSE_HOME_TYPE_BUY_THEME,				  // ���򷿼� p1 ��������
		CS_SPOUSE_HOME_TYPE_VIEW_OTHER_PEOPLE_ROOM,	  // �鿴���� p1 �Է�uid
		CS_SPOUSE_HOME_TYPE_BUY_FURNITURE_ITEM,		  // ������� p1 ��Ʒid     p2 ��Ʒ����
		CS_SPOUSE_HOME_TYPE_FRIEND_LIST_INFO,		  // �����·�������Ϣ
		CS_SPOUSE_HOME_TYPE_GUILD_MEMBER_INFO,		  // �����·�������Ϣ
		CS_SPOUSE_HOME_TYPE_BUY_THEME_FOR_LOVER,		// Ϊ���¹��򷿼� p1 uid p2 ��������
		CS_SPOUSE_HOME_TYPE_FURNITURE_EQUIT_FOR_LOVER,  // Ϊ����װ���Ҿ� p1 uid p2 �������� p3 �Ҿ����� p4 ��������
	};

	//���޼�԰Э��

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
		short room_index;						// ��������
		short theme_type;						// ��������
		ItemID furniture_slot_list[SPOUSE_HOME_FURNITURE_MAX_ITEM_SLOT];	// item_id
	};

	struct SCSpouseHomeRoomInfo					// 8843 ������Ϣ
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

	struct SCSpouseHomeSingleRoomInfo					// 8846 ����������Ϣ
	{
		SCSpouseHomeSingleRoomInfo();
		MessageHeader header;

		short pet_id;
		short room_furniture_limit;
		SpouseHomeRoomParam room_msg;
	};
}