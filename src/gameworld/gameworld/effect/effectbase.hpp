#ifndef __SKILLEFFECT_HPP__
#define __SKILLEFFECT_HPP__

#include "common/tlvprotocol.h"
#include "skill/skilldef.hpp"
#include "effect/buffmanager.hpp"

class Character;
class Role;

class EffectBase
{
public:
	enum MERGE_RULE
	{
		MERGE_RULE_INVALID = -1,

		MERGE_RULE_MERGE,					// ��ɫ��ͬ �����޹� Ч��������ͬ������
		MERGE_RULE_COUNTER,					// ��ɫ��ͬ ������ͬ ������Ч��  ������	

		MERGE_RULE_NOMERGE_1,				// ��ɫ�޹� �����޹� Ч��������ͬ��ȡʣ��ʱ�����											  ------- ������

		MERGE_RULE_NOMERGE_2,				// ��ɫ�޹� ������ͬ Ч��������ͬ���ȼ��ߵ��滻�͵ģ��ȼ���ͬ��ȡʣ��ʱ����ģ���������µ�		
											// ��ɫ�޹� ���ܲ�ͬ Ч��������ͬ���򹲴档													  ------- ������

		MERGE_RULE_NOMERGE_3,				// ��ɫ�޹� �����޹� Ч��������ͬ����ֵ�ߵ��滻�͵ģ���ֵ��ͬ��ȡʣ��ʱ����ģ���������µ� ------- �ٶ���

		MERGE_RULE_NOMERGE_4,				// ��ɫ��ͬ ������ͬ Ч��������ͬ���ȼ��ߵ��滻�͵ģ��ȼ���ͬ��ȡʣ��ʱ����ģ���������µ�
											// ��ɫ�ͼ�����һ����ͬ			 ���򹲴档                                                   ------- �����˺���
		MERGE_RULE_NOMERGE_5,				// ��ɫ��ͬ ������ͬ Ч��������ͬ: ͬ�ȼ��£�ʱ���ۼ�, �µĵȼ����ھɵģ��滻����������µġ�

		MERGE_RULE_NOMERGE_6,				// ��ɫ��ͬ ������ͬ Ч��������ͬ: ͬ�ȼ��£������ۼ�, �µĵȼ����ھɵģ��滻����������µġ�

		MERGE_RULE_IGNORE,					// �����µ�
	};

	enum EXIST_METHOD
	{
		EXIST_METHOD_MERGE = 0,									// ����
		EXIST_METHOD_REPLACE,									// �滻
		EXIST_METHOD_NOTHING,									// ����
		EXIST_METHOD_IGNORE,									// ����
	};

	enum PRODUCT_METHOD
	{
		PRODUCT_METHOD_SKILL = 0,								// �����ͷ�
		PRODUCT_METHOD_SCENE_SKILL,								// ����skill
		PRODUCT_METHOD_SYSTEM,									// ϵͳ ����BOSS���޵�״̬��
		PRODUCT_METHOD_TRIGGER,									// ������
		PRODUCT_METHOD_FRIEND,									// ����
		PRODUCT_METHOD_HUSONG,									// ����
		PRODUCT_METHOD_REBOUNDHURT,								// �˺�����
		PRODUCT_METHOD_FAZHEN_HALO,								// ����͹⻷
		PRODUCT_METHOD_SHENSHI,									// ������ʯ
		PRODUCT_METHOD_CROSS_XIULUO_TOWER_DUR_DEAD,				// �������������
		PRODUCT_METHOD_CROSS_XIULUO_TOWER_BUY_BUFF,				// ���������Buff
		PRODUCT_METHOD_CROSS_1V1,								// ���1V1
		PRODUCT_METHOD_NAME_COLOR,								// ����
		PRODUCT_METHOD_GUILD_HALL,								// ���˽���
		PRODUCT_METHOD_ITEM,									// ��Ʒ
		PRODUCT_METHOD_GONGCHENGZHAN,							// ����ս
		PRODUCT_METHOD_SKILL_READDING,							// ���ܶ����ͷ�
		PRODUCT_METHOD_JINGHUA_HUSONG,							// ��������
		PRODUCT_METHOD_CROSS_TEAM_FB_BUFF,						// �����ӱ�
		PRODUCT_METHOD_XIANNV_SHENGWU_SKILL,					// ��Ůʥ�＼��
		PRODUCT_METHOD_IMG_FULING,								// ������
		PRODUCT_METHOD_CROSS_PASTURE_ANIMAL_DEC_SPEED,			// �����������
		PRODUCT_METHOD_BIANSHEN_TASK,							// ��������
		PRODUCT_METHOD_TALENT_SKILL,							// �츳����
		PRODUCT_METHOD_IMP_GURAD_EXP,                           // �ػ�С�� - ����ӳ�
		PRODUCT_METHOD_ZHUANZHI_SKILL,							// תְ����
		PRODUCT_METHOD_LINGCHONG,								// ����չ�
		PRODUCT_METHOD_GCZCZ_SKILL,								// ����ս��ɱ����	
		PUODUCT_METHOD_GCZ_SKILL,								// ����ս���ܳ���Ч��
		PUODUCT_METHOD_SHENZHOU_WEAPON_SKILL,					// �����
		PRODUCT_METHOD_MOJIE_SKILL,								// ����-ħ�似��
		PRODUCT_METHOD_PER_BAOJI,								// ������
		PRODUCT_METHOD_PER_SHANBI,								// ������
	};

