#ifndef __DEF_GIFT_H__
#define __DEF_GIFT_H__

#include "giftitem.hpp"

class DefGift : public GiftItem
{
public:
	DefGift();
	~DefGift();

	virtual bool Init(const char* path, std::string *err);
	virtual int Use(Role *role, const ItemGridData &item_data, short &num, short equip_index = -1) const;
	void SendDefGiftItemInfo(Role* role, ItemConfigData* item_cfg_list, int count) const;

	ItemConfigData *  GetGiftItemItem(int index);
private:
	int m_need_gold;
};

#endif //__DEF_GIFT_H__


