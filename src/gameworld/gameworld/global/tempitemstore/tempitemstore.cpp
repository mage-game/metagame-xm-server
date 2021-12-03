#include "tempitemstore.h"
#include "item/itempool.h"

TempItemStore::TempItemStore()
{

}

TempItemStore::~TempItemStore()
{

}

void TempItemStore::Clean(UserID uid)
{
	m_data.erase(uid);
}

void TempItemStore::PutItem(UserID uid, const ItemDataWrapper& item_data)
{
	const ItemBase *itembase = ITEMPOOL->GetItem(item_data.item_id);
	if (NULL == itembase) return;

	if (item_data.has_param == 0)
	{
		auto it = std::find_if(m_data[uid].begin(), m_data[uid].end(), [&](ItemDataWrapper& value) {
			if (value.IsEqual(item_data, true))
			{
				value.num += item_data.num;
				return true;
			}
			return false;
		});
		if (it != m_data[uid].end())
		{
			return;
		}
	}
	//有参数的不合并
	m_data[uid].push_back(item_data);
}

void TempItemStore::PutItem(UserID uid, const ItemConfigData& item_data)
{
	const ItemBase *itembase = ITEMPOOL->GetItem(item_data.item_id);
	if (NULL == itembase) return;

	ItemDataWrapper temp_item_wrapper;
	temp_item_wrapper.item_id = item_data.item_id;
	temp_item_wrapper.num = (short)item_data.num;
	temp_item_wrapper.is_bind = (itembase->IsBind() || item_data.is_bind) ? 1 : 0;
	temp_item_wrapper.invalid_time = itembase->CalInvalidTime();

	this->PutItem(uid, temp_item_wrapper);
}

const std::vector<ItemDataWrapper>& TempItemStore::GetItemList(UserID uid)
{
	return m_data[uid];
}

void TempItemStore::PutItemList(UserID uid, const ItemConfigData item_configdata_list[MAX_ATTACHMENT_ITEM_NUM])
{
	for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
	{
		if (ITEMPOOL->GetItem(item_configdata_list[i].item_id) != nullptr)
		{
			this->PutItem(uid, item_configdata_list[i]);
		}
	}
}