#include "norexitem.hpp"

#include "obj/character/role.h"
#include "obj/character/monster.h"

#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"

#include "monster/monsterpool.h"
#include "scene/scene.h"
#include "scene/map.h"
#include "scene/speciallogic/speciallogic.hpp"

#include "world.h"
#include "servercommon/string/gameworldstr.h"

#include "globalconfig/globalconfig.h"

#include "other/mount/mountfunction.hpp"

#include "scene/activityshadow/activityshadow.hpp"

#include "gamelog.h"

#include "config/logicconfigmanager.hpp"
#include "config/guildconfig.hpp"
#include "config/shuxingdanconfig.hpp"
#include "config/expbottleconfig.hpp"
#include "limits.h"
#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectattrcontainer.hpp"
#include "effect/skilleffect/effectotherbuff.hpp"

#include "global/globalrole/globalrole.hpp"
#include "global/globalrole/globalrolemanager.hpp"
#include "global/globalrole/litttlepet/littlepet.hpp"
#include "other/fightmount/fightmountfunction.hpp"
#include "other/wing/wingfunction.hpp"
#include "other/halo/halofunction.hpp"
#include "other/shengong/shengongfunction.hpp"
#include "other/shenyi/shenyifunction.hpp"

#include "equipment/equipmentmanager.h"
#include "other/jingling/jinglingmanager.h"
#include "global/guild/guildmanager.hpp"
#include "global/guild/roleguild.hpp"
#include "other/runesystem/runesystem.hpp"
#include "other/xiannv/xiannvmanager.hpp"
#include "other/mentality/mentality.hpp"
#include "other/shop/roleshop.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/fb/rolefbequip.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/halo/halomanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/footprint/footprint.hpp"
#include "other/cloak/cloakmanager.hpp"
#include "other/specialappearance/specialappearance.hpp"
#include "other/supply/rolesupply.hpp"
#include "other/shizhuang/shizhuang.h"
#include "other/cross/rolecross.h"
#include "other/shengwang/shengwang.h"
#include "other/chengjiu/chengjiu.h"
#include "other/pet/pet.hpp"
#include "other/multimount/multimount.hpp"
#include "other/zhibao/zhibaomanager.hpp"
#include "other/shenbing/shenbingmanager.hpp"
#include "other/huashen/huashen.hpp"
#include "other/magiccard/magiccard.hpp"
#include "other/jinglingguanghuan/jinglingguanghuanmanager.hpp"
#include "other/jinglingfazhen/jinglingfazhenmanager.hpp"
#include "other/zhuanshengequip/zhuanshengequip.hpp"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include "other/shengesystem/shengesystem.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "other/xiannv/xiannvmanager.hpp"
#include "other/lieming/liemingmanager.h"
#include "other/shenshou/shenshou.hpp"
#include "other/shenyin/shenyin.hpp"
#include "other/fabao/fabaomanager.hpp"
#include "other/qingyuan/qingyuanconfig.hpp"
#include "other/upgrade/upgrademanager.hpp"
#include "other/yaoshi/yaoshimanager.hpp"
#include "other/yaoshi/yaoshifunction.hpp"
#include "other/toushi/toushifunction.hpp"
#include "other/toushi/toushimanager.hpp"
#include "other/qilinbi/qilinbifunction.hpp"
#include "other/qilinbi/qilinbimanager.hpp"
#include "other/mask/maskmanager.hpp"
#include "other/mask/maskfunction.hpp"
#include "other/talent/talent.hpp"
#include "other/welfare/welfare.hpp"
#include "other/tianshenhuti/tianshenhuti.hpp"

#include "scene/speciallogic/worldspecial/specialhunyan.hpp"

#include "global/globalrole/qingyuanglobal/qingyuanglobal.hpp"
#include "other/rolebigsmallgoal/rolebigsmallgoal.hpp"
#include "other/xianzunka/xianzunka.hpp"
#include "other/greatesoldier/greatesoldier.hpp"
#include "other/zodiac/zodiac.hpp"
#include "other/crossequip/crossequip.hpp"

NorexItem::NorexItem() 
	: ItemBase(I_TYPE_EXPENSE), m_use_type(0), m_use_daytimes(0), m_colddown_id(0), m_server_colddown(0), m_param1(0), m_param2(0), m_param3(0), m_param4(0), m_param5(0)
{

}

NorexItem::~NorexItem()
{

}

