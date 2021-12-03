#ifndef _UPGRADESYSCONFIG_HPP_
#define _UPGRADESYSCONFIG_HPP_

#include "servercommon/pugixml/pugixml_adapter.hpp"
#include "servercommon/configbase.h"
#include "item/itempool.h"
#include <map>

enum UPGRADE_SYS_CRIT_TYPE
{
	UPGRADE_SYS_CRIT_TYPE_INVALID = 0,
	UPGRADE_SYS_CRIT_TYPE_MOUNT = 1,					//����
	UPGRADE_SYS_CRIT_TYPE_WING = 2,						//����
	UPGRADE_SYS_CRIT_TYPE_SHIZHUANG = 3,				//ʱװ
	UPGRADE_SYS_CRIT_TYPE_SHENBING = 4,					//���
	UPGRADE_SYS_CRIT_TYPE_FABAO = 5,					//����
	UPGRADE_SYS_CRIT_TYPE_FOOTPRINT = 6,				//�㼣
	UPGRADE_SYS_CRIT_TYPE_HALO = 7,						//�⻷
	UPGRADE_SYS_CRIT_TYPE_FIGHTMOUNT = 8,				//ս��
	UPGRADE_SYS_CRIT_TYPE_TOUSHI = 9,					//ͷ��
	UPGRADE_SYS_CRIT_TYPE_MASK = 10,					//����
	UPGRADE_SYS_CRIT_TYPE_YAOSHI = 11,					//����
	UPGRADE_SYS_CRIT_TYPE_QILINBI = 12,					//�����
	UPGRADE_SYS_CRIT_TYPE_LINGTONG = 13,				//��ͯ
	UPGRADE_SYS_CRIT_TYPE_LINGGONG = 14,				//�鹭
	UPGRADE_SYS_CRIT_TYPE_LINGQI = 15,					//����
	UPGRADE_SYS_CRIT_TYPE_SHENGONG = 16,				//��Ů�⻷
	UPGRADE_SYS_CRIT_TYPE_SHENYI = 17,					//��Ů����
	UPGRADE_SYS_CRIT_TYPE_FLYPET = 18,					//�ɳ�
	UPGRADE_SYS_CRIT_TYPE_WEIYAN = 19,					//β��

	UPGRADE_SYS_CRIT_TYPE_END
};

const int MAX_UPGRADE_CRIT_TYPE_COUNT = 20;
UNSTD_STATIC_CHECK(MAX_UPGRADE_CRIT_TYPE_COUNT == UPGRADE_SYS_CRIT_TYPE_END)

struct UpgradeSysCritStruct
{
	UpgradeSysCritStruct() : upgrade_exp(0), crit_rate(0), crit_value(0) {}

	int upgrade_exp;											//���׻�õľ���
	int crit_rate;												//���ױ�����,��ֱ�
	int crit_value;												//����ֵ
};

class UpgradeSysCritConfig : public ConfigBase
{
public:
	UpgradeSysCritConfig();
	~UpgradeSysCritConfig();

	bool Init(const std::string &configname, std::string *err);

	const UpgradeSysCritStruct* GetUpgradeSysCritConfigByType(int type);
	const int GetUpgradeSysCritRate(int type);
	const int GetUpgradeSysCritValue(int type);

private:
	
	int InitUpgradeSysCritConfig(PugiXmlNode RootElement);

	UpgradeSysCritStruct upgrade_sys_crit[MAX_UPGRADE_CRIT_TYPE_COUNT];
};

#endif /*_UPGRADESYSCONFIG_HPP_*/
