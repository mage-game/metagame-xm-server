#ifndef SHENGXINGZHULI_CONFIG_HPP
#define	SHENGXINGZHULI_CONFIG_HPP

#include "item/itembase.h"
#include "servercommon/configcommon.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/configbase.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"
/*
�������ÿ��嵽һ��������Ǯ��
*/

static const int CHONGZHI_MAX_COUNT = 100;						// ��ֵ��λ���ø���

enum SHENGXINGZHULI_SYSTEM_TYPE
{
	SHENGXINGZHULI_SYSTEM_TYPE_BEGIN = 0,
	SHENGXINGZHULI_SYSTEM_TYPE_MOUNT = SHENGXINGZHULI_SYSTEM_TYPE_BEGIN,			// ����ϵͳ
	SHENGXINGZHULI_SYSTEM_TYPE_WING = 1,			// ���
	SHENGXINGZHULI_SYSTEM_TYPE_SHENGONG = 2,		// ��
	SHENGXINGZHULI_SYSTEM_TYPE_HALO = 3,			// �⻷
	SHENGXINGZHULI_SYSTEM_TYPE_SHENYI = 4,			// ����
	SHENGXINGZHULI_SYSTEM_TYPE_FIGHT_MOUNT = 5,		// ս��
	SHENGXINGZHULI_SYSTEM_TYPE_FOOT_PRINT = 6,		// �㼣

	SHENGXINGZHULI_SYSTEM_TYPE_COUNT,
};

static const int NEED_CHONGZHI_FIRST_COUNT = 3;
struct ShengxingzhuliOtherCfg
{
	ShengxingzhuliOtherCfg() : open_day(0), add_value(0)
	{
		memset(need_chongzhi, 0, sizeof(need_chongzhi));
	}

	int need_chongzhi[NEED_CHONGZHI_FIRST_COUNT];				// ������Ҫ��ֵ��Ǯ
	int open_day;					// �����ڼ��쿪��
	int add_value;					// ������ֵ��
	ItemConfigData reward_item_list[SHENGXINGZHULI_SYSTEM_TYPE_COUNT];		// ��Ӧ��������֮���͵ĵ���
};


struct ShengxingzhuliWeekdaySystem
{
	ShengxingzhuliWeekdaySystem() : monday_sys(0), tuesday_sys(0), wednesday_sys(0), thursday_sys(0), friday_sys(0), saturday_sys(0), sunday_sys(0)
	{
	}
	int monday_sys;					// ���һ��Ӧϵͳ
	int tuesday_sys;				// ��ݶ���Ӧϵͳ
	int wednesday_sys;				// �������Ӧϵͳ
	int thursday_sys;				// ����Ķ�Ӧϵͳ
	int friday_sys;					// ������Ӧϵͳ
	int saturday_sys;				// �������Ӧϵͳ
	int sunday_sys;					// ����ն�Ӧϵͳ
};

class ShengxingzhuliConfig : public ConfigBase
{
public:
	ShengxingzhuliConfig();
	bool Init(const std::string &configname, std::string *err);

	const ShengxingzhuliOtherCfg& GetShengxingzhuliOtherCfg() { return m_other_cfg; }
	const ShengxingzhuliWeekdaySystem * GetShengxingzhuliWeekdaySystemCfg() { return &m_weekday_system_cfg; }

	const int GetStageByChongzhi(int chongzhi);
	const int GetNeedChongZhiByStage(int stage);

private:
	ShengxingzhuliWeekdaySystem m_weekday_system_cfg;
	ShengxingzhuliOtherCfg m_other_cfg;
	int InitShengxingzhuliOtherConfig(PugiXmlNode RootElement);
	int InitShengxingzhuliWeekdaySystemConfig(PugiXmlNode RootElement);
};


#endif