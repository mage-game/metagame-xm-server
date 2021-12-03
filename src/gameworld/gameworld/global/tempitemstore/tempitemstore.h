#ifndef _TEMPITEMSTORE_HPP_
#define _TEMPITEMSTORE_HPP_
#include "servercommon/serverdef.h"
#include "servercommon/struct/itemlistparam.h"
class TempItemStore
{
public:
	TempItemStore();
	~TempItemStore();

	static TempItemStore& Instance() 
	{
		static TempItemStore ins;
		return ins;
	}

	void Clean(UserID uid);
	void PutItem(UserID uid, const ItemDataWrapper& item_data );
	void PutItem(UserID uid, const ItemConfigData& item_data);
	void PutItemList(UserID uid, const ItemConfigData item_configdata_list[MAX_ATTACHMENT_ITEM_NUM]);
	const std::vector<ItemDataWrapper>& GetItemList(UserID uid);

private:
	std::map<UserID, std::vector<ItemDataWrapper>> m_data;
	
};

#endif
