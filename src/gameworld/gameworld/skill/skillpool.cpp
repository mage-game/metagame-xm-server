#include "skillpool.hpp"

#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"

#include "skill/skillimpl/commonskill.hpp"
#include "skill/skillimpl/monsterskillimpl.hpp"
#include "skill/skillimpl/roleskillimpl.hpp"
#include "skill/skillimpl/jinglingskillimpl.hpp"
#include "skill/skillimpl/crosspastureskillmpl.hpp"
#include "skill/skillimpl/mojieskillmpl.hpp"
#include "skill/skillimpl/xiannvskillimpl.hpp"
#include "skillimpl/pinkequipskillmpl.hpp"
#include "skill/skillimpl/greatesoldierskillimpl.hpp"
#include "skill/skillimpl/giftharvestskillmpl.hpp"
#include "skill/skillimpl/weekendequipskillimpl.hpp"

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
SkillPool *gs_skill_pool_config = NULL;

SkillPool * SkillPool::Instance()
{
	if (NULL == gs_skill_pool_config)
	{
		gs_skill_pool_config = new SkillPool();
	}

	return gs_skill_pool_config;
}

SkillPool::SkillPool()
{
	memset(m_skill_list, 0, sizeof(m_skill_list));
	memset(m_prof_skill_count_list, 0, sizeof(m_prof_skill_count_list));
	memset(m_prof_skill_list, 0, sizeof(m_prof_skill_list));
}

SkillPool::~SkillPool()
{
	this->Release();
}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define INIT_PROF_SKILL\
	base_prof = skill->GetProfLimit() % PROF_TYPE_PROF_ZHUANZHI_BASE;\
	if (base_prof > PROF_TYPE_INVALID && base_prof <= PROF_BASE_TYPE_MAX)\
	{\
		int &prof_skill_count = m_prof_skill_count_list[base_prof];\
		if (prof_skill_count >= 0 && prof_skill_count < ROLE_PROF_SKILL_COUNT)\
		{\
			m_prof_skill_list[base_prof][prof_skill_count] = skill->GetSkillId();\
			++ prof_skill_count;\
		}\
	}\

#define INIT_SKILL(SKILL_CLASS)\
	skill = new SKILL_CLASS();\
	my_element = RootElement->FirstChildElement(skill->GetSkillName());\
	if (NULL == my_element)\
	{\
		*err = "Load LoadRoleSkill Error. INIT_SKILL 1 " ; \
		*err += skill->GetSkillName(); \
		delete skill;\
		Release();\
		return false;\
	}\
	dataElement = my_element->FirstChildElement("data");\
	while( dataElement )\
	{\
		if (!skill->LoadFromDataNode(dataElement, err)) \
		{\
			*err = "Load LoadRoleSkill Error. INIT_SKILL 2 " ; \
			*err += skill->GetSkillName(); \
			delete skill;\
			Release();\
			return false;\
		}\
		dataElement = dataElement->NextSiblingElement();\
	}\
	if (m_skill_list[skill->GetSkillId()] != 0)\
	{\
		*err = "Load LoadRoleSkill Error. INIT_SKILL 3 " ; \
		*err += skill->GetSkillName(); \
		delete skill;\
		Release();\
		return false;\
	}\
	m_skill_list[skill->GetSkillId()] = skill;\
	INIT_PROF_SKILL

#define INIT_NORMAL_SKILL(SKILL_CLASS)\
	skill = new SKILL_CLASS();\
	if (!skill->LoadFromDataNode(dataElement, err)) \
	{\
		delete skill;\
		Release();\
		return -1000;\
	}\
	if (m_skill_list[skill->GetSkillId()] != 0)\
	{\
		delete skill;\
		Release();\
		return -1001;\
	}\
	m_skill_list[skill->GetSkillId()] = skill;


#define INIT_MONSTER_SKILL(nodename, classname)\
	currentElement = RootElement->FirstChildElement(nodename);\
	if (!currentElement)\
	{\
		*err = configname + ": xml node error in node [" nodename "] ";\
		return false;\
	}\
	currentElement = currentElement->FirstChildElement("skill");\
	while( currentElement )\
	{\
		std::string skillpath;\
		\
		if (!GetNodeValue(currentElement, skillpath) )\
		{\
		*err = configname + ": xml node error in node [" nodename "->skill] ";\
		return false;\
		}\
		\
		int index = (int)skillpath.find('/') + 1;\
		int len   = (int)skillpath.find('.') - index;\
		std::string filename = skillpath.substr(index, len);\
		\
		skill = new classname(filename);\
		\
		if (!skill->LoadConfig((skill_dir + skillpath.substr(0, index)).c_str(), err))\
		{\
			delete skill;\
			Release();\
			return false;\
		}\
		if (0 != m_skill_list[skill->GetSkillId()])\
		{\
			char errinfo[128];\
			sprintf(errinfo,"%s: skill id[%d] %s is existed ",configname.c_str(), skill->GetSkillId(), m_skill_list[skill->GetSkillId()]->GetSkillName());\
			*err = errinfo;\
			delete skill;\
			Release();\
			return false;\
		}\
		m_skill_list[skill->GetSkillId()] = skill;\
		currentElement = currentElement->NextSiblingElement();\
	}
