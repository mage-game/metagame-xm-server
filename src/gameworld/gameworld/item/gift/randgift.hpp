#ifndef __RAND_GIFT_H__
#define __RAND_GIFT_H__

#include "giftitem.hpp"


class RandGift : public GiftItem
{
public:
	
	static const int RAND_GIFT_BROADCAST_ITEM_NUM = 5;			// 随机礼包开出物品需要播报传闻

	RandGift();
	~RandGift();

	virtual bool Init(const char* path, std::string *err);
	virtual int Use(Role *role, const ItemGridData &item_data, short &num, short equip_index = -1) const;
	void SendRandGiftItemInfo(Role* role, ItemConfigData* item_cfg_list, int count) const;

private:
	bool GetRandGiftItemList(Role *role, bool is_bind, int list_num, ItemConfigData *config_list, int *index_count) const;

	int m_rand_num;
	short m_item_rate_list[GIFT_ITEM_MAX_NUM];

	int m_sum_rate;

	ItemID m_boardcast_item_list[RAND_GIFT_BROADCAST_ITEM_NUM];
	int m_boardcast_string_id;
	int m_need_gold;
};

#endif //__RAND_GIFT_H__


