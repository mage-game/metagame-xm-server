#ifndef __OTHER_ITEM_HPP__
#define __OTHER_ITEM_HPP__

#include "item/itembase.h"

class OtherItem : public ItemBase
{
public:
	enum OTHER_ITEM_SUB_TYPE
	{
		I_SUB_TYPE_INVALID = 0,
		I_SUB_TYPE_FLOWER,								// œ ª®

		OTHER_ITEM_SUB_TYPE_NUM,
	};

	OtherItem() : ItemBase(I_TYPE_OTHER), m_sub_type(I_SUB_TYPE_INVALID), m_value(0), m_guild_storage_score(0) {}
	virtual ~OtherItem() {}

	virtual bool Init(const char* path, std::string *err);
	virtual int Use(Role *role, const ItemGridData &item_data, short &num, short equip_index = -1) const;

	short GetSubType() const { return m_sub_type; }
	int GetValue() const { return m_value; }
	int GetGuildStorageScore() const { return m_guild_storage_score; }

protected:
	short m_sub_type;
	int m_value;
	int m_guild_storage_score;										// œ…√À≤÷ø‚÷∞∑÷
};

#endif // __OTHER_ITEM_HPP__

