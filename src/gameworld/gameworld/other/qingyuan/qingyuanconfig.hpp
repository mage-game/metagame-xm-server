#ifndef __QING_YUAN_CONFIG_HPP__
#define __QING_YUAN_CONFIG_HPP__

#include "servercommon/struct/itemlistparam.h"
#include "servercommon/qingyuandef.hpp"
#include <map>
#include "equipment/equipment.h"
#include "item/itembase.h"
#include "item/itemgriddata.h"
#include <vector>
#include "servercommon/activitydef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "config/gameconfigcommon/gameconfigcommon.h"
#include "servercommon/configcommon.h"

const static int QINGYAUN_MAX_ITEM_COUNT = 6;					// ��Ե��Ʒ�б��������
const static int QINGYUAN_PROFESS_GIFT_MAX_TYPE = 10;			// ���ǽ�����������
const static int QINGYUAN_PROFESS_MAX_GRADE = 200;				// ���ǽ�ȼ����ȼ�

struct QingyuanOtherConfig
{
	QingyuanOtherConfig() : marry_limit_level(0), marry_limit_intimacy(0), scene_id(0), birth_pos_x(0), birth_pos_y(0), fb_free_times_limit(0), fb_buy_times_limit(0), 
		fb_buy_times_gold_cost(0), lover_fb_double_reward_need_gold(0), per_wave_max_time_s(0), next_wave_time_interval(0), divorce_limit_days(0), remarry_limit_days(0), divorce_coin_cost(0), divorce_qingyuan_dec(0), divorce_intimacy_dec(0), couple_halo_trigger(0), open_times(0),
		lovecontract_price(0), marry_songhua_consume_glod(0), marry_songhua_fetch_meili(0), equip_add_per(0), gather_rb_id(0), wedding_red_bag_x(0), wedding_red_bag_y(0), wedding_red_bag_offset(0), 
		wedding_guest_num(0), wedding_buy_guest_limit_1(0), wedding_buy_guest_price_1(0), wedding_buy_guest_limit_2(0), wedding_buy_guest_price_2(0), wedding_add_exp_cd_s(0), exp_factor(0),
		first_buy_cost(0), second_buy_cost(0), third_buy_cost(0)
	{

	}
	int marry_limit_level;
	int marry_limit_intimacy;
	int scene_id;
	int birth_pos_x;
	int birth_pos_y;
	int fb_free_times_limit;
	int fb_buy_times_limit;
	int fb_buy_times_gold_cost;
	int lover_fb_double_reward_need_gold;
	int per_wave_max_time_s;
	int next_wave_time_interval;
	int divorce_limit_days;
	int remarry_limit_days;
	int divorce_coin_cost;
	int divorce_qingyuan_dec;
	int divorce_intimacy_dec;
	int couple_halo_trigger;
	ItemConfigData hunjie_item;
	ItemConfigData hunjie_decompose_item;
	ItemConfigData bind_gold_hy_reward_item;
	ItemConfigData gold_hy_reward_item;
	int open_times;
	ItemConfigData lovecontract_title_boy;
	ItemConfigData lovecontract_title_girl;
	int lovecontract_price;

	ItemConfigData marry_zhufu_fetch_item;
	ItemConfigData marry_songhua_fetch_item;
	int marry_songhua_consume_glod;
	int marry_songhua_fetch_meili;

	int equip_add_per;

	int gather_rb_id;
	int wedding_red_bag_x;
	int wedding_red_bag_y;
	int wedding_red_bag_offset;

	int wedding_guest_num;
	int wedding_buy_guest_limit_1;
	int wedding_buy_guest_price_1;
	int wedding_buy_guest_limit_2;
	int wedding_buy_guest_price_2;
	int wedding_add_exp_cd_s;

	float exp_factor;

	int first_buy_cost;
	int second_buy_cost;
	int third_buy_cost;
};

struct HunyanZhufuItem
{
	HunyanZhufuItem() : gift_gold(0), price_gold(0)	{}

	ItemConfigData gift_item;
	int gift_gold;

	int price_gold;
};

// ��ϯ�ɼ���������
struct HunyanGatherPos
{
	const static int GATHER_MAX_COUNT = 30;

	HunyanGatherPos() : pos_count(0) 
	{
		memset(pos_list, 0, sizeof(pos_list));
	}

