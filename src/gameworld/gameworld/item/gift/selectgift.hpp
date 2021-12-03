#ifndef __SELECT_GIFT_H__
#define __SELECT_GIFT_H__

#include "giftitem.hpp"

class SelectGift : public GiftItem
{
public:
	SelectGift();
	~SelectGift();

	virtual bool Init(const char* path, std::string *err);
	virtual int Use(Role *role, const ItemGridData &item_data, short &num, short select_index = -1) const;

private:
	int m_need_gold;
};

#endif //__SELECT_GIFT_H__


