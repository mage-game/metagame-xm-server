#include <iostream>

#include "messagehandler.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "scene/map.h"
#include "obj/otherobj/fallingitem.h"
#include "obj/character/role.h"
#include "obj/otherobj/worldeventobj.h"

#include "servercommon/servercommon.h"
#include "servercommon/userprotocal/msgcode.h"
#include "protocal/msgitem.h"
#include "protocal/msgequipment.h"
#include "protocal/msgnpc.h"
#include "protocal/msghotkey.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/errornum.h"
#include "protocal/msgmoney.h"
#include "protocal/msgfunction.h"

#include "servercommon/internalprotocal/ggprotocal.h"
#include "globalconfig/globalconfig.h"
#include "internalcomm.h"

#include "gamelog.h"
#include "item/knapsack.h"
#include "item/itemfunction.h"
#include "protocal/msgworldevent.h"

#include "other/gm/gm.hpp"

void MessageHandler::OnGetKnapsackInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetKnapsack()->SendKnapsackInfoToRole();
}

void MessageHandler::OnSendKnapsackGridExtendAuto(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetKnapsack()->SendKnapsackGridExtendAuto();
}

void MessageHandler::OnUseItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUseItem* pstUseItem = (Protocol::CSUseItem*)msg;
	role->GetKnapsack()->Use(pstUseItem->index, pstUseItem->num, pstUseItem->equip_index);
}

void MessageHandler::OnUseItemMax(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUseItemMaxNum* pstUseItem = (Protocol::CSUseItemMaxNum*)msg;
	role->GetKnapsack()->UseItem(pstUseItem->item_id, pstUseItem->num);
}

void MessageHandler::OnMoveItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMoveItem* pstUseItem = (Protocol::CSMoveItem*)msg;
	role->GetKnapsack()->Move(pstUseItem->from_index, pstUseItem->to_index);
}

void MessageHandler::OnDiscardItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSDiscardItem* pstDiscardItem = (Protocol::CSDiscardItem*)msg;
	role->GetKnapsack()->Discard(pstDiscardItem->index, pstDiscardItem->discard_num, pstDiscardItem->discard_medthod, pstDiscardItem->item_id_in_client, pstDiscardItem->item_num_in_client);
}

void MessageHandler::OnBatchDiscardItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSBatchDiscardItem* pstDiscardItem = (Protocol::CSBatchDiscardItem*)msg;

	// 修正长度，防止客户端恶意发送数据
	int arr_len = length - (sizeof(Protocol::CSBatchDiscardItem) - sizeof(pstDiscardItem->index_list));
	pstDiscardItem->index_count = arr_len / sizeof(pstDiscardItem->index_list[0]);

	if (pstDiscardItem->index_count > 0)
	{
		role->GetKnapsack()->BatchDiscard(pstDiscardItem);
	}
}

void MessageHandler::OnKnapsackStoragePutInOrder(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSKnapsackStoragePutInOrder* pstOrder = (Protocol::CSKnapsackStoragePutInOrder*)msg;
	role->GetKnapsack()->InOrder(0 != pstOrder->is_storage, 0 != pstOrder->ignore_bind);
}

// 掉落拾取
void MessageHandler::OnPickItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSPickItem *pi = (Protocol::CSPickItem*)msg;
	
	if (pi->count <= 0 || length != static_cast<int>(UNSTD_OFFSETOFF(Protocol::CSPickItem, obj_id_list) + sizeof(pi->obj_id_list[0]) * pi->count)) return;

	// 拾取返回
// 	static Protocol::SCPickItemAck pi_ack;
// 	pi_ack.count = 0;

	for (int i = 0; i < pi->count && i < Protocol::CSPickItem::MAX_PICK_ITEM; ++i)
	{
		if (INVALID_OBJ_ID == pi->obj_id_list[i]) continue;

		Obj *obj = scene->GetObj(pi->obj_id_list[i]);
		if (NULL == obj)
		{
			gamelog::g_log_messagehandler.printf(LL_DEBUG, "Scene: [%d]%s OnPickItem item_obj_id:%d is not exist in scene.", scene->GetSceneID(), scene->GetSceneName(), pi->obj_id_list[i]);
			continue;
		}

		if (Obj::OBJ_TYPE_FALLINGITEM != obj->GetObjType())
		{
			gamelog::g_log_messagehandler.printf(LL_DEBUG, "Scene: [%d]%s OnPickItem item_obj_id:%d is not a fallingitem.", scene->GetSceneID(), scene->GetSceneName(), pi->obj_id_list[i]);
			continue;
		}

		FallingItem *item_obj = (FallingItem*)obj;
		item_obj->Pick(role);
		
		// 拾取返回
// 		pi_ack.obj_id_list[pi_ack.count] = obj->GetId();
// 		++pi_ack.count;
	}

	// 拾取返回
// 	UInt32 sendlen = sizeof(Protocol::SCPickItemAck) - sizeof(ObjID) * (Protocol::SCPickItemAck::MAX_PICK_TIME_ACK - pi_ack.count);
// 	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&pi_ack, sendlen);
}

void MessageHandler::OnSplitItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSplitItem* pstSplitItem = (Protocol::CSSplitItem*)msg;
	role->GetKnapsack()->Split(pstSplitItem->index, pstSplitItem->num);
}

void MessageHandler::OnItemCompose(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSItemCompose *pst = (Protocol::CSItemCompose*)msg;

	ItemFunction::ItemCompose(role, pst->product_seq, pst->num, pst->compose_type);
}

void MessageHandler::OnBluePrintCompose(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSBluePrintCompose *pst = (Protocol::CSBluePrintCompose*)msg;

	if (pst->index_count <= 0 || pst->index_count >= ItemNamespace::MAX_KNAPSACK_GRID_NUM ||
		length != static_cast<int>(UNSTD_OFFSETOFF(Protocol::CSBluePrintCompose, stuff_index_list) + pst->index_count * sizeof(pst->stuff_index_list[0]))) return;

	static Protocol::CSBluePrintCompose msg_compose;
	memcpy((void *)&msg_compose, msg, length);

	//ItemFunction::BluePrintCompose(role, pst->blueprint_id, pst->num, pst->index_count, msg_compose.stuff_index_list);
}

void MessageHandler::OnTakeOutAllItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetKnapsack()->TakeOutAllItemFromStorage();
}

void MessageHandler::OnKnapsackStorageExtendNum(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSKnapsackStorageExtendGridNum *pstExtendGrid = (Protocol::CSKnapsackStorageExtendGridNum*)msg;
	//被包改为vip扩展
//	if (!pstExtendGrid->type) return;
	role->GetKnapsack()->ExtendValidNum(0 != pstExtendGrid->can_use_gold, 0 != pstExtendGrid->type, pstExtendGrid->extend_num);
}

