#ifndef __LOCAL_CONFIG_H__
#define __LOCAL_CONFIG_H__

#include <set>
#include "servercommon/servercommon.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

union PlatTypeUnion
{
	int id;
	char spid[4];
};

class LocalConfig
{
public:
	static const int MAX_ALLOW_SERVER_ID_COUNT = 512;
	static const int SHENHE_SERVER_ID = 1501; // ��˷��Ĺ̶���ID

	static LocalConfig & Instance();
	static void GetPlatName(int plat_type, PlatName plat_name);

	bool Init(const std::string &path, std::string *err);
	
	int GetPlatType() { return m_plat_type; }
	const char * GetPlatName() { return m_plat_name; }
	void GetPlatName(PlatName plat_name);

	bool IsServerIdAllowed(int server_id);
	bool IsMergedServer();

	int GetMergeServerId();
	std::set<int> & GetAllowServerIdSet() { return m_allow_server_id_set; }

	const char * GetRandCfgName() { return m_rand_cfg_name;}

	//2019-03-20 ����������. ֻ����׿ר��1����
	bool IsIOSPlatForm() { return false; /* return m_is_ios_platform;*/ }
	int GetRandActZhuanfuType() { return 1; /*return m_rand_act_zhuanfu_type; */}

	bool IsShenheServer(); // �ж��Ƿ���˷�


	bool IsDevPlatForm() { return m_is_dev_platform; }

private:
	LocalConfig();
	~LocalConfig();

	PlatName m_plat_name;							// ƽ̨��

	int m_rand_act_zhuanfu_type;
	RandConfigName m_rand_cfg_name; 

	int m_plat_type;								// ƽ̨����
	bool m_is_check_server_id;						// �Ƿ����ID
	std::set<int> m_allow_server_id_set;			// ����ķ�ID

	bool m_is_ios_platform;							// �Ƿ���ƻ��ƽ̨

	bool m_is_dev_platform;							// �Ƿ��ǲ���ƽ̨
};

#endif	// __LOCAL_CONFIG_H__