///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

bool SkillPool::Init(const char *config_dir, std::string *err)
{
	std::string skill_dir_str = std::string(config_dir) + "skill/";
	const char *skill_dir = skill_dir_str.c_str();

	if (!this->LoadRoleSkill(skill_dir, std::string("roleskill.xml"), err))
	{
		return false;
	}

	if (!this->LoadMonsterSkill(skill_dir, std::string("MonsterPetSkillManager.xml"), err))
	{
		return false;
	}

	if (!this->CheckSkillInit(err)) 
	{
		return false;
	}

	return true;
}

bool SkillPool::Reload(const char *config_dir, std::string *err)
{
	SkillPool *temp = new SkillPool();

	if (!temp->Init(config_dir, err))
	{
		delete temp; temp = NULL;
		return false;
	}

	if (NULL != gs_skill_pool_config) delete gs_skill_pool_config;

	gs_skill_pool_config = temp;

	return true;
}
 
const SkillBase * SkillPool::GetSkill(UInt16 skillid) const
{
	return m_skill_list[skillid];
}

UInt16 SkillPool::GetSkillId(const char &prof, int index) const
{
	int base_prof = prof % PROF_TYPE_PROF_ZHUANZHI_BASE;
	if (base_prof > PROF_TYPE_INVALID && base_prof <= PROF_BASE_TYPE_MAX && index >= 0 && index < ROLE_PROF_SKILL_COUNT)
	{
		return m_prof_skill_list[base_prof][index];
	}

	return 0;
}

bool SkillPool::CheckSkillInit(std::string *err) const
{
	return true;
}

bool SkillPool::LoadMonsterSkill(const std::string &skill_dir, const std::string &name, std::string *err)
{
	std::string configname = skill_dir+name;
	TiXmlDocument m_document;

	if (configname == "" || !m_document.LoadFile(configname))
	{
		*err = "Load MonsterSkillManger Error." ;
		*err += m_document.ErrorDesc();
		return false;
	}

	TiXmlElement *RootElement = m_document.RootElement();
	if (!RootElement)
	{
		*err = configname + ": xml node error in root";
		return false;
	}

	TiXmlElement *currentElement = NULL;
	SkillBase *skill = NULL;

	INIT_MONSTER_SKILL("CommonSkillToEnemy", MonsterCommonSkillToEnemy);
	INIT_MONSTER_SKILL("RangeCommonSkillToEnemyPos", MonsterRangeCommonSkillToEnemyPos);
	INIT_MONSTER_SKILL("CommonSkillToSelf", MonsterCommonSkillToSelf);
	INIT_MONSTER_SKILL("RangeCommonSkillToSelfPos", MonsterRangeCommonSkillToSelfPos);
	INIT_MONSTER_SKILL("FaZhenSkillToEnemy", MonsterFaZhenSkillToEnemyPos);
	INIT_MONSTER_SKILL("FaZhenSkillToSelf", MonsterFaZhenSkillToSelfPos);
	INIT_MONSTER_SKILL("SkillToEnemyEffectToOther", MonsterSkillToEnemyEffectToOther);
	INIT_MONSTER_SKILL("RandZoneSkillToSelfPos", MonsterMultiRangeCommonSkillToEnemyPos);
	INIT_MONSTER_SKILL("RectRangeSkillToEnemyPos", MonsterRectRangeCommonSkillToEnemyPos);

	return true;
}

void SkillPool::Release()
{
	for (int i = 0; i < MAX_SKILL_ID; ++i)
	{
		if (NULL != m_skill_list[i])
		{
			delete m_skill_list[i];
		}
	}

	memset(m_skill_list, 0, sizeof(m_skill_list));
}

