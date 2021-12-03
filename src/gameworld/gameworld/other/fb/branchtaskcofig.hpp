#ifndef __BRANCH_TASK_FB_CONFIG__
#define __BRANCH_TASK_FB_CONFIG__

#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"
#include <map>

enum BRANCH_FB_TYPE
{
	BRANCH_FB_TYPE_INVALID = 0,

	BRANCH_FB_TYPE_1,
	BRANCH_FB_TYPE_2,
	BRANCH_FB_TYPE_3,
	BRANCH_FB_TYPE_4,
	BRANCH_FB_TYPE_5,

	BRANCH_FB_TYPE_MAX,
};

struct BranchFBCfg
{
	BranchFBCfg():scene_id(0), enter_pos(0, 0), branch_fb_type(0), monster_1(0), monster_2(0), monster_3(0), boss_monster(0),
		param_1(0), param_2(0), param_3(0), finish_param(0), mianshang_id(0), mianshang_pos(0, 0), mianshang_per(0), recover_id(0),
		recover_pos(0, 0), recover_per(0), recover_interval(0), recover_id_count(0), can_move(0), only_effect_boss(0),
		recover_by_attack(0)
	{}

	int scene_id;
	Posi enter_pos;
	int branch_fb_type;
	int monster_1;
	int monster_2;
	int monster_3;
	int boss_monster;
	int param_1;
	int param_2;
	int param_3;
	int finish_param;
	int mianshang_id;
	Posi mianshang_pos;
	int mianshang_per;
	int recover_id;
	Posi recover_pos;
	int recover_per;
	int recover_interval;
	short recover_id_count;
	short can_move;
	short only_effect_boss;
	short recover_by_attack;
};

class BranchTaskFBConfig
{
public:
	BranchTaskFBConfig();
	~BranchTaskFBConfig();

	bool Init(const std::string &configname, std::string *err);

	const BranchFBCfg * GetBranchFBConfig(int scene_id);

private:

	int InitBranchFbCfg(TiXmlElement *RootElement);

	std::map<int, BranchFBCfg> m_branch_fb_cfg_map;
};

#endif