	int pos_count;
	Posi pos_list[GATHER_MAX_COUNT];
};

struct HunyanConfig
{
	HunyanConfig()
		: hunyan_scene_id(0), hunyan_birth_pos_x(0), hunyan_birth_pos_y(0), standby_s(0), xunyou_interval_s(0), start_s(0),
		free_fahongbao_num(0), buy_hongbao_gold(0), buy_max_hongbao(0),
		give_hongbao_num(0), random_hongbao_dis(0), free_yanhua_num(0), buy_yanhua_gold(0),
		gathar_id(0), diamonds_hy_gather_max(0), bind_diamonds_hy_gather_max(0),
		xunyou_scene_id(0), report_interval(0), male_marryobj_seq(0), female_marryobj_seq(0), xunyou_marryobj_speed(0), give_exp_hunyan_times_limit(0),
		per_jingyan(0)
	{

	}

	int hunyan_scene_id;
	int hunyan_birth_pos_x;
	int hunyan_birth_pos_y;

	int standby_s;
	int xunyou_interval_s;
	int start_s;

	int free_fahongbao_num;
	int buy_hongbao_gold;
	int buy_max_hongbao;
	ItemConfigData hongbao_item;
	int give_hongbao_num;
	int random_hongbao_dis;

	int free_yanhua_num;
	int buy_yanhua_gold;

	int gathar_id;
	int diamonds_hy_gather_max;
	int bind_diamonds_hy_gather_max;

	int xunyou_scene_id;
	PointConfig bai_tiandi_pos;
	int report_interval;
	int male_marryobj_seq;
	int female_marryobj_seq;
	int xunyou_marryobj_speed;

	NumListConfig xunyou_marryobj_dis_list;

	int give_exp_hunyan_times_limit;

	long long per_jingyan;	//����ÿ������ֵ
};

// ������������
struct QingyuanHunyanConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 8;

	struct ConfigItem
	{
		ConfigItem() : marry_type(0), need_gold(0), need_goldbind(0), reward_item_count(0), join_hunyan_item_count(0)
		{

		}

		int marry_type;
		int need_gold;
		int need_goldbind;

		int reward_item_count;
		ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];

		int join_hunyan_item_count;
		ItemConfigData join_hunyan_item_list[MAX_ATTACHMENT_ITEM_NUM];
	};

	QingyuanHunyanConfig() : cfg_count(0)
	{

	}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

//��Ե�����������----------------------------------------------------------------------------------------
//��Ե������������
struct QingyuanFBMonsterWavesConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 30;

	struct ConfigItem
	{
		ConfigItem() : waves_num(0), monster_id(0), monster_num(0), boss_id(0), boss_num(0) {}

		int waves_num;

		int monster_id;
		int monster_num;
		int boss_id;
		int boss_num;
	};

	QingyuanFBMonsterWavesConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct QingyuanFBRewardConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 6;

	struct ConfigItem
	{
		ConfigItem() : seq(0), wave_num(0), stuff_id(0), stuff_num(0), exp_reward(0) {}

		int seq;

		int wave_num;
		int stuff_id;
		int stuff_num;
		long long exp_reward;
	};

	QingyuanFBRewardConfig() : cfg_count(0) {}

	int cfg_count;
	ConfigItem cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct QingyuanFBMonsterPosConfig
{
	const static int CONFIG_ITEM_MAX_COUNT = 30;

	QingyuanFBMonsterPosConfig() : cfg_count(0) {}

	int cfg_count;
	Posi cfg_list[CONFIG_ITEM_MAX_COUNT];
};

struct QingyuanFBBuffConfig
{
	QingyuanFBBuffConfig() : buff_gongjing_per(0), cost_gold(0), continue_time_s(0), add_gold_per(0) { }

	int buff_gongjing_per;
	int cost_gold;
	int continue_time_s;
	int add_gold_per;
};

//	��Ե����boss
struct QingyuanFBBossPos
{
	const static int MAX_ITEM_COUNT = 2;

	QingyuanFBBossPos() : pos_count(0)
	{
		memset(pos_list, 0, sizeof(pos_list));
	}

	int pos_count;
	Posi pos_list[MAX_ITEM_COUNT];
};

