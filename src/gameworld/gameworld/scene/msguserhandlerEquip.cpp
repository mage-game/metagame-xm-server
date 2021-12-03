#include <iostream>

#include "messagehandler.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "scene/map.h"
#include "obj/otherobj/fallingitem.h"
#include "obj/character/role.h"

#include "servercommon/servercommon.h"
#include "servercommon/userprotocal/msgcode.h"
#include "protocal/msgequipment.h"
#include "protocal/msgnpc.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/errornum.h"
#include "protocal/msgmoney.h"
#include "protocal/msgfunction.h"
#include "protocal/msgequipment.h"

#include "servercommon/internalprotocal/ggprotocal.h"
#include "globalconfig/globalconfig.h"
#include "internalcomm.h"

#include "gamelog.h"
#include "item/knapsack.h"
#include "equipment/equipmentmanager.h"
#include "equipment/equipmentfunction.h"
#include "equipment/stonemananger.hpp"

#include "global/offlineregister/spacemanager.hpp"

#include "other/gm/gm.hpp"

#include "scene/activityshadow/activityshadow.hpp"
#include "other/equipsuit/equipsuit.hpp"
#include "other/lifeskill/lifeskill.hpp"
#include "protocal/msgfeixian.hpp"

void MessageHandler::OnGetEequipmentInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnTakeOffEquipment(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTakeOffEquipment *msgtakeoff = (Protocol::CSTakeOffEquipment *)msg;
	role->GetEquipmentManager()->TakeOff(msgtakeoff->index);
}

void MessageHandler::OnEquipStrengthen(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSEquipStrengthen *pst = (Protocol::CSEquipStrengthen *)msg;
	EquipFunction::EquipStrengthen(role, 0 != pst->is_puton, 0 != pst->select_bind_first, 0 != pst->is_auto_buy, pst->equip_index, 0 != pst->use_lucky_item, pst->flag);
}

void MessageHandler::OnEquipCompound(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSEquipCompound *pst = (Protocol::CSEquipCompound *)msg;
	EquipFunction::EquipCompound(role, pst->equip_index);
}

void MessageHandler::OnEquipUpEternity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSEquipUpEternity *pro = (Protocol::CSEquipUpEternity *)msg;
	EquipFunction::EquipUpEternity(role, pro->equip_index, 0 != pro->is_auto_buy);
}

void MessageHandler::OnEquipUseEternityLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSEquipUseEternityLevel *pro = (Protocol::CSEquipUseEternityLevel *)msg;
	EquipFunction::UseEternityLevel(role, pro->eternity_level);
}

void MessageHandler::OnEquipUpStar(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSEquipUpStar *pst = (Protocol::CSEquipUpStar *)msg;
	EquipFunction::EquipUpStar(role, pst->equip_index);
}

void MessageHandler::OnEquipUpLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSEquipUpLevel *pst = (Protocol::CSEquipUpLevel *)msg;

	EquipFunction::EquipUpLevel(role, 0 != pst->is_puton, 0 != pst->select_bind_first,pst->equip_index);
	//EquipFunction::EquipUpLevel(role, pst->equip_index1, pst->equip_index2, pst->equip_index3);
}

void MessageHandler::OnEquipInherit(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSEquipInherit *pst = (Protocol::CSEquipInherit *)msg;
	EquipFunction::EquipInherit(role, pst->equip_index1, pst->equip_index2, 0 != pst->is_puton1, 0 != pst->is_puton2, pst->inherit_type, pst->cost_type);
}

 void MessageHandler::OnEquipUpQuality(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
 {
 	Protocol::CSEquipUpQuality *pst = (Protocol::CSEquipUpQuality *)msg;
 	EquipFunction::EquipUpQuality(role, 0 != pst->is_puton, 0 != pst->select_bind_first, pst->equip_index);
 }

void MessageHandler::OnEquipRefine(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSEquipShenZhu *pst = (Protocol::CSEquipShenZhu *)msg;
	EquipFunction::EquipShenOP(role, 0 != pst->is_puton, 0 != pst->select_bind_first, pst->equip_index);
}

void MessageHandler::OnJianxinOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnRoleChangeCamp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRoleChangeCamp *rcc = (Protocol::CSRoleChangeCamp *)msg;
	role->ChangeCamp(static_cast<int>(rcc->camp));
}

void MessageHandler::OnRoleChangeProf(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSChangeProfReq *rcp = (Protocol::CSChangeProfReq *)msg;
	role->ChangeProf(rcp->prof, rcp->sex);
}

void MessageHandler::OnRoleZhuanShen(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->ZhuanSheng();
}

void MessageHandler::OnRoleTransformExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTransformExpToBottle *tetb = (Protocol::CSTransformExpToBottle *)msg;
	
	if (0 != tetb->is_query)
	{
		role->SendExpBottleInfo();
	}
	else 
	{
		role->TransformExp(tetb->seq, tetb->item_num);
	}
}

void MessageHandler::OnGetRoleSpaceMsgInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGetRoleSpaceMsgInfo *grsmi = (Protocol::CSGetRoleSpaceMsgInfo *)msg;
	SpaceManager::Instance().OnGetRoleSpaceMsgInfo(role, grsmi);
}

void MessageHandler::OnGetMillionaireInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	if (ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_MILLIONAIRE))
	{
		role->SendMillionaireInfo();
	}
}

