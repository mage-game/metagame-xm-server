#include "jinjiesysfunchelper.hpp"
#include "other/upgrade/upgrademanager.hpp"
#include "obj/character/role.h"
#include "other/cloak/cloakmanager.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/halo/halomanager.hpp"
#include "other/footprint/footprint.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/yaoshi/yaoshimanager.hpp"
#include "other/toushi/toushimanager.hpp"
#include "other/qilinbi/qilinbimanager.hpp"
#include "other/mask/maskmanager.hpp"
#include "other/fabao/fabaomanager.hpp"
#include "other/shizhuang/shizhuang.h"
//#include "other/xianbao/xianbaomanager.hpp"
//#include "other/lingzhu/lingzhumanager.hpp"
//#include "other/lingchong/lingchongmanager.hpp"
//#include "other/linggong/linggongmanager.hpp"
//#include "other/lingqi/lingqimanager.hpp"
//#include "other/weiyan/weiyan.hpp"
#include "other/jinjiesysreward/jinjiesysrewardmgr.hpp"
//#include "other/shouhuan/shouhuanmanager.hpp"

#define INIT_GET_GRADE_FUNC(sys_type, func)\
{\
	m_func_list[sys_type].get_grade_func = [this](Role *role)->int\
	{\
		if (nullptr == role) \
		{ \
			return 0; \
		} \
		return func;\
	};\
}

#define INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(sys_type, func) \
{ \
	m_func_list[sys_type].check_special_img_func = [this](Role *role, int img_id) -> bool \
	{ \
		if (nullptr == role) \
		{ \
			return false; \
		} \
		return func(img_id); \
	}; \
}

#define INIT_ACTIVE_SPECIAL_IMG_FUNC(sys_type, func) \
{ \
	m_func_list[sys_type].active_special_img_func = [this](Role *role, int img_id) -> bool \
	{ \
		if (nullptr == role) \
		{ \
			return false; \
		} \
		return func(img_id); \
	}; \
}

JinjiesysFuncHelper::JinjiesysFuncHelper()
{
	this->InitGetGradeFunc();
	this->InitCheckSpecialImgActiveFunc();
	this->InitActiveSpecialImgFunc();
}

const JinjiesysFuncStruct * JinjiesysFuncHelper::GetFuncStruct(int jinjie_type)
{
	if (jinjie_type < JINJIE_TYPE_CLOAK || jinjie_type >= JINJIE_TYPE_MAX)
	{
		return nullptr;
	}

	return &m_func_list[jinjie_type];
}

void JinjiesysFuncHelper::OnActiveJinjieSys(Role *role, int jinjie_type)
{
	JinjieSysRewardMgr::Instance().OnActiveJinjieSys(role, jinjie_type);
}

void JinjiesysFuncHelper::OnActiveSpecialImg(Role *role, int jinjie_type, int special_img_id)
{
	if (nullptr == role)
	{
		return;
	}

	if (jinjie_type < JINJIE_TYPE_CLOAK || jinjie_type >= JINJIE_TYPE_MAX)
	{
		return;
	}

	JinjieSysRewardMgr::Instance().CheckJinjieSysReward(role, jinjie_type, special_img_id);
}

void JinjiesysFuncHelper::OnUpgrade(Role *role, int jinjie_type, int old_grade, int new_grade)
{
	if (nullptr == role)
	{
		return;
	}

	if (jinjie_type < JINJIE_TYPE_CLOAK || jinjie_type >= JINJIE_TYPE_MAX)
	{
		return;
	}

	if (old_grade >= new_grade)
	{
		return;
	}

	JinjieSysRewardMgr::Instance().OnJinjieSysRewardUpgrade(role, jinjie_type, new_grade);
}

