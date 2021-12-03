#ifndef __ACTIVITY_DEF_HPP__
#define __ACTIVITY_DEF_HPP__

#include <map>
#include <vector>
#include <set>
#include <deque>

#include "servercommon/servercommon.h"
#include "servercommon/guilddef.hpp"
#include "servercommon/struct/itemlistparam.h"

#include "campdef.hpp"
#include "servercommon/maildef.hpp"
#include "gameworld/gameworld/gamedef.h"

enum ACTIVITY_TYPE
{
	ACTIVITY_TYPE_INVALID = 0,

	ACTIVITY_TYPE_ZHUXIE = 1,												// ��аս��
	ACTIVITY_TYPE_RESERVE_2 = 2,											// 
	ACTIVITY_TYPE_HUSONG = 3,												// ���ͻ	
	ACTIVITY_TYPE_MONSTER_INVADE = 4,										// ��������
	ACTIVITY_TYPE_QUNXIANLUANDOU = 5,										// Ⱥ���Ҷ�
	ACTIVITY_TYPE_GONGCHENGZHAN = 6,										// ����ս
	ACTIVITY_TYPE_XIANMENGZHAN = 7,                                         // ����ս
	ACTIVITY_TYPE_NATIONAL_BOSS = 8,										// ȫ��boss
	ACTIVITY_TYPE_YIZHANDAODI = 9,											// һվ����
	ACTIVITY_TYPE_MOSHENJIANGLIN = 10,										// ħ����
	ACTIVITY_TYPE_CISHA_TASK = 11,											// ��ɱ����
	ACTIVITY_TYPE_GUAJI = 12,												// �һ��
	ACTIVITY_TYPE_WUXING_GUAJI = 13,										// ���йһ�
	ACTIVITY_TYPE_SHUIJING = 14,											// ˮ���
	ACTIVITY_TYPE_HUANGCHENGHUIZHAN = 15,									// �ʳǻ�ս
	ACTIVITY_TYPE_CAMP_DEFEND1 = 16,										// ��������1
	ACTIVITY_TYPE_CAMP_DEFEND2 = 17,										// ��������2
	ACTIVITY_TYPE_CAMP_DEFEND3 = 18,										// ��������3
	ACTIVITY_TYPE_TERRITORY_WAR = 19,										// ����ս
	ACTIVITY_TYPE_TIANJIANGCAIBAO = 20,										// �콵�Ʊ�
	ACTIVITY_TYPE_GUILDBATTLE = 21,											// ��������
	ACTIVITY_TYPE_HAPPYTREE_GROW_EXCHANGE = 22,								// ���ֹ����ɳ�ֵ�һ�
	ACTIVITY_TYPE_NIGHT_FIGHT_FB = 23,										// ҹս����											
	ACTIVITY_TYPE_GUILD_BOSS = 24,											// ����boss
	ACTIVITY_TYPE_MILLIONAIRE = 25,											// �󸻺�
	ACTIVITY_TYPE_WANGLINGEXPLORE = 26,										// ����̽��
	ACTIVITY_TYPE_GUILD_YUNBIAO = 27,										// ��������
	ACTIVITY_TYPE_XINGZUOYIJI = 28,											// �����ż�
	ACTIVITY_TYPE_WEDDING = 29,												// ����
	ACTIVITY_TYPE_GUILD_QUESTION = 30,										// ���ɴ���
	ACTIVITY_TYPE_MESS_BATTLE_FB = 31,										// �Ҷ�ս��
	ACTIVITY_TYPE_WORSHIP = 32,												// Ĥ�ݳ���
	                                                                        // 33�ͻ��� ��������ҡǮ��
	ACTIVITY_TYPE_GUILD_FB = 34,                                            // ��������
	ACTIVITY_TYPE_GUILD_BONFIRE = 35,										// ��������
	ACTIVITY_TYPE_MAX,
};

static const int SPECIAL_ACTIVITY_FREE_REALIVE_HP_PER = 70;					// ս����Ѹ����Ѫ���ٷֱ�

enum SPECIAL_ACTIVITY_TYPE
{
	SPECIAL_ACTIVITY_TYPE_INVALID = 1024,

	SPECIAL_ACTIVITY_TYPE_OPENGAME,										// �����
	SPECIAL_ACTIVITY_TYPE_CLOSEDBETA,									// ���
	SPECIAL_ACTIVITY_TYPE_COMMON_ACTIVITY,								// ͨ�û
	SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER,								// �Ϸ��

	SPECIAL_ACTIVITY_TYPE_RESVERED,
};

static const int SPECIAL_ACTIVITY_TYPE_MAX = SPECIAL_ACTIVITY_TYPE_RESVERED - SPECIAL_ACTIVITY_TYPE_INVALID;

// ����
enum RAND_ACTIVITY_TYPE
{
	RAND_ACTIVITY_TYPE_BEGIN = 2048,
	RAND_ACTIVITY_TYPE_LUCKY_ROLL = RAND_ACTIVITY_TYPE_BEGIN,			// ����ת��
	RAND_ACTIVITY_TYPE_DAY_CHONGZHI_FANLI = 2049,						// ���ճ�ֵ����
	RAND_ACTIVITY_TYPE_DAY_CONSUME_GOLD = 2050,						    // ��������
	RAND_ACTIVITY_TYPE_TOTAL_CONSUME_GOLD = 2051,						// �ۼ�����
	RAND_ACTIVITY_TYPE_DAY_ACTIVIE_DEGREE = 2052,						// ���ջ�Ծ����
	RAND_ACTIVITY_TYPE_CHONGZHI_RANK = 2053,							// ��ֵ����
	RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK = 2054,						// ��������
	RAND_ACTIVITY_TYPE_SERVER_PANIC_BUY = 2055,							// ȫ���������
	RAND_ACTIVITY_TYPE_PERSONAL_PANIC_BUY = 2056,						// ���˷������
	RAND_ACTIVITY_TYPE_CONSUME_GOLD_FANLI = 2057,						// ���ѷ���
	RAND_ACTIVITY_TYPE_EQUIP_STRENGTHEN = 2058,							// װ��ǿ��
	RAND_ACTIVITY_TYPE_CHESTSHOP = 2059,								// �����챦����ɾ��
	RAND_ACTIVITY_TYPE_STONE_UPLEVEL = 2060,							// ��ʯ��������ɾ��
	RAND_ACTIVITY_TYPE_XN_CHANMIAN_UPLEVEL = 2061,						// ��Ů���ࣨ��ɾ��
	RAND_ACTIVITY_TYPE_MOUNT_UPGRADE = 2062,							// �������
	RAND_ACTIVITY_TYPE_QIBING_UPGRADE = 2063,							// �����������ɾ��
	RAND_ACTIVITY_TYPE_MENTALITY_TOTAL_LEVEL = 2064,					// ����ȫ��ȼ�
	RAND_ACTIVITY_TYPE_WING_UPGRADE = 2065,								// �������
	RAND_ACTIVITY_TYPE_QUANMIN_QIFU = 2066,								// ȫ��������ɾ��
	RAND_ACTIVITY_TYPE_SHOUYOU_YUXIANG = 2067,							// ��������
	RAND_ACTIVITY_TYPE_XIANMENG_JUEQI = 2068,							// ��������
	RAND_ACTIVITY_TYPE_XIANMENG_BIPIN = 2069,							// ���˱�ƴ
	RAND_ACTIVITY_TYPE_DAY_ONLINE_GIFT = 2070,							// ÿ�����ߺ���
	RAND_ACTIVITY_TYPE_KILL_BOSS = 2071,								// BOSS��ɱ
	RAND_ACTIVITY_TYPE_DOUFA_KUANGHUAN = 2072,							// ������
	RAND_ACTIVITY_TYPE_ZHANCHANG_FANBEI = 2073,							// ս����������ɾ��
	RAND_ACTIVITY_TYPE_LOGIN_GIFT = 2074,								// ��¼����
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL = 2075,						// ��ƴ�ۺ�ս��
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP = 2076,						// ��ƴװ��ս��
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV = 2077,						// ��ƴ��Ůս��(��ɾ)
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI = 2078,					// ��ƴ����ս��(��ɾ)
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT = 2079,						// ��ƴ����ս��
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING = 2080,						// ��ƴ����ս��
	RAND_ACTIVITY_TYPE_CHARGE_REPAYMENT = 2081,							// ��ֵ����
	RAND_ACTIVITY_TYPE_SINGLE_CHARGE = 2082,							// ���γ�ֵ
	RAND_ACTIVITY_TYPE_CORNUCOPIA = 2083,								// �۱��裨������
	RAND_ACTIVITY_TYPE_DAY_CHONGZHI_DOUBLE = 2084,						// ��ֵ˫������(��ɾ)
	RAND_ACTIVITY_TYPE_DANBI_CHONGZHI = 2085,							// ���ʳ�ֵ����ͬ����������ͬ��
	RAND_ACTIVITY_TYPE_TOTAL_CHARGE_DAY = 2086,							// �ۼƳ�ֵ���������
	RAND_ACTIVITY_TYPE_TOMORROW_REWARD = 2087,							// ���ո����
	RAND_ACTIVITY_TYPE_DAY_CHONGZHI_DOUBLE_2 = 2088,					// ����Ԫ��(�����÷��ذٷֱ�)
	RAND_ACTIVITY_TYPE_DAY_CHONGZHI_RANK = 2089,						// ÿ�ճ�ֵ���а�
	RAND_ACTIVITY_TYPE_DAY_CONSUME_RANK = 2090,							// ÿ���������а�
	RAND_ACTIVITY_TYPE_TOTAL_CHARGE = 2091,								// �ۼƳ�ֵ
	RAND_ACTIVITY_TYPE_DOUBLE_SCORE = 2092,								// ˫������
	RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_EQUIP = 2093,				// ��ʱ���ֶһ�ϡ��װ��
	RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_JL = 2094,					// ��ʱ���ֶһ�����
	RAND_ACTIVITY_TYPE_LEVEL_LOTTERY = 2095,							// �������齱�
	RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG = 2096,							// ��ֵŤ��
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG = 2097,				// ��ƴ��װս��
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING = 2098,					// ��ƴ����ս��
	RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH = 2099,						// ��ƴ����ս��
	RAND_ACTIVITY_TYPE_ZHENBAOGE = 2100,								// �䱦��
	RAND_ACTIVITY_TYPE_MIJINGXUNBAO = 2101,								// �ؾ�Ѱ��
	RAND_ACTIVITY_TYPE_JISUCHPONGZHAN = 2102,							// ���ٳ�ս
	RAND_ACTIVITY_TYPE_MONEY_TREE = 2103,								// ҡǮ���
	RAND_ACTIVITY_TYPE_DAILY_LOVE = 2104,								// ÿ�հ�һ��
	RAND_ACTIVITY_TYPE_KING_DRAW = 2105,								// �����뷭��
	RAND_ACTIVITY_TYPE_TOTAL_CHARGE_3 = 2106,							// �ۼƳ�ֵ3(�����׻)
	RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_PASSIVE_RANK = 2107,			// �����������а�
	RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_RANK = 2108,					// ��������������а�
	RAND_ACTIVITY_TYPE_ANY_MONSTER_DROP = 2109,							// ȫ���������
	RAND_ACTIVITY_TYPE_TOTAL_CHARGE_4 = 2110,							// �ۼƳ�ֵ4(�������׻)
	RAND_ACTIVITY_TYPE_MINE = 2111,										// Ȥζ�ڿ�
	RAND_ACTIVITY_TYPE_GUAGUA = 2112,									// ���ι�
	RAND_ACTIVITY_TYPE_TIANMING_DIVINATION = 2113,						// ��������
	RAND_ACTIVITY_TYPE_FANFAN = 2114,									// ����ת
	RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI = 2115,						// ������ֵ
	RAND_ACTIVITY_TYPE_CONTINUE_CONSUME = 2116,							// ��������
	RAND_ACTIVITY_TYPE_ARMY_DAY = 2117,									// ��������
	RAND_ACTIVITY_TYPE_ANY_MONSTER_DROP2 = 2118,						// ��ֵ���2
	RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI = 2119,						// ѭ����ֵ
	RAND_ACTIVITY_TYPE_EXTREME_LUCKY = 2120,							// ��������
	RAND_ACTIVITY_TYPE_MIJINGXUNBAO2 = 2121,							// �ؾ�Ѱ��2
	RAND_ACTIVITY_TYPE_MONEY_TREE2 = 2122,								// ҡǮ��2
	RAND_ACTIVITY_TYPE_NATIONAL_DAY = 2123,								// ���ȹ���
	RAND_ACTIVITY_TYPE_ZHENBAOGE2 = 2124,								// �䱦��2
	RAND_ACTIVITY_TYPE_PROMOTING_POSITION = 2125,						// ��������
	RAND_ACTIVITY_TYPE_BLACK_MARKET = 2126,								// ��������
	RAND_ACTIVITY_TYPE_TREASURES_MALL = 2127,							// �䱦�̳�

	RAND_ACTIVITY_TYPE_OPEN_SERVER_BEGIN = 2128,
	RAND_ACTIVITY_TYPE_ROLE_UPLEVEL = RAND_ACTIVITY_TYPE_OPEN_SERVER_BEGIN,// �弶����(�����)
	RAND_ACTIVITY_TYPE_PATA = 2129,										// ����֮��(�����)
	RAND_ACTIVITY_TYPE_EXP_FB = 2130,									// ���鸱��(�����)
	//RAND_ACTIVITY_TYPE_UPGRADE_MOUNT = 2131,							// �������(�����)
	//RAND_ACTIVITY_TYPE_UPGRADE_HALO = 2132,							// �⻷����(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK = 2131,					//ʱװ���װ�(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK = 2132,						//������װ�(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_WING = 2133,								// �������(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG = 2134,							// �񹭽���(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_SHENYI = 2135,							// �������(�����)
	RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN = 2136,						// �׳��Ź�(�����)
	RAND_ACTIVITY_TYPE_DAY_TOTAL_CHARGE = 2137,							// ÿ���ۼƳ�ֵ(�����)
	//RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_TOTAL = 2138,					// ȫ���������(�����)
	//RAND_ACTIVITY_TYPE_UPGRADE_HALO_TOTAL = 2139,						// ȫ���⻷����(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK = 2138,						//�������װ�(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK = 2139,					//�㼣���װ�(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_WING_TOTAL = 2140,						// ȫ���������(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_TOTAL = 2141,					// ȫ���񹭽���(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_TOTAL = 2142,						// ȫ���������(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK = 2143,						// ������װ�(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK = 2144,						// �⻷���װ�(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK = 2145,						// ������װ�(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK = 2146,					// �񹭽��װ�(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK = 2147,						// ������װ�(�����)
	RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL = 2148,				// װ��ȫ��ǿ���ܵȼ����������
	RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL = 2149,						// װ��ȫ��ʯ�ܵȼ����������
	RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK = 2150,			// װ��ȫ��ǿ���ܵȼ���(�����)
	RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK = 2151,					// װ��ȫ��ʯ�ܵȼ���(�����)
	RAND_ACTIVITY_TYPE_BOSS_KILLER = 2152,								// boss����
	RAND_ACTIVITY_TYPE_OPEN_SERVER_BATTLE = 2153,						// ��������
	RAND_ACTIVITY_TYPE_XIANNV_RANK = 2154,								// Ů��ս�����а�
	RAND_ACTIVITY_TYPE_JINGLING_RANK = 2155,							// ����ս�����а�
	RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK = 2156,							// ս������������а�
	RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK = 2157,							// ������ս�����а�
	RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK = 2158,						// ����۽��װ�(�����)	
	RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK = 2159,						// ͷ�ν��װ�(�����)	
	RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK = 2160,						// ���ν��װ�(�����)	
	RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK= 2161,						// ��ͯ���װ�(�����)	
	RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK = 2162,					// �鹭���װ�(�����)	
	RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK = 2163,						// ������װ�(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK = 2164,						// �ɳ���װ�(�����)
	RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK = 2165,						// β����װ�(�����)
	RAND_ACTIVITY_TYPE_OPEN_SERVER_END = 2166,