void MessageHandler::OnOperSpace(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSpaceOper *so = (Protocol::CSSpaceOper *)msg;
	SpaceManager::Instance().OnOperSpace(role, so);
}

void MessageHandler::OnRemoveRecord(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSpaceRemoveRecord *srr = (Protocol::CSSpaceRemoveRecord *)msg;
	SpaceManager::Instance().OnRemoveRecord(role, srr);
}

void MessageHandler::OnLifeSkillOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSLifeSkillOpera *cmd = (Protocol::CSLifeSkillOpera *)msg;
	if (Protocol::LIFE_SKILL_OPERAT_TYPE_REQ_INFO == cmd->operat_type)
	{
		role->GetLifeSkillMgr()->SyncLifeSkillInfo();
	}
	else if (Protocol::LIFE_SKILL_OPERAT_TYPE_UPLEVEL == cmd->operat_type)
	{
		role->GetLifeSkillMgr()->UpLevelSkill(cmd->param1);
	}
	else if (Protocol::LIFE_SKILL_OPERAT_TYPE_MAKE == cmd->operat_type)
	{
		role->GetLifeSkillMgr()->Make(cmd->param1);
	}
}

void MessageHandler::OnStoneUpLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSStoneUplevel *pst = (Protocol::CSStoneUplevel *)msg;
	role->GetStoneManager()->Uplevelstone(pst->equip_part, pst->stone_slot, pst->uplevel_type);	
}

void MessageHandler::OnStoneInlay(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSStoneInlay *pst = (Protocol::CSStoneInlay *)msg;
	if (1 == pst->is_inlay)
	{
		role->GetStoneManager()->InlayStone(pst->equip_part, pst->stone_slot, pst->index_in_bag);
	}
	else
	{
		role->GetStoneManager()->UnInlayStone(pst->equip_part, pst->stone_slot);
	}
}

void MessageHandler::OnReqStoneInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	if (NULL == role) return;
	role->GetStoneManager()->SendToClient(Protocol::SCStoneInfo::STONE_REASON_DEFAULT);
}

void MessageHandler::OnDuanzaoSuitReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSDuanzaoSuitReq *esr = (Protocol::CSDuanzaoSuitReq *)msg;

	role->GetEquipSuit()->OnSuitEquipOperate(esr->operate_type, esr->equip_index);
}

void MessageHandler::OnWash(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnShenbingInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnShenbingComposeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnShenbingOPReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnFulingReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSEquipFuling *cs_fulin = (Protocol::CSEquipFuling*)msg;
	
	EquipFunction::EquipFuling(role, cs_fulin->is_puton == 1, true, cs_fulin->equip_index);
}

void MessageHandler::OnEquipCrossEquipOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSEquipCrossEquipOpera *eceo = (Protocol::CSEquipCrossEquipOpera *)msg;

	if (0 != eceo->is_puton)
	{
		role->GetEquipmentManager()->PutOnCrossEquip(eceo->grid_idx);
	}
	else
	{
		role->GetEquipmentManager()->TakeOffCrossEquip();
	}
}
// 飞仙装备
void MessageHandler::OnFeixian(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFeixianEquipOpe *msgParam = (Protocol::CSFeixianEquipOpe *)msg;
	switch (msgParam->operate)
	{
		// 发送飞仙装备信息
	case Protocol::FEIXIAN_EQUIP_OPERATE_TYPE_REQ_INFO:
		{
			role->GetFeixianManager()->SyncEquipFeixianInfo();
		}
		break;

		// 脱下飞仙装备
	case Protocol::FEIXIAN_EQUIP_OPERATE_TYPE_TAKE_OFF:
		{
			role->GetFeixianManager()->TakeOff(msgParam->param1, false);
		}
		break;

		// 合成飞仙装备
	case Protocol::FEIXIAN_EQUIP_OPERATE_TYPE_COMPOSE:
		{
			if (msgParam->param3 > 0)
			{
				int ret = role->GetFeixianManager()->ComposeEquipInBody(msgParam->param1, msgParam->param2);
				short result = ret < 0 ? 0 : 1;
				role->SendOperateResult(Protocol::SCOperateResult::OP_FEIXIAN_COMPOSE, result, msgParam->param1);
			}
			else
			{
				int ret = role->GetFeixianManager()->ComposeEquipInKnapsack(msgParam->param1, msgParam->param2);
				short result = ret < 0 ? 0 : 1;
				role->SendOperateResult(Protocol::SCOperateResult::OP_FEIXIAN_COMPOSE, result, msgParam->param2);
			}
		}
		break;

		// 升级飞仙准备成红装
	case Protocol::FEIXIAN_EQUIP_OPERATE_TYPE_LEVELUP:
		{
			int index = 0;
			if (msgParam->param3 > 0)
			{
				int ret = role->GetFeixianManager()->LevelUpInBody(msgParam->param1);
				short result = ret < 0 ? 0 : 1;
				role->SendOperateResult(Protocol::SCOperateResult::OP_FEIXIAN_UPLEVEL, result, msgParam->param1);
			}
			else
			{
				int ret = role->GetFeixianManager()->LevelUpInKnapsack(msgParam->param1, index);
				short result = ret < 0 ? 0 : 1;
				role->SendOperateResult(Protocol::SCOperateResult::OP_FEIXIAN_UPLEVEL, result, index);
			}
		}
		break;
		
	default:
		break;
	}
}

