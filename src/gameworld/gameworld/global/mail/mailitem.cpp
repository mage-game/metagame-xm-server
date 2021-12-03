#include <memory>
#include "mailitem.hpp"

#include "servercommon/servercommon.h"
#include "engineadapter.h"

MailItem::MailItem()
	: last_fetch_time(0)
{
}

MailItem::~MailItem()
{

}

bool MailItem::OperTimeIsValid()
{
	unsigned int now_sceond = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_sceond >= last_fetch_time + 2)
	{
		last_fetch_time = now_sceond;	
		return true;
	}

	return false;
}

bool MailItem::HasAttachment() const
{ 
	UNSTD_STATIC_CHECK(3 == MAX_ATTACHMENT_ITEM_NUM);
	
	for (int i = 0; i < MAX_MAIL_VIRTUAL_ITEM_NUM; i++)
	{
		if (mail_param.contentparam.virtual_item_list.virtual_item_type[i] >= 0 &&
			mail_param.contentparam.virtual_item_list.virtual_item_type[i] < MAIL_VIRTUAL_ITEM_MAX &&
			0 != mail_param.contentparam.virtual_item_list.virtual_item[i])
		{
			return true;
		}
	}

	return mail_param.contentparam.coin > 0 || mail_param.contentparam.coin_bind > 0 || mail_param.contentparam.gold > 0 || mail_param.contentparam.gold_bind > 0 ||
		mail_param.contentparam.item_list[0].item_id > 0 || mail_param.contentparam.item_list[1].item_id > 0 || mail_param.contentparam.item_list[2].item_id > 0;
}

void MailItem::ClearAttachment()
{
	mail_param.contentparam.coin = 0; mail_param.contentparam.coin_bind = 0;
	mail_param.contentparam.gold = 0; mail_param.contentparam.gold_bind = 0;
	
	for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; i++)
	{
		mail_param.contentparam.item_list[i].Reset();
	}

	memset((void*)&mail_param.contentparam.virtual_item_list, 0, sizeof(mail_param.contentparam.virtual_item_list));
}

bool MailItem::Invalid()
{
	return mail_param.status.kind <= MAIL_TYPE_INVALID || mail_param.status.kind >= MAIL_TYPE_MAX;
}

