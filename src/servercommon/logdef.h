#ifndef __LOGDEF_H__
#define __LOGDEF_H__

#include "utility/logagent.h"

enum LOG_TYPE
{
	LOG_TYPE_INVALID = 0,
	LOG_TYPE_GOLD_ADD = 1,						// ����Ԫ��
	LOG_TYPE_GOLD_USE = 2,						// ʹ��Ԫ��
	LOG_TYPE_GOLD_BIND_ADD = 3,					// ���Ӱ�Ԫ��
	LOG_TYPE_GOLD_BIND_USE = 4,					// ʹ�ð�Ԫ��
	LOG_TYPE_COIN_BIND_ADD = 5,					// ���Ӱ�ͭǮ
	LOG_TYPE_COIN_BIND_USE = 6,					// ʹ�ð�ͭǮ

	LOG_TYPE_ITEM_PUT = 7,						// �����Ʒ
	LOG_TYPE_ITEM_USE = 8,						// ʹ����Ʒ
	LOG_TYPE_ITEM_DISCARD = 9,					// ������Ʒ
	LOG_TYPE_ITEM_CONSUME = 10,					// ������Ʒ

	LOG_TYPE_PUBLIC_SALE_ADD = 11,				// ������Ʒ
	LOG_TYPE_PUBLIC_SALE_DEL = 12,				// ȡ������
	LOG_TYPE_PUBLIC_SALE_BUY = 13,				// �����ɹ�
	LOG_TYPE_PUBLIC_SALE_EXPIRED = 14,			// ��������

	LOG_TYPE_MAIL_ITEM_IN = 15,					// �ʼ������Ʒ
	LOG_TYPE_MAIL_ITEM_OUT = 16,				// �ʼ�������Ʒ

	LOG_TYPE_FORBID_TALK = 17,					// ����
	LOG_TYPE_FORBID_ROLE = 18,					// ��ֹ�û���¼

	LOG_TYPE_CHALLENGE_FIELD_GUANGHUI = 19,		// ���������
	LOG_TYPE_ROLE_SHENGWANG = 20,				// ��ɫ����

	LOG_TYPE_P2P_TRADE_AFFIRM = 21,				// p2p����ȷ��
	LOG_TYPE_FUNCTION_STATUS = 22,				// ���ܲ���
	LOG_TYPE_ACTIVITY_ZHUXIE_TASK = 23,			// ��а������¼
	LOG_TYPE_ACTIVITY_QUNXIANLUANDOU = 24,		// Ⱥ���Ҷ��
	LOG_TYPE_ACTIVITY_XIANMENGZHAN = 25,		// ����ս
	LOG_TYPE_ACTIVITY_GONGCHENGZHAN = 26,		// ����ս

	LOG_TYPE_HUSONG_TASK_ACCEPT = 27,			// �����������
	LOG_TYPE_HUSONG_TASK_ROB = 28,				// ��������
	LOG_TYPE_HUSONG_TASK_BEROB = 29,			// ���ͱ�����
	LOG_TYPE_MAZE_BUY_MOVE = 30,				// �Թ�Ѱ�������ƶ�����(���ã�
	LOG_TYPE_GUILD_PARTY_DOUBLE_REWARD = 31,	// ���˾ƻ�˫������(���ã�
	LOG_TYPE_GUILD_PARTY_RESET_GATHER = 32,		// ���˾ƻ����òɼ�(���ã�
	LOG_TYPE_WABAO = 33,						// ��Ů�Ӷᣨ��Ϊ�ڱ���
	LOG_TYPE_JILIAN = 34,						// �����ɵ�(���ã�
	LOG_TYPE_TEAM_FB_OPEN = 35,					// ��Ӹ�������(���ã�
	LOG_TYPE_TEAM_FB_ASSIST = 36,				// ��Ӹ���Э��(���ã�
	LOG_TYPE_DAILY_TASK_COMPLETE = 37,			// ����ճ�����
	LOG_TYPE_GUILD_TASK_COMPLETE = 38,			// �����������(����)
	LOG_TYPE_CISHA_TASK_COMPLETE = 39,			// ��ɴ�ɱ����(����)