	RAND_ACTIVITY_TYPE_NEW_CORNUCOPIA = 2167,							// �¾۱���
	RAND_ACTIVITY_TYPE_ITEM_COLLECTION = 2168,							// ���ֻ
	RAND_ACTIVITY_TYPE_MARRY_ME = 2169,									// ���ǽ���
	RAND_ACTIVITY_TYPE_RED_ENVELOPE_GIFT = 2170,						// �������
	RAND_ACTIVITY_TYPE_SUPPER_GIFT = 2171,								// ����޹�
	RAND_ACTIVITY_TYPE_EXP_REFINE = 2172,								// ��������
	RAND_ACTIVITY_TYPE_GOLDEN_PIG =	2173,								// �����ٻ�
	RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU = 2174,					// �����ػݳ�
	RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO = 2175,					// �����ػݸ�
	RAND_ACTIVITY_TYPE_OPEN_SERVER_INVEST = 2176,						// ����Ͷ��
	RAND_ACTIVITY_TYPE_TREASURE_CONVERT = 2177,							// �䱦�һ�
	RAND_ACTIVITY_TYPE_SINGLE_CHONGZHI = 2178,							// ��������
	RAND_ACTIVITY_TYPE_XIANYUAN_TREAS = 2179,							// �ۻ���
	RAND_ACTIVITY_TYPE_RUSH_BUYING = 2180,								// ��ʱ��ɱ
	RAND_ACTIVITY_TYPE_SINGLE_CHARGE_2 = 2181,							// ���ٳ�ս2
	RAND_ACTIVITY_TYPE_SINGLE_CHARGE_3 = 2182,							// ���ٳ�ս3
	RAND_ACTIVITY_TYPE_SINGLE_CHARGE_4 = 2183,							// ���ٳ�ս4
	RAND_ACTIVITY_TYPE_SINGLE_CHARGE_5 = 2184,							// ���ٳ�ս5
	RAND_ACTIVITY_TYPE_MAP_HUNT = 2185,									// ��ͼѰ��
	RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE = 2186,						// ����Ѱ��
	RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE = 2187,							// ���ۼƳ�ֵ
	RAND_ACTIVITY_TYPE_MAGIC_SHOP = 2188,								// ��װ�̵�
	RAND_ACTIVITY_TYPE_PLANTING_TREE = 2189,							// Ȥζֲ��(�������а�Ű�����а�  Ű�����а�ͻ����Լ�������ŵ������а�һ����)
	RAND_ACTIVITY_TYPE_FISHING = 2190,									// ����
	RAND_ACTIVITY_TYPE_CRYSTAL_DALARAN = 2191,							// ����Ȼˮ��
	RAND_ACTIVITY_TYPE_CHONGZHI_CRAZY_REBATE = 2192,					// ��Ԫ��
	RAND_ACTIVITY_TYPE_PERFECT_LOVER = 2193,							// ��������
	RAND_ACTIVITY_TYPE_CONSUM_GIFT = 2194,                              // �����콱
	RAND_ACTIVITY_TYPE_BUY_LIMIT = 2195,                                // ÿ���޹�
	RAND_ACTIVITY_TYPE_COLLECT_TREASURE = 2196,                         // �۱�����
	RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY = 2197,                      // ������
	RAND_ACTIVITY_TYPE_HAPPY_ACCUMUL_RECHARGE = 2198,                   // �����۳�
	RAND_ACTIVITY_TYPE_UPLEVEL_RETURN = 2199,                           // ���׷���
	RAND_ACTIVITY_TYPE_QUANMINJINJIE = 2200,							// ȫ�����
	RAND_ACTIVITY_TYPE_QUANMINZONGDONGYUAN = 2201,						// ȫ���ܶ�Ա
	RAND_ACTIVITY_TYPE_GUILDBATTLE = 2202,								// ��������
	RAND_ACTIVITY_TYPE_LIMIT_TIME_REBATE = 2203,						// ��ʱ����
	RAND_ACTIVITY_TYPE_TIME_LIMIT_GIFT = 2204,							// ��ʱ���
	RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI_2 = 2205,					// ѭ����ֵ2
	RAND_ACTIVITY_TYPE_SHAKE_MONEY = 2206,								// ���ҡǮ��
	RAND_ACTIVITY_TYPE_LIMIT_LUXURY_GIFT_BAG_GIFT = 2207,				// ��ʱ����
	RAND_ACTIVITY_TYPE_DOUBLE_CHONGZHI = 2208,							// ����ͬ��
	RAND_ACTIVITY_TYPE_RMB_BUT_CHEST_SHOP = 2209,						// �����̵�
	RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD = 2210,						// ���ѷ���
	RAND_ACTIVITY_TYPE_ITEM_COLLECTION_2 = 2211,                        // ���±��
	RAND_ACTIVITY_TYPE_VERSIONS_CONTINUE_CHARGE = 2212,					// �汾������ֵ
	RAND_ACTIVITY_TYPE_VERSIONS_GRAND_TOTAL_CHARGE = 2213,				// �汾�ۼƳ�ֵ
	RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2 = 2214,							// ����ҡ��2
	RAND_ACTIVITY_TYPE_OFFLINE_SINGLE_CHARGE_0 = 2215,					// ���µ��ʳ�ֵ0
	RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE = 2216,					    // ������ս
	RAND_ACTIVITY_TYPE_LOGIN_GIFT_0 = 2217,								// ��¼����0
	RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT = 2218,					    // ÿ�պ���
	RAND_ACTIVITY_TYPE_BUY_ONE_GET_ONE_FREE = 2219,						// ��һ��һ
	RAND_ACTIVITY_TYPE_CONSUME_FOR_GIFT = 2220,							// ��������
	RAND_ACTIVITY_TYPE_MIJINGXUNBAO3 = 2221,							// �ؾ�Ѱ��3
	RAND_ACTIVITY_TYPE_HUANLE_ZADAN = 2222,								// �����ҵ�
	RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG = 2223,							// ����ҡ��
	RAND_ACTIVITY_TYPE_TOTAL_CHARGE_FIVE = 2224,						// �ۼƳ�ֵ5(�汾��������)
	RAND_ACTIVITY_TYPE_HOLIDAY_GUARD = 2225,							// �����ػ�(�Լ�)
	RAND_ACTIVITY_TYPE_SPECIAL_IMG_SUIT = 2226,							// �޶���װ
	RAND_ACTIVITY_TYPE_CHONGZHI_GIFT = 2227,							// �������
	RAND_ACTIVITY_TYPE_PROFESS_RANK = 2228,								// ������а�
	RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2 = 2229,                         // ���׷���2
	RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY_2 = 2230,                    // ������2
	RAND_ACTIVITY_TYPE_COMBINE_BUY = 2231,								// ��Ϲ���
	RAND_ACTIVITY_TYPE_LUCKY_WISH = 2232,								// ������Ը
	RAND_ACTIVITY_TYPE_IMAGE_COMPETITION = 2233,						// �����ƴ
	RAND_ACTIVITY_TYPE_DISCOUNT_BUY_HOME = 2234,						// �ۿ���(���޼�԰)
	RAND_ACTIVITY_TYPE_DISCOUNT_BUY_FURNITURE= 2235,					// ��һ��һ(���޼�԰)
	RAND_ACTIVITY_TYPE_COUPLE_HALO_SELL = 2236,							// ���޹⻷�ع�
	RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2 = 2237,						// ��ʱ����
	RAND_ACTIVITY_TYPE_WEST_WEDDING = 2238,                				// ŷʽ������
	RAND_ACTIVITY_TYPE_LOTTERY_1 = 2239,                				// ��ĩ�񻶣��齱��
	RAND_ACTIVITY_TYPE_DISCOUNT_GET_BABY = 2240,                		// ���۱���
	RAND_ACTIVITY_TYPE_SUPPER_GIFT_2 = 2241,							// ����޹�2
	RAND_ACTIVITY_TYPE_ZERO_BUY_RETURN = 2242,                			// ��Ԫ��(�ȹ����ٷ���)
	RAND_ACTIVITY_TYPE_DAILY_LOVE_2 = 2243,								// ÿ��һ��2
	RAND_ACTIVITY_TYPE_KUANG_HAI_QING_DIAN = 2244,						// �������
	RAND_ACTIVITY_TYPE_GIFT_HARVEST = 2245,								// �����ո�
	RAND_ACTIVITY_TYPE_LUCKY_CLOUD_BUY = 2246,							// �����ƹ�
	RAND_ACTIVITY_TYPE_CRACYBUY = 2247,									// �񻶴��ֹ�
	RAND_ACTIVITY_TYPE_XIAN_ZUN_KA = 2248,								// ���𿨻
	RAND_ACTIVITY_TYPE_SINGLE_CHARGE1 = 2249,							// ���γ�ֵ1
	RAND_ACTIVITY_TYPE_SINGLE_CHARGE2 = 2250,							// ���γ�ֵ2
	RAND_ACTIVITY_TYPE_SINGLE_CHARGE3 = 2251,							// ���γ�ֵ3
	RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1 = 2252,						// ���ۼƳ�ֵ1
	RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2 = 2253,						// ���ۼƳ�ֵ2
	RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3 = 2254,						// ���ۼƳ�ֵ3
	RAND_ACTIVITY_TYPE_DOUBLE_GET = 2255,								// ˫��Ԫ��
	RAND_ACTIVITY_TYPE_CHONGZHI_RANK2 = 2256,							// ��ֵ����2

	RAND_ACTIVITY_TYPE_END,
};

static const int RAND_ACTIVITY_TYPE_MAX = RAND_ACTIVITY_TYPE_END - RAND_ACTIVITY_TYPE_BEGIN;

// ������������
enum RAND_ACTIVITY_OPEN_TYPE
{
	RAND_ACTIVITY_OPEN_TYPE_BEGIN = 0,

	RAND_ACTIVITY_OPEN_TYPE_NORMAL = RAND_ACTIVITY_OPEN_TYPE_BEGIN,		// ��ͨ����
	RAND_ACTIVITY_OPEN_TYPE_VERSION,									// �汾�

	RAND_ACTIVITY_OPEN_TYPE_END,
};

enum JINJIE_ACT_THEME_TYPE                  // ���׻����
{
	JINJIE_ACT_THEME_TYPE_INVALID = 0,
	JINJIE_ACT_THEME_TYPE_MOUNT = 1,       // ����
	JINJIE_ACT_THEME_TYPE_WING = 2,        // ����
	JINJIE_ACT_THEME_TYPE_FABAO = 3,       // ����
	JINJIE_ACT_THEME_TYPE_SHENBING = 4,    // ���
	JINJIE_ACT_THEME_TYPE_FOOTSPRINT = 5,  // �㼣
	JINJIE_ACT_THEME_TYPE_GUANGHUAN = 6 ,   // �⻷
	JINJIE_ACT_THEME_TYPE_FASHION = 7,     // ʱװ
	JINJIE_ACT_THEME_TYPE_FIGHT_MOUNT = 8, // ս��
	JINJIE_ACT_THEME_TYPE_TOUSHI = 9,      // ͷ��
	JINJIE_ACT_THEME_TYPE_MASK = 10,        // ����
	JINJIE_ACT_THEME_TYPE_YAOSHI = 11,      // ����
	JINJIE_ACT_THEME_TYPE_QILINBI = 12,     // �����
	JINJIE_ACT_THEME_TYPE_LINGTONG = 13,    // ��ͯ
	JINJIE_ACT_THEME_TYPE_LINGGONG = 14,    // �鹭
	JINJIE_ACT_THEME_TYPE_LINGQI = 15,		// ����
	JINJIE_ACT_THEME_TYPE_SHENGONG = 16,	// ��Ů�⻷
	JINJIE_ACT_THEME_TYPE_SHENYI = 17,		// ��Ů����
	JINJIE_ACT_THEME_TYPE_FLYPET = 18,		// �ɳ�
	JINJIE_ACT_THEME_TYPE_WEIYAN = 19,		// β��

	JINJIE_ACT_THEME_TYPE_MAX,
};

struct RandActivityData
{
	static const int DATA_LENGTH = 127;

	RandActivityData() : begin_time(0), end_time(0), open_type(RAND_ACTIVITY_OPEN_TYPE_BEGIN)
	{
		memset(data, 0, sizeof(data));
	}

	unsigned int begin_time;
	unsigned int end_time;
	char data[DATA_LENGTH];
	char open_type;
};

struct RandActivityParam
{
	RandActivityParam() {}

	void Reset()
	{
		memset(data_list, 0, sizeof(data_list));
	}

	RandActivityData data_list[RAND_ACTIVITY_TYPE_MAX];
};

//////////////////////////////////////////////// ͨ�û ////////////////////////////////////////////////
static const int ACTIVITY_DEFAULT_READY_TIME_S = 300;						// �Ĭ�ϵȴ�ʱ��

union RewardState
{
	RewardState() : data(0) {}

	char reward_state_list[8];
	long long data;
};

static const int COMMON_ACTIVITY_UNIT_MAX_NUM = 32;						// ͨ�û�ӻ�������

enum COMMON_ACTIVITY_UNIT_TYPE											// ͨ�û�ӻ����
{
	COMMON_ACTIVITY_UNIT_TYPE_INVALID = 0,
	COMMON_ACTIVITY_UNIT_TYPE_FOR_TEST = 1,								// Ϊ�˲���

	COMMON_ACTIVITY_UNIT_TYPE_MAX,
};

UNSTD_STATIC_CHECK(COMMON_ACTIVITY_UNIT_TYPE_MAX <= COMMON_ACTIVITY_UNIT_MAX_NUM);

//////////////////////////////////////////////// ���� ////////////////////////////////////////////////
enum CROSS_ACTIVITY_TYPE
{
	CROSS_ACTIVITY_TYPE_INVALID = 3072,
	CROSS_ACTIVITY_TYPE_CROSS_XIULUO_TOWER = 3073,								// ���������
	CROSS_ACTIVITY_TYPE_CROSS_1V1 = 3074,										// ���1v1
	CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE = 3075,						// ���3V3
	CROSS_ACTIVITY_TYPE_TUANZHAN = 3076,										// �����ս
	CROSS_ACTIVITY_TYPE_PASTURE = 3077,											// �������
	CROSS_ACTIVITY_TYPE_CROSS_BOSS = 3078,										// ���boss
	CROSS_ACTIVITY_TYPE_TEAM_FB = 3079,											// �����ӣ����ڿ��ų�����
	CROSS_ACTIVITY_TYPE_HOT_SPRING = 3080,										// �����Ȫ
	CROSS_ACTIVITY_TYPE_SHUIJING = 3081,										// ���ˮ��
	CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE = 3082,								// �������ս
	CROSS_ACTIVITY_TYPE_DARK_NIGHT = 3083,										// ����ºڷ��
	CROSS_ACTIVITY_TYPE_FISHING = 3084,											// �������
	CROSS_ACTIVITY_TYPE_NIGHT_FIGHT_FB = 3085,									// ҹս����
	CROSS_ACTIVITY_TYPE_MESS_BATTLE_FB = 3086,									// �Ҷ�ս��
	CROSS_ACTIVITY_TYPE_LIEKUN_FB = 3087,										// ����ش�
	CROSS_ACTIVITY_TYPE_VIP_BOSS = 3088,                                        // ���vipBoss    
	CROSS_ACTIVITY_TYPE_CROSS_MIZANG_BOSS = 3089,								// ����ز�boss
	CROSS_ACTIVITY_TYPE_CROSS_YOUMING_BOSS = 3090,								// �����ڤboss
										// 3091,								// �����ħ,�ٻ,�������ö�ٵ�ʱ��Ҫ��3091,ֱ������������
	CROSS_ACTIVITY_TYPE_CROSS_BIANJINGZHIDI = 3092,								// ����߾�֮��
	CROSS_ACTIVITY_TYPE_CROSS_CHALLENGEFIELD = 3093,							// ���������
	CROSS_ACTIVITY_TYPE_HUSONG_SHUIJING = 3094,									// �������ˮ��

	CROSS_ACTIVITY_TYPE_RESERVED
};

static const int CROSS_ACTIVITY_TYPE_MAX = CROSS_ACTIVITY_TYPE_RESERVED - CROSS_ACTIVITY_TYPE_INVALID;

enum ACTIVITY_STATUS
{
	ACTIVITY_STATUS_CLOSE = 0,											// ��ر�״̬
	ACTIVITY_STATUS_STANDY,												// �׼��״̬
	ACTIVITY_STATUS_OPEN,												// ���ʼ�� 
	ACTIVITY_STATUS_MAX,
};

struct ActivityStatus
{
	short activity_type;
	short status;
	unsigned int activity_begin_time;
	unsigned int activity_end_time;
	int data;
};

static const int MAX_ACTIVIYT_TIME_CFG_NUM = 5;							// ÿ�������������
static const int MAX_ACTIVITY_OPEN_WEEKDAY = 7;							// ÿ��������������������

struct StatusTimeCfg
{
	StatusTimeCfg() : status(0), start_hour(0), start_min(0), start_sec(0), dur_s(0) {}

	bool Invalid() { return 0 == dur_s; }

	int status;															// ״̬
	int start_hour;														// ��ʼ H
	int start_min;														// ��ʼ M
	int start_sec;														// ��ʼ S
	int dur_s;															// ��������S
};

struct StatusTimeCfgList
{
	StatusTimeCfg statustime[ACTIVITY_STATUS_MAX];
};

struct ActivityCfg
{
	ActivityCfg() : type(ACTIVITY_TYPE_INVALID), is_open(false), statustime_list_count(0) 
	{
		memset(open_weekday_list, 0, sizeof(open_weekday_list));
	}

	bool Invalid() { return ACTIVITY_TYPE_INVALID == type; }

	int type;
	bool is_open;

	int open_weekday_list[MAX_ACTIVITY_OPEN_WEEKDAY];

	int statustime_list_count;
	StatusTimeCfgList statustime_list[MAX_ACTIVIYT_TIME_CFG_NUM];
};

struct SpecialActivityCfg
{
	SpecialActivityCfg() : type(SPECIAL_ACTIVITY_TYPE_INVALID), is_open(false), begin_time(0), end_time(0) {}

	bool Invalid() { return SPECIAL_ACTIVITY_TYPE_INVALID == type; }

	int type;
	bool is_open;

	unsigned int begin_time;
	unsigned int end_time;
};

struct RandActivityCfg
{
	RandActivityCfg() : type(0), is_open(false), duration_day(0) {}

