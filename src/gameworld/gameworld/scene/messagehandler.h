#ifndef __MESSAGEHANDLER_H__
#define __MESSAGEHANDLER_H__

#include <limits.h>

#include "gamedef.h"
#include "engineadapter.h"
#include "inetworkmodule.h"

class SceneManager;
class Scene;
class Role;

class MessageHandler;
class SerializeMessageHandler;

typedef void (MessageHandler::*OnUIDHandler)(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);

class MessageHandler 
{
public:
	MessageHandler();
	~MessageHandler();

	void Init(SceneManager *scene_manager);
	void Release();

	bool HandleMessage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, const char *msg, unsigned int length);
	void HandleMessageNoLogin(const GSNetID &netid, IP ip, const char *msg, unsigned int length);

	void HandleUIDMessage(const UserID &user_id, Scene *scene, ObjID obj_id, const char *msg, unsigned int length);

	static Role * GetRoleHelper(Scene *scene, ObjID obj_id, const char *funstr);

	static bool is_print_msg_type_from_client;

protected:
	void OnReqEnter(const GSNetID &netid, IP userip, void *ul);		// client未登录前

	typedef void (MessageHandler::*HandleFunc)(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	static const int MAX_MSG_TYPE = (int)USHRT_MAX + 1;

	struct HandlerItem 
	{
		HandlerItem() : func(0), data_size(0) {}
		HandlerItem(HandleFunc f, int s) : func(f), data_size(s) {}

		HandleFunc	func;
		int			data_size;
	};
	HandlerItem m_msg_handler_list[MAX_MSG_TYPE];

	// client登陆后
	void OnTimeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnYiZhanDaoDiGuWu(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnWuXingGuaJiGetReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnLogout(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRoleMove(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnTurnTableRollReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnPerformSkill(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnSetAttackMode(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRoleInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnAllInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTransportReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnHeartBeat(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRoleReAlive(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetEffectListReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetEffectInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChangePoseReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnPerformQTEReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnStartGatherReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnStopGatherReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRestReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRestInviteReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRestInviteAck(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRestStartAffirm(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTeamCallTogether(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTeamCallIn(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnLearnSkillRole(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnSkillCD(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGetAllObjMoveInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnPickBigCoin(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnReqTeamMemberPos(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChangeSceneLineReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnRoleReturnReAlivePosiReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 飞行
	void OnObjFlyHeightChange(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGetKnapsackInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnSendKnapsackGridExtendAuto(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);
	void OnUseItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnUseItemMax(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMoveItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnDiscardItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnBatchDiscardItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnKnapsackStoragePutInOrder(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnPickItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnSplitItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnNPCStorageMove(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnItemCompose(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnBluePrintCompose(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTakeOutAllItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnKnapsackStorageExtendNum(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGetEequipmentInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTakeOffEquipment(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnEquipStrengthen(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnEquipUpStar(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnEquipUpLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnEquipInherit(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnEquipUpQuality(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnEquipLiftLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnEquipRefine(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnStoneUpLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);				//宝石升级
	void OnStoneInlay(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);					//宝石镶嵌
	void OnReqStoneInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnDuanzaoSuitReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnWash(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnEquipCompound(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnEquipUpEternity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnEquipUseEternityLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnShenbingInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShenbingComposeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShenbingOPReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShenbingUpLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFulingReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnEquipCrossEquipOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnJianxinOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRoleChangeCamp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRoleChangeProf(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRoleZhuanShen(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRoleTransformExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGetMillionaireInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGetRoleSpaceMsgInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnOperSpace(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRemoveRecord(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnLifeSkillOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnTaskListReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTaskGiveup(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFlyByShoe(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnSyncJump(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTaskAccept(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTaskCommit(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTaskTalkToNpc(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCishaTaskOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnPaoHuanRollReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnSeekRoleWhereReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShenZhuangOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCampEquipOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCampNormalDuobaoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCSAEquipOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGoldVipOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnHotkeyInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChangeHotkey(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnAccountGoldReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnAccountGetGoldReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnFriendListReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnAddFriendRet(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnAddFriendReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnDeleteFriendReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnEnemyDeleteReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGiveFlower(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGiveFlowerKissReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFriendBlessOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnAddBlackReq( const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnDeleteBlackReq( const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnSongHuaInfoReq( const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnOfflineFriendAutoDecFlag( const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGmCommand(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTestSpeed(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnSendMail(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnDeleteMail(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnLockMail(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnUnlockMail(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetMailList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnReadMail(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetMailCoin(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFetchMailAttachment(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCleanMail(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnOneKeyFetchMailAttachment(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnAddPublicSaleItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRemovePublicSaleItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnBuyPublicSaleItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnPublicSaleGetUserItemList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnPublicSaleSendItemInfoToWorld(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnPublicSearch(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnPublicSaleTypeCountReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGetCampInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChooseCamp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChooseProf(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnLeaveFB(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnEnterFB(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCancelMonsterStaticState(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnAutoFB(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnSetAvatarTimestamp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetRoleExpExtraPer(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCrossQueryRoleInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnQueryRoleInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnQueryRoleInfoByName(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void ONCrossFindRoleByUUID(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCreateGuild(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);	
	void OnDimissGuild(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnApplyForJoinGuild(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnApplyForJoinGuildAck(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnQuitGuild(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnInviteGuild(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnInviteGuildAck(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnKickoutGuild(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnAppointGuild(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnLeaveGuildPost(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildChangeNotice(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildMailAll(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetGuildInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildSetStorageShopUse(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildDelate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildStorageReqUse(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildStoragePutItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildStorageTakeItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildBossFeed(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildBossCall(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildSignIn(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildBackToStation(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildTianCiTongBiReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildTowerDefendReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildTowerDefendNextWaveReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildXianshuInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildXianshuUplevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildEnemyRankList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCreateGuildFailRollback(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg);
	void OnGuildDelateFailRollback(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg);
	void OnGuildBossFeedConsumeReq(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg);
	void OnGuildStoragePutItemConsumeReq(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg);
	void OnGuildStorageTakeItemGiveReq(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg);
	void OnGuildXiangfangUpgradeLevelConsumeReq(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg);
	void OnGuildJianZhuAddExpConsumeReq(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg);

	void OnClearTrainMentalityCD(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTrainMentality(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnUpgradeMentalityGengu(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnSpeedUpMentalityTrain(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMentalityYijianTisheng(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMentalityUplevelWuxing(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnDiscountBuyGetInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnDiscountBuyReqBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnZeroGiftOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnAdvanceNoticeOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGoldTurntableOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnFetchVipLevelReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFetchVipWeekReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFetchTimeLimitVip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnQiFuReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFetchQiFuReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGetRoleCapability(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnSetMoveMode(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnChestShopBuyItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChestShopFetchItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChestShopGetSelfItemList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChestShopGetFreeInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChestShopAutoRecycle(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRareChestShopBuyItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChestShopRecordListReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnMoveChessResetReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMoveChessShakeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMoveChessFreeInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnShopBuyItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMysteriousShopOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMysteriousShopInMallOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnSpeakerTalk(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGetTitleList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnUseTitle(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnUpgradeTitle(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGuildShopBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildShopGetRoleBuyInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildResetName(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildApplyforSetup(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildAddGuildExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildUpLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildGetBuff(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildExchange(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildGetFuliGift(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildYaojiangReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildGetShenluBindCoin(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildGetLonghuiInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildTrainLonghui(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildUPgradeLonghui(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildSetAutoKickoutSetup(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildCallIn(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildCheckCanDelate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildFbStartReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildBonfireStartReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildFbEnterReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildExtendMemberReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildZhufuMemberLuckyReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetAllMemberLuckyInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnInviteZhuFuLuckyReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildCallBeastReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildActiveDegreeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildBossOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGotoBonfirePos(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnAddBonfireMucai(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildStorgeOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildStorgeOneKeyOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildAutoClearReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildSkillUplevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildUpTotemLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildBoxOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildCommonReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildFetchTerritoryReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildFetchBossRedbag(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildPaoSaizi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnReqGuildPaoSaizi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildRedPaperListInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildRedPaperCreateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildRedPaperSingalChatReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildMazeOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildSigninReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildChangeAvatar(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGetDailyWelfareInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFetchSevenDayLoginReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetOfflineExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetDailyFindWelfare(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFetchSignInReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFetchOnlineReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnSignInFindBack(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnWelfareActivityFind(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnWelfareAutoActivity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFetchHappytreeReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFetchChongjihaoliReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnActiveFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnSaveHangupSet(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnSaveCustomSet(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRoleResetName(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRoleSetZombieReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnDailyWelfareTipsReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);	

	void OnEvaluateRole(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnConvertScoreToItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetConvertRecordInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetSocreInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnClientSpecialOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnFetchDailyLoginReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnSublockCreatePw(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnSublockClearPw(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnSublockLock(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnSublockUnlock(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnMysteriousShopBuyItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMysteriousShopFlush(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMysteriousShopNewsReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMysteriousShopItemInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCallXiannv(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnXiannvRename(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnXiannvActiveReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnXiannvAddZizhiReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnXiannvActiveHuanhuaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnXiannvChangeImageReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnXiannvHuanHuaUpLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnXiannvChangeSkillReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnXiannvShengwuReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnFamousManOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnWabaoOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChongzhiFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnZaiChongzhiFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChargeWantMoneyFetchMsg(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChargeWantMoneyFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnJilianOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGoonMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnStrengthMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnUpgradeMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnUseMountImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMountGetInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMountUplevelEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMountSpecialImgUpgrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMountSkillUplevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMountUpStarLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGoonFightMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnStrengthFightMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnUpgradeFightMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnUseFightMountImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFightMountGetInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFightMountUplevelEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFightMountSpecialImgUpgrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFightMountSkillUplevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFightMountUpStarLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnUpgradeWing(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnUseWingImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnWingSpecialImgUpgrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnWingUplevelEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnWingSkillUplevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnWingGetInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnWingUpStarLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnUpgradeHalo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnUseHaloImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnHaloSpecialImgUpgrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnHaloUplevelEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnHaloSkillUplevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnHaloGetInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnHaloUpStarLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnUpgradeShengong(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnUseShengongImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShengongSpecialImgUpgrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShengongUplevelEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShengongSkillUplevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShengongGetInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShengongUpStarLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnUnUseShengongImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnUpgradeShenyi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnUseShenyiImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShenyiSpecialImgUpgrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShenyiUplevelEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShenyiSkillUplevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShenyiGetInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShenyiUpStarLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnUnUseShenyiImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnFootprintOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCloakOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnMojieInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMojieUplevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMojieChangeSkillReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnXiannvShouhuUseImageReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnXiannvShouhuGetInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnXiannvShouhuUpstarReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnJinglingGuanghuanUseImageReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnJinglingGuanghuanGetInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnJinglingGuanghuanUpstarReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnJinglingGuanghuanSpecialImgUpgrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnFairyBuddhaCardActivateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFairyBuddhaCardGoldBindReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGuildQuestionEnterReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnJinglingFazhenUseImageReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnJinglingFazhenGetInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnJinglingFazhenUpstarReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnJinglingFazhenSpecialImgUpgrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnActivityEnterReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnSendGuildSosReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnReplyGuildSosReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnZhuXieFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnImmediatelyCompleteTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnTuMoTaskOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGetPaoHuanTaskInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnRefreshHoSongTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGetFBEquipInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnNeqTakeStarRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnNeqExchangeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnNeqBuyTimesReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnNeqAutoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnNeqInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnNeqRollReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnArmorDefendRoleReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnDailyFBGetRoleInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFetchExpFbFirstRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnExpFbPayBuff(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnPataFBGetAllInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnVipFBGetAllInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGuildMonsterQuery(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFightingMiningReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnPhaseFBInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnEnterBossFamilyReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnBossFamilyOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetBossInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnBossKillerInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetMonsterGenerateInfo(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnGetGatherGenerateInfo(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnFollowBossOperReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnPreciousPosReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnYunyouBossOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGetYaoShouGuangChangState(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnGetYaoShouGuangChangReward(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnEnterYaoShouGuangChang(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnGetSuoYaoTaState(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnGetSuoYaoTaReward(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnEnterSuoYaoTa(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	void OnGetGuildFBGuardPos(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	void OnTeamEquipFbBuyDropCount(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	void OnBattleFieldPickUpFlagReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnQunxianLuandouFirstRankReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	// 公会争霸领取每日奖励
	void OnGuildBattleFetchDailyRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGuildInfoStatisticReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGczChangePlace(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnTeamFbRoomOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGetTowerDefendRoleInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTowerDefendBuyJoinTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTowerDefendNextWave(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	void OnMiGongXianFuTouchDoor(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnStoryFBGetInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnFunOpenStoryStep(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFunOpenSetObjPos(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnFbGuideFinish(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFbGuideCreateGather(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnFbGuwu(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnExpFBFetchChapterReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChallengeFBBuyJoinTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChallengeFBReqInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChallengeFBOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnBattleFieldHonorInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnQueryActivityRomeStatusReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnEnterYaoshouzhiluanReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShiZongCaiFengGetTaskList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShiZongCaiFengAcceptTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGuildBattleAddHuDunReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildBattleSubmitGoodsReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGuildBattleWinerInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnMillionaireUseSkillReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnTerritoryWarReliveBuyReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTerritoryWarFightBuyReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTerritoryQualificationReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTerritorySetLandMineReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnHotspringGivePresentReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnHotspringAddPartnerReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnHotspringAddPartnerRet(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnHotspringDeletePartnerReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnHotspringQAFirstPostReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnHotspringQAAnswerReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnHotspringQAUseCardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnHotspringUseSkillReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCrossGuildBattleOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCrossGuildBattleGetRankInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnQuestionAnswerReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnXianMengZhanGuildCall(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnXianMengZhanGuildFollow(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnHusongBuyTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnHusongAddShiled(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnRoleTaskTransmit(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnFinishAllGuildTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetGuildDoubleReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnForceFinishSpecialTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnOpenTaskPanel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFetchTouZiJiHuaReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTouzijihuaActive(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnNewTouzijihuaOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTouzijihuaFbBossOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnYaojiangReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnDatingInviteReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnDatingInviteAck(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnFishPoolRaiseReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFishPoolBuyBulletReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFishPoolQueryReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFishPoolStealFish(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFishPoolHarvestFish(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFishPoolExtendCapacity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFishPoolUpQuality(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnPaoHuanSkipTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnMountBianshenOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnSpeedupHello(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnMarryXunyouOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMarryHunyanBless(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMarryHunyanGetBlessNews(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMarryHunyanOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnQingyuanOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnResetLoverName(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMarryReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMarryRet(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnBuyXunyouOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnDivorceRet(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMarryZhuheShou(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnQingYuanShengDiOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 表白墙
	void OnProfessWallReq(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnProfessToReq(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	void OnLightenWeddingRing(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnUplevelWeddingRing(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnPerformWeddingRingSkill(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnSupplyBuyItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnSupplySetRecoverRangePer(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnNationalBossBuyBuffReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnGetOpenGameActivityInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetOpenGameFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnBaiBeiFanLiBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnBaiBeiFanLi2Buy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetTitleOwnerInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnSupperGiftBuyReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnSupperGiftInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnSupperGiftBuy2Req(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);		//限购礼包2

	void OnFetchMagicalPreciousReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnRAExpRefineOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnChallengeFieldGetUserInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChallengeFieldResetOpponentReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChallengeFieldFightReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChallengeFieldBuyJoinTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChallengeFieldFetchJifenRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChallengeFieldGetRankInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChallengeFieldGetLuckyInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChallengeFieldFetchGuangHui(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChallengeFieldGetOpponentDetailInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChallengeFieldBuyBuff(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChallengeFieldBestRankBreakReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChallengeFieldReadyStartFightReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCloseBetaActivityOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnLuckyRollActivityOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnRandActivityOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnUpdateNoticeFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCommonActivityFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnMazeQuery(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMazeMove(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMazeBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMazeFetchJifenReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	//////////////////////////////////////////////////////////////////////////////////////////////////
	void OnFriendResetName(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnFriendSetSex(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnFriendSetLevel(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);

	void OnForbidTalk(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnGetRoleOfficerInfo(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnGlobalRemoveUser(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnMailAttachment(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnGuildSyncRoleInfo(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnCmdToRole(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnGetOneEquipmentInfo(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnGetOnePetInfo(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnGetOneMountInfo(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnChongzhiNotice(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnGetYuanLiFBChapterLordGift(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnMarryReserveReq(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnMarryHunyanEnter(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);

	void OnXiannvBuyReqToGS(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnActivityFindReqToGS(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);

	void OnDatingInviteCheckTarget(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnDatingConsume(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnDatingReward(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnDatingUpdateAddExp(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnDatingDaycountCheck(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnLoverResetName(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);

	// p2p交易
	void OnTradeAffirmRoute(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg);
	void OnTradeSyncState(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg);
	void OnTradeAffirmSuccReq(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg);

	void OnGuildShopBuyAck(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg);
	void OnShopBuyLimitItemAck(const UserID& user_id, Scene* scene, Role* role, ObjID obj_id, void* msg);

	void OnKillOtherCampGetHonour(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);

	void OnBuyPublicSaleItemReq(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);

	void OnAddMountWithData(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnAddPetWithData(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);

	void OnFetchOpenGameSubActivityRankReward(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnXianMengZhanEnterReq(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);

	void OnQueryPosition(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);
	void OnRspPosition(const UserID &user_id, Scene *scene, Role *role, ObjID obj_id, void *msg);

	void OnCreateTeam(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void *msg, int length);
	void OnInviteUser(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void *msg, int length);
	void OnInviteUniqueUser(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void *msg, int length);
	void OnInviteRet(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void *msg, int length);
	void OnJoinTeamRet(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnReqJoinTeam(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnDismissTeam(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnKickOutOfTeam(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnChangeTeamLeader(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnExitTeam(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnTeamListReq(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnChangeMustCheck(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnChangeAssignMode(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnChangeMemberCanInvite(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnChangeTeamLimit(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnAutoHaveTeam(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnAutoApplyJoinTeam(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	void OnGetRandomRoleList(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	void OnChannelChat(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnSingleChat(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnSingleChatStatusReq(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnRequestForbidChatInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnWorldBossKillerInfoReq(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnGetWorldBossInfo(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnGetWorldBossPersonalHurtInfo(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnGetWorldBossGuildHurtInfo(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnGetWorldBossWeekRankInfo(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnGetWorldBossRollReq(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	void OnGetPersonRankList(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnGetPersonRankTopUser(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnGetGuildRankList(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnGetTeamRankList(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	void OnQingyuanFBOperaReq(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnHunjieUpLevel(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnHunjieReqEquipInfo(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnQingyuanTakeOffEquip(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnQingyuanQueryMateQingyuanValue(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnQingyuanDivorceReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFBReqNextLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRollMoneyOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnQingyuanEquipOperate(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	void OnRoleGoalOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnZhanshendianFetchDayRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnReqTrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnReqTradeRet(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTradeLock(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTradeAffirm(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTradeCancle(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTradeGold(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTradeItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTradePet(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnTouchWorldEventObj(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnWorldEventFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnWorldEventQueryRoleInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnWorldEventQueryCommonInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnWorldEventLightWord(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnWorldEventGetNpcLocateInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnRedPaperCreateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRedPaperFetchReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnRedPaperQueryDetailReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCreateCommandRedPaperReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFetchCommandRedPaperReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCommandRedPaperCheckReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnMarriageSeekingReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnShizhuangUseReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShizhuangUpgradeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShizhuangUpgradeSpecialImgReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShizhuangUpLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShizhuangUpEquipLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCardOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMonthCardFetchDayReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCSAQueryActivityInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCSARoleOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnDisconnectReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnZhuanzhiReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnQingyuanCardUpgradeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnWushuangEquipOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnHunshouOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnWushuangFBInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCrossStartReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCardzuOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnZhuanShengOpearReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnRuneSystemOperaReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);
	void OnRuneSystemDiscopeOneKey(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);
	void OnRuneTowerFetchTime(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);
	void OnRuneTowerAutoFb(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	void OnCrossXiuluoTowerBuyBuff(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCrossXiuluoTowerScoreRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCrossXiuluoTowerBuyBuffReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCross1V1MatchReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCross1V1FightReady(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCross1v1FetchRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCross1v1RingOperReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCross1v1MatchResultReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCross1v1BuyTimeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCrossMultiuserChallengeMatchingReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCrossMultuuserChallengeGetBaseSelfSideInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCrossMultiuserChallengeFetchDaycountReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCrossMultiuserChallengeCancelMatching(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCrossMultiuserChallengeReqSideMemberPos(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCrossMultiuserChallengeFetchGongxunReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnMultiuserChallengeWearCardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCrossTuanZhanFetchRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnMessBattleEnterReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnBuildTowerReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnBuildTowerBuyTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnLieMingHunshouOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnLieMingHunshouExchangeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnJingLingOperReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnJingLingHomeOperReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnJinglIngExploreOperReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnShengWangOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnChengJiuOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnShenzhouWeaponOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShenzhouWeaponOneIdentifyReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnShenzhouWeaponHunyinResolveReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnUseTuHaoJinReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnTuHaoJinUpLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnBigFaceUpLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnBabyOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnBabyUpgradeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnBabyRenameReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnRATotalCharge3RequestInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnPetOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnPetUpgradeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnPetUseSpecialImg(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnRATotalCharge4RequestInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnHuaShenOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnQingyuanCoupleHaloOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnQingyuanFetchBlessRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnQingyuanAddBlessDays(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnLoveContractOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnQingyuanFetchLoveContract(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnPastureSpiritOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnMultiMountOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnPersonalizeWindowOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnMagicCardOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnWushangEquipOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCrossBossBuyReliveTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnCrossBossBossInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnWingFuMoUplevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnMitamaOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnFairyTreeOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnMagicEquipmentOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnXunZhangUpLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnZhibaoUplevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnZhibaoUseImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnZhibaoHuanhua(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCrossTeamFBOptionReq(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	void OnCommonOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	//	小宠物
	void OnLittlePetOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnLittlePetReName(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	//	相思树
	void OnLoveTreeWaterReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnLoveTreeInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 星座
	void OnChineseZodiacPromoteEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChineseZodiacPromoteXingHun(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChineseZodiacPromote(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChineseTianxiangReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChineseZodiacTakeOffEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnChineseZodiacRecycle(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 好友送礼
	void OnFriendGiftSongGift(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFriendGiftInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnFriendHeliRet(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGiveItemOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGiveItemReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnCrossShuiJingBuyBuff(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnShuiJingBuyBuff(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnShuiJingFetchTaskReward(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	void OnWanglingExploreBuyBuff(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnCrossBianjingZhiDiBuyBuff(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	// 好友经验瓶
	void OnFriendExpBottle(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	// 我要脱单
	void OnTuodanReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnGetAllTuodanInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	OnUIDHandler m_on_uid_handler[1024];

	// 攻城战
	void OnGongChengZhanFetchDailyRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	void OnChongZhi7DayFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	
	// 神格
	void OnShengeSystemOperaReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	// 推图副本
	void OnTuituFbOperaReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	// 金猪召唤
	void OnGoldenPigOperate(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	// 神兽
	void OnShenshouOperaReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);
	void OnShenshouReqStrength(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	// 一键跳过
	void OnSkipReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	// 形象赋灵
	void OnImgFulingOperaReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	// 元素之心
	void OnElementHeartOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 神印系统批量回收
	void OnShenYinOneKeyRecyleOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 升星助力信息
	void OnGetShengxingzhuliInfoReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	// 升星助力领取奖励
	void OnGetShengxingzhuliRewardReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	// 飞仙
	void OnFeixian(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	//境界
	void OnRoleJingJieOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	// 月黑风高
	void OnCrossDarkNightOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	//物品掉落
	void OnGetDropLog(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	//宝宝boss
	void OnBabyBossOperate(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);
	//天赋
	void OnRoleTelentOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	//进阶天赋
	void OnJinJieTalentOperaReqAll(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	//河神洛书
	void OnHeShenLuoShuReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	//圣器系统
	void OnShengqiOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 上古boss
	void OnShangGuBossReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	//守护小鬼
	void OnImpGuardOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	//情缘排行榜
	void OnGetCoupleRankList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);	
	//boss图鉴
	void OnBossCardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	//人物转职
	void OnRoleZhuanZhiReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 进阶系统操作
	void OnUpgradeSysOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 腰饰操作
	void OnYaoShiOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 头饰操作
	void OnToushiOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 麒麟臂操作
	void OnQiLinBiOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 面具操作
	void OnMaskOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 转职装备
	void OnZhuanzhiEquipOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	//跨服钓鱼
	void OnFishingOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	//夜战王城
	void OnNightFightOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	
	// 圣印系统
	void OnSealOperaReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);
	void OnSealRecycleReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	// 组队守护
	void OnTeamTowerDefendOpreatReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	
	// 帮派运镖
	void OnGuildBiaocheOpera(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	// 膜拜城主
	void OnGongChenZhanWorship(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	// 膜拜
	void OnRoleWorship(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	//幸运儿日志
	void OnLuckyLogItemReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	void OnJingLingAdvantageBossEnter(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 节日守护(塔防)
	void OnHolidayGuardReqAction(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);
	void OnHolidayGuardRankInfoReq(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	// 累计充值5
	void OnRATotalCharge5RequestInfo(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);
	// 进阶系统大小目标
	void OnJinjiesysRewardOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 衣柜
	void OnDressingRoomOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	void OnDressingRoomExchange(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 角色大小目标
	void OnRoleBigSmallGoalOper(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);
	
	void OnShenqiOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 跨服排行榜
	void OnGetSpecialRankValue(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	//天天返利
	void OnDayChongzhiRewardReq(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	//仙尊卡
	void OnXianZunKaOperaReq(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	// 夫妻家园操作请求
	void OnSpouseHomeOpera(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	//装备洗炼
	void OnEquipBaptizeOperaReq(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	//视野协议广播,客户端自己用特殊操作
	void OnRolePersonAreaMsgInfoReq(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length);

	// 跨服秘藏boss
	void OnCrossMizangBossBossInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 跨服幽冥boss
	void OnCrossYoumingBossBossInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	//随机活动-幸运许愿
	void OnRALuckyWishOpera(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	// 购买双倍奖励
	void OnFetchDoubleReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 个人boss
	void OnPersonBossReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	
	// 名将
	void OnGreateSoldierOperaReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);
	void OnGreateSoldierUpgradeEquipReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	// 跨服随机活动请求
	void OnCrossRandActivityRequest(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	//直升丹购买
	void OnUpgradeCardBuyReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	//神魔boss
	void OnGodmagicBossOperate(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	// 天神护体
	void OnHuguozhiliReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	//今日主题
	void OnTodayThemeRewardReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);
	
	// 全服收购
	void OnWorldAcquisitionLogReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 跨服目标
	void OnCrossGoalReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 百战装备
	void OnBaizhanEquipOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 斗气装备
	void OnCSCrossEquipOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	//小助手
	void OnLittleHelperOpera(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);
	void OnLittleHelperRepeatOpera(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);
	
	//仙盟珍稀日志
	void OnGuildRareLogItemReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	//等级装备套装激活
	void OnActiveEquipGradeSuitReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	
	void OnTianShuXZFetchReward(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);

	// 跨服竞技场
	void OnCrossChallengefieldReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);
	
	//转职装备合成请求
	void OnZhuanzhiEquipComposeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	//特惠商店请求购买
	void OnDiscountShopReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);
	//神话
	void OnMythOperaReq(const GSNetID & netid, Scene * scnen, Role * role, ObjID obj_id, void * msg, int length);
	//星象
	void OnZodiacReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);
	void OnZodiacDecomposeReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length);
	//跨服护送水晶
	void OnCrossHusongShuijingReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

	// 周末装备
	void OnTianshenhutiOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length);

private:
	SceneManager	*m_scene_manager;
	SerializeMessageHandler *m_serialize_message_handler;
}; 

#endif

