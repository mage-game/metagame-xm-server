#ifndef __TIANSHUXUNZHU_CONFIG_HPP__
#define __TIANSHUXUNZHU_CONFIG_HPP__

#include <vector>
#include <set>

#include "servercommon/xml_adapter.hpp"
#include "servercommon/configcommon.h"
#include "gameworld/gameworld/item/itempool.h"
#include "servercommon/activitydef.hpp"

enum TIANSHUXZ_TYPE			//����Ѱ������
{
	TIANSHUXZ_TYPE_EXP = 0,	 // �������飬Ŀ��Ϊתְװ����
	TIANSHUXZ_TYPE_TUMO,	 // ��ħ���飬Ŀ��Ϊ����x��x��װ��
	TIANSHUXZ_TYPE_SHIXUE,	 // ��Ѫ���飬Ŀ��ɳ�
	TIANSHUXZ_TYPE_SHOUHU,	 // �ػ����飬Ŀ��Ϊ����x����װ��λ
	TIANSHUXZ_TYPE_BAIZHAN,	 // ��ս����, ����X����սװ��
	TIANSHUXZ_TYPE_SHENSHOU, // ��������, ��������

	TIANSHU_TYPE_MAX,
};

UNSTD_STATIC_CHECK(TIANSHU_TYPE_MAX <= TIANSHU_XUNZHU_MAX_TYPE_COUNT);

enum TIANSHUXZ_COND_TYPE	//����Ѱ��Ŀ������
{
	TIANSHUXZ_COND_INVILAD = 0,
	TIANSHUXZ_COND_ZHUANZI_EQUIP,	 //תְװ����(������������)
	TIANSHUXZ_COND_HAVE_EQUIP,		//����x��װ��y����������x��|y����
	TIANSHUXZ_COND_CHENGZHANG,		//�ɳ��������������ͣ�
	TIANSHUXZ_COND_FORGE,			//����x����װ��λ����λ|x�ף�
	TIANSHUXZ_COND_BAIZHAN,			//��ս
	TIANSHUXZ_COND_SHENSHOU,		//����
	TIANSHUXZ_COND_MAX,
}; 

enum CHENGZHANG_TIANSHU_TYPE		//����Ѱ���ɳ�������
{
	CHENGZHANG_TIANSHU_TYPE_FINISH_SHOUCHONG = 0,		// ����׳�
	CHENGZHANG_TIANSHU_TYPE_ZERO_BUY_RETURN,			// 0Ԫ�� 388
	CHENGZHANG_TIANSHU_TYPE_BUY_IMPGUARD,				// ����С��è
	CHENGZHANG_TIANSHU_TYPE_ACTIVE_WUQI_SHIZHUANG,		// ����1������ʱװ
	CHENGZHANG_TIANSHU_TYPE_TOUTZHIJIHUA,				// ����Ͷ�ʼƻ�(boss�򸱱�)
	CHENGZHANG_TIANSHU_TYPE_ZEROGIFT,					// ���Ǻ�װ
	CHENGZHANG_TIANSHU_TYPE_SUPPER_GIFT,				// �������
	CHENGZHANG_TIANSHU_TYPE_ONMARRY,					// ���

	CHENGZHANG_TIANSHU_TYPE_MAX,
};

UNSTD_STATIC_CHECK(CHENGZHANG_TIANSHU_TYPE_MAX < 31);

struct TianShuXZOtherCfg
{
	TianShuXZOtherCfg() : shixue_open_day_end(0){}
	int shixue_open_day_end;
};

struct TianShuXZGoalItem
{
	TianShuXZGoalItem() :seq(0), cond_type(0)
	{
	}
	int seq;
	int cond_type;
	std::vector<int> cond_param;
	ItemConfigData goal_reward[MAX_ATTACHMENT_ITEM_NUM];//����
};

struct TianShuXZGoalConfig
{
	TianShuXZGoalConfig() :type(-1),count(0){}

	int type;
	int count;
	TianShuXZGoalItem golal_list[TIANSHU_XUNZHU_SIGNLE_MAX_NUM];
};

struct TianshuXZSkillCfg
{
	TianshuXZSkillCfg() : type(0), isvalid(false), effect_per_val(0){}

	int type;
	bool isvalid;
	int effect_per_val;	// Ч����%��
};

class TianshuXZConfig : public ConfigBase
{
public:

	TianshuXZConfig();
	~TianshuXZConfig();

	bool Init(const std::string &configname, std::string *err);
	const TianShuXZOtherCfg & GetOtherCfg() { return m_other_cfg; }
	const TianShuXZGoalConfig * GetTianShuXZGoalConfig(int type) const;
	const TianShuXZGoalItem * GetTianShuShuXZGoalItem(int type, int seq) const;
	int GetTianShuCountByType(int type) const;
	const TianshuXZSkillCfg* GetSKillCfg(int type)const;
private:
	int InitOtherCfg(PugiXmlNode RootElement);
	int InitGoalCfg(PugiXmlNode RootElement);
	int InitSkillCfg(PugiXmlNode RootElement);
	
	TianShuXZOtherCfg m_other_cfg;

	int m_goal_count;
	TianShuXZGoalConfig m_goal_config[TIANSHU_TYPE_MAX];
	
	TianshuXZSkillCfg m_skill_config[TIANSHU_TYPE_MAX];
};

#endif	// __TIANSHUXUNZHU_CONFIG_HPP__