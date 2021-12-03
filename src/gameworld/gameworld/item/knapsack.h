#ifndef __KNAPSACK_H__
#define __KNAPSACK_H__

#include "item/itembase.h"
#include "item/itemgriddata.h"

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/basedef.h"
#include "servercommon/serverdef.h"
#include "money.h"
#include "itemcolddownmanager.hpp"
#include "servercommon/maildef.hpp"
#include "itemextern.hpp"
#include "gameworld/gameworld/protocal/msgitem.h"

#include <map>
#include <vector>

class Role;


enum PUT_REASON_TYPE
{
	PUT_REASON_INVALID = 0,								// ��Ч
	PUT_REASON_NO_NOTICE = 1,							// ��֪ͨ
	PUT_REASON_GM = 2,									// GM����
	PUT_REASON_PICK = 3,								// ��ȡ����				
	PUT_REASON_GIFT = 4,								// �����
	PUT_REASON_COMPOSE = 5,								// �ϳɲ���
	PUT_REASON_TASK_REWARD = 6,							// ������
	PUT_REASON_MAIL_RECEIVE = 7,						// �ʼ�
	PUT_REASON_CHEST_SHOP = 8,							// ����
	PUT_REASON_RANDOM_CAMP = 9,							// �����������
	PUT_REASON_SHOP_BUY = 10,							// �̳ǹ���
	PUT_REASON_WELFARE = 11,							// ����
	PUT_REASON_ACTIVE_DEGREE = 12,						// ��Ծ��
	PUT_REASON_CONVERT_SHOP = 13,						// �һ��̵�
	PUT_REASON_ZHUXIE_ACTIVITY_REWARD = 14,				// ��аս������
	PUT_REASON_FB_TOWERDEFEND_TEAM = 15,				// ������������
	PUT_REASON_SEVEN_DAY_LOGIN_REWARD = 16,				// �����¼�����
	PUT_REASON_YAOJIANG = 17,							// ҡ��
	PUT_REASON_ACTIVITY_FIND = 18,						// ��һ�
	PUT_REASON_NEQ_STAR_REWARD = 19,					// ��װ�������ǽ���
	PUT_REASON_NEQ_AUTO = 20,							// ��װ����ɨ��
	PUT_REASON_STORY_ROLL = 21,							// ���鱾����
	PUT_REASON_MAZE = 22,								// �Թ�Ѱ��
	PUT_REASON_STORY_FB = 23,							// ���鸱��
	PUT_REASON_CHALLENGE_FB = 24,						// ��ս����
	PUT_REASON_VIP_LEVEL_REWARD = 25,					// VIP�ȼ�����
	PUT_REASON_GUILD_TASK_REWARD = 26,					// ����������
	PUT_REASON_CHONGZHI_REWARD = 27,					// ��ֵ����
	PUT_REASON_OPENGAME_ACTIVITY = 28,					// �����
	PUT_REASON_DISCOUNT_BUY = 29,						// һ������
	PUT_REASON_PHASE_AUTO = 30,							// �׶α�ɨ������
	PUT_REASON_TURNTABLE = 31,							// ת�̽���
	PUT_REASON_WABAO = 32,								// �ڱ�����
	PUT_REASON_DAILY_TASK_REWARD = 33,					// �ճ�������
	PUT_REASON_CISHA_TASK_REWARD = 34,					// ��ɱ������
	PUT_REASON_JILIAN = 35,								// ��������
	PUT_REASON_GUILD_TASK_ALL_COMPLETE = 36,			// ����������ɽ���
	PUT_REASON_P2PTRADE = 37,							// P2P����
	PUT_REASON_CHALLENGE_FIELD = 38,					// 1v1������
	PUT_REASON_LUCKYROLL = 39,							// ����ת��
	PUT_REASON_LUCKYROLL_EXTRAL = 40,					// ����ת�̶��⽱��
	PUT_REASON_CLOSE_BETA_ACTIVITY = 41,				// ���
	PUT_REASON_UPDATE_NOTICE = 42,						// ���¹��潱��
	PUT_REASON_FETCH_ROLE_UPLEVEL_REWARD = 43,			// ��ȡ��������
	PUT_REASON_FETCH_SIGN_IN_REWARD = 44,				// ��ȡ��¼����
	PUT_REASON_SIGN_IN_FIND_BACK = 45,					// ��ȡ�һص�¼�Ľ���
	PUT_REASON_GET_DAILY_FIND = 46,						// �ճ��һػ����Ʒ
	PUT_REASON_FISH_POOL_HARVEST = 47,					// ����ջ�
	PUT_REASON_FISH_POOL_STEAL = 48,					// ���͵ȡ
	PUT_REASON_ZHANSHENDIAN_DAY_REWARD = 49,			// ս���ÿ�ս���
	PUT_REASON_ZHANSHENDIAN_PASS_REWARD = 50,			// ս���ͨ�ؽ���
	PUT_REASON_WORLD_EVENT_REWARD = 51,					// �����¼�����
	PUT_REASON_RA_SERVER_PANIC_BUY = 52,				// ����ȫ�����
	PUT_REASON_RA_PERSONAL_PANIC_BUY = 53,				// �������˷���
	PUT_REASON_RA_CHESTSHOP_REWARD = 54,				// ���������챦�����
	PUT_REASON_RA_STONE_UPLEVEL_REWARD = 55,			// ������ʯ���������
	PUT_REASON_RA_DAY_CHONGZHI_FANLI_REWARD = 56,		// ����ÿ�ճ�ֵ��������
	PUT_REASON_RA_DAY_CONSUME_GOLD_REWARD = 57,			// ����ÿ�����ѽ���
	PUT_REASON_RA_XN_CHANMIAN_UPLEVEL_REWARD = 58,		// ������Ů�������������
	PUT_REASON_RA_MOUNT_UPGRADE_REWARD = 59,			// ����������׽���
	PUT_REASON_RA_QIBING_UPGRADE_REWARD = 60,			// ����������׽���
	PUT_REASON_RA_MENTALITY_TOTAL_LEVEL_REWARD = 61,	// ��������ȫ��ȼ�����
	PUT_REASON_RA_WING_UPGRADE_REWARD = 62,				// ����������׽���
	PUT_REASON_RA_QUANMIN_QIFU_REWARD = 63,				// ����ȫ��������
	PUT_REASON_ROLE_GOAL_REWARD = 64,					// ��ɫĿ�꽱��
	PUT_REASON_RA_TOTAL_CONSUME_GOLD_REWARD = 65,		// �����ۼ����ѽ���
	PUT_REASON_RA_DAY_ACTIVE_DEGREE_REWARD = 66, 		// ������Ծ��������
	PUT_REASON_RA_KILL_BOSS_REWARD = 67, 				// ������ɱboss����
	PUT_REASON_RA_SHOUYOU_YUXIANG_REWARD = 68, 			// �����������㽱��
	PUT_REASON_RA_LOGIN_GIFT_REWARD = 69,				// ������¼����
	PUT_REASON_LINGYUSHUISHOU = 70,						// ����˰��
	PUT_REASON_USE_MOUNT_JINJIE_CARD = 71,				// ʹ�����������
	PUT_REASON_WUXINGGUAJI_GETREWARD = 72,				// ���йһ���ȡ����
	PUT_REASON_USE_QIBING_JINJIE_CARD = 73,				// ʹ�����������
	PUT_REASON_ZAI_CHONGZHI_REWARD = 74,				// �ٳ�ֵ����
	PUT_REASON_RA_BIPIN_CAPABILITY = 75,				// ��ƴս��
	PUT_REASON_USE_WING_BLESS_CARD = 76,				// ʹ��������׿�Ƭ
	PUT_REASON_RA_DANBI_CHONGZHI_REWARD = 77,			// ���ʳ�ֵ����
	PUT_REASON_CSA_RANK_QIANGGOU = 78,					// �Ϸ���������
	PUT_REASON_CSA_ROLL = 79,							// �Ϸ�ת��
	PUT_REASON_CSA_KILL_BOSS_REWARD = 80,				// �Ϸ���ɱboss����
	PUT_REASON_CSA_LOGIN_GIFT = 81,						// �Ϸ���¼����
	PUT_REASON_CSA_PERSONAL_PANIC_BUY = 82,				// �Ϸ��������˷���
	PUT_REASON_CSA_SERVER_PANIC_BUY = 83,				// �Ϸ��ȫ�����
	PUT_REASON_RA_TOTAL_CHARGE_DAY = 84,				// ����-�ۼƳ�ֵ������������
	PUT_REASON_WUSHAUNG_FB_DAY_REWARD = 85,				// ��˫������ÿ�ս���
	PUT_REASON_WUSHAUNG_FB_PASS_REWARD = 86,			// ��˫������ͨ�ؽ���
	PUT_REASON_RA_TOMORROW_REWARD = 87,					// ����-���ո�������
	PUT_REASON_RA_TOTAL_CHARGE = 88,					// ����-�ۼƳ�ֵ
	PUT_REASON_WUSHUANG_FUMO_TAKEOFF = 89,				// ��˫װ�����¸�ħ
	PUT_REASON_PATAFB_PASS_REWARD = 90,					// ��������ͨ�ؽ���
	PUT_REASON_PATAFB_GLOBAL_FIRSTPASS_REWARD = 91,		// ��������ȫ���״�ͨ�ؽ���
	PUT_REASON_TOWERDEFENDTEAM_PASS_REWARD = 92,		// �����������ͨ�ؽ���
	PUT_REASON_YAOSHOUJITAN_PASS_REWARD = 93,			// ���޼�̳����ͨ�ؽ���
	PUT_REASON_MIGONGXIANFU_REWARD = 94,				// �Թ��ɸ�����ͨ�ؽ���
	PUT_REASON_ZHUXIE_KILL_MONSTER = 95,				// ��а��ɱ������
	PUT_REASON_ZHUXIE_GATHER = 96,						// ��а�ɼ����
	PUT_REASON_EXP_BOTTLE = 97,							// ���۾���
	PUT_REASON_GCZ_DAILY_REWARD = 98,					// ����սÿ�ս���
	PUT_REASON_LIFE_SKILL_MAKE= 99,						// ���������
	PUT_REASON_PAOHUAN_ROLL = 100,						// �ܻ�������
	PUT_REASON_PUTOFF_GUILDSOTRGE = 101,				// ���˲ֿ�
	PUT_REASON_SHENZHUANG_KILLROLE = 102,				// ��װ��ɱ���
	PUT_REASON_RA_CONVERT_SHOP = 103,					// �������ֶһ�
	PUT_REASON_MYSTERIOUSSHOP = 104,					// �����̳ǹ���
	PUT_REASON_RA_LEVEL_LOTTERY = 105,					// ����������
	PUT_REASON_RA_CHONGZHI_NIUEGG_REWARD = 106,			// ������ֵŤ�� 
	PUT_REASON_DIFF_WEEKDAY_CHONGZHI_REWARD = 107,		// ÿ���۳�
	PUT_REASON_RA_ZHENBAOGE_REWARD = 108,				// ����-�䱦������
	PUT_REASON_SHENZHOU_WEAPON_TAKE_OUT = 109,			// ��������-ȡ����Ʒ
	PUT_REASON_SHENZHOU_WEAPON_GATHER = 110,			// ��������-�ɼ�
	PUT_REASON_RA_MONEY_TREE_REWARD = 111,				// ����--ҡǮ��
	PUT_REASON_PET_RECYCLE = 112,						// ���ﵰ����	
	PUT_REASON_PET_REWARD = 113,						// ���ｱ��	
	PUT_REASON_MINE_FETCH_SERVER_REWARD = 114,			// Ȥζ�ڿ�ȫ������
	PUT_REASON_MINE_EXCANGE = 115,						// Ȥζ�ڿ�-�һ�����
	PUT_REASON_FANFAN_EXCHANGE_WORD_REWARD = 116,		// ����ת-�һ�
	PUT_REASON_CONTINUE_CHONGZHI_FETCH_REWARD = 117,	// ������ֵ����
	PUT_REASON_CONTINUE_CHONGZHI_FETCH_EXTRA_REWARD = 118,	// ������ֵ���⽱��
	PUT_REASON_CONTINUE_CONSUME_FETCH_REWARD = 119,		// �������ѽ���
	PUT_REASON_CONTINUE_CONSUME_FETCH_EXTRA_REWARD = 120,	// �������Ѷ��⽱��
	PUT_REASON_ARMY_DAY_EXCHANGE_REWARD = 121,			// ���������һ�
	PUT_REASON_ARMY_DAY_WIN_AYMY_SIDE_REWARD = 122,		// ��������--ʤ������
	PUT_REASON_PASTURE_SPIRIT_LUCKY_DRAW_REWARD = 123,	// ��������齱����
	PUT_REASON_CIRCULATION_CHONGZHI_FETCH_REWARD = 124,	// ������ֵ
	PUT_REASON_EXTREME_LUCKY_DRAW_REWARD = 125,			// �������˳齱����
	PUT_REASON_RA_MONEY_TREE2_REWARD = 126,				// ҡǮ��2
	PUT_REASON_WUSHANG_EQUIP_TAKE_OUT = 127,			// �������ȡ��
	PUT_REASON_WUSHANG_EQUIP_EXCHANGE = 128,			// ��������һ�
	PUT_REASON_RA_NATIONAL_DAY_EXCHANGE = 129,			// ����--���ȹ���
	PUT_REASON_RA_ZHENBAOGE2_REWARD = 130,				// �䱦��2
	PUT_REASON_MITAMA_TASK_REWARD = 131,				// ����������
	PUT_REASON_MITAMA_EXCHANGE_ITEM = 132,				// ��Ȫ���ֶһ�
	PUT_REASON_FAIRY_TREE_FETCH_GIFT = 133,				// ��������
	PUT_REASON_FAIRY_TREE_DRAW_REWARD = 134,			// �����齱
	PUT_REASON_MAGIC_EQUIPMENT_TAKEOFF_STONE = 135,		// ħ��ȡ�±�ʯ
	PUT_REASON_TREASURES_MALL_BUY = 136,				// �䱦�̳ǹ���
	PUT_REASON_TREASURES_MALL_EXCHANGE = 137,			// �䱦�̳Ƕһ�
	PUT_REASON_EXP_FB_FIRST_REWARD = 138,				// ���鱾��ͨ����
	PUT_REASON_ONLINE_REWARD = 139,						// ��ȡ���߽���
	PUT_REASON_HAPPY_TREE_REWARD = 140,					// ��ȡ���ֹ�������
	PUT_REASON_EXCHANG_REWARD = 141,					// �����һ���Ʒ
	PUT_REASON_CROSS_1V1 = 142,							// ���1v1
	PUT_REASON_XIULUOTOWER = 143,						// ���������
	PUT_REASON_QINGYUAN_BLESS_REWARD = 144,				// ��Եף������
	PUT_REASON_HUNYAN_GATHER = 145,						// ����ɼ�
	PUT_REASON_HUNYAN_ZHUFU = 146,						// ����ף��
	PUT_REASON_QINGYUAN_FB_REWARD = 147,				// ��Ե��������
	PUT_REASON_QUESTION_ACTIVY = 148,					// ����-�°�
	PUT_REASON_TIANJIANGCAIBAO_REWARD = 149,			// �콵�Ʊ�����
	PUT_REASON_MOVE_CHESS = 150,						// ������
	PUT_REASON_MIllIONAIRE = 151,						// �󸻺�
	PUT_REASON_WANGLINGEXPLORE_ACTIVITY_REWARD = 152,	// ����̽�ս���
	PUT_REASON_TOUZIHIHUA = 153,						// Ͷ�ʼƻ�����
	PUT_REASON_NEW_CORNUCOPIA = 154,					// �¾۱��轱��
	PUT_REASON_CROSS_TEAM = 155,						// ������
	PUT_REASON_GUILD_REWARD = 156,						// ���ά��
	PUT_REASON_TERRITORY_WAR = 157,						// ����ս
	PUT_REASON_VIP_WEEK_REWARD = 158,					// VIP�ܽ���
	PUT_REASON_ZHUANSHENG_EQUIP = 159,					// ת��װ��
	PUT_REASON_LITTLE_PET_EXCHANGE_ITEM = 160,			// С����һ�
	PUT_REASON_LITTLE_PET_RECYCLE = 161,				// С�������
	PUT_REASON_LITTLE_PET_CHOUJIANG_ONE = 162,			// С����齱1��
	PUT_REASON_LITTLE_PET_CHOUJIANG_TEN = 163,			// С����齱10��
	PUT_REASON_GUILD_TERRITORY_WELF = 164,				// ������ظ���
	PUT_REASON_VIP_PASS_REWARD = 165,					// VIP����ͨ�ؽ���
	PUT_REASON_SHENZHOU_WEAPON_IDENTIFY = 166,			// ��������-����
	PUT_REASON_GUILD_BONFIRE_GATHER = 167,				// �ɼ���������
	PUT_REASON_GUILD_BONFIRE_ADD_MUCAI = 168,			// ��������ľ��
	PUT_REASON_GUILD_BATTEL = 169,						// �������Խ���
	PUT_REASON_HUSONG_TASK = 170,						// ����������
	PUT_REASON_HOTSPRING_ACTIVITY = 171,				// ��Ȫ�����
	PUT_REASON_GCZ_ACTIVITY_ZHANGONG = 172,				// ����սս������
	PUT_REASON_TAKEOFF_SUIT_RETURN = 173,				// ������װ��������
	PUT_REASON_MAGICAL_PRECIOUS = 174,					// ħ���ر�����
	PUT_REASON_ITEM_COLLECTION_REWARD = 175,			// ���ֻ�һ�
	PUT_REASON_ITEM_LOVE_CONTRACT_REWARD = 176,			// ��Ե������Լ
	PUT_REASON_MARRY_REWARD = 177,						// ��Ե���
	PUT_REASON_FB_GUIDE = 178,							// ��������
	PUT_REASON_QUNXIANLUANDOU = 179,					// Ԫ��ս��
	PUT_REASON_GOLD_VIP_DAY_REWARD = 180,				// �ƽ��Աÿ�ս���
	PUT_REASON_GOLD_VIP_CONVERT_ITEM = 181,				// �ƽ��Ա���߶һ�
	PUT_REASON_EQUI_COMPOUND = 182,						// װ���ϳ�
	PUT_REASON_RUNE_TOWER = 183,						// ������
	PUT_REASON_TEAM_EQUIP_FB = 184,						// ���װ������
	PUT_REASON_GUILD_BOX_ASSIST = 185,					// ���ᱦ��Э��
	PUT_REASON_GUILD_BOX_OPEN = 186,					// ���ᱦ�俪��
	PUT_REASON_GCZ_LUCK_REWARD = 187,					// ����ս���˽���
	PUT_REASON_LTZ_LUCK_REWARD = 188,					// ����ս���˽���
	PUT_REASON_GHZB_LUCK_REWARD = 189,					// �����������˽���
	PUT_REASON_YSZC_LUCK_REWARD = 190,					// Ԫ��ս�����˽���
	PUT_REASON_SZLQ_OPEN_BOX_REWARD = 191,				// ���������򿪱��佱��
	PUT_REASON_SZLQ_HELP_BOX_REWARD = 192,				// ��������Э�����佱��
	PUT_REASON_ZODIAC_GGL_REWARD = 193,					// ����������
	PUT_REASON_ZODIAC_BEAD_UNLOAD = 194,				// ��;��ж��
	PUT_REASON_XZYJ_HELP_BOX_REWARD = 195,				// �����ż��򿪱��佱��
	PUT_REASON_XINGZUO_MIJI_COMPOUND = 196,				// �ؼ��ϳ�
	PUT_REASON_ZERO_GIFT = 197,							// ��Ԫ���
	PUT_REASON_ZHIXIAN_TASK_REWARD = 198,				// ֧��������
	PUT_REASON_MARRY_ZHUFU_REWARD = 199,				// ���ף�������Ʒ
	PUT_REASON_MARRY_SONGHUA_REWARD = 200,				// ����ͻ������Ʒ
	PUT_REASON_JING_LING_SKILL = 201,					// ���鼼��
	PUT_REASON_DISCARD_EQUIP = 202,						// �ֽ�װ��
	PUT_REASON_GUILD_MAZE = 203,						// �����Թ�
	PUT_REASON_SHENZHOU_HUNYIN_INLAY = 204,				// ��������-��ӡ��Ƕ
	PUT_REASON_YUANBAO_ZHUANPAN = 205,					// ת�̽���
	PUT_REASON_TUITU_FB_STAR_REWARD = 206,				// ��ͼ�����Ǽ�����
	PUT_REASON_TUITU_FB_SAODANG = 207,					// ��ͼ����ɨ��
	PUT_REASON_JING_LING_HOME = 208,					// �����԰
	PUT_REASON_CONTINUE_CHONGZHI_FETCH_REWARD_CHU = 209,// �����ػݳ�����
	PUT_REASON_CONTINUE_CHONGZHI_FETCH_REWARD_GAO = 210,// �����ػݸ߽���
	PUT_REASON_FIGHTING_MININE_ROB = 211,				// ������ - �ڿ� - ���ٽ���
	PUT_REASON_FIGHTING_MININE_REVENGE = 212,			// ������ - �ڿ� - ������
	PUT_REASON_FIGHTING_SAILINE_ROB = 213,				// ������ - ���� - ���ٽ���
	PUT_REASON_FIGHTING_SAILINE_REVENGE = 214,			// ������ - ���� - ������
	PUT_REASON_JING_LING_EXPLORE = 215,					// ����̽��
	PUT_REASON_GOLDEN_PIG_RANDOM_REWARD = 216,			// �����ٻ��������
	PUT_REASON_OPEN_SERVER_INVEST = 217,				// ����Ͷ�ʽ���
	PUT_REASON_TUITU_FB_FIRST_PASS_REWARD = 218,		// ��ͼ������ͨ����
	PUT_REASON_ADVANCE_NOTICE_REWARD = 219,				// ����Ԥ�潱��
	PUT_REASON_SINGLE_CHONGZHI_REWAED = 220,			// ��������
	PUT_REASON_RA_XIANYUAN_TREAS_REWARD = 221,			// �ۻ���
	PUT_REASON_RA_RUSH_BUYING_BUY_ITEM = 222,			// ��ʱ����
	PUT_REASON_EXTREME_LUCKY_RETURN_REWARD = 223,		// ���������Ƿ�������
	PUT_REASON_PROMOTING_POSITION_REWARD = 224,			// ����������������
	PUT_REASON_MAP_HUNT_EXTERN_REWARD = 225,			// ��ͼѰ������
	PUT_REASON_MAP_HUNT_BASE_REWARD = 226,				// ��ͼѰ����������
	PUT_REASON_MAP_HUNT_RETURN_REWARD = 227,			// ��ͼѰ����������
	PUT_REASON_KING_DRAW_RETURN_REWARD = 228,			// �����뷭���۳鷵��
	PUT_REASON_FIGHTING_CHALLENGE = 229,				// ������ - ��������
	PUT_REASON_JINGLING_ADVANTAGE = 230,				// ��������
	PUT_REASON_PAOHUAN = 231,							// �ܻ�����
	PUT_REASON_QING_YUAN_SHENG_DI = 232,				// ��Եʥ��
	PUT_REASON_DISCARD_JINGLING = 233,					// ���վ���
	PUT_REASON_SHENSHOU_HUANLING = 234,					// ���޻���	
	PUT_REASON_CROSS_GUILDBATTLE_SCORE = 235,			// �������ս
	PUT_REASON_CROSS_GUILDBATTLE_OWN = 236,				// �������ս
	PUT_REASON_LIGHT_TOWER_EXPLORE_EXTERN_REWARD = 237,	// ����Ѱ��
	PUT_REASON_LIGHT_TOWER_EXPLORE_SERVER_REWARD = 238, // ����Ѱ������
	PUT_REASON_MIJINGXUNBAO_REWARD = 239,				// �ؾ�Ѱ������
	PUT_REASON_GUILD_SIGNIN = 240,						// ����ǩ��
	PUT_REASON_CSA_BOSS_GATHER = 241,					// �Ϸ�boss�ɼ�
	PUT_REASON_MAGIC_SHOP_BUY = 242,					// ��װ�̵깺��
	PUT_REASON_MAGIC_SHOP_FETCH = 243,					// ��װ�̵���ȡ
	PUT_REASON_SHEN_YIN = 244,							// ��ӡ����
	PUT_REASON_CROSS_TUANZHAN_FETCH = 245,				// �����ս��ȡ
	PUT_REASON_FISHING_EXCHANGE = 246,					// ����һ�
	PUT_REASON_ELEMENT_HEART_TRANST = 247,				// Ԫ��������Ʒת��
	PUT_REASON_ELEMENT_HEART_SHOP_BUY = 248,			// Ԫ���̵깺��
	PUT_REASON_ELEMENT_EQUIP_RECYCLE = 249,				// Ԫ��װ���ֽ�
	PUT_REASON_ELEMENT_HEART_PRODUCT = 250,				// Ԫ�ز���
	PUT_REASON_UPGRADE_RUSH_RETURN = 251,				// ���ٽ��׷���
	PUT_REASON_FEIXIAN_UP = 252,						// ����װ������
	PUT_REASON_FABAO_UP = 253,						    // �������׷���
	PUT_REASON_SHIZHUANG_UP = 254,                      // ʱװ���������
	PUT_REASON_CHALLENGE_FIELD_BEST_RANK_BREAK=255,     // ����������
	PUT_REASON_NEQ_ROLL = 256,							// ��װ��������
	PUT_REASON_SHENYI_UPGRADE_ONE_RETURN = 257,			// ����ֱ��һ�׷���
	PUT_REASON_CROSS_DARK_NIGHT_REWARD = 258,			// �ºڷ�߽���
	PUT_REASON_SHENGONG_UPGRADE_ONE_RETURN = 259,		// ��ֱ��һ�׷���
	PUT_REASON_LITTLE_PET_TAKEOFF = 260,				// С����ж��
	PUT_REASON_LITTLE_PET_EQUIPMENT_TAKEOFF = 261,		// С����װ������
	PUT_REASON_YUYUE_HUNYAN_REWARD = 262,				// ����ԤԼ���Ž���
	PUT_REASON_QINGYUAN_GIFT = 263,						// ������Ե���
	PUT_REASON_TALENT_GIRD_PUTOFF = 264,				// �츳������ж��
	PUT_REASON_ARMOT_DEFEND_AUTO = 265,					// ����װ����ɨ��
	PUT_REASON_YAOSHI_UPGRADE_ONE_RETURN = 266,			// ����ֱ��һ�׷���
	PUT_REASON_TOUSHI_UPGRADE_ONE_RETURN = 267,			// ͷ��ֱ��һ�׷���
	PUT_REASON_QILINBI_UPGRADE_ONE_RETURN = 268,		// �����ֱ��һ�׷���
	PUT_REASON_MASK_UPGRADE_ONE_RETURN = 270,			// ���ֱ��һ�׷���
	PUT_REASON_GUILD_BOSS_GATHER = 271,					// ����BOSS�ɼ�
	PUT_REASON_ZHAUNGZHI_SRTONE_UNLAY= 272,				// ��ʯж��
	PUT_REASON_ZHUANZHI_SUIT_STUFF = 273,				// ����תְ��װ����
	PUT_REASON_FISHING_GIVE_BAIT = 274,					// �����״ν��뷢�����
	PUT_REASON_FISHING_GIVE_TREASURE_REWARD = 275,		// �������佱��
	PUT_REASON_FISHING_BUY_BAIT = 276,					// �����������
	PUT_REASON_FISH_SCORE_REWARD = 277,					// ���������ֽ���
	PUT_REASON_NIGHT_FIGHT = 278,						// ҹս����
	PUT_REASON_SHANGGU_BOSS_GATHER_REWARD = 279,        // �Ϲ�boss�ɼ��ｱ��
	PUT_REASON_BOSSCARD_REWARD = 280,				    // bossͼ�����뽱��
	PUT_REASON_GUILD_QUESTION_GATHER = 281,				// ���ɴ���ɼ�
	PUT_REASON_1V1_GONGXUN_REWARD = 282,                // ���1v1-��ѫ����
	PUT_REASON_ZHIZUN_EQUIP_COMPOSE = 283,              // ����װ���ϳ�
	PUT_REASON_MESS_BATTLE = 284,						// �Ҷ�ս��
	PUT_REASON_CROSS_PASTURE = 285,						// �������
	PUT_REASON_CROSS_BOSS_GATHER = 286,					// ���boss�ɼ����
	PUT_REASON_3V3_GONGXUN_REWARD = 287,				// ���3v3-��ѫ����
	PUT_REASON_3V3_JOIN_TIMES_REWARD = 288,				// ���3v3-�����������
	PUT_REASON_CROSS_FB_LIEKUN_GATHER = 289,			// �������-����ش��ɼ�
	PUT_REASON_EQUIPMENT_DECOMPOSE = 290,				// ��ͨװ���ֽ�
	PUT_REASON_CONSUM_GIFT_REWARD = 291,                // �����콱
	PUT_REASON_CONSUM_GIFT_ROLL_REWARD = 292,           // �����콱ҡ������
	PUT_REASON_LIMIT_BUY_BUY_ITEM = 293,                // ÿ���޹�������Ʒ
	PUT_REASON_RA_UPGRADE_REWARD = 294, 				// ����ȫ����׽���
	PUT_REASON_HAPPY_CUMUL_CHONGZHI_REWARD = 295,       // �����۳佱��
	PUT_REASON_JINJIE_RETURN_REWARD = 296,              // ���׷�������
	PUT_REASON_SHENGQI_STUFF_DECOMPOSE = 297,           // ʥ�����Ϸֽ�
	PUT_REASON_RA_CONSUME_FOR_GIFT_EXCHANGE_ITEM = 298,	// ��������
	PUT_RA_LIMIT_TIME_REBATE_REWARD = 299,              // ��ʱ����
	PUT_REASON_TIME_LIMIT_GIFT = 300,                   // ��ʱ���
	PUT_REASON_CIRCULATION_CHONGZHI = 301,              // ѭ����ֵ2
	PUT_REASON_TIME_LIMIT_LIUXURY_GIFT_BAG = 302,       // ��ʱ����
	PUT_REASON_RA_RMB_BUY_CHEST_SHOP = 303,             // �����̵�
	PUT_REASON_RA_CONSUME_GOLD_REWARD = 304,            // ���ѷ���
	PUT_REASON_ITEM_COLLECTION_SECOND_REWARD = 305,     // ���±��
	PUT_REASON_VERSION_GRAND_TOTAL_CHARGE = 306,        // �汾�ۼƳ�ֵ
	PUT_REASON_HUANLE_YAOJIANG2 = 307,                  // ����ҡ��2
	PUT_RA_BUY_ONE_GET_ONE_FREE_REWARD = 308,			// ��һ��һ
	PUT_REASON_EVERYDAY_NICE_GIFT_ERWARD = 309,			// ����ÿ�պ���
	PUT_REASON_GUAGUA = 310,                            // ���������ɽ���
	PUT_REASON_HUNJIE_DECOMPOSE = 311,                  // ���ֽ�
	PUT_REASON_BUILD_TOWER_AUTO = 312,					// ������ɨ��
	PUT_REASON_HOLIDAY_GUARD_REWARD = 313,				// �����ػ�����
	PUT_REASON_EXTREME_CHALLENGE_REWARD = 314,          // ������ս����
	PUT_REASON_DRESSING_ROOM_EXCHANGE = 315,			// �¹�һ�
	PUT_REASON_JINJIESYS_REWARD = 316,					// ����ϵͳ����
	PUT_ROLE_BIG_SMALL_GOAL_REWARD = 317,               // �����СĿ��
	PUT_REASON_SHENQI_DECOMPOSE = 318,					// �����ֽ�
	PUT_REASON_SHENQI_INLAY_GIVE_BACK = 319,			// ������Ƕ����
	PUT_REASON_YIZHANDAODI_KILL_REWARD = 320,			// һվ���׻�ɱ����
	PUT_REASON_ZHUANZHI_SOTNE_CONVERT = 321,			// תְ��ʯ�һ�
	PUT_REASON_HUANLEYAOJIANG_REWARD = 322,             // ����ҡ��
	PUT_REASON_HUANLEZADAN_REWARD = 323,                // �����ҵ�
	PUT_REASON_ARMORDEFEND_NEW_PLAYER_REWARD = 324,		// ���߱����ֽ���
	PUT_REASON_RED_EQUIP_COLLECT = 325,                 // ���ͼ¼--��װ�ռ�
	PUT_REASON_JINJIE_RETURN2_REWARD = 326,             // ���׷���2
	PUT_REASON_RA_COMBINE_BUY = 327,                    // ��Ϲ��� 
	PUT_REASON_DAY_CHONGZHI_REWARD = 328,               // ���췵��
	PUT_REASON_XIANZUNKA_FIRST_ACTIVE_REWARD = 329,     // ����
	PUT_REASON_SPOUSE_HOME_BUY = 330,                   // ���޼�԰
	PUT_REASON_GUILD_FB_WAVE_REWARD = 331,              // �����������ν���
	PUT_REASON_CROSS_GUILDBATTLE_TASK = 332,			// �������ս
	PUT_REASON_CROSS_MIZANG_BOSS_GATHER = 333,			// ����ز�boss�ɼ����
	PUT_REASON_CROSS_YOUMING_BOSS_GATHER = 334,			// �����ڤboss�ɼ����
	PUT_REASON_EQUIP_TEAM_LAYER_JUMP = 335,				// ���������������֮ǰ��ĵ��������
	PUT_REASON_SHENYIN_DECOMPOSE = 336,					// ��ӡ�ֽ�
	PUT_REASON_CROSS_GUILDBATTLE_BOSS = 337,			// �������ս��ħ
	PUT_REASON_GREATESOLDIER_FETCH_DRAW_REWARD = 338,	// ������ȡ���׽���
	PUT_REASON_GREATESOLDIER = 339,						// ����
	PUT_REASON_EQUIP_TEAM_BUY_MULTIPLE_REWARD = 340,	// �����������౶����
	PUT_REASON_TOWER_DEFEND_TEAM_BUY_MULTIPLE_REWARD = 341,	// ����ػ�����౶����
	PUT_REASON_MOUNT_UPGRADE_ONE_RETURN = 342,          // ����ֱ��һ�׷���
	PUT_REASON_WING_UPGRADE_ONE_RETURN = 343,           // ����ֱ��һ�׷��� 
	PUT_REASON_FIGHTMOUNT_UPGRADE_ONE_RETURN = 344,     // ս��ֱ��һ�׷���
	PUT_REASON_FOOTPRINT_UPGRADE_ONE_RETURN = 345,      // �㼣ֱ��һ�׷���
	PUT_REASON_HALO_UPGRADE_ONE_RETURN = 346,           // �⻷ֱ��һ�׷���
	PUT_REASON_SHIZHUANG_UPGRADE_ONE_RETURN = 347,		// ʱװֱ��һ�׷���
	PUT_REASON_FABAO_UPGRADE_ONE_RETURN = 348,			// ����ֱ��һ�׷���
	PUT_REASON_RECYCLE_LITTLEPET_RETURN_FEED_STUFF = 349,// ����С���ﷵ��ι������
	PUT_REASON_TOWER_DEFEND_TEAM_WAVE_JUMP = 350,		// ����ػ���������
	PUT_REASON_CRA_CLOUD_PURCHASE_BUY_TICKET = 351,		// һԪ�ƹ���������ȯ
	PUT_REASON_CLOUD_PURCHASE_CONVERT_REWARD = 352,		// һԪ�ᱦ�һ�����
	PUT_REASON_UPGRADE_CARD_BUY = 353,					// ֱ��������
	PUT_REASON_CONTINUE_CHONGZHI_FETCH_REWARD_2 = 354,	// ��ʱ���佱��
	PUT_REASON_LOTTERY_1_SERVER_REWARD = 355,			// ��ĩ��ȫ������
	PUT_REASON_GODMAGIC_BOSS_GATHER = 356,				// ��ħboss
	PUT_REASON_HUNYAN_USE_YANHUA = 357,					// ����ʹ���̻�����
	PUT_REASON_TOWERDEFEND_FIRST_REWARD = 358,			// ���߱����ֽ���
	PUT_REASON_TODAY_THEME_REWARD = 359,				// �������⽱��
	PUT_REASON_CROSS_GOAL_REWARD = 360,					// ���Ŀ�꽱��
	PUT_REASON_ZERO_GIFT_GOD_COSTUME = 361,				// ��Ԫ����Ľ׺�װ
	PUT_REASON_ZERO_BUY_RETURN_REWARD = 362,			// ��Ԫ��������Ʒ
	PUT_REASON_RA_KUANG_HAI_REWARD = 363,				// ������� �˵�һ�����
	PUT_REASON_CROSS_BIANJINGZHIDI_ACTIVITY_REWARD = 364,// ����߾�֮�ؽ���
	PUT_REASON_TIANSHUXZ_REWARD = 365,					// ����Ѱ������
	PUT_REASON_LITTLE_HELPER_BOSS = 366,				// С���֣��Զ���ɱboss
	PUT_REASON_ZHIZUN_EQUIP_COMPOSE2 = 367,				//תְװ���ϳ�
	PUT_REASON_DISCOUNTSHOP_BUY = 367,					// �ػ��̳ǹ���
	PUT_REASON_LUCKY_CLOUD_BUY_MIN_REWARD = 368,		// �����ƹ����׽���
	PUT_REASON_TOWERDEFENF_TEAM_FB_EXTRA_REWARD = 369,	// ����ػ��������⽱��
	PUT_REASON_EQUIP_TEAM_FB_EXTRA_REWARD = 370,		// ��������������⽱��
	PUT_REASON_CRACYBUY_REWARD = 371,					// �񻶴��ֹ�
	PUT_REASON_WEEKEND_EQUIP_ACCUMULATE_REWARD = 372,	// ��ĩװ���۳齱��
	PUT_REASON_CROSS_EQUIP_ROLL = 373,					// ����װ���齱
	PUT_REASON_CROSS_EQUIP_BOSS_GATHER = 374,			// ����װ��boss�ɼ�
	PUT_REASON_CROSS_EQUIP_LIANZHI = 375,				// ����װ������
	PUT_REASON_CROSS_HUSONG_SHUIJING = 376,				// �������ˮ��

	PUT_REASON_UPGRADE_ONE_RETURN = 1000,				// ����ϵͳ��һ�׷���(1000 + upgrade_type)
};

enum DisCardMethod
{
	DISCARD_METHOD_SELL = 0,						// ����
	DISCARD_METHOD_RECYCL = 1,						// ����
};

enum RecyclType
{
	RECYCL_NONE = 0,								// �����޶���
	RECYCL_COIN = 1,								// ͭǮ(������)
	RECYCL_NVWASHI = 2,								// Ů�ʯ(������)
	RECYCL_LINGJING = 3,							// �龫
	RECYCL_BEAST_EXP = 4,							// ʦ�����޾���(������)
	RECYCL_XIUWEEI = 5,								// ��Ϊ
	RECYCL_MOJING = 6,								// ħ��
	RECYCL_LITTLE_PET = 7,							// ����ǿ����
	RECYCL_MARRY_LEVEL_EXP = 8,						// ��龭��ֵ
	RECYCL_XUANTIE = 9,								// ����
	RECYCL_MIYIN = 10,								// ����
	RECYCL_HUNJIE_DECOMPOSE = 11,					// ���ֽ�
	RECYCL_LITTLE_PET_SCORE = 12,					// С������ջ���
	RECYCL_HUNLI = 13,								// �ɳ����ֽ�
	RECYCL_CHUANSHI_FRAGMENT = 14,					// ������Ƭ
};