	LOG_TYPE_OPEN_GAME_ACTIVITY_TOTAL_CHONGZHI = 40,// �����ۼƳ�ֵ����
	LOG_TYPE_OPEN_GAME_ACTIVITY_XIANNV = 41,	// �������Ů
	LOG_TYPE_OPEN_GAME_ACTIVITY_MOUNT = 42,		// ���������
	LOG_TYPE_OPEN_GAME_ACTIVITY_STONE = 43,		// �������ʯ
	LOG_TYPE_DISCOUNT_BUY = 44,					// һ������
	LOG_TYPE_DAILY_TOTAL_CHONGZHI_REWARD = 45,	// �ճ��ۼƳ�ֵ����
	LOG_TYPE_VIP_BUY_LEVEL_REWARD = 46,			// Vip�ȼ�����
	LOG_TYPE_VIP_UPLEVEL = 47,					// Vip����
	LOG_TYPE_MONTH_CARD = 48,					// �¿�

	LOG_TYPE_ONLINE_ROLE_NUM = 49,				// ��������
	LOG_TYPE_XIANHUN = 50,						// �ɻ�(���ã�
	LOG_TYPE_YUANLI = 51,						// Ԫ��(���ã�
	LOG_TYPE_HONOUR = 52,						// ����(���ã�
	LOG_TYPE_REGISTER = 53,						// ע��
	LOG_TYPE_LOGIN = 54,						// ��¼
	LOG_TYPE_LOGOUT = 55,						// �ǳ�
	LOG_TYPE_UPGRADE_LEVEL = 56,				// ����
	LOG_TYPE_CHANGE_AUTHORITY_TYPE = 57,		// ����Ȩ��

	LOG_TYPE_ACCEPT_TASK = 58,					// ��������
	LOG_TYPE_COMMIT_TASK = 59,					// �ύ����
	LOG_TYPE_GIVEUP_TASK= 60,					// ��������
	LOG_TYPE_COMPLETE_TASK = 61,				// �������
	
	LOG_TYPE_TOUZIJIHUA = 62,					// Ͷ�ʼƻ�
	LOG_TYPE_WORLD_EVENT_REWARD = 63,			// �����¼�����
	LOG_TYPE_PAY = 64,							// ��ֵ
	LOG_TYPE_PLAT_ONLINE = 65,					// ƽ̨����(����ɫƽ̨��ǰ׺����ͳ��)

	LOG_TYPE_GET_ITEM = 66,						// �����Ʒ

	LOG_TYPE_CROSS_HONOR = 67,					// �������

	LOG_TYPE_CROSS_1V1 = 68,					// ���1v1

	LOG_TYPE_RAND_ACTIVITY_TOTAL_CHONGZHI = 69, // ����-�ۼƳ�ֵ����

	LOG_TYPE_MAIL_SEND_GOLD = 70,				// �ʼ�����Ԫ��
	LOG_TYPE_MAIL_GET_GOLD = 71,				// �ʼ����Ԫ��
	LOG_TYPE_TRADE_SUCC = 72,					// �������飨����Ԫ������Ʒ)
	
