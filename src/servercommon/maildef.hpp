/******************************************************************************************
FileName: maildef.hpp
Author: 
Description: �ʼ�ϵͳ����  
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang	2011/07/17/ 09:04:05     1.0           ����
*****************************************************************************************/
#ifndef __MAIL_DEF_HPP__
#define __MAIL_DEF_HPP__

#include <memory>

#include "servercommon/serverdef.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/bitmapfunction.hpp"

#pragma pack(push, 4)

static const int MIN_GET_MAIL_LIST_INTERVAL = 10;				// �����û���ȡ�ʼ��б��ʱ��
static const int MAIL_SAVE_DB_INTERVAL = 10;					// ��дDB��ʱ����
static const int GET_SYSTEM_MAIL_INTERVAL = 60 * 5;				// ȥDB��ϵͳ�ʼ���ʱ���� 
static const int MAX_GET_MAIL_TIMEOUT_INTERVAL = 15;			// ��ʱ ���ڼ�����״̬�����ʱ����
static const int MAIL_UN_ACTIVE_TIMEOUT_INTERVAL = 60 * 10;		// ���Ĳ���Ծʱ�� ��̭
static const int MAX_MAIL_CHECK_STATE_INTERVAL = 10;			// ����û��ʼ�״̬ʱ����

static const int MAX_SUBJECT_LENTH = 128;						// ���ⳤ��
static const int MAX_CONTENT_TXT_LENTH = 512;					// ���ݳ��� �ͻ��˳�����512 ������Ԥ����һ�� ���ڽض�

static const int MAX_MAIL_NUM = 160;							// �������п�ӵ�е��ʼ�
static const int MAX_MAIL_SEND_LIST_SIZE = 50;					// �ְ��� һ�η��ĸ��� 

static const int MAX_PERSONAL_MAIL = MAX_MAIL_NUM - 20;			// ˽���ʼ���������֮ǰ20��Ͳ��ܷ���
static const int MAX_LOCK_MAIL_NUM = 10;						// �������10�� 
static const int MAX_MAIL_RESERVER_TIME = 3600 * 24 * 15;		// ��ౣ��7��

static const int MAIL_POSTAGE = 500;							// ���ʣ�500ͭ��
static const int MAIL_POSTAGE_ATTACHMENT = 500;					// ���ʴ�������500ͭ��

static const int MAX_GAMEWORLD_SEND_MAIL_USER = 2048;			// ֧��ͬʱ��2048���˷��ʼ�

static const int MAX_ATTACHMENT_ITEM_NUM = 3;					// ��󸽼���Ʒ�� ����ı���Ҫͬʱ�޸����ݿ�

static const int MULTI_MAX_ATTACHMENT_ITEM_NUM = 6;				// ��󸽼���Ʒ�� ���ڶ���ʼ� 

static const int MAX_VIRTUAL_ITEM_NUM = 15;						// ���������Ʒ�� �������� ս��

static const int MAX_MAIL_VIRTUAL_ITEM_NUM = 4;					// �ʼ����������Ʒ��

static const int MAIL_SEND_OPEN_LEVEL = 199;						// �����ʼ����ŵȼ�

typedef char MailSubject[128];					// �ʼ�����
typedef char MailContentTxt[512];				// �ʼ�����

enum MAIL_TYPE 
{
	MAIL_TYPE_INVALID = 0,
	MAIL_TYPE_PERSONAL,											// �����ʼ�
	MAIL_TYPE_SYSTEM,											// ϵͳ�ʼ�
	MAIL_TYPE_GUILD,											// �����ʼ�
	MAIL_TYPE_CHONGZHI,											// ��ֵ�ʼ�
	MAIL_TYPE_MAX,
};

enum MAIL_STATE
{
	MAIL_STATE_INVALID = 0,										// ��Ч״̬
	MAIL_STATE_LOAD_ING,										// ������
	MAIL_STATE_LOAD_OK,											// ������� 
};

enum MAIL_VIRTUAL_ITEM
{
	MAIL_VIRTUAL_ITEM_INVALID = -1,

