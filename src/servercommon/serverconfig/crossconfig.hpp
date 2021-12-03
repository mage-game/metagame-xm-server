#ifndef __CROSS_CONFIG_H__
#define __CROSS_CONFIG_H__

#include <string>

#include "servercommon/crossdef.hpp"
#include "servercommon/servercommon.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

/*
* ����������ͣ����ݲ�ͬ���Ϳ������ø��ĸ��������
* �ڵײ㷢Э��ʱҪ����������;������͵��Ǹ�cross����
*/
enum CROSS_FUNCTION_TYPE
{
	CROSS_FUNCTION_TYPE_MIN = 0,
	CROSS_FUNCTION_TYPE_COMMON = CROSS_FUNCTION_TYPE_MIN,							// ͨ�ÿ������
	CROSS_FUNCTION_TYPE_CROSS_RANK,													// ������а�

	// ����  =====================================================================================
	// ������������Ϊ���


	// �������  =================================================================================
	// ���������������Ϊ���
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

	bool CheckCrossServerHasCrossActivity(int cross_activity_type, int cross_type = -1); // ����Ӧ��������Ƿ������˿�����Ӧ�Ŀ���
	bool CheckCrossServerHasCrossRandActivity(int cross_rand_activity_type, int cross_type = -1); // ����Ӧ��������Ƿ������˿�����Ӧ�Ŀ������
	int GetCrossOpenDay(int activity_type);
	
private:
	CrossConfig();
	~CrossConfig();
	
	std::map<int, NetAddress> m_cross_config_map;									// cross����
	std::map<int, CrossTypeInfo> m_cross_function_type_map;							// CROSS_FUNCTION_TYPE��Ӧ��cross_type_infoӳ���ϵ
	int m_common_cross_type;														// Ĭ�Ͽ�����ܶ�Ӧ��cross_type

	bool m_is_open_cross;															// �Ƿ񿪷�cross
	bool m_is_hidden_server;														// �Ƿ����ط�

	int m_hidden_server_cross_type;													// ��ǰ���ط���Ӧ�Ŀ�����ͣ�������ط�����Ч��
};

#endif	// __CROSS_CONFIG_H__

