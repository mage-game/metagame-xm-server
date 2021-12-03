#include "scenemanager.h"
#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/servercommon.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/activitydef.hpp"
#include "internalcomm.h"
#include "scene/scene.h"
#include "globalconfig/globalconfig.h"
#include "item/itempool.h"
#include "world.h"


#include "protocal/msgfriend.h"
#include "protocal/msgrole.h"
#include "protocal/msgequipment.h"
#include "protocal/msgmount.hpp"
#include "protocal/msgother.h"
#include "protocal/msgtitle.hpp"

#include "gamelog.h"

#include "messagehandler.h"

#include "other/route/mailroute.hpp"
#include "other/route/guildroute.hpp"

#include "other/shop/shop.hpp"

#include "servercommon/internalprotocal/gglobalprotocal.h"

#include "other/cmdhandler/cmdhandler.hpp"
#include "other/event/eventhandler.hpp"
#include "other/route/publicsaleroute.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/marryconfig.hpp"
#include "friend/friendmanager.h"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/roleactivity/roleactivity.hpp"

void MessageHandler::OnForbidTalk(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::RoleForbidTalk *rdt = (ggprotocal::RoleForbidTalk*)msg;
	role->ForbidTalk((time_t)rdt->last_time, "Command", "Command");
}

void MessageHandler::OnGetYuanLiFBChapterLordGift(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
}

void MessageHandler::OnMarryReserveReq(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::MarryReserveReq *mrr = (ggprotocal::MarryReserveReq *)msg;

	role->GetRoleActivity()->OnMarryReserveReq(mrr->reserve_index, mrr->marry_level, mrr->rune_index);
}

void MessageHandler::OnMarryHunyanEnter(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	if (!role->GetRoleStatusManager()->CanEnterFB())
	{
		return;
	}

	int scene_id = 0; Posi pos(0, 0);
	if (LOGIC_CONFIG->GetMarryConfig().GetMarryHunyanEnterPos(&scene_id, &pos))
	{
		m_scene_manager->GoTo(role, scene_id, MARRY_HUNYAN_SCENE_KEY, pos);
	}
}

void MessageHandler::OnDatingInviteCheckTarget(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::DatingInviteCheckTarget *dict = (ggprotocal::DatingInviteCheckTarget *)msg;
	if (dict->req_uid <= 0) return;

	role->GetRoleActivity()->OnDatingInviteCheckTarget(IntToUid(dict->req_uid), 0 != dict->is_specific_invite);
}

void MessageHandler::OnDatingConsume(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::DatingConsume *dc = (ggprotocal::DatingConsume *)msg;

	role->GetRoleActivity()->OnDatingConsume(dc);
}

void MessageHandler::OnDatingReward(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::DatingReward *dr = (ggprotocal::DatingReward *)msg;
	if (dr->question_score <= 0 || dr->other_uid <= 0) return;

	role->GetRoleActivity()->OnDatingReward(dr);
}

void MessageHandler::OnDatingUpdateAddExp(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::DatingUpdateAddExp *duae = (ggprotocal::DatingUpdateAddExp *)msg;

	role->GetRoleActivity()->OnDatingUpdateAddExp(duae);
}

void MessageHandler::OnDatingDaycountCheck(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::DatingDaycountCheck *ddc = (ggprotocal::DatingDaycountCheck *)msg;

	role->GetRoleActivity()->OnDatingDaycountCheck(ddc);
}

void MessageHandler::OnLoverResetName(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::LoverResetName *lrn = (ggprotocal::LoverResetName*)msg;

	role->SetMarryInfo(lrn->lover_role_id, lrn->lover_new_name);
}

void MessageHandler::OnXiannvBuyReqToGS(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	
}

