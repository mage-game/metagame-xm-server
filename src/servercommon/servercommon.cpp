#include "common/md5/md5cpp.h"
#include "common/bintohex.h"

#include "servercommon/servercommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/tradedef.hpp"
#include "servercommon/maildef.hpp"
#include "servercommon/struct/itemlistparam.h"
#include <sstream>

#include <time.h>

OLD_USE_INTERNAL_NETWORK(); 

void GetSessionKey(const std::string &key_base, int scene_id, int scene_key, int last_scene_id, int role_id, GameName role_name, unsigned int time, char first_login, PlatName pname, SessionKey key)
{
	char str_tmp[256];
	int str_len = SNPRINTF(str_tmp, sizeof(str_tmp), "%s;%d;%s;%u;%d;%s", key_base.c_str(), (scene_id + scene_key + last_scene_id + role_id),
		(char *)role_name, time, (int)first_login, (char*)pname);

	MD5 md5;
	MD5_buffer(&md5, str_tmp, (unsigned long)str_len);
	BinToHex((const char*)md5, sizeof(MD5), key);
}

unsigned int GetChatLoginKey(int uid)
{
	return (unsigned int)uid;
}

long long ConvertParamToLongLong(int param_0, int param_1)
{
	union 
	{
		long long ll_value;
		struct  
		{
			int param_0;
			int param_1;
		};
	} union_param;

	union_param.param_0 = param_0;
	union_param.param_1 = param_1;

	return union_param.ll_value;
}

long long ConvertParamToLongLong(int param_0, short param_1, short param_2)
{
	union 
	{
		long long ll_value; 
		struct  
		{
			int param_0;
			short param_1;
			short param_2;
		};
	} union_param;

	union_param.param_0 = param_0;
	union_param.param_1 = param_1;
	union_param.param_2 = param_2;

	return union_param.ll_value;
}

bool HasInvalidChar(char *str)
{
	while (*str)
	{
		if (*str == ';' || *str == '|' || *str == '\n' || *str == '\t' || *str == ' ' || *str == '[' || *str == ']')  
		{
			return true;
		}

		str++;
	}

	return false;
}

void InvalidCharFilte(char *str)
{
	while (*str)
	{
		if (*str == ';' || *str == '|' || *str == '\n' || *str == '\t' || *str == ' ' || *str == '[' || *str == ']') *str = '*';
		str++;
	}
}

void RoleNameCatPostfix(GameName rolename, short server)
{
	//GameName tmp;
	//F_STRNCPY(tmp, rolename, sizeof(GameName));
	//SNPRINTF(rolename, sizeof(GameName), "%s[%d]", tmp, (int)server);
}

void PlatNameCatPostfix(PlatName pname, short server)
{
	PlatName tmp;
	F_STRNCPY(tmp, pname, sizeof(PlatName));
	SNPRINTF(pname, sizeof(PlatName), "%s_%d", tmp, (int)server);
}

void GuildNameCatPostfix(GuildName guildname, short server)
{
	//GuildName tmp;
	//F_STRNCPY(tmp, guildname, sizeof(GuildName));
	//SNPRINTF(guildname, sizeof(GuildName), "%s[%d]", tmp, (int)server); 
}

char * GetCurVersion(unsigned int unVer)
{
	char szVer[32];
	SNPRINTF(szVer, sizeof(szVer), "%u", unVer);	

	static char szVersion[128];
	const char *szVer2 = ('0' == szVer[2]) ? "trunk" : ('1' == szVer[2] ? "branch" : "");
	SNPRINTF(szVersion, sizeof(szVersion), " %c.%c %s %c%c build (%d)", szVer[0], szVer[1], szVer2, szVer[3], szVer[4], unVer);

	return &szVersion[0];
}

void PlatNameReCatPostfix(PlatName pname)
{
	int fix_pos = -1;

	for (int i = 0; i < static_cast<int>(sizeof(PlatName)); i++)
	{
		if (0 == pname[i])
		{
			break;
		}
		else if ('_' == pname[i])
		{
			fix_pos = i;
		}
	}

	if (fix_pos >= 0 && fix_pos < static_cast<int>(sizeof(PlatName)))
	{
		pname[fix_pos] = 0;
	}

	//for (int i = sizeof(PlatName) - 1; i >= 0; i--)   旧代码考虑不周，因为并没有memset整个结构，无法保证\0后续全部为0
	//{
	//	if ('_' == pname[i])
	//	{
	//		pname[i] = '\0';
	//		break;
	//	}
	//}
}

