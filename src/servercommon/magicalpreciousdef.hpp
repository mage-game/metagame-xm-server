#ifndef __MAGICAL_PRECIOUS_DEF_H__
#define __MAGICAL_PRECIOUS_DEF_H__

#include "servercommon.h"

#pragma pack(push, 4)

static const int MAGICAL_PRECIOUS_CHAPTER_COUNT = 16;																// ���½���
static const int MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT = 16;													// ÿ�µĽ���������
static const int MAGICAL_PRECIOUS_CHAPTER_FLAG_COUNT = 5;															// ���������������
static const int MAGICAL_PRECIOUS_REWARD_COUNT_PER_CHAPTER = MAGICAL_PRECIOUS_CHAPTER_FLAG_COUNT * 32;				// ÿ�½�����������

enum MAGICAL_PRECIOUS_REWARD_TYPE
{
	MAGICAL_PRECIOUS_REWARD_TYPE_INVALID = -1,

	MAGICAL_PRECIOUS_REWARD_TYPE_UNDONE = 0,					// δ���
	MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH = 1,					// ����ȡ
	MAGICAL_PRECIOUS_REWARD_TYPE_FINISH = 2,					// ����ȡ

	MAGICAL_PRECIOUS_REWARD_TYPE_MAX
};

struct MagicalPreciousParam
{
	MagicalPreciousParam() { this->Reset(); }

	void Reset()
	{
		current_chapter = -1;
		reserve_ch = 0;
		reserve_sh = 0;
		chapter_invalid_time = 0;
		chapter_fetch_finish_reward_flag = 0;
		memset(chapter_score_list, 0, sizeof(chapter_score_list));
		memset(chapter_fetch_reward_flag, 0, sizeof(chapter_fetch_reward_flag));

		finish_guild_task_times = 0;
		finish_daily_task_times = 0;
		wabao_times = 0;
		open_guild_box_times = 0;
		hotspring_join_times = 0;
		hotspring_answer_right_times = 0;
		millionaire_box_times = 0;
		husong_times = 0;
		enter_exp_fb_times = 0;
		pass_phase_fb_times = 0;
		wangling_task_times = 0;
		tianjiangcaibao_task_times = 0;
		shuijing_task_times = 0;
		shuijing_gather_times = 0;
		kill_miku_boss = 0;
		kill_world_boss = 0;
		kill_active_boss = 0;
		kill_dabao_boss = 0;
		kill_other_role_in_fighting = 0;
		mine_times = 0;
		fish_num = 0;
		sailing_num = 0;
		kill_monster_num = 0;
		active_degree = 0;
		kill_vip_boss = 0;
		join_gongcheng_zhan_times = 0;
		reserve_sh1 = 0;

		kill_boss_count_1 = 0;
		kill_boss_count_2 = 0;
		kill_boss_count_3 = 0;
	}

	char current_chapter;									// ��ǰ�½�
	char reserve_ch;
	short reserve_sh;
	unsigned int chapter_invalid_time;						// �½�ʧЧʱ��
	unsigned int chapter_fetch_finish_reward_flag;			// �½���ɽ�����־
	unsigned short chapter_score_list[MAGICAL_PRECIOUS_CHAPTER_COUNT]; // ÿ���½ڵĻ����б�
	char chapter_fetch_reward_flag[MAGICAL_PRECIOUS_CHAPTER_COUNT][MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT];	// �½ڻ���״̬��־��0��δ��ɣ�1��������ҿ���ȡ���֣�2������ȡ����

	unsigned short finish_guild_task_times;					// ��ɹ����������
	unsigned short finish_daily_task_times;					// ����ճ��������
	unsigned short wabao_times;								// �ڱ�����
	unsigned short open_guild_box_times;					// �������ᱦ�����
	unsigned short hotspring_join_times;					// ��������Ȫ����
	unsigned short hotspring_answer_right_times;			// �����Ȫ�ش���ȷ����
	unsigned short millionaire_box_times;					// �󸻺��ɼ��������
	unsigned short husong_times;							// ���ʹ���
	unsigned short enter_exp_fb_times;						// ���뾭�鸱������
	unsigned short pass_phase_fb_times;						// ͨ�ؽ׶θ�������
	unsigned short wangling_task_times;						// �������̽�������ܴ���
	unsigned short tianjiangcaibao_task_times;				// ����콵�Ʊ������ܴ���
	unsigned short shuijing_task_times;						// ���ˮ���þ������ܴ���
	unsigned short shuijing_gather_times;					// ˮ���þ��ɼ��ܴ���
	unsigned short kill_miku_boss;							// ��ɱ��Ӣboss�ܴ���
	unsigned short kill_world_boss;							// ��ɱ����boss�ܴ���
	unsigned short kill_active_boss;						// ��ɱ��Ծboss�ܴ���
	unsigned short kill_dabao_boss;							// ��ɱ��boss�ܴ���
	unsigned short kill_other_role_in_fighting;				// ��ɱİ���˴���
	unsigned short mine_times;								// �ڿ����
	unsigned short fish_num;								// ������
	unsigned short sailing_num;								// ����
	unsigned short kill_monster_num;						// ��ɱ��������
	unsigned short active_degree;							// ��Ծ��
	unsigned short kill_vip_boss;							// ��ɱVIPboss�ܴ���
	unsigned short join_gongcheng_zhan_times;				// �μӹ���ս����
	unsigned short reserve_sh1;								// 
	unsigned short kill_boss_count_1;						// ��ɱboss X�ȼ�Y����(��Ӣ+vip)
	unsigned short kill_boss_count_2;						// ��ɱboss X�ȼ�Y��������(��Ӣ+vip)
	unsigned short kill_boss_count_3;						// ��ɱboss X�ȼ�Y��������(��Ӣ+vip)
};

typedef char MagicalPreciousParamHex[sizeof(MagicalPreciousParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(MagicalPreciousParamHex) < 2048);

#pragma pack(pop)

#endif