	LOG_TYPE_COIN_ADD = 90,						// ���ӷǰ�ͭǮ
	LOG_TYPE_COIN_USE = 91,						// ʹ�÷ǰ�ͭǮ
	LOG_TYPE_PAOHUAN_TASK_COMPLETE = 92,		// ����ܻ�����
	LOG_TYPE_NV_WA_SHI = 93,					// Ů�ʯ
	LOG_TYPE_CHENGJIU = 94,						// �ɾ�
	LOG_TYPE_LINGJING = 95,						// �龫
	LOG_TYPE_HUNLI = 96,						// ����
	LOG_TYPE_CHALLENGE_FIELD_BIND_GOLD = 97,	// ��������Ԫ��
	LOG_TYPE_PET = 98,							// ����
	LOG_TYPE_MAGIC_CARD = 99,					// ħ��
	LOG_TYPE_CROSS_BOSS_SCORE = 100,			// ���boss
	LOG_TYPE_ACTIVITY_TIANJIANGBAO = 101,		// �콵�Ʊ����¼
	LOG_TYPE_ACTIVITY_GUILDBATTLE = 102,		// ��������
	LOG_TYPE_ACTIVITY_QUESTION = 103,			// ����-�°�
	LOG_TYPE_ACTIVITY_WANGLINGEXPLORE = 104,	// ����̽�ջ��¼
	LOG_TYPE_ACTIVITY_TERRITORYWAR = 105,		// ����ս
	LOG_TYPE_ZHUANSHENG = 106,					// ת��
	LOG_TYPE_ROLE_GOAL = 107,					// ����Ŀ��
	LOG_TYPE_GUILD_JOIN = 108,					// ��������
	LOG_TYPE_PHASE_FB = 109,					// ���ױ�
	LOG_TYPE_EXP_FB = 110,						// ���鱾
	LOG_TYPE_TEAM_FB = 111,						// ��ӱ�
	LOG_TYPE_PATA_FB = 112,						// ������
	LOG_TYPE_RUNE_TOWER = 113,					// ������
	LOG_TYPE_KILL_BOSS = 114,					// ��ɱboss
	LOG_TYPE_CROSS_JOIN = 115,					// ������
	LOG_TYPE_GUILD_FB = 116,					// ���ḱ��
	LOG_TYPE_GUILD_BOSS = 117,					// ����boss
	LOG_TYPE_GUILD_BONFIRE = 118,				// Ů��ף��
	LOG_TYPE_RUNE_TOWER_GUAJI = 119,			// �������һ�ʱ��
	LOG_TYPE_CHAT_RECORD = 120,					// �����¼
	LOG_TYPE_CAPABILITY = 121,					// ս���仯
	LOG_TYPE_GUANG_HUI = 122,					// ��Ըı�
	LOG_TYPE_RANDACTIVITY_TOTAL_DATA = 123,		// ����
	LOG_TYPE_HESHENLUOSHU = 124,				// ��������
	LOG_TYPE_JINJIESYS_REWARD = 125,			// ����ϵͳ����
	LOG_TYPE_WASH = 126,                        // װ��ϴ��

	// �������־������һ�״�ӡ���򣬸�֮ǰ����������һ��������LOG2_QUICK16
	LOG2_TYPE_USER_LOGIN = 200,						// ��ҵ�¼
	LOG2_TYPE_USER_LOGOUT = 201,					// �������
	LOG2_TYPE_ADD_GOLD = 202,						// ���Ԫ��
	LOG2_TYPE_USE_GOLD = 203,						// ����Ԫ��
	LOG2_TYPE_GET_ITEM = 204,						// �����Ʒ
	LOG2_TYPE_CONSUME_ITEM = 205,					// ������Ʒ
	LOG2_TYPE_P2P_TRADE = 206,						// ��ҽ���
	LOG2_TYPE_PUBLIC = 207,							// �г�����

	LOG_TYPE_MAX,
};

enum LOG_GET_ITEM_WAY
{
	LOG_GET_ITEM_WAY_XUNBAO = 0,
	LOG_GET_ITEM_WAY_SHOP,
	LOG_GET_ITEM_WAY_COMPOSE,
	LOG_GET_ITEM_WAY_DUIHUAN,

	LOG_GET_ITEM_WAY_MAX,
};