//��Ե��������----------------------------------------------------------------------------------------
struct QingyuanCardConfig
{
	struct ConfigItem
	{
		ConfigItem() : card_id(-1), card_level(-1), need_role_level(1), comsume_item_id_1(0), comsume_item_num_1(0), comsume_item_id_2(0), comsume_item_num_2(0),
			comsume_item_id_3(0), comsume_item_num_3(0), gongji(0), fangyu(0), maxhp(0) {}
	
		int card_id;
		int card_level;
		int need_role_level;

		int comsume_item_id_1;
		int comsume_item_num_1;
		int comsume_item_id_2;
		int comsume_item_num_2;
		int comsume_item_id_3;
		int comsume_item_num_3;

		int gongji;
		int fangyu;
		int maxhp;
	};

	QingyuanCardConfig() : card_id(-1), card_max_level(-1) {}

	const QingyuanCardConfig::ConfigItem * GetCardItemCfg(int card_level) const;

	int card_id;
	int card_max_level;
	ConfigItem card_level_cfglist[QINGYUAN_CARD_MAX_LEVEL + 1];
};

//�������----------------------------------------------------------------------------------------
struct EquipUpLevelConfig
{
	struct EquipStarItemConfig
	{
		EquipStarItemConfig(): equip_id(0), star(0), max_exp(0), baoji_rate(0), gongji(0), fangyu(0), maxhp(0), mingzhong(0), shanbi(0) {}

		ItemID equip_id;
		short star;
		int max_exp;
		short baoji_rate;
		Attribute gongji;
		Attribute fangyu;
		Attribute maxhp;
		Attribute mingzhong;
		Attribute shanbi;
	};

	EquipStarItemConfig star_cfg_list[QINGYUAN_EQUIP_MAX_STAR + 1];
};

struct EquipUpLevelStuffConfig
{
	EquipUpLevelStuffConfig(): stuff_id(0), add_exp(0), add_baoji_exp(0) {}

	ItemID stuff_id;
	int add_exp;
	int add_baoji_exp;
};


struct EquipUpLevelGoldConfig
{
	EquipUpLevelGoldConfig(): times_min(0), times_max(0), cost_gold(0) {}

	int times_min;
	int times_max;
	int cost_gold;
};

// �⻷�������---------------------------------------------------------------------------------------
struct QingyuanCoupleHaloLevelConfig
{
	QingyuanCoupleHaloLevelConfig(): halo_type(0), color(0), level(0), pre_halo_type(0), pre_halo_level(0), stuff_id_man(0), stuff_id_woman(0), stuff_count(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
	{}

	int halo_type;
	int color;
	int level;

	int is_active_image;

	int pre_halo_type;
	int pre_halo_level;

	int stuff_id_man;
	int stuff_id_woman;
	int stuff_count;

	Attribute maxhp;
	Attribute gongji;
	Attribute fangyu;
	Attribute mingzhong;
	Attribute shanbi;
	Attribute baoji;
	Attribute jianren;
};

// �⻷��������
struct HaloSaleCfg
{
	int halo_type = 0;
	int create_role_day_open = 0;
	int normal_price = 0;
	int off_price = 0;

	int reward_item_count = 0;
	ItemConfigData reward_item_list[QINGYAUN_MAX_ITEM_COUNT];
};

// ף������
struct QingyuanBlessRewardConfig
{
	QingyuanBlessRewardConfig() {}

	ItemConfigData reward_item;
};

struct QingyuanHunyanActiviryConfig
{
	QingyuanHunyanActiviryConfig() : uplevel_stuff_id(0), paohuaqui_gather_id(0), paohuaqiu_num(0), paohuaqiu_cd_s(0), random_huaqiu_dis(0), phq_gather_max_count(0), phq_reward_diamonds(0), per_day_saxianhua_count(0), saxianhua_count(0),
		saxianhua_reward_diamonds(0), saxianhua_get_hunjie_stuff_per(0), saxainhua_get_stuff_num(0),
		zhufu_get_diamonds(0), zhufu_get_hunjie_stuff_per(0), zhufu_get_hunjie_stuff_count(0), be_zhufu_get_hunjie_stuff_count(0), food_gather_id(0), food_get_diamonds(0),
		food_get_diamonds_per(0), food_get_exp(0), food_get_hunjie_stuf_num(0), food_other_reward_item_per(0), join_yanhui_max_count(0),
		per_day_max_bind_diamonds(0), per_day_max_stuff_num(0), per_day_max_exp(0), per_day_max_rose_num(0), per_day_max_other_stuff_num(0), bind_diamonds_hy_can_phq(0), paohuaqiu_times(0), hunyan_gather_day_max_stuff_num(0), 
		be_zhufu_get_hunjie_stuff_limit(0), zhufu_get_hunjie_stuff_limit(0)
	{

	}

