#ifndef __TALENT_DEF_HPP__
#define __TALENT_DEF_HPP__

#include "servercommon.h"

enum TALENT_ATTR_TYPE
{
	TALENT_ATTR_TYPE_INVALID = 0,

	TALENT_ATTR_TYPE_MAXHP,						// ��ɫ��Ѫ
	TALENT_ATTR_TYPE_GONGJI,					// ��ɫ�﹥/����
	TALENT_ATTR_TYPE_FANGYU,					// ��ɫ����
	TALENT_ATTR_TYPE_BAOJI,						// ��ɫ����
	TALENT_ATTR_TYPE_BAOJI_PER,					// ��ɫ�����������˺��ʣ�
	TALENT_ATTR_TYPE_JIANREN,					// ��ɫ����(����)
	TALENT_ATTR_TYPE_POFANG,					// ��ɫ�Ʒ��ٷֱȣ������ʣ�

	TALENT_ATTR_TYPE_PVP_JIANSHANG_PER,			// pvp������ֱ�
	TALENT_ATTR_TYPE_PVP_ZENGSHANG_PER,         // pvp������ֱ�
	TALENT_ATTR_TYPE_PVE_JIANSHANG_PER,         // pve������ֱ�
	TALENT_ATTR_TYPE_PVE_ZENGSHANG_PER,         // pve������ֱ�

	TALENT_ATTR_TYPE_FABAO_GONGJI_PER,			// ����������ֱ�
	TALENT_ATTR_TYPE_FABAO_FANGYU_PER,			// ����������ֱ�
	TALENT_ATTR_TYPE_FIGHT_MOUNT_GONGJI_PER,	// ս�����﹥����ֱ�
	TALENT_ATTR_TYPE_FIGHT_MOUNT_FANGYU_PER,	// ս�����������ֱ�
	TALENT_ATTR_TYPE_MOUNT_GONGJI_PER,			// ���﹥����ֱ�
	TALENT_ATTR_TYPE_MOUNT_FANGYU_PER,			// ���������ֱ�
	TALENT_ATTR_TYPE_SHENBING_GONGJI_PER,		// ���������ֱȣ�ʱװ������
	TALENT_ATTR_TYPE_SHENBING_FANGYU_PER,		// ���������ֱȣ�ʱװ������
	TALENT_ATTR_TYPE_WING_GONGJI_PER,			// ��������ֱ�
	TALENT_ATTR_TYPE_WING_FANGYU_PER,			// ���������ֱ�
	TALENT_ATTR_TYPE_HALO_GONGJI_PER,			// �⻷������ֱ�
	TALENT_ATTR_TYPE_HALO_FANGYU_PER,			// �⻷������ֱ�
	TALENT_ATTR_TYPE_SHENYI_GONGJI_PER,			// ��������ֱ�
	TALENT_ATTR_TYPE_SHENYI_FANGYU_PER,			// ���������ֱ�
	TALENT_ATTR_TYPE_SHENGONG_GONGJI_PER,		// �񹭹�����ֱ�
	TALENT_ATTR_TYPE_SHENGONG_FANGYU_PER,		// �񹭷�����ֱ�
	TALENT_ATTR_TYPE_SHIZHUANG_GONGJI_PER,		// ʱװ������ֱ�
	TALENT_ATTR_TYPE_SHIZHUANG_FANGYU_PER,		// ʱװ������ֱ�
	TALENT_ATTR_TYPE_FOOTPRINT_GONGJI_PER,		// �㼣������ֱ�
	TALENT_ATTR_TYPE_FOOTPRINT_FANGYU_PER,		// �㼣������ֱ�
	TALENT_ATTR_TYPE_SHENSHOU_GONGJI_PER,		// ���޹�����ֱ�(����)
	TALENT_ATTR_TYPE_SHENSHOU_FANGYU_PER,		// ���޷�����ֱ�(����)
	TALENT_ATTR_TYPE_SHENSHOU_ALL_PER,			// ����ȫ������ֱ�(����)
	TALENT_ATTR_TYPE_YUSHI_GONGJI_PER,			// ��ʯ������ֱ�
	TALENT_ATTR_TYPE_YUSHI_FANGYU_PER,			// ��ʯ������ֱ�
	TALENT_ATTR_TYPE_YUSHI_ALL_PER,				// ��ʯȫ������ֱ�
	TALENT_ATTR_TYPE_STONE_GONGJI_PER,			// ��ʯ������ֱ�
	TALENT_ATTR_TYPE_STONE_FANGYU_PER,			// ��ʯ������ֱ�
	TALENT_ATTR_TYPE_STONE_ALL_PER,				// ��ʯȫ������ֱ�
	TALENT_ATTR_TYPE_NORMAL_EQUIP_GONGJI_PER,	// ��ͨװ��������ֱ�
	TALENT_ATTR_TYPE_NORMAL_EQUIP_FANGYU_PER,	// ��ͨװ��������ֱ�
	TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_GONGJI_PER,	// תְװ��������ֱ�
	TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_FANGYU_PER,	// תְװ��������ֱ�
	TALENT_ATTR_TYPE_YONGHENG_SUIT_PER,			// ������װ������ֱ�
	TALENT_ATTR_TYPE_ZHUXIAN_SUIT_PER,			// ������װ������ֱ�
	TALENT_ATTR_TYPE_ZHUSHEN_SUIT_PER,			// ������װ������ֱ�

	TALENT_ATTR_TYPE_SKILL_HURT_ADD_PER,		// �����˺�������ֱ�
	TALENT_ATTR_TYPE_SKILL_HURT_MINUS_PER,		// �����˺�������ֱ�

	TALENT_ATTR_TYPE_SKILL_ZENGSHANG_PER,		// ����������ֱ�
	TALENT_ATTR_TYPE_SKILL_JIANSHANG_PER,		// ���ܼ�����ֱ�

	TALENT_ATTR_TYPE_QILINBI_UPGRADE_PER,		// ����۽���������ֱ�
	TALENT_ATTR_TYPE_WING_UPGRADE_PER,			// �������������ֱ�
	TALENT_ATTR_TYPE_FABAO_UPGRADE_PER,			// ��������������ֱ�
	TALENT_ATTR_TYPE_LINGTONG_UPGRADE_PER,		// ��ͯ����������ֱ�
	TALENT_ATTR_TYPE_MOUNT_UPGRADE_PER,			// �������������ֱ�
	TALENT_ATTR_TYPE_SHENGBING_UPGRADE_PER,		// �������������ֱ�
	TALENT_ATTR_TYPE_SHIZHUANG_UPGRADE_PER,		// ʱװ����������ֱ�
	TALENT_ATTR_TYPE_FIGHT_MOUNT_UPGRADE_PER,	// ս�����������ֱ�
	TALENT_ATTR_TYPE_SHENGONG_UPGRADE_PER,		// �񹭽���������ֱȣ���Ů�⻷��
	TALENT_ATTR_TYPE_LINGGONG_UPGRADE_PER,		// �鹭����������ֱ�
	TALENT_ATTR_TYPE_TOUSHI_UPGRADE_PER,		// ͷ�ν���������ֱ�
	TALENT_ATTR_TYPE_LINGQI_UPGRADE_PER,		// �������������ֱ�
	TALENT_ATTR_TYPE_YAOSHI_UPGRADE_PER,		// ���ν���������ֱ�
	TALENT_ATTR_TYPE_HALO_UPGRADE_PER,			// �⻷����������ֱ�
	TALENT_ATTR_TYPE_MASK_UPGRADE_PER,			// ���ν���������ֱ�
	TALENT_ATTR_TYPE_WEIYAN_UPGRADE_PER,		// β�����������ֱ�

	TALENT_ATTR_TYPE_EQUIP_BASE_PER,			// ��ͨװ��������ֱ�
	TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_BASE_PER,	// תְװ��������ֱ�

	TALENT_ATTR_TYPE_MAX,
};

struct ConfigTalentAttrDefineTypeString
{
	char talent_attr_name[32];
	short talent_attr_type;
};

const static ConfigTalentAttrDefineTypeString gs_config_talent_attr_define_string_array[] =
{
	{ "role_maxhp", TALENT_ATTR_TYPE_MAXHP },								// ��ɫ���Ѫ��
	{ "role_gongji", TALENT_ATTR_TYPE_GONGJI },								// ��ɫ����
	{ "role_fangyu", TALENT_ATTR_TYPE_FANGYU },								// ��ɫ����
	{ "role_baoji", TALENT_ATTR_TYPE_BAOJI },								// ��ɫ����
	{ "role_baoji_per", TALENT_ATTR_TYPE_BAOJI_PER },						// ��ɫ�����˺�
	{ "role_jianren", TALENT_ATTR_TYPE_JIANREN },							// ��ɫ����(����)
	{ "role_pofang", TALENT_ATTR_TYPE_POFANG},								// �Ʒ��ٷֱ�(������)

	{ "pvp_mianshang_per", TALENT_ATTR_TYPE_PVP_JIANSHANG_PER},			// pvp������ֱ�
	{ "pvp_zengshang_per", TALENT_ATTR_TYPE_PVP_ZENGSHANG_PER},         // pvp������ֱ�
	{ "pve_mianshang_per", TALENT_ATTR_TYPE_PVE_JIANSHANG_PER},			// pve������ֱ�
	{ "pve_zengshang_per", TALENT_ATTR_TYPE_PVE_ZENGSHANG_PER},         // pve������ֱ�

	{ "fight_mount_gongji_per", TALENT_ATTR_TYPE_FIGHT_MOUNT_GONGJI_PER},	// ս�����﹥����ֱ�
	{ "fight_mount_fangyu_per", TALENT_ATTR_TYPE_FIGHT_MOUNT_FANGYU_PER},	// ս�����������ֱ�
	{ "wing_gongji_per", TALENT_ATTR_TYPE_WING_GONGJI_PER},					// ��������ֱ�
	{ "wing_fangyu_per", TALENT_ATTR_TYPE_WING_FANGYU_PER},					// ���������ֱ�
	{ "halo_gongji_per", TALENT_ATTR_TYPE_HALO_GONGJI_PER},					// �⻷������ֱ�
	{ "halo_fangyu_per", TALENT_ATTR_TYPE_HALO_FANGYU_PER},					// �⻷������ֱ�
	{ "fabao_gongji_per", TALENT_ATTR_TYPE_FABAO_GONGJI_PER },				// ����������ֱ�
	{ "fabao_fangyu_per", TALENT_ATTR_TYPE_FABAO_FANGYU_PER },				// ����������ֱ�
	{ "mount_gongji_per", TALENT_ATTR_TYPE_MOUNT_GONGJI_PER },				// ���﹥����ֱ�
	{ "mount_fangyu_per", TALENT_ATTR_TYPE_MOUNT_FANGYU_PER },				// ���������ֱ�
	{ "shenbing_gongji_per", TALENT_ATTR_TYPE_SHENBING_GONGJI_PER },		// ���������ֱ�
	{ "shenbing_fangyu_per", TALENT_ATTR_TYPE_SHENBING_FANGYU_PER },		// ���������ֱ�
	{ "shenyi_gongji_per", TALENT_ATTR_TYPE_SHENYI_GONGJI_PER },			// ��������ֱ�(��Ůϵͳ����)
	{ "shenyi_fangyu_per", TALENT_ATTR_TYPE_SHENYI_FANGYU_PER },			// ���������ֱ�
	{ "shengong_gongji_per", TALENT_ATTR_TYPE_SHENGONG_GONGJI_PER },		// �񹭹�����ֱ�(��Ůϵͳ�⻷)
	{ "shengong_fangyu_per", TALENT_ATTR_TYPE_SHENGONG_FANGYU_PER },		// �񹭷�����ֱ�
	{ "shizhuang_gongji_per", TALENT_ATTR_TYPE_SHIZHUANG_GONGJI_PER },		// ʱװ������ֱ�
	{ "shizhuang_fangyu_per", TALENT_ATTR_TYPE_SHIZHUANG_FANGYU_PER },		// ʱװ������ֱ�
	{ "footprint_gongji_per", TALENT_ATTR_TYPE_FOOTPRINT_GONGJI_PER },		// �㼣������ֱ�
	{ "footprint_fangyu_per", TALENT_ATTR_TYPE_FOOTPRINT_FANGYU_PER },		// �㼣������ֱ�
	{ "shenshou_gongji_per", TALENT_ATTR_TYPE_SHENSHOU_GONGJI_PER },		// ���޹�����ֱ�(����)
	{ "shenshou_fangyu_per", TALENT_ATTR_TYPE_SHENSHOU_FANGYU_PER },		// ���޷�����ֱ�(����)
	{ "shenshou_all_per", TALENT_ATTR_TYPE_SHENSHOU_ALL_PER },				// ����ȫ������ֱ�(����)
	{ "yushi_gongji_per", TALENT_ATTR_TYPE_YUSHI_GONGJI_PER },				// ��ʯ������ֱ�
	{ "yushi_fangyu_per", TALENT_ATTR_TYPE_YUSHI_FANGYU_PER },				// ��ʯ������ֱ�
	{ "yushi_all_per", TALENT_ATTR_TYPE_YUSHI_ALL_PER },					// ��ʯȫ������ֱ�
	{ "stone_gongji_per", TALENT_ATTR_TYPE_STONE_GONGJI_PER },				// ��ʯ������ֱ�
	{ "stone_fangyu_per", TALENT_ATTR_TYPE_STONE_FANGYU_PER },				// ��ʯ������ֱ�
	{ "stone_all_per", TALENT_ATTR_TYPE_STONE_ALL_PER },					// ��ʯȫ������ֱ�
	{ "normal_equip_gongji_per", TALENT_ATTR_TYPE_NORMAL_EQUIP_GONGJI_PER },		// ��ͨװ��������ֱ�
	{ "normal_equip_fangyu_per", TALENT_ATTR_TYPE_NORMAL_EQUIP_FANGYU_PER },		// ��ͨװ��������ֱ�
	{ "zhuanzhi_equip_gongji_per", TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_GONGJI_PER },	// תְװ��������ֱ�
	{ "zhuanzhi_equip_fangyu_per", TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_FANGYU_PER },	// תְװ��������ֱ�
	{ "yushi_gongji_per", TALENT_ATTR_TYPE_YUSHI_GONGJI_PER },						// ��ʯ������ֱ�
	{ "yushi_fangyu_per", TALENT_ATTR_TYPE_YUSHI_FANGYU_PER },						// ��ʯ������ֱ�
	{ "yongheng_suit_per", TALENT_ATTR_TYPE_YONGHENG_SUIT_PER},						// ������װ������ֱ�
	{ "zhuxian_suit_per", TALENT_ATTR_TYPE_ZHUXIAN_SUIT_PER },						// ������װ������ֱ�
	{ "zhushen_suit_per", TALENT_ATTR_TYPE_ZHUSHEN_SUIT_PER },						// ������װ������ֱ�

	{ "skill_hurt_add_per", TALENT_ATTR_TYPE_SKILL_HURT_ADD_PER },					// �����˺�������ֱ�
	{ "skill_hurt_minus_per", TALENT_ATTR_TYPE_SKILL_HURT_MINUS_PER },				// �����˺�������ֱ�

	{ "skill_zengshang_per", TALENT_ATTR_TYPE_SKILL_ZENGSHANG_PER },				// ����������ֱ�
	{ "skill_jianshang_per", TALENT_ATTR_TYPE_SKILL_JIANSHANG_PER },				// ���ܼ�����ֱ�

	{ "qilingbi_upgrade_per", TALENT_ATTR_TYPE_QILINBI_UPGRADE_PER },				// ����۽���������ֱ�
	{ "wing_upgrade_per", TALENT_ATTR_TYPE_WING_UPGRADE_PER },						// �������������ֱ�
	{ "fabao_upgrade_per", TALENT_ATTR_TYPE_FABAO_UPGRADE_PER },					// ��������������ֱ�
	{ "lingtong_upgrade_per", TALENT_ATTR_TYPE_LINGTONG_UPGRADE_PER },				// ��ͯ����������ֱ�
	{ "mount_upgrade_per", TALENT_ATTR_TYPE_MOUNT_UPGRADE_PER },					// �������������ֱ�
	{ "shenbing_upgrade_per", TALENT_ATTR_TYPE_SHENGBING_UPGRADE_PER },				// �������������ֱ�
	{ "shizhuang_upgrade_per", TALENT_ATTR_TYPE_SHIZHUANG_UPGRADE_PER },			// ʱװ����������ֱ�
	{ "fight_mount_upgrade_per", TALENT_ATTR_TYPE_FIGHT_MOUNT_UPGRADE_PER },		// ս�����������ֱ�
	{ "shengong_upgrade_per", TALENT_ATTR_TYPE_SHENGONG_UPGRADE_PER },				// �񹭽���������ֱȣ���Ů�⻷��
	{ "linggong_upgrade_per", TALENT_ATTR_TYPE_LINGGONG_UPGRADE_PER },				// �鹭����������ֱ�
	{ "toushi_upgrade_per", TALENT_ATTR_TYPE_TOUSHI_UPGRADE_PER },				    // ͷ�ν���������ֱ�
	{ "lingqi_upgrade_per", TALENT_ATTR_TYPE_LINGQI_UPGRADE_PER },				    // �������������ֱ�
	{ "yaoshi_upgrade_per", TALENT_ATTR_TYPE_YAOSHI_UPGRADE_PER },				    // ���ν���������ֱ�
	{ "halo_upgrade_per", TALENT_ATTR_TYPE_HALO_UPGRADE_PER },					    // �⻷����������ֱ�
	{ "mask_upgrade_per", TALENT_ATTR_TYPE_MASK_UPGRADE_PER },						// ���ν���������ֱ�
	{ "weiyan_upgrade_per", TALENT_ATTR_TYPE_WEIYAN_UPGRADE_PER },					// β�����������ֱ�

	{ "equip_base_per", TALENT_ATTR_TYPE_EQUIP_BASE_PER },							// ��ͨװ��������ֱ�
	{ "zhuanzhi_equip_base_per", TALENT_ATTR_TYPE_ZHUANZHI_EQUIP_BASE_PER },		// תְװ��������ֱ�
	
	{ "0", TALENT_ATTR_TYPE_INVALID }
};