	bool Invalid() { return 0 == type; }

	int type;
	bool is_open;

	int duration_day;
};

//////////////////////////////////////////////// � ��ض��� ////////////////////////////////////////////////
struct ActivityRoomStatus
{
	ActivityRoomStatus() : is_open(0), role_num(0) {}

	bool IsOpen() { return 0 != is_open; }
	void Open() { is_open = 1; }

	int is_open;													
	int role_num;
};

static const int ACTIVITY_ROOM_MAX = 8;												// ������Ļ ͳһΪ8��
static const int ACTIVITY_ROOM_DEFAULT_USER_MAX = 120;								// ������ĻĬ���������
static const int ZHANCHANG_LUACY_ROLE_MAX = 5;										// ����ս������ս���������ԡ�Ԫ��ս������������ֵ

inline const int GetSkillIDBySceneTypeAndIndex(int scene_type, int skill_index)
{
	return 1000 + scene_type * 50 + skill_index;
}

//////////////////////////////////////////////// ս��� ��ض��� ////////////////////////////////////////////////
static const unsigned int FIRST_GUILD_INTERVAL = 60;
static const unsigned int BEST_KILLER_INTERVAL = 30;
static const unsigned int ASSIST_LIMIT_TIME = 30;
static const int GUILD_BATTLE_NEW_POINT_NUM = 5;									// �������Ծݵ�����
static const int GUILD_BATTLE_NOTICE_INTERVAL = 30;									// ���;ݵ�Ѫ��������ʾ��ʱ����
static const int GUILD_BATTLE_NEW_REWARD_CITY_COUNT = 5;							// �������Խ�����������

//////////////////////////////////////////////// ��аս�� ��ض��� ////////////////////////////////////////////////
enum ZHUXIE_TASK_TYPE
{
	ZHUXIE_TASK_TYPE_INVALID = 0,
	ZHUXIE_TASK_TYPE_GATHER,														// ��а�������� �ɼ�
	ZHUXIE_TASK_TYPE_KILL_BOSS,														// ��а�������� ��BOSS
	ZHUXIE_TASK_TYPE_KILL_MONSTER,													// ��а�������� �����
	ZHUXIE_TASK_TYPE_KILL_ROLE,														// ��а�������� ��ɱ��ɫ
	ZHUXIE_TASK_TYPE_MAX,
};

struct ZhuXieTaskInfo																// ��ɫ����ӵ�е���а������Ϣ 
{
	ZhuXieTaskInfo() : task_id(0), task_world_min_level(0), task_world_max_level(0), task_type(ZHUXIE_TASK_TYPE_INVALID), param_id(0), param_value(0), max_value(0), is_fetch_reward(0),
		shengwang(0) {}

	bool IsFinish() { return param_value >= max_value; }
	bool IsFetchReward() { return 0 != is_fetch_reward; }
	bool IsMatch(short _task_type, int _param_id)
	{
		if (task_type == _task_type && 0 == param_id)  //ֻ�������Ͳ�����Ŀ��id
		{
			return true;
		}

		return task_type == _task_type && param_id == _param_id; 
	}

	short task_id;																	// ����ID
	short task_world_min_level;
	short task_world_max_level;
	short task_type;																// ��������
	int param_id;																	// ��������ID �ɼ���ID�����ID
	short param_value;																// ��ǰֵ
	short max_value;																// ���ֵ
	int is_fetch_reward;															// �Ƿ���ȡ����
	int shengwang;

	ItemConfigData reward_item1;
	ItemConfigData reward_item2;
	ItemConfigData reward_item3;
};

struct SpecialZhuXieUser
{
	SpecialZhuXieUser() : user_id(INVALID_USER_ID), kills(0), role_level(0), kill_role_get_total_shengwang(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UserID user_id;
	int kills;
	int role_level;
	GameName user_name;

	int kill_role_get_total_shengwang;
	std::vector<ZhuXieTaskInfo> zhuxietask_list;									// ��аս�� ��ҵ�������Ϣ 
};
typedef std::map<UserID, SpecialZhuXieUser> SpecialZhuXieUserMap;
typedef std::map<UserID, SpecialZhuXieUser>::iterator SpecialZhuXieUserMapIt;

static const int ZHUXIE_TASK_MAX = 4;												// ��аս������������ 

static const int ZHUXIE_REALIVE_POS_MAX = 7;										// ��аս�� ��������
static const int ZHUXIE_LEVEL_REWARD_MAX = 100;										// ��а������ �ȼ����ֶ���
static const int ZHUXIE_BOSS_NUM = 5;												// ��аս��BOSS����
static const int ZHUXIE_MONSTER_NUM = 4;											// ��аս��������������
static const int ZHUXIE_GATHER_NUM = 3;												// ��аս���ɼ�����������
static const int ZHUXIE_FIRST_FLUSH_BOSS_TIME = 600;								// ��аս����һ��ˢ��BOSS��ʱ��

static const int ZHUXIE_MAX_RELACE_BOSS_NUM = 10;									// ��аս���滻BOSS���������
static const int ZHUXIE_MAX_RELACE_MONSTER_NUM = 10;								// ��аս���滻������������
static const int ZHUXIE_MAX_RELACE_GATHER_NUM = 10;									// ��аս���ɼ�����������

static const int ZHUXIE_MONSTER_BORN_LESS_NUM = 10;									// ��������10��ʱˢ��
static const int ZHUXIE_GATHER_BORN_LESS_NUM = 5;									// �ɼ�����10��ʱˢ��

//////////////////////////////////////////////// �콵�Ʊ� ��ض��� ////////////////////////////////////////////////
enum TIANJIANGCAIBAO_TASK_TYPE
{
	TIANJIANGCAIBAO_TYPE_INVALID = 0,
	TIANJIANGCAIBAO_TYPE_GATHER,													// �콵�Ʊ����� �ɼ�
	TIANJIANGCAIBAO_TYPE_KILL_MONSTER,												// �콵�Ʊ����� ��ɱҰ��
	TIANJIANGCAIBAO_TYPE_MAX,
};

static const int TIANJIANGCAIBAO_MAX_REWARD_ITEM_COUNT = 10;						// ������Ʒ���������
static const int TIANJIANGCAIBAO_GOLD_RANK_MAX = 10;								// �콵�Ʊ�Ԫ�����������
static const int MAX_REWARD_CFG_COUNT = 4;											// ���⽱��������

static const int TIANJIANGCAIBAO_TASK_MAX = 20;										// �콵�Ʊ� ���������� 
static const int TIANJIANGCAIBAO_BIGQIAN_NUM = 1;									// �콵�Ʊ� ����Ǯ�������
static const int TIANJIANGCAIBAO_REFLUSH_NOTICE_TIME = 10;							// �콵�Ʊ� ˢ����ʾʱ��

struct TianJiangCaiBaoTaskInfo														// ��ɫ����ӵ�е��콵�Ʊ�������Ϣ 
{
	TianJiangCaiBaoTaskInfo() : task_id(0), task_type(0), param_id(0), param_count(0), cur_param_value(0), is_finish(0), reward_exp(0), exp_factor_task(0){}

	bool IsFinish() { return is_finish != 0; }
	bool IsMatch(short _task_type, int _param_id)
	{
		return task_type == _task_type && param_id == _param_id; 
	}

	void AddParamVal() 
	{
		++ cur_param_value; 
		
		if (cur_param_value >= param_count)
		{
			is_finish = 1;
		}
	}

	short task_id;																	// ����ID

	int task_type;
	int param_id;																	// ���������id(�ɼ������)
	int param_count;																// ��������Ĳɼ������������
	int cur_param_value;															// ��ǰֵ
	int is_finish;																	// �Ƿ��������

	int reward_exp;
	int exp_factor_task;
	ItemConfigData reward_list[MAX_ATTACHMENT_ITEM_NUM];							// ������Ʒ
};

struct RewardItemInfo																// ��õ���Ʒ��Ϣ
{
	RewardItemInfo() : item_id(0), num(0) {}

	ItemID item_id;
	short num;
};

struct RankGold
{
	RankGold() : user_id(INVALID_USER_ID), rank_value(0), get_total_gold(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UserID user_id;
	GameName user_name;

	int rank_value;
	int get_total_gold;	
};

struct TianJiangCaiBaoUser
{
	TianJiangCaiBaoUser() : user_id(INVALID_USER_ID), task_finish_num(0), get_total_gold(0), curr_task_id(-1), curr_task_param(0), lianzhan(0), assist(0)
	{
		memset(is_fetch_reward, 0, sizeof(is_fetch_reward));
	}

	UserID user_id;

	int task_finish_num;
	int get_total_gold;
	int curr_task_id;
	int curr_task_param;
	int lianzhan;
	int assist;

	char is_fetch_reward[MAX_REWARD_CFG_COUNT];
	std::map<int, int> role_get_item_map;												// �콵�Ʊ� ��һ����Ʒ��Ϣ
	std::vector<int> task_list;															// �콵�Ʊ� ������Ϣ�����ڼ�¼δ��ɵ�����
	std::map<UserID, time_t> hurt_map;													// �˺��ҵ� �û��б�	
};

typedef std::map<UserID, TianJiangCaiBaoUser> TianJiangCaiBaoUserMap;
typedef std::map<UserID, TianJiangCaiBaoUser>::iterator TianJiangCaiBaoUserMapIt;

//////////////////////////////////////////////// ����̽�� ��ض��� ////////////////////////////////////////////////
enum WANGLINGEXPLORE_TASK_TYPE
{
	WANGLINGEXPLORE_TYPE_INVALID = 0,
	WANGLINGEXPLORE_TYPE_GATHER,														// ����̽�� �ɼ�
	WANGLINGEXPLORE_TYPE_KILL_MONSTER,													// ����̽�� ��ɱҰ��
	WANGLINGEXPLORE_TYPE_MAX,
};

static const int WANGLINGEXPLORE_TASK_MAX = 5;											// ����̽�� ����������
static const int WANGLINGEXPLORE_REFLUSH_NOTICE_TIME = 10;								// ����̽�� ˢ����ʾʱ��

static const int WANGLINGEXPLORE_MAX_REWARD_ITEM_COUNT = 10;							// ����̽�� ������Ʒ�����
static const int WANGLINGEXPLORE_OWNER_ROLE_AND_BOSS_DISTANCE = 200;					// ����̽�� ����Ȩ������

struct WangLingExploreTaskInfo															// ��ɫ����ӵ�е�����̽��������Ϣ 
{
	WangLingExploreTaskInfo() : task_id(0), task_type(0), param_id(0), param_count(0), cur_param_value(0), is_finish(0), is_double_reward(false), is_fetch_reward(false), item_count(0), reward_exp(0), exp_factor_task(0){}

	bool IsFinish() { return is_finish != 0; }
	bool IsMatch(short _task_type, int _param_id)
	{
		return task_type == _task_type && param_id == _param_id;
	}

	void AddParamVal() 
	{
		++ cur_param_value; 

		if (cur_param_value >= param_count)
		{
			is_finish = 1;
		}
	}

	bool IsDoubleReward() { return is_double_reward; }
	bool IsFetchReward() { return is_fetch_reward; }

	short task_id;																	// ����ID

	int task_type;
	int param_id;																	// ���������id(�ɼ������)
	int param_count;																// ��������Ĳɼ������������
	int cur_param_value;															// ��ǰ���ֵ
	int is_finish;																	// �Ƿ��������
	bool is_double_reward;
	bool is_fetch_reward;

	int item_count;
	ItemConfigData reward_item[MAX_ATTACHMENT_ITEM_NUM];
	int reward_exp;
	double exp_factor_task;
};

struct WangLingExploreUser
{
	WangLingExploreUser() : user_id(INVALID_USER_ID), is_send_item_info(false), task_finish_num(0), is_fetch_reward(false), lianzhan(0), assist(0),
	is_brocast_gather_buff(0),wangling_buff_timestamp(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UserID user_id;
	GameName user_name;

	bool is_send_item_info;															// ����̽�� ���ȫ��������ʾ�������
	int task_finish_num;
	bool is_fetch_reward;															// ����̽�� ���������⽱��
	int lianzhan;
	int assist;
	bool is_brocast_gather_buff;	
	unsigned int wangling_buff_timestamp;										// ����̽�� ���buffʱ��
	std::vector<WangLingExploreTaskInfo> wanglingexplore_task_list;					// ����̽�� ��ҵ�������Ϣ
	std::map<int, int> role_get_item_map;											// ����̽�� ��һ����Ʒ��Ϣ
	std::map<UserID, time_t> hurt_map;												// �˺��ҵ� �û��б�	
};

typedef std::map<UserID, WangLingExploreUser> WangLingExploreUserMap;
typedef std::map<UserID, WangLingExploreUser>::iterator WangLingExploreUserMapIt;

//////////////////////////////////////////////// ����-�ɰ� ��ض��� ////////////////////////////////////////////////
enum QUESTION_STATE
{
	QUESTION_STATE_INVALID = 0,
	QUESTION_STATE_READY,
	QUESTION_STATE_READ,
	QUESTION_STATE_ANSWER,
	QUESTION_STATE_MAX,
};

static const int QUESTION_LIST_NUM = 30;											// ���� �б��С

static const int QUESTION_STRING_MAX = 128;											// ��Ŀ����ַ�		
typedef char QuestionStr[QUESTION_STRING_MAX];										// ��Ŀ����

static const int QUESTION_START_READY_TIME_S = 60;									// ���⿪ʼ׼��ʱ��
static const int QUESTION_READY_TIME_S = 3;											// ��һ���� ׼��ʱ�� 
static const int QUESTION_READ_TIME_S = 7;											// ����ʱ�� 
static const int QUESTION_ANSWER_TIME_S = 15;										// ����ʱ��

static const int QUESTION_RANK_MAX = 10;											// �������а�

struct QuestionRankUser
{
	QuestionRankUser() : uid(0), score(0), right_num(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	int uid;
	GameName user_name;
	int score;
	int right_num;
};

typedef std::vector<QuestionRankUser> QuestionRankUserVec;
typedef std::vector<QuestionRankUser>::iterator QuestionRankUserVecIt;

struct QuestionUser
{
	QuestionUser() : user_id(INVALID_USER_ID), role_level(0), right_num(0), score(0), rankpos(0), answer_num(0), reward_exp(0), reward_shengwang(0), answer(-1), answer_time_s(0), kick_cd_time(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	bool AlreadyAnswer() { return -1 != answer; }
	void ClearAnswerInfo() { answer = -1; answer_time_s = 0; }

	UserID user_id;
	GameName user_name;

	int role_level;
	int right_num;
	int score;
	int rankpos;
	int answer_num;
	int reward_exp;
	int reward_shengwang;

	int answer;
	int answer_time_s;

	unsigned int kick_cd_time;
};
typedef std::map<UserID, QuestionUser> QuestionUserMap;
typedef std::map<UserID, QuestionUser>::iterator QuestionUserMapIt;

//////////////////////////////////////////////// ��Ȫ� ��ض��� ////////////////////////////////////////////////
enum HOTSPRING_ACTION
{
	HOTSPRING_ACTION_INVALID = 0,
	HOTSPRING_ACTION_SANDBALL,														// ��ɳ��
	HOTSPRING_ACTION_HOTWATER,														// ����ˮ
	HOTSPRING_ACTION_MAX,
};

enum HOTSPRING_STATUS
{
	HOTSPRING_STATUS_INVALID = 0,													// ����
	HOTSPRING_STATUS_SAND,															// ɳ��
	HOTSPRING_STATUS_HOT,															// ����
	HOTSPRING_STATUS_MAX,
};

//////////////////////////////////////////////////  �������ս /////////////////////////////
const static int CROSS_GUILDBATTLE_MAX_FLAG_IN_SCENE = 3;				// ����������ڳ�����
const static int CROSS_GUILDBATTLE_MAX_SCENE_NUM = 6;					// ���ɳ�������
const static int CROSS_GUILDBATTLE_MAX_TASK_NUM = 6;					// �����������
UNSTD_STATIC_CHECK(CROSS_GUILDBATTLE_MAX_SCENE_NUM == 6);
const static int CROSS_GUILDBATTLE_MAX_GUILD_RANK_NUM = 5;				// �������սǰ5
const static int CROSS_GUILDBATTLE_MAX_LOG_RANK_NUM = 10;				// �������ս�����ʱ����ǰ10��������
const static int CROSS_GUILDBATTLE_MAX_USER_SCORE_RANK_NUM = 100;		// �������ս���һ��������������

///////////////////////////////////////////////////////////  �����������  /////////////////////////////////////////////////////////////
static const int LINGMAI_LINGZHU_NUM_MAX = 4;										// һ��������ͬʱ��ֲ��������
static const int LINGMAI_RAND_ITEM_GET_TIME_S = 12 * 3600;							// 12Сʱ�ɻ��ϡ����Ʒ
static const int LINGMAI_LINGZHU_REWARD_TIME_MAX_HOUR = 24;							// ����������24Сʱ�Ľ���
static const int LINGMAI_DISAPPEAR_TIME_S = LINGMAI_LINGZHU_REWARD_TIME_MAX_HOUR * 3600;	// 24��ʧ��������ʧ
static const int LINGMAI_ZHADAN_ROLE_EFFECT_LEVEL = 48;								// ը������ը���Ľ�ɫ����ȼ�

enum LINGMAI_TYPE
{
	LINGMAI_TYPE_INVALID = 0,
	LINGMAI_TYPE_LINGZHU,															// ����
	LINGMAI_TYPE_ZHADAN,															// ը��
	LINGMAI_TYPE_SOULINGFU,															// �����
	LINGMAI_TYPE_MAX,
}; 

struct LingMaiItem
{
	LingMaiItem() 
		: unique_id(0), owner_uid(0), owner_level(0), owner_camp(0), lingmai_type(LINGMAI_TYPE_INVALID), reward_index(-1), 
		scene_id(0), pos_x(0), pos_y(0), plant_timestamp(0)
	{
		memset(owner_name, 0, sizeof(owner_name));
	}