	int uplevel_stuff_id;
	int paohuaqui_gather_id;
	int paohuaqiu_num;
	int paohuaqiu_cd_s;
	int random_huaqiu_dis;
	int phq_gather_max_count;
	int phq_reward_diamonds;
	ItemConfigData gather_hq_reward_item;
	ItemConfigData pao_hq_reward_item;

	int per_day_saxianhua_count;
	int saxianhua_count;
	int saxianhua_reward_diamonds;
	int saxianhua_get_hunjie_stuff_per;
	int saxainhua_get_stuff_num;

	int zhufu_get_diamonds;
	int zhufu_get_hunjie_stuff_per;
	int zhufu_get_hunjie_stuff_count;
	int be_zhufu_get_hunjie_stuff_count;

	int food_gather_id;
	int food_get_diamonds;
	int food_get_diamonds_per;
	int food_get_exp;
	int food_get_hunjie_stuf_num;
	int food_other_reward_item_per;
	ItemConfigData food_other_reward_item;

	int join_yanhui_max_count;

	int per_day_max_bind_diamonds;
	int per_day_max_stuff_num;
	int per_day_max_exp;
	int per_day_max_rose_num;
	int per_day_max_other_stuff_num;
	int bind_diamonds_hy_can_phq;

	int paohuaqiu_times;
	int hunyan_gather_day_max_stuff_num;
	int be_zhufu_get_hunjie_stuff_limit;
	int zhufu_get_hunjie_stuff_limit;
};

// ��������
struct QingyuanHunliTypeConfig
{
	static const int MAX_MAIL_ITEM_NUM = 6;
	enum HUNLI_TYPE_STYLE
	{
		HUNLI_TYPE_STYLE_CHINESE	= 1,  // ��ʽ
		HUNLI_TYPE_STYLE_WEST		= 2,  // ��ʽ
	};

	QingyuanHunliTypeConfig() : hunli_type(0), hunli_style(0), need_bind_gold(0), need_gold(0), title_id(0), reward_count(0), activity_price(0){}
	int hunli_type;
	int hunli_style;
	int need_bind_gold;
	int need_gold;
	int title_id;
	int reward_count;
	int activity_price;		//Ŀǰ��������ʽ����
	ItemConfigData reward_item_list[MAX_MAIL_ITEM_NUM];
};

// ���ȼ�
struct QingyuanMarryLevelConfig
{
	QingyuanMarryLevelConfig() : level(0), up_level_exp(0), maxhp(0), gongji(0), fangyu(0),
		mingzhong(0), shanbi(0), baoji(0), jianren(0)
	{

	}

	int level;
	int up_level_exp;

	int maxhp;											// ��Ѫ
	int gongji;											// ����
	int fangyu;											// ����
	int mingzhong;										// ����
	int shanbi;											// ����
	int baoji;											// ����
	int jianren;										// ����
};

// ���װ��ͼ��
struct QingyuanEquipSuitConfig
{
	QingyuanEquipSuitConfig() : type(0), slot(0), man_item(0), woman_item(0), color_limit(0), order_limit(0), banglv_add_per(0),
		maxhp(0), gongji(0), fangyu(0), mingzhong(0), shanbi(0), baoji(0), jianren(0)
	{

	}

	int type;
	int slot;

	int man_item;
	int woman_item;

	int color_limit;
	int order_limit;

	int banglv_add_per;

	Attribute maxhp;											// ��Ѫ
	Attribute gongji;											// ����
	Attribute fangyu;											// ����
	Attribute mingzhong;										// ����
	Attribute shanbi;											// ����
	Attribute baoji;											// ����
	Attribute jianren;										// ����
};

// ������Լ
struct QingyuanLoveContractReward
{
	QingyuanLoveContractReward() : day(0), reward_gold_bind(0), reward_count(0)
	{
		
	}

	int day;
	int reward_gold_bind;
	int reward_count;
	ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM];
};