int NorexItem::Use(Role *role, const ItemGridData &item_data, short &num, short equip_index) const
{
	if (NULL == role || NULL == role->GetScene() || num <= 0 ) return -1;

	bool succ_msg = true;

	if (m_colddown_id > 0 && m_server_colddown > 0 && role->GetKnapsack()->GetItemColddownManager()->HasColddown(m_colddown_id))
	{
		role->NoticeNum(errornum::EN_ITEM_COLDDOWN);
		return -999;
	}

	if (m_use_daytimes > 0)
	{
		short remain_times = m_use_daytimes - role->GetUseNorexItemDayTimes(m_item_id);
		if (remain_times <= 0)
		{
			role->NoticeNum(errornum::EN_ITEM_USE_DAY_TEIMS_LIMIT);
			return -1000;
		}
		else
		{
			num = num > remain_times ? remain_times : num;
		}
	}

	if (num > 1)
	{
		switch(m_use_type)
		{
		case I_NOREX_ITEM_JINGYAN_DAN:
		case I_NOREX_ITEM_BIND_COIN_CARD:
		case I_NOREX_ITEM_XIANHUN_DAN:
		case I_NOREX_ITEM_YUANLI_DAN:
		case I_NOREX_ITEM_GOLD_BIND_CARD:
		case I_NOREX_ITEM_SHUXINGDAN:
		case I_NOREX_ITEM_HUNLI:
		case I_NOREX_ITEM_LINGJING:
		case I_NOREX_ITEM_MOJING:
		case I_NOREX_ITEM_COIN_CARD:
		case I_NOREX_ITEM_SHENGWANG:
		case I_NOREX_ITEM_CHENGJIU:
		case I_NOREX_ITEM_EXP_BOTTLE:
		case I_NOREX_ITEM_HUOLI:
		case I_NOREX_ITEM_ZHUANSHENG_XIUWEI_DAN:
		case I_NOREX_ITEM_RUNE_TOWER_OFFLINE_TIME:
		case I_NOREX_ITEM_SHENGE_SUIPIAN:
		case I_NOREX_ITEM_XIANNV_SHENGWU_ADD_LINGYE:
		case I_NOREX_ITEM_RUNE:
		case I_NOREX_ITEM_LIEMING:
		case I_NOREX_ITEM_REDUCE_WEARY:
		case I_NOREX_ITEM_MARRY_EXP:
		case I_NOREX_ITEM_SHENSHOU:
		case I_NOREX_ITEM_SHENYIN:
		case I_NOREX_ITEM_TIAN_XIANG:
		case I_NOREX_ITEM_YAOSHI_UP_ONE_GRADE_CARD:
		case I_NOREX_ITEM_TOUSHI_UP_ONE_GRADE_CARD:
		case I_NOREX_ITEM_QILINBI_UP_ONE_GRADE_CARD:
		case I_NOREX_ITEM_MASK_UP_ONE_GRADE_CARD:
		case I_NOREX_ITEM_UPGRADE_SYS_UP_ONE_GRADE_CARD:
		case I_NOREX_ITEM_BIG_SMALL_GOAL_SPECIAL_ATTR:
		case I_NOREX_ITEM_RUNE_BOX:
		case I_NOREX_ITEM_CROSS_HONOR:
		case I_NOREX_ITEM_TALENT_SKILL_BOOK:
		case I_NOREX_ITEM_SHENGWU_JINGHUA:
		case I_NOREX_ITEM_JINGLING_HUNLI:
		case I_NOREX_ITEM_GUILD_GONGZI:
		case I_NOREX_ITEM_DOUQIDAN:
		case I_NOREX_ITEM_ZODIAC:
		case I_NOREX_ITEM_TIANSHENHUTI_EQUIP:
			break;

		default:
			role->NoticeNum(errornum::EN_ITEM_CANNOT_MULTI_USE);
			return -1;
		}
	}

	switch (m_use_type)
	{
	case I_NOREX_ITEM_JINGYAN_DAN:
		{
			if (!role->AddExp((long long)m_param1 * num, "ItemUse:JingYanDan", Role::EXP_ADD_REASON_EXP_ITEM))
			{
				return -1;
			}
		}
		break;

	case I_NOREX_ITEM_BIND_COIN_CARD:
		{
			if (!role->GetKnapsack()->GetMoney()->AddCoinBind((long long)m_param1 * num, "ItemUse:BindCoinCard"))
			{
				return -2;
			}
		}
		break;

	case I_NOREX_ITEM_TITLE_CARD:
		{
			if (!role->GetTitleManager()->OnActive(m_param1, true, true, true, true))
			{
				return -3;
			}
		}
		break;

	case I_NOREX_ITEM_XIANHUN_DAN:
		{
			if (!role->AddXianhun(m_param1 * num, "ItemUse:XianhunDan"))
			{
				return -5;
			}
		}
		break;

	case I_NOREX_ITEM_SHENGJI_DAN:
		{
			int level_limit = m_param1; 
			long long add_exp = m_param2;
			int add_level = m_param3;
			
			if (role->GetLevel() >= level_limit)
			{
				if (!role->AddExp(add_exp, "ShengjiDanItem", Role::EXP_ADD_REASON_UPLEVEL_PILL))
				{
					return -6;
				}
			}
			else
			{
				for (int i = 0; i < add_level; ++ i)
				{
					add_exp = GLOBALCONFIG->GetRoleMaxExp(role->GetLevel());

					if (!role->AddExp(add_exp, "ShengjiDanItem", Role::EXP_ADD_REASON_UPLEVEL_PILL))
					{
						return -6;
					}
				}
			}
		}
		break;

	case I_NOREX_ITEM_YUANLI_DAN:
		{
			if (!role->AddYuanli(m_param1 * num, "ItemUse:YuanliDan"))
			{
				return -7;
			}
		}
		break;

	case I_NOREX_ITEM_GOLD_BIND_CARD:
		{
			if (!role->GetKnapsack()->GetMoney()->AddGoldBind(m_param1 * num, "ItemUse:GoldBindCard"))
			{
				return -8;
			}
		}
		break;

	case I_NOREX_ITEM_WING_BLESS_CARD:
		{
// 			if (!role->GetWing()->OnUseWingBlessCard(m_param1, m_param2))
// 			{
// 				return -8;
// 			}
			return -9;
		}
		break;

	case I_NOREX_ITEM_TILI_CARD:
		{
			if (!role->AddEnergyIgnoreOverRange(m_param1, "UseTiliCard"))
			{
				return -10;
			}
		}
		break;

	case I_NOREX_ITEM_FB_EQUIP_CARD:
		{
			if (!role->GetFBEquip()->OnNeqItemBuyTimes(m_param1))
			{
				return -11;
			}
		}
		break;

	case I_NOREX_ITEM_WING_SPECIAL_IMG_CARD:
		{
			if (!role->GetWing()->ActiveSpecialImage(m_param1))
			{
				return -12;
			}
		}
		break;

	case I_NOREX_ITEM_SPECIAL_MOUNT_IMG_CARD:
		{
			if (!role->GetMountManager()->ActiveSpecialImage(m_param1))
			{
				return -13;
			}
		}
		break;

	case I_NOREX_ITEM_SPECIAL_APPEARANCE_CARD:
		{
			if (!role->GetSpecialAppearance()->OnUseSpecialAppearanceCard(static_cast<short>(m_param1), m_param2))
			{
				return -14;
			}
		}
		break;
		
	case I_NOREX_ITEM_MOUNT_JINJIE_CARD:
		{
			if(!MountFunction::UpgradeByItem(role, m_param1, m_param2))
			{
				return -15;
			}
		}
		break;

	case I_NOREX_ITEM_REDUCE_EVIL_CARD:
		{
			if (role->GetEvil() <= 0 || !role->AddEvil(-m_param1))
			{
				role->NoticeNum(errornum::EN_KNAPSACK_USE_EVIEL_DECCARD);
				return -16;
			}
		}
		break;

	case I_NOREX_ITEM_QIBING_JINJIE_CARD:
		{
			return -17;
		}
		break;

	case I_NOREX_ITEM_SHIZHUANG_CARD:
		{
			if (!role->GetShizhuang()->ActiveSpecialImg(m_param1, m_param2))
			{
				return -18;
			}
		}
		break;

	case I_NOREX_ITEM_RED_PAPER_CARD:
		{
			if (0 == RedPaperManager::Instance().OnCreateRedPaper(role, RED_PAPER_TYPE_GLOBAL, INT_MAX, m_param2, false))
			{
				return -19;
			}
		}
		break;

	case I_NOREX_ITEM_SHUXINGDAN:
		{
			bool ret = false;

			//进阶系统属性丹类型
			if (m_param1 >= SHUXINGDAN_TYPE_UPGRADE_BEGIN)
			{
				int upgrade_type = m_param1 - SHUXINGDAN_TYPE_UPGRADE_BEGIN;
				ret = role->GetUpgradeManager()->OnUseShuxingdan(upgrade_type, m_param2, num);
			}
			else
			{
				switch (m_param1)
				{
				case SHUXINGDAN_TYPE_XIULIAN:
				{
					ret = role->GetMentality()->OnUseShuxingdan(m_param2, num);
				}
				break;

				case SHUXINGDAN_TYPE_CHENGJIU:
				{
					ret = role->GetChengJiuMgr()->OnUseShuxingdan(m_param2, num);
				}
				break;

				case SHUXINGDAN_TYPE_SHENGWANG:
				{
					ret = role->GetShengWangMgr()->OnUseShuxingdan(m_param2, num);
				}
				break;

				case SHUXINGDAN_TYPE_MOUNT:
				{
					ret = role->GetMountManager()->OnUseShuxingdan(m_param2, num);
				}
				break;

				case SHUXINGDAN_TYPE_WING:
				{
					ret = role->GetWing()->OnUseShuxingdan(m_param2, num);
				}
				break;

				case SHUXINGDAN_TYPE_HALO:
				{
					ret = role->GetHalo()->OnUseShuxingdan(m_param2, num);
				}
				break;

				case SHUXINGDAN_TYPE_SHENGONG:
				{
					ret = role->GetShengong()->OnUseShuxingdan(m_param2, num);
				}
				break;

				case SHUXINGDAN_TYPE_SHENYI:
				{
					ret = role->GetShenyi()->OnUseShuxingdan(m_param2, num);
				}
				break;

				case SHUXINGDAN_TYPE_FIGHTMOUNT:
				{
					ret = role->GetFightMountManager()->OnUseShuxingdan(m_param2, num);
				}
				break;

				case SHUXINGDAN_TYPE_SHENBING:
				{
					ret = role->GetShenBingManager()->OnUseShuxingdan(m_param2, num);
				}
				break;

				case SHUXINGDAN_TYPE_FOOTPRINT:
				{
					ret = role->GetFootPrint()->OnUseShuxingdan(m_param2, num);
				}
				break;

				case SHUXINGDAN_TYPE_CLOAK:
				{
					ret = role->GetCloakManager()->OnUseShuxingdan(m_param2, num);
				}
				break;

				case SHUXINGDAN_TYPE_FABAO:
				{
					ret = role->GetFabaoManager()->OnUseShuxingdan(m_param2, num);
				}
				break;

				case SHUXINGDAN_TYPE_SHIZHUANG:
				{
					ret = role->GetShizhuang()->OnUseShuxingdan(SHIZHUANG_TYPE_BODY, num);
				}
				break;

				case SHUXINGDAN_TYPE_SHIZHUANG_SHENBING:
				{
					ret = role->GetShizhuang()->OnUseShuxingdan(SHIZHUANG_TYPE_WUQI, num);
				}
				break;

				case SHUXINGDAN_TYPE_YAOSHI:
				{
					ret = role->GetYaoShiManager()->OnUseShuxingdan(m_param2, num);
				}
				break;

				case SHUXINGDAN_TYPE_TOUSHI:
				{
					ret = role->GetTouShiManager()->OnUseShuxingdan(m_param2, num);
				}
				break;

				case SHUXINGDAN_TYPE_QILINBI:
				{
					ret = role->GetQiLinBiManager()->OnUseShuxingdan(m_param2, num);
				}
				break;

				case SHUXINGDAN_TYPE_MASK:
				{
					ret = role->GetMaskManager()->OnUseShuxingdan(m_param2, num);
				}
				break;

				}
			}

			if (!ret)
			{
				return -20;
			}
		}
		break;
	
	case I_NOREX_ITEM_HUNLI:
		{
			if (!role->AddHunli(m_param1 * num, "ItemUse:Hunli"))
			{
				return -21;
			}
		}
		break;

	case I_NOREX_ITEM_LINGJING:
		{
			if (!role->AddLingJing(m_param1 * num, "ItemUse:Lingjing"))
			{
				return -22;
			}
		}
		break;

	case I_NOREX_ITEM_MOJING:
		{
			if (!role->GetRoleShop()->AddChestShopMojing(m_param1 * num, "ItemUse:Mojing"))
			{
				return -23;
			}
		}
		break;

	case I_NOREX_ITEM_COIN_CARD:
		{
			if (!role->GetKnapsack()->GetMoney()->AddCoin(m_param1 * num, "ItemUse:CoinCard"))
			{
				return -24;
			}
		}
		break;

	case I_NOREX_ITEM_SHENGWANG:
		{
			//if (!role->AddShengwang(m_param1 * num, "ItemUse:ShengWang"))
			if (!role->GetRoleShop()->AddChestShopShengwang(m_param1 * num, "ItemUse:ShengWang"))
			{
				return -25;
			}
		}
		break;

	case I_NOREX_ITEM_CHENGJIU:
		{
			if (!role->AddChengJiu(m_param1 * num, "ItemUse:ChengJiu"))
			{
				return -26;
			}
		}
		break;

	case I_NOREX_ITEM_HP:
		{
			if (!role->GetScene()->GetSpecialLogic()->SpecialCanRcoverHpByMedicine())
			{
				role->NoticeNum(errornum::EN_USE_ITEM_SCENE_LIMIT);
				return -27;
			}

			if (role->IsInForbidRcover())  // 角色禁止回血
			{
				role->NoticeNum(errornum::EN_ROLE_STATE_CANNT_RECOVER);
				return -27;
			}

			Attribute add_hp_per = Attribute(m_param1);

			Attribute add_hp = static_cast<Attribute>(role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (add_hp_per * 0.01));
			role->ChangeHp(&add_hp, true);
		}
		break;

	// 气血持续回复
	case I_NOREX_ITEM_HP_CONTINUE:
		{
			// 满血
			if (role->IsFullHp())
			{
				return -28;
			}

			// 是否能回复血量
			if (!role->GetScene()->GetSpecialLogic()->SpecialCanRcoverHpByMedicine())
			{
				role->NoticeNum(errornum::EN_USE_ITEM_SCENE_LIMIT);
				return -28;
			}

			if (role->IsInForbidRcover())  // 角色禁止回血
			{
				role->NoticeNum(errornum::EN_ROLE_STATE_CANNT_RECOVER);
				return -28;
			}

			int level = m_param1, time = 0, interval = 1000, total_hp = m_param2, fight_hp = m_param3, idle_hp = m_param4;
			EffectAttrContainer *buff = role->GetEffectList()->FindAttrContainerBuff(EffectBase::PRODUCT_ID_HP_CONTINUE);
			if (NULL != buff && buff->GetProductLevel() > level)
			{
				role->NoticeNum(errornum::EN_HAS_BUFF_MORE_LEVEL);
				return -28;
			}

			EffectAttrContainer *effect = new EffectAttrContainer(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_HP_CONTINUE, time, level, EffectBase::PRODUCT_METHOD_ITEM);
			effect->SetContinueHp(interval, total_hp, fight_hp, idle_hp);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_6);

// 			int level = m_param1, time = m_param2 * 1000, interval = m_param3 * 1000, interval_addhp = m_param4;
// 			
// 			EffectOtherBuff *buff = role->GetEffectList()->FindOtherEffectBuff(EffectBase::PRODUCT_ID_HP_CONTINUE);
// 			if (NULL != buff && buff->GetProductLevel() > level)
// 			{
// 				role->NoticeNum(errornum::EN_HAS_BUFF_MORE_LEVEL);
// 				return -26;
// 			}
// 
// 			EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_HP_CONTINUE, time, level, EffectBase::PRODUCT_METHOD_ITEM);
// 			effect->SetContinueHp(interval, interval_addhp);
// 			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_5);
// 			
			if (1 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_HP1);
			if (2 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_HP2);
			if (3 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_HP3);
			if (4 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_HP4);

			effect->SetSave(true); // 存盘
			role->AddEffect(effect);
		}
		break;

	// 魔法持续回复
	case I_NOREX_ITEM_MP_CONTINUE:
		{
			// TODO: 使用新的效果 EffectAttrContainer
			// 满蓝
			if (role->IsFullMp())
			{
				return -29;
			}

			int level = m_param1, time = 0, interval = 1000, total_mp = m_param2, fight_mp = m_param3, idle_mp = m_param4;

			EffectAttrContainer *buff = role->GetEffectList()->FindAttrContainerBuff(EffectBase::PRODUCT_ID_MP_CONTINUE);
			if (NULL != buff && buff->GetProductLevel() > level)
			{
				role->NoticeNum(errornum::EN_HAS_BUFF_MORE_LEVEL);
				return -29;
			}

			EffectAttrContainer *effect = new EffectAttrContainer(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_MP_CONTINUE, time, level, EffectBase::PRODUCT_METHOD_ITEM);
			effect->SetContinueMp(interval, total_mp, fight_mp, idle_mp);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_6);

// 			int level = m_param1, time = m_param2 * 1000, interval = m_param3 * 1000, interval_addmp = m_param4;
// 
// 			EffectOtherBuff *buff = role->GetEffectList()->FindOtherEffectBuff(EffectBase::PRODUCT_ID_MP_CONTINUE);
// 			if (NULL != buff && buff->GetProductLevel() > level)
// 			{
// 				role->NoticeNum(errornum::EN_HAS_BUFF_MORE_LEVEL);
// 				return -27;
// 			}
// 
// 			EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_MP_CONTINUE, time, level, EffectBase::PRODUCT_METHOD_ITEM);
// 			effect->SetContinueMp(interval, interval_addmp);
// 			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_5);
		
			if (1 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_MP1);
			if (2 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_MP2);
			if (3 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_MP3);
			if (4 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_MP4);

			effect->SetSave(true);
			role->AddEffect(effect);
		}
		break;

	case I_NOREX_ITEM_ADDJINGZHUN:
		{
			int level = m_param1, time = m_param2 * 1000, add_jingzhunper = m_param3;
			
			EffectOtherBuff *buff = role->GetEffectList()->FindOtherEffectBuff(EffectBase::PRODUCT_ID_BASE_ATTR_ADD_JINGZHUN);
			if (NULL != buff && buff->GetProductLevel() > level)
			{
				role->NoticeNum(errornum::EN_HAS_BUFF_MORE_LEVEL);
				return -30;
			}

			EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_BASE_ATTR_ADD_JINGZHUN, time, level, EffectBase::PRODUCT_METHOD_ITEM);
			effect->SetRoleBaseAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN, add_jingzhunper);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_5);

			if (1 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_JZ1);
			if (2 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_JZ2);
			if (3 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_JZ3);
			if (4 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_JZ4);

			role->AddEffect(effect);
		}
		break;

	case I_NOREX_ITEM_ADDBAOJI:
		{
			int level = m_param1, time = m_param2 * 1000, add_baoji_per = m_param3; 
			
			EffectOtherBuff *buff = role->GetEffectList()->FindOtherEffectBuff(EffectBase::PRODUCT_ID_BASE_ATTR_ADD_BAOJI);
			if (NULL != buff && buff->GetProductLevel() > level)
			{
				role->NoticeNum(errornum::EN_HAS_BUFF_MORE_LEVEL);
				return -31;
			}

			EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_BASE_ATTR_ADD_BAOJI, time, level, EffectBase::PRODUCT_METHOD_ITEM);
			effect->SetRoleBaseAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT, add_baoji_per);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_5);

			if (1 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_BJ1);
			if (2 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_BJ2);
			if (3 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_BJ3);
			if (4 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_BJ4);

			role->AddEffect(effect);
		}
		break;

	case I_NOREX_ITEM_INC_PUTON_LEVEL: //
		{
			int level = m_param1, time = m_param2 * 1000, inc_level = m_param3;

			EffectOtherBuff *buff = role->GetEffectList()->FindOtherEffectBuff(EffectBase::PRODUCT_ID_INC_PUTON_LEVEL);
			if (NULL != buff && buff->GetProductLevel() > level)
			{
				role->NoticeNum(errornum::EN_HAS_BUFF_MORE_LEVEL);
				return -32;
			}

			EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_INC_PUTON_LEVEL, time, level, EffectBase::PRODUCT_METHOD_ITEM);
			effect->IncPutOnLevel(inc_level);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_5);
			
			if (1 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_YJ1);
			if (2 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_YJ2);

			role->AddEffect(effect);
		}
		break;

	case I_NOREX_ITEM_INC_EXP_LEVEL:
		{
			int level = m_param1, time = m_param2 * 1000, inc_exp_per = m_param3;

			EffectOtherBuff *buff = role->GetEffectList()->FindOtherEffectBuff(EffectBase::PRODUCT_ID_INC_EXTRAL_EXP_PER);
			if (NULL != buff && buff->GetProductLevel() > level)
			{
				role->NoticeNum(errornum::EN_HAS_BUFF_MORE_LEVEL);
				return -33;
			}

			// 检查保存低级buff信息
			if (NULL != buff && buff->GetProductLevel() < level)
			{
				role->GetWelfare()->SetSaveExpBuff();
			}

			EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_INC_EXTRAL_EXP_PER, time, level, EffectBase::PRODUCT_METHOD_ITEM);
			effect->IncExtraExpPer(inc_exp_per);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_5);

			if (1 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_EXP1);
			if (2 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_EXP2);
			if (3 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_EXP3);
			if (4 == level) effect->SetClientEffectType(EffectBase::ECT_ITEM_EXP4);

			role->AddEffect(effect);
		}
		break;

	case I_NOREX_ITEM_EXP_BOTTLE:
		{
			const ExpBottleLvCfg *lv_cfg = LOGIC_CONFIG->GetExpBottleConfig().GetExpBottleLvCfg(role->GetLevel());
			if (NULL == lv_cfg)
			{
				return -34;
			}

			CommonDataParam *common_p = role->GetCommonDataParam();

			int left_count = lv_cfg->use_maxcount - common_p->expbottle_use_count;
			if (left_count <= 0)
			{
				role->NoticeNum(errornum::EN_EXPBOTTLE_USE_LIMIT);
				return -34;
			}

			if (num > left_count)  // 修正数量
			{
				num = left_count;
			}

			role->AddExpBottleUseCount(num);

			role->AddExp((long long)m_param1 * num, "ItemUse:ExpBollte", Role::EXP_ADD_FRIEND_EXP_BOTTLE);
		}
		break;

	case I_NOREX_ITEM_FUMOFU:
		{
			 if (!role->GetEquipmentManager()->OnFumo(m_param1))
			 {
				 role->NoticeNum(errornum::EN_EQUIPMENT_FUMO_FAIL);
				 return -35;
			 }
		}
		break;

	case I_NOREX_ITEM_HUOLI:
		{
			if (!role->AddHuoli(m_param1 * num, "I_NOREX_ITEM_HUOLI"))
			{
				return -36;
			}
		}
		break;

	// 气血丹
	case I_NOREX_ITEM_HP_DAN:
		{
			if (!role->GetScene()->GetSpecialLogic()->SpecialCanRcoverHpByMedicine())
			{
				role->NoticeNum(errornum::EN_USE_ITEM_SCENE_LIMIT);
				return -37;
			}

			int level = m_param1, time = m_param2 * 1000, interval = m_param3 * 1000, interval_addhp = m_param4;

			EffectOtherBuff *buff = role->GetEffectList()->FindOtherEffectBuff(EffectBase::PRODUCT_ID_HP_DAN);
			if (NULL != buff && buff->GetProductLevel() > level)
			{
				role->NoticeNum(errornum::EN_HAS_BUFF_MORE_LEVEL);
				return -37;
			}

			EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_HP_DAN, time, level, EffectBase::PRODUCT_METHOD_ITEM);
			effect->SetContinueHp(interval, interval_addhp);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_5);
			effect->SetClientEffectType(EffectBase::ECT_HP_DAN);
			effect->SetSave(true);
			role->AddEffect(effect);
		}
		break;

	case I_NOREX_ITEM_SZ_PROTECT:
		{
			int time = m_param1 * 1000;

			EffectOtherBuff *effect = new EffectOtherBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_SZ_PROTECT, time, 0, EffectBase::PRODUCT_METHOD_ITEM);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_5);
			effect->SetClientEffectType(EffectBase::ECT_SZ_PROTECT);
			role->AddEffect(effect);
		}
		break;

	case I_NOREX_ITEM_XIANZUNKA_CARD:
	{
		if (!role->GetXianZunKa()->OnUseXianZunKa(m_param1))
		{
			return -39;
		}
	}
	break;

	case I_NOREX_ITEM_JINGLING_HUNLI:
		{
			role->GetLieMingMgr()->AddExp(m_param1 * num);
		}
		break;

	case I_NOREX_ITEM_LIEMING:
		{
			//for (int i = 0; i < num; i++)
			//{
			//	if (!role->GetLieMingMgr()->AddHunshou(m_param1))
			//	{
			//		num = i;
			//		break;
			//	}
			//}

			if (0 >= num)
			{
				return -41;
			}
		}
		break;

	case I_NOREX_ITEM_ANY_MONSTER_DROP_ACTIVITY_ITEM:
		{
			if (!role->GetRoleActivity()->OnUseAnyMonsterDropActivityItem(item_data.item_id, num))
			{
				return -42;
			}
		}
		break;

	case I_NOREX_ITEM_PET_SPECIAL_IMG_CARD:
		{
			if (!role->GetPet()->ActiveSpecialImage(m_param1))
			{
				return -43;
			}
		} 
		break;

	case I_NOREX_ITEM_HUASHEN_CARD:
		{
			if (!role->GetHuaShen()->OnUseHuaShenCard(m_param1))
			{
				return -44;
			}
		}
		break;

	case I_NOREX_ITEM_MULTI_MOUNT_CARD:
		{
			if (!role->GetMultiMount()->OnUseMultiMountCard(m_param1, m_param2))
			{
				return -45;
			}
		}
		break;

	case I_NOREX_ITEM_MULTI_MOUNT_SPECIAL_IMG_CARD:
		{
			if (!role->GetMultiMount()->ActiveSpecialImage(m_param1))
			{
				return -46;
			}
		}
		break;

	case I_NOREX_ITEM_MAGIC_CARD:
		{
			if (!role->GetMagicCard()->ActiveCard(item_data.item_id, num))
			{
				return -47;
			}
		}
		break;

	case I_NOREX_ITEM_WUSHANG_EQUIP:
		{
			return -48;
		}
		break;

	case I_NOREX_ITEM_CROSS_BOSS_SCORE_CARD:
		{
		}
		break;

	case I_NOREX_ITEM_SPECIAL_HALO_IMG_CARD:
		{
			if (!role->GetHalo()->ActiveSpecialImage(m_param1))
			{
				return -51;
			}
		}
		break;

	case I_NOREX_ITEM_SPECIAL_SHENGONG_IMG_CARD:
		{
			if (!role->GetShengong()->ActiveSpecialImage(m_param1))
			{
				return -52;
			}
		}
		break;

	case I_NOREX_ITEM_SPECIAL_SHENYI_IMG_CARD:
		{
			if (!role->GetShenyi()->ActiveSpecialImage(m_param1))
			{
				return -53;
			}
		}
		break;