void JinjiesysFuncHelper::InitGetGradeFunc()
{
	UNSTD_STATIC_CHECK(24 == JINJIE_TYPE_MAX);

	INIT_GET_GRADE_FUNC(JINJIE_TYPE_CLOAK, role->GetCloakManager()->GetCloakLevel());
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_FIGHT_MOUNT, role->GetFightMountManager()->GetGrade());
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_FOOTPRINT, role->GetFootPrint()->GetGrade());
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_HALO, role->GetHalo()->GetGrade());
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_MASK, role->GetMaskManager()->GetGrade());
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_MOUNT, role->GetMountManager()->GetGrade());
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_QILINBI, role->GetQiLinBiManager()->GetGrade());
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_SHENGONG, role->GetShengong()->GetGrade());
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_SHENYI, role->GetShenyi()->GetGrade());
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_TOUSHI, role->GetTouShiManager()->GetGrade());
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_WING, role->GetWing()->GetGrade());
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_YAOSHI, role->GetYaoShiManager()->GetGrade());
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_LINGZHU, role->GetUpgradeManager()->GetGrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_LINGZHU)));
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_XIANBAO, role->GetUpgradeManager()->GetGrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_XIANBAO)));
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_LINGGONG, role->GetUpgradeManager()->GetGrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_LINGGONG)));
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_LINGQI, role->GetUpgradeManager()->GetGrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_LINGQI)));
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_LINGCHONG, role->GetUpgradeManager()->GetGrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_LINGCHONG)));
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_WEIYAN, role->GetUpgradeManager()->GetGrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_WEIYAN)));
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_SHOUHUAN, role->GetUpgradeManager()->GetGrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_SHOUHUAN)));
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_TAIL, role->GetUpgradeManager()->GetGrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_TAIL)));
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_FLYPET, role->GetUpgradeManager()->GetGrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_FLYPET)));
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_FABAO, role->GetFabaoManager()->GetGrade());
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_SHIZHUANG, role->GetShizhuang()->GetShiZhuangGrade(SHIZHUANG_TYPE_BODY));
	INIT_GET_GRADE_FUNC(JINJIE_TYPE_SHENBING, role->GetShizhuang()->GetShiZhuangGrade(SHIZHUANG_TYPE_WUQI));

}

void JinjiesysFuncHelper::InitCheckSpecialImgActiveFunc()
{
	UNSTD_STATIC_CHECK(24 == JINJIE_TYPE_MAX);

	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_CLOAK, role->GetCloakManager()->GetCloakParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_FIGHT_MOUNT, role->GetFightMountManager()->GetFightMountParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_FOOTPRINT, role->GetFootPrint()->GetFootprintParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_HALO, role->GetHalo()->GetHaloParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_MASK, role->GetMaskManager()->GetMaskParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_MOUNT, role->GetMountManager()->GetMountParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_QILINBI, role->GetQiLinBiManager()->GetQiLinBiParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_SHENGONG, role->GetShengong()->GetShengongParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_SHENYI, role->GetShenyi()->GetShenyiParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_TOUSHI, role->GetTouShiManager()->GetTouShiParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_WING, role->GetWing()->GetWingParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_YAOSHI, role->GetYaoShiManager()->GetYaoShiParam()->IsSpecialImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_LINGZHU, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_LINGZHU))->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_XIANBAO, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_XIANBAO))->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_LINGGONG, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_LINGGONG))->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_LINGQI, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_LINGQI))->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_LINGCHONG, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_LINGCHONG))->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_WEIYAN, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_WEIYAN))->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_SHOUHUAN, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_SHOUHUAN))->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_TAIL, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_TAIL))->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_FLYPET, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_FLYPET))->IsImageActive);
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_FABAO, role->GetFabaoManager()->GetParam()->IsSpecialImageActive);
	UNSTD_STATIC_CHECK(SHIZHUANG_TYPE_BODY > SHIZHUANG_TYPE_INVALID && SHIZHUANG_TYPE_BODY < SHIZHUANG_TYPE_MAX);	//GetShizhuangItem可能为空,由静态检查确保传入的是正确的参数
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_SHIZHUANG, role->GetShizhuang()->GetParam()->GetShizhuangItem(SHIZHUANG_TYPE_BODY)->IsSpecialImageActive);
	UNSTD_STATIC_CHECK(SHIZHUANG_TYPE_WUQI > SHIZHUANG_TYPE_INVALID && SHIZHUANG_TYPE_WUQI < SHIZHUANG_TYPE_MAX);	//GetShizhuangItem可能为空,由静态检查确保传入的是正确的参数
	INIT_CHECK_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_SHENBING, role->GetShizhuang()->GetParam()->GetShizhuangItem(SHIZHUANG_TYPE_WUQI)->IsSpecialImageActive);
}

