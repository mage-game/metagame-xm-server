#ifndef __MARRY_CONFIG_HPP__
#define __MARRY_CONFIG_HPP__

#include <string>

#include "gamedef.h"
#include "servercommon/servercommon.h"
#include "servercommon/marrydef.hpp"
#include "servercommon/maildef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

#include "item/itemextern.hpp"

#include "gamedef.h"

struct MarryLevelShizhuangCfg
{
	MarryLevelShizhuangCfg() : prof(0), sex(0) {}

	short prof;
	short sex;
	ItemConfigData shizhuang_item;
};

struct MarryLevelCfg
{
	MarryLevelCfg() : need_intimacy(0), need_coin(0), need_gold(0), reward_count(0) {}

	bool Invalid() { return 0 == reward_count; }

	int need_intimacy;
	int need_coin;
	int need_gold;

	int reward_count;
	ItemConfigData reward_item_list[MARRY_REWARD_ITEM_MAX_NUM];

	int reward_shizhuang_count;
	MarryLevelShizhuangCfg reward_shizhuang_list[MARRY_LEVEL_REWARD_SHIZHUANG_NUM];
};

struct MarryXunYouPoint
{
	MarryXunYouPoint() : pos(0, 0), stop_time(0) {}

	Posi pos;
	int stop_time;
};

struct MarryHunyanBlessGift
{
	MarryHunyanBlessGift() : gift_coin(0), item_cost_gold(0) {}

	int gift_coin;
	int item_cost_gold;
	ItemConfigData gift_item;
};

struct MarryHunyanCfg
{
	MarryHunyanCfg() : scene_id(0), enter_pos(0, 0), add_exp(0), invite_rune_item_id(0), firework_item_id(0), firework_add_exp(0), bless_gift_count(0) {}

	int scene_id;
	Posi enter_pos;
	int add_exp;
	ItemID invite_rune_item_id;
	ItemID firework_item_id;
	int firework_add_exp;

	int bless_gift_count; 
	MarryHunyanBlessGift bless_gift_list[MARRY_HUNYAN_BLESS_GIFT_MAX_NUM];
};

struct BuyXunyouOperaCfg
{
	BuyXunyouOperaCfg() : need_gold(0), buy_times(0) {}

	int need_gold;
	int buy_times;
};

class MarryConfig : public ConfigBase
{
public:

	MarryConfig();
	~MarryConfig();

	bool Init(const std::string &path, std::string *err);

	bool GetMarryLevelCost(int marry_level, int *need_intimacy, int *need_coin, int *need_gold);
	int GetMarryXunyouOperaCount(int opera_type);
	int GetMarryXunyouPointStopTimeS(int point_index);
	const Posi GetMarryXunyouPointPos(int point_index);
	bool GetMarryHunyanEnterPos(int *scene_id, Posi *pos);
	MarryHunyanBlessGift * GetMarryHunyanBlessGift(int index);
	int GetMarryLevelReward(int marry_level, char prof, char sex, ItemConfigData reward_item_list[MAX_ATTACHMENT_ITEM_NUM]);
	ItemID GetMarryLevelHunJieItemID(int marry_level);
	int GetMarryXunyouMarryobjDis(int marryobj_seq);

	int GetMarryXunyouPointCount() { return m_point_count; }
	ItemID GetMarryXunyouHongbaoItemId() { return m_marry_xunyou_hongbao_item_id; }
	ItemID GetMarryXunyouFlowerItemId() { return m_marry_xunyou_flower_item_id; }
	int GetMarryHunyanAddExp() { return m_marry_hunyan_cfg.add_exp; }
	int GetMarrySceneId() { return m_marry_scene_id; }
	const Posi GetMarryBaiTianDiPos() { return m_bai_tiandi_pos; }
	ItemID GetMarryHunyanInviteRuneItemId() { return m_marry_hunyan_cfg.invite_rune_item_id; }
	ItemID GetMarryHunyanFireworkItemId() { return m_marry_hunyan_cfg.firework_item_id; }
	int GetMarryHunyanFireworkAddExp() { return m_marry_hunyan_cfg.firework_add_exp; }
	int GetMarryNeedIntimacy() { return m_marry_need_intimacy; }
	int GetMarryXunyouHongbaoDropNum() { return m_marry_xunyou_hongbao_drop_num; }
	int GetMarryXunyouHongbaoDropDis() { return m_marry_xunyou_hongbao_drop_dis; }
	int GetMarryXunyouAddExpInterval() { return m_marry_xunyou_add_exp_interval; }
	int GetMarryXunyouMarryobjCount() { return m_marry_xunyou_marryobj_count; }
	int GetMarryXunyouMarryobjSpeed() { return m_marry_xunyou_marryobj_speed; }
	ItemID GetMarryReserveItemId() { return m_marry_reserve_item_id; }
	bool GetBuyXunyouOperaCfg(int op_type, int *need_gold, int *buy_times);

private:

	MarryLevelCfg m_marry_level_cfg_list[MARRY_LEVEL_TYPE_MAX];

	int m_marry_xunyou_opera_count_list[MARRY_XUNYOU_OPERA_TYPE_MAX];
	ItemID m_marry_xunyou_hongbao_item_id;
	ItemID m_marry_xunyou_flower_item_id;
	ItemID m_marry_reserve_item_id;

	int m_point_count;
	MarryXunYouPoint m_marry_xunyou_point_list[MARRY_XUNYOU_POINT_MAX_NUM];

	MarryHunyanCfg m_marry_hunyan_cfg;

	int m_marry_scene_id;
	Posi m_bai_tiandi_pos;
	int m_marry_need_intimacy;
	int m_marry_xunyou_hongbao_drop_num;
	int m_marry_xunyou_hongbao_drop_dis;
	int m_marry_xunyou_add_exp_interval;
	int m_marry_xunyou_marryobj_speed;

	int m_marry_xunyou_marryobj_count;
	int m_marry_xunyou_marryobj_dis_list[MARRY_MARRYOBJ_MAX_NUM];

	BuyXunyouOperaCfg m_buy_xunyou_cfg_list[MARRY_XUNYOU_OPERA_TYPE_MAX];
};

#endif // __MARRY_CONFIG_HPP__