void MessageHandler::OnActivityFindReqToGS(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{

}

void MessageHandler::OnFriendResetName(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::FriendResetName *frn = (ggprotocal::FriendResetName*)msg;

	role->GetFriendManager()->OnResetFriendName(frn->friend_role_id, frn->friend_new_name);
}

void MessageHandler::OnFriendSetSex(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::FriendSetSex *fss = (ggprotocal::FriendSetSex*)msg;

	role->GetFriendManager()->OnSetFriendSex(fss->friend_role_id, fss->friend_plat_type, static_cast<char>(fss->sex));
}

void MessageHandler::OnFriendSetLevel(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::FriendSetLevel *pro = (ggprotocal::FriendSetLevel*)msg;

	role->GetFriendManager()->OnSetFriendLevel(pro->friend_role_id);
}

void MessageHandler::OnGlobalRemoveUser(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	//SceneIndex get_scene_index = -1; ObjID get_obj_id = INVALID_OBJ_ID;
	//if (m_scene_manager->OnDisconnect(role->GetNetId(), &get_scene_index, &get_obj_id))
	//{
	//	if (NULL == role->GetScene() || role->GetScene()->GetIndex() != get_scene_index || role->GetId() != get_obj_id)
	//	{
	//		gamelog::g_log_login.printf(LL_ERROR, "[MessageHandler::OnGlobalRemoveUser(UserID) Dismatch][%d][%s]", UidToInt(role->GetUserId()), role->GetName());
	//	}
	//}
}

void MessageHandler::OnMailAttachment(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{	
	ggprotocal::MailAttachment *attachment = (ggprotocal::MailAttachment*)msg;
	MailRoute::Instance().OnMailAttachment(role, attachment);
}

void MessageHandler::OnGuildSyncRoleInfo(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
}

void MessageHandler::OnCmdToRole(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::SystemCmdToRole *cmd_to_role = (ggprotocal::SystemCmdToRole *)msg;

	bool ret = CmdHandler::Instance().OnCmdToRole(role, cmd_to_role->effect_type, cmd_to_role->effect_param, cmd_to_role->effect_param1);

	gamelog::g_log_scenestatus.printf(LL_INFO, "[CmdToRole %s! User[%d, %s], effect_type:%d, effect_param:%d, effect_param1:%d]", (ret ? "Succ" : "Fail"),
		UidToInt(user_id), role->GetName(), cmd_to_role->effect_type, cmd_to_role->effect_param, cmd_to_role->effect_param1);
}

void MessageHandler::OnChongzhiNotice(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	//role->GetKnapsack()->GetMoney()->ForceGetGold();
}

void MessageHandler::OnGetOneEquipmentInfo(const UserID &beviewer_user_id, Scene *scene, Role *beviewer_role, ObjID obj_id, void *msg)
{
	
}

void MessageHandler::OnGetOnePetInfo(const UserID &beviewer_user_id, Scene *scene, Role *beviewer_role, ObjID obj_id, void *msg)
{
	
}

void MessageHandler::OnGetOneMountInfo(const UserID &beviewer_user_id, Scene *scene, Role *beviewer_role, ObjID obj_id, void *msg)
{
	
}

void MessageHandler::OnTradeAffirmRoute(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg)
{
	
}

void MessageHandler::OnTradeSyncState(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg)
{
	
}

void MessageHandler::OnTradeAffirmSuccReq(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg)
{
	
}

void MessageHandler::OnCreateGuildFailRollback(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg)
{
}

void MessageHandler::OnGuildDelateFailRollback(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg)
{
}

void MessageHandler::OnGuildBossFeedConsumeReq(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg)
{
	
}

void MessageHandler::OnGuildStoragePutItemConsumeReq(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg)
{

}

void MessageHandler::OnGuildStorageTakeItemGiveReq(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg)
{
	
}

void MessageHandler::OnGuildXiangfangUpgradeLevelConsumeReq(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg)
{
	
}

void MessageHandler::OnGuildJianZhuAddExpConsumeReq(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg)
{
	
}

void MessageHandler::OnGuildShopBuyAck(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg)
{
	
}

void MessageHandler::OnShopBuyLimitItemAck(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg)
{
	
}

void MessageHandler::OnKillOtherCampGetHonour(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::KillOtherCampGetHonour *kocgh = (ggprotocal::KillOtherCampGetHonour *)msg;
	role->OnKillOtherCampGetHonour(kocgh->bekiller_user_id, kocgh->delta_level);
}

void MessageHandler::OnQueryPosition(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::QueryPosition *bgf = (ggprotocal::QueryPosition*)msg;
	role->OnQueryMyPosition(bgf->reason, bgf->from_uid);
}

void MessageHandler::OnRspPosition(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::RspPosition *rsp = (ggprotocal::RspPosition*)msg;
	role->OnRspPosition(rsp->reason, 0 != rsp->is_static_scene, rsp->my_scene_id, rsp->my_scene_key, rsp->pos_x, rsp->pos_y);
}

void MessageHandler::OnBuyPublicSaleItemReq(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
}

void MessageHandler::OnAddMountWithData(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::AddMountWithData *amwd = (ggprotocal::AddMountWithData *)msg;

	bool ret = CmdHandler::Instance().OnAddMountWithData(role, amwd->mount_id, amwd->grade, amwd->strengthen_level);

	gamelog::g_log_scenestatus.printf(LL_INFO, "[AddMountWithData %s! User[%d, %s], mount_id:%d, grade:%d, strengthen_level:%d]", (ret ? "Succ" : "Fail"),
		UidToInt(user_id), role->GetName(), amwd->mount_id, amwd->grade, amwd->strengthen_level);
}

void MessageHandler::OnAddPetWithData(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	ggprotocal::AddPetWithData *apwd = (ggprotocal::AddPetWithData *)msg;

	bool ret =  CmdHandler::Instance().OnAddPetWithData(role, apwd->pet_id, apwd->level, apwd->wuxing, apwd->grow, apwd->zizhi);

	gamelog::g_log_scenestatus.printf(LL_INFO, "[AddPetWithData %s! User[%d, %s], pet_id:%d, level:%d, wuxing:%d, grow:%d, zizhi:%d]", (ret ? "Succ" : "Fail"),
		UidToInt(user_id), role->GetName(), apwd->pet_id, apwd->level, apwd->wuxing, apwd->grow, apwd->zizhi);
}

void MessageHandler::OnFetchOpenGameSubActivityRankReward(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	
}

void MessageHandler::OnXianMengZhanEnterReq(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg)
{
	role->GetRoleActivity()->OnXianMengZhanEnterReq();
}


