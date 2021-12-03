#ifndef __HUGUOZHILI_CONFIG_H__
#define __HUGUOZHILI_CONFIG_H__

#include <map>
#include "servercommon/configcommon.h"

static const int ACTIVE_CFG_COUNT = 32;

// ����
struct HuguozhiliOtherConfig
{
	HuguozhiliOtherConfig(): injure_maxhp_per(0),buff_interval_s(0), buff_cd_s(0)
	{};

	int injure_maxhp_per;			// �������ޣ����Ѫ���İٷֱȣ�
	int buff_interval_s;			// buff����ʱ�䣨�룩
	int buff_cd_s;					// buff��ȴʱ�䣨�룬�Ӽ���ʱ���㣩
};


// ������ - ����
struct HuguozhiliActiveConfig
{
	HuguozhiliActiveConfig() : can_active_times(0), need_die_times(0)
	{};

	int can_active_times;			// �ɶһ����������
	int need_die_times;				// ��Ҫ��������
};

class HuguozhiliConfig : public ConfigBase
{
public:
	HuguozhiliConfig() {};
	~HuguozhiliConfig() {};

	bool Init(const std::string &configname, std::string *err);

	const HuguozhiliOtherConfig * GetOtherConfig() { return &m_other_cfg; }
	const HuguozhiliActiveConfig * GetActiveConfig(short exchange_times);
	int GetCanActiveTimes(int die_times);	// ��ȡ���Լ���Ĵ���

private:
	int InitOtherCfg(TiXmlElement* root_element);
	int InitActiveCfg(TiXmlElement* root_element);

	HuguozhiliOtherConfig m_other_cfg;
	std::map<int, HuguozhiliActiveConfig> m_active_cfglist;
};


#endif // __HUGUOZHILI_CONFIG_H__