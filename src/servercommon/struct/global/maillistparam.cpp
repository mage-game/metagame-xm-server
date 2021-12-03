#include "maillistparam.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/struct/structcommon.h"

OLD_USE_INTERNAL_NETWORK();

bool MailListParam::Serialize(TLVSerializer &outstream) const
{
	bool ret = outstream.Push(count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_MAIL_NUM; ++i)
	{
		const char *sender_name_tmp = mail_list[i].mail_param.status.sender_name;
		const char *subject_tmp = mail_list[i].mail_param.contentparam.subject;
		const char *content_tmp = mail_list[i].mail_param.contentparam.contenttxt;

		ret = outstream.Push(mail_list[i].change_state)
			&& outstream.Push(mail_list[i].mail_param.mail_index)
			&& outstream.Push(mail_list[i].system_mail_id);
		if (!ret) return false;

		ret = outstream.Push(mail_list[i].check_uid);
		if (!ret) return false;

		if (mail_list[i].change_state == structcommon::CS_DELETE) continue;

		ret = outstream.Push(mail_list[i].mail_param.status.sender_uid) 
			&& outstream.Push(sender_name_tmp) 
			&& outstream.Push(mail_list[i].mail_param.status.recv_time) 
			&& outstream.Push(mail_list[i].mail_param.status.kind) 
			&& outstream.Push(mail_list[i].mail_param.status.is_read) 
			&& outstream.Push(mail_list[i].mail_param.status.is_lock) 
			&& outstream.Push(mail_list[i].mail_param.contentparam.coin) 
			&& outstream.Push(mail_list[i].mail_param.contentparam.coin_bind) 
			&& outstream.Push(mail_list[i].mail_param.contentparam.gold) 
			&& outstream.Push(mail_list[i].mail_param.contentparam.gold_bind) 
			&& outstream.Push(subject_tmp) 
			&& outstream.Push(content_tmp);
		if (!ret) return false;

		for (int k = 0; k < MAX_ATTACHMENT_ITEM_NUM; ++k)
		{
			if (!mail_list[i].mail_param.contentparam.item_list[k].Serialize(outstream)) return false;
		}

		TLVSerializer mail_virtual_data;
		mail_virtual_data.Reset((void*)&mail_list[i].mail_param.contentparam.virtual_item_list, sizeof(mail_list[i].mail_param.contentparam.virtual_item_list));
		mail_virtual_data.MoveCurPos(sizeof(mail_list[i].mail_param.contentparam.virtual_item_list));

		ret = outstream.Push(mail_virtual_data);
		if (!ret) return false;
	}
	
	return ret;
}