const char * ItemParamDataStructLog(const ItemParamDataStruct *item_param)
{
	static char item_param_log[1024] = {0};

	if (NULL == item_param)
	{
		SNPRINTF(item_param_log, sizeof(item_param_log), "NULL");
	}
	else
	{
		SNPRINTF(item_param_log, sizeof(item_param_log), 
			"strengthen_level:%d,quality:%d,shen_level:%d,fuling_level:%d",
			(int)item_param->strengthen_level,
			(int)item_param->quality,
			(int)item_param->shen_level,
			(int)item_param->fuling_level);
	}

	return item_param_log;
}

const char * ItemDataQuickLog(const ItemDataWrapper *item_wrapper)
{
	static char item_data_log[1024] = {0};
	item_data_log[0] = '\0';

	if (NULL != item_wrapper)
	{
		const ItemParamDataStruct *item_param = &item_wrapper->param_data;

		SNPRINTF(item_data_log, sizeof(item_data_log), 
			"%d|%d|%d|%d|%d|"
			"%d|%d|%d|%d|",
			(int)item_wrapper->item_id,
			(int)item_wrapper->num,
			(int)item_wrapper->is_bind,
			(int)item_wrapper->has_param,
			(int)item_wrapper->invalid_time,

			(int)item_param->strengthen_level,
			(int)item_param->quality,
			(int)item_param->shen_level,
			(int)item_param->fuling_level);
	}

	return item_data_log;
}

void ItemConfigDataListLog(std::string &log_str, int item_count, const ItemConfigData *item_cfglist)
{
	std::ostringstream oss;
	static char tmp_log_str[LOG_BASE_LENGTH];

	memset(tmp_log_str, 0, sizeof(tmp_log_str));
	SNPRINTF(tmp_log_str, sizeof(tmp_log_str), "count[%d] : ", item_count);
	oss << tmp_log_str;

	if (item_count > 0 && NULL != item_cfglist)
	{
		for (int i = 0; i < item_count; ++ i)
		{
			memset(tmp_log_str, 0, sizeof(tmp_log_str));
			SNPRINTF(tmp_log_str, sizeof(tmp_log_str), "item_%d[%s] ", i, ItemConfigDataLog(&item_cfglist[i], NULL));
			oss << tmp_log_str;
		}
	}

	log_str = oss.str();
}

const char * ItemConfigDataLog(const ItemConfigData *item_cfg, char log_str[LOG_BASE_LENGTH])
{
	static char defalut_log_str[LOG_BASE_LENGTH]; memset(defalut_log_str, 0, sizeof(defalut_log_str));

	if (NULL == item_cfg)
	{
		return defalut_log_str;
	}

	char *out_log = log_str;
	if (NULL == out_log)
	{
		out_log = defalut_log_str;
	}

	SNPRINTF(out_log, LOG_BASE_LENGTH, "item_id:%d, num:%d, is_bind:%d", (int)item_cfg->item_id, item_cfg->num, (item_cfg->is_bind ? 1 : 0));

	return out_log;
}

const char * ItemDataWrapperLog(const ItemDataWrapper *item_wrapper, int index)
{
	if (index < 0 || index >= LOG_MAX_ITEM_NUM) return "NULL";
	
	static char item_wrapper_log[LOG_BASE_LENGTH * LOG_MAX_ITEM_NUM] = {0};
	char *p = item_wrapper_log + index * LOG_BASE_LENGTH;

	if (NULL == item_wrapper)
	{
		SNPRINTF(p, LOG_BASE_LENGTH, "NULL");
	}
	else
	{
		SNPRINTF(p, LOG_BASE_LENGTH, 
			"item_id:%d, num:%d, is_bind:%d, has_param:%d, invalid_time:%d, item_param[%s]",
			(int)item_wrapper->item_id,
			(int)item_wrapper->num,
			(int)item_wrapper->is_bind,
			(int)item_wrapper->has_param,
			(int)item_wrapper->invalid_time,
			ItemParamDataStructLog(&item_wrapper->param_data));
	}
	
	return p;
}

