#ifndef __DATING_DEF_HPP__
#define __DATING_DEF_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/servercommon.h"
#include "servercommon/campdef.hpp"

enum DATING_SIDE
{
	DATING_SIDE_SPONSOR = 0,										// 邀请方
	DATING_SIDE_TARGET,												// 被邀请方
	DATING_SIDE_MAX
};

enum DATING_STATUS													// 约会状态
{
	DATING_STATUS_INVALID = 0,
	DATING_STATUS_GIVE_PRESENT,										// 赠送礼物
	DATING_STATUS_DAYCOUNT_CHECK_WAIT,								// 次数消耗等待
	DATING_STATUS_QUESTION,											// 约会中
	DATING_STATUS_EVALUATE,											// 相互评价

	DATING_STATUS_MAX
};

enum DATING_PRESENT_TYPE											// 约会赠送礼物类型
{
	DATING_PRESENT_TYPE_INVALID = 0,
	DATING_PRESENT_TYPE_MORE_EXP,									// 更多经验
	DATING_PRESENT_TYPE_DOUBLE_EXP,									// 双倍经验
	DATING_PRESENT_TYPE_NO_ITEM,									// 不赠送礼物
	DATING_PRESENT_TYPE_MAX
};

static const int DATING_OPEN_LEVEL = 58;							// 约会开启等级
static const int DATING_RANDOM_TARGET_COUNT = 2;					// 约会随机异性对象数量
static const int DATING_XUNYOU_POINT_MAX_COUNT = 10;				// 约会巡游目标点数
static const int DATING_REFRESH_TARGET_LIST_INTERVAL_S = 180;		// 约会刷新对象列表限制时间间隔
static const int DATING_QUESTION_NUM = 25;							// 约会答题题目数量
static const int DATING_QUESTION_ANSWER_NUM = 10;					// 约会需要的答题数量

static const int DATING_DAYCOUNT_CHECK_WAIT_TIMEOUT_S = 30;			// 约会次数检查限制时间
static const int DATING_GIVE_PRESENT_TIMEOUT_S = 600;				// 约会赠送礼物限制时间
static const int DATING_QUESION_TIMEOUT_S = 180;					// 约会答题总限制时间
static const int DATING_EVALUATE_TIMEOUT_S = 60;					// 评价限制时间
static const int DATING_UPDATE_ADD_EXP_ITEM_COUNT = 30;				// 约会增加经验配置数

struct DatingTargetUser						// 约会目标
{
	DatingTargetUser() : camp(CAMP_TYPE_INVALID), prof(0), avatar(0), sex(0), guild_id(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UserID user_id;							// 目标ID
	GameName user_name;						// 名称
	char camp;								// 国家
	char prof;								// 职业
	char avatar;							// 头像
	char sex;								// 性别
	int guild_id;							// 军团ID
};

struct UserDatingTargetInfo					// 角色的随机约会目标列表信息
{
	UserDatingTargetInfo() : refresh_time(0), target_count(0) { }

	bool IsInTargetList(const UserID &target_user_id)
	{
		for (int i = 0; i < target_count && i < DATING_RANDOM_TARGET_COUNT; ++ i)
		{
			if (target_user_id == target_list[i].user_id)
			{
				return true;
			}
		}

		return false;
	}

	unsigned int refresh_time;				// 刷新时间
	int target_count;						// 目标数量
	DatingTargetUser target_list[DATING_RANDOM_TARGET_COUNT];		// 目标信息
};

struct Dating
{
	Dating() : dating_status(DATING_STATUS_INVALID), status_timeout(0), present_type(DATING_PRESENT_TYPE_INVALID), cur_question_index(0), answer_question_num(0), question_score(0), quesiont_next_statu_time(0) 
	{
		memset(cur_answer_list, 0, sizeof(cur_answer_list));
	}

	void *			operator new(size_t c);
	void			operator delete(void *m);

	struct SideInfo							// 一方信息
	{
		SideInfo() : is_evaluate(false), is_daycount_check(false) {}

		UserID user_id;						// 角色ID
		bool is_evaluate;					// 是否评价
		bool is_daycount_check;				// 是否检查能扣次数
	};

	int dating_status;						// 状态
	unsigned int status_timeout;			// 超时时间
	int present_type;						// 赠送礼物类型
	SideInfo side_list[DATING_SIDE_MAX];	// 双方信息

	int cur_question_index;
	int answer_question_num;
	int question_score;
	int cur_answer_list[DATING_SIDE_MAX];
	time_t quesiont_next_statu_time;
};


#endif	// __DATING_DEF_HPP__