bool SkillPool::LoadRoleSkill(const std::string &skill_dir, const std::string &name, std::string *err)
{
	std::string configname = skill_dir+name;
	TiXmlDocument m_document;

	if (configname == "" || !m_document.LoadFile(configname))
	{
		*err = "Load LoadRoleSkill Error." ;
		*err += m_document.ErrorDesc();
		return false;
	}

	TiXmlElement *RootElement = m_document.RootElement();
	if (!RootElement)
	{
		*err = configname + ": xml node error in root";
		return false;
	}

	char errinfo[1024] = {0};
	int iRet = 0;

	{
		// normal skill
		TiXmlElement *noraml_element = RootElement->FirstChildElement("normal_skill");
		if (NULL == noraml_element)
		{
			*err = configname + ": no [normal_skill].";
			return false;
		}

		iRet = LoadNormalSkill(noraml_element, err);
		if (iRet)
		{
			sprintf(errinfo,"%s: LoadNormalSkill failed %d", configname.c_str(), iRet);
			*err = errinfo;
			return false;
		}
	}

	SkillBase *skill = NULL;
	int base_prof = 0;

	TiXmlElement *my_element = NULL;
	TiXmlElement *dataElement = NULL;

	INIT_SKILL(RoleSkill6);
	INIT_SKILL(RoleSkill7);

	INIT_SKILL(RoleSkill41);
	INIT_SKILL(RoleSkill42);
	INIT_SKILL(RoleSkill43);
	INIT_SKILL(RoleSkill44);
	INIT_SKILL(RoleSkill45);
	INIT_SKILL(RoleSkill46);
	INIT_SKILL(RoleSkill47);

	INIT_SKILL(NuqiSkill101);
	INIT_SKILL(NuqiSkill201);
	INIT_SKILL(NuqiSkill301);
	INIT_SKILL(NuqiSkill401);

	INIT_SKILL(RoleSkill111);
	INIT_SKILL(RoleSkill121);
	INIT_SKILL(RoleSkill131);
	INIT_SKILL(RoleSkill141);
	
	INIT_SKILL(RoleSkill211);
	INIT_SKILL(RoleSkill221);
	INIT_SKILL(RoleSkill231);
	INIT_SKILL(RoleSkill241);

	INIT_SKILL(RoleSkill311);
	INIT_SKILL(RoleSkill321);
	INIT_SKILL(RoleSkill331);
	INIT_SKILL(RoleSkill341);

	INIT_SKILL(RoleSkill411);
	INIT_SKILL(RoleSkill421);
	INIT_SKILL(RoleSkill431);
	INIT_SKILL(RoleSkill441);


	INIT_SKILL(MojieSkill70);
	INIT_SKILL(MojieSkill71);
	INIT_SKILL(MojieSkill72);
	INIT_SKILL(MojieSkill73);

	INIT_SKILL(XiannvSkill80);
	INIT_SKILL(XiannvSkill81);
	INIT_SKILL(XiannvSkill82);
	INIT_SKILL(XiannvSkill83);
	INIT_SKILL(XiannvSkill84);
	INIT_SKILL(XiannvSkill85);
	INIT_SKILL(XiannvSkill86);

	// 名将技能
	INIT_SKILL(GreateSoldierSkill600);
	INIT_SKILL(GreateSoldierSkill601);
	INIT_SKILL(GreateSoldierSkill602);
	// 名将特殊技能
	INIT_SKILL(GreateSoldierSkill603);
	INIT_SKILL(GreateSoldierSkill604);
	INIT_SKILL(GreateSoldierSkill605);
	INIT_SKILL(GreateSoldierSkill606);

// 	INIT_SKILL(PetSkill146);
// 	INIT_SKILL(PetSkill147);
// 	INIT_SKILL(PetSkill148);
// 	INIT_SKILL(PetSkill149);
// 	INIT_SKILL(PetSkill150);

	return true;
}