struct KnapsackDiscardItem
{
	int index = -1;
	int discard_num = 0;
	int discard_method = 0;
	bool is_succ = false;
};

const char *ItemGridDataLog(const ItemBase *itemimp, const ItemGridData &grid_data);
const char *ItemGridQuickLog(const ItemBase *itemimp, const ItemGridData &grid_data);

class Knapsack
{
public:
	Knapsack();
	~Knapsack();

	friend class ItemExtern::ItemConsumeConfigList;

	void SetRole(Role *role);

	void Init(Role *role, int param_num, const ItemListParam::ItemListData *param_list[], int valid_knapsack_num, int valid_storage_num,
		int valid_grid_auto_add_count, Int64 gold, Int64 gold_bind, Int64 coin, Int64 coin_bind);
	void Update(time_t now_second);
	void OnDayChange(unsigned int old_dayid, unsigned int now_dayid);
	void OnResetData();			

	//ʹ�ù̶��������ӵ���Ʒ
	bool Use(short index, short num, short equip_index = -1);
	//����item_id, num �Զ�������Ʒʹ��(���Ȱ�)
	bool UseItem(ItemID item_id, short num);
	
	// �ֽ�--------------------------------------------------------------------------------------------------------
	bool Discard(short index, short discard_num, int discard_method = 0, ItemID item_id_in_client = 0, int item_num_in_client = 0, bool is_notice_client = true);
	bool BatchDiscard(Protocol::CSBatchDiscardItem *discard_item_list);
	bool OneKeyDiscardJiling(int color,int discard_method = DISCARD_METHOD_RECYCL);