const char * SaleItemParamLog(const SaleItemParam *param)
{
	static char sale_param_log[1024] = {0};

	if (NULL == param)
	{
		SNPRINTF(sale_param_log, sizeof(sale_param_log), "NULL");
	}
	else
	{
		SNPRINTF(sale_param_log, sizeof(sale_param_log), 
			"sale_index:%d, sale_time:%d, due_time:%d, price:%d, " 
			"sale_value:%d, sale_type:%d, sale_item_type:%d," 
			"color:%d, level:%d, prof:%d, order:%d, item_data:%s",
			(int)param->sale_index,
			(int)param->sale_time,
			(int)param->due_time,
			(int)param->price,
			(int)param->sale_value,
			(int)param->sale_type,
			(int)param->sale_item_type,
			(int)param->color,
			(int)param->level,
			(int)param->prof,
			(int)param->order,
			ItemDataWrapperLog(&param->item_wrapper));
	}

	return sale_param_log;
}

const char * MailParamLog(const MailParam *param)
{
	static char mail_param_log[4096] = {0};

	if (NULL == param)
	{
		SNPRINTF(mail_param_log, sizeof(mail_param_log), "NULL");
	}
	else
	{
		UNSTD_STATIC_CHECK(3 == MAX_ATTACHMENT_ITEM_NUM);

		SNPRINTF(mail_param_log, sizeof(mail_param_log), 
			"sender[uid:%d, name:%s], mail_index:%d, recv_time:%d, kind:%d, is_read:%d, is_lock:%d, subject:%s"
			"attachment[coin:%d, coin_bind:%d, gold:%d, gold_bind:%d, item1[%s], item2[%s], item3[%s], virtual_items[%s]]",
			(int)param->status.sender_uid,
			(const char*)param->status.sender_name,
			(int)param->mail_index,
			(int)param->status.recv_time,
			(int)param->status.kind,
			(int)param->status.is_read,
			(int)param->status.is_lock,
			(const char*)param->contentparam.subject,
			(int)param->contentparam.coin, 
			(int)param->contentparam.coin_bind, 
			(int)param->contentparam.gold, 
			(int)param->contentparam.gold_bind, 
			ItemDataWrapperLog(&param->contentparam.item_list[0], 0), 
			ItemDataWrapperLog(&param->contentparam.item_list[1], 1), 
			ItemDataWrapperLog(&param->contentparam.item_list[2], 2),
			MailVirtualItemLog(&param->contentparam.virtual_item_list));
	}

	return mail_param_log;
}

const char * MailVirtualItemLog(const MailVirtualItemList *param)
{
	static char mail_virtual_item_log[1024] = {0};

	if (NULL == param)
	{
		SNPRINTF(mail_virtual_item_log, sizeof(mail_virtual_item_log), "NULL");
	}
	else
	{
		UNSTD_STATIC_CHECK(15 == MAX_VIRTUAL_ITEM_NUM);

		SNPRINTF(mail_virtual_item_log, sizeof(mail_virtual_item_log),
			"battlefield_honor:%d, vir_item_0[type:%d, value:%lld] vir_item_1[type:%d, value:%lld] vir_item_2[type:%d, value:%lld] vir_item_3[type:%d, value:%lld]",
			(int)param->virtual_item_type[0],
			param->virtual_item[0],
			(int)param->virtual_item_type[1],
			param->virtual_item[1],
			(int)param->virtual_item_type[2],
			param->virtual_item[2],
			(int)param->virtual_item_type[3],
			param->virtual_item[3]);
	}

	return mail_virtual_item_log;
}

int GetDayIndex(time_t begin_time, time_t t)
{
	const bool is_past = (t < begin_time);
	if (is_past)
	{
		time_t tmp_time = begin_time; begin_time = t; t = tmp_time;
	}

	struct tm local_time_tm;
	struct tm *local_begin_time = &local_time_tm;

#ifdef _WIN32
	if (0 != ::localtime_s(local_begin_time, &begin_time)) return 0;
#else
	if (NULL == ::localtime_r(&begin_time, local_begin_time)) return 0;
#endif

	const time_t zero_time = begin_time - (local_begin_time->tm_hour * SECOND_PER_HOUR + local_begin_time->tm_min * SECOND_PER_MIN + local_begin_time->tm_sec);
	const int day_index = static_cast<int>((t - zero_time) / SECOND_PER_DAY);

	return (is_past ? -day_index : day_index);
}

