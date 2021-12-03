
#ifndef __MSGMAIL_HPP_
#define __MSGMAIL_HPP_

#include "servercommon/userprotocal/msgheader.h"

#include "gamedef.h"
#include "servercommon/serverdef.h"

#include "servercommon/maildef.hpp"

#pragma pack(push) 
#pragma pack(4)

namespace Protocol
{

	//  C -> S
	class CSMailSend								// 3750�����ʼ�
	{
	public:
		CSMailSend();
		MessageHeader			header;

		void ResetMailContentTxt()
		{
			memset(contenttxt, 0, sizeof(contenttxt));
		}

		int						recver_uid;
		int						gold;
		int						coin;

		short					item_count;
		short					item_knapindex_list[MAX_ATTACHMENT_ITEM_NUM];
		int						item_comsume_num[MAX_ATTACHMENT_ITEM_NUM];					

		MailSubject				subject;
		int						contenttxt_len;
		MailContentTxt			contenttxt;
	};

	class CSMailDelete								// 3751ɾ���ʼ�
	{
	public:
		CSMailDelete();
		MessageHeader			header;

		int						mail_index;
	};

	class CSMailGetList								// 3752��ȡ�ʼ��б�
	{
	public:
		CSMailGetList();
		MessageHeader			header;
	};

	class CSMailRead								// 3753��ȡ�ʼ�
	{
	public:
		CSMailRead();
		MessageHeader			header;

		int						mail_index;
	};

	class CSMailFetchAttachment						// 3754��ȡ����
	{
	public:
		CSMailFetchAttachment();
		MessageHeader			header;

		int						mail_index;

		int						item_index;			// ץȡ������Ʒʱ��ָ������ ����ץȡ����Ϊ-1
//		int						item_num;
		int						is_last;
	};

	class CSMailClean								// 3755����ʼ�
	{
	public:
		CSMailClean();
		MessageHeader			header;
	};

	class CSMailOneKeyFetchAttachment				// 3756 һ����ȡ����
	{
	public:
		CSMailOneKeyFetchAttachment();
		MessageHeader			header;
	};

}

#pragma pack(pop)

#endif