// �����ȶ�
struct WeddingLivenessCfg
{

	struct LivenessCfg
	{
		LivenessCfg() : seq(0), liveness_var(0), every_time_bind_gold(0), red_bag_num(0), every_turn_gather_limit(0), wedding_rewrad_exp_add_per(0) {}

		int seq;
		int liveness_var;
		int every_time_bind_gold;
		int red_bag_num;
		int every_turn_gather_limit;
		int wedding_rewrad_exp_add_per;
	};

	LivenessCfg cfg_list[WEDDING_MAX_LIVENESS_TYPE];
};

// �����ȶȺ���ɼ�����
struct LivenessGatherPosCfg
{
	LivenessGatherPosCfg() : seq(0), x_pos(0), y_pos(0) {}

	int seq;
	int x_pos;
	int y_pos;
};

// ����ף��
struct WeddingBlessingCfg
{
	struct BlessingCfg
	{
		BlessingCfg() : seq(0), param1(0), param2(0), add_liveness(0), item_count(0)
		{

		}

		int seq;
		int param1;
		int param2;
		int add_liveness;

		int item_count;
		ItemConfigData reward_item_list[QINGYAUN_MAX_ITEM_COUNT];
	};

	BlessingCfg cfg_list[WEDDING_BLESSING_MAX_CFG_NUM];
};

// ����ԤԼʱ�������
struct WeddingYuYueTimeCfg
{
	struct ConfigItem
	{
		ConfigItem() : seq(0), apply_time(0), begin_time(0), end_time(0) {}

		int seq;
		int apply_time;
		int begin_time;
		int end_time;
	};


	ConfigItem cfg_list[WEDDING_YUYUE_TIME_MAX];
};

// �����齱������
struct WeddingRewardExpCfg
{
	long long reward_exp_list[YUYUE_HUNYAN_TYPE_MAX];
};

// ��Ƚ��װ��
struct QingyuanEquip2Cfg
{
	QingyuanEquip2Cfg() { this->Reset(); }

	void Reset()
	{
		equip_index = 0;
		equip_level = 0;
		stuff_id = 0;
		stuff_num = 0;
	}

	bool operator==(const QingyuanEquip2Cfg &cfg) const
	{
		return (equip_index == cfg.equip_index) && (equip_level == cfg.equip_level);
	}

	int equip_index;
	int equip_level;
	int stuff_id;
	int stuff_num;

	AttrConfig attr_cfg;
};

// �������
struct QingyuanGiftReward
{
	QingyuanGiftReward(): opengame_day(0), seq(0), need_gold(0), item_count(0)
	{

	}

	int opengame_day;
	int seq;
	int need_gold;
	int item_count;
	ItemConfigData reward_item_list[QINGYAUN_MAX_ITEM_COUNT];
};

//Ѳ��---------------------------------------------------------
struct XunyouCfg
{
	XunyouCfg(): hunyan_type(-1), every_time_bind_gold(0), red_bag_num(0), per_day_gather_limit(0), red_bag_drop_range(0), free_sa_red_bag_count(0),
		buy_sa_red_bag_count(0), red_bag_need_gold(0), xunyou_red_bag_gather_id(0), gather_time_ms(0), free_give_flower_count(0), buy_give_flower_count(0), flower_need_gold(0),
		flower_id(0)
	{
	}

	int hunyan_type;
	int every_time_bind_gold;
	int red_bag_num;
	int per_day_gather_limit;
	int red_bag_drop_range;
	int free_sa_red_bag_count;
	int buy_sa_red_bag_count;
	int red_bag_need_gold;
	int xunyou_red_bag_gather_id;
	int gather_time_ms;
	int free_give_flower_count;
	int buy_give_flower_count;
	int flower_need_gold;
	ItemID flower_id;
};

struct XunyouPathCfg
{
	XunyouPathCfg() : pos_seq(0), stop_time_s(0)
	{
	}

	int pos_seq;
	int stop_time_s;
	PointConfig xunyou_pos;
};

//���---------------------------------------------------------
// �����������
struct QingyuanProfessGiftConfig
{
	QingyuanProfessGiftConfig() :gift_type(0), gift_id(0), exp(0), score(0), gift_effect(0), self_charm(0), other_charm(0)
	{ }

