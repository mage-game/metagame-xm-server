#ifndef __DATING_DEF_HPP__
#define __DATING_DEF_HPP__

#include "servercommon/serverdef.h"
#include "servercommon/servercommon.h"
#include "servercommon/campdef.hpp"

enum DATING_SIDE
{
	DATING_SIDE_SPONSOR = 0,										// ���뷽
	DATING_SIDE_TARGET,												// �����뷽
	DATING_SIDE_MAX
};

enum DATING_STATUS													// Լ��״̬
{
	DATING_STATUS_INVALID = 0,
	DATING_STATUS_GIVE_PRESENT,										// ��������
	DATING_STATUS_DAYCOUNT_CHECK_WAIT,								// �������ĵȴ�
	DATING_STATUS_QUESTION,											// Լ����
	DATING_STATUS_EVALUATE,											// �໥����

	DATING_STATUS_MAX
};

enum DATING_PRESENT_TYPE											// Լ��������������
{
	DATING_PRESENT_TYPE_INVALID = 0,
	DATING_PRESENT_TYPE_MORE_EXP,									// ���ྭ��
	DATING_PRESENT_TYPE_DOUBLE_EXP,									// ˫������
	DATING_PRESENT_TYPE_NO_ITEM,									// ����������
	DATING_PRESENT_TYPE_MAX
};

static const int DATING_OPEN_LEVEL = 58;							// Լ�Ὺ���ȼ�
static const int DATING_RANDOM_TARGET_COUNT = 2;					// Լ��������Զ�������
static const int DATING_XUNYOU_POINT_MAX_COUNT = 10;				// Լ��Ѳ��Ŀ�����
static const int DATING_REFRESH_TARGET_LIST_INTERVAL_S = 180;		// Լ��ˢ�¶����б�����ʱ����
static const int DATING_QUESTION_NUM = 25;							// Լ�������Ŀ����
static const int DATING_QUESTION_ANSWER_NUM = 10;					// Լ����Ҫ�Ĵ�������

static const int DATING_DAYCOUNT_CHECK_WAIT_TIMEOUT_S = 30;			// Լ������������ʱ��
static const int DATING_GIVE_PRESENT_TIMEOUT_S = 600;				// Լ��������������ʱ��
static const int DATING_QUESION_TIMEOUT_S = 180;					// Լ�����������ʱ��
static const int DATING_EVALUATE_TIMEOUT_S = 60;					// ��������ʱ��
static const int DATING_UPDATE_ADD_EXP_ITEM_COUNT = 30;				// Լ�����Ӿ���������

struct DatingTargetUser						// Լ��Ŀ��
{
	DatingTargetUser() : camp(CAMP_TYPE_INVALID), prof(0), avatar(0), sex(0), guild_id(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UserID user_id;							// Ŀ��ID
	GameName user_name;						// ����
	char camp;								// ����
	char prof;								// ְҵ
	char avatar;							// ͷ��
	char sex;								// �Ա�
	int guild_id;							// ����ID
};

struct UserDatingTargetInfo					// ��ɫ�����Լ��Ŀ���б���Ϣ
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

	unsigned int refresh_time;				// ˢ��ʱ��
	int target_count;						// Ŀ������
	DatingTargetUser target_list[DATING_RANDOM_TARGET_COUNT];		// Ŀ����Ϣ
};

struct Dating
{
	Dating() : dating_status(DATING_STATUS_INVALID), status_timeout(0), present_type(DATING_PRESENT_TYPE_INVALID), cur_question_index(0), answer_question_num(0), question_score(0), quesiont_next_statu_time(0) 
	{
		memset(cur_answer_list, 0, sizeof(cur_answer_list));
	}

	void *			operator new(size_t c);
	void			operator delete(void *m);

	struct SideInfo							// һ����Ϣ
	{
		SideInfo() : is_evaluate(false), is_daycount_check(false) {}

		UserID user_id;						// ��ɫID
		bool is_evaluate;					// �Ƿ�����
		bool is_daycount_check;				// �Ƿ����ܿ۴���
	};

	int dating_status;						// ״̬
	unsigned int status_timeout;			// ��ʱʱ��
	int present_type;						// ������������
	SideInfo side_list[DATING_SIDE_MAX];	// ˫����Ϣ

	int cur_question_index;
	int answer_question_num;
	int question_score;
	int cur_answer_list[DATING_SIDE_MAX];
	time_t quesiont_next_statu_time;
};


#endif	// __DATING_DEF_HPP__

