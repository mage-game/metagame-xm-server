
#ifndef GAMEDEF_H
#define GAMEDEF_H

#include "servercommon/servercommon.h"
#include "common/vector2d.h"

typedef unsigned short	TaskID;
typedef float			Dir;
typedef float			Scalar;
typedef long long		Attribute;
typedef unsigned long long		Exp_t;
typedef TVector2<float>				Posf;
typedef TVector2<unsigned int>		Posu;
typedef char						ObjType;
typedef char						SkillType;

#define INVALID_TASK_ID		0		

enum _Dir
{
	// Dir_S = 0,
	Dir_S_RESERVED = 0,
	Dir_LU,
	Dir_U,
	Dir_RU,
	Dir_L,
	Dir_RESERVED,
	Dir_R,
	Dir_LD,
	Dir_D,
	Dir_RD,
	Dir_Count_Reserved
};

static const float Dir_S = 1000.0f;				// 一个无效的值 比-3.14 3.14要大 
static const float Dir_Max = 3.15f;				// 最大的方向 π的值

// 外观武器使用类型
enum  APPEARANCE_WUQI_USE_TYPE
{
	APPEARANCE_WUQI_USE_TYPE_INVALID = 0,
	APPEARANCE_WUQI_USE_TYPE_SHIZHUANG,
	APPEARANCE_WUQI_USE_TYPE_SHENQI,

	APPEARANCE_WUQI_USE_TYPE_MAX
};

// 外观衣服使用类型
enum APPEARANCE_BODY_USE_TYPE
{
	APPEARANCE_BODY_USE_TYPE_INVALID = 0,
	APPEARANCE_BODY_USE_TYPE_SHIZHUANG,
	APPEARANCE_BODY_USE_TYPE_SHENQI,

	APPEARANCE_BODY_USE_TYPE_MAX
};

struct RoleAppearance
{
	RoleAppearance()
	{
		this->Reset();
	}

	void Reset()
	{
		wuqi_id = 0;
		shizhuang_wuqi = 0;
		shizhuang_wuqi_is_special = 0;
		shizhuang_body = 0;
		shizhuang_body_is_special = 0;
		mount_used_imageid = 0;
		wing_used_imageid = 0;
		halo_used_imageid = 0;
		shengong_used_imageid = 0;
		shenyi_used_imageid = 0;
		xiannvshouhu_imageid = 0;
		jingling_guanghuan_imageid = 0;
		jingling_fazhen_imageid = 0;
		fight_mount_used_imageid = 0;
		zhibao_used_imageid = 0;
		use_eternity_level = 0;
		footprint_used_imageid = 0;
		cloak_used_imageid = 0;
		fabao_used_imageid = 0;
		jingjie_imageid = 0;
		yaoshi_used_imageid = 0;
		toushi_used_imageid = 0;
		qilinbi_used_imageid = 0;
		mask_used_imageid = 0;

		reserve_sh = 0;
		upgrade_used_imageid_0 = 0;
		upgrade_used_imageid_1 = 0;
		upgrade_used_imageid_2 = 0;
		upgrade_used_imageid_3 = 0;
		upgrade_used_imageid_4 = 0;
		upgrade_used_imageid_5 = 0;
		upgrade_used_imageid_6 = 0;
		upgrade_used_imageid_7 = 0;
		upgrade_used_imageid_8 = 0;

		shenbing_image_id = 0;
		shenbing_texiao_id = 0;
		baojia_image_id = 0;
		baojia_texiao_id = 0;
	}

	bool IsInvalid()
	{
		return wuqi_id == 0 &&
		shizhuang_wuqi == 0 && 
		shizhuang_wuqi_is_special == 0 &&
		shizhuang_body == 0 &&
		shizhuang_body_is_special == 0 &&
		mount_used_imageid == 0 &&
		wing_used_imageid == 0 &&
		halo_used_imageid == 0 &&
		shengong_used_imageid == 0 &&
		shenyi_used_imageid == 0 &&
		xiannvshouhu_imageid == 0 &&
		jingling_guanghuan_imageid == 0 &&
		jingling_fazhen_imageid == 0 &&
		fight_mount_used_imageid == 0 &&
		zhibao_used_imageid == 0 &&
		use_eternity_level == 0 &&
		footprint_used_imageid == 0 &&
		cloak_used_imageid == 0 &&
		fabao_used_imageid == 0 &&
		jingjie_imageid == 0 &&
		yaoshi_used_imageid == 0 &&
		toushi_used_imageid == 0 &&
		qilinbi_used_imageid == 0 &&
		mask_used_imageid == 0 &&

		reserve_sh == 0 &&
		upgrade_used_imageid_0 == 0 &&
		upgrade_used_imageid_1 == 0 &&
		upgrade_used_imageid_2 == 0 &&
		upgrade_used_imageid_3 == 0 &&
		upgrade_used_imageid_4 == 0 &&
		upgrade_used_imageid_5 == 0 &&
		upgrade_used_imageid_6 == 0 &&
		upgrade_used_imageid_7 == 0 &&
		upgrade_used_imageid_8 == 0 &&

		shenbing_image_id == 0 &&
		shenbing_texiao_id == 0 &&
		baojia_image_id == 0 &&
		baojia_texiao_id == 0;
	}

