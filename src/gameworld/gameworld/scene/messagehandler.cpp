#include <assert.h>

#include "netmsg/serialize_messagehandler.h"
#include "messagehandler.h"
#include "gamelog.h"

#include "servercommon/servercommon.h"
#include "servercommon/userprotocal/msgcode.h"
#include "servercommon/userprotocal/globalmsgcode.h"
#include "servercommon/userprotocal/chatmsgcode.h"
#include "servercommon/internalprotocal/ggprotocal.h"
#include "servercommon/userprotocal/crossmsgcode.h"
#include "internalcomm.h"

#include "config/sharedconfig/sharedconfig.h"
#include "global/cross/crossmanager.h"
#include "other/greatesoldier/greatesoldier.hpp"
#include "other/rolecrossrandactivity/rolecrossrandactivity.hpp"

#include "protocal/msgentergs.h"
#include "protocal/msgscene.h"
#include "protocal/msgfight.h"
#include "protocal/msgrole.h"
#include "protocal/msgitem.h"
#include "protocal/msgnpc.h"
#include "protocal/msghotkey.h"
#include "protocal/msgmoney.h"
#include "protocal/msgwushuang.h"
#include "protocal/msgfriend.h"
#include "protocal/msgother.h"
#include "protocal/msgroleother.h"
#include "protocal/msggm.hpp"
#include "protocal/msgmail.hpp"
#include "protocal/msgcspublicsale.hpp"
#include "protocal/msgcswantbuy.hpp"
#include "protocal/msgmentality.hpp"
#include "protocal/msgguild.hpp"
#include "protocal/msgmount.hpp"
#include "protocal/msgfightmount.hpp"
#include "protocal/msgwing.hpp"
#include "protocal/msghalo.hpp"
#include "protocal/msgshengong.hpp"
#include "protocal/msgshenyi.hpp"
#include "protocal/msgfootprint.hpp"
#include "protocal/msgcloak.hpp"
#include "protocal/msgactivity.hpp"
#include "protocal/msgactivedegree.hpp"
#include "protocal/msgsublock.hpp"
#include "protocal/msgtitle.hpp"
#include "protocal/msgfb.h"
#include "protocal/msgteam.hpp"
#include "protocal/msgserver.h"
#include "protocal/msgcsrank.h"
#include "protocal/msgscpublicsale.h"
#include "protocal/msgcscamp.hpp"
#include "protocal/msgtrade.h"
#include "protocal/msgfishpool.h"
#include "protocal/msglieming.h"
#include "protocal/msgshengwang.h"
#include "protocal/msgxiannvshouhu.hpp"
#include "protocal/msgjinglingfazhen.hpp"
#include "protocal/msgjinglingguanghuan.hpp"
#include "protocal/msgzhuanshengequip.h"
#include "protocal/msgmojie.hpp"
#include "protocal/msgtuhaojin.h"
#include "protocal/msgbigchatface.h"
#include "protocal/msgshenbing.hpp"
#include "protocal/msgshop.h"
#include "protocal/msgchatmsg.h"
#include "protocal/msgworldevent.h"
#include "protocal/msgcardzu.h"
#include "protocal/msgchengjiu.h"
#include "protocal/msgshenzhouweapon.h"
#include "protocal/msgbaby.h"
#include "protocal/msgpet.h"
#include "protocal/msghuashen.h"
#include "protocal/msgpasturespirit.h"
#include "protocal/msgmultimount.h"
#include "protocal/msgpersonalizewindow.h"
#include "protocal/msgmagiccard.h"
#include "protocal/msgwushangequip.h"
#include "protocal/msgmitama.h"
#include "protocal/msgfairytree.h"
#include "protocal/msgmagicequipment.h"
#include "protocal/msgxunzhang.h"
#include "protocal/msglittlepet.h"
#include "protocal/msglovetree.h"
#include "protocal/msgchinesezodiac.h"
#include "protocal/msgfriendgift.hpp"
#include "protocal/msgtuodan.h"
#include "protocal/msgequipment.h"
#include "protocal/msgjingling.h"
#include "protocal/msgrunesystem.h"
#include "protocal/msgxiannv.h"
#include "protocal/msgfighting.h"
#include "protocal/msgfamousman.h"
#include "protocal/msgshengesystem.h"
#include "protocal/msgshenshou.h"
#include "protocal/msgimgfuling.hpp"
#include "protocal/msgelementheart.h"
#include "protocal/msgshengxingzhuli.h"
#include "protocal/msgfeixian.hpp"
#include "gameworld/netmsg/protocol/protocol_110.hpp"
#include "protocal/msgjinjietalent.hpp"
#include "protocal/msgheshenluoshu.h"
#include "protocal/msgshengqi.hpp"
#include "protocal/msgimpguard.h"
#include "protocal/msgzhuanzhi.h"
#include "protocal/msgupgrade.hpp"
#include "protocal/msgyaoshi.hpp"
#include "protocal/msgtoushi.hpp"
#include "protocal/msgqilinbi.hpp"
#include "protocal/msgmask.hpp"
#include "protocal/msgzhuanzhiequip.hpp"
#include "protocal/msgdressingroom.hpp"
#include "protocal/msgshenqi.hpp"
#include "protocal/msgspousehome.hpp"
#include "protocal/msggreatesoldier.h"
#include "protocal/msgcsrandactivity.hpp"
#include "protocal/msgcrossequip.h"
#include "protocal/msgmyth.h"
#include "protocal/msgzodiac.hpp"
#include "protocal/msgtianshenhuti.h"



