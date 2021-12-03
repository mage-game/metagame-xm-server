#include "itembase.h"
#include "itemgriddata.h"

#include "equipment/equipment.h"

#include "gamedef.h"
#include "itempool.h"

#include "engineadapter.h"
#include "servercommon/configcommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "item/itemextern.hpp"

ItemDataWrapper::ItemDataWrapper(ItemID _item_id, short _num, short _is_bind, ItemParam *item_param, UInt32 _invalid_time, int _gold_price) : item_id(_item_id), num(_num), is_bind(_is_bind), invalid_time(_invalid_time), gold_price(_gold_price)
{
	if (NULL != item_param)
	{
		has_param = item_param->SetInStructData(&param_data);
	}
}

//////////////////////////////////////////////////////////////////////////

ItemBase::ItemBase(short i_type) 
	: m_item_type(i_type), 
	m_search_type(0), 
	m_item_id(0), 
	m_color(0), 
	m_is_bind(false), 
	m_can_discard(false), 
	m_can_sell(false),
	m_is_broadcast(false), 
	m_sell_price(0), 
	m_recycl_type(0),
	m_recycl_get(0),
	m_is_record(true),
	m_time_length(0),
	m_invalid_time(0),
	m_is_curday_valid(false),
	m_limit_prof(0),
	m_limit_level(0),
	m_limit_sex(0),
	m_other_sex_itemid(0),
	m_pile_limit(0)
{
	memset(m_item_name, 0, sizeof(m_item_name));
}

ItemBase::~ItemBase()
{

}

bool ItemBase::CanDiscard(ItemParam *param, int *reason) const
{
	if (NULL != reason) *reason = NDR_DEFAULT;

	return m_can_discard;
}

bool ItemBase::CanMerge(UInt16 item_id)
{
	const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
	if (itembase == NULL) return false;

	return I_TYPE_EQUIPMENT != itembase->GetItemType();  // 非装备都可以叠加
}

bool ItemBase::CanTrade(const ItemGridData *item_data)
{
	if (NULL == item_data || item_data->is_bind) return false;

	return true;
}

bool ItemBase::HasInvalidTime() const
{
	return m_time_length > 0 || m_invalid_time > 0;
}

UInt32 ItemBase::CalInvalidTime() const
{
	UInt32 tmp_time = 0;
	if (m_time_length > 0)
	{
		tmp_time = static_cast<UInt32>(EngineAdapter::Instance().Time()) + m_time_length;
	}

	if (m_invalid_time > 0 && (0 == tmp_time || tmp_time > m_invalid_time))
	{
		tmp_time = m_invalid_time;
	}

	if (m_invalid_time <= 0 && m_is_curday_valid)
	{
		tmp_time = static_cast<unsigned int>(GetZeroTime(EngineAdapter::Instance().Time())) + 3600 * 24;
	}

	return tmp_time;
}

bool ItemBase::CheckIsSameItemExceptSex(const ItemBase *other_item) const
{
	if (NULL == other_item) return false;

	bool ret = m_can_sell == other_item->CanSell(NULL);
	ret &= m_color == other_item->GetColor();
	ret &= m_sell_price == other_item->GetSellPrice(NULL);
	ret &= m_recycl_type == other_item->GetRecyclType(NULL);
	ret &= m_recycl_get == other_item->GetRecyclGet(NULL);
	ret &= m_limit_level == other_item->GetLimitLevel();
	ret &= m_limit_prof == other_item->GetLimitProf();
	ret &= m_item_type == other_item->GetItemType();
	ret &= m_is_broadcast == other_item->IsBroadcast();
	ret &= m_is_record == other_item->IsRecord();
	ret &= m_is_bind == other_item->IsBind();
	ret &= m_pile_limit == other_item->GetPileLimit();
	
	return ret;
}