// 	case I_NOREX_ITEM_CHENGZHANGDAN:
// 		{
// 			bool ret = false;
// 
// 			switch (m_param1)
// 			{
// 			case CHENGZHANGDAN_TYPE_MOUNT:
// 				{
// 					ret = role->GetMountManager()->OnUseChengzhangdan(num) ;
// 				}
// 				break;
// 
// 			case CHENGZHANGDAN_TYPE_WING:
// 				{	
// 					ret = role->GetWing()->OnUseChengzhangdan(num);
// 				}
// 				break;
// 
// 			case CHENGZHANGDAN_TYPE_HALO:
// 				{
// 					ret = role->GetHalo()->OnUseChengzhangdan(num);
// 				}
// 				break;
// 
// 			case CHENGZHANGDAN_TYPE_SHENGONG:
// 				{
// 					ret = role->GetShengong()->OnUseChengzhangdan(num);
// 				}
// 				break;
// 
// 			case CHENGZHANGDAN_TYPE_SHENYI:
// 				{
// 					ret = role->GetShenyi()->OnUseChengzhangdan(num);
// 				}
// 				break;
// 			}
// 
// 			if (!ret)
// 			{
// 				return -54;
// 			}
// 		}
// 		break;

	case I_NOREX_ITEM_ZHUANSHENG_EQUIP:
		{
			return -55;
		}
		break;

	case I_NOREX_ITEM_ZHUANSHENG_XIUWEI_DAN:
		{
		// 转生已屏蔽
			/*if (!role->GetZhuanShengEquip()->OnUseXiuweiDan(m_param1, num))
			{
				return -56;
			}*/
		}
		break;

	case I_NOREX_ITEM_XIAO_CHONG_WU:
		{
			//	小宠物
			LittlePet *m_little_pet = GLOBAL_ROLE_MGR.GetLittlePet(role->GetUID());
			if (NULL == m_little_pet)
			{
				return -57;
			}
			if (!m_little_pet->ActiveLittlePet(item_data.item_id))
			{
				return -57;
			}
		}
		break;

	case I_NOREX_ITEM_CHINESE_ZODIAC:
		{
			// 星座
			//if (!role->GetChineseZodiac()->OnUseSoul(m_param1))
			//{
				//return -58;
			//}
		}
		break;

	case I_NOREX_ITEM_SUPPLY_XUEBAO:
		{
			// 补给血包
			if (!role->GetRoleSupply()->OnUseXuebao(m_param1, m_param2))
			{
				return -59;
			}
		}
		break;

	case I_NOREX_ITEM_SPECIAL_FIGHTMOUNT_IMG_CARD:
		{
			// 战斗坐骑特殊形象卡
			if (!role->GetFightMountManager()->ActiveSpecialImage(m_param1))
			{
				return -60;
			}
		}
		break;

	case I_NOREX_ITEM_JINGLINGFAZHEN_IMG_CARD:
		{
			// 精灵法阵特殊形象卡
			if (!role->GetJinglingFazhen()->ActiveSpecialImage(m_param1))
			{
				return -61;
			}
		}
		break;

	case I_NOREX_ITEM_JINGLINGGUANGHUAN_IMG_CARD:
		{
			// 精灵光环特殊形象卡
			if (!role->GetJinglingGuanghuan()->ActiveSpecialImage(m_param1))
			{
				return -62;
			}
		}
		break;

	case I_NOREX_ITEM_XIANNV_HUANHUA_ACTIVE_CARD:
		{
			// 仙女幻化形象激活卡
			if (!role->GetXiannvManager()->ActiveHuanHua(m_param1))
			{
				return -63;
			}
		}
		break;

	case I_NOREX_ITEM_ZHIBAO_HUANHUA_ACTIVE_CARD:
		{
			// 至宝幻化激活卡
			if (!role->GetZhiBaoManager()->ActiveHuanHua(m_param1))
			{
				return -64;
			}
		}
		break;

	case I_NOREX_ITEM_REDUCE_WEARY:
		{
			// 减少boss疲劳值   // param1 Boss类型， param2 减少值
			for (int i = 0; i < num; i++)
			{
				if (!role->ReduceBossWeary(m_param1, m_param2))
				{
					num = i;
					break;
				}
			}

			if (num <= 0)
			{
				return -65;
			}
		}
		break;

	case I_NOREX_ITEM_SPECIAL_JINGLING_IMG_CARD:
		{
			// 精灵特殊形象卡
			if (!role->GetJingLingMgr()->ActivePhantomImage(m_param1))
			{
				return -66;
			}
		}
		break;

	case I_NOREX_ITEM_MOUNT_UP_ONE_GRADE_CARD:
		{
			// 坐骑直升一阶进阶卡
			if (!MountFunction::UpOneGrade(role, m_param1, m_param2, m_param3, m_param4))
			{
				return -67;
			}
		}
		break;

	case I_NOREX_ITEM_FIGHT_MOUNT_UP_ONE_GRADE_CARD:
		{
			// 战斗坐骑直升一阶进阶卡
			if (!FightMountFunction::UpOneGrade(role, m_param1, m_param2, m_param3, m_param4))
			{
				return -68;
			}

		}
		break;

	case I_NOREX_ITEM_WING_UP_ONE_GRADE_CARD:
		{
			// 羽翼直升一阶进阶卡
			if (!WingFunction::UpOneGrade(role, m_param1, m_param2, m_param3, m_param4))
			{
				return -69;
			}
		}
		break;

	case I_NOREX_ITEM_HALO_UP_ONE_GRADE_CARD:
		{
			// 光环直升一阶进阶卡
			if (!HaloFunction::UpOneGrade(role, m_param1, m_param2, m_param3, m_param4))
			{
				return -70;
			}
		}
		break;

	case I_NOREX_ITEM_SHENGONG_UP_ONE_GRADE_CARD:
		{
			// 神弓直升一阶进阶卡
			if (!ShengongFunction::UpOneGrade(role, m_param1, m_param2, m_param3, m_param4))
			{
				return -71;
			}
		}
		break;

	case I_NOREX_ITEM_SHENYI_UP_ONE_GRADE_CARD:
		{
			// 神翼直升一阶进阶卡
			if (!ShenyiFunction::UpOneGrade(role, m_param1, m_param2,m_param3,m_param4))
			{
				return -82;
			}
		}
		break;

	case I_NOREX_ITEM_RUNE_BOX:
		{
			// 符文宝箱
			if (!role->GetRuneSystem()->UseRuneBox(m_param1, m_param2))
			{
				return -73;
			}
		}
		break;

	case I_NOREX_ITEM_RUNE:
		{
			// 符文
			for (int i = 0; i < num; i++)
			{
				if (!role->GetRuneSystem()->AddRuneByItemId(item_data.item_id, 1))
				{
					num = i;
					break;
				}
			}

			if (0 >= num)
			{
				return -74;
			}
		}
		break;

	case I_NOREX_ITEM_RUNE_TOWER_OFFLINE_TIME:
		{
			// 符文塔离线卡
			num = role->GetRuneSystem()->AddRuneTowerOfflineTime(m_param1, num);
			if (num <= 0)
			{
				role->NoticeNum(errornum::EN_RUNE_TOWER_OFFLINE_TIME_MAX);
				return -75;
			}
		}
		break;

	case I_NOREX_ITEM_PUT_SHENGE:
		{
			if (!role->GetShengeSystem()->AddShengeByItemId(item_data.item_id))
			{
				return -76;
			}
			if (GetColor() <= ItemBase::I_COLOR_PURPLE)
			{
				succ_msg = false;
			}
		}
		break;

	case I_NOREX_ITEM_XINHUN:
		{
			if (!role->GetChineseZodiac()->TrantsItem(m_param1))
			{
				return -77;
			}
		}
		break;

	case I_NOREX_ITEM_SHENGE_SUIPIAN:
		{
			if (!role->GetShengeSystem()->AddMarrowScore(m_param1 * num, "norex"))
			{
				return -78;
			}
		}
		break;

	case I_NOREX_ITEM_LUCKY_RED_PAPER:
		{
			if (!role->GetRoleGuild()->AddGuildRedPaperAuthority(PAPER_TYPE_LUCKY_PAPER, m_item_id))
			{
				return -79;
			}
		}
		break;

	case I_NOREX_ITEM_GOLD_CAR:
		{
			if (!role->UseGoldCard(m_param1))
			{
				return -80;
			}
		}
		break;

	case I_NOREX_ITEM_FB_TOWER_DEFEND_CARD:
		{
			if (!role->GetFBTowerDefend()->PersonalItemAddJoinTimes(m_param1))
			{
				return -81;
			}
		}
		break;

	case I_NOREX_ITEM_XIANNV_SHENGWU_ADD_LINGYE:
		{
			if (!role->GetXiannvManager()->ShengwuAddLingye(m_param1 * num, "NorexItem::Use"))
			{
				return -82;
			}
		}
		break;

	case I_NOREX_ITEM_SPECIAL_FOOTPRINT_IMG_CARD:
	{
		if (!role->GetFootPrint()->ActiveSpecialImage(m_param1))
		{
			return -83;
		}
	}
	break;

	case I_NOREX_ITEM_FOOTPRINT_UP_ONE_GRADE_CARD:
	{
		// 足迹直升一阶进阶卡
		if (!role->GetFootPrint()->UpOneGrade(m_param1, m_param2, m_param3, m_param4))
		{
			return -84;
		}
	}
	break;

	case I_NOREX_ITEM_SPECIAL_CLOAK_IMG_CARD:
	{
		if (!role->GetCloakManager()->ActiveSpecialImage(m_param1))
		{
			return -85;
		}
	}
	break;

	case I_NOREX_ITEM_MARRY_EXP:
	{
		QingyuanGlobal *global_qingyuan = GLOBAL_ROLE_MGR.GetQingyuanGlobal(role->GetUID());
		if (NULL == global_qingyuan) return -86;
		
		for (int i = 0; i < num; i++)
		{
			if (!global_qingyuan->AddMarryLevelExp(role, m_param1))
			{
				num = i;
				break;
			}
		}

		if (num <= 0)
		{
			return - 86;
		}
	}
	break;
	
	case I_NOREX_ITEM_SHENSHOU:
	{
		//if (!role->GetShenshou()->TransformFromKnapsackItem(m_item_id, num, static_cast<short>(m_param1), static_cast<short>(m_param2), m_param3 > 0, m_param4, "NorexItem::Use"))
		{
			return -87;
		}
	}
	break;

	case I_NOREX_ITEM_SHENYIN:
	{
		// 铭文特殊处理   直接返回失败，在背包处理
		//if (!role->GetShenYin()->PutOnItem(m_item_id, num, m_is_bind))
		{
			return -88;
		}
	}
	break;

	case I_NOREX_ITEM_TIAN_XIANG:
	{
		if (!role->GetShenYin()->TrantsItem())
		{
			return -89;
		}
	}
	break;

	case I_NOREX_ITEM_FABAO:
	{
		if (!role->GetFabaoManager()->ActiveSpecialImg(m_param1))
		{
			return -90;
		}
	}
	break;
	
	case I_NOREX_ITEM_HUNYAN_YANHUA:
	{
		const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();
		if (role->GetScene()->GetSceneID() != hunyan_cfg.hunyan_scene_id)
		{
			role->NoticeNum(errornum::EN_KNAPSACK_USE_SCENE_LIMIT);
			return -91;
		}

		SpecialHunyan *hunyan_scene = World::GetInstWorld()->GetSceneManager()->GetSpecialHunyan(COMMON_SCENE_KEY, false);
		if (NULL != hunyan_scene)
		{
			if (!hunyan_scene->UseYanHua(role, m_item_id, m_param1, m_param2))
			{
				return -91;
			}
		}
	}
	break;

	case I_NOREX_ITEM_SHIZHUANG_UP_ONE_GRADE_CARD:
		{
			if (!role->GetShizhuang()->UpOneGradeShiZhuang(m_param1, m_param2, m_param3, m_param4))
			{
				return 91;
			}
		}
	break;

	case I_NOREX_ITEM_SHENBING_UP_ONE_GRADE_CARD:
		{
			if (!role->GetShizhuang()->UpOneGradeShenBing(m_param1, m_param2, m_param3, m_param4))
			{
				return 92;
			}
		}
	break;

	case I_NOREX_ITEM_FABAO_UP_ONE_GRADE_CARD:
		{
			if (!role->GetFabaoManager()->UpOneGrade(m_param1, m_param2, m_param3, m_param4))
			{
				return 93;
			}
		}
	break;
	
	case I_NOREX_ITEM_ITEM_SEAL:
	{
		if (!role->GetSeal()->AddSealByItemId(item_data.item_id, num, item_data.is_bind, m_param1))
		{
			return -96;
		}
	}
	break;

	case I_NOREX_ITEM_PVP_REWARD_ITEM:
	{
		if (!role->GetRoleCross()->OnActivePVPRewardItem(m_param2, m_param1, m_param3))
		{
			return -97;
		}
	}
	break;

	case I_NOREX_ITEM_YAOSHI_UP_ONE_GRADE_CARD:
	{
		// 腰饰直升一阶进阶卡
		if (!YaoShiFunction::UpOneGrade(role, m_param1, m_param2, m_param3, m_param4))
		{
			return -97;
		}
	}
	break;

	case I_NOREX_ITEM_TOUSHI_UP_ONE_GRADE_CARD:
	{
		// 头饰直升一阶进阶卡
		if (!TouShiFunction::UpOneGrade(role, m_param1, m_param2, m_param3, m_param4))
		{
			return -98;
		}
	}
	break;

	case I_NOREX_ITEM_QILINBI_UP_ONE_GRADE_CARD:
	{
		// 麒麟臂直升一阶进阶卡
		if (!QiLinBiFunction::UpOneGrade(role, m_param1, m_param2, m_param3, m_param4))
		{
			return -99;
		}
	}
	break;

	case I_NOREX_ITEM_MASK_UP_ONE_GRADE_CARD:
	{
		if (!MaskFunction::UpOneGrade(role, m_param1, m_param2, m_param3, m_param4))
		{
			return -100;
		}
	}
	break;

	case I_NOREX_ITEM_SPECIAL_YAOSHI_IMG_CARD:
	{
		// 腰饰特殊形象卡
		if (role->GetYaoShiManager()->GetYaoShiParam()->IsSpecialImageActive(m_param1))
		{
			if (YaoShiFunction::YaoShiSpecialImgUpgrade(role, m_param1))
			{
				num = 0;		// 升阶成功了不应该再消耗了，如果物品可以堆叠就出bug了
			}
			else
			{
				return -101;
			}
		}
		else if (!role->GetYaoShiManager()->ActiveSpecialImage(m_param1))
		{
			return -101;
		}
	}
	break;

	case I_NOREX_ITEM_SPECIAL_TOUSHI_IMG_CARD:
	{
		// 头饰特殊形象卡
		if (role->GetTouShiManager()->GetTouShiParam()->IsSpecialImageActive(m_param1))
		{
			if (TouShiFunction::TouShiSpecialImgUpgrade(role, m_param1))
			{
				num = 0;		// 升阶成功了不应该再消耗了，如果物品可以堆叠就出bug了
			}
			else
			{
				return -102;
			}
		}
		else if (!role->GetTouShiManager()->ActiveSpecialImage(m_param1))
		{
			return -102;
		}
	}
	break;

	case I_NOREX_ITEM_SPECIAL_MASK_IMG_CARD:
	{
		// 面具特殊形象卡
		if (role->GetMaskManager()->GetMaskParam()->IsSpecialImageActive(m_param1))
		{
			if (MaskFunction::MaskSpecialImgUpgrade(role, m_param1))
			{
				num = 0;		// 升阶成功了不应该再消耗了，如果物品可以堆叠就出bug了
			}
			else
			{
				return -103;
			}
		}
		else if (!role->GetMaskManager()->ActiveSpecialImage(m_param1))
		{
			return -103;
		}
	}
	break;

	case I_NOREX_ITEM_SPECIAL_QILINBI_IMG_CARD:
	{
		// 麒麟臂特殊形象卡
		if (role->GetQiLinBiManager()->GetQiLinBiParam()->IsSpecialImageActive(m_param1))
		{
			if (QiLinBiFunction::QiLinBiSpecialImgUpgrade(role, m_param1))
			{
				num = 0;		// 升阶成功了不应该再消耗了，如果物品可以堆叠就出bug了
			}
			else
			{
				return -104;
			}
		}
		else if (!role->GetQiLinBiManager()->ActiveSpecialImage(m_param1))
		{
			return -104;
		}
	}
	break;

	case I_NOREX_ITEM_PLANTING_TREE_SEED:
	{
		if (!role->GetRoleActivity()->OnRAPlantingTreePlanting(m_item_id))
		{
			return -105;
		}
	}
	break;

	case I_NOREX_ITEM_UPGRADE_SYS_IMG_CARD:
	{
		if (role->GetUpgradeManager()->IsImageActive(m_param1, m_param2))
		{
			if (role->GetUpgradeManager()->OnImgUpgrade(m_param1, m_param2))
			{
				num = 0;
			}
			else
			{
				return -106;
			}
		}
		else if (!role->GetUpgradeManager()->ActiveImage(m_param1, m_param2))
		{
			return -106;
		}
	}
	break;

	case I_NOREX_ITEM_UPGRADE_SYS_UP_ONE_GRADE_CARD:
	{
		if (!role->GetUpgradeManager()->UpOneGrade(m_param1, m_param2, m_param3, m_param4, m_param5))
		{
			return -107;
		}
	}
	break;

	case I_NOREX_ITEM_BIG_SMALL_GOAL_SPECIAL_ATTR:
	{
		if (!role->GetRoleBigSmallGoal()->ActiveRoleBigSmallGoalSpecialAttr(m_param1, 0))
		{
			return -108;
		}
	}
	break;

	case I_NOREX_ITEM_CROSS_HONOR:
	{
		//跨服荣耀
		if (!role->AddCrossHonor(m_param1 * num, "ItemUse:CrossHonor"))
		{
			return -109;
		}
	}
	break;

	case I_NOREX_ITEM_TALENT_SKILL_BOOK:
	{
		//天赋技能书
		if (!role->GetTalent()->OnUseTalenBook(m_param1 * num))
		{
			return -110;
		}
	}
	break;

 	case I_NOREX_ITEM_GREATE_SOLDIER_CARD:
 	{
		if (!GREATE_SOLDIER(role)->OnLevelUp(m_param1))
		{
			return -111;
		}
 	}
 	break;
 
 	case I_NOREX_ITEM_GREATE_SOLDIER_HUANHUA_CARD:
 	{
		int ret = GREATE_SOLDIER(role)->OnUseHuanhuaCard(m_param1);
 		if (ret != 0)
 		{
 			return -112;
 		}
 	}
 	break;
 