int GetWeekIndex(time_t begin_time, time_t t)
{
	bool is_past = (t < begin_time);
	if (is_past)
	{
		time_t tmp_time = begin_time; begin_time = t; t = tmp_time;
	}

	struct tm *local_begin_time = ::localtime(&begin_time);
	if (NULL == local_begin_time)
	{
		return 0;
	}

	int begin_time_passday = (local_begin_time->tm_wday + 6) % 7;  // 从周一开始算第一天 举例周日tm_wday=0 但实际上是6天

	time_t week_zerotime = begin_time - (begin_time_passday * 24 * 3600 + local_begin_time->tm_hour * 3600 + local_begin_time->tm_min * 60 + local_begin_time->tm_sec);
	int week_index = static_cast<int>((t - week_zerotime) / (7 * 24 * 3600));

	return (is_past ? -week_index : week_index);
}

int GetMonthIndex(time_t begin_time, time_t t)
{
	bool is_past = (t < begin_time);
	if (is_past)
	{
		time_t tmp_time = begin_time; begin_time = t; t = tmp_time;
	}

	struct tm * tmp_time = ::localtime(&begin_time);
	if (NULL == tmp_time)
	{
		return 0;
	}
	struct tm local_begin_time = *tmp_time;

	tmp_time = ::localtime(&t);
	if (NULL == tmp_time)
	{
		return 0;
	}
	struct tm local_end_time = *tmp_time;

	int month_index = (local_end_time.tm_year - local_begin_time.tm_year) * 12 + (local_end_time.tm_mon - local_begin_time.tm_mon);

	return month_index;
}

time_t GetZeroTime(time_t time_stamp)
{
#ifdef _WIN32
	struct tm *local_time = ::localtime(&time_stamp);
	if (NULL == local_time)
	{
		return 0;
	}
#else	
	struct tm local_time_item;
	struct tm *local_time = &local_time_item;

	if (NULL == ::localtime_r(&time_stamp, &local_time_item))
	{
		return 0;
	}
#endif

	time_t zero_time = time_stamp - (local_time->tm_hour * 3600 + local_time->tm_min * 60 + local_time->tm_sec);
	return zero_time;
}

unsigned int GetTodaySecond(unsigned int now_sec)
{
	unsigned int today_zero_time = static_cast<unsigned int>(GetZeroTime(static_cast<time_t>(now_sec)));
	return now_sec - today_zero_time;
}

bool ConvertTimeStringToUnixTime(const char *time_str, time_t *timestamp)
{
	if (NULL == time_str || NULL == timestamp) return false;

	char temp[256] = {0};
	STRNCPY(temp, time_str, sizeof(temp));

	if (strlen(temp) < 19) return false;

	struct tm local_tm;

	memset(&local_tm, 0, sizeof(local_tm));

	char *pt = &temp[0];

	pt[4] = '\0';
	local_tm.tm_year = atoi(pt) - 1900; if (local_tm.tm_year <= 70 || local_tm.tm_year > 200) return false;
	pt += 5;

	pt[2] = '\0';
	local_tm.tm_mon = atoi(pt) - 1; if (local_tm.tm_mon < 0 || local_tm.tm_mon > 11) return false;
	pt += 3;

	pt[2] = '\0';
	local_tm.tm_mday = atoi(pt); if (local_tm.tm_mday < 1 || local_tm.tm_mday > 31) return false;
	pt += 3;

	pt[2] = '\0';
	local_tm.tm_hour = atoi(pt); if (local_tm.tm_hour < 0 || local_tm.tm_hour > 23) return false;
	pt += 3;

	pt[2] = '\0';
	local_tm.tm_min = atoi(pt); if (local_tm.tm_min < 0 || local_tm.tm_min > 59) return false;
	pt += 3;

	pt[2] = '\0';
	local_tm.tm_sec = atoi(pt); if (local_tm.tm_sec < 0 || local_tm.tm_sec > 59) return false;
	pt += 3;

	*timestamp = mktime(&local_tm);

	return true;
}

