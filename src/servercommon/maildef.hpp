/******************************************************************************************
FileName: maildef.hpp
Author: 
Description: 邮件系统定义  
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang	2011/07/17/ 09:04:05     1.0           创建
*****************************************************************************************/
#ifndef __MAIL_DEF_HPP__
#define __MAIL_DEF_HPP__

#include <memory>

#include "servercommon/serverdef.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/bitmapfunction.hpp"

#pragma pack(push, 4)

static const int MIN_GET_MAIL_LIST_INTERVAL = 10;				// 限制用户拉取邮件列表的时间
static const int MAIL_SAVE_DB_INTERVAL = 10;					// 回写DB的时间间隔
static const int GET_SYSTEM_MAIL_INTERVAL = 60 * 5;				// 去DB拉系统邮件的时间间隔 
static const int MAX_GET_MAIL_TIMEOUT_INTERVAL = 15;			// 超时 处于加载中状态的最大时间间隔
static const int MAIL_UN_ACTIVE_TIMEOUT_INTERVAL = 60 * 10;		// 最大的不活跃时间 淘汰
static const int MAX_MAIL_CHECK_STATE_INTERVAL = 10;			// 检查用户邮件状态时间间隔

static const int MAX_SUBJECT_LENTH = 128;						// 主题长度
static const int MAX_CONTENT_TXT_LENTH = 512;					// 内容长度 客户端长度是512 服务器预留多一点 用于截断

static const int MAX_MAIL_NUM = 160;							// 玩家最多有可拥有的邮件
static const int MAX_MAIL_SEND_LIST_SIZE = 50;					// 分包发 一次发的个数 

static const int MAX_PERSONAL_MAIL = MAX_MAIL_NUM - 20;			// 私人邮件在邮箱满之前20封就不能发了
static const int MAX_LOCK_MAIL_NUM = 10;						// 最多能锁10封 
static const int MAX_MAIL_RESERVER_TIME = 3600 * 24 * 15;		// 最多保存7天

static const int MAIL_POSTAGE = 500;							// 邮资：500铜币
static const int MAIL_POSTAGE_ATTACHMENT = 500;					// 邮资带附件：500铜币

static const int MAX_GAMEWORLD_SEND_MAIL_USER = 2048;			// 支持同时给2048个人发邮件

static const int MAX_ATTACHMENT_ITEM_NUM = 3;					// 最大附件物品数 如果改变需要同时修改数据库

static const int MULTI_MAX_ATTACHMENT_ITEM_NUM = 6;				// 最大附件物品数 用于多封邮件 

static const int MAX_VIRTUAL_ITEM_NUM = 15;						// 最大虚拟物品数 比如声望 战魂

static const int MAX_MAIL_VIRTUAL_ITEM_NUM = 4;					// 邮件最大虚拟物品数

static const int MAIL_SEND_OPEN_LEVEL = 199;						// 发送邮件开放等级

typedef char MailSubject[128];					// 邮件主题
typedef char MailContentTxt[512];				// 邮件正文

enum MAIL_TYPE 
{
	MAIL_TYPE_INVALID = 0,
	MAIL_TYPE_PERSONAL,											// 个人邮件
	MAIL_TYPE_SYSTEM,											// 系统邮件
	MAIL_TYPE_GUILD,											// 军团邮件
	MAIL_TYPE_CHONGZHI,											// 充值邮件
	MAIL_TYPE_MAX,
};

enum MAIL_STATE
{
	MAIL_STATE_INVALID = 0,										// 无效状态
	MAIL_STATE_LOAD_ING,										// 加载中
	MAIL_STATE_LOAD_OK,											// 加载完成 
};

enum MAIL_VIRTUAL_ITEM
{
	MAIL_VIRTUAL_ITEM_INVALID = -1,

