#ifndef __MOVE_CHESS_HPP__
#define __MOVE_CHESS_HPP__

#include <map>
#include <vector>
#include <string>

#include "protocal/msgshop.h"
#include "servercommon/maildef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

class Role;

static const int ITEM_LIST_MAX_COUNT = 50;

enum MOVE_CHESS_MODE
{
	MOVE_CHESS_MODE_NORMAL = 0,									
	MOVE_CHESS_MODE_SPECIAL = 1,									

	MOVE_CHESS_MODE_MAX,
};

enum MOVE_CHESS_POINT
{
	MOVE_CHESS_POINT_INVALID = 0,
	MOVE_CHESS_POINT_ONE,	
	MOVE_CHESS_POINT_TWO,	
	MOVE_CHESS_POINT_THREE,	
	MOVE_CHESS_POINT_FOUR,	
	MOVE_CHESS_POINT_FIVE,	
	MOVE_CHESS_POINT_SIX,

	MOVE_CHESS_POINT_MAX,	
};

struct MoveChessOtherCfg
{
	MoveChessOtherCfg():consume_gold_count(0),free_times_per_day(0),reset_time_per_day(0),reset_consume_gold(0),free_cd(0),replace_item_id(0){}

	int consume_gold_count;

	short free_times_per_day;
	short reset_time_per_day;
	int reset_consume_gold;
	int	free_cd;

	int replace_item_id;

	ItemConfigData consume_item;
	ItemConfigData reset_reward_item;
	ItemConfigData max_step_reward[MAX_ATTACHMENT_ITEM_NUM];
};

struct MoveChessRewardCfg
{
	MoveChessRewardCfg():is_broadcast(0),can_repeat(0),is_bind(1)
	{
		memset(rate_list,0,sizeof(rate_list));
	}

	ItemConfigData reward_item;
	short is_broadcast;
	short can_repeat;
	short is_bind;
	int rate_list[MOVE_CHESS_MODE_MAX];
};

struct MoveChessSpecialStepCfg
{
	MoveChessSpecialStepCfg() : is_notice(0), special_reward_grid(0)
	{}

	short is_notice;
	int special_reward_grid;
	ItemConfigData reward[MAX_ATTACHMENT_ITEM_NUM];
};

class MoveChess : public ConfigBase
{
public:
	MoveChess();
	~MoveChess();
	
	bool Init(const std::string &configname, std::string *err);

	void Shake(Role *role, short is_use_item);
	void BuyReset(Role *role);
	void SendShakePoint(Role* role, int shake_point);

private:
	bool GetReward(Role *role,int shake_point, bool is_bind);

	int InitMoveChessOtherCfg(PugiXmlNode RootElement);
	int InitMoveChessRewardCfg(PugiXmlNode RootElement);
	int InitMoveChessShakeCfg(PugiXmlNode RootElement);
	int InitMoveChessSpecialGirdCfg(PugiXmlNode RootElement);

	typedef std::map<short, MoveChessRewardCfg>::iterator MoveChessItemMapIt;

	MoveChessOtherCfg m_other_cfg;
	int m_max_step;
	std::map<short, MoveChessRewardCfg> m_reward_item_map;
	int m_shake_point_rate[MOVE_CHESS_POINT_MAX];
	std::map<int, MoveChessSpecialStepCfg> m_special_step;
	
};

#endif // __MOVE_CHESS_HPP__

