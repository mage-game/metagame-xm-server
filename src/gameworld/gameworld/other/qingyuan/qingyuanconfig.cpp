#include "qingyuanconfig.hpp"
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include "monster/monsterpool.h"
#include "checkresourcecenter.hpp"
#include "item/itempool.h"
#include "config/logicconfigmanager.hpp"

#include <algorithm>

QingyuanConfig::QingyuanConfig() : m_qingyuan_card_max_id(0), m_bless_price_gold(0), m_equip_max_id(0), m_couple_halo_max_type_count(0), m_hunli_type_cfg_list_count(0), m_marry_max_level(0),
									m_xunyou_pos_count(0)
{
}

QingyuanConfig::~QingyuanConfig()
{
}

bool QingyuanConfig::Init(const std::string &configname, std::string *err)
{
	PUGI_XML_CONFIG_PRE_LOAD;

	// 其他
	PUGI_XML_LOAD_CONFIG("other", InitOtherConfig);

	// 婚宴类型配置
	PUGI_XML_LOAD_CONFIG("marry_cfg", InitMarryConfig);

	// 亲密度波数
	PUGI_XML_LOAD_CONFIG("waves_count", InitMonsterWaveConfig);

	// 情缘副本怪物坐标
	PUGI_XML_LOAD_CONFIG("monster_pos", InitMonsterPosConfig);

	// 情缘副本奖励
	PUGI_XML_LOAD_CONFIG("qingyuan_fb_reward", InitFBRewardConfig);

	// 情缘副本BUFF
	PUGI_XML_LOAD_CONFIG("qingyuan_fb_buff", InitFBBuffConfig);

	// 婚戒升级
	PUGI_XML_LOAD_CONFIG("uplevel", InitEquipUpLevelConfig);

	// 婚戒升级材料
	PUGI_XML_LOAD_CONFIG("uplevel_stuff", InitEquipUpLevelStuffConfig);

	// 婚戒升级消耗元宝
	PUGI_XML_LOAD_CONFIG("uplevel_gold", InitEquipUpLevelGoldConfig);

	// 婚宴配置
	PUGI_XML_LOAD_CONFIG("hunyan_cfg", InitHunyanConfig);

	// 婚宴采集(酒席)配置
	PUGI_XML_LOAD_CONFIG("hunyan_gather_cfg", InitHunyanGatherConfig);

	// 情缘卡牌
//	PUGI_XML_LOAD_CONFIG("qingyuan_card_cfg", InitQingyuanCardConfig);

	// 蜜月祝福奖励
	PUGI_XML_LOAD_CONFIG("honeymoon_reward", InitHoneymoonBlessRewardConfig);

	//祝福价格
	PUGI_XML_LOAD_CONFIG("bless_price", InitBlessPriceGoldConfig);

	//婚宴活动
	PUGI_XML_LOAD_CONFIG("hunyan_activity", InitHunyanActiviryConfig);

	// 婚礼类型
	PUGI_XML_LOAD_CONFIG("hunli_type", initHunliTypeConfig);

	// 爱情契约
	PUGI_XML_LOAD_CONFIG("love_contract", InitLoveContractReward);

	// 抛花球随机坐标
	PUGI_XML_LOAD_CONFIG("phq_random_pos", InitPhqRandomPos);

	// 夫妻光环等级
	PUGI_XML_LOAD_CONFIG("couple_halo_cfg", InitCoupleHaloLevelConfig);

	// 光环特卖
	PUGI_XML_LOAD_CONFIG("halo_sale", InitHaloSaleConfig);

	// 结婚等级
	PUGI_XML_LOAD_CONFIG("marry_level_cfg", InitMarryLevelConfig);

	// 结婚装备图鉴
	PUGI_XML_LOAD_CONFIG("qingyuan_equip_handbook", InitQingyuanEquipSuitConfig);

	// 婚礼热度
	PUGI_XML_LOAD_CONFIG("wedding_liveness", InitWeddingLivenessCfg);

	// 婚礼热度采集坐标
	PUGI_XML_LOAD_CONFIG("liveness_gather_pos", InitLivenessGatherPosCfg);

	// 婚礼祝福
	PUGI_XML_LOAD_CONFIG("wedding_blessing", InitWeddingBlessingCfg);

	// 婚礼预约
	PUGI_XML_LOAD_CONFIG("wedding_yuyue_time", InitWeddingYuYueTimeCfg);

	// 婚礼经验奖励
	PUGI_XML_LOAD_CONFIG("wedding_rewrad_exp", InitWeddingRewardExpCfg);

	// 情比金坚装备配置
	PUGI_XML_LOAD_CONFIG("qingyuan_equip2_cfg", InitQingyuanEquip2Config);

	// 礼包配置
	PUGI_XML_LOAD_CONFIG("gift_cfg", InitQingyuanGiftConfig);

	// 巡游配置
	PUGI_XML_LOAD_CONFIG("hunyan_xunyou", InitXunyouConfig);

	// 巡游路径配置
	PUGI_XML_LOAD_CONFIG("hunyan_xunyou_path", InitXunyouPathConfig);

	//表白墙礼物
	PUGI_XML_LOAD_CONFIG("profess_gift", InitProfessGiftConfig);
	//表白墙等级
	PUGI_XML_LOAD_CONFIG("profess_grade", InitProfessGradeConfig);
	//表白墙其他
	PUGI_XML_LOAD_CONFIG("profess_other", InitProfessOtherConfig);

	return true;
}

int QingyuanConfig::InitOtherConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "marry_limit_level", m_other_cfg.marry_limit_level) || m_other_cfg.marry_limit_level <= 0 || m_other_cfg.marry_limit_level > MAX_ROLE_LEVEL)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "marry_limit_intimacy", m_other_cfg.marry_limit_intimacy) || m_other_cfg.marry_limit_intimacy < 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "scene_id", m_other_cfg.scene_id) || m_other_cfg.scene_id <= 0)
	{
		return -3;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_other_cfg.scene_id);

	if (!PugiGetSubNodeValue(dataElement, "pos_x", m_other_cfg.birth_pos_x) || m_other_cfg.birth_pos_x <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "pos_y", m_other_cfg.birth_pos_y) || m_other_cfg.birth_pos_y <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "fb_free_times_limit", m_other_cfg.fb_free_times_limit) || m_other_cfg.fb_free_times_limit <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "fb_buy_times_limit", m_other_cfg.fb_buy_times_limit) || m_other_cfg.fb_buy_times_limit < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "fb_buy_times_gold_cost", m_other_cfg.fb_buy_times_gold_cost) || m_other_cfg.fb_buy_times_gold_cost <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement, "divorce_limit_days", m_other_cfg.divorce_limit_days) || m_other_cfg.divorce_limit_days < 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElement, "remarry_limit_days", m_other_cfg.remarry_limit_days) || m_other_cfg.remarry_limit_days < 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElement, "divorce_coin_cost", m_other_cfg.divorce_coin_cost) || m_other_cfg.divorce_coin_cost <= 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(dataElement, "divorce_qingyuan_dec", m_other_cfg.divorce_qingyuan_dec) || m_other_cfg.divorce_qingyuan_dec <= 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(dataElement, "divorce_intimacy_dec", m_other_cfg.divorce_intimacy_dec) || m_other_cfg.divorce_intimacy_dec < 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(dataElement, "couple_halo_trigger_distance", m_other_cfg.couple_halo_trigger) || m_other_cfg.couple_halo_trigger <= 0)
	{
		return -15;
	}