	int gift_type;	// �����������
	int gift_id;	// �������Ʒid
	int exp;		// ���ӵľ���
	int score;		// ���ӵĻ���
	int gift_effect;// ������Ч
	int self_charm;	// ������������ֵ
	int other_charm;// �Է���������ֵ
};

// ��׵ȼ�����
struct QingyuanProfessGradeConfig
{
	QingyuanProfessGradeConfig() :level(0), exp(0)
	{ }

	int level;	// �ȼ�
	int exp;	// �������辭��
	AttrConfig attr_cfg;
};

// �����������
struct QingyuanProfessOtherCfg
{
	QingyuanProfessOtherCfg() :profess_exp_add_per(0), profess_att_add_per(0), add_per_shizhuang_id(0), add_profess_share_per(0)
	{ }

	int profess_exp_add_per;	// �Է�����ӳ�
	int profess_att_add_per;	// �Է����Լӳ�
	int add_per_shizhuang_id;	// �������Լӳ�ʱװid
	int add_profess_share_per;  // �������Լӳ�
};

class QingyuanConfig : public ConfigBase
{
public:
	QingyuanConfig();
	~QingyuanConfig();

	bool Init(const std::string &configname, std::string *err);

	const HunyanConfig & GetHunyanConfig() { return m_hunyan_cfg; }

	const QingyuanHunyanConfig::ConfigItem * GetHunyanCfg(int marry_type);
	const QingyuanOtherConfig & GetOtherCfg() { return m_other_cfg; }
	const QingyuanFBMonsterWavesConfig & GetWaveCfg() { return m_fb_monster_wave_cfg; }
	const QingyuanFBMonsterPosConfig & GetPosCfg() { return m_fb_monster_pos_cfg; }
	const QingyuanCardConfig * GetCardCfg(int card_id);

	const QingyuanFBRewardConfig & GetFBRewardCfg() { return m_fb_reward_cfg; }
	const QingyuanFBBuffConfig & GetFBBuffCfg() { return m_fb_buff_cfg; }
	const QingyuanFBBossPos & GetFBBossPosCfg() { return m_fb_boss_pos_cfg; }

	int GetMaxWaveCount();
	int GetMonsterIDByWaveIndex(int index);

	const QingyuanBlessRewardConfig & GetBlessRewardCfg() { return m_bless_reward_cfg; }

	const HunyanGatherPos & GetHunyanGatherCfg() { return m_hunyan_gather_cfg; }
	const int GetQingyuanCardMaxID() { return m_qingyuan_card_max_id; }

	const EquipUpLevelConfig::EquipStarItemConfig *GetEquipStarCfg(ItemID item_id, short star);
	const EquipUpLevelConfig::EquipStarItemConfig *GetNextEquipStarCfg(ItemID item_id, short star);
	void GetEquipUpLevelStuffAddExp(ItemID item_id, int *exp, bool is_baoji);
	void GetEquipUpLevelCostGold(int times, int *cost_gold); 
	const int GetEquipMaxId() { return m_equip_max_id; }
	const bool SetHunJieInitLevel(ItemGridData *item_grid_data);

	// �⻷---------------------------------------------------------------------
	const int GetQingyuanCoupleHaloMaxType() { return m_couple_halo_max_type_count; }
	const QingyuanCoupleHaloLevelConfig * GetQingyuanCoupleHaloLevelCfg(int halo_type, int level);
	const HaloSaleCfg * GetHaloSaleCfg(int halo_type);

	const int getQingyuanBlessPriceGold() { return m_bless_price_gold; }
	const QingyuanHunyanActiviryConfig * GetQingyuanHunyanActivityCfg() { return &m_hunyan_activity_cfg; }

	const QingyuanHunliTypeConfig * GetQingyuanHunliTypeConfig(int hunli_type);

	const QingyuanMarryLevelConfig * GetMarryLevelCfg(int level);
	const QingyuanEquipSuitConfig * GetQingyuanEquipSuitCfg(int type, int slot);

	const QingyuanLoveContractReward * GetLoveContractReward(int day_num);
	const std::vector<Posi> & GetQingyuanPhqRandomVec();

