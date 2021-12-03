#ifndef __FRIEND_GIFT_CONFIG_HPP__
#define __FRIEND_GIFT_CONFIG_HPP__

#include "obj/character/attribute.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/struct/friendgiftparam.hpp"
#include "servercommon/frienddef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct FfOtherCfg
{
	FfOtherCfg() : receive_gift_count_limit(0), add_intimacy(0), free_song_hua_times(0), heli_exp(0), heli_notice_time_interval(0){}

	ItemConfigData gift_item;			// ����item
	int receive_gift_count_limit;		// ÿ��������������
	int add_intimacy;					// ÿ�������������ܶ�
	int free_song_hua_times;
	ItemConfigData free_song_hua_reward;

	long long heli_exp;						// ���Ѻ����þ���
	int heli_notice_time_interval;		// ����֪ͨ����ʱ����
	int start_level;					// ���Ƶȼ�
};

struct FriendHeliCfg
{
	FriendHeliCfg() : type(-1), value(-1){}

	int type;
	int value;
};

class FriendGiftConfig : public ConfigBase
{
public:
	FriendGiftConfig();
	~FriendGiftConfig();
	
	bool Init(const std::string &configname, std::string *err);
	const FfOtherCfg &GetOtherCfg() { return m_other_cfg;}

	bool IsReqFriendHeli(int type, int value);

private:

	FfOtherCfg m_other_cfg;

	int InitOther(PugiXmlNode RootElement);
	int InitFriendHeliCfg(PugiXmlNode RootElement);

	FriendHeliCfg m_friend_heli_cfg[FRIENDNUM_HELI_COND_TYPE_MAX_COUNT];
};

#endif