// 	PugiXmlNode itemelement = dataElement.child("hunjie_item");
// 	ItemID itemid = 0;
// 	if (NULL != itemelement && PugiGetSubNodeValue(itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
// 	{
// 		if (!m_other_cfg.hunjie_item.ReadConfig(itemelement))
// 		{
// 			return -16;
// 		}
// 	}
// 	else
// 	{
// 		return -17;
// 	}

	PugiXmlNode itemelement = dataElement.child("hunjie_decompose_item");
	ItemID itemid = 0;
	if (!itemelement.empty() && PugiGetSubNodeValue(itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
	{
		if (!m_other_cfg.hunjie_decompose_item.ReadConfig(itemelement))
		{
			return -16;
		}
	}
	else
	{
		return -17;
	}

	itemelement = dataElement.child("bind_gold_hy_reward_item");
	itemid = 0;
	if (!itemelement.empty() && PugiGetSubNodeValue(itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
	{
		if (!m_other_cfg.bind_gold_hy_reward_item.ReadConfig(itemelement))
		{
			return -18;
		}
	}
	else
	{
		return -19;
	}
	
	itemelement = dataElement.child("gold_hy_reward_item");
	itemid = 0;
	if (!itemelement.empty() && PugiGetSubNodeValue(itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
	{
		if (!m_other_cfg.gold_hy_reward_item.ReadConfig(itemelement))
		{
			return -20;
		}
	}
	else
	{
		return -21;
	}

	if (!PugiGetSubNodeValue(dataElement, "open_times", m_other_cfg.open_times) || m_other_cfg.open_times <= 0)
	{
		return -22;
	}
	
	itemelement = dataElement.child("lovecontract_title_boy");
	itemid = 0;
	if (!itemelement.empty() && PugiGetSubNodeValue(itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
	{
		if (!m_other_cfg.lovecontract_title_boy.ReadConfig(itemelement))
		{
			return -23;
		}
	}
	else
	{
		return -24;
	}

	itemelement = dataElement.child("lovecontract_title_girl");
	itemid = 0;
	if (!itemelement.empty() && PugiGetSubNodeValue(itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
	{
		if (!m_other_cfg.lovecontract_title_girl.ReadConfig(itemelement))
		{
			return -25;
		}
	}
	else
	{
		return -26;
	}

	if (!PugiGetSubNodeValue(dataElement, "lovecontract_price", m_other_cfg.lovecontract_price) || m_other_cfg.lovecontract_price <= 0)
	{
		return -27;
	}

	itemelement = dataElement.child("marry_zhufu_fetch_item");
	itemid = 0;
	if (!itemelement.empty() && PugiGetSubNodeValue(itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
	{
		if (!m_other_cfg.marry_zhufu_fetch_item.ReadConfig(itemelement))
		{
			return -28;
		}
	}
	else
	{
		return -29;
	}

	itemelement = dataElement.child("marry_songhua_fetch_item");
	itemid = 0;
	if (!itemelement.empty() && PugiGetSubNodeValue(itemelement, "item_id", itemid) && NULL != ITEMPOOL->GetItem(itemid))
	{
		if (!m_other_cfg.marry_songhua_fetch_item.ReadConfig(itemelement))
		{
			return -30;
		}
	}
	else
	{
		return -32;
	}

	if (!PugiGetSubNodeValue(dataElement, "marry_songhua_consume_glod", m_other_cfg.marry_songhua_consume_glod) || m_other_cfg.marry_songhua_consume_glod <= 0)
	{
		return -33;
	}

	if (!PugiGetSubNodeValue(dataElement, "marry_songhua_fetch_meili", m_other_cfg.marry_songhua_fetch_meili) || m_other_cfg.marry_songhua_fetch_meili <= 0)
	{
		return -34;
	}

	if (!PugiGetSubNodeValue(dataElement, "equip_add_per", m_other_cfg.equip_add_per) || m_other_cfg.equip_add_per < 0)
	{
		return -35;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_rb_id", m_other_cfg.gather_rb_id) || m_other_cfg.gather_rb_id <= 0)
	{
		return -36;
	}

	if (!PugiGetSubNodeValue(dataElement, "wedding_red_bag_x", m_other_cfg.wedding_red_bag_x) || m_other_cfg.wedding_red_bag_x < 0)
	{
		return -37;
	}

	if (!PugiGetSubNodeValue(dataElement, "wedding_red_bag_y", m_other_cfg.wedding_red_bag_y) || m_other_cfg.wedding_red_bag_y < 0)
	{
		return -38;
	}

	if (!PugiGetSubNodeValue(dataElement, "wedding_red_bag_offset", m_other_cfg.wedding_red_bag_offset) || m_other_cfg.wedding_red_bag_offset <= 0)
	{
		return -39;
	}

	if (!PugiGetSubNodeValue(dataElement, "wedding_guest_num", m_other_cfg.wedding_guest_num) || m_other_cfg.wedding_guest_num <= 0)
	{
		return -40;
	}

	if (!PugiGetSubNodeValue(dataElement, "wedding_buy_guest_limit_1", m_other_cfg.wedding_buy_guest_limit_1) || m_other_cfg.wedding_buy_guest_limit_1 <= 0)
	{
		return -41;
	}

	if (!PugiGetSubNodeValue(dataElement, "wedding_buy_guest_price_1", m_other_cfg.wedding_buy_guest_price_1) || m_other_cfg.wedding_buy_guest_price_1 <= 0)
	{
		return -42;
	}

	if (!PugiGetSubNodeValue(dataElement, "wedding_buy_guest_limit_2", m_other_cfg.wedding_buy_guest_limit_2) || m_other_cfg.wedding_buy_guest_limit_2 <= 0)
	{
		return -43;
	}

	if (!PugiGetSubNodeValue(dataElement, "wedding_buy_guest_price_2", m_other_cfg.wedding_buy_guest_price_2) || m_other_cfg.wedding_buy_guest_price_2 <= 0)
	{
		return -44;
	}

	if (!PugiGetSubNodeValue(dataElement, "wedding_add_exp_cd_s", m_other_cfg.wedding_add_exp_cd_s) || m_other_cfg.wedding_add_exp_cd_s <= 0)
	{
		return -45;
	}

	if (!PugiGetSubNodeValue(dataElement, "lover_fb_double_reward_need_gold", m_other_cfg.lover_fb_double_reward_need_gold) || m_other_cfg.lover_fb_double_reward_need_gold <= 0)
	{
		return -46;
	}

	if (!PugiGetSubNodeValue(dataElement, "per_wave_max_time_s", m_other_cfg.per_wave_max_time_s) || m_other_cfg.per_wave_max_time_s <= 0)
	{
		return -47;
	}

	if (!PugiGetSubNodeValue(dataElement, "next_wave_time_interval", m_other_cfg.next_wave_time_interval) || m_other_cfg.next_wave_time_interval <= 0)
	{
		return -48;
	}

	if (!PugiGetSubNodeValue(dataElement, "exp_factor", m_other_cfg.exp_factor) || m_other_cfg.exp_factor < 0)
	{
		return -49;
	}

	if (!PugiGetSubNodeValue(dataElement, "first_buy_cost", m_other_cfg.first_buy_cost) || m_other_cfg.first_buy_cost < 0)
	{
		return -50;
	}

	if (!PugiGetSubNodeValue(dataElement, "second_buy_cost", m_other_cfg.second_buy_cost) || m_other_cfg.second_buy_cost < 0)
	{
		return -51;
	}

	if (!PugiGetSubNodeValue(dataElement, "third_buy_cost", m_other_cfg.third_buy_cost) || m_other_cfg.third_buy_cost < 0)
	{
		return -52;
	}

	return 0;
}

int QingyuanConfig::InitLoveContractReward(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while(!dataElement.empty())
	{
		QingyuanLoveContractReward reward_cfg;

		if (!PugiGetSubNodeValue(dataElement, "day", reward_cfg.day) || reward_cfg.day < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "reward_gold_bind", reward_cfg.reward_gold_bind) || reward_cfg.reward_gold_bind <= 0)
		{
			return -2;
		}

// 		{
// 			reward_cfg.reward_count = ItemConfigData::ReadConfigList(dataElement, std::string("reward_item"),
// 				reward_cfg.reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
// 			if (reward_cfg.reward_count < 0)
// 			{
// 				return -200 + reward_cfg.reward_count;
// 			}
// 		}

		m_love_contract_reward_vec.push_back(reward_cfg);
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitPhqRandomPos(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int count = 0;
	while (!dataElement.empty())
	{
		Posi pos;

		if (!PugiGetSubNodeValue(dataElement, "pos_x", pos.x) || pos.x < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", pos.y) || pos.y < 0)
		{
			return -2;
		}

		++count;
		m_phq_random_pos_vec.push_back(pos);
		dataElement = dataElement.next_sibling();
	}

	if (count < m_hunyan_activity_cfg.paohuaqiu_num)
	{
		return -3;
	}

	return 0;
}

int QingyuanConfig::InitCoupleHaloLevelConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	memset(m_couple_halo_level_cfg_list, 0, sizeof(m_couple_halo_level_cfg_list));
	int last_halo_type = -1;
	while (!data_element.empty())
	{
		int halo_type = -1;
		int halo_level = -1;
		if (!PugiGetSubNodeValue(data_element, "halo_type", halo_type) || halo_type < 0 || halo_type >= QINGYUAN_COUPLE_HALO_MAX_TYPE)
		{
			return -1;
		}

		if (last_halo_type != halo_type && (last_halo_type + 1) != halo_type)
		{
			return -1000;
		}

		last_halo_type = halo_type;

		if (!PugiGetSubNodeValue(data_element, "level", halo_level) || halo_level < 0 || halo_level > QINGYUAN_COUPLE_HALO_MAX_LEVEL)
		{
			return -2;
		}

		QingyuanCoupleHaloLevelConfig &level_cfg = m_couple_halo_level_cfg_list[halo_type][halo_level];
		m_couple_halo_max_type_count = halo_type + 1;
		level_cfg.halo_type = halo_type;
		level_cfg.level = halo_level;

		if (!PugiGetSubNodeValue(data_element, "color", level_cfg.color) || level_cfg.color <= ItemBase::I_COLOR_INVALID || level_cfg.color >= ItemBase::I_COLOR_MAX)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "pre_halo_type", level_cfg.pre_halo_type) || level_cfg.pre_halo_type < 0 || level_cfg.pre_halo_type >= QINGYUAN_COUPLE_HALO_MAX_TYPE || level_cfg.pre_halo_type > level_cfg.halo_type)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "pre_halo_level", level_cfg.pre_halo_level) || level_cfg.pre_halo_level < 0 || level_cfg.pre_halo_level > QINGYUAN_COUPLE_HALO_MAX_LEVEL)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_id", level_cfg.stuff_id_man) || NULL == ITEMPOOL->GetItem(level_cfg.stuff_id_man))
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_id_woman", level_cfg.stuff_id_woman) || NULL == ITEMPOOL->GetItem(level_cfg.stuff_id_woman))
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_count", level_cfg.stuff_count) || level_cfg.stuff_count <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "is_active_image", level_cfg.is_active_image) || level_cfg.is_active_image < 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "maxhp", level_cfg.maxhp) || level_cfg.maxhp < 0)
		{
			return -101;
		}

		if (!PugiGetSubNodeValue(data_element, "gongji", level_cfg.gongji) || level_cfg.gongji < 0)
		{
			return -102;
		}

		if (!PugiGetSubNodeValue(data_element, "fangyu", level_cfg.fangyu) || level_cfg.fangyu < 0)
		{
			return -103;
		}

		if (!PugiGetSubNodeValue(data_element, "mingzhong", level_cfg.mingzhong) || level_cfg.mingzhong < 0)
		{
			return -104;
		}

		if (!PugiGetSubNodeValue(data_element, "shanbi", level_cfg.shanbi) || level_cfg.shanbi < 0)
		{
			return -105;
		}

		if (!PugiGetSubNodeValue(data_element, "baoji", level_cfg.baoji) || level_cfg.baoji < 0)
		{
			return -106;
		}

		if (!PugiGetSubNodeValue(data_element, "jianren", level_cfg.jianren) || level_cfg.jianren < 0)
		{
			return -107;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitHaloSaleConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{
		HaloSaleCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "halo_type", cfg.halo_type) || cfg.halo_type < 0 || cfg.halo_type >= QINGYUAN_COUPLE_HALO_MAX_TYPE)
		{
			return -1;
		}

		if (cfg.halo_type >= m_couple_halo_max_type_count)
		{
			return -1000;
		}

		if (!PugiGetSubNodeValue(data_element, "create_role_day_open", cfg.create_role_day_open) || cfg.create_role_day_open <= 0)
		{
			return -2;
		}
	
		if (!PugiGetSubNodeValue(data_element, "normal_price", cfg.normal_price) || cfg.normal_price <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "off_price", cfg.off_price) || cfg.off_price <= 0)
		{
			return -4;
		}

		{
			cfg.reward_item_count = ItemConfigData::ReadConfigList(data_element, std::string("reward_item"),
				cfg.reward_item_list, QINGYAUN_MAX_ITEM_COUNT);
			if (cfg.reward_item_count < 0)
			{
				return -200 + cfg.reward_item_count;
			}
		}

		auto it = m_halo_sale_cfg_map.find(cfg.halo_type);
		if (it != m_halo_sale_cfg_map.end())
		{
			return -1001;
		}

		m_halo_sale_cfg_map[cfg.halo_type] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

const QingyuanLoveContractReward * QingyuanConfig::GetLoveContractReward(int day_num)
{
	if (day_num < 0 || day_num >= static_cast<int>(m_love_contract_reward_vec.size()))
	{
		return NULL;
	}

	if (m_love_contract_reward_vec[day_num].day == day_num)
	{
		return &m_love_contract_reward_vec[day_num];
	}
	
	return NULL;	
}

const std::vector<Posi> & QingyuanConfig::GetQingyuanPhqRandomVec()
{
	if (!m_phq_random_pos_vec.empty())
	{
		std::random_shuffle(m_phq_random_pos_vec.begin(), m_phq_random_pos_vec.end());
	}

	return m_phq_random_pos_vec;
}

int QingyuanConfig::InitMarryConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_marry_cfg.cfg_count = 0;

	while (!dataElement.empty())
	{
		if (m_marry_cfg.cfg_count >= QingyuanHunyanConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}

		QingyuanHunyanConfig::ConfigItem &marry_cfg = m_marry_cfg.cfg_list[m_marry_cfg.cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "marry_type", marry_cfg.marry_type) || marry_cfg.marry_type < 0 || marry_cfg.marry_type != m_marry_cfg.cfg_count)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_coin", marry_cfg.need_goldbind) || marry_cfg.need_goldbind < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "need_gold", marry_cfg.need_gold) || marry_cfg.need_gold < 0)
		{
			return -4;
		}

		{
			marry_cfg.reward_item_count = ItemConfigData::ReadConfigList(dataElement, std::string("reward_item"),
				marry_cfg.reward_item_list, MAX_ATTACHMENT_ITEM_NUM);
			if (marry_cfg.reward_item_count < 0)
			{
				return -200 + marry_cfg.reward_item_count;
			}
		}

		{
			marry_cfg.join_hunyan_item_count = ItemConfigData::ReadConfigList(dataElement, std::string("join_hunyan_item"),
				marry_cfg.join_hunyan_item_list, MAX_ATTACHMENT_ITEM_NUM);
			if (marry_cfg.join_hunyan_item_count < 0)
			{
				return -210 + marry_cfg.join_hunyan_item_count;
			}
		}

		++ m_marry_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitMonsterWaveConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_fb_monster_wave_cfg.cfg_count = 0;
	while(!dataElement.empty())
	{
		if (m_fb_monster_wave_cfg.cfg_count >= QingyuanFBMonsterWavesConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}
		
		QingyuanFBMonsterWavesConfig::ConfigItem &waves_cfg = m_fb_monster_wave_cfg.cfg_list[m_fb_monster_wave_cfg.cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "monster_num", waves_cfg.monster_num) || waves_cfg.monster_num < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_id", waves_cfg.boss_id) || waves_cfg.boss_id < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "boss_num", waves_cfg.boss_num) || waves_cfg.boss_num < 0)
		{
			return -4;
		}
		
		
		if (!PugiGetSubNodeValue(dataElement, "waves_num", waves_cfg.waves_num) || waves_cfg.waves_num != m_fb_monster_wave_cfg.cfg_count)
		{
			return -5;
		}

		UInt16 monster_id = 0;
		if (!PugiGetSubNodeValue(dataElement, "monster_id", monster_id) || !MONSTERPOOL->IsMonsterExist(monster_id))
		{
			return -6;
		}
		waves_cfg.monster_id = monster_id;

		++ m_fb_monster_wave_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitMonsterPosConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_fb_monster_pos_cfg.cfg_count = 0;
	while(!dataElement.empty())
	{
		if (m_fb_monster_pos_cfg.cfg_count >= QingyuanFBMonsterPosConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}

		Posi &pos_cfg = m_fb_monster_pos_cfg.cfg_list[m_fb_monster_pos_cfg.cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "pos_x", pos_cfg.x) || pos_cfg.x <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", pos_cfg.y) || pos_cfg.y <= 0)
		{
			return -3;
		}

		++ m_fb_monster_pos_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitHunyanConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	if (!PugiGetSubNodeValue(dataElement, "hunyan_scene_id", m_hunyan_cfg.hunyan_scene_id) || m_hunyan_cfg.hunyan_scene_id <= 0)
	{
		return -1;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_hunyan_cfg.hunyan_scene_id);

	if (!PugiGetSubNodeValue(dataElement, "pos_x", m_hunyan_cfg.hunyan_birth_pos_x) || m_hunyan_cfg.hunyan_birth_pos_x <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "pos_y", m_hunyan_cfg.hunyan_birth_pos_y) || m_hunyan_cfg.hunyan_birth_pos_y <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "standby_s", m_hunyan_cfg.standby_s) || m_hunyan_cfg.standby_s <= 0)
	{
		return -4;
	}

	if (!PugiGetSubNodeValue(dataElement, "xunyou_interval_s", m_hunyan_cfg.xunyou_interval_s) || m_hunyan_cfg.xunyou_interval_s <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "start_s", m_hunyan_cfg.start_s) || m_hunyan_cfg.start_s <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "free_fahongbao_num", m_hunyan_cfg.free_fahongbao_num) || m_hunyan_cfg.free_fahongbao_num < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "buy_hongbao_gold", m_hunyan_cfg.buy_hongbao_gold) || m_hunyan_cfg.buy_hongbao_gold <= 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement, "buy_max_hongbao", m_hunyan_cfg.buy_max_hongbao) || m_hunyan_cfg.buy_max_hongbao <= 0)
	{
		return -9;
	}

	PugiXmlNode hongbao_reward_element = dataElement.child("hongbao_reward");
	if (hongbao_reward_element.empty())
	{
		return -10;
	}

	if (!m_hunyan_cfg.hongbao_item.ReadConfig(hongbao_reward_element))
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(dataElement, "give_hongbao_num", m_hunyan_cfg.give_hongbao_num) || m_hunyan_cfg.give_hongbao_num <= 0)
	{
		return -12;
	}

	if (!PugiGetSubNodeValue(dataElement, "random_hongbao_dis", m_hunyan_cfg.random_hongbao_dis) || m_hunyan_cfg.random_hongbao_dis <= 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(dataElement, "free_yanhua_num", m_hunyan_cfg.free_yanhua_num) || m_hunyan_cfg.free_yanhua_num <= 0)
	{
		return -14;
	}

	if (!PugiGetSubNodeValue(dataElement, "buy_yanhua_gold", m_hunyan_cfg.buy_yanhua_gold) || m_hunyan_cfg.buy_yanhua_gold <= 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_id", m_hunyan_cfg.gathar_id) || m_hunyan_cfg.gathar_id <= 0)
	{
		return -20;
	}

	if (!PugiGetSubNodeValue(dataElement, "gather_max", m_hunyan_cfg.diamonds_hy_gather_max) || m_hunyan_cfg.diamonds_hy_gather_max <= 0)
	{
		return -21;
	}

	if (!PugiGetSubNodeValue(dataElement, "bind_gold_gather_max", m_hunyan_cfg.bind_diamonds_hy_gather_max) || m_hunyan_cfg.bind_diamonds_hy_gather_max <= 0)
	{
		return -22;
	}

	if (!PugiGetSubNodeValue(dataElement, "xunyou_scene_id", m_hunyan_cfg.xunyou_scene_id) || m_hunyan_cfg.xunyou_scene_id <= 0)
	{
		return -23;
	}

	CheckResourceCenter::Instance().GetSceneCheck()->Add(m_hunyan_cfg.xunyou_scene_id);

	if (!m_hunyan_cfg.bai_tiandi_pos.ReadConfig(dataElement, "bai_tiandi_pos"))
	{
		return -24;
	}

	if (!PugiGetSubNodeValue(dataElement, "xunyou_marryobj_speed", m_hunyan_cfg.xunyou_marryobj_speed) || m_hunyan_cfg.xunyou_marryobj_speed <= 0)
	{
		return -25;
	}

	if (!m_hunyan_cfg.xunyou_marryobj_dis_list.ReadConfig(dataElement, "xunyou_marryobj_dis_list"))
	{
		return -26;
	}

	if (!m_hunyan_cfg.xunyou_marryobj_dis_list.Check(NumListConfig::CHECK_POLICY_GRANTER_EQUAL_0))
	{
		return -27;
	}

	if (!PugiGetSubNodeValue(dataElement, "report_interval", m_hunyan_cfg.report_interval) || m_hunyan_cfg.report_interval <= 0)
	{
		return -28;
	}

	if (!PugiGetSubNodeValue(dataElement, "male_marryobj_seq", m_hunyan_cfg.male_marryobj_seq) || m_hunyan_cfg.male_marryobj_seq < 0)
	{
		return -29;
	}

	if (!PugiGetSubNodeValue(dataElement, "female_marryobj_seq", m_hunyan_cfg.female_marryobj_seq) || m_hunyan_cfg.female_marryobj_seq < 0)
	{
		return -30;
	}

	if (!PugiGetSubNodeValue(dataElement, "give_exp_hunyan_times_limit", m_hunyan_cfg.give_exp_hunyan_times_limit) || m_hunyan_cfg.give_exp_hunyan_times_limit <= 0)
	{
		return -31;
	}

	if (!PugiGetSubNodeValue(dataElement, "per_jingyan", m_hunyan_cfg.per_jingyan) || m_hunyan_cfg.per_jingyan < 0)
	{
		return -32;
	}

	return 0;
}

int QingyuanConfig::InitEquipUpLevelConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	ItemID last_equip_id = 0;
	short last_star = 0;
	//int last_max_exp = 0;

	while(!dataElement.empty())
	{
		ItemID equip_id;
		short star;

		if (!PugiGetSubNodeValue(dataElement, "equip_id", equip_id) || NULL == ITEMPOOL->GetItem(equip_id) || (last_equip_id != 0 && equip_id - last_equip_id != 1))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "star", star) || star <= 0 || star > QINGYUAN_EQUIP_MAX_STAR || star - last_star != 1)
		{
			return -2;
		}
		last_star = star;
		m_equip_max_id = equip_id;

		if (m_equip_uplevel_map.end() == m_equip_uplevel_map.find(equip_id))
		{
			EquipUpLevelConfig uplevel_cfg;
			m_equip_uplevel_map[equip_id] = uplevel_cfg;
		}

		std::map<ItemID, EquipUpLevelConfig>::iterator iter = m_equip_uplevel_map.find(equip_id);
		EquipUpLevelConfig::EquipStarItemConfig *star_cfg = &iter->second.star_cfg_list[star];

		if (QINGYUAN_EQUIP_MAX_STAR == star)
		{
			last_equip_id = equip_id;
			last_star = 0;
		}

		star_cfg->equip_id = equip_id;
		star_cfg->star = star;

		if (!PugiGetSubNodeValue(dataElement, "exp", star_cfg->max_exp) || star_cfg->max_exp <= 0)
		{
			return -10;
		}
		//last_max_exp = star_cfg->max_exp; 

		if (!PugiGetSubNodeValue(dataElement, "baoji_rate", star_cfg->baoji_rate) || star_cfg->baoji_rate < 0 || star_cfg->baoji_rate > 100)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", star_cfg->gongji) || star_cfg->gongji < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", star_cfg->fangyu) || star_cfg->fangyu < 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", star_cfg->maxhp) || star_cfg->maxhp < 0)
		{
			return -14;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", star_cfg->mingzhong) || star_cfg->mingzhong < 0)
		{
			return -15;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", star_cfg->shanbi) || star_cfg->shanbi < 0)
		{
			return -16;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitEquipUpLevelStuffConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while(!dataElement.empty())
	{
		ItemID stuff_id;

		if (!PugiGetSubNodeValue(dataElement, "stuff_id", stuff_id) || NULL == ITEMPOOL->GetItem(stuff_id))
		{
			return -1;
		}
		

		if (m_equip_uplevel_stuff_map.end() == m_equip_uplevel_stuff_map.find(stuff_id))
		{
			EquipUpLevelStuffConfig stuff_cfg;
			m_equip_uplevel_stuff_map[stuff_id] = stuff_cfg;
		}

		std::map<ItemID, EquipUpLevelStuffConfig>::iterator iter = m_equip_uplevel_stuff_map.find(stuff_id);
		EquipUpLevelStuffConfig *stuff_cfg = &iter->second;
		stuff_cfg->stuff_id = stuff_id;

		if (!PugiGetSubNodeValue(dataElement, "add_exp", stuff_cfg->add_exp) || stuff_cfg->add_exp <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_baoji_exp", stuff_cfg->add_baoji_exp))
		{
			return -4;
		}

		dataElement = dataElement.next_sibling();
	}
	
	return 0;
}

int QingyuanConfig::InitEquipUpLevelGoldConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_max_times = 0;
	int last_gold = 0;

	while(!dataElement.empty())
	{
		int times_min = 0;
		int times_max = 0;

		if (!PugiGetSubNodeValue(dataElement, "times_min", times_min) ||  (last_max_times != 0 && times_min - last_max_times != 1))
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "times_max", times_max) ||  (times_max <= times_min))
		{
			return -2;
		}
		last_max_times = times_max;

		EquipUpLevelGoldConfig &gold_cfg = m_equip_uplevel_gold_map[times_min];
		gold_cfg.times_min = times_min;
		gold_cfg.times_max = times_max;

		if (!PugiGetSubNodeValue(dataElement, "gold", gold_cfg.cost_gold) || gold_cfg.cost_gold < last_gold)
		{
			return -3;
		}
		last_gold = gold_cfg.cost_gold;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitQingyuanCardConfig(PugiXmlNode RootElement)
{
// 	PugiXmlNode dataElement = RootElement.child("data");
// 	if (dataElement.empty())
// 	{
// 		return -10000;
// 	}
// 
// 	m_qingyuan_card_max_id = -1;
// 	
// 	while(!dataElement.empty())
// 	{
// 		int card_id = 0;
// 		if (!PugiGetSubNodeValue(dataElement, "card_id", card_id) || card_id < m_qingyuan_card_max_id)
// 		{
// 			return -1;
// 		}
// 		
// 		if (card_id < 0 || card_id > QINGYUAN_CARD_MAX_ID)
// 		{
// 			return -2;
// 		}
// 
// 		m_qingyuan_card_max_id = card_id;
// 
// 		QingyuanCardConfig &card_cfg = m_qingyuan_card_cfglist[card_id];
// 		card_cfg.card_id = card_id;
// 
// 		int card_level;
// 		if (!PugiGetSubNodeValue(dataElement, "card_level", card_level) || card_level != card_cfg.card_max_level + 1)
// 		{
// 			return -3;
// 		}
// 
// 		if (card_level < 0 || card_level > QINGYUAN_CARD_MAX_LEVEL)
// 		{
// 			return -4;
// 		}
// 		card_cfg.card_max_level = card_level;
// 
// 		QingyuanCardConfig::ConfigItem &sub_cfg = card_cfg.card_level_cfglist[card_level];
// 
// 		sub_cfg.card_id = card_id;
// 		sub_cfg.card_level = card_level;
// 
// 		if (!PugiGetSubNodeValue(dataElement, "need_role_level", sub_cfg.need_role_level) || sub_cfg.need_role_level <= 0)
// 		{
// 			return -5;
// 		}
// 
// 		if (!PugiGetSubNodeValue(dataElement, "comsume_item_id_1", sub_cfg.comsume_item_id_1) || NULL == ITEMPOOL->GetItem(sub_cfg.comsume_item_id_1))
// 		{
// 			return -6;
// 		}
// 		
// 		if (!PugiGetSubNodeValue(dataElement, "comsume_item_num_1", sub_cfg.comsume_item_num_1) || sub_cfg.comsume_item_num_1 <= 0)
// 		{
// 			return -7;
// 		}
// 
// 		{
// 			GetSubNodeValue(dataElement, "comsume_item_id_2", sub_cfg.comsume_item_id_2);
// 			GetSubNodeValue(dataElement, "comsume_item_num_2", sub_cfg.comsume_item_num_2);
// 
// 			if (sub_cfg.comsume_item_num_2 > 0)
// 			{
// 				if (NULL == ITEMPOOL->GetItem(sub_cfg.comsume_item_id_2))
// 				{
// 					return -8;
// 				}
// 			}
// 			else
// 			{
// 				sub_cfg.comsume_item_id_2 = 0;
// 				sub_cfg.comsume_item_num_2 = 0;
// 			}
// 		}
// 
// 		{
// 			GetSubNodeValue(dataElement, "comsume_item_id_3", sub_cfg.comsume_item_id_3);
// 			GetSubNodeValue(dataElement, "comsume_item_num_3", sub_cfg.comsume_item_num_3);
// 
// 			if (sub_cfg.comsume_item_num_3 > 0)
// 			{
// 				if (NULL == ITEMPOOL->GetItem(sub_cfg.comsume_item_id_3))
// 				{
// 					return -9;
// 				}
// 			}
// 			else
// 			{
// 				sub_cfg.comsume_item_id_3 = 0;
// 				sub_cfg.comsume_item_num_3 = 0;
// 			}
// 		}
// 		
// 		if (!PugiGetSubNodeValue(dataElement, "gongji", sub_cfg.gongji) || sub_cfg.gongji < 0)
// 		{
// 			return -100;
// 		}
// 
// 		if (!PugiGetSubNodeValue(dataElement, "fangyu", sub_cfg.fangyu) || sub_cfg.fangyu < 0)
// 		{
// 			return -101;
// 		}
// 
// 		if (!PugiGetSubNodeValue(dataElement, "maxhp", sub_cfg.maxhp) || sub_cfg.maxhp < 0)
// 		{
// 			return -102;
// 		}
// 
// 		dataElement = dataElement.next_sibling();
// 	}

	return 0;
}


int QingyuanConfig::InitHunyanGatherConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_hunyan_gather_cfg.pos_count = 0;

	while(!dataElement.empty())
	{
		if (m_hunyan_gather_cfg.pos_count >= HunyanGatherPos::GATHER_MAX_COUNT)
		{
			return -20000;
		}

		Posi &pos = m_hunyan_gather_cfg.pos_list[m_hunyan_gather_cfg.pos_count];

		if (!PugiGetSubNodeValue(dataElement, "pos_x", pos.x) || pos.x <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", pos.y) || pos.y <= 0)
		{
			return -2;
		}

		++ m_hunyan_gather_cfg.pos_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}


const QingyuanHunyanConfig::ConfigItem * QingyuanConfig::GetHunyanCfg(int marry_type)
{
	if (marry_type >= 0 && marry_type < m_marry_cfg.cfg_count && marry_type < QingyuanHunyanConfig::CONFIG_ITEM_MAX_COUNT)
	{
		return &m_marry_cfg.cfg_list[marry_type];
	}

	return NULL;
}

const QingyuanCardConfig * QingyuanConfig::GetCardCfg(int card_id)
{
	if (card_id >= 0 || card_id <= m_qingyuan_card_max_id)
	{
		return &m_qingyuan_card_cfglist[card_id];
	}

	return NULL;
}

const QingyuanCardConfig::ConfigItem * QingyuanCardConfig::GetCardItemCfg(int card_level) const
{
	if (card_level >= 0 || card_level <= card_max_level)
	{
		return &card_level_cfglist[card_level];
	}

	return NULL;
}

int QingyuanConfig::GetMaxWaveCount()
{
	return m_fb_monster_wave_cfg.cfg_count;
}

int QingyuanConfig::GetMonsterIDByWaveIndex(int index)
{
	if (index >= 0 && index < m_fb_monster_wave_cfg.cfg_count && index < QingyuanFBMonsterWavesConfig::CONFIG_ITEM_MAX_COUNT)
	{
		return m_fb_monster_wave_cfg.cfg_list[index].monster_id;
	}

	return 0;
}

const EquipUpLevelConfig::EquipStarItemConfig *QingyuanConfig::GetEquipStarCfg(ItemID item_id, short star)
{
	if (item_id < 0 || star < 0 || star > QINGYUAN_EQUIP_MAX_STAR)
	{
		return NULL;
	}

	std::map<ItemID, EquipUpLevelConfig>::const_iterator iter = m_equip_uplevel_map.find(item_id);
	if (m_equip_uplevel_map.end() == iter)
	{
		return NULL;
	}

	return &iter->second.star_cfg_list[star];
}

const EquipUpLevelConfig::EquipStarItemConfig *QingyuanConfig::GetNextEquipStarCfg(ItemID item_id, short star)
{
	if (item_id < 0 || star < 0 || star > QINGYUAN_EQUIP_MAX_STAR) 
	{
		return NULL;
	}

	if (star == QINGYUAN_EQUIP_MAX_STAR)
	{
		return this->GetEquipStarCfg(item_id + 1, 1);
	}
	else
	{
		return this->GetEquipStarCfg(item_id, star);
	}
}

void QingyuanConfig::GetEquipUpLevelStuffAddExp(ItemID item_id, int *exp, bool is_baoji)
{
	if (NULL == exp)
	{
		return;
	}

	std::map<ItemID, EquipUpLevelStuffConfig>::const_iterator iter = m_equip_uplevel_stuff_map.find(item_id);
	if(m_equip_uplevel_stuff_map.end() == iter)
	{
		return;
	}

	int per_exp = is_baoji ? iter->second.add_baoji_exp : iter->second.add_exp;
	*exp += per_exp;
}

void QingyuanConfig::GetEquipUpLevelCostGold(int times, int *cost_gold)
{
	if (times < 0 || NULL == cost_gold)
	{
		return;
	}

	std::map<int, EquipUpLevelGoldConfig>::const_iterator iter;
	for (iter = m_equip_uplevel_gold_map.begin(); iter != m_equip_uplevel_gold_map.end(); ++ iter)
	{
		if (times >= iter->second.times_min && times <= iter->second.times_max)
		{
			*cost_gold = iter->second.cost_gold;
			break;
		}
	}
}

const bool QingyuanConfig::SetHunJieInitLevel(ItemGridData *item_grid_data)
{
	if (NULL == item_grid_data) return false;

	const Equipment* equip_item = (const Equipment *)ITEMPOOL->GetItem(item_grid_data->item_id);
	if (NULL == equip_item || ItemBase::I_TYPE_EQUIPMENT != equip_item->GetItemType())
	{
		return false;
	}

	if (Equipment::E_TYPE_SPECIAL_HUNJIE != equip_item->GetEquipType())
	{
		return false;
	}
	EquipmentParam *equip_param = (EquipmentParam *)item_grid_data->param;
	equip_param->star_level = 1;

	return true;
}

const QingyuanCoupleHaloLevelConfig * QingyuanConfig::GetQingyuanCoupleHaloLevelCfg(int halo_type, int level)
{
	if (halo_type < 0 || halo_type >= QINGYUAN_COUPLE_HALO_MAX_TYPE) return NULL;

	if (level < 0 || level > QINGYUAN_COUPLE_HALO_MAX_LEVEL) return NULL;

	QingyuanCoupleHaloLevelConfig &cfg = m_couple_halo_level_cfg_list[halo_type][level];
	if (cfg.halo_type != halo_type || cfg.level != level) return NULL;

	return &cfg;
}

const HaloSaleCfg * QingyuanConfig::GetHaloSaleCfg(int halo_type)
{
	auto it = m_halo_sale_cfg_map.find(halo_type);
	if (it != m_halo_sale_cfg_map.end())
	{
		return &it->second;
	}

	return nullptr;
}

const QingyuanHunliTypeConfig * QingyuanConfig::GetQingyuanHunliTypeConfig(int hunli_type)
{
	if (hunli_type < 0 || hunli_type >= m_hunli_type_cfg_list_count)
	{
		return NULL;
	}
	return &m_hunli_type_cfg_list[hunli_type];
}

int QingyuanConfig::InitHoneymoonBlessRewardConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10001;
	}

	QingyuanBlessRewardConfig &reward_cfg = m_bless_reward_cfg;
	while (!data_element.empty())
	{
		PugiXmlNode reward_item_element = data_element.child("reward_item");
		if (reward_item_element.empty())
		{
			return -1;
		}

		if (!reward_cfg.reward_item.ReadConfig(reward_item_element))
		{
			return -2;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitBlessPriceGoldConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10001;
	}

	int &price_cfg = m_bless_price_gold;
	while (!data_element.empty())
	{
		
		if (!PugiGetSubNodeValue(data_element, "bless_price_gold", price_cfg) || price_cfg <= 0)
		{
			return -1;
		}
		data_element = data_element.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitHunyanActiviryConfig(PugiXmlNode RootElement)
{	
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10009;
	}

	if (!PugiGetSubNodeValue(dataElement, "paohuaqiu_num", m_hunyan_activity_cfg.paohuaqiu_num) || m_hunyan_activity_cfg.paohuaqiu_num <= 0)
	{
		return -1;
	}

	if (!PugiGetSubNodeValue(dataElement, "paohuaqiu_cd_s", m_hunyan_activity_cfg.paohuaqiu_cd_s) || m_hunyan_activity_cfg.paohuaqiu_cd_s <= 0)
	{
		return -2;
	}

	if (!PugiGetSubNodeValue(dataElement, "phq_gather_max_count", m_hunyan_activity_cfg.phq_gather_max_count) || m_hunyan_activity_cfg.phq_gather_max_count <= 0)
	{
		return -3;
	}

	if (!PugiGetSubNodeValue(dataElement, "phq_reward_diamonds", m_hunyan_activity_cfg.phq_reward_diamonds) || m_other_cfg.birth_pos_x < 0)
	{
		return -4;
	}

	PugiXmlNode reward_item_element = dataElement.child("phq_reward_item");
	if (reward_item_element.empty())
	{
		return -100;
	}

	if (!m_hunyan_activity_cfg.gather_hq_reward_item.ReadConfig(reward_item_element))
	{
		return -101;
	}

// 	reward_item_element = dataElement.child("pao_hq_reward_item");
// 	if (reward_item_element.empty())
// 	{
// 		return -102;
// 	}
// 
// 	if (!m_hunyan_activity_cfg.pao_hq_reward_item.ReadConfig(reward_item_element))
// 	{
// 		return -103;
// 	}

	if (!PugiGetSubNodeValue(dataElement, "per_day_saxianhua_count", m_hunyan_activity_cfg.per_day_saxianhua_count) || m_hunyan_activity_cfg.per_day_saxianhua_count <= 0)
	{
		return -5;
	}

	if (!PugiGetSubNodeValue(dataElement, "saxianhua_count", m_hunyan_activity_cfg.saxianhua_count) || m_hunyan_activity_cfg.saxianhua_count <= 0)
	{
		return -6;
	}

	if (!PugiGetSubNodeValue(dataElement, "saxianhua_reward_diamonds", m_hunyan_activity_cfg.saxianhua_reward_diamonds) || m_hunyan_activity_cfg.saxianhua_reward_diamonds < 0)
	{
		return -7;
	}

	if (!PugiGetSubNodeValue(dataElement, "saxianhua_get_hunjie_stuff_per", m_hunyan_activity_cfg.saxianhua_get_hunjie_stuff_per) || m_hunyan_activity_cfg.saxianhua_get_hunjie_stuff_per < 0)
	{
		return -8;
	}

	if (!PugiGetSubNodeValue(dataElement, "zhufu_get_diamonds", m_hunyan_activity_cfg.zhufu_get_diamonds) || m_hunyan_activity_cfg.zhufu_get_diamonds < 0)
	{
		return -9;
	}

	if (!PugiGetSubNodeValue(dataElement, "zhufu_get_hunjie_stuff_per", m_hunyan_activity_cfg.zhufu_get_hunjie_stuff_per) || m_hunyan_activity_cfg.zhufu_get_hunjie_stuff_per < 0)
	{
		return -10;
	}

	if (!PugiGetSubNodeValue(dataElement, "zhufu_get_hunjie_stuff_count", m_hunyan_activity_cfg.zhufu_get_hunjie_stuff_count) || m_hunyan_activity_cfg.zhufu_get_hunjie_stuff_count <= 0)
	{
		return -11;
	}

	if (!PugiGetSubNodeValue(dataElement, "paohuaqui_gather_id", m_hunyan_activity_cfg.paohuaqui_gather_id) || m_hunyan_activity_cfg.paohuaqui_gather_id <= 0)
	{
		return -13;
	}

	if (!PugiGetSubNodeValue(dataElement, "food_get_exp", m_hunyan_activity_cfg.food_get_exp) || m_hunyan_activity_cfg.food_get_exp < 0)
	{
		return -15;
	}

	if (!PugiGetSubNodeValue(dataElement, "food_get_hunjie_stuf_num", m_hunyan_activity_cfg.food_get_hunjie_stuf_num) || m_hunyan_activity_cfg.food_get_hunjie_stuf_num <= 0)
	{
		return -16;
	}

	PugiXmlNode other_reward_item_element = dataElement.child("food_other_reward_item");
	if (other_reward_item_element.empty())
	{
		return -102;
	}

	if (!m_hunyan_activity_cfg.food_other_reward_item.ReadConfig(other_reward_item_element))
	{
		return -103;
	}

	if (!PugiGetSubNodeValue(dataElement, "join_yanhui_max_count", m_hunyan_activity_cfg.join_yanhui_max_count) || m_hunyan_activity_cfg.join_yanhui_max_count <= 0)
	{
		return -17;
	}

	if (!PugiGetSubNodeValue(dataElement, "food_gather_id", m_hunyan_activity_cfg.food_gather_id) || m_hunyan_activity_cfg.food_gather_id <= 0)
	{
		return -18;
	}

	if (!PugiGetSubNodeValue(dataElement, "random_huaqiu_dis", m_hunyan_activity_cfg.random_huaqiu_dis) || m_hunyan_activity_cfg.random_huaqiu_dis <= 0)
	{
		return -19;
	}

	if (!PugiGetSubNodeValue(dataElement, "uplevel_stuff_id", m_hunyan_activity_cfg.uplevel_stuff_id) || m_hunyan_activity_cfg.uplevel_stuff_id <= 0)
	{
		return -20;
	}

	if (!PugiGetSubNodeValue(dataElement, "saxainhua_get_stuff_num", m_hunyan_activity_cfg.saxainhua_get_stuff_num) || m_hunyan_activity_cfg.saxainhua_get_stuff_num < 0)
	{
		return -21;
	}

	if (!PugiGetSubNodeValue(dataElement, "food_get_diamonds_per", m_hunyan_activity_cfg.food_get_diamonds_per) || m_hunyan_activity_cfg.food_get_diamonds_per < 0)
	{
		return -22;
	}

	if (!PugiGetSubNodeValue(dataElement, "food_other_reward_item_per", m_hunyan_activity_cfg.food_other_reward_item_per) || m_hunyan_activity_cfg.food_other_reward_item_per < 0)
	{
		return -23;
	}

	if (!PugiGetSubNodeValue(dataElement, "per_day_max_other_stuff_num", m_hunyan_activity_cfg.per_day_max_other_stuff_num) || m_hunyan_activity_cfg.per_day_max_other_stuff_num < 0)
	{
		return -24;
	}

	if (!PugiGetSubNodeValue(dataElement, "per_day_max_rose_num", m_hunyan_activity_cfg.per_day_max_rose_num) || m_hunyan_activity_cfg.per_day_max_rose_num < 0)
	{
		return -25;
	}

	if (!PugiGetSubNodeValue(dataElement, "per_day_max_stuff_num", m_hunyan_activity_cfg.per_day_max_stuff_num) || m_hunyan_activity_cfg.per_day_max_stuff_num < 0)
	{
		return -26;
	}

	if (!PugiGetSubNodeValue(dataElement, "per_day_max_exp", m_hunyan_activity_cfg.per_day_max_exp) || m_hunyan_activity_cfg.per_day_max_exp < 0)
	{
		return -27;
	}

	if (!PugiGetSubNodeValue(dataElement, "per_day_max_bind_diamonds", m_hunyan_activity_cfg.per_day_max_bind_diamonds) || m_hunyan_activity_cfg.per_day_max_bind_diamonds < 0)
	{
		return -28;
	}

	if (!PugiGetSubNodeValue(dataElement, "bind_diamonds_hy_can_phq", m_hunyan_activity_cfg.bind_diamonds_hy_can_phq) || m_hunyan_activity_cfg.bind_diamonds_hy_can_phq < 0)
	{
		return -29;
	}

	if (!PugiGetSubNodeValue(dataElement, "food_get_diamonds", m_hunyan_activity_cfg.food_get_diamonds) || m_hunyan_activity_cfg.food_get_diamonds < 0)
	{
		return -30;
	}

	if (!PugiGetSubNodeValue(dataElement, "paohuaqiu_times", m_hunyan_activity_cfg.paohuaqiu_times) || m_hunyan_activity_cfg.paohuaqiu_times < 0)
	{
		return -31;
	}

	if (!PugiGetSubNodeValue(dataElement, "hunyan_gather_day_max_stuff_num", m_hunyan_activity_cfg.hunyan_gather_day_max_stuff_num) || m_hunyan_activity_cfg.hunyan_gather_day_max_stuff_num < 0)
	{
		return -32;
	}

	if (!PugiGetSubNodeValue(dataElement, "be_zhufu_get_hunjie_stuff_count", m_hunyan_activity_cfg.be_zhufu_get_hunjie_stuff_count) || m_hunyan_activity_cfg.be_zhufu_get_hunjie_stuff_count < 0)
	{
		return -33;
	}

	if (!PugiGetSubNodeValue(dataElement, "be_zhufu_get_hunjie_stuff_limit", m_hunyan_activity_cfg.be_zhufu_get_hunjie_stuff_limit) || m_hunyan_activity_cfg.be_zhufu_get_hunjie_stuff_limit < 0)
	{
		return -34;
	}

	if (!PugiGetSubNodeValue(dataElement, "zhufu_get_hunjie_stuff_limit", m_hunyan_activity_cfg.zhufu_get_hunjie_stuff_limit) || m_hunyan_activity_cfg.zhufu_get_hunjie_stuff_limit < 0)
	{
		return -35;
	}

	return 0;
}

int QingyuanConfig::InitFBRewardConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_fb_reward_cfg.cfg_count = 0;
	while(!dataElement.empty())
	{
		if (m_fb_reward_cfg.cfg_count >= QingyuanFBRewardConfig::CONFIG_ITEM_MAX_COUNT)
		{
			return -1;
		}

		QingyuanFBRewardConfig::ConfigItem &reward_cfg = m_fb_reward_cfg.cfg_list[m_fb_reward_cfg.cfg_count];

		if (!PugiGetSubNodeValue(dataElement, "waves_num", reward_cfg.wave_num) || reward_cfg.wave_num <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_id", reward_cfg.stuff_id) || reward_cfg.stuff_id < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "stuff_num", reward_cfg.stuff_num) || reward_cfg.stuff_num < 0)
		{
			return -4;
		}


		if (!PugiGetSubNodeValue(dataElement, "exp_reward", reward_cfg.exp_reward) || reward_cfg.exp_reward < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "seq", reward_cfg.seq) || reward_cfg.seq < 0 || reward_cfg.seq >= QingyuanFBRewardConfig::CONFIG_ITEM_MAX_COUNT || reward_cfg.seq != m_fb_reward_cfg.cfg_count)
		{
			return -6;
		}

		++ m_fb_reward_cfg.cfg_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitFBBuffConfig(PugiXmlNode RootElement)
{

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while(!dataElement.empty())
	{
		if (!PugiGetSubNodeValue(dataElement, "buff_gongjing_per",m_fb_buff_cfg.buff_gongjing_per) || m_fb_buff_cfg.buff_gongjing_per < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "cost_gold", m_fb_buff_cfg.cost_gold) || m_fb_buff_cfg.cost_gold < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "continue_time_s", m_fb_buff_cfg.continue_time_s) || m_fb_buff_cfg.continue_time_s < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "add_gold_per", m_fb_buff_cfg.add_gold_per) || m_fb_buff_cfg.add_gold_per < 0)
		{
			return -4;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int QingyuanConfig::initFBBossPosConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	m_fb_boss_pos_cfg.pos_count = 0;
	while(!dataElement.empty())
	{
		if (m_fb_boss_pos_cfg.pos_count >= QingyuanFBBossPos::MAX_ITEM_COUNT)
		{
			return -1;
		}

		Posi &pos_cfg = m_fb_boss_pos_cfg.pos_list[m_fb_boss_pos_cfg.pos_count];

		if (!PugiGetSubNodeValue(dataElement, "pos_x", pos_cfg.x) || pos_cfg.x <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "pos_y", pos_cfg.y) || pos_cfg.y <= 0)
		{
			return -3;
		}

		++ m_fb_boss_pos_cfg.pos_count;
		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int QingyuanConfig::initHunliTypeConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	m_hunli_type_cfg_list_count = 0;
	int last_hunli_type = -1;

	while (!data_element.empty())
	{
		if (m_hunli_type_cfg_list_count >= QINGYUAN_HUNLI_MAX_TYPE)
		{
			return -1001;
		}
		
		int hunli_type = -1;
		if (!PugiGetSubNodeValue(data_element, "hunli_type", hunli_type) || hunli_type < 0 || hunli_type >= QINGYUAN_HUNLI_MAX_TYPE || hunli_type - 1 != last_hunli_type)
		{
			return -1;
		}

		last_hunli_type = hunli_type;

		QingyuanHunliTypeConfig &hunli_cfg = m_hunli_type_cfg_list[hunli_type];
		hunli_cfg.hunli_type = hunli_type;

		if (!PugiGetSubNodeValue(data_element, "hunli_style", hunli_cfg.hunli_style) || (hunli_cfg.hunli_style != QingyuanHunliTypeConfig::HUNLI_TYPE_STYLE_CHINESE && hunli_cfg.hunli_style != QingyuanHunliTypeConfig::HUNLI_TYPE_STYLE_WEST))
		{
			return -2;
		}
		
		if (!PugiGetSubNodeValue(data_element, "need_bind_gold", hunli_cfg.need_bind_gold) || hunli_cfg.need_bind_gold < 0)
		{
			return -3;
		}

		if(!PugiGetSubNodeValue(data_element, "need_gold", hunli_cfg.need_gold) || hunli_cfg.need_gold < 0)
		{
			return -4;
		}

		if (0 == hunli_cfg.need_bind_gold && 0 == hunli_cfg.need_gold)
		{
			return -5;
		}

		if (0 != hunli_cfg.need_bind_gold && 0 != hunli_cfg.need_gold)
		{
			return -6;
		}

// 		if(!PugiGetSubNodeValue(data_element, "title_id", hunli_cfg.title_id) || hunli_cfg.title_id <= 0)
// 		{
// 			return -6;
// 		}
		
		{
			PugiXmlNode  item_list_element = data_element.child("reward_item_list");
			if (item_list_element.empty()) return -7;

			PugiXmlNode  item_element = item_list_element.child("reward_item");
			if (item_element.empty()) return -8;
			
			while (!item_element.empty())
			{
				if (hunli_cfg.reward_count >= QingyuanHunliTypeConfig::MAX_MAIL_ITEM_NUM) return -9;
				
				// 可能会配空物品，所有没做检查
				hunli_cfg.reward_item_list[hunli_cfg.reward_count].ReadConfig(item_element);

				hunli_cfg.reward_count++;

				item_element = item_element.next_sibling();
			}
		}

		if (hunli_cfg.hunli_style == QingyuanHunliTypeConfig::HUNLI_TYPE_STYLE_WEST)
		{
			if (!PugiGetSubNodeValue(data_element, "activity_price", hunli_cfg.activity_price) || hunli_cfg.activity_price < 0)
			{
				return -9;
			}
		}
		
		++m_hunli_type_cfg_list_count;
		data_element = data_element.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitMarryLevelConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	int last_level = -1;
	m_marry_max_level = 0;

	while(!dataElement.empty())
	{
		int level = -1;
		if (!PugiGetSubNodeValue(dataElement, "level", level) || level < 0 || level > QINGYUAN_MARRY_MAX_LEVEL || level != last_level + 1)
		{
			return -1;
		}

		QingyuanMarryLevelConfig &level_cfg = m_marry_level_cfg[level];
		level_cfg.level = level;
		m_marry_max_level = level;
		last_level = level;

		if (!PugiGetSubNodeValue(dataElement, "up_level_exp", level_cfg.up_level_exp) || level_cfg.up_level_exp <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", level_cfg.maxhp) || level_cfg.maxhp < 0) 
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", level_cfg.gongji) || level_cfg.gongji < 0) 
		{
			return -40;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", level_cfg.fangyu) || level_cfg.fangyu < 0) 
		{
			return -50;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", level_cfg.mingzhong) || level_cfg.mingzhong < 0) 
		{
			return -60;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", level_cfg.shanbi) || level_cfg.shanbi < 0) 
		{
			return -70;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", level_cfg.baoji) || level_cfg.baoji < 0) 
		{
			return -80;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren", level_cfg.jianren) || level_cfg.jianren < 0) 
		{
			return -90;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitQingyuanEquipSuitConfig(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		int type = -1;
		int slot = -1;
		if (!PugiGetSubNodeValue(dataElement, "type", type) || type < 0 || type >= QINGYUAN_EQUIPSUIT_MAX_COUNT)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "slot", slot) || slot < 0 || slot >= QINGYUAN_EQUIPSUIT_SLOT_MAX_COUNT)
		{
			return -2;
		}

		QingyuanEquipSuitConfig &suit_cfg = m_qingyuan_equip_suit_cfg[type][slot];
		suit_cfg.type = type;
		suit_cfg.slot = slot;

		if (!PugiGetSubNodeValue(dataElement, "man_item", suit_cfg.man_item) || suit_cfg.man_item <= 0 || NULL == ITEMPOOL->GetItem(suit_cfg.man_item))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "woman_item", suit_cfg.woman_item) || suit_cfg.woman_item <= 0 || NULL == ITEMPOOL->GetItem(suit_cfg.woman_item))
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(dataElement, "color_limit", suit_cfg.color_limit) || suit_cfg.color_limit <= 0 || suit_cfg.color_limit >= ItemBase::I_COLOR_MAX)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(dataElement, "order_limit", suit_cfg.order_limit) || suit_cfg.order_limit < 0 || suit_cfg.order_limit >= ItemPool::MAX_EQUIPMENT_ORDER)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(dataElement, "banlv_add_per", suit_cfg.banglv_add_per) || suit_cfg.banglv_add_per < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(dataElement, "maxhp", suit_cfg.maxhp) || suit_cfg.maxhp < 0)
		{
			return -20;
		}

		if (!PugiGetSubNodeValue(dataElement, "gongji", suit_cfg.gongji) || suit_cfg.gongji < 0)
		{
			return -40;
		}

		if (!PugiGetSubNodeValue(dataElement, "fangyu", suit_cfg.fangyu) || suit_cfg.fangyu < 0)
		{
			return -50;
		}

		if (!PugiGetSubNodeValue(dataElement, "mingzhong", suit_cfg.mingzhong) || suit_cfg.mingzhong < 0)
		{
			return -60;
		}

		if (!PugiGetSubNodeValue(dataElement, "shanbi", suit_cfg.shanbi) || suit_cfg.shanbi < 0)
		{
			return -70;
		}

		if (!PugiGetSubNodeValue(dataElement, "baoji", suit_cfg.baoji) || suit_cfg.baoji < 0)
		{
			return -80;
		}

		if (!PugiGetSubNodeValue(dataElement, "jianren", suit_cfg.jianren) || suit_cfg.jianren < 0)
		{
			return -90;
		}

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitWeddingLivenessCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int seq = 0;
	int last_seq = -1;
	int last_liveness_var = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= WEDDING_MAX_LIVENESS_TYPE || seq != last_seq + 1)
		{
			return -1;
		}

		last_seq = seq;
		WeddingLivenessCfg::LivenessCfg &liveness_cfg = m_wedding_liveness_cfg.cfg_list[seq];
		liveness_cfg.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "liveness_var", liveness_cfg.liveness_var) || liveness_cfg.liveness_var <= last_liveness_var)
		{
			return -2;
		}

		last_liveness_var = liveness_cfg.liveness_var;

		if (!PugiGetSubNodeValue(data_element, "red_bag_num", liveness_cfg.red_bag_num) || liveness_cfg.red_bag_num < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "every_time_bind_gold", liveness_cfg.every_time_bind_gold) || liveness_cfg.every_time_bind_gold < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "every_turn_gather_limit", liveness_cfg.every_turn_gather_limit) || liveness_cfg.every_turn_gather_limit < 0)
		{
			return -5;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitLivenessGatherPosCfg(PugiXmlNode RootElement)
{
	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -10000;
	}

	while (!dataElement.empty())
	{
		LivenessGatherPosCfg pos_cfg;
		if (!PugiGetSubNodeValue(dataElement, "seq", pos_cfg.seq) || pos_cfg.seq < 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(dataElement, "x_pos", pos_cfg.x_pos) || pos_cfg.x_pos <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(dataElement, "y_pos", pos_cfg.y_pos) || pos_cfg.y_pos <= 0)
		{
			return -4;
		}

		m_liveness_gather_pos_vec.push_back(pos_cfg);

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

const LivenessGatherPosCfg *QingyuanConfig::GetLivenessGatherPosCfg(int seq)
{
	for (std::vector<LivenessGatherPosCfg>::iterator it = m_liveness_gather_pos_vec.begin(); it != m_liveness_gather_pos_vec.end(); it++)
	{
		if (seq == (*it).seq)
		{
			return &(*it);
		}
	}

	return NULL;
}

int QingyuanConfig::InitWeddingBlessingCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int blessing_type = 0;
	int last_blessing_type = -1;
	int seq = 0;
	int last_seq = -1;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "blessing_type", blessing_type) || blessing_type < 0 || blessing_type >= HUNYAN_ZHUFU_TYPE_MAX)
		{
			return -1;
		}

		if (blessing_type != last_blessing_type)
		{
			if (blessing_type != last_blessing_type + 1)
			{
				return -100;
			}

			last_seq = -1;
			last_blessing_type = blessing_type;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= WEDDING_BLESSING_MAX_CFG_NUM || seq != last_seq + 1)
		{
			return -2;
		}

		WeddingBlessingCfg::BlessingCfg &blessing_cfg = m_wedding_blessing_cfg_list[blessing_type].cfg_list[seq];
		blessing_cfg.seq = seq;
		last_seq = seq;

		if (!PugiGetSubNodeValue(data_element, "param", blessing_cfg.param1) || blessing_cfg.param1 <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "param2", blessing_cfg.param2) || blessing_cfg.param2 < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "add_liveness", blessing_cfg.add_liveness) || blessing_cfg.add_liveness < 0)
		{
			return -5;
		}

		{
			blessing_cfg.item_count = ItemConfigData::ReadConfigList(data_element, std::string("reward_item"),
				blessing_cfg.reward_item_list, QINGYAUN_MAX_ITEM_COUNT);
			if (blessing_cfg.item_count < 0)
			{
				return -100 + blessing_cfg.item_count;
			}
		}

		data_element = data_element.next_sibling();

	}

	return 0;
}

const WeddingBlessingCfg::BlessingCfg *QingyuanConfig::GetWeddingBlessingCfgBySeq(int blessing_type, int seq)
{
	if (blessing_type < 0 || blessing_type >= HUNYAN_ZHUFU_TYPE_MAX || seq < 0 || seq >= WEDDING_BLESSING_MAX_CFG_NUM)
	{
		return NULL;
	}

	return &m_wedding_blessing_cfg_list[blessing_type].cfg_list[seq];
}

int QingyuanConfig::InitWeddingYuYueTimeCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	int seq = 0;
	int last_seq = -1;
	int last_apply_time = 0;
	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "seq", seq) || seq < 0 || seq >= WEDDING_YUYUE_TIME_MAX || seq != last_seq + 1)
		{
			return -1;
		}

		last_seq = seq;

		WeddingYuYueTimeCfg::ConfigItem &cfg_item = m_hunyan_yuyue_time_cfg.cfg_list[seq];
		cfg_item.seq = seq;

		if (!PugiGetSubNodeValue(data_element, "apply_time", cfg_item.apply_time) || cfg_item.apply_time < 0 || cfg_item.apply_time <= last_apply_time)
		{
			return -2;
		}

		last_apply_time = cfg_item.apply_time;

		if (!PugiGetSubNodeValue(data_element, "begin_time", cfg_item.begin_time) || cfg_item.begin_time <= cfg_item.apply_time)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "end_time", cfg_item.end_time) || cfg_item.end_time <= cfg_item.begin_time)
		{
			return -4;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

const WeddingYuYueTimeCfg::ConfigItem *QingyuanConfig::GetWeddingYuYueTimeBySeq(int seq)
{
	if (seq < 0 || seq >= WEDDING_YUYUE_TIME_MAX) return NULL;

	return &m_hunyan_yuyue_time_cfg.cfg_list[seq];
}

int QingyuanConfig::InitWeddingRewardExpCfg(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_wedding_reward_exp_map.clear();

	int role_level = 0;
	int last_role_level = 0;

	while (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "role_level", role_level) || role_level <= last_role_level)
		{
			return -1;
		}

		last_role_level = role_level;

		WeddingRewardExpCfg cfg;

		if (!PugiGetSubNodeValue(data_element, "every_time_exp_0", cfg.reward_exp_list[YUYUE_HUNYAN_TYPE_0]) || cfg.reward_exp_list[YUYUE_HUNYAN_TYPE_0] <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "every_time_exp_1", cfg.reward_exp_list[YUYUE_HUNYAN_TYPE_1]) || cfg.reward_exp_list[YUYUE_HUNYAN_TYPE_1] <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "every_time_exp_2", cfg.reward_exp_list[YUYUE_HUNYAN_TYPE_2]) || cfg.reward_exp_list[YUYUE_HUNYAN_TYPE_2] <= 0)
		{
			return -4;
		}

		if (m_wedding_reward_exp_map.find(role_level) != m_wedding_reward_exp_map.end())
		{
			return -3;
		}

		m_wedding_reward_exp_map[role_level] = cfg;

		data_element = data_element.next_sibling();
	}

	return 0;
}