	long long unique_id;															// ����Ψһ��ʾID ʱ���+����ֵ

	int owner_uid;																	// ����ID
	GameName owner_name;															// ��������
	int owner_level;																// ���˵ȼ�

	char owner_camp;																// ������Ӫ
	char lingmai_type;																// �������ͣ������ը����������
	short reward_index;																// ������Ʒ���±�

	int scene_id;																	// ����ID
	int pos_x;																		// ��������X
	int pos_y;																		// ��������Y

	unsigned int plant_timestamp;													// ������ֲʱ���
};

//////////////////////////////////////////////// Ⱥ���Ҷ� ��ض��� ////////////////////////////////////////////////

static const int MAX_MULTI_BATTLEFILED_SKILL_COUNT = 4;								// ����ս����������

static const int ZHANCHANG_RESERVE_INFO_TIME_S = 30 * 60;							// ս�������û���Ϣ ʱ��

static const int QUNXIANLUANDOU_RANK_NUM= 10;										// Ⱥ���Ҷ� ʵʱ���а���Ϣ����
static const int QUNXIANLUANDOU_SHENSHI_REFRESH_TIME_S = 5 * 60;					// ��ʯˢ��ʱ�� ����ʯ������5���Ӻ���ˢһ����

static const int QUNXIANLUANDOU_KILL_VALID_TIME_S = 30;								// ��ɱ��Ч���ʱ��
static const int QUNXIANLUANDOU_ASSIST_VALID_TIME_S = 5;							// ������Чʱ��
static const unsigned int QUNXIANLUANDOU_REALIVE_HERE_TIME_S = 0;					// ԭ�ظ���ʱ����0s

static const int BATTLEFIELD_SHENSHI_MAX_OWNER_DISTENCE = 2;						// ս����ʯ �������
static const int BATTLEFIELD_SHENSHI_DEST_POINT_DISTANCE = 10;						// ս����ʯ ����Ŀ��������Χ
static const int BATTLEFIELD_SHENSHI_MAX_PICK_UP_DISTANCE = 10;						// ��ʯʰȡ��Զ����

enum QUNXIANLUANDOU_SIDE
{
	QUNXIANLUANDOU_SIDE_1,															// Ⱥ���Ҷ� ��1							
	QUNXIANLUANDOU_SIDE_2,															// Ⱥ���Ҷ� ��2
	QUNXIANLUANDOU_SIDE_3,															// Ⱥ���Ҷ� ��3
	QUNXIANLUANDOU_SIDE_MAX,
};

struct QunXianLuanDouSideInfo														// Ⱥ���Ҷ� ����Ϣ
{
	QunXianLuanDouSideInfo() : role_num(0), score(0), robot_num(0), is_weak_side_protect(0) {}

	int role_num;
	int score;																		// ����
	int robot_num;
	bool is_weak_side_protect;														// �Ƿ�����Ӫ����
};

struct QunXianLuanDouUserRankItem													// Ⱥ���Ҷ� ���а���Ϣ
{
	QunXianLuanDouUserRankItem() : user_id(INVALID_USER_ID), side(0), score(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UserID user_id;	
	GameName user_name;																// ��ɫ��
	int side;																		// ��ɫ��ս���е���Ӫ
	int score;																		// նɱ����
};

class SpecialQunXianLuanDou;
class Role;
class Scene;

struct QunXianLuanDouUser															// Ⱥ���Ҷ� �û���Ϣ
{
	QunXianLuanDouUser() : user_id(INVALID_USER_ID), vip(0), sex(0), prof(0), camp(0), level(0), gsnetid(INVALID_GSNETID),
		side(-1), kills(0), lianzhan(0), assists(0), lianzhan_max(0), rob_shenshi(0), 
		obj_id(INVALID_OBJ_ID), kill_honor(0), assist_honor(0), rob_shenshi_honor(0), free_reward_honor(0), extra_honor(0), last_leavetime(0), dead_time(0),
		last_realive_here_timestamp(0), score_reward_flag(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	bool GSNetIDValid() { return INVALID_GSNETID != gsnetid; }

	void SetSide(int _side) { side = _side; }
	bool InvalidSide() { return (side < QUNXIANLUANDOU_SIDE_1 || side >= QUNXIANLUANDOU_SIDE_MAX); }
	int GetSide() { return side; }

	void OnEnterActivity(Role *role); 
	void OnLeaveActivity(time_t now_second) { gsnetid = INVALID_GSNETID; obj_id = INVALID_OBJ_ID; last_leavetime = now_second; }

	void OnHurt(const UserID &attack_userid, time_t now_second);
	void OnKill(SpecialQunXianLuanDou *special_logic, bool is_reward);
	void OnDead(SpecialQunXianLuanDou *special_logic, time_t _dead_time, bool reset_lianzhan);
	void OnAssist(SpecialQunXianLuanDou *special_logic);
	void OnRobShenShi(SpecialQunXianLuanDou *special_logic);
	void OnSideMemberRobShenShi(SpecialQunXianLuanDou *special_logic);
	void OnFreeReward(SpecialQunXianLuanDou *special_logic, int reward_honor);
	void ChangeZhanyi(int add_zhanyi);
	void OnExtraHonor(SpecialQunXianLuanDou *special_logic, int reward_honor);

	int TotalHonor() const { return kill_honor + assist_honor + rob_shenshi_honor + free_reward_honor + extra_honor; } 
	int TotalHonor() { return kill_honor + assist_honor + rob_shenshi_honor + free_reward_honor + extra_honor; } 
	bool IsLeaveBattleField() { return 0 != last_leavetime; }

	bool IsFetchScoreRewardFlag(int reward_index)
	{
		return 0 != (score_reward_flag & (1 << reward_index));
	}

	void SetFetchScoreRewardFlag(int reward_index)
	{
		score_reward_flag |= (1 << reward_index);
	}

	Role * GetRole(Scene *scene);

	UserID user_id;																	// �û�ID
	GameName user_name;																// �û���
	char vip;
	char sex;
	char prof;
	char camp;
	int level;

	GSNetID gsnetid;																// �û��������ID 

	short side;																		// ��ɫ��ս���е���Ӫ
	short kills;																	// ɱ�˴���
	short lianzhan;																	// ��ɱ����
	short assists;																	// ��������
	short lianzhan_max;																// �����ɱ
	short rob_shenshi;																// ��ʯ����
	ObjID obj_id;																	// �û��ڳ����ϵ�ID

	int kill_honor;																	// ɱ������
	int assist_honor;																// ��������
	int rob_shenshi_honor;															// ��ʯ����
	int free_reward_honor;															// ��ѽ�������
	int extra_honor;																// ��������

	time_t last_leavetime;															// ����뿪ʱ��
	time_t dead_time;																// ս��������ʱ��

	unsigned int last_realive_here_timestamp;										// �ϴ�ԭ�ظ���ʱ��

	int score_reward_flag;															// ���˽������

	std::map<UserID, time_t> hurt_map;												// �˺��ҵ� �û��б�

	std::map<UserID, time_t> kill_user_map;											// ��ɱ������б� 
};
typedef std::map<UserID, QunXianLuanDouUser> QunXianLuanDouUserMap;
typedef std::map<UserID, QunXianLuanDouUser>::iterator QunXianLuanDouUserMapIt;

//////////////////////////////////////////////// ����ս ��ض��� ////////////////////////////////////////////////
static const int XIANMENGZHAN_REALIVE_AREA_NUM    = 5;                              // ���������
static const int XIANMENGZHAN_DEFEND_AREA_NUM     = 5;                              // ����������

static const unsigned int XIANMENGZHAN_ROLE_REALIVE__HERE_TIME_S = 20;				// ���ԭ�ظ���CD
static const int XIANMENGZHAN_SYNC_USER_INFO_TIME_S = 1;                            // ͬ�������Ϣ���
static const int XIANMENGZHAN_SYNC_GUILD_INFO_TIME_S = 3;                           // ͬ��������Ϣ���
static const int XIANMENGZHAN_SYNC_DEFEND_AREA_LIST_TIME_S = 5;                     // ͬ���ݵ���Ϣ���
static const int XIANMENGZHAN_SYNC_GUILD_RANK_TIME_S = 3;							// ͬ������������Ϣ���
static const int XIANMENGZHAN_KILL_VALID_TIME_S = 10;								// ����ս��ɱ��Ч���ʱ��
static const int XIANMENGZHAN_ASSIST_VALID_TIME_S = 5;								// ����ս������Ч���ʱ��

static const int XIANMENGZHAN_REWARD_GUILD_RANK_MAX = 10;							// ����ս���н���
static const int XIANMENGZHAN_REWARD_GUILD_RANK_GIFT_MAX = 10;						// ����ս�����������

struct XianMengZhanUser
{
	XianMengZhanUser() 
		: dirty(false), user_id(INVALID_USER_ID), net_id(INVALID_GSNETID), obj_id(INVALID_OBJ_ID), level(0),
		guild_id(INVALID_GUILD_ID), call_allow(false), call_count(0), score(0), assist_count(0), free_score(0), kill_count(0), kill_score(0), lianzhan(0), last_realive_here_timestamp(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	bool dirty;

	UserID user_id;
	GSNetID net_id;
	ObjID obj_id;
	int level;
	GameName user_name;

	GuildID guild_id;
	bool call_allow;
	int call_count;

	int score;
	int assist_count;
	int free_score;
	int kill_count;
	int kill_score;
	int lianzhan;

	unsigned int last_realive_here_timestamp;

	std::map<UserID, time_t> hurt_map;							// �˺��ҵ� �û��б�
	std::map<UserID, time_t> kill_user_map;						// ��ɱ������б� 
};

typedef std::map<UserID, XianMengZhanUser> XianMengZhanUserMap;
typedef XianMengZhanUserMap::iterator XianMengZhanUserMapIt;

struct XianMengZhanData
{
	XianMengZhanData()
		: guild_id(INVALID_GUILD_ID), guild_score(0), camp(0)
	{
		memset(guild_name, 0, sizeof(guild_name));
	}
	void Clear()
	{
		guild_id = INVALID_GUILD_ID;
		memset(guild_name, 0, sizeof(guild_name));
		guild_score = 0;
		camp = CAMP_TYPE_INVALID;
	}

	GuildID guild_id;
	GuildName guild_name;
	int guild_score;
	int camp;
};

struct XianMengZhanGuild
{
	XianMengZhanGuild()
		: dirty(false), defend_area(-1), caller_pos_x(0), caller_pos_y(0), last_call_time(0), enter_area(-1)
	{
	}
	bool HasDefendArea() { return defend_area >= 0 && defend_area < XIANMENGZHAN_DEFEND_AREA_NUM; }

	bool dirty;

	int defend_area;

	int caller_pos_x;
	int caller_pos_y;
	time_t last_call_time;

	int enter_area;

	XianMengZhanData data;
};

typedef std::map<GuildID, XianMengZhanGuild> XianMengZhanGuildMap;
typedef XianMengZhanGuildMap::iterator XianMengZhanGuildMapIt;

struct XianMengZhanDefendArea
{
	XianMengZhanDefendArea()
		: dirty(false), gather_obj_id(INVALID_OBJ_ID)
	{
	}

	void Clear()
	{
		dirty = false;
		gather_obj_id = INVALID_OBJ_ID;
		data.Clear();
	}
	bool HasGuild() { return INVALID_GUILD_ID != data.guild_id; }

	bool dirty;
	ObjID gather_obj_id;
	XianMengZhanData data;
};

/////////////////////////////////////////////////  �Ҷ�ս�� /////////////////////////////
const static int MESS_BATTLE_TOP_RANK_MAX = 3;						// �Ҷ�ս���ܰ�����
const static int CROSS_MESS_BATTLE_MAX_SCENE_NUM = 10;					// �Ҷ�ս����󷿼����

////////////////////////////////////////////////// ȫ�� boss ////////////////////////
struct NationalBossRankItem													
{
	NationalBossRankItem() : uid(0), camp(0), hurt(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	int uid;	
	char camp;
	GameName user_name;
	long long hurt;																	
};

static const int NATIONALBOSS_FRESH_CONFIG_COUNT = 16;
static const int NATIONALBOSS_FRESH_BOSS_COUNT = 4;

//////////////////////////////////////////////// ����� /////////////////////////////////////////////////////////
static const int OGA_PUTONEQUIPMENT_REWARD_MAX_SEQ = 32;
static const int OGA_CAPABILITY_REWARD_MAX_SEQ = 32;
static const int OGA_ROLELEVEL_REWARD_MAX_SEQ = 32;
static const int OGA_BOSS_MAX_SEQ = 64;								// BOSS������ֵ(���ܴ���32)
static const int OGA_BOSS_REWARD_MAX_SEQ = 20;						// BOSS������ֵ(���ܴ���32)
static const int OGA_GIFT_MAX_TYPE = 8;								// ����޹��������ֵ
static const int OGA_GIFT_MAX_SEQ = 20;								// ����޹�������ֵ(���ܴ���32)

//////////////////////////////////////////////// ���� /////////////////////////////////////////////////////////

//////////////////////////////////////////////// ���� ����ҡ�� /////////////////////////////////////////////////////////
static const int RAND_ACTIVITY_TEN_COUNT_ROLL = 10;
struct RandActivityLuckyRollNewsItem
{
	RandActivityLuckyRollNewsItem() : news_id(0), role_id(0), role_camp(0), buy_item_id(0) 
	{
		memset(role_name, 0, sizeof(role_name));
	}

	int news_id;
	int role_id;
	GameName role_name;
	int role_camp;
	int buy_item_id;
};

//////////////////////////////////////////////// ���� ȫ������ /////////////////////////////////////////////////////////
static const int RAND_ACTIVITY_SERVER_PANIC_BUY_ITEM_MAX_COUNT = 16;// ȫ��������Ʒ��������

//////////////////////////////////////////////// ���� �������� /////////////////////////////////////////////////////////
static const int RAND_ACTIVITY_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT = 8;// ����������Ʒ��������

//////////////////////////////////////////////// ���� ������ /////////////////////////////////////////////////////////
static const int RAND_ACTIVITY_DOUFA_KUANGHUAN_REWARD_TIMES = 2;	// ������ ��������

//////////////////////////////////////////////// ���� ս������ /////////////////////////////////////////////////////////
static const int ZHANCHANG_FANGBEI_MAX_REWARD_TIMES = 2;	// ս������ ��������

//////////////////////////////////////////////// ���� ������� /////////////////////////////////////////////////////////
static const int RAND_ACTIVITY_HONGBAO_HAOLI_MAX_COUNT = 7;// ������񣬺������

//////////////////////////////////////////////// ���� ÿ���޹� /////////////////////////////////////////////////////////
static const int RAND_ACTIVITY_DAILY_LIMIT_BUY_MAX_SEQ = 4;

//////////////////////////////////////////////// ���� �۱��� /////////////////////////////////////////////////////////
static const int RAND_ACTIVITY_COLLECT_TREASURE_MAX_RECORD_COUNT = 10;

struct CollectTreasureRollRecord
{
	CollectTreasureRollRecord() : role_id(0), roll_mul(0)
	{
		memset(role_name, 0, sizeof(role_name));
	}

	int role_id;
	GameName role_name;
	int roll_mul;     //����
};

////////////////////////////////////////////////// ����-��ʱ���� ////////////////////////////////////////////////
static const int RA_LIMIT_TIME_REBATE_MAX_DAY = 9;

////////////////////////////////////////////////// ����-�����̵� ////////////////////////////////////////////////
static constexpr int RA_RMB_BUY_CHEST_SHOP_MAX_BUY_COUNT = 64;

////////////////////////////////////////////////���±��//////////////////////////////////////////
static const int RAND_ACTIVITY_ITEM_COLLECTION_SECOND_REWARD_MAX_COUNT = 5;	// ���±������������

////////////////////////////////////////////////// ����-���ѻ����� ////////////////////////////////////////////////
static const int RA_CONSUME_FOR_GIFT_MAX_ITEM_COUNT = 20;				// ���һ���Ʒ��

/////////////////////////////////////////////////  ����-�ؾ�Ѱ��3/�����ҵ�/����ҡ��  /////////////////////////////
enum RA_MIJINGXUNBAO3_CHOU_TYPE							//�Ա�����
{
	RA_MIJINGXUNBAO3_CHOU_TYPE_1 = 0,					//�Ա�һ��
	RA_MIJINGXUNBAO3_CHOU_TYPE_10,						//�Ա�ʮ��						
	RA_MIJINGXUNBAO3_CHOU_TYPE_30,						//�Ա���ʮ��

	RA_MIJINGXUNBAO3_CHOU_TYPE_MAX,
};

/////////////////////////////////////////////////  ����-���ʳ�ֵ  /////////////////////////////
static const int RAND_ACTIVITY_SINGLE_CHARGE_CONFIG_MAX_COUNT = 10;							// ���ʳ�ֵ���������

////////////////////////////////////////////////// ����-������ս ////////////////////////////////////////////////
static const int RA_EXREME_CHALLENGE_PERSON_TASK_MAX_NUM = 5;			// ���������������		

enum RA_EXREME_CHALLENGE_TASK_TYPE
{
	RA_EXTREME_CHALLENGE_ACTIVE,					// ��Ծ����
	RA_EXTREME_CHALLENGE_KILL_MONSTER,				// ɱ������
	RA_EXTREME_CHALLENGE_ON_LINE,					// ��������
	RA_EXTREME_CHALLENGE_KILL_BOSS,					// BOSS����
	RA_EXTREME_CHALLENGE_REPUTATION,				// ��������
	RA_EXTREME_CHALLENGE_COMPOUND,					// �ϳ�����

	RA_EXTREME_CHALLENGE_MAX_NUM,
};

struct RAExremeChallengeTaskInfo
{
	RAExremeChallengeTaskInfo() { this->Reset(); }

	void Reset()
	{
		task_id = 0;
		task_type = -1;
		is_finish = 0;
		is_already_fetch = 0;
		task_plan = 0;
	}

	char task_id;				// ����id
	char task_type;				// ��������
	char is_finish;				// �Ƿ����
	char is_already_fetch;		// �Ƿ���ȡ
	unsigned int task_plan;		// �������
};

/////////////////////////////////////	��Ϲ���		/////////////////////////
static const int RA_COMBINE_BUY_BUCKET_ITEM_COUNT = 6;				// ���ﳵ�������
static const int RA_COMBINE_BUY_MAX_ITEM_COUNT = 96;				// ������Ʒ������
static const int RA_COMBINE_BUY_LIMIT_BUY_ITEM_COUNT = 255;			// �޹���Ʒ����

//////////////////////////////////////////////// ��һ� ////////////////////////////////////////////////
enum ACTIVITY_FIND_TYPE
{
	ACTIVITY_FIND_TYPE_GONGCHENGZHAN = 0,				// ����ս 6
	ACTIVITY_FIND_TYPE_TIANJIANGCAIBAO = 1,				// �콵�Ʊ�
	ACTIVITY_FIND_TYPE_QUESTION_2 = 2,					// ����-�°�
	ACTIVITY_FIND_TYPE_MILLIONAIRE = 3,					// �󸻺�	25
	ACTIVITY_FIND_TYPE_WANGLINGEXPLORE = 4,				// ����̽��	26
	ACTIVITY_FIND_TYPE_QUNXIANLUANDOU = 5,				// Ԫ��ս��	5
	ACTIVITY_FIND_TYPE_GUILDBATTLE = 6,					// ��������	21
	ACTIVITY_FIND_TYPE_TERRITORYWAR = 7,				// ����ս
	ACTIVITY_FIND_TYPE_FISHING = 8,						// ����		3084
	ACTIVITY_FIND_TYPE_BANGPAIDATI = 9,					// ���ɴ���	30
	ACTIVITY_FIND_TYPE_SHUIJING = 10,					// ˮ��		14
	ACTIVITY_FIND_TYPE_WORSHIP = 11,					// Ĥ�ݳ���	32
	ACTIVITY_FIND_TYPE_NIGHT_FIGHT_FB = 12,				// ҹս����	23
	ACTIVITY_FIND_TYPE_MESS_BATTLE_FB = 13,				// �Ҷ�ս��	31
	ACTIVITY_FIND_TYPE_GUILD_YUNBIAO = 14,				// ��������	27
	ACTIVITY_FIND_TYPE_GUILD_FB = 15,					// ��������  34
	ACTIVITY_FIND_TYPE_GUILD_YANHUI = 16,				// �������	33
	ACTIVITY_FIND_TYPE_CROSS_1V1 = 17,					// ���1v1 3074
	ACTIVITY_FIND_TYPE_CROSS_MULTIUSER_CHALLENGE = 18,	// ���3V3 3075
	ACTIVITY_FIND_TYPE_HOT_SPRING = 19,					// �����Ȫ 3080
	ACTIVITY_FIND_TYPE_CROSS_SHUIJING = 20,				// ���ˮ��	3081
	ACTIVITY_FIND_TYPE_CROSS_GUILDBATTLE = 21,			// �������ս  3082
	ACTIVITY_FIND_TYPE_DARK_NIGHT = 22,					// ����ºڷ�� 3083
	ACTIVITY_FIND_TYPE_CROSS_NIGHT_FIGHT_FB = 23,		// ���ҹս���� 3085
	ACTIVITY_FIND_TYPE_CROSS_MESS_BATTLE_FB = 24,		// ����Ҷ�ս��	3086
	ACTIVITY_FIND_TYPE_LIEKUN_FB = 25,					// ����ش�	3087
	ACTIVITY_FIND_TYPE_CHU_MO = 26,						// �����ħ
	ACTIVITY_FIND_TYPE_XIULUOTOWER = 27,				// ���������

	ACTIVITY_FIND_TYPE_MAX,
};

/////////////////////////////////////	���췵��		/////////////////////////
static const int DAY_CHONGZHI_REWARD_FLAG_LIST_LEN = 4;

static const int ACTIVITY_FIND_CONFIG_LEVEL_INTERVAL_COUNT = 200;	// ��һ����� ���ȼ��������
static const int ACTIVITY_FIND_CONFIG_REWARD_ITEM_MAX_COUNT = 3;	// ��һ����� �������Ʒ��
static const int ACTIVITY_OPEN_RECORD_MAX_COUNT = 3;				// �������¼�������

struct ActivityOpenRecords											// �������¼
{
	ActivityOpenRecords() 
	{
		this->Reset();
	}

	void Reset()
	{
		memset(record_list, 0, sizeof(record_list));
	}

	void OnResetData()
	{
		for (int i = ACTIVITY_OPEN_RECORD_MAX_COUNT - 1; i > 0 ; -- i)
		{
			record_list[i] = record_list[i - 1];
		}

		record_list[0] = 0;
	}

	bool IsActivityOpen(int pass_day, int activity_find_type)
	{
		if (pass_day < 0 || pass_day >= ACTIVITY_OPEN_RECORD_MAX_COUNT || activity_find_type < 0 || activity_find_type >= (int)sizeof(record_list[0]) * 8)
		{
			return false;
		}

		bool is_open = 0 != (record_list[pass_day] & (1 << activity_find_type));

		return is_open;
	}

	int record_list[ACTIVITY_OPEN_RECORD_MAX_COUNT];
};

//////////////////////////////////////////////// ������ս� ////////////////////////////////////////////////
static const int SEVEN_DAY_CHALLENGE_SUB_ACTIVITY_REWARD_CFG_MAX_COUNT  = 10;	// ������ս�ӻ�����õ��������

enum SEVEN_DAY_CHALLENGE_SUB_TYPE
{
	SDCST_INVALID = 0,
	SDCST_XUYUTIAOZHAN,
	SDCST_JINGYINGXUYU,
	SDCST_SHENMOZHIJING,
	SDCST_JINGYINGSHENMO,
	SDCST_XIANHUNTASHEN,
	SDCST_BOSSTIAOZHAN,
	SDCST_XIEMEITONGYOU,

	SDCST_MAX
};

///////////////////////////////////////////////////////////  ����ת�����  /////////////////////////////////////////////////////////////
const static int LUCKY_ROLL_REWARD_RECORD_MAX_COUNT = 10;

enum LUCKY_ROLL_REWARD_TYPE
{
	LUCKY_ROLL_REWARD_TYPE_INVALID = 0,
	LUCKY_ROLL_REWARD_TYPE_ITEM, 
	LUCKY_ROLL_REWARD_TYPE_GOLD, 

	LUCKY_ROLL_REWARD_TYPE_MAX,
};
struct LuckyRollRewardRecord										// ����ת�̻񽱼�¼
{
	LuckyRollRewardRecord() { this->Reset(); }

	void Reset() 
	{ 
		user_id = 0; 
		memset(user_name, 0, sizeof(user_name));
		reward_type = LUCKY_ROLL_REWARD_TYPE_INVALID; 
		num = 0; 
		item_id = 0; 
		gold = 0; 
		time_stamp = 0; 
	}

	int user_id;
	GameName user_name;
	char reward_type;
	char num;
	ItemID item_id;
	int gold;
	unsigned int time_stamp;
};

/////////////////////////////////////////////////////////// ��ʱ�һ�װ��� //////////////////////////////////////////////////////////
static const int TIME_LIMIT_EXCHANGE_ITEM_COUNT = 10;

///////////////////////////////////////////////////////////  ������  /////////////////////////////////////////////////////////////
const static int CLOSE_BETA_CHALLENGE_FIELD_REWARD_TIMES = 2;		// ����ڼ䣬���������⽱������


//////////////////////////////////////////////// һվ���� ��ض��� ////////////////////////////////////////////////

static const int YIZHANDAODI_REALIVE_POS_MAX = 16;								// ��������
static const int YIZHANDAODI_JISHA_REWARD_NUM = 16;								// ��ɱ��������
static const int YIZHANDAODI_JISHA_TITLE_MAX = 16;								// ��ɱ�ƺ�����
static const int YIZHANDAODI_LUACY_ROLE_MAX = 5;								// ������Ҹ���
static const int YIZHANDAODI_RANK_NUM = 100;									// һվ���� ʵʱ���а���Ϣ����
static const int YIZHANDAODI_JISHA_SCORE_REWARD_MAX = 16;						// һվ���� ��ɱ���ֶһ�����������
static const int YIZHANDAODI_REWARD_TITLE_RANK_COUNT = 3;						// һս���ף�ǰn����ȡ�ƺŽ���
UNSTD_STATIC_CHECK(YIZHANDAODI_REWARD_TITLE_RANK_COUNT <= YIZHANDAODI_RANK_NUM);


struct YiZhanDaoDiUser
{
	YiZhanDaoDiUser() 
		: user_id(INVALID_USER_ID),
		jisha_count(0), jisha_title_id(0), dead_count(0), lucky_count(0), jisha_score(0), gongji_guwu_per(0), maxhp_guwu_per(0),lianzhan(0),
		assist(0),uid(0), kill_num_reward_flag(0), hp_when_leave(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UserID user_id;
	GameName user_name;

	int jisha_count;
	int jisha_title_id;
	int dead_count;
	int lucky_count;
	int jisha_score;

	int gongji_guwu_per;
	int maxhp_guwu_per;
	int lianzhan;
	int assist;
	int uid;
	int kill_num_reward_flag;
	Attribute hp_when_leave;
	std::vector<ItemConfigData> reward_item_vec;
	std::map<UserID, time_t> hurt_map;
};

typedef std::map<UserID, YiZhanDaoDiUser> YiZhanDaoDiUserMap;
typedef std::map<UserID, YiZhanDaoDiUser>::iterator YiZhanDaoDiUserMapIt;

struct YiZhanDaoDiUserRankItem
{
	YiZhanDaoDiUserRankItem() : user_id(INVALID_USER_ID), jisha_count(0), dead_count(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UserID user_id;	
	GameName user_name;																// ��ɫ��
	short jisha_count;																// ��ɫ��ս���е���Ӫ
	short dead_count;																// նɱ����
};

//////////////////////////////////////////////// ���йһ� ��ض��� ////////////////////////////////////////////////
static const int WUXINGGUAJI_STUFF_MAX = 5;											// ���ϸ���
static const int WUXINGGUAJI_TARGET_MAX = 5;										// Ŀ�����
static const int WUXINGGUAJI_MAX_RELACE_BOSS_NUM = 10;								// ������ȼ��滻BOSS���������
static const int WUXINGGUAJI_BOSS_NUM = 1;											// BOSS���������
static const int WUXINGGUAJI_FLUSH_BOSS_INTERVAL = 4 * 60;							// BOSSˢ�¼��
static const int WUXINGGUAJI_LUACY_ROLE_MAX = 5;									// ������Ҹ���

struct WuXingGuaJiTargetInfo
{
	WuXingGuaJiTargetInfo() : is_finish(0), is_getreward(0) 
	{
		memset(stuff_id_list, 0, sizeof(stuff_id_list));
		memset(stuff_num_list, 0, sizeof(stuff_num_list));
	}

	bool IsFinish() { return 0 != is_finish; }
	bool IsGetReward() { return 0 != is_getreward; }

	int stuff_id_list[WUXINGGUAJI_STUFF_MAX];
	int stuff_num_list[WUXINGGUAJI_STUFF_MAX];
	short is_finish;
	short is_getreward;
};

struct WuXingGuaJiUser
{
	WuXingGuaJiUser() 
		: user_id(INVALID_USER_ID), lucky_count(0)
	{
	}

	UserID user_id;
	int lucky_count;
	GSNetID net_id;

	WuXingGuaJiTargetInfo target_list[WUXINGGUAJI_TARGET_MAX];
};

typedef std::map<UserID, WuXingGuaJiUser> WuXingGuaJiUserMap;
typedef std::map<UserID, WuXingGuaJiUser>::iterator WuXingGuaJiUserMapIt;

//////////////////////////////////////////////// ����-�۱��� ��ض��� ////////////////////////////////////////////////

struct RACornucopiaRewardRecord										// �۱����¼
{
	RACornucopiaRewardRecord() { this->Reset(); }

	void Reset() 
	{ 
		user_id = 0; 
		memset(user_name, 0, sizeof(user_name));
		reward_gold = 0;
		camp = 0;
		reserve_ch = 0;
		need_put_gold = 0;
	}

	int user_id;
	GameName user_name;
	short reward_gold;
	char camp;
	char reserve_ch;
	int need_put_gold;
};

const static int RAND_ACTIVITY_CORNUCOPIA_RECORD_MAX_COUNT = 100;	// �۱����¼�������

//////////////////////////////////////////////// �Ϸ�� ////////////////////////////////////////////////
static const int COMBINE_SERVER_ACTIVITY_RANK_REWARD_ROLE_NUM = 3;		// �������� ��������
static const int COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE = 3;			// ����������Ʒ����
static const int COMBINE_SERVER_ROLL_CFG_MAX_COUNT = 16;				// ҡ�����������
static const int COMBINE_SERVER_SINGLE_CHARGE_CONFIG_MAX_COUNT = 10;	// ���ʳ�ֵ���������
static const int COMBINE_SERVER_LOGIN_GIFT_REWARD_CONFIG_MAX_COUNT = 32;// ��¼������������ø���
static const int COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT = 16;	// ȫ��������Ʒ��������
static const int COMBINE_SERVER_PERSONAL_PANIC_BUY_ITEM_MAX_COUNT = 8;	// ����������Ʒ��������
static const int COMBINE_SERVER_BOSS_RANK_NUM = 10;						// �Ϸ�boss���а��ϰ���
static const int COMBINE_SERVER_BOSS_MAX_COUNT = 15;					// �Ϸ�boss�������
static const int COMBINE_SERVER_CONSUME_RANK_COUNT = 10;				// �Ϸ�������������(��ʱ�����Ѻͳ�ֵ��������һ��,�����������������)
static const int COMBINE_SERVER_CHONGZHI_RANK_COUNT = 10;				// �Ϸ���ֵ��������
static const int COMBINE_SERVER_MAX_FOUNDATION_TYPE = 10;				// �ɳ������������
static const int COMBINE_SERVER_MAX_LOGIN_GIFT_NUM = 20;				// ��¼�����������

struct CSAFoundationStatus
{
	CSAFoundationStatus() : buy_level(0), reward_phase(0)
	{}

	short buy_level;
	short reward_phase;
};

enum COMBINE_SERVER_ACTIVITY_SUB_ACTIVITY_STATE
{
	CSA_SUB_ACTIVITY_STATE_NO_START = 0,								// δ��ʼ
	CSA_SUB_ACTIVITY_STATE_OPEN,										// ������
	CSA_SUB_ACTIVITY_STATE_FINISH,										// ����
};

enum COMBINE_SERVER_ACTIVITY_SUB_TYPE									// �Ϸ��������
{
	CSA_SUB_TYPE_INVALID = 0,
	CSA_SUB_TYPE_RANK_QIANGGOU,											// ����
	CSA_SUB_TYPE_ROLL,													// ת��
	CSA_SUB_TYPE_GONGCHENGZHAN,											// ����ս
	CSA_SUB_TYPE_XIANMENGZHAN,											// ����ս
	CSA_SUB_TYPE_CHONGZHI_RANK,											// ��ֵ����
	CSA_SUB_TYPE_CONSUME_RANK,											// ��������
	CSA_SUB_TYPE_KILL_BOSS,												// ��ɱboss
	CSA_SUB_TYPE_SINGLE_CHARGE,											// ���ʳ�ֵ
	CSA_SUB_TYPE_LOGIN_Gift,											// ��¼����
	CSA_SUB_TYPE_PERSONAL_PANIC_BUY,									// ��������
	CSA_SUB_TYPE_SERVER_PANIC_BUY,										// ȫ������
	CSA_SUB_TYPE_ZHANCHANG_FANBEI,										// ս������
	CSA_SUB_TYPE_CHARGE_REWARD_DOUBLE,									// ��ֵ˫������
	CSA_SUB_TYPE_BOSS,													// �Ϸ�boss
	CSA_SUB_TYPE_TOUZIJIHUA,											// �Ϸ�Ͷ�ʼƻ�
	CSA_SUB_TYPE_FOUNDATION,											// �Ϸ��ɳ�����
	CSA_SUB_TYPE_BUY_EXP,												// �Ϸ���������

	CSA_SUB_TYPE_MAX,
};

enum COMBINE_SERVER_ACTIVITY_RANK_TYPE
{
	CSA_RANK_TYPE_INVALID = 0,
	CSA_RANK_TYPE_QIANGGOU,
	CSA_RANK_TYPE_CHONGZHI,
	CSA_RANK_TYPE_CONSUME,

	CSA_RANK_TYPE_MAX,
};

struct CombineServerActivityOtherParam
{
	CombineServerActivityOtherParam() { this->Reset(); }

	void Reset()
	{
		memset(rank_qianggou_buynum_list, 0, sizeof(rank_qianggou_buynum_list));

		reserve_int = 0;
		csa_xmz_winner_roleid = 0;
		csa_gcz_winner_roleid = 0;

		memset(server_panic_buy_num_list, 0, sizeof(server_panic_buy_num_list));
		cas_gcz_winner_lover_id = 0;
	}

	UNSTD_STATIC_CHECK(3 == COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE);

	int rank_qianggou_buynum_list[COMBINE_SERVER_RANK_QIANGOU_ITEM_MAX_TYPE];
	int reserve_int;

	int csa_xmz_winner_roleid;
	int csa_gcz_winner_roleid;

	UNSTD_STATIC_CHECK(16 == COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT);

	int server_panic_buy_num_list[COMBINE_SERVER_SERVER_PANIC_BUY_ITEM_MAX_COUNT];
	int cas_gcz_winner_lover_id;
};

struct CombineServerActivityRankUser
{
	CombineServerActivityRankUser() { this->Reset(); }

	void Reset() 
	{
		role_id = 0;
		rank_value = 0;
		rank_reserve = 0;
	}

	bool IsFree() { return 0 == role_id; }

	int role_id;
	long long rank_value;
	long long rank_reserve;
};

struct CombineServerActivityRankItem
{
	CombineServerActivityRankItem() { this->Reset(); }

	void Reset() 
	{
		is_finish = 0;
		reserve_ch = 0;
		reserve_sh = 0;

		for (int i = 0; i < COMBINE_SERVER_CONSUME_RANK_COUNT; ++ i)
		{
			user_list[i].Reset();
		}
	}

	char is_finish;
	char reserve_ch;
	short reserve_sh;

	CombineServerActivityRankUser user_list[COMBINE_SERVER_CONSUME_RANK_COUNT];
};

struct CombineServerActivityRankParam
{
	CombineServerActivityRankParam() { this->Reset(); }

	void Reset()
	{
		for (int i = 0; i < CSA_RANK_TYPE_MAX; ++ i)
		{
			rank_item_list[i].Reset();
		}
	}

	CombineServerActivityRankItem rank_item_list[CSA_RANK_TYPE_MAX];
};

struct CombineServerActivityBossPersonalRankItem
{
	CombineServerActivityBossPersonalRankItem()
	{
		this->Reset();
	}

	void Reset()
	{
		id = 0;
		rank_value = 0;
	}

	int id;
	int rank_value;
};

struct CombineServerActivityBossGuildRankItem
{
	CombineServerActivityBossGuildRankItem()
	{
		this->Reset();
	}

	void Reset()
	{
		id = 0;
		rank_value = 0;
	}

	int id;
	int rank_value;
};

struct CombineServerActivityBossRankParam
{
	CombineServerActivityBossRankParam()
	{
		this->Reset();
	}

	void Reset()
	{
		for (int i = 0; i < COMBINE_SERVER_BOSS_RANK_NUM; i++)
		{
			personal_rank[i].Reset();
			guild_rank[i].Reset();
		}
	}

	CombineServerActivityBossPersonalRankItem personal_rank[COMBINE_SERVER_BOSS_RANK_NUM];
	CombineServerActivityBossGuildRankItem guild_rank[COMBINE_SERVER_BOSS_RANK_NUM];
};

//////////////////////////////////////////////// ���ɴ��� ��ض���////////////////////////////////////////////////
static const int GUILD_QUESTION_SCORE_RANK_MAX = 10;		// ���ɴ�������������

static const int GUILD_QUESTION_STRING_MAX = 256;											// ��Ŀ����ַ�		
typedef char GuildQuestionStr[GUILD_QUESTION_STRING_MAX];									// ��Ŀ����

enum GUILD_QUESTION_STATE
{
	GUILD_QUESTION_STATE_STANDY = 0,	// ׼��
	GUILD_QUESTION_STATE_START,			// ��ʼ����
	GUILD_QUESTION_STATE_CLOSE,			// ��Ҫ�رգ�Ҳ���ڴ��⣩
	GUILD_QUESTION_STATE_MAX,
};

struct GuildQuestionPlayerInfo
{
	GuildQuestionPlayerInfo() : user_id(INVALID_USER_ID), right_num(0), exp(0), guild_gongxian(0), gather_count(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	UserID user_id;
	GameName user_name;

	int right_num;			// ������ȷ��
	long long exp;			// �����þ���
	int guild_gongxian;		// Ϊ���ɻ�ù���
	int gather_count;		// �ɼ�����
};

typedef std::map<UserID, GuildQuestionPlayerInfo> GuildQuestionPlayerInfoMap;
typedef std::map<UserID, GuildQuestionPlayerInfo>::iterator GuildQuestionPlayerInfoMapIt;

struct GuildQuestionPersonRank
{
	GuildQuestionPersonRank() : user_id(0), right_num(0)
	{
		memset(user_name, 0, sizeof(user_name));
		memset(guild_name, 0, sizeof(guild_name));
	}

	bool operator <(const GuildQuestionPersonRank &other) const
	{
		return right_num > other.right_num;
	}

	int user_id;
	GameName user_name;					// �������
	GuildName guild_name;				// ��������

	int right_num;			// ������ȷ��
};

// ������ȷ����������
typedef std::vector<GuildQuestionPersonRank> GuilldQuestionPersonRankVec;
typedef std::vector<GuildQuestionPersonRank>::iterator GuilldQuestionPersonRankVecIt;

struct GuildQuestionGuildRank
{
	GuildQuestionGuildRank() : guild_id(0), answer_true_timestamp(0), guild_score(0)
	{
		memset(guild_name, 0, sizeof(guild_name));
	}

	GuildID guild_id;					// ����ID
	GuildName guild_name;				// ��������
	unsigned int answer_true_timestamp;	// ���ʱ���
	int guild_score;					// ���ɻ���
};

typedef std::vector<GuildQuestionGuildRank> GuildQuestionGuildRankVec;
typedef std::vector<GuildQuestionGuildRank>::iterator GuildQuestionGuildRankVecIt;

struct GuildQuestionQuestion
{
	GuildQuestionQuestion() : guild_id(0), question_number(0), question_right_num(0), question_id(-1), cur_question_timestamp(0), next_question_timestamp(0), is_aready_broadcast(false), is_already_answer_true(false) {}

	GuildID guild_id;					// ����ID
	int question_number;
	int question_right_num;				// �����Ŀ����
	int question_id;
	std::set<int> question_set;			// ��Ŀ����
	time_t cur_question_timestamp;		// ��ǰ�����ʱ���
	time_t next_question_timestamp;		// ������ʱ���
	bool is_aready_broadcast;			// �Ƿ��Ѿ��㲥
	bool is_already_answer_true;
};
typedef std::map<GuildID, GuildQuestionQuestion> GuildQuestionQuestionMap;
typedef std::map<GuildID, GuildQuestionQuestion>::iterator GuildQuestionQuestionMapIt;

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

////////////////////////////////////////////////// ����-�������齱� ////////////////////////////////////////////////
static const int RA_LEVEL_LOTTERY_MAX_LEVEL_LIMIT = 6;							// ����-�������齱������Ĳ�������
static const int RA_LEVEL_LOTTERY_MAX_REWARD_ID_LIMIT = 27;						// ����-�������齱������Ľ���ID����
static const int RA_LEVEL_LOTTERY_REWARD_MAX_COUNT = 52;						// �齱����������
static const int RA_LEVEL_LOTTERY_HISTORY_MAX_COUNT = 10;						// �齱��ʷ��¼�������

struct RALevelLotteryHistoryItem
{
	RALevelLotteryHistoryItem() : uid(0), reward_index(-1) { memset(user_name, 0, sizeof(user_name)); }

	GameName user_name;
	int uid;
	int reward_index; // ��ҳ�ȡ���Ľ���id
};

////////////////////////////////////////////////// ����-��ֵŤ��� ////////////////////////////////////////////////
static const int RA_NIU_EGG_CHOU_REWARD_CFG_COUNT_LIMIT = 26;					// Ť���齱��Ʒ��������
static const int RA_NIU_EGG_SERVER_REWARD_CFG_COUNT_LIMIT = 6;					// Ť��ȫ��������Ʒ�б�
static const int RA_NIU_EGG_CHOU_HISTORY_MAX_COUNT = 10;						// �齱��ʷ��¼�������

struct RAChongzhiNiuEggHistoryItem
{
	RAChongzhiNiuEggHistoryItem() { this->Reset(); }
	
	void Reset()
	{
		uid = 0;
		reward_req = -1;
		memset(user_name, 0, sizeof(user_name));
	}
		
	GameName user_name;
	int uid;
	int reward_req; // ��ҳ�ȡ���Ľ����±�
};

////////////////////////////////////////////////// �䱦�� ////////////////////////////////////////////////
static const int RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT = 9;				// ��ʾ������Ʒ�б�����
static const int RAND_ACTIVITY_ZHENBAOGE_ADD_WEIGHT_COUNT = 5;			// �����������Ȩ������

UNSTD_STATIC_CHECK((RAND_ACTIVITY_ZHENBAOGE_ITEM_COUNT + 1) % 2 == 0);

struct ZhenbaogeAddWeightInfo
{
	ZhenbaogeAddWeightInfo() : seq(0), weight(0){}

	short seq;
	short weight;
};

////////////////////////////////////////////////// �ؾ�Ѱ��� ///////////////////////////////////////////////
static const int RAND_ACTIVITY_MIJINGXUNBAO_ADD_WEIGHT_COUNT = 10;			// �����������Ȩ������

struct MiJingXunBaoAddWeightInfo
{
	MiJingXunBaoAddWeightInfo() : seq(0), weight(0), tao_none_rare_times(0){}

	short seq;
	short weight;
	int tao_none_rare_times;
};

enum RA_MIJINGXUNBAO_CHOU_TYPE							//�Ա�����
{
	RA_MIJINGXUNBAO_CHOU_TYPE_1 = 0,					//�Ա�һ��
	RA_MIJINGXUNBAO_CHOU_TYPE_10,						//�Ա�ʮ��						
	RA_MIJINGXUNBAO_CHOU_TYPE_50,						//�Ա���ʮ��

	RA_MIJINGXUNBAO_CHOU_TYPE_MAX,
};

////////////////////////////////////////////////// ����-ҡǮ��� ////////////////////////////////////////////////
static const int RA_MONEY_TREE_REWARD_CFG_COUNT = 40;						// ҡǮ����Ʒ��������
static const int RA_MONEY_TREE_SERVER_REWARD_CFG_COUNT = 6;					// ҡǮ��ȫ��������Ʒ�б�

////////////////////////////////////////////////// ����-�����뷭�ƻ ////////////////////////////////////////////////
static const int RA_KING_DRAW_LEVEL_COUNT = 3;									// �������ȼ��ķ��ƣ��������м����߼�
static const int RA_KING_DRAW_MAX_REWARD_COUNT = 50;							// ����Ľ������������ͻ��ˣ�
UNSTD_STATIC_CHECK(RA_KING_DRAW_MAX_REWARD_COUNT <= MAX_ROLE_CHEST_GRID);
static const int RA_KING_DRAW_MAX_SHOWED_COUNT = 9;								// ���ʾ�Ŀɷ�����
static const int RA_KING_DRAW_MAX_SELF_WEIGTH_COUNT = 5;						// ��������Ȩ�صĽ���������
static const int RA_KING_DRAW_MAX_RETURN_REWARD = 10;							// �۳齱�������Ŀ
UNSTD_STATIC_CHECK(1 == RA_KING_DRAW_MAX_SHOWED_COUNT % 2);

// ĳһ�ȼ��ķ�����Ϣ
struct RAKingDrawCardList
{
	RAKingDrawCardList() { this->Reset(); }

	void Reset()
	{
		reserve_sh = 0;

		for (int i = 0; i < RA_KING_DRAW_MAX_SHOWED_COUNT; ++i)
		{
			reward_list[i] = -1;										// -1����û����
		}
	}

	short reward_list[RA_KING_DRAW_MAX_SHOWED_COUNT];					// ֵ�ǽ���seq
	short reserve_sh;
};

// ĳһ�ȼ��ķ��Ƶ�����Ȩ�����
struct RAKingDrawSelfWeight
{
	RAKingDrawSelfWeight() { this->Reset(); }

	void Reset()
	{
		seq = -1;
		auto_weight_add = 0;
	}

	short seq;															// ������seq
	short auto_weight_add;												// ��ǰ��weight����ֵ
};


////////////////////////////////////////////////// �������� ////////////////////////////////////////////////

static const int RA_SPECIAL_APPEARANCE_RANK_MAX_COUNT = 10;						// ����-����������������������

struct RASpecialAppearacePlayerRankInfo
{
	RASpecialAppearacePlayerRankInfo() : uid(0), change_num(0), total_capablity(0), avatar(0), sex(0), prof(0), reserve_1(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	int uid;
	GameName user_name;
	int change_num;
	int total_capablity;
	char avatar;
	char sex;
	char prof;
	char reserve_1;
};

static const int RA_SPECIAL_APPEARANCE_PASSIVE_RANK_MAX_COUNT = 10;						// ����-�������󱻱��������������

struct RASpecialAppearacePassivePlayerRankInfo
{
	RASpecialAppearacePassivePlayerRankInfo() : uid(0), change_num(0), total_capablity(0) , avatar(0), sex(0), prof(0), reserve_1(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	int uid;
	GameName user_name;
	int change_num;
	int total_capablity;
	char avatar;
	char sex;
	char prof;
	char reserve_1;
};

////////////////////////////////////////////////// ����-�ڿ� ////////////////////////////////////////////////
static const int RA_MINE_MAX_REFRESH_COUNT = 8;							// һ��ˢ�³�������ʯ��Ŀ
static const int RA_MINE_MAX_TYPE_COUNT = 12;							// ��ʯ���������Ŀ
static const int RA_MINE_REWARD_MAX_COUNT = 900;						// ȫ����������
static const int RA_MINE_REFRESH_TIME_INTERVAL = SECOND_PER_HOUR * 4;	// ϵͳ�Զ�ˢ��ʱ����
UNSTD_STATIC_CHECK(0 == RA_MINE_MAX_TYPE_COUNT % 2)
static const int RA_MINE_MAX_CFG_COUNT = 900;							// �����������

enum RA_MINE_TYPES
{
	RA_MINE_TYPES_INVALID = 0,

	RA_MINE_TYPES_BEGIN = 10,

	RA_MINE_TYPES_END = RA_MINE_TYPES_BEGIN,
};

////////////////////////////////////////////////// ����-���ιλ ////////////////////////////////////////////////
static const int RA_GUAGUA_MAX_REWARD_COUNT = 50;						// ��ཱ����Ŀ
static const int RA_GUAGUA_ICON_TYPE_COUNT = 10;						// ͼ��������Ŀ
static const int RA_GUAGUA_REWARD_AREA_COUNT = 5;						// �ν�������Ŀ
static const int RA_GUAGUA_REWARD_COUNT_PER_AREA = 3;					// �����ν�����Ľ�����
static const int RA_GUAGUA_ACC_REWARD_COUNT = 16;						// �ۼƳ齱�Ľ�����

enum RA_GUAGUA_PLAY_MULTI_TYPES											//�ν���ε�����
{
	RA_GUAGUA_PLAY_ONE_TIME = 0,										// �ν�1��
	RA_GUAGUA_PLAY_TEN_TIMES = 1,										// �ν�10��
	RA_GUAGUA_PLAY_THIRTY_TIMES = 2,									// �ν�30��

	RA_GUAGUA_TYPE_MAX
};

////////////////////////////////////////////////// ����-�������Ի ////////////////////////////////////////////////
static const int RA_TIANMING_LOT_COUNT = 6;								// �ɼ�ע��ǩ����
static const int RA_TIANMING_REWARD_HISTORY_COUNT = 20;					// ������ʷ��¼����

struct RATianMingRewardHistoryItem
{
	RATianMingRewardHistoryItem() { this->Reset(); }

	void Reset()
	{
		seq = 0;
		add_lot = 0;
	}

