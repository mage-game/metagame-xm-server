#ifndef __ROLEGLOABALCONFIG_H__
#define __ROLEGLOABALCONFIG_H__

#include <string>

#include "gamedef.h"
#include "servercommon/servercommon.h"

#include "obj/character/attribute.hpp"

class TiXmlElement;

namespace globalconfig
{
	static const int MAX_DEFAULT_SKILL_COUNT = 30;			// 默认技能最大个数

	struct RoleGlobalConfigForbidNeiwanAccount
	{
		RoleGlobalConfigForbidNeiwanAccount() : is_forbid(false), no_login_sec(0), chongzhi_max(0) {}

		bool is_forbid;										// 是否达到一定条件就封号内玩（以下是条件）
		unsigned int no_login_sec;							// 上次登录距离现在的最大秒数
		int chongzhi_max;									// 充值不超过的数量（元宝）
	};

	struct RoleForbidChannel
	{
		RoleForbidChannel() : role_level_limit(0), max_world_chat_count1(0), max_world_chat_count2(0) {}

		int role_level_limit;										
		int max_world_chat_count1;							
		int max_world_chat_count2;									
	};

	struct RoleContinueKillCfg
	{
		RoleContinueKillCfg() : kill_count(0), buff_time(0), gongji_add_per(0)
		{}

		int kill_count;
		int buff_time;
		int gongji_add_per;
	};

	class RoleGlobalConfig
	{
	public:
		RoleGlobalConfig();
		~RoleGlobalConfig();

		bool Init(const std::string &path, std::string *err);

		// 返回可以学习的默认技能的数量（支持多个默认技能）
		int GetDefaultSkillID(int prof, int default_skill_id_list[MAX_DEFAULT_SKILL_COUNT]);
		int GetFlyIncreaseSpeedPercent();
		bool GetIsForbidNeiwanAction();
		float GetProfHurtFactor(int attack_prof, int defend_prof);
		float GetProfMingZhongFactor(int prof);
		RoleGlobalConfigForbidNeiwanAccount & GetForbidNeiwanAccountCfg() { return m_forbid_neiwan_account; }
		RoleForbidChannel & GetRoleForbidChannel() { return m_forbid_channel; }
		RoleContinueKillCfg * GetContinueKillCfg(int kill_count);
		unsigned int GetTriggerContinueKillMaxInterval() { return m_trigger_continue_kill_interval_max; }

	public:

		Attribute m_cooldown;									// 全局CD
		int m_xinshuo_protected_level;							// 新手保护等级
		int m_upgrade_log_level;								// 升级作日志起始等级
		int m_peace_mode_interval;								// 切换到和平模式时间间隔
		int m_realive_interval;									// 复活限制时间间隔
		int m_in_fight_ms;										// 处于战斗状态的时间 豪秒
		Posi m_in_team_distance;								// 非离队状态矩形距离半径
		unsigned int m_recover_hp_interval;						// 非战斗状态回血间隔 秒
		int m_recover_hp_per;									// 非战斗状态回血万分比
		int m_recover_hp_fight_per;								// 战斗状态回血万分比

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		struct ProfCfg
		{
			ProfCfg() : default_skill_id_count(0)
			{
				memset(default_skill_id_list, 0, sizeof(default_skill_id_list));
			}

			int default_skill_id_count;
			int default_skill_id_list[MAX_DEFAULT_SKILL_COUNT];
		};
		ProfCfg * GetProfCfg(int prof);
		ProfCfg m_prof_cfg_list[PROF_TYPE_MAX];

		////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
		
		int m_fly_increase_speed_percent;									// 飞行模式提高速度百分比

		bool m_forbid_neiwan_action;										// 是否屏蔽内玩做一些操作，比如交易，发邮件，购买市场物品等等 

		RoleGlobalConfigForbidNeiwanAccount m_forbid_neiwan_account;		// 内玩封号条件
		RoleForbidChannel m_forbid_channel;									// 禁言条件

		unsigned int m_trigger_continue_kill_interval_max;					// 连斩触发最大时间间隔
		std::map<int, RoleContinueKillCfg> m_continue_kill_cfg_map;			// 连斩效果配置

	private:
		int InitContinueKillCfg(TiXmlElement *RootElement);
	};
};

#endif // __ROLEGLOABALCONFIG_H__