	bool CheckCanDiscard(short index, short discard_num, int discard_method = 0, ItemID item_id_in_client = 0, int item_num_in_client = 0);
	void DoneDiscard(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);				// ִ�б�����Ʒ����
	bool DiscardSell(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleNone(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleLingjing(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleXiuwei(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleMojing(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleXuantie(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleMiyin(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleLittlePet(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleMarrylevelExp(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleHunjieDecompose(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecyclePetScore(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleHunli(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);
	bool DiscardRecycleChuanshiFragment(std::vector<KnapsackDiscardItem> &index_list, bool is_notice);

	// ------------------------------------------------------------------------------------------------------------

	bool Move(short from_index, short to_index);
	bool Split(short index, short num);
	bool InOrder(bool is_storage, bool ignore_bind);
	bool TakeOutAllItemFromStorage();			
	
	bool Put(const ItemConfigData &item_configdata, int reason_type, int *out_index = NULL, UInt16 drop_monster_id = 0, int gold_price = 0);
	bool Put(const ItemDataWrapper &item_wrapper, int reason_type, int *out_index = NULL, UInt16 drop_monster_id = 0);
	bool PutList(short list_num, const ItemConfigData *item_configdata_list, short reason_type, int *out_index_list = NULL);		// �о�ֵ��Ʒ�����ô˽ӿ�
	bool PutList(short list_num, const ItemDataWrapper *item_wrapper_list, short reason_type, int *out_index_list = NULL);
	bool PutOrMail(const ItemConfigData &item_configdata, short reason_type);
	bool PutListOrMail(const ItemConfigData item_configdata_list[MAX_ATTACHMENT_ITEM_NUM], short reason_type);
	void PutInKnapsack(const ItemConfigData item_list[MAX_ATTACHMENT_ITEM_NUM]);
	bool PutOrSendToOtherSystem(const ItemConfigData &item_configdata, short reason_type, int *out_index = NULL);					// ���뱳������������������������Ӧϵͳ���ⱳ��
	
	void ChangeAllItemToOtherSex();
	void OnChangeProf();

	int  Count(ItemID item_id);
	bool CheckEmptyGridNoLessThan(int check_num);												// ��鵱ǰ������λ�Ƿ��������ֵ
	bool HaveSpace() { return CheckEmptyGridNoLessThan(1); }
	bool HasItem(ItemID item_id);
	bool ExtendValidNum(bool can_use_gold, bool is_storage, short num, bool is_auto_extend = false); // ��չ����/�ֿ����
	void VipLevelExtendValidNum(Role * level);														 // vip�ȼ���չ��������

	void ClearAll();																				 // ��ձ���������GM����ʹ��
	
	short GetCurrKnapsackValidNum() { return m_curr_knapsack_valid_num; }
	short GetCurrStorageValidNum() { return m_curr_storage_valid_num; }
	short GetValidGridIndexEnd() { return m_valid_grid_index_end; }
	int GetValidGridAutoAddCount() { return m_valid_grid_auto_add_count; }

	short GetCurrKnapscakEmptyGridNum();
	
	bool Lock() { if (m_is_lock) return false; return (m_is_lock = true); }
	void UnLock() { m_is_lock = false; }
	bool IsLock() { return m_is_lock; }

	Money * GetMoney() { return &m_money; }
	ItemColddownManager * GetItemColddownManager() { return &m_item_codldown_manager; }

	bool ConsumeItem(ItemID item_id, const int num, const char *reason, bool need_audit_log = true);
	bool ConsumeItemByIndex(int knapsack_index, int num, const char *reason, bool is_real_consume = true, bool need_audit_log = true);
	bool ConsumeItemByIndexList(int list_num, const short *knapsack_index_list, const short *consume_num_list, const char *reason, bool is_real_consume = true, bool need_audit_log = true);		// ����index�ظ�

	void SendKnapsackInfoToRole();
	void SendKnapsackGridExtendAuto();							// ����ʱ���Զ���չ������Ϣ
	
	bool GetItemGridData(int index, ItemGridData *item_data);
	ItemGridData *GetItemGridData(short index);
	void SetItemDirty(int index, bool to_notice_client = true, short protocol_change_type = ITEM_CHANGE_TYPE_CHANGE, short reason_type = PUT_REASON_INVALID);			
	void RefreshValidGridIndexEnd(int change_index);											// �����ǿո����������
	void GridChangeEquipId(int index, ItemID new_equip_id, bool need_set_dirty);
	void GridSetBind(int index, bool need_set_dirty);
	void GridClearInvalidTime(int index, bool need_set_dirty);

	void GetItemList(int *count, ItemListParam::ItemListData *save_list);
	void GetChangeItemList(int *count, ItemListParam::ItemListData *save_list);
	void ClearDirtyMark();

	bool GetListOfItem(ItemID item_id, short max_num,
					   short *bind_index_list, short *bind_num_list, short *bind_count,
					   short *nobind_index_list, short *nobind_num_list, short *nobind_count);
	
	int GetItemNum(ItemID item_id);

	void GMExtendAll();

	void CheckImpGuardMailInKnapsack(time_t time);

	bool ConsumeItemOrUseGold(ItemID item_id, const int num, const char *reason, bool need_audit_log = true, bool is_use_bind_gold = false);

	void PushNoHaveSpaceWrapper(const MailContentParam &mail_content);
	bool UpdateToMail();

private:

	enum ITEM_CHANGE_TYPE
	{
		ITEM_CHANGE_TYPE_SNEAKY_IN = -4,	// ͵͵�ķ��� ����Ҫ֪ͨ��һ�� ������װ���ͱ�ʯ��Ƕ�ͷ�ʱʹ�����
		ITEM_CHANGE_TYPE_CHANGE = -3,		// �����ı�
		ITEM_CHANGE_TYPE_OUT = -2,			// �ӱ��������ⲿ
		ITEM_CHANGE_TYPE_IN = -1,			// ���ⲿ���뱳��
		// 0���ϱ�ʾ�Ǵӱ���/�ֿ�������������ƶ�����/ȥ ֵ��ʾԭ�����±�
	};

	void SendknapsackGridInfoToRole(int index, int change_type, short reason_type = PUT_REASON_INVALID);

	void CertainPut(const ItemDataWrapper &item_info, int reason_type, int *out_index, UInt16 drop_monster_id = 0);
	bool MoveHelper(int from_index, int to_index);

	Role *m_role;

	bool m_old_state[ItemNamespace::MAX_KNAPSACK_STORAGE_GRID_NUM];
	bool m_dirty_mark[ItemNamespace::MAX_KNAPSACK_STORAGE_GRID_NUM];
	bool m_knapsack_is_change;

	bool m_is_lock;												// ��̯ʱ����
	
	short m_curr_knapsack_valid_num;							// ��ǰ���������� 
	short m_curr_storage_valid_num;								// ��ǰ�ֿ������� 
	short m_valid_grid_index_end;								// ��ǰ�����зǿո��ӵ�����һ���������
	int m_valid_grid_auto_add_count;							// ��ǰ��������������

	ItemGridData m_grid_list[ItemNamespace::MAX_KNAPSACK_STORAGE_GRID_NUM];		// �����ֿ�����

	UInt32 m_knapsack_last_inorder_time;						// �ϴα�������ʱ��
	UInt32 m_storage_last_inorder_time;							// �ϴβֿ�����ʱ��
	UInt32 m_last_inorder_merge_time;							// �ϴ������ϲ�ʱ��

	Money m_money;												// Ǯ
	ItemColddownManager m_item_codldown_manager;				// ��ȴʱ��
	std::vector<MailContentParam> m_no_have_space_mail_vec;		// ִ��CertainPutʱ����û�пռ��ˣ��ڷ��ʼ�ǰ�ȷ������С�����ʼ�����
};

#endif