	MAIL_VIRTUAL_ITEM_BATTLEFIELDHONOR = 0,						// ս������
	MAIL_VIRTUAL_ITEM_EXP = 1,									// ����
	MAIL_VIRTUAL_ITEM_GUILDGONGXIAN = 2,						// ���˹���
	MAIL_VIRTUAL_ITEM_SHENGWANG = 3,							// ����
	MAIL_VIRTUAL_ITEM_BIND_GOLD = 4,							// ��Ԫ��
	MAIL_VIRTUAL_ITEM_GOLD = 5,									// Ԫ��
	MAIL_VIRTUAL_ITEM_BIND_COIN = 6,							// ��ͭ��
	MAIL_VIRTUAL_ITEM_CROSS_HONOR = 7,							// �������
	MAIL_VIRTUAL_ITEM_SHENMOZHILI = 8,							// ��ħ֮��
	MAIL_VIRTUAL_ITEM_GONGXUN = 9,								// ��ѫ
	MAIL_VIRTUAL_ITEM_CONVERTSHOP_MOJING = 10,					// ħ��-�һ��̵����
	MAIL_VIRTUAL_ITEM_CONVERTSHOP_SHENGWANG = 11,				// ����-�һ��̵����
	MAIL_VIRTUAL_ITEM_CONVERTSHOP_GONGXUN = 12,					// ��ѫ-�һ��̵����
	MAIL_VIRTUAL_ITEM_CONVERTSHOP_WEIWANG = 13,					// ����-�һ��̵����
	MAIL_VIRTUAL_ITEM_CONVERTSHOP_HUNJING = 14,					// �꾧-�һ��̵����
	MAIL_VIRTUAL_ITEM_TIANSHENHUTI_SCORE = 15,					// ��ĩװ���齱����

	MAIL_VIRTUAL_ITEM_MAX,
};

//UNSTD_STATIC_CHECK(MAIL_VIRTUAL_ITEM_MAX < MAX_VIRTUAL_ITEM_NUM);

enum SYSTEM_MAIL_REASON
{
	SYSTEM_MAIL_REASON_INVALID = 0,

	SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_FULL,				// ���۵���Ʒ���� �˻�
	SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_CANCEL,				// ȡ������ 
	SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_EXPIRE,				// �������� 
	SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_BUY_SUCC,			// ����ɹ�
	SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_MONEY_BUY_FAIL,			// ����ʧ�� ��Ǯ
	SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_MONEY_SALE_SUCC,			// ���۳ɹ�

	SYSTEM_MAIL_REASON_GUILD_CREATE,							// ���Ŵ���֪ͨ
	SYSTEM_MAIL_REASON_GUILD_DIMISS,							// ���Ž�ɢ֪ͨ
	SYSTEM_MAIL_REASON_GUILD_BEKICKED,							// ��������֪ͨ
	SYSTEM_MAIL_REASON_GUILD_DELATE,							// ���ŵ���

	SYSTEM_MAIL_REASON_MAIL_RECEIVER_FULL,						// �ʼ����շ�������

	SYSTEM_MAIL_REASON_YIZHANDAODI_LUCKY_REWARD,				// һվ�������˽���
	SYSTEM_MAIL_REASON_WUXINGGUAJI_LUCKY_REWARD,				// ���йһ����˽���

	SYSTEM_MAIL_REASON_GCZ_RANK_REWARD,							// ����ս��������
	SYSTEM_MAIL_REASON_GCZ_CHENGZHU_REWARD,						// ����ս��������
	SYSTEM_MAIL_REASON_DAILY_TASK_FINISH_REWARD,				// �ճ�������ɽ���
	SYSTEM_MAIL_REASON_GCZ_LUCKY_REWARD,						// ����ս���˽���
	SYSTEM_MAIL_REASON_LTZ_LUCKY_REWARD,						// ����ս���˽���
	SYSTEM_MAIL_REASON_GHZB_LUCKY_REWARD,						// �����������˽���
	SYSTEM_MAIL_REASON_YSZC_LUCKY_REWARD,						// Ԫ��ս�����˽���

	SYSTEM_MAIL_REASON_MAX,
};

enum MAIL_FETCHATTCHMENT_TYPE
{
	MAIL_FETCH_VIRTUAL = -2,
	MAIL_FETCH_ALL = -1,
};

