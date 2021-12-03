// F-����-��������.xls
#ifndef __MOJIE_SKILL_CONFIG_HPP__
#define __MOJIE_SKILL_CONFIG_HPP__

#include <string>
#include "servercommon/configbase.h"
#include "servercommon/servercommon.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

struct MojieSkillActiveCfg
{
	MojieSkillActiveCfg() : skill_id(0), pata_layer(0)
	{}

	int skill_id;
	int pata_layer;
};

struct MojieSkillCfg 
{
	MojieSkillCfg() : skill_id(0), skill_type(0)
	{
		memset(param_list, 0, sizeof(param_list));
	}

	int skill_id;
	int skill_type;

	enum SKILL_PARAM
	{
		SKILL_PARAM_0 = 0,
		SKILL_PARAM_1 = 1,
		SKILL_PARAM_2 = 2,
		SKILL_PARAM_3 = 3,

		SKILL_PARAM_COUNT,
	};
	static const int MAX_PARAM_COUNT = 4;
	UNSTD_STATIC_CHECK(SKILL_PARAM_COUNT == MAX_PARAM_COUNT)

	int param_list[MAX_PARAM_COUNT];

//	static const int INVALID_PARAM = -1;
};

enum SKILL_TYPE
{
	SKILL_TYPE_0 = 0,							//����ʱ�и����ٻ����ף�����Ŀ�����X%�������˺���
	SKILL_TYPE_1 = 1,							//�ƶ��ٶ�+10%
	SKILL_TYPE_2 = 2,							//ÿ10��ָ�X%�������ֵ��
	SKILL_TYPE_3 = 3,							//�����ȡ+20%
	SKILL_TYPE_4 = 4,							//PVE�˺��ӳ�+X%
	SKILL_TYPE_5 = 5,							//�����˺�+X%�������˺�+Y%
	SKILL_TYPE_6 = 6,							//����X%��PVP����Y%
	SKILL_TYPE_7 = 7,							//����һ�������X%�������˺����Y%
	SKILL_TYPE_8 = 8,							//��������+X%�������ֿ�+Y%
	SKILL_TYPE_9 = 9,							//�ܵ�����Ч��Ӱ���5���ڣ���������Ч������ʱ������50%��
	SKILL_TYPE_10 = 10,							//����ʱ�и���ʹĿ���ƶ��ٶȽ�����X%������2��

	SKILL_TYPE_MAX,
};

class MojieSkillConfig : public ConfigBase
{
public:
	MojieSkillConfig();
	~MojieSkillConfig();

	static const int MOJIE_SKILL_COUNT = 40;

	bool Init(const std::string &configname, std::string *err);

	void SetActiveSkillFlag(unsigned long long &flag, int pata_layer);
	const MojieSkillCfg * GetSkillCfg(int skill_id);
	const int GetSkillTypeByLayer(int layer);

private:
	int InitActiveCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);

	bool CheckSkillParam();

	MojieSkillActiveCfg m_active_cfg_list[MOJIE_SKILL_COUNT];
	MojieSkillCfg m_skill_cfg_list[MOJIE_SKILL_COUNT];
};

#endif // __MOJIE_SKILL_CONFIG_HPP__