	char seq;															// �����±�
	char add_lot;														// �񽱼�ע����
};

////////////////////////////////////////////////// ����-����ת� ////////////////////////////////////////////////
static const int RA_FANFAN_MAX_ITEM_COUNT = 50;						// ���������
static const int RA_FANFAN_MAX_WORD_COUNT = 10;						// �����������
static const int RA_FANFAN_CARD_COUNT = 40;							// �ɷ�����
static const int RA_FANFAN_LETTER_COUNT_PER_WORD = 4;				// ÿ�����������
static const int RA_FANFAN_BAODI_REWARD_CFG_COUNT = 10;				// �۳鷵�������������
UNSTD_STATIC_CHECK(0 == RA_FANFAN_MAX_WORD_COUNT % 2)
UNSTD_STATIC_CHECK(RA_FANFAN_CARD_COUNT == 40)

enum RA_FANFAN_CARD_TYPE
{
	RA_FANFAN_CARD_TYPE_BEGIN,

	RA_FANFAN_CARD_TYPE_HIDDEN = RA_FANFAN_CARD_TYPE_BEGIN,			// ���ؿ�������
	RA_FANFAN_CARD_TYPE_ITEM_BEGIN = 100,							// ��Ʒ����������ʼֵ
	RA_FANFAN_CARD_TYPE_WORD_BEGIN = 200,							// ���鿨��������ʼֵ

	RA_FANFAN_CARD_TYPE_MAX,
};
UNSTD_STATIC_CHECK(RA_FANFAN_CARD_TYPE_WORD_BEGIN - RA_FANFAN_CARD_TYPE_ITEM_BEGIN >= RA_FANFAN_MAX_ITEM_COUNT);
UNSTD_STATIC_CHECK(UCHAR_MAX - RA_FANFAN_CARD_TYPE_WORD_BEGIN >= RA_FANFAN_MAX_WORD_COUNT);

struct RAFanFanHiddenWordInfo
{
	static const int POS_BEGIN = 100;

	RAFanFanHiddenWordInfo() { this->Reset(); }

	void Reset()
	{
		hidden_word_seq = 0;
		memset(reserve, 0, sizeof(reserve));
		memset(hidden_letter_pos_list, 0, sizeof(hidden_letter_pos_list));
	}

	bool IsValid()
	{
		int hidden_word_real_seq = hidden_word_seq - RA_FANFAN_CARD_TYPE_WORD_BEGIN;

		return (hidden_word_real_seq >= 0 && hidden_word_real_seq < RA_FANFAN_MAX_WORD_COUNT);
	}

	int GetHiddenLetterCount()
	{
		int hidden_letter_count = 0;
		for (int i = 0; i < RA_FANFAN_LETTER_COUNT_PER_WORD; ++ i)
		{
			if (0 == hidden_letter_pos_list[i])
			{
				++ hidden_letter_count;
			}
		}

		return hidden_letter_count;
	}

	unsigned char hidden_word_seq;				// ��Ӧ����seq
	char reserve[3];
	unsigned char hidden_letter_pos_list[RA_FANFAN_LETTER_COUNT_PER_WORD];
};

struct RAFanFanWordActiveInfo
{
	static const int MAX_WORD_ACTIVE_COUNT = 99;

	RAFanFanWordActiveInfo() { this->Reset(); }

	void Reset()
	{
		active_flag = 0;
		active_count = 0;
	}

	int ActiveWord()
	{
		int active_bit = 0;
		for (int bit = 0; bit < RA_FANFAN_LETTER_COUNT_PER_WORD; ++ bit)
		{
			if (0 == ((1 << bit) & active_flag))
			{
				active_bit = bit;
				active_flag |= (1 << active_bit);

				break;
			}
		}

		UNSTD_STATIC_CHECK(RA_FANFAN_LETTER_COUNT_PER_WORD == 4);
		if (active_flag >= 0x0f)
		{
			active_flag = 0;
			++ active_count;
		}

		return active_bit;
	}

	bool IsLetterActive(int bit) { return (0 != ((1 << bit) & active_flag)); }

	void ClearActiveFlag() { active_flag = 0; }

	unsigned char active_flag;
	unsigned char active_count;
};

////////////////////////////////////////////////// ����-������ֵ ////////////////////////////////////////////////
static const int RA_CONTINUE_CHONGZHI_MAX_DAY_INDEX = 15;				// ������ֵ�������ػݳ��������ػݸ� ��������

////////////////////////////////////////////////// ����-�������� ////////////////////////////////////////////////
static const int RA_CONTINUE_CONSUME_MAX_DAY_INDEX = 15;				// �������ѻ�������

////////////////////////////////////////////////// ����-�������� ////////////////////////////////////////////////
enum ARMY_SIDE_TYPE
{
	INVALID_ARMY_SIDE = -1,
	BEGIN_ARMY_SIDE = 0,
	BLUE_ARMY_SIDE = BEGIN_ARMY_SIDE,
	RED_ARMY_SIDE,
	YELLOW_ARMY_SIDE,

	MAX_ARMY_SIDE,
};

static const int RA_ARMY_DAY_ARMY_SIDE_NUM = MAX_ARMY_SIDE - BEGIN_ARMY_SIDE;
static const int RA_ARMY_DAY_ACTIVE_DEGREE_EXCHANGE_NUM = 20;							// ��Ծ�ȶһ�����������

////////////////////////////////////////////////// ����-ѭ����ֵ ////////////////////////////////////////////////
static const int RA_CIRCULATION_CHONGZHI_MAX_REWARD_COUNT = 15;			// ѭ����ֵ���ཱ����Ŀ

////////////////////////////////////////////////// ����-�������� ////////////////////////////////////////////////
static const int RA_EXTREME_LUCKY_DRAW_CONSUME_GOLD_CFG_COUNT = 10;			// 
static const int RA_EXTREME_LUCKY_DRAW_REWARD_CFG_COUNT = 100;				// ȫ���齱��Ʒ��
static const int RA_EXTREME_LUCKY_PER_TURN_REWARD_COUNT = 10;				// ÿ�ֳ齱��Ʒ����
static const int RA_EXTREME_LUCKY_NEXT_FLUSH_INTERVAL = 4;					// �´�ˢ�¼��(Сʱ)
static const int RA_EXTREME_LUCKY_RETURN_REWARD_CFG_COUNT = 900;			// �����������
static const int RA_EXTREME_LUCKY_RETURN_REWARD_ONCE_COUNT = 10;
UNSTD_STATIC_CHECK(RA_EXTREME_LUCKY_RETURN_REWARD_ONCE_COUNT < 16)

struct RAExtremeLuckyRewardInfo
{
	RAExtremeLuckyRewardInfo() : seq(0), invalid(0), has_fetch(0), weight(0)
	{
		
	}

	short seq;
	char invalid;
	char has_fetch;
	int weight;
};

static const int RA_CONSUM_GIFT_MAX_ROLL_COUNT = 10;		//����-�����콱�������ҡ������

////////////////////////////////////////////////// ����-���ȹ��� ////////////////////////////////////////////////
static const int RA_NATIONAL_DAY_EXCHANGE_CFG_COUNT = 10;					//���ȹ���һ���Ʒ��

////////////////////////////////////////////////// ����-�������� ////////////////////////////////////////////////
enum RA_PROMOTING_POSITION_REWARD_TYPE			//�������� 
{
	RA_PROMOTING_POSITION_REWARD_TYPE_GOLD,		// Ԫ��
	RA_PROMOTING_POSITION_REWARD_TYPE_ITEM,		// ��Ʒ
	RA_PROMOTING_POSITION_REWARD_TYPE_INSIDE,	// ��Ȧ
	RA_PROMOTING_POSITION_REWARD_TYPE_SPLIT,	// ˫��

	RA_PROMOTING_POSITION_REWARD_TYPE_MAX
};

enum RA_PROMOTING_POSITION_PLAY_TYPE				// �齱��ʽ
{
	RA_PROMOTING_POSITION_PLAY_TYPE_INVALID = -1,
	RA_PROMOTING_POSITION_PLAY_TYPE_FREE,			// ��ѳ�
	RA_PROMOTING_POSITION_PLAY_TYPE_GOLD,			// Ԫ����
	RA_PROMOTING_POSITION_PLAY_TYPE_EXTRA_TIMES,	// ��ֵ���ʹ�����
	RA_PROMOTING_POSITION_PLAY_TYPE_USE_ITEM,		// �����Ʒ��

	RA_PROMOTING_POSITION_PLAY_TYPE_MAX
};

static const int RA_PROMOTING_POSITION_OUTSIDE_REWARD_MAX_SEQ = 255;		// ��Ȧ��ཱ����
static const int RA_PROMOTING_POSITION_INSIDE_REWARD_MAX_SEQ = 255;			// ��Ȧ��ཱ����
static const int RA_PROMOTION_POSITION_MULTIPLAY_TIMES = 30;				// ������
static const int RA_PROMOTION_POSITION_REWARD_RECORDS_MAX_COUNT = 14;		// �齱��ʷ��¼�����
static const int RA_PROMOTION_POSITION_REWARD_CFG_MAX_COUNT = 200;			// �齱�������������
static const int RA_PROMOTION_POSITION_REWARD_CFG_INTERVAL = 10;

enum RA_PROMOTING_POSITION_CIRCLE_TYPE
{
	RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE,
	RA_PROMOTING_POSITION_CIRCLE_TYPE_INSIDE,
};

struct RAPromotingPositionInfo					// ҡ��λ��
{
	RAPromotingPositionInfo() { this->Reset(); }

	void Reset()
	{
		circle_type = RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE;
		position = 0;
	}

	char circle_type;
	unsigned char position;						// 0������㣬����ȡֵ��������seq��Ӧ
};

struct RAPromotingPositionRewardInfo			// ������Ϣ
{
	RAPromotingPositionRewardInfo() : circle_type(RA_PROMOTING_POSITION_CIRCLE_TYPE_OUTSIDE), seq(0) {}

	char circle_type;
	unsigned char seq;
};

// ����ʷ��¼��Ϣ
struct RAPromotingPositionRewardRecord
{
	RAPromotingPositionRewardRecord() : uid(0), reserve_sh(0) { memset(user_name, 0, sizeof(user_name)); }

	GameName user_name;
	int uid;
	RAPromotingPositionRewardInfo reward_info;
	short reserve_sh;
};

////////////////////////////////////////////////// ����-�������� ////////////////////////////////////////////////
static const int RA_BLACK_MARKET_MAX_ITEM_COUNT = 3;		// ������Ʒ�����
static const int RA_BLACK_MARKET_MAX_DAY_COUNT = 7;			// �������������

struct RABlackMarketItemInfo		// ������Ʒ��Ϣ
{
	RABlackMarketItemInfo() { this->Reset(); }

	void Reset()
	{
		seq = -1;
		is_offered = 0;
		cur_price = 0;
		buyer_uid = 0;
	}

	void SetItemInfo(int _seq, int _is_offered, int _cur_price, int _buyer_uid)
	{
		seq = _seq;
		is_offered = _is_offered;
		cur_price = _cur_price;
		buyer_uid = _buyer_uid;
	}

	int seq;						// ��Ʒ������seq
	int is_offered;					// �Ƿ�Ҫ�۹�
	int cur_price;					// ��ǰ�����۸�
	int buyer_uid;					// �������
};

////////////////////////////////////////////////// ����-�䱦�̳� ////////////////////////////////////////////////
static const int RA_TREASURES_MALL_MAX_BUY_ITEM_COUNT = 10;					// �䱦�̳ǹ�����Ʒ��
static const int RA_TREASURES_MALL_MAX_EXCHANGE_ITEM_COUNT = 10;			// �䱦�̳ǿɶһ���Ʒ��

////////////////////////////////////////////////// ����-�¾۱��� ////////////////////////////////////////////////
enum RA_NEW_CORNUCOPIA_TASK_TYPE
{
	RA_NEW_CORNUCOPIA_TASK_TYPE_MOUNT_UPGRADE,								// ����ﵽָ������
	RA_NEW_CORNUCOPIA_TASK_TYPE_HALO_UPGRADE,								// �⻷�ﵽָ������
	RA_NEW_CORNUCOPIA_TASK_TYPE_WING_UPGRADE,								// ����ﵽָ������
	RA_NEW_CORNUCOPIA_TASK_TYPE_SHENGONG_UPGRADE,							// �񹭴ﵽָ������
	RA_NEW_CORNUCOPIA_TASK_TYPE_SHENYI_UPGRADE,								// ����ﵽָ������
	RA_NEW_CORNUCOPIA_TASK_TYPE_WORLD_BOSS,									// ��������BOSS
	RA_NEW_CORNUCOPIA_TASK_TYPE_DAILY_TASK,									// ����ճ�����
	RA_NEW_CORNUCOPIA_TASK_TYPE_GUILD_TASK,									// ��ɹ�������
	RA_NEW_CORNUCOPIA_TASK_TYPE_CROSS_TEAM_FB,								// ��������Ӹ���
	RA_NEW_CORNUCOPIA_TASK_TYPE_HUSONG,										// ����
	RA_NEW_CORNUCOPIA_TASK_TYPE_STRENGTHEN,									// ǿ��
	RA_NEW_CORNUCOPIA_TASK_TYPE_CROSS_1V1,									// ������1V1
	RA_NEW_CORNUCOPIA_TASK_TYPE_UPGRADE_FB,									// ������׸���

	RA_NEW_CORNUCOPIA_TASK_TYPE_MAX,
};

////////////////////////////////////////////////���ֻ//////////////////////////////////////////
static const int RAND_ACTIVITY_ITEM_COLLECTION_REWARD_MAX_COUNT = 5;		// ���ֻ����������

////////////////////////////////////////////////���ǽ���//////////////////////////////////////////
static const int RA_MARRY_REWARD_COUPLE_COUNT_MAX = 100;					// ���ǽ�����ཱ������
static const int RA_MARRY_SHOW_COUPLE_COUNT_MAX = 10;						// ���ǽ��������ʾ����

////////////////////////////////////////////////����Ͷ��//////////////////////////////////////////
static const int RA_OPEN_SERVER_INVEST_TYPE_MAX = 3;						// ����Ͷ����������

///////////////////////////////////////////////�����ż�///////////////////////////////////////////
static const int XINGZUOYIJI_BOX_TYPE_MAX = 4;								// �����ż�������������

struct CoupleInfo
{
	CoupleInfo() { this->Reset(); }

	void Reset()
	{
		propose_id = 0;
		accept_proposal_id = 0;
	}

	bool is_valid() { return (0 != propose_id && 0 != accept_proposal_id); }

	void SetCouple(int _accept_proposal_id, int _proposal_id)
	{
		propose_id = _proposal_id;
		accept_proposal_id = _accept_proposal_id;
	}

	int	propose_id;
	int accept_proposal_id;
};

////////////////////////////////////////////////// �ۻ��� ////////////////////////////////////////////////
static const int RAND_ACTIVITY_XIANYUAN_TREAS_COUNT = 10;

struct XianyuanTreasInfo
{
	XianyuanTreasInfo() : num(0), buy_day_index(0) {}

	short num;					//	��ǰ����ȡ�Ĵ���
	short buy_day_index;
};

////////////////////////////////////////////////// ��ʱ��ɱ ////////////////////////////////////////////////
static const int RA_RUSH_BUYING_SALES_ITEMS_COUNT = 8;					// ������������Ʒ
static const int RA_RUSH_BUYING_PHASES_COUNT = 4;						// �����׶���
static const int RA_RUSH_BUYING_MAX_SERVER_BUYING_TIMES = 1000;			// ȫ���������������������Ʒ��
static const int RA_RUSH_BUYING_MAX_ROLE_BUYING_TIMES = 100;			// ����������������������Ʒ��
UNSTD_STATIC_CHECK(RA_RUSH_BUYING_MAX_ROLE_BUYING_TIMES <= CHAR_MAX);

////////////////////////////////////////////////// ����-��ͼѰ�� ////////////////////////////////////////////////
static const int MAX_RA_MAP_HUNT_CITY_COUNT = 20;							// ����̽�ճ���20��
static const int MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE = 3;					// һ��·�����3������
static const int MAX_RA_MAP_HUNT_ROUTE_COUNT = MAX_RA_MAP_HUNT_CITY_COUNT;	// �������-ÿ�����п�ͷ��·�߱������ҽ���1��
static const int MAX_RA_MAP_HUNT_SERVER_REWARD_COUNT = 10;					// ȫ����������

struct RAMapHuntRouteInfo
{
	RAMapHuntRouteInfo()
	{
		this->Reset();
	}

	void Reset()
	{
		route_index = 0;
		route_active_flag = 0;
		reserve_sh = 0;
		memset(city_list, 0, sizeof(city_list));
		city_fetch_flag = 0;
	}

	char route_index;													//��ǰ·��index
	char route_active_flag;												//��ǰ·�߼�����
	short reserve_sh;
	UNSTD_STATIC_CHECK(3 == MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE);
	char city_list[MAX_RA_MAP_HUNT_CITY_COUNT_OF_ROUTE];				//ˢ�����ĳ����б�
	char city_fetch_flag;
};

////////////////////////////////////////////////// ����-����Ѱ�� ////////////////////////////////////////////////
enum LIGHT_TOWER_EXPLORE_REWAED_TYPE
{
	LIGHT_TOWER_EXPLORE_REWAED_TYPE_INVALID = -1,