enum SYSMAIL_EVENT_CONDITION
{
	SYSMAIL_EVENT_CONDITION_INVALID = 0,
	SYSMAIL_EVENT_CONDITION_LEVEL_EQUAL_GREATER,				// �ȼ������
	SYSMAIL_EVENT_CONDITION_MAX,
};

struct MailStatus
{
	MailStatus() : sender_uid(0), recv_time(0), kind(0), is_read(0), is_lock(0), vip_level(0)
	{
		memset(sender_name, 0, sizeof(sender_name));
	}

	int sender_uid;
	GameName sender_name;

	unsigned int recv_time;	

	char kind;
	char is_read;
	char is_lock;
	char resvered;
	int vip_level;
};

struct MailBrief
{
	MailBrief() : mail_index(-1)
	{
		memset(subject, 0, sizeof(subject));
	}

	int mail_index;
	int	has_attachment;

	MailStatus mail_status;
	MailSubject subject;
};

struct MailVirtualItemList
{
	MailVirtualItemList() { this->Reset(); }

	void Reset()
	{
		memset(virtual_item_type, -1, sizeof(virtual_item_type));
		memset(virtual_item, 0, sizeof(virtual_item));
	}

	bool Invalid()
	{
		for (int i = 0; i < MAX_MAIL_VIRTUAL_ITEM_NUM; ++i)
		{
			if (virtual_item_type[i] >= 0 && virtual_item_type[i] < MAIL_VIRTUAL_ITEM_MAX && virtual_item[i] > 0)
			{
				return false;
			}
		}
		return true;
	}

	bool Invalid(int index) const
	{
		if (index < 0 || index >= MAX_MAIL_VIRTUAL_ITEM_NUM)
		{
			return true;
		}
		if (virtual_item_type[index] >= 0 && virtual_item_type[index] < MAIL_VIRTUAL_ITEM_MAX && virtual_item[index] > 0)
		{
			return false;
		}
		return true;
	}

	bool CanMerge(const MailVirtualItemList &mail_virtual_item_list)
	{
		if (&mail_virtual_item_list == this)
		{
			return false;
		}
		size_t count = 0;
		for (int i = 0; i < MAX_MAIL_VIRTUAL_ITEM_NUM; ++i)
		{
			if (!this->Invalid(i))
			{
				++count;
			}
			if (!mail_virtual_item_list.Invalid(i))
			{
				++count;
			}
		}
		if (count > MAX_MAIL_VIRTUAL_ITEM_NUM)
		{
			return false;
		}
		return true;
	}

	bool Merge(const MailVirtualItemList &mail_virtual_item_list)
	{
		if (&mail_virtual_item_list == this)
		{
			return false;
		}
		int count = 0;
		int this_invalid_index[MAX_MAIL_VIRTUAL_ITEM_NUM];
		memset(this_invalid_index, -1, sizeof(this_invalid_index));
		int this_index_count = 0;
		int valid_index[MAX_MAIL_VIRTUAL_ITEM_NUM];
		memset(valid_index, -1, sizeof(valid_index));
		int index_count = 0;
		for (int i = 0; i < MAX_MAIL_VIRTUAL_ITEM_NUM; ++i)
		{
			if (this->Invalid(i))
			{
				this_invalid_index[this_index_count] = i;
				++this_index_count;
			}
			else
			{
				++count;
			}
			if (!mail_virtual_item_list.Invalid(i))
			{
				valid_index[index_count] = i;
				++index_count;
				++count;
			}
			if (count > MAX_MAIL_VIRTUAL_ITEM_NUM)
			{
				return false;
			}
		}
		for (int i = 0; i < index_count && i < MAX_MAIL_VIRTUAL_ITEM_NUM; ++i)
		{
			if (this_invalid_index[i] < 0 || this_invalid_index[i] >= MAX_MAIL_VIRTUAL_ITEM_NUM || valid_index[i] < 0 || valid_index[i] >= MAX_MAIL_VIRTUAL_ITEM_NUM)
			{
				break;
			}
			virtual_item_type[this_invalid_index[i]] = mail_virtual_item_list.virtual_item_type[valid_index[i]];
			virtual_item[this_invalid_index[i]] = mail_virtual_item_list.virtual_item[valid_index[i]];
		}
		return true;
	}