int RandomNum(int max_num)
{
	return RandomNum(0, max_num);
}

int RandomNum(int min_num, int max_num)
{
	if (min_num > max_num)
	{
		int tmp_num = min_num;
		min_num = max_num;
		max_num = tmp_num;
	}

	int interval_num = max_num - min_num;
	if (interval_num <= 0)
	{
		return min_num;
	}

	if (interval_num < RAND_MAX)
	{
		return min_num + (rand() % interval_num);
	}
	else
	{
		int rand_num = (rand() << 16) + rand(); // 两次随机数补成一个int（主要解决windows下rand最大值只有2^15的问题）
		if (rand_num < 0)
		{
			rand_num *= -1;
		}
		return min_num + int(rand_num % interval_num);
	}
}

namespace gamecommon
{
	const Posi GetDisRandPos(const Posi &curr_pos, int rand_dis)
	{
		if (rand_dis <= 0) return curr_pos;

		int x = curr_pos.x + RandomNum(-rand_dis, rand_dis);
		int y = curr_pos.y + RandomNum(-rand_dis, rand_dis);

		return Posi(x, y);
	}
}

int CalcDayIndexWithClockDayItem(ClockReturnDayItem *day1, ClockReturnDayItem *day2)
{
	if (NULL == day1 || NULL == day2)
	{
		return 0;
	}

	struct tm day1_tm, day2_tm;
	memset(&day1_tm, 0, sizeof(day1_tm));
	memset(&day2_tm, 0, sizeof(day2_tm));

	day1_tm.tm_year = day1->year;
	day1_tm.tm_mon = day1->mon;
	day1_tm.tm_mday = day1->day;
	day1_tm.tm_hour = 0;
	day1_tm.tm_min = 0;
	day1_tm.tm_sec = 1;

	day2_tm.tm_year = day2->year;
	day2_tm.tm_mon = day2->mon;
	day2_tm.tm_mday = day2->day;
	day2_tm.tm_hour = 0;
	day2_tm.tm_min = 0;
	day2_tm.tm_sec = 1;

	time_t day1_time = mktime(&day1_tm);
	time_t day2_time = mktime(&day2_tm);

	return GetDayIndex(day1_time, day2_time);
}

UInt32 HHMMToNow(UInt32 zero_time, short hhmm)
{
	if (hhmm < 0 || hhmm >= 2359) return 0;
	return zero_time + (hhmm / 100) * SECOND_PER_HOUR + (hhmm % 100) * SECOND_PER_MIN;
}

short NowToHHMM(UInt32 now_sec)
{
	auto zero_time = (UInt32)GetZeroTime(now_sec);
	now_sec -= zero_time;
	auto hour = now_sec / SECOND_PER_HOUR;
	auto minutes = (now_sec - hour * SECOND_PER_HOUR) / SECOND_PER_MIN;
	return (short)(hour * 100 + minutes);
}

/**
* 输入格式：2015-08-25
* 输出时间戳
*/
bool ConvertTimeStringToUnixTime2(const char *time_str, time_t *timestamp)
{
	if (NULL == time_str || NULL == timestamp) return false;

	char temp[12] = { 0 };
	STRNCPY(temp, time_str, sizeof(temp));

	if (strlen(temp) < 10) return false;

	struct tm local_tm;

	char *pt = &temp[0];

	pt[4] = '\0';
	local_tm.tm_year = atoi(pt) - 1900; if (local_tm.tm_year <= 70 || local_tm.tm_year > 200) return false;
	pt += 5;

	pt[2] = '\0';
	local_tm.tm_mon = atoi(pt) - 1; if (local_tm.tm_mon < 0 || local_tm.tm_mon > 11) return false;
	pt += 3;

	pt[2] = '\0';
	local_tm.tm_mday = atoi(pt); if (local_tm.tm_mday < 1 || local_tm.tm_mday > 31) return false;
	pt += 3;

	local_tm.tm_hour = 0;
	local_tm.tm_min = 0;
	local_tm.tm_sec = 0;

	*timestamp = mktime(&local_tm);

	return true;
}