	const WeddingLivenessCfg &GetWeddingLivenessCfg() { return m_wedding_liveness_cfg; }
	const LivenessGatherPosCfg *GetLivenessGatherPosCfg(int seq);
	const WeddingBlessingCfg::BlessingCfg *GetWeddingBlessingCfgBySeq(int blessing_type, int seq);
	const WeddingYuYueTimeCfg &GetWeddingYuYueTime() { return m_hunyan_yuyue_time_cfg; }
	const WeddingYuYueTimeCfg::ConfigItem *GetWeddingYuYueTimeBySeq(int seq);
	long long GetWeddingAddExp(int level, int hunyan_type);
	const QingyuanEquip2Cfg *GetQingyuanEquip2Cfg(int equip_index, int equip_level);
	const QingyuanGiftReward *GetGiftCfg(int opengame_day, int seq);

	int GetMarryTypeCount() { return m_marry_cfg.cfg_count; }

	// Ѳ�� ------------------------------------------------------------
	int GetXunyouObjDis(int marryobj_seq);
	const XunyouCfg *GetXunyouCfg(int hunyan_type);
	const Posi GetXunyouPos(int pos_seq);
	int GetXunyouPosStopTimeS(int pos_seq);

	int GetXunyouPosCount() { return m_xunyou_pos_count; }
	int GetXunyouSceneId() { return m_hunyan_cfg.xunyou_scene_id; }
	int GetXunyouReportInterval() { return m_hunyan_cfg.report_interval; }

	// ��� ------------------------------------------------------------
	const QingyuanProfessGiftConfig * GetProfessGift(int gift_type);							// ��ȡ�����������
	const QingyuanProfessGradeConfig * GetProfessGrade(int level);								// ��ȡ��׵ȼ�����
	const QingyuanProfessOtherCfg &	GetProfessOtherCfg() { return m_profess_other_cfg; }		// ��ȡ�����������
	int GetQingyuanProfessExpAddPer() { return m_profess_other_cfg.profess_exp_add_per; }		// �Է�����ӳɣ���׵ȼ���
	int GetQingyuanProfessAttrAddPer() { return m_profess_other_cfg.profess_att_add_per; }		// �Է����Լӳɣ���׵ȼ���

private:
	int InitOtherConfig(PugiXmlNode RootElement);
	int InitMarryConfig(PugiXmlNode RootElement);
	int InitMonsterWaveConfig(PugiXmlNode RootElement);
	int InitMonsterPosConfig(PugiXmlNode RootElement);
	int InitHunyanConfig(PugiXmlNode RootElement);
	int InitHunyanGatherConfig(PugiXmlNode RootElement);
	int InitQingyuanCardConfig(PugiXmlNode RootElement);
	int InitEquipUpLevelConfig(PugiXmlNode RootElement);
	int InitEquipUpLevelStuffConfig(PugiXmlNode RootElement);
	int InitEquipUpLevelGoldConfig(PugiXmlNode RootElement);
	int InitHoneymoonBlessRewardConfig(PugiXmlNode RootElement);
	int InitBlessPriceGoldConfig(PugiXmlNode RootElement);
	int InitHunyanActiviryConfig(PugiXmlNode RootElement);

	int InitFBRewardConfig(PugiXmlNode RootElement);
	int InitFBBuffConfig(PugiXmlNode RootElement);
	int initFBBossPosConfig(PugiXmlNode RootElement);
	int initHunliTypeConfig(PugiXmlNode RootElement);
	int InitLoveContractReward(PugiXmlNode RootElement);
	int InitPhqRandomPos(PugiXmlNode RootElement);

	int InitCoupleHaloLevelConfig(PugiXmlNode RootElement);
	int InitHaloSaleConfig(PugiXmlNode RootElement);

	int InitMarryLevelConfig(PugiXmlNode RootElement);
	int InitQingyuanEquipSuitConfig(PugiXmlNode RootElement);

	int InitWeddingLivenessCfg(PugiXmlNode RootElement);
	int InitLivenessGatherPosCfg(PugiXmlNode RootElement);
	int InitWeddingBlessingCfg(PugiXmlNode RootElement);
	int InitWeddingYuYueTimeCfg(PugiXmlNode RootElement);
	int InitWeddingRewardExpCfg(PugiXmlNode RootElement);
	int InitQingyuanEquip2Config(PugiXmlNode RootElement);
	int InitQingyuanGiftConfig(PugiXmlNode RootElement);

	int InitXunyouConfig(PugiXmlNode RootElement);
	int InitXunyouPathConfig(PugiXmlNode RootElement);

