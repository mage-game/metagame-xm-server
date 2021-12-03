#include "servercommon/userprotocal/systemmsgcode.h"
#include "servercommon/userprotocal/msgheader.h"
#include "servercommon/userprotocal/msgcode.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/userprotocal/globalmsgcode.h"
#include "servercommon/userprotocal/chatmsgcode.h"
#include "servercommon/msgcommonstruct.h"

#include "msgwushuang.h"
#include "msgentergs.h"
#include "msgscene.h"
#include "msgfb.h"
#include "msgrole.h"
#include "msgitem.h"
#include "msgnpc.h"
#include "msghotkey.h"
#include "msgmoney.h"
#include "msgfriend.h"
#include "msgother.h"
#include "msgroleother.h"
#include "msggm.hpp"
#include "msgmail.hpp"
#include "msgtrade.h"
#include "msgcspublicsale.hpp"
#include "msgmentality.hpp"
#include "msgmount.hpp"
#include "msgfightmount.hpp"
#include "msgwing.hpp"
#include "msghalo.hpp"
#include "msgshengong.hpp"
#include "msgshenyi.hpp"
#include "msgfootprint.hpp"
#include "msgcloak.hpp"
#include "msgxiannvshouhu.hpp"
#include "msgjinglingfazhen.hpp"
#include "msgjinglingguanghuan.hpp"
#include "msgzhuanshengequip.h"
#include "msgguild.hpp"
#include "msgactivity.hpp"
#include "msgactivedegree.hpp"
#include "msgdaycounter.hpp"
#include "msgsublock.hpp"
#include "msgchatctrl.h"
#include "msgchatmsg.h"
#include "msgshop.h"
#include "msgtitle.hpp"
#include "msgscmail.hpp"
#include "msgteam.hpp"
#include "msgscrank.h"
#include "msgcsrank.h"
#include "msgscguild.hpp"
#include "msgscpublicsale.h"
#include "msgserver.h"
#include "msgcscamp.hpp"
#include "msgsccamp.hpp"
#include "msgfishpool.h"
#include "msgworldevent.h"
#include "msgcardzu.h"
#include "msglieming.h"
#include "msgshengwang.h"
#include "msgchengjiu.h"
#include "msgtuhaojin.h"
#include "msgbigchatface.h"
#include "msgshenzhouweapon.h"
#include "msgbaby.h"
#include "msgpet.h"
#include "msghuashen.h"
#include "msgpasturespirit.h"
#include "msgmultimount.h"
#include "msgpersonalizewindow.h"
#include "msgmagiccard.h"
#include "msgwushangequip.h"
#include "msgmitama.h"
#include "msgfairytree.h"
#include "msgmagicequipment.h"
#include "msgxunzhang.h"
#include "msgchinesezodiac.h"
#include "msglittlepet.h"
#include "msglovetree.h"
#include "msgmojie.hpp"
#include "msgfriendgift.hpp"
#include "msgtuodan.h"
#include "msgshenbing.hpp"
#include "msgequipment.h"
#include "msgjingling.h"
#include "msgrunesystem.h"
#include "msgxiannv.h"
#include "msgfight.h"
#include "msgfighting.h"
#include "msgfamousman.h"
#include "msgshengesystem.h"
#include "msgshenshou.h"
#include "msgimgfuling.hpp"
#include "msgelementheart.h"
#include "msgshengxingzhuli.h"
#include "msgfeixian.hpp"
#include "msgjinjietalent.hpp"
#include "msgheshenluoshu.h"
#include "msgshengqi.hpp"
#include "msgimpguard.h"
#include "msgzhuanzhi.h"
#include "msgupgrade.hpp"
#include "msgyaoshi.hpp"
#include "msgtoushi.hpp"
#include "msgqilinbi.hpp"
#include "msgmask.hpp"
#include "msgzhuanzhiequip.hpp"
#include "msgseal.hpp"
#include "msgdressingroom.hpp"
#include "msgshenqi.hpp"
#include "msgspousehome.hpp"
#include "msggreatesoldier.h"
#include "msgcsrandactivity.hpp"
#include "msgcrossequip.h"
#include "msgmyth.h"
#include "msgzodiac.hpp"
#include "msgtianshenhuti.h"

namespace Protocol
{
	SCTimeAck::SCTimeAck():header(MT_TIME_ACK_SC){}
	SCDisconnectNotice::SCDisconnectNotice():header(MT_DISCONNECT_NOTICE_SC){}
	CSTimeReq::CSTimeReq():header(MT_TIME_REQ_CS){}

	SCNoticeNum::SCNoticeNum():header(MT_SYSTEM_NOTICE_CODE_SC){}
	SCSystemMsg::SCSystemMsg():header(MT_SYSTEM_MSG_SC){}
	SCZeroHour::SCZeroHour():header(MT_SYSTEM_ZERO_HOUR_SC){}
	SCSpeaker::SCSpeaker():header(MT_SYSTEM_SPEAKER_SC){}

	CSUserEnterGSReq::CSUserEnterGSReq():header(MT_ENTER_GS_REQ_CS){}
	SCUserEnterGSAck::SCUserEnterGSAck():header(MT_ENTER_GS_ACK_SC){}
	CSUserLogout::CSUserLogout():header(MT_LOGOUT_CS){}
	SCChangeGS::SCChangeGS():header(MT_CHANGE_GS_SC){}
	SCServerBusy::SCServerBusy():header(MT_SERVER_BUSY_SC){}
	CSTransportReq::CSTransportReq():header(MT_TRANSPORT_REQ){}
	CSHeartBeat::CSHeartBeat():header(MT_HEARTBEAT_CS){}

	SCEnterScene::SCEnterScene():header(MT_ENTER_SCENE_SC){}
	SCLeaveScene::SCLeaveScene():header(MT_LEAVE_SCENE_SC){}

	SCVisibleObjEnterRole::SCVisibleObjEnterRole():header(MT_VISIBLE_ENTER_ROLE_SC){}
	SCVisibleObjEnterMonster::SCVisibleObjEnterMonster():header(MT_VISIBLE_ENTER_MONSTER_SC){}
	SCVisibleObjEnterPet::SCVisibleObjEnterPet():header(MT_VISIBLE_ENTER_PET_SC){}
	SCVisibleObjEnterFalling::SCVisibleObjEnterFalling():header(MT_VISIBLE_ENTER_FALLING_SC){}	
	SCVisibleObjEnterGather::SCVisibleObjEnterGather():header(MT_VISIBLE_ENTER_GATHER_OBJ_SC){}
	SCVisibleObjEnterWorldEventObj::SCVisibleObjEnterWorldEventObj():header(MT_VISIBLE_ENTER_WORLD_EVENT_OJB_SC){}
	SCVisibleObjEnterEffect::SCVisibleObjEnterEffect():header(MT_VISIBLE_ENTER_EFFECT_OBJ_SC){}
	SCVisibleObjEnterBigCoin::SCVisibleObjEnterBigCoin():header(MT_VISIBLE_ENTER_BIG_COIN_OBJ_SC) {}
	SCVisibleObjEnterBattleFieldShenShi::SCVisibleObjEnterBattleFieldShenShi() : header(MT_VISIBLE_ENTER_BATTLEFIELD_SHENSHI_SC) {}
	SCVisibleObjEnterTrigger::SCVisibleObjEnterTrigger():header(MT_VISIBLE_ENTER_TRIGGER_OBJ_SC) {}
	SCVisibleObjEnterMonsterHalo::SCVisibleObjEnterMonsterHalo():header(MT_VISIBLE_ENTER_MONSTER_HALO_OBJ_SC) {}
	SCVisibleObjEnterMarryObj::SCVisibleObjEnterMarryObj():header(MT_VISIBLE_ENTER_MARRY_OBJ_SC) {}
	SCVisibleObjEnterRoleShadow::SCVisibleObjEnterRoleShadow() : header(MT_VISIBLE_ENTER_ROLE_SHADOW_OBJ_SC) {}
	SCGatherChange::SCGatherChange() : header(MT_GATHER_INFO_CHANGE_SC) {}
	CSFeixianEquipOpe::CSFeixianEquipOpe() : header(MT_FEIXIAN_OP_CS) {}
	SCFeixianEquipInfo::SCFeixianEquipInfo() : header(MT_FEIXIAN_EQUIP_INFO_SC) {}

	SCVisibleObjLeave::SCVisibleObjLeave():header(MT_VISIBLE_LEAVE_SC){}
	SCSceneMonsterDie::SCSceneMonsterDie():header(MT_SCENE_MONSTER_DIE_SC){}
	SCObjMove::SCObjMove():header(MT_OBJ_MOVE_SC){}
	CSObjMove::CSObjMove():header(MT_OBJ_MOVE_CS){}
	SCResetPost::SCResetPost():header(MT_RESET_POST_SC){}
	SCFixPos::SCFixPos():header(MT_FIX_POS_SC){}
	SCActionReject::SCActionReject():header(MT_ACTION_REJECT_SC){}
	SCStartGather::SCStartGather():header(MT_GATHER_BEGIN_SC){}
	SCStopGather::SCStopGather():header(MT_GATHER_STOP_SC){}
	SCStartGatherTimer::SCStartGatherTimer():header(MT_GATHER_TIMER_SC){}
	SCHookStatus::SCHookStatus():header(MT_HOOK_STATUS_SC){}
	CSStartGatherReq::CSStartGatherReq():header(MT_START_GATHER_REQ_CS){}
	CSStopGatherReq::CSStopGatherReq():header(MT_STOP_GATHER_REQ_CS){}
	SCRestStatus::SCRestStatus():header(MT_REST_STATUS_SC){}
	CSRestReq::CSRestReq():header(MT_REST_REQ_CS){}
	CSInviteRestReq::CSInviteRestReq():header(MT_REST_INVITE_CS){}
	SCRestInviteNotify::SCRestInviteNotify():header(MT_REST_INVITE_NOTIFY_SC){}
	CSInviteRestAck::CSInviteRestAck():header(MT_REST_INVITE_ACK_CS){}
	CSRestStartAffirm::CSRestStartAffirm():header(MT_REST_START_AFFIRM_CS){}
	SCRestInviteResult::SCRestInviteResult():header(MT_REST_INVITE_RESULT_SC){}
	CSTeamCallTogether::CSTeamCallTogether():header(MT_TEAM_CALL_TOGETHER_CS){}
	SCTeamCallTogether::SCTeamCallTogether():header(MT_TEAM_CALL_TOGETHER_SC){}
	CSTeamCallIn::CSTeamCallIn():header(MT_TEAM_CALL_IN_CS){}
	SCTeamCallIn::SCTeamCallIn():header(MT_TEAM_CALL_IN_SC){}
	SCUserSceneSkillStatus::SCUserSceneSkillStatus():header(MT_USER_SCENE_SKILL_STATUS_SC){}
	SCAllObjMoveInfo::SCAllObjMoveInfo():header(MT_ALL_OBJ_MOVE_INFO_SC){}
	
	SCObjMoveMode::SCObjMoveMode():header(MT_OBJ_MOVE_MODE_SC){}
	SCRoleAccetpTaskAppearn::SCRoleAccetpTaskAppearn() : header(MT_ROLE_TASK_APPEARN_SC) {}
	SCFBFollowNPCInfo::SCFBFollowNPCInfo():header(MT_FB_FOLLOW_NPC_INFO_SC){}
	SCTriggerNotify::SCTriggerNotify():header(MT_TRIGGER_NOTIFY_SC){}
	CSPickBigCoin::CSPickBigCoin():header(MT_PICK_BIG_COIN_CS) {}
	SCPickBigCoinSucc::SCPickBigCoinSucc():header(MT_PICK_BIG_COIN_SUCC_SC) {}
	SCChangeSceneNotify::SCChangeSceneNotify():header(MT_CHANGE_SCENE_NOTIFY_SC){}
	SCYanhuaExplode::SCYanhuaExplode():header(MT_YANHUA_EXPLODE_SC) {}
	CSReqTeamMemberPos::CSReqTeamMemberPos():header(MT_REQ_TEAM_MEMBER_POS_CS) {}
	SCTeamMemberPosList::SCTeamMemberPosList():header(MT_REQ_TEAM_MEMBER_POS_LIST_SC) {}
	SCBossDpsFlag::SCBossDpsFlag():header(MT_BOSS_DPS_FLAG_SC) {}
	CSChangeSceneLineReq::CSChangeSceneLineReq():header(MT_CHANGE_SCENE_LINE_REQ_CS) {}
	SCGatherBeGather::SCGatherBeGather() : header(MT_GATHER_BE_GATHER_SC) {}

	///////////////////////////////////////////////// 飞行高度变化  //////////////////////////////////////////////////	
	CSObjFlyHeightChange::CSObjFlyHeightChange() : header(MT_OBJ_FLY_HEIGHT_CHANGE_CS) {}
	SCObjFlyHeightChange::SCObjFlyHeightChange() : header(MT_OBJ_FLY_HEIGHT_CHANGE_SC) {}

	///////////////////////////////////////////////// boss首刀  //////////////////////////////////////////////////	
	SCBossFirstHurtInfo::SCBossFirstHurtInfo() : header(MT_BOSS_FIRST_HIT_FLAG_SC) {}
	SCMonsterFirstHitInfo::SCMonsterFirstHitInfo() : header(MT_BOSS_FIRST_HIT_INFO_SC) {}

	SCBossDpsTitle::SCBossDpsTitle() : header(MT_BOSS_DPS_TITLE_SC) {}

	/////////////////////////////////////////////////   //////////////////////////////////////////////////	
	SCRoleVisibleChange::SCRoleVisibleChange():header(MT_ROLE_VISIBLE_CHANGE_SC){}
	CSLeaveFB::CSLeaveFB():header(MT_LEAVE_FB_CS){}
	CSEnterFB::CSEnterFB():header(MT_ENTER_FB_CS){}
	CSCancelMonsterStaticState::CSCancelMonsterStaticState() : header(MT_CANCEL_MONSTER_STATIC_STATE_CS) {}
	SCPerformSceneSkill::SCPerformSceneSkill():header(MT_PERFORM_SCENE_SKILL_SC){}
	CSGetAllObjMoveInfoReq::CSGetAllObjMoveInfoReq():header(MT_GET_ALL_OBJ_MOVE_INFO_REQ_CS){}
	CSAutoFB::CSAutoFB():header(MT_AUTO_FB_CS){}

	SCSkillResetPos::SCSkillResetPos() : header(MT_SKILL_RESET_POS_SC) {}
	SCObjEffectShow::SCObjEffectShow():header(MT_OBJ_EFFECT_SHOW_SC){}
	SCSceneFbTimeOut::SCSceneFbTimeOut():header(MT_SCENE_FB_DYNINFO_SC){}
	SCTeamMemberPos::SCTeamMemberPos():header(MT_TEAM_MEMBER_POS_SC){}

	CSPerformSkillReq::CSPerformSkillReq():header(MT_PERFORM_SKILL_CS){}
	CSPerformQTESkillReq::CSPerformQTESkillReq():header(MT_PERFORM_QTE_SKILL_CS){}
	SCPerformSkill::SCPerformSkill():header(MT_PERFORM_SKILL_SC){}
	SCPerformAOESkill::SCPerformAOESkill():header(MT_PERFORM_AOE_SKILL_SC){}
	SCObjChangeBlood::SCObjChangeBlood():header(MT_OBJ_CHANGE_BLOOD_SC){}
	SCSpecialShieldChangeBlood::SCSpecialShieldChangeBlood():header(MT_WORLD_BOSS_SPECIAL_SHIELD_SC){}
	SCRoleReAlive::SCRoleReAlive():header(MT_ROLE_REALIVE_SC){}
	SCObjChangeMP::SCObjChangeMP():header(MT_OBJ_CHANGE_MP_SC){}
	SCBuffAdd::SCBuffAdd():header(MT_BUFF_ADD_SC){}
	SCBuffRemove::SCBuffRemove():header(MT_BUFF_REMOVE_SC){}
	CSRoleReAliveReq::CSRoleReAliveReq():header(MT_REALIVE_REQ_CS){}
	SCEffectList::SCEffectList():header(MT_EFFECT_LIST_SC){}
	SCBianShenView::SCBianShenView():header(MT_BIANSHEN_EFFECT_IMAGE_BROADCAST_SC){}
	SCEffectInfo::SCEffectInfo():header(MT_EFFECT_INFO_SC){}
	SCEffectRemove::SCEffectRemove():header(MT_EFFECT_REMOVE_SC){}
	SCItemEffectPackageCD::SCItemEffectPackageCD():header(MT_ITEM_EFFECT_PACKAGE_CD){}
	SCBuffMark::SCBuffMark():header(MT_BUFF_MARK_SC){}
	SCExplode::SCExplode():header(MT_EXPLODE_SC){}
	SCSkillPhase::SCSkillPhase():header(MT_SKILL_PHASE_SC) {}
	SCReAliveWaiting::SCReAliveWaiting():header(MT_REALIVE_WAITING_SC) {}
	SCQTEReady::SCQTEReady():header(MT_QTE_READY_SC) {}
	SCContinueKillInfo::SCContinueKillInfo() : header(MT_CONTINUE_KILL_INFO_SC) {}
	SCInvisibleView::SCInvisibleView() : header(MT_INVISIBLE_VIEW_SC) {}
	SCSkillTargetPos::SCSkillTargetPos():header(MT_SKILL_TARGET_POS_SC) {}
	SCSkillEffect::SCSkillEffect():header(MT_SKILL_EFFECT_SC) {}
	SCSkillTip::SCSkillTip():header(MT_SKILL_TIP_SC) {}
	SCFightSpecialFloat::SCFightSpecialFloat():header(MT_FIGHT_SPECIAL_FLOAT_SC) {}
	SCUpGradeSkillInfo::SCUpGradeSkillInfo():header(MT_UPGRADE_SKILL_INFO_SC) {}
	CSSkillCDReq::CSSkillCDReq():header(MT_SKILL_CD_CS) {}

	CSGetEffectListReq::CSGetEffectListReq():header(MT_GET_EFFECT_LIST_REQ_CS){}
	CSGetEffectInfoReq::CSGetEffectInfoReq():header(MT_GET_EFFECT_INFO_REQ_CS){}

	SCFindRoleByNameRet::SCFindRoleByNameRet():header(MT_FIND_ROLE_BY_NAME_RET_SC){}
	CSFindRoleByName::CSFindRoleByName():header(MT_QUERY_ROLE_INFO_BY_NAME_CS) {}
	CSQueryRoleInfo::CSQueryRoleInfo():header(MT_QUERY_ROLE_INFO_CS){}
	CSCrossQueryRoleInfo::CSCrossQueryRoleInfo():header(MT_CROSS_QUERY_ROLE_INFO_CS){}
	CSCrossFindRoleByUUID::CSCrossFindRoleByUUID():header(MT_CROSS_FIND_ROLE_BY_UUID){}
	CSRoleInfoReq::CSRoleInfoReq():header(MT_ROLE_INFO_REQ_CS){}
	SCRoleInfoAck::SCRoleInfoAck():header(MT_ROLE_INFO_ACK_SC){}
	CSSkillInfoReq::CSSkillInfoReq():header(MT_SKILL_INFO_REQ_CS){}
	SCSkillListInfoAck::SCSkillListInfoAck():header(MT_SKILL_LIST_INFO_ACK_SC){}
	SCSkillInfoAck::SCSkillInfoAck():header(MT_SKILL_INFO_ACK_SC) {}
	SCSkillOtherSkillInfo::SCSkillOtherSkillInfo():header(MT_ROLE_SKILL_OTHER_SKILL_INFO) {}
	SCRoleAttributeValue::SCRoleAttributeValue():header(MT_ROLE_ATTR_VALUE_SC){}
	SCRoleXianhun::SCRoleXianhun():header(MT_ROLE_CHANGE_XIANHUN_SC){}
	SCRoleYuanli::SCRoleYuanli():header(MT_ROLE_CHANGE_YUANLI_SC){}
	SCRoleNvWaShi::SCRoleNvWaShi():header(MT_ROLE_CHANGE_NV_WA_SHI_SC){}
	SCRoleHunli::SCRoleHunli():header(MT_ROLE_CHANGE_HUNLI_SC){}
	SCRoleLingJing::SCRoleLingJing():header(MT_ROLE_CHANGE_LINGJING_SC){}
	SCRoleChengJiu::SCRoleChengJiu():header(MT_ROLE_CHANGE_CHENGJIU_SC){}
	SCRoleDayRevivalTimes::SCRoleDayRevivalTimes():header(MT_ROLE_CHANGE_DAY_REVIVAL_TIMES_SC){}
	SCRoleNuqi::SCRoleNuqi():header(MT_ROLE_CHANGE_NUQI_SC){}
	SCRoleCampHonour::SCRoleCampHonour():header(MT_ROLE_CHANGE_CAMP_HONOUR_SC){}
	SCRoleEnergy::SCRoleEnergy():header(MT_ROLE_ENERGY_SC){}
	SCRoleShengwang::SCRoleShengwang():header(MT_ROLE_SHENGWANG_SC){}
	SCRoleExpExtraPer::SCRoleExpExtraPer():header(MT_ROLE_EXP_EXTRA_PER_SC){}
	SCRoleJingJie::SCRoleJingJie() : header(MT_ROLE_JINGJIE_INFO_SC) {}
	CSRoleJingJieReq::CSRoleJingJieReq() : header(MT_ROLE_JINGJIE_REQ_CS) {}
	CSSetAvatarTimeStamp::CSSetAvatarTimeStamp():header(MT_SET_AVATAR_TIMESTAMP_CS){}
	SCAvatarTimeStampInfo::SCAvatarTimeStampInfo():header(MT_SEND_AVATAR_TIMESTAMP_INFO_CS){}
	SCZhuaGuiRoleInfo::SCZhuaGuiRoleInfo():header(MT_ROLE_CHANGE_DAY_ZHUAGUI_INFO_SC){}
	SCRoleGongxun::SCRoleGongxun():header(MT_ROLE_GONGXUN_SC){}

	CSAllInfoReq::CSAllInfoReq():header(MT_ALL_INFO_REQ_CS){}

	CSRoleSkillLearnReq::CSRoleSkillLearnReq():header(MT_ROLE_SKILL_LEARN_REQ_CS){}

	SCChaExpChange::SCChaExpChange():header(MT_CHA_EXP_CHANGE_SC){}
	SCChaLevelUpgrade::SCChaLevelUpgrade():header(MT_CHA_LEVEL_UPGRADE_SC){}

	CSRoleChangeCamp::CSRoleChangeCamp():header(MT_ROLE_CHANGE_CAMP_CS) {}
	SCRoleChangeCamp::SCRoleChangeCamp():header(MT_ROLE_CHANGE_CAMP_SC) {}
	SCChangeProfResult::SCChangeProfResult():header(MT_CHANGE_PROF_RESULT_SC) {}
	CSChangeProfReq::CSChangeProfReq():header(MT_CHANGE_PROF_REQ_CS) {}

	CSRoleZhuanSheng::CSRoleZhuanSheng():header(MT_ROLE_ZHUAN_SHENG) {}
	SCRoleNorexItemUseTimes::SCRoleNorexItemUseTimes():header(MT_ROLE_NOREXITEM_USE_TIMES) {}
	CSTransformExpToBottle::CSTransformExpToBottle():header(MT_TRANSFORM_EXP_REQ_CS) {}
	SCExpBottleInfo::SCExpBottleInfo():header(MT_EXPBOTTLE_INFO_SC) {}
	SCMoveChessInfo::SCMoveChessInfo():header(MT_MOVE_CHESS_INFO_SC) {}
	SCMoveChessStepRewardInfo::SCMoveChessStepRewardInfo() : header(MT_MOVE_CHESS_STEP_REWARD_INFO_SC) {}

	CSMillionaireInfoReq::CSMillionaireInfoReq():header(MT_MILLIONAIRE_INFO_CS) {}
	SCMillionaireInfo::SCMillionaireInfo():header(MT_MILLIONAIRE_INFO_SC) {}

	CSGetRoleSpaceMsgInfo::CSGetRoleSpaceMsgInfo():header(MT_GET_ROLE_SPACE_MSG_INFO_CS){}
	SCRoleSpaceMsgInfo::SCRoleSpaceMsgInfo():header(MT_ROLE_SPACE_MSG_INFO_SC){}
	CSSpaceOper::CSSpaceOper():header(MT_OPER_SPACE_CS){}
	CSSpaceRemoveRecord::CSSpaceRemoveRecord():header(MT_REMOVE_RECORD_CS){}
	SCSpaceSelfInfo::SCSpaceSelfInfo():header(MT_ROLE_SPACE_SELF_INFO_SC) {}
	CSLifeSkillOpera::CSLifeSkillOpera():header(MT_ROLE_LIFESKILL_OPERA_CS) {}
	SCLifeSkillInfo::SCLifeSkillInfo():header(MT_ROLE_LIFE_SKILL_INFO_SC) {}
	SCWashInfo::SCWashInfo():header(MT_WASH_INFO_SC) {}
	
	SCOtherUserOnlineStatus::SCOtherUserOnlineStatus():header(MT_OTHER_USER_ONLINE_STATUS_SC){}

	SCRoleChangeProf::SCRoleChangeProf():header(MT_ROLE_CHANGE_PROFESSIONAL_SC){}
	SCRoleAddCamp::SCRoleAddCamp():header(MT_ROLE_ADD_COUNTRY_SC){}
	
	CSSetAttackMode::CSSetAttackMode():header(MT_ROLE_SET_ATTACK_MODE_CS){}
	SCSetAttackMode::SCSetAttackMode():header(MT_ROLE_SET_ATTACK_MODE_SC){}
	SCRoleGuildInfoChange::SCRoleGuildInfoChange():header(MT_ROLE_GUILD_INFO_CHANGE){}
	SCRoleNameColorChange::SCRoleNameColorChange():header(MT_ROLE_NAME_COLOR_CHANGE_SC){}
	SCRoleEvilChange::SCRoleEvilChange():header(MT_CHANGE_EVIL_SC){}
	SCGetRoleBaseInfoAck::SCGetRoleBaseInfoAck():header(MT_GET_ROLE_INFO_ACK_SC){}
	CSRoleChooseCamp::CSRoleChooseCamp():header(MT_CHOOSE_CAMP_CS){}
	CSGetCampInfo::CSGetCampInfo() : header(MT_CAMP_INFO_CS) {}
	SCCampInfo::SCCampInfo() : header(MT_CAMP_INFO_SC) {}
	CSRoleChooseProf::CSRoleChooseProf():header(MT_CHOOSE_PROF_CS){}
	SCVipInfo::SCVipInfo():header(MT_VIP_INFO_SC){}
	CSGetRoleCapability::CSGetRoleCapability():header(MT_GET_ROLE_CAPABILITY_CS){}
	SCRoleCapability::SCRoleCapability():header(MT_ROLE_CAPABILITY_SC){}
	CSSetMoveMode::CSSetMoveMode():header(MT_SET_MOVE_MODE_CS) {}
	SCRoleAuthority::SCRoleAuthority():header(MT_ROLE_AUTHORITY_TYPE_SC) {}
	SCRoleTeamLeaderInfo::SCRoleTeamLeaderInfo():header(MT_ROLE_TEAM_LEADER_INFO_SC){}
	SCRoleResetName::SCRoleResetName():header(MT_ROLE_RESET_NAME_SC){}
	CSRoleResetName::CSRoleResetName():header(MT_ROLE_RESET_NAME_CS) {}
	SCRoleDayCount::SCRoleDayCount():header(MT_ROLE_DAY_COUNT_SC){}
	SCInvestCountInfo::SCInvestCountInfo():header(MT_INVEST_COUNT_INFO_SC){}
	SCRoleChangeSex::SCRoleChangeSex():header(MT_ROLE_CHANGE_SEX_SC){}
	SCRoleSpecialAppearanceChange::SCRoleSpecialAppearanceChange():header(MT_CHANGE_SPECIAL_APPEARANCE_SC){}
	CSRoleSetZombieReq::CSRoleSetZombieReq():header(MT_ROLE_SET_ZOMBIE_REQ_CS) {}
	SCNewhandEndTime::SCNewhandEndTime():header(MT_NEWHAND_GUILD_ENDTIME_SC) {}
	CSMountBianshenOpera::CSMountBianshenOpera():header(MT_MOUNT_BIANSHEN_OPERA_CS) {}
	SCMountBianshenInfo::SCMountBianshenInfo():header(MT_MOUNT_BIANSHEN_INFO_SC) {}
	SCRoleMarryInfoChange::SCRoleMarryInfoChange():header(MT_ROLE_MARRY_INFO_CHANGE_SC) {}
	CSSpeedUpHello::CSSpeedUpHello():header(MT_SPEEDUP_HELLO_CS) {}

	//-------------------------------------------------------------------------------------------

	// 放一些通用的协议
	SCOperateResult::SCOperateResult():header(MT_OPERATE_RET_SC) {}
	SCDrawResult::SCDrawResult() : header(MT_DRAW_RESULT_SC) {}

	//-------------------------------------------------------------------------------------------

	CSQiFuReq::CSQiFuReq():header(MT_QIFU_REQ_CS) {}
	CSFetchQiFuReward::CSFetchQiFuReward():header(MT_FETCH_QIFU_REWARD_CS) {}
	
	//物品相关
	CSKnapsackInfoReq::CSKnapsackInfoReq():header(MT_KNAPSACK_INFO_REQ_CS){}
	SCKnapsackInfoAck::SCKnapsackInfoAck():header(MT_KNAPSACK_INFO_ACK_SC){}
	CSUseItem::CSUseItem():header(MT_USE_ITEM_CS){}
	CSUseItemMaxNum::CSUseItemMaxNum() : header(MT_USE_ITEM_MAX_NUM_CS) {}
	SCUseItemSuc::SCUseItemSuc():header(MT_USE_ITEM_SUC_SC){}
//	SCFunctionNewItemNotice::SCFunctionNewItemNotice():header(MT_FUNCTION_NEW_ITEM_NOTICE_SC){}
	CSMoveItem::CSMoveItem():header(MT_MOVE_ITEM_CS){}
	CSDiscardItem::CSDiscardItem() : header(MT_DICARD_ITEM_CS) {}
	CSBatchDiscardItem::CSBatchDiscardItem():header(MT_BATCH_DICARD_ITEM_CS){}
	CSKnapsackStoragePutInOrder::CSKnapsackStoragePutInOrder():header(MT_KNAPSACK_STORAGE_PUTINORDER_CS){}
	SCKnapsackItemChange::SCKnapsackItemChange():header(MT_KNAPSACK_GRID_CHANGE_SC){}
	SCLackItem::SCLackItem():header(MT_LACK_ITEM_SC) {}
	
	SCKnapsackMaxGridNum::SCKnapsackMaxGridNum():header(MT_KNAPSACK_MAX_GRID_NUM_SC){}
	SCStorageMaxGridNum::SCStorageMaxGridNum():header(MT_STORAGE_MAX_GRID_NUM_SC){}

	SCKnapsackGridExtendAuto::SCKnapsackGridExtendAuto() : header(MT_KNAPSACK_GRID_EXTEND_AUTO_SC) {}
	CSKnapsackGridExtendAuto::CSKnapsackGridExtendAuto() : header(MT_KNAPSACK_GRID_EXTEND_AUTO_CS) {}

	CSPickItem::CSPickItem():header(MT_PICK_ITEM_CS){}
	SCPickItemAck::SCPickItemAck():header(MT_PICK_ITEM_ACT_SC){}
 	SCTeamMemberPickItem::SCTeamMemberPickItem():header(MT_TEAM_MEMBER_PICK_ITEM_SC){}
	CSSplitItem::CSSplitItem():header(MT_SPLIT_ITEM_CS){}
	CSItemCompose::CSItemCompose():header(MT_ITEM_COMPOSE_CS){}
	CSBluePrintCompose::CSBluePrintCompose():header(MT_BLUEPRINT_COMPOSE_CS){}
	CSTakeOutAllItem::CSTakeOutAllItem():header(MT_TAKE_OUT_ALL_ITEM) {}
	CSGemstoneBatchCompose::CSGemstoneBatchCompose():header(MT_GEMSTONE_BATCH_COMPOSE_CS) {}
	CSReqStoneInfo::CSReqStoneInfo():header(MT_STONE_INFO_REQ_CS) {}

	CSKnapsackStorageExtendGridNum::CSKnapsackStorageExtendGridNum():header(MT_KNAPSACKSTORAGE_EXTEND_GRID_NUM_CS){}
	SCItemMultiInvalid::SCItemMultiInvalid():header(MT_ITEM_MULTI_INVALID_SC){}
	SCRewardListInfo::SCRewardListInfo() : header(MT_REWARD_ITEM_LIST_ACT_SC) {}
	
	SCKnapsackItemChangeParam::SCKnapsackItemChangeParam():header(MT_KNAPSACK_GRID_CHANGE_PARAM_SC){}
	SCKnapsackInfoParam::SCKnapsackInfoParam():header(MT_KNAPSACK_PARAM_SC){}
	SCItemColddownInfo::SCItemColddownInfo():header(MT_ITEM_COLDDOWN_INFO_SC) {}

	SCEquipmentInfoAck::SCEquipmentInfoAck():header(MT_EQUIPMENT_INFO_ACK_SC){}
	SCEquipmentGridInfo::SCEquipmentGridInfo():header(MT_EQUIPMENT_GRID_INFO_SC){}
	SCEquipmentItemChange::SCEquipmentItemChange():header(MT_EQUIPMENT_ITEM_CHANGE_SC){}
	SCGetOneEquipmentInfoAck::SCGetOneEquipmentInfoAck():header(MT_GET_ONE_EQUIPMENT_INFO_ACK_SC){}
	SCEquipSuitInfo::SCEquipSuitInfo():header(MT_EQUIP_SUIT_INFO_SC) {}
	SCDuanzaoSuitInfo::SCDuanzaoSuitInfo() : header(MT_DUANZAO_SUIT_INFO_SC) {}
	SCShengbingInfo::SCShengbingInfo():header(MT_SHENBING_INFO_SC) {}
	SCAllShenBingInfo::SCAllShenBingInfo():header(MT_SHENBING_ALL_INFO_SC) {}
	SCShenbingAppeChg::SCShenbingAppeChg():header(MT_SHENBING_APPE_CHG_SC) {}
	SCStoneInfo::SCStoneInfo():header(MT_STONE_INFO_SC) {}
	SCNoticeWuqiColor::SCNoticeWuqiColor():header(MT_EQUIPMENT_WUQI_COLOR_SC){}

	CSEquipmentInfoReq::CSEquipmentInfoReq():header(MT_EQUIPMENT_INFO_REQ_CS){}
	CSTakeOffEquipment::CSTakeOffEquipment():header(MT_TAKE_OFF_EQUIPMENT_CS){}
	CSEquipStrengthen::CSEquipStrengthen():header(MT_EQUIP_STRENGTHEN_REQ_CS){}
	CSEquipUpLevel::CSEquipUpLevel():header(MT_EQUIP_UPLEVEL_REQ_CS){}
	CSEquipUpQuality::CSEquipUpQuality() : header(MT_EQUIP_UPQUALITY_REQ_CS) {}
	CSEquipCompound::CSEquipCompound():header(MT_EQUIP_COMPOUND_CS){}
	CSEquipUpEternity::CSEquipUpEternity() : header(MT_EQUIP_UP_ETERNITY_CS) {}
	CSEquipUseEternityLevel::CSEquipUseEternityLevel() : header(MT_EQUIP_USE_ETERNITY_LEVEL_CS) {}
	CSEquipShenZhu::CSEquipShenZhu():header(MT_EQUIP_SHENZHU_REQ_CS){}
	CSStoneUplevel::CSStoneUplevel():header(MT_STONE_UPLVL_CS) {}
	CSStoneInlay::CSStoneInlay():header(MT_STONE_INLAY_CS) {}
	CSWash::CSWash():header(MT_WASH_CS) {}
	CSShenbingReq::CSShenbingReq():header(MT_SHENBING_REQ_CS){}
	CSShenbingCompose::CSShenbingCompose():header(MT_SHENBING_COMPOSE_CS){}
	CSShenbingOP::CSShenbingOP():header(MT_SHENBING_OP_CS){}
	CSShenBingUpLevel::CSShenBingUpLevel():header(MT_SHENBING_UP_LEVEL_CS){}
	CSEquipFuling::CSEquipFuling() : header(MT_EQUIP_FULING_CS) {}
	CSEquipCrossEquipOpera::CSEquipCrossEquipOpera() : header(MT_EQUIP_CROSS_EQUIP_OPERA_CS) {}