bool MailListParam::Unserialize(TLVUnserializer &instream)
{
	bool ret = instream.Pop(&count);
	if (!ret) return false;

	for (int i = 0; i < count && i < MAX_MAIL_NUM; ++i)
	{
		const char *sender_name_tmp = 0;
		const char *subject_tmp = 0;
		const char *content_tmp = 0;

		ret = instream.Pop(&mail_list[i].change_state)
			&& instream.Pop(&mail_list[i].mail_param.mail_index)
			&& instream.Pop(&mail_list[i].system_mail_id);
		if (!ret) return false;

		ret = instream.Pop(&mail_list[i].check_uid);
		if (!ret) return false;

		if (mail_list[i].change_state == structcommon::CS_DELETE) continue;

		ret = instream.Pop(&mail_list[i].mail_param.status.sender_uid)  
			&& instream.Pop(&sender_name_tmp) 
			&& instream.Pop(&mail_list[i].mail_param.status.recv_time) 
			&& instream.Pop(&mail_list[i].mail_param.status.kind) 
			&& instream.Pop(&mail_list[i].mail_param.status.is_read) 
			&& instream.Pop(&mail_list[i].mail_param.status.is_lock) 
			&& instream.Pop(&mail_list[i].mail_param.contentparam.coin) 
			&& instream.Pop(&mail_list[i].mail_param.contentparam.coin_bind) 
			&& instream.Pop(&mail_list[i].mail_param.contentparam.gold) 
			&& instream.Pop(&mail_list[i].mail_param.contentparam.gold_bind) 
			&& instream.Pop(&subject_tmp)
			&& instream.Pop(&content_tmp);
		if (!ret) return false;

		for (int k = 0; k < MAX_ATTACHMENT_ITEM_NUM; ++k)
		{
			if (!mail_list[i].mail_param.contentparam.item_list[k].Unserialize(instream)) return false;
		}

		TLVUnserializer mail_virtual_data;
		ret = instream.Pop(&mail_virtual_data);
		if (!ret) return false;

		if (mail_virtual_data.Size() <= sizeof(mail_list[i].mail_param.contentparam.virtual_item_list))
		{
			memcpy(&mail_list[i].mail_param.contentparam.virtual_item_list, mail_virtual_data.Ptr(), mail_virtual_data.Size());
		}

		STRNCPY(mail_list[i].mail_param.status.sender_name, sender_name_tmp, sizeof(mail_list[0].mail_param.status.sender_name));
		STRNCPY(mail_list[i].mail_param.contentparam.subject, subject_tmp, sizeof(mail_list[0].mail_param.contentparam.subject));
		STRNCPY(mail_list[i].mail_param.contentparam.contenttxt, content_tmp, sizeof(mail_list[0].mail_param.contentparam.contenttxt));
	}

	return ret;
}

//////////////////////////////////////////////////////////////////////////
bool SystemMailEventParam::Serialize(TLVSerializer &outstream) const
{
	const char *subject_tmp = contentparam.subject;
	const char *content_tmp = contentparam.contenttxt;

	bool ret = outstream.Push(condition)
		&& outstream.Push(param)
		&& outstream.Push(contentparam.coin) 
		&& outstream.Push(contentparam.coin_bind) 
		&& outstream.Push(contentparam.gold) 
		&& outstream.Push(contentparam.gold_bind) 
		&& outstream.Push(subject_tmp) 
		&& outstream.Push(content_tmp);
	if (!ret) return false;

	for (int k = 0; k < MAX_ATTACHMENT_ITEM_NUM; ++k)
	{
		if (!contentparam.item_list[k].Serialize(outstream)) return false;
	}

	TLVSerializer mail_virtual_data;
	mail_virtual_data.Reset((void*)&contentparam.virtual_item_list, sizeof(contentparam.virtual_item_list));
	mail_virtual_data.MoveCurPos(sizeof(contentparam.virtual_item_list));

	ret = outstream.Push(mail_virtual_data);
	if (!ret) return false;

	return ret;
}

bool SystemMailEventParam::Unserialize(TLVUnserializer &instream)
{
	const char *subject_tmp = 0;
	const char *content_tmp = 0;

	bool ret = instream.Pop(&condition) 
		&& instream.Pop(&param) 
		&& instream.Pop(&contentparam.coin) 
		&& instream.Pop(&contentparam.coin_bind) 
		&& instream.Pop(&contentparam.gold) 
		&& instream.Pop(&contentparam.gold_bind) 
		&& instream.Pop(&subject_tmp)
		&& instream.Pop(&content_tmp);
	if (!ret) return false;

	for (int k = 0; k < MAX_ATTACHMENT_ITEM_NUM; ++k)
	{
		if (!contentparam.item_list[k].Unserialize(instream)) return false;
	}

	TLVUnserializer mail_virtual_data;
	ret = instream.Pop(&mail_virtual_data);
	if (!ret) return false;

	if (mail_virtual_data.Size() <= sizeof(contentparam.virtual_item_list))
	{
		memcpy(&contentparam.virtual_item_list, mail_virtual_data.Ptr(), mail_virtual_data.Size());
	}

	STRNCPY(contentparam.subject, subject_tmp, sizeof(contentparam.subject));
	STRNCPY(contentparam.contenttxt, content_tmp, sizeof(contentparam.contenttxt));

	return ret;
}