	MAIL_VIRTUAL_ITEM_BATTLEFIELDHONOR = 0,						// 战场荣誉
	MAIL_VIRTUAL_ITEM_EXP = 1,									// 经验
	MAIL_VIRTUAL_ITEM_GUILDGONGXIAN = 2,						// 仙盟贡献
	MAIL_VIRTUAL_ITEM_SHENGWANG = 3,							// 声望
	MAIL_VIRTUAL_ITEM_BIND_GOLD = 4,							// 绑定元宝
	MAIL_VIRTUAL_ITEM_GOLD = 5,									// 元宝
	MAIL_VIRTUAL_ITEM_BIND_COIN = 6,							// 绑定铜币
	MAIL_VIRTUAL_ITEM_CROSS_HONOR = 7,							// 跨服荣誉
	MAIL_VIRTUAL_ITEM_SHENMOZHILI = 8,							// 神魔之力
	MAIL_VIRTUAL_ITEM_GONGXUN = 9,								// 功勋
	MAIL_VIRTUAL_ITEM_CONVERTSHOP_MOJING = 10,					// 魔晶-兑换商店货币
	MAIL_VIRTUAL_ITEM_CONVERTSHOP_SHENGWANG = 11,				// 声望-兑换商店货币
	MAIL_VIRTUAL_ITEM_CONVERTSHOP_GONGXUN = 12,					// 功勋-兑换商店货币
	MAIL_VIRTUAL_ITEM_CONVERTSHOP_WEIWANG = 13,					// 威望-兑换商店货币
	MAIL_VIRTUAL_ITEM_CONVERTSHOP_HUNJING = 14,					// 魂晶-兑换商店货币
	MAIL_VIRTUAL_ITEM_TIANSHENHUTI_SCORE = 15,					// 周末装备抽奖积分

	MAIL_VIRTUAL_ITEM_MAX,
};

//UNSTD_STATIC_CHECK(MAIL_VIRTUAL_ITEM_MAX < MAX_VIRTUAL_ITEM_NUM);

enum SYSTEM_MAIL_REASON
{
	SYSTEM_MAIL_REASON_INVALID = 0,

	SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_FULL,				// 出售的物品已满 退还
	SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_CANCEL,				// 取消拍卖 
	SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_EXPIRE,				// 拍卖过期 
	SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_ITEM_BUY_SUCC,			// 购买成功
	SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_MONEY_BUY_FAIL,			// 购买失败 退钱
	SYSTEM_MAIL_REASON_PUBLICSALE_GIVE_MONEY_SALE_SUCC,			// 出售成功

	SYSTEM_MAIL_REASON_GUILD_CREATE,							// 军团创建通知
	SYSTEM_MAIL_REASON_GUILD_DIMISS,							// 军团解散通知
	SYSTEM_MAIL_REASON_GUILD_BEKICKED,							// 军团踢人通知
	SYSTEM_MAIL_REASON_GUILD_DELATE,							// 军团弹劾

	SYSTEM_MAIL_REASON_MAIL_RECEIVER_FULL,						// 邮件接收方邮箱满

	SYSTEM_MAIL_REASON_YIZHANDAODI_LUCKY_REWARD,				// 一站到底幸运奖励
	SYSTEM_MAIL_REASON_WUXINGGUAJI_LUCKY_REWARD,				// 五行挂机幸运奖励

	SYSTEM_MAIL_REASON_GCZ_RANK_REWARD,							// 攻城战排名奖励
	SYSTEM_MAIL_REASON_GCZ_CHENGZHU_REWARD,						// 攻城战城主奖励
	SYSTEM_MAIL_REASON_DAILY_TASK_FINISH_REWARD,				// 日常任务完成奖励
	SYSTEM_MAIL_REASON_GCZ_LUCKY_REWARD,						// 攻城战幸运奖励
	SYSTEM_MAIL_REASON_LTZ_LUCKY_REWARD,						// 领土战幸运奖励
	SYSTEM_MAIL_REASON_GHZB_LUCKY_REWARD,						// 公会争霸幸运奖励
	SYSTEM_MAIL_REASON_YSZC_LUCKY_REWARD,						// 元素战场幸运奖励

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
	SYSMAIL_EVENT_CONDITION_LEVEL_EQUAL_GREATER,				// 等级大等于
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
		//不能自己合并自己
		if (&mail_content == this)
		{
			return false;
		}
		//不能溢出
		if (coin + mail_content.coin < coin ||
			coin_bind + mail_content.coin_bind < coin_bind ||
			gold + mail_content.gold < gold ||
			gold_bind + mail_content.gold_bind < gold_bind)
		{
			return false;
		}
		//邮件标题和内容必须相同
		if (NULL == strstr(mail_content.subject, subject) || NULL == strstr(mail_content.contenttxt, contenttxt))
		{
			return false;
		}
		//合成后占用的物品格子不能超上限
		if (item_count + mail_content.item_count > MAX_ATTACHMENT_ITEM_NUM)
		{
			return false;
		}
		//虚拟物品能否合成
		if (!virtual_item_list.CanMerge(mail_content.virtual_item_list))
		{
			return false;
		}
		//普通物品能否合成
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
	int	result;					// 0 成功 其他 失败
};

#pragma pack(pop)

#endif  // __MAIL_DEF_H__

