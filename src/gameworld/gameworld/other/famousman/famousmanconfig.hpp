#ifndef __FAMOUS_MAN_CONFIG_HPP__
#define __FAMOUS_MAN_CONFIG_HPP__

#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "other/achieve/title/titleconfig.hpp"

enum FAMOUNS_MAN_TYPE
{
	FAMOUSN_MAN_INVAILD = 0,

	FAMOUS_MAN_ROLE_LEVEL,		// �ȼ����ȴﵽX��
	FAMOUS_MAN_MOUNT_GRADE,		// �������ȴﵽX��
	FAMOUS_MAN_WING_GRADE,		// �������ȴﵽX��
	FAMOUS_MAN_EQUIP_GRADE,		// ���ȼ���һ��X�׺�ɫװ��
	FAMOUS_MAN_TOWER_LEVEL,		// ����֮������ͨ��X��
	FAMOUS_MAN_ACTIVE_XIANNV,	// ���ȼ���Ů��XXX

	FAMOUS_MAN_MAX_TYPE,
};

struct FamousManCfg
{
	FamousManCfg():famous_type(FAMOUSN_MAN_INVAILD), param_1(0), param_2(0){}

	int famous_type;
	int param_1;
	int param_2;
};

class FamousManConfig : public ConfigBase
{
public:

	FamousManConfig();
	~FamousManConfig();

	bool Init(const std::string &path, std::string *err);

	const FamousManCfg * GetFamousManCfg(int famous_type);

	int GetOpenTime() { return m_during_times; }

private:
	int InitFamousManCfg(PugiXmlNode RootElement);
	int InitOthernCfg(PugiXmlNode RootElement);

	FamousManCfg m_famous_man_cfg[FAMOUS_MAN_MAX_TYPE];
	int m_during_times;
};

#endif // __FAMOUS_MAN_CONFIG_HPP__