	// ����productidΪ�Զ����id.Ϊȷ�����԰�ȫ����Ӧ���뼼��id�ظ�
	enum PRODUCT_ID
	{
		PRODUCT_ID_GATHER_SPEED = 10000,		// �ɼ��ٶ�����
		PRODUCT_ID_GATHER_NOT_STOP,				// �ɼ������
		PRODUCT_ID_TO_BUILDINF_INC_HURT,		// �Խ����������˺�
		PRODUCT_ID_HP_CONTINUE,					// ��Ѫ�����ظ�
		PRODUCT_ID_MP_CONTINUE,					// mp�����ظ�
		PRODUCT_ID_INC_PUTON_LEVEL,				// ���Ӵ����ȼ�
		PRODUCT_ID_INC_EXTRAL_EXP_PER,			// �������ӳɰٷֱ�
		PRODUCT_ID_INC_INHURT_ON_ATK,			// ����ʱ��߱����˺�
		PRODUCT_ID_HP_DAN,						// ��Ѫ���ظ���Ѫ
		PRODUCT_ID_SZ_PROTECT,					// ��װ����
		PRODUCT_ID_JINGHUAHUSONG,				// ��������
		PRODUCT_ID_NAMECOLOR,					// ����
		PRODUCT_ID_GREATE_SOLDIER_PASSIVE_SKILL,// ������������
		PRODUCT_ID_GREATE_SOLDIER_SPECIAL_SKILL,// �������⼼��
		PRODUCT_ID_HUGUOZHILI,					// ������

		PRODUCT_ID_WEEKEND_EQUIP_PASSIVE_SKILL1,// ��ĩװ����������1
		PRODUCT_ID_WEEKEND_EQUIP_PASSIVE_SKILL2,// ��ĩװ����������2
		PRODUCT_ID_WEEKEND_EQUIP_PASSIVE_SKILL3,// ��ĩװ����������3

		PRODUCT_ID_GCZ_ZHIMINGYIJI = 10100,		// ����ս������һ��
		PRODUCT_ID_GCZ_YOUDIANYIJI,				// ����ս������һ��
		PRODUCT_ID_GCZ_POZHENGYIJI,				// ����ս������һ��
		PRODUCT_ID_GCZ_NEXT_CHENGMO,			// ����ս����һ�ι�����Ĭ
		PRODUCT_ID_GCZ_NEXT_DINGSHENG,			// ����ս����һ�ι�������
		PRODUCT_ID_GCZ_NEXT_XUANYUN,			// ����ս����һ�ι���ѣ��

		PRODUCT_ID_BASE_ATTR_BEGIN = 10200,
		PRODUCT_ID_BASE_ATTR_ADD_JINGZHUN,		// ���Ӿ�׼�̶�ֵ
		PRODUCT_ID_BASE_ATTR_ADD_BAOJI,			// ���ӱ����̶�ֵ
		PRODUCT_ID_BASE_ATTR_END,

		PRODUCT_ID_IMP_GURAD_EXP,              // �ػ�С�� - ����

		PRODUCT_ID_MASK_FANQIAO,			   // ���

		PRODUCT_ID_MOJIE_SKILL,					// ħ�似��(��MojieSkill������MojieSkillImpl)

		PRODUCT_ID_CONTINUE_KILL,				// ��ն