MessageHandler::MessageHandler() : m_scene_manager(NULL)
{
	m_serialize_message_handler = new SerializeMessageHandler();
	memset(m_msg_handler_list, 0, sizeof(m_msg_handler_list));

	m_msg_handler_list[Protocol::MT_LOGOUT_CS]					= HandlerItem(&MessageHandler::OnLogout, sizeof(Protocol::CSUserLogout));
	m_msg_handler_list[Protocol::MT_OBJ_MOVE_CS]				= HandlerItem(&MessageHandler::OnRoleMove, sizeof(Protocol::CSObjMove));
	m_msg_handler_list[Protocol::MT_ROLE_SET_ATTACK_MODE_CS]	= HandlerItem(&MessageHandler::OnSetAttackMode, sizeof(Protocol::CSSetAttackMode));
	m_msg_handler_list[Protocol::MT_PERFORM_SKILL_CS]			= HandlerItem(&MessageHandler::OnPerformSkill, sizeof(Protocol::CSPerformSkillReq));
	m_msg_handler_list[Protocol::MT_ROLE_INFO_REQ_CS]			= HandlerItem(&MessageHandler::OnRoleInfoReq, sizeof(Protocol::CSRoleInfoReq));
	m_msg_handler_list[Protocol::MT_ALL_INFO_REQ_CS]			= HandlerItem(&MessageHandler::OnAllInfoReq, sizeof(Protocol::CSAllInfoReq));
	m_msg_handler_list[Protocol::MT_TRANSPORT_REQ]				= HandlerItem(&MessageHandler::OnTransportReq, sizeof(Protocol::CSTransportReq));
	m_msg_handler_list[Protocol::MT_HEARTBEAT_CS]				= HandlerItem(&MessageHandler::OnHeartBeat, sizeof(Protocol::CSHeartBeat));
	m_msg_handler_list[Protocol::MT_REALIVE_REQ_CS]				= HandlerItem(&MessageHandler::OnRoleReAlive, sizeof(Protocol::CSRoleReAliveReq));
	m_msg_handler_list[Protocol::MT_GET_EFFECT_LIST_REQ_CS]		= HandlerItem(&MessageHandler::OnGetEffectListReq, sizeof(Protocol::CSGetEffectListReq));
	m_msg_handler_list[Protocol::MT_GET_EFFECT_INFO_REQ_CS]		= HandlerItem(&MessageHandler::OnGetEffectInfoReq, sizeof(Protocol::CSGetEffectInfoReq));
	m_msg_handler_list[Protocol::MT_PERFORM_QTE_SKILL_CS]		= HandlerItem(&MessageHandler::OnPerformQTEReq, sizeof(Protocol::CSPerformQTESkillReq));
	m_msg_handler_list[Protocol::MT_START_GATHER_REQ_CS]		= HandlerItem(&MessageHandler::OnStartGatherReq, sizeof(Protocol::CSStartGatherReq));
	m_msg_handler_list[Protocol::MT_STOP_GATHER_REQ_CS]			= HandlerItem(&MessageHandler::OnStopGatherReq, sizeof(Protocol::CSStopGatherReq));
	m_msg_handler_list[Protocol::MT_LEAVE_FB_CS]				= HandlerItem(&MessageHandler::OnLeaveFB, sizeof(Protocol::CSLeaveFB));
	m_msg_handler_list[Protocol::MT_ENTER_FB_CS]				= HandlerItem(&MessageHandler::OnEnterFB, sizeof(Protocol::CSEnterFB));
	m_msg_handler_list[Protocol::MT_CANCEL_MONSTER_STATIC_STATE_CS] = HandlerItem(&MessageHandler::OnCancelMonsterStaticState, sizeof(Protocol::CSCancelMonsterStaticState));
	m_msg_handler_list[Protocol::MT_QUERY_ROLE_INFO_CS]			= HandlerItem(&MessageHandler::OnQueryRoleInfo, sizeof(Protocol::CSQueryRoleInfo));
	m_msg_handler_list[Protocol::MT_CROSS_QUERY_ROLE_INFO_CS]	= HandlerItem(&MessageHandler::OnCrossQueryRoleInfo, sizeof(Protocol::CSCrossQueryRoleInfo));
	m_msg_handler_list[Protocol::MT_QUERY_ROLE_INFO_BY_NAME_CS]	= HandlerItem(&MessageHandler::OnQueryRoleInfoByName, sizeof(Protocol::CSFindRoleByName));
	m_msg_handler_list[Protocol::MT_AUTO_FB_CS]					= HandlerItem(&MessageHandler::OnAutoFB, sizeof(Protocol::CSAutoFB));
	m_msg_handler_list[Protocol::MT_SET_AVATAR_TIMESTAMP_CS]	= HandlerItem(&MessageHandler::OnSetAvatarTimestamp, sizeof(Protocol::CSSetAvatarTimeStamp));
	m_msg_handler_list[Protocol::MT_CROSS_QUERY_EXP_EXTRA_CS]	= HandlerItem(&MessageHandler::OnGetRoleExpExtraPer, sizeof(Protocol::CSGetExpExtraPer));
	m_msg_handler_list[Protocol::MT_ROLE_RETURN_REALIVE_POSI_CS] = HandlerItem(&MessageHandler::OnRoleReturnReAlivePosiReq, sizeof(Protocol::CSRoleReturnReAlivePosi));

	m_msg_handler_list[Protocol::MT_REST_REQ_CS]				= HandlerItem(&MessageHandler::OnRestReq, sizeof(Protocol::CSRestReq));
	m_msg_handler_list[Protocol::MT_REST_INVITE_CS]				= HandlerItem(&MessageHandler::OnRestInviteReq, sizeof(Protocol::CSInviteRestReq));
	m_msg_handler_list[Protocol::MT_REST_INVITE_ACK_CS]			= HandlerItem(&MessageHandler::OnRestInviteAck, sizeof(Protocol::CSInviteRestAck));
	m_msg_handler_list[Protocol::MT_REST_START_AFFIRM_CS]		= HandlerItem(&MessageHandler::OnRestStartAffirm, sizeof(Protocol::CSRestStartAffirm));
	m_msg_handler_list[Protocol::MT_TEAM_CALL_TOGETHER_CS]		= HandlerItem(&MessageHandler::OnTeamCallTogether, sizeof(Protocol::CSTeamCallTogether));
	m_msg_handler_list[Protocol::MT_TEAM_CALL_IN_CS]			= HandlerItem(&MessageHandler::OnTeamCallIn, sizeof(Protocol::CSTeamCallIn));
	m_msg_handler_list[Protocol::MT_ROLE_SKILL_LEARN_REQ_CS]	= HandlerItem(&MessageHandler::OnLearnSkillRole, sizeof(Protocol::CSRoleSkillLearnReq));
	m_msg_handler_list[Protocol::MT_SKILL_CD_CS]				= HandlerItem(&MessageHandler::OnSkillCD, sizeof(Protocol::CSSkillCDReq));

	m_msg_handler_list[Protocol::MT_GET_ALL_OBJ_MOVE_INFO_REQ_CS] = HandlerItem(&MessageHandler::OnGetAllObjMoveInfoReq, sizeof(Protocol::CSGetAllObjMoveInfoReq));
	m_msg_handler_list[Protocol::MT_PICK_BIG_COIN_CS]			= HandlerItem(&MessageHandler::OnPickBigCoin, sizeof(Protocol::CSPickBigCoin));
	m_msg_handler_list[Protocol::MT_REQ_TEAM_MEMBER_POS_CS]		= HandlerItem(&MessageHandler::OnReqTeamMemberPos, sizeof(Protocol::CSReqTeamMemberPos));
	m_msg_handler_list[Protocol::MT_CHANGE_SCENE_LINE_REQ_CS]	= HandlerItem(&MessageHandler::OnChangeSceneLineReq, sizeof(Protocol::CSChangeSceneLineReq));

	// 飞行
	m_msg_handler_list[Protocol::MT_OBJ_FLY_HEIGHT_CHANGE_CS] = HandlerItem(&MessageHandler::OnObjFlyHeightChange, sizeof(Protocol::CSObjFlyHeightChange));

	m_msg_handler_list[Protocol::MT_KNAPSACK_INFO_REQ_CS]		= HandlerItem(&MessageHandler::OnGetKnapsackInfo, sizeof(Protocol::CSKnapsackInfoReq));
	m_msg_handler_list[Protocol::MT_KNAPSACK_GRID_EXTEND_AUTO_CS] = HandlerItem(&MessageHandler::OnSendKnapsackGridExtendAuto, sizeof(Protocol::CSKnapsackGridExtendAuto));
	m_msg_handler_list[Protocol::MT_USE_ITEM_CS]				= HandlerItem(&MessageHandler::OnUseItem, sizeof(Protocol::CSUseItem));
	m_msg_handler_list[Protocol::MT_MOVE_ITEM_CS]				= HandlerItem(&MessageHandler::OnMoveItem, sizeof(Protocol::CSMoveItem));
	m_msg_handler_list[Protocol::MT_DICARD_ITEM_CS] = HandlerItem(&MessageHandler::OnDiscardItem, sizeof(Protocol::CSDiscardItem));
	m_msg_handler_list[Protocol::MT_BATCH_DICARD_ITEM_CS]				= HandlerItem(&MessageHandler::OnBatchDiscardItem, sizeof(Protocol::CSBatchDiscardItem));

	m_msg_handler_list[Protocol::MT_KNAPSACK_STORAGE_PUTINORDER_CS]		= HandlerItem(&MessageHandler::OnKnapsackStoragePutInOrder, sizeof(Protocol::CSKnapsackStoragePutInOrder));
	m_msg_handler_list[Protocol::MT_PICK_ITEM_CS]				= HandlerItem(&MessageHandler::OnPickItem, sizeof(Protocol::CSPickItem));
	m_msg_handler_list[Protocol::MT_SPLIT_ITEM_CS]				= HandlerItem(&MessageHandler::OnSplitItem, sizeof(Protocol::CSSplitItem));
	m_msg_handler_list[Protocol::MT_USE_ITEM_MAX_NUM_CS]		= HandlerItem(&MessageHandler::OnUseItemMax, sizeof(Protocol::CSUseItemMaxNum));

	m_msg_handler_list[Protocol::MT_GET_RANDOM_ROLE_LIST_CS]	= HandlerItem(&MessageHandler::OnGetRandomRoleList, sizeof(Protocol::CSGetRandomRoleList));
	m_msg_handler_list[Protocol::MT_EQUIPMENT_INFO_REQ_CS]		= HandlerItem(&MessageHandler::OnGetEequipmentInfo, sizeof(Protocol::CSEquipmentInfoReq));
	m_msg_handler_list[Protocol::MT_TAKE_OFF_EQUIPMENT_CS]		= HandlerItem(&MessageHandler::OnTakeOffEquipment, sizeof(Protocol::CSTakeOffEquipment));
	m_msg_handler_list[Protocol::MT_ITEM_COMPOSE_CS]			= HandlerItem(&MessageHandler::OnItemCompose, sizeof(Protocol::CSItemCompose));
	m_msg_handler_list[Protocol::MT_BLUEPRINT_COMPOSE_CS]		= HandlerItem(&MessageHandler::OnBluePrintCompose, sizeof(Protocol::CSBluePrintCompose));
	m_msg_handler_list[Protocol::MT_TAKE_OUT_ALL_ITEM]			= HandlerItem(&MessageHandler::OnTakeOutAllItem, sizeof(Protocol::CSTakeOutAllItem));

	m_msg_handler_list[Protocol::MT_EQUIP_STRENGTHEN_REQ_CS]	= HandlerItem(&MessageHandler::OnEquipStrengthen, sizeof(Protocol::CSEquipStrengthen));
	m_msg_handler_list[Protocol::MT_EQUIP_UP_STAR_CS]			= HandlerItem(&MessageHandler::OnEquipUpStar, sizeof(Protocol::CSEquipUpStar));
	m_msg_handler_list[Protocol::MT_EQUIP_UPLEVEL_REQ_CS]		= HandlerItem(&MessageHandler::OnEquipUpLevel, sizeof(Protocol::CSEquipUpLevel));
	m_msg_handler_list[Protocol::MT_EQUIP_UPQUALITY_REQ_CS]		= HandlerItem(&MessageHandler::OnEquipUpQuality, sizeof(Protocol::CSEquipUpQuality));
	m_msg_handler_list[Protocol::MT_EQUIP_INHERIT_REQ_CS]		= HandlerItem(&MessageHandler::OnEquipInherit, sizeof(Protocol::CSEquipInherit));
	m_msg_handler_list[Protocol::MT_EQUIP_COMPOUND_CS]			= HandlerItem(&MessageHandler::OnEquipCompound, sizeof(Protocol::CSEquipCompound));
	m_msg_handler_list[Protocol::MT_EQUIP_UP_ETERNITY_CS]		= HandlerItem(&MessageHandler::OnEquipUpEternity, sizeof(Protocol::CSEquipUpEternity));
	m_msg_handler_list[Protocol::MT_EQUIP_USE_ETERNITY_LEVEL_CS]= HandlerItem(&MessageHandler::OnEquipUseEternityLevel, sizeof(Protocol::CSEquipUseEternityLevel));
	m_msg_handler_list[Protocol::MT_EQUIP_SHENZHU_REQ_CS]		= HandlerItem(&MessageHandler::OnEquipRefine, sizeof(Protocol::CSEquipShenZhu));
	m_msg_handler_list[Protocol::MT_STONE_UPLVL_CS]				= HandlerItem(&MessageHandler::OnStoneUpLevel, sizeof(Protocol::CSStoneUplevel));
	m_msg_handler_list[Protocol::MT_DUANZAO_SUIT_REQ_CS]		= HandlerItem(&MessageHandler::OnDuanzaoSuitReq, sizeof(Protocol::CSDuanzaoSuitReq));
	m_msg_handler_list[Protocol::MT_STONE_INFO_REQ_CS]			= HandlerItem(&MessageHandler::OnReqStoneInfo,sizeof(Protocol::CSReqStoneInfo));
	m_msg_handler_list[Protocol::MT_STONE_INLAY_CS]				= HandlerItem(&MessageHandler::OnStoneInlay,sizeof(Protocol::CSStoneInlay));
	m_msg_handler_list[Protocol::MT_WASH_CS]					= HandlerItem(&MessageHandler::OnWash, sizeof(Protocol::CSWash));
	m_msg_handler_list[Protocol::MT_ZHIBAO_UPLEVEL_CS]			= HandlerItem(&MessageHandler::OnZhibaoUplevel,sizeof(Protocol::CSZhiBaoUplevel));
	m_msg_handler_list[Protocol::MT_ZHIBAO_USEIMAGE_CS]			= HandlerItem(&MessageHandler::OnZhibaoUseImage,sizeof(Protocol::CSZhiBaoUseImage));
	m_msg_handler_list[Protocol::MT_ZHIBAO_HUANHUA_CS]			= HandlerItem(&MessageHandler::OnZhibaoHuanhua, sizeof(Protocol::CSZhiBaoHuanhua));

	m_msg_handler_list[Protocol::MT_SHENBING_REQ_CS]= HandlerItem(&MessageHandler::OnShenbingInfoReq, sizeof(Protocol::CSShenbingReq));
	m_msg_handler_list[Protocol::MT_SHENBING_COMPOSE_CS]= HandlerItem(&MessageHandler::OnShenbingComposeReq, sizeof(Protocol::CSShenbingCompose));
	m_msg_handler_list[Protocol::MT_SHENBING_OP_CS]= HandlerItem(&MessageHandler::OnShenbingOPReq, sizeof(Protocol::CSShenbingOP));
	m_msg_handler_list[Protocol::MT_SHENBING_UP_LEVEL_CS]		= HandlerItem(&MessageHandler::OnShenbingUpLevelReq, sizeof(Protocol::CSShenBingUpLevel));
	m_msg_handler_list[Protocol::MT_EQUIP_FULING_CS]= HandlerItem(&MessageHandler::OnFulingReq, sizeof(Protocol::CSEquipFuling));
	m_msg_handler_list[Protocol::MT_EQUIP_CROSS_EQUIP_OPERA_CS]= HandlerItem(&MessageHandler::OnEquipCrossEquipOpera, sizeof(Protocol::CSEquipCrossEquipOpera));

	m_msg_handler_list[Protocol::MT_ROLE_CHANGE_CAMP_CS]		= HandlerItem(&MessageHandler::OnRoleChangeCamp, sizeof(Protocol::CSRoleChangeCamp));
	m_msg_handler_list[Protocol::MT_CHANGE_PROF_REQ_CS]			= HandlerItem(&MessageHandler::OnRoleChangeProf, sizeof(Protocol::CSChangeProfReq));
	m_msg_handler_list[Protocol::MT_ROLE_ZHUAN_SHENG]			= HandlerItem(&MessageHandler::OnRoleZhuanShen, sizeof(Protocol::CSRoleZhuanSheng));
	m_msg_handler_list[Protocol::MT_TRANSFORM_EXP_REQ_CS]			= HandlerItem(&MessageHandler::OnRoleTransformExp, sizeof(Protocol::CSTransformExpToBottle));

	m_msg_handler_list[Protocol::MT_MILLIONAIRE_INFO_CS]		= HandlerItem(&MessageHandler::OnGetMillionaireInfo, sizeof(Protocol::CSMillionaireInfoReq));

	m_msg_handler_list[Protocol::MT_GET_ROLE_SPACE_MSG_INFO_CS]		= HandlerItem(&MessageHandler::OnGetRoleSpaceMsgInfo, sizeof(Protocol::CSGetRoleSpaceMsgInfo));
	m_msg_handler_list[Protocol::MT_OPER_SPACE_CS]			= HandlerItem(&MessageHandler::OnOperSpace, sizeof(Protocol::CSSpaceOper));
	m_msg_handler_list[Protocol::MT_REMOVE_RECORD_CS]			= HandlerItem(&MessageHandler::OnRemoveRecord, sizeof(Protocol::CSSpaceRemoveRecord));

	m_msg_handler_list[Protocol::MT_ROLE_LIFESKILL_OPERA_CS]			= HandlerItem(&MessageHandler::OnLifeSkillOpera, sizeof(Protocol::CSLifeSkillOpera));

	m_msg_handler_list[Protocol::MT_KNAPSACKSTORAGE_EXTEND_GRID_NUM_CS]		= HandlerItem(&MessageHandler::OnKnapsackStorageExtendNum, sizeof(Protocol::CSKnapsackStorageExtendGridNum));

	m_msg_handler_list[Protocol::MT_TASK_LIST_REQ_CS]			= HandlerItem(&MessageHandler::OnTaskListReq, sizeof(Protocol::CSTaskListReq));
	m_msg_handler_list[Protocol::MT_TASK_GIVEUP_CS]				= HandlerItem(&MessageHandler::OnTaskGiveup, sizeof(Protocol::CSTaskGiveup));
	m_msg_handler_list[Protocol::MT_FLY_BY_SHOE_CS]				= HandlerItem(&MessageHandler::OnFlyByShoe, sizeof(Protocol::CSFlyByShoe));
	m_msg_handler_list[Protocol::MT_FLY_SYNC_JUMP_CS]			= HandlerItem(&MessageHandler::OnSyncJump, sizeof(Protocol::CSSyncJump));
	m_msg_handler_list[Protocol::MT_TASK_TALK_TO_NPC]			= HandlerItem(&MessageHandler::OnTaskTalkToNpc, sizeof(Protocol::CSTaskTalkToNpc));
	m_msg_handler_list[Protocol::MT_TASK_ACCEPT_CS]				= HandlerItem(&MessageHandler::OnTaskAccept, sizeof(Protocol::CSTaskAccept));
	m_msg_handler_list[Protocol::MT_TASK_COMMIT_CS]				= HandlerItem(&MessageHandler::OnTaskCommit, sizeof(Protocol::CSTaskCommit));

	m_msg_handler_list[Protocol::MT_CISHA_TASK_OPERA_CS]		= HandlerItem(&MessageHandler::OnCishaTaskOpera, sizeof(Protocol::CSCishaTaskOpera));
	m_msg_handler_list[Protocol::MT_PAOHUAN_TASK_ROLL_REQ_CS]	= HandlerItem(&MessageHandler::OnPaoHuanRollReq, sizeof(Protocol::CSPaoHuanRollReq));

	m_msg_handler_list[Protocol::MT_CS_SEEK_ROLE_WHERE_CS]	= HandlerItem(&MessageHandler::OnSeekRoleWhereReq, sizeof(Protocol::CSSeekRoleWhere));
	m_msg_handler_list[Protocol::MT_CS_SHENZHUANG_OPER_CS]  = HandlerItem(&MessageHandler::OnShenZhuangOperateReq, sizeof(Protocol::CSShenzhaungOper));
	m_msg_handler_list[Protocol::MT_CSA_EQUIP_OPERA_CS] = HandlerItem(&MessageHandler::OnCSAEquipOperateReq, sizeof(Protocol::CSShenzhaungOper));
	m_msg_handler_list[Protocol::MT_CS_CAMPEQUIP_OPERATE_CS]	= HandlerItem(&MessageHandler::OnCampEquipOperateReq, sizeof(Protocol::CSCampEquipOperate));
	m_msg_handler_list[Protocol::MT_CS_CAMP_NORMALDUOBAO_OPERATE_CS]	= HandlerItem(&MessageHandler::OnCampNormalDuobaoReq, sizeof(Protocol::CSCampNormalDuobaoOperate));

	m_msg_handler_list[Protocol::MT_GOLD_VIP_OPERA_REQ_CS]		= HandlerItem(&MessageHandler::OnGoldVipOperaReq, sizeof(Protocol::CSGoldVipOperaReq));

	m_msg_handler_list[Protocol::MT_HOTKEY_INFO_REQ_CS]			= HandlerItem(&MessageHandler::OnHotkeyInfoReq, sizeof(Protocol::CSHotkeyInfoReq));
	m_msg_handler_list[Protocol::MT_CHANGE_HOTKEY_CS]			= HandlerItem(&MessageHandler::OnChangeHotkey, sizeof(Protocol::CSChangeHotkey));
	m_msg_handler_list[Protocol::MT_ACCOUNT_GOLD_REQ_CS]		= HandlerItem(&MessageHandler::OnAccountGoldReq, sizeof(Protocol::CSAccountGoldReq));
	m_msg_handler_list[Protocol::MT_ACCOUNT_GET_GOLD_REQ_CS]	= HandlerItem(&MessageHandler::OnAccountGetGoldReq, sizeof(Protocol::CSAccountGetGoldReq));

	m_msg_handler_list[Protocol::MT_FRIEND_LIST_REQ_CS]			= HandlerItem(&MessageHandler::OnFriendListReq, sizeof(Protocol::CSFriendInfoReq));
	m_msg_handler_list[Protocol::MT_ADD_FRIEND_REQ_CS]			= HandlerItem(&MessageHandler::OnAddFriendReq, sizeof(Protocol::CSAddFriendReq));
	m_msg_handler_list[Protocol::MT_ADD_FRIEND_RET_CS]			= HandlerItem(&MessageHandler::OnAddFriendRet, sizeof(Protocol::CSAddFriendRet));
	m_msg_handler_list[Protocol::MT_FRIEND_DELETE_CS]			= HandlerItem(&MessageHandler::OnDeleteFriendReq, sizeof(Protocol::CSDeleteFriend));
	m_msg_handler_list[Protocol::MT_ENEMY_DELETE_CS]			= HandlerItem(&MessageHandler::OnEnemyDeleteReq, sizeof(Protocol::CSEnemyDelete));
	m_msg_handler_list[Protocol::MT_GIVE_FLOWER_CS]				= HandlerItem(&MessageHandler::OnGiveFlower, sizeof(Protocol::CSGiveFlower));
	m_msg_handler_list[Protocol::MT_GIVE_FLOWER_KISS_REQ_CS]	= HandlerItem(&MessageHandler::OnGiveFlowerKissReq, sizeof(Protocol::CSGiveFlowerKissReq));
	m_msg_handler_list[Protocol::MT_SONG_HUA_INFO_REQ]			= HandlerItem(&MessageHandler::OnSongHuaInfoReq, sizeof(Protocol::CSSoneHuaInfoReq));

	m_msg_handler_list[Protocol::MT_ADD_BLACK_CS]				= HandlerItem(&MessageHandler::OnAddBlackReq, sizeof(Protocol::CSAddBlackReq));
	m_msg_handler_list[Protocol::MT_DELETE_BLACK_CS]			= HandlerItem(&MessageHandler::OnDeleteBlackReq, sizeof(Protocol::CSDeleteBlackReq));

	m_msg_handler_list[Protocol::MT_FRIEND_BLESS_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnFriendBlessOperaReq, sizeof(Protocol::CSFriendBlessOperaReq));
	m_msg_handler_list[Protocol::MT_OFFLINE_FRIEND_AUTO_DEC_FALG] = HandlerItem(&MessageHandler::OnOfflineFriendAutoDecFlag, sizeof(Protocol::CSOfflineFriendAutoDecFlag));

	m_msg_handler_list[Protocol::MT_GM_COMMAND_CS]				= HandlerItem(&MessageHandler::OnGmCommand, sizeof(Protocol::CSGMCommand));
	m_msg_handler_list[Protocol::MT_GM_TESTSPEED_CS]			= HandlerItem(&MessageHandler::OnTestSpeed, sizeof(Protocol::CSTestSpeed));

	m_msg_handler_list[Protocol::MT_DISCOUNT_BUY_GET_INFO_CS]	= HandlerItem(&MessageHandler::OnDiscountBuyGetInfo, sizeof(Protocol::CSDiscountBuyGetInfo));
	m_msg_handler_list[Protocol::MT_DISCOUNT_BUY_REQ_BUY_CS]	= HandlerItem(&MessageHandler::OnDiscountBuyReqBuy, sizeof(Protocol::CSDiscountBuyReqBuy));

	m_msg_handler_list[Protocol::MT_ZERO_GIFT_OPERATE_REQ_CS]	= HandlerItem(&MessageHandler::OnZeroGiftOperate, sizeof(Protocol::CSZeroGiftOperate));
	m_msg_handler_list[Protocol::MT_ADVANCE_NOTIC_OPERATE_CS] = HandlerItem(&MessageHandler::OnAdvanceNoticeOperate, sizeof(Protocol::CSAdvanceNoitceOperate));
	m_msg_handler_list[Protocol::MT_GOLD_TURNTABLE_OPERATE_REQ_CS]	= HandlerItem(&MessageHandler::OnGoldTurntableOperateReq, sizeof(Protocol::CSGoldTurntableOperateReq));

	m_msg_handler_list[Protocol::MT_MAIL_SEND_CS]               = HandlerItem(&MessageHandler::OnSendMail, sizeof(Protocol::CSMailSend));
	m_msg_handler_list[Protocol::MT_MAIL_DELETE_CS]             = HandlerItem(&MessageHandler::OnDeleteMail, sizeof(Protocol::CSMailDelete));
	m_msg_handler_list[Protocol::MT_MAIL_GETLIST_CS]            = HandlerItem(&MessageHandler::OnGetMailList, sizeof(Protocol::CSMailGetList));
	m_msg_handler_list[Protocol::MT_MAIL_READ_CS]				= HandlerItem(&MessageHandler::OnReadMail, sizeof(Protocol::CSMailRead));
	m_msg_handler_list[Protocol::MT_MAIL_ATTACHMENT_CS]			= HandlerItem(&MessageHandler::OnFetchMailAttachment, sizeof(Protocol::CSMailFetchAttachment));
	m_msg_handler_list[Protocol::MT_MAIL_CLEAN_CS]				= HandlerItem(&MessageHandler::OnCleanMail, sizeof(Protocol::CSMailClean));
	m_msg_handler_list[Protocol::MT_MAIL_ONE_KEY_FETCH_ATTACHMENT]	= HandlerItem(&MessageHandler::OnOneKeyFetchMailAttachment, sizeof(Protocol::CSMailOneKeyFetchAttachment));

	m_msg_handler_list[Protocol::MT_PUBLICSALE_SEARCH_CS]		= HandlerItem(&MessageHandler::OnPublicSearch, sizeof(Protocol::CSPublicSaleSearch));
	m_msg_handler_list[Protocol::MT_ADD_PUBLICSALE_ITEM_CS]		= HandlerItem(&MessageHandler::OnAddPublicSaleItem, sizeof(Protocol::CSAddPublicSaleItem));
	m_msg_handler_list[Protocol::MT_REMOVE_PUBLICSALE_ITEM_CS]	= HandlerItem(&MessageHandler::OnRemovePublicSaleItem, sizeof(Protocol::CSRemovePublicSaleItem));
	m_msg_handler_list[Protocol::MT_BUY_PUBLICSALE_BUY_CS]		= HandlerItem(&MessageHandler::OnBuyPublicSaleItem, sizeof(Protocol::CSBuyPublicSaleItem));
	m_msg_handler_list[Protocol::MT_PUBLICSALE_GET_ITEM_LIST_CS]= HandlerItem(&MessageHandler::OnPublicSaleGetUserItemList, sizeof(Protocol::CSPublicSaleGetUserItemList));
	m_msg_handler_list[Protocol::MT_PBULICSALE_SEND_ITEM_INFO_TO_WORLD_CS]= HandlerItem(&MessageHandler::OnPublicSaleSendItemInfoToWorld, sizeof(Protocol::CSPublicSaleSendItemInfoToWorld));
	m_msg_handler_list[Protocol::MT_PBULICSALE_TYPE_COUNT_REQ_CS]	= HandlerItem(&MessageHandler::OnPublicSaleTypeCountReq, sizeof(Protocol::CSPublicSaleTypeCountReq));

	m_msg_handler_list[Protocol::MT_CAMP_INFO_CS]				= HandlerItem(&MessageHandler::OnGetCampInfo, sizeof(Protocol::CSGetCampInfo));
	m_msg_handler_list[Protocol::MT_CHOOSE_CAMP_CS]				= HandlerItem(&MessageHandler::OnChooseCamp, sizeof(Protocol::CSRoleChooseCamp));
	m_msg_handler_list[Protocol::MT_CHOOSE_PROF_CS]				= HandlerItem(&MessageHandler::OnChooseProf, sizeof(Protocol::CSRoleChooseProf));

	m_msg_handler_list[Protocol::MT_CREATE_GUILD_CS]			= HandlerItem(&MessageHandler::OnCreateGuild, sizeof(Protocol::CSCreateGuild));
	m_msg_handler_list[Protocol::MT_DISMISS_GUILD_CS]			= HandlerItem(&MessageHandler::OnDimissGuild, sizeof(Protocol::CSDismissGuild));
	m_msg_handler_list[Protocol::MT_APPLY_FOR_JOIN_GUILD_CS]	= HandlerItem(&MessageHandler::OnApplyForJoinGuild, sizeof(Protocol::CSApplyForJoinGuild));
	m_msg_handler_list[Protocol::MT_APPLY_FOR_JOIN_GUILD_ACK_CS]= HandlerItem(&MessageHandler::OnApplyForJoinGuildAck, sizeof(Protocol::CSApplyForJoinGuildAck));
	m_msg_handler_list[Protocol::MT_QUIT_GUILD_CS]				= HandlerItem(&MessageHandler::OnQuitGuild, sizeof(Protocol::CSQuitGuild));
	m_msg_handler_list[Protocol::MT_INVITE_GUILD_CS]			= HandlerItem(&MessageHandler::OnInviteGuild, sizeof(Protocol::CSInviteGuild));
	m_msg_handler_list[Protocol::MT_INVITE_GUILD_ACK_CS]		= HandlerItem(&MessageHandler::OnInviteGuildAck, sizeof(Protocol::CSInviteGuildAck));
	m_msg_handler_list[Protocol::MT_KICKOUT_GUILD_CS]			= HandlerItem(&MessageHandler::OnKickoutGuild, sizeof(Protocol::CSKickoutGuild));
	m_msg_handler_list[Protocol::MT_APPOINT_GUILD_CS]			= HandlerItem(&MessageHandler::OnAppointGuild, sizeof(Protocol::CSAppointGuild));
	m_msg_handler_list[Protocol::MT_LEAVE_GUILD_POST_CS]		= HandlerItem(&MessageHandler::OnLeaveGuildPost, sizeof(Protocol::CSLeaveGuildPost));
	m_msg_handler_list[Protocol::MT_CHANGE_GUILD_NOTICE_CS]		= HandlerItem(&MessageHandler::OnGuildChangeNotice, sizeof(Protocol::CSGuildChangeNotice));
	m_msg_handler_list[Protocol::MT_GUILD_MAIL_ALL_CS]			= HandlerItem(&MessageHandler::OnGuildMailAll, sizeof(Protocol::CSGuildMailAll));
	m_msg_handler_list[Protocol::MT_GET_GUILD_INFO_CS]			= HandlerItem(&MessageHandler::OnGetGuildInfo, sizeof(Protocol::CSGetGuildInfo));
	m_msg_handler_list[Protocol::MT_GUILD_DELATE_CS]			= HandlerItem(&MessageHandler::OnGuildDelate, sizeof(Protocol::CSGuildDelate));
	m_msg_handler_list[Protocol::MT_GUILD_BACK_TO_STATION_CS]	= HandlerItem(&MessageHandler::OnGuildBackToStation, sizeof(Protocol::CSGuildBackToStation));
	m_msg_handler_list[Protocol::MT_GUILD_XIANSHU_INFO_REQ_CS]	= HandlerItem(&MessageHandler::OnGuildXianshuInfoReq, sizeof(Protocol::CSGuildXianshuInfoReq));
	m_msg_handler_list[Protocol::MT_GUILD_XIANSHU_UPLEVEL_CS]	= HandlerItem(&MessageHandler::OnGuildXianshuUplevelReq, sizeof(Protocol::CSGuildXianshuUplevelReq));

	m_msg_handler_list[Protocol::MT_MENTALITY_CLEAR_TRAIN_CD_CS]		= HandlerItem(&MessageHandler::OnClearTrainMentalityCD, sizeof(Protocol::CSMentalityClearTrainCD));
	m_msg_handler_list[Protocol::MT_MENTALITY_TRAIN_CS]			= HandlerItem(&MessageHandler::OnTrainMentality, sizeof(Protocol::CSTrainMentality));
	m_msg_handler_list[Protocol::MT_MENTALITY_UPGRADE_GENGU_CS] = HandlerItem(&MessageHandler::OnUpgradeMentalityGengu, sizeof(Protocol::CSUpgradeMentalityGengu));
	m_msg_handler_list[Protocol::MT_MENTALITY_SPEED_UP_TRAIN_CS]= HandlerItem(&MessageHandler::OnSpeedUpMentalityTrain, sizeof(Protocol::CSSpeedUpMentalityTrain));
	m_msg_handler_list[Protocol::MT_MENTALITY_YJTS_CS]			= HandlerItem(&MessageHandler::OnMentalityYijianTisheng, sizeof(Protocol::CSMentalityYijianTisheng));
	m_msg_handler_list[Protocol::MT_MENTALITY_UPLEVEL_WUXING_CS] = HandlerItem(&MessageHandler::OnMentalityUplevelWuxing, sizeof(Protocol::CSMentalityUplevelWuxing));

	m_msg_handler_list[Protocol::MT_SUBLOCK_CREATE_PW_CS]		= HandlerItem(&MessageHandler::OnSublockCreatePw, sizeof(Protocol::CSSubLockCreatePw));
	m_msg_handler_list[Protocol::MT_SUBLOCK_CLEAR_PW_CS]		= HandlerItem(&MessageHandler::OnSublockClearPw, sizeof(Protocol::CSSubLockDeletePw));
	m_msg_handler_list[Protocol::MT_SUBLOCK_LOCK_CS]			= HandlerItem(&MessageHandler::OnSublockLock, sizeof(Protocol::CSSubLockLock));
	m_msg_handler_list[Protocol::MT_SUBLOCK_UNLOCK_CS]			= HandlerItem(&MessageHandler::OnSublockUnlock, sizeof(Protocol::CSSubLockUnlock));

	m_msg_handler_list[Protocol::MT_FETCH_VIP_LEVEL_REWARD]		= HandlerItem(&MessageHandler::OnFetchVipLevelReward, sizeof(Protocol::CSFetchVipLevelReward));
	m_msg_handler_list[Protocol::MT_FETCH_VIP_WEEK_REWARD]		= HandlerItem(&MessageHandler::OnFetchVipWeekReward, sizeof(Protocol::CSFetchVipWeekReward));
	m_msg_handler_list[Protocol::MT_FETCH_TIME_LIMIT_VIP_CS]	= HandlerItem(&MessageHandler::OnFetchTimeLimitVip, sizeof(Protocol::CSFetchTimeLimitVip));
	m_msg_handler_list[Protocol::MT_QIFU_REQ_CS]				= HandlerItem(&MessageHandler::OnQiFuReq, sizeof(Protocol::CSQiFuReq));
	m_msg_handler_list[Protocol::MT_FETCH_QIFU_REWARD_CS]		= HandlerItem(&MessageHandler::OnFetchQiFuReward, sizeof(Protocol::CSFetchQiFuReward));

	m_msg_handler_list[Protocol::MT_GET_ROLE_CAPABILITY_CS]		= HandlerItem(&MessageHandler::OnGetRoleCapability, sizeof(Protocol::CSGetRoleCapability));

	m_msg_handler_list[Protocol::MT_SET_MOVE_MODE_CS]			= HandlerItem(&MessageHandler::OnSetMoveMode, sizeof(Protocol::CSSetMoveMode));

	m_msg_handler_list[Protocol::MT_CHEST_SHOP_BUY_ITEM_CS]		= HandlerItem(&MessageHandler::OnChestShopBuyItem, sizeof(Protocol::CSBuyChestShopItem));
	m_msg_handler_list[Protocol::MT_CHEST_SHOP_FETCH_ITEM_CS]	= HandlerItem(&MessageHandler::OnChestShopFetchItem, sizeof(Protocol::CSFetchChestShopItem));
	m_msg_handler_list[Protocol::MT_CHEST_SHOP_GET_SELF_ITEM_LIST_CS]	= HandlerItem(&MessageHandler::OnChestShopGetSelfItemList, sizeof(Protocol::CSGetSelfChestShopItemList));
	m_msg_handler_list[Protocol::MT_CHEST_SHOP_GET_FREE_INFO]	= HandlerItem(&MessageHandler::OnChestShopGetFreeInfo, sizeof(Protocol::CSChestShopGetFreeInfo));
	m_msg_handler_list[Protocol::MT_CHEST_SHOP_AUTO_RECYCLE_CS]	= HandlerItem(&MessageHandler::OnChestShopAutoRecycle, sizeof(Protocol::CSChestShopAutoRecycle));
	m_msg_handler_list[Protocol::MT_RARE_CHEST_SHOP_BUY_CS] = HandlerItem(&MessageHandler::OnRareChestShopBuyItem, sizeof(Protocol::CSRareChestShopReq));
	m_msg_handler_list[Protocol::MT_CHEST_SHOP_GET_RECORD_LIST_CS] = HandlerItem(&MessageHandler::OnChestShopRecordListReq, sizeof(Protocol::CSChestShopRecordList));

	m_msg_handler_list[Protocol::MT_MOVE_CHESS_RESET_REQ_CS] = HandlerItem(&MessageHandler::OnMoveChessResetReq, sizeof(Protocol::CSMoveChessResetReq));
	m_msg_handler_list[Protocol::MT_MOVE_CHESS_SHAKE_REQ_CS] = HandlerItem(&MessageHandler::OnMoveChessShakeReq, sizeof(Protocol::CSMoveChessShakeReq));
	m_msg_handler_list[Protocol::MT_MOVE_CHESS_FREE_INFO_REQ_CS] = HandlerItem(&MessageHandler::OnMoveChessFreeInfoReq, sizeof(Protocol::CSMoveChessFreeInfo));

	m_msg_handler_list[Protocol::MT_SHOP_BUY_ITEM_CS]			= HandlerItem(&MessageHandler::OnShopBuyItem, sizeof(Protocol::CSShopBuy));
	m_msg_handler_list[Protocol::MT_MYSTERIOUSSHOP_OPERATE_CS]	= HandlerItem(&MessageHandler::OnMysteriousShopOperate, sizeof(Protocol::CSMysteriosshopOperate));
	m_msg_handler_list[Protocol::MT_MYSTERIOUS_SHOP_BUY_OPERATE_CS]	= HandlerItem(&MessageHandler::OnMysteriousShopInMallOperate, sizeof(Protocol::CSMysteriosshopinMallOperate));

	m_msg_handler_list[Protocol::MT_GET_ALL_TITLE_CS]		= HandlerItem(&MessageHandler::OnGetTitleList, sizeof(Protocol::CSGetTitleList));
	m_msg_handler_list[Protocol::MT_USE_TITLE_CS]	= HandlerItem(&MessageHandler::OnUseTitle, sizeof(Protocol::CSUseTitle));
	m_msg_handler_list[Protocol::MT_TITLE_UPGRADE_CS]	= HandlerItem(&MessageHandler::OnUpgradeTitle, sizeof(Protocol::CSUpgradeTitle));

	m_msg_handler_list[Protocol::MT_LOUD_SPEAKER_CS]			= HandlerItem(&MessageHandler::OnSpeakerTalk, sizeof(Protocol::CSSpeaker));
	m_msg_handler_list[Protocol::MT_GUILD_RESET_NAME_CS]		= HandlerItem(&MessageHandler::OnGuildResetName, sizeof(Protocol::CSGuildResetName));
	m_msg_handler_list[Protocol::MT_GUILD_APPLYFOR_SETUP_CS]	= HandlerItem(&MessageHandler::OnGuildApplyforSetup, sizeof(Protocol::CSApplyforSetup));
	m_msg_handler_list[Protocol::MT_GUILD_ADDEXP_CS]			= HandlerItem(&MessageHandler::OnGuildAddGuildExp, sizeof(Protocol::CSAddGuildExp));
	m_msg_handler_list[Protocol::MT_GUILD_UP_LEVEL_CS]			= HandlerItem(&MessageHandler::OnGuildUpLevel, sizeof(Protocol::CSGuildUpLevel));
	m_msg_handler_list[Protocol::MT_GUILD_GET_BUFF_CS]			= HandlerItem(&MessageHandler::OnGuildGetBuff, sizeof(Protocol::CSGuildGetBuff));
	m_msg_handler_list[Protocol::MT_GUILD_EXCHANGE_CS]			= HandlerItem(&MessageHandler::OnGuildExchange, sizeof(Protocol::CSGuildExchange));
	m_msg_handler_list[Protocol::MT_GUILD_SET_AUTO_KICKOUT_SETUP_CS] = HandlerItem(&MessageHandler::OnGuildSetAutoKickoutSetup, sizeof(Protocol::CSGuildSetAutoKickoutSetup));
	m_msg_handler_list[Protocol::MT_GUILD_CALL_IN_CS]			= HandlerItem(&MessageHandler::OnGuildCallIn, sizeof(Protocol::CSGuildCallIn));
	m_msg_handler_list[Protocol::MT_GUILD_CHECK_CAN_DELATE_CS]	= HandlerItem(&MessageHandler::OnGuildCheckCanDelate, sizeof(Protocol::CSGuildCheckCanDelate));
	m_msg_handler_list[Protocol::MT_GUILD_FB_START_REQ_CS]	= HandlerItem(&MessageHandler::OnGuildFbStartReq, sizeof(Protocol::CSGuildFbStartReq));
	m_msg_handler_list[Protocol::MT_GUILD_BONFIRE_START_REQ_CS]	= HandlerItem(&MessageHandler::OnGuildBonfireStartReq, sizeof(Protocol::CSGuildBonfireStartReq));
	m_msg_handler_list[Protocol::MT_GUILD_FB_ENTER_REQ_CS]		= HandlerItem(&MessageHandler::OnGuildFbEnterReq, sizeof(Protocol::CSGuildFbEnterReq));
	m_msg_handler_list[Protocol::MT_GUILD_EXTEND_MEMBER_CS]		= HandlerItem(&MessageHandler::OnGuildExtendMemberReq, sizeof(Protocol::CSGuildExtendMemberReq));
	m_msg_handler_list[Protocol::MT_GUILD_BONFIRE_GOTO_REQ_CS]	= HandlerItem(&MessageHandler::OnGotoBonfirePos, sizeof(Protocol::CSGuildBonfireGotoReq));
	m_msg_handler_list[Protocol::MT_GUILD_BONFIRE_ADDMUCAI_REQ_CS] = HandlerItem(&MessageHandler::OnAddBonfireMucai, sizeof(Protocol::CSGuildBonfireAddMucaiReq));
	m_msg_handler_list[Protocol::MT_GUILD_STORGE_REQ_CS]		= HandlerItem(&MessageHandler::OnGuildStorgeOperate, sizeof(Protocol::CSGuildStorgeOperate));
	m_msg_handler_list[Protocol::MT_GUILD_STORGE_ONE_KEY_REQ_CS]= HandlerItem(&MessageHandler::OnGuildStorgeOneKeyOperate, sizeof(Protocol::CSGuildStorgeOneKeyOperate));
	m_msg_handler_list[Protocol::MT_GUILD_SET_AUTO_CLEAR_REQ] = HandlerItem(&MessageHandler::OnGuildAutoClearReq, sizeof(Protocol::CSGuildSetAutoClearReq));
	m_msg_handler_list[Protocol::MT_GUILD_ZHUFU_LUCKY_REQ_CS]	= HandlerItem(&MessageHandler::OnGuildZhufuMemberLuckyReq, sizeof(Protocol::CSGuildZhuLuckyReq));
	m_msg_handler_list[Protocol::MT_GUILD_GET_GUILD_ALL_LUCKY_REQ_CS] = HandlerItem(&MessageHandler::OnGetAllMemberLuckyInfoReq, sizeof(Protocol::CSGetAllGuildMemberLuckyInfo));
	m_msg_handler_list[Protocol::MT_GUILD_INVITE_ZHUFU_LUCKY_CS]= HandlerItem(&MessageHandler::OnInviteZhuFuLuckyReq, sizeof(Protocol::CSInviteLuckyZhufu));
	m_msg_handler_list[Protocol::MT_GUILD_CALL_BEAST_CS]		= HandlerItem(&MessageHandler::OnGuildCallBeastReq, sizeof(Protocol::CSGuildCallBeastReq));
	m_msg_handler_list[Protocol::MT_GUILD_ACTIVE_DEGREE_INFO_CS]= HandlerItem(&MessageHandler::OnGuildActiveDegreeReq, sizeof(Protocol::CSGuildActiveDegreeInfoReq));
	m_msg_handler_list[Protocol::MT_GUILD_BOSS_CS]				= HandlerItem(&MessageHandler::OnGuildBossOperateReq, sizeof(Protocol::CSGuildBossOperate));
	m_msg_handler_list[Protocol::MT_GUILD_SKILL_UPLEVEL_CS]		= HandlerItem(&MessageHandler::OnGuildSkillUplevel, sizeof(Protocol::CSGuildSkillUplevel));
	m_msg_handler_list[Protocol::MT_GUILD_TOTEM_UP_LEVEL_CS]	= HandlerItem(&MessageHandler::OnGuildUpTotemLevel, sizeof(Protocol::CSGuildUpTotemLevel));
	m_msg_handler_list[Protocol::MT_GUILD_BOX_OPERATE_CS]		= HandlerItem(&MessageHandler::OnGuildBoxOperate, sizeof(Protocol::CSGuildBoxOperate));
	m_msg_handler_list[Protocol::MT_GUILD_COMMON_REQ_CS]		= HandlerItem(&MessageHandler::OnGuildCommonReq, sizeof(Protocol::CSGuildCommonReq));
	m_msg_handler_list[Protocol::MT_GUILD_FETCH_TERRITORY_REWARD_CS] = HandlerItem(&MessageHandler::OnGuildFetchTerritoryReward, sizeof(Protocol::CSGuildTerritoryWelfOperate));
	m_msg_handler_list[Protocol::MT_GUILD_FETCH_GUILD_BOSS_REDBAG] = HandlerItem(&MessageHandler::OnGuildFetchBossRedbag, sizeof(Protocol::CSFetchGuildBossRedbag));
	m_msg_handler_list[Protocol::MT_GUILD_RED_PAPER_REQ_LIST_INFO_CS] = HandlerItem(&MessageHandler::OnGuildRedPaperListInfoReq, sizeof(Protocol::CSGuildRedPaperListInfoReq));
	m_msg_handler_list[Protocol::MT_GUILD_ROLE_GUILD_PAPER_CREATE_CS] = HandlerItem(&MessageHandler::OnGuildRedPaperCreateReq, sizeof(Protocol::CSCreateGuildRedPaperReq));
	m_msg_handler_list[Protocol::MT_GUILD_ROLE_GUILD_PAPER_SINGAL_CHAT_CS] = HandlerItem(&MessageHandler::OnGuildRedPaperSingalChatReq, sizeof(Protocol::CSSingleChatRedPaperRole));
	m_msg_handler_list[Protocol::MT_GUILD_FETCH_GUILD_PAO_SAIZI] = HandlerItem(&MessageHandler::OnGuildPaoSaizi, sizeof(Protocol::CSGulidPaoSaizi));
	m_msg_handler_list[Protocol::MT_GUILD_FETCH_GUILD_SAIZI_INFO_REQ] = HandlerItem(&MessageHandler::OnReqGuildPaoSaizi, sizeof(Protocol::CSReqGulidSaiziInfo));
	m_msg_handler_list[Protocol::MT_GUILD_MAZE_OPERATE_CS]		= HandlerItem(&MessageHandler::OnGuildMazeOperate, sizeof(Protocol::CSGuildMazeOperate));
	m_msg_handler_list[Protocol::MT_GUILD_SIGNIN_REQ_CS] = HandlerItem(&MessageHandler::OnGuildSigninReq, sizeof(Protocol::CSGuildSinginReq));
	m_msg_handler_list[Protocol::MT_GUILD_CHANGE_AVATAR_CS] = HandlerItem(&MessageHandler::OnGuildChangeAvatar, sizeof(Protocol::CSGuildChangeAvatar));
	m_msg_handler_list[Protocol::MT_GUILD_BIAOCHE_OPERA_CS] = HandlerItem(&MessageHandler::OnGuildBiaocheOpera, sizeof(Protocol::CSBiaoCheOpera));
	m_msg_handler_list[Protocol::MT_GUILD_TIANCI_TONGBI_REQ_CS] = HandlerItem(&MessageHandler::OnGuildTianCiTongBiReq, sizeof(Protocol::CSGuildTianCiTongBiReq));
	m_msg_handler_list[Protocol::MT_GUILD_ENEMY_RANK_LIST_CS] = HandlerItem(&MessageHandler::OnGuildEnemyRankList, sizeof(Protocol::CSGuildEnemyRankList));

	m_msg_handler_list[Protocol::MT_WELFARE_GET_DAILY_INFO_CS]	= HandlerItem(&MessageHandler::OnGetDailyWelfareInfo, sizeof(Protocol::CSGetDailyLoginWelfareInfo));
	m_msg_handler_list[Protocol::MT_WELFARE_FETCH_SEVEN_DAY_LOGIN_REWARD_CS] = HandlerItem(&MessageHandler::OnFetchSevenDayLoginReward, sizeof(Protocol::CSFetchSevenDayLoginReward));
	m_msg_handler_list[Protocol::MT_GET_OFFLINE_EXP_CS]			= HandlerItem(&MessageHandler::OnGetOfflineExp, sizeof(Protocol::CSGetOfflineExp));
	m_msg_handler_list[Protocol::MT_WELFARE_GET_DAILY_FIND_CS]	= HandlerItem(&MessageHandler::OnGetDailyFindWelfare, sizeof(Protocol::CSGetDailyFindWelfare));
	m_msg_handler_list[Protocol::MT_WELFARE_SIGN_IN_REWARD_CS]	= HandlerItem(&MessageHandler::OnFetchSignInReward, sizeof(Protocol::CSWelfareSignInReward));
	m_msg_handler_list[Protocol::MT_WELFARE_ONLINE_REWARD_CS]	= HandlerItem(&MessageHandler::OnFetchOnlineReward, sizeof(Protocol::CSWelfareOnlineReward));
	m_msg_handler_list[Protocol::MT_WELFARE_SIGN_IN_FIND_BACK_CS] = HandlerItem(&MessageHandler::OnSignInFindBack, sizeof(Protocol::CSWelfareSignInFindBack));
	m_msg_handler_list[Protocol::MT_WELFARE_ACTIVITY_FIND_CS]	= HandlerItem(&MessageHandler::OnWelfareActivityFind, sizeof(Protocol::CSWelfareActivityFind));
	m_msg_handler_list[Protocol::MT_WELFARE_HAPPYTREE_REWARD_CS]= HandlerItem(&MessageHandler::OnFetchHappytreeReward, sizeof(Protocol::CSWelfareFetchHappytreeReward));
	m_msg_handler_list[Protocol::MT_WELFARE_CHONGJIHAOLI_REWARD_CS] = HandlerItem(&MessageHandler::OnFetchChongjihaoliReward, sizeof(Protocol::CSWelfareFetchChongjihaoliReward));

	m_msg_handler_list[Protocol::MT_ACTIVEDEFREE_FETCH_REWARD_CS]= HandlerItem(&MessageHandler::OnActiveFetchReward, sizeof(Protocol::CSActiveFetchReward));

	m_msg_handler_list[Protocol::MT_SAVE_HANGUP_SET_CS]			= HandlerItem(&MessageHandler::OnSaveHangupSet, sizeof(Protocol::CSSaveRoleHangupSet));
	m_msg_handler_list[Protocol::MT_SAVE_CUSTOM_SET_CS]			= HandlerItem(&MessageHandler::OnSaveCustomSet, sizeof(Protocol::CSSaveRoleCustomSet));
	m_msg_handler_list[Protocol::MT_ROLE_RESET_NAME_CS]			= HandlerItem(&MessageHandler::OnRoleResetName, sizeof(Protocol::CSRoleResetName));
	m_msg_handler_list[Protocol::MT_ROLE_SET_ZOMBIE_REQ_CS]		= HandlerItem(&MessageHandler::OnRoleSetZombieReq, sizeof(Protocol::CSRoleSetZombieReq));

	// m_msg_handler_list[Protocol::MT_DAILY_WELFARE_TIPS_CS]		= HandlerItem(&MessageHandler::OnDailyWelfareTipsReq, sizeof(Protocol::CSDailyWelfareTipsReq));
	// m_msg_handler_list[Protocol::MT_DAILY_BUY_COIN_GET_CS]		= HandlerItem(&MessageHandler::OnGetDaillyBuyCoin, sizeof(Protocol::CSGetDailyBuyCoin));

	m_msg_handler_list[Protocol::MT_EVALUATE_ROLE_CS]			= HandlerItem(&MessageHandler::OnEvaluateRole, sizeof(Protocol::CSEvaluateRole));

	m_msg_handler_list[Protocol::MT_CONVERT_SCORE_TO_ITEM_CS]	= HandlerItem(&MessageHandler::OnConvertScoreToItem, sizeof(Protocol::CSScoreToItemConvert));
	m_msg_handler_list[Protocol::MT_CONVERT_RECORD_GET_INFO_CS]	= HandlerItem(&MessageHandler::OnGetConvertRecordInfo, sizeof(Protocol::CSGetConvertRecordInfo));
	m_msg_handler_list[Protocol::MT_CONVERT_GET_SOCRE_INFO_CS]	= HandlerItem(&MessageHandler::OnGetSocreInfoReq, sizeof(Protocol::CSGetSocreInfoReq));

	m_msg_handler_list[Protocol::MT_CLIENT_SPECIAL_OPERATE_CS]	= HandlerItem(&MessageHandler::OnClientSpecialOperate, sizeof(Protocol::CSClientSpecialOperate));

	m_msg_handler_list[Protocol::MT_XIANNV_CALL_CS] = HandlerItem(&MessageHandler::OnCallXiannv, sizeof(Protocol::CSXiannvCall));
	m_msg_handler_list[Protocol::MT_XIANNV_RENAME_CS] = HandlerItem(&MessageHandler::OnXiannvRename, sizeof(Protocol::CSXiannvRename));
	m_msg_handler_list[Protocol::MT_XIANNV_ACTIVE_REQ_CS] = HandlerItem(&MessageHandler::OnXiannvActiveReq, sizeof(Protocol::CSXiannvActiveReq));
	m_msg_handler_list[Protocol::MT_XIANNV_ADD_ZIZHI_REQ_CS] = HandlerItem(&MessageHandler::OnXiannvAddZizhiReq, sizeof(Protocol::CSXiannvAddZizhiReq));
	m_msg_handler_list[Protocol::MT_XIANNV_ACTIVE_HUANHUA_REQ_CS] = HandlerItem(&MessageHandler::OnXiannvActiveHuanhuaReq, sizeof(Protocol::CSXiannvActiveHuanhua));
	m_msg_handler_list[Protocol::MT_XIANNV_CHANGEIMAGE_REQ_CS] = HandlerItem(&MessageHandler::OnXiannvChangeImageReq, sizeof(Protocol::CSXiannvImageReq));
	m_msg_handler_list[Protocol::MT_XIANNV_HUANGUA_UPLEVEL_REQ_CS] = HandlerItem(&MessageHandler::OnXiannvHuanHuaUpLevelReq, sizeof(Protocol::CSXiannvHuanHuaUpLevelReq)); 
	m_msg_handler_list[Protocol::MT_XIANNV_CHANGE_SKILL_REQ_CS] = HandlerItem(&MessageHandler::OnXiannvChangeSkillReq, sizeof(Protocol::CSXiannvChangeSkillReq));
	m_msg_handler_list[Protocol::MT_XIANNV_SHENGWU_REQ_CS] = HandlerItem(&MessageHandler::OnXiannvShengwuReq, sizeof(Protocol::CSXiannvShengwuReq));

	m_msg_handler_list[Protocol::MT_FAMOUS_MAN_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnFamousManOperaReq, sizeof(Protocol::CSFamousManOpera));

	m_msg_handler_list[Protocol::MT_WABAO_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnWabaoOperaReq, sizeof(Protocol::CSWabaoOperaReq));
	m_msg_handler_list[Protocol::MT_CHONGZHI_FETCH_REWARD_CS] = HandlerItem(&MessageHandler::OnChongzhiFetchReward, sizeof(Protocol::CSChongzhiFetchReward));
	m_msg_handler_list[Protocol::MT_ZAI_CHONGZHI_FETCH_REWARD_CS] = HandlerItem(&MessageHandler::OnZaiChongzhiFetchReward, sizeof(Protocol::CSZaiChongzhiFetchReward));
	m_msg_handler_list[Protocol::MT_TOTAL_CHONGZHI_WANT_MONEY_FETCH_MSG_CS] = HandlerItem(&MessageHandler::OnChargeWantMoneyFetchMsg, sizeof(Protocol::CSTotalChongzhiWantMoneyFetchInfo));
	m_msg_handler_list[Protocol::MT_TOTAL_CHONGZHI_WANT_MONEY_FETCH_REWARD_CS] = HandlerItem(&MessageHandler::OnChargeWantMoneyFetchReward, sizeof(Protocol::CSTotalChongzhiWantMoneyFetchReward));
	m_msg_handler_list[Protocol::MT_JILIAN_OPERA_CS] = HandlerItem(&MessageHandler::OnJilianOpera, sizeof(Protocol::CSJilianOpera));

	m_msg_handler_list[Protocol::MT_MOUNT_GOON_REQ_CS]	= HandlerItem(&MessageHandler::OnGoonMount, sizeof(Protocol::CSGoonMount));
	m_msg_handler_list[Protocol::MT_MOUNT_UPGRADE_REQ_CS]	= HandlerItem(&MessageHandler::OnUpgradeMount, sizeof(Protocol::CSUpgradeMount));
	m_msg_handler_list[Protocol::MT_MOUNT_USE_IMAGE_CS]	= HandlerItem(&MessageHandler::OnUseMountImage, sizeof(Protocol::CSUseMountImage));
	m_msg_handler_list[Protocol::MT_MOUNT_GET_MOUNT_INFO_CS]	= HandlerItem(&MessageHandler::OnMountGetInfo, sizeof(Protocol::CSMountGetInfo));
	m_msg_handler_list[Protocol::MT_MOUNT_UPLEVEL_EQUIP_CS]	= HandlerItem(&MessageHandler::OnMountUplevelEquip, sizeof(Protocol::CSMountUplevelEquip));
	m_msg_handler_list[Protocol::MT_MOUNT_SPECIAL_IMG_UPGRADE_CS]	= HandlerItem(&MessageHandler::OnMountSpecialImgUpgrade, sizeof(Protocol::CSMountSpecialImgUpgrade));
	m_msg_handler_list[Protocol::MT_MOUNT_SKILL_UPLEVEL_REQ_CS]	= HandlerItem(&MessageHandler::OnMountSkillUplevelReq, sizeof(Protocol::CSMountSkillUplevelReq));
	m_msg_handler_list[Protocol::MT_MOUNT_UP_STAR_LEVEL_CS]	= HandlerItem(&MessageHandler::OnMountUpStarLevelReq, sizeof(Protocol::CSMountUpStarLevel));

	m_msg_handler_list[Protocol::MT_FIGHT_MOUNT_GOON_REQ_CS]	= HandlerItem(&MessageHandler::OnGoonFightMount, sizeof(Protocol::CSGoonFightMount));
	m_msg_handler_list[Protocol::MT_FIGHT_MOUNT_UPGRADE_REQ_CS]	= HandlerItem(&MessageHandler::OnUpgradeFightMount, sizeof(Protocol::CSUpgradeFightMount));
	m_msg_handler_list[Protocol::MT_FIGHT_MOUNT_USE_IMAGE_CS]	= HandlerItem(&MessageHandler::OnUseFightMountImage, sizeof(Protocol::CSUseFightMountImage));
	m_msg_handler_list[Protocol::MT_FIGHT_MOUNT_GET_MOUNT_INFO_CS]	= HandlerItem(&MessageHandler::OnFightMountGetInfo, sizeof(Protocol::CSFightMountGetInfo));
	m_msg_handler_list[Protocol::MT_FIGHT_MOUNT_UPLEVEL_EQUIP_CS]	= HandlerItem(&MessageHandler::OnFightMountUplevelEquip, sizeof(Protocol::CSFightMountUplevelEquip));
	m_msg_handler_list[Protocol::MT_FIGHT_MOUNT_SPECIAL_IMG_UPGRADE_CS]	= HandlerItem(&MessageHandler::OnFightMountSpecialImgUpgrade, sizeof(Protocol::CSFightMountSpecialImgUpgrade));
	m_msg_handler_list[Protocol::MT_FIGHT_MOUNT_SKILL_UPLEVEL_REQ_CS]	= HandlerItem(&MessageHandler::OnFightMountSkillUplevelReq, sizeof(Protocol::CSFightMountSkillUplevelReq));
	m_msg_handler_list[Protocol::MT_FIGHT_MOUNT_UP_STAR_LEVEL_CS]	= HandlerItem(&MessageHandler::OnFightMountUpStarLevelReq, sizeof(Protocol::CSFightMountUpStarLevel));

	m_msg_handler_list[Protocol::MT_WING_UPGRADE_REQ_CS]	= HandlerItem(&MessageHandler::OnUpgradeWing, sizeof(Protocol::CSUpgradeWing));
	m_msg_handler_list[Protocol::MT_WING_USE_IMAGE_CS]	= HandlerItem(&MessageHandler::OnUseWingImage, sizeof(Protocol::CSUseWingImage));
	m_msg_handler_list[Protocol::MT_WING_SPECIAL_IMG_UPGRADE_CS]	= HandlerItem(&MessageHandler::OnWingSpecialImgUpgrade, sizeof(Protocol::CSWingSpecialImgUpgrade));
	m_msg_handler_list[Protocol::MT_WING_UPLEVEL_EQUIP_CS]	= HandlerItem(&MessageHandler::OnWingUplevelEquip, sizeof(Protocol::CSWingUplevelEquip));
	m_msg_handler_list[Protocol::MT_WING_SKILL_UPLEVEL_REQ_CS]	= HandlerItem(&MessageHandler::OnWingSkillUplevelReq, sizeof(Protocol::CSWingSkillUplevelReq));
	m_msg_handler_list[Protocol::MT_WING_GET_INFO_CS]	= HandlerItem(&MessageHandler::OnWingGetInfo, sizeof(Protocol::CSWingGetInfo));
	m_msg_handler_list[Protocol::MT_WING_UP_STAR_LEVEL_CS]	= HandlerItem(&MessageHandler::OnWingUpStarLevelReq, sizeof(Protocol::CSWingUpStarLevel));

	m_msg_handler_list[Protocol::MT_HALO_UPGRADE_REQ_CS]	= HandlerItem(&MessageHandler::OnUpgradeHalo, sizeof(Protocol::CSUpgradeHalo));
	m_msg_handler_list[Protocol::MT_HALO_USE_IMAGE_CS]	= HandlerItem(&MessageHandler::OnUseHaloImage, sizeof(Protocol::CSUseHaloImage));
	m_msg_handler_list[Protocol::MT_HALO_SPECIAL_IMG_UPGRADE_CS]	= HandlerItem(&MessageHandler::OnHaloSpecialImgUpgrade, sizeof(Protocol::CSHaloSpecialImgUpgrade));
	m_msg_handler_list[Protocol::MT_HALO_UPLEVEL_EQUIP_CS]	= HandlerItem(&MessageHandler::OnHaloUplevelEquip, sizeof(Protocol::CSHaloUplevelEquip));
	m_msg_handler_list[Protocol::MT_HALO_SKILL_UPLEVEL_REQ_CS]	= HandlerItem(&MessageHandler::OnHaloSkillUplevelReq, sizeof(Protocol::CSHaloSkillUplevelReq));
	m_msg_handler_list[Protocol::MT_HALO_GET_INFO_CS]	= HandlerItem(&MessageHandler::OnHaloGetInfo, sizeof(Protocol::CSHaloGetInfo));
	m_msg_handler_list[Protocol::MT_HALO_UP_STAR_LEVEL_CS]	= HandlerItem(&MessageHandler::OnHaloUpStarLevelReq, sizeof(Protocol::CSHaloUpStarLevel));

	m_msg_handler_list[Protocol::MT_SHENGONG_UPGRADE_REQ_CS]	= HandlerItem(&MessageHandler::OnUpgradeShengong, sizeof(Protocol::CSUpgradeShengong));
	m_msg_handler_list[Protocol::MT_SHENGONG_USE_IMAGE_CS]	= HandlerItem(&MessageHandler::OnUseShengongImage, sizeof(Protocol::CSUseShengongImage));
	m_msg_handler_list[Protocol::MT_SHENGONG_SPECIAL_IMG_UPGRADE_CS]	= HandlerItem(&MessageHandler::OnShengongSpecialImgUpgrade, sizeof(Protocol::CSShengongSpecialImgUpgrade));
	m_msg_handler_list[Protocol::MT_SHENGONG_UPLEVEL_EQUIP_CS]	= HandlerItem(&MessageHandler::OnShengongUplevelEquip, sizeof(Protocol::CSShengongUplevelEquip));
	m_msg_handler_list[Protocol::MT_SHENGONG_SKILL_UPLEVEL_REQ_CS]	= HandlerItem(&MessageHandler::OnShengongSkillUplevelReq, sizeof(Protocol::CSShengongSkillUplevelReq));
	m_msg_handler_list[Protocol::MT_SHENGONG_GET_INFO_CS]	= HandlerItem(&MessageHandler::OnShengongGetInfo, sizeof(Protocol::CSShengongGetInfo));
	m_msg_handler_list[Protocol::MT_SHENGONG_UP_STAR_LEVEL_CS]	= HandlerItem(&MessageHandler::OnShengongUpStarLevelReq, sizeof(Protocol::CSShengongUpStarLevel));
	m_msg_handler_list[Protocol::MT_SHENGONG_UNUSE_IMAGE_CS]	= HandlerItem(&MessageHandler::OnUnUseShengongImage, sizeof(Protocol::CSUnUseShengongImage));

	m_msg_handler_list[Protocol::MT_SHENYI_UPGRADE_REQ_CS]	= HandlerItem(&MessageHandler::OnUpgradeShenyi, sizeof(Protocol::CSUpgradeShenyi));
	m_msg_handler_list[Protocol::MT_SHENYI_USE_IMAGE_CS]	= HandlerItem(&MessageHandler::OnUseShenyiImage, sizeof(Protocol::CSUseShenyiImage));
	m_msg_handler_list[Protocol::MT_SHENYI_SPECIAL_IMG_UPGRADE_CS]	= HandlerItem(&MessageHandler::OnShenyiSpecialImgUpgrade, sizeof(Protocol::CSShenyiSpecialImgUpgrade));
	m_msg_handler_list[Protocol::MT_SHENYI_UPLEVEL_EQUIP_CS]	= HandlerItem(&MessageHandler::OnShenyiUplevelEquip, sizeof(Protocol::CSShenyiUplevelEquip));
	m_msg_handler_list[Protocol::MT_SHENYI_SKILL_UPLEVEL_REQ_CS]	= HandlerItem(&MessageHandler::OnShenyiSkillUplevelReq, sizeof(Protocol::CSShenyiSkillUplevelReq));
	m_msg_handler_list[Protocol::MT_SHENYI_GET_INFO_CS]	= HandlerItem(&MessageHandler::OnShenyiGetInfo, sizeof(Protocol::CSShenyiGetInfo));
	m_msg_handler_list[Protocol::MT_SHENYI_UP_STAR_LEVEL_CS]	= HandlerItem(&MessageHandler::OnShenyiUpStarLevelReq, sizeof(Protocol::CSShenyiUpStarLevel));
	m_msg_handler_list[Protocol::MT_SHENYI_UNUSE_IMAGE_CS]	= HandlerItem(&MessageHandler::OnUnUseShenyiImage, sizeof(Protocol::CSUnUseShenyiImage));

	m_msg_handler_list[Protocol::MT_FOOTPRINT_OPERATE_CS] = HandlerItem(&MessageHandler::OnFootprintOperate, sizeof(Protocol::CSFootprintOperate));
	m_msg_handler_list[Protocol::MT_CLOAK_OPERATE_CS] = HandlerItem(&MessageHandler::OnCloakOperate, sizeof(Protocol::CSCloakOperate));

	m_msg_handler_list[Protocol::MT_XN_SHOUHU_USE_IMAGE_CS]	= HandlerItem(&MessageHandler::OnXiannvShouhuUseImageReq, sizeof(Protocol::CSUseXiannvShouhuImage));
	m_msg_handler_list[Protocol::MT_XN_SHOUHU_INFO_REQ_CS]	= HandlerItem(&MessageHandler::OnXiannvShouhuGetInfoReq, sizeof(Protocol::CSXiannvShouhuGetInfo));
	m_msg_handler_list[Protocol::MT_XN_SHOUHU_UPSTAR_CS]	= HandlerItem(&MessageHandler::OnXiannvShouhuUpstarReq, sizeof(Protocol::CSXiannvShouhuUpStarLevel));

	m_msg_handler_list[Protocol::MT_JL_GUANGHUAN_USE_IMAGE_CS]	= HandlerItem(&MessageHandler::OnJinglingGuanghuanUseImageReq, sizeof(Protocol::CSUseJinglingGuanghuanImage));
	m_msg_handler_list[Protocol::MT_JL_GUANGHUAN_INFO_REQ_CS]	= HandlerItem(&MessageHandler::OnJinglingGuanghuanGetInfoReq, sizeof(Protocol::CSJinglingGuanghuanGetInfo));
	m_msg_handler_list[Protocol::MT_JL_GUANGHUAN_UPSTAR_CS]	= HandlerItem(&MessageHandler::OnJinglingGuanghuanUpstarReq, sizeof(Protocol::CSJinglingGuanghuanUpStarLevel));
	m_msg_handler_list[Protocol::MT_JL_GUANGHUAN_SPECIAL_IMG_UPGRADE_CS]	= HandlerItem(&MessageHandler::OnJinglingGuanghuanSpecialImgUpgrade, sizeof(Protocol::CSJinglingFazhenSpecialImgUpgrade));

	m_msg_handler_list[Protocol::MT_FAIRY_BUDDHA_CARD_REQ_CS] = HandlerItem(&MessageHandler::OnFairyBuddhaCardActivateReq, sizeof(Protocol::CSFairyBuddhaCardActivateReq));
	m_msg_handler_list[Protocol::MT_FAIRY_BUDDHA_CARD_GOLD_REQ_CS] = HandlerItem(&MessageHandler::OnFairyBuddhaCardGoldBindReq, sizeof(Protocol::CSFairyBuddhaCardGoldBindReq));

	m_msg_handler_list[Protocol::MT_GUILD_QUESTION_ENTER_REQ_CS] = HandlerItem(&MessageHandler::OnGuildQuestionEnterReq, sizeof(Protocol::CSGuildQuestionEnterReq));

	m_msg_handler_list[Protocol::MT_JL_FAZHEN_USE_IMAGE_CS]	= HandlerItem(&MessageHandler::OnJinglingFazhenUseImageReq, sizeof(Protocol::CSUseJinglingFazhenImage));
	m_msg_handler_list[Protocol::MT_JL_FAZHEN_INFO_REQ_CS]	= HandlerItem(&MessageHandler::OnJinglingFazhenGetInfoReq, sizeof(Protocol::CSJinglingFazhenGetInfo));
	m_msg_handler_list[Protocol::MT_JL_FAZHEN_UPSTAR_CS]	= HandlerItem(&MessageHandler::OnJinglingFazhenUpstarReq, sizeof(Protocol::CSJinglingFazhenUpStarLevel));
	m_msg_handler_list[Protocol::MT_JL_FAZHEN_SPECIAL_IMG_UPGRADE_CS]	= HandlerItem(&MessageHandler::OnJinglingFazhenSpecialImgUpgrade, sizeof(Protocol::CSJinglingFazhenSpecialImgUpgrade));

	m_msg_handler_list[Protocol::M_MOJIE_INFO_REQ_CS]	= HandlerItem(&MessageHandler::OnMojieInfoReq, sizeof(Protocol::CSMojieGetInfo));
	m_msg_handler_list[Protocol::M_MOJIE_UP_LEVEL_REQ_CS]	= HandlerItem(&MessageHandler::OnMojieUplevelReq, sizeof(Protocol::CSMojieUplevelReq));
	m_msg_handler_list[Protocol::M_MOJIE_CHANGE_SKILL_REQ_CS]	= HandlerItem(&MessageHandler::OnMojieChangeSkillReq, sizeof(Protocol::CSMojieChangeSkillReq));

	m_msg_handler_list[Protocol::MT_ACTIVITY_ENTER_REQ_CS]	= HandlerItem(&MessageHandler::OnActivityEnterReq, sizeof(Protocol::CSActivityEnterReq));
	m_msg_handler_list[Protocol::MT_SEND_GUILD_SOS_REQ]	= HandlerItem(&MessageHandler::OnSendGuildSosReq, sizeof(Protocol::CSSendGuildSosReq));
	m_msg_handler_list[Protocol::MT_REPLY_GUILD_SOS_REQ] = HandlerItem(&MessageHandler::OnReplyGuildSosReq, sizeof(Protocol::CSReplyGuildSosReq));

	m_msg_handler_list[Protocol::MT_ZHUXIE_FETCH_TASK_REWARD_CS]	= HandlerItem(&MessageHandler::OnZhuXieFetchReward, sizeof(Protocol::CSZhuXieFetchTaskReward));

	m_msg_handler_list[Protocol::MT_FB_NEQ_TAKEREWARD_CS] = HandlerItem(&MessageHandler::OnNeqTakeStarRewardReq, sizeof(Protocol::CSNeqFBStarRewardReq));
	m_msg_handler_list[Protocol::MT_FB_NEQ_EXCHANGE_CS] = HandlerItem(&MessageHandler::OnNeqExchangeReq, sizeof(Protocol::CSNeqFBExchangeReq));
	m_msg_handler_list[Protocol::MT_FB_NEQ_BUY_TIMES_CS] = HandlerItem(&MessageHandler::OnNeqBuyTimesReq, sizeof(Protocol::CSNeqFBBuyTimesReq));
	m_msg_handler_list[Protocol::MT_FB_NEQ_AUTO_CS] = HandlerItem(&MessageHandler::OnNeqAutoReq, sizeof(Protocol::CSNeqFBAutoReq));
	m_msg_handler_list[Protocol::MT_FB_NEQ_INFO_CS] = HandlerItem(&MessageHandler::OnNeqInfoReq, sizeof(Protocol::CSNeqInfoReq));
	m_msg_handler_list[Protocol::MT_FB_NEQ_ROLL_CS] = HandlerItem(&MessageHandler::OnNeqRollReq, sizeof(Protocol::CSNeqRollReq));

	m_msg_handler_list[Protocol::MT_FB_ARMORDEFEND_ROLE_REQ_CS] = HandlerItem(&MessageHandler::OnArmorDefendRoleReq, sizeof(Protocol::CSArmorDefendRoleReq));
	m_msg_handler_list[Protocol::MT_FB_DAILY_REQ_ROLE_INFO_CS]	= HandlerItem(&MessageHandler::OnDailyFBGetRoleInfoReq, sizeof(Protocol::CSDailyFBGetRoleInfo));
	m_msg_handler_list[Protocol::MT_EXP_FB_FETCH_FIRST_REWARD_REQ_CS] = HandlerItem(&MessageHandler::OnFetchExpFbFirstRewardReq, sizeof(Protocol::CSExpFBRetchFirstRewardReq));
	m_msg_handler_list[Protocol::MT_EXP_FB_PAY_BUFF] = HandlerItem(&MessageHandler::OnExpFbPayBuff, sizeof(Protocol::CSExpFbPayBuff));

	m_msg_handler_list[Protocol::MT_FB_PATA_REQ_ALL_INFO_CS] = HandlerItem(&MessageHandler::OnPataFBGetAllInfoReq, sizeof(Protocol::CSPataFbAllInfo));
	m_msg_handler_list[Protocol::MT_FB_VIP_ALL_INFO_REQ_CS]	= HandlerItem(&MessageHandler::OnVipFBGetAllInfoReq, sizeof(Protocol::CSVipFbAllInfoReq));

	m_msg_handler_list[Protocol::MT_FB_GUILDMONSTER_CS]	= HandlerItem(&MessageHandler::OnGuildMonsterQuery, sizeof(Protocol::CSQueryGuildMonster));
	m_msg_handler_list[Protocol::MT_FITHTING_MINING_REQ_CS] = HandlerItem(&MessageHandler::OnFightingMiningReq, sizeof(Protocol::CSFightingReq));

	m_msg_handler_list[Protocol::MT_FB_PHASE_INFO_CS] = HandlerItem(&MessageHandler::OnPhaseFBInfoReq, sizeof(Protocol::CSPhaseFBInfoReq));

	m_msg_handler_list[Protocol::MT_ENTER_BOSS_FAMILY_CS] = HandlerItem(&MessageHandler::OnEnterBossFamilyReq, sizeof(Protocol::CSEnterBossFamily));
	m_msg_handler_list[Protocol::MT_ENTER_BOSS_FAMILY_OPERATE_CS] = HandlerItem(&MessageHandler::OnBossFamilyOperateReq, sizeof(Protocol::CSBossFamilyOperate));
	m_msg_handler_list[Protocol::MT_GET_BOSS_INFO_REQ_CS] = HandlerItem(&MessageHandler::OnGetBossInfoReq, sizeof(Protocol::CSGetBossInfoReq));
	m_msg_handler_list[Protocol::MT_BOSS_KILLER_INFO_REQ] = HandlerItem(&MessageHandler::OnBossKillerInfoReq, sizeof(Protocol::CSBossKillerInfoReq));
	m_msg_handler_list[Protocol::MT_GET_SCENE_MONSTER_GENERATE_INFO_CS]	= HandlerItem(&MessageHandler::OnGetMonsterGenerateInfo, sizeof(Protocol::CSReqMonsterGeneraterList));
	m_msg_handler_list[Protocol::MT_GET_SCENE_GATHER_GENERATE_INFO_CS] = HandlerItem(&MessageHandler::OnGetGatherGenerateInfo, sizeof(Protocol::CSReqGatherGeneraterList));
	m_msg_handler_list[Protocol::MT_FOLLOW_BOSS_REQ_CS] = HandlerItem(&MessageHandler::OnFollowBossOperReq, sizeof(Protocol::CSFollowBossReq));
	m_msg_handler_list[Protocol::MT_PRECIOUS_BOSS_POS_REQ_CS] = HandlerItem(&MessageHandler::OnPreciousPosReq, sizeof(Protocol::CSPreciousOperaReq));
	m_msg_handler_list[Protocol::MT_YUNYOU_BOSS_OPERA_CS] = HandlerItem(&MessageHandler::OnYunyouBossOperateReq, sizeof(Protocol::CSYunyouBossOperate));

	m_msg_handler_list[Protocol::MT_GET_YAOSHOUGUANGCHANG_STATE_CS]		= HandlerItem(&MessageHandler::OnGetYaoShouGuangChangState, sizeof(Protocol::CSGetYaoShouGuangChangState));
	m_msg_handler_list[Protocol::MT_GET_YAOSHOUGUANGCHANG_REWARD_CS]		= HandlerItem(&MessageHandler::OnGetYaoShouGuangChangReward, sizeof(Protocol::CSGetYaoShouGuangChangReward));
	m_msg_handler_list[Protocol::MT_ENTER_YAOSHOUGUANGCHANG_CS]		= HandlerItem(&MessageHandler::OnEnterYaoShouGuangChang, sizeof(Protocol::CSEnterYaoShouGuangChang));

	m_msg_handler_list[Protocol::MT_GET_SUOYAOTA_STATE_CS]		= HandlerItem(&MessageHandler::OnGetSuoYaoTaState, sizeof(Protocol::CSGetSuoYaoTaState));
	m_msg_handler_list[Protocol::MT_GET_SUOYAOTA_REWARD_CS]		= HandlerItem(&MessageHandler::OnGetSuoYaoTaReward, sizeof(Protocol::CSGetSuoYaoTaReward));

	m_msg_handler_list[Protocol::MT_GET_GUILDFB_GUARD_POS_CS]		= HandlerItem(&MessageHandler::OnGetGuildFBGuardPos, sizeof(Protocol::CSGetGuildFBGuardPos));

	m_msg_handler_list[Protocol::MT_TEAM_EQUIP_FB_BUY_DROP_TIMES_CS]		= HandlerItem(&MessageHandler::OnTeamEquipFbBuyDropCount, sizeof(Protocol::CSTeamEquipFbBuyDropCount));

	m_msg_handler_list[Protocol::MT_ENTER_SUOYAOTA_CS]		= HandlerItem(&MessageHandler::OnEnterSuoYaoTa, sizeof(Protocol::CSGetSuoYaoTaState));

	m_msg_handler_list[Protocol::MT_FB_EQUIP_GETINFO_CS]		= HandlerItem(&MessageHandler::OnGetFBEquipInfo, sizeof(Protocol::CSEquipFBGetInfo));
	m_msg_handler_list[Protocol::MT_TUMO_TASK_OPERA_CS]	= HandlerItem(&MessageHandler::OnTuMoTaskOpera, sizeof(Protocol::CSTuMoTaskOpera));
	m_msg_handler_list[Protocol::MT_PAOHUAN_TASK_INFO_REQ_CS]	= HandlerItem(&MessageHandler::OnGetPaoHuanTaskInfo, sizeof(Protocol::CSGetPaoHuanTaskInfo));
	m_msg_handler_list[Protocol::MT_HUSONG_TASK_REFRESH_CS]	= HandlerItem(&MessageHandler::OnRefreshHoSongTask, sizeof(Protocol::CSRefreshHusongTask));
	m_msg_handler_list[Protocol::MT_HUSONG_BUY_TIMES_CS] = HandlerItem(&MessageHandler::OnHusongBuyTimes, sizeof(Protocol::CSHusongBuyTimes));
	m_msg_handler_list[Protocol::MT_HUSONG_ADD_SHIIELD_CS] = HandlerItem(&MessageHandler::OnHusongAddShiled, sizeof(Protocol::CSHusongAddShield));

	m_msg_handler_list[Protocol::MT_FB_TEAM_ROOM_OPERATE_CS]	= HandlerItem(&MessageHandler::OnTeamFbRoomOperaReq, sizeof(Protocol::CSTeamFbRoomOperate));

	m_msg_handler_list[Protocol::MT_FB_TOWERDEFEND_ROLE_INFO_CS]	= HandlerItem(&MessageHandler::OnGetTowerDefendRoleInfo, sizeof(Protocol::CSTowerDefendRoleInfo));
	m_msg_handler_list[Protocol::MT_FB_TOWERDEFEND_BUY_JOIN_TIMES_CS]	= HandlerItem(&MessageHandler::OnTowerDefendBuyJoinTimes, sizeof(Protocol::CSTowerDefendBuyJoinTimes));
	m_msg_handler_list[Protocol::MT_FB_TOWERDEFEND_REFRESH_REQ_CS] = HandlerItem(&MessageHandler::OnTowerDefendNextWave, sizeof(Protocol::CSTowerDefendNextWave));

	m_msg_handler_list[Protocol::MT_FB_MGXF_TOUCH_DOOR_CS]	= HandlerItem(&MessageHandler::OnMiGongXianFuTouchDoor, sizeof(Protocol::CSMgxfTeamFbTouchDoor));

	m_msg_handler_list[Protocol::MT_FB_STORY_REQ_INFO_CS]	= HandlerItem(&MessageHandler::OnStoryFBGetInfo, sizeof(Protocol::CSStoryFBGetInfo));

	m_msg_handler_list[Protocol::MT_FUN_OPEN_STORY_STEP_CS]	= HandlerItem(&MessageHandler::OnFunOpenStoryStep, sizeof(Protocol::CSFunOpenStoryStep));
	m_msg_handler_list[Protocol::MT_FUN_OPEN_SET_OBJ_POS_CS]	= HandlerItem(&MessageHandler::OnFunOpenSetObjPos, sizeof(Protocol::CSFunOpenSetObjToPos));

	m_msg_handler_list[Protocol::MT_FB_GUIDE_FINISH]	= HandlerItem(&MessageHandler::OnFbGuideFinish, sizeof(Protocol::CSFbGuideFinish));
	m_msg_handler_list[Protocol::MT_FB_GUIDE_CREATE_GATHER]	= HandlerItem(&MessageHandler::OnFbGuideCreateGather, sizeof(Protocol::CSFbGuideCreateGather));

	m_msg_handler_list[Protocol::MT_FB_SCENE_GUWU_CS]	= HandlerItem(&MessageHandler::OnFbGuwu, sizeof(Protocol::CSFbGuwu));
	m_msg_handler_list[Protocol::MT_FB_CHALLENGE_FB_OPERATE_CS] = HandlerItem(&MessageHandler::OnChallengeFBOperateReq, sizeof(Protocol::CSChallengeFBOP));

	m_msg_handler_list[Protocol::MT_QUNXIANLUANDOU_PICKUP_SHENSHI_CS]	= HandlerItem(&MessageHandler::OnBattleFieldPickUpFlagReq, sizeof(Protocol::CSQunXianLuanDouPickUpShenShi));
	m_msg_handler_list[Protocol::MT_QUNXIANLUANDOU_RANK_FIRST_REQ_CS] = HandlerItem(&MessageHandler::OnQunxianLuandouFirstRankReq, sizeof(Protocol::CSQunxianLuandouFirstRankReq));

	m_msg_handler_list[Protocol::MT_GUILD_BATTLE_DAILY_REWARD_FETCH_CS] = HandlerItem(&MessageHandler::OnGuildBattleFetchDailyRewardReq, sizeof(Protocol::CSFetchGuildBattleDailyReward));
	 
	m_msg_handler_list[Protocol::MT_GUILD_INFO_STATISTIC_REQ_CS] = HandlerItem(&MessageHandler::OnGuildInfoStatisticReq, sizeof(Protocol::CSGuildInfoStatisticReq));

	m_msg_handler_list[Protocol::MT_GCZ_CHANGE_PLACE_CS]	= HandlerItem(&MessageHandler::OnGczChangePlace, sizeof(Protocol::CSGCZChangePlace));

	m_msg_handler_list[Protocol::MT_GET_BATTLEFIELD_HONOR_INFO_REQ_CS]	= HandlerItem(&MessageHandler::OnBattleFieldHonorInfoReq, sizeof(Protocol::CSBattleFieldHonorInfoReq));
	m_msg_handler_list[Protocol::MT_GET_ACTIVITY_ROME_STATUS_CS]	= HandlerItem(&MessageHandler::OnQueryActivityRomeStatusReq, sizeof(Protocol::CSActivityRoomStatusReq));

	//m_msg_handler_list[Protocol::MT_GONGCHENGZHAN_GET_OWNER_INFO_REQ_CS] = HandlerItem(&MessageHandler::OnGongChengZhanGetOwnerInfoReq, sizeof(Protocol::CSGongChengZhanGetOwnerInfoReq));

	m_msg_handler_list[Protocol::MT_GB_ROLE_ADD_HUDUN_CS] = HandlerItem(&MessageHandler::OnGuildBattleAddHuDunReq, sizeof(Protocol::CSGBAddHuDun));
	m_msg_handler_list[Protocol::MT_GB_ROLE_SUBMIT_GOODS_CS] = HandlerItem(&MessageHandler::OnGuildBattleSubmitGoodsReq, sizeof(Protocol::CSGBRoleCalcSubmitReq));
	m_msg_handler_list[Protocol::MT_GB_WINNER_INFO_CS] = HandlerItem(&MessageHandler::OnGuildBattleWinerInfoReq, sizeof(Protocol::CSGBWinnerInfoReq));

	m_msg_handler_list[Protocol::MT_ACTIVIT_MILLIONAIRE_USE_FROZEN_SKILL] = HandlerItem(&MessageHandler::OnMillionaireUseSkillReq, sizeof(Protocol::CSMillionaireUseFrozenSkill));

	m_msg_handler_list[Protocol::MT_TW_RELIVE_SHOP_BUY_CS] = HandlerItem(&MessageHandler::OnTerritoryWarReliveBuyReq, sizeof(Protocol::CSTerritoryWarReliveShopBuy));
	m_msg_handler_list[Protocol::MT_TW_FIGHT_SHOP_BUY_CS] = HandlerItem(&MessageHandler::OnTerritoryWarFightBuyReq, sizeof(Protocol::CSTerritoryWarReliveFightBuy));
	m_msg_handler_list[Protocol::MT_TW_QUALIFICATION_CS] = HandlerItem(&MessageHandler::OnTerritoryQualificationReq, sizeof(Protocol::CSTerritoryWarQualification));
	m_msg_handler_list[Protocol::MT_TW_SET_LANDMINE_REQ_CS] = HandlerItem(&MessageHandler::OnTerritorySetLandMineReq, sizeof(Protocol::CSTerritorySetLandMine));

	m_msg_handler_list[Protocol::MT_HOTSPRING_GIVE_PRESEN_CS] = HandlerItem(&MessageHandler::OnHotspringGivePresentReq, sizeof(Protocol::CSHotspringGivePresent));
	m_msg_handler_list[Protocol::MT_HOTSPRING_ADD_PARTNER_CS] = HandlerItem(&MessageHandler::OnHotspringAddPartnerReq, sizeof(Protocol::CSHSAddPartnerReq));
	m_msg_handler_list[Protocol::MT_HOTSPRING_ADD_PARTNER_RET_CS] = HandlerItem(&MessageHandler::OnHotspringAddPartnerReq, sizeof(Protocol::CSHSAddPartnerRet));
	m_msg_handler_list[Protocol::MT_HOTSPRING_DELETE_PARTNER_CS] = HandlerItem(&MessageHandler::OnHotspringDeletePartnerReq, sizeof(Protocol::CSHSDeleteParter));
	m_msg_handler_list[Protocol::MT_HOTSPRING_QA_FIRST_REQ_CS] = HandlerItem(&MessageHandler::OnHotspringQAFirstPostReq, sizeof(Protocol::CSHSQAFirstPosReq));
	m_msg_handler_list[Protocol::MT_HOTSPRING_QA_ANSWER_REQ_CS] = HandlerItem(&MessageHandler::OnHotspringQAAnswerReq, sizeof(Protocol::CSHSQAAnswerReq));
	m_msg_handler_list[Protocol::MT_HOTSPRING_QA_USE_CAR_REQ_CS] = HandlerItem(&MessageHandler::OnHotspringQAUseCardReq, sizeof(Protocol::CSHSQAUseCardReq));
	m_msg_handler_list[Protocol::MT_HOTSPRING_USE_SKILL_REQ_CS] = HandlerItem(&MessageHandler::OnHotspringUseSkillReq, sizeof(Protocol::CSHSUseSkillReq));

	m_msg_handler_list[Protocol::MT_CROSS_GUILDBATTLE_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnCrossGuildBattleOperaReq, sizeof(Protocol::CSCrossGuildBattleOperate));
	m_msg_handler_list[Protocol::MT_CROSS_GUILDBATTLE_GET_RANK_INFO_REQ_CS] = HandlerItem(&MessageHandler::OnCrossGuildBattleGetRankInfoReq, sizeof(Protocol::CSCrossGuildBattleGetRankInfoReq));

	m_msg_handler_list[Protocol::MT_QUESTION_ANSWER_REQ_CS] = HandlerItem(&MessageHandler::OnQuestionAnswerReq, sizeof(Protocol::CSQuestionAnswerReq));

	m_msg_handler_list[Protocol::MT_XIANMENGZHAN_GUILD_CALL_CS] = HandlerItem(&MessageHandler::OnXianMengZhanGuildCall, sizeof(Protocol::CSXianMengZhanGuildCall));
	m_msg_handler_list[Protocol::MT_XIANMENGZHAN_GUILD_FOLLOW_CS] = HandlerItem(&MessageHandler::OnXianMengZhanGuildFollow, sizeof(Protocol::CSXianMengZhanGuildFollow));

	m_msg_handler_list[Protocol::MT_ROLE_TASK_TRANSMIT_CS] = HandlerItem(&MessageHandler::OnRoleTaskTransmit, sizeof(Protocol::CSRoleTaskTransmit));

	m_msg_handler_list[Protocol::MT_GUILD_TASK_FINISH_ALL_CS] = HandlerItem(&MessageHandler::OnFinishAllGuildTask, sizeof(Protocol::CSFinishAllGuildTask));
	m_msg_handler_list[Protocol::MT_GUILD_TASK_DOUBLE_REWARD_CS] = HandlerItem(&MessageHandler::OnGetGuildDoubleReward, sizeof(Protocol::CSGetGuildTaskDoubleReward));

	m_msg_handler_list[Protocol::MT_FORCE_FINISH_SPECIAL_TASK_SC] = HandlerItem(&MessageHandler::OnForceFinishSpecialTask, sizeof(Protocol::CSForceFinishSpecialTask));

	m_msg_handler_list[Protocol::MT_TASK_OPEN_PANEL_CS] = HandlerItem(&MessageHandler::OnOpenTaskPanel, sizeof(Protocol::CSTaskOpenPanel));
	m_msg_handler_list[Protocol::MT_FETCH_TOUZIJIHUA_REWARD_CS] = HandlerItem(&MessageHandler::OnFetchTouZiJiHuaReward, sizeof(Protocol::CSFetchTouZiJiHuaReward));
	m_msg_handler_list[Protocol::MT_TOUZIJIHUA_ACTIVE_CS] = HandlerItem(&MessageHandler::OnTouzijihuaActive, sizeof(Protocol::CSTouzijihuaActive));
	m_msg_handler_list[Protocol::MT_NEW_TOUZIJIHUA_OPERATE_CS] = HandlerItem(&MessageHandler::OnNewTouzijihuaOperate, sizeof(Protocol::CSNewTouzijihuaOperate));
	m_msg_handler_list[Protocol::MT_TOUZIJIHUA_FB_BOSS_OPERATE_CS] = HandlerItem(&MessageHandler::OnTouzijihuaFbBossOperate, sizeof(Protocol::CSTouzijihuaFbBossOperate));

	m_msg_handler_list[Protocol::MT_YAOJIANG_REQ_CS] = HandlerItem(&MessageHandler::OnYaojiangReq, sizeof(Protocol::CSYaojiangReq));

	m_msg_handler_list[Protocol::MT_DATING_INVITE_REQ_CS]		= HandlerItem(&MessageHandler::OnDatingInviteReq, sizeof(Protocol::CSDatingInviteReq));
	m_msg_handler_list[Protocol::MT_DATING_INVITE_ACK_CS]		= HandlerItem(&MessageHandler::OnDatingInviteAck, sizeof(Protocol::CSDatingInviteAck));

	m_msg_handler_list[Protocol::MT_FISH_POOL_RAISE_REQ_CS]		= HandlerItem(&MessageHandler::OnFishPoolRaiseReq, sizeof(Protocol::CSFishPoolRaiseReq));
	m_msg_handler_list[Protocol::MT_FISH_POOL_BUY_BULLET_REQ_CS]= HandlerItem(&MessageHandler::OnFishPoolBuyBulletReq, sizeof(Protocol::CSFishPoolBuyBulletReq));
	m_msg_handler_list[Protocol::MT_FISH_POOL_QUERY_REQ_CS]		= HandlerItem(&MessageHandler::OnFishPoolQueryReq, sizeof(Protocol::CSFishPoolQueryReq));
	m_msg_handler_list[Protocol::MT_FISH_POOL_STEAL_FISH_CS]	= HandlerItem(&MessageHandler::OnFishPoolStealFish, sizeof(Protocol::CSFishPoolStealFish));
	m_msg_handler_list[Protocol::MT_FISH_POOL_HARVEST_FISH_CS]	= HandlerItem(&MessageHandler::OnFishPoolHarvestFish, sizeof(Protocol::CSFishPoolHarvest));

	m_msg_handler_list[Protocol::MT_MARRY_XUNYOU_OPERA_CS]		= HandlerItem(&MessageHandler::OnMarryXunyouOpera, sizeof(Protocol::CSMarryXunyouOpera));
	m_msg_handler_list[Protocol::MT_MARRY_HUNYAN_BLESS_CS]		= HandlerItem(&MessageHandler::OnMarryHunyanBless, sizeof(Protocol::CSMarryHunyanBless));
	m_msg_handler_list[Protocol::MT_MARRY_HUNYAN_GET_BLESS_NEWS_CS]		= HandlerItem(&MessageHandler::OnMarryHunyanGetBlessNews, sizeof(Protocol::CSMarryHunyanGetBlessNews));
	m_msg_handler_list[Protocol::MT_MARRY_HUNYAN_OPERA_CS]		= HandlerItem(&MessageHandler::OnMarryHunyanOpera, sizeof(Protocol::CSMarryHunyanOpera));
	m_msg_handler_list[Protocol::MT_QINGYUAN_OPERA_CS]		= HandlerItem(&MessageHandler::OnQingyuanOperaReq, sizeof(Protocol::CSQingYuanOperaReq));
	m_msg_handler_list[Protocol::MT_RESET_LOVER_NAME_CS]		= HandlerItem(&MessageHandler::OnResetLoverName, sizeof(Protocol::CSResetLoverName));
	m_msg_handler_list[Protocol::MT_MARRY_REQ_CS]				= HandlerItem(&MessageHandler::OnMarryReq, sizeof(Protocol::CSMarryReq));
	m_msg_handler_list[Protocol::MT_MARRY_RET_CS]				= HandlerItem(&MessageHandler::OnMarryRet, sizeof(Protocol::CSMarryRet));
	m_msg_handler_list[Protocol::MT_TURNTABLE_ROLL_REQ_CS]		= HandlerItem(&MessageHandler::OnTurnTableRollReq, sizeof(Protocol::CSTurnTableReq));
	m_msg_handler_list[Protocol::MT_DIVORCE_RET_CS]				= HandlerItem(&MessageHandler::OnDivorceRet, sizeof(Protocol::CSDivorceRet));
	m_msg_handler_list[Protocol::MT_MARRY_ZHUHE_SEND_CS] = HandlerItem(&MessageHandler::OnMarryZhuheShou, sizeof(Protocol::CSMarryZhuheSend));
	m_msg_handler_list[Protocol::MT_QING_YUAN_SHENG_DI_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnQingYuanShengDiOperaReq, sizeof(Protocol::CSQingYuanShengDiOperaReq));

	// 表白
	m_msg_handler_list[Protocol::MT_PROFESS_WALL_REQ_CS] = HandlerItem(&MessageHandler::OnProfessWallReq, sizeof(Protocol::CSProfessWallReq));
	m_msg_handler_list[Protocol::MT_PROFESS_TO_REQ_CS] = HandlerItem(&MessageHandler::OnProfessToReq, sizeof(Protocol::CSProfessToReq));

	m_msg_handler_list[Protocol::MT_PAOHUAN_TASK_COMMIT_REQ_CS] = HandlerItem(&MessageHandler::OnPaoHuanSkipTask, sizeof(Protocol::CSPaoHuanSkipTask));
	m_msg_handler_list[Protocol::MT_COMMON_OPERATE_CS] = HandlerItem(&MessageHandler::OnCommonOperateReq, sizeof(Protocol::CSReqCommonOpreate));

	m_msg_handler_list[Protocol::MT_MOUNT_BIANSHEN_OPERA_CS] = HandlerItem(&MessageHandler::OnMountBianshenOpera, sizeof(Protocol::CSMountBianshenOpera));

	m_msg_handler_list[Protocol::MT_SPEEDUP_HELLO_CS] = HandlerItem(&MessageHandler::OnSpeedupHello, sizeof(Protocol::CSSpeedUpHello));

	m_msg_handler_list[Protocol::MT_HUNYAN_BUY_XUNYOU_OPERA_CS] = HandlerItem(&MessageHandler::OnBuyXunyouOpera, sizeof(Protocol::CSBuyXunyouOpera));

	m_msg_handler_list[Protocol::MT_LIGHTEN_WEDDINGRING_CS] = HandlerItem(&MessageHandler::OnLightenWeddingRing, sizeof(Protocol::CSLightenWeddingRing));
	m_msg_handler_list[Protocol::MT_UPLEVEL_WEDDINGRING_CS] = HandlerItem(&MessageHandler::OnUplevelWeddingRing, sizeof(Protocol::CSUplevelWeddingRing));
	m_msg_handler_list[Protocol::MT_PERFORM_WEDDINGRING_SKILL_CS] = HandlerItem(&MessageHandler::OnPerformWeddingRingSkill, sizeof(Protocol::CSPerformWeddingRingSkill));

	m_msg_handler_list[Protocol::MT_MAZE_QUERY_CS] = HandlerItem(&MessageHandler::OnMazeQuery, sizeof(Protocol::CSQueryMaze));
	m_msg_handler_list[Protocol::MT_MAZE_MOVE_CS] = HandlerItem(&MessageHandler::OnMazeMove, sizeof(Protocol::CSMoveMaze));
	m_msg_handler_list[Protocol::MT_MAZE_BUY_CS] = HandlerItem(&MessageHandler::OnMazeBuy, sizeof(Protocol::CSBuyMaze));
	m_msg_handler_list[Protocol::MT_MAZE_FETCH_JIFEN_REWARD_CS] = HandlerItem(&MessageHandler::OnMazeFetchJifenReward, sizeof(Protocol::CSFetchMazeJifenReward));

	m_msg_handler_list[Protocol::MT_SUPPLY_BUY_IYEM_CS]	= HandlerItem(&MessageHandler::OnSupplyBuyItem, sizeof(Protocol::CSSupplyBuyItem));
	m_msg_handler_list[Protocol::MT_SUPPLY_SET_RECOVER_RANGE_PER_CS] = HandlerItem(&MessageHandler::OnSupplySetRecoverRangePer, sizeof(Protocol::CSSupplySetRecoverRangePer));

	m_msg_handler_list[Protocol::MT_NATIONAL_BOSS_BUY_BUFF_CS] = HandlerItem(&MessageHandler::OnNationalBossBuyBuffReq, sizeof(Protocol::CSNationalBossBuyBuffReq));

	m_msg_handler_list[Protocol::MT_OPEN_GAME_ACTIVITY_REQ_INFO_CS] = HandlerItem(&MessageHandler::OnGetOpenGameActivityInfo, sizeof(Protocol::CSOpenGameActivityInfoReq));
	m_msg_handler_list[Protocol::MT_OPEN_GAME_ACTIVITY_FETCH_REWARD_CS] = HandlerItem(&MessageHandler::OnGetOpenGameFetchReward, sizeof(Protocol::CSOpenGameActivityFetchReward));
	m_msg_handler_list[Protocol::MT_BAI_BEI_FAN_LI_BUY_CS] = HandlerItem(&MessageHandler::OnBaiBeiFanLiBuy, sizeof(Protocol::CSBaiBeiFanLiBuy));
	m_msg_handler_list[Protocol::MT_BAI_BEI_FAN_LI2_BUY_CS] = HandlerItem(&MessageHandler::OnBaiBeiFanLi2Buy, sizeof(Protocol::CSBaiBeiFanLi2Buy));
	m_msg_handler_list[Protocol::MT_TITLE_OWNER_INFO_REQ_CS] = HandlerItem(&MessageHandler::OnGetTitleOwnerInfo, sizeof(Protocol::CSTitleOwnerInfoReq));

	m_msg_handler_list[Protocol::MT_RA_OPEN_BUY_GAME_GIFT_SHOP_INFO_CS] = HandlerItem(&MessageHandler::OnSupperGiftBuyReq, sizeof(Protocol::CSRAOpenGameGiftShopBuy));
	m_msg_handler_list[Protocol::MT_RA_OPEN_GAME_GIFT_SHOP_INFO_CS] = HandlerItem(&MessageHandler::OnSupperGiftInfoReq, sizeof(Protocol::CSRAOpenGameGiftShopBuyInfoReq));

	m_msg_handler_list[Protocol::MT_RA_OPEN_BUY_GAME_GIFT_SHOP2_INFO_CS] = HandlerItem(&MessageHandler::OnSupperGiftBuy2Req, sizeof(Protocol::CSRAOpenGameGiftShopBuy2));

	m_msg_handler_list[Protocol::MT_MAGICAL_PRECIOUS_FETCH_REWARD_CS] = HandlerItem(&MessageHandler::OnFetchMagicalPreciousReward, sizeof(Protocol::CSFetchMagicalPreciousRewardReq));

	m_msg_handler_list[Protocol::MT_RA_EXP_REFIN_REQ_CS] = HandlerItem(&MessageHandler::OnRAExpRefineOperaReq, sizeof(Protocol::CSRAExpRefineReq));

	m_msg_handler_list[Protocol::MT_CHALLENGE_FIELD_GET_USER_INFO_CS] = HandlerItem(&MessageHandler::OnChallengeFieldGetUserInfo, sizeof(Protocol::CSChallengeFieldGetUserInfo));
	m_msg_handler_list[Protocol::MT_CHALLENGE_FIELD_RESET_OPPONENT_REQ_CS] = HandlerItem(&MessageHandler::OnChallengeFieldResetOpponentReq, sizeof(Protocol::CSChallengeFieldResetOpponentList));
	m_msg_handler_list[Protocol::MT_CHALLENGE_FIELD_FIGHT_REQ_CS] = HandlerItem(&MessageHandler::OnChallengeFieldFightReq, sizeof(Protocol::CSChallengeFieldFightReq));
	m_msg_handler_list[Protocol::MT_CHALLENGE_FIELD_BUY_JOIN_TIMES_CS] = HandlerItem(&MessageHandler::OnChallengeFieldBuyJoinTimes, sizeof(Protocol::CSChallengeFieldBuyJoinTimes));
	m_msg_handler_list[Protocol::MT_CHALLENGE_FIELD_FETCH_JIFEN_REWARD_REQ_CS] = HandlerItem(&MessageHandler::OnChallengeFieldFetchJifenRewardReq, sizeof(Protocol::CSChallengeFieldFetchJifenRewardReq));
	m_msg_handler_list[Protocol::MT_CHALLENGE_FIELD_GET_RANK_INFO_CS] = HandlerItem(&MessageHandler::OnChallengeFieldGetRankInfo, sizeof(Protocol::CSChallengeFieldGetRankInfo));
	m_msg_handler_list[Protocol::MT_CHALLENGE_FIELD_GET_LUCKY_INFO_CS] = HandlerItem(&MessageHandler::OnChallengeFieldGetLuckyInfo, sizeof(Protocol::CSChallengeFieldGetLuckyInfo));
	m_msg_handler_list[Protocol::MT_CHALLENGE_FIELD_FETCH_GUANGHUI_CS] = HandlerItem(&MessageHandler::OnChallengeFieldFetchGuangHui, sizeof(Protocol::CSChallengeFieldFetchGuangHui));
	m_msg_handler_list[Protocol::MT_CHALLENGE_FIELD_GET_OPPONENT_INFO_CS] = HandlerItem(&MessageHandler::OnChallengeFieldGetOpponentDetailInfo, sizeof(Protocol::CSChallengeFieldGetOpponentInfo));
	m_msg_handler_list[Protocol::MT_CHALLENGE_FIELD_BUY_BUFF_CS] = HandlerItem(&MessageHandler::OnChallengeFieldBuyBuff, sizeof(Protocol::CSChallengeFieldBuyBuff));
	m_msg_handler_list[Protocol::MT_CHALLENGE_FIELD_BEST_RANK_BREAK_REQ_CS] = HandlerItem(&MessageHandler::OnChallengeFieldBestRankBreakReq, sizeof(Protocol::CSChallengeFieldBestRankBreakReq));
	m_msg_handler_list[Protocol::MT_CHALLENGE_FIELD_READY_START_FIGHT_CS] = HandlerItem(&MessageHandler::OnChallengeFieldReadyStartFightReq, sizeof(Protocol::CSChallengeFieldReadyStartFightReq));

	m_msg_handler_list[Protocol::MT_CLOSE_BETA_ACTIVITY_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnCloseBetaActivityOperaReq, sizeof(Protocol::CSCloseBetaActivityOperaReq));
	m_msg_handler_list[Protocol::MT_LUCKY_ROLL_ACTIVITY_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnLuckyRollActivityOperaReq, sizeof(Protocol::CSLuckyRollActivityOperaReq));

	m_msg_handler_list[Protocol::MT_RAND_ACTIVITY_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnRandActivityOperaReq, sizeof(Protocol::CSRandActivityOperaReq));

	m_msg_handler_list[Protocol::MT_UPDATE_NOTICE_FETCH_REWARD_CS] = HandlerItem(&MessageHandler::OnUpdateNoticeFetchReward, sizeof(Protocol::CSUpdateNoticeFetchReward));

	m_msg_handler_list[Protocol::MT_COMMON_ACTIVITY_FETCH_REWARD_CS] = HandlerItem(&MessageHandler::OnCommonActivityFetchReward, sizeof(Protocol::CSCommonActivityFetchReward));

	m_msg_handler_list[Protocol::MT_TIME_REQ_CS]				= HandlerItem(&MessageHandler::OnTimeReq, sizeof(Protocol::CSTimeReq));

	m_msg_handler_list[Protocol::MT_YIZHANDAODI_GUWU_CS] = HandlerItem(&MessageHandler::OnYiZhanDaoDiGuWu, sizeof(Protocol::CSYiZhanDaoDiGuwu));
	m_msg_handler_list[Protocol::MT_WUXINGGUAJI_GET_REWARD_CS] = HandlerItem(&MessageHandler::OnWuXingGuaJiGetReward, sizeof(Protocol::CSWuXingGuaJiGetReward));

	m_msg_handler_list[Protocol::MT_CHINESEZODIAC_PROMOTE_XINGHUN_LEVEL_CS] = HandlerItem(&MessageHandler::OnChineseZodiacPromoteXingHun, sizeof(Protocol::CSChineseZodiacPromoteXingHun));
	m_msg_handler_list[Protocol::MT_CHINESEZODIAC_PROMOTE_EQUIP_LEVEL_CS] = HandlerItem(&MessageHandler::OnChineseZodiacPromoteEquip, sizeof(Protocol::CSChineseZodiacPromoteEquip));
	m_msg_handler_list[Protocol::MT_CHINESEZODIAC_PROMOTE_LEVEL_CS] = HandlerItem(&MessageHandler::OnChineseZodiacPromote, sizeof(Protocol::CSChineseZodiacPromote));
	m_msg_handler_list[Protocol::MT_CHINESEZODIAC_TIANXIANG_REQ_CS] = HandlerItem(&MessageHandler::OnChineseTianxiangReq, sizeof(Protocol::CSTianxiangReq));
	m_msg_handler_list[Protocol::MT_CHINESEZODIAC_TAKE_OFF_EQUIP_CS] = HandlerItem(&MessageHandler::OnChineseZodiacTakeOffEquip, sizeof(Protocol::CSChineseZodiacTakeOffEquip));
	m_msg_handler_list[Protocol::MT_CHINESEZODIAC_RECYCLE_CS] = HandlerItem(&MessageHandler::OnChineseZodiacRecycle, sizeof(Protocol::CSChineseZodiacRecycle));


	// 组队相关
	m_msg_handler_list[Protocol::MT_CREATE_TEAM_CS]				= HandlerItem(&MessageHandler::OnCreateTeam, sizeof(Protocol::CSCreateTeam));
	m_msg_handler_list[Protocol::MT_INVITE_USER_CS]				= HandlerItem(&MessageHandler::OnInviteUser, sizeof(Protocol::CSInviteUser));
	m_msg_handler_list[Protocol::MT_INVITE_RET_CS]				= HandlerItem(&MessageHandler::OnInviteRet, sizeof(Protocol::CSInviteUserTransmitRet));
	m_msg_handler_list[Protocol::MT_REQ_JOIN_TEAM_RET_CS]		= HandlerItem(&MessageHandler::OnJoinTeamRet, sizeof(Protocol::CSReqJoinTeamRet));
	m_msg_handler_list[Protocol::MT_REQ_JOIN_TEAM_CS]			= HandlerItem(&MessageHandler::OnReqJoinTeam, sizeof(Protocol::CSReqJoinTeam));
	m_msg_handler_list[Protocol::MT_DISMISS_TEAM_CS]			= HandlerItem(&MessageHandler::OnDismissTeam, sizeof(Protocol::CSDismissTeam));
	m_msg_handler_list[Protocol::MT_KICK_OUT_OF_TEAM_CS]		= HandlerItem(&MessageHandler::OnKickOutOfTeam, sizeof(Protocol::CSKickOutOfTeam));
	m_msg_handler_list[Protocol::MT_CHANGE_TEAM_LEADER_CS]		= HandlerItem(&MessageHandler::OnChangeTeamLeader, sizeof(Protocol::CSChangeTeamLeader));
	m_msg_handler_list[Protocol::MT_EXIT_TEAM_CS]				= HandlerItem(&MessageHandler::OnExitTeam, sizeof(Protocol::CSExitTeam));
	m_msg_handler_list[Protocol::MT_TEAM_LIST_REQ_CS]			= HandlerItem(&MessageHandler::OnTeamListReq, sizeof(Protocol::CSTeamListReq));
	m_msg_handler_list[Protocol::MT_CHANGE_MUST_CHECK_CS]		= HandlerItem(&MessageHandler::OnChangeMustCheck, sizeof(Protocol::CSChangeMustCheck));
	m_msg_handler_list[Protocol::MT_CHANGE_ASSIGN_MODE_CS]		= HandlerItem(&MessageHandler::OnChangeAssignMode, sizeof(Protocol::CSChangeAssignMode));
	m_msg_handler_list[Protocol::MT_CHANGE_MEMBER_CAN_INVITE_CS]= HandlerItem(&MessageHandler::OnChangeMemberCanInvite, sizeof(Protocol::CSChangeMemberCanInvite));
	m_msg_handler_list[Protocol::MT_CHANGE_TEAM_LIMIT_CS]		= HandlerItem(&MessageHandler::OnChangeTeamLimit, sizeof(Protocol::CSChangeTeamLimit));

	m_msg_handler_list[Protocol::MT_AUTO_HAVE_TEAM_CS]			= HandlerItem(&MessageHandler::OnAutoHaveTeam,sizeof(Protocol::CSAutoHaveTeam));
	m_msg_handler_list[Protocol::MT_AUTO_APPLY_JOIN_TEAM_CS]	= HandlerItem(&MessageHandler::OnAutoApplyJoinTeam,sizeof(Protocol::CSAutoApplyJoinTeam));
	m_msg_handler_list[Protocol::MT_INVITE_UNIQUE_USER_CS]		= HandlerItem(&MessageHandler::OnInviteUniqueUser, sizeof(Protocol::CSInviteUniqueUser));

	m_msg_handler_list[Protocol::MT_CHANNEL_CHAT_CS] = HandlerItem(&MessageHandler::OnChannelChat, sizeof(Protocol::CSChannelChat));
	m_msg_handler_list[Protocol::MT_SINGLE_CHAT_CS]		= HandlerItem(&MessageHandler::OnSingleChat, sizeof(Protocol::CSSingleChat));
	m_msg_handler_list[Protocol::MT_SINGLE_CHAT_REQ_CS] = HandlerItem(&MessageHandler::OnSingleChatStatusReq, sizeof(Protocol::CSSingleChatReq));
	m_msg_handler_list[Protocol::MT_FORBID_CHAT_INFO_CS] = HandlerItem(&MessageHandler::OnRequestForbidChatInfo, sizeof(Protocol::CSForbidChatInfo));

	m_msg_handler_list[Protocol::MT_WORLDBOSS_KILLER_INFO_CS] = HandlerItem(&MessageHandler::OnWorldBossKillerInfoReq, sizeof(Protocol::CSWorldBossKillerInfoReq));
	m_msg_handler_list[Protocol::MT_GET_WORLDBOSS_INFO_CS] = HandlerItem(&MessageHandler::OnGetWorldBossInfo, sizeof(Protocol::CSGetWorldBossInfo));
	m_msg_handler_list[Protocol::MT_GET_WORLDBOSS_PRESONAL_HURT_INFO_CS] = HandlerItem(&MessageHandler::OnGetWorldBossPersonalHurtInfo, sizeof(Protocol::CSWorldBossPersonalHurtInfoReq));
	m_msg_handler_list[Protocol::MT_GET_WORLDBOSS_GUILD_HURT_INFO_CS] = HandlerItem(&MessageHandler::OnGetWorldBossGuildHurtInfo, sizeof(Protocol::CSWorldBossGuildHurtInfoReq));
	m_msg_handler_list[Protocol::MT_GET_WORLDBOSS_WEEK_RANK_INFO_CS] = HandlerItem(&MessageHandler::OnGetWorldBossWeekRankInfo, sizeof(Protocol::CSWorldBossWeekRankInfoReq));
	m_msg_handler_list[Protocol::MT_GET_WORLDBOSS_ROLL_REQ_CS] = HandlerItem(&MessageHandler::OnGetWorldBossRollReq, sizeof(Protocol::CSWorldBossRollReq));

	m_msg_handler_list[Protocol::MT_RANK_GET_PERSONRANK_LIST_CS] = HandlerItem(&MessageHandler::OnGetPersonRankList, sizeof(Protocol::CSGetPersonRankList));
	m_msg_handler_list[Protocol::MT_RANK_GET_PERSONRANK_TOP_USER_CS] = HandlerItem(&MessageHandler::OnGetPersonRankTopUser, sizeof(Protocol::CSGetPersonRankTopUser));
	m_msg_handler_list[Protocol::MT_RANK_GET_GUILDRANK_LIST_CS] = HandlerItem(&MessageHandler::OnGetGuildRankList, sizeof(Protocol::CSGetGuildRankList));
	m_msg_handler_list[Protocol::MT_RANK_GET_TEAMRANK_LIST_CS] = HandlerItem(&MessageHandler::OnGetTeamRankList, sizeof(Protocol::CSGetTeamRankList));
	m_msg_handler_list[Protocol::MT_QINGYUAN_FB_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnQingyuanFBOperaReq, sizeof(Protocol::CSQingyuanFBOperaReq));
	m_msg_handler_list[Protocol::MT_HUNJIE_UP_LEVEl_CS] = HandlerItem(&MessageHandler::OnHunjieUpLevel, sizeof(Protocol::CSHunjieUpLevel));
	m_msg_handler_list[Protocol::MT_HUNJIE_REQ_EQUIP_INFO_CS] = HandlerItem(&MessageHandler::OnHunjieReqEquipInfo, sizeof(Protocol::CSHunjieReqEquipInfo));
	m_msg_handler_list[Protocol::MT_QINGYUAN_TAKE_OFF_EQUIP] = HandlerItem(&MessageHandler::OnQingyuanTakeOffEquip, sizeof(Protocol::CSQingyuanTakeOffEquip));
	m_msg_handler_list[Protocol::MT_QINGYUAN_QUERY_MATE_QINGYUAN_VALUE_CS] = HandlerItem(&MessageHandler::OnQingyuanQueryMateQingyuanValue, sizeof(Protocol::CSQingyuanQueryMateQingyuanValue));
	m_msg_handler_list[Protocol::MT_QINGYUAN_DIVORCE_REQ_CS] = HandlerItem(&MessageHandler::OnQingyuanDivorceReq, sizeof(Protocol::CSQingyuanDivorceReq));
	m_msg_handler_list[Protocol::MT_FB_REQ_NEXT_LEVEL_CS] = HandlerItem(&MessageHandler::OnFBReqNextLevel, sizeof(Protocol::CSFBReqNextLevel));

	m_msg_handler_list[Protocol::MT_QINGYUAN_EQUIP_OPERATE_CS] = HandlerItem(&MessageHandler::OnQingyuanEquipOperate, sizeof(Protocol::CSQingyuanEquipOperate));

	m_msg_handler_list[Protocol::MT_ROLL_MONEY_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnRollMoneyOperaReq, sizeof(Protocol::CSRollMoneyOperaReq));

	m_msg_handler_list[Protocol::MT_ROLE_GOAL_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnRoleGoalOperaReq, sizeof(Protocol::CSRoleGoalOperaReq));

	m_msg_handler_list[Protocol::MT_ZHANSHENDIAN_FETCH_DAY_REWARD_CS] = HandlerItem(&MessageHandler::OnZhanshendianFetchDayRewardReq, sizeof(Protocol::CSZhanshendianFetchDayReward));

	m_msg_handler_list[Protocol::MT_REQ_TRADE_CS]				= HandlerItem(&MessageHandler::OnReqTrade, sizeof(Protocol::CSReqTrade));
	m_msg_handler_list[Protocol::MT_TRADE_RET_CS]				= HandlerItem(&MessageHandler::OnReqTradeRet, sizeof(Protocol::CSReqTradeRet));
	m_msg_handler_list[Protocol::MT_TRADE_LOCK_REQ_CS]			= HandlerItem(&MessageHandler::OnTradeLock, sizeof(Protocol::CSTradeLockReq));
	m_msg_handler_list[Protocol::MT_TRADE_AFFIRM_REQ_CS]		= HandlerItem(&MessageHandler::OnTradeAffirm, sizeof(Protocol::CSTradeAffirmReq));
	m_msg_handler_list[Protocol::MT_TRADE_CANCLE_CS]			= HandlerItem(&MessageHandler::OnTradeCancle, sizeof(Protocol::CSTradeCancle));
	m_msg_handler_list[Protocol::MT_TRADE_PUT_GOLD_REQ_CS]		= HandlerItem(&MessageHandler::OnTradeGold, sizeof(Protocol::CSTradeGoldReq));
	m_msg_handler_list[Protocol::MT_TRADE_PUT_ITEM_REQ_CS]		= HandlerItem(&MessageHandler::OnTradeItem, sizeof(Protocol::CSTradeItemReq));

	m_msg_handler_list[Protocol::MT_WORLD_EVENT_OBJ_TOUCH_CS]		= HandlerItem(&MessageHandler::OnTouchWorldEventObj, sizeof(Protocol::CSWorldEventObjTouch));
	m_msg_handler_list[Protocol::MT_WORLD_EVENT_FETCH_REWARD_CS]	= HandlerItem(&MessageHandler::OnWorldEventFetchReward, sizeof(Protocol::CSWorldEventFetchReward));
	m_msg_handler_list[Protocol::MT_WORLD_EVENT_QUERY_ROLE_INFO_CS]	= HandlerItem(&MessageHandler::OnWorldEventQueryRoleInfo, sizeof(Protocol::CSWorldEventQueryRoleInfo));
	m_msg_handler_list[Protocol::MT_WORLD_EVENT_COMMON_INFO_REQ_CS]	= HandlerItem(&MessageHandler::OnWorldEventQueryCommonInfo, sizeof(Protocol::CSWorldEventQueryCommonInfo));
	m_msg_handler_list[Protocol::MT_WORLD_EVENT_LIGHT_WORD_CS]		= HandlerItem(&MessageHandler::OnWorldEventLightWord, sizeof(Protocol::CSWorldEventLightWord));
	m_msg_handler_list[Protocol::MT_WORLD_EVENT_GET_NPC_LOCATE_INFO_CS]		= HandlerItem(&MessageHandler::OnWorldEventGetNpcLocateInfo, sizeof(Protocol::CSWorldEventGetNpcLocateInfo));

	m_msg_handler_list[Protocol::MT_RED_PAPER_CREATE_REQ_CS]		= HandlerItem(&MessageHandler::OnRedPaperCreateReq, sizeof(Protocol::CSRedPaperCreateReq));
	m_msg_handler_list[Protocol::MT_RED_PAPER_FETCH_REQ_CS]			= HandlerItem(&MessageHandler::OnRedPaperFetchReq, sizeof(Protocol::CSRedPaperFetchReq));
	m_msg_handler_list[Protocol::MT_RED_PAPER_QUERY_DETAIL_REQ_CS]	= HandlerItem(&MessageHandler::OnRedPaperQueryDetailReq, sizeof(Protocol::CSRedPaperQueryDetailReq));
	m_msg_handler_list[Protocol::MT_RED_PAPER_SEND_COMMAND_CS]		= HandlerItem(&MessageHandler::OnCreateCommandRedPaperReq, sizeof(Protocol::CSCreateCommandRedPaper));
	m_msg_handler_list[Protocol::MT_RED_PAPER_COMMAND_REWARD_CS] = HandlerItem(&MessageHandler::OnFetchCommandRedPaperReq, sizeof(Protocol::CSFetchCommandRedPaper));
	m_msg_handler_list[Protocol::MT_RED_PAPER_COMMAND_CHECK_INFO_CS] = HandlerItem(&MessageHandler::OnCommandRedPaperCheckReq, sizeof(Protocol::CSCommandRedPaperCheckInfo));

	m_msg_handler_list[Protocol::MT_MARRIAGE_SEEKING_REQ_CS]	= HandlerItem(&MessageHandler::OnMarriageSeekingReq, sizeof(Protocol::CSMarriageSeekingOpera));

	m_msg_handler_list[Protocol::MT_SHIZHUANG_USE_REQ_CS]	= HandlerItem(&MessageHandler::OnShizhuangUseReq, sizeof(Protocol::CSShizhuangUseReq));
	m_msg_handler_list[Protocol::MT_SHIZHUANG_UPGRADE_REQ_CS]	= HandlerItem(&MessageHandler::OnShizhuangUpgradeReq, sizeof(Protocol::CSShizhuangUseReq));
	m_msg_handler_list[Protocol::MT_SHIZHUANG_SPECIAL_IMG_UPGRADE_CS] = HandlerItem(&MessageHandler::OnShizhuangUpgradeSpecialImgReq, sizeof(Protocol::CSShizhuangSpecialImgUpgradeReq));

	m_msg_handler_list[Protocol::MT_CARD_OPERATE_CS]	= HandlerItem(&MessageHandler::OnCardOperate, sizeof(Protocol::CSCardOperate));
	m_msg_handler_list[Protocol::MT_MONTH_CARD_FETCH_DAY_REWARD_CS]	= HandlerItem(&MessageHandler::OnMonthCardFetchDayReward, sizeof(Protocol::CSMonthCardFetchDayReward));

	m_msg_handler_list[Protocol:: MT_CSA_QUERY_ACTIVITY_INFO_CS]	= HandlerItem(&MessageHandler::OnCSAQueryActivityInfo, sizeof(Protocol::CSCSAQueryActivityInfo));
	m_msg_handler_list[Protocol:: MT_CSA_ROLE_OPERA_REQ_CS]	= HandlerItem(&MessageHandler::OnCSARoleOperaReq, sizeof(Protocol::CSCSARoleOperaReq));

	m_msg_handler_list[Protocol:: MT_DISCONNECT_REQ_CS]	= HandlerItem(&MessageHandler::OnDisconnectReq, sizeof(Protocol::CSDisconnectReq));
	m_msg_handler_list[Protocol:: MT_ZHUANZHI_REQ_CS]	= HandlerItem(&MessageHandler::OnZhuanzhiReq, sizeof(Protocol::CSZhuanzhiReq));
	m_msg_handler_list[Protocol:: MT_QINGYUAN_CARD_UPGRADE_REQ_CS]	= HandlerItem(&MessageHandler::OnQingyuanCardUpgradeReq, sizeof(Protocol::CSQingyuanCardUpgradeReq));
	m_msg_handler_list[Protocol:: MT_WUSHUANG_OPERA_REQ_CS]	= HandlerItem(&MessageHandler::OnWushuangEquipOperaReq, sizeof(Protocol::CSWushuangEquipOperaReq));
	m_msg_handler_list[Protocol:: MT_HUNSHOU_OPERA_REQ_CS]	= HandlerItem(&MessageHandler::OnHunshouOperaReq, sizeof(Protocol::CSHunshouOperaReq));
	m_msg_handler_list[Protocol:: MT_WUSHUANG_FB_INFO_CS]	= HandlerItem(&MessageHandler::OnWushuangFBInfoReq, sizeof(Protocol::CSWushuangFBInfo));
	m_msg_handler_list[Protocol:: MT_CROSS_START_REQ_CS]	= HandlerItem(&MessageHandler::OnCrossStartReq, sizeof(Protocol::CSCrossStartReq));

	m_msg_handler_list[Protocol:: MT_CARDZU_OPERA_REQ_CS]	= HandlerItem(&MessageHandler::OnCardzuOperaReq, sizeof(Protocol::CSCardzuOperaReq));

	m_msg_handler_list[Protocol:: MT_ZHUANSHENG_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnZhuanShengOpearReq, sizeof(Protocol::CSZhuanShengOpearReq));

	m_msg_handler_list[Protocol::MT_RUNE_SYSTEM_REQ_CS] = HandlerItem(&MessageHandler::OnRuneSystemOperaReq, sizeof(Protocol::CSRuneSystemReq));
	m_msg_handler_list[Protocol::MT_RUNE_SYSTEM_DISPOSE_ONE_KEY_CS] = HandlerItem(&MessageHandler::OnRuneSystemDiscopeOneKey, sizeof(Protocol::CSRuneSystemDisposeOneKey));
	m_msg_handler_list[Protocol::MT_RUNE_TOWER_FETCH_TIME_CS] = HandlerItem(&MessageHandler::OnRuneTowerFetchTime, sizeof(Protocol::CSRuneTowerFetchTime));
	m_msg_handler_list[Protocol::MT_RUNE_SYSTEM_AUTO_FB_CS] = HandlerItem(&MessageHandler::OnRuneTowerAutoFb, sizeof(Protocol::CSRuneTowerAutoFb));

	m_msg_handler_list[Protocol::MT_CROSS_XIULUO_TOWER_BUY_BUFF_CS] = HandlerItem(&MessageHandler::OnCrossXiuluoTowerBuyBuff, sizeof(Protocol::CSCrossXiuluoTowerBuyBuff));
	m_msg_handler_list[Protocol::MT_CROSS_XIULUO_TOWER_SCORE_REWARD_REQ_CS] = HandlerItem(&MessageHandler::OnCrossXiuluoTowerScoreRewardReq, sizeof(Protocol::CSCrossXiuluoTowerScoreRewardReq));
	m_msg_handler_list[Protocol::MT_CROSS_XIULUO_TOWER_BUY_BUFF_REQ_CS] = HandlerItem(&MessageHandler::OnCrossXiuluoTowerBuyBuffReq, sizeof(Protocol::CSCrossXiuluoTowerBuyBuffReq));

	m_msg_handler_list[Protocol::MT_CROSS_1V1_MATCH_REQ_CS]		= HandlerItem(&MessageHandler::OnCross1V1MatchReq, sizeof(Protocol::CSCrossMatch1V1Req));
	m_msg_handler_list[Protocol::MT_CROSS_1V1_FIGHT_READY_CS]	= HandlerItem(&MessageHandler::OnCross1V1FightReady, sizeof(Protocol::CSCross1v1FightReady));
	m_msg_handler_list[Protocol::MT_CROSS_1V1_FETCH_REWARD_REQ_CS]	= HandlerItem(&MessageHandler::OnCross1v1FetchRewardReq, sizeof(Protocol::CSCross1v1FetchRewardReq));
	m_msg_handler_list[Protocol::MT_CROSS_1V1_RING_OPER_CS] = HandlerItem(&MessageHandler::OnCross1v1RingOperReq, sizeof(Protocol::CSCross1v1WearRingReq));
	m_msg_handler_list[Protocol::MT_CROSS_1V1_MATCH_RESULT_REQ_CS] = HandlerItem(&MessageHandler::OnCross1v1MatchResultReq, sizeof(Protocol::CSCross1v1MatchResultReq));
	m_msg_handler_list[Protocol::MT_CROSS_1V1_BUY_TIME_REQ_CS] = HandlerItem(&MessageHandler::OnCross1v1BuyTimeReq, sizeof(Protocol::CSCross1v1BuyTimeReq));

	m_msg_handler_list[Protocol::MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_REQ_CS] = HandlerItem(&MessageHandler::OnCrossMultiuserChallengeMatchingReq, sizeof(Protocol::CSCrossMultiuserChallengeMatchgingReq));
	m_msg_handler_list[Protocol::MT_CROSS_MULTIUSER_CHALLENGE_GET_BASE_SELF_SIDE_INFO_CS] = HandlerItem(&MessageHandler::OnCrossMultuuserChallengeGetBaseSelfSideInfo, sizeof(Protocol::CSCrossMultiuserChallengeGetBaseSelfSideInfo));
	m_msg_handler_list[Protocol::MT_CROSS_MULTIUSER_CHALLENGE_FETCH_DAYCOUNT_REWARD_CS] = HandlerItem(&MessageHandler::OnCrossMultiuserChallengeFetchDaycountReward, sizeof(Protocol::CSCrossMultiuserChallengeFetchDaycountReward));
	m_msg_handler_list[Protocol::MT_CROSS_MULTIUSER_CHALLENGE_CANCLE_MATCHING_CS] = HandlerItem(&MessageHandler::OnCrossMultiuserChallengeCancelMatching, sizeof(Protocol::CSCrossMultiuerChallengeCancelMatching));
	m_msg_handler_list[Protocol::MT_CROSS_MULTIUSER_CHALLENGE_REQ_SIDE_MEMBER_POS_CS] = HandlerItem(&MessageHandler::OnCrossMultiuserChallengeReqSideMemberPos, sizeof(Protocol::CSMultiuserChallengeReqSideMemberPos));
	m_msg_handler_list[Protocol::MT_CROSS_MULTIUSER_CHALLENGE_FETCH_GONGXUN_REWARD_SC] = HandlerItem(&MessageHandler::OnCrossMultiuserChallengeFetchGongxunReward, sizeof(Protocol::CSCrossMultiuserChallengeFetchGongxunReward));
	m_msg_handler_list[Protocol::MT_CROSS_MULTIUSER_CHALLENGE_CARD_OPER_REQ] = HandlerItem(&MessageHandler::OnMultiuserChallengeWearCardReq, sizeof(Protocol::CSMultiuserChallengeWearCardReq));

	m_msg_handler_list[Protocol::MT_CROSS_TUANZHAN_FETCH_SCOR_REWARD_CS] = HandlerItem(&MessageHandler::OnCrossTuanZhanFetchRewardReq, sizeof(Protocol::CSCrossTuanzhanFetchReward));

	m_msg_handler_list[Protocol::MT_MESS_BATTLE_ENTER_REQ_CS] = HandlerItem(&MessageHandler::OnMessBattleEnterReq, sizeof(Protocol::CSMessBattleEnterReq));

	m_msg_handler_list[Protocol::MT_BUILD_TOWER_OPERA_CS] = HandlerItem(&MessageHandler::OnBuildTowerReq, sizeof(Protocol::CSBuildTowerReq));
	m_msg_handler_list[Protocol::MT_BUILD_TOWER_BUY_TIMES_CS] = HandlerItem(&MessageHandler::OnBuildTowerBuyTimes, sizeof(Protocol::CSBuildTowerBuyTimes));

	// 组队塔防
	m_msg_handler_list[Protocol::MT_FB_TEAMTOWERDEFEND_OPREAT_REQ_CS] = HandlerItem(&MessageHandler::OnTeamTowerDefendOpreatReq, sizeof(Protocol::CSTeamTowerDefendOpreatReq));

	m_msg_handler_list[Protocol:: MT_LIEMING_HUNSHOU_OPERA_REQ_CS]	= HandlerItem(&MessageHandler::OnLieMingHunshouOperaReq, sizeof(Protocol::CSLieMingHunshouOperaReq));
	m_msg_handler_list[Protocol:: MT_LIEMING_HUNSHOU_EXCHANGE_CS]	= HandlerItem(&MessageHandler::OnLieMingHunshouExchangeReq, sizeof(Protocol::CSLieMingExchangeList));

	m_msg_handler_list[Protocol::MT_JINGLING_OPER_REQ_CS] = HandlerItem(&MessageHandler::OnJingLingOperReq, sizeof(Protocol::CSJingLingOper));
	m_msg_handler_list[Protocol::MT_JINGLING_HOME_OPER_REQ_CS] = HandlerItem(&MessageHandler::OnJingLingHomeOperReq, sizeof(Protocol::CSJingLingHomeOperReq));
	m_msg_handler_list[Protocol::MT_JINGLING_EXPLORE_OPER_REQ_CS] = HandlerItem(&MessageHandler::OnJinglIngExploreOperReq, sizeof(Protocol::CSJinglIngExploreOperReq));

	m_msg_handler_list[Protocol:: MT_SHENGWANG_OPERA_CS] = HandlerItem(&MessageHandler::OnShengWangOperaReq, sizeof(Protocol::CSShengWangOpera));

	m_msg_handler_list[Protocol:: MT_CHENGJIU_OPERA_CS] = HandlerItem(&MessageHandler::OnChengJiuOperaReq, sizeof(Protocol::CSChengJiuOpera));

	m_msg_handler_list[Protocol::MT_SHENZHOU_WEAPON_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnShenzhouWeaponOperaReq, sizeof(Protocol::CSSHenzhouWeaponOperaReq));
	m_msg_handler_list[Protocol::MT_SHEN_ZHOU_ONE_KEY_IDENTIFY_CS] = HandlerItem(&MessageHandler::OnShenzhouWeaponOneIdentifyReq, sizeof(Protocol::CSSHenzhouWeaponOneKeyIdentifyReq));
	m_msg_handler_list[Protocol::MT_SHEN_ZHOU_HUNYIN_RESOLVE_CS] = HandlerItem(&MessageHandler::OnShenzhouWeaponHunyinResolveReq, sizeof(Protocol::CSShenzhouHunyinResolveReq));

	m_msg_handler_list[Protocol::MT_TUHAOJIN_USE_REQ_CS] = HandlerItem(&MessageHandler::OnUseTuHaoJinReq, sizeof(Protocol::CSUseTuHaoJinReq));
	m_msg_handler_list[Protocol::MT_TUHAOJIN_UP_LEVEL_REQ_CS] = HandlerItem(&MessageHandler::OnTuHaoJinUpLevelReq, sizeof(Protocol::CSTuhaojinUpLevelReq));
	m_msg_handler_list[Protocol::MT_BIG_CHATFACE_UP_LEVEL_REQ_CS] = HandlerItem(&MessageHandler::OnBigFaceUpLevelReq, sizeof(Protocol::CSBigChatFaceUpLevelReq));

	m_msg_handler_list[Protocol:: MT_BABY_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnBabyOperaReq, sizeof(Protocol::CSBabyOperaReq));
	m_msg_handler_list[Protocol:: MT_BABY_UPGRADE_REQ_CS] = HandlerItem(&MessageHandler::OnBabyUpgradeReq, sizeof(Protocol::CSBabyUpgradeReq));
	m_msg_handler_list[Protocol:: MT_BABY_RENAME_REQ_CS] = HandlerItem(&MessageHandler::OnBabyRenameReq, sizeof(Protocol::CSBabyRenameReq));

	m_msg_handler_list[Protocol:: MT_PET_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnPetOperaReq, sizeof(Protocol::CSPetOperaReq));
	m_msg_handler_list[Protocol:: MT_PET_UPGRADE_REQ_CS] = HandlerItem(&MessageHandler::OnPetUpgradeReq, sizeof(Protocol::CSPetUpgradeReq));
	m_msg_handler_list[Protocol:: MT_PET_USE_SPECIAL_IMG_CS] = HandlerItem(&MessageHandler::OnPetUseSpecialImg, sizeof(Protocol::CSPetUseSpecialImg));

	m_msg_handler_list[Protocol::MT_RA_TOTAL_CHARGE_3_INFO_CS] = HandlerItem(&MessageHandler::OnRATotalCharge3RequestInfo, sizeof(Protocol::CSRATotalCharge3Info));

	m_msg_handler_list[Protocol::MT_RA_TOTAL_CHARGE_4_INFO_CS] = HandlerItem(&MessageHandler::OnRATotalCharge4RequestInfo, sizeof(Protocol::CSRATotalCharge4Info));

	m_msg_handler_list[Protocol::MT_CROSS_BOSS_BUY_RELIVE_TIMES_CS] = HandlerItem(&MessageHandler::OnCrossBossBuyReliveTimes, sizeof(Protocol::CSCrossBossBuyReliveTimes));
	m_msg_handler_list[Protocol::MT_CROSS_BOSS_BOSS_INFO_REQ_CS] = HandlerItem(&MessageHandler::OnCrossBossBossInfoReq, sizeof(Protocol::CSCrossBossBossInfoReq));

	m_msg_handler_list[Protocol:: MT_MITAMA_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnMitamaOperaReq, sizeof(Protocol::CSMitamaOperaReq));

	m_msg_handler_list[Protocol:: MT_FAIRY_TREE_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnFairyTreeOperaReq, sizeof(Protocol::CSFairyTreeOperaReq));

	m_msg_handler_list[Protocol::MT_MAGIC_EQUIPMENT_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnMagicEquipmentOperaReq, sizeof(Protocol::CSMagicEquipmentOperaReq));

	m_msg_handler_list[Protocol::MT_XUNZHANG_UPLEVEL_REQ_CS] = HandlerItem(&MessageHandler::OnXunZhangUpLevelReq, sizeof(Protocol::CSXunZhangUplevelReq));

	// 小宠物
	m_msg_handler_list[Protocol::MT_LITTLE_PET_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnLittlePetOperaReq, sizeof(Protocol::CSLittlePetREQ));
	m_msg_handler_list[Protocol::MT_LITTLE_PET_RENAME_CS] = HandlerItem(&MessageHandler::OnLittlePetReName, sizeof(Protocol::CSLittlePetRename));

	// 相思树
	m_msg_handler_list[Protocol::MT_LOVE_TREE_WATER_REQ_CS] = HandlerItem(&MessageHandler::OnLoveTreeWaterReq, sizeof(Protocol::CSLoveTreeWaterReq));
	m_msg_handler_list[Protocol::MT_LOVE_TREE_INFO_REQ_CS] = HandlerItem(&MessageHandler::OnLoveTreeInfoReq, sizeof(Protocol::CSLoveTreeInfoReq));

	// 好友送礼
	m_msg_handler_list[Protocol::MT_FRIEND_GIFT_SONG_GIFT_CS] = HandlerItem(&MessageHandler::OnFriendGiftSongGift, sizeof(Protocol::CSFriendSongGift));
	m_msg_handler_list[Protocol::MT_FRIEND_GIFT_ALL_INFO_GIFT_CS] = HandlerItem(&MessageHandler::OnFriendGiftInfoReq, sizeof(Protocol::CSFriendGiftAllInfoReq));
	m_msg_handler_list[Protocol::MT_FRIEND_HELI_SEND_REQ_CS] = HandlerItem(&MessageHandler::OnFriendHeliRet, sizeof(Protocol::CSFriendHeliSendReq));
	m_msg_handler_list[Protocol::MT_GIVE_ITEM_OPERA_CS] = HandlerItem(&MessageHandler::OnGiveItemOpera, sizeof(Protocol::CSGiveItemOpera));
	m_msg_handler_list[Protocol::MT_GIVE_ITEM_REQ_CS] = HandlerItem(&MessageHandler::OnGiveItemReq, sizeof(Protocol::CSGiveItemReq));

	m_msg_handler_list[Protocol::MT_CROSS_SHUIJING_BUY_BUFF_CS] = HandlerItem(&MessageHandler::OnCrossShuiJingBuyBuff, sizeof(Protocol::CSCrossShuijingBuyBuff));
	m_msg_handler_list[Protocol::MT_SHUIJING_BUY_BUFF_CS] = HandlerItem(&MessageHandler::OnShuiJingBuyBuff, sizeof(Protocol::CSShuijingBuyBuff));
	m_msg_handler_list[Protocol::MT_SHUIJING_FETCH_TASK_REWARD_CS] = HandlerItem(&MessageHandler::OnShuiJingFetchTaskReward, sizeof(Protocol::CSShuijingFetchTaskReward));

	m_msg_handler_list[Protocol::MT_WANGLINGEXPLORE_BUY_BUFF_CS] = HandlerItem(&MessageHandler::OnWanglingExploreBuyBuff, sizeof(Protocol::CSWangLingExploreBuyBuff));
	m_msg_handler_list[Protocol::MT_CROSS_BIANJINGZHIDI_BUY_BUFF_CS] = HandlerItem(&MessageHandler::OnCrossBianjingZhiDiBuyBuff, sizeof(Protocol::CSCrossBianJingZhiDiBuyBuff));
	// 好友经验瓶
	m_msg_handler_list[Protocol::MT_FRIEND_EXP_BOTTLE_OPERATE_REQ_CS] = HandlerItem(&MessageHandler::OnFriendExpBottle, sizeof(Protocol::CSFriendExpBottleOP));

	// 我要脱单
	m_msg_handler_list[Protocol::MT_FRIEND_TUODAN_ALL_INFO_CS] = HandlerItem(&MessageHandler::OnGetAllTuodanInfo, sizeof(Protocol::CSGetAllTuodanInfo));
	m_msg_handler_list[Protocol::MT_FRIEND_TUODAN_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnTuodanReq, sizeof(Protocol::CSTuodanREQ));

	// 攻城战
	m_msg_handler_list[Protocol::MT_GCZ_FETCH_DAILY_REWARD_CS] = HandlerItem(&MessageHandler::OnGongChengZhanFetchDailyRewardReq, sizeof(Protocol::CSGCZFetchDailyRewardReq));

	m_msg_handler_list[Protocol::MT_CHONGZHI_7DAY_FETCH_REWARD_CS] = HandlerItem(&MessageHandler::OnChongZhi7DayFetchReward, sizeof(Protocol::CSChongZhi7DayFetchReward));

	// 神格
	m_msg_handler_list[Protocol::MT_SHENGE_SYSTEM_REQ_CS] = HandlerItem(&MessageHandler::OnShengeSystemOperaReq, sizeof(Protocol::CSShengeSystemReq));

	m_msg_handler_list[Protocol::MT_TUITU_FB_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnTuituFbOperaReq, sizeof(Protocol::CSTuituFbOperaReq));

	// 金猪召唤
	m_msg_handler_list[Protocol::MT_GOLDEN_PIG_OPERATE_REQ_CS] = HandlerItem(&MessageHandler::OnGoldenPigOperate, sizeof(Protocol::CSGoldenPigOperateReq));

	// 神兽
	m_msg_handler_list[Protocol::MT_SHENSHOU_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnShenshouOperaReq, sizeof(Protocol::CSShenshouOperaReq));
	m_msg_handler_list[Protocol::MT_SHENSHOU_REQ_UPGRADE_EQUIP_CS] = HandlerItem(&MessageHandler::OnShenshouReqStrength, sizeof(Protocol::CSSHenshouReqStrength));

	// 形象赋灵
	//m_msg_handler_list[Protocol::MT_IMG_FULING_OPERA_CS] = HandlerItem(&MessageHandler::OnImgFulingOperaReq, sizeof(Protocol::CSImgFulingOperate));

	m_msg_handler_list[Protocol::MT_ELEMENT_HEART_REQ_CS] = HandlerItem(&MessageHandler::OnElementHeartOperaReq, sizeof(Protocol::CSElementHeartReq));

	m_msg_handler_list[Protocol::MT_SHEN_YIN_ONE_KEY_RECYLE_CS] = HandlerItem(&MessageHandler::OnShenYinOneKeyRecyleOperaReq, sizeof(Protocol::CSShenYinOneKeyRecyleReq));
	
	// 飞仙
	m_msg_handler_list[Protocol::MT_FEIXIAN_OP_CS] = HandlerItem(&MessageHandler::OnFeixian, sizeof(Protocol::CSFeixianEquipOpe));


	m_msg_handler_list[Protocol::MT_CROSS_FIND_ROLE_BY_UUID] = HandlerItem(&MessageHandler::ONCrossFindRoleByUUID, sizeof(Protocol::CSCrossFindRoleByUUID));
	
	// 境界
	m_msg_handler_list[Protocol::MT_ROLE_JINGJIE_REQ_CS] = HandlerItem(&MessageHandler::OnRoleJingJieOpera, sizeof(Protocol::CSRoleJingJieReq));

	//河神洛书
	m_msg_handler_list[Protocol::MT_HESHENLUOSHU_REQ_CS] = HandlerItem(&MessageHandler::OnHeShenLuoShuReq, sizeof(Protocol::CSHeShenLuoShuReq));

	// 上古boss
	m_msg_handler_list[Protocol::MT_SHANGGU_BOSS_OPERATE_CS] = HandlerItem(&MessageHandler::OnShangGuBossReq, sizeof(Protocol::CSShangGuBossReq));
	
	// 守护小鬼
	m_msg_handler_list[Protocol::MT_IMP_GURAD_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnImpGuardOperaReq, sizeof(Protocol::CSImpGuardOperaReq));
	
	//情缘排行榜
	m_msg_handler_list[Protocol::MT_RANK_GET_COUPLE_RANK_LIST_CS] = HandlerItem(&MessageHandler::OnGetCoupleRankList, sizeof(Protocol::CSGetCoupleRankList));

	//boss图鉴
	m_msg_handler_list[Protocol::MT_BOSS_CARD_REQ_CS] = HandlerItem(&MessageHandler::OnBossCardReq, sizeof(Protocol::CSBossCardReq));

	//转职
	m_msg_handler_list[Protocol::MT_ROLE_ZHUANZHI_REQ_CS] = HandlerItem(&MessageHandler::OnRoleZhuanZhiReq, sizeof(Protocol::CSRoleZhuanZhiReq));

	//排行榜
	m_msg_handler_list[Protocol::MT_RANK_GET_SPECIAL_RANK_LIST_CS] = HandlerItem(&MessageHandler::OnGetSpecialRankValue, sizeof(Protocol::CSGetSpecialRankValue));

	// 跨服竞技场
	m_msg_handler_list[Protocol::MT_CROSS_CHALLENGE_FIELD_REQ_CS] = HandlerItem(&MessageHandler::OnCrossChallengefieldReq, sizeof(Protocol::CSCrossChallengeFieldOpera));

	// 周末装备
	m_msg_handler_list[Protocol::MT_TIANSHENHUTI_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnTianshenhutiOperaReq, sizeof(Protocol::CSTianshenhutiReq));
	
	// 跨服护送水晶
	m_msg_handler_list[Protocol::MT_CROSS_HUSONG_SHUIJING_REQ_CS] = HandlerItem(&MessageHandler::OnCrossHusongShuijingReq, sizeof(Protocol::CSCrossHusongShuijingOpera));
// ------------------------------------------  根据UID直接定位通讯  ------------------------------------------
	memset(m_on_uid_handler, 0, sizeof(m_on_uid_handler));

	m_on_uid_handler[ggprotocal::MT_FRIEND_RESET_NAME - ggprotocal::MT_GG_BEGIN]				= &MessageHandler::OnFriendResetName;
	m_on_uid_handler[ggprotocal::MT_FRIEND_SET_SEX - ggprotocal::MT_GG_BEGIN]					= &MessageHandler::OnFriendSetSex;
	m_on_uid_handler[ggprotocal::MT_FRIEND_SET_LEVEL - ggprotocal::MT_GG_BEGIN]					= &MessageHandler::OnFriendSetLevel;
	m_on_uid_handler[ggprotocal::MT_ROLE_FORBID_TALK - ggprotocal::MT_GG_BEGIN]					= &MessageHandler::OnForbidTalk;
	m_on_uid_handler[ggprotocal::MT_ON_USER_REMOVE - ggprotocal::MT_GG_BEGIN]					= &MessageHandler::OnGlobalRemoveUser;
	m_on_uid_handler[ggprotocal::MT_MAIL_ATTACHMENT - ggprotocal::MT_GG_BEGIN]					= &MessageHandler::OnMailAttachment;
	m_on_uid_handler[ggprotocal::MT_GUILD_SYNC_ROLE_INFO - ggprotocal::MT_GG_BEGIN]				= &MessageHandler::OnGuildSyncRoleInfo;
	m_on_uid_handler[ggprotocal::MT_SYSTEMCMD_TO_ROLE - ggprotocal::MT_GG_BEGIN]				= &MessageHandler::OnCmdToRole;
	m_on_uid_handler[ggprotocal::MT_GET_ONE_EQUIPMENT_INFO - ggprotocal::MT_GG_BEGIN]			= &MessageHandler::OnGetOneEquipmentInfo;
	m_on_uid_handler[ggprotocal::MT_GET_ONE_PET_INFO - ggprotocal::MT_GG_BEGIN]					= &MessageHandler::OnGetOnePetInfo;
	m_on_uid_handler[ggprotocal::MT_GET_ONE_MOUNT_INFO - ggprotocal::MT_GG_BEGIN]				= &MessageHandler::OnGetOneMountInfo;
	m_on_uid_handler[ggprotocal::MT_TRADE_AFFIRM_ROUTE - ggprotocal::MT_GG_BEGIN]				= &MessageHandler::OnTradeAffirmRoute;
	m_on_uid_handler[ggprotocal::MT_TRADE_SYNC_STATE_ROUTE - ggprotocal::MT_GG_BEGIN]			= &MessageHandler::OnTradeSyncState;
	m_on_uid_handler[ggprotocal::MT_TRADE_CONSUME_REQ - ggprotocal::MT_GG_BEGIN]				= &MessageHandler::OnTradeAffirmSuccReq;
	m_on_uid_handler[ggprotocal::MT_CREATE_GUILD_FAIL_ROLLBACK - ggprotocal::MT_GG_BEGIN]		= &MessageHandler::OnCreateGuildFailRollback;
	m_on_uid_handler[ggprotocal::MT_GUILD_DELATE_FAIL_ROLLBACK - ggprotocal::MT_GG_BEGIN]		= &MessageHandler::OnGuildDelateFailRollback;
	m_on_uid_handler[ggprotocal::MT_GUILD_BOSS_FEED_CONSUME_REQ - ggprotocal::MT_GG_BEGIN]		= &MessageHandler::OnGuildBossFeedConsumeReq;
	m_on_uid_handler[ggprotocal::MT_GUILD_STORAGE_PUT_ITEM_CONSUME_REQ - ggprotocal::MT_GG_BEGIN]	= &MessageHandler::OnGuildStoragePutItemConsumeReq;
	m_on_uid_handler[ggprotocal::MT_GUILD_STORAGE_TAKE_ITEM_GIVE_REQ - ggprotocal::MT_GG_BEGIN]	= &MessageHandler::OnGuildStorageTakeItemGiveReq;
	m_on_uid_handler[ggprotocal::MT_GUILD_XIANGFANG_UPGRADE_LEVEL_CONSUME_REQ - ggprotocal::MT_GG_BEGIN] = &MessageHandler::OnGuildXiangfangUpgradeLevelConsumeReq;
	m_on_uid_handler[ggprotocal::MT_GUILD_JIANZHU_ADDEXP_CONSUME_REQ - ggprotocal::MT_GG_BEGIN] = &MessageHandler::OnGuildJianZhuAddExpConsumeReq;

	m_on_uid_handler[ggprotocal::MT_GUILD_SHOP_BUY_ACK - ggprotocal::MT_GG_BEGIN]				= &MessageHandler::OnGuildShopBuyAck;

	m_on_uid_handler[ggprotocal::MT_SHOP_BUY_LIMIT_ITEM_ACK - ggprotocal::MT_GG_BEGIN]			= &MessageHandler::OnShopBuyLimitItemAck;

	// m_on_uid_handler[ggprotocal::MT_CHONGZHI_NOTICE - ggprotocal::MT_GG_BEGIN]					= &MessageHandler::OnChongzhiNotice;

	m_on_uid_handler[ggprotocal::MT_KILL_OTHER_CAMP_GET_HONOUR - ggprotocal::MT_GG_BEGIN]		= &MessageHandler::OnKillOtherCampGetHonour;

	m_on_uid_handler[ggprotocal::MT_GET_YUANLIFB_CHAPTER_LORD_GIFT - ggprotocal::MT_GG_BEGIN]	= &MessageHandler::OnGetYuanLiFBChapterLordGift;

	m_on_uid_handler[ggprotocal::MT_QUERY_POSITION - ggprotocal::MT_GG_BEGIN]					= &MessageHandler::OnQueryPosition;
	m_on_uid_handler[ggprotocal::MT_RSP_POSITION - ggprotocal::MT_GG_BEGIN]						= &MessageHandler::OnRspPosition;

	m_on_uid_handler[ggprotocal::MT_BUY_PUBLICSALE_ITEM_REQ - ggprotocal::MT_GG_BEGIN]			= &MessageHandler::OnBuyPublicSaleItemReq;

	m_on_uid_handler[ggprotocal::MT_ADD_MOUNT_WITH_DATA - ggprotocal::MT_GG_BEGIN]				= &MessageHandler::OnAddMountWithData;
	m_on_uid_handler[ggprotocal::MT_ADD_PET_WITH_DATA - ggprotocal::MT_GG_BEGIN]				= &MessageHandler::OnAddPetWithData;
	m_on_uid_handler[ggprotocal::MT_FETCH_OPEN_GAME_SUB_ACTIVITY_RANK_REWARD - ggprotocal::MT_GG_BEGIN] = &MessageHandler::OnFetchOpenGameSubActivityRankReward;
	m_on_uid_handler[ggprotocal::MT_XIANMENGZHAN_ENTER_REQ - ggprotocal::MT_GG_BEGIN]			= &MessageHandler::OnXianMengZhanEnterReq;
	m_on_uid_handler[ggprotocal::MT_MARRY_RESERVE_REQ - ggprotocal::MT_GG_BEGIN]				= &MessageHandler::OnMarryReserveReq;
	m_on_uid_handler[ggprotocal::MT_MARRY_HUNYAN_ENTER - ggprotocal::MT_GG_BEGIN]				= &MessageHandler::OnMarryHunyanEnter;
	m_on_uid_handler[ggprotocal::MT_RANDACTIIVITY_BUY_REQ_TO_GS - ggprotocal::MT_GG_BEGIN]				= &MessageHandler::OnXiannvBuyReqToGS;
	m_on_uid_handler[ggprotocal::MT_ACTIVITY_FIND_REQ_TO_GS - ggprotocal::MT_GG_BEGIN]			= &MessageHandler::OnActivityFindReqToGS;

	m_on_uid_handler[ggprotocal::MT_DATING_INVITE_CHECK_TARGET - ggprotocal::MT_GG_BEGIN]		= &MessageHandler::OnDatingInviteCheckTarget;
	m_on_uid_handler[ggprotocal::MT_DATING_CONSUME - ggprotocal::MT_GG_BEGIN]					= &MessageHandler::OnDatingConsume;
	m_on_uid_handler[ggprotocal::MT_DATING_REWARD - ggprotocal::MT_GG_BEGIN]					= &MessageHandler::OnDatingReward;
	m_on_uid_handler[ggprotocal::MT_DATING_UPDATE_ADD_EXP - ggprotocal::MT_GG_BEGIN]			= &MessageHandler::OnDatingUpdateAddExp;
	m_on_uid_handler[ggprotocal::MT_DATING_DAYCOUNT_CHECK - ggprotocal::MT_GG_BEGIN]			= &MessageHandler::OnDatingDaycountCheck;

	m_msg_handler_list[Protocol::MT_HUASHEN_OPERA_REQ_CS]										= HandlerItem(&MessageHandler::OnHuaShenOperaReq, sizeof(Protocol::CSHuaShenOperaReq));
	m_msg_handler_list[Protocol::MT_QINGYUAN_COUPLE_HALO_OPERA_REQ_CS]							= HandlerItem(&MessageHandler::OnQingyuanCoupleHaloOperaReq, sizeof(Protocol::CSQingyuanCoupleHaloOperaReq));
	m_msg_handler_list[Protocol::MT_PASTURE_SPIRIT_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnPastureSpiritOperaReq, sizeof(Protocol::CSPastureSpiritOperaReq));
	m_msg_handler_list[Protocol::MT_MULTI_MOUNT_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnMultiMountOperaReq, sizeof(Protocol::CSMultiMountOperaReq));
	m_msg_handler_list[Protocol::MT_PERSONALIZE_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnPersonalizeWindowOperaReq, sizeof(Protocol::CSPersonalizeWindowOperaReq));
	m_msg_handler_list[Protocol::MT_MAGIC_CARD_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnMagicCardOperaReq, sizeof(Protocol::CSMagicCardOperaReq));
	m_msg_handler_list[Protocol::MT_WUSHANG_EQUIP_REQ_CS] = HandlerItem(&MessageHandler::OnWushangEquipOperaReq, sizeof(Protocol::CSWushangEquipOpearReq));
	m_msg_handler_list[Protocol::MT_QINGYUAN_BLESS_REWARD_REQ_CS] = HandlerItem(&MessageHandler::OnQingyuanFetchBlessRewardReq, sizeof(Protocol::CSQingyuanFetchBlessRewardReq));
	m_msg_handler_list[Protocol::MT_QINGYUAN_ADD_BLESSDAYS_REQ_CS] = HandlerItem(&MessageHandler::OnQingyuanAddBlessDays, sizeof(Protocol::CSQingyuanAddBlessDaysReq));
	m_msg_handler_list[Protocol::MT_LOVE_CONTRACT_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnLoveContractOperaReq, sizeof(Protocol::CSLoveContractOperaReq));
	m_msg_handler_list[Protocol::MT_QINGYUAN_FETCH_LOVE_CONTRACT_CS] = HandlerItem(&MessageHandler::OnQingyuanFetchLoveContract, sizeof(Protocol::CSQingyuanFetchLoveContract));
	
	m_msg_handler_list[Protocol::MT_CROSS_TEAM_FB_OPTION_CS] = HandlerItem(&MessageHandler::OnCrossTeamFBOptionReq, sizeof(Protocol::CSCrossTeamFBOption));

	m_msg_handler_list[Protocol::MT_SKIP_REQ_CS] = HandlerItem(&MessageHandler::OnSkipReq, sizeof(Protocol::CSSkipReq));

	m_msg_handler_list[Protocol::MT_CROSS_DARK_NIGHT_PLAYER_REQ_CS] = HandlerItem(&MessageHandler::OnCrossDarkNightOperaReq,sizeof(Protocol::CSCrossDarkNightRankOpera));
	m_msg_handler_list[Protocol::MT_GET_DROP_LOG_CS] = HandlerItem(&MessageHandler::OnGetDropLog, sizeof(Protocol::CSGetDropLog));

	m_msg_handler_list[Protocol::MT_BABY_BOSS_OPERATE_CS] = HandlerItem(&MessageHandler::OnBabyBossOperate, sizeof(Protocol::CSBabyBossOperate));

	m_msg_handler_list[Protocol::MT_SHENGXINGZHULI_INFO_REQ_CS] = HandlerItem(&MessageHandler::OnGetShengxingzhuliInfoReq, sizeof(Protocol::CSGetShengxingzhuliInfoReq));
	m_msg_handler_list[Protocol::MT_SHENGXINGZHULI_GET_REWARD_REQ_CS] = HandlerItem(&MessageHandler::OnGetShengxingzhuliRewardReq, sizeof(Protocol::CSGetShengxingzhuliRewardReq));

	m_msg_handler_list[Protocol::MT_TALENT_OPERATE_CS] = HandlerItem(&MessageHandler::OnRoleTelentOperate, sizeof(Protocol::CSRoleTelentOperate));
	//m_msg_handler_list[Protocol::MT_JINJIE_TALENT_INFO_REQ_CS] = HandlerItem(&MessageHandler::OnJinJieTalentOperaReqAll, sizeof(Protocol::CSJinJieTalentOperaReqAll));

	m_msg_handler_list[Protocol::MT_SHENGQI_REQ_CS] = HandlerItem(&MessageHandler::OnShengqiOpera, sizeof(Protocol::CSShengqiReq));

	m_msg_handler_list[Protocol::MT_UPGRADE_SYS_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnUpgradeSysOperaReq, sizeof(Protocol::CSUpgradeOperaReq));

	m_msg_handler_list[Protocol::MT_YAOSHI_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnYaoShiOperaReq, sizeof(Protocol::CSYaoShiOperaReq));

	m_msg_handler_list[Protocol::MT_TOUSHI_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnToushiOperaReq, sizeof(Protocol::CSTouShiOperaReq));

	m_msg_handler_list[Protocol::MT_QILINBI_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnQiLinBiOperaReq, sizeof(Protocol::CSQiLinBiOperaReq));

	m_msg_handler_list[Protocol::MT_MASK_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnMaskOperaReq, sizeof(Protocol::CSMaskOperaReq));

	m_msg_handler_list[Protocol::MT_ZHUANZHI_EQUIP_REQ_CS] = HandlerItem(&MessageHandler::OnZhuanzhiEquipOpera, sizeof(Protocol::CSZhuanzhiEquipOpe));

	m_msg_handler_list[Protocol::MT_FISHING_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnFishingOperaReq, sizeof(Protocol::CSFishingOperaReq));

	m_msg_handler_list[Protocol::MT_NIGHT_FIGHT_REQ_CS] = HandlerItem(&MessageHandler::OnNightFightOperaReq, sizeof(Protocol::CSNightFightOperaReq));
	
	m_msg_handler_list[Protocol::MT_SEAL_REQ_CS] = HandlerItem(&MessageHandler::OnSealOperaReq, sizeof(Protocol::CSSealReq));
	m_msg_handler_list[Protocol::MT_SEAL_RECYCLE_REQ_CS] = HandlerItem(&MessageHandler::OnSealRecycleReq, sizeof(Protocol::CSSealRecycleReq));
	// 膜拜城主
	m_msg_handler_list[Protocol::MT_GCZ_WORSHIP_CS] = HandlerItem(&MessageHandler::OnGongChenZhanWorship, sizeof(Protocol::CSGCZWorshipReq));

	m_msg_handler_list[Protocol::MT_WORSHIP_COMMON_CS] = HandlerItem(&MessageHandler::OnRoleWorship, sizeof(Protocol::CSRoleWorshipReq));

	m_msg_handler_list[Protocol::MT_LUCKY_LOG_ITEM_REQ_CS] = HandlerItem(&MessageHandler::OnLuckyLogItemReq, sizeof(Protocol::CSGetLuckyLog));

	m_msg_handler_list[Protocol::MT_JINGLING_ADVANTAGE_BOSS_ENTER_CS] = HandlerItem(&MessageHandler::OnJingLingAdvantageBossEnter, sizeof(Protocol::CSJingLingAdvantageBossEnter));

	// 节日守护(塔防) 
	m_msg_handler_list[Protocol::MT_FB_HOLIDAY_GUARD_ROLE_REQ_CS] = HandlerItem(&MessageHandler::OnHolidayGuardReqAction, sizeof(Protocol::CSHolidayGuardRoleReq));
	m_msg_handler_list[Protocol::MT_RA_HOLIDAY_GUARD_INFO_CS] = HandlerItem(&MessageHandler::OnHolidayGuardRankInfoReq, sizeof(Protocol::CSRAHolidayGuardRankInfoReq));

	m_msg_handler_list[Protocol::MT_RA_TOTAL_CHARGE_5_INFO_CS] = HandlerItem(&MessageHandler::OnRATotalCharge5RequestInfo, sizeof(Protocol::CSRATotalChargeFiveInfo));
	// 衣柜
	m_msg_handler_list[Protocol::MT_DRESSING_ROOM_OPEAR_CS] = HandlerItem(&MessageHandler::OnDressingRoomOperaReq, sizeof(Protocol::CSDressingRoomOpera));
	m_msg_handler_list[Protocol::MT_DRESSING_ROOM_EXCHANGE_CS] = HandlerItem(&MessageHandler::OnDressingRoomExchange, sizeof(Protocol::CSDressingRoomExchange));
	// 角色大小目标
	m_msg_handler_list[Protocol::MT_RA_ROLE_BIG_SMALL_GOAL_CS] = HandlerItem(&MessageHandler::OnRoleBigSmallGoalOper, sizeof(Protocol::CSRoleBigSmallGoalOper));
	//进阶系统大小目标
	m_msg_handler_list[Protocol::MT_JINJIESYS_REWARD_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnJinjiesysRewardOpera, sizeof(Protocol::CSJinjiesysRewardOpera));
	//神器
	m_msg_handler_list[Protocol::MT_SHENQI_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnShenqiOperaReq, sizeof(Protocol::CSShenqiOperaReq));
	//天天返利
	m_msg_handler_list[Protocol::MT_RA_DAY_CHONGZHI_REQ_CS] = HandlerItem(&MessageHandler::OnDayChongzhiRewardReq, sizeof(Protocol::CSDayChongzhiRewardReq));
	// 仙尊卡 
	m_msg_handler_list[Protocol::MT_XIANZUNKA_REQ_CS] = HandlerItem(&MessageHandler::OnXianZunKaOperaReq, sizeof(Protocol::CSXianZunKaOperaReq));
	// 夫妻家园
	m_msg_handler_list[Protocol::MT_SPOUSE_HOME_OPERE_REQ_CS] = HandlerItem(&MessageHandler::OnSpouseHomeOpera, sizeof(Protocol::CSSpouseHomeOperaReq));
	//装备洗炼
	m_msg_handler_list[Protocol::MT_EQUIP_BAPTIZE_REQ_CS] = HandlerItem(&MessageHandler::OnEquipBaptizeOperaReq, sizeof(Protocol::CSEquipBaptizeOperaReq));

	//视野协议广播,客户端自己用特殊操作
	m_msg_handler_list[Protocol::MT_PERSON_AREAMSG_INFO_CS] = HandlerItem(&MessageHandler::OnRolePersonAreaMsgInfoReq, sizeof(Protocol::CSRolePersonAreaMsgInfo));

	//跨服秘藏boss请求
	m_msg_handler_list[Protocol::MT_MIZANG_BOSS_BOSS_INFO_CS] = HandlerItem(&MessageHandler::OnCrossMizangBossBossInfoReq, sizeof(Protocol::CSCrossMiZangBossBossInfoReq));

	//跨服幽冥boss请求
	m_msg_handler_list[Protocol::MT_YOUMING_BOSS_BOSS_INFO_CS] = HandlerItem(&MessageHandler::OnCrossYoumingBossBossInfoReq, sizeof(Protocol::CSCrossYouMingBossBossInfoReq));

	// 随机活动-幸运许愿请求
	m_msg_handler_list[Protocol::MT_RA_LUCKY_WISH_OPERA_CS] = HandlerItem(&MessageHandler::OnRALuckyWishOpera, sizeof(Protocol::CSLuckyWishOpera));

	//请求购买双倍奖励
	m_msg_handler_list[Protocol::MT_FETCH_DOUBLE_REWARD_CS] = HandlerItem(&MessageHandler::OnFetchDoubleReward, sizeof(Protocol::CSFetchDoubleRewardReq));

	//个人boss信息请求
	m_msg_handler_list[Protocol::MT_PERSON_BOSS_INFO_CS] = HandlerItem(&MessageHandler::OnPersonBossReq, sizeof(Protocol::CSPersonBossInfoReq));

	// 名将
	m_msg_handler_list[Protocol::MT_GREATE_SOLDIER_OPERA_CS] = HandlerItem(&MessageHandler::OnGreateSoldierOperaReq, sizeof(Protocol::CSGreateSoldierOpera));
	m_msg_handler_list[Protocol::MT_GREATE_SOLDIER_UPGREADE_EQUIP_CS] = HandlerItem(&MessageHandler::OnGreateSoldierUpgradeEquipReq, sizeof(Protocol::CSGreateSoldierReqStrength));

	// 跨服活动请求
	m_msg_handler_list[Protocol::MT_CROSS_RA_REQUEST_OPERA_CS] = HandlerItem(&MessageHandler::OnCrossRandActivityRequest, sizeof(Protocol::CSCrossRandActivityRequest));

	//直升丹购买
	m_msg_handler_list[Protocol::MT_UPGRADE_CARD_BUY_REQ_CS] = HandlerItem(&MessageHandler::OnUpgradeCardBuyReq, sizeof(Protocol::CSUpgradeCardBuyReq));

	//神魔boss
	m_msg_handler_list[Protocol::MT_GODMAGIC_BOSS_INFO_REQ_CS] = HandlerItem(&MessageHandler::OnGodmagicBossOperate, sizeof(Protocol::CSGodmagicBossInfoReq));

	// 天神护体
	m_msg_handler_list[Protocol::MT_HUGUOZHILI_REQ_CS] = HandlerItem(&MessageHandler::OnHuguozhiliReq, sizeof(Protocol::CSHuguozhiliReq));

	// 今日主题
	m_msg_handler_list[Protocol::MT_TODAY_THEME_REWARD_REQ_CS] = HandlerItem(&MessageHandler::OnTodayThemeRewardReq, sizeof(Protocol::CSTodayThemeRewardReq));
	
	// 全服收购
	m_msg_handler_list[Protocol::MT_WORLD_ACQUISITION_LOG_REQ_CS] = HandlerItem(&MessageHandler::OnWorldAcquisitionLogReq, sizeof(Protocol::CSWorldAcquisitionLogReq));

	// 跨服目标
	m_msg_handler_list[Protocol::MT_CROSS_GOAL_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnCrossGoalReq,sizeof(Protocol::CSCrossGoalOperaReq));

	// 百战装备
	m_msg_handler_list[Protocol::MT_BAIZHAN_EQUIP_REQ_CS] = HandlerItem(&MessageHandler::OnBaizhanEquipOpera, sizeof(Protocol::CSBaizhanEquipOpera));

	// 斗气装备 
	m_msg_handler_list[Protocol::MT_CROSSEQUIP_REQ_CS] = HandlerItem(&MessageHandler::OnCSCrossEquipOpera, sizeof(Protocol::CSCrossEquipOpera));

	// 小助手
	m_msg_handler_list[Protocol::MT_LITTLE_HELPER_REQ_CS] = HandlerItem(&MessageHandler::OnLittleHelperOpera, sizeof(Protocol::CSLittleHelperOpera));

	//仙盟珍稀日志
	m_msg_handler_list[Protocol::MT_GUILD_RARE_LOG_RQE_CS] = HandlerItem(&MessageHandler::OnGuildRareLogItemReq, sizeof(Protocol::CSGetGuildRareLog));

	m_msg_handler_list[Protocol::MT_TIANSHUXZ_FETCH_REWARD_CS] = HandlerItem(&MessageHandler::OnTianShuXZFetchReward, sizeof(Protocol::CSTianShuXZFetchReward));

	//等级装备套装激活
	m_msg_handler_list[Protocol::MT_EQUIP_GRADE_SUIT_ACTIVE_CS] = HandlerItem(&MessageHandler::OnActiveEquipGradeSuitReq, sizeof(Protocol::CSEquipUplevelSuitActive));

	//套装合成
	m_msg_handler_list[Protocol::MT_ZHUANZHI_EQUIP_COMPOSE_REQ_CS] = HandlerItem(&MessageHandler::OnZhuanzhiEquipComposeReq, sizeof(Protocol::CSZhuanzhiEquipCompose));

	//特惠商店
	m_msg_handler_list[Protocol::MT_DISCOUNTSHOP_BUY_CS] = HandlerItem(&MessageHandler::OnDiscountShopReq, sizeof(Protocol::CSDiscountShopBuy));

	//小助手完成全部
	m_msg_handler_list[Protocol::MT_LITTLE_HELPER_REPEAT_REQ_CS] = HandlerItem(&MessageHandler::OnLittleHelperRepeatOpera, sizeof(Protocol::CSLittleHelperRepeatOpera));
	// 神话
	m_msg_handler_list[Protocol::MT_MYTH_OPERA_REQ_CS] = HandlerItem(&MessageHandler::OnMythOperaReq, sizeof(Protocol::CSMythOpera));
	// 星象
	m_msg_handler_list[Protocol::MT_ZODIAC_REQ_CS] = HandlerItem(&MessageHandler::OnZodiacReq, sizeof(Protocol::CSZodiacReq));
	m_msg_handler_list[Protocol::MT_ZODIAC_DECOMPOSE_REQ_CS] = HandlerItem(&MessageHandler::OnZodiacDecomposeReq, sizeof(Protocol::CSZodiacDecomposeReq));
}

