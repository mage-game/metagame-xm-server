#ifndef __SKILLDEF_HPP__
#define __SKILLDEF_HPP__

#include <memory>

#include "servercommon/servercommon.h"
#include "servercommon/serverdef.h"
#include "gamedef.h"

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#pragma pack(push, 4)

static const int EFFECT_INFO_PARAM_MAX = 12;				// Effect��Ϣ����������

struct EffectInfo											// Effect��Ϣ ������Э���� ע�����
{
	char			effect_type;							// EffectBase.EFFECT_TYPE
	char			product_method;							// EffectBase::PRODUCT_METHOD
	UInt16			product_id;								// ����ID ��ƷID ��
	unsigned int	unique_key;								// Ψһkey
	int				client_effect_type;						// �ͻ���Ч������
	char			merge_layer;							// ���Ӳ���
	char			reserve;
	short			param_count;
	long long		param_list[EFFECT_INFO_PARAM_MAX];		// ����
};

#pragma pack(pop)

static const int MAX_SKILL_LEVEL = 200;						// ���ļ��ܵȼ� 
static const int MAX_PET_SKILL_LEVEL = 18;					// ��������ܵȼ�
static const int MAX_ROLE_SKILL_LEVEL = 21;

enum COMMONSKILL_ID											// ��ͨ��������ID
{
	COMMONSKILL_ID_INVALID = 0,

	COMMONSKILL_ID_JIANLING,
	COMMONSKILL_ID_CIKE,
	COMMONSKILL_ID_BIXIAN,
	COMMONSKILL_ID_FAZUN,

	COMMONSKILL_ID_MAX,
};

static const int PET_COMMON_SKILL_ID = 500;					// ������ͨ����ID
static const int NUQI_SKILL_ID = 5;							// ŭ������ID
static const int MOSHOU_CHONGFENG_SKILL_ID = 520;			// ī�޳�漼��ID

static const int GREATE_SOLDIER_SKILL_1_ID = 600;			// ����1����ID
static const int GREATE_SOLDIER_SKILL_2_ID = 601;			// ����2����ID
static const int GREATE_SOLDIER_SKILL_3_ID = 602;			// ����3����ID
static const int GREATE_SOLDIER_SKILL_4_ID = 603;			// �������⼼��ID603
static const int GREATE_SOLDIER_SKILL_5_ID = 604;			// �������⼼��ID604
static const int GREATE_SOLDIER_SKILL_6_ID = 605;			// �������⼼��ID605
static const int GREATE_SOLDIER_SKILL_7_ID = 606;			// �������⼼��ID606

static const int WEDDINGRING_SKILL_ID_OFFSET = 80;			// ��似��IDƫ����

static const int DEFAULT_AOE_ATTACK_RANGE = 10;				// Ⱥ��Ĭ�Ϲ�����Χ
static const int DEFAULT_AOE_ENEMY_NUM = 7;					// Ⱥ��Ĭ�ϵ�������

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum ROLE_SHADOW_SKILL_COND
{
	ROLE_SHADOW_SKILL_COND_0,								// �������ȼ�0
	ROLE_SHADOW_SKILL_COND_1,								// �������ȼ�1
	ROLE_SHADOW_SKILL_COND_2,								// �������ȼ�2

	ROLE_SHADOW_SKILL_COND_MAX,
};

enum BOSS_SKILL_COND
{
	BOSS_SKILL_COND_HPLOW_PER = 0,							// Ѫ�����ڰٷֱ�
	BOSS_SKILL_COND_HPLOW_RANGE,							// ����һ����Ѫ����Χ
	BOSS_SKILL_COND_FIGHTTIMES,								// ս��ʱ�䴥��
	BOSS_SKILL_COND_ON_DIE,									// ����ʱ����
	BOSS_SKILL_COND_TOUCH,									// ����Ŀ��
	BOSS_SKILL_COND_NEAR_ROLE_DIE,							// �����������
	BOSS_SKILL_COND_BORNTIME,								// ����ʱ�䴥��
	BOSS_SKILL_COND_ANYWAY,									// ��ʱ�ͷ�
	BOSS_SKILL_COND_MAX,
};

static const int BOSS_SKILL_TYPE_COND_COUNT = 8;			// ͬ������ ���õ�������

