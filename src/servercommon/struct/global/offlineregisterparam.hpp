#ifndef __OFFLINE_REGISTER_PARAM_HPP__
#define __OFFLINE_REGISTER_PARAM_HPP__

#include "common/tlvprotocol.h"
#include "servercommon/serverdef.h"
#include "servercommon/guilddef.hpp"
#include "servercommon/qingyuandef.hpp"

struct OfflineRegisterCommonParam
{
	OfflineRegisterCommonParam()
	{
		this->Reset();
	}

	bool CanRemove()
	{
		return -1 == authority_type && 0 == forbid_talk_end_timestamp &&
			item_id == 0 && item_num == 0 && change_gold == 0 && change_bind_gold == 0; 
	}

	void Reset()
	{
		authority_type = -1;
		reserve_sh = 0;
		forbid_talk_end_timestamp = 0;
		item_id = 0;
		item_num = 0;
		change_gold = 0;
		change_bind_gold = 0;
	}

	short authority_type;
	short reserve_sh;
	unsigned int forbid_talk_end_timestamp;

	//����ɾ����ɫ��Ʒ
	unsigned short item_id;
	unsigned short item_num;

	int change_gold;
	int change_bind_gold;
};


class OfflineRegisterParam
{
public:
	bool Serialize(TLVSerializer &outstream) const;
	bool Unserialize(TLVUnserializer &instream);

	struct OfflineRegisterAttr
	{
		long long id_offline_register;			// ������ʼ����ʱ������ 
		char change_state; 

		int role_id;

		int increase_charm;
		unsigned int last_divorce_time;
		int chongzhi_cmd_flag;
		int red_paper_consume_gold;
		int red_paper_lucky_value;

		int space_cai_count;												// ���� ����
		int space_gift_count;												// ����������

		int qingyuan_bless_days;											// ��Եף������

		GuildBoxInfo guild_box_info;										// ���˱�����Ϣ

		OfflineRegisterCommonParam common_param;							// ������Ϣ
	};

	static const int MAX_OFFLINE_REGISTER_COUNT = 256;

	int count;
	OfflineRegisterAttr offline_register_list[MAX_OFFLINE_REGISTER_COUNT];
};

typedef char OfflineRegisterCommonParamHex[sizeof(OfflineRegisterCommonParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(OfflineRegisterCommonParamHex) < 128);

#endif  // __OFFLINE_REGISTER_PARAM_HPP__