// 	case I_NOREX_ITEM_GREATE_SOLDIER_EXPE_CARD:
// 	{
// 		// 名将体验
// 		if (!GREATE_SOLDIER(role)->OnTiyanActive(m_param1, m_param2))
// 		{
// 			return -113;
// 		}
// 	}
// 	break;

	case I_NOREX_ITEM_SHENGWU_JINGHUA:
	{
		//仙器精华
		if (!role->GetXiannvManager()->AddShengwuEssence(m_param1 * num, "NorexItem::Use"))
		{
			return -114;
		}
	}
	break;

	case I_NOREX_ITEM_GUILD_GONGZI:
	{
		//仙盟工资
		if (!GuildManager::Instance().OnAddGongzi(role->GetGuildID(), role->GetUID(), m_param1, m_param2 * num, "NorexItem::Use"))
		{
			return -115;
		}
	}
	break;

	case I_NOREX_ITEM_BOSS_REFRESH_CARD:
	{
		// 刷新BOSS
		succ_msg = false;

		int boss_id = equip_index;
		if (role->OnUserRefreshBossCard(boss_id))
		{
			gstr::SendNotice(role, "use_boss_refresh_card_succ");
		}
		else
		{
			gstr::SendNotice(role, "use_boss_refresh_card_fail");
			return -116;
		}
	}
	break;

	case I_NOREX_ITEM_DOUQIDAN:
	{
		if (!role->GetCrossEquip()->UseDouqidan(m_param1, num))
		{
			return -117;
		}
	}
	break;

	case I_NOREX_ITEM_TIANSHENHUTI_EQUIP:
	{
		TianshenHuti* tianshenhuti = role->GetOtherModule<TianshenHuti>(ROLE_MODULE_TYPE_TIANSHENHUTI);
		if (nullptr == tianshenhuti) return -118;

		if (!tianshenhuti->OnAddEquipByItem(m_param1, "UseItem"))
		{
			return -118;
		}
	}
	break;

	case I_NOREX_ITEM_ZODIAC:
	{
		if (!role->GetZodiac()->AddItemsToBackpack(item_data.item_id, num))
		{
			return -101;
		}
	}
	break;
	
	default:
		return -200;
	}
	
	if (m_colddown_id > 0 && m_server_colddown > 0)
	{
		role->GetKnapsack()->GetItemColddownManager()->AddColddown(m_colddown_id, m_server_colddown);
	}

	if (m_use_daytimes > 0)
	{
		role->AddUseNorexItemDayTimes(m_item_id, num);
	}

	if (succ_msg)
	{
		role->NoticeNum(noticenum::NT_ITEM_USE_SUCC);
	}
	return 0;
}


