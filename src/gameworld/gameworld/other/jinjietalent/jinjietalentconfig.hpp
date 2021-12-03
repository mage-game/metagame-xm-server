#ifndef __JINJIE_TALENT_CONFIG_HPP__
#define __JINJIE_TALENT_CONFIG_HPP__

#include <map>
#include <vector>

#include "servercommon/talentdef.hpp"
#include "servercommon/configbase.h"
#include "servercommon/configcommon.h"



struct JinJieActivateTalentGridCondCfg
{
	int need_grade;																//��Ҫ�׼�
	int pre_index1;																//ǰ����Ÿ��ӿ���
	int pre_index2;																//ǰ����Ÿ��ӿ���
	int pre_index3;																//ǰ����Ÿ��ӿ���
	int per_gird_quality;														//ǰ�ø���Ʒ��
};

struct JinJieTalenSkillParamCfg
{
	int skill_id;
	int skill_type;																//��������
	int book_id;																//�츳��
	int skill_quality;															//����Ʒ��
	int skill_star;																//�����Ǽ�
	int need_item_id;															//��������Ҫ�Ĳ���
	int need_item_count;														//��������Ҫ�Ĳ�������
	int forget_gold;															//������
	Attribute value;															//����ֵ
	int per;																	//������ֱ�
	int param1;																	//���⼼�ܲ���1
	int param2;																	//���⼼�ܲ���2
	int param3;																	//���⼼�ܲ���3
	int param4;																	//���⼼�ܲ���4
	int capability;																//����ս��
	int cd;																		//CDʱ��
};

struct JinJieTalentOtherCfg
{
	JinJieTalentOtherCfg() : free_count(0) {}

	int free_count;
};

typedef std::map<int, JinJieTalenSkillParamCfg> TalentSkillStarMp;
//typedef std::map<int, TalentSkillStarMp> TalentSkillQualityMp;

struct JinJieTalentOtherConfig
{
	std::map<int, std::pair<int,int> > tl_type_skill_final_skill;				//�ռ����� �츳����:[��������,����index]
	std::map<int, int> skill_tl_type_final_skill;								//�ռ����� ��������:�츳����
	std::map<int, JinJieActivateTalentGridCondCfg> activate_cond;						//���������Ҫ������
	std::map<int, TalentSkillStarMp> skill_info;								//������Ϣ {skill_id::{star:skill_param}}
	std::map<int, int> bookid_skillid;											//bookId��Ӧ�ļ���
	std::map<int, std::pair<int, int> > stage_array;								//�׶�:[min_count, max_count]
	std::map<int, std::map<int, std::pair<int,int> > > stage_skill_weight;		//�׶�:{skill:{min_weight,max_weight}}
	std::map<int, int> flush_cost_gold;											//ˢ�����ĵ�Ԫ��
	JinJieTalentOtherCfg other_cfg;
};

class JinJieTalentConfig : public ConfigBase
{
public:
	JinJieTalentConfig();
	~JinJieTalentConfig();

	bool Init(const std::string &configname, std::string *err);
	int InitTalentList(TiXmlElement *RootElement);
	int InitGridList(TiXmlElement *RootElement);
	int InitTalentSkill(TiXmlElement *RootElement);
	int InitChoujiangStage(TiXmlElement *RootElement);
	int InitSkillWeight(TiXmlElement *RootElement);
	int InitFlushCost(TiXmlElement *RootElement);
	int InitOther(TiXmlElement *RootElement);

	const JinJieTalentOtherConfig& GetTalentOtherCfg();

	const JinJieActivateTalentGridCondCfg* GetActivateGridCondCfg(int index);			//��������
	const JinJieTalenSkillParamCfg* GetTalenSkillInfoCfg(int skill_id, int star);		//���ܲ���
	int GetTalenSkillId(int skill_type, int skill_quality, int star);			//����
	short GetSkillByBookId(int book_id);										//��ȡ����id
	std::vector<int> RandomSkill(int stage,int count);							//�������
	bool IsFinalSkill(int skill_type);											//�Ƿ��ռ�����
	std::pair<int,int> GetTalentFinalSkill(int tl_type);						//�ռ�����
	short GetStage(int flush_count);											//���ݴ����ҽ׶�
	short GetFlushGold(int flush_number);										//��Ҫ��Ԫ��
	int GetSkillStarAllStuff(int skill_id, int skill_star, ItemConfigData* data, size_t max_count);
	
	const JinJieTalentOtherCfg* GetOtherCfg();

private:
	JinJieTalentOtherConfig m_other_cfg;
};

#endif //__TALENT_CONFIG_HPP__