		PRODUCT_ID_DAILY_TASK_ACCEPT,			// �ճ�����
	};

	enum CMP_RET
	{
		CMP_RET_BIGGER,											// ����
		CMP_RET_EQUAL,											// ���
		CMP_RET_SMALLER,										// С��
		CMP_RET_NOTHING,										// �޹�
	};

	enum FIGHT_TYPE
	{
		FIGHT_TYPE_SHANBI = 0,									// ����
		FIGHT_TYPE_NORMAL,										// ��������
		FIGHT_TYPE_GEDANG,										// ��
		FIGHT_TYPE_BAOJI,										// ����
		FIGHT_TYPE_MIANYI,										// ����
		FIGHT_TYPE_DIKANG,										// �ֿ�
		FIGHT_TYPE_POJI,										// �ƻ�
		FIGHT_TYPE_IGNORE_DEFENSE,								// ����-���ӷ���
		FIGHT_TYPE_SHEHUN_DUOPO,								// ������
		FIGHT_TYPE_TALENT_ZHIXIN,								// �츳����
		FIGHT_TYPE_TALENT_XIXING,								// �츳����
		FIGHT_TYPE_TALENT_ZHENDANGFAJIAN,						// �츳�𵴷���
		FIGHT_TYPE_TALENT_QIAODUOTIANGONG,						// �츳�ɶ��칤
		FIGHT_TYPE_TALENT_ZHUIHUNDUOHUN,						// �츳׷�����
		FIGHT_TYPE_HUIXINYIJI,									// ����һ��
		FIGHT_TYPE_LINGCHONG,									// ����˺�
		FIGHT_TYPE_GREATE_SOLDIER,								// ��������
	};

	enum PASSIVE_FLAG
	{
		PASSIVE_FLAG_ZHIBAO_TIANLEI,							// ��������
		PASSIVE_FLAG_ZHIBAO_NULEI,								// ����ŭ��
		PASSIVE_FLAG_MOUNT_JIANTA,								// �����̤
		PASSIVE_FLAG_MOUNT_NUTA,								// ����ṳ̆
		PASSIVE_FLAG_SHENGONG_SHENGA,							// ����
		PASSIVE_FLAG_SHENGONG_GUANGJIAN,						// �񹭹��
		PASSIVE_FLAG_SHENGONG_ZHUMO,							// ����ħ
		PASSIVE_FLAG_SHENYI_HUANYING,							// �����Ӱ
		PASSIVE_FLAG_XIANNV_1,									// ��Ů1
		PASSIVE_FLAG_XIANNV_2,									// ��Ů2
		PASSIVE_FLAG_XIANNV_3,									// ��Ů3
		PASSIVE_FLAG_XIANNV_4,									// ��Ů4
		PASSIVE_FLAG_XIANNV_5,									// ��Ů5
		PASSIVE_FLAG_XIANNV_6,									// ��Ů6
		PASSIVE_FLAG_XIANNV_7,									// ��Ů7
		PASSIVE_FLAG_JING_LING_LEI_TING,						// ���� ����һŭ
		PASSIVE_FLAG_JING_LING_XI_XUE,							// ���� Ѫ֮ʢ��
	};

	enum EFFECT_TYPE
	{
		EFFECT_TYPE_INVALID = -1,