	int InitProfessGiftConfig(PugiXmlNode RootElement);
	int InitProfessGradeConfig(PugiXmlNode RootElement);
	int InitProfessOtherConfig(PugiXmlNode RootElement);

	HunyanConfig m_hunyan_cfg;
	HunyanGatherPos m_hunyan_gather_cfg;

	QingyuanOtherConfig m_other_cfg;
	QingyuanHunyanConfig m_marry_cfg;
	QingyuanFBMonsterWavesConfig m_fb_monster_wave_cfg;
	QingyuanFBMonsterPosConfig m_fb_monster_pos_cfg;
	QingyuanBlessRewardConfig m_bless_reward_cfg;
	QingyuanHunyanActiviryConfig m_hunyan_activity_cfg;

	QingyuanFBRewardConfig m_fb_reward_cfg;
	QingyuanFBBuffConfig m_fb_buff_cfg;
	QingyuanFBBossPos m_fb_boss_pos_cfg;

	int m_qingyuan_card_max_id;
	int m_bless_price_gold;
	QingyuanCardConfig m_qingyuan_card_cfglist[QINGYUAN_CARD_MAX_ID + 1];

	ItemID m_equip_max_id;
	std::map<ItemID, EquipUpLevelConfig> m_equip_uplevel_map;
	std::map<ItemID, EquipUpLevelStuffConfig> m_equip_uplevel_stuff_map;
	std::map<int, EquipUpLevelGoldConfig> m_equip_uplevel_gold_map;

	// �⻷---------------------------------------------------------------------
	int m_couple_halo_max_type_count;
	QingyuanCoupleHaloLevelConfig m_couple_halo_level_cfg_list[QINGYUAN_COUPLE_HALO_MAX_TYPE][QINGYUAN_COUPLE_HALO_MAX_LEVEL + 1];

	std::map<int, HaloSaleCfg> m_halo_sale_cfg_map;		// �⻷��������

	// ---------------------------------------------------------------------
	int m_hunli_type_cfg_list_count;
	QingyuanHunliTypeConfig m_hunli_type_cfg_list[QINGYUAN_HUNLI_MAX_TYPE];

	QingyuanEquipSuitConfig m_qingyuan_equip_suit_cfg[QINGYUAN_EQUIPSUIT_MAX_COUNT][QINGYUAN_EQUIPSUIT_SLOT_MAX_COUNT];

	int m_marry_max_level;
	QingyuanMarryLevelConfig m_marry_level_cfg[QINGYUAN_MARRY_MAX_LEVEL + 1];

	std::vector<QingyuanLoveContractReward> m_love_contract_reward_vec; // ������Լ����
	std::vector<Posi> m_phq_random_pos_vec;

	WeddingLivenessCfg m_wedding_liveness_cfg; // �����ȶ�����

	std::vector<LivenessGatherPosCfg> m_liveness_gather_pos_vec; // �����ȶȺ���ɼ�����

	WeddingBlessingCfg m_wedding_blessing_cfg_list[HUNYAN_ZHUFU_TYPE_MAX]; // ����ף������

	WeddingYuYueTimeCfg m_hunyan_yuyue_time_cfg; // ����ԤԼʱ�������

	std::map<int, WeddingRewardExpCfg> m_wedding_reward_exp_map; // �����齱������

	std::vector<QingyuanEquip2Cfg> m_qingyuan_equip_cfg_vec;	// ��Ƚ��װ������

	std::vector<QingyuanGiftReward> m_qingyuan_equip_gift_cfg_vec;	// ���������������

	// Ѳ��---------------------------------------------------------------------
	XunyouCfg m_xunyou_cfg[YUYUE_HUNYAN_TYPE_MAX];			// Ѳ������

	int m_xunyou_pos_count;
	XunyouPathCfg m_path_cfg[HUNYAN_XUNYOU_POINT_MAX_NUM];	// Ѳ��·������

	// ���---------------------------------------------------------------------
	// �������
	QingyuanProfessGiftConfig m_profess_gift_list[QINGYUAN_PROFESS_GIFT_MAX_TYPE + 1];
	// ��׵ȼ�
	QingyuanProfessGradeConfig m_profess_grade_list[QINGYUAN_PROFESS_MAX_GRADE + 1];

	QingyuanProfessOtherCfg m_profess_other_cfg;
};

#endif
