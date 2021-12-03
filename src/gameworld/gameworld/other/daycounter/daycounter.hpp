#ifndef __DAY_COUNTER_HPP__
#define __DAY_COUNTER_HPP__

#include <string>
#include "servercommon/serverdef.h"
#include "servercommon/servercommon.h"
#include "config/fbconfig.hpp"

class Role;

// �ṩͳһ��ÿ�����ƴ������߼���Ŀǰֻ������Ҫ���ƴ����ĸ�����������������ÿ���޴��߼��Ͻ���


// Ŀǰ��Ҫ�޴ε��߼���
//			1: ���� (0-127)
//			2: ����

// Ŀǰ�����������������25��	A ~ Z (��daycountrecorder.cpp�ж���)

class DayCounter
{
public:
	DayCounter();
	~DayCounter();

	void SetRole(Role *role) { m_role = role; }

	enum
	{
		DAYCOUNT_ID_FB_START = 0,											// ������ʼ
		DAYCOUNT_ID_FB_XIANNV = 1,											//
		DAYCOUNT_ID_FB_COIN = 2,
		DAYCOUNT_ID_FB_WING = 3,
		DAYCOUNT_ID_FB_XIULIAN = 4,
		DAYCOUNT_ID_FB_QIBING = 5,
		DAYCOUNT_ID_FB_EXP = 6,												// ���鱾
		DAYCOUNT_ID_FB_PATA = 7,											// ����
		DAYCOUNT_ID_FB_STORY = 8,											// ���鱾										
		DAYCOUNT_ID_FB_PHASE_MOUNT = 9,										// ���ױ�-����
		DAYCOUNT_ID_FB_PHASE_SHIZHUANG_BODY = 10,							// ���ױ�-ʱװ
		DAYCOUNT_ID_FB_PHASE_SHIZHUANG_WUQI = 11,							// ���ױ�-���
		DAYCOUNT_ID_FB_PHASE_LINGTONG = 12,									// ���ױ�-��ͯ
		DAYCOUNT_ID_FB_PHASE_FIGHT_MOUNT = 13,								// ���ױ�-ս��
		DAYCOUNT_ID_FB_PHASE_LINGGONG = 14,									// ���ױ�-�鹭
		DAYCOUNT_ID_FB_PHASE_LINGQI = 15,									// ���ױ�-����

		DAYCOUNT_ID_FB_END = MAX_FB_NUM - 1,								// ��������+