		EFFECT_TYPE_ATTRBUFF,									// �޸������࣬��ͬʱ����3��
		EFFECT_TYPE_SPECIAL_STATE,								// �����࣬4ְҵ�Σ�������Ĭ���������ߣ����ߣ��޵е�
		EFFECT_TYPE_INTERVAL_HP,								// ��ʱ��HP
		EFFECT_TYPE_COUNTER,									// ������
		EFFECT_TYPE_INC_HURT,									// �˺�����
		EFFECT_TYPE_DEC_HURT,									// �˺�����
		EFFECT_TYPE_DOUBLE_HURT,								// �����˺�
		EFFECT_TYPE_MOVESPEED,									// �ٶ��࣬�ӻ��
		EFFECT_TYPE_HPSTORE,									// ����
		EFFECT_TYPE_BOMB,										// ը��
		EFFECT_TYPE_WUDI_PROTECT,								// �޵б���	�ܵ��������˺���Ϊ1
		EFFECT_TYPE_MODIFY_GLOBAL_CD,							// �޸�ȫ��CD
		EFFECT_TYPE_REBOUND_HURT,								// �˺�����
		EFFECT_TYPE_RECOVER_ON_ATKED,							// ������Ѫ
		EFFECT_TYPE_GRADUAL_HURT,								// �����˺�
		EFFECT_TYPE_JUHUAPIG,									// �ջ���
		EFFECT_TYPE_INVISIBLE,									// ����
		EFFECT_TYPE_ATTR_RAND_INC_HURT,							// �����Բ�����˺�����
		EFFECT_TYPE_BIANSHEN,									// ����
		EFFECT_TYPE_MP_NO_COST,									// ����������
		EFFECT_TYPE_SHENBING,									// ���
		EFFECT_TYPE_ABSORB_HP,									// ��Ѫ
		EFFECT_TYPE_ATTR_PER,									// �޸����԰ٷֱ�
		EFEECT_TYPE_OTHER,										// ����buff
		EFFECT_TYPE_ATTR_CONTAINER,								// ������
		EFFECT_TYPE_ATTR_STORE,									// �洢��
		EFFECT_TYPE_FIRE,										// �������Լ�����Χ����
		EFFECT_TYPE_SPECIAL_SHIELD,								// ���ܣ��ܵ��˺����̶�Ѫ
		EFFECT_TYPE_DEADLY_ATTACK,								// ����һ���������ߵ��̶��ٷֱ�Ѫ��
		EFFECT_TYPE_ABSORB_HP_BEEN_ATTACK,						// ������ʱ��Ѫ
		EFFECT_TYPE_TALENT_HPSTORE,								// ���������˺�, ������ʧʱ�ָ������˺���%
		EFFECT_TYPE_FORBID_RECOVER,								// ��ֹ�ָ�

		EFFECT_TYPE_RESERVE,									// ���ﲻ�ܳ���32������Ȼ��Ӱ�������
		
		EFFECT_TYPE_ONCE = 33,									// һ����Ч��
	};

	// ��Ӧ�ͻ��˵�Buff��
	enum EFFECT_CLIENT_TYPE
	{
		ECT_SKILL_DROP_BLOOD = 1001,						// ��Ѫ
		ECT_SKILL_DINGSHEN = 1002,							// ����
		ECT_SKILL_BIANSHEN = 1003,							// ����
		ECT_SKILL_DEC_ON_HURT = 1004,						// �ܵ��˺�����(����)
		ECT_SKILL_XUANYUN = 1005,							// ѣ��
		ECT_SKILL_DEC_HURT = 1006,							// ����������
		ECT_SKILL_DEC_FANGYU = 1007,						// �����½�
		ECT_SKILL_ADD_GONGJI = 1008,						// ����������
		ECT_SKILL_ADD_FANGYU = 1009,						// ��������
		ECT_SKILL_ADD_BAOJI = 1010,							// ��������
		ECT_SKILL_DEC_SPEED = 1011,							// �ٶ��½�
		ECT_SKILL_REBOUNDHURT = 1012,						// ����BUFF,����
		ECT_SKILL_POISON = 1013,							// �ж�
		ECT_SKILL_HPSTORE = 1014,							// ����
		ECT_SKILL_ADD_SHANBI_PER = 1015,					// ����������
		ECT_SKILL_ADD_GEDANG_PER = 1016,					// ��������
		ECT_SKILL_ADD_BAOJI_PER = 1017,						// ����������
		ECT_SKILL_WUDI = 1018,								// �޵�(ͨ��)
		ECT_SKILL_CHENMO = 1019,							// ��Ĭ

		ECT_JL_SKILL_CJJS = 1101,							// ����-�ɼ�����
		ECT_JL_SKILL_CJKDD = 1102,							// ����-�ɼ������
		ECT_JL_SKILL_JF = 1103,								// ����-����
		ECT_JL_SKILL_JG = 1104,								// ����-����
		ECT_JL_SKILL_JSHAN = 1105,							// ����-����
		ECT_JL_SKILL_JSHU = 1106,							// ����-����
		ECT_JL_SKILL_JZGS = 1107,							// ����-��������
		ECT_JL_SKILL_WD = 1108,								// ����-�޵�
		ECT_JL_SKILL_XY = 1109,								// ����-ѣ��

		ECT_UPGRADE_WING_SHOUHU = 1501,						// ����-�ػ�
		ECT_UPGRADE_WING_FENGDUN = 1502,					// ����-���
		ECT_UPGRADE_WING_JIEJIE = 1503,						// ����-���

		ECT_UPGRADE_SHENGONG_ZHUMO = 1511,					// ����-��ħ

		ECT_UPGRADE_SHENYI_SHENPAN = 1521,					// ����-����

		ECT_UPGRADE_TOUSHI_SUYI = 1525,						// ͷ�ν���-����
		ECT_UPGRADE_YAOSHI_FANQIAO = 1526,					// ���ν���-����
		ECT_UPGRADE_MASK_SHUNSHANG = 1527,					// ��߽���-˲��
		ECT_UPGRADE_QILINBI_BAONU = 1528,					// ����۽���-��ŭ

		ECT_ITEM_HP1 = 2001,								// ҩˮ-��Ѫҩ1
		ECT_ITEM_HP2 = 2002,								// ҩˮ-��Ѫҩ2
		ECT_ITEM_HP3 = 2003,								// ҩˮ-��Ѫҩ3
		ECT_ITEM_HP4 = 2004,								// ҩˮ-��Ѫҩ4
		
		ECT_ITEM_MP1 = 2101,								// ҩˮ-����ҩ1
		ECT_ITEM_MP2 = 2102,								// ҩˮ-����ҩ2
		ECT_ITEM_MP3 = 2103,								// ҩˮ-����ҩ3
		ECT_ITEM_MP4 = 2104,								// ҩˮ-����ҩ4
		
		ECT_ITEM_EXP1 = 2201,								// ҩˮ-����ӳ�1
		ECT_ITEM_EXP2 = 2202,								// ҩˮ-����ӳ�2
		ECT_ITEM_EXP3 = 2203,								// ҩˮ-����ӳ�3
		ECT_ITEM_EXP4 = 2204,								// ҩˮ-����ӳ�4

		ECT_ITEM_BJ1 = 2211,								// ҩˮ-����һ��1
		ECT_ITEM_BJ2 = 2212,								// ҩˮ-����һ��2
		ECT_ITEM_BJ3 = 2213,								// ҩˮ-����һ��3
		ECT_ITEM_BJ4 = 2214,								// ҩˮ-����һ��4

		ECT_ITEM_JZ1 = 2221,								// ҩˮ-�Ӿ�׼1
		ECT_ITEM_JZ2 = 2222,								// ҩˮ-�Ӿ�׼2
		ECT_ITEM_JZ3 = 2223,								// ҩˮ-�Ӿ�׼3
		ECT_ITEM_JZ4 = 2224,								// ҩˮ-�Ӿ�׼4

		ECT_ITEM_YJ1 = 2231,								// ҩˮ-Խ��1
		ECT_ITEM_YJ2 = 2232,								// ҩˮ-Խ��2

		ECT_TW_EDGE_MEDICINE = 3000,						// ����ս-����ҩ��
		ECT_TW_LANDMINE_FROZEN = 3001,						// ����ս-����˪���׶���

		ECT_QY_FB_GONGJI = 3010,							// ��Ե��������buff

		ECT_XIULUOTOWER_BUFF = 3030,						// ������buff

		ECT_YZDD_ADD_SPEED = 3040,							// һվ����-����
		ECT_YZDD_DEC_SPEED = 3041,							// һվ����-����
		ECT_YZDD_ADD_GONGJI = 3042,							// һվ����-�ӹ�
		ECT_YZDD_DEC_GONGJI = 3043,							// һվ����-����
		ECT_YZDD_ADD_FANGYU = 3044,							// һվ����-�ӷ�
		ECT_YZDD_DEC_FANGYU = 3045,							// һվ����-����
		ECT_YZDD_RECOVER = 3046,							// һվ����-�ظ�
		ECT_YZDD_WUDI = 3047,								// һվ����-�޵�
		ECT_YZDD_BIANSHEN = 3048,							// һվ����-����
		ECT_YZDD_DEADLY_ATTACK = 3049,						// һվ����-����һ��
		ECT_YZDD_DEAD_ADD = 3050,							// һվ����-������Ѫ
		ECT_YZDD_GUWU_GONGJI = 3051,						// һվ����-���蹥��
		ECT_YZDD_GUWU_MAXHP = 3052,							// һվ����-������Ѫ

		ECT_NSTF_ADD_GONGJI = 3060,							// Ů������-�ӹ�

		ECT_TDT_ADD_GONGJI = 3065,							// ����ػ�-�ӹ�
		ECT_TDT_ADD_FANGYU = 3066,							// ����ػ�-�ӷ�

		ECT_HUGUOZHILI = 3070,								// ������

		ECT_MOJIE_SKILL_9 = 3080,							// ħ�似��9
		ECT_MOJIE_SKILL_3 = 3081,							// ħ�似��3 ����ӳ�
		ECT_MOJIE_SKILL_WUDI = 3082,						// ħ�似�� �޵�
		ECT_MOJIE_SKILL_RESTORE_HP = 3083,					// ħ�似�� ��Ѫ

		ECT_IMG_FULING_MOHUA = 4000,						// ������-ħ��
		ECT_IMG_FULING_ABSORB = 4001,						// ������-��Ѫ
		ECT_IMG_FULING_WUDI = 4002,							// ������-�޵�

		ECT_DARK_NIGHT_WUDI = 4010,							// ����䱦�ؾ�-�޵�

		ECT_IMG_SHENZHOU_WEAPON_ZHUOSHAO = 4100,			// ���-����
			
		ECT_TALENT_INC_HURT = 4110,							// ��ɫ�츳-�������� ɱ�Ⲩ��
		ECT_TALENT_DEC_ON_HURT = 4111,						// ��ɫ�츳-�ܻ����� ��а����

		ECT_SKILL_WEEKEND_EQUIP_MODEL_SIZE_CHANGE = 4220,	// ��ĩװ�� ģ�ʹ�С�ı�
		ECT_SKILL_WEEKEND_EQUIP_INC_HURT = 4221,			// ��ĩװ�� ����
		ECT_SKILL_WEEKEND_EQUIP_DEC_HURT = 4222,			// ��ĩװ�� ����
		ECT_SKILL_WEEKEND_EQUIP_INC_HURT_INVISIABLE = 4223,	// ��ĩװ�� ��������

		ECT_BUFF_GATHER_NO_STOP = 4300,						// buff�ɼ������

		ECT_OTHER_VIP = 9001,								// vip�ӳ�
		ECT_OTHER_FBGF = 9002,								// ��������
		ECT_OTHER_HMCF = 9003,								// �����ͷ�
		ECT_OTHER_SJJC = 9004,								// ����ӳ�
		ECT_HP_DAN = 9005,									// ��Ѫ��
		ECT_SZ_PROTECT = 9006,								// ��װ������
		ECT_OTHER_CALL_MONSTER_DEC_HURT = 9007,				// �ٻ�������������

		ECT_IMP_GURAD_EXP1 = 9031,                          // �ػ�С�� - ����ӳ�
		ECT_IMP_GURAD_EXP2 = 9032,

		ECT_SKILL_CONTINUE_KILL = 9050,						// ��նBUFF
	};

	enum CLEAR_REASON
	{
		CLEAR_REASON_DIE = 1,									// ��������
		CLEAR_REASON_LEAVE_SCENE,								// �뿪����
		CLEAR_REASON_SKILL,										// ����
		CLEAR_REASON_ENTER_SCENE,								// ���볡��
	};

	EffectBase() : m_deliverer(0), m_product_id(0), m_product_method(0), m_product_level(0), m_merge_rule(0),
					m_is_save(true), m_clear_on_die(true), m_merge_layer(1), m_max_merge_layer(1), m_is_main_target(false), m_client_effect_type(0), m_unique_key(0), m_recalc_reason_on_add_or_del(0) {}

	EffectBase(ObjID deliverer, UInt16 product_id, char product_method, char product_level)
		: m_deliverer(deliverer), m_product_id(product_id), m_product_method(product_method), m_product_level(product_level),
		m_merge_rule(0), m_is_save(true), m_clear_on_die(true), m_merge_layer(1), m_max_merge_layer(1), m_is_main_target(false), m_client_effect_type(0), m_unique_key(0), m_recalc_reason_on_add_or_del(0) {}

	virtual ~EffectBase() {}

	void SetSave(bool is_save) { m_is_save = is_save; }
	void SetClearOnDie(bool is_clear) { m_clear_on_die = is_clear; }

	void SetMergeRule(char merge_rule) { m_merge_rule = merge_rule; }
	char GetMergeRule() const { return m_merge_rule; }