void JinjiesysFuncHelper::InitActiveSpecialImgFunc()
{
	UNSTD_STATIC_CHECK(24 == JINJIE_TYPE_MAX);

	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_CLOAK, role->GetCloakManager()->ActiveSpecialImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_FIGHT_MOUNT, role->GetFightMountManager()->ActiveSpecialImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_FOOTPRINT, role->GetFootPrint()->ActiveSpecialImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_HALO, role->GetHalo()->ActiveSpecialImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_MASK, role->GetMaskManager()->ActiveSpecialImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_MOUNT, role->GetMountManager()->ActiveSpecialImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_QILINBI, role->GetQiLinBiManager()->ActiveSpecialImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_SHENGONG, role->GetShengong()->ActiveSpecialImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_SHENYI, role->GetShenyi()->ActiveSpecialImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_TOUSHI, role->GetTouShiManager()->ActiveSpecialImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_WING, role->GetWing()->ActiveSpecialImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_YAOSHI, role->GetYaoShiManager()->ActiveSpecialImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_LINGZHU, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_LINGZHU))->ActiveImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_XIANBAO, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_XIANBAO))->ActiveImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_LINGGONG, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_LINGGONG))->ActiveImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_LINGQI, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_LINGQI))->ActiveImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_LINGCHONG, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_LINGCHONG))->ActiveImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_WEIYAN, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_WEIYAN))->ActiveImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_SHOUHUAN, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_SHOUHUAN))->ActiveImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_TAIL, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_TAIL))->ActiveImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_FLYPET, role->GetUpgradeManager()->GetUpgrade(this->JinJieSysTypeToUpgradeManagerType(JINJIE_TYPE_FLYPET))->ActiveImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_FABAO, role->GetFabaoManager()->ActiveSpecialImg);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_SHIZHUANG, role->GetShizhuang()->ActiveBodySpecialImage);
	INIT_ACTIVE_SPECIAL_IMG_FUNC(JINJIE_TYPE_SHENBING, role->GetShizhuang()->ActiveWuqiSpecialImage);
}

int JinjiesysFuncHelper::JinJieSysTypeToUpgradeManagerType(int type)
{
	switch (type)
	{
	case JINJIE_TYPE_LINGZHU: return UPGRADE_TYPE_0;
	case JINJIE_TYPE_XIANBAO: return UPGRADE_TYPE_1;
	case JINJIE_TYPE_LINGGONG: return UPGRADE_TYPE_3;
	case JINJIE_TYPE_LINGQI: return UPGRADE_TYPE_4;
	case JINJIE_TYPE_LINGCHONG: return UPGRADE_TYPE_2;
	case JINJIE_TYPE_WEIYAN: return UPGRADE_TYPE_5;
	case JINJIE_TYPE_SHOUHUAN: return UPGRADE_TYPE_6;
	case JINJIE_TYPE_TAIL: return UPGRADE_TYPE_7;
	case JINJIE_TYPE_FLYPET: return UPGRADE_TYPE_8;
	default: return UPGRADE_TYPE_INVALID;
	}
}

int JinjiesysFuncHelper::UpgradeManagerTypeToJinJieSysType(int type)
{
	switch (type)
	{
	case UPGRADE_TYPE_0: return JINJIE_TYPE_LINGZHU;
	case UPGRADE_TYPE_1: return JINJIE_TYPE_XIANBAO;
	case UPGRADE_TYPE_2: return JINJIE_TYPE_LINGCHONG;
	case UPGRADE_TYPE_3: return JINJIE_TYPE_LINGGONG;
	case UPGRADE_TYPE_4: return JINJIE_TYPE_LINGQI;
	case UPGRADE_TYPE_5: return JINJIE_TYPE_WEIYAN;
	case UPGRADE_TYPE_6: return JINJIE_TYPE_SHOUHUAN;
	case UPGRADE_TYPE_7: return JINJIE_TYPE_TAIL;
	case UPGRADE_TYPE_8: return JINJIE_TYPE_FLYPET;
	default: return JINJIE_TYPE_INVALID;
	}
}