MessageHandler::~MessageHandler()
{
	delete m_serialize_message_handler;
}

void MessageHandler::Init(SceneManager * scene_manager)
{
	assert(NULL != scene_manager);
	m_scene_manager = scene_manager;
	m_serialize_message_handler->Init(scene_manager);
}

void MessageHandler::Release()
{
	m_scene_manager = NULL;
	m_serialize_message_handler->Release();
}

bool MessageHandler::HandleMessage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, const char *msg, unsigned int length)
{
	Protocol::MessageHeader *header = (Protocol::MessageHeader*)msg;

	unsigned short msg_type = header->msg_type;

	{
		// crossserver 协议 转发
		if (msg_type >= Protocol::CROSS_SERVER_MSG_BEGIN && msg_type < Protocol::CROSS_SERVER_MSG_END)		
		{
			//InternalComm::Instance().UserMsgTransmitToCross(role->GetUserId(), msg_type, msg, length);
			CrossManager::Instance().UserMsgTransmitToCross(role->GetUserId(), msg_type, msg, length);

			return false;
		}
	}

	// 新一代自动生成协议的Handle
	if (msg_type >= Protocol::MT_AUTO_PROTOCOL_RESERVE_MIN && msg_type < Protocol::MT_AUTO_PROTOCOL_RESERVE_MAX)
	{
		const char *s_msg = msg + sizeof(Protocol::MessageHeader);
		int s_length = length - sizeof(Protocol::MessageHeader);
		m_serialize_message_handler->HandleMessage(netid, scene, role, msg_type, s_msg, s_length);
		return false;
	}

	if (NULL == m_msg_handler_list[msg_type].func)
	{
		// printf("MessageHandler::HandleMessage NULL == m_msg_handler_list[msg_type].func Error type:%d \n", (int)msg_type);
		// EngineAdapter::Instance().NetDisconnect(netid);
		// gamelog::g_log_disconnect.printf(LL_CRITICAL, "HandleMessage Unknow msg[%d].", (int)header->msg_type);

		return false;
	}

	unsigned int msg_size = static_cast<unsigned int>(m_msg_handler_list[msg_type].data_size);

	if ((Protocol::MT_MAIL_SEND_CS == msg_type || 
		Protocol::MT_ITEM_COMPOSE_CS == msg_type ||
		Protocol::MT_BLUEPRINT_COMPOSE_CS == msg_type ||
		Protocol::MT_EQUIP_UPQUALITY_REQ_CS == msg_type ||
		Protocol::MT_EQUIP_UPLEVEL_REQ_CS == msg_type ||
		Protocol::MT_LOUD_SPEAKER_CS == msg_type ||
		Protocol::MT_PICK_ITEM_CS == msg_type ||
		Protocol::MT_KICKOUT_GUILD_CS == msg_type ||
		Protocol::MT_APPLY_FOR_JOIN_GUILD_ACK_CS == msg_type ||
		Protocol::MT_GUILD_MAIL_ALL_CS == msg_type || 
		Protocol::MT_CONVERT_SCORE_TO_ITEM_CS == msg_type || 
		Protocol::MT_MARRY_HUNYAN_BLESS_CS == msg_type ||
		Protocol::MT_PUBLICSALE_SEARCH_CS == msg_type ||
		Protocol::MT_RUNE_SYSTEM_DISPOSE_ONE_KEY_CS == msg_type ||
		Protocol::MT_SHENGE_SYSTEM_REQ_CS == msg_type ||
		Protocol::MT_GUILD_STORGE_ONE_KEY_REQ_CS == msg_type ||
		Protocol::MT_SHEN_ZHOU_ONE_KEY_IDENTIFY_CS == msg_type ||
		Protocol::MT_SHEN_ZHOU_HUNYIN_RESOLVE_CS == msg_type ||
		Protocol::MT_SHENSHOU_REQ_UPGRADE_EQUIP_CS == msg_type ||
		Protocol::MT_SHEN_YIN_ONE_KEY_RECYLE_CS == msg_type ||
		Protocol::MT_BATCH_DICARD_ITEM_CS == msg_type || 
		Protocol::MT_GREATE_SOLDIER_UPGREADE_EQUIP_CS == msg_type ||
		Protocol::MT_PERSON_AREAMSG_INFO_CS == msg_type ||
		Protocol::MT_GIVE_ITEM_REQ_CS == msg_type) ? length <= msg_size : length >= msg_size)
	{
		(this->*m_msg_handler_list[msg_type].func)(netid, scene, role, obj_id, (void*)msg, length);

#ifdef _DEBUG
		// 打印协议号，用于调试
		if (msg_type != Protocol::MT_SPEEDUP_HELLO_CS
			&& msg_type != Protocol::MT_TIME_REQ_CS
			&& msg_type != Protocol::MT_GET_ALL_OBJ_MOVE_INFO_REQ_CS
			&& msg_type != Protocol::MT_HOTKEY_INFO_REQ_CS
			&& msg_type != Protocol::MT_CHANGE_HOTKEY_CS
			&& msg_type != Protocol::MT_OBJ_MOVE_CS)
		{
			if (is_print_msg_type_from_client)
			{
				static unsigned msg_cnt = 0;
				printf("[DEBUG] New user msg[%u], user[%d], msg_type[%d], msg_length[%u]\n",
					++msg_cnt, role->GetUID(), (int)msg_type, length);
			}
		}
#endif

		return true;
	}
	else
	{
		printf("MessageHandler::HandleMessage msg_size Error type:%d \n", (int)msg_type);
		// EngineAdapter::Instance().NetDisconnect(netid);
		gamelog::g_log_disconnect.printf(LL_CRITICAL, "HandleMessage msg[%d] length[%d] is not %d.", (int)header->msg_type, length, msg_size);

		return false;
	}
}