	void SetMergeLayer(char merge_layer) { m_merge_layer = merge_layer; }
	char GetMergeLayer() const { return m_merge_layer; }
	void SetMaxMergeLayer(char max_layer) { m_max_merge_layer = max_layer; }
	int GetClientEffectType() { return m_client_effect_type; }
	void SetClientEffectType(int effect_type) { m_client_effect_type = effect_type; }

	UInt16 GetProductId() const { return m_product_id; }
	ObjID GetDeliverer() const { return m_deliverer; }
	int GetProductMethod() const { return m_product_method; }
	char GetProductLevel() const { return m_product_level; }

	virtual int CompareWith(EffectBase *effect) { return CMP_RET_NOTHING; }

	void SetUniqueKey(unsigned int unique_key) { m_unique_key = unique_key; }
	unsigned int GetUniqueKey() { return m_unique_key; }

	virtual void Merge(EffectBase *new_effect, Character *cha) {}
	
	virtual void SetLeftTimeMS(int effect_time) {}
	virtual int GetLeftTimeMS() = 0;
	virtual char GetEffectType() = 0;
	virtual int GetImage(){return 0;}
	virtual void Effect(Character *cha, bool *die) = 0;
	virtual void ReEffect(Character *cha, bool *die) = 0;				// ��ɫinit��ʱ����ã���effect�����Լ����ʵ�֣�����ԭ���ǲ�Ҫ��HP
	virtual bool Update(unsigned long interval, Character *cha, bool *die) = 0;
	virtual void ClearEffect(Character *cha) {}

	virtual bool IsPositiveEffect() const { return true; }
	virtual bool IsDingshenEffect() const { return false; }
	virtual bool IsXuanyunEffect() const { return false; }
	virtual bool IsChemoEffect() const { return false; }
	virtual bool IsChihuanEffect() const { return false; }

	virtual unsigned char GetBuffType() const { return EBT_INVALID; }

	virtual void GetEffectParam(short *count, long long param[EFFECT_INFO_PARAM_MAX]) {}
	void GetEffectInfo(EffectInfo *effect_info);
	// virtual void MergeParam(EffectBase *effect) {}; // �ϲ�����

	virtual bool Serialize(TLVSerializer &s) const { return false; }
	virtual bool Unserialize(TLVUnserializer &s) { return false; }

	bool IsSave() { return m_is_save; }
	bool IsClearOnDie() { return m_clear_on_die; }

	void SetMainTarget(bool is_main_target) { m_is_main_target = is_main_target; }
	bool IsMainTarget() { return m_is_main_target; }

	virtual bool CanEffect(Character *cha);

	virtual void Recalc(Character *chara) {}

	void SetRecalcReasonOnAddOrDel(char reason) { m_recalc_reason_on_add_or_del = reason; }
	char GetRecalcReasonOnAddOrDel() { return m_recalc_reason_on_add_or_del; }

	static EffectBase * UnserializeCreateEffect(char type);
	
protected:
	bool SerializeBase(TLVSerializer &s) const;
	bool UnserializeBase(TLVUnserializer &s);

	ObjID	m_deliverer;
	UInt16	m_product_id;
	char	m_product_method;

	char	m_product_level;				// һ���Ǽ��ܵȼ�
	char	m_merge_rule;					// ���ӻ����滻��6�ֹ���
	bool	m_is_save;						// �Ƿ����
	bool	m_clear_on_die;					// �����Ƿ����
	char	m_merge_layer;					// ���Ӳ���
	char	m_max_merge_layer;				// �ɵ���������
	bool	m_is_main_target;				// �Ƿ�Ϊ��Ŀ��
	int		m_client_effect_type;			// �ͻ���Ч������
	unsigned int m_unique_key;

	char	m_recalc_reason_on_add_or_del;	// ����ӻ�ɾ��ʱ��������������ԭ��

	// ��buff����Լ�����˺�Ҫ���������
	void EffectHP(Attribute *real_delta_hp, Attribute *view_delta_hp, Character *cha, bool *die, bool is_from_skill, int fight_type = FIGHT_TYPE_NORMAL);

	// ��buff��Ա�������˺�Ҫ���������
	void HurtOthers(Character *cha, short target_count, ObjID *target_objid_list, long long injure, int fight_type);

	void SendAttackBlock(Character *cha, Attribute real_delta_hp, Attribute delta_hp, unsigned short fight_type, unsigned char blood_seq, bool is_send_product_id = true, int passive_flag = 0);
	void SendMPChangeMsg(Character *cha, Attribute delta_hp);
};

#endif