enum TALENT_OTHER_MODULE
{
	TALENT_OTHER_MODULE_INVALID,
	TALENT_OTHER_MODULE_FABAO,
	TALENT_OTHER_MODULE_FIGHT_MOUNT,
	TALENT_OTHER_MODULE_MOUNT,
	TALENT_OTHER_MODULE_WING,			// ����
	TALENT_OTHER_MODULE_HALO,			// �⻷
	TALENT_OTHER_MODULE_SHENBING,		// ���
	TALENT_OTHER_MODULE_SHENYI,
	TALENT_OTHER_MODULE_SHENGONG,
	TALENT_OTHER_MODULE_SHIZHUANG,
	TALENT_OTHER_MODULE_FOOTPRINT,
	TALENT_OTHER_MODULE_SHENSHOU,
	TALENT_OTHER_MODULE_YUSHI,
	TALENT_OTHER_MODULE_STONE,
	TALENT_OTHER_MODULE_YONGHENG_SUIT,
	TALENT_OTHER_MODULE_ZHUXIAN_SUIT,
	TALENT_OTHER_MODULE_ZHUSHEN_SUIT,
	TALENT_OTHER_MODULE_NORMAL_EQUIP,
	TALENT_OTHER_MODULE_ZHUANZHI_EQUIP,
	TALENT_OTHER_MODULE_FABAO_BASE,
	TALENT_OTHER_MODULE_FIGHT_MOUNT_BASE,
	TALENT_OTHER_MODULE_MOUNT_BASE,
	TALENT_OTHER_MODULE_WING_BASE,
	TALENT_OTHER_MODULE_HALO_BASE,
	TALENT_OTHER_MODULE_SHENBING_BASE,
	TALENT_OTHER_MODULE_SHENGONG_BASE,
	TALENT_OTHER_MODULE_SHIZHUANG_BASE,
	TALENT_OTHER_MODULE_QILINBI_BASE,
	TALENT_OTHER_MODULE_MASK_BASE,
	TALENT_OTHER_MODULE_TOUSHI_BASE,
	TALENT_OTHER_MODULE_YAOSHI_BASE,
	TALENT_OTHER_MODULE_NORMAL_EQUIP_BASE,
	TALENT_OTHER_MODULE_ZHUANZHI_EQUIP_BASE,