long long QingyuanConfig::GetWeddingAddExp(int level, int hunyan_type)
{
	if (hunyan_type <= YUYUE_HUNYAN_INVALUE || hunyan_type >= YUYUE_HUNYAN_TYPE_MAX)
	{
		return 0;
	}

	if (int(m_wedding_reward_exp_map.size()) <= 0) return 0;
	std::map<int, WeddingRewardExpCfg>::reverse_iterator rit;
	for (rit = m_wedding_reward_exp_map.rbegin(); rit != m_wedding_reward_exp_map.rend(); rit++)
	{
		if (level >= rit->first)
		{
			return rit->second.reward_exp_list[hunyan_type];
		}
	}

	return 0;
}

int QingyuanConfig::InitQingyuanEquip2Config(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	m_qingyuan_equip_cfg_vec.clear();

	while (!data_element.empty())
	{
		QingyuanEquip2Cfg cfg;

		if (!PugiGetSubNodeValue(data_element, "equip_index", cfg.equip_index) || cfg.equip_index < 0 || cfg.equip_index >= QINGYUAN_EQUIP_TYPE_MAX)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "equip_level", cfg.equip_level) || cfg.equip_level < 0 || cfg.equip_level > QINGYUAN_EQUIP2_MAX_LEVEL)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_id", cfg.stuff_id) || cfg.stuff_id <= 0 || NULL == ITEMPOOL->GetItem(cfg.stuff_id))
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "stuff_num", cfg.stuff_num) || cfg.stuff_num <= 0)
		{
			return -4;
		}

		int iRet = cfg.attr_cfg.ReadConfig(data_element);
		if (iRet < 0)
		{
			return iRet;
		}

		m_qingyuan_equip_cfg_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