	bool Set(const MailVirtualItemList &mail_virtual_item_list)
	{
		if (&mail_virtual_item_list == this)
		{
			return false;
		}
		memcpy(virtual_item_type, mail_virtual_item_list.virtual_item_type, sizeof(virtual_item_type));
		memcpy(virtual_item, mail_virtual_item_list.virtual_item, sizeof(virtual_item));
		return true;
	}

	char virtual_item_type[MAX_MAIL_VIRTUAL_ITEM_NUM];
	long long virtual_item[MAX_MAIL_VIRTUAL_ITEM_NUM];
};

struct MailContentParam
{
	MailContentParam() { this->Reset(); }
	
	void Reset()
	{
		coin = 0; coin_bind = 0; gold = 0; gold_bind = 0; item_count = 0;
		
		memset(subject, 0, sizeof(subject));
		memset(contenttxt, 0, sizeof(contenttxt));
		memset(item_list, 0, sizeof(item_list));
		virtual_item_list.Reset();
	}

	void ResetExceptText()
	{
		coin = 0; coin_bind = 0; gold = 0; gold_bind = 0; item_count = 0;

		memset(item_list, 0, sizeof(item_list));
		virtual_item_list.Reset();
	}

	void AddVirtualItem(int index, char virtual_type, long long value)
	{
		if (index < 0 || index >= MAX_MAIL_VIRTUAL_ITEM_NUM || virtual_type < 0 || virtual_type >= MAIL_VIRTUAL_ITEM_MAX || value <= 0)
		{
			return;
		}

		virtual_item_list.virtual_item_type[index] = virtual_type;
		virtual_item_list.virtual_item[index] = value;
	}

	void AddItem(const ItemConfigData &item)
	{
		if (item.item_id <= 0 || item.num <= 0 || item_count >= MAX_ATTACHMENT_ITEM_NUM)
		{
			return;
		}
		item_list[item_count].item_id = item.item_id;
		item_list[item_count].num = item.num;
		item_list[item_count].is_bind = item.is_bind ? 1 : 0;

		++item_count;
	}

	void AddItem(ItemID itemid, int num, bool is_bind,unsigned int invalid_time = 0)
	{
		if (itemid <= 0 || num <= 0 || item_count >= MAX_ATTACHMENT_ITEM_NUM)
		{
			return;
		}

		item_list[item_count].item_id = itemid;
		item_list[item_count].num = num;
		item_list[item_count].is_bind = is_bind ? 1 : 0;
		item_list[item_count].invalid_time = invalid_time;

		++ item_count;
	}

	void SetContentTxt(char *content)
	{
		SNPRINTF(contenttxt, sizeof(contenttxt), content);
	}

	void SetSubject(char *content)
	{
		SNPRINTF(subject, sizeof(subject), content);
	}