	CSGetRandomRoleList::CSGetRandomRoleList() : header(MT_GET_RANDOM_ROLE_LIST_CS){}
	SCRandomRoleListRet::SCRandomRoleListRet() : header(MT_RANDOM_ROLE_LIST_SC){}
	
	CSTaskListReq::CSTaskListReq():header(MT_TASK_LIST_REQ_CS){}
	SCTaskListAck::SCTaskListAck():header(MT_TASK_LIST_ACK_SC){}
	SCTaskInfo::SCTaskInfo():header(MT_TASK_INFO_SC){}
	SCTaskRecorderList::SCTaskRecorderList():header(MT_TASK_RECORDER_LIST_SC){}
	SCTaskRecorderInfo::SCTaskRecorderInfo():header(MT_TASK_RECORDER_INFO_SC){}
	CSTaskGiveup::CSTaskGiveup():header(MT_TASK_GIVEUP_CS){}
	CSFlyByShoe::CSFlyByShoe():header(MT_FLY_BY_SHOE_CS){}
	CSSyncJump::CSSyncJump():header(MT_FLY_SYNC_JUMP_CS){}
	CSTaskAccept::CSTaskAccept():header(MT_TASK_ACCEPT_CS){};
	CSTaskCommit::CSTaskCommit():header(MT_TASK_COMMIT_CS){};
	SCTaskAccpetableTaskList::SCTaskAccpetableTaskList():header(MT_TASK_ACCEPTABLE_TASK_LIST_SC){}
	CSTaskTalkToNpc::CSTaskTalkToNpc():header(MT_TASK_TALK_TO_NPC){};
	SCTaskRollReward::SCTaskRollReward() :header(MT_TASK_RANDOM_REWARD) {};
	CSJinjiesysRewardOpera::CSJinjiesysRewardOpera() : header(MT_JINJIESYS_REWARD_OPERA_REQ_CS) {}
	SCJinjiesysRewardInfo::SCJinjiesysRewardInfo() : header(MT_JINJIESYS_REWARD_INFO_SC) {}
	SCJinjiesysRewardTimestamp::SCJinjiesysRewardTimestamp() : header(MT_JINJIESYS_REWARD_TIMESTAMP_SC) {}

	CSPaoHuanSkipTask::CSPaoHuanSkipTask() : header(MT_PAOHUAN_TASK_COMMIT_REQ_CS) {}

	CSTuMoTaskOpera::CSTuMoTaskOpera() : header(MT_TUMO_TASK_OPERA_CS) {}
	CSGetPaoHuanTaskInfo::CSGetPaoHuanTaskInfo() : header(MT_PAOHUAN_TASK_INFO_REQ_CS) {}

	CSCishaTaskOpera::CSCishaTaskOpera() : header(MT_CISHA_TASK_OPERA_CS) {}
	SCCishaTaskInfo::SCCishaTaskInfo() : header(MT_CISHA_TASK_INFO_SC) {}

	CSRefreshHusongTask::CSRefreshHusongTask() : header(MT_HUSONG_TASK_REFRESH_CS) {}
	SCHusongInfo::SCHusongInfo() : header(MT_HUSONG_TASK_INFO_SC) {}
	CSHusongBuyTimes::CSHusongBuyTimes() : header(MT_HUSONG_BUY_TIMES_CS) {}
	CSHusongAddShield::CSHusongAddShield() : header(MT_HUSONG_ADD_SHIIELD_CS) {}
	SCHusongConsumeInfo::SCHusongConsumeInfo() : header(MT_HUSONG_TASK_COMSUEM_INFO_SC) {}

	SCTuMoTaskInfo::SCTuMoTaskInfo() : header(MT_TUMO_TASK_INFO_SC) {}

	SCPaohuanTaskInfo::SCPaohuanTaskInfo() : header(MT_PAOHUAN_TASK_INFO_SC){}
	SCPaoHuanRollPool::SCPaoHuanRollPool() : header(MT_PAOHUAN_TASK_ROLL_POOL_SC){}
	SCPaoHuanRollInfo::SCPaoHuanRollInfo() : header(MT_PAOHUAN_TASK_ROLL_INFO_SC){}
	CSPaoHuanRollReq::CSPaoHuanRollReq() : header(MT_PAOHUAN_TASK_ROLL_REQ_CS){}

	CSReqCommonOpreate::CSReqCommonOpreate() : header(MT_COMMON_OPERATE_CS){}
	CSShenzhaungOper::CSShenzhaungOper() : header(MT_CS_SHENZHUANG_OPER_CS){}
	SCShenzhaungInfo::SCShenzhaungInfo() : header(MT_SHENZHUANG_INFO_CS) {}
	CSCSAEquipOpera::CSCSAEquipOpera() : header(MT_CSA_EQUIP_OPERA_CS) {};
	SCCSAEquipInfo::SCCSAEquipInfo() : header(MT_CSA_EQUIP_INFO_SC) {};
	SCCSAActivePower::SCCSAActivePower() : header(MT_CSA_EQUIP_ACTIVE_SC) {};
	CSSeekRoleWhere::CSSeekRoleWhere() : header(MT_CS_SEEK_ROLE_WHERE_CS){}
	CSCampEquipOperate::CSCampEquipOperate() : header(MT_CS_CAMPEQUIP_OPERATE_CS){}
	CSCampNormalDuobaoOperate::CSCampNormalDuobaoOperate() : header(MT_CS_CAMP_NORMALDUOBAO_OPERATE_CS){}

	SCCommonInfo::SCCommonInfo() : header(MT_COMMON_INFO_SC){}
	SCJinghuaHusongViewChange::SCJinghuaHusongViewChange() : header(MT_JINGHUAHUSONG_VIEW_CHANGE_SC){}
	SCRoleSeekWhereInfo::SCRoleSeekWhereInfo() : header(MT_SEEKROLE_WHERE_INFO_SC) {}
	SCHuangChengHuiZhanInfo::SCHuangChengHuiZhanInfo() : header(MT_HUANGCHENGHUIZHAN_INFO_SC) {}
	SCCampEquipInfo::SCCampEquipInfo() : header(MT_CAMPEQUIP_INFO_SC) {}
	SCCampDefendInfo::SCCampDefendInfo() : header(MT_CAMPDEFEND_INFO_SC) {}

	CSGoldVipOperaReq::CSGoldVipOperaReq():header(MT_GOLD_VIP_OPERA_REQ_CS){}
	SCGoldVipInfo::SCGoldVipInfo():header(MT_GOLD_VIP_INFO_SC){}

	SCWorldBossWearyInfo::SCWorldBossWearyInfo():header(MT_WORLD_BOSS_WEARY_INFO_SC){}

	SCGuildTaskInfo::SCGuildTaskInfo() : header(MT_GUILD_TASK_INFO_SC) {}
	CSFinishAllGuildTask::CSFinishAllGuildTask() : header(MT_GUILD_TASK_FINISH_ALL_CS) {}
	CSGetGuildTaskDoubleReward::CSGetGuildTaskDoubleReward() : header(MT_GUILD_TASK_DOUBLE_REWARD_CS) {}
	CSForceFinishSpecialTask::CSForceFinishSpecialTask() : header(MT_FORCE_FINISH_SPECIAL_TASK_SC) {}

	CSHotkeyInfoReq::CSHotkeyInfoReq():header(MT_HOTKEY_INFO_REQ_CS){}
	SCHotkeyInfoAck::SCHotkeyInfoAck():header(MT_HOTKEY_INFO_ACK_SC){}
	CSChangeHotkey::CSChangeHotkey():header(MT_CHANGE_HOTKEY_CS){}

	SCMoney::SCMoney():header(MT_MONEY_SC){}
	CSAccountGoldReq::CSAccountGoldReq():header(MT_ACCOUNT_GOLD_REQ_CS){}
	SCAccountGoldAck::SCAccountGoldAck():header(MT_ACCOUNT_GOLD_ACK_SC){}
	CSAccountGetGoldReq::CSAccountGetGoldReq():header(MT_ACCOUNT_GET_GOLD_REQ_CS){}
	SCAccountGetGoldAck::SCAccountGetGoldAck():header(MT_ACCOUNT_GET_GOLD_ACK_SC){}

	CSFriendInfoReq::CSFriendInfoReq():header(MT_FRIEND_LIST_REQ_CS){}
	SCFriendInfoAck::SCFriendInfoAck():header(MT_FRIEND_LIST_ACK_SC){}
	CSAddFriendRet::CSAddFriendRet():header(MT_ADD_FRIEND_RET_CS){}
	CSAddFriendReq::CSAddFriendReq():header(MT_ADD_FRIEND_REQ_CS){}
	CSDeleteFriend::CSDeleteFriend():header(MT_FRIEND_DELETE_CS){}
	SCAddFriendRoute::SCAddFriendRoute():header(MT_ADD_FRIEND_ROUTE_SC){}
	SCChangeFriend::SCChangeFriend():header(MT_CHANGE_FRIEND_SC){}
	SCAddFriendRet::SCAddFriendRet():header(MT_ADD_FRIEND_RET_SC){}
	SCEnemyListACK::SCEnemyListACK():header(MT_ENEMY_LIST_ACK_SC){}
	CSEnemyDelete::CSEnemyDelete():header(MT_ENEMY_DELETE_CS){}
	SCChangeEnemy::SCChangeEnemy():header(MT_ENEMY_CHANGE_SC){}

	SCBlacklistsACK::SCBlacklistsACK():header(MT_BLACKLISTS_SC) {}
	CSAddBlackReq::CSAddBlackReq():header(MT_ADD_BLACK_CS) {}
	CSDeleteBlackReq::CSDeleteBlackReq():header(MT_DELETE_BLACK_CS) {}
	SCChangeBlacklist::SCChangeBlacklist():header(MT_CHANGE_BLACKLISTS_SC) {}

	CSGiveFlower::CSGiveFlower():header(MT_GIVE_FLOWER_CS) {}
	CSGiveFlowerKissReq::CSGiveFlowerKissReq():header(MT_GIVE_FLOWER_KISS_REQ_CS) {}
	SCGiveFlower::SCGiveFlower():header(MT_GIVE_FLOWER_SC) {}
	SCSoneHuaInfo::SCSoneHuaInfo():header(MT_SONG_HUA_INFO) {}
	CSSoneHuaInfoReq::CSSoneHuaInfoReq():header(MT_SONG_HUA_INFO_REQ) {}
	SCGiveFlowerKissAck::SCGiveFlowerKissAck():header(MT_GIVE_FLOWER_KISS_ACK_SC) {}
	SCAllCharmChange::SCAllCharmChange():header(MT_ALL_CHARM_CHANGE_SC) {}
	CSFriendBlessOperaReq::CSFriendBlessOperaReq():header(MT_FRIEND_BLESS_OPERA_REQ_CS){}
	SCFriendBlessAllInfo::SCFriendBlessAllInfo():header(MT_FRIEND_BLESS_ALL_INFO_SC){}	
	SCFriendBlessChangeInfo::SCFriendBlessChangeInfo():header(MT_FRIEND_BLESS_CHANGE_INFO_SC){}
	SCFriendBlessInviteBless::SCFriendBlessInviteBless() : header(MT_FRIEND_BLESS_INVITE_BLESS_SC) {}
	CSOfflineFriendAutoDecFlag::CSOfflineFriendAutoDecFlag() : header(MT_FRIEND_BLESS_INVITE_BLESS_SC) {}

	SCWingInfo::SCWingInfo():header(MT_WING_INFO_SC) {}
	CSUpgradeWing::CSUpgradeWing():header (MT_WING_UPGRADE_REQ_CS) {}
	CSUseWingImage::CSUseWingImage():header (MT_WING_USE_IMAGE_CS) {}
	CSWingGetInfo::CSWingGetInfo():header(MT_WING_GET_INFO_CS) {}
	CSWingUplevelEquip::CSWingUplevelEquip():header(MT_WING_UPLEVEL_EQUIP_CS) {}
	CSWingSkillUplevelReq::CSWingSkillUplevelReq():header(MT_WING_SKILL_UPLEVEL_REQ_CS) {}
	CSWingSpecialImgUpgrade::CSWingSpecialImgUpgrade():header(MT_WING_SPECIAL_IMG_UPGRADE_CS){}
	CSWingUpStarLevel::CSWingUpStarLevel():header(MT_WING_UP_STAR_LEVEL_CS) {}

	SCHaloInfo::SCHaloInfo():header(MT_HALO_INFO_SC) {}
	CSUpgradeHalo::CSUpgradeHalo():header (MT_HALO_UPGRADE_REQ_CS) {}
	CSUseHaloImage::CSUseHaloImage():header (MT_HALO_USE_IMAGE_CS) {}
	CSHaloGetInfo::CSHaloGetInfo():header(MT_HALO_GET_INFO_CS) {}
	CSHaloUplevelEquip::CSHaloUplevelEquip():header(MT_HALO_UPLEVEL_EQUIP_CS) {}
	CSHaloSkillUplevelReq::CSHaloSkillUplevelReq():header(MT_HALO_SKILL_UPLEVEL_REQ_CS) {}
	CSHaloSpecialImgUpgrade::CSHaloSpecialImgUpgrade():header(MT_HALO_SPECIAL_IMG_UPGRADE_CS){}
	CSHaloUpStarLevel::CSHaloUpStarLevel():header(MT_HALO_UP_STAR_LEVEL_CS){}

	SCShengongInfo::SCShengongInfo():header(MT_SHENGONG_INFO_SC) {}
	CSUpgradeShengong::CSUpgradeShengong():header (MT_SHENGONG_UPGRADE_REQ_CS) {}
	CSUseShengongImage::CSUseShengongImage():header (MT_SHENGONG_USE_IMAGE_CS) {}
	CSShengongGetInfo::CSShengongGetInfo():header(MT_SHENGONG_GET_INFO_CS) {}
	CSShengongUplevelEquip::CSShengongUplevelEquip():header(MT_SHENGONG_UPLEVEL_EQUIP_CS) {}
	CSShengongSkillUplevelReq::CSShengongSkillUplevelReq():header(MT_SHENGONG_SKILL_UPLEVEL_REQ_CS) {}
	CSShengongSpecialImgUpgrade::CSShengongSpecialImgUpgrade():header(MT_SHENGONG_SPECIAL_IMG_UPGRADE_CS){}
	CSShengongUpStarLevel::CSShengongUpStarLevel():header(MT_SHENGONG_UP_STAR_LEVEL_CS){}

	SCShenyiInfo::SCShenyiInfo():header(MT_SHENYI_INFO_SC) {}
	CSUpgradeShenyi::CSUpgradeShenyi():header (MT_SHENYI_UPGRADE_REQ_CS) {}
	CSUseShenyiImage::CSUseShenyiImage():header (MT_SHENYI_USE_IMAGE_CS) {}
	CSShenyiGetInfo::CSShenyiGetInfo():header(MT_SHENYI_GET_INFO_CS) {}
	CSShenyiUplevelEquip::CSShenyiUplevelEquip():header(MT_SHENYI_UPLEVEL_EQUIP_CS) {}
	CSShenyiSkillUplevelReq::CSShenyiSkillUplevelReq():header(MT_SHENYI_SKILL_UPLEVEL_REQ_CS) {}
	CSShenyiSpecialImgUpgrade::CSShenyiSpecialImgUpgrade():header(MT_SHENYI_SPECIAL_IMG_UPGRADE_CS){}
	CSShenyiUpStarLevel::CSShenyiUpStarLevel():header(MT_SHENYI_UP_STAR_LEVEL_CS){}

	SCFootprintInfo::SCFootprintInfo() : header(MT_FOOTPRINT_INFO_SC) {}
	CSFootprintOperate::CSFootprintOperate() : header(MT_FOOTPRINT_OPERATE_CS) {}

	SCCloakInfo::SCCloakInfo() : header(MT_CLOAK_INFO_SC) {}
	CSCloakOperate::CSCloakOperate() : header(MT_CLOAK_OPERATE_CS) {}

	SCXiannvShouhuInfo::SCXiannvShouhuInfo():header(MT_XN_SHOUHU_INFO_SC) {}
	CSXiannvShouhuUpStarLevel::CSXiannvShouhuUpStarLevel():header(MT_XN_SHOUHU_UPSTAR_CS){}
	CSUseXiannvShouhuImage::CSUseXiannvShouhuImage():header(MT_XN_SHOUHU_USE_IMAGE_CS) {}
	CSXiannvShouhuGetInfo::CSXiannvShouhuGetInfo():header(MT_XN_SHOUHU_INFO_REQ_CS){}

	SCJinglingFazhenInfo::SCJinglingFazhenInfo():header(MT_JL_FAZHEN_INFO_SC) {}
	CSJinglingFazhenUpStarLevel::CSJinglingFazhenUpStarLevel():header(MT_JL_FAZHEN_UPSTAR_CS){}
	CSUseJinglingFazhenImage::CSUseJinglingFazhenImage():header(MT_JL_FAZHEN_USE_IMAGE_CS){}
	CSJinglingFazhenGetInfo::CSJinglingFazhenGetInfo():header(MT_JL_FAZHEN_INFO_REQ_CS) {}
	CSJinglingFazhenSpecialImgUpgrade::CSJinglingFazhenSpecialImgUpgrade():header(MT_JL_FAZHEN_SPECIAL_IMG_UPGRADE_CS) {}
	