	TALENT_OTHER_MODULE_UPGRADE_0 = 200,  // ����
	TALENT_OTHER_MODULE_UPGRADE_1,		  // �ɱ�
	TALENT_OTHER_MODULE_UPGRADE_2,		  // ��ͯ
	TALENT_OTHER_MODULE_UPGRADE_3,		  // �鹭
	TALENT_OTHER_MODULE_UPGRADE_4,        // ����
	TALENT_OTHER_MODULE_UPGRADE_5,        // β��
	TALENT_OTHER_MODULE_UPGRADE_6,        // �ֻ�
	TALENT_OTHER_MODULE_UPGRADE_7,        // β��
	TALENT_OTHER_MODULE_UPGRADE_8,        // �ɳ�
};

enum TALENT_TYPE
{
	TALENT_TYPE_INVALID = 0,

	TALENT_TYPE_ROLE_GONGJI,            // ��ɫ����ϵ
	TALENT_TYPE_ROLE_FANGYU,			// ��ɫ����ϵ
	TALENT_TYPE_LINGTONG,				// ���ϵ
	TALENT_TYPE_PROFICIENT,				// ��ͨϵ

	TALENT_TYPE_MAX,
};

enum TALENT_EFFECT_TYPE
{
	TALENT_EFFECT_TYPE_ATTR = 0,		// ������
	TALENT_EFFECT_TYPE_SKILL,           // ������
};