	bool IsEquil(RoleAppearance &other)
	{
		return wuqi_id == other.wuqi_id &&
			shizhuang_wuqi == other.shizhuang_wuqi &&
			shizhuang_wuqi_is_special == other.shizhuang_wuqi_is_special &&
			shizhuang_body == other.shizhuang_body &&
			shizhuang_body_is_special == other.shizhuang_body_is_special &&
			mount_used_imageid == other.mount_used_imageid &&
			wing_used_imageid == other.wing_used_imageid &&
			halo_used_imageid == other.halo_used_imageid &&
			shengong_used_imageid == other.shengong_used_imageid &&
			shenyi_used_imageid == other.shenyi_used_imageid &&
			xiannvshouhu_imageid == other.xiannvshouhu_imageid &&
			jingling_guanghuan_imageid == other.jingling_guanghuan_imageid &&
			jingling_fazhen_imageid == other.jingling_fazhen_imageid &&
			fight_mount_used_imageid == other.fight_mount_used_imageid &&
			zhibao_used_imageid == other.zhibao_used_imageid &&
			use_eternity_level == other.use_eternity_level &&
			footprint_used_imageid == other.footprint_used_imageid &&
			cloak_used_imageid == other.cloak_used_imageid &&
			fabao_used_imageid == other.fabao_used_imageid &&
			jingjie_imageid == other.jingjie_imageid &&
			yaoshi_used_imageid == other.yaoshi_used_imageid &&
			toushi_used_imageid == other.toushi_used_imageid &&
			qilinbi_used_imageid == other.qilinbi_used_imageid &&
			mask_used_imageid == other.mask_used_imageid && 
			upgrade_used_imageid_0 == other.upgrade_used_imageid_0 &&
			upgrade_used_imageid_1 == other.upgrade_used_imageid_1 &&
			upgrade_used_imageid_2 == other.upgrade_used_imageid_2 &&
			upgrade_used_imageid_3 == other.upgrade_used_imageid_3 &&
			upgrade_used_imageid_4 == other.upgrade_used_imageid_4 &&
			upgrade_used_imageid_5 == other.upgrade_used_imageid_5 &&
			upgrade_used_imageid_6 == other.upgrade_used_imageid_6 &&
			upgrade_used_imageid_7 == other.upgrade_used_imageid_7 &&
			upgrade_used_imageid_8 == other.upgrade_used_imageid_8 &&
			shenbing_image_id == other.shenbing_image_id &&
			shenbing_texiao_id == other.shenbing_texiao_id &&
			baojia_image_id == other.baojia_image_id &&
			baojia_texiao_id == other.baojia_texiao_id;
	}

	ItemID wuqi_id;							// 武器ID
	char shizhuang_wuqi;					// 时装武器
	char shizhuang_wuqi_is_special;			// 时装武器
	char shizhuang_body;					// 时装身体
	char shizhuang_body_is_special;			// 时装身体
	short mount_used_imageid;				// 坐骑形象
	short wing_used_imageid;				// 羽翼形象
	short halo_used_imageid;				// 光环形象
	short shengong_used_imageid;			// 神弓形象
	short shenyi_used_imageid;				// 神翼形象
	short xiannvshouhu_imageid;				// 仙女守护
	short jingling_guanghuan_imageid;		// 精灵光环
	short jingling_fazhen_imageid;			// 精灵法阵
	short fight_mount_used_imageid;			// 战斗坐骑形象
	short zhibao_used_imageid;				// 至宝形象
	short use_eternity_level;				// 最小永恒等级
	short footprint_used_imageid;			// 足迹形象
	short cloak_used_imageid;				// 披风形象
	short fabao_used_imageid;               // 法宝形象
	short jingjie_imageid;                  // 境界形象
	short yaoshi_used_imageid;				// 腰饰形象id
	short toushi_used_imageid;				// 头饰形象id
	short qilinbi_used_imageid;				// 麒麟臂形象id
	short mask_used_imageid;				// 面具形象id
	short reserve_sh;
	short upgrade_used_imageid_0;			// 进阶系统形象id
	short upgrade_used_imageid_1;			// 进阶系统形象id
	short upgrade_used_imageid_2;			// 进阶系统形象id
	short upgrade_used_imageid_3;			// 进阶系统形象id
	short upgrade_used_imageid_4;			// 进阶系统形象id
	short upgrade_used_imageid_5;			// 进阶系统形象id
	short upgrade_used_imageid_6;			// 进阶系统形象id
	short upgrade_used_imageid_7;			// 进阶系统形象id
	short upgrade_used_imageid_8;			// 进阶系统形象id

	short shenbing_image_id;				// 神兵形象id
	short shenbing_texiao_id;				// 神兵特效id
	short baojia_image_id;					// 宝甲形象id
	short baojia_texiao_id;					// 宝甲特效id
};

typedef char RoleAppearanceHex[sizeof(RoleAppearance) * 2 + 1];
UNSTD_STATIC_CHECK(sizeof(RoleAppearanceHex) < 256);

#endif