	SCJinglingGuanghuanInfo::SCJinglingGuanghuanInfo():header(MT_JL_GUANGHUAN_INFO_SC) {}
	CSJinglingGuanghuanUpStarLevel::CSJinglingGuanghuanUpStarLevel():header(MT_JL_GUANGHUAN_UPSTAR_CS){}
	CSUseJinglingGuanghuanImage::CSUseJinglingGuanghuanImage():header(MT_JL_GUANGHUAN_USE_IMAGE_CS){}
	CSJinglingGuanghuanGetInfo::CSJinglingGuanghuanGetInfo():header(MT_JL_GUANGHUAN_INFO_REQ_CS) {}
	CSJinglingGuanghuanSpecialImgUpgrade::CSJinglingGuanghuanSpecialImgUpgrade():header(MT_JL_GUANGHUAN_SPECIAL_IMG_UPGRADE_CS) {}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSDiscountBuyReqBuy::CSDiscountBuyReqBuy() : header(MT_DISCOUNT_BUY_REQ_BUY_CS) {}
	CSDiscountBuyGetInfo::CSDiscountBuyGetInfo() : header(MT_DISCOUNT_BUY_GET_INFO_CS) {}
	SCDiscountBuyInfo::SCDiscountBuyInfo() : header(MT_DISCOUNT_BUY_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSZeroGiftOperate::CSZeroGiftOperate() : header(MT_ZERO_GIFT_OPERATE_REQ_CS) {}
	SCZeroGiftInfo::SCZeroGiftInfo() : header(MT_ZERO_GIFT_INFO_SC) {}
	CSAdvanceNoitceOperate::CSAdvanceNoitceOperate() : header(MT_ADVANCE_NOTIC_OPERATE_CS) {}
	SCAdvanceNoticeInfo::SCAdvanceNoticeInfo() : header(MT_ADVANCE_NOTICE_INFO_SC) {}
	CSGoldTurntableOperateReq::CSGoldTurntableOperateReq() : header(MT_GOLD_TURNTABLE_OPERATE_REQ_CS) {}
	SCGoldTurntableReward::SCGoldTurntableReward() : header(MT_GOLD_TURNTABLE_REWARD_SC) {}
	SCGoldTurntableInfo::SCGoldTurntableInfo() : header(MT_GOLD_TURNTABLE_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSReqTrade::CSReqTrade():header(MT_REQ_TRADE_CS){}
	SCReqTradeRoute::SCReqTradeRoute():header(MT_REQ_TRADE_ROUTE_SC){}
	CSReqTradeRet::CSReqTradeRet():header(MT_TRADE_RET_CS){}
	CSTradeLockReq::CSTradeLockReq():header(MT_TRADE_LOCK_REQ_CS){}
	CSTradeAffirmReq::CSTradeAffirmReq():header(MT_TRADE_AFFIRM_REQ_CS){}
	CSTradeCancle::CSTradeCancle():header(MT_TRADE_CANCLE_CS){}
	CSTradeGoldReq::CSTradeGoldReq():header(MT_TRADE_PUT_GOLD_REQ_CS){}
	CSTradeItemReq::CSTradeItemReq():header(MT_TRADE_PUT_ITEM_REQ_CS){}
	SCTradeGold::SCTradeGold():header(MT_TRADE_PUT_GOLD_SC){}
	SCTradeItem::SCTradeItem():header(MT_TRADE_PUT_ITEM_SC){}
	SCTradeItemParam::SCTradeItemParam():header(MT_TRADE_PUT_ITEM_PARAM_SC){}
	SCTradeState::SCTradeState():header(MT_TRADE_STATE_SC){}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCMonthCardInfo::SCMonthCardInfo() : header(MT_MONTH_CARD_INFO_SC) {}
	CSMonthCardFetchDayReward::CSMonthCardFetchDayReward() : header(MT_MONTH_CARD_FETCH_DAY_REWARD_CS) {}
	CSDisconnectReq::CSDisconnectReq() : header(MT_DISCONNECT_REQ_CS) {}
	CSZhuanzhiReq::CSZhuanzhiReq() : header(MT_ZHUANZHI_REQ_CS) {}
	CSQingyuanCardUpgradeReq::CSQingyuanCardUpgradeReq() : header(MT_QINGYUAN_CARD_UPGRADE_REQ_CS) {}
	SCQingyuanCoupleHaloInfo::SCQingyuanCoupleHaloInfo() : header(MT_QINGYUAN_COUPLE_HALO_INFO_SC) {}
	SCQingyuanCoupleHaloTrigger::SCQingyuanCoupleHaloTrigger() : header(MT_QINGYUAN_COUPLE_HALO_TRIGGER_SC) {}
	CSQingyuanCoupleHaloOperaReq::CSQingyuanCoupleHaloOperaReq() : header(MT_QINGYUAN_COUPLE_HALO_OPERA_REQ_CS) {}
	CSQingyuanFetchBlessRewardReq::CSQingyuanFetchBlessRewardReq() : header(MT_QINGYUAN_BLESS_REWARD_REQ_CS) {}
	CSQingyuanAddBlessDaysReq::CSQingyuanAddBlessDaysReq() : header(MT_QINGYUAN_ADD_BLESSDAYS_REQ_CS) {}
	CSLoveContractOperaReq::CSLoveContractOperaReq() : header(MT_LOVE_CONTRACT_OPERA_REQ_CS) {}
	CSQingyuanFetchLoveContract::CSQingyuanFetchLoveContract() : header(MT_QINGYUAN_FETCH_LOVE_CONTRACT_CS) {}
	SCQingyuanLoveContractInfo::SCQingyuanLoveContractInfo() : header(MT_QINGYUAN_LOVE_CONTRACT_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRollMoneyInfo::SCRollMoneyInfo() : header(MT_ROLL_MONEY_INFO_SC) {}
	CSRollMoneyOperaReq::CSRollMoneyOperaReq() : header(MT_ROLL_MONEY_OPERA_REQ_CS) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSZhanshendianFetchDayReward::CSZhanshendianFetchDayReward() : header(MT_ZHANSHENDIAN_FETCH_DAY_REWARD_CS) {}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSRoleGoalOperaReq::CSRoleGoalOperaReq() : header(MT_ROLE_GOAL_OPERA_REQ_CS) {}
	SCRoleGoalInfo::SCRoleGoalInfo() : header(MT_ROLE_GOAL_INFO_SC) {}
	SCRoleGoalChange::SCRoleGoalChange() : header(MT_ROLE_GOAL_CHANGE_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSRedPaperCreateReq::CSRedPaperCreateReq() : header(MT_RED_PAPER_CREATE_REQ_CS) {}
	CSRedPaperFetchReq::CSRedPaperFetchReq() : header(MT_RED_PAPER_FETCH_REQ_CS) {}
	CSRedPaperQueryDetailReq::CSRedPaperQueryDetailReq() : header(MT_RED_PAPER_QUERY_DETAIL_REQ_CS) {}
	SCRedPaperDetailInfo::SCRedPaperDetailInfo() : header(MT_RED_PAPER_DETAIL_INFO_SC) {}
	SCRedPaperFetchResult::SCRedPaperFetchResult() : header(MT_RED_PAPER_FETCH_RESULT_SC) {}
	SCRedPaperRoleInfo::SCRedPaperRoleInfo() : header(MT_RED_PAPER_ROLE_INFO_SC) {}
	CSCreateCommandRedPaper::CSCreateCommandRedPaper() : header(MT_RED_PAPER_SEND_COMMAND_CS) {}
	CSFetchCommandRedPaper::CSFetchCommandRedPaper() : header(MT_RED_PAPER_COMMAND_REWARD_CS) {}
	CSCommandRedPaperCheckInfo::CSCommandRedPaperCheckInfo() : header(MT_RED_PAPER_COMMAND_CHECK_INFO_CS) {}
	SCCommandRedPaperSendInfo::SCCommandRedPaperSendInfo() : header(MT_RED_PAPER_COMMAND_SEND_INFO_SC) {}
	
	SCMarriageSeekingInfo::SCMarriageSeekingInfo() : header(MT_MARRIAGE_SEEKING_INFO_SC) {}
	CSMarriageSeekingOpera::CSMarriageSeekingOpera() : header(MT_MARRIAGE_SEEKING_REQ_CS) {}

	CSShizhuangUseReq::CSShizhuangUseReq() : header(MT_SHIZHUANG_USE_REQ_CS) {}
	SCShizhuangInfo::SCShizhuangInfo() : header(MT_SHIZHUANG_INFO_SC) {}
	CSShizhuangSpecialImgUpgradeReq::CSShizhuangSpecialImgUpgradeReq() : header(MT_SHIZHUANG_SPECIAL_IMG_UPGRADE_CS) {}

	CSCardOperate::CSCardOperate() : header(MT_CARD_OPERATE_CS) {}
	SCCardInfo::SCCardInfo() : header(MT_CARD_INFO_SC) {}
	SCCardUplevelSucc::SCCardUplevelSucc() : header(MT_CARD_UPLEVEL_SUCC_SC) {}
	SCRoleShadowBossInfo::SCRoleShadowBossInfo() : header(MT_ROLE_SHADOW_BOSS_INFO_SC) {}

	SCFakePrivateChat::SCFakePrivateChat() : header(MT_FAKE_PRIVATE_CHAT_SC) {}
	SCQingyuanCardLevelList::SCQingyuanCardLevelList() : header(MT_QINGYUAN_CARD_LEVEL_LIST_SC) {}
	SCQingyuanCardUpdate::SCQingyuanCardUpdate() : header(MT_QINGYUAN_CARD_LEVEL_UPDATE_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSSpeaker::CSSpeaker():header(MT_LOUD_SPEAKER_CS){}
	CSGetVipWelfare::CSGetVipWelfare():header(MT_VIP_GET_WELFARE_CS){}
	CSFetchVipLevelReward::CSFetchVipLevelReward():header(MT_FETCH_VIP_LEVEL_REWARD){}

	CSGMCommand::CSGMCommand():header(MT_GM_COMMAND_CS) {}
	SCGMCommand::SCGMCommand():header(MT_GM_COMMAND_SC) {}

	CSTestSpeed::CSTestSpeed():header(MT_GM_TESTSPEED_CS) {}
	SCTestSpeed::SCTestSpeed():header(MT_GM_TESTSPEED_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCFishPoolAllRaiseInfo::SCFishPoolAllRaiseInfo():header(MT_FISH_POOL_ALL_RAISE_INFO_SC) {}
	SCFishPoolCommonInfo::SCFishPoolCommonInfo():header(MT_FISH_POOL_COMMON_INFO_SC) {}
	SCFishPoolWorldGeneralInfo::SCFishPoolWorldGeneralInfo() : header(MT_FISH_POOL_WORLD_GENERAL_INFO_SC) {}
	SCFishPoolStealGeneralInfo::SCFishPoolStealGeneralInfo() : header(MT_FISH_POOL_STEAL_GENERAL_INFO_SC) {}
	SCFishPoolShouFishRewardInfo::SCFishPoolShouFishRewardInfo() : header(MT_FISH_POOL_SHOU_FISH_REWARD_INFO_SC) {}

	CSFishPoolBuyBulletReq::CSFishPoolBuyBulletReq():header(MT_FISH_POOL_BUY_BULLET_REQ_CS) {}
	CSFishPoolRaiseReq::CSFishPoolRaiseReq():header(MT_FISH_POOL_RAISE_REQ_CS) {}
	CSFishPoolQueryReq::CSFishPoolQueryReq():header(MT_FISH_POOL_QUERY_REQ_CS) {}
	CSFishPoolStealFish::CSFishPoolStealFish():header(MT_FISH_POOL_STEAL_FISH_CS) {}
	CSFishPoolHarvest::CSFishPoolHarvest():header(MT_FISH_POOL_HARVEST_FISH_CS) {}
	SCUpFishQualityRet::SCUpFishQualityRet() : header(MT_FISH_POOL_UP_FISH_QUALITY_RET_SC) {}
	SCFishPoolChange::SCFishPoolChange() : header(MT_FISH_POOL_FISH_POOL_CHANGE) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCWorldEventCommonInfo::SCWorldEventCommonInfo():header(MT_WORLD_EVENT_COMMON_INFO_SC) {}
	CSWorldEventQueryCommonInfo::CSWorldEventQueryCommonInfo():header(MT_WORLD_EVENT_COMMON_INFO_REQ_CS) {}
	SCWorldEventRoleInfo::SCWorldEventRoleInfo():header(MT_WORLD_EVENT_ROLE_INFO_SC) {}
	CSWorldEventQueryRoleInfo::CSWorldEventQueryRoleInfo():header(MT_WORLD_EVENT_QUERY_ROLE_INFO_CS) {}
	CSWorldEventFetchReward::CSWorldEventFetchReward():header(MT_WORLD_EVENT_FETCH_REWARD_CS) {}
	CSWorldEventObjTouch::CSWorldEventObjTouch():header(MT_WORLD_EVENT_OBJ_TOUCH_CS) {}
	CSWorldEventLightWord::CSWorldEventLightWord():header(MT_WORLD_EVENT_LIGHT_WORD_CS) {}
	SCWorldEventNpcLocateInfo::SCWorldEventNpcLocateInfo():header(MT_WORLD_EVENT_NPC_LOCATE_INFO_SC) {}
	CSWorldEventGetNpcLocateInfo::CSWorldEventGetNpcLocateInfo():header(MT_WORLD_EVENT_GET_NPC_LOCATE_INFO_CS) {}
	SCWorldEventKillFindNpcMonster::SCWorldEventKillFindNpcMonster():header(MT_WORLD_EVENT_KILL_FIND_NPC_MONSTER_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSMailSend::CSMailSend():header(MT_MAIL_SEND_CS){}
	CSMailDelete::CSMailDelete():header(MT_MAIL_DELETE_CS){}
	CSMailGetList::CSMailGetList():header(MT_MAIL_GETLIST_CS){}
	CSMailRead::CSMailRead():header(MT_MAIL_READ_CS){}
	CSMailFetchAttachment::CSMailFetchAttachment():header(MT_MAIL_ATTACHMENT_CS){}
	CSMailClean::CSMailClean():header(MT_MAIL_CLEAN_CS){}
	CSMailOneKeyFetchAttachment::CSMailOneKeyFetchAttachment():header(MT_MAIL_ONE_KEY_FETCH_ATTACHMENT){}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSAddPublicSaleItem::CSAddPublicSaleItem():header(MT_ADD_PUBLICSALE_ITEM_CS){}
	CSRemovePublicSaleItem::CSRemovePublicSaleItem():header(MT_REMOVE_PUBLICSALE_ITEM_CS){}
	CSBuyPublicSaleItem::CSBuyPublicSaleItem():header(MT_BUY_PUBLICSALE_BUY_CS){}
	CSPublicSaleGetUserItemList::CSPublicSaleGetUserItemList():header(MT_PUBLICSALE_GET_ITEM_LIST_CS){}
	CSPublicSaleSendItemInfoToWorld::CSPublicSaleSendItemInfoToWorld():header(MT_PBULICSALE_SEND_ITEM_INFO_TO_WORLD_CS){}
	CSPublicSaleTypeCountReq::CSPublicSaleTypeCountReq():header(MT_PBULICSALE_TYPE_COUNT_REQ_CS) {}
	SCPublicSaleTypeCountAck::SCPublicSaleTypeCountAck():header(MT_PBULICSALE_TYPE_COUNT_ACK_CS) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSTrainMentality::CSTrainMentality() : header(MT_MENTALITY_TRAIN_CS) {}
	CSUpgradeMentalityGengu::CSUpgradeMentalityGengu() : header(MT_MENTALITY_UPGRADE_GENGU_CS) {}
	CSSpeedUpMentalityTrain::CSSpeedUpMentalityTrain() : header(MT_MENTALITY_SPEED_UP_TRAIN_CS) {}
	SCMentalityList::SCMentalityList() : header(MT_MENTALITY_LIST_SC) {}
	CSMentalityYijianTisheng::CSMentalityYijianTisheng() : header(MT_MENTALITY_YJTS_CS) {}
	CSMentalityUplevelWuxing::CSMentalityUplevelWuxing() : header(MT_MENTALITY_UPLEVEL_WUXING_CS) {}
	SCMentalityYijianTishengAck::SCMentalityYijianTishengAck() : header(MT_MENTALITY_YJTS_SC) {}
	CSMentalityClearTrainCD::CSMentalityClearTrainCD() : header(MT_MENTALITY_CLEAR_TRAIN_CD_CS) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSCreateGuild::CSCreateGuild() : header(MT_CREATE_GUILD_CS) {}
	CSDismissGuild::CSDismissGuild() : header(MT_DISMISS_GUILD_CS) {}
	CSApplyForJoinGuild::CSApplyForJoinGuild() : header(MT_APPLY_FOR_JOIN_GUILD_CS) {}
	CSApplyForJoinGuildAck::CSApplyForJoinGuildAck() : header(MT_APPLY_FOR_JOIN_GUILD_ACK_CS) {}
	CSQuitGuild::CSQuitGuild() : header(MT_QUIT_GUILD_CS) {}
	CSInviteGuild::CSInviteGuild() : header(MT_INVITE_GUILD_CS) {}
	CSInviteGuildAck::CSInviteGuildAck() : header(MT_INVITE_GUILD_ACK_CS) {}
	CSKickoutGuild::CSKickoutGuild() : header(MT_KICKOUT_GUILD_CS) {}
	CSAppointGuild::CSAppointGuild() : header(MT_APPOINT_GUILD_CS) {}
	CSLeaveGuildPost::CSLeaveGuildPost() : header(MT_LEAVE_GUILD_POST_CS) {}
	CSGuildChangeNotice::CSGuildChangeNotice() : header(MT_CHANGE_GUILD_NOTICE_CS) {}
	CSGuildMailAll::CSGuildMailAll() : header(MT_GUILD_MAIL_ALL_CS) {}
	CSGetGuildInfo::CSGetGuildInfo() : header(MT_GET_GUILD_INFO_CS) {}
	CSGuildDelate::CSGuildDelate() : header(MT_GUILD_DELATE_CS) {}
	CSGuildBackToStation::CSGuildBackToStation() : header(MT_GUILD_BACK_TO_STATION_CS) {}
	CSGuildResetName::CSGuildResetName() : header(MT_GUILD_RESET_NAME_CS) {}
	CSApplyforSetup::CSApplyforSetup() : header(MT_GUILD_APPLYFOR_SETUP_CS) {}
	CSAddGuildExp::CSAddGuildExp() : header(MT_GUILD_ADDEXP_CS) {}
	CSGuildUpLevel::CSGuildUpLevel() : header(MT_GUILD_UP_LEVEL_CS) {}
	CSGuildGetBuff::CSGuildGetBuff() : header(MT_GUILD_GET_BUFF_CS) {}
	CSGuildExchange::CSGuildExchange() : header(MT_GUILD_EXCHANGE_CS) {}
	CSGuildSetAutoKickoutSetup::CSGuildSetAutoKickoutSetup() : header(MT_GUILD_SET_AUTO_KICKOUT_SETUP_CS) {}
	CSGuildCallIn::CSGuildCallIn() : header(MT_GUILD_CALL_IN_CS) {}
	SCGuildBackToStation::SCGuildBackToStation() : header(MT_GUILD_BACK_TO_STATION_SC) {}
	SCAddGuildExpSucc::SCAddGuildExpSucc() : header(MT_GUILD_ADD_GUILD_EXP_SC) {}
	SCGuildFbStatus::SCGuildFbStatus() : header(MT_GUILD_FB_STATUS_SC) {}
	SCGuildBossInfo::SCGuildBossInfo() : header(MT_GUILD_BOSS_SC) {}
	SCGuildBossActivityInfo::SCGuildBossActivityInfo() : header(MT_GUILD_BOSS_ACTIVITY_SC) {}
	SCGuildStorgeInfo::SCGuildStorgeInfo() : header(MT_GUILD_STORGE_SC) {}
	SCGuildStorgeChange::SCGuildStorgeChange() : header(MT_GUILD_STORGE_CHANGE_SC) {}
	SCGuildBoxInfo::SCGuildBoxInfo() : header(MT_GUILD_BOX_INFO_SC) {}
	SCGuildBoxNeedAssistInfo::SCGuildBoxNeedAssistInfo() : header(MT_GUILD_BOX_NEED_ASSIST_INFO_SC) {}
	SCGuildStorageLogList::SCGuildStorageLogList() : header(MT_GUILD_STORGE_LOG_LIST_SC) {}

	CSGuildRedPaperListInfoReq::CSGuildRedPaperListInfoReq() : header(MT_GUILD_RED_PAPER_REQ_LIST_INFO_CS) {}
	SCGuildRedPaperListInfo::SCGuildRedPaperListInfo() : header(MT_GUILD_RED_PAPER_LIST_INFO_SC) {}
	CSCreateGuildRedPaperReq::CSCreateGuildRedPaperReq() : header(MT_GUILD_ROLE_GUILD_PAPER_CREATE_CS) {}
	SCNoticeGuildPaperInfo::SCNoticeGuildPaperInfo():header(MT_GUILD_ROLE_GUILD_PAPER_INFO_SC) {}
	CSSingleChatRedPaperRole::CSSingleChatRedPaperRole() : header(MT_GUILD_ROLE_GUILD_PAPER_SINGAL_CHAT_CS) {}
	CSGuildMazeOperate::CSGuildMazeOperate() : header(MT_GUILD_MAZE_OPERATE_CS) {}
	SCGuildMemberMazeInfo::SCGuildMemberMazeInfo() : header(MT_GUILD_MEMBER_MAZE_INFO_SC) {}
	SCGuildMazeRankInfo::SCGuildMazeRankInfo() : header(MT_GUILD_MAZE_RANK_INFO_SC) {}
	CSGuildSinginReq::CSGuildSinginReq() : header(MT_GUILD_SIGNIN_REQ_CS) {}
	SCGuildSinginAllInfo::SCGuildSinginAllInfo() : header(MT_GUILD_SIGNIN_ALL_INFO_SC) {}
	CSGuildChangeAvatar::CSGuildChangeAvatar() : header(MT_GUILD_CHANGE_AVATAR_CS) {}
	SCGuildSendAppearance::SCGuildSendAppearance() : header(MT_GUILD_APPEARANCE_INFO_SC){}

	SCGuildXianshuInfo::SCGuildXianshuInfo() : header(MT_GUILD_XIANSHU_INFO_SC) {}
	CSGuildXianshuInfoReq::CSGuildXianshuInfoReq() : header(MT_GUILD_XIANSHU_INFO_REQ_CS) {}
	CSGuildXianshuUplevelReq::CSGuildXianshuUplevelReq() : header(MT_GUILD_XIANSHU_UPLEVEL_CS) {}

	// 军团
	SCCreateGuild::SCCreateGuild():header(MT_CREATE_GUILD_SC){}
	SCDismissGuild::SCDismissGuild():header(MT_DISMISS_GUILD_SC){}
	SCApplyForJoinGuild::SCApplyForJoinGuild():header(MT_APPLY_FOR_JOIN_GUILD_SC){}
	SCApplyForJoinGuildAck::SCApplyForJoinGuildAck():header(MT_APPLY_FOR_JOIN_GUILD_ACK_SC){}
	SCNotifyGuildSuper::SCNotifyGuildSuper():header(MT_NOTIFY_SUPER_SC){}
	SCQuitGuild::SCQuitGuild():header(MT_QUIT_GUILD_SC){}
	SCInviteGuild::SCInviteGuild():header(MT_INVITE_GUILD_SC){}
	SCInviteNotify::SCInviteNotify():header(MT_INVITE_GUILD_NOTIFY_SC){}
	SCKickoutGuild::SCKickoutGuild():header(MT_KICK_OUT_GUILD_SC){}
	SCKickoutGuildNotify::SCKickoutGuildNotify():header(MT_KICK_OUT_GUILD_NOTIFY_SC){}
	SCAppointGuild::SCAppointGuild():header(MT_APPOINT_GUILD_SC){}
	SCLeavePost::SCLeavePost():header(MT_LEAVEPOST_GUILD_SC){}
	SCChangeNotice::SCChangeNotice():header(MT_CHANGE_NOTICE_SC){}
	SCGuildMailAll::SCGuildMailAll():header(MT_MAILALL_SC){}
	SCAllGuildBaseInfo::SCAllGuildBaseInfo():header(MT_GUILD_ALL_BASE_INFO_SC){}
	SCGuildGetApplyForList::SCGuildGetApplyForList():header(MT_GUILD_APPLY_FOR_LIST_SC){}
	SCApplyForJoinGuildList::SCApplyForJoinGuildList():header(MT_APPLY_FOR_JOIN_GUILD_LIST_SC){}
	SCGuildEventList::SCGuildEventList():header(MT_GUILD_EVENT_LIST_SC){}
	SCGuildMemberList::SCGuildMemberList():header(MT_GUILD_MEMBER_LIST_SC){}
	SCGuildBaseInfo::SCGuildBaseInfo():header(MT_GUILD_BASE_INFO_SC){}
	SCGuildMemberSos::SCGuildMemberSos():header(MT_GUILD_MEMBER_SOS_SC){}
	SCInviteGuildList::SCInviteGuildList():header(MT_GUILD_INVITE_LIST_SC){} 
	SCGuildResetName::SCGuildResetName():header(MT_GUILD_RESET_NAME_SC) {}
	SCGuildRoleGuildInfo::SCGuildRoleGuildInfo():header(MT_GUILD_ROLE_GUILD_INFO_SC) {}

	CSGuildFbStartReq::CSGuildFbStartReq():header(MT_GUILD_FB_START_REQ_CS){}
	CSGuildFbEnterReq::CSGuildFbEnterReq():header(MT_GUILD_FB_ENTER_REQ_CS){}
	CSGuildExtendMemberReq::CSGuildExtendMemberReq():header(MT_GUILD_EXTEND_MEMBER_CS){}
	CSGuildZhuLuckyReq::CSGuildZhuLuckyReq():header(MT_GUILD_ZHUFU_LUCKY_REQ_CS){}
	CSGetAllGuildMemberLuckyInfo::CSGetAllGuildMemberLuckyInfo():header(MT_GUILD_GET_GUILD_ALL_LUCKY_REQ_CS){}
	CSInviteLuckyZhufu::CSInviteLuckyZhufu():header(MT_GUILD_INVITE_ZHUFU_LUCKY_CS){}	
	CSGuildCallBeastReq::CSGuildCallBeastReq():header(MT_GUILD_CALL_BEAST_CS){}

	SCGuildBonfireStatus::SCGuildBonfireStatus() : header(MT_GUILD_BONFIRE_STATUS_SC) {}
	CSGuildBossOperate::CSGuildBossOperate() : header(MT_GUILD_BOSS_CS) {}
	CSGuildBonfireStartReq::CSGuildBonfireStartReq() : header(MT_GUILD_BONFIRE_START_REQ_CS) {}
	CSGuildBonfireGotoReq::CSGuildBonfireGotoReq() : header(MT_GUILD_BONFIRE_GOTO_REQ_CS) {}
	CSGuildBonfireAddMucaiReq::CSGuildBonfireAddMucaiReq() : header(MT_GUILD_BONFIRE_ADDMUCAI_REQ_CS) {}
	CSGuildStorgeOperate::CSGuildStorgeOperate() : header(MT_GUILD_STORGE_REQ_CS) {}
	CSGuildStorgeOneKeyOperate::CSGuildStorgeOneKeyOperate() : header(MT_GUILD_STORGE_ONE_KEY_REQ_CS) {}
	CSGuildSetAutoClearReq::CSGuildSetAutoClearReq() : header(MT_GUILD_SET_AUTO_CLEAR_REQ) {}
	CSGuildSkillUplevel::CSGuildSkillUplevel() : header(MT_GUILD_SKILL_UPLEVEL_CS) {}
	CSGuildUpTotemLevel::CSGuildUpTotemLevel() : header(MT_GUILD_TOTEM_UP_LEVEL_CS) {}
	CSGuildBoxOperate::CSGuildBoxOperate() : header(MT_GUILD_BOX_OPERATE_CS) {}
	CSGuildCommonReq::CSGuildCommonReq() : header(MT_GUILD_COMMON_REQ_CS) {}
	CSGuildTerritoryWelfOperate::CSGuildTerritoryWelfOperate() : header(MT_GUILD_FETCH_TERRITORY_REWARD_CS) {}
	CSFetchGuildBossRedbag::CSFetchGuildBossRedbag() : header(MT_GUILD_FETCH_GUILD_BOSS_REDBAG) {}
	SCGulidReliveTimes::SCGulidReliveTimes() : header(MT_GUILD_FETCH_GUILD_RELIVE_TIMES_INFO) {}
	SCGulidBossRedbagInfo::SCGulidBossRedbagInfo() : header(MT_GUILD_FETCH_GUILD_BOSS_REDBAG_INFO) {}
	SCGuildBossRedBagInfo::SCGuildBossRedBagInfo() : header(MT_GUILD_FETCH_GUILD_BOSS_FETCH_INFO) {}
	SCGulidSaiziInfo::SCGulidSaiziInfo() : header(MT_GUILD_FETCH_GUILD_SAIZI_INFO) {}
	CSGulidPaoSaizi::CSGulidPaoSaizi() : header(MT_GUILD_FETCH_GUILD_PAO_SAIZI) {}
	CSReqGulidSaiziInfo::CSReqGulidSaiziInfo() : header(MT_GUILD_FETCH_GUILD_SAIZI_INFO_REQ) {}

	SCInviteLuckyZhufu::SCInviteLuckyZhufu():header(MT_GUILD_INVITE_ZHUFU_LUCKY_SC){}
	SCGuildLuckyInfo::SCGuildLuckyInfo():header(MT_GUILD_GUILD_ALL_LUCKY_SC){}
	SCGuildLuckyChangeNotice::SCGuildLuckyChangeNotice():header(MT_GUILD_LUCKY_CHANGE_NOTICE_SC){}
	SCGuildActiveDegreeInfo::SCGuildActiveDegreeInfo():header(MT_GUILD_ACTIVE_DEGREE_INFO_SC){}

	CSGuildCheckCanDelate::CSGuildCheckCanDelate():header(MT_GUILD_CHECK_CAN_DELATE_CS) {}
	SCGuildCheckCanDelateAck::SCGuildCheckCanDelateAck():header(MT_GUILD_CHECK_CAN_DELATE_ACK_SC) {}
	SCGuildMemberNum::SCGuildMemberNum():header(MT_GUILD_CUR_MAX_MEMBER_COUNT_SC) {}
	SCGuildOperaSucc::SCGuildOperaSucc():header(MT_GUILD_OPERA_SUCC_SC){}

	CSBiaoCheOpera::CSBiaoCheOpera() : header(MT_GUILD_BIAOCHE_OPERA_CS) {}

	SCGuildTianCiTongBiResult::SCGuildTianCiTongBiResult() : header(MT_GUILD_TIANCI_TONGBI_RESULT_SC) {}
	SCGuildSyncTianCiTongBi::SCGuildSyncTianCiTongBi() : header(MT_GUILD_SYNC_TIANCI_TONGBI) {}
	SCGuildTianCiTongBiUserGatherChange::SCGuildTianCiTongBiUserGatherChange() : header(MT_GUILD_TIANCI_TONGBI_USER_GATHER_CHANGE_SC) {}
	SCGuildTianCiTongBiRankInfo::SCGuildTianCiTongBiRankInfo() : header(MT_GUILD_TIANCI_TONGBI_RANK_INFO_SC) {}
	CSGuildTianCiTongBiReq::CSGuildTianCiTongBiReq() : header(MT_GUILD_TIANCI_TONGBI_REQ_CS) {}
	SCGuildTianCiTongBiNpcInfo::SCGuildTianCiTongBiNpcInfo() : header(MT_GUILD_TIANCI_TONGBI_NPC_INFO) {}
	SCGuildTianCiTongBiGatherAOIInfo::SCGuildTianCiTongBiGatherAOIInfo() : header(MT_GUILD_TIANCI_TONGBI_AOI_GATHER_INFO) {}
	SCGuildGongziRankList::SCGuildGongziRankList() : header(MT_GUILD_GONGZI_LIST_INFO) {}
	SCGuildEnemyRankList::SCGuildEnemyRankList() : header(MT_GUILD_ENEMY_RANK_LIST_SC) {}
	CSGuildEnemyRankList::CSGuildEnemyRankList() : header(MT_GUILD_ENEMY_RANK_LIST_CS) {}

	//SCGuildBossList::SCGuildBossList():header(MT_GUILD_OPERA_SUCC_SC){}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCSelfChestShopItemList::SCSelfChestShopItemList() : header(MT_CHEST_SHOP_SELF_ITEM_LIST_SC) {}
	SCChestShopItemListPerBuy::SCChestShopItemListPerBuy() : header(MT_CHEST_SHOP_ITEM_LIST_PER_BUY_SC) {}
	SCChestShopFreeInfo::SCChestShopFreeInfo() : header(MT_CHEST_SHOP_FREE_INFO_SC) {}
	CSRareChestShopReq::CSRareChestShopReq() : header(MT_RARE_CHEST_SHOP_BUY_CS) {}

	CSMoveChessResetReq::CSMoveChessResetReq() : header(MT_MOVE_CHESS_RESET_REQ_CS) {}
	CSMoveChessShakeReq::CSMoveChessShakeReq() : header(MT_MOVE_CHESS_SHAKE_REQ_CS) {}
	SCMoveChessShakePoint::SCMoveChessShakePoint() : header(MT_MOVE_CHESS_SHAKE_SC) {}
	CSMoveChessFreeInfo::CSMoveChessFreeInfo() : header(MT_MOVE_CHESS_FREE_INFO_REQ_CS) {}

	CSChestShopRecordList::CSChestShopRecordList() : header(MT_CHEST_SHOP_GET_RECORD_LIST_CS) {}
	SCChestShopRecordList::SCChestShopRecordList() : header(MT_CHEST_SHOP_SEND_RECORD_LIST_SC) {}
	CSGetSelfChestShopItemList::CSGetSelfChestShopItemList() : header(MT_CHEST_SHOP_GET_SELF_ITEM_LIST_CS) {}
	CSBuyChestShopItem::CSBuyChestShopItem() : header(MT_CHEST_SHOP_BUY_ITEM_CS) {}
	CSFetchChestShopItem::CSFetchChestShopItem() : header(MT_CHEST_SHOP_FETCH_ITEM_CS) {}
	CSChestShopGetFreeInfo::CSChestShopGetFreeInfo() : header(MT_CHEST_SHOP_GET_FREE_INFO) {}
	CSChestShopAutoRecycle::CSChestShopAutoRecycle() : header(MT_CHEST_SHOP_AUTO_RECYCLE_CS) {}
	CSShopBuy::CSShopBuy() : header(MT_SHOP_BUY_ITEM_CS) {} 
	CSScoreToItemConvert::CSScoreToItemConvert() : header(MT_CONVERT_SCORE_TO_ITEM_CS) {}
	CSGetConvertRecordInfo::CSGetConvertRecordInfo() : header(MT_CONVERT_RECORD_GET_INFO_CS) {}
	CSGetSocreInfoReq::CSGetSocreInfoReq() : header(MT_CONVERT_GET_SOCRE_INFO_CS) {}
	SCSendScoreInfo::SCSendScoreInfo() : header(MT_CONVERT_GET_SOCRE_INFO_SC) {}
	SCSendScoreInfoNotice::SCSendScoreInfoNotice() : header(MT_CONVERT_GET_SOCRE_INFO_NOTICE_SC) {}
	CSMysteriosshopOperate::CSMysteriosshopOperate() : header(MT_MYSTERIOUSSHOP_OPERATE_CS) {} 
	SCConvertRecordInfo::SCConvertRecordInfo() : header(MT_CONVERT_RECORD_INFO_SC) {}
	SCMysteriosNpcRefresh::SCMysteriosNpcRefresh() : header(MT_MYSTERIOUS_NPC_REFRESH_SC) {}
	SCMysteriosshopInfo::SCMysteriosshopInfo() : header(MT_MYSTERIOUS_SHOPITEM_INFO_SC) {}
	CSMysteriosshopinMallOperate::CSMysteriosshopinMallOperate() : header(MT_MYSTERIOUS_SHOP_BUY_OPERATE_CS) {}
	SCSendMysteriosshopItemInfo::SCSendMysteriosshopItemInfo() : header(MT_MYSTERIOUS_SHOP_SEQ_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCAllXiannvInfo::SCAllXiannvInfo() : header(MT_XIANNV_ALL_INFO_SC) {}
	SCXiannvInfo::SCXiannvInfo() : header(MT_XIANNV_INFO_SC) {}
	SCXiannvViewChange::SCXiannvViewChange() : header(MT_XIANNV_VIEW_CHANGE_SC) {}
	CSXiannvRename::CSXiannvRename() : header(MT_XIANNV_RENAME_CS) {}
	CSXiannvCall::CSXiannvCall() : header(MT_XIANNV_CALL_CS) {}
	CSXiannvActiveReq::CSXiannvActiveReq() : header(MT_XIANNV_ACTIVE_REQ_CS) {}
	CSXiannvAddZizhiReq::CSXiannvAddZizhiReq() : header(MT_XIANNV_ADD_ZIZHI_REQ_CS) {}
	CSXiannvActiveHuanhua::CSXiannvActiveHuanhua() : header(MT_XIANNV_ACTIVE_HUANHUA_REQ_CS) {}
	CSXiannvImageReq::CSXiannvImageReq() : header(MT_XIANNV_CHANGEIMAGE_REQ_CS) {}
	CSXiannvHuanHuaUpLevelReq::CSXiannvHuanHuaUpLevelReq() : header(MT_XIANNV_HUANGUA_UPLEVEL_REQ_CS) {}
	CSXiannvChangeSkillReq::CSXiannvChangeSkillReq() : header(MT_XIANNV_CHANGE_SKILL_REQ_CS) {}
	CSXiannvShengwuReq::CSXiannvShengwuReq() : header(MT_XIANNV_SHENGWU_REQ_CS) {}
	SCXiannvShengwuChangeInfo::SCXiannvShengwuChangeInfo() : header(MT_XIANNV_SHENGWU_CHANGE_INFO_SC) {}
	SCXiannvShengwuMilingList::SCXiannvShengwuMilingList() : header(MT_XIANNV_SHENGWU_MILING_LIST_SC) {}
	SCXiannvShengwuChouExpList::SCXiannvShengwuChouExpList() : header(MT_XIANNV_SHENGWU_CHOU_EXP_LIST_SC) {}
	SCXiannvShengwuChouExpResult::SCXiannvShengwuChouExpResult() : header(MT_XIANNV_SHENGWU_CHOU_EXP_RESULT_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSFamousManOpera::CSFamousManOpera() : header(MT_FAMOUS_MAN_OPERA_REQ_CS) {}
	SCSendFamousManInfo::SCSendFamousManInfo() : header(MT_FAMOUS_MAN_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSTurnTableReq::CSTurnTableReq() : header(MT_TURNTABLE_ROLL_REQ_CS) {}
	SCTurnTableInfo::SCTurnTableInfo() : header(MT_TURNTABLE_INFO_SC) {}
	SCTurnTableReward::SCTurnTableReward() : header(MT_TURNTABLE_REWARD_SC) {}
	SCTurnTableMillionaireView::SCTurnTableMillionaireView() : header(MT_TURNTABLE_MILLIONAIRE_VIEW_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSWabaoOperaReq::CSWabaoOperaReq() : header(MT_WABAO_OPERA_REQ_CS) {}
	SCWabaoInfo::SCWabaoInfo() : header(MT_WABAO_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSJilianOpera::CSJilianOpera() : header(MT_JILIAN_OPERA_CS) {}
	SCJilianInfo::SCJilianInfo() : header(MT_JILIAN_INFO_SC) {}
	SCJianlianViewChange::SCJianlianViewChange() : header(MT_JILIAN_VIEW_CHANGE_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSQingyuanFBOperaReq::CSQingyuanFBOperaReq() : header(MT_QINGYUAN_FB_OPERA_REQ_CS) {}
	SCQingyuanBaseFBInfo::SCQingyuanBaseFBInfo() : header(MT_QINGYUAN_BASE_FB_INFO_SC) {}
	SCWeddingRoleInfo::SCWeddingRoleInfo() : header(MT_HUNYAN_ROLE_INFO_SC) {}
	SCQingyuanFBInfo::SCQingyuanFBInfo() : header(MT_QINGYUAN_FB_INFO_SC) {}
	SCQingyuanFBRewardRecordInfo::SCQingyuanFBRewardRecordInfo() : header(MT_QINGYUAN_FB_REWARD_RECORD_INFO_SC) {}
	CSHunjieUpLevel::CSHunjieUpLevel() : header(MT_HUNJIE_UP_LEVEl_CS) {}
	SCHunjieEquipInfo::SCHunjieEquipInfo() : header(MT_HUNJIE_EQUIP_INFO_SC) {}
	SCHunjieEquipUpLevelSendLover::SCHunjieEquipUpLevelSendLover() : header(MT_HUNJIE_EQUIP_UPLEVEL_SEND_LOVER_INFO_SC) {}
	SCQingyuanBlessInfo::SCQingyuanBlessInfo() : header(MT_QINGYUAN_BLESS_INFO_SC) {}
	CSQingyuanTakeOffEquip::CSQingyuanTakeOffEquip() : header(MT_QINGYUAN_TAKE_OFF_EQUIP) {}
	CSHunjieReqEquipInfo::CSHunjieReqEquipInfo() : header(MT_HUNJIE_REQ_EQUIP_INFO_CS) {}
	CSQingyuanQueryMateQingyuanValue::CSQingyuanQueryMateQingyuanValue() : header(MT_QINGYUAN_QUERY_MATE_QINGYUAN_VALUE_CS) {}
	SCQingyuanMateQingyuanValue::SCQingyuanMateQingyuanValue() : header(MT_QINGYUAN_MATE_QINGYUAN_VALUE_SC) {}
	CSQingyuanDivorceReq::CSQingyuanDivorceReq() : header(MT_QINGYUAN_DIVORCE_REQ_CS) {}
	SCQingyuanHunyanInviteInfo::SCQingyuanHunyanInviteInfo() : header(MT_QINGYUAN_HUNYAN_INVITE_SC) {}
	CSQingyuanEquipOperate::CSQingyuanEquipOperate() : header(MT_QINGYUAN_EQUIP_OPERATE_CS) {}
	SCQingyuanEquipInfo::SCQingyuanEquipInfo() : header(MT_QINGYUAN_EQUIP_INFO_SC) {}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSZhuanShengOpearReq::CSZhuanShengOpearReq() : header(MT_ZHUANSHENG_OPERA_REQ_CS) {}
	SCZhuanShengAllInfo::SCZhuanShengAllInfo() : header(MT_ZHUANSHENG_ALL_INFO_SC) {}
	SCZhuanShengBackpackInfo::SCZhuanShengBackpackInfo() : header(MT_ZHUANSHENG_BACKPACK_INFO_SC) {}
	SCZhuanShengOtherInfo::SCZhuanShengOtherInfo() : header(MT_ZHUANSHENG_OTHER_INFO_SC) {}
	SCZhuanShengXiuweiNotice::SCZhuanShengXiuweiNotice() : header(MT_ZHUANSHENG_XIUWEI_NOTICE_SC) {}
	SCZhuanShengChouResult::SCZhuanShengChouResult() : header(MT_ZHUANSHENG_CHOU_RESULT_SC) {}
	SCZhuanShengCombineResult::SCZhuanShengCombineResult() : header(MT_ZHUANSHENG_COMBINE_RESULT_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSRuneSystemReq::CSRuneSystemReq() : header(MT_RUNE_SYSTEM_REQ_CS) {}
	CSRuneSystemDisposeOneKey::CSRuneSystemDisposeOneKey() : header(MT_RUNE_SYSTEM_DISPOSE_ONE_KEY_CS) {}
	SCRuneSystemBagInfo::SCRuneSystemBagInfo() : header(MT_RUNE_SYSTEM_BAG_INFO_SC) {}
	SCRuneSystemRuneGridInfo::SCRuneSystemRuneGridInfo() : header(MT_RUNE_SYSTEM_RUNE_GRID_INFO_SC) {}
	SCRuneSystemOtherInfo::SCRuneSystemOtherInfo() : header(MT_RUNE_SYSTEM_RUNE_OTHER_INFO_SC) {}
	CSRuneTowerFetchTime::CSRuneTowerFetchTime() : header(MT_RUNE_TOWER_FETCH_TIME_CS) {}
	SCRuneTowerInfo::SCRuneTowerInfo() : header(MT_RUNE_TOWER_INFO_SC) {}
	SCRuneTowerFbInfo::SCRuneTowerFbInfo() : header(MT_RUNE_TOWER_FB_INFO_SC) {}
	SCRuneTowerOfflineInfo::SCRuneTowerOfflineInfo() : header(MT_RUNE_TOWER_OFFLINE_INFO_SC) {}
	SCRuneSystemComposeInfo::SCRuneSystemComposeInfo() : header(MT_RUNE_SYSTEM_COMPOSE_INFO_SC) {}
	SCRuneSystemRuneGridAwakenInfo::SCRuneSystemRuneGridAwakenInfo() : header(MT_RUNE_SYSTEM_AWAKEN_INFO_SC) {}
	CSRuneTowerAutoFb::CSRuneTowerAutoFb() : header(MT_RUNE_SYSTEM_AUTO_FB_CS) {}
	SCRuneTowerAutoReward::SCRuneTowerAutoReward() : header(MT_RUNE_SYSTEM_AUTO_REWARD_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCZhanShenDianFBInfo::SCZhanShenDianFBInfo() : header(MT_ZHANSHENDIAN_FB_INFO_SC) {}
	SCZhanShenDianFBResult::SCZhanShenDianFBResult() : header(MT_ZHANSHENDIAN_FB_RESULT_SC) {}
	CSFBReqNextLevel::CSFBReqNextLevel() : header(MT_FB_REQ_NEXT_LEVEL_CS) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCMountInfo::SCMountInfo() : header(MT_MOUNT_INFO_SC) {}
	SCMountAppeChange::SCMountAppeChange() : header(MT_MOUNT_APPE_CHANGE_SC) {}

	CSGoonMount::CSGoonMount() : header (MT_MOUNT_GOON_REQ_CS) {}
	CSUpgradeMount::CSUpgradeMount() : header (MT_MOUNT_UPGRADE_REQ_CS) {}
	CSUseMountImage::CSUseMountImage() : header (MT_MOUNT_USE_IMAGE_CS) {}
	CSMountGetInfo::CSMountGetInfo() : header(MT_MOUNT_GET_MOUNT_INFO_CS) {}
	CSMountUplevelEquip::CSMountUplevelEquip() : header(MT_MOUNT_UPLEVEL_EQUIP_CS) {}
	CSMountSkillUplevelReq::CSMountSkillUplevelReq() : header(MT_MOUNT_SKILL_UPLEVEL_REQ_CS) {}
	CSMountSpecialImgUpgrade::CSMountSpecialImgUpgrade():header(MT_MOUNT_SPECIAL_IMG_UPGRADE_CS){}
	CSMountUpStarLevel::CSMountUpStarLevel() : header(MT_MOUNT_UP_STAR_LEVEL_CS) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCMojieInfo::SCMojieInfo() : header(MT_MOJIE_INFO_SC) {}
	CSMojieGetInfo::CSMojieGetInfo() : header(M_MOJIE_INFO_REQ_CS) {}
	CSMojieUplevelReq::CSMojieUplevelReq() : header(M_MOJIE_UP_LEVEL_REQ_CS) {}
	CSMojieChangeSkillReq::CSMojieChangeSkillReq() : header(M_MOJIE_CHANGE_SKILL_REQ_CS) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCFightMountInfo::SCFightMountInfo() : header(MT_FIGHT_MOUNT_INFO_SC) {}
	SCFightMountAppeChange::SCFightMountAppeChange() : header(MT_FIGHT_MOUNT_APPE_CHANGE_SC) {}

	CSGoonFightMount::CSGoonFightMount() : header (MT_FIGHT_MOUNT_GOON_REQ_CS) {}
	CSUpgradeFightMount::CSUpgradeFightMount() : header (MT_FIGHT_MOUNT_UPGRADE_REQ_CS) {}
	CSUseFightMountImage::CSUseFightMountImage() : header (MT_FIGHT_MOUNT_USE_IMAGE_CS) {}
	CSFightMountGetInfo::CSFightMountGetInfo() : header(MT_FIGHT_MOUNT_GET_MOUNT_INFO_CS) {}
	CSFightMountUplevelEquip::CSFightMountUplevelEquip() : header(MT_FIGHT_MOUNT_UPLEVEL_EQUIP_CS) {}
	CSFightMountSkillUplevelReq::CSFightMountSkillUplevelReq() : header(MT_FIGHT_MOUNT_SKILL_UPLEVEL_REQ_CS) {}
	CSFightMountSpecialImgUpgrade::CSFightMountSpecialImgUpgrade():header(MT_FIGHT_MOUNT_SPECIAL_IMG_UPGRADE_CS){}
	CSFightMountUpStarLevel::CSFightMountUpStarLevel():header(MT_FIGHT_MOUNT_UP_STAR_LEVEL_CS){}

	//////////////////////////////////////////////////////////////////////////
	CSGetWorldBossInfo::CSGetWorldBossInfo():header(MT_GET_WORLDBOSS_INFO_CS) {}
	SCWorldBossInfoToAll::SCWorldBossInfoToAll():header(MT_WORLD_BOSS_INFO_TO_ALL_SC) {}
	SCWorldBossInfoList::SCWorldBossInfoList():header(MT_WORLDBOSS_INFO_SC) {}
	SCWorldBossBorn::SCWorldBossBorn():header(MT_WORLDBOSS_BORN_SC) {}
	CSWorldBossPersonalHurtInfoReq::CSWorldBossPersonalHurtInfoReq():header(MT_GET_WORLDBOSS_PRESONAL_HURT_INFO_CS) {}
	SCWorldBossSendPersonalHurtInfo::SCWorldBossSendPersonalHurtInfo():header(MT_SEND_WORLDBOSS_PERSONAL_HURT_INFO_SC) {}
	CSWorldBossGuildHurtInfoReq::CSWorldBossGuildHurtInfoReq():header(MT_GET_WORLDBOSS_GUILD_HURT_INFO_CS) {}
	SCWorldBossSendGuildHurtInfo::SCWorldBossSendGuildHurtInfo():header(MT_SEND_WORLDBOSS_GUILD_HURT_INFO_SC) {}
	CSWorldBossWeekRankInfoReq::CSWorldBossWeekRankInfoReq():header(MT_GET_WORLDBOSS_WEEK_RANK_INFO_CS) {}
	SCWorldBossWeekRankInfo::SCWorldBossWeekRankInfo():header(MT_WORLDBOSS_WEEK_RANK_SC) {}
	SCWorldBossCanRoll::SCWorldBossCanRoll():header(MT_WORLDBOSS_CAN_ROLL_SC) {}
	CSWorldBossRollReq::CSWorldBossRollReq():header(MT_GET_WORLDBOSS_ROLL_REQ_CS) {}
	SCWorldBossRollInfo::SCWorldBossRollInfo():header(MT_WORLDBOSS_ROLL_INFO_SC) {}
	SCWorldBossRollTopPointInfo::SCWorldBossRollTopPointInfo():header(MT_WORLDBOSS_TOP_ROLL_INFO_SC) {}
	SCWorldBossKillerList::SCWorldBossKillerList():header(MT_WORLDBOSS_KILLER_LIST_SC) {}
	SCWorldBossHPInfo::SCWorldBossHPInfo() : header(MT_WORLDBOSS_HP_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCWelfareInfo::SCWelfareInfo() : header(MT_WELFARE_INFO_SC) {}
	CSGetOfflineExp::CSGetOfflineExp() : header(MT_GET_OFFLINE_EXP_CS) {}

	CSGetDailyLoginWelfareInfo::CSGetDailyLoginWelfareInfo() : header(MT_WELFARE_GET_DAILY_INFO_CS) {}
	CSGetDailyFindWelfare::CSGetDailyFindWelfare() : header(MT_WELFARE_GET_DAILY_FIND_CS) {}
	SCDailyFindItemChange::SCDailyFindItemChange() : header(MT_WELFARE_DAILYFIND_CHANGE_SC) {}
	CSWelfareSignInReward::CSWelfareSignInReward() : header(MT_WELFARE_SIGN_IN_REWARD_CS) {}
	CSWelfareOnlineReward::CSWelfareOnlineReward() : header(MT_WELFARE_ONLINE_REWARD_CS) {}
	CSWelfareSignInFindBack::CSWelfareSignInFindBack() : header(MT_WELFARE_SIGN_IN_FIND_BACK_CS) {}
	CSWelfareActivityFind::CSWelfareActivityFind() : header(MT_WELFARE_ACTIVITY_FIND_CS) {}
	CSWelfareAutoActivity::CSWelfareAutoActivity() : header(MT_WELFARE_AUTO_ACTIVITY_CS) {}
	CSWelfareFetchHappytreeReward::CSWelfareFetchHappytreeReward() : header(MT_WELFARE_HAPPYTREE_REWARD_CS) {}
	CSWelfareFetchChongjihaoliReward::CSWelfareFetchChongjihaoliReward() : header(MT_WELFARE_CHONGJIHAOLI_REWARD_CS) {}

	SCHangupSetInfo::SCHangupSetInfo(): header(MT_HANGUP_SET_INFO_SC) {}
	CSSaveRoleHangupSet::CSSaveRoleHangupSet(): header(MT_SAVE_HANGUP_SET_CS) {}
	CSSaveRoleCustomSet::CSSaveRoleCustomSet(): header(MT_SAVE_CUSTOM_SET_CS) {}

	CSLightenWeddingRing::CSLightenWeddingRing(): header(MT_LIGHTEN_WEDDINGRING_CS) {}
	CSUplevelWeddingRing::CSUplevelWeddingRing(): header(MT_UPLEVEL_WEDDINGRING_CS) {}
	CSPerformWeddingRingSkill::CSPerformWeddingRingSkill(): header(MT_PERFORM_WEDDINGRING_SKILL_CS) {}
	SCWeddingRingSkillInfo::SCWeddingRingSkillInfo(): header(MT_WEDDINGRING_SKILL_INFO_SC) {}

	SCMazeMoveResult::SCMazeMoveResult(): header(MT_MAZE_MOVE_RET_SC) {}
	SCMazeInfo::SCMazeInfo(): header(MT_MAZE_INFO_SC) {}
	CSQueryMaze::CSQueryMaze(): header(MT_MAZE_QUERY_CS) {}
	CSMoveMaze::CSMoveMaze(): header(MT_MAZE_MOVE_CS) {}
	CSBuyMaze::CSBuyMaze(): header(MT_MAZE_BUY_CS) {}
	CSFetchMazeJifenReward::CSFetchMazeJifenReward() : header(MT_MAZE_FETCH_JIFEN_REWARD_CS) {}


// 	SCDailyWelfareTips::SCDailyWelfareTips(): header(MT_DAILY_WELFARE_TIPS_SC) {}
// 	CSDailyWelfareTipsReq::CSDailyWelfareTipsReq(): header(MT_DAILY_WELFARE_TIPS_CS) {}

	CSGetTitleList::CSGetTitleList():header(MT_GET_ALL_TITLE_CS){}
	CSUseTitle::CSUseTitle():header(MT_USE_TITLE_CS){}
	CSUpgradeTitle::CSUpgradeTitle():header(MT_TITLE_UPGRADE_CS){}
	SCTitleList::SCTitleList():header(MT_ALL_TITLE_LIST_SC){}
	SCUsedTitleList::SCUsedTitleList():header(MT_USED_TITLE_LIST_SC){}
	SCRoleUsedTitleChange::SCRoleUsedTitleChange():header(MT_USED_TITLE_CHANGE_SC){}

	SCActiveDegreeInfo::SCActiveDegreeInfo():header(MT_ACTIVEDEFREE_INFO_SC) {}
	CSActiveFetchReward::CSActiveFetchReward():header(MT_ACTIVEDEFREE_FETCH_REWARD_CS) {}

	SCDailyBuyCoinInfo::SCDailyBuyCoinInfo():header(MT_DAILY_BUY_COIN_INFO_SC) {}
	CSGetDailyBuyCoin::CSGetDailyBuyCoin():header(MT_DAILY_BUY_COIN_GET_CS){}

	CSEvaluateRole::CSEvaluateRole():header(MT_EVALUATE_ROLE_CS) {}

	SCFunctionSuccNotice::SCFunctionSuccNotice():header(MT_FUNCTION_SUCC_NOTICE_SC) {}
	SCFunctionFailNotice::SCFunctionFailNotice():header(MT_FUNCTION_FAIL_NOTICE_SC) {}

	CSClientSpecialOperate::CSClientSpecialOperate():header(MT_CLIENT_SPECIAL_OPERATE_CS) {}

	SCOperateSucc::SCOperateSucc():header(MT_OPER_SUCC_SC) {}

	CSRoleTaskTransmit::CSRoleTaskTransmit() : header(MT_ROLE_TASK_TRANSMIT_CS) {}
	CSTaskOpenPanel::CSTaskOpenPanel() : header(MT_TASK_OPEN_PANEL_CS) {}

	SCTouZiJiHuaInfo::SCTouZiJiHuaInfo() : header(MT_TOUZIJIHUA_INFO_SC) {}
	CSFetchTouZiJiHuaReward::CSFetchTouZiJiHuaReward() : header(MT_FETCH_TOUZIJIHUA_REWARD_CS) {}
	CSTouzijihuaActive::CSTouzijihuaActive() : header(MT_TOUZIJIHUA_ACTIVE_CS) {}
	CSNewTouzijihuaOperate::CSNewTouzijihuaOperate() : header(MT_NEW_TOUZIJIHUA_OPERATE_CS) {}
	CSTouzijihuaFbBossOperate::CSTouzijihuaFbBossOperate():header(MT_TOUZIJIHUA_FB_BOSS_OPERATE_CS){}
	SCTouzijihuaFbBossInfo::SCTouzijihuaFbBossInfo():header(MT_TOUZIJIHUA_FB_BOSS_INFO_SC){}

	CSYaojiangReq::CSYaojiangReq() : header(MT_YAOJIANG_REQ_CS) {}
	SCYaojiangInfo::SCYaojiangInfo() : header(MT_YAOJIANG_INFO_SC) {}

	CSDatingInviteReq::CSDatingInviteReq() : header(MT_DATING_INVITE_REQ_CS) {}
	CSDatingInviteAck::CSDatingInviteAck() : header(MT_DATING_INVITE_ACK_CS) {}

	SCActivityFindInfo::SCActivityFindInfo() : header(MT_ACTIVITY_FIND_INFO_SC) {}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCDayCounterInfo::SCDayCounterInfo():header(MT_DAYCOUNTER_INFO_SC) {}
	SCDayCounterItemInfo::SCDayCounterItemInfo():header(MT_DAYCOUNTER_ITEM_INFO_SC) {}

	SCSpecialParamChange::SCSpecialParamChange() : header(MT_SPECIAL_PARAM_CHANGE_SC) {}
	SCHunyanStateInfo::SCHunyanStateInfo() : header(MT_MARRY_HUNYAN_STATE_SC) {}
	SCHunYanCurWeddingAllInfo::SCHunYanCurWeddingAllInfo() : header(MT_CUR_WDDING_INFO_SC) {}
	SCQingYuanAllInfo::SCQingYuanAllInfo():header(MT_QINGYUAN_INFO_SC){}
	SCWeddingBlessingRecordInfo::SCWeddingBlessingRecordInfo() : header(MT_HUNYAN_BLESS_RECORD_SC) {}
	SCMarryNotic::SCMarryNotic() : header(MT_MARRY_NOTICE_SC) {}
	CSMarryZhuheSend::CSMarryZhuheSend() : header(MT_MARRY_ZHUHE_SEND_CS) {}
	SCMarryZhuheShou::SCMarryZhuheShou() : header(MT_MARRY_ZHUHE_SHOU_SC) {}
	SCWeddingApplicantInfo::SCWeddingApplicantInfo() : header(MT_HUNYAN_APPLY_LIST_SC) {}
	CSQingYuanShengDiOperaReq::CSQingYuanShengDiOperaReq() : header(MT_QING_YUAN_SHENG_DI_OPERA_REQ_CS) {}
	SCQingYuanShengDiTaskInfo::SCQingYuanShengDiTaskInfo() : header(MT_QING_YUAN_SHENG_DI_TASK_INFO_SC) {}
	SCQingYuanShengDiBossInfo::SCQingYuanShengDiBossInfo() : header(MT_QING_YUAN_SHENG_DI_BOSS_INFO_SC) {}

	SCMarrySpecialEffect::SCMarrySpecialEffect():header(MT_MARRY_SPECIAL_EFFECT_SC) {}
	SCMarryXunyouFirework::SCMarryXunyouFirework():header(MT_MARRY_XUNYOU_FIREWORK_SC){}
	SCQingYuanWeddingAllInfo::SCQingYuanWeddingAllInfo():header(MT_HUNYAN_YUEYUE_INFO_SC){}
	SCMarryHunyanOpera::SCMarryHunyanOpera():header(MT_MARRY_HUNYAN_OPERA_SC){}
	SCMarryReqRoute::SCMarryReqRoute():header(MT_MARRY_REQ_ROUTE_SC){}
	SCDivorceReqRoute::SCDivorceReqRoute():header(MT_DIVORCE_REQ_ROUTE_SC){}
	CSMarryXunyouOpera::CSMarryXunyouOpera():header(MT_MARRY_XUNYOU_OPERA_CS){}
	CSMarryHunyanBless::CSMarryHunyanBless():header(MT_MARRY_HUNYAN_BLESS_CS){}
	CSMarryHunyanGetBlessNews::CSMarryHunyanGetBlessNews():header(MT_MARRY_HUNYAN_GET_BLESS_NEWS_CS){}
	CSMarryHunyanOpera::CSMarryHunyanOpera():header(MT_MARRY_HUNYAN_OPERA_CS){}
	CSQingYuanOperaReq::CSQingYuanOperaReq():header(MT_QINGYUAN_OPERA_CS){}
	CSResetLoverName::CSResetLoverName():header(MT_RESET_LOVER_NAME_CS){}
	CSMarryReq::CSMarryReq():header(MT_MARRY_REQ_CS){}
	CSMarryRet::CSMarryRet():header(MT_MARRY_RET_CS){}
	CSDivorceRet::CSDivorceRet():header(MT_DIVORCE_RET_CS){}
	SCMarryInformation::SCMarryInformation():header(MT_MARRY_INFOMATION_SC){}
	SCIsAcceptMarry::SCIsAcceptMarry():header(MT_IS_ACCEPT_MARRY_SC){}
	SCMarryRetInfo::SCMarryRetInfo() : header(MT_MARRY_RET_INFO_SC) {}

	CSProfessWallReq::CSProfessWallReq() : header(MT_PROFESS_WALL_REQ_CS) {}
	CSProfessToReq::CSProfessToReq() : header(MT_PROFESS_TO_REQ_CS) {}
	SCGlobalProfessWallInfo::SCGlobalProfessWallInfo() : header(MT_GLOBAL_PROFESS_WALL_INFO_SC) {}
	SCPersonProfessWallInfo::SCPersonProfessWallInfo() : header(MT_PERSON_PROFESS_WALL_INFO_SC) {}
	SCProfessWallEffect::SCProfessWallEffect() : header(MT_PROFESS_EFFECT_SC) {}
	SCProfessLevelInfo::SCProfessLevelInfo() : header(MT_PROFESS_LEVEL_INFO_SC) {}

	//////////////////////////////////////////////////////////////////////////
	SCSubLockOperateSucc::SCSubLockOperateSucc():header(MT_SUBLOCK_OPERATE_SUCC_SC) {}
	SCSubLockOperateFail::SCSubLockOperateFail():header(MT_SUBLOCK_OPERATE_FAIL_SC) {}
	SCSubLockInfo::SCSubLockInfo():header(MT_SUBLOCK_INFO_SC) {}

	CSSubLockCreatePw::CSSubLockCreatePw():header(MT_SUBLOCK_CREATE_PW_CS) {}
	CSSubLockDeletePw::CSSubLockDeletePw():header(MT_SUBLOCK_CLEAR_PW_CS) {}
	CSSubLockLock::CSSubLockLock():header(MT_SUBLOCK_LOCK_CS) {}
	CSSubLockUnlock::CSSubLockUnlock():header(MT_SUBLOCK_UNLOCK_CS) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCXianMengZhanUserInfo::SCXianMengZhanUserInfo() : header(MT_XIANMENGZHAN_USER_INFO_SC) {}
	SCXianMengZhanGuildInfo::SCXianMengZhanGuildInfo() : header(MT_XIANMENGZHAN_GUILD_INFO_SC) {}
	SCXianMengZhanDefendAreaInfo::SCXianMengZhanDefendAreaInfo() : header(MT_XIANMENGZHAN_DEFEND_AREA_INFO_SC) {}
	SCXianMengZhanGuildCallNotice::SCXianMengZhanGuildCallNotice() : header(MT_XIANMENGZHAN_GUILD_CALL_NOTICE_SC) {}
	SCXianMengZhanGuildRankInfo::SCXianMengZhanGuildRankInfo() : header(MT_XIANMENGZHEN_GUILD_RANK_INFO_SC) {}
	SCXianMengZhanResult::SCXianMengZhanResult() : header(MT_XIANMENGZHAN_RESULT_SC) {}
	SCXianMengZhanDanyaoBossInfo::SCXianMengZhanDanyaoBossInfo() : header(MT_XIANMENGZHAN_DANYAO_BOSS_INFO_SC) {}
	SCXianMengZhanDefendAreaBeAttackNotice::SCXianMengZhanDefendAreaBeAttackNotice() : header(MT_XIANMENGZHAN_DEFEND_AREA_BE_ATTACK_NOTICE_SC) {}
	CSXianMengZhanGuildFollow::CSXianMengZhanGuildFollow() : header(MT_XIANMENGZHAN_GUILD_FOLLOW_CS) {}
	CSXianMengZhanGuildCall::CSXianMengZhanGuildCall() : header(MT_XIANMENGZHAN_GUILD_CALL_CS) {}
	SCXianmengzhanLianzhanChange::SCXianmengzhanLianzhanChange() : header(MT_XIANMENGZHAN_LIANZHAN_CHANGE_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCXingzuoYijiChangeBoxAndBoss::SCXingzuoYijiChangeBoxAndBoss() : header(MT_XINGZUOYIJI_BOX_AND_BOSS_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCHotspringPlayerInfo::SCHotspringPlayerInfo() : header(MT_HOTSPRINT_PLAYER_INFO_SC) {}
	SCHotspringRankInfo::SCHotspringRankInfo() : header(MT_HOTSPRINT_RANK_INFO_SC) {}
	CSHotspringGivePresent::CSHotspringGivePresent() : header(MT_HOTSPRING_GIVE_PRESEN_CS) {}
	CSHSAddPartnerReq::CSHSAddPartnerReq() : header(MT_HOTSPRING_ADD_PARTNER_CS) {}
	SCHSAddPartnerReqRoute::SCHSAddPartnerReqRoute() : header(MT_HOTSPRING_ADD_PARTNER_ROUT_SC) {}
	CSHSAddPartnerRet::CSHSAddPartnerRet() : header(MT_HOTSPRING_ADD_PARTNER_RET_CS) {}
	CSHSDeleteParter::CSHSDeleteParter() : header(MT_HOTSPRING_DELETE_PARTNER_CS) {}
	CSHSQAFirstPosReq::CSHSQAFirstPosReq() : header(MT_HOTSPRING_QA_FIRST_REQ_CS) {}
	CSHSQAAnswerReq::CSHSQAAnswerReq() : header(MT_HOTSPRING_QA_ANSWER_REQ_CS) {}
	CSHSQAUseCardReq::CSHSQAUseCardReq() : header(MT_HOTSPRING_QA_USE_CAR_REQ_CS) {}
	SCHSSendPartnerInfo::SCHSSendPartnerInfo() : header(MT_HOTSPRING_SEND_PARTNER_INFO_SC) {}
	SCHSAddExpInfo::SCHSAddExpInfo() : header(MT_HOTSPRING_SEND_EXP_INFO_SC) {}
	SCHSShuangxiuInfo::SCHSShuangxiuInfo() : header(MT_HOTSPRING_SEND_SHUANGXIU_INFO_SC) {}
	SCHSQARankInfo::SCHSQARankInfo() : header(MT_HOTSPRING_SEND_QA_RANK_INFO_SC) {}
	SCHSQAQuestionBroadcast::SCHSQAQuestionBroadcast() : header(MT_HOTSPRING_SEND_QA_QUESTION_INFO_SC) {}
	SCHSQASendFirstPos::SCHSQASendFirstPos() : header(MT_HOTSPRING_SEND_QA_FIRST_POST_SC) {}
	SCHSQAnswerResult::SCHSQAnswerResult() : header(MT_HOTSPRING_SEND_QA_RESULT_SC) {}
	CSHSUseSkillReq::CSHSUseSkillReq() : header(MT_HOTSPRING_USE_SKILL_REQ_CS) {}
	SCHSNoticeSkillInfo::SCHSNoticeSkillInfo() : header(MT_HOTSPRING_NOTICE_SKILL_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCCrossGuildBattleSceneInfo::SCCrossGuildBattleSceneInfo() : header(MT_CROSS_GUILDBATTLE_SCENE_INFO_SC) {}
	SCCrossGuildBattleNotifyInfo::SCCrossGuildBattleNotifyInfo() : header(MT_CROSS_GUILDBATTLE_NOTIFY_INFO_SC) {}
	SCCrossGuildBattleInfo::SCCrossGuildBattleInfo() : header(MT_CROSS_GUILDBATTLE_INFO_SC) {}
	CSCrossGuildBattleOperate::CSCrossGuildBattleOperate() : header(MT_CROSS_GUILDBATTLE_OPERA_REQ_CS) {}
	CSCrossGuildBattleGetRankInfoReq::CSCrossGuildBattleGetRankInfoReq() : header(MT_CROSS_GUILDBATTLE_GET_RANK_INFO_REQ_CS) {}
	SCCrossGuildBattleGetRankInfoResp::SCCrossGuildBattleGetRankInfoResp() : header(MT_CROSS_GUILDBATTLE_GET_RANK_INFO_RESP_SC) {}
	SCCrossGuildBattleTaskInfo::SCCrossGuildBattleTaskInfo() : header(MT_CROSS_GUILDBATTLE_TASK_INFO_SC) {}
	SCCrossGuildBattleBossInfo::SCCrossGuildBattleBossInfo() : header(MT_CROSS_GUILDBATTLE_BOSS_INFO_SC) {}
	SCCrossGuildBattleFlagInfo::SCCrossGuildBattleFlagInfo() : header(MT_CROSS_GUILDBATTLE_FLAG_INFO_SC) {}
	SCCrossGuildBattleGetMonsterInfoResp::SCCrossGuildBattleGetMonsterInfoResp() : header(MT_CROSS_GUILDBATTLE_MONSTER_INFO_SC) {}
	SCCrossGuildBattleBossHurtInfo::SCCrossGuildBattleBossHurtInfo(): header(MT_CROSS_GUILDBATTLE_BOSS_HURT_INFO_SC){}
	SCCrossGuildBattleSceneGuilderNum::SCCrossGuildBattleSceneGuilderNum():header(MT_CROSS_GUILDBATTLE_SCENE_GUILDER_NUM_SC){}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCQuestionBroadcast::SCQuestionBroadcast() : header(MT_QUESTION_RIGHT_BROCAST_SC) {}
	CSQuestionAnswerReq::CSQuestionAnswerReq() : header(MT_QUESTION_ANSWER_REQ_CS) {}
	SCQuestionAnswerResult::SCQuestionAnswerResult() : header(MT_QUESTION_ANSWER_RESULT_SC) {}
	SCQuestionGuildRankInfo::SCQuestionGuildRankInfo() : header(MT_QUESTION_ANSWER_GUILD_RANK_SC) {}
	SCQuestionRightAnswerNum::SCQuestionRightAnswerNum() : header(MT_QUESTION_ANSWER_RIGHT_NUM_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSBattleFieldHonorInfoReq::CSBattleFieldHonorInfoReq() : header(MT_GET_BATTLEFIELD_HONOR_INFO_REQ_CS) {}

	//////////////////////////////////////////////////////////////////////////
	SCActivityStatus::SCActivityStatus():header(MT_ACTIVITY_STATUS_SC) {}
	SCActivityRoomStatusAck::SCActivityRoomStatusAck():header(MT_ACTIVITY_ROOM_INFO_ACK_SC) {}
	CSActivityRoomStatusReq::CSActivityRoomStatusReq():header(MT_GET_ACTIVITY_ROME_STATUS_CS) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCChongZhiInfo::SCChongZhiInfo() : header(MT_CHONG_ZHI_INFO_SC) {}
	CSChongzhiFetchReward::CSChongzhiFetchReward() : header(MT_CHONGZHI_FETCH_REWARD_CS) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSTotalChongzhiWantMoneyFetchInfo::CSTotalChongzhiWantMoneyFetchInfo() : header(MT_TOTAL_CHONGZHI_WANT_MONEY_FETCH_MSG_CS) {}
	CSTotalChongzhiWantMoneyFetchReward::CSTotalChongzhiWantMoneyFetchReward() : header(MT_TOTAL_CHONGZHI_WANT_MONEY_FETCH_REWARD_CS) {}

	SCTotalChongzhiWantMoneyFetchInfo::SCTotalChongzhiWantMoneyFetchInfo() : header(MT_TOTAL_CHONGZHI_WANT_MONEY_INFO_SC) {}
	SCTotalChongzhiWantMoneyFetchReward::SCTotalChongzhiWantMoneyFetchReward() : header(MT_TOTAL_CHONGZHI_WANT_MONEY_FETCH_REWARD_SC) {}

	SCCollectExchangeInfo::SCCollectExchangeInfo() : header(MT_RA_COLLECT_EXCHANGE_INFO_SC) {}

	SCRACornucopiaFetchInfo::SCRACornucopiaFetchInfo() : header(MT_RA_CORNUCOPIA_FETCH_INFO_SC) {}
	SCRACornucopiaFetchReward::SCRACornucopiaFetchReward() : header(MT_RA_CORNUCOPIA_FETCH_REWARD_SC) {}
	SCRANewCornucopiaInfo::SCRANewCornucopiaInfo() : header(MT_RA_NEW_CORNUCOPIA_INFO_SC) {}

	//////////////////////////////////////////////////////////////////////////
	CSBuyXunyouOpera::CSBuyXunyouOpera() : header(MT_HUNYAN_BUY_XUNYOU_OPERA_CS) {}
	SCXunyouOperaInfo::SCXunyouOperaInfo() : header(MT_HUNYAN_XUNYOU_OPERA_INFO_SC) {}
	CSSupplyBuyItem::CSSupplyBuyItem():header(MT_SUPPLY_BUY_IYEM_CS) {}
	CSSupplySetRecoverRangePer::CSSupplySetRecoverRangePer():header(MT_SUPPLY_SET_RECOVER_RANGE_PER_CS) {}
	SCSupplyInfo::SCSupplyInfo():header(MT_SUPPLY_INFO_SC) {}
	SCTotalLoginDays::SCTotalLoginDays():header(MT_TOTAL_LOGIN_DAYS_SC) {}

	CSFetchSevenDayLoginReward::CSFetchSevenDayLoginReward() : header(MT_WELFARE_FETCH_SEVEN_DAY_LOGIN_REWARD_CS){}
	SCSevenDayLoginRewardInfo::SCSevenDayLoginRewardInfo() : header(MT_SEVEN_DAY_LOGIN_REWARD_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSGuildInfoStatisticReq::CSGuildInfoStatisticReq():header(MT_GUILD_INFO_STATISTIC_REQ_CS){}
	SCGuildInfoStatistic::SCGuildInfoStatistic():header(MT_GUILD_INFO_STATISTIC_SC){}
	SCGuildMvpInfo::SCGuildMvpInfo():header(MT_GUILD_MVP_INFO_SC){}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCGCZGlobalInfo::SCGCZGlobalInfo() : header(MT_GCZ_GLOBAL_INFO_SC) {}
	SCGCZRoleInfo::SCGCZRoleInfo() : header(MT_GCZ_ROLE_INFO_SC) {}
	SCGongChengZhanOwnerInfo::SCGongChengZhanOwnerInfo() : header(MT_GONGCHENGZHAN_OWNER_INFO_SC) {}
	SCGCZRewardInfo::SCGCZRewardInfo() : header(MT_GCZ_REWARD_INFO_SC) {}
	SCGongChengZhanFlagInfo::SCGongChengZhanFlagInfo() : header(MT_GCZ_FLAG_INFO_SC) {}
	CSGCZChangePlace::CSGCZChangePlace() : header(MT_GCZ_CHANGE_PLACE_CS) {}
	CSGCZFetchDailyRewardReq::CSGCZFetchDailyRewardReq() : header(MT_GCZ_FETCH_DAILY_REWARD_CS) {}
	SCGczLuckyRewardInfo::SCGczLuckyRewardInfo() : header(MT_GCZ_LUCK_REWARD_SC) {}

	SCGCZWorshipInfo::SCGCZWorshipInfo() : header(MT_GCZ_WORSHIP_INFO_SC) {}
	SCGCZWorshipActivityInfo::SCGCZWorshipActivityInfo() : header(MT_GCZ_WORSHIP_ACTIVITY_INFO_SC) {}
	CSGCZWorshipReq::CSGCZWorshipReq() : header(MT_GCZ_WORSHIP_CS) {}
	
	SCRoleWorshipInfo::SCRoleWorshipInfo() : header(MT_WORSHIP_COMMON_INFO_SC) {}
	SCRoleWorshipStart::SCRoleWorshipStart() : header(MT_WORSHIP_COMMON_START_SC) {}
	CSRoleWorshipReq::CSRoleWorshipReq() : header(MT_WORSHIP_COMMON_CS) {}

	SCTwLuckyRewardInfo::SCTwLuckyRewardInfo() : header(MT_TW_LUCK_REWARD_SC) {}
	SCGBLuckyRewardInfo::SCGBLuckyRewardInfo() : header(MT_GB_LUCK_REWARD_SC) {}
	SCGBGlobalInfoNew::SCGBGlobalInfoNew() : header(MT_GB_GLOBAL_INFO_NEW_SC) {}
	SCGBRoleInfoNew::SCGBRoleInfoNew() : header(MT_GB_ROLE_INFO_NEW_SC) {}
	SCQunxianluandouLuckyRewardInfo::SCQunxianluandouLuckyRewardInfo() : header(MT_QUNXIANLUANDOU_LUCK_REWARD_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSGBAddHuDun::CSGBAddHuDun() : header(MT_GB_ROLE_ADD_HUDUN_CS) {}
	SCGBGlobalInfo::SCGBGlobalInfo() : header(MT_GB_GLOBAL_INFO_SC) {}
	SCGBRoleInfo::SCGBRoleInfo() : header(MT_GB_ROLE_INFO_SC) {}
	CSGBRoleCalcSubmitReq::CSGBRoleCalcSubmitReq() : header(MT_GB_ROLE_SUBMIT_GOODS_CS) {}
	SCGBBianShenView::SCGBBianShenView() : header(MT_GB_ROLE_BIANSHEN_IMAGE_SC) {}
	CSGBWinnerInfoReq::CSGBWinnerInfoReq() : header(MT_GB_WINNER_INFO_CS) {}
	SCGBSendWinnerInfo::SCGBSendWinnerInfo() : header(MT_GB_WINER_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCTerritoryWarRoleInfo::SCTerritoryWarRoleInfo() : header(MT_TW_ROLE_INFO_SC) {}
	SCTerritoryWarApperance::SCTerritoryWarApperance() : header(MT_TW_ROLE_IMAGE_INFO_SC) {}
	SCTerritoryWarGlobeInfo::SCTerritoryWarGlobeInfo() : header(MT_TW_GLOBE_INFO_SC) {}
	CSTerritoryWarReliveShopBuy::CSTerritoryWarReliveShopBuy() : header(MT_TW_RELIVE_SHOP_BUY_CS) {}
	CSTerritoryWarReliveFightBuy::CSTerritoryWarReliveFightBuy() : header(MT_TW_FIGHT_SHOP_BUY_CS) {}
	CSTerritoryWarQualification::CSTerritoryWarQualification() : header(MT_TW_QUALIFICATION_CS) {}
	SCTerritoryWarQualification::SCTerritoryWarQualification() : header(MT_TW_QUALIFICATION_SC) {}
	CSTerritorySetLandMine::CSTerritorySetLandMine() : header(MT_TW_SET_LANDMINE_REQ_CS) {}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSNationalBossBuyBuffReq::CSNationalBossBuyBuffReq() : header(MT_NATIONAL_BOSS_BUY_BUFF_CS) {}
	SCNationalBossRankInfo::SCNationalBossRankInfo() : header(MT_NATIONAL_BOSS_RANK_INFO_SC) {}
	SCNationalBossUserInfo::SCNationalBossUserInfo() : header(MT_NATIONAL_BOSS_USER_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSChallengeFieldGetUserInfo::CSChallengeFieldGetUserInfo() : header(MT_CHALLENGE_FIELD_GET_USER_INFO_CS) {}
	CSChallengeFieldResetOpponentList::CSChallengeFieldResetOpponentList() : header(MT_CHALLENGE_FIELD_RESET_OPPONENT_REQ_CS) {}
	CSChallengeFieldFightReq::CSChallengeFieldFightReq() : header(MT_CHALLENGE_FIELD_FIGHT_REQ_CS) {}
	CSChallengeFieldBuyJoinTimes::CSChallengeFieldBuyJoinTimes() : header(MT_CHALLENGE_FIELD_BUY_JOIN_TIMES_CS) {}
	CSChallengeFieldGetOpponentInfo::CSChallengeFieldGetOpponentInfo() : header(MT_CHALLENGE_FIELD_GET_OPPONENT_INFO_CS) {}
	CSChallengeFieldFetchJifenRewardReq::CSChallengeFieldFetchJifenRewardReq() : header(MT_CHALLENGE_FIELD_FETCH_JIFEN_REWARD_REQ_CS) {}
	CSChallengeFieldGetRankInfo::CSChallengeFieldGetRankInfo() : header(MT_CHALLENGE_FIELD_GET_RANK_INFO_CS) {}
	CSChallengeFieldGetLuckyInfo::CSChallengeFieldGetLuckyInfo() : header(MT_CHALLENGE_FIELD_GET_LUCKY_INFO_CS) {}
	CSChallengeFieldFetchGuangHui::CSChallengeFieldFetchGuangHui() : header(MT_CHALLENGE_FIELD_FETCH_GUANGHUI_CS) {}
	CSChallengeFieldBuyBuff::CSChallengeFieldBuyBuff() : header(MT_CHALLENGE_FIELD_BUY_BUFF_CS) {}
	SCChallengeFieldStatus::SCChallengeFieldStatus() : header(MT_CHALLENGE_FIELD_STATUS_INFO_SC) {}
	SCChallengeFieldUserInfo::SCChallengeFieldUserInfo() : header(MT_CHALLENGE_FIELD_USER_INFO_SC) {}
	SCChallengeFieldOpponentRankPosChange::SCChallengeFieldOpponentRankPosChange() : header(MT_CHALLENGE_FIELD_OPPONENT_RANK_POS_CHANGE) {}
	SCChallengeFieldReportInfo::SCChallengeFieldReportInfo() : header(MT_CHALLENGE_FIELD_REPORT_INFO_SC) {}
	SCChallengeFieldRankInfo::SCChallengeFieldRankInfo() : header(MT_CHALLENGE_FIELD_RANK_INFO_SC) {}
	SCChallengeFieldLuckyPosInfo::SCChallengeFieldLuckyPosInfo() : header(MT_CHALLENGE_FIELD_LUCKY_POS_INFO_SC) {}
	SCChallengeFieldOpponentInfo::SCChallengeFieldOpponentInfo() : header(MT_CHALLENGE_FIELD_OPPONENT_INFO_SC) {}
	SCChallengeFieldBeDefeatNotice::SCChallengeFieldBeDefeatNotice() : header(MT_CHALLENGE_BE_DEFEAT_NOTICE_SC) {}
	SCChallengeFieldRewardGoldNotice::SCChallengeFieldRewardGoldNotice() : header(MT_CHALLENGE_FIELD_REWARD_BIND_GOLD_INFO_SC) {}
	SCChallengeFieldWin::SCChallengeFieldWin() : header(MT_CHALLENGE_FIELD_WIN) {}

	CSChallengeFieldBestRankBreakReq::CSChallengeFieldBestRankBreakReq() : header(MT_CHALLENGE_FIELD_BEST_RANK_BREAK_REQ_CS) {}
	SCChallengeFieldBestRankBreakInfo::SCChallengeFieldBestRankBreakInfo() : header(MT_CHALLENGE_FIELD_BEST_RANK_BREAK_INFO_SC) {}
	CSChallengeFieldReadyStartFightReq::CSChallengeFieldReadyStartFightReq() : header(MT_CHALLENGE_FIELD_READY_START_FIGHT_CS) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSCloseBetaActivityOperaReq::CSCloseBetaActivityOperaReq() : header(MT_CLOSE_BETA_ACTIVITY_OPERA_REQ_CS) {}
	SCCloseBetaActivityInfo::SCCloseBetaActivityInfo() : header(MT_CLOSE_BETA_ACTIVITY_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSLuckyRollActivityOperaReq::CSLuckyRollActivityOperaReq() : header(MT_LUCKY_ROLL_ACTIVITY_OPERA_REQ_CS) {}
	SCLuckyRollActivityInfo::SCLuckyRollActivityInfo() : header(MT_LUCKY_ROLL_ACTIVITY_INFO_SC) {}
	SCLuckyRollActivityRollResult::SCLuckyRollActivityRollResult() : header(MT_LUCKY_ROLL_ACTIVITY_ROLL_RESULT_SC) {}
	CSRandActivityOperaReq::CSRandActivityOperaReq():header(MT_RAND_ACTIVITY_OPERA_REQ_CS) {}
	SCRAServerPanicBuyInfo::SCRAServerPanicBuyInfo() : header(MT_RA_SERVER_PANIC_BUY_INFO_SC) {}
	SCRAPersonalPanicBuyInfo::SCRAPersonalPanicBuyInfo() : header(MT_RA_PERSONAL_PANIC_BUY_INFO_SC) {}
	SCRAChestshopInfo::SCRAChestshopInfo() : header(MT_RA_CHESTSHOP_INFO_SC) {}
	SCRAStoneUplevelInfo::SCRAStoneUplevelInfo() : header(MT_RA_STONE_UPLEVEL_INFO_SC) {}
	SCRAXiannvChanmianUplevelInfo::SCRAXiannvChanmianUplevelInfo() : header(MT_RA_XN_CHANMIAN_UPLEVEL_INFO_SC) {}
	SCRAMountUpgradeInfo::SCRAMountUpgradeInfo() : header(MT_RA_MOUNT_UPGRADE_INFO_SC) {}
	SCRAQibingUpgradeInfo::SCRAQibingUpgradeInfo() : header(MT_RA_QIBING_UPGRADE_INFO_SC) {}
	SCRAMentalityUplevelInfo::SCRAMentalityUplevelInfo() : header(MT_RA_MENTALITY_TOTAL_LEVEL_INFO_SC) {}
	SCRAWingUpgradeInfo::SCRAWingUpgradeInfo() : header(MT_RA_WING_UPGRADE_INFO_SC) {}
	SCRAQuanminQifuInfo::SCRAQuanminQifuInfo() : header(MT_RA_QUANMIN_QIFU_INFO_SC) {}
	SCRALoginGiftInfo::SCRALoginGiftInfo() : header(MT_RA_LOGIN_GIFT_INFO_SC) {}
	SCRALoginGiftInfo_0::SCRALoginGiftInfo_0() : header(MT_RA_LOGIN_GIFT_INFO_0_SC) {}
	/////////////////////////////////////////////////  每日好礼  //////////////////////////////////////////////////	
	SCRAEverydayNiceGiftInfo::SCRAEverydayNiceGiftInfo() : header(MT_RA_EVERYDAY_NICE_GIFT_INFO_SC) {}
	SCRAShouYouYuXiangInfo::SCRAShouYouYuXiangInfo() : header(MT_RA_SHOUYOU_YUXIANGNG_INFO_SC) {}
	SCRAXianMengBiPinInfo::SCRAXianMengBiPinInfo() : header(MT_RAND_ACTIVITY_XIANMENG_BIPIN_SC) {}
	SCRAXianMengJueQiInfo::SCRAXianMengJueQiInfo() : header(MT_RAND_ACTIVITY_XIANMENG_JUEQI_SC) {}
	SCRABipinCapabilityInfo::SCRABipinCapabilityInfo() : header(MT_RA_BIPIN_CAPABILITY_INFO_SC) {}
	SCRADanbiChongzhiInfo::SCRADanbiChongzhiInfo() : header(MT_RA_DANBICHONGZHI_INFO_SC) {}
	SCRATimeLimitExchangeEquiInfo::SCRATimeLimitExchangeEquiInfo() : header(MT_RA_TIME_LIMIT_EXCHANGE_EQUI_INFO_SC) {}
	SCRATimeLimitExchangeJLInfo::SCRATimeLimitExchangeJLInfo() : header(MT_RA_TIME_LIMIT_EXCHANGE_JL_INFO_SC) {}
	SCRALevelLotteryInfo::SCRALevelLotteryInfo() : header(MT_RA_LEVEL_LOTTERY_INFO_SC) {}
	SCRALevelLotteryRewardList::SCRALevelLotteryRewardList() : header(MT_RA_LEVEL_LOTTERY_REWARD_LIST_SC) {}
	SCRALevelLotteryActivityInfo::SCRALevelLotteryActivityInfo() : header(MT_RA_LEVEL_LOTTERY_ACTIVITY_INFO_SC) {}
	SCRANiuEggInfo::SCRANiuEggInfo() : header(MT_RA_CHARGE_NIU_EGG_INFO_SC) {}
	SCRANiuEggChouResultInfo::SCRANiuEggChouResultInfo() : header(MT_RA_CHARGE_NIU_EGG_REWARD_SC) {}
	SCRAZhenbaogeInfo::SCRAZhenbaogeInfo() : header(MT_RA_ZHENBAOGE_INFO_SC) {}

	//////////////////////////////////////////////////////////////////////////
	SCYiZhanDaoDiRankInfo::SCYiZhanDaoDiRankInfo() : header(MT_YIZHANDAODI_RANKINIO_SC) {}
	SCYiZhanDaoDiKickout::SCYiZhanDaoDiKickout() : header(MT_YIZHANDAODI_KICKOUT_SC) {}
	SCYiZhanDaoDiLuckyInfo::SCYiZhanDaoDiLuckyInfo() : header(MT_YIZHANDAODI_LUCKYINFO_SC) {}
	SCYiZhanDaoDiTitleChange::SCYiZhanDaoDiTitleChange() : header(MT_YIZHANDAODI_TITLE_CHANGE_SC) {}
	SCYiZhanDaoDiUserInfo::SCYiZhanDaoDiUserInfo() : header(MT_YIZHANDAODI_USER_INFO_SC) {}
	CSYiZhanDaoDiGuwu::CSYiZhanDaoDiGuwu() : header(MT_YIZHANDAODI_GUWU_CS) {}
	SCYiZhanDaoDiLastFirstInfo::SCYiZhanDaoDiLastFirstInfo() : header(MT_YIZHANDAODI_LAST_FIRST_USER_INFO_SC) {}

	CSWuXingGuaJiGetReward::CSWuXingGuaJiGetReward() : header(MT_WUXINGGUAJI_GET_REWARD_CS) {}
	SCWuXingGuaJiInfo::SCWuXingGuaJiInfo() : header(MT_WUXINGGUAJI_INFO_SC) {}
	SCWuXingGuaJiLuckyInfo::SCWuXingGuaJiLuckyInfo() : header(MT_WUXINGGUAJI_LUCKY_INFO_SC) {}
	SCGuajiBossBornNotice::SCGuajiBossBornNotice() : header(MT_GUAJI_BOSS_BORN_NOTICE_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	SCChargeRewardInfo::SCChargeRewardInfo() : header(MT_RA_CHARGE_REPAYMENT_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////
	SCCSASubActivityState::SCCSASubActivityState() : header(MT_CSA_SUB_ACTIVITY_STATE_SC) {}
	SCCSAActivityInfo::SCCSAActivityInfo() : header(MT_CSA_ACTIVITY_INFO_SC) {}
	CSCSAQueryActivityInfo::CSCSAQueryActivityInfo() : header(MT_CSA_QUERY_ACTIVITY_INFO_CS) {}
	CSCSARoleOperaReq::CSCSARoleOperaReq() : header(MT_CSA_ROLE_OPERA_REQ_CS) {}
	SCCSARoleInfo::SCCSARoleInfo() : header(MT_CSA_ROLE_INFO_SC) {}
	SCCSARollResult::SCCSARollResult() : header(MT_CSA_ROLL_RESULT_SC) {}
	SCCSABossInfo::SCCSABossInfo() : header(MT_CSA_BOSS_INFO_SC) {}
	SCCSABossRankInfo::SCCSABossRankInfo() : header(MT_CSA_BOSS_RANK_SC) {}
	SCCSABossRoleInfo::SCCSABossRoleInfo() : header(MT_CSA_BOSS_ROLE_INFO_SC) {}
	SCRALottery1Info::SCRALottery1Info() : header(MT_RA_LOTTERY_1_INFO_SC) {}
	SCRATotalChargeDayInfo::SCRATotalChargeDayInfo() : header(MT_RA_TOTAL_CHARGE_DAY_INFO_SC) {}
	SCRATomorrowRewardInfo::SCRATomorrowRewardInfo() : header(MT_RA_TOMORROW_REWARD_INFO_SC) {}
	SCRADayChongzhiRankInfo::SCRADayChongzhiRankInfo() : header(MT_RA_DAY_CHONGZHI_RANK_INFO_SC) {}
	SCRADayConsumeRankInfo::SCRADayConsumeRankInfo() : header(MT_RA_DAY_CONSUME_RANK_INFO_SC) {}
	SCRATotalChargeInfo::SCRATotalChargeInfo() : header(MT_RA_TOTAL_CHARGE_INFO_SC) {}
	SCRANewTotalChargeInfo::SCRANewTotalChargeInfo() : header(MT_RA_NEW_TOTAL_CHARGE_INFO_SC) {}
	SCRAZeroBuyReturnInfo::SCRAZeroBuyReturnInfo() : header(MT_RA_ZERO_BUY_RETURN_SC){}
	SCRATotalChargeMultiInfo::SCRATotalChargeMultiInfo():header(MT_RA_TOTAL_CHARGE_MULTI_INFO_SC){}
	SCRADoubleGetInfo::SCRADoubleGetInfo():header(MT_RA_DOUBLE_GET_INFO_SC){}
	SCSingleChargeInfoMulti::SCSingleChargeInfoMulti():hearder(MT_RA_SINGLE_CHARGE_INFO_MULTI_SC){}

	///////////////////////////////////////////////// 狂嗨庆典 //////////////////////////////////////////////
	SCRAKuangHaiInfo::SCRAKuangHaiInfo() : header(MT_RA_KUANG_HAI_QING_DIAN_INFO_SC) {}

	///////////////////////////////////////////////// 礼物收割 //////////////////////////////////////////////
	SCRAGiftHarvestPlayerInfo::SCRAGiftHarvestPlayerInfo() : header(MT_RA_GIFT_HARVEST_PLAYER_INFO_SC) {}
	SCRAGiftHarvestRankInfo::SCRAGiftHarvestRankInfo() : header(MT_RA_GIFT_HARVEST_RANK_INFO_SC) {}
	SCGiftHarvestSkill::SCGiftHarvestSkill() : header(MT_RA_GIFT_HARVEST_SKILL_INFO_SC) {}
	SCGiftHarvestRoundNotice::SCGiftHarvestRoundNotice() : header(MT_RA_GIFT_HARVEST_ROUND_INFO_SC) {}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCMonsterInvadeTimeNotice::SCMonsterInvadeTimeNotice() : header(MT_ACTIVITY_MONSTER_INVADE_TIME_NOTICE_SC) {}
	SCMillionaireTimeNotice::SCMillionaireTimeNotice() : header(MT_ACTIVIT_MILLIONAIRE_TIME_NOTICE_SC) {}
	CSMillionaireUseFrozenSkill::CSMillionaireUseFrozenSkill() : header(MT_ACTIVIT_MILLIONAIRE_USE_FROZEN_SKILL) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRADayChongZhiFanLiInfo::SCRADayChongZhiFanLiInfo() : header(MT_RA_DAY_CHONGZHI_FANLI_INFO_SC) {}
	SCRADayConsumeGoldInfo::SCRADayConsumeGoldInfo() : header(MT_RA_DAY_CONSUME_GOLD_INFO_SC) {}
	SCRATotalConsumeGoldInfo::SCRATotalConsumeGoldInfo() : header(MT_RA_TOTAL_CONSUME_GOLD_INFO_SC) {}
	SCRADayActiveDegreeInfo::SCRADayActiveDegreeInfo() : header(MT_RA_DAY_ACTIVE_DEGREE_INFO_SC) {}
	SCRAKillBossInfo::SCRAKillBossInfo() :header(MT_RA_KILL_BOSS_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSUpdateNoticeFetchReward::CSUpdateNoticeFetchReward() : header(MT_UPDATE_NOTICE_FETCH_REWARD_CS) {}
	SCUpdateNoticeInfo::SCUpdateNoticeInfo() : header(MT_UPDATE_NOTICE_INFO_SC) {}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAChongzhiRankInfo::SCRAChongzhiRankInfo() : header(MT_RA_CHONGZHI_RANK_INFO_SC) {}
	SCRAConsumeGoldRankInfo::SCRAConsumeGoldRankInfo() : header(MT_RA_CONSUME_RANK_INFO_SC) {}
	SCRAConsumeGoldFanliInfo::SCRAConsumeGoldFanliInfo() : header(MT_RA_CONSUME_GOLD_FANLI_INFO_SC) {}

	SCRAProfessRankInfo::SCRAProfessRankInfo() : header(MT_RA_PROFESS_RANK_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSCommonActivityFetchReward::CSCommonActivityFetchReward() : header(MT_COMMON_ACTIVITY_FETCH_REWARD_CS) {}
	SCCommonActivityOpenUnitInfo::SCCommonActivityOpenUnitInfo() : header(MT_COMMON_ACTIVITY_OPEN_UNIT_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSOpenGameActivityInfoReq::CSOpenGameActivityInfoReq() : header(MT_OPEN_GAME_ACTIVITY_REQ_INFO_CS) {}
	CSOpenGameActivityFetchReward::CSOpenGameActivityFetchReward() : header(MT_OPEN_GAME_ACTIVITY_FETCH_REWARD_CS) {}
	SCOpenGameActivityInfo::SCOpenGameActivityInfo() : header(MT_OPEN_GAME_ACTIVITY_INFO_SC) {}
	SCTitleOwnerInfo::SCTitleOwnerInfo() : header(MT_TITLE_OWNER_INFO_SC) {}
	CSTitleOwnerInfoReq::CSTitleOwnerInfoReq() : header(MT_TITLE_OWNER_INFO_REQ_CS) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCQunXianLuanDouUserInfo::SCQunXianLuanDouUserInfo() : header(MT_QUNXIANLUANDOU_USER_INFO_SC) {}
	SCQunXianLuanDouRankInfo::SCQunXianLuanDouRankInfo() : header(MT_QUNXIANLUANDOU_RANK_INFO_SC) {}
	SCQunXianLuanDouSideInfo::SCQunXianLuanDouSideInfo() : header(MT_QUNXIANLUANDOU_SIDE_INFO_SC) {}
	SCQunXianLuanDouShenShiStatus::SCQunXianLuanDouShenShiStatus() : header(MT_QUNXIANLUANDOU_SHENSHI_STATUS_SC) {}
	SCBattleFieldHonorChange::SCBattleFieldHonorChange() : header(MT_BATTLEFIELD_HONOR_CHANGE_SC) {}
	CSQunXianLuanDouPickUpShenShi::CSQunXianLuanDouPickUpShenShi() : header(MT_QUNXIANLUANDOU_PICKUP_SHENSHI_CS) {}
	SCQunxianluandouLianzhanChange::SCQunxianluandouLianzhanChange() : header(MT_QUNXIANLUANDOU_LIANZHAN_CHANGE_SC) {}
	CSQunxianLuandouFirstRankReq::CSQunxianLuandouFirstRankReq() : header(MT_QUNXIANLUANDOU_RANK_FIRST_REQ_CS) {}
	SCQunxianLuandouFirstRankInfo::SCQunxianLuandouFirstRankInfo() : header(MT_QUNXIANLUANDOU_RANK_FIRST_INFO_SC) {}

	CSFetchGuildBattleDailyReward::CSFetchGuildBattleDailyReward() : header(MT_GUILD_BATTLE_DAILY_REWARD_FETCH_CS) {}
	SCSendGuildBattleDailyRewardFlag::SCSendGuildBattleDailyRewardFlag() : header(MT_GUILD_BATTLE_DAILY_REWARD_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSActivityEnterReq::CSActivityEnterReq():header(MT_ACTIVITY_ENTER_REQ_CS) {}
	CSSendGuildSosReq::CSSendGuildSosReq() : header(MT_SEND_GUILD_SOS_REQ) {}
	CSReplyGuildSosReq::CSReplyGuildSosReq() : header(MT_REPLY_GUILD_SOS_REQ) {}
	CSZhuXieFetchTaskReward::CSZhuXieFetchTaskReward():header(MT_ZHUXIE_FETCH_TASK_REWARD_CS) {}
	SCZhuXieUserInfo::SCZhuXieUserInfo():header(MT_ZHUXIE_USER_INFO_SC) {}
	SCActivitySkillUseInfo::SCActivitySkillUseInfo():header(MT_ACTIVITY_SKILL_USE_INFO_SC) {}
	SCTianJiangCaiBaoUserInfo::SCTianJiangCaiBaoUserInfo():header(MT_TIANJIANGCAIBAO_USER_INFO_SC) {}
	SCWangLingExploreUserInfo::SCWangLingExploreUserInfo():header(MT_WANGLINGEXPLORE_USER_INFO_SC) {}
	WangLingExploreBossInfo::WangLingExploreBossInfo():header(MT_WANGLINGEXPLORE_BOSS_INFO_SC){ }
	CSWangLingExploreBuyBuff::CSWangLingExploreBuyBuff():header(MT_WANGLINGEXPLORE_BUY_BUFF_CS){}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCCrossBianJingZhiDiUserInfo::SCCrossBianJingZhiDiUserInfo() : header(MT_CROSS_BIANJINGZHIDI_USER_INFO_SC) {}
	SCCrossBianJingZhiDiBossInfo::SCCrossBianJingZhiDiBossInfo() : header(MT_CROSS_BIANJINGZHIDI_BOSS_INFO_SC) {}
	SCCrossBianJingZhiDiBossHurtInfo::SCCrossBianJingZhiDiBossHurtInfo() : header(MT_CROSS_BIANJINGZHIDI_BOSS_HURT_INFO_SC) {}
	CSCrossBianJingZhiDiBuyBuff::CSCrossBianJingZhiDiBuyBuff() : header(MT_CROSS_BIANJINGZHIDI_BUY_BUFF_CS) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCFunOpenMountInfo::SCFunOpenMountInfo():header(MT_FUN_OPEN_MOUNT_INFO_SC) {}
	SCFunOpenWingInfo::SCFunOpenWingInfo():header(MT_FUN_OPEN_WING_INFO_SC) {}
	SCFunOpenXiannvInfo::SCFunOpenXiannvInfo():header(MT_FUN_OPEN_XIANNV_INFO_SC) {}
	CSFunOpenStoryStep::CSFunOpenStoryStep():header(MT_FUN_OPEN_STORY_STEP_CS) {}
	CSFunOpenSetObjToPos::CSFunOpenSetObjToPos():header(MT_FUN_OPEN_SET_OBJ_POS_CS) {}
	SCFunOpenStoryStepEnd::SCFunOpenStoryStepEnd():header(MT_FUN_OPEN_CUR_STEP_END) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CSFbGuideFinish::CSFbGuideFinish():header(MT_FB_GUIDE_FINISH) {}
	CSFbGuideCreateGather::CSFbGuideCreateGather():header(MT_FB_GUIDE_CREATE_GATHER) {}
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCMoJieFBAllInfo::SCMoJieFBAllInfo():header(MT_MO_JIE_ALL_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCEquipFBResult::SCEquipFBResult():header(MT_FB_EQUIP_RESULT_SC) {}
	CSEquipFBGetInfo::CSEquipFBGetInfo():header(MT_FB_EQUIP_GETINFO_CS) {}
	SCEquipFBTotalPassExp::SCEquipFBTotalPassExp() : header(MT_FB_EQUIP_EXP_SC) {}
	SCEquipFBInfo::SCEquipFBInfo() : header(MT_FB_EQUIP_INFO_SC) {}

	SCChallengeLayerInfo::SCChallengeLayerInfo() : header(MT_FB_CHALLENGE_LAYER_INFO_SC) {}
	SCChallengePassLevel::SCChallengePassLevel() : header(MT_FB_CHALLENGE_PASS_LEVEL_SC) {}
	SCChallengeFBInfo::SCChallengeFBInfo() : header(MT_FB_CHALLENGE_INFO_SC) {}
	CSChallengeFBOP::CSChallengeFBOP() : header(MT_FB_CHALLENGE_FB_OPERATE_CS) {}

	CSStoryFBGetInfo::CSStoryFBGetInfo() : header(MT_FB_STORY_REQ_INFO_CS) {}
	SCStoryFBInfo::SCStoryFBInfo() : header(MT_FB_STORY_INFO_SC) {}
	SCFbPickItemInfo::SCFbPickItemInfo() : header(MT_FB_PICK_ITEM_INFO_SC) {}
	CSFbGuwu::CSFbGuwu() : header(MT_FB_SCENE_GUWU_CS) {}
	SCFBSceneLogicInfo::SCFBSceneLogicInfo() : header(MT_FB_SCENE_LGIC_INFO_SC) {}

	SCExpFbInfo::SCExpFbInfo() : header(MT_EXP_FB_INFO) {}
	CSExpFbPayBuff::CSExpFbPayBuff() : header(MT_EXP_FB_PAY_BUFF) {}

	CSDailyFBGetRoleInfo::CSDailyFBGetRoleInfo() : header(MT_FB_DAILY_REQ_ROLE_INFO_CS) {}
	SCDailyFBRoleInfo::SCDailyFBRoleInfo() : header(MT_FB_DAILY_ROLE_INFO_SC) {}
	CSExpFBRetchFirstRewardReq::CSExpFBRetchFirstRewardReq() : header(MT_EXP_FB_FETCH_FIRST_REWARD_REQ_CS) {}

	CSPataFbAllInfo::CSPataFbAllInfo() : header(MT_FB_PATA_REQ_ALL_INFO_CS) {}
	SCPataFbAllInfo::SCPataFbAllInfo() : header(MT_FB_PATA_ALL_INFO_SC) {}

	CSVipFbAllInfoReq::CSVipFbAllInfoReq() : header(MT_FB_VIP_ALL_INFO_REQ_CS) {}
	SCVipFbAllInfo::SCVipFbAllInfo() : header(MT_FB_VIP_ALL_INFO_SC) {}
	CSFetchTimeLimitVip::CSFetchTimeLimitVip() : header(MT_FETCH_TIME_LIMIT_VIP_CS) {}

	CSTeamFbRoomOperate::CSTeamFbRoomOperate() : header(MT_FB_TEAM_ROOM_OPERATE_CS) {}
	SCTeamFbRoomList::SCTeamFbRoomList() : header(MT_FB_TEAM_ROOM_LIST_SC) {}
	SCTeamFbRoomEnterAffirm::SCTeamFbRoomEnterAffirm() : header(MT_FB_TEAM_ROOM_ENTER_AFFIRM_SC) {}

	CSTowerDefendBuyJoinTimes::CSTowerDefendBuyJoinTimes() : header(MT_FB_TOWERDEFEND_BUY_JOIN_TIMES_CS) {}
	CSTowerDefendRoleInfo::CSTowerDefendRoleInfo() : header(MT_FB_TOWERDEFEND_ROLE_INFO_CS) {}
	SCTowerDefendRoleInfo::SCTowerDefendRoleInfo() : header(MT_FB_TOWERDEFEND_ROLE_INFO_SC) {}
	SCTowerDefendInfo::SCTowerDefendInfo() : header(MT_FB_TOWERDEFEND_INFO_SC) {}
	SCTowerDefendWarning::SCTowerDefendWarning() : header(MT_FB_TOWERDEFEND_WARNING_SC) {}
	SCFBDropInfo::SCFBDropInfo() : header(MT_FB_DROP_INFO_SC){}
	SCTowerDefendResult::SCTowerDefendResult() : header(MT_FB_TOWERDEFEND_RESULT_SC) {}
	CSTowerDefendNextWave::CSTowerDefendNextWave() : header(MT_FB_TOWERDEFEND_REFRESH_REQ_CS) {}

	SCYsjtTeamFbSceneLogicInfo::SCYsjtTeamFbSceneLogicInfo() : header(MT_FB_YSJT_TEAM_FB_LOGIC_INFO_SC) {}
	CSMgxfTeamFbTouchDoor::CSMgxfTeamFbTouchDoor() : header(MT_FB_MGXF_TOUCH_DOOR_CS) {}
	SCMgxfTeamFbSceneLogicInfo::SCMgxfTeamFbSceneLogicInfo() : header(MT_FB_MGXF_TEAM_FB_LOGIC_INFO_SC) {}
	SCGuildFBInfo::SCGuildFBInfo():header(MT_GUILDFB_INFO_SC){}
	SCGuildFBRewardRecordInfo::SCGuildFBRewardRecordInfo() : header(MT_GUILDFB_REWARD_INFO_SC) {}

	SCTiangongShilianFBInfo::SCTiangongShilianFBInfo() : header(MT_FB_TIANGONGSHILIAN_FB_INFO_SC) {}
	SCTiangongShilianFBResult::SCTiangongShilianFBResult() : header(MT_FB_TIANGONGSHILIAN_FB_RESULT_SC) {}

	SCFBFinish::SCFBFinish() : header(MT_FB_FINISH_SC) {}
	SCFBBossHPPercentInfo::SCFBBossHPPercentInfo() : header(MT_FB_BOSS_HP_PERCENT_INFO_SC) {}

	SCNeqFBInfo::SCNeqFBInfo() : header(MT_FB_NEQ_INFO_SC) {}
	SCNeqPass::SCNeqPass() : header(MT_FB_NEQ_PASS_INFO_SC) {}
	SCNeqRollInfo::SCNeqRollInfo() : header(MT_FB_NEQ_ROLL_INFO_SC) {}
	SCNeqRollPool::SCNeqRollPool() : header(MT_FB_NEQ_ROLL_POOL_SC) {}

	SCPhaseFBInfo::SCPhaseFBInfo() : header(MT_FB_PHASE_INFO_SC){}
	CSPhaseFBInfoReq::CSPhaseFBInfoReq() : header(MT_FB_PHASE_INFO_CS){}

	CSEnterBossFamily::CSEnterBossFamily() : header(MT_ENTER_BOSS_FAMILY_CS) {}
	CSBossFamilyOperate::CSBossFamilyOperate() : header(MT_ENTER_BOSS_FAMILY_OPERATE_CS) {}
	CSGetBossInfoReq::CSGetBossInfoReq() : header(MT_GET_BOSS_INFO_REQ_CS) {}
	CSBossKillerInfoReq::CSBossKillerInfoReq() : header(MT_BOSS_KILLER_INFO_REQ) {}
	SCDabaoBossInfo::SCDabaoBossInfo() : header(MT_ENTER_BOSS_DABAO_INFO_SC) {}
	SCFamilyBossInfo::SCFamilyBossInfo() : header(MT_ENTER_BOSS_FAMILY_INFO_SC) {}
	SCMikuBossInfo::SCMikuBossInfo() : header(MT_ENTER_BOSS_MIKU_INFO_SC) {}
	SCMikuMonsterInfo::SCMikuMonsterInfo() : header(MT_ENTER_BOSS_MIKU_MONSTER_INFO_SC) {}
	SCBossRoleInfo::SCBossRoleInfo() : header(MT_ENTER_BOSS_ROLE_INFO_SC) {}
	SCDabaoBossNextFlushInfo::SCDabaoBossNextFlushInfo() : header(MT_DABAO_BOSS_NEXT_FLUSH_TIME_SC) {}
	SCBossInfoToAll::SCBossInfoToAll() : header(MT_BOSS_INFO_TO_ALL_SC) {}
	SCBossKillerList::SCBossKillerList() : header(MT_BOSS_KILLER_LIST_SC) {}
	SCFollowBossInfo::SCFollowBossInfo() : header(MT_FOLLOW_BOSS_INFO_SC) {}
	SCActiveBossNextFlushInfo::SCActiveBossNextFlushInfo() : header(MT_ACTIVE_BOSS_NEXT_FLUSH_TIME_SC) {}
	SCActiveBossInfo::SCActiveBossInfo() : header(MT_ENTER_BOSS_ACTIVE_INFO_SC) {}
	SCActiveBossHurtRankInfo::SCActiveBossHurtRankInfo() : header(MT_ACTIVE_BOSS_HURT_RANK_INFO_SC) {}
	SCActiveBossLeaveInfo::SCActiveBossLeaveInfo():header(MT_ACTIVE_BOSS_LEAVE_INFO_SC){}
	SCMikuBossHurtRankInfo::SCMikuBossHurtRankInfo() : header(MT_MIKU_BOSS_HURT_RANK_INFO_SC) {}
	SCMikuBossLeaveInfo::SCMikuBossLeaveInfo() : header(MT_MIKU_BOSS_LEAVE_INFO_SC) {}
	SCPreciousBossTaskInfo::SCPreciousBossTaskInfo() : header(MT_PRECIOUS_BOSS_TASK_INFO_SC) {}
	SCPreciousBossInfo::SCPreciousBossInfo() : header(MT_PRECIOUS_BOSS_INFO_SC) {}
	CSPreciousOperaReq::CSPreciousOperaReq() : header(MT_PRECIOUS_BOSS_POS_REQ_CS) {}
	SCPreciousPosInfo::SCPreciousPosInfo() : header(MT_PRECIOUS_BOSS_POS_INFO_CS) {}
	
	CSReqMonsterGeneraterList::CSReqMonsterGeneraterList():header(MT_GET_SCENE_MONSTER_GENERATE_INFO_CS) {}
	SCMonsterGeneraterList::SCMonsterGeneraterList():header(MT_GET_SCENE_MONSTER_GENERATE_INFO_SC) {}
	CSReqGatherGeneraterList::CSReqGatherGeneraterList() : header(MT_GET_SCENE_GATHER_GENERATE_INFO_CS) {}
	SCGatherGeneraterList::SCGatherGeneraterList() : header(MT_GET_SCENE_GATHER_GENERATE_INFO_SC) {}

	CSEnterYaoShouGuangChang::CSEnterYaoShouGuangChang() : header(MT_ENTER_YAOSHOUGUANGCHANG_CS) {}
	CSGetYaoShouGuangChangState::CSGetYaoShouGuangChangState() : header(MT_GET_YAOSHOUGUANGCHANG_STATE_CS) {}
	SCYaoShouGuangChangState::SCYaoShouGuangChangState() : header(MT_YAOSHOUGUANGCHANG_STATE_SC) {}
	SCYaoShouGuangChangFBInfo::SCYaoShouGuangChangFBInfo() : header(MT_YAOSHOUGUANGCHANG_FBINFO_SC) {}
	CSGetYaoShouGuangChangReward::CSGetYaoShouGuangChangReward() : header(MT_GET_YAOSHOUGUANGCHANG_REWARD_CS) {}
	SCNotifyYaoShouGuangChangReward::SCNotifyYaoShouGuangChangReward() : header(MT_NOTIFY_YAOSHOUGUANGCHANG_REWARD_SC) {}
	
	SCZhuaGuiFbInfo::SCZhuaGuiFbInfo() : header(MT_ZHUA_GUI_FBINFO_CS) {}
	SCZhuaguiAddPerInfo::SCZhuaguiAddPerInfo() : header(MT_ZHUA_GUI_ADD_PER_INFO_CS) {}
	SCCrossTeamFbInfo::SCCrossTeamFbInfo() : header(MT_CROSS_TEAM_FB_INFO_SC) {}

	SCTeamEquipFbInfo::SCTeamEquipFbInfo() : header(MT_TEAM_EQUIP_FB_INFO_SC) {}
	SCTeamEquipFbDropCountInfo::SCTeamEquipFbDropCountInfo() : header(MT_TEAM_EQUIP_FB_DROP_TIMES_INFO_SC) {}
	CSTeamEquipFbBuyDropCount::CSTeamEquipFbBuyDropCount() : header(MT_TEAM_EQUIP_FB_BUY_DROP_TIMES_CS) {}
	
	CSEnterSuoYaoTa::CSEnterSuoYaoTa() : header(MT_ENTER_SUOYAOTA_CS) {}
	CSGetSuoYaoTaState::CSGetSuoYaoTaState() : header(MT_GET_SUOYAOTA_STATE_CS) {}
	SCSuoYaoTaState::SCSuoYaoTaState() : header(MT_SUOYAOTA_STATE_SC) {}
	SCSuoYaoTaFBInfo::SCSuoYaoTaFBInfo() : header(MT_SUOYAOTA_FBINFO_SC) {}
	CSGetSuoYaoTaReward::CSGetSuoYaoTaReward() : header(MT_GET_SUOYAOTA_REWARD_CS) {}
	SCNotifySuoYaoTaReward::SCNotifySuoYaoTaReward() : header(MT_NOTIFY_SUOYAOTA_REWARD_SC) {}
	
	CSGetGuildFBGuardPos::CSGetGuildFBGuardPos() : header(MT_GET_GUILDFB_GUARD_POS_CS) {}
	SCGuildFBGuardPos::SCGuildFBGuardPos() : header(MT_GUILDFB_GUARD_POS_SC) {}

	SCWorldBossDead::SCWorldBossDead() : header(MT_WORLD_BOSS_DEAD_SC) {}

	CSNeqFBStarRewardReq::CSNeqFBStarRewardReq() : header(MT_FB_NEQ_TAKEREWARD_CS) {}
	CSNeqFBExchangeReq::CSNeqFBExchangeReq() : header(MT_FB_NEQ_EXCHANGE_CS) {}
	CSNeqFBBuyTimesReq::CSNeqFBBuyTimesReq() : header(MT_FB_NEQ_BUY_TIMES_CS) {}
	CSNeqFBAutoReq::CSNeqFBAutoReq() : header(MT_FB_NEQ_AUTO_CS) {}
	CSNeqInfoReq::CSNeqInfoReq() : header(MT_FB_NEQ_INFO_CS) {}
	CSNeqRollReq::CSNeqRollReq() : header(MT_FB_NEQ_ROLL_CS) {}

	CSArmorDefendRoleReq::CSArmorDefendRoleReq():header(MT_FB_ARMORDEFEND_ROLE_REQ_CS){}
	SCArmorDefendRoleInfo::SCArmorDefendRoleInfo():header(MT_FB_ARMORDEFEND_ROLE_INFO_SC){}
	SCArmorDefendResult::SCArmorDefendResult():header(MT_FB_ARMORDEFEND_RESULT_SC){}
	SCArmorDefendInfo::SCArmorDefendInfo():header(MT_FB_ARMORDEFEND_INFO_SC){}
	SCArmorDefendWarning::SCArmorDefendWarning():header(MT_FB_ARMORDEFEND_WARNING_SC){}
	SCArmorDefendPerformSkill::SCArmorDefendPerformSkill():header(MT_FB_ARMORDEFEND_PERFORM_SKILL_SC){}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSBuildTowerReq::CSBuildTowerReq() : header(MT_BUILD_TOWER_OPERA_CS) {}
	SCBuildTowerFBSceneLogicInfo::SCBuildTowerFBSceneLogicInfo() : header(MT_BUILD_TOWER_SCENE_INFO_SC) {}
	CSBuildTowerBuyTimes::CSBuildTowerBuyTimes() : header(MT_BUILD_TOWER_BUY_TIMES_CS) {}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCTeamTowerDefendInfo::SCTeamTowerDefendInfo() : header(MT_FB_TEAMTOWERDEFEND_INFO_SC) {}
	SCTeamTowerDefendAttrType::SCTeamTowerDefendAttrType() : header(MT_FB_TEAMTOWERDEFEND_ATTR_TYPE_INFO_SC) {}
	SCTeamTowerDefendSkill::SCTeamTowerDefendSkill() : header(MT_FB_TEAMTOWERDEFEND_SKILL_INFO_SC) {}
	CSTeamTowerDefendOpreatReq::CSTeamTowerDefendOpreatReq() : header(MT_FB_TEAMTOWERDEFEND_OPREAT_REQ_CS) {}
	SCTeamTowerDefendAllRole::SCTeamTowerDefendAllRole():header(MT_FB_TEAMTOWERDEFEND_ALL_ROLE_SC){}
	SCTeamTowerDefendResult::SCTeamTowerDefendResult():header(MT_FB_TEAMTOWERDEFEND_RESULTS_SC){}

	CSShenqiOperaReq::CSShenqiOperaReq() : header(MT_SHENQI_OPERA_REQ_CS) {}
	SCShenqiAllInfo::SCShenqiAllInfo() : header(MT_SHENQI_ALL_INFO_SC) {}
	SCShenqiSingleInfo::SCShenqiSingleInfo() : header(MT_SHENQI_SINGLE_INFO_SC) {}
	SCShenqiImageInfo::SCShenqiImageInfo() : header(MT_SHENQI_IMAGE_INFO_SC) {}
	SCShenqiDecomposeResult::SCShenqiDecomposeResult() : header(MT_SHENQI_DECOMPOSE_RESULT_SC) {}
	
	/////////////////////////////////////////////////////////////////////////////////////
	CSQueryGuildMonster::CSQueryGuildMonster() : header(MT_FB_GUILDMONSTER_CS) {}	
	SCGuildMonsterInfo::SCGuildMonsterInfo() : header(MT_FB_GUILDMONSTER_INFO_SC) {}
	SCGuildMonsterFBResult::SCGuildMonsterFBResult() : header(MT_FB_GUILDMONSTER_RESULT_SC) {}
	SCAutoFBRewardDetail::SCAutoFBRewardDetail() : header(MT_FB_AUTO_FB_REWARD_DETAIL_SC), item_count(0) {}
	CSFightingReq::CSFightingReq() : header(MT_FITHTING_MINING_REQ_CS) {}
	SCFightingMiningBaseInfo::SCFightingMiningBaseInfo() : header(MT_FIGHTING_MINING_BASE_INFO_SC) {}
	SCFightingMiningBeenRobList::SCFightingMiningBeenRobList() : header(MT_FIGHTING_MINING_BEEN_ROB_LIST_SC) {}
	SCFightingMiningList::SCFightingMiningList() : header(MT_FIGHTING_MINING_LIST_SC) {}
	SCFightingChallengeBaseInfo::SCFightingChallengeBaseInfo() : header(MT_FIGHTING_CHALLENGE_BASE_INFO_SC) {}
	SCFightingResultNotify::SCFightingResultNotify() : header(MT_FIGHTING_MINING_FIGHTING_RESULT) {}
	SCFightingChallengeList::SCFightingChallengeList() : header(MT_FIGHTING_CHALLENGE_LIST_SC) {}
	SCFightingSailingBaseInfo::SCFightingSailingBaseInfo() : header(MT_FIGHTING_SAILING_BASE_INFO_SC) {}
	SCFightingSailingBeenRobList::SCFightingSailingBeenRobList() : header(MT_FIGHTING_SAILING_BEEN_ROB_LIST_SC) {}
	SCFightingSailingList::SCFightingSailingList() : header(MT_FIGHTING_SAILING_LIST_SC) {}
	SCFightingCountDownNotify::SCFightingCountDownNotify() : header(MT_FIGHTING_COUNT_DOWN_NOTIFY_SC) {}
	SCFightingBeenRobNotify::SCFightingBeenRobNotify() : header(MT_FIGHTING_BEEN_ROB_NOTIFY_SC) {}
	SCFightingRobingNotify::SCFightingRobingNotify() : header(MT_FIGHTING_ROBING_NOTIFY_SC) {}

	//////////////////////////////////////////////////////////////////////////
	CSUserLogin::CSUserLogin():header(MT_USER_LOGIN_CS) {}
	SCUserLoginAck::SCUserLoginAck():header(MT_USER_LOGIN_ACK_SC) {}
	SCUserBeDestroy::SCUserBeDestroy():header(MT_USER_BE_DESTROY_SC){}
	SCIntoChannel::SCIntoChannel():header(MT_INTO_CHANNEL_SC){}
	SCOutOfChannel::SCOutOfChannel():header(MT_OUT_OF_CHANNEL_SC){}

	SCNoticeOnline::SCNoticeOnline():header(MT_NOTICE_ONLINE_SC){}
	SCNoticeOffline::SCNoticeOffline():header(MT_NOTICE_OFFLINE_SC){}
	CSChangeOnlineStatus::CSChangeOnlineStatus():header(MT_CHANGE_ONLINE_STATUS_CS){}
	SCNoticeOnlineStatus::SCNoticeOnlineStatus():header(MT_NOTICE_ONLINE_STATUS_SC){}

	CSChannelChat::CSChannelChat():header(MT_CHANNEL_CHAT_CS){}
	SCChannelChat::SCChannelChat():header(MT_CHANNEL_CHAT_SC){}
	CSSingleChat::CSSingleChat():header(MT_SINGLE_CHAT_CS){}
	SCSingleChat::SCSingleChat():header(MT_SINGLE_CHAT_SC){}
	SCSingleChatUserNotExist::SCSingleChatUserNotExist():header(MT_SINGLE_CHAT_USET_NOT_EXIST_SC){}
	SCOpenLevelLimit::SCOpenLevelLimit():header(MT_CHAT_LEVEL_LIMIT_SC){}

	CSForbidChatInfo::CSForbidChatInfo() : header(MT_FORBID_CHAT_INFO_CS) {}
	SCForbidChatInfo::SCForbidChatInfo() : header(MT_FORBID_CHAT_INFO_SC) {}
	SCForbidUserInfo::SCForbidUserInfo() : header(MT_FORBID_TALK_USER_INFO_SC) {}

	SCChatBoardListInfo::SCChatBoardListInfo() : header(MT_CHAT_BOARD_LIST_SC) {}

	CSSingleChatReq::CSSingleChatReq() : header(MT_SINGLE_CHAT_REQ_CS) {}
	SCSingleChatOnlineStatus::SCSingleChatOnlineStatus() : header(MT_SINGLE_CHAT_INFO_SC) {}

	CSMuteUser::CSMuteUser():header(MT_MUTE_USER_CS){}
	CSUnMuteUser::CSUnMuteUser():header(MT_UNMUTE_USER_CS){}

	//////////////////////////////////////////////////////////////////////////
	SCMailSendAck::SCMailSendAck() : header(MT_GLOBAL_MAIL_SEND_RESULT_SC) {}
	SCMailDeleteAck::SCMailDeleteAck() : header(MT_GLOBAL_MAIL_DELETE_SC) {}
	SCMailLockAck::SCMailLockAck() : header(MT_GLOBAL_MAIL_LOCK_SC) {}
	SCMailUnlockAck::SCMailUnlockAck() : header(MT_GLOBAL_MAIL_UNLOCK_SC) {}
	SCMailListAck::SCMailListAck() : header(MT_GLOBAL_MAIL_LIST_SC) {}
	SCMailDetailAck::SCMailDetailAck() : header(MT_GLOBAL_MAIL_DETAIL_SC) {}
	SCFetchAttachmentAck::SCFetchAttachmentAck() : header(MT_GLOBAL_MAIL_ATTACHMENT_SC) {}
	SCRecvNewMail::SCRecvNewMail() : header(MT_GLOBAL_MAIL_NEW_MAIL_NOTICE_SC) {}
	SCHasUnReadMail::SCHasUnReadMail() : header(MT_GLOBAL_MAIL_HAS_UNREAD_MAIL_SC) {}

	CSCreateTeam::CSCreateTeam():header(MT_CREATE_TEAM_CS){}
	SCTeamInfo::SCTeamInfo():header(MT_TEAM_INFO_SC){}
	SCOutOfTeam::SCOutOfTeam():header(MT_OUT_OF_TEAM_SC){}
	SCJoinTeam::SCJoinTeam():header(MT_JOIN_TEAM_SC){}
	SCTeamLeaderChange::SCTeamLeaderChange():header(MT_TEAM_LEADER_CHANGE_SC){}
	CSInviteUser::CSInviteUser():header(MT_INVITE_USER_CS){}
	SCInviteUserTransmit::SCInviteUserTransmit():header(MT_INVITE_USER_TRANSMIT_SC){}
	CSInviteUserTransmitRet::CSInviteUserTransmitRet():header(MT_INVITE_RET_CS){}
	SCReqJoinTeamTransmit::SCReqJoinTeamTransmit():header(MT_REQ_JOIN_TEAM_SC){}
	SCRoleTeamInfo::SCRoleTeamInfo():header(MT_ROLE_TEAM_INFO_SC) {}
	SCTeamRollDropRet::SCTeamRollDropRet():header(MT_TEAM_ROLL_DROP_RET_SC) {}

	CSReqJoinTeamRet::CSReqJoinTeamRet():header(MT_REQ_JOIN_TEAM_RET_CS){}
	CSReqJoinTeam::CSReqJoinTeam():header(MT_REQ_JOIN_TEAM_CS){}
	CSDismissTeam::CSDismissTeam():header(MT_DISMISS_TEAM_CS){}
	CSKickOutOfTeam::CSKickOutOfTeam():header(MT_KICK_OUT_OF_TEAM_CS){}
	CSChangeTeamLeader::CSChangeTeamLeader():header(MT_CHANGE_TEAM_LEADER_CS){}
	CSExitTeam::CSExitTeam():header(MT_EXIT_TEAM_CS){}
	CSAutoHaveTeam::CSAutoHaveTeam() : header(MT_AUTO_HAVE_TEAM_CS){}
	CSAutoApplyJoinTeam::CSAutoApplyJoinTeam() : header(MT_AUTO_APPLY_JOIN_TEAM_CS){}
	CSTeamListReq::CSTeamListReq():header(MT_TEAM_LIST_REQ_CS){}
	SCTeamListAck::SCTeamListAck():header(MT_TEAM_LIST_ACK_SC){}
	CSChangeMustCheck::CSChangeMustCheck():header(MT_CHANGE_MUST_CHECK_CS){}
	CSChangeAssignMode::CSChangeAssignMode():header(MT_CHANGE_ASSIGN_MODE_CS){}
	CSChangeMemberCanInvite::CSChangeMemberCanInvite():header(MT_CHANGE_MEMBER_CAN_INVITE_CS){}
	CSChangeTeamLimit::CSChangeTeamLimit():header(MT_CHANGE_TEAM_LIMIT_CS){}
	CSInviteUniqueUser::CSInviteUniqueUser() : header(MT_INVITE_UNIQUE_USER_CS) {}

	CSGetPersonRankList::CSGetPersonRankList() : header(MT_RANK_GET_PERSONRANK_LIST_CS) {}
	SCGetPersonRankListAck::SCGetPersonRankListAck() : header(MT_RANK_GET_PERSONRANK_LIST_ACK_SC) {}

	CSGetGuildRankList::CSGetGuildRankList() : header(MT_RANK_GET_GUILDRANK_LIST_CS) {}
	SCGetGuildRankListAck::SCGetGuildRankListAck() : header(MT_RANK_GET_GUILDRANK_LIST_ACK_SC) {}

	CSGetTeamRankList::CSGetTeamRankList() : header(MT_RANK_GET_TEAMRANK_LIST_CS) {}
	SCGetTeamRankListAck::SCGetTeamRankListAck() : header(MT_RANK_GET_TEAMRANK_LIST_ACK_SC) {}

	CSGetPersonRankTopUser::CSGetPersonRankTopUser() : header(MT_RANK_GET_PERSONRANK_TOP_USER_CS) {}
	SCGetPersonRankTopUser::SCGetPersonRankTopUser() : header(MT_RANK_GET_PERSONRANK_TOP_USER_SC) {}
	SCGetWorldLevelAck::SCGetWorldLevelAck() : header(MT_RANK_GET_WORLD_LEVEL_ACK_SC) {}

	CSGetCoupleRankList::CSGetCoupleRankList() : header(MT_RANK_GET_COUPLE_RANK_LIST_CS) {}
	SCGetCoupleRankListAck::SCGetCoupleRankListAck() : header(MT_RANK_GET_COUPLE_RANK_SC) {}

	SCGetSpecialRankValueAck::SCGetSpecialRankValueAck() : header(MT_RANK_GET_SPECIAL_RANK_SC) {}
	CSGetSpecialRankValue::CSGetSpecialRankValue() : header(MT_RANK_GET_SPECIAL_RANK_LIST_CS) {}
	//////////////////////////////////////////////////////////////////////////

	SCAddPublicSaleItemAck::SCAddPublicSaleItemAck() : header(MT_PUBLICSALE_ADD_ACK_SC) {}
	SCRemovePublicSaleItemAck::SCRemovePublicSaleItemAck() : header(MT_PUBLICSALE_DEL_ACK_SC) {}
	SCBuyPublicSaleItemAck::SCBuyPublicSaleItemAck() : header(MT_PUBLICSALE_BUY_ACK_SC) {}
	SCGetPublicSaleItemListAck::SCGetPublicSaleItemListAck() : header(MT_PUBLICSALE_GET_ITEM_LIST_SC) {}

	CSPublicSaleSearch::CSPublicSaleSearch() : header(MT_PUBLICSALE_SEARCH_CS) {}
	SCPublicSaleSearchAck::SCPublicSaleSearchAck() : header(MT_PUBLICSALE_SEARCH_SC) {}

	//////////////////////////////////////////////////////////////////////////
	SCRoleMsgReply::SCRoleMsgReply() : header(MT_SYSTEM_ROLE_MSG_REPLY) {}
	//////////////////////////////////////////////////////////////////////////
	SCWushuangEquipInfo::SCWushuangEquipInfo() : header(MT_WUSHUANG_EQUIP_INFO_SC) {}
	SCWushuangFBKickOutNotify::SCWushuangFBKickOutNotify() : header(MT_WUSHUANG_FB_KICKOUT_NOTIFY_SC) {}
	SCWushuangFBInfo::SCWushuangFBInfo() : header(MT_WUSHUANG_FB_INFO_SC) {}
	SCWushuangBagInfo::SCWushuangBagInfo() : header(MT_WUSHUANG_BAG_INFO_SC) {}
	SCHushouOperaStatus::SCHushouOperaStatus() : header(MT_HUNSHOU_OPERA_STATUS_SC) {}
	SCZhuanzhiFBNotify::SCZhuanzhiFBNotify() : header(MT_ZHUANZHI_FB_NOTIFY_SC) {}

	CSWushuangEquipOperaReq::CSWushuangEquipOperaReq() : header(MT_WUSHUANG_OPERA_REQ_CS) {}
	CSWushuangFBInfo::CSWushuangFBInfo() : header(MT_WUSHUANG_FB_INFO_CS) {}
	CSHunshouOperaReq::CSHunshouOperaReq() : header(MT_HUNSHOU_OPERA_REQ_CS) {}

	//////////////////////////////////////////////////////////////////////////
	CSCrossStartReq::CSCrossStartReq() : header(MT_CROSS_START_REQ_CS) {}
	SCCrossEnterServer::SCCrossEnterServer() : header(MT_CROSS_ENTER_SERVER_SC) {}

	//////////////////////////////////////////////////////////////////////////
	CSLieMingHunshouOperaReq::CSLieMingHunshouOperaReq() : header(MT_LIEMING_HUNSHOU_OPERA_REQ_CS) {}
	SCLieMingSlotInfo::SCLieMingSlotInfo() : header(MT_LIEMING_SLOT_INFO_SC) {}
	SCLieMingBagInfo::SCLieMingBagInfo() : header(MT_LIEMING_BAG_INFO_SC) {}

	//////////////////////////////////////////////////////////////////////////
	SCJingLingInfo::SCJingLingInfo() : header(MT_JINGLING_INFO_SC) {}
	SCJingLingViewChange::SCJingLingViewChange() : header(MT_JINGLING_VIEW_CHANGE_SC) {}
	CSJingLingHomeOperReq::CSJingLingHomeOperReq() : header(MT_JINGLING_HOME_OPER_REQ_CS) {}
	SCJingLingHomeInfo::SCJingLingHomeInfo() : header(MT_JINGLING_HOME_INFO_SC) {}
	SCJingLingHomeListInfo::SCJingLingHomeListInfo() : header(MT_JINGLING_HOME_LINT_INFO_SC) {}
	SCJingLingHomeRobRecord::SCJingLingHomeRobRecord() : header(MT_JINGLING_HOME_ROB_RECORD_SC) {}
	CSJinglIngExploreOperReq::CSJinglIngExploreOperReq() : header(MT_JINGLING_EXPLORE_OPER_REQ_CS) {}
	SCJinglIngExploreInfo::SCJinglIngExploreInfo() : header(MT_JINGLING_EXPLORE_INFO_SC) {}
	SCJingLingHomeRobAck::SCJingLingHomeRobAck() : header(MT_JINGLING_HOME_ROB_ACK_SC) {}

	SCJingLingAdvantageInfo::SCJingLingAdvantageInfo() : header(MT_JINGLING_ADVANTAGE_INFO_SC){}
	SCJingLingAdvantageCount::SCJingLingAdvantageCount() : header(MT_JINGLING_ADVANTAGE_COUNT_SC) {}

	SCJingLingAdvantageBossInfo::SCJingLingAdvantageBossInfo() : header(MT_JINGLING_ADVANTAGE_BOSS_POS_SC) {}
	CSJingLingAdvantageBossEnter::CSJingLingAdvantageBossEnter() : header(MT_JINGLING_ADVANTAGE_BOSS_ENTER_CS) {}
	SCJingLingEggPosInfo::SCJingLingEggPosInfo() : header(MT_JINGLING_EGG_POS_INFO) {}
	
	//////////////////////////////////////////////////////////////////////////
	SCShengWangInfo::SCShengWangInfo() : header(MT_SHENGWANG_INFO_SC) {}
	SCXianJieViewChange::SCXianJieViewChange() : header(MT_XIANJIE_VIEW_CHANGE_SC) {}

	//////////////////////////////////////////////////////////////////////////
	SCChengJiuInfo::SCChengJiuInfo() : header(MT_CHENGJIU_INFO_SC) {}
	SCChengJiuTitleViewChange::SCChengJiuTitleViewChange() : header(MT_CHENGJIU_TITLE_VIEW_CHANGESC) {}
	SCChengJiuRewardChange::SCChengJiuRewardChange() : header(MT_CHENGJIU_REWARD_CHANGESC) {}
	SCChengJiuRewardInfo::SCChengJiuRewardInfo() : header(MT_CHENGJIU_REWARD_INFO_SC) {}

	//////////////////////////////////////////////////////////////////////////
	SCCardzuAllInfo::SCCardzuAllInfo() : header(MT_CARDZU_BASE_INFO_SC) {}
	SCCardzuChangeNotify::SCCardzuChangeNotify() : header(MT_CARDZU_LEVEL_LIST_INFO_SC) {}
	SCCardzuChouCardResult::SCCardzuChouCardResult() : header(MT_CARDZU_CARD_LIST_INFO_SC) {}

	CSCardzuOperaReq::CSCardzuOperaReq() : header(MT_CARDZU_OPERA_REQ_CS) {}

	//////////////////////////////////////////////////////////////////////////
	CSCrossXiuluoTowerJoinReq::CSCrossXiuluoTowerJoinReq():header(MT_CROSS_XIULUO_TOWER_JOIN_REQ_CS) {}
	SCCrossXiuluoTowerSelfActivityInfo::SCCrossXiuluoTowerSelfActivityInfo():header(MT_CROSS_XIULUO_TOWER_SELF_ACTIVITY_INFO_SC) {}
	SCCrossXiuluoTowerRankInfo::SCCrossXiuluoTowerRankInfo():header(MT_CROSS_XIULUO_TOWER_RANK_INFO_SC) {}
	SCCrossXiuluoTowerChangeLayerNotice::SCCrossXiuluoTowerChangeLayerNotice():header(MT_CROSS_XIULUO_TOWER_CHANGE_LAYER_NOTICE_SC) {}
	SCCrossXiuluoTowerUserResult::SCCrossXiuluoTowerUserResult():header(MT_CROSS_XIULUO_TOWER_USER_RESULT_SC) {}
	SCCrossXiuluoTowerInfo::SCCrossXiuluoTowerInfo():header(MT_CROSS_XIULUO_TOWER_INFO_SC) {}
	CSCrossXiuluoTowerBuyBuff::CSCrossXiuluoTowerBuyBuff():header(MT_CROSS_XIULUO_TOWER_BUY_BUFF_CS) {}
	SCCrossHonorChange::SCCrossHonorChange():header(MT_CROSS_HONOR_CHANGE_SC) {}
	SCCrossXiuluoTowerBuffInfo::SCCrossXiuluoTowerBuffInfo():header(MT_CROSS_XIULUO_TOWER_BUFF_INFO_SC) {}
	CSCrossXiuluoTowerScoreRewardReq::CSCrossXiuluoTowerScoreRewardReq():header(MT_CROSS_XIULUO_TOWER_SCORE_REWARD_REQ_CS) {}
	SCCrossXiuluoTowerGatherInfo::SCCrossXiuluoTowerGatherInfo():header(MT_CROSS_XIULUO_TOWER_GATHER_INFO_SC) {}
	SCCossXiuluoTowerRankTitleInfo::SCCossXiuluoTowerRankTitleInfo():header(MT_CROSS_XIULUO_RNAK_TITLE_INFO_SC){}
	SCCossXiuluoTowerBossInfo::SCCossXiuluoTowerBossInfo():header(MT_CROSS_XIULUO_TOWER_BOSS_INFO){}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSCrossMatch1V1Req::CSCrossMatch1V1Req() : header(MT_CROSS_1V1_MATCH_REQ_CS) {}
	CSCross1v1FightReady::CSCross1v1FightReady() : header(MT_CROSS_1V1_FIGHT_READY_CS) {}
	SCCrossActivity1V1SelfInfo::SCCrossActivity1V1SelfInfo() : header(MT_CROSS_1V1_SELF_INFO_SC) {}
	SCCross1v1FightStart::SCCross1v1FightStart() : header(MT_CROSS_1V1_FIGHT_START_SC) {}
	CSCross1v1FetchRewardReq::CSCross1v1FetchRewardReq() : header(MT_CROSS_1V1_FETCH_REWARD_REQ_CS) {}
	CSCross1v1WearRingReq::CSCross1v1WearRingReq() : header(MT_CROSS_1V1_RING_OPER_CS) {}
	CSCross1v1MatchResultReq::CSCross1v1MatchResultReq() : header(MT_CROSS_1V1_MATCH_RESULT_REQ_CS) {}
	CSCross1v1BuyTimeReq::CSCross1v1BuyTimeReq() : header(MT_CROSS_1V1_BUY_TIME_REQ_CS) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSCrossMultiuserChallengeMatchgingReq::CSCrossMultiuserChallengeMatchgingReq():header(MT_CROSS_MULTIUSER_CHALLENGE_MATCHING_REQ_CS) {}
	CSCrossMultiuserChallengeGetBaseSelfSideInfo::CSCrossMultiuserChallengeGetBaseSelfSideInfo():header(MT_CROSS_MULTIUSER_CHALLENGE_GET_BASE_SELF_SIDE_INFO_CS) {}
	CSCrossMultiuserChallengeFetchDaycountReward::CSCrossMultiuserChallengeFetchDaycountReward():header(MT_CROSS_MULTIUSER_CHALLENGE_FETCH_DAYCOUNT_REWARD_CS) {}
	CSCrossMultiuerChallengeCancelMatching::CSCrossMultiuerChallengeCancelMatching():header(MT_CROSS_MULTIUSER_CHALLENGE_CANCLE_MATCHING_CS) {}
	CSMultiuserChallengeReqSideMemberPos::CSMultiuserChallengeReqSideMemberPos():header(MT_CROSS_MULTIUSER_CHALLENGE_REQ_SIDE_MEMBER_POS_CS) {}
	CSMultiuserChallengeWearCardReq::CSMultiuserChallengeWearCardReq() : header(MT_CROSS_MULTIUSER_CHALLENGE_CARD_OPER_REQ) {}
	CSCrossMultiuserChallengeFetchGongxunReward::CSCrossMultiuserChallengeFetchGongxunReward() : header(MT_CROSS_MULTIUSER_CHALLENGE_FETCH_GONGXUN_REWARD_SC) {}

	SCCrossMultiuserChallengeSelfInfoRefresh::SCCrossMultiuserChallengeSelfInfoRefresh():header(MT_CROSS_MULTIUSER_CHALLENGE_SELF_INFO_REFRESH_SC) {}
	SCCrossMultiuserChallengeMatchInfoRefresh::SCCrossMultiuserChallengeMatchInfoRefresh():header(MT_CROSS_MULTIUSER_CHALLENGE_MATCH_INFO_REFRESH_SC) {}
	SCCrossMultiuserChallengeMatchState::SCCrossMultiuserChallengeMatchState():header(MT_CROSS_MULTIUSER_CHALLENGE_MATCH_STATE_SC) {}
	SCCrossMultiuserChallengeBaseSelfSideInfo::SCCrossMultiuserChallengeBaseSelfSideInfo():header(MT_CROSS_MULTIUSER_CHALLENGE_BASE_SELF_SIDE_INFO_SC) {}
	SCCrossMultiuserChallengeSelfActicityInfo::SCCrossMultiuserChallengeSelfActicityInfo():header(MT_CROSS_MULTIUSER_CHALLENGE_SELF_ACTIVITY_INFO_SC) {}
	SCMultiuserChallengeTeamMemberPosList::SCMultiuserChallengeTeamMemberPosList():header(MT_CROSS_MULTIUSER_CHALLENGE_REQ_SIDE_MEMBER_POS_SC) {}



	CSCrossTuanzhanFetchReward::CSCrossTuanzhanFetchReward() : header(MT_CROSS_TUANZHAN_FETCH_SCOR_REWARD_CS) {}
	SCCrossTuanzhanStateNotify::SCCrossTuanzhanStateNotify() : header(MT_CROSS_TUANZHAN_STATE_NOTIFY_SC) {}
	SCCrossTuanzhanPlayerInfo::SCCrossTuanzhanPlayerInfo() : header(MT_CROSS_TUANZHAN_PLAYER_INFO_SC) {}
	SCCrossTuanzhanRankInfo::SCCrossTuanzhanRankInfo() : header(MT_CROSS_TUANZHAN_RANK_INFO_SC) {}
	SCCrossTuanzhanSideInfo::SCCrossTuanzhanSideInfo() : header(MT_CROSS_TUANZHAN_SIDE_INFO_SC) {}
	SCCrossTuanzhanPillaInfo::SCCrossTuanzhanPillaInfo() : header(MT_CROSS_TUANZHAN_PILLA_INFO_SC) {}
	SCCrossTuanzhanPlayerDurKillInfo::SCCrossTuanzhanPlayerDurKillInfo() : header(MT_CROSS_TUANZHAN_DUR_KILL_INFO_SC) {}
	SCCrossTuanzhanResultInfo::SCCrossTuanzhanResultInfo() : header(MT_CROSS_TUANZHAN_RESULT_INFO_SC) {}
	SCCrossPasturePlayerInfo::SCCrossPasturePlayerInfo() : header(MT_CROSS_PASTURE_PLAYER_INFO_SC) {}
	SCCrossPastureRankInfo::SCCrossPastureRankInfo() : header(MT_CROSS_PASTURE_RANK_INFO_SC) {}

	CSCrossBossBuyReliveTimes::CSCrossBossBuyReliveTimes() : header(MT_CROSS_BOSS_BUY_RELIVE_TIMES_CS) {}
	SCCrossBossPlayerInfo::SCCrossBossPlayerInfo() : header(MT_CROSS_BOSS_PLAYER_INFO_SC) {}
	SCCrossBossSceneInfo::SCCrossBossSceneInfo() : header(MT_CROSS_BOSS_SCENE_PLAYER_INFO_SC) {}
	SCServerShutdownNotify::SCServerShutdownNotify() : header(MT_SERVER_CLOSE_NOTIFY_SC) {}
	CSCrossBossBossInfoReq::CSCrossBossBossInfoReq() : header(MT_CROSS_BOSS_BOSS_INFO_REQ_CS) {}
	SCCrossBossBossInfoAck::SCCrossBossBossInfoAck() : header(MT_CROSS_BOSS_BOSS_INFO_ACK_SC) {}
	SCCPPlayerHasAttachAnimalNotic::SCCPPlayerHasAttachAnimalNotic() :header(MT_CROSS_PASTURE_ATTACH_ANIMAL_NOTIC) {}
	SCCrossGuildBattleSpecialTimeNotice::SCCrossGuildBattleSpecialTimeNotice() : header(MT_CROSS_CROSS_GUILD_BATTLE_SPECIAL_TIME_NOTICE_SC) {}

	SCCrossDarkNightUserInfo::SCCrossDarkNightUserInfo() : header(MT_CROSS_DARK_NIGHT_USER_INFO_SC) {}
	SCCrossDarkNightRankInfo::SCCrossDarkNightRankInfo() : header(MT_CROSS_DARK_NIGHT_RANK_INFO_SC) {}
	SCCrossDarkNightBossInfo::SCCrossDarkNightBossInfo() : header(MT_CROSS_DARK_NIGHT_BOSS_INFO_SC) {}
	SCCrossDarkNightPlayerInfoBroadcast::SCCrossDarkNightPlayerInfoBroadcast() : header(MT_CROSS_DARK_NIGHT_PLAYER_INFO_BROADCAST_SC) {}
	SCCrossDarkNightRewardTimestampInfo::SCCrossDarkNightRewardTimestampInfo() : header(MT_CROSS_DARK_NIGHT_REWARD_TIMESTAMP_INFO_SC) {}
	SCCrossDarkNightTopPlayerPosi::SCCrossDarkNightTopPlayerPosi() : header(MT_CROSS_DARK_NIGHT_TOP_PLAYERPOS_INFO_SC) {}
	CSCrossDarkNightRankOpera::CSCrossDarkNightRankOpera() : header(MT_CROSS_DARK_NIGHT_PLAYER_REQ_CS) {}
	SCCrossBossBossKillRecord::SCCrossBossBossKillRecord() : header(MT_CROSS_BOSS_KILLER_RECORD_SC) {}
	SCCrossBossDropRecord::SCCrossBossDropRecord() : header(MT_CROSS_BOSS_DROP_RECORD_SC) {}
	SCReliveTire::SCReliveTire() : header(MT_RELIVE_TIRE_INFO_SC) {}

	//////////////////////////////////////////////////////////////////////////
	SCTuHaoJinInfo::SCTuHaoJinInfo() : header(MT_TUHAOJIN_ALL_INFO_SC) {}
	CSUseTuHaoJinReq::CSUseTuHaoJinReq() : header(MT_TUHAOJIN_USE_REQ_CS) {}
	CSTuhaojinUpLevelReq::CSTuhaojinUpLevelReq() : header(MT_TUHAOJIN_UP_LEVEL_REQ_CS) {}
	SCBigChatFaceAllInfo::SCBigChatFaceAllInfo() : header(MT_BIG_CHATFACE_ALL_INFO_SC) {}
	CSBigChatFaceUpLevelReq::CSBigChatFaceUpLevelReq() : header(MT_BIG_CHATFACE_UP_LEVEL_REQ_CS) {}

	//////////////////////////////////////////////////////////////////////////
	SCShenzhouWeapondAllInfo::SCShenzhouWeapondAllInfo() : header(MT_SHENZHOU_WEAPON_ALL_INFO_SC) {}
	SCShenzhouWeapondGatherInfo::SCShenzhouWeapondGatherInfo() : header(MT_SHENZHOU_WEAPON_GATHER_INFO_SC) {}
	CSSHenzhouWeaponOperaReq::CSSHenzhouWeaponOperaReq() : header(MT_SHENZHOU_WEAPON_OPERA_REQ_CS) {}
	SCShenzhouBoxInfo::SCShenzhouBoxInfo() : header(MT_SHEN_ZHOU_BOX_INFO_SC) {}
	CSSHenzhouWeaponOneKeyIdentifyReq::CSSHenzhouWeaponOneKeyIdentifyReq() : header(MT_SHEN_ZHOU_ONE_KEY_IDENTIFY_CS) {}
	CSShenzhouHunyinResolveReq::CSShenzhouHunyinResolveReq() : header(MT_SHEN_ZHOU_HUNYIN_RESOLVE_CS) {}

	//////////////////////////////////////////////////////////////////////////
	SCRAMiJingXunBaoInfo::SCRAMiJingXunBaoInfo() : header(MT_RA_MIJINGXUNBAO_INFO_SC) {}  
	SCRAMiJingXunBaoTaoResultInfo::SCRAMiJingXunBaoTaoResultInfo() : header(MT_RA_MIJINGXUNBAO_TAO_REWARD_INFO_SC) {}

	SCRAMiJingXunBao2Info::SCRAMiJingXunBao2Info() : header(MT_RA_MIJINGXUNBAO2_INFO_SC) {}  
	SCRAMiJingXunBao2TaoResultInfo::SCRAMiJingXunBao2TaoResultInfo() : header(MT_RA_MIJINGXUNBAO2_TAO_REWARD_INFO_SC) {}

	//////////////////////////////////////////////////////////////////////////
	SCRAMoneyTreeInfo::SCRAMoneyTreeInfo() : header(MT_RA_MONEY_TREE_INFO_SC) {}
	SCRAMoneyTreeChouResultInfo::SCRAMoneyTreeChouResultInfo() : header(MT_RA_MONEY_TREE_CHOU_REWARD_INFO_SC) {}

	SCRAMoneyTree2Info::SCRAMoneyTree2Info() : header(MT_RA_MONEY_TREE2_INFO_SC) {}
	SCRAMoneyTree2ChouResultInfo::SCRAMoneyTree2ChouResultInfo() : header(MT_RA_MONEY_TREE2_CHOU_REWARD_INFO_SC) {}

	//////////////////////////////////////////////////////////////////////////
	SCBabyInfo::SCBabyInfo() : header(MT_BABY_INFO_SC) {}
	SCBabyBackInfo::SCBabyBackInfo() : header(MT_BABY_ROUTE_SC) {}
	SCBabyAllInfo::SCBabyAllInfo() : header(MT_BABY_ALL_INFO_SC) {}
	SCBabySpiritInfo::SCBabySpiritInfo() : header(MT_BABY_SPIRIT_INFO_SC) {}
	CSBabyOperaReq::CSBabyOperaReq() : header(MT_BABY_OPERA_REQ_CS) {}
	CSBabyUpgradeReq::CSBabyUpgradeReq() : header(MT_BABY_UPGRADE_REQ_CS){}
	CSBabyRenameReq::CSBabyRenameReq() : header(MT_BABY_RENAME_REQ_CS) {}

	//////////////////////////////////////////////////////////////////////////
	SCRAKingDrawInfo::SCRAKingDrawInfo() : header(MT_RA_KING_DRAW_INFO_SC) {}
	SCRAKingDrawMultiReward::SCRAKingDrawMultiReward() : header(MT_RA_KING_DRAW_PLAY_TIMES_REWARD_SC) {}

	//////////////////////////////////////////////////////////////////////////
	SCRATotalCharge3Info::SCRATotalCharge3Info() : header(MT_RA_TOTAL_CHARGE_3_INFO_SC) {}

	//////////////////////////////////////////////////////////////////////////
	SCMolongInfo::SCMolongInfo() : header(MT_MOLONG_INFO_SC) {}

	/////////////////////////////////////////////////////////////////////////////////////////
	SCPetViewChangeNotify::SCPetViewChangeNotify() : header(MT_PET_RENAME_NOTIFY_SC) {}
	SCPetBackpackInfo::SCPetBackpackInfo() : header(MT_PET_BACKPACK_INFO_SC) {}
	SCPetInfo::SCPetInfo() : header(MT_PET_INFO_SC) {}
	SCPetChouResult::SCPetChouResult() : header(MT_PET_CHOU_RESULT_SC) {}
	SCFightOutPetInfo::SCFightOutPetInfo() : header(MT_PET_FIGHT_OUT_INFO_SC) {}

	CSPetOperaReq::CSPetOperaReq() : header(MT_PET_OPERA_REQ_CS) {}
	CSPetUpgradeReq::CSPetUpgradeReq() : header(MT_PET_UPGRADE_REQ_CS) {}
	CSPetUseSpecialImg::CSPetUseSpecialImg() : header(MT_PET_USE_SPECIAL_IMG_CS) {}

	SCPetQinmiAllInfo::SCPetQinmiAllInfo() : header(MT_PET_QINMI_ALL_INFO_SC) {}
	SCPetQinmiSingleInfo::SCPetQinmiSingleInfo() : header(MT_PET_QINMI_SINGLE_INFO_SC) {}
	SCPetFoodMarcketChouResult::SCPetFoodMarcketChouResult() : header(MT_PET_FOOD_MARKET_CHOU_RESULT_SC) {}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCChineseZodiacAllInfo::SCChineseZodiacAllInfo() : header(MT_CHINESE_ZODIAC_ALL_INFO_SC) {}
	SCChineseZodiacEquipInfo::SCChineseZodiacEquipInfo() : header(MT_CHINESE_ZODIAC_EQUIP_INFO_SC) {}
	CSChineseZodiacPromoteXingHun::CSChineseZodiacPromoteXingHun() : header(MT_CHINESEZODIAC_PROMOTE_XINGHUN_LEVEL_CS) {}
	CSChineseZodiacPromoteEquip::CSChineseZodiacPromoteEquip() : header(MT_CHINESEZODIAC_PROMOTE_EQUIP_LEVEL_CS) {}
	CSChineseZodiacPromote::CSChineseZodiacPromote() : header(MT_CHINESEZODIAC_PROMOTE_LEVEL_CS) {}
	CSTianxiangReq::CSTianxiangReq() : header(MT_CHINESEZODIAC_TIANXIANG_REQ_CS) {}
	SCTianXiangAllInfo::SCTianXiangAllInfo() : header(MT_CHINESEZODIAC_TIANXIANG_INFO_SC) {}
	SCTianXiangSignBead::SCTianXiangSignBead() : header(MT_CHINESEZODIAC_TIANXIANG_BEAD_SC) {}
	SCTianXiangCombind::SCTianXiangCombind() : header(MT_CHINESEZODIAC_TIANXIANG_COMBIND_SC) {}
	SCGunGunLeInfo::SCGunGunLeInfo() : header(MT_CHINESEZODIAC_GUNGUN_LE_SC) {}
	SCMijiSingleChange::SCMijiSingleChange() : header(MT_CHINESEZODIAC_SINGLE_CHANGE_SC) {}
	SCMijiCombineSucc::SCMijiCombineSucc() : header(MT_CHINESEZODIAC_COMBINE_SUCC_SC) {}
	SCXinglingInfo::SCXinglingInfo() : header(MT_CHINESEZODIAC_XINGLING_INFO_SC) {}
	CSChineseZodiacTakeOffEquip::CSChineseZodiacTakeOffEquip() : header(MT_CHINESEZODIAC_TAKE_OFF_EQUIP_CS){}
	CSChineseZodiacRecycle::CSChineseZodiacRecycle() : header(MT_CHINESEZODIAC_RECYCLE_CS){}
	//////////////////////////////////////////////////////////////////////////////////////////
	SCRASpecialAppearancePassiveInfo::SCRASpecialAppearancePassiveInfo() : header(MT_RA_SPECIAL_APPEARACE_PASSIVE_RANK_INFO_SC) {}
	SCRASpecialAppearanceInfo::SCRASpecialAppearanceInfo() : header(MT_RA_SPECIAL_APPEARACE_RANK_INFO_SC) {}

	//////////////////////////////////////////////////////////////////////////////////////////
	CSRATotalCharge4Info::CSRATotalCharge4Info() : header(MT_RA_TOTAL_CHARGE_4_INFO_CS) {}
	SCRATotalCharge4Info::SCRATotalCharge4Info() : header(MT_RA_TOTAL_CHARGE_4_INFO_SC) {}

	//////////////////////////////////////////////////////////////////////////////////////////
	SCRAMineAllInfo::SCRAMineAllInfo() : header(MT_RA_MINE_ALL_INFO_SC) {}

	//////////////////////////////////////////////////////////////////////////////////////////
	SCOtherCapabilityInfo::SCOtherCapabilityInfo() : header(MT_OTHERCAPABILITY_INFO_SC) {}
	SCRAGuaGuaInfo::SCRAGuaGuaInfo() : header(MT_RA_GUAGUA_INFO_SC) {}
	SCRAGuaGuaMultiReward::SCRAGuaGuaMultiReward() : header(MT_RA_GUAGUA_PLAY_TIMES_REWARD_SC) {}

	SCRATianMingDivinationInfo::SCRATianMingDivinationInfo() : header(MT_RA_TIAN_MING_DIVINATION_INFO_SC) {}
	SCTianMingDivinationActivityStartChouResult::SCTianMingDivinationActivityStartChouResult() : header(MT_RA_TIANMING_DIVINATION_START_CHOU_RESULT_SC) {}

	SCHuaShenAllInfo::SCHuaShenAllInfo() : header(MT_HUASHEN_ALL_INFO_SC) {}
	CSHuaShenOperaReq::CSHuaShenOperaReq() : header(MT_HUASHEN_OPERA_REQ_CS) {}
	SCHuaShenSpiritInfo::SCHuaShenSpiritInfo() : header(MT_HUASHEN_SPIRIT_INFO_SC) {}

	SCRAFanFanAllInfo::SCRAFanFanAllInfo() : header(MT_RA_FANFAN_ALL_INFO_SC) {}
	SCRAFanFanWordExchangeResult::SCRAFanFanWordExchangeResult() : header(MT_RA_FANFAN_WORD_EXCHANGE_RESULT_SC) {}
	SCRAContinueChongzhiInfo::SCRAContinueChongzhiInfo() : header(MT_RA_CONTINUE_CHONGZHI_INFO_SC) {}
	SCRAContinueChongzhiInfoChu::SCRAContinueChongzhiInfoChu() : header(MT_RA_CONTINUE_CHONGZHI_INFO_CHU_SC) {}
	SCRAContinueChongzhiInfoGao::SCRAContinueChongzhiInfoGao() : header(MT_RA_CONTINUE_CHONGZHI_INFO_GAO_SC) {}
	SCRAContinueChongzhiInfo2::SCRAContinueChongzhiInfo2() : header(MT_RA_CONTINUE_CHONGZHI_2_INFO_GAO_SC) {}
	SCRAContinueConsumeInfo::SCRAContinueConsumeInfo() : header(MT_RA_CONTINUE_CONSUME_INFO_SC) {}
	SCRAArmyDayInfo::SCRAArmyDayInfo() : header(MT_RA_ARMY_DAY_INFO_SC) {}

	CSPastureSpiritOperaReq::CSPastureSpiritOperaReq() : header(MT_PASTURE_SPIRIT_OPERA_REQ_CS) {}
	SCPastureSpiritAllInfo::SCPastureSpiritAllInfo() : header(MT_PASTURE_SPIRIT_ALL_INFO_SC) {}
	SCPastureSpiritSinglelInfo::SCPastureSpiritSinglelInfo() : header(MT_PASTURE_SPIRIT_SINGLE_INFO_SC) {}
	SCPastureSpiritLuckyDrawResult::SCPastureSpiritLuckyDrawResult() : header(MT_PASTURE_SPIRIT_LUCKY_DRAW_RESULT_SC) {}

	CSMultiMountOperaReq::CSMultiMountOperaReq() : header(MT_MULTI_MOUNT_OPERA_REQ_CS) {}
	SCMultiMountAllInfo::SCMultiMountAllInfo() : header(MT_MULTI_MOUNT_ALL_INFO_SC){}
	SCMultiMountChangeNotify::SCMultiMountChangeNotify() : header(MT_MULTI_MOUNT_CHANGE_NOTIFY_SC){}
	SCMultiMountNotifyArea::SCMultiMountNotifyArea() : header(MT_MULTI_MOUNT_NOTIFY_AREA_SC){}

	SCRACirculationChongzhiInfo::SCRACirculationChongzhiInfo() : header(MT_RA_CIRCULATION_CHONGZHI_INFO_SC) {}

	CSPersonalizeWindowOperaReq::CSPersonalizeWindowOperaReq() : header(MT_PERSONALIZE_OPERA_REQ_CS) {}
	SCBubbleWindowInfo::SCBubbleWindowInfo() : header(MT_PERSONALIZE_WINDOW_ALL_INFO_SC) {}
	SCAvatarWindowInfo::SCAvatarWindowInfo() : header(MT_PERSONALIZE_AVATAR_WINDOW_ALL_INFO_SC) {}

	CSMagicCardOperaReq::CSMagicCardOperaReq() : header(MT_MAGIC_CARD_OPERA_REQ_CS) {}
	SCMagicCardAllInfo::SCMagicCardAllInfo() : header(MT_MAGIC_CARD_ALL_INFO_SC) {}
	SCMagicCardChouCardResult::SCMagicCardChouCardResult() : header(MT_MAGIC_CARD_CHOU_RESULT_SC){}

	SCRAExtremeLuckyAllInfo::SCRAExtremeLuckyAllInfo() : header(MT_RA_EXTREME_LUCKY_ALL_INFO_SC) {}
	SCRAExtremeLuckySingleInfo::SCRAExtremeLuckySingleInfo() : header(MT_RA_EXTREME_LUCKY_SINGLE_INFO_SC) {}

	CSWushangEquipOpearReq::CSWushangEquipOpearReq() : header(MT_WUSHANG_EQUIP_REQ_CS) {}
	SCWushangEquipAllInfo::SCWushangEquipAllInfo() : header(MT_WUSHANG_EQUIP_INFO_SC) {}

	SCRAZhenbaoge2Info::SCRAZhenbaoge2Info() : header(MT_RA_ZHENBAOGE2_INFO_SC) {}

	SCRAPromotingPositionAllInfo::SCRAPromotingPositionAllInfo() : header(MT_RA_PROMOTING_POSITION_ALL_INFO_SC) {}
	SCRAPromotingPositionRewardInfo::SCRAPromotingPositionRewardInfo() : header(MT_RA_PROMOTING_POSITION_REWARD_INFO_SC) {}

	CSMitamaOperaReq::CSMitamaOperaReq() : header(MT_MITAMA_OPERA_REQ_CS) {}
	SCMitamaAllInfo::SCMitamaAllInfo() : header(MT_MITAMA_ALL_MITAMA_INFO_SC) {}
	SCMitamaSingleInfo::SCMitamaSingleInfo() : header(MT_MITAMA_SINGLE_MITAMA_INFO_SC) {}
	SCMitamaHotSpringScore::SCMitamaHotSpringScore() : header(MT_MITAMA_HOT_SPRING_SCORE_SC) {}

	CSFairyTreeOperaReq::CSFairyTreeOperaReq() : header(MT_FAIRY_TREE_OPERA_REQ_CS) {}
	SCFairyTreeAllInfo::SCFairyTreeAllInfo() : header(MT_FAIRY_TREE_ALL_INFO_SC) {}
	SCFairyTreeDrawRewardInfo::SCFairyTreeDrawRewardInfo() : header(MT_FAIRY_TREE_DRAW_REWARD_INFO_SC) {}

	SCMagicEquipmentAllInfo::SCMagicEquipmentAllInfo() : header(MT_MAGIC_EQUIPMENT_ALL_INFO_SC) {}
	SCMagicEquipmentChangeNotify::SCMagicEquipmentChangeNotify() : header(MT_MAGIC_EQUIPMENT_CHANGE_NOTIFY_SC) {}

	SCRABlackMarketAllInfo::SCRABlackMarketAllInfo() : header(MT_RA_BLACK_MARKET_ALL_INFO_SC) {}
	SCRATreasuresMallAllInfo::SCRATreasuresMallAllInfo() : header(MT_RA_TREASURES_MALL_ALL_INFO_SC) {}

	SCAllXunZhangInfo::SCAllXunZhangInfo() : header(MT_XUNZHANG_ALL_INFO_SC) {}
	CSXunZhangUplevelReq::CSXunZhangUplevelReq() : header(MT_XUNZHANG_UPLEVEL_REQ_CS) {}

	SCAllZhiBaoInfo::SCAllZhiBaoInfo() : header(MT_ZHIBAO_ALL_INFO_SC) {}
	CSZhiBaoUplevel::CSZhiBaoUplevel() : header(MT_ZHIBAO_UPLEVEL_CS) {}
	CSZhiBaoUseImage::CSZhiBaoUseImage() : header(MT_ZHIBAO_USEIMAGE_CS) {}
	CSZhiBaoHuanhua::CSZhiBaoHuanhua() : header(MT_ZHIBAO_HUANHUA_CS) {}
	SCZhiBaoAttack::SCZhiBaoAttack() : header(MT_ZHIBAO_ATTACK_SC) {}

	CSCrossTeamFBOption::CSCrossTeamFBOption() : header(MT_CROSS_TEAM_FB_OPTION_CS) {}
	SCCrossTeamFBSelfInfo::SCCrossTeamFBSelfInfo() : header(MT_CROSS_TEAM_FB_SELF_INFO_SC) {}
	SCRAOpenServerInfo::SCRAOpenServerInfo() : header(MT_RA_OPEN_SERVER_INFO_SC) {}
	SCRAOpenServerUpgradeInfo::SCRAOpenServerUpgradeInfo() : header(MT_RA_OPEN_SERVER_UPGRADE_INFO_SC) {}
	SCRAOpenServerRankInfo::SCRAOpenServerRankInfo() : header(MT_RA_OPEN_SERVER_RANK_INFO_SC) {}
	SCRAOpenServerBossInfo::SCRAOpenServerBossInfo() : header(MT_RA_OPEN_SERVER_BOSS_INFO_SC) {}
	SCRAOpenServerBattleInfo::SCRAOpenServerBattleInfo() : header(MT_RA_OPEN_SERVER_BATTEL_INFO_SC) {}
	SCBaiBeiFanLiInfo::SCBaiBeiFanLiInfo() : header(MT_BAI_BEI_FAN_LI_INFO_SC) {}
	CSBaiBeiFanLiBuy::CSBaiBeiFanLiBuy() : header(MT_BAI_BEI_FAN_LI_BUY_CS) {}

	CSRAOpenGameGiftShopBuy::CSRAOpenGameGiftShopBuy() : header(MT_RA_OPEN_BUY_GAME_GIFT_SHOP_INFO_CS) {}
	CSRAOpenGameGiftShopBuyInfoReq::CSRAOpenGameGiftShopBuyInfoReq() : header(MT_RA_OPEN_GAME_GIFT_SHOP_INFO_CS) {}
	SCRAOpenGameGiftShopBuyInfo::SCRAOpenGameGiftShopBuyInfo() : header(MT_RA_OPEN_GAME_GIFT_SHOP_INFO_SC) {}

	SCCrossShuijingPlayerInfo::SCCrossShuijingPlayerInfo() : header(MT_CROSS_SHUIJING_PLAYER_INFO_SC) {}
	CSCrossShuijingBuyBuff::CSCrossShuijingBuyBuff() : header(MT_CROSS_SHUIJING_BUY_BUFF_CS) {}
	SCShuijingPlayerInfo::SCShuijingPlayerInfo() : header(MT_SHUIJING_PLAYER_INFO_SC) {}
	SCShuijingSceneInfo::SCShuijingSceneInfo() : header(MT_SHUIJING_SCENE_INFO_SC) {}
	SCShuijingTaskInfo::SCShuijingTaskInfo() : header(MT_SHUIJING_TASK_INFO_SC) {}
	CSShuijingBuyBuff::CSShuijingBuyBuff() : header(MT_SHUIJING_BUY_BUFF_CS) {}
	CSShuijingFetchTaskReward::CSShuijingFetchTaskReward() : header(MT_SHUIJING_FETCH_TASK_REWARD_CS) {}
	SCNoticeGatherBuffInfo::SCNoticeGatherBuffInfo() : header(MT_SHUIJING_GATHER_WUDI_INFO) {}

	CSFetchMagicalPreciousRewardReq::CSFetchMagicalPreciousRewardReq() : header(MT_MAGICAL_PRECIOUS_FETCH_REWARD_CS) {}
	SCSendMagicalPreciousAllInfo::SCSendMagicalPreciousAllInfo() : header(MT_MAGICAL_PRECIOUS_ALL_INFO_REQ_SC) {}
	SCSendMagicalPreciousCurChapterInfo::SCSendMagicalPreciousCurChapterInfo() : header(MT_MAGICAL_PRECIOUS_CUR_CHAPTER_INFO_REQ_SC) {}
	SCMagicalPreciousConditionParamChange::SCMagicalPreciousConditionParamChange() : header(MT_MAGICAL_PRECIOUS_CONDITION_CHANGE_SC) {}

	CSRAExpRefineReq::CSRAExpRefineReq() : header(MT_RA_EXP_REFIN_REQ_CS) {}
	SCRAExpRefineInfo::SCRAExpRefineInfo() : header(MT_RA_EXP_REFINE_INFO_SC) {}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSLittlePetREQ::CSLittlePetREQ() : header(MT_LITTLE_PET_OPERA_REQ_CS){}
	SCLittlePetAllInfo::SCLittlePetAllInfo() : header(MT_LITTLE_PET_ALL_INFO_SC){}
	SCLittlePetSingleInfo::SCLittlePetSingleInfo() : header(MT_LITTLE_PET_SINGLE_INFO_SC){}
	SCLittlePetChangeInfo::SCLittlePetChangeInfo() : header(MT_LITTLE_PET_PEI_CHANGE_INFO_SC){}
	SCLittlePetChouRewardList::SCLittlePetChouRewardList() : header(MT_LITTLE_PET_PEI_CHOUJIANG_INFO_SC){}
	SCLittlePetNotifyInfo::SCLittlePetNotifyInfo() : header(MT_LITTLE_PET_NOTIFY_INFO_SC){}
	SCLittlePetFriendInfo::SCLittlePetFriendInfo() : header(MT_LITTLE_PET_FRIEND_INFO_SC){}
	SCLittlePetUsingImg::SCLittlePetUsingImg() : header(MT_LITTLE_PET_USING_IMG_SC){}
	SCLittlePetFriendPetListInfo::SCLittlePetFriendPetListInfo() : header(MT_LITTLE_PET_FRIEND_PET_LIST_SC){}
	SCLittlePetInteractLog::SCLittlePetInteractLog() : header(MT_LITTLE_PET_INTERACT_LOG_SC){}
	CSLittlePetRename::CSLittlePetRename() : header(MT_LITTLE_PET_RENAME_CS){}
	SCLittlePetRename::SCLittlePetRename() : header(MT_LITTLE_PET_RENAME_SC){}
	SCLittlePetWalk::SCLittlePetWalk() : header(MT_LITTLE_PET_WALK_SC) {}

	SCBabyWalk::SCBabyWalk() : header(MT_BABY_TYPE_WALK_SC) {}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSLoveTreeWaterReq::CSLoveTreeWaterReq() : header(MT_LOVE_TREE_WATER_REQ_CS){}
	CSLoveTreeInfoReq::CSLoveTreeInfoReq() : header(MT_LOVE_TREE_INFO_REQ_CS){}
	SCLoveTreeInfo::SCLoveTreeInfo() : header(MT_LOVE_TREE_SEND_INFO_SC){}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCFriendGiftAllInfo::SCFriendGiftAllInfo() : header(MT_FRIEND_GIFT_INFO_SC) {}
	CSFriendSongGift::CSFriendSongGift() : header(MT_FRIEND_GIFT_SONG_GIFT_CS){}
	SCFriendGiftShouNotice::SCFriendGiftShouNotice() : header(MT_FRIEND_GIFT_SHOU_NOTICE_CS){}
	SCFriendHeliNotice::SCFriendHeliNotice() : header(MT_FRIEND_HELI_NOTICE_SC){}
	CSFriendHeliSendReq::CSFriendHeliSendReq() : header(MT_FRIEND_HELI_SEND_REQ_CS){}
	SCFriendHeliSend::SCFriendHeliSend() : header(MT_FRIEND_HELI_SEND_SC){}
	CSGiveItemOpera::CSGiveItemOpera() : header(MT_GIVE_ITEM_OPERA_CS) {}
	CSGiveItemReq::CSGiveItemReq() : header(MT_GIVE_ITEM_REQ_CS) {}
	SCGiveItemRecord::SCGiveItemRecord() : header(MT_GIVE_ITEM_RECORD_SC) {}

	CSFriendExpBottleOP::CSFriendExpBottleOP() : header(MT_FRIEND_EXP_BOTTLE_OPERATE_REQ_CS){}
	SCFriendExpBottleAddExp::SCFriendExpBottleAddExp() : header(MT_FRIEND_EXP_BOTTLE_ADD_EXP){}
	
	CSSkipReq::CSSkipReq() : header(MT_SKIP_REQ_CS) {}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSGetAllTuodanInfo::CSGetAllTuodanInfo() : header(MT_FRIEND_TUODAN_ALL_INFO_CS) {}
	SCAllTuodanInfo::SCAllTuodanInfo() : header(MT_FRIEND_TUODAN_ALL_INFO_SC){}
	CSTuodanREQ::CSTuodanREQ() : header(MT_FRIEND_TUODAN_OPERA_REQ_CS){}
	SCSingleTuodanInfo::SCSingleTuodanInfo() : header(MT_FRIEND_TUODAN_SINGLE_INFO_SC){}
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRARedEnvelopeGiftInfo::SCRARedEnvelopeGiftInfo() : header(MT_RA_RED_ENVELOPE_INFO){}
	
	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAMarryMeAllInfo::SCRAMarryMeAllInfo() : header(MT_RA_MARRY_ME_COUPLE_INFO_SC) {}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSChongZhi7DayFetchReward::CSChongZhi7DayFetchReward() : header(MT_CHONGZHI_7DAY_FETCH_REWARD_CS) {}

	CSShengeSystemReq::CSShengeSystemReq() : header(MT_SHENGE_SYSTEM_REQ_CS) {}
	SCShengeSystemBagInfo::SCShengeSystemBagInfo() : header(MT_SHENGE_SYSTEM_INFO_SC) {}
	SCShengeZhangkongInfo::SCShengeZhangkongInfo() : header(MT_SHENGE_SYSTEM_ZHANGKONG_INFO_SC) {}
	SCZhangkongSingleChange::SCZhangkongSingleChange() : header(MT_SHENGE_SYSTEM_ZHANGKONG_SIGN_INFO_SC) {}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSTuituFbOperaReq::CSTuituFbOperaReq() : header(MT_TUITU_FB_OPERA_REQ_CS) {}
	SCTuituFbInfo::SCTuituFbInfo() : header(MT_TUITU_FB_INFO_SC) {}
	SCTuituFbResultInfo::SCTuituFbResultInfo() : header(MT_TUITU_FB_RESULT_SC) {}
	SCTuituFbSingleInfo::SCTuituFbSingleInfo() : header(MT_TUITU_FB_SINGLE_INFO_SC) {}
	SCTuituFbFetchResultInfo::SCTuituFbFetchResultInfo() : header(MT_TUITUI_FB_FETCH_RESULT_SC) {}

	CSBabyBossOperate::CSBabyBossOperate() : header(MT_BABY_BOSS_OPERATE_CS) {}
	SCBabyBossRoleInfo::SCBabyBossRoleInfo() : header(MT_BABY_BOSS_ROLE_INFO_SC) {}
	SCAllBabyBossInfo::SCAllBabyBossInfo() : header(MT_ALL_BABY_BOSS_INFO_SC) {}
	SCSingleBabyBossInfo::SCSingleBabyBossInfo() : header(MT_SINGLE_BABY_BOSS_INFO_SC) {}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSGoldenPigOperateReq::CSGoldenPigOperateReq() : header(MT_GOLDEN_PIG_OPERATE_REQ_CS) {}
	SCGoldenPigSendInfo::SCGoldenPigSendInfo() : header(MT_GOLDEN_PIG_SENG_INFO_SC) {}
	SCGoldenPigBossState::SCGoldenPigBossState() : header(MT_GOLDEN_PIG_BOSS_STATE_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCOpenServerInvestInfo::SCOpenServerInvestInfo() : header(MT_RA_OPEN_SERVER_INVEST_INFO_SC) {}
	SCRASingleChongZhiInfo::SCRASingleChongZhiInfo() : header(MT_RA_SINGLE_CHONGZHI_INFO_SC) {}
	SCRAXianyuanTreasInfo::SCRAXianyuanTreasInfo() : header(MT_RA_XIANYUAN_TREAS_INFO_SC) {}
	SCRARushBuyingAllInfo::SCRARushBuyingAllInfo() : header(MT_RA_RUSH_BUYING_ALL_INFO_SC) {}
	SCRAMapHuntAllInfo::SCRAMapHuntAllInfo() : header(MT_RA_MAP_HUNT_ALL_INFO_SC) {}
	SCRALightTowerExploreInfo::SCRALightTowerExploreInfo() : header(MT_RA_LIGHT_TOWER_ALL_INFO_SC) {}
	SCRALightTowerExploreDrawInfo::SCRALightTowerExploreDrawInfo() : header(MT_RA_LIGHT_TOWER_DRAW_INFO_SC) {}

	////////////////////////////////////////////////////////////////////////////////////////////////////
	CSShenshouOperaReq::CSShenshouOperaReq() : header(MT_SHENSHOU_OPERA_REQ_CS) {}
	CSSHenshouReqStrength::CSSHenshouReqStrength() : header(MT_SHENSHOU_REQ_UPGRADE_EQUIP_CS) {}
	SCShenshouBackpackInfo::SCShenshouBackpackInfo() : header(MT_SHENSHOU_BACKPACK_INFO_SC) {}
	SCShenshouListInfo::SCShenshouListInfo() : header(MT_SHENSHOU_SHENSHOU_INFO_SC) {}
	SCShenshouBaseInfo::SCShenshouBaseInfo() : header(MT_SHENSHOU_SHENSHOU_BASE_INFO_SC) {}
	SCShenshouHuanlingListInfo::SCShenshouHuanlingListInfo() : header(MT_SHENSHOU_HUANLING_LIST_INFO_SC) {}
	SCShenshouHuanlingDrawInfo::SCShenshouHuanlingDrawInfo() : header(MT_SHENSHOU_HUANLING_DRAW_RESULT_SC) {}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	SCImgFulingInfo::SCImgFulingInfo() : header(MT_IMG_FULING_INFO_SC) {}
	CSImgFulingOperate::CSImgFulingOperate() : header(MT_IMG_FULING_OPERA_CS) {}
	SCShengeShenquAllInfo::SCShengeShenquAllInfo() : header(MT_SHENGE_SYSTEM_SHENQU_XILIAN_ALL_INFO_SC) {}
	SCShengeShenquInfo::SCShengeShenquInfo() : header(MT_SHENGE_SYSTEM_SHENQU_XILIAN_INFO_SC) {}

	SCRuneSystemZhulingRandResultInfo::SCRuneSystemZhulingRandResultInfo() : header(MT_RUNE_SYSTEM_ZHULING_NOTIFY_INFO_SC) {}
	SCRuneSystemZhulingAllInfo::SCRuneSystemZhulingAllInfo() : header(MT_RUNE_SYSTEM_ZHULING_ALL_INFO_SC) {}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAMagicShopAllInfo::SCRAMagicShopAllInfo() : header(MT_RA_MAGIC_SHOP_ALL_INFO_SC) {}

	/////////////////////////////////////////////////////////////////////////////////////////////////////
	SCShenYinScoreInfo::SCShenYinScoreInfo() : header(MT_SHEN_YIN_SCORE_INFO_SC) {}
	SCShenYinBagInfo::SCShenYinBagInfo() : header(MT_SHEN_YIN_BAG_INFO_SC) {}
	SCShenYinShopInfo::SCShenYinShopInfo() : header(MT_SHEN_YIN_SHOP_INFO_SC) {}
	SCShenYinLieMingBagInfo::SCShenYinLieMingBagInfo() : header(MT_SHEN_YIN_LIEHUN_POOL_INFO_SC) {}

	SCPlantingTreeRankInfo::SCPlantingTreeRankInfo() : header(MT_RA_PLANTING_TREE_RANK_INFO) {}
	SCPlantingTreeTreeInfo::SCPlantingTreeTreeInfo() : header(MT_RA_PLANTING_TREE_TREE_INFO) {}
	SCPlantingTreeMiniMapInfo::SCPlantingTreeMiniMapInfo() : header(MT_RA_PLANTING_TREE_MINI_MAP_INFO) {}

	SCRAFishAllinfo::SCRAFishAllinfo() : header(MT_RA_FISHING_ALL_INFO_SC) {}
	SCRAFishStatusInfo::SCRAFishStatusInfo() : header(MT_RA_FISHING_STATUS_SC) {}

	SCRAStealCrystalDalatanInfo::SCRAStealCrystalDalatanInfo() : header(MT_RA_CRYSTAL_DALARAN_INFO_SC) {}

	//元素之心
	CSElementHeartReq::CSElementHeartReq() : header(MT_ELEMENT_HEART_REQ_CS) {}
	SCElementHeartInfo::SCElementHeartInfo() : header(MT_ELEMENT_HEART_INFO_SC) {}
	SCElementTextureInfo::SCElementTextureInfo() : header(MT_ELEMENT_HEART_CHARM_INFO_SC) {}
	SCElemnetHeartSingleCharmInfo::SCElemnetHeartSingleCharmInfo() : header(MT_ELEMENT_HEART_SINGLE_CHARM_INFO_SC) {}
	SCElementHeartChouRewardListInfo::SCElementHeartChouRewardListInfo() : header(MT_ELEMENT_HEART_REWARD_INFO_SC) {}
	SCElementShopInfo::SCElementShopInfo() : header(MT_ELEMENT_SHOP_INFO_SC) {}
	SCElementProductListInfo::SCElementProductListInfo() : header(MT_ELEMENT_HEART_PRODUCT_INFO_SC) {}
	SCElementXiLianSingleInfo::SCElementXiLianSingleInfo() : header(MT_ELEMENT_XILIAN_SINGLE_INFO_SC) {}
	SCElementXiLianAllInfo::SCElementXiLianAllInfo() : header(MT_ELEMENT_XILIAN_ALL_INFO_SC) {}

	SCRaCrazyRebateChongInfo::SCRaCrazyRebateChongInfo() : header(MT_RA_CRAZY_REBATE_CHONGZHI_INFO) {}
	SCLoveDailyInfo::SCLoveDailyInfo() : header(MT_RA_LOVE_DAILY_CHONGZHI_INFO) {}

	SCLoveDaily2Info::SCLoveDaily2Info():header(MT_RA_LOVE_DAILY2_CHONGZHI_INFO) {}

	CSShenYinOneKeyRecyleReq::CSShenYinOneKeyRecyleReq() : header(MT_SHEN_YIN_ONE_KEY_RECYLE_CS) {}

	SCUpgradeRushInfo::SCUpgradeRushInfo() : header(MT_UPGRADE_RUSH_INFO_SC) {}

	CSGetDropLog::CSGetDropLog() : header(MT_GET_DROP_LOG_CS) {}
	SCDropLogRet::SCDropLogRet() : header(MT_DROP_LOG_RET_SC) {}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	CSGetShengxingzhuliInfoReq::CSGetShengxingzhuliInfoReq() : header(MT_SHENGXINGZHULI_INFO_REQ_CS) {}
	SCGetShengxingzhuliInfoAck::SCGetShengxingzhuliInfoAck() : header(MT_SHENGXINGZHULI_INFO_ACK_SC) {}

	CSGetShengxingzhuliRewardReq::CSGetShengxingzhuliRewardReq() : header(MT_SHENGXINGZHULI_GET_REWARD_REQ_CS) {}
	SCGetShengxingzhuliRewardAck::SCGetShengxingzhuliRewardAck() : header(MT_SHENGXINGZHULI_GET_REWARD_ACK_SC) {}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


	CSFairyBuddhaCardActivateReq::CSFairyBuddhaCardActivateReq() : header(MT_FAIRY_BUDDHA_CARD_REQ_CS) {}
	SCFairyBuddhaCardActivateInfo::SCFairyBuddhaCardActivateInfo() : header(MT_FAIRY_BUDDHA_CARD_INFO_SC) {}
	CSFairyBuddhaCardGoldBindReq::CSFairyBuddhaCardGoldBindReq() : header(MT_FAIRY_BUDDHA_CARD_GOLD_REQ_CS) {}
	////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSGuildQuestionEnterReq::CSGuildQuestionEnterReq() : header(MT_GUILD_QUESTION_ENTER_REQ_CS) {}
	SCGuildQuestionPlayerInfo::SCGuildQuestionPlayerInfo() : header(MT_GUILD_QUESTION_PLAYER_INFO_SC) {}
	SCGuildQuestionGuildRankInfo::SCGuildQuestionGuildRankInfo() : header(MT_GUILD_QUESTION_GUILD_RANK_INFO_SC) {}
	SCGuildQuestionQuestionInfo::SCGuildQuestionQuestionInfo() : header(MT_GUILD_QUESTION_QUESTION_INFO_SC) {}


	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCTalentSkillTargetList::SCTalentSkillTargetList() : header(MT_TALENT_SKILL_TARGET_LIST) {}	
	CSRoleTelentOperate::CSRoleTelentOperate() : header(MT_TALENT_OPERATE_CS) {}
	SCRoleTelentInfo::SCRoleTelentInfo() : header(MT_TALENT_INFO_SC) {}
	
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSJinJieTalentOperaReqAll::CSJinJieTalentOperaReqAll() : header(MT_JINJIE_TALENT_INFO_REQ_CS) {}
	SCJinJieTalentAllInfo::SCJinJieTalentAllInfo() : header(MT_JINJIE_TALENT_ALL_INFO_SC) {}
	SCJinJieTalentChoujiangPage::SCJinJieTalentChoujiangPage() : header(MT_JINJIE_TALENT_CHOUJIANG_PAGE_INFO_SC) {}
	SCJinJieTalentUpdateSingleGrid::SCJinJieTalentUpdateSingleGrid() : header(MT_JINJIE_TALENT_UPDATE_GRID_SC) {}
	SCTalentAttentionSkillID::SCTalentAttentionSkillID() : header(MT_TALENT_ATTENTION_SKILL_SC) {}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCUpgradeInfo::SCUpgradeInfo() : header(MT_UPGRADE_SYS_INFO_SC) {}
	SCUpgradeAppeChange::SCUpgradeAppeChange() : header(MT_UPGRADE_SYS_APPE_CHANGE_SC) {}
	CSUpgradeOperaReq::CSUpgradeOperaReq() : header(MT_UPGRADE_SYS_OPERA_REQ_CS) {}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCYaoShiInfo::SCYaoShiInfo() : header(MT_YAOSHI_INFO_SC) {}
	SCYaoShiAppeChange::SCYaoShiAppeChange() : header(MT_YAOSHI_APPE_CHANGE_SC) {}
	CSYaoShiOperaReq::CSYaoShiOperaReq() : header(MT_YAOSHI_OPERA_REQ_CS) {}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCTouShiInfo::SCTouShiInfo() : header(MT_TOUSHI_INFO_SC) {}
	SCTouShiAppeChange::SCTouShiAppeChange() : header(MT_TOUSHI_APPE_CHANGE_SC) {}
	CSTouShiOperaReq::CSTouShiOperaReq() : header(MT_TOUSHI_OPERA_REQ_CS) {}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCQiLinBiInfo::SCQiLinBiInfo() : header(MT_QILINBI_INFO_SC) {}
	SCQiLinBiAppeChange::SCQiLinBiAppeChange() : header(MT_QILINBI_APPE_CHANGE_SC) {}
	CSQiLinBiOperaReq::CSQiLinBiOperaReq() : header(MT_QILINBI_OPERA_REQ_CS) {}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCMaskInfo::SCMaskInfo() : header(MT_MASK_INFO_SC) {}
	SCMaskAppeChange::SCMaskAppeChange() : header(MT_MASK_APPE_CHANGE_SC) {}
	CSMaskOperaReq::CSMaskOperaReq() : header(MT_MASK_OPERA_REQ_CS) {}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSZhuanzhiEquipOpe::CSZhuanzhiEquipOpe() : header(MT_ZHUANZHI_EQUIP_REQ_CS) {}
	SCZhuanzhiEquipInfo::SCZhuanzhiEquipInfo() : header(MT_ZHUANZHI_EQUIP_INFO_SC) {}
	SCZhuanzhiStoneInfo::SCZhuanzhiStoneInfo() : header(MT_ZHUANZHI_STONE_INFO_SC) {}
	SCZhuanzhiSuitInfo::SCZhuanzhiSuitInfo() : header(MT_ZHUANZHI_SUIT_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSFishingOperaReq::CSFishingOperaReq() : header(MT_FISHING_OPERA_REQ_CS) {}

	SCFishingUserInfo::SCFishingUserInfo() : header(MT_FISHING_USER_INFO_SC) {}
	SCFishingCheckEventResult::SCFishingCheckEventResult() : header(MT_FISHING_CHECK_EVENT_RESULT_SC) {}
	SCFishingGearUseResult::SCFishingGearUseResult() : header(MT_FISHING_GEAR_USE_RESULT_SC) {}
	SCFishingEventBigFish::SCFishingEventBigFish() : header(MT_FISHING_EVENT_BIG_FISH_SC) {}
	SCFishingTeamMemberInfo::SCFishingTeamMemberInfo() : header(MT_FISHING_TEAM_MEMBER_INFO_SC) {}
	SCFishingFishInfo::SCFishingFishInfo() : header(MT_FISHING_FISH_INFO_SC) {}
	SCFishingRandUserInfo::SCFishingRandUserInfo() : header(MT_FISHING_RAND_USER_INFO_SC) {}
	SCFishingScoreInfo::SCFishingScoreInfo() : header(MT_FISHING_SCORE_INFO_SC) {}
	SCFishingStealResult::SCFishingStealResult() : header(MT_FISHING_STEAL_RESULT_SC) {}
	SCFishingGetFishBrocast::SCFishingGetFishBrocast() : header(MT_FISHING_GET_FISH_BROCAST) {}
	SCFishingScoreRankList::SCFishingScoreRankList() : header(MT_FISHING_SCORE_RANK) {}
	SCFishingScoreStageInfo::SCFishingScoreStageInfo() : header(MT_FISHING_STAGE_SCORE_INFO) {}
	SCFishingNotifyStatusChange::SCFishingNotifyStatusChange() : header(MT_FISHING_NOTIFY_STATUS_CHANGE_SC) {}
	SCFishingStealInfo::SCFishingStealInfo() : header(MT_FISHING_STEAL_INFO_SC) {}
	SCFishingConfirmResult::SCFishingConfirmResult() : header(MT_FISHING_CONFIRM_RESULT_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCNightFightRoleInfo::SCNightFightRoleInfo() : header(MT_NIGHT_FIGHT_ROLE_INFO_SC) {}
	CSNightFightOperaReq::CSNightFightOperaReq() : header(MT_NIGHT_FIGHT_REQ_CS) {}
	SCNightFightRankInfo::SCNightFightRankInfo() : header(MT_NIGHT_FIGHT_RANK_INFO_SC) {}
	SCNightFightReward::SCNightFightReward() : header(MT_NIGHT_FIGHT_REWARD_INFO_SC) {}
	SCNightFightRedSideListInfo::SCNightFightRedSideListInfo() : header(MT_NIGHT_FIGHT_RED_SIDE_LIST_INFO_SC) {}
	SCNightFightAllRoleScoreInfo::SCNightFightAllRoleScoreInfo() : header(MT_NIGHT_FIGHT_HURT_ALL_ROLE_SCORE_INFO_SC) {}
	SCNightFightPlayerPosi::SCNightFightPlayerPosi() : header(MT_NIGHT_FIGHT_RANK_ROLE_POSI_INFO_SC) {}
	SCNightFightBossRankInfo::SCNightFightBossRankInfo() : header(MT_NIGHT_FIGHT_HURT_BOSS_RANK_SC) {}
	SCNightFightTotalScoreRank::SCNightFightTotalScoreRank():header(MT_NIGHT_FIGHT_TOTAL_RANK_SC){}
	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCMessBattleRoleInfo::SCMessBattleRoleInfo() : header(MT_MESS_BATTLE_ROLE_INFO_SC) {}
	CSMessBattleEnterReq::CSMessBattleEnterReq() : header(MT_MESS_BATTLE_ENTER_REQ_CS) {}
	SCMessBattleRankInfo::SCMessBattleRankInfo() : header(MT_MESS_BATTLE_RANK_INFO_SC) {}
	SCMessBattleReward::SCMessBattleReward() : header(MT_MESS_BATTLE_REWARD_INFO_SC) {}
	SCMessBattleHurtRankInfo::SCMessBattleHurtRankInfo() : header(MT_MESS_BATTLE_HURT_RANK_LIST_INFO_SC) {}
	SCMessBattleAllRoleScoreInfo::SCMessBattleAllRoleScoreInfo() : header(MT_MESS_BATTLE_HURT_ALL_ROLE_SCORE_INFO_SC) {}
	SCMessBattleToalScoreRank::SCMessBattleToalScoreRank():header(MT_MESS_BATTLE_TOTAL_RANK_SC){}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCCrossLieKunFBSceneInfo::SCCrossLieKunFBSceneInfo() : header(MT_CROSS_LIEKUN_FB_SCENE_INFO_SC) {}
	SCCrossLieKunFBLiveFlagInfo::SCCrossLieKunFBLiveFlagInfo() : header(MT_CROSS_LIEKUN_FB_LIVE_FLAG_SC) {}
	SCCrossLieKunFBBossHurtInfo::SCCrossLieKunFBBossHurtInfo() : header(MT_CROSS_LIEKUN_FB_BOSS_HURT_INFO){}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSHeShenLuoShuReq::CSHeShenLuoShuReq() : header(MT_HESHENLUOSHU_REQ_CS) {}
	SCHeShenLuoShuAllInfo::SCHeShenLuoShuAllInfo() : header(MT_HESHENLUOSHU_ALL_INFO_SC) {}
	SCHeShenLuoShuChangeInfo::SCHeShenLuoShuChangeInfo() : header(MT_HESHENLUOSHU_CHANGE_INFO_SC) {}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCShengqiInfo::SCShengqiInfo() : header(MT_SHENGQI_INFO_SC) {}
	CSShengqiReq::CSShengqiReq() : header(MT_SHENGQI_REQ_CS) {}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSShangGuBossReq::CSShangGuBossReq() : header(MT_SHANGGU_BOSS_OPERATE_CS) {}
	SCShangGuBossAllInfo::SCShangGuBossAllInfo() : header(MT_SHANGGU_BOSS_ALL_INFO_SC) {}
	SCShangGuBossLayerInfo::SCShangGuBossLayerInfo() : header(MT_SHANGGU_BOSS_LAYER_INFO_SC) {}
	SCShangGuBossSceneInfo::SCShangGuBossSceneInfo() : header(MT_SHANGGU_BOSS_SCENE_INFO_SC) {}
	SCShangGuBossSceneOtherInfo::SCShangGuBossSceneOtherInfo() : header(MT_SHANGGU_BOSS_SCENE_OTHER_INFO_SC) {}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSImpGuardOperaReq::CSImpGuardOperaReq() : header(MT_IMP_GURAD_OPERA_REQ_CS) {}
	SCImpGuardInfo::SCImpGuardInfo() : header(MT_IMP_GUARD_INFO_SC) {}
	SCRoleImpAppeChange::SCRoleImpAppeChange() : header(MT_IMP_GUARD_APPE_CHANGE_SC) {}
	SCRoleImpExpireTime::SCRoleImpExpireTime() : header(MT_IMP_GUARD_EXPIRE_TIME_SC) {}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSBossCardReq::CSBossCardReq() : header(MT_BOSS_CARD_REQ_CS) {}
	SCBossCardAllInfo::SCBossCardAllInfo() : header(MT_BOSS_CARD_INFO_SC) {}

	/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSRoleZhuanZhiReq::CSRoleZhuanZhiReq() : header(MT_ROLE_ZHUANZHI_REQ_CS) {}
	SCRoleZhuanZhiInfo::SCRoleZhuanZhiInfo() : header(MT_ROLE_ZHUANZHI_INFO_SC) {}
	SCZhuanzhiSkillTrigger::SCZhuanzhiSkillTrigger() : header(MT_ROLE_ZHUANZHI_SKILL_TRIGGER_SC) {}
	
	///////////////////////////////////		圣印系统	///////////////////////////////////////////////////////////
	CSSealReq::CSSealReq() : header(MT_SEAL_REQ_CS) {}
	SCSealBackpackInfo::SCSealBackpackInfo() : header(MT_SEAL_BACKPACK_INFO_SC) {}
	SCSealSlotInfo::SCSealSlotInfo() : header(MT_SEAL_SLOT_INFO_SC) {}
	SCSealBaseInfo::SCSealBaseInfo() : header(MT_SEAL_BASE_INFO_SC) {}
	CSSealRecycleReq::CSSealRecycleReq() : header(MT_SEAL_RECYCLE_REQ_CS) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////		完美情人	///////////////////////////////////////////////////////////
	SCRAPerfectLoverInfo::SCRAPerfectLoverInfo() : header(MT_RA_PERFECT_LOVER_INFO_SC) {}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////		全民进阶	///////////////////////////////////////////////////////////
	SCQuanMinJinJieInfo::SCQuanMinJinJieInfo() : header(MT_RA_QUANMIN_JINJIE_INFO_SC) {}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////

	///////////////////////////////////		全民总动员	///////////////////////////////////////////////////////////
	SCUpgradeGroupeInfo::SCUpgradeGroupeInfo() : header(MT_RA_UPGRADE_GROUP_INFO_SC) {}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAConsumGift::SCRAConsumGift() : header(MT_RA_CONSUM_GIFT_INFO_SC) {}
	SCRAConsumGiftRollReward::SCRAConsumGiftRollReward() : header(MT_RA_CONSUM_GIFT_ROLL_REWARD_SC) {}
	//消费领奖摇奖10次
	SCRAConsumGiftRollRewardTen::SCRAConsumGiftRollRewardTen() : header(MT_RA_CONSUM_GIFT_ROLL_REWARD_TEN_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRALimitBuyInfo::SCRALimitBuyInfo() : header(MT_RA_LIMIT_BUY_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRACollectTreasureInfo::SCRACollectTreasureInfo() : header(MT_RA_COLLECT_TREASURE_INFO_SC) {}
	SCRACollectTreasureResult::SCRACollectTreasureResult() : header(MT_RA_COLLECT_TREASURE_RESULT_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAHappyCumulChongzhiInfo::SCRAHappyCumulChongzhiInfo() : header(MT_RA_HAPPY_CUMUL_CHONGZHI_INFO_SC) {}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRACriticalStrikeInfo::SCRACriticalStrikeInfo() : header(MT_RA_CRITICAL_STRIKE_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAJinJieReturnInfo::SCRAJinJieReturnInfo() : header(MT_RA_JINJIE_RETURN_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAConsumeForGiftAllInfo::SCRAConsumeForGiftAllInfo() : header(MT_RA_CONSUME_FOR_GIFT_ALL_INFO_SC) {}

	///////////////////////////////////////////////////秘境寻宝3/////////////////////////////////////////////////////////////////
	SCRAMiJingXunBaoInfo3::SCRAMiJingXunBaoInfo3() : header(MT_RA_MIJINGXUNBAO3_INFO_SC) {}
	SCRAMiJingXunBaoTaoResultInfo3::SCRAMiJingXunBaoTaoResultInfo3() : header(MT_RA_MIJINGXUNBAO3_TAO_REWARD_INFO_SC) {}

	///////////////////////////////////////////////////欢乐砸蛋//////////////////////////////////////////////////////////////////
	SCRAHuanLeZaDanInfo::SCRAHuanLeZaDanInfo() : header(MT_RA_HUANLEZADAN_INFO_SC) {}
	SCRAHuanLeZaDanTaoResultInfo::SCRAHuanLeZaDanTaoResultInfo() : header(MT_RA_HUANLEZADAN_TAO_REWARD_INFO_SC) {}

	///////////////////////////////////////////////////欢乐摇奖//////////////////////////////////////////////////////////////////
	SCRAHuanLeYaoJiangInfo::SCRAHuanLeYaoJiangInfo() : header(MT_RA_HUANLEYAOJIANG_INFO_SC) {}
	SCRAHuanLeYaoJiangTaoResultInfo::SCRAHuanLeYaoJiangTaoResultInfo() : header(MT_RA_HUANLEYAOJIANG_TAO_REWARD_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRuneTowerPassRewardInfo::SCRuneTowerPassRewardInfo() : header(MT_FUNE_TOWER_REWARD_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRALimitTimeRebateInfo::SCRALimitTimeRebateInfo() : header(MT_RA_LIMIT_TIME_REBATE_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRATimeLimitGiftInfo::SCRATimeLimitGiftInfo() : header(MT_RA_TIME_LIMIT_GIFT_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCCirculationChongzhiInfo::SCCirculationChongzhiInfo() : header(MT_RA_CIRCUL_CHONGZHI_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAShakeMoneyInfo::SCRAShakeMoneyInfo() : header(MT_RA_SHAKE_MONEY_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRATimeLimitLuxuryGiftBagInfo::SCRATimeLimitLuxuryGiftBagInfo() : header(MT_RA_TIME_LIMIT_LUXURY_GIFT_INFO_SC) {}	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAResetDoubleChongzhi::SCRAResetDoubleChongzhi() : header(MT_RA_DOUBLE_CHONGZHI_INFO_SC) {}	

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRARmbBugChestShopInfo::SCRARmbBugChestShopInfo() : header(MT_RA_RMB_CHEST_SHOP_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAConsumeGoldRewardInfo::SCRAConsumeGoldRewardInfo() : header(MT_RA_CONSUME_GOLD_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCCollectSecondExchangeInfo::SCCollectSecondExchangeInfo() : header(MT_RA_COLLECT_SECOND_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAVersionTotalChargeInfo::SCRAVersionTotalChargeInfo() : header(MT_RA_VERSION_TOTAL_CHARGE_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAVersionContinueChongzhiInfo::SCRAVersionContinueChongzhiInfo() : header(MT_RA_VERSION_CONTINUE_CHARGE_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAHuanLeYaoJiangTwoInfo::SCRAHuanLeYaoJiangTwoInfo() : header(MT_RA_HUANLE_YAOJIANG_TWO_INFO_SC) {}
	SCRAHuanLeYaoJiangTwoTaoResultInfo::SCRAHuanLeYaoJiangTwoTaoResultInfo() : header(MT_RA_HUANLE_YAOJIANG_TWO_TAO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAOfflineSingleChargeInfo0::SCRAOfflineSingleChargeInfo0() : header(MT_RA_OFFLINE_SINGLE_CHARGE_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCBuyOneGetOneFreeInfo::SCBuyOneGetOneFreeInfo() : header(MT_RA_BUY_ONE_GET_ONE_FREE_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSGetLuckyLog::CSGetLuckyLog() : header(MT_LUCKY_LOG_ITEM_REQ_CS) {}
	SCLuckyLogRet::SCLuckyLogRet() : header(MT_LUCKY_LOG_ITEM_RET_SC) {}

	///////////////////////////////////////////////// 节日守护(个人塔防) ///////////////////////////////////////////
	CSHolidayGuardRoleReq::CSHolidayGuardRoleReq() : header(MT_FB_HOLIDAY_GUARD_ROLE_REQ_CS) {}
	SCHolidayGuardRoleInfo::SCHolidayGuardRoleInfo() : header(MT_FB_HOLIDAY_GUARD_ROLE_INFO_SC) {}
	SCHolidayGuardInfo::SCHolidayGuardInfo() : header(MT_FB_HOLIDAY_GUARD_INFO_SC) {}
	SCHolidayGuardFBDropInfo::SCHolidayGuardFBDropInfo() : header(MT_FB_HOLIDAY_GUARD_DROP_INFO_SC) {}
	SCHolidayGuardResult::SCHolidayGuardResult() : header(MT_FB_HOLIDAY_GUARD_RESULT_SC) {}
	SCHolidayGuardWarning::SCHolidayGuardWarning() : header(MT_FB_HOLIDAY_GUARD_WARNING_SC) {}
	SCRAHolidayGuardRanKInfo::SCRAHolidayGuardRanKInfo() : header(MT_RA_HOLIDAY_GUARD_KILL_RANK_INFO_SC) {}
	CSRAHolidayGuardRankInfoReq::CSRAHolidayGuardRankInfoReq() : header(MT_RA_HOLIDAY_GUARD_INFO_CS) {}
	SCRAExtremeChallengeNpcInfo::SCRAExtremeChallengeNpcInfo() : header(MT_RA_HOLIDAY_GUARD_NPC_INFO) {}

	///////////////////////////////////////////////// 版本活动 累计充值5 ////////////////////////////////////////////////////////////
	CSRATotalChargeFiveInfo::CSRATotalChargeFiveInfo() : header(MT_RA_TOTAL_CHARGE_5_INFO_CS) {}
	SCRATotalChargeFiveInfo::SCRATotalChargeFiveInfo() : header(MT_RA_TOTAL_CHARGE_5_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAExtremeChallengeTaskInfo::SCRAExtremeChallengeTaskInfo() : header(MT_RA_EXTREME_CHALLENGE_INFO_SC) {}

	/////////////////////////////////////////////////限定套装//////////////////////////////////////////////////////////////////
	CSDressingRoomOpera::CSDressingRoomOpera() : header(MT_DRESSING_ROOM_OPEAR_CS) {}
	SCDressingRoomInfo::SCDressingRoomInfo() : header(MT_DRESSING_ROOM_INFO_SC) {}
	SCDressingRoomSingleInfo::SCDressingRoomSingleInfo() : header(MT_DRESSING_ROOM_SINGLE_INFO_SC) {}
	CSDressingRoomExchange::CSDressingRoomExchange() : header(MT_DRESSING_ROOM_EXCHANGE_CS) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSRoleBigSmallGoalOper::CSRoleBigSmallGoalOper() : header(MT_RA_ROLE_BIG_SMALL_GOAL_CS) {}
	SCRoleBigSmallGoalInfo::SCRoleBigSmallGoalInfo() : header(MT_RA_ROLE_BIG_SMALL_GOAL_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAChongZhiGiftInfo::SCRAChongZhiGiftInfo() : header(MT_RA_CHONGZHI_GIFT_INFO_CS) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCCSATouzijihuaInfo::SCCSATouzijihuaInfo() : header(MT_CSA_TOUZI_JIHUA_INFO_CS) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCCSAFoundationInfo::SCCSAFoundationInfo() : header(MT_CSA_FOUNDATION_INFO_CS) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCCSAExpRefineInfo::SCCSAExpRefineInfo() : header(MT_CSA_EXPREFINE_INFO_CS) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCCSAGongChengZhanInfo::SCCSAGongChengZhanInfo() : header(MT_CSA_GONGCHENGZHAN_INFO_CS) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCNoticeBossDead::SCNoticeBossDead() : header(MT_BOSS_DEAD_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRedEquipCollect::SCRedEquipCollect() : header(MT_RED_COLLECT_INFO_SC) {}
	SCRedEquipCollectOther::SCRedEquipCollectOther() : header(MT_RED_COLLECT_OTHER_INFO_SC) {}
	SCOrangeEquipCollect::SCOrangeEquipCollect() : header(MT_ORANGE_COLLECT_INFO_SC) {}
	SCOrangeEquipCollectOther::SCOrangeEquipCollectOther() : header(MT_ORANGE_COLLECT_OTHER_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRAJinJieReturnInfo2::SCRAJinJieReturnInfo2() : header(MT_RA_JINJIE_RETURN_INFO2_SC) {}
	SCRACriticalStrike2Info::SCRACriticalStrike2Info() : header(MT_RA_CRITICAL_STRIKE2_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRACombineBuyBucketInfo::SCRACombineBuyBucketInfo() : header(MT_RA_COMBINEBUY_BUCKET_INFO_SC) {}
	SCRACombineBuyItemInfo::SCRACombineBuyItemInfo() : header(MT_RA_COMBINEBUY_ITEM_INFO2_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSDayChongzhiRewardReq::CSDayChongzhiRewardReq() : header(MT_RA_DAY_CHONGZHI_REQ_CS) {}
	SCDayChongzhiRewardInfo::SCDayChongzhiRewardInfo() : header(MT_RA_DAY_CHONGZHI_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSXianZunKaOperaReq::CSXianZunKaOperaReq() : header(MT_XIANZUNKA_REQ_CS) {}
	SCXianZunKaAllInfo::SCXianZunKaAllInfo() : header(MT_XIANZUNKA_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCRoleAppeChange::SCRoleAppeChange() : header(MT_ROLE_APPE_CHANGE_SC) {}

	CSRolePersonAreaMsgInfo::CSRolePersonAreaMsgInfo() : header(MT_PERSON_AREAMSG_INFO_CS) {}
	SCRolePersonAreaMsgInfo::SCRolePersonAreaMsgInfo() : header(MT_PERSON_AREAMSG_INFO_SC) {}

	///////////////////////////////////////////////// 夫妻家园 //////////////////////////////////////////////
	CSSpouseHomeOperaReq::CSSpouseHomeOperaReq() : header(MT_SPOUSE_HOME_OPERE_REQ_CS) {}
	SCSpouseHomeRoomInfo::SCSpouseHomeRoomInfo() : header(MT_SPOUSE_HOME_ALL_ROOM_INFO_SC) {}
	SCSpouseHomeFirendInfo::SCSpouseHomeFirendInfo() : header(MT_SPOUSE_HOME_FIREND_INFO_SC) {}
	SCSpouseHomeGuildMemberInfo::SCSpouseHomeGuildMemberInfo() : header(MT_SPOUSE_HOME_GUILD_MEMBER_INFO_SC) {}
	SCSpouseHomeSingleRoomInfo::SCSpouseHomeSingleRoomInfo() : header(MT_SPOUSE_HOME_GUILD_SINGLE_ROOM_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSEquipBaptizeOperaReq::CSEquipBaptizeOperaReq() : header(MT_EQUIP_BAPTIZE_REQ_CS) {}
	SCEquipBaptizeAllInfo::SCEquipBaptizeAllInfo() : header(MT_EQUIP_BAPTIZE_INFO_SC) {}

	//////////////////////////////////////		神格套装		///////////////////////////////////////////////////
	SCShengeSuitInfo::SCShengeSuitInfo() : header(MT_SHENGE_SYSTEM_SUIT_INFO_SC) {}

	///////////////////////////////////////////////// 斗气装备 //////////////////////////////////////////////
	CSCrossEquipOpera::CSCrossEquipOpera() : header(MT_CROSSEQUIP_REQ_CS) {}
	SCCrossEquipAllInfo::SCCrossEquipAllInfo() : header(MT_CROSSEQUIP_ALL_INFO_SC) {}
	SCCrossEquipRollResult::SCCrossEquipRollResult() : header(MT_CROSSEQUIP_ROLL_RESULT_SC) {}
// 	SCCrossEquipBossInfo::SCCrossEquipBossInfo() : header(MT_CROSS_EQUIP_BOSS_INFO_SC) {}
// 	SCCrossEquipGatherInfo::SCCrossEquipGatherInfo() : header(MT_CROSS_EQUIP_GATHER_INFO_SC) {}
	SCCrossEquipOneEquip::SCCrossEquipOneEquip() : header(MT_CROSS_EQUIP_BOSS_ONE_EQUIP_SC) {}
	SCCrossEquipAllEquip::SCCrossEquipAllEquip() : header(MT_CROSS_EQUIP_BOSS_ALL_EQUIP_SC) {}
	SCCrossEquipChuanshiFragmentChange::SCCrossEquipChuanshiFragmentChange() : header(MT_CROSS_EQUIP_BOSS_CHUANSHI_FRAG_CHANGE_SC) {}
	SCCrossEquipDouqiExpChange::SCCrossEquipDouqiExpChange() : header(MT_CROSS_EQUIP_DOUQI_EXP_CHANGE_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCMiZangBossReliveTire::SCMiZangBossReliveTire() : header(MT_MIZANG_BOSS_RELIVE_TIRE_SC) {}
	CSCrossMiZangBossBossInfoReq::CSCrossMiZangBossBossInfoReq() : header(MT_MIZANG_BOSS_BOSS_INFO_CS) {}
	SCCrossMizangBossBossInfoAck::SCCrossMizangBossBossInfoAck() : header(MT_MIZANG_BOSS_BOSS_INFO_SC) {}
	SCCrossMizangBossSceneInfo::SCCrossMizangBossSceneInfo() : header(MT_MIZANG_BOSS_SCENE_INFO_SC) {}
	SCCrossMiZangBossPlayerInfo::SCCrossMiZangBossPlayerInfo() : header(MT_MIZANG_BOSS_PLAYER_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCYouMingBossReliveTire::SCYouMingBossReliveTire() : header(MT_YOUMING_BOSS_RELIVE_TIRE_SC) {}
	CSCrossYouMingBossBossInfoReq::CSCrossYouMingBossBossInfoReq() : header(MT_YOUMING_BOSS_BOSS_INFO_CS) {}
	SCCrossYouMingBossBossInfoAck::SCCrossYouMingBossBossInfoAck() : header(MT_YOUMING_BOSS_BOSS_INFO_SC) {}
	SCCrossYouMingBossSceneInfo::SCCrossYouMingBossSceneInfo() : header(MT_YOUMING_BOSS_SCENE_INFO_SC) {}
	SCCrossYouMingBossPlayerInfo::SCCrossYouMingBossPlayerInfo() : header(MT_YOUMING_BOSS_PLAYER_INFO_SC) {}

	SCLuckyWishInfo::SCLuckyWishInfo() : header(MT_RA_LUCKY_WISH_INFO_SC) {}
	CSLuckyWishOpera::CSLuckyWishOpera() : header(MT_RA_LUCKY_WISH_OPERA_CS) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCImageCompetitionInfo::SCImageCompetitionInfo() : header(MT_IMAGE_COMPETITION_INFO_SC) {}

	SCRandGiftItemInfo::SCRandGiftItemInfo() : header(MT_RAND_GIFT_REWARD_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSFetchDoubleRewardReq::CSFetchDoubleRewardReq() : header(MT_FETCH_DOUBLE_REWARD_CS) {}
	SCFetchDoubleRewardResult::SCFetchDoubleRewardResult() : header(MT_FETCH_DOUBLE_REWARD_RESULT_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSPersonBossInfoReq::CSPersonBossInfoReq() : header(MT_PERSON_BOSS_INFO_CS) {}
	SCPersonBossInfo::SCPersonBossInfo() : header(MT_PERSON_BOSS_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSYunyouBossOperate::CSYunyouBossOperate() : header(MT_YUNYOU_BOSS_OPERA_CS) {}
	SCYouyouBossInfo::SCYouyouBossInfo() : header(MT_YUNYOU_BOSS_INFO_SC) {}
	SCYouyouSceneInfo::SCYouyouSceneInfo() : header(MT_YUNYOU_BOSS_SCENE_INFO_SC) {}

	///////////////////////////////////////////  名将 ///////////////////////////////////////////////////////////////////////////////
	CSGreateSoldierOpera::CSGreateSoldierOpera() : header(MT_GREATE_SOLDIER_OPERA_CS) {}
	CSGreateSoldierReqStrength::CSGreateSoldierReqStrength() : header(MT_GREATE_SOLDIER_UPGREADE_EQUIP_CS) {}

	SCGreateSoldierItemInfo::SCGreateSoldierItemInfo() : header(MT_GREATE_SOLDIER_ITEM_INFO_SC) {}
	SCGreateSoldierOtherInfo::SCGreateSoldierOtherInfo() : header(MT_GREATE_SOLDIER_OTHER_SC) {}
	SCGreateSoldierSlotInfo::SCGreateSoldierSlotInfo() : header(MT_GREATE_SOLDIER_SLOT_SC) {}
	SCGreateSoldierFetchReward::SCGreateSoldierFetchReward() : header(MT_GREATE_SOLDIER_FETCH_REWARD_SC) {}
	SCGreateSoldierGoalInfo::SCGreateSoldierGoalInfo() : header(MT_GREATE_SOLDIER_GOAL_INFO_SC) {}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	CSTianshenhutiReq::CSTianshenhutiReq() : header(MT_TIANSHENHUTI_OPERA_REQ_CS) {}
	SCTianshenhutiALlInfo::SCTianshenhutiALlInfo() : header(MT_TIANSHENHUTI_ALL_EQUIP_INFO_SC) {}
	SCTianshenhutiRollResult::SCTianshenhutiRollResult() : header(MT_TIANSHENHUTI_ROLL_RESULT_SC) {}
	SCTianshenhutiReqResult::SCTianshenhutiReqResult() : header(MT_TIANSHENHUTI_OPERA_REQ_RESULT_SC) {}
	SCTianshenhutiScoreChange::SCTianshenhutiScoreChange() : header(MT_TIANSHENHUTI_SCORE_CHANGE_CS) {}
	SCTianshenhutiCombineOneKeyResult::SCTianshenhutiCombineOneKeyResult() : header(MT_TIANSHENHUTI_COMBINE_ONEKEY_RESULT_SC) {}

	//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCCrossRandActivityStatus::SCCrossRandActivityStatus() : header(MT_CROSS_RA_ACTIVITY_STATUS_SC) {}
	SCCrossRAChongzhiRankChongzhiInfo::SCCrossRAChongzhiRankChongzhiInfo() : header(MT_CROSS_RA_CHONGZHI_RANK_INFO_SC) {}
	CSCrossRandActivityRequest::CSCrossRandActivityRequest() : header(MT_CROSS_RA_REQUEST_OPERA_CS) {}

	SCCloudPurchaseInfo::SCCloudPurchaseInfo() : header(MT_CLOUD_PURCHASE_TOTAL_INFO) {}
	SCCloudPurchaseConvertInfo::SCCloudPurchaseConvertInfo() : header(MT_CLOUD_PURCHASE_CONVERT_INFO) {}
	SCCloudPurchaseBuyRecordInfo::SCCloudPurchaseBuyRecordInfo() : header(MT_CLOUD_PURCHASE_BUY_INFO) {}
	SCCloudPurchaseServerRecord::SCCloudPurchaseServerRecord() : header(MT_CLOUD_PURCHASE_SERVER_RECORE_INFO) {}
	SCCloudPurchaseUserInfo::SCCloudPurchaseUserInfo() : header(MT_CLOUD_PURCHASE_USER_INFO_SC) {}
	SCCrossRAConsumeRankConsumeInfo::SCCrossRAConsumeRankConsumeInfo() : header(MT_CROSS_RA_CONSUME_RANK_INFO_SC) {}

	SCUpgradeCardBuyInfo::SCUpgradeCardBuyInfo() : header(MT_UPGRADE_CARD_BUY_INFO_SC) {}
	CSUpgradeCardBuyReq::CSUpgradeCardBuyReq() : header(MT_UPGRADE_CARD_BUY_REQ_CS) {}

	CSGodmagicBossInfoReq::CSGodmagicBossInfoReq():header(MT_GODMAGIC_BOSS_INFO_REQ_CS){}
	SCGodmagicBossInfoAck::SCGodmagicBossInfoAck():header(MT_GODMAGIC_BOSS_INFO_ACK_SC){}
	SCGodmagicBossPlayerInfo::SCGodmagicBossPlayerInfo():header(MT_GODMAGIC_PLAYER_INFO_SC){}
	SCGodmagicBossSceneInfo::SCGodmagicBossSceneInfo():header(MT_GODMAGIC_BOSS_SCENE_INFO_SC){}

	SCHuguozhiliInfo::SCHuguozhiliInfo() : header(MT_HUGUOZHILI_INFO_SC) {}
	CSHuguozhiliReq::CSHuguozhiliReq() : header(MT_HUGUOZHILI_REQ_CS) {}

	CSRoleReturnReAlivePosi::CSRoleReturnReAlivePosi() : header(MT_ROLE_RETURN_REALIVE_POSI_CS) {}

	CSTodayThemeRewardReq::CSTodayThemeRewardReq() : header(MT_TODAY_THEME_REWARD_REQ_CS) {}
	SCTodayThemeRewardFlagInfo::SCTodayThemeRewardFlagInfo() : header(MT_TODAY_THEME_REWARD_FLAG_INFO_SC) {}


	SCWorldAcquisitionLog::SCWorldAcquisitionLog() : header(MT_WORLD_ACQUISITION_LOG_SC) {}
	CSWorldAcquisitionLogReq::CSWorldAcquisitionLogReq() : header(MT_WORLD_ACQUISITION_LOG_REQ_CS) {}

	SCBaiBeiFanLi2Info::SCBaiBeiFanLi2Info() : header(MT_BAI_BEI_FAN_LI2_INFO_SC) {}
	CSBaiBeiFanLi2Buy::CSBaiBeiFanLi2Buy() : header(MT_BAI_BEI_FAN_LI2_BUY_CS) {}

	SCRAOpenGameGiftShopBuy2Info::SCRAOpenGameGiftShopBuy2Info() : header(MT_RA_OPEN_GAME_GIFT_SHOP2_INFO_SC) {}
	CSRAOpenGameGiftShopBuy2::CSRAOpenGameGiftShopBuy2() : header(MT_RA_OPEN_BUY_GAME_GIFT_SHOP2_INFO_CS) {}

	CSCrossGoalOperaReq::CSCrossGoalOperaReq():header(MT_CROSS_GOAL_OPERA_REQ_CS){}
	SCCrossGoalInfo::SCCrossGoalInfo():header(MT_CROSS_GOAL_INFO_SC){}

	SCCrossGoalGuildNotify::SCCrossGoalGuildNotify():header(MT_CROSS_GOAL_GUILD_NOTIFY){}
	
	SCZeroGiftGodCostumeInfo::SCZeroGiftGodCostumeInfo() : header(MT_ZERO_GIFT_GOD_COSTUME_INFO_SC) {}

	SCFightBackRoleList::SCFightBackRoleList() : header(MT_FIGHT_BACK_ROLE_LIST_SC) {}
	
	CSBaizhanEquipOpera::CSBaizhanEquipOpera() : header(MT_BAIZHAN_EQUIP_REQ_CS) {}
	SCBaizhanEquipAllInfo::SCBaizhanEquipAllInfo() : header(MT_BAIZHAN_EQUIP_INFO_SC) {}

	SCEquipUplevelSuitInfo::SCEquipUplevelSuitInfo() : header(MT_EQUIP_GRADE_SUIT_INFO_SC) {}
	SCSceneActivityRewardInfo::SCSceneActivityRewardInfo() : header(MT_SCENE_ACTIVITY_REWARD_SC) {}

	SCZhuanzhiEquipAwakeningAllInfo::SCZhuanzhiEquipAwakeningAllInfo() : header(MT_ZHUANZHI_EQUIP_AWAKENING_ALL_INFO) {}
	SCZhuanzhiEquipAwakeningInfo::SCZhuanzhiEquipAwakeningInfo() : header(MT_ZHUANZHI_EQUIP_AWAKENING_INFO) {}

	CSLittleHelperOpera::CSLittleHelperOpera() : header(MT_LITTLE_HELPER_REQ_CS) {}
	CSLittleHelperRepeatOpera::CSLittleHelperRepeatOpera() : header(MT_LITTLE_HELPER_REPEAT_REQ_CS) {}
	CSGetGuildRareLog::CSGetGuildRareLog() : header(MT_GUILD_RARE_LOG_RQE_CS) {}
	SCGuildRareLogRet::SCGuildRareLogRet() : header(MT_GUILD_RARE_LOG_INFO_SC) {}
	SCFirstRechargeBuffFlag::SCFirstRechargeBuffFlag() : header(MT_FIRST_RECHARGE_BUFF_FLAG_SC) {}
	
	SCTianShuXZInfo::SCTianShuXZInfo():header(MT_TIANSHUXZ_INFO_SC){}
	CSTianShuXZFetchReward::CSTianShuXZFetchReward():header(MT_TIANSHUXZ_FETCH_REWARD_CS){}

	CSEquipUplevelSuitActive::CSEquipUplevelSuitActive() : header(MT_EQUIP_GRADE_SUIT_ACTIVE_CS) {}

	SCExpBuffInfo::SCExpBuffInfo() : header(MT_EXP_BUFF_INFO_SC) {}

	CSZhuanzhiEquipCompose::CSZhuanzhiEquipCompose() : header(MT_ZHUANZHI_EQUIP_COMPOSE_REQ_CS) {}
	SCLittleHelperInfo::SCLittleHelperInfo() : header(MT_LITTLE_HELPER_INFO_SC) {}
	SCLittleHelperItemInfo::SCLittleHelperItemInfo() : header(MT_LITTLE_HELPER_DROP_ITEM_INFO_SC) {}

	///////////////////////////////////////////  跨服竞技场 ///////////////////////////////////////////////////////////
	CSCrossChallengeFieldOpera::CSCrossChallengeFieldOpera() : header(MT_CROSS_CHALLENGE_FIELD_REQ_CS) {}
	SCCrossChallengeFieldStatus::SCCrossChallengeFieldStatus() : header(MT_CROSS_CHALLENGE_FIELD_STATUS_INFO_SC) {}
	SCCrossChallengeFieldUserInfo::SCCrossChallengeFieldUserInfo() : header(MT_CROSS_CHALLENGE_FIELD_USER_INFO_SC) {}
	SCCrossChallengeFieldOpponentInfo::SCCrossChallengeFieldOpponentInfo() : header(MT_CROSS_CHALLENGE_FIELD_OPPONENT_INFO_SC) {}
	SCCrossChallengeFieldBeDefeatNotice::SCCrossChallengeFieldBeDefeatNotice() : header(MT_CROSS_CHALLENGE_BE_DEFEAT_NOTICE_SC) {}
	SCCrossChallengeFieldWin::SCCrossChallengeFieldWin() : header(MT_CROSS_CHALLENGE_FIELD_WIN) {}
	SCCrossChallengeFieldReportInfo::SCCrossChallengeFieldReportInfo() : header(MT_CROSS_CHALLENGE_FIELD_REPORT_INFO_SC) {}
	SCCrossChallengeFieldRankInfo::SCCrossChallengeFieldRankInfo() : header(MT_CROSS_CHALLENGE_FIELD_RANK_INFO_SC) {}
	SCCrossChallengeFieldOpponentRankPosChange::SCCrossChallengeFieldOpponentRankPosChange() : header(MT_CROSS_CHALLENGE_FIELD_OPPONENT_RANK_POS_CHANGE) {}

	CSDiscountShopBuy::CSDiscountShopBuy() : header(MT_DISCOUNTSHOP_BUY_CS) {}
	SCSendDiscounthopItemInfo::SCSendDiscounthopItemInfo() : header(MT_DISCOUNTSHOP_INFO_SC) {}

	/////////////////////////////////////	幸运云购		/////////////////////////
	SCRALuckyCloudBuyInfo::SCRALuckyCloudBuyInfo() : header(MT_RA_LUCKYCLOUDBUYINFO_SC) {}
	SCRALuckyCloudBuyBuyList::SCRALuckyCloudBuyBuyList() : header(MT_RA_LUCKYCLOUDBUYLIST_SC) {}
	SCRALuckyCloudBuyOpenInfo::SCRALuckyCloudBuyOpenInfo() : header(MT_RA_LUCKYCLOUDBUYOPENINFO_SC) {}
	SCZhuanzhiEquipComposeSucceed::SCZhuanzhiEquipComposeSucceed() : header(MT_ZHUANZHI_EQUIP_COMPOSE_SUCCEED_SC) {}

	///////////////////////////////////////////////// 狂欢大乐购 //////////////////////////////////////////////
	SCRACrazyBuyAllInfo::SCRACrazyBuyAllInfo() : header(MT_CRACY_BUY_ALL_INFO) {}
	SCRACracyBuyLimitInfo::SCRACracyBuyLimitInfo() : header(MT_CRACY_BUY_LIMIT_INFO) {}

	///////////////////////////////////////////////// 神话系统 //////////////////////////////////////////////
	SCMythChpaterInfo::SCMythChpaterInfo() : header(MT_MYTH_CHPATER_INFO_SC) {}
	SCMythKnapaskInfo::SCMythKnapaskInfo() : header(MT_MYTH_KNAPSACK_INTFO_SC) {}
	CSMythOpera::CSMythOpera() : header(MT_MYTH_OPERA_REQ_CS) {}
	SCMythChpaterSingleInfo::SCMythChpaterSingleInfo() : header(MT_MYTH_CHPATER_SINGLE_INFO_SC) {}

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	SCZodiacInfo::SCZodiacInfo() : header(MT_ZODIAC_INFO_SC) {}
	SCZodiacBackpackInfo::SCZodiacBackpackInfo() : header(MT_ZODIAC_BACKPACK_INFO_SC) {}
	CSZodiacReq::CSZodiacReq() : header(MT_ZODIAC_REQ_CS) {}
	CSZodiacDecomposeReq::CSZodiacDecomposeReq() : header(MT_ZODIAC_DECOMPOSE_REQ_CS) {}
	SCZodiacBaseInfo::SCZodiacBaseInfo() : header(MT_ZODIAC_BASE_INFO_SC) {}

	/////////////////////////////////////	跨服护送水晶  /////////////////////////
	CSCrossHusongShuijingOpera::CSCrossHusongShuijingOpera():header(MT_CROSS_HUSONG_SHUIJING_REQ_CS){}
	SCCrossHusongShuijingInfo::SCCrossHusongShuijingInfo():header(MT_CROSS_HUSONG_SHUIJING_INFO_SC){}
	SCCrossHusongShuijingGatherInfo::SCCrossHusongShuijingGatherInfo():header(MT_CROSS_HUSONG_SHUIJING_GATHER_INFO_SC){}

	SCRAChongzhiRank2Info::SCRAChongzhiRank2Info() : header(MT_RA_CHONGZHI_RANK2_INFO_SC) {}
}