static const int MAX_TELENT_TYPE_COUT = 4;
static const int MAX_TELENT_INDEX_COUT = 20;
static const int ROLE_TALENT_LEVEL_MAX = 50;
const static int RECALC_REASON_STONE = 10;

static const int SPECIAL_TALENT_ID_107 = 107;	// ������	����ʱ�����֮param_a�ĸ���ʹ���ֱ����ʧ�������ֵ�����֮param_b
static const int SPECIAL_TALENT_ID_108 = 108;	// ս���ȳ�	����ʱ�����֮param_d�ĸ��ʶ������ɹ���ʱ�������֮param_a���˺���������param_b�㣬����param_c����
static const int SPECIAL_TALENT_ID_109 = 109;	// ɱ�Ⲩ��	��ɱ��Һ�param_b�����˺��������֮param_a����ȴʱ��20��
static const int SPECIAL_TALENT_ID_110 = 110;	// �����ɹ�	����ʱ�����֮param_a����������ѪЧ����ÿ����ʧ�ҷ����������֮param_b��Ѫ��������param_c���룬��ȴʱ��5��
static const int SPECIAL_TALENT_ID_414 = 414; 	// ���ǣ�����ʱ�������֮param_a�ļ���ʹ���ι���������˺������֮param_bת��Ϊ�Լ�������ֵ����ȴʱ��5S��
static const int SPECIAL_TALENT_ID_412 = 412; 	// ���ģ�����ʱ�����֮param_a����ʹ�Է��������״̬��param_b����֮���޷�ʹ���κη����ظ���Ѫ����ȴʱ��60�롣
static const int SPECIAL_TALENT_ID_115 = 115;	// ���ģ�����ʱ�����֮param_a���ʸ���param_b����ʵ�˺�
static const int SPECIAL_TALENT_ID_310 = 310;	// ׷����ǣ�����ʱ�����֮param_a�ļ����������֮param_b����������param_c���룬��ȴʱ��X��

static const int SPECIAL_TALENT_ID_206 = 206; 	// ����֮��	�ܵ��˺�ʱ���֮param_a�ļ��ʷ���param_b����ʵ�˺�
static const int SPECIAL_TALENT_ID_207 = 207; 	// �����׳�	�ܵ��˺�ʱ�����֮param_a�ĸ���ʹ���������ֱ����ʧ���֮param_b���������ֵ
static const int SPECIAL_TALENT_ID_210 = 210; 	// ��Ѫ����	�ܵ���ҹ���ʱ�������֮param_a�ĸ��ʻظ��������ֵ���֮param_b��Ѫ��
static const int SPECIAL_TALENT_ID_311 = 311; 	// �Ƿ��켫���ܵ�����ʱ���֮param_a�����������֮param_b����������param_c���룬��ȴʱ��30��
static const int SPECIAL_TALENT_ID_312 = 312; 	// �ɶ��칤��������ʱ�����֮param_a���������ܵ��˺������֮param_bת����������Ѫ����ȴʱ��10��
static const int SPECIAL_TALENT_ID_417 = 417; 	// �컯���ܵ�����ʱ�������֮param_a����ʹ�Լ��������︴��״̬���޵�param_b���벢������param_b�����������˺������֮param_cת��Ϊ�Լ�������ֵ����ȴʱ��60�롣

static const int SPECIAL_TALENT_ID_208 = 208; 	// ������־	ÿ�ܵ�����˺�ʱ���֮param_d�ĸ��ʽ������֮param_a���˺���������param_b�㣬����param_c����
static const int SPECIAL_TALENT_ID_209 = 209; 	// ��а����	��ɱ��Һ�param_b�������ܵ����˺��������֮param_a����ȴʱ��20��

static const int SPECIAL_TALENT_ID_306 = 306; 	// �𵴷��� ����ʱ�����֮param_a�����ͷ�ǿ�����ܣ���param_b��������ɹ��������֮param_c���˺�

UNSTD_STATIC_CHECK(MAX_TELENT_TYPE_COUT == TALENT_TYPE_MAX - 1)
UNSTD_STATIC_CHECK(MAX_TELENT_INDEX_COUT < 32)

#pragma pack(push, 4)

struct TalentParam
{
	TalentParam() { this->Reset(); }

	void Reset()
	{
		talent_point = 0;
		talent_book_add_point = 0;
		reserve_sh = 0;

		memset(talent_level_list, 0, sizeof(talent_level_list));
		memset(talent_first_point_flag, 0, sizeof(talent_first_point_flag));
	}

	int talent_point;														// �츳����
	unsigned short talent_book_add_point;									// �츳���������ӵĵ���
	short reserve_sh;

	char talent_level_list[MAX_TELENT_TYPE_COUT][MAX_TELENT_INDEX_COUT];	// �츳�ȼ�
	int talent_first_point_flag[MAX_TELENT_TYPE_COUT];						// �츳�״μӵ��¼
};

typedef char TalentParamHex[sizeof(TalentParam) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(TalentParamHex) < 256);

#pragma pack(pop)

#endif