static const int BOSS_SKILL_PARAM_COUNT = 4;				// ������������
static const int BOSS_SKILL_PARAM_SKILL_COUNT = 4;			// �������� ���������б����

struct BossSkillCondParam
{
	BossSkillCondParam()
		: cond_param_skill_count(0)
	{
		memset(cond_param_list, 0, sizeof(cond_param_list));
		memset(cond_param_skill_index_list, 0, sizeof(cond_param_skill_index_list));
	}

	int cond_param_list[BOSS_SKILL_PARAM_COUNT];							// ��������

	int cond_param_skill_count;												// �����������ܸ���
	int cond_param_skill_index_list[BOSS_SKILL_PARAM_SKILL_COUNT];			// �������������б�
};

struct BossSkillTypeCond
{
	BossSkillTypeCond() : count(0) {}

	int count;
	BossSkillCondParam lists[BOSS_SKILL_TYPE_COND_COUNT];
};

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

enum SKILL_ATTACH_EFFECT_TARGET
{
	SKILL_ATTACH_EFFECT_TARGET_INVALID = 0,

	SKILL_ATTACH_EFFECT_TARGET_BOSS,						// BOSS
	SKILL_ATTACH_EFFECT_TARGET_MONSTER,						// С��
	SKILL_ATTACH_EFFECT_TARGET_ALL_MONSTER,					// ȫ������
	SKILL_ATTACH_EFFECT_TARGET_ROLE_PET,					// ��Ҽ������
	SKILL_ATTACH_EFFECT_TARGET_ALL_CHARACTER,				// ȫ������
	SKILL_ATTACH_EFFECT_TARGET_ENEMY,						// ����

	SKILL_ATTACH_EFFECT_TARGET_MAX,
};

enum SKILL_ATTACH_EFFECT
{
	SKILL_ATTACH_EFFECT_INVALID = 0,

	SKILL_ATTACH_EFFECT_XUANYUN,							// ѣ��
	SKILL_ATTACH_EFFECT_JITUI,								// ����
	SKILL_ATTACH_EFFECT_CHENMO,								// ��Ĭ
	SKILL_ATTACH_EFFECT_CHIHUAN,							// �ٻ�

	SKILL_ATTACH_EFFECT_CALL_SAME_MONSTER,					// �ٻ���ͬ��
	SKILL_ATTACH_EFFECT_CALL_DIFF_MONSTER,					// �ٻ���ͬ��

	SKILL_ATTACH_EFFECT_INC_GONGJI,							// ���ӹ���
	SKILL_ATTACH_EFFECT_DEC_GONGJI,							// ���ٹ���
	SKILL_ATTACH_EFFECT_INC_FANGYU,							// ���ӷ���
	SKILL_ATTACH_EFFECT_DEC_FANGYU,							// ���ٷ���
	SKILL_ATTACH_EFFECT_INC_BAOJI,							// ���ӱ���

	SKILL_ATTACH_EFFECT_ADD_HP,								// ��Ѫ
	SKILL_ATTACH_EFFECT_DUR_ADD_HP,							// ������Ѫ

	SKILL_ATTACH_EFFECT_WUDI_PROTECT,						// �޵б��� ʹ��Χ�ڵ�ָ�������ܵ��������˺���Ϊ1

	SKILL_ATTACH_EFFECT_HURT_MAXHP_PERCENT,					// ��ָ����������������ѪֵA%+B���˺�

	SKILL_ATTACH_EFFECT_KILL_SELF,							// ��ɱ

	SKILL_ATTACH_EFFECT_CHG_GLOBAL_CD,						// ��ȫ��CD

	SKILL_ATTACH_EFFECT_HURT,								// �����˺�

	SKILL_ATTACH_EFFECT_DUR_BLOOD_NOMERGE,					// ���ӳ�����Ѫ ���ɵ���
	SKILL_ATTACH_EFFECT_DUR_BLOOD_MERGE,					// ���ӳ�����Ѫ �ɵ���

	SKILL_ATTACH_EFFECT_DUR_GRADUAL_BLOOD,					// ���ӳ���������Ѫ
	SKILL_ATTACH_EFFECT_FAZHEN_GRADUAL_BLOOD,				// ��������Ѫ

	SKILL_ATTACH_EFFECT_REDUCE_MP,							// ����������

	SKILL_ATTACH_EFFECT_RECOVER_ON_ATKED,					// ������Ѫ
	SKILL_ATTACH_EFFECT_REBOUND_HURT,						// �˺�����

	SKILL_ATTACH_EFFECT_JUHUAPIG_REBOUND,					// �ջ���ʽ����

	SKILL_ATTACH_EFFECT_CLEAR_FIGHT_EFFECT,					// ������ս��buff
	SKILL_ATTACH_EFFECT_CLEAR_ALL_FIGHT_EFFECT,				// �������ս��buff

	SKILL_ATTACH_EFFECT_ADD_HP_ON_ATTACK_OTHER,				// ������Ѫ

	SKILL_ATTACH_EFFECT_BLAST,								// �Ա�
	SKILL_ATTACH_EFFECT_SPECIAL_LOGIC,						// �ص����������߼�

	SKILL_ATTACH_SUPER_MIANYI,								// ���߶������ԣ���Ĭ������

	SKILL_ATTACH_DOUBLE_ATTR_BUFF,							// ˫����buff

	SKILL_ATTACH_ADD_HP,									// ����ʱ���ӱ��������������A%Ѫ��

	SKILL_ATTACH_ADD_SPECIAL_SHIELD,						// ���ӹ��˻���

	SKILL_ATTACH_EFFECT_MAX,
};

enum SKILL_FAZHEN_EFFECT_TYPE
{
	SKILL_FAZHEN_EFFECT_TYPE_INVALID = 0,

	SKILL_FAZHEN_EFFECT_TYPE_FAZHEN,						// ����
	SKILL_FAZHEN_EFFECT_TYPE_HALO,							// �⻷

	SKILL_FAZHEN_EFFECT_TYPE_MAX,
};

enum SKILL_EFFECT_SRC
{
	SKILL_EFFECT_SRC_SKILL = 0,								// Ч����Դ ����
	SKILL_EFFECT_SRC_FAZHEN,								// Ч����Դ ����
	SKILL_EFFECT_SRC_HALO,									// Ч����Դ �⻷
};

enum SKILL_PHASE
{
	SKILL_PHASE_READING = 0,								// ����
	SKILL_PHASE_PERFORM,									// �ͷ�
	SKILL_PHASE_CONTINUE_PERFORM,							// ����ʩ��
	SKILL_PHASE_END,										// ���ܽ���
};

enum SKILL_CHECK_FLAG
{
	SKILL_CHECK_FLAG_CD				= (0x1 << 0),			// ���CD
	SKILL_CHECK_FLAG_TARGET			= (0x1 << 1),			// ���Ŀ��
	SKILL_CHECK_FLAG_DISTANCE		= (0x1 << 2),			// ������
};

enum SKILL_SPECIAL_LOGIC_TYPE
{
	SKILL_SPECIAL_LOGIC_TYPE_INVALID = 0,

	SKILL_SPECIAL_LOGIC_TYPE_TOWERDEFEND_SCORE = 1,			// ��������

	SKILL_SPECIAL_LOGIC_TYPE_MAX,
};

static const int SKILL_ATTACH_MAX_EFFECT = 2;				// ���ܸ���Ч��
static const int SKILL_ATTACH_EFFECT_PARAM_COUNT = 6;		// ���ܸ���Ч�� ��������
static const int SKILL_ATTACH_EFFECT_MAX_TARGET = 32;		// ����Ч��Ŀ���������

class Character;
class Scene;

struct SkillAttachEffectParam
{
	static const int FAZHEN_BUFF_DUR_MS = 1500;				// ����ӵ�buff����ʱ��

	SkillAttachEffectParam() : effect_type(SKILL_ATTACH_EFFECT_INVALID)
	{
		memset(effect_param_list, 0, sizeof(effect_param_list));
	}

	bool CheckParamConfig(UInt16 skill_id, bool used_in_fazhen, bool is_range_skill, int effect_target, std::string &err);

	void Attach(Scene *scene, ObjID deliver_objid, const Posi &pos, short target_count, Character *target_list[SKILL_ATTACH_EFFECT_MAX_TARGET], 
				UInt16 skill_id, int effect_src = SKILL_EFFECT_SRC_SKILL, int param = 0) const;

	int effect_type;
	long long effect_param_list[SKILL_ATTACH_EFFECT_PARAM_COUNT];
};

#endif // __SKILLDEF_HPP__

