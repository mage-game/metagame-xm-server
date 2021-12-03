#ifndef __DYNAMIC_ADJUST_CONFIG_HPP__
#define __DYNAMIC_ADJUST_CONFIG_HPP__

#include "servercommon/configbase.h"
#include "servercommon/servercommon.h"
#include <string>
#include <vector>
#include <map>
#include "servercommon/pugixml/pugixml_adapter.hpp"

// 其他配置
struct DynamicAdjustOtherCfg
{
	int default_server_role_num = 0;
	int min_server_role_num = 0;
	int max_server_role_num = 0;
};

struct RefreshAdjustKeyObj 
{
	RefreshAdjustKeyObj() : scene_id(0), refresh_type(-1), refresh_id(0)
	{}

	RefreshAdjustKeyObj(int _scene_id, short _refresh_type, unsigned short _refresh_obj_id) :
		scene_id(_scene_id), refresh_type(_refresh_type), refresh_id(_refresh_obj_id)
	{}

	enum REFRESH_OBJ_TYPE
	{
		REFRESH_OBJ_TYPE_GATHER = 0,				//采集物
		REFRESH_OBJ_TYPE_COMMON_MONSTER = 1,		//小怪

		REFRESH_OBJ_TYPE_MAX_COUNT,
	};

	bool operator <(const RefreshAdjustKeyObj &rhs) const
	{
		return ConvertParamToLongLong(scene_id, refresh_type, refresh_id) < ConvertParamToLongLong(rhs.scene_id, rhs.refresh_type, rhs.refresh_id);
	}

	int scene_id;
	short refresh_type;
	unsigned short refresh_id;
};

struct RefreshAdjustRoleNumToTimeCfg
{
	RefreshAdjustRoleNumToTimeCfg() : scene_role_num(0), refresh_interval_ms(0)
	{}

	int scene_role_num;
	int refresh_interval_ms;
};

//玩家数刷新调整配置--------------------------------------------------------
struct ServerRoleNumRefreshAdjustCfg
{
	int server_role_num = 0;
	int refresh_time_rate = 0;
};

//渠道默认玩家数配置--------------------------------------------------------
struct AgentDefaultServerRoleNumCfg
{
	int default_server_role_num = 0;
// 	int min_server_role_num = 0;
// 	int max_server_role_num = 0;
};

class DynamicAdjustConfig : public ConfigBase
{
public:
	DynamicAdjustConfig();
	~DynamicAdjustConfig();

	bool Init(const std::string &path, std::string *err);

	const DynamicAdjustOtherCfg &GetAdjustOtherCfg() { return m_other_cfg; }

	bool IsAdjustRefreshTime(const RefreshAdjustKeyObj &obj);
	int GetRefreshAdjustInterval(const RefreshAdjustKeyObj &obj, int role_num);

	float GetRefreshAdjustTimeRate(int server_role_num);

	const AgentDefaultServerRoleNumCfg *GetAgentDefalutNumCfg(std::string plat_name);

private:
	int InitOtherCfg(PugiXmlNode root_element);
	int InitRefreshAdjustCfg(PugiXmlNode root_element);
	int InitServerRoleNumRefreshAdjustCfg(PugiXmlNode root_element);
	int InitAgentServerRoleNumDefaultCfg(PugiXmlNode root_element);

	DynamicAdjustOtherCfg m_other_cfg;

	using RefreshAdjustRoleNumToTimeCfgVec = std::vector<RefreshAdjustRoleNumToTimeCfg>;
	std::map<RefreshAdjustKeyObj, RefreshAdjustRoleNumToTimeCfgVec> m_refresh_adjust_cfg_map;

	std::vector<ServerRoleNumRefreshAdjustCfg> m_server_adjust_refresh_vec;

	std::map<std::string, AgentDefaultServerRoleNumCfg> m_plat_default_role_num_map;
};

#endif // __DYNAMIC_ADJUST_CONFIG_HPP__