		DAYCOUNT_ID_EVALUATE = 61,											// ���۴���
		DAYCOUNT_ID_DAILYFB_EXP_FREE_TIMES = 62,							// �ճ����鸱����Ѵ���
		DAYCOUNT_ID_ACCEPT_HUSONG_TASK_COUNT = 63,							// �������� ��ȡ����
		DAYCOUNT_ID_SHUIJING_GATHER = 64,									// ˮ���ɼ� ����
		DAYCOUNT_ID_YAOSHOUJITAN_JOIN_TIMES = 65,							// ���޼�̳�μӴ���
		DAYCOUNT_ID_FREE_CHEST_BUY_1 = 66,									// һ��Ѱ����Ѵ���
		DAYCOUNT_ID_COMMIT_DAILY_TASK_COUNT = 67,							// �ճ����� �ύ����
		DAYCOUNT_ID_HUSONG_ROB_COUNT = 68,									// �������ٴ���
		DAYCOUNT_ID_HUSONG_TASK_VIP_BUY_COUNT = 69,							// �������� vip�������
		DAYCOUNT_ID_HUSONG_REFRESH_COLOR_FREE_TIMES = 70,					// �������� ���ˢ�´���
		DAYCOUNT_ID_DAILYFB_COIN_FREE_TIMES = 71,							// �ճ�ͭ�Ҹ�����Ѵ���
		DAYCOUNT_ID_GUILD_TASK_COMPLETE_COUNT = 72,							// ����������ɴ���
		DAYCOUNT_ID_FETCH_DAILY_COMPLETE_TASK_REWARD_TIMES = 73,			// �ճ���ȡȫ��������ɽ�������
		DAYCOUNT_ID_ANSWER_QUESTION_COUNT = 74,								// �������-�ɰ�
		DAYCOUNT_ID_VIP_FREE_REALIVE = 75,									// VIP��Ѹ������
		DAYCOUNT_ID_CHALLENGE_BUY_JOIN_TIMES = 76,							// ��ս��������������
		DAYCOUNT_ID_CHALLENGE_FREE_AUTO_FB_TIMES = 77,						// ��ս�������ɨ������
		DAYCOUNT_ID_BUY_ENERGY_TIMES = 78,									// ������������
		DAYCOUNT_ID_MAZE_JOIN_FLAG = 79,									// �Թ�Ѱ��������
		DAYCOUNT_ID_WABAO = 80,												// �ڱ�
		DAYCOUNT_ID_TEAM_TOWERDEFEND_JOIN_TIMES = 82,						// �������������� 
		DAYCOUNT_ID_GCZ_DAILY_REWARD_TIMES = 83,							// ����ս��ȡÿ�ս�������
		DAYCOUNT_ID_XIANMENGZHAN_RANK_REWARD_TIMES = 84,					// ����ս������������
		DAYCOUNT_ID_MOBAI_CHENGZHU_REWARD_TIMES = 85,						// Ĥ�ݳ�������
		DAYCOUNT_ID_GUILD_ZHUFU_TIMES = 86,									// ��������ף������
		DAYCOUNT_ID_KILL_REDNAME_REWARD_TIMES = 87,							// ��ɱ������һ�ȡ�������
		DAYCOUNT_KILL_OTHER_CAMP_COUNT = 88,								// ��ɱ������Ӫ��� ˫����������
		DAYCOUNT_ID_DATING_INVITE = 89,										// Լ������
		DAYCOUNT_ID_CROSS_MULTIUSER_CHALLENGE = 90,							// ���3V3��������
		DAYCOUNT_ID_CROSS_MULTIUSER_CHALLENGE_DAYCOUNT_REWARD = 91,			// ���3V3ÿ�ս���
		DAYCOUNT_ID_MIGOGNXIANFU_JOIN_TIMES = 92,							// �Թ��ɸ��������
		DAYCOUNT_ID_JOIN_YAOSHOUGUANGCHANG = 93,							// �μ����޹㳡ÿ�մ���
		DAYCOUNT_ID_JOIN_SUOYAOTA = 94,										// �μ�������ÿ�մ���
		DAYCOUNT_ID_GATHAR_SELF_BONFIRE = 95,								// �ɼ��Լ��������� ÿ�մ���
		DAYCOUNT_ID_GATHAR_BONFIRE_TOTAL = 96,								// �ɼ��������� �ܴ���
		DAYCOUNT_ID_DABAO_BOSS_BUY_COUNT = 97,								// ����򱦵�ͼ�������
		DAYCOUNT_ID_DABAO_ENTER_COUNT = 98,									// �򱦵�ͼ�������
		DAYCOUNT_ID_WABAO_GATHAR_SUCC_COUNT = 99,							// �ڱ��ɼ��ɹ� ������
		DAYCOUNT_ID_DAILYFB_DAOJU_FREE_TIMES = 100,							// ���߸�����Ѵ���
		DAYCOUNT_ID_JINGHUA_GATHER_COUNT = 101,								// �����ɼ�����
		DAYCOUNT_ID_CAMP_GAOJIDUOBAO = 102,									// ʦ�Ÿ߼��ᱦ
		DAYCOUNT_ID_FREE_CHEST_JINGLING_BUY_1 = 103,						// ����һ��Ѱ����Ѵ���
		DAYCOUNT_ID_GUILD_REWARD = 104,										// ���˽���
		DAYCOUNT_ID_GUILD_BONFIRE_ADD_MUCAI = 105,							// �����������ľ�Ĵ���
		DAYCOUNT_ID_ACTIVE_ENTER_COUNT = 106,								// ��Ծboss��ͼ�������
		DAYCOUNT_ID_JINGLING_SKILL_COUNT = 107,								// ���鼼�����ˢ�´���
		DAYCOUNT_ID_BUY_MIKU_WERARY = 108,									// �����ؿ�ƣ��ֵ
		DAYCOUNT_ID_MONEY_TREE_COUNT = 109,									// ҡǮ���齱����
		DAYCOUNT_ID_JING_LING_HOME_ROB_COUNT = 110,							// ����֮���Ӷ����
		DAYCOUNT_ID_JING_LING_EXPLORE_CHALLENGE = 111,						// ����̽����ս����
		DAYCOUNT_ID_JING_LING_EXPLORE_RESET = 112,							// ����̽�����ô���
		DAYCOUNT_ID_NEQ_FREE_AUTO_FB = 113,									// ��װ�������ɨ������
		DAYCOUNT_ID_BUY_ACTIVE_BOSS_WERARY = 114,							// �����Ծbossƣ��ֵ
		DAYCOUNT_ID_SHANGGUBOSS_ENTER_TIMES = 115,							// �Ϲ�boss�������
		DAYCOUNT_ID_PERSON_BOSS_ENTER_TIMES = 116,                          // ����boss�������
		DAYCOUNT_ID_BUILD_TOWER_FB_BUY_TIMES = 117,							// �������������
		DAYCOUNT_ID_BUILD_TOWER_FB_ENTER_TIMES = 118,						// �������������
		DAYCOUNT_ID_ROB_GUILD_BIAOCHE_COUNT = 119,							// �������������ڳ�����
		DAYCOUNT_ID_TEAM_EQUIP_FB_JOIN_TIMES = 120,                         // ��Ӣ��Ӹ����������
		DAYCOUNT_ID_JINGLING_ADVANTAGE_BOSS_KILL_COUNT = 121,				// ����boss��ɱ����
		DAYCOUNT_ID_PERSON_BOSS_BUY_TIMES = 122,                            // ����boss�������
		DAYCOUNT_ID_COMMIT_PAOHUAN_TASK_COUNT = 123,						// �ܻ����� �ύ����

		DAYCOUNT_ID_MAX,
	};

	UNSTD_STATIC_CHECK(MAX_FB_NUM == 60);
	UNSTD_STATIC_CHECK(DAYCOUNT_ID_MAX <= MAX_DAYCOUNT_COUNT_ID);

	void Init(Role *role, const DayCountData daycount_data);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);	

	bool DayCountCheck(int day_count_id, int max_count);
	void DayCountIncrease(int day_count_id);
	void DayCountReduce(int day_count_id);
	void DayCountReset(int day_count_id);
	void DayCountSet(int day_count_id, int count);

	int GetDayCount(int day_count_id);
	void GetCountData(DayCountData day_count_data); 

	void SendDayCounterInfo();
	void GMResetDayCount();

private:
	void SendDayCounterItemInfo(int day_count_id);

	Role *m_role;

	DayCountData m_daycount_data;
};

#endif // __DAY_COUNTER_HPP__