	LIGHT_TOWER_EXPLORE_REWAED_TYPE_ITEM = 0,				//������Ʒ
	LIGHT_TOWER_EXPLORE_REWAED_TYPE_GOTO_LAYER = 1,			//��ת

	LIGHT_TOWER_EXPLORE_REWAED_TYPE_MAX
};

static const int RA_LIGHT_TOWER_EXPLORE_MAX_LAYER = 5;						// ����Ѱ��������'
static const int RA_LIGHT_TOWER_EXPLORE_MAX_PER_LAYER_REWAED_COUNT = 6;		// ����Ѱ��ÿ�㽱����
static const int RA_LIGHT_TOWER_EXPLORE_MAX_SERVER_REWARD_COUNT = 10;		// ȫ������������
static const int MAX_LIGHT_TOWER_EXPLORE_REWARD_COUNT = 10;

struct LightTowerExploreGridInfo
{
	LightTowerExploreGridInfo() : layer(0), index(0)
	{

	}

	char layer;												//����
	char index;												//����
};

////////////////////////////////////////////////// ����-��װ�̵� ////////////////////////////////////////////////
static const int RA_MAGIC_SHOP_ITEM_MAX_INDEX = 3;

////////////////////////////////////////////////// ����-������� ////////////////////////////////////////////////
static const int RA_CHONGZHI_GIFT_ITEM_MAX_INDEX = 3;

////////////////////////////////////////////////// ���� //////////////////////////////////////////////////////////////
static const int RA_FISHING_FISH_TYPE_COUNT = 6;						// ����������
static const int RA_FISHING_RANK_MAX_COUNT = 30;

////////////////////////////////////////////////// ����-Ȥζֲ�� ////////////////////////////////////////////////
static const int RA_TREE_PLANTING_RANK_MAX_COUNT = 10;					// ���а���������

struct RAPlantingTreeRankInfo
{
	RAPlantingTreeRankInfo() { this->Reset(); }

	void Reset()
	{
		uid = 0;
		memset(name, 0, sizeof(name));
		opera_items = 0;
		prof = 0;
		sex = 0;
		reserve_sh = 0;
	}

	int uid;
	GameName name;
	int opera_items;
	char prof;
	char sex;
	short reserve_sh;
};

//////////////////////////////////////////////////// ����Ȼˮ�� ///////////////////////////////////////////////////////////
struct RandActivityStealNoteCfg
{
	RandActivityStealNoteCfg() { this->Reset(); }

	void Reset()
	{
		uid = 0;
		be_steal_times = 0;
		reserve_sh = 0;
	}

	int		uid;
	short	be_steal_times;
	short   reserve_sh;
};

static const int RA_CRYSTAL_DALARAN_RANK_REWARD_COUNT = 10;							// ���н��������
static const int RA_CRYSTA_DALARAN_STEAL_USER_MAX_PER_DAY = 30;					// ���͵ȡ����

////////////////////////////////////////////////// ����-��ĩ�񻶻 ////////////////////////////////////////////////
static const int RA_LOTTERY_1_HISTORY_MAX_COUNT = 10;					// �齱��ʷ��¼�������
static const int RA_LOTTERY_1_ONECHOU_COUNT = 1;						// һ�����
static const int RA_LOTTERY_1_TEN_CHOU_COUNT = 10;						// ʮ�����

enum RA_LOTTERY_1_TYPE
{
	RA_LOTTERY_1_TYPE_INVALID = 0,
	RA_LOTTERY_1_TYPE_ONE,		// һ��
	RA_LOTTERY_1_TYPE_TEN,		// ʮ��

	RA_LOTTERY_1_TYPE_MAX,
};

struct RALottery1ChouItem
{
	RALottery1ChouItem() { this->Reset(); }

	void Reset()
	{
		reward_item = 0;
		reward_num = 0;
		is_bind = 0;
		reserve3 = 0;

		memset(xianpin_type_list, 0, sizeof(xianpin_type_list));
	}

	ItemID reward_item;			// ��ҳ�ȡ���Ľ���id
	short reward_num;			// ����

	char is_bind;				// �Ƿ��
	char reserve3;
	char xianpin_type_list[MAX_CHEST_XIANPIN_NUM];
};

/////////////////////////////////////	�������		/////////////////////////////////////////////////////////////////////////

static const int RA_KUANG_HAI_TASK_ITEM_MAX_NUM = 32;		// �������item����
static const int RA_KUANG_HAI_REWARD_LIST_MAX_NUM = 16;		// ���Ľ����б���
static const int RA_KUANG_HAI_REWARD_MAX_ITEM_NUM = 6;		// �����б������Ʒ��

enum RA_KUANG_HAI_TASK_TYPE
{
	RA_KUANG_HAI_TASK_TYPE_BEGIN,
	RA_KUANG_HAI_TASK_TYPE_RECHARGE = 1,				// ��ֵ	����������ֵԪ����
	RA_KUANG_HAI_TASK_TYPE_EXP_FB = 2,					// ��ɾ��鸱��	�����������������
	RA_KUANG_HAI_TASK_TYPE_PHASE_FB = 3,				// ��ɽ��׸���
	RA_KUANG_HAI_TASK_TYPE_PROTECT_FB = 4,				// ͨ���ػ�����������������
	RA_KUANG_HAI_TASK_TYPE_CHALLENGE_FB = 5,			// ͨ����ս����
	RA_KUANG_HAI_TASK_TYPE_ARENA = 6,					// ��ս������	�����������������
	RA_KUANG_HAI_TASK_TYPE_XUN_BAO = 7,					// Ѱ��
	RA_KUANG_HAI_TASK_TYPE_KILL_BOSS = 8,				// ��ɱboss����Ӣ�����磬VIP����Ծ��������
	RA_KUANG_HAI_TASK_TYPE_BB_BOSS = 9,					// ��ɱ����boss
	RA_KUANG_HAI_TASK_TYPE_HU_SONG = 10,				// ��Ů����
	RA_KUANG_HAI_TASK_TYPE_EXP_TASK = 11,				// ��ɾ��������ճ���
	RA_KUANG_HAI_TASK_TYPE_WL_TANGXIAN = 12,			// �μ�����̽��
	RA_KUANG_HAI_TASK_TYPE_TIAN_JIANG_CAI_BAO = 13,		// �μ��콵�Ʊ�
//	RA_KUANG_HAI_TASK_TYPE_LING_SHI_HU_SONG = 14,		// ��ʯ����
	RA_KUANG_HAI_TASK_TYPE_GONG_CHENG_ZHAN = 15,		// ����ս
	RA_KUANG_HAI_TASK_TYPE_YI_ZHAN_FENG_SHEN = 16,		// �μ�һս����
//	RA_KUANG_HAI_TASK_TYPE_XIAN_MO_ZHAN_CHANG = 17,		// �μ���ħս��
	RA_KUANG_HAI_TASK_TYPE_XIAN_MENG_ZHENG_BA = 18,		// �μ���������
	RA_KUANG_HAI_TASK_TYPE_SHUI_JING_HUAN_JING = 19,	// �μ�ˮ���þ�
//	RA_KUANG_HAI_TASK_TYPE_CROSS_MINING = 20,			// �μӿ���ڿ�
	RA_KUANG_HAI_TASK_TYPE_CROSS_FISHING = 21,			// �������
	RA_KUANG_HAI_TASK_TYPE_DIAN_FENG_JING_JI = 22,		// �μ��۷徺������������������1����1�Σ�
//	RA_KUANG_HAI_TASK_TYPE_TIAN_SHENG_ZHONG = 23,		// �μ�����ڣ
	RA_KUANG_HAI_TASK_TYPE_CROSS_XIU_LUO_TA = 24,		// ���������
//	RA_KUANG_HAI_TASK_TYPE_6_JIE_DUO_QI = 25,			// �μ��������
	RA_KUANG_HAI_TASK_TYPE_CROSS_6_JIE = 26,			// �μӿ������
	RA_KUANG_HAI_TASK_TYPE_GUILD = 27,					// �����������
	RA_KUANG_HAI_TASK_TYPE_GUA_JI = 28,					// ��ɹһ������ܻ���
	RA_KUANG_HAI_TASK_TYPE_BANG_BEN_CHOU_JIANG = 29,	// �μӻ��ֳ齱2
//	RA_KUANG_HAI_TASK_TYPE_EXPENSE_NICE_GIFT = 30,		// ���Ѻ���
	RA_KUANG_HAI_TASK_TYPE_LOGIN = 31,					// ��½
	RA_KUANG_HAI_TASK_TYPE_TARGET_RECHARGE = 32,		// ��ֵĿ���������Ŀ��Ԫ����

	RA_KUANG_HAI_TASK_TYPE_MAX_NUM,
};

/////////////////////////////////////	�����ո� - �����		//////////////////////////////////////////
static const int GIFT_HARVEST_OPEN_TIME_MAX_COUNT = 36;						// �ִο���ʱ��	�������
static const int GIFT_HARVEST_MONSTER_CFG_MAX_COUNT = 6;					// ��������		�������
static const int GIFT_HARVEST_FENCE_CFG_MAX_COUNT = 6;						// Χ��������	�������
static const int GIFT_HARVEST_SKILL_CFG_MAX_COUNT = 3;						// ��������		�������
static const int GIFT_HARVEST_RELIVE_POS_MAX_COUNT = 100;					// ���︴���	�������
static const int GIFT_HARVEST_PERSON_RANK_MAX = 10;							// ���а�����	�������

////////////////////////////////////////////////   �������  /////////////////////////////////////
static const int CROSS_PASTURE_RANK_COUNT = 10;                //������������������
static const int CROSS_PASTURE_TOP_RANK_COUNT_MAX = 3;		   // ����������и���

//////////////////////////////////////////////// ����ڿ󡢿������ //////////////////////////////////////////
static const int CROSS_MINING_TOP_RANK_COUNT_MAX = 3;				// ���и���

//////////////////////////////////////////////////// ������� //////////////////////////////
static const int FISHING_FISH_BAIT_CFG_MAX_COUNT = 3;			// ������������
static const int FISHING_FISH_LEAST_COUNT_CFG_MAX_COUNT = 10;	// ��ȡ˫������������Ҫ������Ŀ�����õ������
static const int FISHING_TREASURE_CFG_MAX_COUNT = 20;			// �������������
static const int FISHING_RAND_ROLE_NUM = 4;						// �������չʾ��ɫ����
static const int FISHING_NEWS_MAX = 10;							// ������־����
static const int FISHING_STEAL_COUNT_BUY_MAX = 10;				// ����͵��������������
static const int FISHING_SCORE_MAX_RANK = 10;				    // ����������а��������
static const int CROSS_FISHING_TOP_RANK_COUNT_MAX = 3;			// �������и���

enum FISHING_GEAR
{
	FISHING_GEAR_NET = 0,				// ����
	FISHING_GEAR_SPEAR,					// ���
	FISHING_GEAR_OIL,					// ����

	FISHING_GEAR_COUNT
};
static const int FISHING_GEAR_MAX_COUNT = 3;
UNSTD_STATIC_CHECK(FISHING_GEAR_MAX_COUNT == FISHING_GEAR_COUNT);

enum FISHING_FISH_TYPE
{
	FISHING_FISH_TYPE_NOFISH = 0,		// û�㣨û���ϣ�
	FISHING_FISH_TYPE_QIUDAOYU,			// �ﵶ��
	FISHING_FISH_TYPE_HAIXIA,			// ��Ϻ
	FISHING_FISH_TYPE_DAHUANGYU,		// �����
	FISHING_FISH_TYPE_JINQIANGYU,		// ��ǹ��
	FISHING_FISH_TYPE_MAOXIE,			// ëз
	FISHING_FISH_TYPE_HAIGUI,			// ����
	FISHING_FISH_TYPE_LANXUELONGXIA,	// ��Ѫ��Ϻ

	FISHING_FISH_TYPE_COUNT,
};

static const int FISHING_FISH_TYPE_MAX_COUNT = 8;
UNSTD_STATIC_CHECK(FISHING_FISH_TYPE_MAX_COUNT == FISHING_FISH_TYPE_COUNT);

enum FISHING_NEWS_TYPE
{
	FISHING_NEWS_TYPE_INVALID = 0,
	FISHING_NEWS_TYPE_STEAL = 1,		// ͵����־
	FISHING_NEWS_TYPE_BE_STEAL = 2,		// ��͵��־

	FISHING_NEWS_TYPE_MAX
};

struct FishingRandUserInfo
{
	FishingRandUserInfo() : uid(0), prof(0), least_count_cfg_index(0)
	{
		memset(user_name, 0, sizeof(user_name));
		memset(fish_num_list, 0, sizeof(fish_num_list));
	}

	long long uid;
	GameName user_name;
	short prof;
	short least_count_cfg_index;
	int fish_num_list[FISHING_FISH_TYPE_MAX_COUNT];
};

struct FishingNews
{
	FishingNews() : news_type(0), fish_type(0), fish_num(0), reserve_sh(0)
	{
		memset(user_name, 0, sizeof(user_name));
	}

	short news_type;
	short fish_type;
	short fish_num;
	short reserve_sh;
	GameName user_name;
};

/////////////////////////////////////////////////  ҹս���� /////////////////////////////
const static int NIGHT_FIGHT_RANK_TOP_MAX = 3;							//ҹս���ǽ�����������
const static int CROSS_NIGHT_FIGHT_MAX_SCENE_NUM = 10;					// ҹս������󷿼����

////////////////////////	�������Խ�������   //////////////////////////////////////////
enum OGA_GUILD_BATTLE_OPERA_TYPE
{
	OGA_GUILD_BATTLE_OPERA_TYPE_INFO = 0,						// ��ȡ��Ϣ
	OGA_GUILD_BATTLE_OPERA_TYPE_FETCH_REWARD = 1				// ��ȡ����
};

enum OGA_GUILD_BATTLE_REWARD_TYPE
{
	OGA_GUILD_BATTLE_REWARD_TYPE_WINNER_BANGZHU = 1,			// ������������ȡ
	OGA_GUILD_BATTLE_REWARD_TYPE_WINNER_OWENER = 2,				// ���װ��ɳ�Ա����ȡ
	OGA_GUILD_BATTLE_REWARD_TYPE_LOSER_BANGZHU = 3,				// �������ɰ�������ȡ
	OGA_GUILD_BATTLE_REWARD_TYPE_LOSER_OTHER = 4,				// ����������ҿ���ȡ

	OGA_GUILD_BATTLE_REWARD_TYPE_MAX
};

const static int LUCKY_WISH_MAX_ITEM_COUNT = 30;				//������Ը�����Ʒ����

const static int TODAY_THEME_REWARD_MAX_COUNT = 128;
const static int TODAY_THEME_REWARD_MAX_BYTE = TODAY_THEME_REWARD_MAX_COUNT / BIT_COUNT_OF_BYTE;
UNSTD_STATIC_CHECK(TODAY_THEME_REWARD_MAX_COUNT == 128);

////////////////////////	��Ԫ��   //////////////////////////////////////////
static const int RAND_ACTICITY_ZERO_BUY_RETURN_MAX_TYPE = 16;	// ��Ԫ���������
static const int RAND_ACTICITY_ZERO_BUY_RETURN_MAX_DAY = 20;	// ��Ԫ�������������

///////////////////////// ����Ѱ��  /////////////////////////////////////////////
static const int TIANSHU_XUNZHU_MAX_TYPE_COUNT = 8;			// ����Ѱ�����������
static const int TIANSHU_XUNZHU_SIGNLE_MAX_NUM = 32;		// ����Ѱ�� ���������������
static const int TIANSHU_XUNZHU_TUMO_MONSTER_MAX_NUM = 3;	// ����Ѱ�� ��ħ����һ���������Ĺ�������
static const int TIANSHU_XUNZHU_TUMO_MAX_BOSS_COUNT = 100;	// ����Ѱ�� ��ħ�������BOSS��

/////////////////////////////////////	�����ƹ�		/////////////////////////
static const int  LUCKY_BUY_RECORD_MAX_NUM = 12;			// �����¼���������
static const int  LUCKY_BUY_ROLE_RECORD_MAX_NUM = 10;		// ��ҹ�������������¼

struct RandActivityLuckyCloudBuyParam
{
	RandActivityLuckyCloudBuyParam() { this->Reset(); }

	void Reset()
	{
		round_index = 0;
		round_buy_num = 0;
		round_sale_num = 0;
		round_end_tick = 0;
		next_round_begin_tick = 0;
		memset(role_buy, 0, sizeof(role_buy));
		memset(buy_log, 0, sizeof(buy_log));
	}

	struct BuyLogItem
	{
		int role_id = 0;
		GameName name = { 0 };
		unsigned int timestamp = 0;
	};

	struct RoleBuy
	{
		int role_id = 0;
		int buy_num = 0;
	};

	int round_index;
	int round_buy_num;
	int round_sale_num;
	unsigned int round_end_tick;
	unsigned int next_round_begin_tick;
	RoleBuy role_buy[LUCKY_BUY_ROLE_RECORD_MAX_NUM];
	BuyLogItem buy_log[LUCKY_BUY_RECORD_MAX_NUM];
};

/////////////////////////////////////  �񻶴��ֹ�  /////////////////////////////////////
static const int RA_MAX_CRACY_BUY_NUM_LIMIT = 21;	// ��Ʒ��������

#endif // __ACTIVITY_DEF_HPP__
