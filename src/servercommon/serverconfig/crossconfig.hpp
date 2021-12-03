#ifndef __CROSS_CONFIG_H__
#define __CROSS_CONFIG_H__

#include <string>

#include "servercommon/crossdef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

/*
* 跨服功能类型，根据不同类型可以配置跟哪个跨服交互
* 在底层发协议时要根据这个类型决定发送到那个cross连接
*/
enum CROSS_FUNCTION_TYPE
{
	CROSS_FUNCTION_TYPE_MIN = 0,
	CROSS_FUNCTION_TYPE_COMMON = CROSS_FUNCTION_TYPE_MIN,							// 通用跨服功能
	CROSS_FUNCTION_TYPE_CROSS_RANK,													// 跨服排行榜

	// 跨服活动  =====================================================================================
	// 跨服活动功能类型为活动号


	// 跨服随机活动  =================================================================================
	// 跨服随机活动功能类型为活动号
};

struct CrossTypeInfo
{
	CrossTypeInfo() : cross_type(-1), opengame_day(0) {}

	int cross_type;
	int opengame_day;
};

class CrossConfig
{
public:
	static CrossConfig & Instance();

	bool Init(const std::string &path, std::string *err);
	bool Reload(const std::string &path, std::string *err, bool *is_cross_addr_change, bool *is_open_cross_change);

	std::map<int, NetAddress> & GetCrossAddrMap() { return m_cross_config_map; }
	bool IsOpenCross() { return m_is_open_cross; }
	bool IsHiddenServer() { return m_is_hidden_server; }

	int GetHiddenServerCrossType() { return m_hidden_server_cross_type; }

	int GetCrossTypeWithCrossFuncType(int func_type);

	bool CheckCrossServerHasCrossActivity(int cross_activity_type, int cross_type = -1); // 检查对应跨服类型是否设置了开启对应的跨服活动
	bool CheckCrossServerHasCrossRandActivity(int cross_rand_activity_type, int cross_type = -1); // 检查对应跨服类型是否设置了开启对应的跨服随机活动
	int GetCrossOpenDay(int activity_type);
	
private:
	CrossConfig();
	~CrossConfig();
	
	std::map<int, NetAddress> m_cross_config_map;									// cross配置
	std::map<int, CrossTypeInfo> m_cross_function_type_map;							// CROSS_FUNCTION_TYPE对应的cross_type_info映射关系
	int m_common_cross_type;														// 默认跨服功能对应的cross_type

	bool m_is_open_cross;															// 是否开放cross
	bool m_is_hidden_server;														// 是否隐藏服

	int m_hidden_server_cross_type;													// 当前隐藏服对应的跨服类型（针对隐藏服才有效）
};

#endif	// __CROSS_CONFIG_H__

