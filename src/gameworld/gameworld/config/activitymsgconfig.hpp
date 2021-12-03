#ifndef _ACTIVITY_MSG_CONFIG_HPP_
#define _ACTIVITY_MSG_CONFIG_HPP_

#include <map>
#include <vector>
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"


enum SEND_MSG_TYPE
{
	MSG_TYPE_BROADCAST_INVILIDE,
	MSG_TYPE_BROADCAST_SCENE,
	MSG_TYPE_BROADCAST_GUILD,
	MSG_TYPE_BROADCAST_GEMEWORLD,
	MSG_TYPE_BROADCAST_ALL_GAMEWORLD,
	MSG_TYPE_BROADCAST_MAX,
};

struct ActivityMsg
{
	ActivityMsg() :guilder_die(0), role_die(0), monster_attack(0), monster_die(0),lian_zhan(0)
	{}
	int guilder_die;
	int role_die; 
	int monster_attack;
	int monster_die;
	int lian_zhan;
};

struct RoleDeadCfg
{
	RoleDeadCfg()
	{ 
		dead_num.clear(); 
	}
	std::vector<int> dead_num;
};

struct MonsterAttackCfg
{
	MonsterAttackCfg() :notify_attack_cd(-1)
	{
		monster_id.clear();
		monster_hp.clear();
		hp_percent.clear();
	}
	int notify_attack_cd;
	std::vector<int> monster_id;
	std::vector<int> monster_hp;
	std::vector<int> hp_percent;
};

struct LianZhanCfg
{
	LianZhanCfg() :lianzhan_end(0)
	{
		lianzhan_num.clear();
	}
	int lianzhan_end;
	std::vector<int> lianzhan_num;
};

struct MonsterDeadCfg
{
	MonsterDeadCfg()
	{
		monster_id.clear();
	}
	std::vector<int> monster_id;
};

struct ActivityMsgOtherCfg
{
	ActivityMsgOtherCfg():guild_time(0),rank_top_interval(0){}
	int guild_time;
	int rank_top_interval;
};
class ActivityMsgConfig : public ConfigBase
{
public:
	ActivityMsgConfig(){}
	~ActivityMsgConfig(){}
	
	bool Init(const std::string &configname, std::string *err);
	ActivityMsg* GetActivityMsgType(int scene_type);

	int GetGuilderDieTime() const { return m_other_cfg.guild_time;}
	int GetRankTopInterval() const { return m_other_cfg.rank_top_interval; }
	bool CheckRoleDeadCount( const int scene_type, const int dead_count) const;
	int GetMonsterAttackNotifyCD(const int scene_type,const int monster_id) const;
	bool CheckRoleLianZhanCount(const int scene_type, const int lianzhan) const;
	bool CheckRoleLianZhanEnd(const int scene_type, const int lianzhan) const ;
	bool NeedMonsterHpNotify(const int scene_type, const int monster_id) const ;
	int CheckMonsterHpPercent(const int scene_type, const int old_percent, const int now_percent)const;
	int NeedMonsterDeadMsg(const int scene_type, const int monster_id) const;
	static int ReadConfig(PugiXmlNode dataElement, const char * name, std::vector<int> &veci);
private:
	int InitOtherCfg(PugiXmlNode rootElement);
	int InitFunctionOpenCfg(PugiXmlNode rootElement);
	int InitRoleDeadCfg(PugiXmlNode rootElement);
	int InitMonsterAttackCfg(PugiXmlNode rootElement);
	int InitLianZhanCfg(PugiXmlNode rootElement);
	int InitMonsterDeadCfg(PugiXmlNode rootElement);

	ActivityMsgOtherCfg m_other_cfg;
	std::map<int, ActivityMsg> m_function_open;
	std::map<int, RoleDeadCfg> m_role_dead_cfg;
	std::map<int, MonsterAttackCfg> m_monster_attack_cfg;
	std::map<int, LianZhanCfg> m_lian_zhan_cfg;
	std::map<int, MonsterDeadCfg> m_monster_dead_cfg;
	std::map<long long, unsigned int> m_monster_attack_time;
};
#endif