void MessageHandler::HandleMessageNoLogin(const GSNetID &netid, IP ip, const char *msg, unsigned int length)
{
	Protocol::MessageHeader *header = (Protocol::MessageHeader*)msg;

	switch (header->msg_type)
	{
	case Protocol::MT_ENTER_GS_REQ_CS:
		{
			if (length == sizeof(Protocol::CSUserEnterGSReq))
			{
				this->OnReqEnter(netid, ip, (Protocol::CSUserEnterGSReq*)msg);
			}
			else 
			{
				// gamelog::g_log_messagehandler.printf(LL_CRITICAL, "HandleMessageNoLogin Unknow msg type:%d length:%d.", (int)header->msg_type, length);
				// EngineAdapter::Instance().NetDisconnect(netid);
			}
		}
		break;

	case Protocol::MT_TIME_REQ_CS:
		{
			Protocol::SCTimeAck ta;
			ta.server_time = (unsigned int)EngineAdapter::Instance().Time();
			ta.server_real_start_time = (unsigned int)SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime();
			ta.server_real_combine_time = (unsigned int)SHAREDCONFIG->GetOpenServerConfig().GetServerRealCombineTime();
			ta.open_days = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;
			EngineAdapter::Instance().NetSend(netid, (const char *)&ta, sizeof(Protocol::SCTimeAck));
		}
		break;

	default:
		{
			// gamelog::g_log_messagehandler.printf(LL_CRITICAL, "HandleMessageNoLogin Unknow msg type:%d.", (int)header->msg_type);
			// EngineAdapter::Instance().NetDisconnect(netid);
		}
	}
}

void MessageHandler::OnGreateSoldierOperaReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSGreateSoldierOpera* req = (Protocol::CSGreateSoldierOpera*)msg;
	GreateSoldier* greate_soldier = GREATE_SOLDIER(role);
	if (nullptr == greate_soldier)
	{
		return;
	}

	switch (req->req_type)
	{
	case Protocol::GREATE_SOLDIER_REQ_TYPE_INFO:
	{
		greate_soldier->SendAllInfo();
	}
	break;

	case Protocol::GREATE_SOLDIER_REQ_TYPE_LEVEL_UP:
	{
		greate_soldier->OnLevelUp(req->param_1);
	}
	break;

	case Protocol::GREATE_SOLDIER_REQ_TYPE_BIANSHEN:
	{
		greate_soldier->OnBianshen();
	}
	break;

	case Protocol::GREATE_SOLDIER_REQ_TYPE_WASH_POTENTIAL:
	{
		greate_soldier->OnUpgradePotential(req->param_1);
	}
	break;

	case Protocol::GRAETE_SOLDIER_REQ_TYPE_CONFIRM_WASH_POTENTIAL:
	{
		//greate_soldier->OnConfirmWashPotential(req->param_1);
	}
	break;

	case Protocol::GREATE_SOLDIER_REQ_TYPE_PUTON:
	{
		greate_soldier->OnPuton(req->param_1, req->param_2);
	}
	break;

	case Protocol::GREATE_SOLDIER_REQ_TYPE_PUTOFF:
	{
		greate_soldier->OnPutoff(req->param_1);
	}
	break;

	case Protocol::GREATE_SOLDIER_REQ_TYPE_SLOT_LEVEL_UP:
	{
		bool ret = false;
		const int old_slot_level = greate_soldier->GetSlotLevel(req->param_1);
		int now_slot_level = 0;

		for (int i = 0; i < req->param_2 && i < 100; ++i)
		{
			ret = greate_soldier->OnSlotLevelUp(req->param_1);
			now_slot_level = greate_soldier->GetSlotLevel(req->param_1);
			if (!ret || now_slot_level != old_slot_level)
			{
				role->SendOperateResult(Protocol::SCOperateResult::OP_GREATE_SOLDIER_SLOT_UPLEVEL, 0);
				return;
			}
		}

		if (req->param_2 > 0)
		{
			role->SendOperateResult(Protocol::SCOperateResult::OP_GREATE_SOLDIER_SLOT_UPLEVEL, 1);
		}
	}
	break;

	case Protocol::GREATE_SOLDIER_REQ_TYPE_DRAW:
	{
		greate_soldier->OnDraw(req->param_1, req->param_2 == 1);
	}
	break;

	case Protocol::GRAETE_SOLDIER_REQ_TYPE_WASH_ATTR:
	{
		//bool ret = greate_soldier->OnWashAttr(req->param_1);
		//role->SendOperateResult(Protocol::SCOperateResult::OP_GREATE_SOLDIER_WASH_ATTR, ret);
	}
	break;

	case Protocol::GRAETE_SOLDIER_REQ_TYPE_BIANSHEN_TRIAL:
	{
		greate_soldier->OnBianshenTrial(req->param_1);
	}
	break;

	case Protocol::GRAETE_SOLDIER_REQ_TYPE_FETCH_REWARD:
	{
		greate_soldier->OnFetchLimitDrawReward(req->param_1);
	}
	break;

	case Protocol::GRAETE_SOLDIER_REQ_TYPE_FETCH_GOAL_REWARD:
	{
		greate_soldier->GetGoal().OnFetchReward(req->param_1);
	}
	break;

	case Protocol::GRAETE_SOLDIER_REQ_TYPE_BUY_GOAL_REWARD:
	{
		greate_soldier->GetGoal().OnBuyReward(req->param_1);
	}
	break;

	case Protocol::GREATE_SOLDIER_REQ_TYPE_USE_HUANHUA_ID:
	{
		greate_soldier->OnUseHuanhuaID(req->param_1);
	}
	break;

	case Protocol::GREATE_SOLDIER_REQ_TYPE_EXCHANGE:
	{
		greate_soldier->OnExchangeSeq(req->param_1);
	}
	break;

	case Protocol::GREATE_SOLDIER_REQ_TYPE_PUTOFF_EQUIPMENT:
	{
		greate_soldier->TakeOffEquip(req->param_1, req->param_2, false);
	}
	break;

	}
}