const QingyuanEquip2Cfg *QingyuanConfig::GetQingyuanEquip2Cfg(int equip_index, int equip_level)
{
	QingyuanEquip2Cfg cfg_item;
	cfg_item.equip_index = equip_index;
	cfg_item.equip_level = equip_level;

	std::vector<QingyuanEquip2Cfg>::iterator iter = std::find(m_qingyuan_equip_cfg_vec.begin(), m_qingyuan_equip_cfg_vec.end(), cfg_item);
	if (iter != m_qingyuan_equip_cfg_vec.end())
	{
		return &(*iter);
	}

	return nullptr;
}

int QingyuanConfig::InitQingyuanGiftConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	//int lase_seq = -1;
	while (!data_element.empty())
	{

		QingyuanGiftReward cfg;
		if (!PugiGetSubNodeValue(data_element, "opengame_day", cfg.opengame_day) || cfg.opengame_day <= 0)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "seq", cfg.seq) || cfg.seq < 0 || cfg.seq >= QINGYUAN_GIFT_MAX_COUNT)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "need_gold", cfg.need_gold) || cfg.need_gold <= 0)
		{
			return -3;
		}

		{
			cfg.item_count = ItemConfigData::ReadConfigList(data_element, std::string("reward_item"),
				cfg.reward_item_list, QINGYAUN_MAX_ITEM_COUNT);
			if (cfg.item_count < 0)
			{
				return -100 + cfg.item_count;
			}
		}

		m_qingyuan_equip_gift_cfg_vec.push_back(cfg);

		data_element = data_element.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitXunyouConfig(PugiXmlNode RootElement)
{
	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -10000;
	}

	while (!data_element.empty())
	{

		int hunyan_type = -1;
		if (!PugiGetSubNodeValue(data_element, "hunyan_type", hunyan_type) || hunyan_type < 0 || hunyan_type >= YUYUE_HUNYAN_TYPE_MAX)
		{
			return -1;
		}

		XunyouCfg &cfg = m_xunyou_cfg[hunyan_type];
		cfg.hunyan_type = hunyan_type;

		if (!PugiGetSubNodeValue(data_element, "every_time_bind_gold", cfg.every_time_bind_gold) || cfg.every_time_bind_gold <= 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "red_bag_num", cfg.red_bag_num) || cfg.red_bag_num <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "per_day_gather_limit", cfg.per_day_gather_limit) || cfg.per_day_gather_limit <= 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "red_bag_drop_range", cfg.red_bag_drop_range) || cfg.red_bag_drop_range <= 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "free_sa_red_bag_count", cfg.free_sa_red_bag_count) || cfg.free_sa_red_bag_count < 0)
		{
			return -6;
		}

		if (!PugiGetSubNodeValue(data_element, "buy_sa_red_bag_count", cfg.buy_sa_red_bag_count) || cfg.buy_sa_red_bag_count < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "red_bag_need_gold", cfg.red_bag_need_gold) || cfg.red_bag_need_gold <= 0)
		{
			return -8;
		}

		if (!PugiGetSubNodeValue(data_element, "xunyou_red_bag_gather_id", cfg.xunyou_red_bag_gather_id) || cfg.xunyou_red_bag_gather_id <= 0)
		{
			return -9;
		}

		if (!PugiGetSubNodeValue(data_element, "gather_time_ms", cfg.gather_time_ms) || cfg.gather_time_ms <= 0)
		{
			return -10;
		}

		if (!PugiGetSubNodeValue(data_element, "free_give_flower_count", cfg.free_give_flower_count) || cfg.free_give_flower_count < 0)
		{
			return -11;
		}

		if (!PugiGetSubNodeValue(data_element, "buy_give_flower_count", cfg.buy_give_flower_count) || cfg.buy_give_flower_count < 0)
		{
			return -12;
		}

		if (!PugiGetSubNodeValue(data_element, "flower_need_gold", cfg.flower_need_gold) || cfg.flower_need_gold <= 0)
		{
			return -13;
		}

		if (!PugiGetSubNodeValue(data_element, "flower_id", cfg.flower_id) || cfg.flower_id <= 0 || NULL == ITEMPOOL->GetItem(cfg.flower_id))
		{
			return -101;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitXunyouPathConfig(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode dataElement = RootElement.child("data");
	if (dataElement.empty())
	{
		return -1000;
	}

	int pos_count = 0;
	int last_seq = -1;
	while (!dataElement.empty())
	{

		if (pos_count >= HUNYAN_XUNYOU_POINT_MAX_NUM)
		{
			return -100;
		}

		int pos_seq = -1;
		if (!PugiGetSubNodeValue(dataElement, "pos_seq", pos_seq) || pos_seq < 0 || pos_seq >= HUNYAN_XUNYOU_POINT_MAX_NUM || pos_seq != last_seq + 1)
		{
			return -1;
		}

		XunyouPathCfg &cfg = m_path_cfg[pos_seq];
		cfg.pos_seq = pos_seq;

		last_seq = pos_seq;

		if (!PugiGetSubNodeValue(dataElement, "stop_time_s", cfg.stop_time_s) || cfg.stop_time_s < 0)
		{
			return -2;
		}

		if (!cfg.xunyou_pos.ReadConfig(dataElement, "xunyou_pos"))
		{
			return -101;
		}

		++pos_count;

		m_xunyou_pos_count = pos_count;

		dataElement = dataElement.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitProfessGiftConfig(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	memset(m_profess_gift_list, 0, sizeof(m_profess_gift_list));

	while (!data_element.empty())
	{
		int gift_type = -1;

		if (!PugiGetSubNodeValue(data_element, "gift_type", gift_type) || gift_type < 0 || gift_type > QINGYUAN_PROFESS_GIFT_MAX_TYPE)
		{
			return -1;
		}

		QingyuanProfessGiftConfig &gift_cfg = m_profess_gift_list[gift_type];

		gift_cfg.gift_type = gift_type;

		if (!PugiGetSubNodeValue(data_element, "gift_effect", gift_cfg.gift_effect) || gift_cfg.gift_effect < 0)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "exp", gift_cfg.exp) || gift_cfg.exp < 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "score", gift_cfg.score) || gift_cfg.score < 0)
		{
			return -7;
		}

		if (!PugiGetSubNodeValue(data_element, "gift_id", gift_cfg.gift_id) || gift_cfg.gift_id < 0)
		{
			return -4;
		}

		if (!PugiGetSubNodeValue(data_element, "self_charm", gift_cfg.self_charm) || gift_cfg.self_charm < 0)
		{
			return -5;
		}

		if (!PugiGetSubNodeValue(data_element, "other_charm", gift_cfg.other_charm) || gift_cfg.other_charm < 0)
		{
			return -6;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitProfessGradeConfig(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	memset(m_profess_grade_list, 0, sizeof(m_profess_grade_list));

	int level_check = -1;
	while (!data_element.empty())
	{
		int level = -1;

		if (!PugiGetSubNodeValue(data_element, "level", level) || level < 0 || level > QINGYUAN_PROFESS_MAX_GRADE)
		{
			return -1;
		}

		if (level - level_check != 1)
		{
			return -100;	// 等级必须连续按1递增
		}
		level_check = level;

		m_profess_grade_list[level].level = level;

		if (!PugiGetSubNodeValue(data_element, "exp", m_profess_grade_list[level].exp) || m_profess_grade_list[level].exp <= 0)
		{
			return -2;
		}

		int iRet = m_profess_grade_list[level].attr_cfg.ReadConfig(data_element);
		if (iRet < 0)
		{
			return iRet;
		}

		data_element = data_element.next_sibling();
	}

	return 0;
}

int QingyuanConfig::InitProfessOtherConfig(PugiXmlNode RootElement)
{
	if (RootElement.empty())
	{
		return -10000;
	}

	PugiXmlNode data_element = RootElement.child("data");
	if (data_element.empty())
	{
		return -1000;
	}

	if (!data_element.empty())
	{
		if (!PugiGetSubNodeValue(data_element, "exp_add_per", m_profess_other_cfg.profess_exp_add_per) || m_profess_other_cfg.profess_exp_add_per < 0 || m_profess_other_cfg.profess_exp_add_per > 10000)
		{
			return -1;
		}

		if (!PugiGetSubNodeValue(data_element, "att_add_per", m_profess_other_cfg.profess_att_add_per) || m_profess_other_cfg.profess_att_add_per < 0 || m_profess_other_cfg.profess_att_add_per > 10000)
		{
			return -2;
		}

		if (!PugiGetSubNodeValue(data_element, "add_per_shizhuang_id", m_profess_other_cfg.add_per_shizhuang_id) || m_profess_other_cfg.add_per_shizhuang_id <= 0)
		{
			return -3;
		}

		if (!PugiGetSubNodeValue(data_element, "add_profess_share_per", m_profess_other_cfg.add_profess_share_per) || m_profess_other_cfg.add_profess_share_per < 0 || m_profess_other_cfg.add_profess_share_per > 10000)
		{
			return -4;
		}
	}
	return 0;
}

const QingyuanGiftReward *QingyuanConfig::GetGiftCfg(int opengame_day, int seq)
{
	if (opengame_day <= 0 || seq < 0 || seq >= QINGYUAN_GIFT_MAX_COUNT)
	{
		return NULL;
	}

	auto it = std::find_if(m_qingyuan_equip_gift_cfg_vec.begin(), m_qingyuan_equip_gift_cfg_vec.end(), [opengame_day, seq](const QingyuanGiftReward &cmp_item) {
		if (opengame_day <= cmp_item.opengame_day && seq == cmp_item.seq)
		{
			return true;
		}
		else
		{
			return false;
		}
	});

	if (it != m_qingyuan_equip_gift_cfg_vec.end())
	{
		return &(*it);
	}

	return NULL;
}

int QingyuanConfig::GetXunyouObjDis(int marryobj_seq)
{
	if (marryobj_seq < 0 || marryobj_seq >= m_hunyan_cfg.xunyou_marryobj_dis_list.Count()) return 0;

	int dis = *m_hunyan_cfg.xunyou_marryobj_dis_list.Value(marryobj_seq);
	return dis;
}

const XunyouCfg * QingyuanConfig::GetXunyouCfg(int hunyan_type)
{
	if (hunyan_type <= YUYUE_HUNYAN_INVALUE || hunyan_type >= YUYUE_HUNYAN_TYPE_MAX)
	{
		return nullptr;
	}

	return &m_xunyou_cfg[hunyan_type];
}

const Posi QingyuanConfig::GetXunyouPos(int pos_seq)
{
	if (pos_seq < 0  || pos_seq >= m_xunyou_pos_count)
	{
		return Posi(0, 0);
	}

	return m_path_cfg[pos_seq].xunyou_pos.GetPos();
}

int QingyuanConfig::GetXunyouPosStopTimeS(int pos_seq)
{
	if (pos_seq < 0 || pos_seq >= m_xunyou_pos_count)
	{
		return 0;
	}

	return m_path_cfg[pos_seq].stop_time_s;
}

const QingyuanProfessGiftConfig * QingyuanConfig::GetProfessGift(int gift_type)
{
	if (gift_type < 0 || gift_type > QINGYUAN_PROFESS_GIFT_MAX_TYPE) return nullptr;

	QingyuanProfessGiftConfig &cfg = m_profess_gift_list[gift_type];
	if (cfg.gift_type != gift_type) return nullptr;

	return &cfg;
}

const QingyuanProfessGradeConfig * QingyuanConfig::GetProfessGrade(int level)
{
	if (level < 0 || level > QINGYUAN_PROFESS_MAX_GRADE) return nullptr;

	QingyuanProfessGradeConfig &cfg = m_profess_grade_list[level];
	if (cfg.level != level) return nullptr;

	return &cfg;
}

const QingyuanMarryLevelConfig * QingyuanConfig::GetMarryLevelCfg(int level)
{
	if (level < 0 || level > QINGYUAN_MARRY_MAX_LEVEL) return NULL;

	QingyuanMarryLevelConfig &cfg = m_marry_level_cfg[level];
	if (cfg.level != level) return NULL;

	return &cfg;
}

const QingyuanEquipSuitConfig * QingyuanConfig::GetQingyuanEquipSuitCfg(int type, int slot)
{
	if (type < 0 || type >= QINGYUAN_EQUIPSUIT_MAX_COUNT) return NULL;
	
	if (slot < 0 || slot >= QINGYUAN_EQUIPSUIT_SLOT_MAX_COUNT) return NULL;

	QingyuanEquipSuitConfig &cfg = m_qingyuan_equip_suit_cfg[type][slot];
	if (cfg.slot != slot || cfg.type != type) return NULL;

	return &cfg;
}