	bool CanMerge(const MailContentParam &mail_content)
	{
		//�����Լ��ϲ��Լ�
		if (&mail_content == this)
		{
			return false;
		}
		//�������
		if (coin + mail_content.coin < coin ||
			coin_bind + mail_content.coin_bind < coin_bind ||
			gold + mail_content.gold < gold ||
			gold_bind + mail_content.gold_bind < gold_bind)
		{
			return false;
		}
		//�ʼ���������ݱ�����ͬ
		if (NULL == strstr(mail_content.subject, subject) || NULL == strstr(mail_content.contenttxt, contenttxt))
		{
			return false;
		}
		//�ϳɺ�ռ�õ���Ʒ���Ӳ��ܳ�����
		if (item_count + mail_content.item_count > MAX_ATTACHMENT_ITEM_NUM)
		{
			return false;
		}
		//������Ʒ�ܷ�ϳ�
		if (!virtual_item_list.CanMerge(mail_content.virtual_item_list))
		{
			return false;
		}
		//��ͨ��Ʒ�ܷ�ϳ�
		ItemDataWrapper temp_item_list[MAX_ATTACHMENT_ITEM_NUM];
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			temp_item_list[i].Reset(item_list[i]);
		}
		int can_merge_count = 0;
		const int need_flag_count = MAX_ATTACHMENT_ITEM_NUM / BIT_COUNT_OF_BYTE;
		const int flag_count = MAX_ATTACHMENT_ITEM_NUM % BIT_COUNT_OF_BYTE == 0 ? need_flag_count : need_flag_count + 1;
		unsigned char merge_flag[flag_count] = { 0 };
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			for (int j = 0; j < MAX_ATTACHMENT_ITEM_NUM; ++j)
			{
				if (BitmapFunction::IsBitSet(merge_flag, flag_count, j))
				{
					continue;
				}
				if (temp_item_list[i].Merge(mail_content.item_list[j]))
				{
					BitmapFunction::SetBit(merge_flag, flag_count, j);
					++can_merge_count;
					i = -1;
					break;
				}
			}
		}
		if (can_merge_count != MAX_ATTACHMENT_ITEM_NUM)
		{
			return false;
		}
		return true;
	}

	bool Merge(const MailContentParam &mail_content)
	{
		if (!this->CanMerge(mail_content))
		{
			return false;
		}
		coin += mail_content.coin;
		coin_bind += mail_content.coin_bind;
		gold += mail_content.gold;
		gold_bind += mail_content.gold_bind;
		item_count += mail_content.item_count;
		virtual_item_list.Merge(mail_content.virtual_item_list);
		const int need_flag_count = MAX_ATTACHMENT_ITEM_NUM / BIT_COUNT_OF_BYTE;
		const int flag_count = MAX_ATTACHMENT_ITEM_NUM % BIT_COUNT_OF_BYTE == 0 ? need_flag_count : need_flag_count + 1;
		unsigned char merge_flag[flag_count] = { 0 };
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			for (int j = 0; j < MAX_ATTACHMENT_ITEM_NUM; ++j)
			{
				if (BitmapFunction::IsBitSet(merge_flag, flag_count, j))
				{
					continue;
				}
				if (item_list[i].Merge(mail_content.item_list[j]))
				{
					BitmapFunction::SetBit(merge_flag, flag_count, j);
					i = -1;
					break;
				}
			}
		}
		return true;
	}

	void Set(const MailContentParam &mail_content)
	{
		if (&mail_content == this)
		{
			return;
		}
		coin = mail_content.coin;
		coin_bind = mail_content.coin_bind;
		gold = mail_content.gold;
		gold_bind = mail_content.gold_bind;
		memcpy(subject, mail_content.subject, sizeof(subject));
		memcpy(contenttxt, mail_content.contenttxt, sizeof(contenttxt));
		item_count = mail_content.item_count;
		for (int i = 0; i < MAX_ATTACHMENT_ITEM_NUM; ++i)
		{
			item_list[i].Reset(mail_content.item_list[i]);
		}
		virtual_item_list.Set(mail_content.virtual_item_list);
	}

	int coin;
	int coin_bind;
	int gold;
	int gold_bind;

	MailSubject subject;
	MailContentTxt contenttxt;

	int item_count;
	ItemDataWrapper item_list[MAX_ATTACHMENT_ITEM_NUM];

	MailVirtualItemList virtual_item_list;
};

struct MailParam
{
	MailParam() : mail_index(-1) {}

	int mail_index;

	MailStatus status;
	MailContentParam contentparam;
};

const char * GetMailReasonSubject(int reason);
const char * GetMailReasonContent(int reason);

typedef char MailVirtualItemListHex[sizeof(MailVirtualItemList) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MailVirtualItemListHex) < 128)

//////////////////////////////////////////////////////////////////////////
struct MailSend
{
	int	recver_dbindex;
	int	recver_roleid;

	int	sender_dbindex;
	int	sender_roleid;
	GameName sender_name;

	char kind;
	char reserved1;
	short reserved2;

	MailContentParam contentparam;
};

struct FetchAttachment
{
	int db_index;
	int	role_id;

	int	mail_index;
	int item_index;
	int is_last;
};

struct FetchAttachmentAffirm
{
	int	db_index;
	int	role_id;
	int	mail_index;
	int	item_index;
	int	is_last;
	int	result;					// 0 �ɹ� ���� ʧ��
};

#pragma pack(pop)

#endif  // __MAIL_DEF_H__