int SkillPool::LoadNormalSkill(TiXmlElement *noraml_element, std::string *err)
{
	SkillBase *skill = NULL;

	TiXmlElement *dataElement = noraml_element->FirstChildElement("data");
	while (NULL != dataElement)
	{
		short skill_id = 0;
		if (!GetSubNodeValue(dataElement, "skill_id", skill_id))
		{
			*err += "no skill_id";
			return -1;
		}

		if (m_skill_list[skill_id] != NULL)
		{
			*err += "skill_id repeat";
			return -2;
		}

		switch (skill_id)
		{
		case 1:
			INIT_NORMAL_SKILL(NormalSkill1);
			break;
		case 2:
			INIT_NORMAL_SKILL(NormalSkill2);
			break;
		case 3:
			INIT_NORMAL_SKILL(NormalSkill3);
			break;
		case 4:
			INIT_NORMAL_SKILL(NormalSkill4);
			break;

		case 151:
			INIT_NORMAL_SKILL(JingLingSkill151);
			break;
		case 152:
			INIT_NORMAL_SKILL(JingLingSkill152);
			break;
		case 153:
			INIT_NORMAL_SKILL(JingLingSkill153);
			break;
		case 154:
			INIT_NORMAL_SKILL(JingLingSkill154);
			break;
		case 155:
			INIT_NORMAL_SKILL(JingLingSkill155);
			break;
		case 156:
			INIT_NORMAL_SKILL(JingLingSkill156);
			break;
		case 157:
			INIT_NORMAL_SKILL(JingLingSkill157);
			break;
		case 158:
			INIT_NORMAL_SKILL(JingLingSkill158);
			break;
		case 159:
			INIT_NORMAL_SKILL(JingLingSkill159);
			break;
		case 160:
			INIT_NORMAL_SKILL(JingLingSkill160);
			break;
		case 161:
			INIT_NORMAL_SKILL(JingLingSkill161);
			break;
		case 162:
			INIT_NORMAL_SKILL(JingLingSkill162);
			break;
		case 163:
			INIT_NORMAL_SKILL(JingLingSkill163);
			break;
		case 164:
			INIT_NORMAL_SKILL(JingLingSkill164);
			break;
		case 165:
			INIT_NORMAL_SKILL(JingLingSkill165);
			break;
		case 166:
			INIT_NORMAL_SKILL(JingLingSkill166);
			break;

		case 170:
			INIT_NORMAL_SKILL(PinkEquipSkill170);
			break;
		case 171:
			INIT_NORMAL_SKILL(PinkEquipSkill171);
			break;
		case 172:
			INIT_NORMAL_SKILL(PinkEquipSkill172);
			break;

		case 180:
			INIT_NORMAL_SKILL(NormalSkill180);
			break;
		case 181:
			INIT_NORMAL_SKILL(NormalSkill181);
			break;

		case 182:
			INIT_NORMAL_SKILL(NormalSkill182);
			break;

		case 183:
			INIT_NORMAL_SKILL(NormalSkill183);
			break;

		case 184:
			INIT_NORMAL_SKILL(NormalSkill184);
			break;

		case 185:
			INIT_NORMAL_SKILL(NormalSkill185);
			break;

		case 186:
			INIT_NORMAL_SKILL(NormalSkill186);
			break;

		case 187:
			INIT_NORMAL_SKILL(NormalSkill187);
			break;

		case 188:
			INIT_NORMAL_SKILL(NormalSkill188);
			break;

		case 189:
			INIT_NORMAL_SKILL(NormalSkill189);
			break;

		// 魔卡技能暂时没用
		//case 200:
		//	INIT_NORMAL_SKILL(MagicCardSkill200);
		//	break;
		//case 201:
		//	INIT_NORMAL_SKILL(MagicCardSkill201);
		//	break;
		//case 202:
		//	INIT_NORMAL_SKILL(MagicCardSkill202);
		//	break;
		//case 203:
		//	INIT_NORMAL_SKILL(MagicCardSkill203);
		//	break;

		case 250:
			INIT_NORMAL_SKILL(CrossPastureSkill250);
			break;
		case 251:
			INIT_NORMAL_SKILL(CrossPastureSkill251);
			break;
		case 252:
			INIT_NORMAL_SKILL(CrossPastureSkill252);
			break;
		case 253:
			INIT_NORMAL_SKILL(CrossPastureSkill253);
			break;

		// 周末装备主动技能
		//case 610:
		//	INIT_NORMAL_SKILL(WeekendEquipSkill610);
		//	break;
		//case 611:
		//	INIT_NORMAL_SKILL(WeekendEquipSkill611);
		//	break;
		//case 612:
		//	INIT_NORMAL_SKILL(WeekendEquipSkill612);
		//	break;

		case 701:
			INIT_NORMAL_SKILL(GiftHarvestSkill701);
			break;

		case 702:
			INIT_NORMAL_SKILL(GiftHarvestSkill702);
			break;

		case 703:
			INIT_NORMAL_SKILL(GiftHarvestSkill703);
			break;

		// 名将的特殊技能
		//case 603:
		//	INIT_NORMAL_SKILL(GreateSoldierSkill603);
		//	break;
		//case 604:
		//	INIT_NORMAL_SKILL(GreateSoldierSkill604);
		//	break;

		default:
			*err += "skill_id unknown";
			return -100;
		}	

		dataElement = dataElement->NextSiblingElement();
	}

	return 0;
}