enum LOG_MONEY_TYPE
{
	LOG_MONEY_TYPE_GOLD = 0,
	LOG_MONEY_TYPE_GOLD_BIND,
	LOG_MONEY_TYPE_COIN,
	LOG_MONEY_TYPE_MOJING,
	LOG_MONEY_TYPE_SHENGWANG,
	LOG_MONEY_TYPE_GONGXUN,
	LOG_MONEY_TYPE_WEIWANG,
	LOG_MONEY_TYPE_TREASURE_HUNT_CREDIT,
	LOG_MONEY_TYPE_JINGLING_CREDIT,
	LOG_MONEY_TYPE_HAPPYTREE_GROW,
	LOG_MONEY_TYPE_MYSTERIOUS_SHOP_CREDIT,
	LOG_MONEY_TYPE_GUANGHUI,
	LOG_MONEY_TYPE_BLUE_LINGZHI,
	LOG_MONEY_TYPE_PURPLE_LINGZHI,
	LOG_MONEY_TYPE_ORANGE_LINGZHI,
	LOG_MONEY_TYPE_PRECIOUS_BOSS,
	LOG_MONEY_TYPE_ZHUANZHI_STONE,
	LOG_MONEY_TYPE_HUNJING,
	LOG_MONEY_TYPE_MAX,
};

enum LOG_TYPE_FUNCTION_STATUS_TYPE
{
	LOG_TYPE_FUNCTION_STATUS_TYPE_QIFU_COIN = 0,
	LOG_TYPE_FUNCTION_STATUS_TYPE_QIFU_YUANLI,
	LOG_TYPE_FUNCTION_STATUS_TYPE_QIFU_XIANHUN,
	LOG_TYPE_FUNCTION_STATUS_TYPE_ACCEPT_HUSONG,
	LOG_TYPE_FUNCTION_STATUS_TYPE_ACCEPT_DAILY,
	LOG_TYPE_FUNCTION_STATUS_TYPE_ACCEPT_GUILDTASK,
	LOG_TYPE_FUNCTION_STATUS_TYPE_XUNBAO,
	LOG_TYPE_FUNCTION_STATUS_TYPE_MOVECHESS,

	LOG_TYPE_FUNCTION_STATUS_TYPE_ACTIVITY = 10000,

	LOG_TYPE_FUNCTION_STATUS_TYPE_FB = 20000,
};

enum LOG_CHAT_TYPE
{
	LOG_CHAT_TYPE_SINGLE = 0,
	LOG_CHAT_TYPE_WORLD,
	LOG_CHAT_TYPE_SCENE,
	LOG_CHAT_TYPE_TEAM,
	LOG_CHAT_TYPE_GUILD,
	LOG_CHAT_TYPE_CAMP,
	LOG_CHAT_TYPE_WORLD_QUESTION,
	LOG_CHAT_TYPE_GUILD_QUESTION,

	LOG_CHAT_TYPE_GUILD_NOTICE = 100,
	LOG_CHAT_TYPE_MAX,
};

inline void LOG_QUICK8(LogAgent &log_agent, long long n1, long long n2, const char *z1, const char *z2, long long n3, long long n4, const char *z3, const char *z4)
{
	log_agent.printf(LL_INFO, "%lld\t%lld\t%s\t%s\t%lld\t%lld\t%s\t%s", 
		(long long)n1, 
		(long long)n2,
		(NULL == z1 ? "-" : z1),
		(NULL == z2 ? "-" : z2),
		(long long)n3,
		(long long)n4,
		(NULL == z3 ? "-" : z3),
		(NULL == z4 ? "-" : z4)
		);
}

inline void LOG_QUICK12(LogAgent &log_agent, long long n1, long long n2, const char *z1, const char *z2, long long n3, long long n4, const char *z3, const char *z4, long long n5, long long n6, long long n7, long long n8)
{
	log_agent.printf(LL_INFO, "%lld\t%lld\t%s\t%s\t%lld\t%lld\t%s\t%s\t%lld\t%lld\t%lld\t%lld", 
		(long long)n1, 
		(long long)n2,
		(NULL == z1 ? "-" : z1),
		(NULL == z2 ? "-" : z2),
		(long long)n3,
		(long long)n4,
		(NULL == z3 ? "-" : z3),
		(NULL == z4 ? "-" : z4),
		(long long)n5,
		(long long)n6,
		(long long)n7,
		(long long)n8
		);
}

#endif