void MessageHandler::OnGreateSoldierUpgradeEquipReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSGreateSoldierReqStrength *ssrs = (Protocol::CSGreateSoldierReqStrength *)msg;
	GreateSoldier* greate_soldier = GREATE_SOLDIER(role);
	if (nullptr == greate_soldier)
	{
		return;
	}

	// 修正长度，防止客户端恶意发送数据
	int arr_len = length - (sizeof(Protocol::CSGreateSoldierReqStrength) - sizeof(ssrs->destroy_backpack_index_list));
	ssrs->destroy_num = short(arr_len / sizeof(ssrs->destroy_backpack_index_list[0]));

	if (ssrs->destroy_num > 0)
	{
		greate_soldier->UpgradeEquip(ssrs->seq, ssrs->equip_index, ssrs->destroy_num, ssrs->destroy_backpack_index_list);
	}
}

void MessageHandler::OnCrossRandActivityRequest(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCrossRandActivityRequest *rar = (Protocol::CSCrossRandActivityRequest *)msg;
	role->GetRoleCrossRandActivity()->OnClientRequest(rar);
}

//////////////////////////////////////////////////////////////////////////

void MessageHandler::HandleUIDMessage(const UserID &user_id, Scene *scene, ObjID obj_id, const char *msg, unsigned int length)
{
	ggprotocal::MessageHeader *header = (ggprotocal::MessageHeader*)msg;

	int msg_type_index = header->msg_type - ggprotocal::MT_GG_BEGIN;
	if (msg_type_index < 0 || msg_type_index >= static_cast<int>(sizeof(m_on_uid_handler) / sizeof(m_on_uid_handler[0])) || 
		NULL == m_on_uid_handler[msg_type_index])
	{
		gamelog::g_log_messagehandler.printf(LL_CRITICAL, "HandleUIDMessage Unknow msg type:%d.", (int)header->msg_type);
		return;
	}

	Role *role = this->GetRoleHelper(scene, obj_id, "MessageHandler::HandleUIDMessage");
	if (NULL == role)
	{
		return;
	}

	(this->*(m_on_uid_handler[msg_type_index]))(user_id, scene, role, obj_id, (void*)msg);
}