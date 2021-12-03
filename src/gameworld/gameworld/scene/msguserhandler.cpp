#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "scene/map.h"
#include "obj/otherobj/fallingitem.h"
#include "obj/character/role.h"
#include "obj/otherobj/bigcoinobj.hpp"
#include "world.h"
#include "gstr.h"
#include "servercommon/servercommon.h"
#include "servercommon/noticenum.h"
#include "protocal/msgentergs.h"
#include "protocal/msgfight.h"
#include "protocal/msgrole.h"
#include "protocal/msgitem.h"
#include "protocal/msgworldevent.h"
#include "protocal/msgnpc.h"
#include "protocal/msghotkey.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/errornum.h"
#include "protocal/msgfriend.h"
#include "protocal/msgscene.h"
#include "protocal/msgother.h"
#include "protocal/msgroleother.h"
#include "protocal/msggm.hpp"
#include "protocal/msgmentality.hpp"
#include "protocal/msgmount.hpp"
#include "protocal/msgfightmount.hpp"
#include "protocal/msgwing.hpp"
#include "protocal/msghalo.hpp"
#include "protocal/msgshengong.hpp"
#include "protocal/msgshenyi.hpp"
#include "protocal/msgfootprint.hpp"
#include "protocal/msgcloak.hpp"
#include "protocal/msgactivedegree.hpp"
#include "protocal/msgdaycounter.hpp"
#include "protocal/msgactivity.hpp"
#include "protocal/msgsublock.hpp"
#include "protocal/msgtitle.hpp"
#include "protocal/msgfb.h"
#include "protocal/msgserver.h"
#include "config/sharedconfig/sharedconfig.h"
#include "protocal/msgxiannv.h"
#include "protocal/msgscpublicsale.h"
#include "protocal/msgtrade.h"
#include "protocal/msglieming.h"
#include "protocal/msgshengwang.h"
#include "protocal/msgchengjiu.h"
#include "protocal/msgtuhaojin.h"
#include "protocal/msgbigchatface.h"
#include "protocal/msgshenzhouweapon.h"
#include "protocal/msgbaby.h"
#include "protocal/msgpet.h"
#include "protocal/msghuashen.h"
#include "protocal/msgmultimount.h"
#include "protocal/msgpersonalizewindow.h"
#include "protocal/msgmagiccard.h"
#include "protocal/msgmitama.h"
#include "protocal/msgxunzhang.h"
#include "protocal/msgxiannvshouhu.hpp"
#include "protocal/msgjinglingfazhen.hpp"
#include "protocal/msgjinglingguanghuan.hpp"
#include "protocal/msgzhuanshengequip.h"
#include "protocal/msgmojie.hpp"
#include "protocal/msgequipment.h"
#include "protocal/msgimgfuling.hpp"
#include "protocal/msgjinjietalent.hpp"
#include "protocal/msgheshenluoshu.h"
#include "protocal/msgdressingroom.hpp"
#include "protocal/msgtianshenhuti.h"

#include "servercommon/internalprotocal/ggprotocal.h"
#include "globalconfig/globalconfig.h"
#include "internalcomm.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "gamelog.h"
#include "item/knapsack.h"
#include "friend/friendmanager.h"
#include "npc/transmit/fbmanager.hpp"
#include "other/gm/gm.hpp"
#include "other/route/mailroute.hpp"
#include "other/route/publicsaleroute.hpp"
#include "other/route/guildroute.hpp"
#include "other/queryhandler/queryhandler.hpp"
#include "other/fb/rolefbchallenge.hpp"
#include "global/trade/p2ptrademanager.h"
#include "friend/friendblessmanager.h"
#include "global/worldevent/worldeventmanager.hpp"
#include "obj/otherobj/worldeventobj.h"
#include "global/cross/crossmanager.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "messagehandler.h"
#include "other/shop/shop.hpp"
#include "other/convertshop/convertshop.hpp"
#include "other/sublock/sublock.hpp"
#include "other/daycounter/daycounter.hpp"
#include "scene/speciallogic/speciallogic.hpp"
#include "scene/speciallogic/guildspecial/specialguildstation.hpp"
#include "scene/speciallogic/worldspecial/specialgongchengzhan.hpp"
#include "scene/speciallogic/personspecial/specialexpfb.hpp"
#include "scene/speciallogic/worldspecial/specialhunyan.hpp"
#include "scene/speciallogic/personspecial/specialtowerdefend.hpp"
#include "scene/speciallogic/teamspecial/specialmigongxianfuteamfb.hpp"
#include "scene/speciallogic/worldspecial/specialcrosshotspring.hpp"
#include "scene/speciallogic/worldspecial/specialguildbattlenew.hpp"
#include "scene/speciallogic/worldspecial/specialterritorywar.hpp"
#include "scene/speciallogic/worldspecial/specialcrossxiuluotower.hpp"
#include "scene/speciallogic/personspecial/specialfunopenwing.hpp"
#include "scene/speciallogic/personspecial/specialfunopenmountfb.hpp"
#include "scene/speciallogic/personspecial/specialfunopenxiannv.hpp"
#include "scene/speciallogic/worldspecial/specialmillionaire.hpp"
#include "scene/speciallogic/worldspecial/specialcrossmultiuserchallenge.hpp"
#include "scene/speciallogic/personspecial/specialphasefb.hpp"
#include "scene/speciallogic/personspecial/specialdailytaskfb.hpp"
#include "scene/speciallogic/worldspecial/specialcrossdarknight.hpp"
#include "scene/speciallogic/worldspecial/specialcrossfishing.hpp"
#include "scene/speciallogic/doublesidefbspecial/specialnightfight.hpp"
#include "scene/speciallogic/personspecial/specialbuildtower.hpp"
#include "scene/speciallogic/teamspecial/specialtowerdefendteam.hpp"
#include "scene/speciallogic/worldspecial/specialcrossguildbattle.hpp"
#include "scene/speciallogic/other/specialchallengefield.hpp"
#include "scene/speciallogic/other/specialcrosslocalchallengefield.hpp"

#include "teamkeyregister/teamkeyregister.hpp"
#include "guildkeyregister/guildkeyregister.hpp"
#include "scene/sceneregister/sceneregister.hpp"
#include "scene/marryshadow/marryshadow.h"
#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/namefilter.hpp"
#include "other/mount/mountfunction.hpp"
#include "other/fightmount/fightmountfunction.hpp"
#include "other/wing/wingfunction.hpp"
#include "other/halo/halofunction.hpp"
#include "other/shengong/shengongfunction.hpp"
#include "other/shenyi/shenyifunction.hpp"
#include "other/cloak/cloakmanager.hpp"
#include "other/xiannvshouhu/xiannvshouhufunction.hpp"
#include "other/jinglingfazhen/jinglingfazhenfunction.hpp"
#include "other/jinglingguanghuan/jinglingguanghuanfunction.hpp"
#include "other/mojie/mojiemanager.hpp"
#include "skill/skillfunction.hpp"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/combineserveractivity/combineserveractivitymanager.hpp"
#include "config/logicconfigmanager.hpp"
#include "other/mount/mountopconfig.hpp"
#include "other/fightmount/fightmountconfig.hpp"
#include "global/globalrole/lovetree/lovetree.hpp"
#include "global/globalrole/qingyuanglobal/qingyuanglobal.hpp"
#include "other/event/eventhandler.hpp"
#include "chat/chatmanager.h"
#include "global/team/teammanager.hpp"
#include "global/worldboss/worldbossmanager.hpp"
#include "global/bossfamily/bossfamilymanager.hpp"
#include "global/usercache/usercache.hpp"
#include "global/rank/rankmanager.hpp"
#include "global/mail/mailmanager.hpp"
#include "global/activity/activitymanager.hpp"
#include "global/trade/userpublicsalemanager.hpp"
#include "global/challengefield/challengefield.hpp"
#include "global/crosschallengefield/crosschallengefield.hpp"
#include "global/offlineregister/spacemanager.hpp"
#include "global/camp/campmanager.hpp"
#include "global/tuodan/tuodanmanager.hpp"
#include "servercommon/performancecatch.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "global/fishpool/fishpoolmanager.hpp"
#include "protocal/msgfishpool.h"
#include "forcekickuserdelay.hpp"
#include "protocal/msgcardzu.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "speciallogic/worldspecial/specialcross1v1.hpp"
#include "gameworld/other/fb/teamfbroom.hpp"
#include "scene/speciallogic/worldspecial/yaoshouguangchangmgr.hpp"
#include "scene/speciallogic/worldspecial/suoyaotamgr.hpp"
#include "scene/speciallogic/guildspecial/specialguildfb.hpp"
#include "scene/speciallogic/worldspecial/jinghuahusongmgr.hpp"
#include "scene/speciallogic/worldspecial/specialcrossshuijing.hpp"
#include "scene/speciallogic/worldspecial/specialshuijing.hpp"
#include "scene/speciallogic/worldspecial/specialwanglingexplore.h"
#include "gameworld/other/camp/campnormalduobao.h"
#include "gameworld/scene/speciallogic/personspecial/specialloversfb.hpp"
#include "protocal/msglittlepet.h"
#include "global/globalrole/litttlepet/littlepet.hpp"
#include "global/globalrole/globalrolemanager.hpp"
#include "global/globalrole/jinglinghome/jinglinghome.hpp"
#include "protocal/msgchinesezodiac.h"
#include "protocal/msgfriendgift.hpp"
#include "global/globalrole/lovecontract/lovecontract.hpp"
#include "scene/speciallogic/personspecial/specialfbguide.hpp"
#include "other/shengesystem/shengesystem.hpp"
#include "protocal/msgshenbing.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"

#include "equipment/equipmentmanager.h"
#include "other/friendexpbottle/friendexpbottle.hpp"
#include "other/jingling/jinglingmanager.h"
#include "config/activityconfig/millionaireconfig.hpp"
#include "other/movechess/movechess.hpp"
#include "other/shop/mysteriousshopinmall.hpp"
#include "config/activityconfig/questionconfig.hpp"
#include "global/guild/roleguild.hpp"
#include "other/runesystem/runesystem.hpp"
#include "other/shenzhouweapon/shenzhouweapon.hpp"
#include "other/xiannv/xiannvmanager.hpp"
#include "gameworld/other/fb/rolefbtuitu.h"
#include "protocal/msgfighting.h"
#include "global/globalrole/fighting/rolefighting.hpp"
#include "other/shop/shop.hpp"
#include "other/shop/mysteriousshop.hpp"
#include "other/vip/vipconfig.hpp"
#include "other/rolegoal/rolegoalconfig.hpp"
#include "other/qingyuan/qingyuanconfig.hpp"
#include "config/activityconfig/territorywarconfig.hpp"
#include "other/wing/wingconfig.hpp"
#include "other/halo/haloconfig.hpp"
#include "other/shengong/shengongconfig.hpp"
#include "other/shenyi/shenyiconfig.hpp"
#include "other/fb/dailyfbconfig.hpp"
#include "globalconfig/otherconfig.h"
#include "globalconfig/otherglobalconfig.h"
#include "gameworld/netmsg/protocol/protocol_110.hpp"

#include "item/knapsack.h"
#include "item/hotkeyrecorder.h"
#include "friend/friendmanager.h"
#include "task/newtaskmanager.h"
#include "task/taskrecorder.h"
#include "other/rank/rankshadow.h"
#include "other/rolegoal/rolegoal.hpp"
#include "friend/teaminfo.hpp"
#include "other/mentality/mentality.hpp"
#include "other/vip/vip.hpp"
#include "other/shop/roleshop.hpp"
#include "other/capability/capability.hpp"
#include "other/chestshop/rolechestshop.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "other/fb/rolestoryfb.hpp"
#include "other/fb/rolemojiefb.hpp"
#include "other/convertshop/roleconvertshop.hpp"
#include "other/discountbuy/discountbuy.hpp"
#include "other/footprint/footprint.hpp"
#include "other/activedegree/activedegree.hpp"
#include "other/welfare/welfare.hpp"
#include "other/hangup/hangup.hpp"
#include "other/evaluate/roleevaluate.hpp"
#include "other/maze/mazemanager.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/halo/halomanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/monitor/monitor.hpp"
#include "other/sublock/sublock.hpp"
#include "other/monthcard/monthcard.h"
#include "other/p2ptrade/traderegister.h"
#include "other/fb/rolefbzhanshendian.hpp"
#include "other/fb/rolepatafb.hpp"
#include "other/fb/rolevipfb.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "equipment/stonemananger.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/wabao/wabao.hpp"
#include "other/qingyuan/qingyuan.hpp"
#include "other/baby/baby.hpp"
#include "other/touzijihua/touzijihua.hpp"
#include "other/commonactivity/commonactivityunit.hpp"
#include "other/fb/roledailyfb.hpp"
#include "other/lieming/liemingmanager.h"
#include "other/xiannvshouhu/xiannvshouhumanager.hpp"
#include "other/jinglingfazhen/jinglingfazhenmanager.hpp"
#include "other/jinglingguanghuan/jinglingguanghuanmanager.hpp"
#include "other/cardzu/cardzu.hpp"
#include "other/zhuanshengequip/zhuanshengequip.hpp"
#include "other/mojie/mojiemanager.hpp"
#include "other/shenbing/shenbingmanager.hpp"
#include "other/equipsuit/equipsuit.hpp"
#include "other/zerogift/zerogift.hpp"
#include "task/other/tumotask.hpp"
#include "task/other/husongtask.hpp"
#include "task/other/datingtask.hpp"
#include "task/other/cishatask.hpp"
#include "task/other/guildtask.hpp"
#include "other/fb/rolefbequip.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "other/yaojiang/roleyaojiang.hpp"
#include "other/specialappearance/specialappearance.hpp"
#include "other/guildxianshu/guildxianshu.hpp"
#include "other/supply/rolesupply.hpp"
#include "other/fb/rolefbchallenge.hpp"
#include "gameworld/gameworld/other/fb/rolefbphase.hpp"
#include "gameworld/doublesidefb/doublesidefbmanager.hpp"
#include "other/turntable/turntable.h"
#include "other/jilian/jilian.hpp"
#include "global/worldevent/roleworldevent.hpp"
#include "other/shizhuang/shizhuang.h"
#include "other/cross/rolecross.h"
#include "other/shengwang/shengwang.h"
#include "other/chengjiu/chengjiu.h"
#include "other/lifeskill/lifeskill.hpp"
#include "task/other/paohuantask.hpp"
#include "other/camp/gongxun.h"
#include "other/tuhaojin/tuhaojin.hpp"
#include "other/bigchatface/bigchatface.hpp"
#include "other/pet/pet.hpp"
#include "other/multimount/multimount.hpp"
#include "other/personalizewindow/personalizewindow.hpp"
#include "other/xunzhang/xunzhangmanager.hpp"
#include "other/zhibao/zhibaomanager.hpp"
#include "other/huashen/huashen.hpp"
#include "other/magiccard/magiccard.hpp"
#include "other/mitama/mitama.hpp"
#include "other/crossroleglobal/crossroleglobal.hpp"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include "other/friendgift/friendgiftmanager.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "other/followboss/followbossmanager.hpp"
#include "other/goldvip/goldvip.hpp"
#include "other/famousman/famousman.hpp"
#include "other/shengesystem/shengesystem.hpp"
#include "other/rollmoney/rollmoney.h"
#include "other/shenzhuang/shenzhuang.h"
#include "servercommon/serverconfig/crossguildbattleconfig.hpp"
#include "protocal/msgshenshou.h"
#include "other/shenshou/shenshou.hpp"
#include "global/fishpool/fishpool.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "scene/speciallogic/worldspecial/specialxingzuoyiji.hpp"
#include "other/fb/rolepreciousboss.hpp"
#include "crossserver/crossserver/protocal/msgcross.h"
#include "other/imgfuling/imgfuling.hpp"
#include "global/combineserveractivity/combineserverequip.hpp"
#include "speciallogic/worldspecial/specialcrosstuanzhan.hpp"
#include "other/shenyin/shenyin.hpp"
#include "protocal/msgelementheart.h"
#include "other/elementheart/elementheart.hpp"
#include "other/shengxingzhuli/shengxingzhulimanager.hpp"
#include "other/fabao/fabaomanager.hpp"
#include "other/challenge/rolechallengefield.hpp"
#include "other/jingjie/jingjie.hpp"
#include "global/droplog/droplogmanager.hpp"
#include "other/talent/talent.hpp"
#include "other/jinjietalent/jinjietalent.hpp"
#include "speciallogic/personspecial/specialarmordefend.hpp"
#include "other/shengqi/shengqi.hpp"
#include "other/fb/rolefbarmordefend.hpp"
#include "other/heshenluoshu/heshenluoshu.hpp"
#include "global/bossshanggu/bossshanggumanager.hpp"
#include "other/impguard/impguard.hpp"
#include "protocal/msgimpguard.h"
#include "global/rank/couplerankmanager.hpp"
#include "other/bosscard/bosscard.hpp"
#include "protocal/msgzhuanzhi.h"
#include "other/rolezhuanzhi/rolezhuanzhi.hpp"
#include "protocal/msgyaoshi.hpp"
#include "other/yaoshi/yaoshimanager.hpp"
#include "other/yaoshi/yaoshifunction.hpp"
#include "protocal/msgtoushi.hpp"
#include "other/toushi/toushimanager.hpp"
#include "other/toushi/toushifunction.hpp"
#include "protocal/msgqilinbi.hpp"
#include "other/qilinbi/qilinbimanager.hpp"
#include "other/qilinbi/qilinbifunction.hpp"
#include "protocal/msgmask.hpp"
#include "other/mask/maskmanager.hpp"
#include "other/mask/maskfunction.hpp"
#include "protocal/msgzhuanzhiequip.hpp"
#include "other/zhuanzhiequip/zhuanzhiequip.h"
#include "protocal/msgseal.hpp"
#include "protocal/msgupgrade.hpp"
#include "other/fb/roleteamfb.hpp"
#include "other/upgrade/upgrademanager.hpp"
#include "speciallogic/teamspecial/specialequipteam.hpp"
#include "global/activity/impl/activityworship.hpp"
#include "scene/speciallogic/personspecial/specialholidayguard.hpp"
#include "global/randactivity/randactivity.hpp"
#include "global/randactivity/impl/randactivityholidayguard.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "other/dressingroom/dressingroommgr.hpp"
#include "other/jinjiesysreward/jinjiesysrewardmgr.hpp"
#include "other/rolebigsmallgoal/rolebigsmallgoal.hpp"
#include "other/shenqi/shenqi.hpp"
#include "protocal/msgshenqi.hpp"
#include "other/redequipcollect/redequipcollect.hpp"
#include "other/orangeequipcollect/orangeequipcollect.hpp"
#include "other/molong/molong.h"
#include "other/xianzunka/xianzunka.hpp"
#include "speciallogic/personspecial/specialrunetower.hpp"
#include "roleequipfbmanager/roleequipfbmanager.hpp"
#include "protocal/msgspousehome.hpp"
#include "global/globalrole/spousehome/spousehome.hpp"
#include "other/equipbaptize/equipbaptize.hpp"
#include "global/bossfamily/bossfamilyconfig.hpp"
#include "other/fb/rolepersonbossfb.hpp"
#include "other/greatesoldier/greatesoldier.hpp"
#include "gameworld/gameworld/config/agentadapterconfig.hpp"
#include "other/upgradecardbuy/upgradecardbuy.hpp"
#include "other/huguozhili/huguozhili.hpp"
#include "other/crossgoal/crossgoal.hpp"
#include "global/fightback/fightbackmanager.hpp"
#include "other/baizhanequip/baizhanequip.h"
#include "speciallogic/worldspecial/specialcorssbianjingzhidi.h"
#include "gameworld/other/tianshuxunzhu/tianshuxunzhu.hpp"
#include "servercommon/serverconfig/crossbianjingzhidiconfig.h"
#include "other/littlehelper/littlehelper.h"
#include "speciallogic/worldspecial/specialqunxianluandou.hpp"
#include "protocal/msgcrossequip.h"
#include "other/crossequip/crossequip.hpp"
#include "protocal/msgmyth.h"
#include "other/myth/myth.hpp"
#include "protocal/msgzodiac.hpp"
#include "other/zodiac/zodiac.hpp"
#include "other/tianshenhuti/tianshenhuti.hpp"
#include "speciallogic/worldspecial/specialcrosshusongshuijing.hpp"

//#ifdef _WIN32
//	#include <time.h>
//	#include <windows.h>
//#else
//	#include <sys/time.h>
//#endif

bool MessageHandler::is_print_msg_type_from_client = false;

// 进入场景
void MessageHandler::OnReqEnter(const GSNetID &netid, IP userip, void *msg)
{
	if (m_scene_manager->GetOnlineRoleNum() >= m_scene_manager->GetMaxRoleNum()) // 登录人数超过允许上限
	{
		Protocol::SCServerBusy sb;
		EngineAdapter::Instance().NetSend(netid, (const char*)&sb, sizeof(sb));

		return;
	}

	Protocol::CSUserEnterGSReq *ul = (Protocol::CSUserEnterGSReq*)msg;

	UserID user_id = IntToUid(ul->role_id);

	PlatName pname;
	F_STRNCPY(pname, ul->pname, sizeof(PlatName));
	PlatNameCatPostfix(pname, ul->server);

	GameName role_name;
	F_STRNCPY(role_name, ul->role_name, sizeof(GameName));

	SessionKey key;
	GetSessionKey(InternalComm::Instance().m_session_key_base, ul->scene_id, ul->scene_key, ul->last_scene_id, ul->role_id, role_name, 
		ul->time, ul->is_login, pname, key);
	if (memcmp(key, ul->key, sizeof(SessionKey)) != 0) // 验证失败 果断地把你断开
	{
		EngineAdapter::Instance().NetDisconnect(netid, user_id, role_name, ul->scene_id, "SessionKeyError");
		return;
	}

	static const int PLAT_TYPE = LocalConfig::Instance().GetPlatType();

	CrossUserMap &cross_user_map = World::GetInstWorld()->GetSceneManager()->GetCrossUserMap();
	if (cross_user_map.find(UniqueUserIDToLongLong(UniqueUserID(PLAT_TYPE, user_id))) != cross_user_map.end())
	{
		crossgameprotocal::GameCrossKickCrossUser gckcu;
		gckcu.bekick_original_uuid = UniqueUserIDToLongLong(UniqueUserID(PLAT_TYPE, user_id));
		//InternalComm::Instance().SendToCross((const char *)&gckcu, sizeof(gckcu));
		CrossManager::Instance().SendToAllCross((const char *)&gckcu, sizeof(gckcu));

		Protocol::SCUserEnterGSAck login_ack;
		login_ack.result = Protocol::ENTERGS_USER_EXIST;
		EngineAdapter::Instance().NetSend(netid, (char *)&login_ack, sizeof(Protocol::SCUserEnterGSAck));
		return;
	}

	Role *role = m_scene_manager->GetRole(user_id);
	if (NULL != role)
	{
		{
			// 发给新上来的
			Protocol::SCUserEnterGSAck login_ack;
			login_ack.result = Protocol::ENTERGS_USER_EXIST;
			EngineAdapter::Instance().NetSend(netid, (char *)&login_ack, sizeof(Protocol::SCUserEnterGSAck));
		}

		{
			// 发给已经在的 让他下线
			Protocol::SCDisconnectNotice notice;
			notice.reason = DISCONNECT_NOTICE_TYPE_LOGIN_OTHER_PLACE;
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&notice, sizeof(notice));
		}

		if (!role->IsForceKickDelay()) // 如果这个玩家没有被T过 
		{
			role->SetForceKickDelay(); // 切换场景会导致这个值失效 导致没有办法踢玩家下线 但是踢人这一端可以不断重试

			ForceKickUserDelay *timer = new ForceKickUserDelay(role->GetUserId(), DISCONNECT_NOTICE_TYPE_LOGIN_OTHER_PLACE);  // 定时器启动 一定会T 不能相信客户端
			EngineAdapter::Instance().CreateTimerSecond(1, timer);
		}

		return;
	}

	int use_key = ul->scene_key;
	if (1 == ul->is_login) 
	{
		// 经验副本（可以个人进入，也可以组队进入），场景key是递增的，要用到组队副本的取key方法
		const DailyFbItem *fb_cfg = LOGIC_CONFIG->GetDailyFbCfg().GetDailyFbItemCfg(DAILYFB_TYPE_EXP);
		if (NULL != fb_cfg && fb_cfg->scene_id == ul->scene_id)
		{
			use_key = TeamKeyRegister::Instance().GetTeamSceneKey(user_id);
		}
		else if (LOGIC_CONFIG->GetBossFaimlyConfig().IsCrossVipBossScene(ul->scene_id))  // vip跨服boss断线回原服野外场景
		{
			use_key = -1;
		}
		else if (Scene::IsStaticScene(m_scene_manager->GetSceneTypeWithID(ul->scene_id)))
		{
			ul->last_scene_id = ul->scene_id;
			use_key = -1;
		}
		else if (m_scene_manager->CheckIsDelayDeleteScene(ul->scene_id))// 目前仅仅允许单人副本 进行延迟删除 
		{
			use_key = ul->role_id;
		}
		else if (m_scene_manager->CheckIsTeamScene(ul->scene_id))		// 去register查找角色 组队副本相关的key
		{
			use_key = TeamKeyRegister::Instance().GetTeamSceneKey(user_id);
		}
		else if (m_scene_manager->CheckIsGuildScene(ul->scene_id))		// 去register查找角色 军团副本相关的key
		{
			use_key = GuildKeyRegister::Instance().GetGuildSceneKey(user_id);
		}
		else if (m_scene_manager->CheckIsYaoShouGuangChangScene(ul->scene_id))
		{
			use_key = YaoShouGuangChangMgr::Instance().GetUserRoomKey(user_id);
		}
		else if (m_scene_manager->CheckIsSuoYaoTaScene(ul->scene_id))
		{
			use_key = SuoYaoTaMgr::Instance().GetUserRoomKey(user_id);
		}
		else if (m_scene_manager->CheckIsBossScene(ul->scene_id))
		{
			use_key = COMMON_SCENE_KEY;
		}
	}

	{
		if (MarryShadow::Instance().IsMarryUser(user_id))
		{
			int marry_scene_id = 0;
			if (MarryShadow::Instance().GetMarrySceneId(&marry_scene_id))
			{
				ul->scene_id = marry_scene_id;
			}
		}
	}

	Scene *scene = m_scene_manager->GetSceneById(ul->scene_id, use_key);

	if (NULL == scene)
	{
		use_key = -1;
		SceneLineInfo *line_info = SceneRegister::Instance().Instance().GetSceneLineInfo(ul->last_scene_id);
		if (NULL == line_info || line_info->max_line <= 0)
		{
			use_key = 0;
		}
		else
		{
			for (int i = 0; i < line_info->open_line && i < 100; i++)
			{
				Scene *temp_scene = m_scene_manager->GetSceneById(ul->last_scene_id, i);
				if (NULL != temp_scene && (int)temp_scene->RoleNum() < line_info->line_role_count)
				{
					use_key = i;
					break;
				}
			}

			if (use_key < 0)
			{
				if (line_info->open_line < line_info->max_line)
				{
					use_key = line_info->open_line;
					line_info->open_line++;
				}
				else
				{
					use_key = RandomNum(line_info->max_line);
				}
			}
		}

		scene = m_scene_manager->GetSceneById(ul->last_scene_id, use_key);
		if (NULL == scene)
		{
			m_scene_manager->CreateFB(ul->last_scene_id, use_key, &scene);
		}
	}
	if (NULL != scene)
	{
		Protocol::SCUserEnterGSAck login_ack;
		login_ack.result = Protocol::ENTERGS_RESULT_SUC;
		EngineAdapter::Instance().NetSend(netid, (char *)&login_ack, sizeof(Protocol::SCUserEnterGSAck));

		scene->RoleEnterSceneAsyn((0 != ul->is_micro_pc), netid, userip, user_id, pname, true, ul->plat_spid);
		return;
	}

	// 场景不存在
	Protocol::SCUserEnterGSAck login_ack;
	login_ack.result = Protocol::ENTERGS_RESULT_SCENE_NO_EXIST;
	EngineAdapter::Instance().NetSend(netid, (char *)&login_ack, sizeof(Protocol::SCUserEnterGSAck));
}

Role * MessageHandler::GetRoleHelper(Scene *scene, ObjID obj_id, const char *funstr)
{
	if (NULL == scene)
	{
		return NULL;
	}

	Obj *role_obj = scene->GetObj(obj_id);
	if (NULL == role_obj)
	{
		gamelog::g_log_messagehandler.printf(LL_DEBUG, "Scene: [%d]%s %s obj_id:%d is not exist in scene.", scene->GetSceneID(), scene->GetSceneName(), funstr, obj_id);
		return NULL;
	}

	if (role_obj->GetObjType() != Obj::OBJ_TYPE_ROLE)
	{
		gamelog::g_log_messagehandler.printf(LL_DEBUG, "Scene: [%d]%s %s obj_id:%d is not a role.", scene->GetSceneID(), scene->GetSceneName(), funstr, obj_id);
		return NULL;
	}

	return (Role*)role_obj;
}

// 获取服务器时间
void MessageHandler::OnTimeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->SendServerTime();
}

void MessageHandler::OnYiZhanDaoDiGuWu(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSYiZhanDaoDiGuwu* yzddgw = (Protocol::CSYiZhanDaoDiGuwu*)msg;
	role->GetRoleActivity()->YiZhanDaoDiGuWu(yzddgw);
}

void MessageHandler::OnWuXingGuaJiGetReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSWuXingGuaJiGetReward * wxgjigr = (Protocol::CSWuXingGuaJiGetReward*)msg;
	role->GetRoleActivity()->WuXingGuaJiGetReward(wxgjigr);
}

// 退出游戏
void MessageHandler::OnLogout(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	m_scene_manager->Logout(scene, obj_id);

	EngineAdapter::Instance().NetDisconnect(netid, role->GetUserId(), role->GetName(), scene->GetSceneID(), "Logout");
}

// 角色移动
void MessageHandler::OnRoleMove(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSObjMove *um = (Protocol::CSObjMove*)msg;

	bool role_cam_move = true;

	if (Role::MOVE_MODE_JUMP == role->GetMoveMode() || Role::MOVE_MODE_JUMP2 == role->GetMoveMode())
	{
		//if (abs(um->pos_x - role->GetJumpSrcPos().x) >= 100 || abs(um->pos_y - role->GetJumpSrcPos().y) >= 100)
		//{
		//	return;
		//}
	}

	if (0 != um->is_press_onward)
	{
		if (role->GetMultiMount()->IsOnMount())
		{
			return;
		}

		//if (role->GetCoupleMount()->IsOnMount())
		//{
		//	return;
		//}
	}

	MultiMountBindItem *bind_info = role->GetMultiMount()->GetMountOnInfo();
	if (role->GetMultiMount()->IsOnMount() && NULL != bind_info) // 双人坐骑判断
	{
		if (bind_info->owner_uid == role->GetUID())
		{
			role_cam_move = true;

			Role *follower_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(bind_info->follower_uid));
			if (MULTI_MOUNT_RIDE_STATUS_TOW_RIDE == bind_info->status && NULL != follower_role)
			{
				// 跟随者跟着走动
				follower_role->Move(um->pos_x, um->pos_y, um->dir, um->distance, um->height, follower_role->GetLevel() > 10);
			}
		}
		else // 不是坐骑拥有者，不给移动
		{
			role_cam_move = false;
		}
	}

	if (role_cam_move)
	{
		role->Move(um->pos_x, um->pos_y, um->dir, um->distance, um->height, role->GetLevel() > 10 || role->GetMoveMode() == Role::MOVE_MODE_NORMAL, um->is_press_onward);
	}

	if (um->distance >= 0.0001)
	{
		role->StatMoveReq();
	}	
}


// 释放技能
void MessageHandler::OnPerformSkill(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSPerformSkillReq *ps = (Protocol::CSPerformSkillReq*)msg;

	// 客户端和服务器端的位置不一致时，发送位置给客户端修正
	if (ps->client_pos_x != role->GetPos().x || ps->client_pos_y != role->GetPos().y)
	{
		Protocol::SCFixPos fp;
		fp.x = role->GetPos().x;
		fp.y = role->GetPos().y;
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&fp, sizeof(fp));
	}

	role->PerformSkill(ps->skill_index, Posi(ps->pos_x, ps->pos_y), ps->target_id, 0 != ps->is_specialskill, ps->skill_data);
}

void MessageHandler::OnTurnTableRollReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTurnTableReq *ttr = (Protocol::CSTurnTableReq *)msg;

	if (0 == ttr->is_roll)
	{
		role->GetTurnTable()->SendInfo();
	}
	else
	{
		role->GetTurnTable()->OnRoll(ttr->type);
	}
}

// 设置攻击模式
void MessageHandler::OnSetAttackMode(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSetAttackMode *sam = (Protocol::CSSetAttackMode*)msg;
	role->SetAttackMode(sam->mode, false, (0 != sam->is_fanji));
}

void MessageHandler::OnRoleInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

// 获取所有角色信息
void MessageHandler::OnAllInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSAllInfoReq *air = (Protocol::CSAllInfoReq*)msg;

	if (LocalConfig::Instance().IsShenheServer() && role->GetOnlineTime() <= 0)
	{
		//role->ShenheServerRandomRole();

		//m_scene_manager->GotoRandomStaticScene(role);
	}

	role->SendInfoMsg();
	role->GetSkillManager()->SendSkillListInfo(true);
	role->GetSkillManager()->SendOtherSkillInfo();

	role->GetHotkeyRecorder()->SendHotKeyListToRole();

	role->GetKnapsack()->SendKnapsackInfoToRole();

	role->GetEquipmentManager()->SendEquipmentInfoToRole();
	role->GetStoneManager()->SendToClient();
	role->GetXunZhangManager()->SendAllXunZhangInfo();
	role->GetZhiBaoManager()->SendAllZhiBaoInfo();
	role->GetShenBingManager()->SendAllShenBingInfo();
	role->GetQingyuan()->SendBlessInfo();
	role->GetQingyuan()->SendHunjieEquipInfo();
	role->GetQingyuan()->SendCoupleHaloInfo();
	role->GetQingyuan()->SendQingYuanShengDiInfo();

	role->GetTaskManager()->SendTaskListToRole();	
	role->GetTaskManager()->SendAcceptableToRole();
	role->GetTaskRecorder()->SendTaskRecorderListToRole();

	role->GetFriendManager()->SendFriendListToRole();
	role->GetFriendManager()->SendBlackListToRole();
	role->GetFriendManager()->SendEnemyListToRole();
	role->GetFriendManager()->SendAllCharm();

	role->GetTitleManager()->SendAllTitle();
	role->GetTitleManager()->SendUsedTitle();

	role->GetMentality()->SendMentalityInfo();

	role->GetActiveDegree()->SendActiveDegreeInfo();
	role->GetDayCounter()->SendDayCounterInfo();

	//	role->GetHangup()->SendHangupInfo(); 
	//	role->GetKnapsack()->GetItemColddownManager()->SendItemColddown();
	role->GetMonthCard()->SendInfo();

	role->GetEffectList()->SendEffectList();
	//role->GetCapability()->SendRoleCapability();
	role->GetSublock()->SendSubLockInfo();

	role->GetMountManager()->SendMountInfo();
	role->GetFightMountManager()->SendFightMountInfo();
	role->GetFabaoManager()->SendInfo();
	role->GetWing()->SendWingInfo();
	role->GetHalo()->SendHaloInfo();
	role->GetShengong()->SendShengongInfo();
	role->GetShenyi()->SendShenyiInfo();
	role->GetFootPrint()->SendFootprintInfo();
	role->GetCloakManager()->SendCloakInfo();
	role->GetRoleChestShop()->SendFreeInfo();
	//	role->GetZhanShenDian()->SendInfo();
	role->GetVip()->SendVipInfo();
	role->GetWabao()->SendWabaoInfo();
	role->SendAdvanceNoticInfo();
	role->SendAdvanceNoticDayInfo();

	role->GetRoleActivity()->SendChongZhiInfo();
	role->GetRoleActivity()->OnGetBattleFieldHonorReq();
	role->GetRoleActivity()->SendUpdateNoticeInfo();
	role->GetRoleActivity()->SendBaiBeiFanLiInfo();
	role->GetRoleActivity()->SendBaiBeiFanLi2Info();
	role->GetRoleActivity()->SendRARedEnvelopeGiftInfo();

	WorldStatus::Instance().SendGongChengZhangOwnerInfo(role);
	WorldStatus::Instance().SendChestShopRecord(role, CHEST_SHOP_TYPE_EQUIP);
	WorldStatus::Instance().SendChestShopRecord(role, CHEST_SHOP_TYPE_JINGLING);
	role->GetFBEquip()->SendNeqFBInfo();
	role->GetSpecialAppearance()->SendAppearance(false);

	//	role->SendWeddingRingSkillInfo();
	FightBackManager::Instance().SendRoleList(role);

	role->GetXiannvManager()->SendAllXiannvInfo();
	role->GetRoleSupply()->SendSupplyInfo();
	//	role->GetMaze()->OnQueryMaze();
	role->GetCishaTask()->SendInfo();
	//	role->GetJilian()->SendInfo();
	role->GetTouzijihua()->SendInfo();
	role->GetWelfare()->SendTotalLoginDays();
	role->GetWelfare()->SendSevenDayLoginRewardInfo();
	//	role->GetCommonActivityUnitMgr()->SendCommonActivityState();
	role->GetShizhuang()->SendInfo();
	role->GetRoleGuild()->SendRoleGuildInfo();
	role->GetTeamInfo()->SyncRoleTeamInfo();
	role->SyncNorexItemDayTimes();
	role->SendSpaceSelfInfo();
	role->GetTuHaoJin()->SendJinguaInfo();
	role->GetBigChatFace()->SendBigChatFaceAllInfo();
	//	role->GetPet()->SendPetInfo();
	//	role->GetPet()->SendPetQinmiAllInfo();
	//	role->GetMultiMount()->SendAllInfoToRole();
	role->GetPersonalizeWindow()->SendBubbleWindowInfo();
	role->GetPersonalizeWindow()->SendAvatarWindowInfo();
	role->GetRoleCross()->SendCrossXiuluoTowerInfo();
	role->GetRoleCross()->SendCross1V1SelfInfo();
	role->GetRoleCross()->SendCrossMultiuserChallengeSelfActivityInfo();
	role->GetRoleCross()->SendCrossBossSelfInfo();
	role->GetRoleCross()->SendCrossTeamRBInfo();
	role->GetJingLingMgr()->SyncJingLingInfo();
	role->GetJingLingMgr()->SendExploreInfo();
	role->GetCardzu()->SendCardzuAllInfo();
	role->GetLieMingMgr()->SendEquipInfo();
	//role->GetLieMingMgr()->SendBagInfo();
	role->GetHuaShen()->SendHuaShenAllInfo();
	role->GetHuaShen()->SendHuashenSpiritAllInfo();
	role->GetMagicCard()->SendAllInfo();
	role->GetMitama()->SendAllInfo();
	role->GetShenzhouWeapon()->SendBaseInfo();
	role->GetRoleGoal()->SendInfoToRole();
	role->GetXiannvShouhu()->SendXiannvShouhuInfo();
	role->GetJinglingFazhen()->SendJinglingFazhenInfo();
	role->GetJinglingGuanghuan()->SendJinglingGuanghuanInfo();
	//role->GetZhuanShengEquip()->SendAllInfo();
	role->GetChineseZodiac()->SendChineseZodiacAllInfo();
	role->GetFBMojie()->SendInfo();
	role->GetFriendGiftManager()->SendFriendGiftInfo();
	role->GetRoleChestShop()->SendScoreInfo();
	role->GetHusongTask()->SendHusongTaskInfo();
	role->GetEquipSuit()->SendToClient();
	role->GetShenZhuang()->SyncShenZhuangInfo();
	role->GetMagicalPreciousManager()->SendMagicalPreciousAllInfo();
	role->GetGoldVip()->SendGoldVipInfo();
	role->GetFamousMan()->SendFamousManInfo();
	role->SendWorldBossWearyInfo();
	role->GetZeroGift()->SendZeroGiftInfo();
	role->GetZeroGift()->SendZeroGiftGodCostumeInfo();
	role->GetFBTowerDefend()->SendInfo();
	BossFamilyManager::Instance().SendBossRoleInfo(role);
	BossFamilyManager::Instance().OnFamilyBossInfoReq(role);
	BossFamilyManager::Instance().OnMikuBossInfoReq(role);
	BossFamilyManager::Instance().OnDabaoBossInfoReq(role);
	BossFamilyManager::Instance().OnActiveBossInfoReq(role);
	JinglingAdvantage::Instance().OnAdvantageReq(role);
	role->GetFollowBossManager()->SendFollowBossList();
	role->GetRoleActivity()->SendRAExpRefineInfo();
	role->GetRuneSystem()->SendAllInfo();
	role->GetShengeSystem()->SendAllInfo();
	FBManager::Instance().SendTeamEquipFBCountInfo(role);
	role->SendGradeSkillInfo(role);
	role->GetRoleGuild()->OnLoginNoticeGuildRedPaperInfo();
	WorldStatus::Instance().SendYizhanDaoDiKingUserId(role);
	WorldStatus2::Instance().SendWorldAcquisitionLog(Protocol::SCWorldAcquisitionLog::NOTICE_TYPE_ALL, role);
	role->GetMultiMount()->SendAllInfoToRole();
	role->GetCSAEquip()->SyncCSAEquipInfo();
	role->GetShenYin()->SendAllInfo();
	role->GetElementHeart()->SendAllInfo();
	role->GetHeShengLuoShu()->SendHeShenLuoShuAllInfo();
	role->GetImpGuard()->SendImpGuardAllInfo();
	role->GetBaby()->SendAllBabyInfo();
	role->GetZhuanzhiEquip()->SyncZhuanzhiEquipInfo();
	role->GetZhuanzhiEquip()->SyncStoneInfo();
	role->GetZhuanzhiEquip()->SyncZhuanzhiSuitInfo();
	role->GetSeal()->SendAllInfo();
	role->GetRoleZhuanZhi()->SendRoleZhuanZhiInfo();
	role->GetRedEquipCollect()->SendAllInfo();
	role->GetOrangeEquipCollect()->SendAllInfo();
	role->GetMolong()->SendInfo();
	role->GetXianZunKa()->SendAllInfo();
	role->GetRoleCross()->SendCrossMizangBossSelfInfo();
	role->GetRoleCross()->SendCrossYoumingBossSelfInfo();
	role->GetBaizhanEquip()->SyncBaizhanEquipAllInfo();
	role->GetMyth()->SendChpaterAllInfo();
	role->GetZodiac()->SendAllInfo();
	role->GetCrossEquip()->SendAllInfo();
	role->GetCrossEquip()->SendAllEquipInfo();
	role->GetOtherMgr()->OnAllInfoReq();

	ActivityShadow::Instance().SendGuildQuestionQuestionInfo(0, role);

	{
		// 随机活动角色上线下发信息统一接口
		role->GetRoleActivity()->SendRandActivityInfo();
	}

	RedPaperManager::Instance().SendRedPaperRoleInfo(role);

	WorldStatus::Instance().BrocastWorldQuestion(role);
	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL != guild)
	{
		 guild->BrocastGuildQuestion(role);
	     guild->SendGuildQuestionRankInfo(role);
		 guild->SendSigninAllInfo(role);
		 guild->GetStorge()->SyncStorgeInfo(role);
	}
	role->SendQuestionRightAnswerNum();

	LittlePet * little_pet = GLOBAL_ROLE_MGR.GetLittlePet(role->GetUID());
	if (NULL != little_pet)
	{
		little_pet->SendAllInfo();
	}

	LoveTree * my_love_tree = GLOBAL_ROLE_MGR.GetLoveTree(role->GetUID());
	if (NULL != my_love_tree)
	{
		my_love_tree->SendLoveTreeInfo(role, true);
	}

	QingyuanGlobal *qingyuan_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(role->GetUID());
	if (NULL != qingyuan_global)
	{
		qingyuan_global->SendQingyuanGlobalInfo(role, true);
	}

	LOGIC_CONFIG->GetMysteriousShop().SyncRefreshNpcPosToRole(role);

	if (1 == air->is_send_chat_board)
	{
		ChatManager::Instance().CheckSingleChatBoard(role);
		ChatManager::Instance().CheckGuildChatBoard(role);
		ChatManager::Instance().CheckWorldChatBoard(role);
	}

	ChatManager::Instance().SyncChatOpenLevelToClient(role);	// 同步聊天等级给客户端

	FBManager::Instance().SendHunYanFbInviteInfoToRole(role);

	role->GetFeixianManager()->SyncEquipFeixianInfo();
	//进阶系统大小目标
	JinjieSysRewardMgr::Instance().SendJinjieSysRewardInfo(role);
	JinjieSysRewardMgr::Instance().SendJinjieSysRewardTimestamp(role);

	role->GetGreateSoldier()->SendAllInfo();
	role->GetHuguozhili()->SendHuguozhiliInfo();

	role->GetRuneSystem()->SendRuneTowerInfo();
	role->GetWelfare()->SendWelfareInfo();
	role->GetRoleActivity()->SendFetchTodayThemeRewardInfo();
	role->GetRoleActivity()->SyncFirstRechargeBuffInfo();
	role->GetZhuanzhiEquip()->SyncAwakeningAllInfo();
	role->GetQingyuan()->SendYanhuiFBKey();		// 必须在最后 客户端依赖 新添加要放在前面
	role->GetFriendManager()->SendSongHuaInfo();
	// 变身消息广播
	role->NotifyAcceptTaskAppearn();

	role->OnSendAllInfo();

	ActivityShadow::Instance().OnCrossGuildBattleSpecialTimeNotice(role);

	if (!role->IsInCross())
	{
		role->GetRoleCross()->OnCrossTeamFBOption(CROSS_TEAM_FB_STATE_OPTION_ENTER, 0, 0, 0);
	}

	Activity *activity = ActivityManager::Instance().ForceGetActivity(ACTIVITY_TYPE_WORSHIP);
	if (nullptr != activity && activity->GetActivityType() == ACTIVITY_TYPE_WORSHIP)
	{
		ActivityWorship *gcz_activity = (ActivityWorship*)activity;
		gcz_activity->SendWorshipInfo(role);
	}

	// ------------------------------------------------------------------------------------------------------------------
	// 检测跨服活动状态的放在最后，因为接下来可能会触发到服务器强拉玩家进跨服
	//role->GetRoleCross()->CheckCrossServerCross3v3MatchingState();
	//role->GetRoleCross()->CheckCanRetransferToCrossOnRoleLogin();

}

void MessageHandler::OnHeartBeat(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	// 已经不用了， 用时间那个协议
}

// 角色复活
void MessageHandler::OnRoleReAlive(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRoleReAliveReq *rraq = (Protocol::CSRoleReAliveReq*)msg;
	if (role->IsAlive()) 
	{
		return;
	}

	const int ROLE_REALIVE_INTERVAL = GLOBALCONFIG->GetRoleReAliveInterval();
	if (role->GetDeadTime() < ROLE_REALIVE_INTERVAL)
	{
		return;
	}

	if (!scene->GetSpecialLogic()->SpecialCanReAlive(role, rraq->realive_type))
	{
		return;
	}

	if (scene->GetSpecialLogic()->SpecialRelive(role, rraq->realive_type))
	{
		EventHandler::Instance().OnRoleRealiveSucc(role, rraq->realive_type);
	}
	else 
	{
		if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME != rraq->realive_type)  //只要是原地复活，就考虑用vip的免费原地复活次数
		{
			int vip_free_realive_times = role->GetDayCounter()->GetDayCount(DayCounter::DAYCOUNT_ID_VIP_FREE_REALIVE);
			int vip_max_free_realive_times = LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_BUY_FREE_REVIVAL_TIMES);
			if (vip_free_realive_times < vip_max_free_realive_times) 
			{
				role->GetDayCounter()->DayCountIncrease(DayCounter::DAYCOUNT_ID_VIP_FREE_REALIVE);
				role->ReAlive(100, 100, role->GetPos(), true);
				EventHandler::Instance().OnRoleRealiveSucc(role, rraq->realive_type);
				return;
			}
		}

		if (Protocol::CSRoleReAliveReq::REALIVE_TYPE_BACK_HOME == rraq->realive_type)		
		{
			if (Scene::IsBossDongKuScene(role->GetScene()->GetSceneID()))
			{
				if (!role->CanRealiveWithMiKuTire())
				{
					return;
				}
				role->DieOnMiKuScene();
			}

			if (1 == rraq->is_timeout_req)
			{
				role->SendBeKillMail();
			}

			EventHandler::Instance().OnRoleRealiveSucc(role, rraq->realive_type);
			role->ReAliveBackHome();
		}
		else if(Protocol::CSRoleReAliveReq::REALIVE_TYPE_HERE_GOLD == rraq->realive_type) // 使用钻石
		{
			bool is_guild_times_relive = false;		
			
			// 注释公会复活
// 			if(INVALID_GUILD_ID != role->GetGuildID())
// 			{
// 				Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
// 				if (NULL != guild)
// 				{
// 					GuildCommonDataParam *guild_common_data = guild->GetGuildCommonDataParam();
// 					if (NULL != guild_common_data)
// 					{
// 						int relive_times = LOGIC_CONFIG->GetGuildConfig().GetPostReliveTimes(role->GetGuildPost());
// 
// 						if (guild_common_data->daily_guild_relive_times > 0)
// 						{	
// 							if (role->GetDailyUseGuildReliveTimes() < relive_times)
// 							{
// 								guild_common_data->daily_guild_relive_times--;
// 								role->AddDailyUseGuildReliveTimes();
// 								is_guild_times_relive = true;
// 								guild->SyncGulidReliveTimesInfo();
// 								role->SyncSCGulidBossRedbagInfo();
// 							}
// 							else
// 							{
// 								//role->NoticeNum(errornum::EN_GUILD_RELIVE_SELF_NOT_TIMES);
// 							}
// 						}
// 						else
// 						{
// 							//role->NoticeNum(errornum::EN_GUILD_RELIVE_ALL_NOT_TIMES);
// 						}
// 					}
// 				}
// 			}
			
			// 每天免费复活
			//1.每天免费复活次数2次确保标志在人物身上，不管在本服和跨服还有副本都是公用的
			//2.如果有复活物品（26900）就优先使用物品。没有的话就使用元宝复活（只能用非绑元宝）
			CommonDataParam *common_p = role->GetCommonDataParam();
			if (common_p != NULL)
			{
				const OtherOtherConfig &other_cfg = LOGIC_CONFIG->GetOtherCfg().GetOtherCfg();
				if(common_p->today_use_free_relive_num < other_cfg.today_free_relive_num)
				{	
					common_p->today_use_free_relive_num++;

					role->SyncCommonInfo(Protocol::SCIT_TODAY_FREE_RELIVE_NUM, common_p->today_use_free_relive_num);
					
					is_guild_times_relive = true;
				}
			}
	
			if (!is_guild_times_relive)
			{
				if (role->GetSublock()->CheckHasLock()) return;

				if (role->GetKnapsack()->IsLock())
				{
					role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
					return;
				}

				// 计算消耗
				static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
				{
					const ItemID REALIVE_ITEM_ID = GLOBALCONFIG->GetRealiveItemId();

					ItemExtern::ItemConsumeConfig stuff;
					stuff.item_id = REALIVE_ITEM_ID;
					stuff.num = 1;
					stuff.buyable = false;

					if (!ItemExtern::GetConsumeListByOrder(role, false, 1, &stuff, &consume_list, false))
					{
						int relive_need_gold = LOGIC_CONFIG->GetOtherCfg().GetOtherCfg().cross_relive_gold;
						if (relive_need_gold > 0 && !role->GetKnapsack()->GetMoney()->GoldMoreThan(relive_need_gold))
						{
							role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
							return;
						}

						if (!role->GetKnapsack()->GetMoney()->UseGold(relive_need_gold, "RealivelCost"))
						{
							return ;
						}
					}
					else
					{
						if (!ItemExtern::ConsumeItem(role, consume_list, "RealivelCost"))
						{
							return;
						}
					}

// 					if (consume_list.need_gold_num > 0 && !role->GetKnapsack()->GetMoney()->AllGoldMoreThan(consume_list.need_gold_num))
// 					{
// 						role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
// 						return;
// 					}
				}

// 				{
// 					// 消耗
// 					if (consume_list.count > 0)
// 					{
// 						if (!role->GetKnapsack()->ConsumeItemByIndexList(consume_list.count, consume_list.index_list, consume_list.num_list, "RevivalCost"))
// 						{
// 							return;
// 						}
// 					}
// 
// 					if (consume_list.need_gold_num > 0) 
// 					{
// 						long long consume_gold = consume_list.need_gold_num, consume_gold_bind = 0;
// 						if (!role->GetKnapsack()->GetMoney()->UseAllGold(consume_list.need_gold_num, "RevivalCost")) 
// 						{
// 							return;
// 						}
// 
// 						EventHandler::Instance().OnShopBuyItem(role, consume_list.buy_count, consume_list.buy_list, consume_gold, consume_gold_bind);
// 					}
// 				}
 			}

			// 生效
			{
				role->AddOrResetDayRevivalTimes(false);
				role->ReAlive(100, 100, role->GetPos(), true);

				EventHandler::Instance().OnRoleRealiveSucc(role, rraq->realive_type);
			}
		}
	}
}

// 获取效果列表
void MessageHandler::OnGetEffectListReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGetEffectListReq *gelr = (Protocol::CSGetEffectListReq*)msg;
	Obj *target_obj = role->GetScene()->GetObj(gelr->target_obj_id);
	if (NULL != target_obj && Obj::IsCharactor(target_obj->GetObjType()))
	{
		Character *target_cha = (Character*)target_obj;
		target_cha->GetEffectList()->SendEffectList(role);
	}
}

// 获取效果信息
void MessageHandler::OnGetEffectInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGetEffectInfoReq *geir = (Protocol::CSGetEffectInfoReq*)msg;
	Obj *target_obj = role->GetScene()->GetObj(geir->target_obj_id);
	if (NULL != target_obj && Obj::IsCharactor(target_obj->GetObjType()))
	{
		Character *target_cha = (Character*)target_obj;
		target_cha->GetEffectList()->OnEffectInfoReq(geir->effect_type, geir->effect_key, role);
	}
}

void MessageHandler::OnChangePoseReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnPerformQTEReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

// 开始采集
void MessageHandler::OnStartGatherReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSStartGatherReq *sgr = (Protocol::CSStartGatherReq*)msg;
	if (sgr->gather_count < 0) return;

	role->StartGather(sgr->gather_obj_id, (sgr->gather_count > 0 ? sgr->gather_count : 1));
}

// 停止采集
void MessageHandler::OnStopGatherReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	if (Role::ROLE_STATUS_GATHER == role->GetRoleStatus())
	{
		role->ResetRoleStatus();
	}
}

// 休息
void MessageHandler::OnRestReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->OnRest();
}

void MessageHandler::OnRestInviteReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSInviteRestReq *irr = (Protocol::CSInviteRestReq*)msg;
	role->OnRestInvite(irr->obj_id);
}

void MessageHandler::OnRestInviteAck(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSInviteRestAck *ira = (Protocol::CSInviteRestAck*)msg;
	bool is_accept = ira->is_accept > 0 ? true : false;
	role->OnRestInviteAck(ira->obj_id, is_accept);
}

void MessageHandler::OnRestStartAffirm(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRestStartAffirm *rsa = (Protocol::CSRestStartAffirm*)msg;
	role->OnRestStartAffirm(rsa->partner_obj_id);
}

void MessageHandler::OnTeamCallTogether(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{	
	role->GetTeamInfo()->OnTeamCallTogether();
}

void MessageHandler::OnTeamCallIn(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTeamCallIn *tci = (Protocol::CSTeamCallIn*)msg;
	role->GetTeamInfo()->OnTeamCallIn(tci->fb_id);
}

// 学习角色技能
void MessageHandler::OnLearnSkillRole(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRoleSkillLearnReq *slq = (Protocol::CSRoleSkillLearnReq*)msg;
	if (Protocol::CSRoleSkillLearnReq::UPLEVEL_SKILL == slq->req_type)
	{
		SkillFunction::LearnSkill(role, slq->skill_id);
	}
	else if(Protocol::CSRoleSkillLearnReq::UPLEVEL_ACTIVE_SKILL_ALL == slq->req_type)
	{
		SkillFunction::LearnSkillActiveAll(role);
	}
	else if(Protocol::CSRoleSkillLearnReq::UPLEVEL_PASSIVE_SKILL_ALL == slq->req_type)
	{
		SkillFunction::LearnSkillPassiveAll(role);
	}
}

// 技能CD
void MessageHandler::OnSkillCD(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSkillCDReq *scd = (Protocol::CSSkillCDReq*)msg;
	role->GetSkillManager()->StartCD(scd->skill_index);
}

// 获取所有对象移动信息
void MessageHandler::OnGetAllObjMoveInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGetAllObjMoveInfoReq *gaomir = (Protocol::CSGetAllObjMoveInfoReq*)msg;
	SceneRegister::Instance().GetAllObjMoveInfo(role, gaomir);
}

// 捡金币
void MessageHandler::OnPickBigCoin(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSPickBigCoin *pbc = (Protocol::CSPickBigCoin*)msg;

	Obj *obj = scene->GetObj(pbc->obj_id);
	if (NULL == obj)
	{
		gamelog::g_log_messagehandler.printf(LL_DEBUG, "Scene: [%d]%s OnPickBigCoin bigcoin_objid:%d is not exist in scene.", scene->GetSceneID(), scene->GetSceneName(), pbc->obj_id);
		return;
	}

	if (Obj::OBJ_TYPE_BIG_COIN != obj->GetObjType())
	{
		gamelog::g_log_messagehandler.printf(LL_DEBUG, "Scene: [%d]%s OnPickBigCoin bigcoin_objid:%d is not a BigCoinObj.", scene->GetSceneID(), scene->GetSceneName(), pbc->obj_id);
		return;
	}

	BigCoinObj *bigcoin_obj = (BigCoinObj*)obj;
	bigcoin_obj->Pick(role);
}

// 队伍成员的位置信息
void MessageHandler::OnReqTeamMemberPos(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetTeamInfo()->SendTeamMemberPosToMe();
}

void MessageHandler::OnChangeSceneLineReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSChangeSceneLineReq *pro = (Protocol::CSChangeSceneLineReq*)msg;

	if (0 != pro->scene_key && role->GetHusongTask()->IsHusongStatus())
	{
		return;
	}

	SceneLineInfo *line_info = SceneRegister::Instance().Instance().GetSceneLineInfo(scene->GetSceneID());
	if (NULL != line_info)
	{
		m_scene_manager->GoTo(role, scene->GetSceneID(), pro->scene_key, role->GetPos());	
	}
}

void MessageHandler::OnRoleReturnReAlivePosiReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->ForceReturnReAlivePosi();
}

// 飞行
void MessageHandler::OnObjFlyHeightChange(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSObjFlyHeightChange *um = (Protocol::CSObjFlyHeightChange*)msg;

	if (Role::MOVE_MODE_FLY != role->GetMoveMode())
	{
		return;
	}

	role->FlyHeightChange(um->fly_height);

}

// 传送
void MessageHandler::OnTransportReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTransportReq *rt = (Protocol::CSTransportReq*)msg;

	Transport *transport = scene->GetTransport(rt->transport_index);
	if (NULL == transport)
	{
		gamelog::g_log_messagehandler.printf(LL_DEBUG, "Scene: [%d]%s Can NOT Find Transport[%d],", scene->GetSceneID(), scene->GetSceneName(), rt->transport_index);
		return;
	}

	{
		// 检查双人坐骑的另一半是否足够等级进入目标场景
		MultiMountBindItem *bind_info = role->GetMultiMount()->GetMountOnInfo();
		if (NULL != bind_info && bind_info->follower_uid != role->GetUID())
		{
			Role *follower_role = scene->GetRoleByUID(bind_info->follower_uid);
			if (NULL != follower_role && !SceneRegister::Instance().CheckSceneLevelLimit(follower_role, transport->target_scene))
			{
				role->NoticeNum(errornum::EN_ROLE_LEVEL_NOT_ENOUGH);
				return;
			}
		}
	}

	if (!SceneRegister::Instance().CheckSceneLevelLimit(role, transport->target_scene))
	{
		return;
	}

	static const unsigned int TransportReqLength = GLOBALCONFIG->GetOtherConfig_MaxTransportLength();
	if ((role->GetPos() - transport->location).IsLongerThan(TransportReqLength))
	{
		role->NoticeNum(errornum::EN_SCENE_TRANSPORT_TOO_FAR);
		return;
	}

	if (!scene->GetSpecialLogic()->SpecialCanTransport(role))
	{
		return;
	}

	if (!role->GetRoleStatusManager()->CanTransportTo(transport->target_scene))
	{
		return;
	}

	const ConfigSceneDoor *door = SceneRegister::Instance().GetDoor(transport->target_scene, transport->target_door_id);
	if (NULL == door)
	{
		return;
	}

	gamelog::g_log_debug.printf(LL_DEBUG, "MessageHandler::OnTransportReq: %s[%d %d]", role->GetName(), role->GetUserId().db_index, role->GetUserId().role_id);

	int scene_key = scene->GetSceneKey();
	if (scene->GetSceneID() != transport->target_scene && Scene::IsStaticScene(m_scene_manager->GetSceneTypeWithID(transport->target_scene)))
	{
		if (role->GetHusongTask()->IsHusongStatus())
		{
			scene_key = 0;
		}
		else
		{
			scene_key = -1;
		}
	}

	if (Scene::SCENE_TYPE_XINGZUOYIJI == m_scene_manager->GetSceneTypeWithID(transport->target_scene))
	{
		scene_key = 1;
	}

	m_scene_manager->GoTo(role, transport->target_scene, scene_key, door->pos);
}

// 获取任务列表
void MessageHandler::OnTaskListReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetTaskManager()->SendTaskListToRole();
	role->GetTaskRecorder()->SendTaskRecorderListToRole();
}

void MessageHandler::OnTaskGiveup(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	// 	Protocol::CSTaskGiveup *tg = (Protocol::CSTaskGiveup*)msg;
	// 
	// 	if (role->GetTaskManager()->GiveupTask(tg->taskid))
	// 	{
	// 		role->NoticeNum(noticenum::EN_TASK_GIVEUP_SUCC);
	// 	}
}

// 飞鞋
void MessageHandler::OnFlyByShoe(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFlyByShoe *fly_req = (Protocol::CSFlyByShoe*)msg;

	World::GetInstWorld()->GetSceneManager()->FlyByShoe(role, fly_req->scene_id, fly_req->scene_key, fly_req->pos_x, fly_req->pos_y, fly_req->is_auto_buy, fly_req->is_force);
}

// 飞鞋，仅用于跳跃同步
void MessageHandler::OnSyncJump(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSyncJump *fly_req = (Protocol::CSSyncJump*)msg;

	if (fly_req->scene_id == role->GetScene()->GetSceneID())
	{
		World::GetInstWorld()->GetSceneManager()->GoTo(role, fly_req->scene_id, role->GetScene()->GetSceneKey(), Posi(fly_req->pos_x, fly_req->pos_y));
	}
}

// 接受任务
void MessageHandler::OnTaskAccept(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTaskAccept *ta = (Protocol::CSTaskAccept*)msg;

	const TaskCfg *task_cfg = TASKPOOL->GetCfgByTaskID(ta->taskid);
	if (NULL == task_cfg) 
	{
		return;
	}

	if (TASK_TYPE_CISHA != task_cfg->task_type)
	{
		role->GetTaskManager()->AcceptTask(ta->taskid, true);
	}
	else
	{
		role->GetCishaTask()->AcceptTask();
	}
}

// 交任务
void MessageHandler::OnTaskCommit(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTaskCommit *tc = (Protocol::CSTaskCommit*)msg;
	role->GetTaskManager()->CommitTask(tc->taskid, true);
}

// NPC对话
void MessageHandler::OnTaskTalkToNpc(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTaskTalkToNpc *tttn = (Protocol::CSTaskTalkToNpc*)msg;

	role->GetTaskManager()->OnTalkToNPC(tttn->npc_id);
}

void MessageHandler::OnCishaTaskOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCishaTaskOpera *cso = (Protocol::CSCishaTaskOpera *)msg;

	if (Protocol::CSCishaTaskOpera::OPERA_TYPE_GET_INFO == cso->opera_type)
	{
		role->GetCishaTask()->SendInfo();
	}
	else if (Protocol::CSCishaTaskOpera::OPERA_TYPE_ACCEPT_TASK == cso->opera_type)
	{
		role->GetCishaTask()->AcceptTask();
	}
}

// 获取配置信息
void MessageHandler::OnHotkeyInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetHotkeyRecorder()->SendHotKeyListToRole();
}

// 配置修改
void MessageHandler::OnChangeHotkey(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSChangeHotkey *mi = (Protocol::CSChangeHotkey*)msg;
	role->GetHotkeyRecorder()->ChangeHotkey(mi->index, mi->item_id, mi->type);
}

void MessageHandler::OnPaoHuanRollReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSPaoHuanRollReq *phr = (Protocol::CSPaoHuanRollReq*)msg;
	role->GetPaoHuanTask()->OnGetPhaseRewardRollReq(phr->client_click_index);
}

// 查看角色位置
void MessageHandler::OnSeekRoleWhereReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSeekRoleWhere *cmd = (Protocol::CSSeekRoleWhere*)msg;

	Role *seek_role = World::GetInstWorld()->GetSceneManager()->GetRole(cmd->name);

	bool is_consume_stuff = true;

	// 没找到或者在副本活动场景则不消耗
	if (NULL == seek_role || !Scene::IsStaticScene(seek_role->GetScene()->GetSceneType()))
	{
		is_consume_stuff = false;
	}

	if (is_consume_stuff)
	{
		static ItemExtern::ItemConsumeList consume_list; 
		consume_list.Reset();

		int zhuizong_item_id = LOGIC_CONFIG->GetOtherCfg().GetOtherCfg().zhuizong_item_id;

		{
			ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];

			stuff_list[0].item_id = zhuizong_item_id;
			stuff_list[0].num = 1;
			stuff_list[0].buyable = false;

			if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list))
			{
				role->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
				return;
			}
		}

		if (!ItemExtern::ConsumeItem(role, consume_list, "SeekRole"))
		{
			return;
		}
	}

	Protocol::SCRoleSeekWhereInfo rswi;
	rswi.scene_id = 0;
	rswi.posx = 0;
	rswi.posy = 0;

	if (NULL != seek_role)
	{
		rswi.scene_id = seek_role->GetScene()->GetSceneID();
		rswi.posx = seek_role->GetPos().x;
		rswi.posy = seek_role->GetPos().y;
	}

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&rswi, sizeof(rswi));
}

void MessageHandler::OnShenZhuangOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShenzhaungOper *cmd = (Protocol::CSShenzhaungOper*)msg;
	if (Protocol::SHENZHUANG_OPERATE_UPLEVEL == cmd->operate)
	{
		role->GetShenZhuang()->OnUplevel(cmd->param);
	}
}

void MessageHandler::OnCSAEquipOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCSAEquipOpera *cmd = (Protocol::CSCSAEquipOpera*)msg;
	switch (cmd->operate)
	{
	case Protocol::CSA_OPERATE_TYPE_UP_COMMON_LEVEL:
	{
		role->GetCSAEquip()->OnUpCommonlevel(cmd->param);
	}
	break;

	case Protocol::CSA_OPERATE_TYPE_UP_GREAT_LEVEL:
	{
		role->GetCSAEquip()->OnUpGreatlevel(cmd->param);
	}
	break;
	default:
		break;
	}
}

void MessageHandler::OnCampEquipOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnCampNormalDuobaoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCampNormalDuobaoOperate *cmd = (Protocol::CSCampNormalDuobaoOperate*)msg;
	if (Protocol::CAMP_NORMALDUOBAO_OPERATE_ENTER == cmd->operate)
	{
		CampNormalDuobao::Instance().OnEnter(role, cmd->param1);
	}
	else if (Protocol::CAMP_NORMALDUOBAO_OPERATE_EXIT == cmd->operate)
	{
		CampNormalDuobao::Instance().OnExist(role);
	}
}

void MessageHandler::OnGoldVipOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGoldVipOperaReq *gvor = (Protocol::CSGoldVipOperaReq *)msg;
	switch (gvor->type)
	{
	case Protocol::OPERA_TYPE_ACTIVE:
		role->GetGoldVip()->OnActiveGoldVip();
		break;

	case Protocol::OPERA_TYPE_FETCH_RETURN_REWARD:
		role->GetGoldVip()->OnFetchRetrunReward();
		break;

	case Protocol::OPERA_TYPE_CONVERT_SHOP:
		role->GetGoldVip()->OnConvertShop(gvor->param1);
		break;
	}
}

void MessageHandler::OnAccountGoldReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	// role->GetKnapsack()->GetMoney()->GetAccountGoldFromDB(EngineAdapter::Instance().Time());
}

void MessageHandler::OnAccountGetGoldReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	// Protocol::CSAccountGetGoldReq *aggr = (Protocol::CSAccountGetGoldReq*)msg;
	// role->GetKnapsack()->GetMoney()->GetAccountGoldToRole();
}

// 好友列表
void MessageHandler::OnFriendListReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetFriendManager()->SendFriendListToRole();
	role->GetFriendManager()->SendEnemyListToRole();
	role->GetFriendManager()->SendAllCharm();
}

// 添加好友
void MessageHandler::OnAddFriendReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSAddFriendReq *afr = (Protocol::CSAddFriendReq*)msg;

	UserID target_user_id = IntToUid(afr->friend_user_id);

	if (target_user_id == role->GetUserId()) return;

	if (role->GetFriendManager()->IsBlack(target_user_id)) // 邀请者在自己的黑名单中
	{
		Protocol::SCNoticeNum en;
		en.notice_num = errornum::EN_IN_YOUR_BLACKLISTS;
		m_scene_manager->SendToUserByGlobal(target_user_id, (const char *)&en, sizeof(en));
		return;
	}

	if (role->GetFriendManager()->IsFriendFull())   // 好友已经达到最大数量
	{
		role->NoticeNum(errornum::EN_FRIEND_NUM_MAX);
		return;
	}

	if (role->GetFriendManager()->IsFriend(target_user_id)) // 已经是好友
	{
		role->NoticeNum(errornum::EN_FRIEND_EXIST);
		return;
	}

	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(target_user_id);
	if (NULL == target_role)
	{
		role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
	}
	else
	{
		if (target_role->GetFriendManager()->IsFriendFull()) // 好友已满 服务端先过滤
		{
			if (0 == afr->is_yi_jian)
			{
				role->NoticeNum(errornum::EN_FRIEND_OTHER_FULL);
			}
		}
		else
		{
			Protocol::SCAddFriendRoute af_route;
			role->GetName(af_route.req_gamename);
			af_route.req_user_id = role->GetUID();
			af_route.req_avatar = role->GetAvatar();
			af_route.req_sex = role->GetSex();
			af_route.req_prof = role->GetProf();
			af_route.req_camp = role->GetCamp();
			af_route.req_level = role->GetLevel();
			af_route.req_avatar_timestamp = role->GetAvatarTimeStamp();
			af_route.req_total_capability = role->GetCapability()->GetTotalCapability();
			EngineAdapter::Instance().NetSend(target_role->GetNetId(), (char*)&af_route, sizeof(Protocol::SCAddFriendRoute));

			if (0 == afr->is_yi_jian)
			{
				role->NoticeNum(noticenum::NT_SEND_ADD_FRIEND_REQ_SUCC);
			}
		}
	}
}

// 好友确认返回
void MessageHandler::OnAddFriendRet(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSAddFriendRet *afr = (Protocol::CSAddFriendRet*)msg;
	afr->req_gamename[sizeof(afr->req_gamename) - 1] = 0;

	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(afr->req_user_id));
	if (NULL == target_role)
	{
		role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	// 在加入之间先判断基本的
	{
		int index = -1;
		if (role->GetFriendManager()->IsFriend(IntToUid(afr->req_user_id), &index))
		{
			role->NoticeNum(errornum::EN_FRIEND_EXIST);
			return;
		}

		if (target_role->GetFriendManager()->IsFriendFull())
		{
			role->NoticeNum(errornum::EN_FRIEND_OTHER_FULL);
			return;
		}
	}

	ggprotocal::AddFriendAck afa;
	role->GetName(afa.friend_name);
	UserID temp_id = role->GetUserId();
	afa.db_index_friend = temp_id.db_index;
	afa.role_id_friend = temp_id.role_id;
	afa.is_accept = afr->is_accept;
	afa.reserved = 0;
	afa.friend_sex = role->GetSex();
	afa.friend_prof = role->GetProf();

	if (ADDFRIEND_AGREE == afr->is_accept)
	{
		if (role->GetFriendManager()->AddFriend(IntToUid(afr->req_user_id), "ClientAcceptAddFriend"))
		{
			role->NoticeNum(noticenum::NT_FRIEND_ADD_SUCCESS);
		}
		else
		{
			afa.is_accept = ADDFRIEND_OTHERFULL;
		}
	}


	if (NULL != target_role)
	{
		Protocol::SCAddFriendRet afrj;
		F_STRNCPY(afrj.gamename, afa.friend_name, sizeof(afrj.gamename));
		afrj.is_accept = afa.is_accept;
		EngineAdapter::Instance().NetSend(target_role->GetNetId(), (char*)&afrj, sizeof(Protocol::SCAddFriendRet));

		if (ADDFRIEND_REJECT == afa.is_accept)			// 拒绝
		{
			target_role->NoticeNum(errornum::EN_FRIEND_OTHER_REJECT_REQ);
		}
		else if (ADDFRIEND_AGREE == afa.is_accept)		// 同意
		{
			if (target_role->GetFriendManager()->AddFriend(UserID(afa.db_index_friend, afa.role_id_friend), "ClientReqAddFriend"))
			{
				target_role->NoticeNum(noticenum::NT_FRIEND_ADD_SUCCESS);
			}
		}
		else if (ADDFRIEND_OTHERFULL == afa.is_accept)	// 对方已满
		{
			target_role->NoticeNum(errornum::EN_FRIEND_OTHER_FULL);
		}
	}
}

// 删除好友
void MessageHandler::OnDeleteFriendReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSDeleteFriend *df = (Protocol::CSDeleteFriend *)msg;

	if (df->is_silence)
	{
		role->GetFriendManager()->RemoveFriend(IntToUid(df->user_id), "Client Silence Removed Friend");
	}
	else
	{
		if (role->GetSublock()->CheckHasLock()) return;

		if (role->GetFriendManager()->RemoveFriend(IntToUid(df->user_id), "Client Remove Friend"))
		{
			role->NoticeNum(noticenum::NT_REMOVE_FRIEND_SUCC);

			Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(df->user_id));
			if (NULL != target_role)
			{
				target_role->GetFriendManager()->RemoveFriend(role->GetUserId(), "Client Be Removed Friend");
			}
		}
	}
}

// 删除仇人
void MessageHandler::OnEnemyDeleteReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSEnemyDelete *edr = (Protocol::CSEnemyDelete *)msg;

	UniqueUserID unique_userid = LongLongToUniqueUserID(edr->unique_user_id);

	if (role->GetFriendManager()->RemoveEnemy(unique_userid.user_id, unique_userid.plat_type, "Client Remove Enemy"))
	{
		role->NoticeNum(noticenum::NT_REMOVE_ENEMY_SUCC);
	}
}

// 送花
void MessageHandler::OnGiveFlower(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGiveFlower *gf = (Protocol::CSGiveFlower*)msg;
	if (gf->target_uid <= 0) return;

	UserID target_user_id = IntToUid(gf->target_uid);
	if (INVALID_USER_ID == target_user_id) return;

	if (role->GetUserId() == target_user_id)
	{
		role->NoticeNum(errornum::EN_GIVE_FLOWER_CAN_NOT_SELF);
		return;
	}

	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(target_user_id);
	if (NULL == target_role)
	{
		role->NoticeNum(errornum::EN_TARGET_NOT_ONLINE);
		return;
	}

	static ggprotocal::GiveFlower gg_gf;
	gg_gf.grid_index = gf->grid_index;
	gg_gf.item_id = gf->item_id;
	gg_gf.target_uid = gf->target_uid;
	gg_gf.is_anonymity = gf->is_anonymity;
	gg_gf.is_marry = gf->is_marry;

	role->GetFriendManager()->OnGiveFlower(&gg_gf);
}

void MessageHandler::OnSongHuaInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetFriendManager()->SendSongHuaInfo();
}

void MessageHandler::OnOfflineFriendAutoDecFlag(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSOfflineFriendAutoDecFlag *pro = (Protocol::CSOfflineFriendAutoDecFlag*)msg;

	CommonDataParam	* common_data = role->GetCommonDataParam();
	if (NULL == common_data) return;

	if (1 != pro->is_auto_dec && 0 != pro->is_auto_dec) return;

	if (common_data->is_auto_dec_offline_friend != pro->is_auto_dec)
	{
		common_data->is_auto_dec_offline_friend = pro->is_auto_dec;
	}
}

void MessageHandler::OnGiveFlowerKissReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGiveFlowerKissReq *gfkr = (Protocol::CSGiveFlowerKissReq*)msg;
	if (gfkr->target_uid <= 0) return;

	UserID target_user_id = IntToUid(gfkr->target_uid);
	if (INVALID_USER_ID == target_user_id || role->GetUserId() == target_user_id) return;

	static Protocol::SCGiveFlowerKissAck gfka;
	gfka.from_uid = UidToInt(role->GetUserId());
	gfka.from_camp = role->GetCamp();
	role->GetName(gfka.from_name);

	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(target_user_id);
	if (NULL != target_role)
	{
		EngineAdapter::Instance().NetSend(target_role->GetNetId(), (const char *)&gfka, sizeof(gfka));
	}
}

void MessageHandler::OnFriendBlessOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFriendBlessOperaReq *fbor = (Protocol::CSFriendBlessOperaReq*)msg;

	if (Protocol::FRIEND_BLESS_OPERA_TYPE_QUERY == fbor->opera_type)
	{
		FriendBlessManager::Instance().SendAllInfo(role);
	}
	else if (Protocol::FRIEND_BLESS_OPERA_TYPE_BLESS == fbor->opera_type)
	{
		FriendBlessManager::Instance().OnBlessReq(role, fbor->param);
	}
	else if (Protocol::FRIEND_BLESS_OPERA_TYPE_FETCH_REWARD == fbor->opera_type)
	{
		FriendBlessManager::Instance().OnFetchReward(role, fbor->param);
	}
	else if (Protocol::FRIEND_BLESS_OPERA_TYPE_INVITE_BLESS == fbor->opera_type)
	{
		FriendBlessManager::Instance().OnInviteBlessReq(role);
	}
}

// 拉黑
void MessageHandler::OnAddBlackReq( const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSAddBlackReq *abr = (Protocol::CSAddBlackReq *)msg;

	UserID black_user_id = IntToUid(abr->black_id);
	UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(black_user_id);
	if (NULL == user_node)
	{
		return;
	}

	if (black_user_id == role->GetUserId())
	{
		return;
	}

	if (role->GetFriendManager()->AddBlack(black_user_id, user_node->user_name, user_node->sex, user_node->prof, "Client Add Black"))
	{
		role->NoticeNum(noticenum::NT_ADD_BLACK_SUCC);

		Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(black_user_id);
		if (NULL != target_role)
		{
			target_role->GetFriendManager()->RemoveFriend(role->GetUserId(), "Client Be Add Black");
		}
	}
}

// 删除黑名单
void MessageHandler::OnDeleteBlackReq( const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSDeleteBlackReq *dba = (Protocol::CSDeleteBlackReq *)msg;

	if (role->GetFriendManager()->RemoveBlack(IntToUid(dba->black_id), "Client Remove Black"))
	{
		role->NoticeNum(noticenum::NT_REMOVE_BLACK_SUCC);
	}
}

void MessageHandler::OnGmCommand(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGMCommand *gm_command = (Protocol::CSGMCommand*)msg;
	Gm::Instance().OnCommand(netid, scene, role, obj_id, gm_command);
}

void MessageHandler::OnTestSpeed(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTestSpeed *cs_testspeed = (Protocol::CSTestSpeed*)msg;

	Protocol::SCTestSpeed sc_test;
	sc_test.seq = cs_testspeed->seq;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&sc_test, sizeof(sc_test));
}

// 发送邮件
void MessageHandler::OnSendMail(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	static Protocol::CSMailSend stMailSend; 
	stMailSend.ResetMailContentTxt();

	int offset = static_cast<int>(UNSTD_OFFSETOFF(Protocol::CSMailSend, contenttxt));
	if (length < offset) return;

	const char *pt = (const char *)msg;

	memcpy((void*)&stMailSend, pt, offset);
	pt += offset; length -= offset;

	if (length < 0 || length >= MAX_CONTENT_TXT_LENTH || length != stMailSend.contenttxt_len) return;

	memcpy(&stMailSend.contenttxt[0], pt, stMailSend.contenttxt_len);
	stMailSend.contenttxt[sizeof(stMailSend.contenttxt) - 1] = '\0';

	{
		// 策划需求邮件不允许寄东西，要改成可以寄的话把这段代码注释掉
		stMailSend.gold = 0;
		stMailSend.coin = 0;
		stMailSend.item_count = 0;
		memset(stMailSend.item_knapindex_list, 0, sizeof(stMailSend.item_knapindex_list));
		memset(stMailSend.item_comsume_num, 0, sizeof(stMailSend.item_comsume_num));
	}

	MailRoute::Instance().SendMail(role, &stMailSend);
}

void MessageHandler::OnDeleteMail(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMailDelete *md = (Protocol::CSMailDelete*)msg;
	MailRoute::Instance().RemoveMail(role, md->mail_index);
}

void MessageHandler::OnLockMail(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	// 	Protocol::CSMailLock *ml = (Protocol::CSMailLock*)msg;
	// 	MailRoute::Instance().LockMail(role, ml->mail_index);
}

void MessageHandler::OnUnlockMail(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	// 	Protocol::CSMailLock *ml = (Protocol::CSMailLock*)msg;
	// 	MailRoute::Instance().UnlockMail(role, ml->mail_index);
}

void MessageHandler::OnGetMailList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	MailRoute::Instance().GetMailList(role);
}

void MessageHandler::OnReadMail(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMailRead *mr = (Protocol::CSMailRead*)msg;
	MailRoute::Instance().ReadMail(role, mr->mail_index);
}

void MessageHandler::OnFetchMailAttachment(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMailFetchAttachment *mgi = (Protocol::CSMailFetchAttachment*)msg;
	MailRoute::Instance().FetchMailAttachment(role, mgi);
}

void MessageHandler::OnCleanMail(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	MailRoute::Instance().CleanMail(role);
}

void MessageHandler::OnOneKeyFetchMailAttachment(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	UserMailManager::Instance().OneKeyFetchAttachment(role);
}

void MessageHandler::OnAddPublicSaleItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSAddPublicSaleItem *add_sale_req = (Protocol::CSAddPublicSaleItem *)msg;
	PublicSaleRoute::Instance().OnAddPublicSaleItem(role, add_sale_req);
}

void MessageHandler::OnRemovePublicSaleItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRemovePublicSaleItem *remove_sale_req = (Protocol::CSRemovePublicSaleItem *)msg;
	PublicSaleRoute::Instance().OnRemovePublicSaleItem(role, remove_sale_req);
}

void MessageHandler::OnBuyPublicSaleItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSBuyPublicSaleItem *buy_sale_req = (Protocol::CSBuyPublicSaleItem *)msg;
	PublicSaleRoute::Instance().OnBuyPublicSaleItem(role, buy_sale_req);
}

void MessageHandler::OnPublicSaleGetUserItemList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSPublicSaleGetUserItemList *get_useritem_list = (Protocol::CSPublicSaleGetUserItemList *)msg;
	PublicSaleRoute::Instance().OnGetPublicSaleUserItemList(role, get_useritem_list);
}

void MessageHandler::OnPublicSaleSendItemInfoToWorld(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSPublicSaleSendItemInfoToWorld *send_publicsale_iteminfo = (Protocol::CSPublicSaleSendItemInfoToWorld*)msg;
	PublicSaleRoute::Instance().OnSendPublicSaleItemInfoToWorld(role, send_publicsale_iteminfo);
}


static int UnpackFuzzySearch(Protocol::FuzzySearch *fuzzy_search, const char *stream, int len)
{
	if (NULL == fuzzy_search || NULL == stream) return -1;

	int size = 0;
	const char *pt = stream; 

	int offset = static_cast<int>(UNSTD_OFFSETOFF(Protocol::FuzzySearch, item_id_list));

	if (len < offset) return -1; 

	memcpy((void*)fuzzy_search, pt, offset);
	pt += offset; size += offset; len -= offset;

	if (fuzzy_search->item_count > Protocol::FUZZY_SEARCH_ITEM_ID_COUNT) return -1;

	int item_idlist_size = sizeof(fuzzy_search->item_id_list[0]) * fuzzy_search->item_count;
	if (len < item_idlist_size) return -1;

	memcpy(&fuzzy_search->item_id_list[0], pt, item_idlist_size);
	pt += item_idlist_size; size += item_idlist_size; len -= item_idlist_size;

	return size;
}

void MessageHandler::OnPublicSearch(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	static Protocol::CSPublicSaleSearch publicsale_search_req;

	int offset = static_cast<int>(UNSTD_OFFSETOFF(Protocol::CSPublicSaleSearch, fuzzy_type_list));

	if (length < offset) return;

	const char *pt = (const char*)msg;

	memcpy((void*)&publicsale_search_req, pt, offset);
	pt += offset; length -= offset;

	if (publicsale_search_req.fuzzy_type_count > Protocol::FUZZY_SEARCH_ITEM_TYPE_COUNT) return;

	for (int i = 0; i < publicsale_search_req.fuzzy_type_count && length > 0; i++)
	{
		int unpack_size = UnpackFuzzySearch(&publicsale_search_req.fuzzy_type_list[i], pt, length);
		if (unpack_size < 0) return;
		pt += unpack_size; length -= unpack_size;
	}

	UserPublicSaleManager::Instance().OnPublicSaleSearch(role, &publicsale_search_req);
}

void MessageHandler::OnPublicSaleTypeCountReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	UserPublicSaleManager::Instance().OnPublicSaleTypeCountReq(role);
}

void MessageHandler::OnGetCampInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	CampManager::Instance().OnGetCampInfo(role);
}

void MessageHandler::OnChooseCamp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRoleChooseCamp *rcc = (Protocol::CSRoleChooseCamp *)msg;
	role->SetCamp(rcc->camp_type, 0 != rcc->is_random);
}

void MessageHandler::OnChooseProf(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRoleChooseProf *choose_prof = (Protocol::CSRoleChooseProf*)msg;
	role->SetProf(choose_prof->prof_type);
}

void MessageHandler::OnLeaveFB(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	FBManager::Instance().LeaveFB(role);
}

void MessageHandler::OnEnterFB(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSEnterFB *cg = (Protocol::CSEnterFB*)msg;
	FBManager::Instance().EnterFB(role, cg->fb_type, cg->param_1, cg->param_2, cg->param_3);
}

void MessageHandler::OnCancelMonsterStaticState(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	int scene_type = role->GetScene()->GetSpecialLogic()->GetSceneType();
	if (Scene::SCENE_TYPE_PHASE_FB == scene_type || Scene::SCENE_TYPE_DAILY_TASK_FB == scene_type)
	{
		SpecialLogic *special_logic = role->GetScene()->GetSpecialLogic();
		if (NULL != special_logic)
		{
			special_logic->SpecialCancelMonsterStatic();
		}
	}
}

void MessageHandler::OnAutoFB(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSAutoFB *cg = (Protocol::CSAutoFB*)msg;
	FBManager::Instance().AutoFB(role, cg->fb_type, cg->param_1, cg->param_2, cg->param_3);
}

void MessageHandler::OnSetAvatarTimestamp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSetAvatarTimeStamp *sats = (Protocol::CSSetAvatarTimeStamp*)msg;
	role->SetAvatarTimeStamp(sats->avatar_timestamp);
}

void MessageHandler::OnGetRoleExpExtraPer(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::SCRoleExpExtraPer rsw;
	int initial_per = 100;
	int add_percent = 0;
	rsw.exp_extra_per =  static_cast<int>((role->GetExpTotalPer(Role::EXP_ADD_REASON_WATCH, &add_percent) - initial_per) * 100);
	EngineAdapter::Instance().NetSend(netid, (const char*)&rsw, sizeof(rsw));
}

void MessageHandler::OnCrossQueryRoleInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCrossQueryRoleInfo *qri = (Protocol::CSCrossQueryRoleInfo*)msg;

	static crossgameprotocal::GameCrossQueryRoleInfoRequest gcqrir;
	gcqrir.target_plat_type = qri->plat_type;
	gcqrir.target_uid = qri->target_uid;
	gcqrir.request_plat_type = role->GetUniqueUserID().plat_type;
	gcqrir.request_uid = UidToInt(role->GetUniqueUserID().user_id);
	gcqrir.request_type = crossgameprotocal::GameCrossQueryRoleInfoRequest::QUERY_ROLE_INFO_TYPE_ALL;
	gcqrir.param = 0;

	//如果请求的目标是本服的直接处理

	UniqueServerID target_serverid(qri->plat_type, IntToUid(qri->target_uid).db_index);
	UniqueServerID local_serverid(LocalConfig::Instance().GetPlatType(), LocalConfig::Instance().GetMergeServerId());

	if (target_serverid == local_serverid && !CrossConfig::Instance().IsHiddenServer())
	{
		QueryHandler::Instance().OnQuery(gcqrir.target_uid, QUERY_FROM_CLIENT, gcqrir.request_uid, 0);
		return;
	}
	InternalComm::Instance().SendToCross((const char *)&gcqrir, sizeof(gcqrir), CROSS_FUNCTION_TYPE_CROSS_RANK);
}

void MessageHandler::OnQueryRoleInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSQueryRoleInfo *qri = (Protocol::CSQueryRoleInfo*)msg;
	QueryHandler::Instance().OnQuery(qri->target_uid, QUERY_FROM_CLIENT, role->GetUID(), 0);
}

void MessageHandler::ONCrossFindRoleByUUID(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCrossFindRoleByUUID *frbn = (Protocol::CSCrossFindRoleByUUID*)msg;

	Protocol::SCFindRoleByNameRet drbnr;
	drbnr.is_online = false;
	drbnr.msg_identify = frbn->msg_identify;

	UserID user_id = IntToUid(frbn->target_uid);
	
	UniqueServerID target_server_id(frbn->plat_type, user_id.db_index);
	UniqueServerID local_server_id(LocalConfig::Instance().GetPlatType(), LocalConfig::Instance().GetMergeServerId());

	if (target_server_id == local_server_id && !CrossConfig::Instance().IsHiddenServer())
	{
		UserCacheNode *uc = UserCacheManager::Instance().GetUserNode(frbn->target_uid);
		if (NULL == uc)
		{
			drbnr.role_id = 0;
			role->NoticeNum(errornum::EN_USER_IS_NOT_EXIST);
		}
		else
		{
			drbnr.uuserverid = LocalConfig::Instance().GetPlatType();
			drbnr.uuserverid = (drbnr.uuserverid << 32) + LocalConfig::Instance().GetMergeServerId();
			drbnr.uuid = uc->uuid;
			drbnr.role_id = uc->uid;
			drbnr.is_online = World::GetInstWorld()->GetSceneManager()->IsOnLine(uc->uuid);
			drbnr.at_cross = 0;
			if (drbnr.is_online != 0)
			{
				drbnr.at_cross = World::GetInstWorld()->GetSceneManager()->UserCrosserExist(uc->uuid);
			}
			STRNCPY(drbnr.role_name, uc->user_name, sizeof(GameName));
			drbnr.sex = uc->sex;
			drbnr.prof = uc->prof;
			drbnr.level = uc->level;
			drbnr.camp = uc->camp;
			drbnr.avatar_timestamp = uc->avatar_timestamp;
			drbnr.capability = uc->capability;
		}
		EngineAdapter::Instance().NetSend(netid, (const char *)&drbnr, sizeof(Protocol::SCFindRoleByNameRet));
		return;
	}
	else 
	{
		static crossgameprotocal::GameCrossQueryRoleInfoRequest gcqrir;
		gcqrir.target_plat_type = frbn->plat_type;
		gcqrir.target_uid = frbn->target_uid;
		gcqrir.request_plat_type = role->GetRoleCross()->GetOriginPlatType();
		gcqrir.request_uid = UidToInt(role->GetRoleCross()->GetCrossOriginalUniqueUserID().user_id);
		gcqrir.request_type = crossgameprotocal::GameCrossQueryRoleInfoRequest::QUERY_ROLE_INFO_TYPE_UUID_NAME;
		gcqrir.param = frbn->msg_identify;

		InternalComm::Instance().SendToCross((const char *)&gcqrir, sizeof(gcqrir), CROSS_FUNCTION_TYPE_CROSS_RANK);
	}
}

void MessageHandler::OnQueryRoleInfoByName(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFindRoleByName *frbn = (Protocol::CSFindRoleByName*)msg;
	frbn->name[sizeof(GameName) - 1] = 0;

	Protocol::SCFindRoleByNameRet drbnr;
	drbnr.is_online = false;
	drbnr.msg_identify = frbn->msg_identify;

	bool is_online = false;

	UserCacheNode *uc = UserCacheManager::Instance().GetUserNodeByName(frbn->name, &is_online);
	if (NULL == uc)
	{
		drbnr.role_id = 0;
		role->NoticeNum(errornum::EN_USER_IS_NOT_EXIST);
	}
	else
	{

		drbnr.uuserverid = LocalConfig::Instance().GetPlatType();
		drbnr.uuserverid = (drbnr.uuserverid << 32) + LocalConfig::Instance().GetMergeServerId();
		drbnr.uuid = uc->uuid;
		drbnr.role_id = uc->uid;
		drbnr.is_online = World::GetInstWorld()->GetSceneManager()->IsOnLine(uc->uuid);
		drbnr.at_cross = 0;
		if (drbnr.is_online != 0)
		{
			drbnr.at_cross = World::GetInstWorld()->GetSceneManager()->UserCrosserExist(uc->uuid);
		}
		STRNCPY(drbnr.role_name, uc->user_name, sizeof(GameName));
		drbnr.sex = uc->sex;
		drbnr.prof = uc->prof;
		drbnr.level = uc->level;
		drbnr.camp = uc->camp;
		drbnr.avatar_timestamp = uc->avatar_timestamp;
		drbnr.capability = uc->capability;
	}

	EngineAdapter::Instance().NetSend(netid, (const char *)&drbnr, sizeof(Protocol::SCFindRoleByNameRet));
}

// 创建公会
void MessageHandler::OnCreateGuild(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCreateGuild *cg = (Protocol::CSCreateGuild*)msg;
	GuildRoute::Instance().CreateGuildCS(role, cg);
}

void MessageHandler::OnDimissGuild(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSDismissGuild *dg = (Protocol::CSDismissGuild*)msg;
	GuildRoute::Instance().DismissGuildCS(role, dg);
}

// 申请加入公会
void MessageHandler::OnApplyForJoinGuild(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSApplyForJoinGuild *afjg = (Protocol::CSApplyForJoinGuild*)msg;
	GuildRoute::Instance().ApplyForJoinGuildCS(role, afjg);
}

// 回应申请
void MessageHandler::OnApplyForJoinGuildAck(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSApplyForJoinGuildAck *afjga = (Protocol::CSApplyForJoinGuildAck*)msg;

	if (afjga->count <= 0 || afjga->count > MAX_APPLY_FOR_COUNT || 
		length != static_cast<int>(UNSTD_OFFSETOFF(Protocol::CSApplyForJoinGuildAck, applyfor_uid_list) + afjga->count * sizeof(afjga->applyfor_uid_list[0]))) return;

	static Protocol::CSApplyForJoinGuildAck complete_afjga;
	memcpy((void *)&complete_afjga, afjga, length);

	GuildRoute::Instance().ApplyForJoinGuildAckCS(role, &complete_afjga);
}

// 退出公会
void MessageHandler::OnQuitGuild(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSQuitGuild *qg = (Protocol::CSQuitGuild*)msg;
	GuildRoute::Instance().QuitGuildCS(role, qg);
}

// 邀请进入公会
void MessageHandler::OnInviteGuild(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSInviteGuild *ig = (Protocol::CSInviteGuild*)msg;
	GuildRoute::Instance().InviteGuildCS(role, ig);
}

// 邀请回应
void MessageHandler::OnInviteGuildAck(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSInviteGuildAck *iga = (Protocol::CSInviteGuildAck*)msg;
	GuildRoute::Instance().InviteGuildAckCS(role, iga);
}

// 踢出公会
void MessageHandler::OnKickoutGuild(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSKickoutGuild *kg = (Protocol::CSKickoutGuild*)msg;

	if (kg->bekicker_count <= 0 || kg->bekicker_count > MAX_MEMBER_COUNT || 
		length != static_cast<int>(UNSTD_OFFSETOFF(Protocol::CSKickoutGuild, bekicker_uid_list) + kg->bekicker_count * sizeof(kg->bekicker_uid_list[0]))) return;

	static Protocol::CSKickoutGuild complete_kg;
	memcpy((void *)&complete_kg, kg, length);

	GuildRoute::Instance().KickoutGuildCS(role, &complete_kg);
}

// 任命
void MessageHandler::OnAppointGuild(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSAppointGuild *ag = (Protocol::CSAppointGuild*)msg;
	GuildRoute::Instance().AppointGuildCS(role, ag);
}

void MessageHandler::OnLeaveGuildPost(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSLeaveGuildPost *lgp = (Protocol::CSLeaveGuildPost*)msg;
	GuildRoute::Instance().LeaveGuildPostCS(role, lgp);
}

void MessageHandler::OnGuildChangeNotice(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildChangeNotice *gcn = (Protocol::CSGuildChangeNotice*)msg;
	GuildRoute::Instance().GuildChangeNoticeCS(role, gcn);
}

void MessageHandler::OnGuildMailAll(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	return;
	// 屏蔽该功能
	//Protocol::CSGuildMailAll *gma = (Protocol::CSGuildMailAll*)msg;
	//if (length < static_cast<int>(UNSTD_OFFSETOFF(Protocol::CSGuildMailAll, contenttxt)) ||
	//	gma->contenttxt_len < 0 || gma->contenttxt_len >= MAX_CONTENT_TXT_LENTH || 
	//	gma->contenttxt_len + static_cast<int>(UNSTD_OFFSETOFF(Protocol::CSGuildMailAll, contenttxt)) != length)
	//{
	//	return;
	//}

	//static Protocol::CSGuildMailAll complete_gma;
	//complete_gma.ResetMailContentTxt();

	//memcpy(&complete_gma ,gma, length);
	//complete_gma.contenttxt[sizeof(complete_gma.contenttxt) - 1] = '\0';

	//GuildRoute::Instance().GuildMailAllCS(role, &complete_gma);
}

void MessageHandler::OnGetGuildInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGetGuildInfo *ggi = (Protocol::CSGetGuildInfo*)msg;
	GuildRoute::Instance().GetGuildInfoCS(role, ggi);
}

void MessageHandler::OnGuildSetStorageShopUse(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnGuildDelate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildDelate *gd = (Protocol::CSGuildDelate*)msg;
	GuildRoute::Instance().GuildDelateCS(role, gd);
}

void MessageHandler::OnGuildStorageReqUse(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnGuildStoragePutItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnGuildStorageTakeItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnGuildBossFeed(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnGuildBossCall(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnGuildTianCiTongBiReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildTianCiTongBiReq *gtctbr = (Protocol::CSGuildTianCiTongBiReq*)msg;
	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (nullptr == guild)
	{
		return;
	}

	switch (gtctbr->opera_type)
	{
	case Protocol::GUILD_TIANCITONGBI_REQ_TYPE_OPEN:
	{
		guild->OnGuildTianCiTongBiOpenReq(role->GetUserId(), gtctbr);
	}
	break;
	
	case Protocol::GUILD_TIANCITONGBI_REQ_TYPE_USE_GATHER:
	{
		SpecialGuildStation *special_logic = World::GetInstWorld()->GetSceneManager()->GetGuildStationSpecialLogic(role->GetGuildID());
		if (NULL != special_logic)
		{
			special_logic->OnTianCiTongBiUseGather(role);
		}
	}
	break;

	case Protocol::GUILD_TIANCITONGBI_REQ_TYPE_RANK_INFO:
	{
		SpecialGuildStation *special_logic = World::GetInstWorld()->GetSceneManager()->GetGuildStationSpecialLogic(role->GetGuildID());
		if (NULL != special_logic)
		{
			if (special_logic->IsTianCiTongBiOpen())
			{
				special_logic->SendTianCiTongBiRankInfo(role);
			}
		}
	}
	break;

	default:
		break;
	}
}

void MessageHandler::OnGuildEnemyRankList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	if (nullptr == role)
	{
		return;
	}

	if (CrossConfig::Instance().IsHiddenServer())
	{
		// 如果是跨服，那么要去原服取数据
		ActivityShadow::Instance().OnGetGuildEnemyListFromOriginal(role);

		return;
	}

	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (nullptr == guild)
	{
		return;
	}

	guild->SendGuildEnemy(role);
}

void MessageHandler::OnGuildTowerDefendReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnGuildTowerDefendNextWaveReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnGuildXianshuInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetGuildXianshu()->SendXianshuInfo();
}

void MessageHandler::OnGuildXianshuUplevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetGuildXianshu()->UplevelXianshu();
}

void MessageHandler::OnGuildSignIn(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnGuildBackToStation(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildBackToStation *gbts = (Protocol::CSGuildBackToStation*)msg;
	GuildRoute::Instance().GuildBackToStationCS(role, gbts);
}

void MessageHandler::OnClearTrainMentalityCD(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetMentality()->ClearTrainMentalityCD();
}

void MessageHandler::OnTrainMentality(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetMentality()->TrainMentality();
}

void MessageHandler::OnUpgradeMentalityGengu(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUpgradeMentalityGengu *umg = (Protocol::CSUpgradeMentalityGengu*)msg;
	role->GetMentality()->UpgradeMentalityGengu(umg->type, (0 != umg->is_auto_buy), (0 != umg->use_protect_item));
}

void MessageHandler::OnSpeedUpMentalityTrain(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetMentality()->ClearMentalityCD();
}

void MessageHandler::OnMentalityYijianTisheng(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetMentality()->YijianTishengMentality();
}

void MessageHandler::OnMentalityUplevelWuxing(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMentalityUplevelWuxing *muwx = (Protocol::CSMentalityUplevelWuxing *)msg;
	role->GetMentality()->OnMentalityUplevelWuxing(muwx->wuxing_index, (0 != muwx->is_auto_buy));
}

// 获得VIP等级奖励
void MessageHandler::OnFetchVipLevelReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFetchVipLevelReward *fvlr = (Protocol::CSFetchVipLevelReward *)msg;
	role->GetVip()->FetchLevelReward(fvlr->seq);
}

// 获得VIP周奖励
void MessageHandler::OnFetchVipWeekReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetVip()->FetchVipWeekReward();
}

// 获得限时vip
void MessageHandler::OnFetchTimeLimitVip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetVip()->FetchTimeLimitVip();
}

void MessageHandler::OnQiFuReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnFetchQiFuReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

// 获取角色战力
void MessageHandler::OnGetRoleCapability(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetCapability()->OnGetRoleCapa();
}

void MessageHandler::OnSetMoveMode(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSetMoveMode *smm = (Protocol::CSSetMoveMode*)msg;

	if (Role::MOVE_MODE_FLY == smm->move_mode)
	{
		if (!role->GetMountManager()->MountCanFly())
		{
			return;
		}

		if (role->GetMountManager()->IsOnMount())
		{
			if (!role->GetMountManager()->GoonMount(false))
			{
				role->NoticeNum(errornum::EN_MOUNT_CANNOT_MOUNTON);
				return;
			}
		}
	}

	{
		Scene *scene = role->GetScene();
		if (NULL == scene)
		{
			return;
		}

		if (!scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, role->GetPos().x, role->GetPos().y) && smm->move_mode == Role::MOVE_MODE_NORMAL)
		{
			role->NoticeNum(errornum::EN_STOP_FLY_AREA_LIMIT);
			return;
		}
	}

	role->SetMoveMode(smm->move_mode, Posi(smm->longjump_dest_x, smm->longjump_dest_y), (char)smm->move_mode_param);
}

void MessageHandler::OnChestShopBuyItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSBuyChestShopItem *bcst = (Protocol::CSBuyChestShopItem *)msg;
	LOGIC_CONFIG->GetChestShop().Buy(role, bcst->shop_type, bcst->mode);
}

void MessageHandler::OnRareChestShopBuyItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRareChestShopReq *cmd = (Protocol::CSRareChestShopReq *)msg;
	LOGIC_CONFIG->GetChestShop().RareBuy(role, cmd->times);
}

void MessageHandler::OnChestShopRecordListReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSChestShopRecordList *cmd = (Protocol::CSChestShopRecordList *)msg;
	WorldStatus::Instance().SendChestShopRecord(role, cmd->shop_type);
}

void MessageHandler::OnChestShopFetchItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFetchChestShopItem *fetch_item = (Protocol::CSFetchChestShopItem*)msg;

	if (fetch_item->if_fetch_all > 0)
	{
		role->GetRoleChestShop()->OnFetchAllItems(char(fetch_item->shop_type));
	}
	else
	{
		role->GetRoleChestShop()->OnFetchItem(char(fetch_item->shop_type), fetch_item->grid_index);
	}
}

void MessageHandler::OnChestShopGetSelfItemList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGetSelfChestShopItemList *cmd = (Protocol::CSGetSelfChestShopItemList*)msg;

	role->GetRoleChestShop()->OnGetItemList(char(cmd->shop_type));
}

void MessageHandler::OnChestShopGetFreeInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleChestShop()->SendFreeInfo();
}

void MessageHandler::OnChestShopAutoRecycle(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSChestShopAutoRecycle *cmd = (Protocol::CSChestShopAutoRecycle*)msg;

	if (1 == cmd->is_auto)
	{
		role->GetRoleChestShop()->OnAutoRecycle(cmd->shop_type, cmd->color);
	}
	else if (0 == cmd->is_auto)
	{
		role->GetRoleChestShop()->OnRecycleOne(cmd->shop_type, cmd->gird_index);
	}	
}

void MessageHandler::OnShopBuyItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShopBuy *bs = (Protocol::CSShopBuy*)msg;

	LOGIC_CONFIG->GetShop().OnBuyItemReq(role, bs);
}

void MessageHandler::OnMoveChessResetReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	LOGIC_CONFIG->GetMoveChess().BuyReset(role);
}

void MessageHandler::OnMoveChessShakeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMoveChessShakeReq *cmd = (Protocol::CSMoveChessShakeReq*)msg;
	LOGIC_CONFIG->GetMoveChess().Shake(role ,cmd->is_use_item);
}

void MessageHandler::OnMoveChessFreeInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->SendMoveChessInfo();
}

void MessageHandler::OnMysteriousShopOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMysteriosshopOperate *cmd = (Protocol::CSMysteriosshopOperate*)msg;

	if (Protocol::MYSTERIOUSSHOP_OPERATE_TYPE_REQINFO == cmd->operate_type)
	{
		LOGIC_CONFIG->GetMysteriousShop().SyncShopItemInfo();
	}
	else if (Protocol::MYSTERIOUSSHOP_OPERATE_TYPE_BUY == cmd->operate_type)
	{
		LOGIC_CONFIG->GetMysteriousShop().BuyShopNormalItemReq(role, cmd->param1, cmd->param2);
	}
}

void MessageHandler::OnMysteriousShopInMallOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMysteriosshopinMallOperate *cmd = (Protocol::CSMysteriosshopinMallOperate*)msg;
	role->GetMysteriousShopInMall()->MysteriousshopinmallOp(cmd->operate_type, cmd->param1);
}

void MessageHandler::OnDiscountBuyGetInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetDiscountBuy()->SendDiscountBuyInfo();
}

void MessageHandler::OnDiscountBuyReqBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSDiscountBuyReqBuy *dbrb = (Protocol::CSDiscountBuyReqBuy*)msg;
	role->GetDiscountBuy()->OnBuy(dbrb->seq);
}

void MessageHandler::OnZeroGiftOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSZeroGiftOperate *dbrb = (Protocol::CSZeroGiftOperate*)msg;
	role->GetZeroGift()->ZeroGiftOperate(dbrb->operate_type, dbrb->param_1, dbrb->param_2);
}

void MessageHandler::OnAdvanceNoticeOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSAdvanceNoitceOperate *dbrb = (Protocol::CSAdvanceNoitceOperate*)msg;

	switch (dbrb->operate_type)
	{
	case Protocol::CSAdvanceNoitceOperate::ADVANCE_NOTICE_GET_INFO:
	{
		role->SendAdvanceNoticInfo();
	}
	break;

	case Protocol::CSAdvanceNoitceOperate::ADVANCE_NOTICE_FETCH_REWARD:
	{
		role->FetchAdvanceNoticReward(dbrb->param_1);
	}
	break;

	case Protocol::CSAdvanceNoitceOperate::ADVANCE_NOTICE_DAY_GET_INFO:
	{
		role->SendAdvanceNoticDayInfo();
	}
	break;

	case Protocol::CSAdvanceNoitceOperate::ADVANCE_NOTICE_DAY_FETCH_REWARD:
	{
		role->FetchAdvanceNoticDayReward(dbrb->param_1);
	}
	break;

	default:
		break;
	}
}

void MessageHandler::OnGoldTurntableOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGoldTurntableOperateReq *pro = (Protocol::CSGoldTurntableOperateReq*)msg;
	switch (pro->operate_type)
	{
	case Protocol::CSGoldTurntableOperateReq::INFO_REQ:
		role->GetRoleActivity()->SendGoldTurntableInfo();
		break;

	case Protocol::CSGoldTurntableOperateReq::CHOU_JIANG:
		role->GetRoleActivity()->GoldTurntableChouJiang();
		break;
	}
}

void MessageHandler::OnGetTitleList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetTitleManager()->SendAllTitle();
}

void MessageHandler::OnUseTitle(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUseTitle *ut = (Protocol::CSUseTitle *)msg;

	role->GetTitleManager()->UseTitleList(ut->use_title_list);
}

void MessageHandler::OnUpgradeTitle(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUpgradeTitle *ut = (Protocol::CSUpgradeTitle *)msg;

	role->GetTitleManager()->OnUpGradeReq(ut->title_id);
}

void MessageHandler::OnGuildShopBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnGuildShopGetRoleBuyInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnGuildResetName(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildResetName *grn = (Protocol::CSGuildResetName *)msg;
	GuildRoute::Instance().GuildResetName(role, grn);
}

void MessageHandler::OnGuildApplyforSetup(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSApplyforSetup *afs = (Protocol::CSApplyforSetup *)msg;
	GuildRoute::Instance().GuildApplySetup(role, afs);
}

void MessageHandler::OnGuildAddGuildExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSAddGuildExp *age = (Protocol::CSAddGuildExp *)msg;
	GuildRoute::Instance().GuildAddGuildExp(role, age);
}

void MessageHandler::OnGuildUpLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildUpLevel *cmd = (Protocol::CSGuildUpLevel *)msg;
	GuildRoute::Instance().GuildUpLevel(role, cmd);
}

void MessageHandler::OnGuildGetBuff(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildGetBuff *cmd = (Protocol::CSGuildGetBuff *)msg;
	GuildRoute::Instance().GuildGetBuff(role, cmd);
}

void MessageHandler::OnGuildExchange(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildExchange *cmd = (Protocol::CSGuildExchange *)msg;
	GuildRoute::Instance().GuildExchange(role, cmd);
}

void MessageHandler::OnGuildFbEnterReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	GuildRoute::Instance().OnGuildFbEnterReq(role);
}

void MessageHandler::OnGuildExtendMemberReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildExtendMemberReq *ilz = (Protocol::CSGuildExtendMemberReq *)msg;

	if (Protocol::CSGuildExtendMemberReq::EXTEND_MEMBER == ilz->operate_type)
	{
		GuildManager::Instance().ExtendGuildMember(role, ilz->can_use_gold != 0, ilz->num);
	}
	else if (Protocol::CSGuildExtendMemberReq::MEMBER_MAX_COUNT_INFO == ilz->operate_type)
	{
		GuildManager::Instance().SendGuildMemberCountInfo(role);
	}
}

void MessageHandler::OnGotoBonfirePos(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	GuildRoute::Instance().OnGotoBonfirePos(role);
}

void MessageHandler::OnAddBonfireMucai(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	GuildRoute::Instance().OnAddBonfireMucai(role);
}

void MessageHandler::OnGuildStorgeOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildStorgeOperate *cmd = (Protocol::CSGuildStorgeOperate *)msg;

	if (Protocol::GUILD_STORGE_OPERATE_PUTON_ITEM == cmd->operate_type)
	{
		GuildRoute::Instance().OnStorgePutItem(role, cmd->param1, cmd->param2);
	}
	else if (Protocol::GUILD_STORGE_OPERATE_TAKE_ITEM == cmd->operate_type)
	{
		GuildRoute::Instance().OnStorgeTakeItem(role, cmd->param1, cmd->param2, cmd->param3);
	}
	else if (Protocol::GUILD_STORGE_OPERATE_REQ_INFO == cmd->operate_type)
	{
		GuildRoute::Instance().OnReqStorgeInfo(role);
	}
	else if (Protocol::GUILD_STORGE_OPERATE_DISCARD_ITEM == cmd->operate_type)
	{
		GuildRoute::Instance().OnStorgeDiscardItem(role, cmd->param1, cmd->param2);
	}
}

void MessageHandler::OnGuildStorgeOneKeyOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildStorgeOneKeyOperate *cmd = (Protocol::CSGuildStorgeOneKeyOperate *)msg;

	if (cmd->item_count <= 0 || length != static_cast<int>(UNSTD_OFFSETOFF(Protocol::CSGuildStorgeOneKeyOperate, item_list) + sizeof(cmd->item_list[0]) * cmd->item_count)) return;

	if (Protocol::GUILD_STORGE_OPERATE_PUTON_ITEM_ONE_KEY == cmd->operate_type)
	{
		GuildRoute::Instance().OnStorgePutItemOneKey(role, cmd);
	}
	else if (Protocol::GUILD_STORGE_OPERATE_DISCARD_ITEM_ONE_KEY == cmd->operate_type)
	{
		GuildRoute::Instance().OnStorgeDiscardItemOneKey(role, cmd);
	}
}

void MessageHandler::OnGuildAutoClearReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildSetAutoClearReq *cmd = (Protocol::CSGuildSetAutoClearReq *)msg;

	GuildRoute::Instance().OnSetAutoClear(role, cmd->is_auto_clear);
}

void MessageHandler::OnGuildSkillUplevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildSkillUplevel *pro = (Protocol::CSGuildSkillUplevel *)msg;
	if (Protocol::GUILD_SKILL_UP_LEVEL==pro->up_type)
	{
		role->GetRoleGuild()->OnSkillUplevel(pro->skill_index);
	}
	else if (Protocol::GUILD_SKILL_UP_LEVEL_ALL == pro->up_type)
	{
		role->GetRoleGuild()->OnSkillUplevelALLByLowestLevel();
	}

	role->GetTaskManager()->OnDoOperator(TASK_OPERATOR_TYPE_PUSH_GUILD_SKILL_UP);
}

void MessageHandler::OnGuildUpTotemLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	if (role->GetGuildID() > 0)
	{
		Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
		if (NULL != guild)
		{
			guild->GuildUpTotemLevel(role);
		}
	}
}

void MessageHandler::OnGuildBoxOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildBoxOperate *pro = (Protocol::CSGuildBoxOperate *)msg;
	GuildManager::Instance().GuildBoxOperate(role, pro->operate_type, pro->param1, pro->param2);
}

void MessageHandler::OnGuildCommonReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildCommonReq *gcr = (Protocol::CSGuildCommonReq *)msg;

	switch (gcr->req_type)
	{
	case Protocol::GUILD_COMMON_REQ_TYPE_FETCH_REWARD:
	{
		GuildManager::Instance().OnFetchDailyReward(role);
	}
	break;

	case Protocol::GUILD_COMMON_REQ_TYPE_GIVE_GONGZI:
	{
		GuildManager::Instance().OnGiveGongzi(role);
	}
	break;

	}
}

void MessageHandler::OnGuildFetchBossRedbag(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFetchGuildBossRedbag *pro = (Protocol::CSFetchGuildBossRedbag *)msg;
	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_JOIN_GUILD);
		return;
	}
	GuildManager::Instance().OnFetchGuildBossRedbag(role, pro->index);
}

void MessageHandler::OnGuildPaoSaizi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_JOIN_GUILD);
		return;
	}
	guild->GetGuildMemberManager().ReqGuildPaoSaizi(role);
}

void MessageHandler::OnReqGuildPaoSaizi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		return;
	}
	guild->GetGuildMemberManager().SyncSCGulidSaiziInfo(role);
}

void MessageHandler::OnGuildRedPaperListInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	GuildManager::Instance().SendGuildRedPaperlistInfo(role);
}

void MessageHandler::OnGuildRedPaperCreateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCreateGuildRedPaperReq *pro = (Protocol::CSCreateGuildRedPaperReq *)msg;
	role->GetRoleGuild()->CreateGuildRedPaper(pro->paper_seq, pro->fetech_time, pro->red_paper_index);
}

void MessageHandler::OnGuildRedPaperSingalChatReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSingleChatRedPaperRole *pro = (Protocol::CSSingleChatRedPaperRole *)msg;
	
	GuildManager::Instance().SingalChatRedPaperRole(role, pro->red_paper_index);
}

void MessageHandler::OnGuildMazeOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		return;
	}

	Protocol::CSGuildMazeOperate *pro = (Protocol::CSGuildMazeOperate *)msg;
	switch (pro->operate_type)
	{
	case Protocol::GUILD_MAZE_OPERATE_TYPE_GET_INFO:
		guild->OnGetMazeInfo(role);
		break;

	case Protocol::GUILD_MAZE_OPERATE_TYPE_SELECT:
		guild->OnMazeSelectDoor(role, pro->param1);
		break;
	}
}

void MessageHandler::OnGuildSigninReq(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	const GuildActivityOther &other_cfg = LOGIC_CONFIG->GetGuildActivityCfg().OtherCfg();
	if (!other_cfg.signin_is_open || role->GetLevel() < other_cfg.signin_need_level)
	{
		return;
	}

	Protocol::CSGuildSinginReq* gsr = (Protocol::CSGuildSinginReq*)msg;
	Guild* guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		return;
	}

	switch (gsr->req_type)
	{
	case::Protocol::GUILD_SINGIN_REQ_TYPE_SIGNIN:
	{
		guild->OnSignin(role);
	}
	break;

	case::Protocol::GUILD_SINGIN_REQ_TYPE_FETCH_REWARD:
	{
		guild->FethSinginReward(role, gsr->param1);
	}
	break;

	case::Protocol::GUILD_SINGIN_REQ_ALL_INFO:
	{
		guild->SendSigninAllInfo(role);
	}
	break;

	default:
		break;
	}
}

void MessageHandler::OnGuildChangeAvatar(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildChangeAvatar *gca = (Protocol::CSGuildChangeAvatar*)msg;
	GuildManager::Instance().ChangeAvatar(role, gca->avatar_timestamp);
}

void MessageHandler::OnGuildBiaocheOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSBiaoCheOpera *bco = (Protocol::CSBiaoCheOpera*)msg;

	switch (bco->opera_type)
	{
		case Protocol::BIAOCHE_OPERA_TYPE_START:
		{
			GuildManager::Instance().OnBiaoCheStart(role, bco->param_1);
		}
		break;

		case Protocol::BIAOCHE_OPERA_TYPE_TRANS:
		{
			GuildManager::Instance().OnTransToGuildBiaoCheReq(role, role->GetGuildID());
		}
		break;

		case Protocol::BIAOCHE_OPERA_TYPE_BIAOCHE_POS:
		{
			GuildManager::Instance().OnGetBiaoChePos(role, role->GetGuildID());
		}
		break;
	}
}

void MessageHandler::OnGuildFetchTerritoryReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildTerritoryWelfOperate *pro = (Protocol::CSGuildTerritoryWelfOperate *)msg;

	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_JOIN_GUILD);
		return;
	}
	switch(pro->operate_type)
	{
	case Protocol::GTW_FETCH_REWARD:
		{
			GuildManager::Instance().OnFetchTerritoryReward(role, pro->param1);
		}
		break;

	case Protocol::GTW_FETCH_EXTRA_REWARD:
		{
			GuildManager::Instance().OnFetchTerritoryExtraReward(role);
		}
		break;
	}
}

void MessageHandler::OnGuildYaojiangReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnGuildGetShenluBindCoin(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnGuildGetLonghuiInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnGuildTrainLonghui(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnGuildUPgradeLonghui(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnGuildSetAutoKickoutSetup(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildSetAutoKickoutSetup *gsaks = (Protocol::CSGuildSetAutoKickoutSetup *)msg;
	GuildRoute::Instance().GuildSetAutoKickoutSetup(role, gsaks);
}

void MessageHandler::OnGuildCallIn(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildCallIn *gci = (Protocol::CSGuildCallIn *)msg;

	GuildRoute::Instance().GuildCallIn(role, gci);
}

void MessageHandler::OnGuildCheckCanDelate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildCheckCanDelate *gccd = (Protocol::CSGuildCheckCanDelate *)msg;
	GuildManager::Instance().OnCheckCanDelate(role, gccd);
}

void MessageHandler::OnGuildFbStartReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	GuildManager::Instance().OnStartGuildFbReq(role);
}

void MessageHandler::OnGuildBonfireStartReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	GuildManager::Instance().OnStartGuildBonfireReq(role);
}

void MessageHandler::OnGuildZhufuMemberLuckyReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildZhuLuckyReq *gzlr = (Protocol::CSGuildZhuLuckyReq *)msg;
	GuildManager::Instance().OnZhuFuMemberLuckyReq(role, gzlr->be_zhufu_uid);
}

void MessageHandler::OnGetAllMemberLuckyInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	GuildManager::Instance().OnGetAllMemberLuckyInfoReq(role);
}

void MessageHandler::OnInviteZhuFuLuckyReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSInviteLuckyZhufu *ilz = (Protocol::CSInviteLuckyZhufu *)msg;
	GuildManager::Instance().OnInviteZhuFuLuckyReq(role, ilz->invite_uid);
}

void MessageHandler::OnGuildCallBeastReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	GuildManager::Instance().OnCallBeastReq(role);
}

void MessageHandler::OnGuildActiveDegreeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	GuildManager::Instance().OnActiveDegreeInfoReq(role);
}

void MessageHandler::OnGuildBossOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildBossOperate *cmd = (Protocol::CSGuildBossOperate *)msg;
	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL == guild)
	{
		role->NoticeNum(errornum::EN_GUILD_YOU_NOT_JOIN_GUILD);
		return;
	}

	if (Protocol::GUILD_BOSS_UPLEVEL == cmd->oper_type)
	{
		guild->OnUplevelBoss(role);
	}
	else if (Protocol::GUILD_BOSS_CALL == cmd->oper_type)
	{
		guild->CallGuildBoss(role, 0 != cmd->param);
	}
	else if (Protocol::GUILD_BOSS_INFO_REQ == cmd->oper_type)
	{
		guild->SyncGuildBossInfo(role->GetNetId());
	}
}

void MessageHandler::OnGetDailyWelfareInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetWelfare()->SendWelfareInfo();
}

void MessageHandler::OnFetchSevenDayLoginReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFetchSevenDayLoginReward *fsdlr = (Protocol::CSFetchSevenDayLoginReward *)msg;
	role->GetWelfare()->FetchSevenDayLoginReward(fsdlr->fetch_day);
}

void MessageHandler::OnGetOfflineExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	//Protocol::CSGetOfflineExp *goe = (Protocol::CSGetOfflineExp *)msg;
	//
	//role->GetWelfare()->GetOfflineExp(goe->type);
}

void MessageHandler::OnGetDailyFindWelfare(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGetDailyFindWelfare *gdfw = (Protocol::CSGetDailyFindWelfare*)msg;
	role->GetWelfare()->GetDailyFind(gdfw->dailyfind_type, gdfw->get_type);
}

void MessageHandler::OnFetchSignInReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSWelfareSignInReward *wsir = (Protocol::CSWelfareSignInReward*)msg;

	if (wsir->operate_type == Protocol::CSWelfareSignInReward::PER_DAY_SIGNIN_REWARD)
	{
		role->GetWelfare()->FetchSignInRewardHandle(wsir->param, wsir->is_quick);
	}
	else if(wsir->operate_type == Protocol::CSWelfareSignInReward::ACCUMULATION_REWARD)
	{
		role->GetWelfare()->FetchAccumulationSignInReward(wsir->param);
	}	
}

void MessageHandler::OnFetchOnlineReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSWelfareOnlineReward *wor = (Protocol::CSWelfareOnlineReward*)msg;
	role->GetWelfare()->FetchOnlineReward(wor->part);
}

void MessageHandler::OnSignInFindBack(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	//Protocol::CSWelfareSignInFindBack *wsifb = (Protocol::CSWelfareSignInFindBack*)msg;
	role->GetWelfare()->SignInFindBack();
}

void MessageHandler::OnFetchHappytreeReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSWelfareFetchHappytreeReward *wfhtr = (Protocol::CSWelfareFetchHappytreeReward*)msg;
	role->GetWelfare()->FetchHappyTreeReward(wfhtr->type);
}

void MessageHandler::OnFetchChongjihaoliReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSWelfareFetchChongjihaoliReward *wcjhl = (Protocol::CSWelfareFetchChongjihaoliReward*)msg;
	role->GetWelfare()->FetchChongjihaoliReward(wcjhl->level);
}

void MessageHandler::OnWelfareActivityFind(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSWelfareActivityFind *waf = (Protocol::CSWelfareActivityFind*)msg;
	role->GetWelfare()->ActivityFind(waf->find_type, (0 != waf->is_free_find));
}

void MessageHandler::OnWelfareAutoActivity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	// Protocol::CSWelfareAutoActivity *waa = (Protocol::CSWelfareAutoActivity*)msg;
	// role->GetWelfare()->OnAutoActivityReq(waa->find_type);
}


void MessageHandler::OnSpeakerTalk(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	// 大跨服中不允许频道聊天
	int PLATFROM_BIG_CROSS_TYPE = 2;
	if (CrossConfig::Instance().IsHiddenServer() && PLATFROM_BIG_CROSS_TYPE == CrossConfig::Instance().GetHiddenServerCrossType())
	{
		gstr::SendError(role, "en_channel_speaker_scene_limit");
		return;
	}

	// 等级限制、vip等级限制
	if (!WorldStatus::Instance().CheckSatisfyLevel(role, OPENLEVEL_LIMIT_TYPE_SPEAKER))
	{
		return;
	}

	if (role->IsForbidTalk() || ChatManager::Instance().IsInForbidChannelChatTime(OPENLEVEL_LIMIT_TYPE_SPEAKER))
	{
		return;
	}

	static Protocol::CSSpeaker speaker;
	speaker.ResetSpeakMsg();

	int offset = static_cast<int>(UNSTD_OFFSETOFF(Protocol::CSSpeaker, speaker_msg));
	if (length < offset) return;

	const char *pt = (const char *)msg;

	memcpy((void*)&speaker, pt, offset);
	pt += offset; length -= offset;

	if (static_cast<int>(speaker.speaker_msg_length) < 0 || static_cast<int>(speaker.speaker_msg_length) >= MAX_SPEAKER_MSG_LENGTH || 
		length != static_cast<int>(speaker.speaker_msg_length)) 
	{
		return;
	}

	memcpy(&speaker.speaker_msg[0], pt, length);
	speaker.speaker_msg[sizeof(speaker.speaker_msg) - 1] = '\0';

	if (role->GetSublock()->CheckHasLock()) return;

	Knapsack *knapsack = role->GetKnapsack();
	if (knapsack->IsLock())
	{
		role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	globalconfig::OtherGlobalConfig &other_global_cfg = GLOBALCONFIG->GetOtherGlobalCfg();

	const char *consume_reason = "speaker";
	int consume_gold_num = other_global_cfg.speaker_need_gold;
	int comsume_item_id = other_global_cfg.speaker_item_id;

	if (Protocol::CSSpeaker::SPEAKER_TYPE_CROSS == speaker.speaker_type)
	{
		// 现在跨服可以消耗，允许发喇叭
// 		if (CrossConfig::Instance().IsHiddenServer())
// 		{
//  		return;
//  	}

		if (!CrossManager::Instance().IsCrossServerConnected())
		{
			role->NoticeNum(errornum::EN_CROSS_NOT_CONNECTED_TO_CROSS_SERVER);
			return;
		}

		consume_gold_num = other_global_cfg.cross_speaker_need_gold;
		comsume_item_id = other_global_cfg.cross_speaker_item_id;
		consume_reason = "cross_speaker";
	}

	if (comsume_item_id > 0)
	{
		if (knapsack->ConsumeItem(comsume_item_id, 1, consume_reason))
		{
			consume_gold_num = 0;
		}
	}

	if (consume_gold_num > 0 && !knapsack->GetMoney()->UseGold(consume_gold_num, consume_reason)) 
	{
		role->NoticeNum(errornum::EN_GOLD_NOT_ENOUGH);
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());

	static Protocol::SCSpeaker speaker_ack;
	speaker_ack.from_uid = UidToInt(role->GetUserId());
	speaker_ack.unique_uid = uuid;
	role->GetName(speaker_ack.username);
	speaker_ack.sex = role->GetSex();
	speaker_ack.camp = role->GetCamp();
	speaker_ack.camp_post = 0;
	speaker_ack.guild_post = role->GetGuildPost();
	speaker_ack.prof = role->GetProf();
	speaker_ack.authourity_type = role->GetAuthority();
	speaker_ack.vip_level = role->GetVip()->GetVipLevel();
	speaker_ack.content_type = speaker.content_type;
	speaker_ack.avatar_timestamp = role->GetAvatarTimeStamp();
	speaker_ack.speaker_type = speaker.speaker_type;
	speaker_ack.tuhaojin_color = role->GetTuHaoJin()->GetTuhaojinColor();
	speaker_ack.bigchatface_status = role->GetBigChatFace()->GetBigChatFaceStatus();
	speaker_ack.personalize_window_type = role->GetPersonalizeWindow()->GetCurUseBubbleWindow();
	speaker_ack.personalize_window_bubble_type = role->GetPersonalizeWindow()->GetCurUseBubbleWindow();
	speaker_ack.personalize_window_avatar_type = role->GetPersonalizeWindow()->GetCurUseAvatarWindowForClient();
	LocalConfig::Instance().GetPlatName(speaker_ack.plat_name);
	speaker_ack.server_id = role->GetUserId().db_index;
	speaker_ack.is_record_msg = 0;
	speaker_ack.send_time_stamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	speaker_ack.speaker_msg_length = speaker.speaker_msg_length;
	F_STRNCPY(speaker_ack.speaker_msg, speaker.speaker_msg, sizeof(speaker_ack.speaker_msg));

	int sendlen = int(sizeof(speaker_ack) - MAX_SPEAKER_MSG_LENGTH + speaker_ack.speaker_msg_length);


	{
		int send_len = 0;
		static  char tmp_buffer[MAX_MSG_SIZE]; memset(tmp_buffer, 0, sizeof(tmp_buffer));
		crossgameprotocal::CrossGameSpeaker *cgs = (crossgameprotocal::CrossGameSpeaker *)tmp_buffer;
		*cgs = crossgameprotocal::CrossGameSpeaker();
		memcpy(tmp_buffer + sizeof(crossgameprotocal::CrossGameSpeaker), (const char*)&speaker_ack, sendlen);
		send_len = sendlen + sizeof(crossgameprotocal::CrossGameSpeaker);
		if (Protocol::CSSpeaker::SPEAKER_TYPE_CROSS == speaker.speaker_type)
		{
			// 跨服喇叭走默认跨服通道广播
			InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_OTHER, UniqueUserIDToLongLong(role->GetUniqueUserID()), (const char *)cgs, send_len);
			World::GetInstWorld()->SendToAllGateway((const char*)&speaker_ack, sendlen);
		}
		else
		{
			if (CrossConfig::Instance().IsHiddenServer())
			{
				InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_BY_UUID, UniqueUserIDToLongLong(role->GetUniqueUserID()), (const char *)cgs, send_len);
			}
			else 
			{
				World::GetInstWorld()->SendToAllGatewayAndCrossUser((const char*)&speaker_ack, sendlen);
			}
		}
	}

	ChatManager::Instance().AddWorldChatBoard((const char *)&speaker_ack, sendlen, true);

	role->NoticeNum(noticenum::NT_SPEAK_TALK_SUCC);
}

void MessageHandler::OnSaveHangupSet(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSaveRoleHangupSet *shus = (Protocol::CSSaveRoleHangupSet *)msg;
	role->GetHangup()->SaveHangupSetData(shus->hangup_set);
}

void MessageHandler::OnSaveCustomSet(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSaveRoleCustomSet *srcs = (Protocol::CSSaveRoleCustomSet *)msg;
	role->GetHangup()->SaveCustomSetData(srcs->custom_set);
}

void MessageHandler::OnDailyWelfareTipsReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnRoleResetName(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRoleResetName *rrn = (Protocol::CSRoleResetName *)msg;

	if (0 == rrn->new_name[0]) return;

	rrn->new_name[sizeof(rrn->new_name) - 1] = 0;

	static const bool NAMEFILTER_OPEN_FLAG = ServerConfigPool::Instance().common_config.namefilter_open_flag;
	if (NAMEFILTER_OPEN_FLAG && NameFilter::Instance().HasSensitiveWord(sizeof(GameName), rrn->new_name)) return;

	GameName role_name = {0};
	role->GetName(role_name);

	if (rrn->is_item_reset > 0)
	{
		role->OnResetNameReq(rrn->new_name, true);
	}
	else
	{
		for (int i = sizeof(GameName) - 1; i >= 0; --i)
		{
			if (']' == role_name[i])
			{
				role->OnResetNameReq(rrn->new_name);
				break;
			}
		}
	}
}

void MessageHandler::OnRoleSetZombieReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRoleSetZombieReq *rszr = (Protocol::CSRoleSetZombieReq *)msg;
	role->SetZombie(0 != rszr->is_zombie);
}

void MessageHandler::OnActiveFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSActiveFetchReward *afr = (Protocol::CSActiveFetchReward *)msg;
	if (Protocol::CSActiveFetchReward::FETCH_ACTIVE_DEGREE_REWARD == afr->operate_type)
	{
		role->GetActiveDegree()->OnFetchActiveReward(afr->param);
	}
	else if (Protocol::CSActiveFetchReward::FETCHE_TOTAL_ACTIVE_DEGREE_REWARD == afr->operate_type)
	{
		role->GetActiveDegree()->OnFetchReward(afr->param);
	}
	else if (Protocol::CSActiveFetchReward::FETCHE_TOTAL_ACTIVE_DEGREE_REWARD_ON_DAY == afr->operate_type)
	{
		role->GetActiveDegree()->OnFetchRewardOnDay(afr->param);
	}
}

void MessageHandler::OnEvaluateRole(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSEvaluateRole *er = (Protocol::CSEvaluateRole *)msg;
	role->GetRoleEvaluate()->OnEvaluateRole(er->uid);
}

// 兑换
void MessageHandler::OnConvertScoreToItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSScoreToItemConvert *csc = (Protocol::CSScoreToItemConvert*)msg;

	if (length != static_cast<int>(sizeof(Protocol::CSScoreToItemConvert))) 
	{
		return;
	}

	static Protocol::CSScoreToItemConvert convert;
	memcpy((void *)&convert, csc, length);

	LOGIC_CONFIG->GetConvertShop().ConvertScoreToItem(role, convert);
}

void MessageHandler::OnGetConvertRecordInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetConvertRecord()->SendInfo();
}

void MessageHandler::OnGetSocreInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleChestShop()->SendScoreInfo();
}

void MessageHandler::OnClientSpecialOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSClientSpecialOperate *cso = (Protocol::CSClientSpecialOperate *)msg;

	switch (cso->client_operate_type)
	{
	case Protocol::CSClientSpecialOperate::CLIENT_OPERATE_TYPE_GUILD_GUIDE:
		//role->GetTaskManager()->OnOperate(TaskOperate::OT_GUILD_GUIDE);
		break;
	}
}

void MessageHandler::OnSublockCreatePw(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSubLockCreatePw *sc = (Protocol::CSSubLockCreatePw *)msg;

	role->GetSublock()->CreatePw(sc->password_md5);
}

void MessageHandler::OnSublockClearPw(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSubLockDeletePw *sc = (Protocol::CSSubLockDeletePw *)msg;

	role->GetSublock()->DeletePw(sc->password_md5);
}

void MessageHandler::OnSublockLock(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetSublock()->Lock();
}

void MessageHandler::OnSublockUnlock(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSubLockUnlock *su = (Protocol::CSSubLockUnlock *)msg;

	role->GetSublock()->Unlock(su->password_md5);
}

void MessageHandler::OnCallXiannv(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSXiannvCall *xvc = (Protocol::CSXiannvCall*)msg;

	role->GetXiannvManager()->FightOut(xvc->pos_list);
}

void MessageHandler::OnXiannvRename(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSXiannvRename *xvr = (Protocol::CSXiannvRename*)msg;

	role->GetXiannvManager()->Rename(xvr->xiannv_id,xvr->new_name);
}

void MessageHandler::OnXiannvActiveReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSXiannvActiveReq *xvar = (Protocol::CSXiannvActiveReq*)msg;
	role->GetXiannvManager()->OnActiveReq(xvar->xiannv_id, xvar->item_index);
}

void MessageHandler::OnXiannvAddZizhiReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSXiannvAddZizhiReq *xvzr = (Protocol::CSXiannvAddZizhiReq*)msg;
	bool ret = role->GetXiannvManager()->OnAddZizhiReq(xvzr->xiannv_id, 0 != xvzr->auto_buy);

	role->SendOperateResult(Protocol::SCOperateResult::OP_XIANNV_PROMOTE_ZIZHI, ret ? 1 : 0);
}

void MessageHandler::OnXiannvHuanHuaUpLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSXiannvHuanHuaUpLevelReq *xvul = (Protocol::CSXiannvHuanHuaUpLevelReq*)msg;
	bool ret = role->GetXiannvManager()->OnHuanHuaUplevelReq(xvul->huanhua_id, 0 != xvul->auto_buy);

	role->SendOperateResult(Protocol::SCOperateResult::OP_XIANNV_PROMOTE_HUANHUA_LEVEL, ret ? 1 : 0);
}

void MessageHandler::OnXiannvChangeSkillReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	// 	Protocol::CSXiannvChangeSkillReq *xvul = (Protocol::CSXiannvChangeSkillReq*)msg;
	// 
	// 	role->GetSkillManager()->AddXiannvSkill(xvul->xiannv_skill_id, xvul->xiannv_skill_level);
}

void MessageHandler::OnXiannvShengwuReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSXiannvShengwuReq *xnsqr = (Protocol::CSXiannvShengwuReq*)msg;
	role->GetXiannvManager()->OnShengwuReq(xnsqr->req_type, xnsqr->param1, xnsqr->param2, xnsqr->param3);
}

void MessageHandler::OnXiannvActiveHuanhuaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSXiannvActiveHuanhua *xvar = (Protocol::CSXiannvActiveHuanhua*)msg;
	role->GetXiannvManager()->OnActiveHuanHuaReq(xvar->xiannv_id, xvar->item_index);
}

void MessageHandler::OnXiannvChangeImageReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSXiannvImageReq *xvar = (Protocol::CSXiannvImageReq*)msg;
	role->GetXiannvManager()->OnHuanHuaReq(xvar->huanhua_id);
}

void MessageHandler::OnFamousManOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFamousManOpera *xvc = (Protocol::CSFamousManOpera*)msg;

	role->GetFamousMan()->FamousManOpera(role, xvc->opera_type);
}

void MessageHandler::OnWabaoOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSWabaoOperaReq *wbor= (Protocol::CSWabaoOperaReq*)msg;
	role->GetWabao()->OnOperaReq(wbor->opera_type, wbor->is_killed);
}

void MessageHandler::OnJilianOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSJilianOpera *jlo = (Protocol::CSJilianOpera*)msg;
	if (Protocol::CSJilianOpera::OPERA_TYPE_JILIAN == jlo->opera_type)
	{
		role->GetJilian()->Start(jlo->param);
	}
	else if (Protocol::CSJilianOpera::OPERA_TYPE_CONTINUE == jlo->opera_type)
	{
		role->GetJilian()->Continue();
	}
}

void MessageHandler::OnChongzhiFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSChongzhiFetchReward *czfr= (Protocol::CSChongzhiFetchReward*)msg;
	if (Protocol::CHONGZHI_REWARD_TYPE_SPECIAL_FIRST == czfr->type)
	{
		role->GetRoleActivity()->FetchSpecialFirstChongzhiReward(czfr->param);
	}
	else if (Protocol::CHONGZHI_REWARD_TYPE_DAILY_FIRST == czfr->type)
	{
		role->GetRoleActivity()->FetchDailyFirstChongzhiReward();
	}
	else if (Protocol::CHONGZHI_REWARD_TYPE_DAILY_TOTAL == czfr->type)
	{
		role->GetRoleActivity()->FetchDailyTotalChongzhiReward(czfr->param);
	}
	else if (Protocol::CHONGZHI_REWARD_TYPE_DIFF_WEEKDAY_TOTAL == czfr->type)
	{
		role->GetRoleActivity()->FetchDiffWeekdayTotalChongzhiRewad(czfr->param);
	}
	else if (Protocol::CHONGZHI_REWARD_TYPE_FIRST == czfr->type)
	{
		role->GetRoleActivity()->FetchFirstChongzhiReward(role->GetLevel(), czfr->param);
	}
	else if (Protocol::CHONGZHI_REWARD_TYPE_DAILY == czfr->type)
	{
		role->GetRoleActivity()->FetchDailyChongzhiReward(czfr->param, czfr->param2);
	}
	else if (Protocol::CHONGZHI_REWARD_TYPE_DAILY_TIMES == czfr->type)
	{
		role->GetRoleActivity()->FetchDailyChongzhiTimesReward(czfr->param);
	}
	else if (Protocol::CHONGZHI_REWARD_TYPE_DAILY2 == czfr->type)
	{
		role->GetRoleActivity()->FetchDailyChongzhiReward2(czfr->param);
	}
}

void MessageHandler::OnZaiChongzhiFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSZaiChongzhiFetchReward *cmd = (Protocol::CSZaiChongzhiFetchReward*)msg;
	if (1 == cmd->is_third)
	{
		role->GetRoleActivity()->FetchThirdChongzhiReward();
	}
	else
	{	
		role->GetRoleActivity()->FetchZaiChongzhiReward();
	}
}

void MessageHandler::OnChargeWantMoneyFetchMsg(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->GetTotalChongzhiWantMoneyInfo();
}

void MessageHandler::OnChargeWantMoneyFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->FetchTotalChongzhiWantMoneyReward();
}

void MessageHandler::OnGoonMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGoonMount *gm = (Protocol::CSGoonMount*)msg;
	if (role->GetMultiMount()->GetCurMountId() > 0)
	{
		role->GetMultiMount()->MountOn(0 != gm->goon_mount);
	}
	else
	{
		role->GetMountManager()->GoonMount(0 != gm->goon_mount);
	}
}

void MessageHandler::OnStrengthMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnUpgradeMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUpgradeMount *um = (Protocol::CSUpgradeMount*)msg;
	bool ret = MountFunction::Upgrade(role, 1 == um->auto_buy, um->repeat_times);
	int grade_bless_val = role->GetMountManager()->GetGradeBlessVal();
	role->SendOperateResult(Protocol::SCOperateResult::OP_MOUNT_UPGRADE, ret, grade_bless_val);
}

void MessageHandler::OnUseMountImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUseMountImage *um = (Protocol::CSUseMountImage*)msg;
	if (1 == um->is_temporary_image)
	{
		role->GetMountManager()->UseTemporaryImage(um->image_id);
	}
	else
	{
		role->GetMountManager()->UseImage(um->image_id);
	}

}

void MessageHandler::OnMountGetInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetMountManager()->SendMountInfo();
}

void MessageHandler::OnMountUplevelEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMountUplevelEquip *mue = (Protocol::CSMountUplevelEquip *)msg;
	MountFunction::UplevelEquip(role, (int)mue->equip_idx);
}

void MessageHandler::OnMountSpecialImgUpgrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMountSpecialImgUpgrade *siu = (Protocol::CSMountSpecialImgUpgrade*)msg;
	MountFunction::MountSpecialImgUpgrade(role, siu->special_image_id);
}

void MessageHandler::OnMountSkillUplevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMountSkillUplevelReq *sur = (Protocol::CSMountSkillUplevelReq*)msg;
	MountFunction::UplevelSkill(role, sur->skill_idx, 0 != sur->auto_buy);
}

void MessageHandler::OnMountUpStarLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMountUpStarLevel *cmd = (Protocol::CSMountUpStarLevel*)msg;

	int old_grade = role->GetMountManager()->GetMountParam()->grade;

	const MountGradeConfig * grade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(old_grade);
	if (NULL == grade_cfg) return;

	int max_up_star_stuff_times = grade_cfg->pack_num;

	for (int i = 0; i < cmd->loop_times && i < max_up_star_stuff_times; i++)
	{
		bool ret = MountFunction::UpStarLevel(role, cmd);
		if (!ret || old_grade != role->GetMountManager()->GetMountParam()->grade)
		{
			role->SendOperateResult(Protocol::SCOperateResult::OP_MOUNT_UPSTAR, 0);
			return;
		}
	}

	role->SendOperateResult(Protocol::SCOperateResult::OP_MOUNT_UPSTAR, 1);
}

void MessageHandler::OnGoonFightMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGoonFightMount *gm = (Protocol::CSGoonFightMount*)msg;
	role->GetFightMountManager()->GoonFightMount(0 != gm->goon_mount);
}

void MessageHandler::OnStrengthFightMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnUpgradeFightMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUpgradeFightMount *um = (Protocol::CSUpgradeFightMount*)msg;
	bool ret = FightMountFunction::Upgrade(role, 1 == um->auto_buy, um->repeat_times);
	role->SendOperateResult(Protocol::SCOperateResult::OP_FIGHT_MOUNT_UPGRADE, ret);
}

void MessageHandler::OnUseFightMountImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUseFightMountImage *um = (Protocol::CSUseFightMountImage*)msg;
	role->GetFightMountManager()->UseImage(um->image_id);
}

void MessageHandler::OnFightMountGetInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetFightMountManager()->SendFightMountInfo();
}

void MessageHandler::OnFightMountUplevelEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFightMountUplevelEquip *mue = (Protocol::CSFightMountUplevelEquip *)msg;
	FightMountFunction::UplevelEquip(role, mue->equip_idx);
}

void MessageHandler::OnFightMountSpecialImgUpgrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFightMountSpecialImgUpgrade *siu = (Protocol::CSFightMountSpecialImgUpgrade*)msg;
	FightMountFunction::FightMountSpecialImgUpgrade(role, siu->special_image_id);
}

void MessageHandler::OnFightMountSkillUplevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFightMountSkillUplevelReq *sur = (Protocol::CSFightMountSkillUplevelReq*)msg;
	FightMountFunction::UplevelSkill(role, sur->skill_idx, 0 != sur->auto_buy);
}

void MessageHandler::OnFightMountUpStarLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFightMountUpStarLevel *cmd = (Protocol::CSFightMountUpStarLevel*)msg;

	int old_grade = role->GetFightMountManager()->GetFightMountParam()->grade;

	const FightMountGradeConfig * grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(old_grade);
	if (NULL == grade_cfg) return;

	int max_up_star_stuff_times = grade_cfg->pack_num;

	for (int i = 0; i < cmd->loop_times && i < max_up_star_stuff_times; i++)
	{
		bool ret = FightMountFunction::UpStarLevel(role, cmd);
		if (!ret || old_grade != role->GetFightMountManager()->GetFightMountParam()->grade)
		{
			role->SendOperateResult(Protocol::SCOperateResult::OP_FIGHT_MOUNT_UPSTAR, 0);
			return;
		}
	}

	role->SendOperateResult(Protocol::SCOperateResult::OP_FIGHT_MOUNT_UPSTAR, 1);
}

void MessageHandler::OnUpgradeWing(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUpgradeWing *um = (Protocol::CSUpgradeWing*)msg;
	bool ret = WingFunction::Upgrade(role, 1 == um->auto_buy, um->repeat_times);
	role->SendOperateResult(Protocol::SCOperateResult::OP_WING_UPGRADE, ret ? 1 : 0);
}

void MessageHandler::OnUseWingImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUseWingImage *um = (Protocol::CSUseWingImage*)msg;
	if (1 == um->is_temporary_image)
	{
		role->GetWing()->UseTemporaryImage(um->image_id);
	}
	else
	{
		role->GetWing()->UseImage(um->image_id);
	}
}

void MessageHandler::OnWingGetInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetWing()->SendWingInfo();
}

void MessageHandler::OnWingUplevelEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSWingUplevelEquip *mue = (Protocol::CSWingUplevelEquip *)msg;
	WingFunction::UplevelEquip(role, mue->equip_idx);
}

void MessageHandler::OnWingSpecialImgUpgrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSWingSpecialImgUpgrade *siu = (Protocol::CSWingSpecialImgUpgrade*)msg;
	WingFunction::WingSpecialImgUpgrade(role, siu->special_image_id);
}

void MessageHandler::OnWingSkillUplevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSWingSkillUplevelReq *sur = (Protocol::CSWingSkillUplevelReq*)msg;
	WingFunction::UplevelSkill(role, sur->skill_idx, 0 != sur->auto_buy);
}

void MessageHandler::OnWingUpStarLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSWingUpStarLevel *cmd = (Protocol::CSWingUpStarLevel*)msg;

	int old_grade = role->GetWing()->GetWingParam()->grade;

	const WingGradeConfig * grade_cfg = LOGIC_CONFIG->GetWingConfig().GetGradeConfig(old_grade);
	if (NULL == grade_cfg) return;

	int max_up_star_stuff_times = grade_cfg->pack_num;

	for (int i = 0; i < cmd->loop_times && i < max_up_star_stuff_times; i++)
	{
		bool ret = WingFunction::UpStarLevel(role, cmd);
		if (!ret || old_grade != role->GetWing()->GetWingParam()->grade)
		{
			role->SendOperateResult(Protocol::SCOperateResult::OP_WING_UPSTAR, 0);
			return;
		}
	}

	role->SendOperateResult(Protocol::SCOperateResult::OP_WING_UPSTAR, 1);
}

void MessageHandler::OnUpgradeHalo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUpgradeHalo *um = (Protocol::CSUpgradeHalo*)msg;

	bool ret = HaloFunction::Upgrade(role, 1 == um->auto_buy, um->repeat_times);
	role->SendOperateResult(Protocol::SCOperateResult::OP_HALO_UPGRADE, ret);
}

void MessageHandler::OnUseHaloImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUseHaloImage *um = (Protocol::CSUseHaloImage*)msg;
	role->GetHalo()->UseImage(um->image_id);
}

void MessageHandler::OnHaloGetInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetHalo()->SendHaloInfo();
}

void MessageHandler::OnHaloUplevelEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSHaloUplevelEquip *mue = (Protocol::CSHaloUplevelEquip *)msg;
	HaloFunction::UplevelEquip(role, mue->equip_idx);
}

void MessageHandler::OnHaloSpecialImgUpgrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSHaloSpecialImgUpgrade *siu = (Protocol::CSHaloSpecialImgUpgrade*)msg;
	HaloFunction::HaloSpecialImgUpgrade(role, siu->special_image_id);
}

void MessageHandler::OnHaloSkillUplevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSHaloSkillUplevelReq *sur = (Protocol::CSHaloSkillUplevelReq*)msg;
	HaloFunction::UplevelSkill(role, sur->skill_idx, 0 != sur->auto_buy);
}

void MessageHandler::OnHaloUpStarLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSHaloUpStarLevel *cmd = (Protocol::CSHaloUpStarLevel*)msg;

	int old_grade = role->GetHalo()->GetHaloParam()->grade;

	const HaloGradeConfig * grade_cfg = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(old_grade);
	if (NULL == grade_cfg) return;

	int max_up_star_stuff_times = grade_cfg->pack_num;

	for (int i = 0; i < cmd->loop_times && i < max_up_star_stuff_times; i++)
	{
		bool ret = HaloFunction::UpStarLevel(role, cmd);
		if (!ret || old_grade != role->GetHalo()->GetHaloParam()->grade)
		{
			role->SendOperateResult(Protocol::SCOperateResult::OP_HALO_UPSTAR, 0);
			return;
		}
	}

	role->SendOperateResult(Protocol::SCOperateResult::OP_HALO_UPSTAR, 1);
}

void MessageHandler::OnUpgradeShengong(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUpgradeShengong *um = (Protocol::CSUpgradeShengong*)msg;
	bool ret = ShengongFunction::Upgrade(role, 1 == um->auto_buy, um->repeat_times);
	role->SendOperateResult(Protocol::SCOperateResult::OP_SHENGONG_UPGRADE, ret);
}

void MessageHandler::OnUseShengongImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUseShengongImage *um = (Protocol::CSUseShengongImage*)msg;
	role->GetShengong()->UseImage(um->image_id);
}

void MessageHandler::OnShengongUpStarLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShengongUpStarLevel *cmd = (Protocol::CSShengongUpStarLevel*)msg;

	int old_grade = role->GetShengong()->GetShengongParam()->grade;

	const ShengongGradeConfig * grade_cfg = LOGIC_CONFIG->GetShengongConfig().GetGradeConfig(old_grade);
	if (NULL == grade_cfg) return;

	int max_up_star_stuff_times = grade_cfg->pack_num;

	for (int i = 0; i < cmd->loop_times && i < max_up_star_stuff_times; i++)
	{
		bool ret = ShengongFunction::UpStarLevel(role, cmd);
		if (!ret || old_grade != role->GetShengong()->GetShengongParam()->grade)
		{
			role->SendOperateResult(Protocol::SCOperateResult::OP_SHENGONG_UPSTAR, 0);
			return;
		}
	}

	role->SendOperateResult(Protocol::SCOperateResult::OP_SHENGONG_UPSTAR, 1);
}

void MessageHandler::OnShengongGetInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetShengong()->SendShengongInfo();
}

void MessageHandler::OnShengongUplevelEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShengongUplevelEquip *mue = (Protocol::CSShengongUplevelEquip *)msg;
	ShengongFunction::UplevelEquip(role, mue->equip_idx);
}

void MessageHandler::OnShengongSpecialImgUpgrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShengongSpecialImgUpgrade *siu = (Protocol::CSShengongSpecialImgUpgrade*)msg;
	ShengongFunction::ShengongSpecialImgUpgrade(role, siu->special_image_id);
}

void MessageHandler::OnShengongSkillUplevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShengongSkillUplevelReq *sur = (Protocol::CSShengongSkillUplevelReq*)msg;
	ShengongFunction::UplevelSkill(role, sur->skill_idx, 0 != sur->auto_buy);
}

void MessageHandler::OnUnUseShengongImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUnUseShengongImage *um = (Protocol::CSUnUseShengongImage*)msg;
	role->GetShengong()->UnUseImage(um->image_id);
}

void MessageHandler::OnUpgradeShenyi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUpgradeShenyi *um = (Protocol::CSUpgradeShenyi*)msg;
	bool ret = ShenyiFunction::Upgrade(role, 1 == um->auto_buy, um->repeat_times);
	role->SendOperateResult(Protocol::SCOperateResult::OP_SHENYI_UPGRADE, ret);
}

void MessageHandler::OnUseShenyiImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUseShenyiImage *um = (Protocol::CSUseShenyiImage*)msg;
	role->GetShenyi()->UseImage(um->image_id);
}

void MessageHandler::OnShenyiGetInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetShenyi()->SendShenyiInfo();
}

void MessageHandler::OnShenyiUpStarLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShenyiUpStarLevel *cmd = (Protocol::CSShenyiUpStarLevel*)msg;

	int old_grade = role->GetShenyi()->GetShenyiParam()->grade;

	const ShenyiGradeConfig * grade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetGradeConfig(old_grade);
	if (NULL == grade_cfg) return;

	int max_up_star_stuff_times = grade_cfg->pack_num;

	for (int i = 0; i < cmd->loop_times && i < max_up_star_stuff_times; i++)
	{
		bool ret = ShenyiFunction::UpStarLevel(role, cmd);
		if (!ret || old_grade != role->GetShenyi()->GetShenyiParam()->grade)
		{
			role->SendOperateResult(Protocol::SCOperateResult::OP_SHENYI_UPSTAR, 0);
			return;
		}
	}

	role->SendOperateResult(Protocol::SCOperateResult::OP_SHENYI_UPSTAR, 1);
}

void MessageHandler::OnUnUseShenyiImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUnUseShenyiImage *um = (Protocol::CSUnUseShenyiImage*)msg;
	role->GetShenyi()->UnUseImage(um->image_id);
}

void MessageHandler::OnFootprintOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFootprintOperate *um = (Protocol::CSFootprintOperate*)msg;
	role->GetFootPrint()->OperationReq(um);
}

void MessageHandler::OnCloakOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCloakOperate *um = (Protocol::CSCloakOperate*)msg;
	role->GetCloakManager()->OperationReq(um);
}

void MessageHandler::OnXiannvShouhuUseImageReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUseXiannvShouhuImage *um = (Protocol::CSUseXiannvShouhuImage*)msg;
	role->GetXiannvShouhu()->UseImage(um->image_id);
}

void MessageHandler::OnXiannvShouhuGetInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetXiannvShouhu()->SendXiannvShouhuInfo();
}

void MessageHandler::OnXiannvShouhuUpstarReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSXiannvShouhuUpStarLevel *cmd = (Protocol::CSXiannvShouhuUpStarLevel*)msg;

	int old_grade = role->GetXiannvShouhu()->GetXiannvShouhuParam()->grade;
	bool ret = XiannvShouhuFunction::UpStarLevel(role, cmd);

	if (!ret || old_grade != role->GetXiannvShouhu()->GetXiannvShouhuParam()->grade)
	{
		role->SendOperateResult(Protocol::SCOperateResult::OP_XIANNV_HALO_UPGRADE, 0);
	}
	else
	{
		role->SendOperateResult(Protocol::SCOperateResult::OP_XIANNV_HALO_UPGRADE, 1);
	}
}

void MessageHandler::OnJinglingGuanghuanUseImageReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUseJinglingGuanghuanImage *um = (Protocol::CSUseJinglingGuanghuanImage*)msg;
	role->GetJinglingGuanghuan()->UseImage(um->image_id);
}

void MessageHandler::OnJinglingGuanghuanGetInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetJinglingGuanghuan()->SendJinglingGuanghuanInfo();
}

void MessageHandler::OnJinglingGuanghuanUpstarReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSJinglingGuanghuanUpStarLevel *cmd = (Protocol::CSJinglingGuanghuanUpStarLevel*)msg;

	int old_grade = role->GetJinglingGuanghuan()->GetJinglingGuanghuanParam()->grade;
	bool ret = JinglingGuanghuanFunction::UpStarLevel(role, cmd);

	if (!ret || old_grade != role->GetJinglingGuanghuan()->GetJinglingGuanghuanParam()->grade)
	{
		role->SendOperateResult(Protocol::SCOperateResult::OP_JINGLING_HALO_UPSTAR, 0);
	}
	else
	{
		role->SendOperateResult(Protocol::SCOperateResult::OP_JINGLING_HALO_UPSTAR, 1);
	}
}

void MessageHandler::OnJinglingGuanghuanSpecialImgUpgrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSJinglingGuanghuanSpecialImgUpgrade *siu = (Protocol::CSJinglingGuanghuanSpecialImgUpgrade*)msg;
	JinglingGuanghuanFunction::JinglingGuanghuanSpecialImgUpgrade(role, siu->special_image_id);
}

void MessageHandler::OnFairyBuddhaCardActivateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length) 
{
//	Protocol::CSFairyBuddhaCardActivateReq *caq = (Protocol::CSFairyBuddhaCardActivateReq*)msg;

	//role->GetRoleActivity()->OnFairyBuddhaCardActivateReq(caq->card_type);
}

void MessageHandler::OnFairyBuddhaCardGoldBindReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
//	Protocol::CSFairyBuddhaCardGoldBindReq *gbq = (Protocol::CSFairyBuddhaCardGoldBindReq*)msg;

	//role->GetRoleActivity()->OnFairyBuddhaCardGetBindGoldReq(gbq->card_type);
}

void MessageHandler::OnGuildQuestionEnterReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	ActivityShadow::Instance().OnGuildQuestionEnterSceneReq(role);
}

void MessageHandler::OnJinglingFazhenUseImageReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUseJinglingFazhenImage *um = (Protocol::CSUseJinglingFazhenImage*)msg;
	role->GetJinglingFazhen()->UseImage(um->image_id);
}

void MessageHandler::OnJinglingFazhenGetInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetJinglingFazhen()->SendJinglingFazhenInfo();
}

void MessageHandler::OnJinglingFazhenUpstarReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSJinglingFazhenUpStarLevel *cmd = (Protocol::CSJinglingFazhenUpStarLevel*)msg;

	int old_grade = role->GetJinglingFazhen()->GetJinglingFazhenParam()->grade;
	bool ret = JinglingFazhenFunction::UpStarLevel(role, cmd);

	if (!ret || old_grade != role->GetJinglingFazhen()->GetJinglingFazhenParam()->grade)
	{
		role->SendOperateResult(Protocol::SCOperateResult::OP_JINGLING_FAZHEN_UPGRADE, 0);
	}
	else
	{
		role->SendOperateResult(Protocol::SCOperateResult::OP_JINGLING_FAZHEN_UPGRADE, 1);
	}
}

void MessageHandler::OnJinglingFazhenSpecialImgUpgrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSJinglingFazhenSpecialImgUpgrade *siu = (Protocol::CSJinglingFazhenSpecialImgUpgrade*)msg;
	JinglingFazhenFunction::JinglingFazhenSpecialImgUpgrade(role, siu->special_image_id);
}

void MessageHandler::OnShenyiUplevelEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShenyiUplevelEquip *mue = (Protocol::CSShenyiUplevelEquip *)msg;
	ShenyiFunction::UplevelEquip(role, mue->equip_idx);
}

void MessageHandler::OnShenyiSpecialImgUpgrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShenyiSpecialImgUpgrade *siu = (Protocol::CSShenyiSpecialImgUpgrade*)msg;
	ShenyiFunction::ShenyiSpecialImgUpgrade(role, siu->special_image_id);
}

void MessageHandler::OnShenyiSkillUplevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShenyiSkillUplevelReq *sur = (Protocol::CSShenyiSkillUplevelReq*)msg;
	ShenyiFunction::UplevelSkill(role, sur->skill_idx, 0 != sur->auto_buy);
}

void MessageHandler::OnMojieInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetMojie()->SendMojieInfo();
}

void MessageHandler::OnMojieUplevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMojieUplevelReq *gm = (Protocol::CSMojieUplevelReq*)msg;

	role->GetMojie()->OnUpMojielevel(role, gm->mojie_type, 0 != gm->is_auto_buy);
}

void MessageHandler::OnMojieChangeSkillReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMojieChangeSkillReq *gm = (Protocol::CSMojieChangeSkillReq*)msg;

	role->GetSkillManager()->AddMojieSkill(gm->mojie_skill_id, gm->mojie_skill_level, gm->mojie_skill_type);
}

void MessageHandler::OnActivityEnterReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSActivityEnterReq *aer =  (Protocol::CSActivityEnterReq*)msg;
	role->GetRoleActivity()->OnActivityEnterReq(aer);
}

void MessageHandler::OnSendGuildSosReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSendGuildSosReq *sgsr = (Protocol::CSSendGuildSosReq*)msg;
	switch (sgsr->sos_type)
	{
		case GUILD_SOS_TYPE_GUILD_BATTLE:
		{
			role->GetRoleActivity()->OnGuildBattleSosReq(sgsr);
		}
		break;

		case GUILD_SOS_TYPE_GONGCHENGZHAN:
		{
			role->GetRoleActivity()->OnGongChengZhanSosReq(sgsr);
		}
		break;

		case GUILD_SOS_TYPE_CROSS_GUILD_BATTLE:
		{
			if (!CrossConfig::Instance().IsHiddenServer())
			{
				return;
			}

			if (!CrossGuildBattleConfig::Instance().IsCrossGuildBattleScene(role->GetScene()->GetSceneID()))
			{
				return;
			}

			ActivityShadow::Instance().OnCrossGuildBattlSosCost(role);
		}
		break;

		case GUILD_SOS_TYPE_CROSS_BIANJINGZHIDI:
		{
			if (!CrossConfig::Instance().IsHiddenServer())
			{
				return;
			}

			SpecialCrossBianJingZhiDi *special_logic = dynamic_cast<SpecialCrossBianJingZhiDi *>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_CROSS_BIANJINGZHIDI == special_logic->GetSceneType())
			{
				special_logic->SendGuildSos(sgsr, role);
			}
		}
		break;

		case GUILD_SOS_TYPE_CROSS_BOSS:
		case GUILD_SOS_TYPE_CROSS_MIZANG_BOSS:
		case GUILD_SOS_TYPE_CROSS_VIP_BOSS:
		{
			if (!CrossConfig::Instance().IsHiddenServer())
			{
				return;
			}

			ActivityShadow::Instance().OnCrossGuildSOSReq(role, sgsr);
		}
		break;

		default:
		 {
			role->GetRoleActivity()->OnSendGuildSosReq(sgsr->sos_type);
		}
		 break;
	}
}

void MessageHandler::OnReplyGuildSosReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSReplyGuildSosReq *sos_reply = (Protocol::CSReplyGuildSosReq *)msg;
	switch (sos_reply->sos_type)
	{
	case GUILD_SOS_TYPE_GUILD_BATTLE:
	{
		role->GetRoleActivity()->OnReplyGuildBattleSosReq(sos_reply);
	}
	break;

	case GUILD_SOS_TYPE_GONGCHENGZHAN:
	{
		role->GetRoleActivity()->OnReplyGongChengZhanSosReq(sos_reply);
	}
	break;

	case GUILD_SOS_TYPE_CROSS_GUILD_BATTLE:
	{
		if (!CrossConfig::Instance().IsHiddenServer())
		{
			break;;
		}

		if (!CrossGuildBattleConfig::Instance().IsCrossGuildBattleScene(role->GetScene()->GetSceneID()))
		{
			break;
		}

		if (CrossGuildBattleConfig::Instance().IsCrossGuildBattleScene(sos_reply->scene_id))
		{
			World::GetInstWorld()->GetSceneManager()->GoTo(role, sos_reply->scene_id, CROSS_PUBLIC_SCENE_KEY, Posi(sos_reply->pos_x, sos_reply->pos_y));
		}
	}
	break;

	case GUILD_SOS_TYPE_CROSS_BIANJINGZHIDI:
	{
		if (!CrossConfig::Instance().IsHiddenServer())
		{
			return;
		}

		if (!CrossBianJingZhiDiConfig::Instance().IsCrossBianJingZhiDiScene(role->GetScene()->GetSceneID()))
		{
			break;
		}

		SpecialCrossBianJingZhiDi *special_logic = dynamic_cast<SpecialCrossBianJingZhiDi *>(scene->GetSpecialLogic());
		if (nullptr != special_logic && Scene::SCENE_TYPE_CROSS_BIANJINGZHIDI == special_logic->GetSceneType())
		{
			special_logic->ReplyGuildSos(sos_reply, role);
		}
	}
	break;

	default:
		break;
	}
}

void MessageHandler::OnZhuXieFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSZhuXieFetchTaskReward *zxfr = (Protocol::CSZhuXieFetchTaskReward*)msg;
	role->GetRoleActivity()->OnZhuXieFetchReward(zxfr->task_id);
}

void MessageHandler::OnImmediatelyCompleteTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnTuMoTaskOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTuMoTaskOpera *tmo = (Protocol::CSTuMoTaskOpera*)msg;

	switch (tmo->opera_type)
	{
	case Protocol::TUMO_OPERA_TYPE_GET_INFO:
	{
		role->GetDailyTask()->SendTuMoTaskInfo();
	}
	break;

	case Protocol::TUMO_OPERA_TYPE_FETCH_COMPLETE_ALL_REWARD:
	{
		role->GetDailyTask()->FetchCompleteAllTaskReward();
	}
	break;

	case Protocol::TUMO_OPERA_TYPE_COMMIT_TASK:
	{
		role->GetDailyTask()->OnCommitTask((0 != tmo->param_2), tmo->param_1);
	}
	break;

	case Protocol::TUMO_OPERA_TYPE_FETCH_REWARD:
	{
		role->GetDailyTask()->OnFetchTaskReward(tmo->param_1, tmo->param_2);
	}
	break;

	case Protocol::TUMO_OPERA_TYPE_FETCH_EXP_BALL_REWARD:
	{
		role->GetDailyTask()->OnFetchExpBallReward();
	}
	break;

	}
}

void MessageHandler::OnGetPaoHuanTaskInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetPaoHuanTask()->SendPaohuanTaskInfo();
}

void MessageHandler::OnGetFBEquipInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	//Protocol::CSEquipFBGetInfo *efgi = (Protocol::CSEquipFBGetInfo *)msg;
	role->GetFBEquip()->SendFBInfo();
}

void MessageHandler::OnRefreshHoSongTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRefreshHusongTask *rhst = (Protocol::CSRefreshHusongTask*)msg;
	role->GetHusongTask()->OnRefreshTaskColor(rhst);
}

void MessageHandler::OnNeqTakeStarRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSNeqFBStarRewardReq *efb = (Protocol::CSNeqFBStarRewardReq *)msg;

	role->GetFBEquip()->OnNeqTakeStarReward(efb->chapter, efb->seq);
}

void MessageHandler::OnNeqExchangeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnNeqBuyTimesReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetFBEquip()->OnNeqVipBuyTimes();
}

void MessageHandler::OnNeqAutoReq(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSNeqFBAutoReq* efb = (Protocol::CSNeqFBAutoReq *)msg;

	role->GetFBEquip()->OnNeqAutoReq(efb->chapter, efb->level);
}

void MessageHandler::OnNeqInfoReq(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	role->GetFBEquip()->SendNeqFBInfo();
}

void MessageHandler::OnNeqRollReq(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSNeqRollReq* nrr = (Protocol::CSNeqRollReq *)msg;

	if (0 == nrr->end_roll)
	{
		RoleEquipFBManager::Instance().OnNeqRollReq(role);
	}
	else
	{
		RoleEquipFBManager::Instance().CheckRollState(role);
	}
}

void MessageHandler::OnArmorDefendRoleReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSArmorDefendRoleReq * amreq = (Protocol::CSArmorDefendRoleReq *) msg;

	if (Protocol::CS_ARMOR_DEFEND_ROLE_INFO_REQ == amreq->req_type)
	{
		role->GetRoleFbArmorDefend()->SendInfo();
	}
	else if (Protocol::CS_ARMOR_DEFEND_BUY_JOIN_TIMES == amreq->req_type)
	{
		role->GetRoleFbArmorDefend()->PersonalBuyJoinTimes();
	}
	else if (Protocol::CS_ARMOE_DEFEND_NEXT_WAVE_REQ == amreq->req_type)
	{
		SpecialArmorDefend* special_armor_defend = dynamic_cast<SpecialArmorDefend *>(scene->GetSpecialLogic());
		if (special_armor_defend == NULL) return;
		special_armor_defend->OnNextWaveReq(role);
	}
	else if(Protocol::CS_ARMOR_DEFEND_AUTO_REFRESH == amreq->req_type)
	{
		SpecialArmorDefend* special_armor_defend = dynamic_cast<SpecialArmorDefend *>(scene->GetSpecialLogic());
		if (special_armor_defend == NULL) return;
		special_armor_defend->SetRefreshWhenClean(amreq->parm1);
	}
}

void MessageHandler::OnDailyFBGetRoleInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleDailyFb()->SendAllInfo();
}

void MessageHandler::OnFetchExpFbFirstRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnExpFbPayBuff(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSExpFbPayBuff *req = (Protocol::CSExpFbPayBuff*)msg;
	if (Scene::SCENE_TYPE_EXPFB == role->GetScene()->GetSpecialLogic()->GetSceneType())
	{
		SpecialExpFB *special_logic = dynamic_cast<SpecialExpFB*>(role->GetScene()->GetSpecialLogic());
		if (nullptr != special_logic)
			special_logic->OnPayBuffReq(role, static_cast<bool>(req->is_auto));
	}
}

void MessageHandler::OnPataFBGetAllInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRolePataFB()->SendAllInfo();
}

void MessageHandler::OnVipFBGetAllInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleVipFB()->SendAllInfo();
}

void MessageHandler::OnBattleFieldPickUpFlagReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSQunXianLuanDouPickUpShenShi *qxldpuss = (Protocol::CSQunXianLuanDouPickUpShenShi*)msg;
	role->GetRoleActivity()->OnBattleFieldPickUpShenShiReq(qxldpuss->shenshi_objid);
}

void MessageHandler::OnQunxianLuandouFirstRankReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	WorldStatus::Instance().SendQuanxianLuandouSideFirstUser(role);
}

void MessageHandler::OnGuildBattleFetchDailyRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFetchGuildBattleDailyReward *pro = (Protocol::CSFetchGuildBattleDailyReward *)msg;

	int guild_id = role->GetGuildID();
	if (guild_id != INVALID_GUILD_ID)
	{
		Guild *guild = GuildManager::Instance().GetGuild(guild_id);
		if (nullptr != guild)
		{
			GuildMember *member = guild->GetGuildMemberManager().GetMember(IntToUid(role->GetUID()));
			if (nullptr != member)
			{
				switch (pro->operate_type)
				{
				case Protocol::CSFetchGuildBattleDailyReward::TYPE_INFO_REQ:
				{
					member->SendDailyRewardTimes(role);
				}
				break;

				case Protocol::CSFetchGuildBattleDailyReward::TYPE_FETCH_REQ:
				{
					member->OnFetchDailyReward(role);
				}
				break;
				}
			}
		}
	}
}

void MessageHandler::OnGuildInfoStatisticReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGuildInfoStatisticReq* sta_req = (Protocol::CSGuildInfoStatisticReq*)msg;
	
	switch (sta_req->activity_type)
	{
	case  ACTIVITY_TYPE_GONGCHENGZHAN:
	{
		if (NULL != scene)
		{
			SpecialGongChengZhan * gcz_special = dynamic_cast<SpecialGongChengZhan*>(scene->GetSpecialLogic());
			if (gcz_special != NULL)
			{
				gcz_special->SendGuildInfoStatistic(role->GetGuildID(), Protocol::SCGuildInfoStatistic::NOTIFY_ACTIVITY_REQ, role);
			}
		}
	}
	break;
	case ACTIVITY_TYPE_GUILDBATTLE:
	{
		if (NULL != scene)
		{
			SpecialGuildBattleNew * gb_special = dynamic_cast<SpecialGuildBattleNew*>(scene->GetSpecialLogic());
			if (gb_special != NULL)
			{
				gb_special->SendGuildInfoStatistic(role->GetGuildID(), Protocol::SCGuildInfoStatistic::NOTIFY_ACTIVITY_REQ, role);
			}
		}
	}
	break;

	case CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE:
	{
		ActivityShadow::Instance().SendCrossGuildBattleStatistic(INVALID_UNIQUE_USER_ID, Protocol::SCGuildInfoStatistic::NOTIFY_ACTIVITY_REQ, role);
	}
	break;

	default:
		break;
	}
}

void MessageHandler::OnGczChangePlace(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGCZChangePlace *gczcppuss = (Protocol::CSGCZChangePlace*)msg;
	role->GetRoleActivity()->OnBattleFieldPickUpShenShiReq(gczcppuss->place_type);

	if (Scene::SCENE_TYPE_GONGCHENGZHAN == role->GetScene()->GetSpecialLogic()->GetSceneType())
	{
		SpecialGongChengZhan *special_logic = dynamic_cast<SpecialGongChengZhan*>(role->GetScene()->GetSpecialLogic());
		if (nullptr != special_logic)
			special_logic->OnGczChangePlace(role, gczcppuss->place_type);
	}
}

void MessageHandler::OnGongChengZhanFetchDailyRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->OnCSGCZFetchDailyRewardReq();
}

void MessageHandler::OnTeamFbRoomOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTeamFbRoomOperate *cmd = (Protocol::CSTeamFbRoomOperate *)msg;
	TeamFbRoom::Instance().ReqOperate(role, cmd);
}

void MessageHandler::OnGetTowerDefendRoleInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	//role->GetFBTowerDefend()->SendInfo();
}

void MessageHandler::OnGuildMonsterQuery(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->SendGuildMonsterInfo();
}

void MessageHandler::OnFightingMiningReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFightingReq *fmr = (Protocol::CSFightingReq*)msg;
	RoleFighting *m_little_pet = GLOBAL_ROLE_MGR.GetRoleFighting(role->GetUID());
	if (NULL == m_little_pet) return;

	m_little_pet->OnHandleReq(fmr->req_type, fmr->param1, fmr->param2);
}

void MessageHandler::OnPhaseFBInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSPhaseFBInfoReq * pff = (Protocol::CSPhaseFBInfoReq*)msg;
	switch (pff->operate_type)
	{
	case Protocol::PHASE_FB_OPERATE_TYPE_INFO:
	{
		role->GetRoleFBPhase()->SendInfo();
		break;
	}
	case Protocol::PHASE_FB_OPERATE_TYPE_BUY_TIMES:
	{
		role->GetRoleFBPhase()->BuyFBTimes(pff->fb_index);
		break;
	}
	default:
		break;
	}
}

void MessageHandler::OnEnterBossFamilyReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSEnterBossFamily *ebf = (Protocol::CSEnterBossFamily *)msg;

	if (role->GetRoleStatusManager()->CanTransmit())
	{
		BossFamilyManager::Instance().OnEnterBossFamily(role, ebf);
	}
}

void MessageHandler::OnBossFamilyOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSBossFamilyOperate *ebf = (Protocol::CSBossFamilyOperate *)msg;

	BossFamilyManager::Instance().OnBossFamilyOperate(role, ebf->operate_type);
}

void MessageHandler::OnGetBossInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGetBossInfoReq *gbi = (Protocol::CSGetBossInfoReq *)msg;
	BossFamilyManager::Instance().OnGetBossInfoReq(role, gbi->enter_type);
}

void MessageHandler::OnBossKillerInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSBossKillerInfoReq *gbi = (Protocol::CSBossKillerInfoReq *)msg;
	BossFamilyManager::Instance().OnBossKillerInfoReq(role, gbi->boss_type, gbi->boss_id, gbi->scene_id);
}

void MessageHandler::OnGetMonsterGenerateInfo(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSReqMonsterGeneraterList *rmgl = (Protocol::CSReqMonsterGeneraterList *)msg;

	Scene *get_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(rmgl->get_scene_id, 0);

	if (NULL != get_scene)
	{
		get_scene->GetMonsterGeneraterInfo(role);
	}
}

void MessageHandler::OnGetGatherGenerateInfo(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSReqGatherGeneraterList *rmgl = (Protocol::CSReqGatherGeneraterList *)msg;
	Scene *get_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(rmgl->get_scene_id, rmgl->scene_key);
	if (NULL != get_scene)
	{
		get_scene->GetGatherGeneraterInfo(role);
	}
}

void MessageHandler::OnFollowBossOperReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFollowBossReq *fbr = (Protocol::CSFollowBossReq *)msg;
	// 上古boss
	if (fbr->boss_type == Protocol::BOSS_ENTER_TYPE_BOSS_SHANGGU)
	{
		BossShangGuManager::Instance().OnShangGuBossReq(role, Protocol::SHANGGU_BOSS_OPERA_TYPE_SET_CONCERN, fbr->boss_id, fbr->opera_type);
		return;
	}

	// boss之家boss
	switch(fbr->opera_type)
	{
	case Protocol::CSFollowBossReq::FOLLOW_BOSS:
		{
			BossFamilyManager::Instance().OnFollowBossReq(role, fbr->boss_type, fbr->boss_id, fbr->scene_id);
		}
		break;

	case Protocol::CSFollowBossReq::UNFOLLOW_BOSS:
		{
			BossFamilyManager::Instance().OnUnFollowBossReq(role, fbr->boss_type, fbr->boss_id, fbr->scene_id);
		}
		break;

	case Protocol::CSFollowBossReq::GET_FOLLOW_LIST:
		{
			role->GetFollowBossManager()->SendFollowBossList();
		}
		break;
	}
}

void MessageHandler::OnPreciousPosReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSPreciousOperaReq *pro = (Protocol::CSPreciousOperaReq *)msg;
	switch (pro->type)
	{
	case Protocol::CSPreciousOperaReq::PRECIOUS_OPERA_TYPE_POS:
		BossFamilyManager::Instance().SendPreciousBossPosInfo(role, pro->param, pro->param_2);
		break;

	case Protocol::CSPreciousOperaReq::PRECIOUS_OPERA_TYPE_TASK:
		BossFamilyManager::Instance().SendPreciousBossTaskInfo(role);
		break;
		
	default:
		break;
	}
}

void MessageHandler::OnYunyouBossOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSYunyouBossOperate *ybo = (Protocol::CSYunyouBossOperate *)msg;

	BossFamilyManager::Instance().OnYunyouBossOperate(role, ybo->operate_type, ybo->param_0);
}

void MessageHandler::OnGetYaoShouGuangChangState(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	YaoShouGuangChangMgr::Instance().OnGetYSGCState(role);
}

void MessageHandler::OnGetYaoShouGuangChangReward(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSGetYaoShouGuangChangReward *gysgcr = (Protocol::CSGetYaoShouGuangChangReward *)msg;
	YaoShouGuangChangMgr::Instance().OnGetYSGCReward(role, gysgcr->times);
}

void MessageHandler::OnEnterYaoShouGuangChang(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSEnterYaoShouGuangChang *gysgcr = (Protocol::CSEnterYaoShouGuangChang *)msg;
	YaoShouGuangChangMgr::Instance().OnJoinActivity(role, 0 != gysgcr->is_buy_times);
}

void MessageHandler::OnGetSuoYaoTaState(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	SuoYaoTaMgr::Instance().OnGetSYTState(role);
}

void MessageHandler::OnGetSuoYaoTaReward(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSGetSuoYaoTaReward *gsytr = (Protocol::CSGetSuoYaoTaReward *)msg;
	SuoYaoTaMgr::Instance().OnGetSYTReward(role, gsytr->times);
}

void MessageHandler::OnEnterSuoYaoTa(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSEnterSuoYaoTa *gysgcr = (Protocol::CSEnterSuoYaoTa *)msg;
	SuoYaoTaMgr::Instance().OnJoinActivity(role, 0 != gysgcr->is_buy_times);
}

void MessageHandler::OnGetGuildFBGuardPos(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	if (Scene::SCENE_TYPE_GUILD_FB == scene->GetSpecialLogic()->GetSceneType())
	{
		SpecialGuildFb *special_logic = dynamic_cast<SpecialGuildFb*>(scene->GetSpecialLogic());

		Posi guard_pos;
		if (nullptr != special_logic && special_logic->GetGuardBossPos(&guard_pos))
		{
			static Protocol::SCGuildFBGuardPos gfgp;
			gfgp.scene_id = scene->GetSceneID();
			gfgp.pos_x = guard_pos.x;
			gfgp.pos_y = guard_pos.y;
			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&gfgp, sizeof(gfgp));
		}
	}
}

void MessageHandler::OnTeamEquipFbBuyDropCount(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	FBManager::Instance().TeamEquipFbBuyDropTimes(role);
}

void MessageHandler::OnTowerDefendBuyJoinTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetFBTowerDefend()->PersonalBuyJoinTimes();
}

void MessageHandler::OnTowerDefendNextWave(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	if (Scene::SCENE_TYPE_PERSONAL_TOWERDEFEND == scene->GetSpecialLogic()->GetSceneType())
	{
		SpecialTowerDefend* special_tower_defend = dynamic_cast<SpecialTowerDefend*>(scene->GetSpecialLogic());
		if (nullptr != special_tower_defend)
			special_tower_defend->OnNextWaveReq(role);
	}
}

void MessageHandler::OnMiGongXianFuTouchDoor(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMgxfTeamFbTouchDoor *cmd = (Protocol::CSMgxfTeamFbTouchDoor *)msg;

	if (Scene::SCENE_TYPE_MIGONGXIANFU_TEAM == scene->GetSpecialLogic()->GetSceneType())
	{
		SpecialMiGongXianFuTeamFb *special_logic = dynamic_cast<SpecialMiGongXianFuTeamFb*>(scene->GetSpecialLogic());
		if (nullptr != special_logic)
			special_logic->OnTouchDoor(role, cmd->layer, cmd->door_index);
	}
}

void MessageHandler::OnStoryFBGetInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetFBStory()->SendInfo();
}

void MessageHandler::OnFbGuwu(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFbGuwu *cmd = (Protocol::CSFbGuwu *)msg;
	role->GetScene()->GetSpecialLogic()->OnSpecialGuWu(role, cmd->is_gold == 1);
}

void MessageHandler::OnFunOpenStoryStep(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFunOpenStoryStep *cmd = (Protocol::CSFunOpenStoryStep *)msg;

	Scene * role_scene = role->GetScene();
	if (NULL == role_scene)
	{
		return;
	}
	int scene_type = role_scene->GetSceneType();

	switch(scene_type)
	{
	case Scene::SCENE_TYPE_FUN_TYPE_MOUNT:
		{
			if (NULL != role_scene)
			{
				SpecialFunOpenMountFb *mount_fb_logical = dynamic_cast<SpecialFunOpenMountFb*>(role_scene->GetSpecialLogic());
				if (NULL != mount_fb_logical)
				{
					mount_fb_logical->RefreshStepMonsterClienReq(role, cmd);
				}
			}
		}
		break;

	case Scene::SCENE_TYPE_FUN_TYPE_WING:
		{
			if (NULL != role_scene)
			{
				SpecialFunOpenWingFb *wing_fb_logical = dynamic_cast<SpecialFunOpenWingFb*>(role_scene->GetSpecialLogic());
				if (NULL != wing_fb_logical)
				{
					wing_fb_logical->RefreshStepMonsterClienReq(role, cmd);
				}
			}
		}
		break;

	case Scene::SCENE_TYPE_FUN_TYPE_XIANNV:
		{
			if (NULL != role_scene)
			{
				SpecialFunOpenXiannvFb * xiannv_fb_logical = dynamic_cast<SpecialFunOpenXiannvFb*>(role_scene->GetSpecialLogic());
				if (NULL != xiannv_fb_logical)
				{
					xiannv_fb_logical->RefreshStepMonsterClienReq(role, cmd);
				}
			}
		}
		break;
	}
}

void MessageHandler::OnFbGuideFinish(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	if (NULL == role) return;
	Protocol::CSFbGuideFinish *pro = (Protocol::CSFbGuideFinish *)msg;
	if (Scene::SCENE_TYPE_FB_GUIDE == role->GetScene()->GetSpecialLogic()->GetSceneType())
	{
		SpecialFbGuide *special_logic = dynamic_cast<SpecialFbGuide*>(role->GetScene()->GetSpecialLogic());
		if (NULL != special_logic)
		{
			special_logic->OnFinish(role, pro->monster_x, pro->monster_y, pro->monster_id);
		}
	}
}

void MessageHandler::OnFbGuideCreateGather(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	if (NULL == role) return;
	Protocol::CSFbGuideCreateGather *pro = (Protocol::CSFbGuideCreateGather *)msg;

	Scene *role_scene = role->GetScene();
	if (NULL == role_scene)
	{
		return;
	}

	int scene_type = role_scene->GetSceneType();

	switch(scene_type)
	{
	case Scene::SCENE_TYPE_FB_GUIDE:
		{
			SpecialFbGuide *special_logic = dynamic_cast<SpecialFbGuide*>(role->GetScene()->GetSpecialLogic());
			if (NULL != special_logic)
			{
				special_logic->OnCreateGather(role, pro->gather_x, pro->gather_y, pro->gather_id, pro->gather_time);
			}
		}
		break;

	case Scene::SCENE_TYPE_FUN_TYPE_MOUNT:
		{
			SpecialFunOpenMountFb *special_logic = dynamic_cast<SpecialFunOpenMountFb*>(role->GetScene()->GetSpecialLogic());
			if (nullptr != special_logic)
			{
				special_logic->OnCreateGather(role, pro->gather_x, pro->gather_y, pro->gather_id, pro->gather_time);
			}
		}
		break;

	case Scene::SCENE_TYPE_FUN_TYPE_WING:
		{
			SpecialFunOpenWingFb *special_logic = dynamic_cast<SpecialFunOpenWingFb*>(role->GetScene()->GetSpecialLogic());
			if (nullptr != special_logic)
			{
				special_logic->OnCreateGather(role, pro->gather_x, pro->gather_y, pro->gather_id, pro->gather_time);
			}
		}
		break;

	case Scene::SCENE_TYPE_FUN_TYPE_XIANNV:
		{
			SpecialFunOpenXiannvFb *special_logic = dynamic_cast<SpecialFunOpenXiannvFb*>(role->GetScene()->GetSpecialLogic());
			if (nullptr != special_logic)
			{
				special_logic->OnCreateGather(role, pro->gather_x, pro->gather_y, pro->gather_id, pro->gather_time);
			}
		}
		break;

	default:
		break;
	}
}

void MessageHandler::OnFunOpenSetObjPos(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFunOpenSetObjToPos *cmd = (Protocol::CSFunOpenSetObjToPos *)msg;

	Scene *role_scene = role->GetScene();
	if (NULL == role_scene)
	{
		return;
	}
	int scene_type = role_scene->GetSceneType();

	switch(scene_type)
	{
	case Scene::SCENE_TYPE_FB_GUIDE:
		{
			if (NULL != role_scene)
			{
				SpecialFbGuide *guide_fb_logical = dynamic_cast<SpecialFbGuide*>(role_scene->GetSpecialLogic());
				if (NULL != guide_fb_logical)
				{
					guide_fb_logical->SetObjPos(role, cmd);
				}
			}
		}
		break;

	case Scene::SCENE_TYPE_FUN_TYPE_MOUNT:
		{
			if (NULL != role_scene)
			{
				SpecialFunOpenMountFb *mount_fb_logical = dynamic_cast<SpecialFunOpenMountFb*>(role_scene->GetSpecialLogic());
				if (NULL != mount_fb_logical)
				{
					mount_fb_logical->SetObjPos(role, cmd);
				}
			}
		}
		break;

	case Scene::SCENE_TYPE_FUN_TYPE_WING:
		{
			if (NULL != role_scene)
			{
				SpecialFunOpenWingFb *wing_fb_logical = dynamic_cast<SpecialFunOpenWingFb*>(role_scene->GetSpecialLogic());
				if (NULL != wing_fb_logical)
				{
					wing_fb_logical->SetObjPos(role, cmd);
				}
			}
		}
		break;

	case Scene::SCENE_TYPE_FUN_TYPE_XIANNV:
		{
			if (NULL != role_scene)
			{
				SpecialFunOpenXiannvFb *xiannv_fb_logical = dynamic_cast<SpecialFunOpenXiannvFb*>(role_scene->GetSpecialLogic());
				if (NULL != xiannv_fb_logical)
				{
					xiannv_fb_logical->SetObjPos(role, cmd);
				}
			}
		}
		break;
	}
}

void MessageHandler::OnChallengeFBOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSChallengeFBOP *pro = (Protocol::CSChallengeFBOP *)msg;
	role->GetFBChallenge()->OnOperateReq(pro->type, pro->param);
}

void MessageHandler::OnBattleFieldHonorInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->OnGetBattleFieldHonorReq();
}

void MessageHandler::OnQueryActivityRomeStatusReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSActivityRoomStatusReq *arsr = (Protocol::CSActivityRoomStatusReq*)msg;
	ActivityManager::Instance().OnQueryRoomStatusReq(role, arsr->activity_type);
}

void MessageHandler::OnEnterYaoshouzhiluanReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnShiZongCaiFengGetTaskList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnShiZongCaiFengAcceptTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnGuildBattleAddHuDunReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
// 	if (Scene::SCENE_TYPE_GUILDBATTLE == role->GetScene()->GetSpecialLogic()->GetSceneType())
// 	{
// 		SpecialGuildBattleNew *special_logic = dynamic_cast<SpecialGuildBattleNew*>(role->GetScene()->GetSpecialLogic());
// 		if (NULL != special_logic)
// 		{
// 			special_logic->OnAddHuDun(role);
// 		}
// 	}
}

void MessageHandler::OnGuildBattleSubmitGoodsReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
// 	if (Scene::SCENE_TYPE_GUILDBATTLE == role->GetScene()->GetSpecialLogic()->GetSceneType())
// 	{
// 		SpecialGuildBattleNew *special_logic = dynamic_cast<SpecialGuildBattleNew*>(role->GetScene()->GetSpecialLogic());
// 		if (NULL != special_logic)
// 		{
// 			special_logic->CalcSubmitGoodsScore(role);
// 		}
// 	}
}

void MessageHandler::OnGuildBattleWinerInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	GuildID win_guild = 0;
	GuildRank *guild_rank = RankManager::Instance().GetGuildRank();
	if (NULL != guild_rank)
	{
		guild_rank->GetRankTopGuild(GUILD_RANK_TYPE_GUILDBATTLE, 1, &win_guild);
	}

	if (0 != win_guild)
	{
		Protocol::SCGBSendWinnerInfo msg;
		msg.guild_id = win_guild;

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&msg, sizeof(msg));
	}
}

void MessageHandler::OnMillionaireUseSkillReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMillionaireUseFrozenSkill *arsr = (Protocol::CSMillionaireUseFrozenSkill*)msg;

	if (Scene::SCENE_TYPE_MILLIONAIRE == scene->GetSpecialLogic()->GetSceneType())
	{
		SpecialMillionaire *special_logic = dynamic_cast<SpecialMillionaire*>(role->GetScene()->GetSpecialLogic());
		if (NULL != special_logic)
		{
			special_logic->OnUseFrozenSkill(role, arsr->target_obj_id);
		}
	}
}

void MessageHandler::OnTerritoryWarReliveBuyReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTerritoryWarReliveShopBuy* cmd = (Protocol::CSTerritoryWarReliveShopBuy*)msg;

	if (Scene::SCENE_TYPE_TERRITORY_WAR == role->GetScene()->GetSpecialLogic()->GetSceneType())
	{
		SpecialTerritoryWar *special_logic = dynamic_cast<SpecialTerritoryWar*>(role->GetScene()->GetSpecialLogic());
		if (NULL != special_logic)
		{
			special_logic->OnReliveShopBuyReq(role, cmd->goods_id);
		}
	}
}

void MessageHandler::OnTerritoryWarFightBuyReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTerritoryWarReliveFightBuy* cmd = (Protocol::CSTerritoryWarReliveFightBuy*)msg;

	if (Scene::SCENE_TYPE_TERRITORY_WAR == role->GetScene()->GetSpecialLogic()->GetSceneType())
	{
		SpecialTerritoryWar *special_logic = dynamic_cast<SpecialTerritoryWar*>(role->GetScene()->GetSpecialLogic());
		if (NULL != special_logic)
		{
			special_logic->OnFightShopBuyReq(role, cmd->type, cmd->goods_id);
		}
	}
}

void MessageHandler::OnTerritoryQualificationReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::SCTerritoryWarQualification cmd;

	GuildRank *guild_rank = RankManager::Instance().GetGuildRank();
	if (NULL != guild_rank)
	{
		memset(cmd.m_guildbattel_rank_list, 0, sizeof(cmd.m_guildbattel_rank_list));
		memset(cmd.m_territorywar_rank_list, 0, sizeof(cmd.m_territorywar_rank_list));

		guild_rank->GetRankTopGuild(GUILD_RANK_TYPE_GUILDBATTLE, Protocol::CAN_JOIN_MAX_RANK, cmd.m_guildbattel_rank_list);
		guild_rank->GetRankTopGuild(GUILD_RANK_TYPE_TERRITORYWAR,Protocol::CAN_JOIN_MAX_RANK, cmd.m_territorywar_rank_list);

		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char*)&cmd, sizeof(cmd));
	}
}

void MessageHandler::OnTerritorySetLandMineReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTerritorySetLandMine* cmd = (Protocol::CSTerritorySetLandMine*)msg;

	if (Scene::SCENE_TYPE_TERRITORY_WAR == role->GetScene()->GetSpecialLogic()->GetSceneType())
	{
		SpecialTerritoryWar *special_logic = dynamic_cast<SpecialTerritoryWar*>(role->GetScene()->GetSpecialLogic());
		if (NULL != special_logic)
		{
			special_logic->OnUseLandMine(role, cmd->landmine_type, Posi(cmd->pos_x, cmd->pos_y));
		}
	}
}

void MessageHandler::OnHotspringGivePresentReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSHotspringGivePresent* cmd = (Protocol::CSHotspringGivePresent*)msg;

	if (Scene::SCENE_TYPE_HOTSPRING == role->GetScene()->GetSpecialLogic()->GetSceneType())
	{
		SpecialCrossHotspring *special_logic = dynamic_cast<SpecialCrossHotspring*>(role->GetScene()->GetSpecialLogic());
		if (NULL != special_logic)
		{
			special_logic->OnGivePresentReq(role,cmd);
		}
	}
}

void MessageHandler::OnHotspringAddPartnerReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSHSAddPartnerReq* cmd = (Protocol::CSHSAddPartnerReq*)msg;

	if (Scene::SCENE_TYPE_HOTSPRING == role->GetScene()->GetSpecialLogic()->GetSceneType())
	{
		SpecialCrossHotspring *special_logic = dynamic_cast<SpecialCrossHotspring*>(role->GetScene()->GetSpecialLogic());
		if(NULL != special_logic)
		{
			special_logic->OnAddPartner(role, cmd);
		}
	}
}

void MessageHandler::OnHotspringDeletePartnerReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	if (Scene::SCENE_TYPE_HOTSPRING == role->GetScene()->GetSpecialLogic()->GetSceneType())
	{
		SpecialCrossHotspring *special_logic = dynamic_cast<SpecialCrossHotspring*>(role->GetScene()->GetSpecialLogic());
		if (NULL != special_logic)
		{
			special_logic->OnDeletePartnerReq(role);
		}
	}
}

void MessageHandler::OnHotspringQAFirstPostReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	if (Scene::SCENE_TYPE_HOTSPRING == role->GetScene()->GetSpecialLogic()->GetSceneType())
	{
		SpecialCrossHotspring *special_logic = dynamic_cast<SpecialCrossHotspring*>(role->GetScene()->GetSpecialLogic());
		if (NULL != special_logic)
		{
			special_logic->QAFllowRankFirst(role);
		}	
	}
}

void MessageHandler::OnHotspringQAAnswerReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSHSQAAnswerReq* cmd = (Protocol::CSHSQAAnswerReq*)msg;

	if (Scene::SCENE_TYPE_HOTSPRING == role->GetScene()->GetSpecialLogic()->GetSceneType())
	{
		SpecialCrossHotspring *special_logic = dynamic_cast<SpecialCrossHotspring*>(role->GetScene()->GetSpecialLogic());
		if (NULL != special_logic)
		{
			special_logic->QARoleAnswerReq(role, cmd->choose, cmd->is_use_item != 0);
		}	
	}
}

void MessageHandler::OnHotspringQAUseCardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSHSQAUseCardReq* cmd = (Protocol::CSHSQAUseCardReq*)msg;

	Role *target = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(cmd->target_uid));
	if (NULL == target)
	{
		role->NoticeNum(errornum::EN_ROLE_NOT_ONLINE);
		return;
	}

	if (Scene::SCENE_TYPE_HOTSPRING == role->GetScene()->GetSpecialLogic()->GetSceneType())
	{
		SpecialCrossHotspring *special_logic = dynamic_cast<SpecialCrossHotspring*>(role->GetScene()->GetSpecialLogic());
		if (NULL != special_logic)
		{
			special_logic->OnUseBianshenCard(role, target);
		}
	}
}

void MessageHandler::OnHotspringUseSkillReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSHSUseSkillReq* cmd = (Protocol::CSHSUseSkillReq*)msg;

	if (Scene::SCENE_TYPE_HOTSPRING == scene->GetSpecialLogic()->GetSceneType())
	{
		SpecialCrossHotspring *special_logic = dynamic_cast<SpecialCrossHotspring*>(role->GetScene()->GetSpecialLogic());
		if (NULL != special_logic)
		{
			special_logic->OnUseSkillReq(role, cmd->obj_id, cmd->skill_type);
		}
	}
}

void MessageHandler::OnCrossGuildBattleOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCrossGuildBattleOperate *cmd = (Protocol::CSCrossGuildBattleOperate *)msg;
	if (Protocol::CROSS_GUILDBATTLE_OPERATE_REQ_INFO == cmd->operate_type)
	{
		role->GetRoleCross()->SyncCrossGuildBattleInfo();
	}
	else if (Protocol::CROSS_GUILDBATTLE_OPERATE_FETCH_REWARD == cmd->operate_type)
	{
		role->GetRoleCross()->OnFetchCrossGuildBattleReward(cmd->param1, cmd->param2);
	}
	else if (Protocol::CROSS_GUILDBATTLE_OPERATE_REQ_TASK_INFO == cmd->operate_type)
	{
		role->GetRoleCross()->SendGuildBattleTaskInfo();
	}
	else if(Protocol::CROSS_GUILDBATTLE_OPERATE_BOSS_INFO == cmd->operate_type)
	{
		static crossgameprotocal::CSCrossCommonOperaReq gcscr;
		gcscr.req_type = crossgameprotocal::CROSS_COMMON_OPERA_REQ_CROSS_GUILDBATTLE_BOSS_INFO;
		gcscr.param_1 = cmd->param1;
		gcscr.param_2 = 0;
		gcscr.param_3 = UniqueUserIDToLongLong(role->GetUniqueUserID());
		InternalComm::Instance().SendToCross((const char *)&gcscr, sizeof(gcscr));
	}
	else if (Protocol::CROSS_GUILDBATTLE_OPERATE_SCENE_RANK_INFO == cmd->operate_type)
	{
		if (!CrossConfig::Instance().IsHiddenServer())
		{
			return;
		}

		Scene *battle_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(cmd->param1, CROSS_PUBLIC_SCENE_KEY);
		if (NULL == battle_scene) return;

		if (Scene::SCENE_TYPE_CROSS_GUILDBATTLE == battle_scene->GetSceneType())
		{
			SpecialCrossGuildBattle *battle_logic = dynamic_cast<SpecialCrossGuildBattle*>(battle_scene->GetSpecialLogic());
			if (NULL != battle_logic)
			{
				battle_logic->SyncSceneInfo(role);
			}
		}
	}
	else if (Protocol::CROSS_GUILDBATTLE_OPERATE_GOTO_SCENE == cmd->operate_type)
	{
		if (!CrossConfig::Instance().IsHiddenServer())
		{
			return;
		}

		if (!CrossGuildBattleConfig::Instance().IsCrossGuildBattleScene(role->GetScene()->GetSceneID()))
		{
			return;
		}

		Posi enter_pos(0, 0);
		int scene_id = CrossGuildBattleConfig::Instance().GetRandEnterPosByIndex(cmd->param1, &enter_pos);
		if (scene_id != 0 && enter_pos != Posi(0, 0))
		{
			World::GetInstWorld()->GetSceneManager()->GoTo(role, scene_id, CROSS_PUBLIC_SCENE_KEY, enter_pos);
			return;
		}
	}
	else if(Protocol::CROSS_GUILDBATTLE_OPERATE_GET_DAILY_REWARD == cmd->operate_type)
	{
		role->GetRoleCross()->OnGetCrossGuildBattleDailyReward(cmd->param1);
	}
}

void MessageHandler::OnCrossGuildBattleGetRankInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	// Protocol::CSCrossGuildBattleGetRankInfoReq *gbgrir = (Protocol::CSCrossGuildBattleGetRankInfoReq *)msg;
	role->GetRoleCross()->ReqCrossGuildBattleGetRankLog();
}

void MessageHandler::OnQuestionAnswerReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSQuestionAnswerReq *cmd = (Protocol::CSQuestionAnswerReq*)msg;
	
	if (QUESTION_TYPE_WORLD == cmd->answer_type)
	{
		WorldStatus::Instance().OnAnswerWorldQuestion(role, cmd->choose);
	}
	else if(QUESTION_TYPE_GUILD == cmd->answer_type)
	{
		Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
		if (NULL == guild)
		{
			role->NoticeNum(errornum::EN_GUILD_YOU_NOT_IN_GUILD);
			return;
		}

		guild->OnAnswerGuildQuestion(role, cmd->choose);
	}
}

void MessageHandler::OnHotspringAddPartnerRet(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSHSAddPartnerRet* cmd = (Protocol::CSHSAddPartnerRet*)msg;

	if (Scene::SCENE_TYPE_HOTSPRING == role->GetScene()->GetSpecialLogic()->GetSceneType())
	{
		SpecialCrossHotspring *special_logic = dynamic_cast<SpecialCrossHotspring*>(role->GetScene()->GetSpecialLogic());
		if (NULL != special_logic)
		{
			special_logic->OnAddPartnerRet(role,cmd);
		}
	}

}

void MessageHandler::OnXianMengZhanGuildCall(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSXianMengZhanGuildCall* xmzgc = (Protocol::CSXianMengZhanGuildCall*)msg;
	role->GetRoleActivity()->XianMengZhanGuildCall(0 != xmzgc->use_gold);
}

void MessageHandler::OnXianMengZhanGuildFollow(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->XianMengZhanGuildFollow();
}

void MessageHandler::OnHusongBuyTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetHusongTask()->OnBuyTimes();
}

void MessageHandler::OnHusongAddShiled(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetHusongTask()->OnAddShield();
}

void MessageHandler::OnRoleTaskTransmit(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRoleTaskTransmit *rtt = (Protocol::CSRoleTaskTransmit *)msg; 

	if (NULL == scene) return;

	if (role->GetHusongTask()->IsHusongStatus())
	{
		return;
	}

	const int ROLE_TASK_TRANSMIT_TASK_ID = GLOBALCONFIG->GetRoleTaskTransmitTaskId();
	bool trial_task_iscomplete = true;
	if (role->GetTaskManager()->HasTask(ROLE_TASK_TRANSMIT_TASK_ID, &trial_task_iscomplete))
	{
		if (scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, rtt->pos_x, rtt->pos_y))
		{
			role->GetTaskManager()->CommitTask(ROLE_TASK_TRANSMIT_TASK_ID);

			Posi pos(rtt->pos_x, rtt->pos_y);
			World::GetInstWorld()->GetSceneManager()->GoTo(role, scene->GetSceneID(), -1, pos);
		}
	}
}

void MessageHandler::OnFinishAllGuildTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetGuildTask()->OneKeyCompleteTask();
}

void MessageHandler::OnGetGuildDoubleReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	// role->GetGuildTask()->GetGuildDoubleReward();
}

void MessageHandler::OnForceFinishSpecialTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSForceFinishSpecialTask *pro = (Protocol::CSForceFinishSpecialTask *)msg;

	role->GetTaskManager()->OnPassSpecialTask(pro->special_task_type);
}

void MessageHandler::OnOpenTaskPanel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	/*Protocol::CSTaskOpenPanel *top = (Protocol::CSTaskOpenPanel *)msg;

	if (Protocol::CSTaskOpenPanel::OPEN_TYPE_PET_SKILL_PANEL == top->open_type)
	{
	role->GetTaskManager()->OnOperate(TaskOperate::OT_OPEN_PET_SKILL_PANEL);
	}
	else if (Protocol::CSTaskOpenPanel::OPEN_TYPE_CAPABILITY_PANEL == top->open_type)
	{
	role->GetTaskManager()->OnOperate(TaskOperate::OT_OPEN_CAPABILITY_PANEL);
	}*/
}

void MessageHandler::OnFetchTouZiJiHuaReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFetchTouZiJiHuaReward *ftzjhr = (Protocol::CSFetchTouZiJiHuaReward *)msg;
	role->GetTouzijihua()->OnFetchReward(ftzjhr->plan_type, ftzjhr->seq);
}

void MessageHandler::OnTouzijihuaActive(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTouzijihuaActive *tzjta = (Protocol::CSTouzijihuaActive *)msg;
	role->GetTouzijihua()->OnBuyPlan(tzjta->plan_type);
}

void MessageHandler::OnNewTouzijihuaOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSNewTouzijihuaOperate *pro = (Protocol::CSNewTouzijihuaOperate *)msg;
	if (Protocol::NEW_TOUZIJIHUA_OPERATE_BUY == pro->operate_type)
	{
		role->GetTouzijihua()->OnNewBuy();
	}
	else if (Protocol::NEW_TOUZIJIHUA_OPERATE_FETCH == pro->operate_type)
	{
		role->GetTouzijihua()->OnNewFetchReward(false, pro->param);
	}
	else if (Protocol::NEW_TOUZIJIHUA_OPERATE_FIRST == pro->operate_type)
	{
		role->GetTouzijihua()->OnNewFetchFirstReward();
	}
	else if (Protocol::NEW_TOUZIJIHUA_OPERATE_FOUNDATION_FETCH == pro->operate_type)
	{
		role->GetTouzijihua()->OnFetchFoundationReward(pro->param);
	}

	// 不区分vip奖励了，以前的vip额外奖励去掉，需要再开回来
	// 	else if (Protocol::NEW_TOUZIJIHUA_OPERATE_VIP_FETCH == pro->operate_type)
	// 	{
	// 		role->GetTouzijihua()->OnNewFetchReward(true, pro->param);
	// 	}
}

void MessageHandler::OnTouzijihuaFbBossOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTouzijihuaFbBossOperate * op = (Protocol::CSTouzijihuaFbBossOperate *)msg;

	switch (op->operate_type)
	{
	case Protocol::TOUZIJIHUA_FB_BOSS_OPERATE_FB_BUY:
		{
			role->GetTouzijihua()->OnBuyFbPlan();
		}
		break;

	case Protocol::TOUZIJIHUA_FB_BOSS_OPERATE_FB_REWARD:
		{
			role->GetTouzijihua()->OnFetchFbPlanReward(op->param);
		}
		break;

	case Protocol::TOUZIJIHUA_FB_BOSS_OPERATE_BOSS_BUY:
		{
			role->GetTouzijihua()->OnBuyBossPlan();
		}
		break;

	case Protocol::TOUZIJIHUA_FB_BOSS_OPERATE_BOSS_REWARD:
		{
			role->GetTouzijihua()->OnFetchBossPlanReward(op->param);
		}
		break;

	case Protocol::TOUZIJIHUA_FB_BOSS_OPERATE_SHENYU_BOSS_BUY:
		{
			role->GetTouzijihua()->OnBuyShenyuBossPlan();
		}
		break;

	case Protocol::TOUZIJIHUA_FB_BOSS_OPERATE_SHENYU_BOSS_REWARD:
		{
			role->GetTouzijihua()->OnFetchShenyuBossPlanReward(op->param);
		}
		break;

	}
}

void MessageHandler::OnYaojiangReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSYaojiangReq *yjr = (Protocol::CSYaojiangReq *)msg;

	role->GetRoleYaojiang()->OnYaojiangReq(yjr->yaojiang_type);
}

void MessageHandler::OnDatingInviteReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSDatingInviteReq *dir = (Protocol::CSDatingInviteReq *)msg;
	if (dir->target_uid <= 0) return;

	role->GetRoleActivity()->OnDatingInviteReq(IntToUid(dir->target_uid), 0 != dir->is_specific_invite);
}

void MessageHandler::OnDatingInviteAck(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSDatingInviteAck *dia = (Protocol::CSDatingInviteAck *)msg;
	if (dia->req_uid <= 0) return;

	role->GetRoleActivity()->OnDatingInviteAck(IntToUid(dia->req_uid), 0 != dia->is_agree);
}

void MessageHandler::OnFishPoolRaiseReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	FishPoolManager::Instance().RaiseFish(role);
}

void MessageHandler::OnFishPoolBuyBulletReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	FishPoolManager::Instance().BuyBulletReq(role);
}

void MessageHandler::OnFishPoolQueryReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFishPoolQueryReq *fpqr = (Protocol::CSFishPoolQueryReq *)msg;
	FishPoolManager::Instance().OnQueryReq(role, fpqr->query_type, fpqr->param);
}

void MessageHandler::OnFishPoolStealFish(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFishPoolStealFish *fpsf = (Protocol::CSFishPoolStealFish *)msg;
	FishPoolManager::Instance().StealFish(role, fpsf->target_uid, fpsf->is_fake_pool, fpsf->type, fpsf->quality);
}

void MessageHandler::OnFishPoolHarvestFish(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	FishPoolManager::Instance().HarvestFish(role);
}

void MessageHandler::OnFishPoolExtendCapacity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnFishPoolUpQuality(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	FishPoolManager::Instance().UpFishQuality(role);
}

void MessageHandler::OnPaoHuanSkipTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSPaoHuanSkipTask *tmct = (Protocol::CSPaoHuanSkipTask *)msg;
	role->GetPaoHuanTask()->OnSkipTask((0 != tmct->skip_all), tmct->task_id);
}

void MessageHandler::OnMountBianshenOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMountBianshenOpera *mbso = (Protocol::CSMountBianshenOpera *)msg;
	role->OnMountBianshenOpera(mbso->is_bianshen > 0 ? true : false);
}

void MessageHandler::OnSpeedupHello(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->OnSpeedupHello();
}

void MessageHandler::OnMarryXunyouOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMarryXunyouOpera *mxo = (Protocol::CSMarryXunyouOpera *)msg;

	role->GetRoleActivity()->OnMarryXunyouOpera(mxo->opera_type);
}

void MessageHandler::OnMarryHunyanBless(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	static Protocol::CSMarryHunyanBless mhb;
	mhb.ResetMailContentTxt();

	int offset = static_cast<int>(UNSTD_OFFSETOFF(Protocol::CSMarryHunyanBless, contenttxt));
	if (length < offset) return;

	const char *pt = (const char *)msg;

	memcpy((void*)&mhb, pt, offset);
	pt += offset; length -= offset;

	if (length < 0 || length >= MAX_CONTENT_TXT_LENTH || length != mhb.contenttxt_len) return;

	memcpy(&mhb.contenttxt[0], pt, mhb.contenttxt_len);
	mhb.contenttxt[sizeof(mhb.contenttxt) - 1] = '\0';

	SpecialHunyan *special_logic = dynamic_cast<SpecialHunyan*>(role->GetScene()->GetSpecialLogic());
	if (NULL != special_logic && Scene::SCENE_TYPE_HUN_YAN_FB == special_logic->GetSceneType())
	{
		special_logic->ZhuFuHelper(role, &mhb);
	}
}

void MessageHandler::OnMarryHunyanGetBlessNews(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	SpecialHunyan *special_logic = (SpecialHunyan *)role->GetScene()->GetSpecialLogic();
	if (NULL != special_logic && Scene::SCENE_TYPE_HUN_YAN_FB == special_logic->GetSceneType())
	{
		//special_logic->OnGetHunyanBlessNews(role);
	}
}

void MessageHandler::OnMarryHunyanOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMarryHunyanOpera *mho = (Protocol::CSMarryHunyanOpera *)msg;

	if (HUNYAN_OPERA_TYPE_INVITE_INFO == mho->opera_type)
	{
		FBManager::Instance().SendHunYanFbInviteInfoToRole(role);
		return;
	}

	else if (HUNYAN_OPERA_TYPE_JOIN_HUNYAN == mho->opera_type)
	{
		if (!role->GetRoleStatusManager()->CanEnterFB())
		{
			return;
		}

		// 	const QingyuanHunyanActiviryConfig * hunyan_act_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetQingyuanHunyanActivityCfg();
		// 	if (NULL == hunyan_act_cfg) return;
		// 	if (role->GetQingyuan()->GetTodayHunyanJoinTimes() >= hunyan_act_cfg->join_yanhui_max_count)
		// 	{
		// 		role->NoticeNum(errornum::EN_MARRY_HUNYAN_OPEN_LIMIT);
		// 		return;
		// 	}

		const HunyanConfig &hunyan_cfg = LOGIC_CONFIG->GetQingyuanCfg().GetHunyanConfig();

		Scene *huyan_scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(hunyan_cfg.hunyan_scene_id, mho->param_1);

		if (NULL != huyan_scene && Scene::SCENE_TYPE_HUN_YAN_FB == huyan_scene->GetSceneType())
		{
			SpecialHunyan *hunyan_logic = dynamic_cast<SpecialHunyan*>(huyan_scene->GetSpecialLogic());

			if (nullptr == hunyan_logic || !hunyan_logic->IsCanEnterHunYanSence(role)) return;

			if (!hunyan_logic->HunyanIsStop())
			{
				Posi enter_pos = Posi(hunyan_cfg.hunyan_birth_pos_x, hunyan_cfg.hunyan_birth_pos_y);

				GameName role_name = { 0 };
				role->GetName(role_name);
				int role_uid = role->GetUID();

				bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(role, hunyan_cfg.hunyan_scene_id, COMMON_SCENE_KEY, enter_pos);

				FBManager::Instance().LogFunctionStats(role, FBManager::FBCT_HUNYAN, 0, 0, 0);

				gamelog::g_log_debug.printf(LL_DEBUG, "FBManager::EnterFB: %s[%d] ret:%d fb_param:%d,%d,%d ",
					role_name, role_uid, (ret ? 1 : 0), FBManager::FBCT_HUNYAN, 0, 0);

				if (!ret)
				{
					role->NoticeNum(errornum::EN_FB_ENTER_FB_ERROR);
					return;
				}
			}
			else
			{
				role->NoticeNum(errornum::EN_HUNYAN_ALREADY_STOP);
				return;
			}
		}
		else
		{
			role->NoticeNum(errornum::EN_HUNYAN_ALREADY_STOP);
			return;
		}
	}

	else
	{
//		SpecialHunyan *special_logic = (SpecialHunyan *)role->GetScene()->GetSpecialLogic();
		SpecialHunyan *special_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialHunyan(COMMON_SCENE_KEY, false);
		if (NULL != special_logic && Scene::SCENE_TYPE_HUN_YAN_FB == special_logic->GetSceneType())
		{
			special_logic->OnHunyanOpera(role, mho->opera_type, mho->param_1, mho->param_2);
		}
	}
}

void MessageHandler::OnQingyuanOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSQingYuanOperaReq *qyor = (Protocol::CSQingYuanOperaReq *)msg;
	role->GetQingyuan()->QingYuanOperaReq(qyor->opera_type, qyor->param1, qyor->param2);
}

void MessageHandler::OnResetLoverName(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSResetLoverName *rln = (Protocol::CSResetLoverName *)msg;

	rln->new_lover_name[sizeof(rln->new_lover_name) - 1] = 0;
	role->SetMarryInfo(role->GetLoverUid(), rln->new_lover_name);
}

void MessageHandler::OnMarryReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMarryReq *mr = (Protocol::CSMarryReq *)msg;

	role->GetQingyuan()->OnMarryOperate(mr->ope_type, mr->param_1, mr->param_2);
}

void MessageHandler::OnMarryRet(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMarryRet *mr = (Protocol::CSMarryRet *)msg;
	role->GetQingyuan()->MarryRet(mr->marry_type, mr->req_uid, mr->is_accept > 0);
}

void MessageHandler::OnMarryZhuheShou(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMarryZhuheSend * pro = (Protocol::CSMarryZhuheSend *)msg;
	role->GetQingyuan()->SendMarryZhuhe(pro->uid, pro->type);
}

void MessageHandler::OnQingYuanShengDiOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSQingYuanShengDiOperaReq * pro = (Protocol::CSQingYuanShengDiOperaReq *)msg;
	if (Protocol::QYSD_OPERA_TYPE_FETCH_TASK_REWARD == pro->opera_type)
	{
		role->GetQingyuan()->OnQingYuanShengDiFecthTaskReward(pro->param);
	}
	else if (Protocol::QYSD_OPERA_TYPE_FETCH_OTHER_REWARD == pro->opera_type)
	{
		role->GetQingyuan()->OnQingYuanShengDiFecthOtherReward();
	}
}

void MessageHandler::OnProfessWallReq(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSProfessWallReq *req = (Protocol::CSProfessWallReq*)msg;
	role->GetQingyuan()->OnProfessWallReq(req);
}
void MessageHandler::OnProfessToReq(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSProfessToReq *req = (Protocol::CSProfessToReq*)msg;
	role->GetQingyuan()->OnProfessTo(req->target_role_id, req->gift_id, 1 == req->is_auto_buy, req->content);
}

void MessageHandler::OnDivorceRet(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSDivorceRet *dr = (Protocol::CSDivorceRet *)msg;
	role->GetQingyuan()->DivorceRet(dr->req_uid, dr->is_accept > 0);
}

void MessageHandler::OnBuyXunyouOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSBuyXunyouOpera *bxyo = (Protocol::CSBuyXunyouOpera *)msg;
	role->GetRoleActivity()->OnBuyXunyouOpera(bxyo->type);
}

void MessageHandler::OnLightenWeddingRing(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnUplevelWeddingRing(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnPerformWeddingRingSkill(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSPerformWeddingRingSkill *fcqdr = (Protocol::CSPerformWeddingRingSkill *)msg;
	role->OnWeddingRingSkill(fcqdr->ring_level, fcqdr->target_obj_id);
}

void MessageHandler::OnCreateTeam(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSCreateTeam *ct = (Protocol::CSCreateTeam*)msg;;
	TeamManager::Instance().OnCreateTeam(role, ct);
}

void MessageHandler::OnSupplyBuyItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSupplyBuyItem *sbi = (Protocol::CSSupplyBuyItem*)msg;
	role->GetRoleSupply()->OnBuySupplyItem(sbi->supply_type, sbi->index, 0 != sbi->is_use_no_bind_gold);
}

void MessageHandler::OnSupplySetRecoverRangePer(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSupplySetRecoverRangePer *ssrrp = (Protocol::CSSupplySetRecoverRangePer*)msg;
	role->GetRoleSupply()->OnSetSupplyRecoverRangePer(ssrrp->supply_type, ssrrp->recover_range_per);
}

void MessageHandler::OnNationalBossBuyBuffReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->OnNationalBossBuyBuffReq();
}

void MessageHandler::OnGetOpenGameActivityInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->SendOpenGameActivityInfo();
}

void MessageHandler::OnGetOpenGameFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSOpenGameActivityFetchReward *ogafr = (Protocol::CSOpenGameActivityFetchReward *)msg;
	role->GetRoleActivity()->OnFetchOpenGameActivityReward(ogafr->reward_type, ogafr->reward_seq);
}

void MessageHandler::OnBaiBeiFanLiBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->OnBaiBeiFanLiBuy();
}

void MessageHandler::OnBaiBeiFanLi2Buy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->OnBaiBeiFanLi2Buy();
}

void MessageHandler::OnSupperGiftBuyReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRAOpenGameGiftShopBuy *ogafr = (Protocol::CSRAOpenGameGiftShopBuy *)msg;
	role->GetRoleActivity()->OnOpenGameGiftShopBuy(ogafr->seq);
}

void MessageHandler::OnSupperGiftBuy2Req(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRAOpenGameGiftShopBuy2 *ogafr = (Protocol::CSRAOpenGameGiftShopBuy2 *)msg;
	switch (ogafr->opera_type)
	{
	case Protocol::GiftShopBuy2_OPERA_TYPE::GiftShopBuy2_OPERA_TYPE_INFO:
	{
		role->GetRoleActivity()->SendOpenGameGiftShopBuy2Info();
	}
	break;

	case Protocol::GiftShopBuy2_OPERA_TYPE::GiftShopBuy2_OPERA_TYPE_BUY:
	{
		role->GetRoleActivity()->OnOpenGameGiftShopBuy2(ogafr->seq);
	}
	break;

	default:
		break;
	}
}

void MessageHandler::OnSupperGiftInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->SendOpenGameGiftShopBuyInfo();
}

void MessageHandler::OnGetTitleOwnerInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->OnGetTitleOwnerInfo();
}

void MessageHandler::OnFetchMagicalPreciousReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFetchMagicalPreciousRewardReq *ogafr = (Protocol::CSFetchMagicalPreciousRewardReq *)msg;

	switch (ogafr->operater_type)
	{
	case Protocol::CSFetchMagicalPreciousRewardReq::FETCH_REWARD:
	{
		role->GetMagicalPreciousManager()->FetchScore(ogafr->param_1, ogafr->param_2);
	}
	break;

	case Protocol::CSFetchMagicalPreciousRewardReq::FETCH_ALL_INFO:
	{
		role->GetMagicalPreciousManager()->SendMagicalPreciousAllInfo();
	}
	break;

	case Protocol::CSFetchMagicalPreciousRewardReq::CONVERT_CHAPTER_REWARD:
	{
		role->GetMagicalPreciousManager()->ConvertPreciousChapterReward(ogafr->param_1, ogafr->param_2);
	}
	break;

	case Protocol::CSFetchMagicalPreciousRewardReq::FETCH_CUR_CHAPTER_INFO:
	{
		role->GetMagicalPreciousManager()->SendMagicalPreciousChapterInfo(ogafr->param_2);
	}
	break;
	}
}

void MessageHandler::OnRAExpRefineOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRAExpRefineReq *rerr = (Protocol::CSRAExpRefineReq *)msg;
	role->GetRoleActivity()->OnRaRefineOperaReq(rerr->opera_type);
}

void MessageHandler::OnChallengeFieldGetUserInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	ChallengeField::Instance().OnGetChallengeUserInfo(role);
}

void MessageHandler::OnChallengeFieldResetOpponentReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	ChallengeField::Instance().OnResetOpponentReq(role);
}

void MessageHandler::OnChallengeFieldFightReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSChallengeFieldFightReq *cffr = (Protocol::CSChallengeFieldFightReq *)msg;
	ChallengeField::Instance().OnFightReq(role, cffr->opponent_index, (0 != cffr->ignore_rank_pos), cffr->rank_pos, (0 != cffr->auto_buy_jointimes));
}

void MessageHandler::OnChallengeFieldBuyJoinTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	ChallengeField::Instance().OnBuyJoinTimes(role);
}

void MessageHandler::OnChallengeFieldFetchJifenRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	//	Protocol::CSChallengeFieldFetchJifenRewardReq *cffjfr = (Protocol::CSChallengeFieldFetchJifenRewardReq *)msg;
	//	ChallengeField::Instance().OnFetchJifenReq(role, cffjfr->reward_seq);
}

void MessageHandler::OnChallengeFieldGetRankInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	ChallengeField::Instance().OnGetRankInfo(role);
}

void MessageHandler::OnChallengeFieldGetLuckyInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	//ChallengeField::Instance().OnGetLuckyInfo(role);
}

void MessageHandler::OnChallengeFieldFetchGuangHui(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	ChallengeField::Instance().OnFetchGuangHui(role);
}

void MessageHandler::OnChallengeFieldGetOpponentDetailInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSChallengeFieldGetOpponentInfo *cfgoi = (Protocol::CSChallengeFieldGetOpponentInfo *)msg;
	ChallengeField::Instance().OnGetOpponentDetailInfo(role, (0 != cfgoi->is_get_top));
}

void MessageHandler::OnChallengeFieldBuyBuff(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	ChallengeField::Instance().OnChallengeFieldBuyBuff(role);
}

void MessageHandler::OnChallengeFieldBestRankBreakReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSChallengeFieldBestRankBreakReq *cfbrbr = (Protocol::CSChallengeFieldBestRankBreakReq *)msg;
	switch (cfbrbr->op_type)
	{
	case Protocol::CF_BREAK_TYPE_INFO:
	{
		role->GetRoleChallengeField()->SendRoleCFBestRankBreakInfo();
		break;
	}

	case Protocol::CF_BREAK_TYPE_BREAK:
	{
		role->GetRoleChallengeField()->RoleCFBestRankBreak();
		break;
	}
	}
}

void MessageHandler::OnChallengeFieldReadyStartFightReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
//	Protocol::CSChallengeFieldReadyStartFightReq *cfrsf = (Protocol::CSChallengeFieldReadyStartFightReq *)msg;
	if (NULL == role || NULL == role->GetScene())
	{
		return;
	}
	
	SpecialChallengeField* special_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialChallengeField(role->GetScene()->GetSceneID(), role->GetUID());
	if (NULL != special_logic)
	{
		special_logic->StartReadyFightCountdownReq(role);
	}
}

void MessageHandler::OnCloseBetaActivityOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCloseBetaActivityOperaReq *cbaor = (Protocol::CSCloseBetaActivityOperaReq *)msg;
	role->GetRoleActivity()->OnCloseBetaActivityOperaReq(cbaor->opera_type, cbaor->param_1, cbaor->param_2, cbaor->param_3);
}

void MessageHandler::OnLuckyRollActivityOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSLuckyRollActivityOperaReq *lraor = (Protocol::CSLuckyRollActivityOperaReq *)msg;
	role->GetRoleActivity()->OnRALuckyRollOperaReq(lraor->opera_type);
}

void MessageHandler::OnRandActivityOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRandActivityOperaReq *raor = (Protocol::CSRandActivityOperaReq *)msg;

	switch (raor->rand_activity_type)
	{
	case RAND_ACTIVITY_TYPE_CHONGZHI_RANK:
		{
			role->GetRoleActivity()->SendRAChongzhiRankInfo();
		}
		break;

	case RAND_ACTIVITY_TYPE_CHONGZHI_RANK2:
		{
			role->GetRoleActivity()->SendRAChongzhiRank2Info();
		}
		break;

	case RAND_ACTIVITY_TYPE_CONSUME_GOLD_RANK:
		{
			role->GetRoleActivity()->SendRAConsumeGoldRankInfo();
		}
		break;

	case RAND_ACTIVITY_TYPE_SERVER_PANIC_BUY:
		{
			role->GetRoleActivity()->OnRAServerPanicBuyOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_PERSONAL_PANIC_BUY:
		{
			role->GetRoleActivity()->OnRAPersonalPanicBuyOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_CONSUME_GOLD_FANLI:
		{
			role->GetRoleActivity()->SendRAConsumeGoldFanliInfo();
		}
		break;

	case RAND_ACTIVITY_TYPE_DAY_CHONGZHI_FANLI:
		{
			role->GetRoleActivity()->OnRADayChongZhiFanLiOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_DAY_CONSUME_GOLD:
		{
			role->GetRoleActivity()->OnRADayConsumeGoldOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_CHESTSHOP:
		{
			//role->GetRoleActivity()->OnRAChestshopOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_STONE_UPLEVEL:
		{
			//role->GetRoleActivity()->OnRAStoneUplevelOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_XN_CHANMIAN_UPLEVEL:
		{
			//role->GetRoleActivity()->OnRAXiannvChanmianUplevelOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_MOUNT_UPGRADE:
		{
			role->GetRoleActivity()->OnRAMountUpgradeOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_TOTAL_CONSUME_GOLD:
		{
			role->GetRoleActivity()->OnRATotalConsumeGoldOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_DAY_ACTIVIE_DEGREE:
		{
			role->GetRoleActivity()->OnRADayActiveDegreeOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_QIBING_UPGRADE:
		{
			//role->GetRoleActivity()->OnRAQibingUpgradeOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_MENTALITY_TOTAL_LEVEL:
		{
			role->GetRoleActivity()->OnRAMentalityTotalLevelOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_WING_UPGRADE:
		{
			role->GetRoleActivity()->OnRAWingUpgradeOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_KILL_BOSS:
		{
			role->GetRoleActivity()->OnRAKillBossOperaReq(raor->opera_type);
		}
		break;

	case RAND_ACTIVITY_TYPE_QUANMIN_QIFU:
		{
			//role->GetRoleActivity()->OnRAQuanminQifuOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_SHOUYOU_YUXIANG:
		{
			role->GetRoleActivity()->OnRAShouYouYuXiangOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_LOGIN_GIFT:
		{
			role->GetRoleActivity()->OnRALoginGiftOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_LOGIN_GIFT_0:
	{
		role->GetRoleActivity()->OnRALoginGiftOperaReq_0(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT:
	{
		role->GetRoleActivity()->OnRAEverydayNiceGiftReq(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_XIANMENG_BIPIN:
		{
			role->GetRoleActivity()->SendRAXianMengBiPinInfo();
		}
		break;

	case RAND_ACTIVITY_TYPE_XIANMENG_JUEQI:
		{
			role->GetRoleActivity()->SendRAXianMengJueQiInfo();
		}
		break;

	case RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL:
		{
			role->GetRoleActivity()->OnRABipinCapTotalOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP:
		{
			role->GetRoleActivity()->OnRABipinCapEquipOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV:
		{
			//role->GetRoleActivity()->OnRABipinCapXianNvOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI:
		{
			//role->GetRoleActivity()->OnRABipinCapJingMaiOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING:
		{
			role->GetRoleActivity()->OnRABipinCapWingOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT:
		{
			role->GetRoleActivity()->OnRABipinCapMountOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG:
		{
			role->GetRoleActivity()->OnRABipinCapShenzhuangOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING:
		{
			role->GetRoleActivity()->OnRABipinCapJinglingOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH:
		{
			role->GetRoleActivity()->OnRABipinCapWashlOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_CHARGE_REPAYMENT:
		{
			role->GetRoleActivity()->OnRAChargeRepaymentOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_CORNUCOPIA:
		{
			role->GetRoleActivity()->OnRACornucopiaOperaReq(raor->opera_type);
		}
		break;

	case RAND_ACTIVITY_TYPE_NEW_CORNUCOPIA:
		{
			role->GetRoleActivity()->OnRANewCornucopiaOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_DANBI_CHONGZHI:
		{
			role->GetRoleActivity()->OnRADanbiChongzhiOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_TOTAL_CHARGE_DAY:
		{
			role->GetRoleActivity()->OnRATotalChargeDayOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_TOMORROW_REWARD:
		{
			role->GetRoleActivity()->OnRATomorrwRewardOperaReq(raor->opera_type);
		}
		break;

	case RAND_ACTIVITY_TYPE_DAY_CHONGZHI_RANK: 
		{
			role->GetRoleActivity()->SendRADayChongzhiRankInfo();
		}
		break;

	case RAND_ACTIVITY_TYPE_DAY_CONSUME_RANK:
		{
			role->GetRoleActivity()->SendRADayConsumeRankInfo();
		}
		break;

	case RAND_ACTIVITY_TYPE_TOTAL_CHARGE:
		{
			role->GetRoleActivity()->OnRATotalChargeOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_EQUIP:
		{
			role->GetRoleActivity()->OnRATimeLimitExchangeEquiOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_TIME_LIMIT_EXCHANGE_JL:
		{
			role->GetRoleActivity()->OnRATimeLimitExchangeJLOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_LEVEL_LOTTERY:
		{
			role->GetRoleActivity()->OnRALevelLotteryOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_CHONGZHI_NIU_EGG:
		{
			role->GetRoleActivity()->OnRAChongZhiNiuEggOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_ZHENBAOGE:
		{
			role->GetRoleActivity()->OnRaZhenbaogeOperReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_MIJINGXUNBAO:
		{
			role->GetRoleActivity()->OnRaMiJingXunBaoOperReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_MIJINGXUNBAO2:
		{
			role->GetRoleActivity()->OnRaMiJingXunBao2OperReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_MONEY_TREE:
		{
			role->GetRoleActivity()->OnRAChongzhiMoneyTreeOperReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_MONEY_TREE2:
		{
			role->GetRoleActivity()->OnRAChongzhiMoneyTree2OperReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_KING_DRAW:
		{
			role->GetRoleActivity()->OnRAKingDrawOperaReq(raor->opera_type, raor->param_1, raor->param_2);
		}
		break;

	case RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_PASSIVE_RANK:
		{
			role->GetRoleActivity()->SendRASpecialAppearancePassiveInfo();
		}
		break;

	case RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_RANK:
		{
			role->GetRoleActivity()->SendRASpecialAppearanceInfo();
		}
		break;

	case RAND_ACTIVITY_TYPE_MINE:
		{
			role->GetRoleActivity()->OnRAMineOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_GUAGUA:
		{
			role->GetRoleActivity()->OnRAGuaGuaOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_TIANMING_DIVINATION:
		{
			role->GetRoleActivity()->OnRATianMingDivinationOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_FANFAN:
		{
			role->GetRoleActivity()->OnRAFanFanOperaReq(raor->opera_type, raor->param_1, raor->param_2);
		}
		break;

	case RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI:
		{
			role->GetRoleActivity()->OnRAContinueChongzhiOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_CONTINUE_CONSUME:
		{
			role->GetRoleActivity()->OnRAContinueConsumeOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_ARMY_DAY:
		{
			role->GetRoleActivity()->OnRaArmyDayOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI:
		{
			role->GetRoleActivity()->OnRACirculationChongzhiOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_EXTREME_LUCKY:
		{
			role->GetRoleActivity()->OnRAExtremeLuckyOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_NATIONAL_DAY:
		{
			role->GetRoleActivity()->OnRaNationalDayOpeReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_ZHENBAOGE2:
		{
			role->GetRoleActivity()->OnRaZhenbaoge2OperReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_PROMOTING_POSITION:
		{
			role->GetRoleActivity()->OnRAPromotingPositionOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_BLACK_MARKET:
		{
			role->GetRoleActivity()->OnRABlackMarketOperaReq(raor->opera_type, raor->param_1, raor->param_2);
		}
		break;

	case RAND_ACTIVITY_TYPE_TREASURES_MALL:
		{
			role->GetRoleActivity()->OnRaTreasuresMallOperaReq(raor->opera_type, raor->param_1, raor->param_2);
		}
		break;

	case RAND_ACTIVITY_TYPE_ITEM_COLLECTION:
		{
			role->GetRoleActivity()->OnRAItemCollectionOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_RED_ENVELOPE_GIFT:
		{
			if (1 == raor->opera_type)
			{
				role->GetRoleActivity()->FetchRedEnvelopeGiftDiamond(raor->param_1);
			}
		}
		break;

	case RAND_ACTIVITY_TYPE_MARRY_ME:
		{
			role->GetRoleActivity()->OnRAMarryMeOperaReq(raor->opera_type);
		}
		break;

	case RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU:
		{
			role->GetRoleActivity()->OnRAContinueChongzhiOperaReqChu(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO:
		{
			role->GetRoleActivity()->OnRAContinueChongzhiOperaReqGao(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_2:
	{
		role->GetRoleActivity()->OnRAContinueChongzhiOperaReq2(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_OPEN_SERVER_INVEST:
		{
			role->GetRoleActivity()->OnRAOpenServerInvestOperateReq(raor->opera_type, raor->param_1, raor->param_2);
		}
		break;

	case RAND_ACTIVITY_TYPE_SINGLE_CHONGZHI:
		{
			role->GetRoleActivity()->OnRASingleChongZhiOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_XIANYUAN_TREAS:
		{
			role->GetRoleActivity()->OnRaXianyuanTreasOperReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_RUSH_BUYING:
		{
			role->GetRoleActivity()->OnRARushBuyingOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_MAP_HUNT:
		{
			role->GetRoleActivity()->OnRaMapHuntOperaReq(raor->opera_type, raor->param_1, raor->param_2);
		}
		break;

	case RAND_ACTIVITY_TYPE_LIGHT_TOWER_EXPLORE:
		{
			role->GetRoleActivity()->OnRaLightTowerExploreOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE:
		{
			role->GetRoleActivity()->OnRANewTotalChargeOperaReq(raor->opera_type, raor->param_1);
		}
	break;

	case RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1:
	{
		role->GetRoleActivity()->OnRANewTotalCharge1OperaReq(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2:
	{
		role->GetRoleActivity()->OnRANewTotalCharge2OperaReq(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3:
	{
		role->GetRoleActivity()->OnRANewTotalCharge3OperaReq(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_MAGIC_SHOP:
		{
			role->GetRoleActivity()->OnRAMagicShopOperaReq(raor->opera_type, raor->param_1, raor->param_2);
		}
		break;

	case RAND_ACTIVITY_TYPE_PLANTING_TREE:
		{
			role->GetRoleActivity()->OnRAPlantingTreeOperaReq(raor->opera_type, raor->param_1, raor->param_2);
		}
		break;

	case RAND_ACTIVITY_TYPE_FISHING:
		{
			role->GetRoleActivity()->OnRAFishingOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_PERFECT_LOVER:
		{
			role->GetRoleActivity()->PerfectLoverCheckAndSendInfo();
		}
		break;

	case RAND_ACTIVITY_TYPE_QUANMINJINJIE:
		{
			role->GetRoleActivity()->OnQuanMinJinJie(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_QUANMINZONGDONGYUAN:
		{
			role->GetRoleActivity()->OnUpgradeGroupe(raor->opera_type, raor->param_1);
		}
		break;
	case RAND_ACTIVITY_TYPE_GUILDBATTLE:
		{
			role->GetRoleActivity()->OnNewGuildBattle(raor->opera_type);
		}
		break;

	case RAND_ACTIVITY_TYPE_CONSUM_GIFT:
		{
			role->GetRoleActivity()->OnRAConsumeGiftOpera(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_BUY_LIMIT:
		{
			role->GetRoleActivity()->OnRADailyLimitBuyOpera(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_COLLECT_TREASURE:
		{
			role->GetRoleActivity()->OnRACollectTreasureOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY:
		{
			role->GetRoleActivity()->OnRACriticalStrikeOperaReq();
		}
		break;

	case RAND_ACTIVITY_TYPE_HAPPY_ACCUMUL_RECHARGE:
		{
			role->GetRoleActivity()->OnRAHappyCumulChongzhiOpera(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_UPLEVEL_RETURN:
		{
			role->GetRoleActivity()->OnRAJinJieReturnOpera(raor->opera_type, raor->param_1);
		}
		break;
	case RAND_ACTIVITY_TYPE_LIMIT_TIME_REBATE:
		{
			role->GetRoleActivity()->OnRALimitTimeRebateOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_CONSUME_FOR_GIFT:
		{
			role->GetRoleActivity()->OnRAConsumeForGiftOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_MIJINGXUNBAO3:
		{
			role->GetRoleActivity()->OnRaMiJingXunBao3OperReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_HUANLE_ZADAN:
		{
			role->GetRoleActivity()->OnRaHuanLeZaDanOperReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG:
		{
			role->GetRoleActivity()->OnRaHuanLeYaoJiangOperReq(raor->opera_type, raor->param_1);
		}
		break;
	
	case RAND_ACTIVITY_TYPE_BUY_ONE_GET_ONE_FREE:
		{
			role->GetRoleActivity()->OnRABuyOneGetOneFreeOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_TIME_LIMIT_GIFT:
		{
			role->GetRoleActivity()->OnRATimeLimitGiftOperaReq(raor->opera_type, raor->param_1, raor->param_2);
		}
		break;

	case RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI_2:
		{
			role->GetRoleActivity()->OnRACirculationChongzhiSecondOperaReq(raor->opera_type);
		}
		break;

	case RAND_ACTIVITY_TYPE_SHAKE_MONEY:
		{
			role->GetRoleActivity()->OnRAShakeMoneyOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_LIMIT_LUXURY_GIFT_BAG_GIFT:
		{
			role->GetRoleActivity()->OnRATimeLimitLuxuryGiftBagOperaReq(raor->opera_type, raor->param_1);
		}
		break;

	case RAND_ACTIVITY_TYPE_RMB_BUT_CHEST_SHOP:
		{
			role->GetRoleActivity()->OnRARmbBugChestShopReq();
		}
		break;

	case RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD:
		{
			role->GetRoleActivity()->OnRAConsumeGoldRewardOpera(raor->opera_type);
		}
		break;

	case RAND_ACTIVITY_TYPE_ITEM_COLLECTION_2:
	{
		role->GetRoleActivity()->OnRAItemCollectionSecondOperaReq(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_VERSIONS_GRAND_TOTAL_CHARGE:
	{
		role->GetRoleActivity()->OnRAVersionTotalChargeOperaReq(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_VERSIONS_CONTINUE_CHARGE:
	{
		role->GetRoleActivity()->OnRAVersionContinueGhargeOperaReq(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2:
	{
		role->GetRoleActivity()->OnRaHuanLeYaoJiang2OperReq(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_DOUBLE_CHONGZHI:
	{
		role->GetRoleActivity()->OnRADoubleChongzhiOpera(raor->opera_type);
	}
	break;

	case RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE:
	{
		role->GetRoleActivity()->OnRAExtremeChallengeReq(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_CHONGZHI_GIFT:
	{
		role->GetRoleActivity()->OnRAChongZhiGiftOperaReq(raor->opera_type, raor->param_1, raor->param_2);
	}
	break;

	case RAND_ACTIVITY_TYPE_PROFESS_RANK:
	{
		role->GetRoleActivity()->SendRAProfessRankInfo();
	}
	break;

	case RAND_ACTIVITY_TYPE_CRITICAL_STRIKE_DAY_2:
	{
		role->GetRoleActivity()->OnRACriticalStrike2OperaReq();
	}
	break;

	case RAND_ACTIVITY_TYPE_UPLEVEL_RETURN_2:
	{
		role->GetRoleActivity()->OnRAJinJieReturn2Opera(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_COMBINE_BUY:
	{
		role->GetRoleActivity()->OnRACombineBuyOpera(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_LUCKY_WISH:
	{
		role->GetRoleActivity()->OnRALuckyWishOpera(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_COUPLE_HALO_SELL:
	{
		role->GetQingyuan()->SendCoupleHaloInfo();
	}
	break;

	case RAND_ACTIVITY_TYPE_LOTTERY_1:
	{
		role->GetRoleActivity()->OnLottery1OperaReq(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_ZERO_BUY_RETURN:
	{
		role->GetRoleActivity()->OnRAZeroBuyReturnOperaReq(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_KUANG_HAI_QING_DIAN:
	{
		role->GetRoleActivity()->OnKuangHaiCommonOpera(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_GIFT_HARVEST:
	{
		role->GetRoleActivity()->OnGiftHarvestReq(raor->opera_type);
	}
	break;

	case RAND_ACTIVITY_TYPE_LUCKY_CLOUD_BUY:
	{
		role->GetRoleActivity()->OnLuckyCloudBuyOperaReq(raor->opera_type, raor->param_1);
	}
	break;
	case RAND_ACTIVITY_TYPE_CRACYBUY:
	{
		role->GetRoleActivity()->OnRACracyBuyOperaReq(raor->opera_type, raor->param_1);
	}
	break;

	case RAND_ACTIVITY_TYPE_ROLE_UPLEVEL:
	case RAND_ACTIVITY_TYPE_PATA:
	case RAND_ACTIVITY_TYPE_EXP_FB:
	//case RAND_ACTIVITY_TYPE_UPGRADE_MOUNT:
	//case RAND_ACTIVITY_TYPE_UPGRADE_HALO:
	case RAND_ACTIVITY_TYPE_UPGRADE_WING:
	case RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG:
	case RAND_ACTIVITY_TYPE_UPGRADE_SHENYI:
	case RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN:
	case RAND_ACTIVITY_TYPE_DAY_TOTAL_CHARGE:
	//case RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_TOTAL:
	//case RAND_ACTIVITY_TYPE_UPGRADE_HALO_TOTAL:
	case RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK:
	case RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK:
	case RAND_ACTIVITY_TYPE_UPGRADE_FABAO_RANK:
	case RAND_ACTIVITY_TYPE_UPGRADE_FOOTPRINT_RANK:
	case RAND_ACTIVITY_TYPE_UPGRADE_WING_TOTAL:
	case RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_TOTAL:
	case RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_TOTAL:
	case RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK:
	case RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK:
	case RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK:
	case RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK:
	case RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK:
	case RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK:
	case RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK:
	case RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK:
	case RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL:
	case RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL:
	case RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK:
	case RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK:
	case RAND_ACTIVITY_TYPE_BOSS_KILLER:
	case RAND_ACTIVITY_TYPE_OPEN_SERVER_BATTLE:
	case RAND_ACTIVITY_TYPE_XIANNV_RANK:
	case RAND_ACTIVITY_TYPE_JINGLING_RANK:
	case RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK:
	case RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK:
	case RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK:
	case RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK:
	case RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK:
	case RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK:
	case RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK:
		{
			role->GetRoleActivity()->OnRAOpenServerOperaReq(raor->rand_activity_type, raor->opera_type, raor->param_1, raor->param_2);
		}
		break;
	}
}

void MessageHandler::OnUpdateNoticeFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->OnFetchUpdateNoticeReward();
}

void MessageHandler::OnCommonActivityFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnMazeQuery(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetMaze()->OnQueryMaze();
}

void MessageHandler::OnMazeMove(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMoveMaze *ssrrp = (Protocol::CSMoveMaze*)msg;
	role->GetMaze()->OnMoveReq(ssrrp->target_x, ssrrp->target_y);
}

void MessageHandler::OnMazeBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetMaze()->BuyMove();
}

void MessageHandler::OnMazeFetchJifenReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFetchMazeJifenReward *fmjfr = (Protocol::CSFetchMazeJifenReward *)msg;
	role->GetMaze()->OnFetchJifenReward(fmjfr->seq);
}

void MessageHandler::OnInviteUser(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSInviteUser *iu = (Protocol::CSInviteUser*)msg;
	TeamManager::Instance().OnInviteUser(role, iu);
}

void MessageHandler::OnInviteUniqueUser(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSInviteUniqueUser *iu = (Protocol::CSInviteUniqueUser*)msg;
	TeamManager::Instance().OnInviteUniqueUser(role, iu);
}

void MessageHandler::OnInviteRet(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSInviteUserTransmitRet *iutr = (Protocol::CSInviteUserTransmitRet*)msg;
	TeamManager::Instance().OnInviteRet(role, iutr);
}

void MessageHandler::OnJoinTeamRet(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSReqJoinTeamRet *jtr = (Protocol::CSReqJoinTeamRet*)msg;
	TeamManager::Instance().OnJoinTeamRet(role, jtr);
}

void MessageHandler::OnReqJoinTeam(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSReqJoinTeam *rjt = (Protocol::CSReqJoinTeam*)msg;
	TeamManager::Instance().OnReqJoinTeam(role, rjt);
}

void MessageHandler::OnDismissTeam(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	TeamManager::Instance().OnDismissTeam(role);
}

void MessageHandler::OnKickOutOfTeam(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSKickOutOfTeam *koof = (Protocol::CSKickOutOfTeam*)msg;
	TeamManager::Instance().OnKickOutOfTeam(role, koof);
}

void MessageHandler::OnChangeTeamLeader(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSChangeTeamLeader *ctl = (Protocol::CSChangeTeamLeader*)msg;
	TeamManager::Instance().OnChangeTeamLeader(role, ctl);
}

void MessageHandler::OnExitTeam(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	TeamManager::Instance().OnExitTeam(role);
}

void MessageHandler::OnTeamListReq(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	TeamManager::Instance().OnTeamListReq(role);
}

void MessageHandler::OnChangeMustCheck(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSChangeMustCheck *cmc = (Protocol::CSChangeMustCheck*)msg;
	TeamManager::Instance().ChangeMustCheck(role, 1 == cmc->must_check);
}

void MessageHandler::OnChangeAssignMode(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSChangeAssignMode *cam = (Protocol::CSChangeAssignMode*)msg;
	TeamManager::Instance().ChangeAssignMode(role, cam->assign_mode);
}

void MessageHandler::OnChangeMemberCanInvite(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSChangeMemberCanInvite *cmi = (Protocol::CSChangeMemberCanInvite*)msg;
	TeamManager::Instance().ChangeMemberCanInvite(role, 1 == cmi->member_can_invite);
}

void MessageHandler::OnChangeTeamLimit(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSChangeTeamLimit *ctl = (Protocol::CSChangeTeamLimit*)msg;
	TeamManager::Instance().ChangeTeamLimit(role, ctl->limit_capability, ctl->limit_level);
}

void MessageHandler::OnAutoHaveTeam(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	TeamManager::Instance().AutoHaveTeam(role);
}

void MessageHandler::OnAutoApplyJoinTeam(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSAutoApplyJoinTeam *cmd = (Protocol::CSAutoApplyJoinTeam*)msg;

	role->GetTeamInfo()->SetIsAutoJoinTeam((char)cmd->is_auto_join_team);
}

void MessageHandler::OnGetRandomRoleList(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::SCRandomRoleListRet rruid;

	bool ret = m_scene_manager->RandomGetRoleList(role, rruid.role_list, &(rruid.count));
	if (ret)
	{
		int size_len = sizeof(rruid) - sizeof(rruid.role_list[0]) * (Protocol::MAX_RANDOM_ROLE_LIST_NUM - rruid.count);
		EngineAdapter::Instance().NetSend(netid, (const char*)&rruid, size_len);
	}
}

void MessageHandler::OnChannelChat(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	PerformanceCatch::Instance().CalcPerformanceCatchBegin(PERFORMANCE_TEST_CHANEL_CHAT);

	ChatManager::Instance().OnChannelChat(role, msg, length, chatdef::ORIGIN_TYPE_NORMAL_CHAT);

	PerformanceCatch::Instance().CalcPerformanceCatchEnd(PERFORMANCE_TEST_CHANEL_CHAT);
}

void MessageHandler::OnSingleChat(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	PerformanceCatch::Instance().CalcPerformanceCatchBegin(PERFORMANCE_TEST_SINGLE_CHAT);

	ChatManager::Instance().OnSingleChat(role, msg, length, false);

	PerformanceCatch::Instance().CalcPerformanceCatchEnd(PERFORMANCE_TEST_SINGLE_CHAT);
}

void MessageHandler::OnSingleChatStatusReq(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSSingleChatReq *scos = (Protocol::CSSingleChatReq*)msg;

	//int plat_type = LocalConfig::Instance().GetPlatType();

	switch (scos->req_type)
	{
	case Protocol::SINGLE_CHAT_REQ_ADD:
	{
		UserID target_userid = IntToUid(scos->param_2);
		UniqueUserID unique_userid(scos->param_1, target_userid);
		long long target_uuid = UniqueUserIDToLongLong(unique_userid);

		ChatManager::Instance().AddListenRoleInfo(role, target_uuid);
	}
	break;

	case Protocol::SINGLE_CHAT_REQ_DELETE:
	{
		UserID target_userid = IntToUid(scos->param_2);
		UniqueUserID unique_userid(scos->param_1, target_userid);
		long long target_uuid = UniqueUserIDToLongLong(unique_userid);

		ChatManager::Instance().DeleteListenRoleInfo(role, target_uuid);
	}
	break;

	case Protocol::SINGLE_CHAT_REQ_DELETE_ALL:
	{
		ChatManager::Instance().DeleteListenAllRoleInfo(role);
	}
	break;

	}
}

void MessageHandler::OnRequestForbidChatInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	ChatManager::Instance().OnRequestForbidInfo(role);
}

void MessageHandler::OnWorldBossKillerInfoReq(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSWorldBossKillerInfoReq *wbkir = (Protocol::CSWorldBossKillerInfoReq *)msg;
	WorldBossManager::Instance().SendBossKillerList(role, wbkir->boss_id);
}

void MessageHandler::OnGetWorldBossInfo(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	WorldBossManager::Instance().SendWorldBossListInfo(role);
}

void MessageHandler::OnGetWorldBossPersonalHurtInfo(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSWorldBossPersonalHurtInfoReq *gwbi = (Protocol::CSWorldBossPersonalHurtInfoReq *)msg;
	WorldBossManager::Instance().SendWorldBossPersonalHurtInfo(role,gwbi->boss_id);
}

void MessageHandler::OnGetWorldBossGuildHurtInfo(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSWorldBossGuildHurtInfoReq *gwbi = (Protocol::CSWorldBossGuildHurtInfoReq *)msg;
	WorldBossManager::Instance().SendWorldBossGuildHurtInfo(role,gwbi->boss_id);
}

void MessageHandler::OnGetWorldBossWeekRankInfo(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	WorldBossManager::Instance().SendWorldBossWeekRankInfo(role);
}

void MessageHandler::OnGetWorldBossRollReq(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol:: CSWorldBossRollReq *cmd = (Protocol:: CSWorldBossRollReq *)msg;
	WorldBossManager::Instance().RollPoint(role , cmd->boss_id ,cmd->hudun_index);
}

void MessageHandler::OnGetPersonRankList(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSGetPersonRankList *gprl = (Protocol::CSGetPersonRankList *)msg;
	RankManager::Instance().OnGetPersonRankList(role, gprl);
}

void MessageHandler::OnGetPersonRankTopUser(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSGetPersonRankTopUser *gprtu = (Protocol::CSGetPersonRankTopUser *)msg;
	RankManager::Instance().OnGetPersonRankTopUser(role, gprtu);
}

void MessageHandler::OnGetGuildRankList(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSGetGuildRankList *ggrl = (Protocol::CSGetGuildRankList *)msg;
	RankManager::Instance().OnGetGuildRankList(role, ggrl);
}

void MessageHandler::OnGetTeamRankList(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSGetTeamRankList *gtrl = (Protocol::CSGetTeamRankList *)msg;
	RankManager::Instance().OnGetTeamRankList(role, gtrl);
}

void MessageHandler::OnQingyuanFBOperaReq(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSQingyuanFBOperaReq* qyor = (Protocol::CSQingyuanFBOperaReq*)msg;

	switch (qyor->opera_type)
	{
	case Protocol::QINGYUAN_FB_OPERA_TYPE_BASE_INFO:
		{
			role->GetQingyuan()->SendBaseFBInfo();
		}
		break;

	case Protocol::QINGYUAN_FB_OPERA_TYPE_BUY_TIMES:
		{
			role->GetQingyuan()->BuyFBTimesReq();
		}
		break;

	case Protocol::QINGYUAN_FB_OPERA_TYPE_BUY_BUFF:
		{
			if (Scene::SCENE_TYPE_LOVERS_FB == role->GetScene()->GetSpecialLogic()->GetSceneType())
			{
				SpecialLoversFB *special_logic = dynamic_cast<SpecialLoversFB*>(role->GetScene()->GetSpecialLogic());
				if (nullptr != special_logic)
					special_logic->OnBuyBuff(role);
			}
		}
		break;

	case Protocol::QINGYUAN_FB_OPERA_TYPE_BUY_DOUBLE_REWARD:
		{
			if (Scene::SCENE_TYPE_LOVERS_FB == role->GetScene()->GetSpecialLogic()->GetSceneType())
			{
				SpecialLoversFB *special_logic = dynamic_cast<SpecialLoversFB*>(role->GetScene()->GetSpecialLogic());
				if (nullptr != special_logic)
					special_logic->OnBuyDoubleReward(role);
			}
			else
			{
				role->GetQingyuan()->OnBuyDoubleReward();
			}
		}
		break;
	}
}

void MessageHandler::OnHunjieUpLevel(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSHunjieUpLevel *qyuq = (Protocol::CSHunjieUpLevel *)msg;
	const short repeat_times = qyuq->repeat_times;

	int old_level = role->GetQingyuan()->GetHunjieStarLevel();
	bool ret = role->GetQingyuan()->HunjieEquipUpLevel(qyuq->stuff_id, repeat_times, 1 == qyuq->is_auto_buy ? true: false);

	if (!ret || old_level != role->GetQingyuan()->GetHunjieStarLevel())
	{
		role->SendOperateResult(Protocol::SCOperateResult::OP_QINGYUAN_EQUIP_UPLEVEL, 0);
		return;
	}

	if (repeat_times > 0)
	{
		role->SendOperateResult(Protocol::SCOperateResult::OP_QINGYUAN_EQUIP_UPLEVEL, 1);
	}
}

void MessageHandler::OnHunjieReqEquipInfo(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	role->GetQingyuan()->SendHunjieEquipInfo();
}

void MessageHandler::OnQingyuanTakeOffEquip(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	role->GetQingyuan()->TakeOff();
}

void MessageHandler::OnQingyuanQueryMateQingyuanValue(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetQingyuan()->QueryMateQingyuanValue();
}

void MessageHandler::OnQingyuanDivorceReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSQingyuanDivorceReq *qydr = (Protocol::CSQingyuanDivorceReq *)msg;
	role->GetQingyuan()->DivorceReq(qydr->is_forced_divorce);
}

void MessageHandler::OnFBReqNextLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	FBManager::Instance().ReqNextLevel(role);
}

void MessageHandler::OnQingyuanEquipOperate(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSQingyuanEquipOperate* qyul = (Protocol::CSQingyuanEquipOperate *)msg;
	QingyuanGlobal * qingyuan_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(role->GetUID());
	if (NULL != qingyuan_global)
	{ 
		qingyuan_global->QingyuanEquipOperate(role, qyul->operate_type, qyul->param_1, qyul->param_2, qyul->param_3);
	}
	
}

void MessageHandler::OnRollMoneyOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRollMoneyOperaReq *rmor = (Protocol::CSRollMoneyOperaReq *)msg;
	role->GetRollMoney()->OnOperaReq(rmor->opera_type);
}

void MessageHandler::OnRoleGoalOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRoleGoalOperaReq *rgor = (Protocol::CSRoleGoalOperaReq *)msg;
	if (Protocol::CSRoleGoalOperaReq::PERSONAL_GOAL_INFO_REQ == rgor->opera_type)
	{
		role->GetRoleGoal()->SendInfoToRole();
	}
	else if (Protocol::CSRoleGoalOperaReq::FETCH_PERSONAL_GOAL_REWARD_REQ == rgor->opera_type)
	{
		role->GetRoleGoal()->FetchPersonalGoalReward();
	}
	else if (Protocol::CSRoleGoalOperaReq::FETCH_BATTLE_FIELD_GOAL_REWARD_REQ == rgor->opera_type)
	{
		role->GetRoleGoal()->FetchBattleFieldGoalReward(rgor->param);
	}
	else if (Protocol::CSRoleGoalOperaReq::FINISH_GOLE_REQ == rgor->opera_type)
	{
		role->GetRoleGoal()->CheckCond(COND_TYPE_CLIENT_NOTICE, rgor->param, 0);
	}
	else if (Protocol::CSRoleGoalOperaReq::UPLEVEL_SKILL == rgor->opera_type)
	{
		role->GetRoleGoal()->OnUplevelSkill(rgor->param);
	}
}

void MessageHandler::OnZhanshendianFetchDayRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetZhanShenDian()->FetchDayReward();
}

void MessageHandler::OnReqTrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSReqTrade *rt = (Protocol::CSReqTrade*)msg;
	P2PTradeManager::Instance().OnReqTrade(role, rt->uid);
}

void MessageHandler::OnReqTradeRet(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSReqTradeRet *rtr = (Protocol::CSReqTradeRet*)msg;

	if (0 != rtr->result)
	{
		P2PTradeManager::Instance().OnReqTradeAgree(role, rtr->req_uid);
	}
	else
	{
		UserID req_user_id = IntToUid(rtr->req_uid);

		Protocol::SCNoticeNum en;
		en.notice_num = errornum::EN_REJECT_TRADE;
		m_scene_manager->SendToUser(req_user_id, (const char *)&en, sizeof(en));
	}
}

void MessageHandler::OnTradeLock(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	P2PTradeManager::Instance().OnReqTradeLock(role);
}

void MessageHandler::OnTradeAffirm(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	P2PTradeManager::Instance().OnReqTradeAffirm(role);
}

void MessageHandler::OnTradeCancle(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	P2PTradeManager::Instance().OnReqCancel(role, "UserCancel");
}

void MessageHandler::OnTradeGold(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTradeGoldReq *tc = (Protocol::CSTradeGoldReq*)msg;
	P2PTradeManager::Instance().PutMoney(role, tc->gold, tc->coin);
}

void MessageHandler::OnTradeItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTradeItemReq *ti = (Protocol::CSTradeItemReq*)msg;
	P2PTradeManager::Instance().PutItem(role, ti->trade_index, ti->knapsack_index, ti->item_num);
}

void MessageHandler::OnTouchWorldEventObj(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSWorldEventObjTouch *weot = (Protocol::CSWorldEventObjTouch *)msg;

	Obj *obj = scene->GetObj(weot->obj_id);
	if (NULL == obj)
	{
		return;
	}

	if (Obj::OBJ_TYPE_WORLDEVENT_OBJ != obj->GetObjType())
	{
		return;
	}

	WorldEventObj *world_event_obj = (WorldEventObj*)obj;
	world_event_obj->Touch(role);
}

void MessageHandler::OnWorldEventFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleWorldEvent()->FetchReward();
}

void MessageHandler::OnWorldEventQueryRoleInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleWorldEvent()->SendInfo();
}

void MessageHandler::OnWorldEventQueryCommonInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	WorldEventManager::Instance().SendCommonInfo(role);
}

void MessageHandler::OnWorldEventLightWord(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleWorldEvent()->LightWord();
}

void MessageHandler::OnWorldEventGetNpcLocateInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSWorldEventGetNpcLocateInfo *wegnli = (Protocol::CSWorldEventGetNpcLocateInfo *)msg;
	WorldEventManager::Instance().GetNpcLocateInfo(role, wegnli->npc_id);
}

void MessageHandler::OnRedPaperCreateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRedPaperCreateReq *rpcr = (Protocol::CSRedPaperCreateReq *)msg;

	if (RED_PAPER_TYPE_COMMON != rpcr->type && RED_PAPER_TYPE_RAND != rpcr->type)
	{
		return;
	}

	RedPaperManager::Instance().OnCreateRedPaper(role, rpcr->type, rpcr->gold_num, rpcr->can_fetch_times, false, 0);
}

void MessageHandler::OnRedPaperFetchReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRedPaperFetchReq *rpfr = (Protocol::CSRedPaperFetchReq *)msg;
	RedPaperManager::Instance().OnFetchRedPaper(role, rpfr->red_paper_id);
}

void MessageHandler::OnRedPaperQueryDetailReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRedPaperQueryDetailReq *rpqdr = (Protocol::CSRedPaperQueryDetailReq *)msg;
	RedPaperManager::Instance().OnQueryRedPaperDetailInfo(role, rpqdr->red_paper_id);
}

void MessageHandler::OnCreateCommandRedPaperReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCreateCommandRedPaper *ccrp = (Protocol::CSCreateCommandRedPaper *)msg;
	RedPaperManager::Instance().OnCreateCommandRedPaper(role, msg, ccrp->msg_length);
}

void MessageHandler::OnFetchCommandRedPaperReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFetchCommandRedPaper *fcrp = (Protocol::CSFetchCommandRedPaper *)msg;
	RedPaperManager::Instance().OnFecthCommandRedPaper(role, fcrp->red_paper_id);
}

void MessageHandler::OnCommandRedPaperCheckReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCommandRedPaperCheckInfo *fcrp = (Protocol::CSCommandRedPaperCheckInfo *)msg;
	RedPaperManager::Instance().OnCheckCommandRedPaper(role, fcrp->red_paper_id);
}

void MessageHandler::OnMarriageSeekingReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMarriageSeekingOpera *msi = (Protocol::CSMarriageSeekingOpera *)msg;
	MarriageSeekingManager::Instance().MarriageSeekingOperateType(role, msi->type, msi->marriage_seeking_notice);
}

void MessageHandler::OnShizhuangUseReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShizhuangUseReq *szur = (Protocol::CSShizhuangUseReq *)msg;
	if (0 == szur->img_type)
	{
		role->GetShizhuang()->UseImg(szur->shizhuang_type, szur->img_id);
	}
	else if (1 == szur->img_type)
	{
		role->GetShizhuang()->UseSpecialImg(szur->shizhuang_type, szur->img_id);
	}
}

void MessageHandler::OnShizhuangUpgradeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShizhuangUpgradeReq *um = (Protocol::CSShizhuangUpgradeReq*)msg;

	int MAX_LOOP_TIMES = 50;
	bool need_continue_upgrade = role->GetShizhuang()->OnUpGrade(um->shizhuang_type, um->is_auto_buy > 0, um->repeat_times > MAX_LOOP_TIMES ? MAX_LOOP_TIMES : um->repeat_times);

	if (um->shizhuang_type == SHIZHUANG_TYPE_WUQI)
	{
		role->SendOperateResult(Protocol::SCOperateResult::OP_SHIZHUANG_WEAPON_UPGRADE, need_continue_upgrade ? 1 : 0);
	}

	else if (um->shizhuang_type == SHIZHUANG_TYPE_BODY)
	{
		role->SendOperateResult(Protocol::SCOperateResult::OP_SHIZHUANG_UPGRADE, need_continue_upgrade ? 1 : 0);
	}
	//role->SendOperateResult((um->shizhuang_type > 0) ? Protocol::SCOperateResult::OP_SHIZHUANG_UPGRADE : Protocol::SCOperateResult::OP_SHIZHUANG_WEAPON_UPGRADE, need_continue_upgrade ? 1 : 0);
}


void MessageHandler::OnShizhuangUpgradeSpecialImgReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShizhuangSpecialImgUpgradeReq *um = (Protocol::CSShizhuangSpecialImgUpgradeReq*)msg;

	role->GetShizhuang()->UpgradeSpecialImg(um->shizhuang_type, um->special_img_id);
}

void MessageHandler::OnCardOperate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnMonthCardFetchDayReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetMonthCard()->FetchDayReward();
}

void MessageHandler::OnCSAQueryActivityInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	CombineServerActivityManager::Instance().OnSendActivityInfo(role);
}

void MessageHandler::OnCSARoleOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCSARoleOperaReq *csaror = (Protocol::CSCSARoleOperaReq *)msg;

	role->GetRoleActivity()->OnCSAOperaReq(csaror->sub_type, csaror->param_1, csaror->param_2);
}

void MessageHandler::OnDisconnectReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	if (!role->IsForceKickDelay())
	{
		role->SetForceKickDelay();

		ForceKickUserDelay *timer = new ForceKickUserDelay(role->GetUserId(), DISCONNECT_NOTICE_TYPE_CLIENT_REQ); 
		EngineAdapter::Instance().CreateTimerSecond(1, timer);
	}
}

void MessageHandler::OnZhuanzhiReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->Zhuanzhi();
}

void MessageHandler::OnQingyuanCardUpgradeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnWushuangEquipOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnWushuangFBInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnCrossStartReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCrossStartReq *csr = (Protocol::CSCrossStartReq *)msg;

	if (CROSS_ACTIVITY_TYPE_CROSS_BOSS == csr->cross_activity_type)
	{
		role->GetRoleCross()->OnEnterCrossBossScene(csr->param, csr->sos_pos_x, csr->sos_pos_y);
	}
	else if (CROSS_ACTIVITY_TYPE_CROSS_MIZANG_BOSS == csr->cross_activity_type)
	{
		role->GetRoleCross()->OnEnterCrossMizangBossScene(csr->param, csr->sos_pos_x, csr->sos_pos_y);
	}
	else if (CROSS_ACTIVITY_TYPE_CROSS_YOUMING_BOSS == csr->cross_activity_type)
	{
		role->GetRoleCross()->OnEnterCrossYoumingBossScene(csr->param, csr->sos_pos_x, csr->sos_pos_y);
	}
	else
	{
		role->GetRoleCross()->OnStartCrossReq(csr->cross_activity_type, false, csr->param, csr->param_2, csr->param_3, csr->sos_pos_x, csr->sos_pos_y);
	}
}

void MessageHandler::OnHunshouOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnCardzuOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCardzuOperaReq *cardzu_opera = (Protocol::CSCardzuOperaReq *)msg;

	switch (cardzu_opera->opera_type)
	{
	case Protocol::CARDZU_REQ_TYPE_CHOU_CARD:
		{
			role->GetCardzu()->OnChouCardReq(cardzu_opera->param_1, cardzu_opera->param_2);
		}
		break;

	case Protocol::CARDZU_REQ_TYPE_HUALING:
		{
			role->GetCardzu()->OnHualingReq(cardzu_opera->param_1, cardzu_opera->param_2);
		}
		break;

	case Protocol::CARDZU_REQ_TYPE_LINGZHU:
		{
			role->GetCardzu()->OnLingzhuReq(cardzu_opera->param_1);
		}
		break;

	case Protocol::CARDZU_REQ_TYPE_ACTIVE_ZUHE:
		{
			role->GetCardzu()->OnActiveZuheReq(cardzu_opera->param_1);
		}
		break;

	case Protocol::CARDZU_REQ_TYPE_UPGRADE_ZUHE:
		{
			role->GetCardzu()->OnUpgradeZuheReq(cardzu_opera->param_1, cardzu_opera->param_2);
		}
		break;
	}
}

void MessageHandler::OnZhuanShengOpearReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSZhuanShengOpearReq *zhuansheng_opear = (Protocol::CSZhuanShengOpearReq *)msg;

	switch(zhuansheng_opear->opera_type)
	{
	case Protocol::ZHUANSHENG_REQ_TYPE_ALL_INFO:
		{
			role->GetZhuanShengEquip()->SendAllInfo();
		}
		break;

	case Protocol::ZHUANSHENG_REQ_TYPE_BACKPACK_INFO:
		{
			role->GetZhuanShengEquip()->SendBackpackInfo();
		}
		break;

	case Protocol::ZHUANSHENG_REQ_TYPE_OTHER_INFO:
		{
			role->GetZhuanShengEquip()->SendOtherInfo();
		}
		break;

	case Protocol::ZHUANSHENG_REQ_TYPE_UPLEVEL:
		{
			role->GetZhuanShengEquip()->OnUpZhuanShengLevel();
		}
		break;

	case Protocol::ZHUANSHENG_REQ_TYPE_CHANGE_XIUWEI:
		{
			role->GetZhuanShengEquip()->OnChangeXiuwei();
		}
		break;

	case Protocol::ZHUANSHENG_REQ_TYPE_PUT_ON_EQUIP:
		{

		}
		break;

	case Protocol::ZHUANSHENG_REQ_TYPE_TAKE_OFF_EQUIP:
		{
			role->GetZhuanShengEquip()->TakeOffEquip(zhuansheng_opear->param1);
		}
		break;

	case Protocol::ZHUANSHENG_REQ_TYPE_COMBINE_EQUIP:
		{
			role->GetZhuanShengEquip()->CombineEquip(zhuansheng_opear->param1, zhuansheng_opear->param2, zhuansheng_opear->param3, zhuansheng_opear->reserve_sh);
		}
		break;

	case Protocol::ZHUANSHENG_REQ_TYPE_CHOU_EQUIP:
		{
			role->GetZhuanShengEquip()->Chou(zhuansheng_opear->param1);
		}
		break;

	case Protocol::ZHUANSHENG_REQ_TYPE_TAKE_OUT_EQUIP:
		{
			role->GetZhuanShengEquip()->TakeOutEquip(zhuansheng_opear->param1);
		}
		break;

	case Protocol::ZHUANSHENG_REQ_TYPE_AUTO_COMBINE_EQUIP:
		{
			role->GetZhuanShengEquip()->AutoCombineEquip();
		}
		break;
	}
}

void MessageHandler::OnRuneSystemOperaReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSRuneSystemReq *rsq = (Protocol::CSRuneSystemReq*)msg;
	role->GetRuneSystem()->OperaationReq(rsq);
}

void MessageHandler::OnRuneSystemDiscopeOneKey(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSRuneSystemDisposeOneKey *pi = (Protocol::CSRuneSystemDisposeOneKey*)msg;

	if (pi->list_count <= 0 || length != static_cast<int>(UNSTD_OFFSETOFF(Protocol::CSRuneSystemDisposeOneKey, dispose_list) + sizeof(pi->dispose_list[0]) * pi->list_count)) return;

	role->GetRuneSystem()->DisposeOneKey(pi);
}

void MessageHandler::OnRuneTowerFetchTime(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	role->GetRuneSystem()->OnRuneTowerFetchTime();
}

void MessageHandler::OnRuneTowerAutoFb(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSRuneTowerAutoFb *rta = (Protocol::CSRuneTowerAutoFb *)msg;
	
	switch (rta->req_type)
	{
	case Protocol::RUNE_TOWER_FB_OPER_AUTOFB:
		{
			role->GetRuneSystem()->OnRuneTowerAutoFb();
		}
		break;

	case Protocol::RUNE_TOWER_FB_OPER_REFRESH_MONSTER:
		{
			SpecialLogic *logic = scene->GetSpecialLogic();
			if (logic != nullptr && logic->GetSceneType() == Scene::SCENE_TYPE_RUNE_TOWER)
			{
				SpecialRuneTower *rune_tower = (SpecialRuneTower *)logic;
				rune_tower->OnReqRefreshMonster();
			}
		}
		break;

	}

}

void MessageHandler::OnCrossXiuluoTowerBuyBuff(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCrossXiuluoTowerBuyBuff *cxtbb = (Protocol::CSCrossXiuluoTowerBuyBuff *)msg;
	role->GetRoleCross()->OnCrossXiuluoTowerBuyBuff(cxtbb->is_buy_realive_count > 0);
}

void MessageHandler::OnCrossXiuluoTowerScoreRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCrossXiuluoTowerScoreRewardReq *pro = (Protocol::CSCrossXiuluoTowerScoreRewardReq *)msg;

	SpecialCrossXiuluoTower *special_logic = dynamic_cast<SpecialCrossXiuluoTower*>(scene->GetSpecialLogic());
	if (NULL != special_logic && Scene::SCENE_TYPE_CROSS_XIULUO_TOWER == special_logic->GetSceneType())
	{
		special_logic->OnScoreRewardReq(role, pro->index);
	}
}

void MessageHandler::OnCrossXiuluoTowerBuyBuffReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	SpecialCrossXiuluoTower *special_logic = dynamic_cast<SpecialCrossXiuluoTower*>(scene->GetSpecialLogic());
	if (NULL != special_logic && Scene::SCENE_TYPE_CROSS_XIULUO_TOWER == special_logic->GetSceneType())
	{
		special_logic->OnBuyGatherBuff(role);
	}
}

void MessageHandler::OnCross1V1MatchReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleCross()->OnCrossMatch1v1Req();
}

void MessageHandler::OnCross1V1FightReady(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	if (NULL == role->GetScene()) return;

	SpecialCross1v1 *special_logic = dynamic_cast<SpecialCross1v1*>(role->GetScene()->GetSpecialLogic());
	if (NULL != special_logic && Scene::SCENE_TYPE_CROSS_1V1 == special_logic->GetSceneType())
	{
		special_logic->OnReady(role);
	}
}

void MessageHandler::OnCross1v1FetchRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCross1v1FetchRewardReq *pro = (Protocol::CSCross1v1FetchRewardReq *)msg;
	role->GetRoleCross()->OnCross1v1FecthRewardReq(pro->fetch_type, pro->seq);
}

void MessageHandler::OnCross1v1RingOperReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCross1v1WearRingReq *cwr = (Protocol::CSCross1v1WearRingReq *)msg;
	role->GetRoleCross()->OnCross1v1RingOper(cwr->oper_type, cwr->ring_seq);
}

void MessageHandler::OnCross1v1MatchResultReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCross1v1MatchResultReq *cmrr = (Protocol::CSCross1v1MatchResultReq *)msg;
	role->GetRoleCross()->OnCross1v1MatchResultReq(cmrr->req_type);
}

void MessageHandler::OnCross1v1BuyTimeReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	role->GetRoleCross()->Cross1v1BuyTime();
}

void MessageHandler::OnCrossMultiuserChallengeMatchingReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleCross()->OnCrossMultiuserChallengeOpera(true);
}

void MessageHandler::OnCrossMultuuserChallengeGetBaseSelfSideInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleCross()->OnCrossMultiuserChallengeOpera(false);
}

void MessageHandler::OnCrossMultiuserChallengeFetchDaycountReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCrossMultiuserChallengeFetchDaycountReward *scmd = (Protocol::CSCrossMultiuserChallengeFetchDaycountReward*)msg;
	role->GetRoleCross()->OnCrossMultiuserChallengeFetchJoinTimesReward(scmd->index);
}

void MessageHandler::OnCrossMultiuserChallengeCancelMatching(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleCross()->CancelCrossMultiuserChallengeMatching();
}

void MessageHandler::OnCrossMultiuserChallengeReqSideMemberPos(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	SpecialLogic *special_logic = scene->GetSpecialLogic();
	if (Scene::SCENE_TYPE_CROSS_MULTIUSER_CHALLENGE == special_logic->GetSceneType())
	{
		SpecialCrossMultiuserChallenge *special_cross_multiuser_challenge = dynamic_cast<SpecialCrossMultiuserChallenge*>(special_logic);
		if (NULL != special_cross_multiuser_challenge)
		{
			special_cross_multiuser_challenge->SendSideMemberPosToRole(role);
		}
	}
}

void MessageHandler::OnCrossMultiuserChallengeFetchGongxunReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCrossMultiuserChallengeFetchGongxunReward *scmd = (Protocol::CSCrossMultiuserChallengeFetchGongxunReward*)msg;
	role->GetRoleCross()->OnCrossMultiuserChallengeFetchGongxunReward(scmd->seq);
}

void MessageHandler::OnMultiuserChallengeWearCardReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSMultiuserChallengeWearCardReq *cwr = (Protocol::CSMultiuserChallengeWearCardReq *)msg;
	role->GetRoleCross()->OnCrossMultiuserChallengeCardOper(cwr->oper_type, cwr->card_seq);
}

void MessageHandler::OnCrossTuanZhanFetchRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	SpecialLogic *special_logic = scene->GetSpecialLogic();
	if (Scene::SCENE_TYPE_CROSS_TUANZHAN == special_logic->GetSceneType())
	{
		SpecialCrossTuanzhan *special_cross_tuanzhan = dynamic_cast<SpecialCrossTuanzhan*>(special_logic);
		if (NULL != special_cross_tuanzhan)
		{
			special_cross_tuanzhan->OnFetchScoreReward(role);
		}
	}
}

void MessageHandler::OnMessBattleEnterReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->OnMessBattleEnter();
}

void MessageHandler::OnBuildTowerReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSBuildTowerReq *btr = (Protocol::CSBuildTowerReq*)msg;

	if (Scene::SCENE_TYPE_BUILD_TOWER == scene->GetSpecialLogic()->GetSceneType())
	{
		SpecialBuildTower *special_build_tower = dynamic_cast<SpecialBuildTower*>(scene->GetSpecialLogic());
		if (nullptr != special_build_tower)
			special_build_tower->OnTowerOperate(btr->operate_type, btr->param1, btr->param2);
	}
}
void MessageHandler::OnBuildTowerBuyTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	FBManager::Instance().BuildTowerFbBuyTimes(role);
}

void MessageHandler::OnLieMingHunshouOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSLieMingHunshouOperaReq *hsor = (Protocol::CSLieMingHunshouOperaReq *)msg;

	switch (hsor->opera_type)
	{
	//case Protocol::LIEMING_HUNSHOU_OPERA_TYPE_CHOUHUN:
	//	{
	//		int result_color = -1;
	//		int put_idx = role->GetLieMingMgr()->Chouhun(true, Protocol::LIEMING_HUNSHOU_OPERA_TYPE_CHOUHUN, &result_color);
	//		// 有错误或者已经抽到对应颜色 则停止
	//		if (put_idx < 0 || (put_idx >= 0 && ((result_color - 1) == hsor->param_1)))
	//		{
	//			role->SendOperateResult(Protocol::SCOperateResult::OP_ONEKEY_LIEMING_CHOUHUN, 0);
	//		}
	//		// 成功且是一键抽魂 则继续
	//		else if (hsor->param_1 > 0)
	//		{
	//			role->SendOperateResult(Protocol::SCOperateResult::OP_ONEKEY_LIEMING_CHOUHUN, 1);
	//		}
	//	}
	//	break;

	//case Protocol::LIEMING_HUNSHOU_OPERA_TYPE_SUPER_CHOUHUN:
	//	{
	//		if (role->GetLieMingMgr()->SuperChouhun())
	//		{
	//			// hsor->param_1 = 0 表示普通改命 不会触发客户端的后续操作
	//			if (0 != hsor->param_1)
	//			{
	//				role->SendOperateResult(Protocol::SCOperateResult::OP_ONEKEY_LIEMING_GAIMING, 1);
	//			}
	//		}
	//		else
	//		{
	//			// hsor->param_1 = 0 表示普通改命 不会触发客户端的后续操作
	//			if (0 != hsor->param_1)
	//			{
	//				role->SendOperateResult(Protocol::SCOperateResult::OP_ONEKEY_LIEMING_GAIMING, 0);
	//			}
	//		}
	//	}
	//	break;

	//case Protocol::LIEMING_HUNSHOU_OPERA_TYPE_BATCH_HUNSHOU:
	//	{
	//		role->GetLieMingMgr()->BatchChouhun();
	//	}
	//	break;

	//case Protocol::LIEMING_HUNSHOU_OPERA_TYPE_SPECIAL_BATCH_HUNSHOU:
	//	{
	//		role->GetLieMingMgr()->SpecialBatchChouhun();
	//	}
	//	break;

	//case Protocol::LIEMING_HUNSHOU_OPERA_TYPE_PUT_BAG:
	//	{
	//		role->GetLieMingMgr()->PutHunshouBag(hsor->param_1);
	//	}
	//	break;

	//case Protocol::LIEMING_HUNSHOU_OPERA_TYPE_CONVERT_TO_EXP:
	//	{
	//		role->GetLieMingMgr()->OnChouhunPoolConverToExp();
	//	}
	//	break;

	//case Protocol::LIEMING_HUNSHOU_OPERA_TYPE_MERGE:
	//	{
	//		role->GetLieMingMgr()->OnHunshouBagMerge();
	//	}
	//	break;

	//case Protocol::LIEMING_HUNSHOU_OPERA_TYPE_SINGLE_CONVERT_TO_EXP:
	//	{
	//		if (1 == hsor->param_2)
	//		{
	//			role->GetLieMingMgr()->OnChouhunPoolSingleItemConverToExp(hsor->param_1);
	//		}
	//		else
	//		{
	//			role->GetLieMingMgr()->OnChouhunBagSingleItemConverToExp(hsor->param_1);
	//		}

	//	}
	//	break;

	//case Protocol::LIEMING_HUNSHOU_TAKEON:
	//	{
	//		role->GetLieMingMgr()->OnFuhun(hsor->param_1, hsor->param_2);
	//	}
	//	break;

	case Protocol::LIEMING_HUNSHOU_TAKEOFF:
		{
			//role->GetLieMingMgr()->OnTakeoffFuhun(hsor->param_1);
			role->GetLieMingMgr()->TakeOff(hsor->param_1);
		}
		break;

	case Protocol::LIEMING_HUNSHOU_FUHUN_ADD_EXP:
		{
			//role->GetLieMingMgr()->OnFuhunAddExp(hsor->param_1, hsor->param_2);
			role->GetLieMingMgr()->UpLevel(hsor->param_1);
		}
		break;

	case Protocol::LIEMING_HUNSHOU_OPERA_TYPE_AUTO_UPLEVEL:
		{
			role->GetLieMingMgr()->AutoUpLevel();
		}
		break;

	//case Protocol::LIEMING_HUNSHOU_OPERA_TYPE_PUT_BAG_ONE_KEY:
	//	{
	//		role->GetLieMingMgr()->OnOnekeyPutHunshouBag();
	//	}
	//	break;

	default:
		break;
	}
}

void MessageHandler::OnLieMingHunshouExchangeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
//	Protocol::CSLieMingExchangeList *lmel = (Protocol::CSLieMingExchangeList *)msg;
	//role->GetLieMingMgr()->OnBatchHunshouExchange(lmel);
}

void MessageHandler::OnJingLingOperReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSJingLingOper *cmd = (Protocol::CSJingLingOper *)msg;

	switch(cmd->oper_type)
	{
	case Protocol::JINGLING_OPER_TAKEOFF:
		role->GetJingLingMgr()->TakeOff(cmd->param1);
		break;

	case Protocol::JINGLING_OPER_CALLBACK:
		role->GetJingLingMgr()->CallBackJingLing();
		break;

	case Protocol::JINGLING_OPER_FIGHTOUT:
		role->GetJingLingMgr()->FightOutJingLing(cmd->param1);
		break;

	case Protocol::JINGLING_OPER_UPLEVEL:
		role->GetJingLingMgr()->UpLevel(cmd->param1);
		break;

	case Protocol::JINGLING_OPER_UPLEVELCARD:
		role->GetJingLingMgr()->UpLevelCard(cmd->param1);
		break;

	case Protocol::JINGLING_OPER_RENAME:
		role->GetJingLingMgr()->Rename(cmd->jingling_name);
		break;

	case Protocol::JINGLING_OPER_UPGRADE:
		break;

	case Protocol::JINGLING_OPER_STRENGTH:
		break;

	case Protocol::JINGLING_OPER_USEIMAGE:
		role->GetJingLingMgr()->UseImage(cmd->param4);
		break;

	case Protocol::JINGLING_OPER_USEPHANTOM_IMG:
		role->GetJingLingMgr()->UsePhantomImg(cmd->param4);
		break;

	case Protocol::JINGLING_OPER_PHANTOMUPLEVEL:
		role->GetJingLingMgr()->OnUpLevelPhantom(cmd->param4);
		break;

	case Protocol::JINGLING_OPER_UPGRADESOUL:
		break;

	case Protocol::JINGLING_OPER_UPGRADE_HALO:
		break;

	case Protocol::JINGLING_OPER_USE_HALO_IMG:
		break;

	case Protocol::JINGLING_OPER_ONEKEY_RECYCL_BAG:
		role->GetKnapsack()->OneKeyDiscardJiling(cmd->param1);
		break;

	case Protocol::JINGLING_OPER_UPLEVEL_WUXING:
		role->GetJingLingMgr()->OnUpLevelWuXing(cmd->param1, 0 != cmd->param2, 0 != cmd->param3);
		break;

	case Protocol::JINGLING_OPER_UPLEVEL_XIANZHEN:
		role->GetJingLingMgr()->OnUpLevelXianZhen(0 != cmd->param1);
		break;

	case Protocol::JINGLING_OPER_UPLEVEL_HUNYU:
		role->GetJingLingMgr()->OnUpLevelHunYu(cmd->param1);
		break;

	case Protocol::JINGLING_OPER_REMOVE_SKILL:
		role->GetJingLingMgr()->OnSkillRemove(cmd->param1, cmd->param2, 0 != cmd->param3);
		break;

	case Protocol::JINGLING_OPER_CHANGE_MOVE:
		role->GetJingLingMgr()->OnSkillChangeMove(cmd->param1, cmd->param2, 0 != cmd->param3);
		break;

	case Protocol::JINGLING_OPER_PUT_ON_SKILL:
		role->GetJingLingMgr()->OnSkillPutOn(cmd->param1, cmd->param2, cmd->param3);
		break;

	case Protocol::JINGLING_OPER_TAKE_OFF_SKILL:
		role->GetJingLingMgr()->OnSkillTakeOff(cmd->param1, cmd->param2, cmd->param3);
		break;

	case Protocol::JINGLING_OPER_LEARN_SKILL:
		role->GetJingLingMgr()->OnSkillLearn(cmd->param1, cmd->param2, cmd->param3);
		break;

	case Protocol::JINGLING_OPER_REFRESH:
		role->GetJingLingMgr()->OnSkillRefresh(cmd->param1, 0 != cmd->param2, 0 != cmd->param3);
		break;

	case Protocol::JINGLING_OPER_GET:
		role->GetJingLingMgr()->OnSkillGet(cmd->param1, cmd->param2);
		break;

	case Protocol::JINGLING_OPER_REFRESH_ACTIVE:
		//role->GetJingLingMgr()->OnSkillRefreshActive(cmd->param1);
		break;

	case Protocol::JINGLING_OPER_OPEN_SKILL_SLOT:
		role->GetJingLingMgr()->OnOpenSkillSlot(cmd->param1, cmd->param2);
		break;
	}
}

void MessageHandler::OnJingLingHomeOperReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSJingLingHomeOperReq *cmd = (Protocol::CSJingLingHomeOperReq *)msg;
	JingLingHome *jingling_home = NULL;

	switch (cmd->oper_type)
	{
	case Protocol::JING_LING_HOME_OPER_TYPE_GET_INFO:
		jingling_home = GlobalRoleManager::Instance().GeJingLingHome(cmd->role_id);
		if (NULL != jingling_home)
		{
			jingling_home->OnInfoReq(role);
		}
		break;

	case Protocol::JING_LING_HOME_OPER_TYPE_PUT_HOME:
		jingling_home = GlobalRoleManager::Instance().GeJingLingHome(role->GetUID());
		if (NULL != jingling_home)
		{
			jingling_home->OnPutJingLing(role, cmd->param1, cmd->param2);
		}
		break;

	case Protocol::JING_LING_HOME_OPER_TYPE_QUICK:
		jingling_home = GlobalRoleManager::Instance().GeJingLingHome(role->GetUID());
		if (NULL != jingling_home)
		{
			jingling_home->OnQuick(role, cmd->param1);
		}
		break;

	case Protocol::JING_LING_HOME_OPER_TYPE_GET_REWARD:
		jingling_home = GlobalRoleManager::Instance().GeJingLingHome(role->GetUID());
		if (NULL != jingling_home)
		{
			jingling_home->OnGetReward(role, cmd->param1);
		}
		break;

	case Protocol::JING_LING_HOME_OPER_TYPE_ROB:
		jingling_home = GlobalRoleManager::Instance().GeJingLingHome(cmd->role_id);
		if (NULL != jingling_home)
		{
			jingling_home->OnBeRob(role, cmd->param1, cmd->param2);
		}
		break;

	case Protocol::JING_LING_HOME_OPER_TYPE_OUT:
		jingling_home = GlobalRoleManager::Instance().GeJingLingHome(role->GetUID());
		if (NULL != jingling_home)
		{
			jingling_home->OnTakeOutJingLing(role, cmd->param1);
		}
		break;

	case Protocol::JING_LING_HOME_OPER_TYPE_REFRESH_LIST:
		jingling_home = GlobalRoleManager::Instance().GeJingLingHome(role->GetUID());
		if (NULL != jingling_home)
		{
			jingling_home->OnRefreshList(role);
		}
		break;

	case Protocol::JING_LING_HOME_OPER_TYPE_READ_ROB_RECORD:
		jingling_home = GlobalRoleManager::Instance().GeJingLingHome(role->GetUID());
		if (NULL != jingling_home)
		{
			jingling_home->OnReadRobRecord(role);
		}
		break;
	}
}

void MessageHandler::OnJinglIngExploreOperReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
// 	Protocol::CSJinglIngExploreOperReq *cmd = (Protocol::CSJinglIngExploreOperReq *)msg;
// 	switch (cmd->oper_type)
// 	{
// 	case Protocol::JL_EXPLORE_OPER_TYPE_SELECT_MODE:
// 		role->GetJingLingMgr()->OnExploreSelectMode(cmd->param1);
// 		break;
// 
// 	case Protocol::JL_EXPLORE_OPER_TYPE_EXPLORE:
// 		role->GetJingLingMgr()->OnExploreChallenge();
// 		break;
// 
// 	case Protocol::JL_EXPLORE_OPER_TYPE_FETCH:
// 		role->GetJingLingMgr()->OnExploreFetchReward(cmd->param1, 0 != cmd->param2);
// 		break;
// 
// 	case Protocol::JL_EXPLORE_OPER_TYPE_RESET:
// 		role->GetJingLingMgr()->OnExploreReset();
// 		break;
// 
// 	case Protocol::JL_EXPLORE_OPER_TYPE_BUY_BUFF:
// 		role->GetJingLingMgr()->OnExploreBuyBuff();
// 		break;
// 	}
}

void MessageHandler::OnShengWangOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShengWangOpera *cmd = (Protocol::CSShengWangOpera *)msg;

	if (Protocol::SHENGWANG_OPERA_REQ_INFO == cmd->opera_type)
	{
		role->GetShengWangMgr()->SyncShengWangInfo();
	}

	else if (Protocol::SHENGWANG_OPERA_XIANJIE_UPLEVEL == cmd->opera_type)
	{
		role->GetShengWangMgr()->UpXianjieLevel();
	}

	else if (Protocol::SHENGWANG_OPERA_XIANDAN_UPLEVEL == cmd->opera_type)
	{
		role->GetShengWangMgr()->UpXiandanLevel();
	}
}

void MessageHandler::OnChengJiuOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSChengJiuOpera *cmd = (Protocol::CSChengJiuOpera *)msg;

	if (Protocol::CHENGJIU_REQ_INFO == cmd->opera_type)
	{
		role->GetChengJiuMgr()->SyncChengJiuInfo();
	}

	else if (Protocol::CHENGJIU_OPER_TITLE_UPLEVEL == cmd->opera_type)
	{
		role->GetChengJiuMgr()->UpTitleLevel();
	}

	else if (Protocol::CHENGJIU_OPER_FETCH_REWARD == cmd->opera_type)
	{
		role->GetChengJiuMgr()->FetchReward(cmd->param1);
	}

	else if (Protocol::CHENGJIU_OPER_FUWEN_UPLEVEL == cmd->opera_type)
	{
		role->GetChengJiuMgr()->UpFuwenLevel();
	}
}

void MessageHandler::OnShenzhouWeaponOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSHenzhouWeaponOperaReq *opera = (Protocol::CSSHenzhouWeaponOperaReq *)msg;

	switch (opera->opera_type)
	{
	case Protocol::SHENZHOU_REQ_TYPE_ALL_INFO_REQ:
	{
		role->GetShenzhouWeapon()->SendBaseInfo();
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_BUY_GATHER_TIME:
	{
		role->GetShenzhouWeapon()->BuyGatherTimes();
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_EXCHANGE_IDENTIFY_EXP:
	{
		role->GetShenzhouWeapon()->ExchangeIdentifyExp();
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_INDENTIFY:
	{
		role->GetShenzhouWeapon()->IdentifyBatchItem(opera->param_1, opera->param_2);
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_UPLEVEL_WEAPON_SLOT:
	{
		role->GetShenzhouWeapon()->SlotUpLevel(opera->param_1, opera->param_2);
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_GATHER_INFO_REQ:
	{
		role->GetShenzhouWeapon()->SendGatherInfo();
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_HELP_OTHER_BOX:
	{
		role->GetShenzhouWeapon()->HelpOtherBox(opera->param_1);
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_OPEN_BOX:
	{
		role->GetShenzhouWeapon()->OpenBox(opera->param_1);
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_BOX_INFO:
	{
		role->GetShenzhouWeapon()->SendBoxInfo();
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_PUT_BOX:
	{
		role->GetShenzhouWeapon()->PutBox();
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_UPLEVEL_ELEMENT:
	{
		role->GetShenzhouWeapon()->ElementUpLevel(opera->param_1, opera->param_2);
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_UPLEVEL_LINGSHU:
	{
		role->GetShenzhouWeapon()->LingshuUplevel(opera->param_1, opera->param_2);
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_HUNYIN_INLAY:
	{
		role->GetShenzhouWeapon()->HunyinInlay(opera->param_1, opera->param_2, opera->param_3);
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_INVITE_HELP_OTHER_BOX:
	{
		role->GetShenzhouWeapon()->InviteHelpOtherBox();
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_REMOVE_HELP_BOX:
	{
		role->GetShenzhouWeapon()->RemoveBoxHelp();
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_XILIAN_OPEN_SLOT:
	{
		role->GetShenzhouWeapon()->OpenXilianSlot(opera->param_1, opera->param_2);
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_XILIAN_REQ:
	{
		role->GetShenzhouWeapon()->XilianReq(opera->param_1, opera->param_2, opera->param_3, 0 != opera->param_4, 0 != opera->param_5);
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_OPEN_HUNYIN_SLOT:
	{
		role->GetShenzhouWeapon()->OpenHunYinSlot(opera->param_1, opera->param_2);
	}
	break;

	case Protocol::SHENZHOU_REQ_TYPE_AUTO_UPLEVEL_LINGSHU:
	{
		role->GetShenzhouWeapon()->AutoLingshuUplevel(opera->param_1);
	}
	break;

	}
}

void MessageHandler::OnShenzhouWeaponOneIdentifyReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetShenzhouWeapon()->OneKeyIdentifyItem();
}

void MessageHandler::OnShenzhouWeaponHunyinResolveReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShenzhouHunyinResolveReq *opera = (Protocol::CSShenzhouHunyinResolveReq *)msg;

	role->GetShenzhouWeapon()->HunyinResolve(opera);
}

void MessageHandler::OnUseTuHaoJinReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUseTuHaoJinReq *opera = (Protocol::CSUseTuHaoJinReq *)msg;

	role->GetTuHaoJin()->OnUseTuhaoJin(opera->use_tuhaojin_color);
}

void MessageHandler::OnTuHaoJinUpLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetTuHaoJin()->OnTuhaojinUpLevel();
}

void MessageHandler::OnBigFaceUpLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetBigChatFace()->OnBigFaceUpLevel();
}

void MessageHandler::OnBabyOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSBabyOperaReq *baby_opear = (Protocol::CSBabyOperaReq *)msg;

	switch(baby_opear->opera_type)
	{
	case Protocol::BABY_REQ_TYPE_INFO:
		{
			role->GetBaby()->SendBabyInfo((short)baby_opear->param_1);
		}
		break;

	case Protocol::BABY_REQ_TYPE_ALL_INFO:
		{
			role->GetBaby()->SendAllBabyInfo();
		}
		break;

	case Protocol::BABY_REQ_TYPE_UPLEVEL:
		{
			role->GetBaby()->UplevelBaby((short)baby_opear->param_1);
			role->GetBaby()->CheckSyncCache();
		}
		break;

	case Protocol::BABY_REQ_TYPE_QIFU:
		{
			role->GetBaby()->RequestCreateBaby(baby_opear->param_1);
		}
		break;

	case Protocol::BABY_REQ_TYPE_QIFU_RET:
		{
			role->GetBaby()->ReplyCreateBabyRequest(baby_opear->param_1, 0 != baby_opear->param_2);
			role->GetBaby()->CheckSyncCache();
		}
		break;

	case Protocol::BABY_REQ_TYPE_CHAOSHENG:
		{
			role->GetBaby()->RequestChaosheng();
		}
		break;

	case Protocol::BABY_REQ_TYPE_SPIRIT_INFO:
		{
			role->GetBaby()->SendBabySpiritInfo(baby_opear->param_1);
		}
		break;

	case Protocol::BABY_REQ_TYPE_TRAIN_SPIRIT:
		{
		for (int i = 0; i < baby_opear->param_0; ++i)
		{
			int old_spirit_level = role->GetBaby()->GetSpiritLevel(baby_opear->param_1, baby_opear->param_2);
			bool ret = role->GetBaby()->TrainBabySpirit(baby_opear->param_1, baby_opear->param_2, (0 != baby_opear->param_3));
			if (!ret || old_spirit_level != role->GetBaby()->GetSpiritLevel(baby_opear->param_1, baby_opear->param_2))
			{
				role->SendOperateResult(Protocol::SCOperateResult::OP_BABY_JL_UPGRADE, 0);
				role->GetBaby()->CheckSyncCache();
				return;
			}
		}

		role->SendOperateResult(Protocol::SCOperateResult::OP_BABY_JL_UPGRADE, 1);
		}
		break;

	case Protocol::BABY_REQ_TYPE_REMOVE_BABY:
		{
			role->GetBaby()->OnRemoveBabyReq(baby_opear->param_1);
		}
		break;

	case Protocol::BABY_REQ_TYPE_REMOVE_BABY_RET:
		{
			role->GetBaby()->OnRemoveBabyRet(baby_opear->param_1, baby_opear->param_4, 1 == baby_opear->param_2);
		}
		break;

	case Protocol::BABY_REQ_TYPE_WALK:
		{
			role->GetBaby()->OnBabyWalk(baby_opear->param_1);
		}
		break;
	}
}

void MessageHandler::OnBabyUpgradeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSBabyUpgradeReq *baby_upgrade = (Protocol::CSBabyUpgradeReq*)msg;

	int lover_id = role->GetLoverUid();
	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(lover_id));

	for (int i = 0; i < baby_upgrade->repeat_times; ++ i)
	{
		int old_grade = role->GetBaby()->GetBabyGrade(baby_upgrade->baby_index);

		bool upgrade_opera_succ = role->GetBaby()->UpgradeBaby(baby_upgrade->baby_index, 1 == baby_upgrade->auto_buy);
		if (!upgrade_opera_succ || old_grade != role->GetBaby()->GetBabyGrade(baby_upgrade->baby_index))
		{
			role->SendOperateResult(Protocol::SCOperateResult::OP_BABY_JIE_UPGRADE, 0);
			if (NULL != target_role && baby_upgrade->is_auto_upgrade)		// 自动进阶按钮屏蔽
			{
				target_role->SendOperateResult(Protocol::SCOperateResult::OP_BABY_JIE_UPGRADE, 3);
			}

			role->GetBaby()->CheckSyncCache();
			return;
		}
	}

	if (baby_upgrade->repeat_times > 0)
	{
		role->SendOperateResult(Protocol::SCOperateResult::OP_BABY_JIE_UPGRADE, 1);

		if (NULL != target_role && baby_upgrade->is_auto_upgrade)
		{
			target_role->SendOperateResult(Protocol::SCOperateResult::OP_BABY_JIE_UPGRADE, 2);
		}

		role->GetBaby()->CheckSyncCache();
	}
}

void MessageHandler::OnBabyRenameReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSBabyRenameReq *rename_baby = (Protocol::CSBabyRenameReq *)msg;

	role->GetBaby()->ChangeBabyName(rename_baby->baby_index, rename_baby->newname);
	role->GetBaby()->CheckSyncCache();
}

void MessageHandler::OnRATotalCharge3RequestInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->OnSendTotalCharge3Info();
}


void MessageHandler::OnPetOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSPetOperaReq *pet_opear = (Protocol::CSPetOperaReq *)msg;

	switch(pet_opear->opera_type)
	{
	case Protocol::PET_REQ_TYPE_INFO:
		{
			role->GetPet()->SendPetInfo();
		}
		break;

	case Protocol::PET_REQ_TYPE_BACKPACK_INFO:
		{
			role->GetPet()->SendPetBackpackList();
		}
		break;

	case Protocol::PET_REQ_TYPE_SELECT_PET:
		{
			role->GetPet()->SelectPet(pet_opear->param_1);
		}
		break;

	case Protocol::PET_REQ_TYPE_CHANGE_NAME:
		{
			role->GetPet()->ChangeCurPetName(pet_opear->newname);
		}
		break;

	case Protocol::PET_REQ_TYPE_UP_LEVEL:
		{
			role->GetPet()->UplevelCurPet();
		}
		break;

	case Protocol::PET_REQ_TYPE_CHOU:
		{
			role->GetPet()->ChouPet(pet_opear->param_1, true);		//不允许使用绑元
		}
		break;

	case Protocol::PET_REQ_TYPE_RECYCLE_EGG:
		{
			role->GetPet()->RecycleEgg(pet_opear->param_1);
		}
		break;

	case Protocol::PET_REQ_TYPE_PUT_REWARD_TO_KNAPSACK:
		{
			role->GetPet()->TransportRewardToKnapsack(pet_opear->param_1, pet_opear->param_2);
		}
		break;

	case Protocol::PET_REQ_TYPE_ACTIVE:
		{
			role->GetPet()->ActivePet(pet_opear->param_1);
		}
		break;

	case Protocol::PET_REQ_TYPE_LEARN_SKILL:
		{
			role->GetPet()->LearnSkill(pet_opear->param_1, pet_opear->param_2);
		}
		break;

	case Protocol::PET_REQ_TYPE_UPGRADE_SKILL:
		{
			role->GetPet()->UpgradeSkill(pet_opear->param_1);
		}
		break;

	case Protocol::PET_REQ_TYPE_FORGET_SKILL:
		{
			role->GetPet()->ForgetSkill(pet_opear->param_1);
		}
		break;

	case Protocol::PET_REQ_TYPE_QINMI_PROMOTE:
		{
			int food_index = pet_opear->param_1;			// 传食物index
			int repeat_times = 1;
			int old_grade = role->GetPet()->GetCurPetQinmiLevel();

			for (int i = 0; i < repeat_times; ++ i)
			{
				bool upgrade_succ = role->GetPet()->PetQinmiPromote(food_index, pet_opear->param_2);
				if (!upgrade_succ || old_grade != role->GetPet()->GetCurPetQinmiLevel())
				{
					role->SendOperateResult(Protocol::SCOperateResult::OP_PET_QINMI_PROMOTE, 0);
					break;
				}

				role->SendOperateResult(Protocol::SCOperateResult::OP_PET_QINMI_PROMOTE, 1);
			}
		}
		break;

	case Protocol::PET_REQ_TYPE_QINMI_AUTO_PROMOTE:
		{
			role->GetPet()->PetQinmiAutoPromote(pet_opear->param_1);
		}
		break;

	case Protocol::PET_REQ_TYPE_FOOD_MARKET_CHOU_ONCE:
		{
			role->GetPet()->FoodMarketChouOnce(0 != pet_opear->param_1);
		}
		break;

	case Protocol::PET_REQ_TYPE_FOOD_MARKET_CHOU_TIMES:
		{
			role->GetPet()->FoodMarketChouTimes(pet_opear->param_1, false);
		}
		break;

	case Protocol::PET_REQ_TYPE_UPLEVL_SPECIAL_IMG:
		{
			role->GetPet()->UplevelSpecialImage(pet_opear->param_1);
		}
		break;
	}
}

void MessageHandler::OnPetUpgradeReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSPetUpgradeReq *petupgrade = (Protocol::CSPetUpgradeReq*)msg;
	for (int i = 0; i < petupgrade->repeat_times; ++ i)
	{
		int old_grade = role->GetPet()->GetCurPetGrade();

		bool upgrade_opera_succ = role->GetPet()->UpgradeCurPet(1 == petupgrade->auto_buy);
		if (!upgrade_opera_succ || old_grade != role->GetPet()->GetCurPetGrade())
		{
			role->SendOperateResult(Protocol::SCOperateResult::OP_PET_JIE_UPGRADE, 0);
			break;
		}
	}

	if (petupgrade->repeat_times > 0)
	{
		role->SendOperateResult(Protocol::SCOperateResult::OP_PET_JIE_UPGRADE, 1);
	}
}

void MessageHandler::OnPetUseSpecialImg(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSPetUseSpecialImg *pusi = (Protocol::CSPetUseSpecialImg*)msg;
	role->GetPet()->UseImage(pusi->special_img_id);
}

void MessageHandler::OnRATotalCharge4RequestInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->OnSendTotalCharge4Info();
}

void MessageHandler::OnHuaShenOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSHuaShenOperaReq *huashen_opera = (Protocol::CSHuaShenOperaReq *)msg;

	switch(huashen_opera->req_type)
	{
	case Protocol::HUASHEN_REQ_TYPE_ALL_INFO:
		{
			role->GetHuaShen()->SendHuaShenAllInfo();
		}
		break;

	case Protocol::HUASHEN_REQ_TYPE_CHANGE_IMAGE:
		{
			role->GetHuaShen()->ChangeImage(huashen_opera->param_1);
		}
		break;

	case Protocol::HUASHEN_REQ_TYPE_UP_LEVEL:
		{
			int id = huashen_opera->param_1;
			bool is_auto_by = (0 != huashen_opera->param_2);
			int repeat_times = huashen_opera->param_3;
			int old_grade = role->GetHuaShen()->GetLevel(id);

			for (int i = 0; i < repeat_times; ++ i)
			{
				bool upgrade_succ = role->GetHuaShen()->UpLevel(id, is_auto_by);
				if (!upgrade_succ || old_grade != role->GetHuaShen()->GetLevel(id))
				{
					role->SendOperateResult(Protocol::SCOperateResult::OP_HUASHEN_UPLEVEL, 0);
					break;
				}

				role->SendOperateResult(Protocol::SCOperateResult::OP_HUASHEN_UPLEVEL, 1);
			}
		}
		break;

	case Protocol::HUASHEN_REQ_TYPE_SPIRIT_INFO:
		{
			role->GetHuaShen()->SendHuaShenSpiritInfo(huashen_opera->param_1);
		}
		break;

	case Protocol::HUASHEN_REQ_TYPE_UPGRADE_SPIRIT:
		{
			int old_level = role->GetHuaShen()->GetSpiritLevel(huashen_opera->param_1, huashen_opera->param_2);
			bool is_succ = role->GetHuaShen()->OnUpgradeSpirit(huashen_opera->param_1, huashen_opera->param_2, 0 != huashen_opera->param_3);
			if (!is_succ || old_level != role->GetHuaShen()->GetSpiritLevel(huashen_opera->param_1, huashen_opera->param_2))
			{
				role->SendOperateResult(Protocol::SCOperateResult::OP_HUASHEN_UPGRADE_SPIRIT, 0);
			}
			else
			{
				role->SendOperateResult(Protocol::SCOperateResult::OP_HUASHEN_UPGRADE_SPIRIT, 1);
			}
		}
		break;

	case Protocol::HUASHEN_REQ_TYPE_UP_GRADE:
		{
			role->GetHuaShen()->UpGrade(huashen_opera->param_1, 0 != huashen_opera->param_2);
		}
		break;
	}
}

void MessageHandler::OnQingyuanCoupleHaloOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSQingyuanCoupleHaloOperaReq *chor = (Protocol::CSQingyuanCoupleHaloOperaReq *)msg;

	switch(chor->req_type)
	{
	case Protocol::QINGYUAN_COUPLE_REQ_TYPE_INFO:
		{
			role->GetQingyuan()->SendCoupleHaloInfo();
		}
		break;

	case Protocol::QINGYUAN_COUPLE_REQ_TYPE_USE:
		{
			role->GetQingyuan()->CoupleHaloUse(chor->param_1);
		}
		break;

	case Protocol::QINGYUAN_COUPLE_REQ_TYPE_UP_LEVEL:
		{
			role->GetQingyuan()->UplevelCoupleHalo(chor->param_1, 1 == chor->param_2);
		}
		break;

	case Protocol::QINGYUAN_COUPLE_REQ_TYPE_BUY_HALO:
		{
			role->GetQingyuan()->OnBuyHalo(chor->param_1);
		}
		break;
	}
}

void MessageHandler::OnQingyuanFetchBlessRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetQingyuan()->FetchBlessReward();
}

void MessageHandler::OnQingyuanAddBlessDays(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetQingyuan()->AddBlessDays();
}

void MessageHandler::OnLoveContractOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSLoveContractOperaReq *pro = (Protocol::CSLoveContractOperaReq *)msg;

	LoveContract * love_contract = GLOBAL_ROLE_MGR.GetLoveContract(role->GetUID());
	if (NULL == love_contract) return;

	switch (pro->opera_type)
	{
	case Protocol::LC_REQ_TYPE_INFO:
		{
			love_contract->SendLoveContractInfo();
		}
		break;

	case Protocol::LC_REQ_TYPE_BUY_LOVE_CONTRACT:
		{
			love_contract->BuyLoveContractReq(role);
		}
		break;

	case Protocol::LC_REQ_TYPE_NOTICE_LOVER_BUY_CONTRACT:
		{
			love_contract->NoticeLoverBuyLoveContract(role);
		}
		break;
	}
}

void MessageHandler::OnQingyuanFetchLoveContract(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSQingyuanFetchLoveContract *pro = (Protocol::CSQingyuanFetchLoveContract *)msg;
	LoveContract * love_contract = GLOBAL_ROLE_MGR.GetLoveContract(role->GetUID());
	if (NULL == love_contract) return;

	switch (pro->opera_type)
	{

	case Protocol::LC_NOTICE_TYPE_FETCH:
		{
			love_contract->FetchLoveContractReward(role, pro->day_num, pro->love_contract_notice);
		}
		break;

	case Protocol::LC_NOTICE_TYPE_NOTICE:
		{
			love_contract->OnSendNotice(pro->love_contract_notice);
		}
		break;

	default:
		break;
	}
}

void MessageHandler::OnPastureSpiritOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{

}

void MessageHandler::OnMultiMountOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMultiMountOperaReq *mmor = (Protocol::CSMultiMountOperaReq *)msg;
	role->GetMultiMount()->OnMountReq(mmor->opera_type, mmor->param_1, mmor->param_2, mmor->param_3);
}

void MessageHandler::OnPersonalizeWindowOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSPersonalizeWindowOperaReq *pwor = (Protocol::CSPersonalizeWindowOperaReq *)msg;

	switch (pwor->req_type)
	{
	case Protocol::PERSONALIZE_WINDOW_BUBBLE_INFO:
		{
			role->GetPersonalizeWindow()->SendBubbleWindowInfo();
		}
		break;

	case Protocol::PERSONALIZE_WINDOW_BUBBLE_UP_LEVEL:
		{
			role->GetPersonalizeWindow()->OnBubbleWindowUpLevel(pwor->param_1);
		}
		break;

	case Protocol::PERSONALIZE_WINDOW_BUBBLE_USE:
		{
			role->GetPersonalizeWindow()->OnUseBubbleWindwo(pwor->param_1);
		}
		break;

	case Protocol::PERSONALIZE_WINDOW_AVATAR_INFO:
		{
			role->GetPersonalizeWindow()->SendAvatarWindowInfo();
	}
		break;

	case Protocol::PERSONALIZE_WINDOW_AVATAR_UP_LEVEL:
		{
			role->GetPersonalizeWindow()->OnAvatarWindowUpLevel(pwor->param_1);
		}
		break;

	case Protocol::PERSONALIZE_WINDOW_AVATAR_USE:
		{
			role->GetPersonalizeWindow()->OnUseAvatarWindow(pwor->param_1);
		}
		break;
	}
}

void MessageHandler::OnMagicCardOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMagicCardOperaReq *mcor = (Protocol::CSMagicCardOperaReq *)msg;

	switch(mcor->opera_type)
	{
	case Protocol::MAGIC_CARD_REQ_TYPE_ALL_INFO:
		{
			role->GetMagicCard()->SendAllInfo();
		}
		break;

	case Protocol::MAGIC_CARD_REQ_TYPE_CHOU_CARD:
		{
			role->GetMagicCard()->ChouCard(mcor->param_1, mcor->param_2);
		}
		break;

	case Protocol::MAGIC_CARD_REQ_TYPE_USE_CARD:
		{
			role->GetMagicCard()->UseCard(mcor->param_1);
		}
		break;

	case Protocol::MAGIC_CARD_REQ_TYPE_UPGRADE_CARD:
		{
			role->GetMagicCard()->UpgradeCard(mcor->param_1, mcor->param_2, mcor->param_3, mcor->param_4);
		}
		break;

	case Protocol::MAGIC_CARD_REQ_TYPE_EXCHANGE:
		{
			role->GetMagicCard()->ExchangeCard(mcor->param_1);
		}
		break;

		//case Protocol::MAGIC_CARD_REQ_TYPE_SKILL_ACTIVE:
		//	{
		//		role->GetMagicCard()->ActiveSkill(mcor->param_1);
		//	}
		//	break;
	}
}

void MessageHandler::OnWushangEquipOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnCrossBossBuyReliveTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCrossBossBuyReliveTimes *cbbrt = (Protocol::CSCrossBossBuyReliveTimes*)msg;
	role->GetRoleCross()->OnCrossBossBuyReliveTimes(cbbrt->buy_times);
}

void MessageHandler::OnCrossBossBossInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCrossBossBossInfoReq *cscbir = (Protocol::CSCrossBossBossInfoReq *)msg;
	role->GetRoleCross()->OnCrossBossReq(cscbir->req_type, cscbir->param_1, cscbir->param_2);
}

void MessageHandler::OnWingFuMoUplevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	// 	Protocol::CSWingFumoUplevel *fumo = (Protocol::CSWingFumoUplevel*)msg;
	// 	role->GetWing()->OnFumoUplevel(fumo->type, (0 != fumo->is_auto_buy));
}

void MessageHandler::OnMitamaOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMitamaOperaReq *mor = (Protocol::CSMitamaOperaReq *)msg; 

	role->GetMitama()->CheckTaskStatus();

	switch(mor->req_type)
	{
	case Protocol::MITAMA_REQ_TYPE_ALL_INFO:
		{
			role->GetMitama()->SendAllInfo();
		}
		break;

	case Protocol::MITAMA_REQ_TYPE_UPGRADE:
		{
			role->GetMitama()->OnUpgrade(mor->param_1, mor->param_2, 1 == mor->param_3);
		}
		break;

	case Protocol::MITAMA_REQ_TYPE_TASK_FIGHTING:
		{
			role->GetMitama()->OnTaskFighting(mor->param_1, mor->param_2);
		}
		break;

	case Protocol::MITAMA_REQ_TYPE_TASK_AWARD:
		{
			role->GetMitama()->OnTaskFetchReward(mor->param_1);
		}
		break;

	case Protocol::MITAMA_REQ_TYPE_EXCHANGE_ITEM:
		{
			role->GetMitama()->OnExchangeItem(mor->param_1, mor->param_2);
		}
		break;
	}
}

void MessageHandler::OnFairyTreeOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnMagicEquipmentOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
}

void MessageHandler::OnXunZhangUpLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSXunZhangUplevelReq *pro = (Protocol::CSXunZhangUplevelReq*)msg;
	role->GetXunZhangManager()->OnUplevelReq(pro->xunzhang_id, pro->is_only_bind);
}

void MessageHandler::OnZhibaoUplevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetZhiBaoManager()->UplevelZhiBao();
}

void MessageHandler::OnZhibaoUseImage(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSZhiBaoUseImage *pro = (Protocol::CSZhiBaoUseImage*)msg;
	role->GetZhiBaoManager()->UseImage(pro->use_image);
}

void MessageHandler::OnZhibaoHuanhua(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSZhiBaoHuanhua *pro = (Protocol::CSZhiBaoHuanhua*)msg;
	if (Protocol::CSZhiBaoHuanhua::OT_ACTIVE_AND_UPGRADE == pro->operation)
	{
		role->GetZhiBaoManager()->ActiveOrUpgradeHuanhua(pro->type);
	}

	if (Protocol::CSZhiBaoHuanhua::OT_USE_HUANHUA_XINGXIANG == pro->operation)
	{
		role->GetZhiBaoManager()->UseHuanhuaType(pro->type);
	}
}

void MessageHandler::OnCrossTeamFBOptionReq(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSCrossTeamFBOption *ctfbor = (Protocol::CSCrossTeamFBOption *)msg;
	if (NULL != role)
	{
		if (role->GetHusongTask()->IsHusongStatus())
		{
			role->NoticeNum(errornum::EN_CROSS_TEAM_HUSONG_LIMIT);
			return;
		}
		role->GetRoleCross()->OnCrossTeamFBOption(ctfbor->option_type, ctfbor->layer, ctfbor->room, ctfbor->param, ctfbor->param2, ctfbor->param3);
	}
}

void MessageHandler::OnShenshouOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShenshouOperaReq *sor = (Protocol::CSShenshouOperaReq*)msg;
	role->GetShenshou()->OnHandlerOpera(sor->opera_type, sor->param_1, sor->param_2, sor->param_3, sor->param_4);
}

void MessageHandler::OnShenshouReqStrength(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSHenshouReqStrength *ssrs = (Protocol::CSSHenshouReqStrength *)msg;

	// 根据实际数据量修正真正长度，防止客户端恶意发送数据
	int arr_len = length - (sizeof(Protocol::CSSHenshouReqStrength) - sizeof(ssrs->destroy_backpack_index_list));
	ssrs->destroy_num = short(arr_len / sizeof(ssrs->destroy_backpack_index_list[0]));

	if (ssrs->destroy_num > 0)
	{
		role->GetShenshou()->UpgradeEquip(ssrs->shenshou_id, ssrs->equip_index, ssrs->destroy_num, ssrs->destroy_backpack_index_list, ssrs->is_use_double_shuliandu > 0);
	}
}

//===============================================================
//杂碎简单的协议可以用下面通用的。
//===============================================================
void MessageHandler::OnCommonOperateReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSReqCommonOpreate *cmd = (Protocol::CSReqCommonOpreate *)msg;
	switch (cmd->operate_type)
	{
	case Protocol::COT_JINGHUA_HUSONG_COMMIT:
	{
		JingHuaHuSongMgr::Instance().OnCommit(role);
	}
	break;

	case Protocol::COT_JINGHUA_HUSONG_INFO_REQ:
	{
		JingHuaHuSongMgr::Instance().SyncRoleHusongInfo(role);
	}
	break;

	case Protocol::COT_KEY_ADD_FRIEND:
	{
		SpaceManager::Instance().OnKeyCaiAllFriend(role);
	}
	break;

	case Protocol::COT_ACT_BUY_EQUIPMENT_GIFT:
	{
		role->GetRoleActivity()->OnOGABuyEquipmentGift(cmd->param1);
	}
	break;

	case Protocol::COT_REQ_RED_EQUIP_COLLECT_TAKEON:
	{
		reqhandler_red_equip_collect::TakeOn().OnReq(role, msg);
	}
	break;

	case Protocol::COT_REQ_RED_EQUIP_COLLECT_FETCH_ATC_REWARD:
	{
		reqhandler_red_equip_collect::FetchActReward().OnReq(role, msg);
	}
	break;

	case Protocol::COT_REQ_RED_EQUIP_COLLECT_FETCH_TITEL_REWARD:
	{
		reqhandler_red_equip_collect::RedEquipCollectFetchRewardHandler().OnReq(role, msg);
	}
	break;

	case Protocol::COT_REQ_ORANGE_EQUIP_COLLECT_TAKEON:
	{
		reqhandler_orange_equip_collect::TakeOn().OnReq(role, msg);
	}
	break;

	case Protocol::COT_REQ_PERSONAL_BOSS_BUY_TIMES:
	{
		FBManager::Instance().OnPersonalBossBuyEnterTimes(role);
	}
	break;
	}
}

void MessageHandler::OnLittlePetOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		gstr::SendError(role, "cross_opera_limit");
		return;
	}

	Protocol::CSLittlePetREQ *opera = (Protocol::CSLittlePetREQ*)msg;

	LittlePet *m_little_pet = GLOBAL_ROLE_MGR.GetLittlePet(role->GetUID());
	if (NULL == m_little_pet) return;

	switch (opera->opera_type)
	{
		//case Protocol::LITTLE_PET_REQ_INTENSIFY_SELF:
		//	{
		//		//m_little_pet->Strengthen(role, opera->param1, opera->param2, opera->param3);
		//	}
		//	break;

		//case Protocol::LITTLE_PET_REQ_INTENSIFY_LOVER:
		//	{
		//		//LittlePet *lover_pet = GLOBAL_ROLE_MGR.GetLittlePet(role->GetLoverUid());
		//		//if (NULL == lover_pet) return;

		//		//lover_pet->Strengthen(role, opera->param1, opera->param2, opera->param3);
		//	}
		//	break;

	case Protocol::LITTLE_PET_REQ_CHOUJIANG:
	{
		m_little_pet->Choujiang(opera->param1);
	}
	break;

	//case Protocol::LITTLE_PET_REQ_RECYCLE:
	//	{
	//		m_little_pet->RecyclePetItem(opera->param1, opera->param2, 1 == opera->param3);
	//	}
	//	break;

	//case Protocol::LITTLE_PET_REQ_RELIVE:
	//	{
	//		m_little_pet->Remove(opera->param1);
	//	}
	//	break;

	case Protocol::LITTLE_PET_REQ_FEED:
	{
		if (1 == opera->param2)
		{
			//m_little_pet->FeedLittlePet(role, opera->param1);
			m_little_pet->FeedLittlePetNew(role, opera->param1, 0 != opera->param3);
		}
		else
		{
			LittlePet *lover_pet = GLOBAL_ROLE_MGR.GetLittlePet(role->GetLoverUid());
			if (NULL == lover_pet) return;

			//lover_pet->FeedLittlePet(role, opera->param1);
			lover_pet->FeedLittlePetNew(role, opera->param1, 0 != opera->param3);
		}
	}
	break;

	case Protocol::LITTLE_PET_REQ_PET_FRIEND_INFO:
	{
		m_little_pet->SendPetFriendList();
	}
	break;

	//case Protocol::LITTLE_PET_REQ_INTERACT:
	//	{
	//		//LittlePet *target_pet = GLOBAL_ROLE_MGR.GetLittlePet(opera->param2);
	//		//if (NULL == target_pet) return;

	//		//if (opera->param3 != 1)
	//		//{
	//		//	target_pet = GLOBAL_ROLE_MGR.GetLittlePet(target_pet->GetLoverId());
	//		//	if (NULL == target_pet)	return;
	//		//}

	//		//target_pet->Interact(role, opera->param1);
	//	}
	//	break;

	case Protocol::LITTLE_PET_REQ_EXCHANGE:
	{
		m_little_pet->ExchangeItem(opera->param1, opera->param2);
	}
	break;

	case Protocol::LITTLE_PET_REQ_CHANGE_PET:
	{
		m_little_pet->Replace(opera->param1, opera->param2);
	}
	break;

	case Protocol::LITTLE_PET_REQ_USING_PET:
	{
		m_little_pet->ChangeImg(opera->param1);
	}
	break;

	case Protocol::LITTLE_PET_REQ_FRIEND_PET_LIST:
	{
		LittlePet *friend_pet = GLOBAL_ROLE_MGR.GetLittlePet(opera->param1);
		if (NULL == friend_pet) return;

		friend_pet->SendToFrienPetInfo(role);
	}
	break;

	case Protocol::LITTLE_PET_REQ_INTERACT_LOG:
	{
		m_little_pet->SendInteractLog();
	}
	break;

	case Protocol::LITTLE_PET_PUTON:
	{
		m_little_pet->PutOn(opera->param1, opera->param2);
	}
	break;

	case Protocol::LITTLE_PET_TAKEOFF:
	{
		m_little_pet->TakeOff(opera->param1);
	}
	break;

	case Protocol::LITTLE_PET_REQ_EQUIPMENT_PUTON:
	{
		m_little_pet->EquipmentPutOn(role, opera->param1, opera->param2);
	}
	break;

	case Protocol::LITTLE_PET_REQ_EQUIPMENT_TAKEOFF:
	{
		m_little_pet->EquipmentTakeOff(role, opera->param1, opera->param2);
	}
	break;

	case Protocol::LITTLE_PET_REQ_EQUIPMENT_UPLEVEL_SELF:
	{
		m_little_pet->OnEquipmentUplevel(role, opera->param1, opera->param2, 0 != opera->param3);
	}
	break;

	case Protocol::LITTLE_PET_REQ_EQUIPMENT_UPLEVEL_LOVER:
	{
		LittlePet *little_pet = GlobalRoleManager::Instance().GetLittlePet(m_little_pet->GetLoverId());
		if (nullptr != little_pet)
		{
			little_pet->OnEquipmentUplevel(role, opera->param1, opera->param2, 0 != opera->param3);
		}
	}
	break;

	case Protocol::LITTEL_PET_REQ_WALK:
	{
		m_little_pet->OnLittlePetWalk(role, opera->param1);
	}
	break;

	default:
		break;
	}
}

void MessageHandler::OnLoveTreeWaterReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSLoveTreeWaterReq *cmf = (Protocol::CSLoveTreeWaterReq*)msg;

	if (0 == cmf->is_water_other)
	{
		LoveTree * my_love_tree = GLOBAL_ROLE_MGR.GetLoveTree(role->GetUID());
		if (NULL == my_love_tree) return;
		my_love_tree->AddLoveTreeExp(role, 1 == cmf->is_auto_buy, false);
	}
	else
	{
		LoveTree *lover_tree = GLOBAL_ROLE_MGR.GetLoveTree(role->GetLoverUid());
		if (NULL == lover_tree) return;

		lover_tree->AddLoveTreeExp(role, 1 == cmf->is_auto_buy, true);
	}
}

void MessageHandler::OnLoveTreeInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSLoveTreeInfoReq *cmf = (Protocol::CSLoveTreeInfoReq*)msg;

	if (1 == cmf->is_self)
	{
		LoveTree * my_love_tree = GLOBAL_ROLE_MGR.GetLoveTree(role->GetUID());
		if (NULL == my_love_tree) return;
		my_love_tree->SendLoveTreeInfo(role, true);
	}
	else
	{
		LoveTree *lover_tree = GLOBAL_ROLE_MGR.GetLoveTree(role->GetLoverUid());
		if (NULL == lover_tree) return;

		lover_tree->SendLoveTreeInfo(role, false);
	}
}

void MessageHandler::OnLittlePetReName(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSLittlePetRename *opera = (Protocol::CSLittlePetRename*)msg;

	LittlePet *m_little_pet = GLOBAL_ROLE_MGR.GetLittlePet(role->GetUID());
	if (NULL == m_little_pet) return;

	m_little_pet->ReName(opera->index, opera->pet_name);
}

void MessageHandler::OnChineseZodiacPromoteEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
// 	Protocol::CSChineseZodiacPromoteEquip *chinesezodiac_equip = (Protocol::CSChineseZodiacPromoteEquip *)msg;
// 
// 	role->GetChineseZodiac()->PromoteEquip(chinesezodiac_equip->zodiac_type, chinesezodiac_equip->item_id);
}

void MessageHandler::OnChineseZodiacPromoteXingHun(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	//Protocol::CSChineseZodiacPromoteXingHun *chinesezodiac_xinghun = (Protocol::CSChineseZodiacPromoteXingHun *)msg;

	//role->GetChineseZodiac()->PromoteXingHun(chinesezodiac_xinghun->zodiac_type, 0 != chinesezodiac_xinghun->is_auto_buy, 0 != chinesezodiac_xinghun->is_use_promote_item);
}

void MessageHandler::OnChineseZodiacPromote(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSChineseZodiacPromote *pro = (Protocol::CSChineseZodiacPromote *)msg;

	role->GetChineseZodiac()->PromoteZodiac(pro->zodiac_type, pro->is_auto_buy);
}

void MessageHandler::OnChineseTianxiangReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTianxiangReq *pro = (Protocol::CSTianxiangReq *)msg;

	role->GetChineseZodiac()->ReqOpera(pro->info_type, pro->param1, pro->param2, pro->param3, pro->param4, pro->param5);
}

void MessageHandler::OnChineseZodiacTakeOffEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSChineseZodiacTakeOffEquip * pro = (Protocol::CSChineseZodiacTakeOffEquip *)msg;

	role->GetChineseZodiac()->TakeOffEquip(pro->zodiac_type, pro->equip_type, false);
}

void MessageHandler::OnChineseZodiacRecycle(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSChineseZodiacRecycle * pro = (Protocol::CSChineseZodiacRecycle *)msg;
	
	role->GetChineseZodiac()->Recycle(pro);
}
void MessageHandler::OnFriendGiftSongGift(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFriendSongGift *pro = (Protocol::CSFriendSongGift *)msg;

	role->GetFriendGiftManager()->OnFriendGiftSongGift(pro->friend_uid, pro->is_all_return, pro->is_return_gift);
}

void MessageHandler::OnFriendGiftInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetFriendGiftManager()->SendFriendGiftInfo();
}

void MessageHandler::OnFriendHeliRet(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSFriendHeliSendReq *pro = (Protocol::CSFriendHeliSendReq *)msg;
	role->GetFriendGiftManager()->OnHeliRet(pro->uid, pro->type);
}

void MessageHandler::OnGiveItemOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGiveItemOpera *gio = (Protocol::CSGiveItemOpera *)msg;
	switch (gio->opera_type)
	{
	case Protocol::GIVE_ITEM_OPERA_TYPE_INFO:
	{
		role->GetFriendGiftManager()->SendGiveItemInfo(role, gio->param_1 == 1);
	}
		break;
	}
}

void MessageHandler::OnGiveItemReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGiveItemReq *gir = (Protocol::CSGiveItemReq *)msg;

	// 修正长度，防止客户端恶意发送数据
	int arr_len = length - (sizeof(Protocol::CSGiveItemReq) - sizeof(gir->give_item_list));
	gir->item_count = arr_len / sizeof(gir->give_item_list[0]);

	if (gir->item_count > 0)
	{
		role->GetFriendGiftManager()->OnGiveItemReq(gir);
	}
}

void MessageHandler::OnCrossShuiJingBuyBuff(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	SpecialCrossShuiJing *special_logic = dynamic_cast<SpecialCrossShuiJing*>(scene->GetSpecialLogic());
	if (NULL != special_logic && Scene::SCENE_TYPE_CROSS_SHUIJING == special_logic->GetSceneType())
	{
		special_logic->OnBuyGatherBuff(role);
	}
}

void MessageHandler::OnShuiJingBuyBuff(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	SpecialShuiJing *special_logic = dynamic_cast<SpecialShuiJing*>(scene->GetSpecialLogic());
	if (NULL != special_logic && Scene::SCENE_TYPE_SHUIJING == special_logic->GetSceneType())
	{
		special_logic->OnBuyGatherBuff(role);
	}
}

void MessageHandler::OnShuiJingFetchTaskReward(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSShuijingFetchTaskReward *pro = (Protocol::CSShuijingFetchTaskReward *)msg;

	SpecialShuiJing *special_logic = dynamic_cast<SpecialShuiJing*>(scene->GetSpecialLogic());
	if (NULL != special_logic && Scene::SCENE_TYPE_SHUIJING == special_logic->GetSceneType())
	{
		special_logic->FetchShuijingTaskReward(role, pro->task_id);
	}
}

void MessageHandler::OnWanglingExploreBuyBuff(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	SpecialWangLingExplore *special_logic = dynamic_cast<SpecialWangLingExplore *>(scene->GetSpecialLogic());
	if (nullptr != special_logic && Scene::SCENE_TYPE_WANGLINGEXPLORE == special_logic->GetSceneType())
	{
		special_logic->OnBuyGatherBuff(role);
	}
}

void MessageHandler::OnCrossBianjingZhiDiBuyBuff(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	SpecialCrossBianJingZhiDi *special_logic = dynamic_cast<SpecialCrossBianJingZhiDi *>(scene->GetSpecialLogic());
	if (nullptr != special_logic && Scene::SCENE_TYPE_CROSS_BIANJINGZHIDI == special_logic->GetSceneType())
	{
		special_logic->OnBuyGatherBuff(role);
	}
}

void MessageHandler::OnFriendExpBottle(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSFriendExpBottleOP *pro = (Protocol::CSFriendExpBottleOP *)msg;

	role->GetFriendExpBottle()->OnFriendExpBottleOperateReq(pro->type);
}

void MessageHandler::OnTuodanReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTuodanREQ *pro = (Protocol::CSTuodanREQ *)msg;
	TuodanManager::Instance().TuodanOpera(role, pro->req_type, pro->notice);
}

void MessageHandler::OnGetAllTuodanInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	TuodanManager::Instance().SendAllTuodan(role);
}

void MessageHandler::OnChongZhi7DayFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->FetchChongzhi7DayReward();
}

void MessageHandler::OnShengeSystemOperaReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSShengeSystemReq *rsq = (Protocol::CSShengeSystemReq*)msg;

	if (rsq->index_count < 0 || length != static_cast<int>(UNSTD_OFFSETOFF(Protocol::CSShengeSystemReq, virtual_index_list) + sizeof(rsq->virtual_index_list[0]) * rsq->index_count)) return;

	role->GetShengeSystem()->OperaationReq(rsq);
}

void MessageHandler::OnShenbingUpLevelReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShenBingUpLevel *cmd = (Protocol::CSShenBingUpLevel*)msg;

	bool ret = role->GetShenBingManager()->UpLevel(role, cmd->stuff_index);
	if (!ret)
	{
		role->SendOperateResult(Protocol::SCOperateResult::OP_SHENBING_UPLEVEL, 0);
	}
	else
	{
		role->SendOperateResult(Protocol::SCOperateResult::OP_SHENBING_UPLEVEL, 1);
	}
}

void MessageHandler::OnTuituFbOperaReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSTuituFbOperaReq* ttfor = (Protocol::CSTuituFbOperaReq*)msg;
	role->GetTuituFb()->OnTuituOperaReq(ttfor);
}

void MessageHandler::OnGoldenPigOperate(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSGoldenPigOperateReq *gpor = (Protocol::CSGoldenPigOperateReq*)msg;
	role->GetRoleActivity()->OnRAGoldenPigOperateReq(gpor->operate_type, gpor->param);
}

void MessageHandler::OnSkipReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSSkipReq *pro = (Protocol::CSSkipReq*)msg;
	switch (pro->type)
	{
	case Protocol::CSSkipReq::SKIP_TYPE_CHALLENGE:
	{
		RoleFighting *fighting = GlobalRoleManager::Instance().GetRoleFighting(role->GetUID());
		if (NULL != fighting)
		{
			fighting->OnSkipChallenge(role);
		}
	}
	break;

	case Protocol::CSSkipReq::SKIP_TYPE_SAILING:
	{
		RoleFighting *fighting = GlobalRoleManager::Instance().GetRoleFighting(role->GetUID());
		if (NULL != fighting)
		{
			fighting->OnSkipSailing(role, pro->param);
		}
	}
	break;

	case Protocol::CSSkipReq::SKIP_TYPE_MINE:
	{
		RoleFighting *fighting = GlobalRoleManager::Instance().GetRoleFighting(role->GetUID());
		if (NULL != fighting)
		{
			fighting->OnSkipMining(role, pro->param);
		}
	}
	break;

	case Protocol::CSSkipReq::SKIP_TYPE_FISH:
	{
		FishPool *fish_pool = FishPoolManager::Instance().GetFishPool(role->GetUID());
		if (NULL != fish_pool)
		{
			fish_pool->OnSkipFishPool(role, pro->param);
		}
	}
	break;

	case Protocol::CSSkipReq::SKIP_TYPE_JINGLING_ADVANTAGE:
	{
		JinglingAdvantage::Instance().OnSkipGather(role);
	}
	break;

	case Protocol::CSSkipReq::SKIP_TYPE_SHENZHOU_WEAPON:
	{
		role->GetShenzhouWeapon()->SkipGather();
	}
	break;

	case Protocol::CSSkipReq::SKIP_TYPE_XINGZUOYIJI:
	{
		int fb_key = ActivityShadow::Instance().GetFBKeyByRoomIndex(0);
		SpecialXingzuoYiji *xingzuoyiji_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialXingzuoYiji(fb_key, true);
		if (NULL != xingzuoyiji_logic)
		{
			xingzuoyiji_logic->SkipGather(role);
		}
	}
	break;

	case Protocol::CSSkipReq::SKIP_TYPE_QYSD:
	{
		role->GetQingyuan()->OnQingYuanShengDiSkipTask();
	}
	break;

	case Protocol::CSSkipReq::SKIP_TYPE_PRECIOUS_BOSS:
	{
		role->GetRolePreciousBoss()->SkipTask();
	}
	break;

	case Protocol::CSSkipReq::SKIP_TYPE_PAOHUAN_TASK:
	{
		role->GetPaoHuanTask()->OnSkipTask(false, 0);
	}
	break;

	case Protocol::CSSkipReq::SKIP_TYPE_CROSS_GUIDE_BATTLE:
	{
		role->GetRoleCross()->SkipCrossGuildBattleTask();
	}
	break;

	default:
		break;
	}
}

void MessageHandler::OnImgFulingOperaReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSImgFulingOperate *pro = (Protocol::CSImgFulingOperate *)msg;
	role->GetImgFuling()->OnImgFulingOperate(pro->operate_type, pro->param_1, pro->param_2, pro->param_3);
}

void MessageHandler::OnElementHeartOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSElementHeartReq *cgr = (Protocol::CSElementHeartReq*)msg;

	role->GetElementHeart()->ReqOpera(cgr->info_type, cgr->param1, cgr->param2, cgr->param3, cgr->param4);
}

void MessageHandler::OnShenYinOneKeyRecyleOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShenYinOneKeyRecyleReq *cmd = (Protocol::CSShenYinOneKeyRecyleReq*)msg;

	// 修正长度，防止客户端恶意发送数据
	int arr_len = length - (sizeof(Protocol::CSShenYinOneKeyRecyleReq) - sizeof(cmd->bag_index_list));
	cmd->count = arr_len / sizeof(cmd->bag_index_list[0]);

	if (cmd->count > 0)
	{
		role->GetShenYin()->OneKeyRecycleOpera(cmd);
	}
}

void MessageHandler::OnGetShengxingzhuliInfoReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	role->GetShengxingzhuliManager()->SendAllInfo();
}

void MessageHandler::OnGetShengxingzhuliRewardReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	role->GetShengxingzhuliManager()->OnGetReward(true);
}

void MessageHandler::OnCrossDarkNightOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	if (!ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_DARK_NIGHT))
	{
		return;
	}

	SpecialCrossDarkNight* special_darknight = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossDarkNight(COMMON_SCENE_KEY, true);
	if (NULL == special_darknight)
	{
		return;
	}

	special_darknight->OnPlayerOpera(role);
}
void MessageHandler::OnRoleJingJieOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length) 
{
	Protocol::CSRoleJingJieReq *rjjr = (Protocol::CSRoleJingJieReq *)msg;
	role->GetJingJie()->OnJingJieOpera(rjjr->opera_type, rjjr->is_auto_buy);
}


void MessageHandler::OnGetDropLog(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGetDropLog *csdl = (Protocol::CSGetDropLog *)msg;
	if (csdl->type == Protocol::DOPE_LOG_TYPE_BOSS)
	{
		DropLogManager::Instance().SendLog(role);
	}
	else if (csdl->type == Protocol::DOPE_LOG_TYPE_FB)
	{
		DropLogManager::Instance().SendLogFb(role);
	}
}

void MessageHandler::OnBabyBossOperate(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSBabyBossOperate *pro = (Protocol::CSBabyBossOperate *)msg;
	switch (pro->operate_type)
	{
	case Protocol::CSBabyBossOperate::TYPE_BOSS_INFO_REQ:
	{
		BossFamilyManager::Instance().SendAllBabyBossInfo(role);
	}
	break;

	case Protocol::CSBabyBossOperate::TYPE_ROLE_INFO_REQ:
	{
		BossFamilyManager::Instance().SendBabyBossRoleInfo(role);
	}
	break;

	case Protocol::CSBabyBossOperate::TYPE_ENTER_REQ:
	{
		BossFamilyManager::Instance().OnEnterBabyBoss(role, pro->param_0, pro->param_1);
	}
	break;

	case Protocol::CSBabyBossOperate::TYPE_LEAVE_REQ:
	{
		BossFamilyManager::Instance().OnLeaveBabyBossReq(role);
	}
	break;

	default:
		break;
	}
}

void MessageHandler::OnRoleTelentOperate(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSRoleTelentOperate *rti = (Protocol::CSRoleTelentOperate *)msg;
	role->GetTalent()->OnTalentOperate(rti);
}


void MessageHandler::OnJinJieTalentOperaReqAll(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSJinJieTalentOperaReqAll *cmd = (Protocol::CSJinJieTalentOperaReqAll*)msg;
	switch (cmd->operate_type)
	{
	case Protocol::JINJIE_TALENT_OPERATE_TYPE_INFO:
	{
		role->GetJinJieTalent()->SendTalentAllInfo();
	}
	break;
	case Protocol::JINJIE_TALENT_OPERATE_TYPE_CHOUJIANG_INFO:
	{
		role->GetJinJieTalent()->SendTalentChoujiangPage();
	}
	break;
	case Protocol::JINJIE_TALENT_OPERATE_TYPE_CHOUJIANG_REFRESH:
	{
		role->GetJinJieTalent()->OnChoujiangFlush(cmd->param1);
	}
	break;
	case Protocol::JINJIE_TALENT_OPERATE_TYPE_AWAKE:
	{
		role->GetJinJieTalent()->OnAwakeSkill(cmd->param1);
	}
	break;
	case Protocol::JINJIE_TALENT_OPERATE_TYPE_PUTON:
	{
		role->GetJinJieTalent()->OnPutOn(cmd->param1, cmd->param2, cmd->param3);
	}
	break;
	case Protocol::JINJIE_TALENT_OPERATE_TYPE_PUTOFF:
	{
		role->GetJinJieTalent()->OnPutOff(cmd->param1, cmd->param2);
	}
	break;
	case Protocol::JINJIE_TALENT_OPERATE_TYPE_SKILL_UPGRADE_LEVEL:
	{
		bool auto_buy = cmd->param3 ? true : false;
		role->GetJinJieTalent()->OnUpgrageSkillLevel(cmd->param1, cmd->param2, auto_buy);
	}
	break;
	case Protocol::TALENT_OPERATE_TYPE_SAVE_ATTENTION_SKILL:
	{
		role->GetJinJieTalent()->OnSaveAttentionSkill(cmd->param1);
	}
	break;

	case Protocol::TALENT_OPERATE_TYPE_CANCEL_ATTENTION_SKILL:
	{
		role->GetJinJieTalent()->OnCancelAttentionSkill(cmd->param1);
	}
	break;

	default:
		break;
	}
}

void MessageHandler::OnUpgradeSysOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUpgradeOperaReq *upr = (Protocol::CSUpgradeOperaReq*)msg;

	role->GetUpgradeManager()->OnRoleOpera(upr);
}

void MessageHandler::OnYaoShiOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSYaoShiOperaReq *ysor = (Protocol::CSYaoShiOperaReq*)msg;

	switch (ysor->opera_type)
	{
	case Protocol::YAOSHI_OPERA_TYPE_INFO:
	{
		role->GetYaoShiManager()->SendYaoShiInfo();
	}
	break;

	case Protocol::YAOSHI_OPERA_TYPE_UPGRADE:
	{
		bool ret = YaoShiFunction::Upgrade(role, 1 == ysor->param_2, ysor->param_1);
		role->SendOperateResult(Protocol::SCOperateResult::OP_YAOSHI_UPGRADE, ret);
			}
	break;

	case Protocol::YAOSHI_OPERA_TYPE_SPECIAL_IMAGE_UPGRADE:
	{
		YaoShiFunction::YaoShiSpecialImgUpgrade(role, ysor->param_1);
	}
	break;

	case Protocol::YAOSHI_OPERA_TYPE_USE_IMAGE:
	{
		role->GetYaoShiManager()->UseImage(ysor->param_1);
	}
	break;

	case Protocol::YAOSHI_OPERA_TYPE_SKILL_UPGRADE:
	{
		YaoShiFunction::UplevelSkill(role, ysor->param_1, 1 == ysor->param_2);
	}
	break;
	}
}

void MessageHandler::OnToushiOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTouShiOperaReq *tsor = (Protocol::CSTouShiOperaReq*)msg;

	switch (tsor->opera_type)
	{
	case Protocol::TOUSHI_OPERA_TYPE_INFO:
		{
			role->GetTouShiManager()->SendTouShiInfo();
		}
		break;

	case Protocol::TOUSHI_OPERA_TYPE_UPGRADE:
		{
			bool ret = TouShiFunction::Upgrade(role, 1 == tsor->param_2, tsor->param_1);
			role->SendOperateResult(Protocol::SCOperateResult::OP_TOUSHI_UPGRADE, ret);
		}
		break;

	case Protocol::TOUSHI_OPERA_TYPE_SPECIAL_IMAGE_UPGRADE:
		{
			TouShiFunction::TouShiSpecialImgUpgrade(role, tsor->param_1);
		}
		break;

	case Protocol::TOUSHI_OPERA_TYPE_USE_IMAGE:
		{
			role->GetTouShiManager()->UseImage(tsor->param_1);
		}
		break;

	case Protocol::TOUSHI_OPERA_TYPE_SKILL_UPGRADE:
		{
			TouShiFunction::UplevelSkill(role, tsor->param_1, 1 == tsor->param_2);
		}
		break;
	}
}

void MessageHandler::OnQiLinBiOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSQiLinBiOperaReq *tsor = (Protocol::CSQiLinBiOperaReq*)msg;

	switch (tsor->opera_type)
	{
	case Protocol::QILINBI_OPERA_TYPE_INFO:
	{
		role->GetQiLinBiManager()->SendQiLinBiInfo();
	}
	break;

	case Protocol::QILINBI_OPERA_TYPE_UPGRADE:
	{
		bool ret = QiLinBiFunction::Upgrade(role, 1 == tsor->param_2, tsor->param_1);
		role->SendOperateResult(Protocol::SCOperateResult::OP_QILINBI_UPGRADE, ret);
			}
	break;

	case Protocol::QILINBI_OPERA_TYPE_SPECIAL_IMAGE_UPGRADE:
	{
		QiLinBiFunction::QiLinBiSpecialImgUpgrade(role, tsor->param_1);
	}
	break;

	case Protocol::QILINBI_OPERA_TYPE_USE_IMAGE:
	{
		role->GetQiLinBiManager()->UseImage(tsor->param_1);
	}
	break;

	case Protocol::QILINBI_OPERA_TYPE_SKILL_UPGRADE:
	{
		QiLinBiFunction::UplevelSkill(role, tsor->param_1, 1 == tsor->param_2);
	}
	break;

	case Protocol::QILINBI_OPERA_TYPE_EQUIP_UPGRADE:
	{
		QiLinBiFunction::UplevelEquip(role, tsor->param_1);
	}
	break;

	}
}

void MessageHandler::OnMaskOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSMaskOperaReq *tsor = (Protocol::CSMaskOperaReq*)msg;

	switch (tsor->opera_type)
	{
	case Protocol::MASK_OPERA_TYPE_INFO:
	{
		role->GetMaskManager()->SendMaskInfo();
	}
	break;

	case Protocol::MASK_OPERA_TYPE_UPGRADE:
	{
		bool ret = MaskFunction::Upgrade(role, 1 == tsor->param_2, tsor->param_1);
		role->SendOperateResult(Protocol::SCOperateResult::OP_MASK_UPGRADE, ret);
			}
	break;

	case Protocol::MASK_OPERA_TYPE_SPECIAL_IMAGE_UPGRADE:
	{
		MaskFunction::MaskSpecialImgUpgrade(role, tsor->param_1);
	}
	break;

	case Protocol::MASK_OPERA_TYPE_USE_IMAGE:
	{
		role->GetMaskManager()->UseImage(tsor->param_1);
	}
	break;

	case Protocol::MASK_OPERA_TYPE_SKILL_UPGRADE:
	{
		MaskFunction::UplevelSkill(role, tsor->param_1, 1 == tsor->param_2);
	}
	break;
	}
}

void MessageHandler::OnZhuanzhiEquipOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSZhuanzhiEquipOpe *siu = (Protocol::CSZhuanzhiEquipOpe*)msg;
	role->GetZhuanzhiEquip()->OnOperaReq(siu->operate, siu->param1, siu->param2, siu->param3, siu->param4, siu->param5);
}

void MessageHandler::OnFishingOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFishingOperaReq *op = (Protocol::CSFishingOperaReq*)msg;

	if (op->opera_type != Protocol::FISHING_OPERA_REQ_TYPE_BUY_BAIT && !ActivityShadow::Instance().IsCrossActivityOpen(CROSS_ACTIVITY_TYPE_FISHING))
	{
		return;
	}

	SpecialCrossFishing* special_fishing = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossFishing(COMMON_SCENE_KEY, true);
	if (NULL == special_fishing)
	{
		return;
	}

	bool is_opera_succ = true;
	std::string reset_reason;

	switch (op->opera_type)
	{
	case Protocol::FISHING_OPERA_REQ_TYPE_START_FISHING:
	{
		is_opera_succ = special_fishing->StartFishing(role);
		reset_reason = "StartFishing";
	}
	break;

	case Protocol::FISHING_OPERA_REQ_TYPE_CASTING_RODS:
	{
		is_opera_succ = special_fishing->CastingRods(role, op->param1);
		reset_reason = "CastingRods";
	}
	break;

	case Protocol::FISHING_OPERA_REQ_TYPE_PULL_RODS:
	{
		is_opera_succ = special_fishing->PullRods(role, 1 == op->param1);
		reset_reason = "PullRods";
	}
	break;

	case Protocol::FISHING_OPERA_REQ_TYPE_CONFIRM_EVENT:
	{
		is_opera_succ = special_fishing->ConfirmEvent(role, op->param1);
		reset_reason = "ConfirmEvent";
	}
	break;

	case Protocol::FISHING_OPERA_REQ_TYPE_USE_GEAR:
	{
		special_fishing->UseFishingGear(role, op->param1);
	}
	break;

	case Protocol::FISHING_OPERA_REQ_TYPE_BIG_FISH_HELP:
	{
		special_fishing->HelpBigFish(role, op->param1);
	}
	break;

	case Protocol::FISHING_OPERA_REQ_TYPE_STOP_FISHING:
	{
		special_fishing->StopFishing(role);
	}
	break;

	case Protocol::FISHING_OPERA_REQ_TYPE_AUTO_FISHING:
	{
		special_fishing->AutoFishingReq(role, 0 != op->param1, op->param2);
	}
	break;

	case Protocol::FISHING_OPERA_REQ_TYPE_RAND_USER:
	{
		special_fishing->OnRandUserReq(role);
	}
	break;

	case Protocol::FISHING_OPERA_REQ_TYPE_BUY_STEAL_COUNT:
	{
		special_fishing->OnBuyStealCountReq(role);
	}
	break;

	case Protocol::FISHING_OPERA_REQ_TYPE_RANK_INFO:
	{
		special_fishing->SendFishingRankListToRole(role);
	}
	break;

	//case Protocol::FISHING_OPERA_REQ_TYPE_STEAL_FISH:
	//{
	//	special_fishing->OnStealFishReq(role, op->param1);
	//}
	//break;

	case Protocol::FISHING_OPERA_REQ_TYPE_EXCHANGE:
	{
		special_fishing->OnExChange(role, op->param1);
	}
	break;

	case Protocol::FISHING_OPERA_REQ_TYPE_BUY_BAIT:
	{
		special_fishing->OnBuyBait(role, op->param1, op->param2);
	}
	break;

	case Protocol::FISHING_OPERA_REQ_TYPE_SCORE_REWARD:
	{
		special_fishing->OnFetchScoreReward(role);
	}
	break;
	}

	// 不太可能流程出错，除非：
	// 1，客户端发送了错误请求；2，自动钓鱼没有了鱼饵
	if (!is_opera_succ)
	{
		special_fishing->ResetUserInfoOnFail(role, reset_reason.c_str());
	}
}

void MessageHandler::OnNightFightOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSNightFightOperaReq *nfor = (Protocol::CSNightFightOperaReq *)msg;

	switch (nfor->opera_type)
	{
	case Protocol::NIGHT_FIGHT_OPERA_TYPE_ENTER:
		{
			role->GetRoleActivity()->OnNightFightEnter();
		}
		break;

	case Protocol::NIGHT_FIGHT_OPERA_TYPE_POSI_INFO:
		{
			SpecialNightFight *special_night_fight = dynamic_cast<SpecialNightFight*>(role->GetScene()->GetSpecialLogic());
			if (NULL != special_night_fight && Scene::SCENE_TYPE_NIGHT_FIGHT_FB == special_night_fight->GetSceneType())
			{
				special_night_fight->OnGetPlayerPosi(role, nfor->param_1);
			}
		}
		break;

	}
}

void MessageHandler::OnHeShenLuoShuReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSHeShenLuoShuReq *hsls = (Protocol::CSHeShenLuoShuReq *)msg;

	role->GetHeShengLuoShu()->OnHandlerOpera(hsls->opera_type, hsls->param1);
}

void MessageHandler::OnShengqiOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShengqiReq *sqr = (Protocol::CSShengqiReq*)msg;
	role->GetShengqi()->ShengqiOpera(sqr->req_type, sqr->param1, sqr->param2);
}

void MessageHandler::OnShangGuBossReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShangGuBossReq *cmd = (Protocol::CSShangGuBossReq*)msg;
	BossShangGuManager::Instance().OnShangGuBossReq(role, cmd->opera_type, cmd->param1, cmd->param2, cmd->param3);
}

void MessageHandler::OnImpGuardOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSImpGuardOperaReq *cmd = (Protocol::CSImpGuardOperaReq*)msg;
	role->GetImpGuard()->OnImpGuardOperaReq(cmd->opera_type, cmd->param1, cmd->param2);
}

void MessageHandler::OnBossCardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSBossCardReq *cmd = (Protocol::CSBossCardReq *)msg;
	role->GetBossCard()->OnOperaReq(cmd->req_type, cmd->param1, cmd->param2);
}

void MessageHandler::OnGetCoupleRankList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSGetCoupleRankList *gcrl = (Protocol::CSGetCoupleRankList *)msg;
	CoupleRankManager::Instance().OnGetRankList(role, *gcrl);
}

void MessageHandler::OnRoleZhuanZhiReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSRoleZhuanZhiReq *rzzq = (Protocol::CSRoleZhuanZhiReq *)msg;
	role->GetRoleZhuanZhi()->OnRoleZhuanZhiReq(rzzq->operate_type, rzzq->param_1, rzzq->param_2, rzzq->param_3);
}

void MessageHandler::OnSealOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSealReq *sr = (Protocol::CSSealReq*)msg;
	role->GetSeal()->SealOpera(sr->req_type, sr->param1, sr->param2);
}

void MessageHandler::OnSealRecycleReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSSealRecycleReq *srr = (Protocol::CSSealRecycleReq *)msg;

	if (srr->recycle_num > 0)
	{
		role->GetSeal()->Recycle(srr->recycle_num, srr->recycle_backpack_index_list);
	}
}

void MessageHandler::OnTeamTowerDefendOpreatReq(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
 	Protocol::CSTeamTowerDefendOpreatReq* ttdr = (Protocol::CSTeamTowerDefendOpreatReq*)msg;
	if (Protocol::TEAM_TOWER_DEFEND_SET_ATTR_TYPE == ttdr->req_type)
	{
		role->GetFBTowerDefend()->LeaderSetAttrType(ttdr->param1, static_cast<char>(ttdr->param2));
	}
	else if(Protocol::TEAM_TOWER_DEFEND_NEXT_WAVE_REQ == ttdr->req_type)
	{
		SpecialTeamTowerDefend * sptm = dynamic_cast<SpecialTeamTowerDefend*>(scene->GetSpecialLogic());
		if (NULL != sptm)
		{
			sptm->OnNextWaveReq(role);
		}
	}
	
}

void MessageHandler::OnGongChenZhanWorship(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Activity *activity = ActivityManager::Instance().ForceGetActivity(ACTIVITY_TYPE_WORSHIP);
	if (nullptr == activity || activity->GetActivityType() != ACTIVITY_TYPE_WORSHIP) return;

	ActivityWorship *gcz_activity = (ActivityWorship*)activity;
	gcz_activity->OnWorship(role);
}

void MessageHandler::OnRoleWorship(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	if (nullptr == scene || nullptr == role)
	{
		return;
	}

	switch (scene->GetSceneType())
	{
	case Scene::SCENE_TYPE_QUNXIANLUANDOU:
	{
		SpecialQunXianLuanDou *special_logic = dynamic_cast<SpecialQunXianLuanDou *>(scene->GetSpecialLogic());
		if (nullptr != special_logic && Scene::SCENE_TYPE_QUNXIANLUANDOU == special_logic->GetSceneType())
		{
			special_logic->OnWorship(role);
		}
	}
	break;

	case Scene::SCENE_TYPE_GONGCHENGZHAN:
	{
		SpecialGongChengZhan *special_logic = dynamic_cast<SpecialGongChengZhan *>(scene->GetSpecialLogic());
		if (nullptr != special_logic && Scene::SCENE_TYPE_GONGCHENGZHAN == special_logic->GetSceneType())
		{
			special_logic->OnWorship(role);
		}
	}
	break;

	case Scene::SCENE_TYPE_GUILDBATTLE:
	{
		SpecialGuildBattleNew *special_logic = dynamic_cast<SpecialGuildBattleNew *>(scene->GetSpecialLogic());
		if (nullptr != special_logic && Scene::SCENE_TYPE_GUILDBATTLE == special_logic->GetSceneType())
		{
			special_logic->OnWorship(role);
		}
	}
	break;

	case Scene::SCENE_TYPE_CROSS_GUILDBATTLE:
	{
		SpecialCrossGuildBattle *special_logic = dynamic_cast<SpecialCrossGuildBattle *>(scene->GetSpecialLogic());
		if (nullptr != special_logic && Scene::SCENE_TYPE_CROSS_GUILDBATTLE == special_logic->GetSceneType())
		{
			special_logic->OnWorship(role);
		}
	}
	break;

	}

}

void MessageHandler::OnLuckyLogItemReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSGetLuckyLog *pro = (Protocol::CSGetLuckyLog *)msg;
	LuckyLogManager::Instance().SendLog(role, pro->activity_type);
}

void MessageHandler::OnJingLingAdvantageBossEnter(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSJingLingAdvantageBossEnter *pro = (Protocol::CSJingLingAdvantageBossEnter *)msg;
	if (pro->oper_type == Protocol::JINGLING_ADCANTAGE_OPER_TYPE_BOSS)
	{
		JinglingAdvantage::Instance().OnEnterBossScene(role, pro->monster_id);
	}
	else if (pro->oper_type == Protocol::JINGLING_ADCANTAGE_OPER_TYPE_EGG)
	{
		JinglingAdvantage::Instance().OnGetEggPosBySceneId(role, pro->monster_id);
	}
}

// 节日守护(塔防)
void MessageHandler::OnHolidayGuardReqAction(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSHolidayGuardRoleReq *req = (Protocol::CSHolidayGuardRoleReq*)msg;

	switch (req->req_type)
	{
	case Protocol::HOLIDAY_GUARD_NEXT_WAVE:
	{
		if (Scene::SCENE_TYPE_HOLIDAY_GUARD_FB == scene->GetSpecialLogic()->GetSceneType())
		{
			SpecialHolidayGuard* special_holiday_guard = dynamic_cast<SpecialHolidayGuard*>(scene->GetSpecialLogic());
			if (nullptr != special_holiday_guard)
				special_holiday_guard->OnNextWaveReq(role);
		}
	}
	break;
	}
}

void MessageHandler::OnRolePersonAreaMsgInfoReq(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSRolePersonAreaMsgInfo* req = (Protocol::CSRolePersonAreaMsgInfo*)msg;
	Protocol::SCRolePersonAreaMsgInfo info;
	info.obj_id = role->GetId();
	size_t msg_offset = UNSTD_OFFSETOFF(Protocol::CSRolePersonAreaMsgInfo, msg);
	if ((size_t)length < msg_offset)
	{
		return;
	}

	UNSTD_STATIC_CHECK(sizeof(info.msg) == sizeof(req->msg));
	size_t w_len = std::min(sizeof(info.msg), (size_t)length - msg_offset);
	if (w_len > 0)
	{
		memcpy(info.msg, req->msg, w_len);
	}
	info.count = (short)w_len;

	role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(role->GetObserHandle(), &info, UNSTD_OFFSETOFF(Protocol::SCRolePersonAreaMsgInfo, msg) + w_len);
}

void MessageHandler::OnHolidayGuardRankInfoReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	RandActivity *rand_act = RandActivityManager::Instance().ForceGetRandActivity(RAND_ACTIVITY_TYPE_HOLIDAY_GUARD);
	if (nullptr == rand_act)
	{
		return;
	}

	RandActivityHolidayGuard *holiday_guard_act = static_cast<RandActivityHolidayGuard*> (rand_act);
	holiday_guard_act->SendRankInfo(role);
}

void MessageHandler::OnRATotalCharge5RequestInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	role->GetRoleActivity()->OnSendTotalCharge5Info();
}

void MessageHandler::OnDressingRoomOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSDressingRoomOpera *pro = (Protocol::CSDressingRoomOpera *)msg;
	role->GetDressingRoomMgr()->OnDressingRoomOpera(pro->opera_type);
}

void MessageHandler::OnDressingRoomExchange(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSDressingRoomExchange *dre = (Protocol::CSDressingRoomExchange *)msg;
	role->GetDressingRoomMgr()->OnDressingRoomExchange(dre->suit_index, dre->sub_index);
}

// 进阶系统奖励
void MessageHandler::OnJinjiesysRewardOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSJinjiesysRewardOpera *cmd = (Protocol::CSJinjiesysRewardOpera*)msg;
	JinjieSysRewardMgr::Instance().OnJinjiesysRewardOpera(role, cmd->opera_type, cmd->param_0, cmd->param_1);
}

void MessageHandler::OnRoleBigSmallGoalOper(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSRoleBigSmallGoalOper *rbs = (Protocol::CSRoleBigSmallGoalOper *)msg;
	role->GetRoleBigSmallGoal()->OnRoleBigSmallGoalOpera(rbs->oper_type, rbs->param_1, rbs->param_2);
}

void MessageHandler::OnShenqiOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSShenqiOperaReq* sqor = (Protocol::CSShenqiOperaReq*)msg;
	role->GetShenqi()->OnShenqiOperaReq(sqor);
}

void MessageHandler::OnGetSpecialRankValue(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSGetSpecialRankValue *pro = (Protocol::CSGetSpecialRankValue *)msg;
	int rank_value = 0;
	switch (pro->rank_type)
	{
	case CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY:
	{
		rank_value = role->GetCommonDataParam()->add_cap_for_rank;
	}
	break;

	case CROSS_PERSON_RANK_TYPE_ADD_CHARM:
	{
		rank_value = role->GetCommonDataParam()->add_charm_for_rank;
	}
	break;

	case CROSS_COUPLE_RANK_TYPE_BEGIN + CROSS_COUPLE_RANK_TYPE_QINGYUAN_CAP:
	{
		rank_value = role->GetCapability()->GetTotalCapability();
		if (role->GetLoverUid() > 0)
		{
			UserCacheNode *user = UserCacheManager::Instance().GetUserNode(role->GetLoverUid());
			if (nullptr != user)
			{
				rank_value += user->capability;
			}
		}
	}
	break;

	case CROSS_PERSON_RANK_TYPE_GUILD_KILL_BOSS:
	{
		Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
		if (nullptr != guild)
		{
			rank_value = guild->GetGuildKillBossNum();
		}
	}
	break;

	default:
		break;
	}

	Protocol::SCGetSpecialRankValueAck ack;
	ack.rank_type = pro->rank_type;
	ack.rank_value = rank_value;
	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&ack, sizeof(ack));
}

void MessageHandler::OnDayChongzhiRewardReq(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSDayChongzhiRewardReq *req = (Protocol::CSDayChongzhiRewardReq*)msg;
	role->GetRoleActivity()->OnDayChongzhiRewardOpera(req->opera_type, req->param_0);
}

void MessageHandler::OnXianZunKaOperaReq(const GSNetID &netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSXianZunKaOperaReq *xzkor = (Protocol::CSXianZunKaOperaReq *)msg;

	switch (xzkor->opera_req_type)
	{
	case Protocol::XIANZUNKA_OPERA_REQ_TYPE_ALL_INFO:
	{
		role->GetXianZunKa()->SendAllInfo();
	}
	break;

	case Protocol::XIANZUNKA_OPERA_REQ_TYPE_BUY_CARD:
	{
		role->GetXianZunKa()->OnBuyXianZunKa(xzkor->param_1);
	}
	break;

	case Protocol::XIANZUNKA_OPERA_REQ_TYPE_FETCH_DAILY_REWARD:
	{
		role->GetXianZunKa()->OnFetchDailyReward(xzkor->param_1);
	}
	break;

	case Protocol::XIANZUNKA_OPERA_REQ_TYPE_ACTIVE:
	{
		role->GetXianZunKa()->OnActiveXianZunKa(xzkor->param_1);
	}
	break;

	}
}

void MessageHandler::OnSpouseHomeOpera(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSSpouseHomeOperaReq *req = (Protocol::CSSpouseHomeOperaReq*)msg;

	SpouseHome * target_info = GLOBAL_ROLE_MGR.GetSpouseHomeGlobal(role->GetUID());
	if (nullptr == target_info)
	{
		return;
	}

	target_info->ReqOpera(req->info_type, req->param1, req->param2, req->param3, req->param4);
}

void MessageHandler::OnEquipBaptizeOperaReq(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSEquipBaptizeOperaReq *ebor = (Protocol::CSEquipBaptizeOperaReq*)msg;

	switch (ebor->opera_type)
	{
	case Protocol::EQUIP_BAPTIZE_OPERA_TYPE_ALL_INFO:
	{
		role->GetEquipBaptize()->SendEquipBaptizeAllInfo();
	}
	break;

	case Protocol::EQUIP_BAPTIZE_OPERA_TYPE_OPEN_SLOT:
	{
		role->GetEquipBaptize()->OnOpenBaptizeSlot(ebor->param_1, ebor->param_2);
	}
	break;

	case Protocol::EQUIP_BAPTIZE_OPERA_TYPE_LOCK_OR_UNLOCK:
	{
		role->GetEquipBaptize()->OnLockOrUnLockSlot(ebor->param_1, ebor->param_2);
	}
	break;

	case Protocol::EQUIP_BAPTIZE_OPERA_TYPE_BEGIN_BAPTIZE:
	{
		role->GetEquipBaptize()->BeginBaptize(ebor->param_1, (0 != ebor->param_2) ? true : false, ebor->param_3);
	}
	break;
	}
}

void MessageHandler::OnCrossMizangBossBossInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCrossMiZangBossBossInfoReq *cscbir = (Protocol::CSCrossMiZangBossBossInfoReq *)msg;
	role->GetRoleCross()->OnCrossMizangBossReq(cscbir->req_type, cscbir->param_1, cscbir->param_2);
}

void MessageHandler::OnCrossYoumingBossBossInfoReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCrossYouMingBossBossInfoReq *cscbir = (Protocol::CSCrossYouMingBossBossInfoReq *)msg;
	role->GetRoleCross()->OnCrossYoumingBossReq(cscbir->req_type, cscbir->param_1, cscbir->param_2);
}

void MessageHandler::OnRALuckyWishOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSLuckyWishOpera *protocol = (Protocol::CSLuckyWishOpera *)msg;
	role->GetRoleActivity()->OnRALuckyWishOpera(protocol->type, protocol->param_1);
}

void MessageHandler::OnFetchDoubleReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSFetchDoubleRewardReq *csbuy = (Protocol::CSFetchDoubleRewardReq *)msg;

	switch (csbuy->fb_type)
	{
		case TeamManager::TEAM_TYPE_EQUIP_TEAM_FB:
		{
			role->GetFBEquip()->FetchDoubleReward(csbuy->opera_type);
		}
		break;

		case TeamManager::TEAM_TYPE_TEAM_TOWERDEFEND:
		{
			role->GetFBTowerDefend()->FetchDoubleReward(csbuy->opera_type);
		}
		break;
	}
}

void MessageHandler::OnPersonBossReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	if (role != nullptr)
	{
		role->GetRolePersonBossFb()->ReqPersonBossInfo();
	}
}

void MessageHandler::OnUpgradeCardBuyReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSUpgradeCardBuyReq *ucbq = (Protocol::CSUpgradeCardBuyReq *)msg;

	role->GetUpgradeCardBuy()->OnUpgradeCardBuyReq(ucbq->activity_id, ucbq->item_id);
}

void MessageHandler::OnGodmagicBossOperate(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSGodmagicBossInfoReq * gmbir = (Protocol::CSGodmagicBossInfoReq *)msg;

	BossFamilyManager::Instance().OnGodmagicBossReq(role, gmbir);
}

void MessageHandler::OnHuguozhiliReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSHuguozhiliReq *req = (Protocol::CSHuguozhiliReq *)msg;
	role->GetHuguozhili()->OnReq(req->req_type);
}

void MessageHandler::OnTodayThemeRewardReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSTodayThemeRewardReq *req = (Protocol::CSTodayThemeRewardReq *)msg;
	role->GetRoleActivity()->FetchTodayThemeReward(req->seq);
}

void MessageHandler::OnWorldAcquisitionLogReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSWorldAcquisitionLogReq *req = (Protocol::CSWorldAcquisitionLogReq *)msg;
	role->OnReqWorldAcquisition(req);
}

void MessageHandler::OnCrossGoalReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCrossGoalOperaReq * req = (Protocol::CSCrossGoalOperaReq *)msg;
	role->GetCrossGoal()->OperaReq(req);
}

void MessageHandler::OnBaizhanEquipOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSBaizhanEquipOpera *siu = (Protocol::CSBaizhanEquipOpera*)msg;
	role->GetBaizhanEquip()->OnOperaReq(siu->operate, siu->param1, siu->param2);
}

void MessageHandler::OnCSCrossEquipOpera(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSCrossEquipOpera *req = (Protocol::CSCrossEquipOpera*)msg;
	role->GetCrossEquip()->HandleMsg(req->req_type, req->param_1, req->param_2, req->param_3);
}

void MessageHandler::OnLittleHelperOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSLittleHelperOpera *opera = (Protocol::CSLittleHelperOpera*)msg;
	if (nullptr == opera)
	{
		return;
	}
	role->GetLittleHelper()->OperaReq(opera->req);
}
void MessageHandler::OnLittleHelperRepeatOpera(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSLittleHelperRepeatOpera *opera = (Protocol::CSLittleHelperRepeatOpera*)msg;
	if (nullptr == opera)
	{
		return;
	}
	role->GetLittleHelper()->OperaRepeatReq(opera->req_list, opera->count);
}
void MessageHandler::OnGuildRareLogItemReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	//Protocol::SCGuildRareLogRet *pro = (Protocol::SCGuildRareLogRet *)msg;
	GuildRareLogManager::Instance().SendAllLog(role);
}

void MessageHandler::OnTianShuXZFetchReward(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSTianShuXZFetchReward* fetch = (Protocol::CSTianShuXZFetchReward *)msg;
	if (Protocol::CS_TIANSHUXZ_SEQ_TYPE_INFO == fetch->type)
	{
		TianShuXunZhu::SendTianshuXunzhuInfo(role);
	}
	else if(Protocol::CS_TIANSHUXZ_SEQ_TYPE_FETCH == fetch->type)
	{
		TianShuXunZhu::OnTianshuXunzhuFetchReward(role, fetch->tianshu_type, fetch->seq);
	}
	
}

void MessageHandler::OnZhuanzhiEquipComposeReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSZhuanzhiEquipCompose *req = (Protocol::CSZhuanzhiEquipCompose *)msg;

	role->GetZhuanzhiEquip()->ComposeZhuanzhiEquip(req);
}

void MessageHandler::OnActiveEquipGradeSuitReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSEquipUplevelSuitActive *req = (Protocol::CSEquipUplevelSuitActive *)msg;
	
	role->GetEquipmentManager()->ActiveEquipGradeSuit(req->active_suit_level);
}

void MessageHandler::OnCrossChallengefieldReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCrossChallengeFieldOpera *req = (Protocol::CSCrossChallengeFieldOpera *)msg;

	switch (req->req_type)
	{
	case Protocol::CROSS_CHALLENGEFIELD_OPERA_REQ_OPPONENT_INFO:
	{
		CrossChallengeField::Instance().OnGetOpponentDetailInfo(role, (0 != req->param1));
	}
	break;

	case Protocol::CROSS_CHALLENGEFIELD_OPERA_REQ_RANK_INFO:
	{
		CrossChallengeField::Instance().OnGetRankInfo(role);
	}
	break;

	case Protocol::CROSS_CHALLENGEFIELD_OPERA_REQ_FETCH_REWARD:
	{

	}
	break;

	case Protocol::CROSS_CHALLENGEFIELD_OPERA_REQ_SELFT_INFO:
	{
		CrossChallengeField::Instance().OnGetChallengeUserInfo(role);
	}
	break;

	case Protocol::CROSS_CHALLENGEFIELD_OPERA_REQ_FIGHT:
	{
		CrossChallengeField::Instance().OnFightReq(role, req->param1, (0 != req->param2), req->param3, false);
	}
	break;

	case Protocol::CROSS_CHALLENGEFIELD_OPERA_REQ_REFRESH:
	{
		CrossChallengeField::Instance().OnResetOpponentReq(role);
	}
	break;

	case Protocol::CROSS_CHALLENGEFIELD_OPERA_REQ_READY:
	{
		SpecialCrossLocalChallengeField* special_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossLocalChallengeField(role->GetScene()->GetSceneID(), role->GetUID());
		if (NULL != special_logic)
		{
			special_logic->StartReadyFightCountdownReq(role);
		}
	}
	break;

	default:
		break;
	}
}

void MessageHandler::OnDiscountShopReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSDiscountShopBuy *req = (Protocol::CSDiscountShopBuy *)msg;

	role->GetMysteriousShopInMall()->OnDiscountShopBuy(req->seq, req->num);
}

void MessageHandler::OnMythOperaReq(const GSNetID &netid, Scene *scnen, Role *role, ObjID obj_id, void *msg, int length)
{
	//*************************************************************//
	// 需求变更：1、屏蔽神魂，领悟，萃取，合成，只保留篇章和共鸣。 //
	//			 2、取消虚拟背包，直接检测背包的神话道具。		   //
	//*************************************************************//

	Protocol::CSMythOpera *cmd = (Protocol::CSMythOpera *)msg;

	switch (cmd->opera_type)
	{
	case Protocol::MYTH_OPERA_TYPE_INFO:
	{
		role->GetMyth()->SendChpaterAllInfo();
	}
	break;

	case Protocol::MYTH_OPERA_TYPE_UPLEVEL:
	{
		role->GetMyth()->OnChpaterUplevel(cmd->param1);
	}
	break;

	case Protocol::MYTH_OPERA_TYPE_RESONANCE:
	{
		role->GetMyth()->OnResonance(cmd->param1);
	}
	break;

	default:
		break;
	}
}

void MessageHandler::OnZodiacReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSZodiacReq *zr = (Protocol::CSZodiacReq *)msg;
	role->GetZodiac()->ZodiacOpera(zr->req_type, zr->param1, zr->param2);
}

void MessageHandler::OnZodiacDecomposeReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, void * msg, int length)
{
	Protocol::CSZodiacDecomposeReq *zdr = (Protocol::CSZodiacDecomposeReq *)msg;

	if (zdr->decompose_num > 0)
	{
		role->GetZodiac()->ZodiacDecompose(zdr->decompose_num, zdr->decompose_backpack_index_list);
	}
}

void MessageHandler::OnTianshenhutiOperaReq(const GSNetID& netid, Scene* scene, Role* role, ObjID obj_id, void* msg, int length)
{
	Protocol::CSTianshenhutiReq* tianshen_opera = (Protocol::CSTianshenhutiReq *)msg;

	TianshenHuti* tianshenhuti = role->GetOtherModule<TianshenHuti>(ROLE_MODULE_TYPE_TIANSHENHUTI);
	if (nullptr == tianshenhuti)
	{
		return;
	}

	switch (tianshen_opera->req_type)
	{
	case Protocol::CS_TIANSHENHUTI_REQ_TYPE_PUT_ON:
	{
		tianshenhuti->PutOnEquip(tianshen_opera->param_1);
	}
	break;

	case Protocol::CS_TIANSHENHUTI_REQ_TYPE_TAKE_OFF:
	{
		tianshenhuti->TakeOffEquip(tianshen_opera->param_1);
	}
	break;

	case Protocol::CS_TIANSHENHUTI_REQ_TYPE_TRANSFORM:
	{
		tianshenhuti->TransformEquip(tianshen_opera->param_1, tianshen_opera->param_2, tianshen_opera->param_3);
	}
	break;

	case Protocol::CS_TIANSHENHUTI_REQ_TYPE_COMBINE:
	{
		tianshenhuti->CombineEquip(tianshen_opera->param_1, tianshen_opera->param_2, tianshen_opera->param_3, tianshen_opera->param_4);
	}
	break;

	case Protocol::CS_TIANSHENHUTI_REQ_TYPE_ROLL:
	{
		tianshenhuti->RollReq(tianshen_opera->param_1);
	}
	break;

	case Protocol::CS_TIANSHENHUTI_REQ_TYPE_COMBINE_ONE_KEY:
	{
		tianshenhuti->CombineEquipOneKey(tianshen_opera->param_1);
	}
	break;

	default:
		break;
	}
}

void MessageHandler::OnCrossHusongShuijingReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, void *msg, int length)
{
	Protocol::CSCrossHusongShuijingOpera * req = (Protocol::CSCrossHusongShuijingOpera*)msg;
	if (Protocol::CS_CROSS_HUSONG_SHUIJING_OPERA_INFO == req->req_type)
	{
		if (NULL != scene)
		{
			SpecialCrossHusongShuijing * special_logic = dynamic_cast<SpecialCrossHusongShuijing*>(scene->GetSpecialLogic());
			if (NULL != special_logic)
			{
				special_logic->SendRoleInfo(role);
			}
		}
	}
	else if (Protocol::CS_CROSS_HUSONG_SHUIJING_OPERA_COMMIT == req->req_type)
	{
		if (NULL != scene)
		{
			SpecialCrossHusongShuijing * special_logic = dynamic_cast<SpecialCrossHusongShuijing*>(scene->GetSpecialLogic());
			if (NULL != special_logic)
			{
				special_logic->OnCommit(role);
			}
		}
	}
	else if(Protocol::CS_CROSS_HUSONG_SHUIJING_OPERA_GATHER_INFO == req->req_type)
	{
		if (!CrossConfig::Instance().IsHiddenServer())
		{
			crossgameprotocal::CrossHusongShuijingGatherInfoReq  tmp_req;
			tmp_req.uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
			InternalComm::Instance().SendToCrossTransferToGame(crossgameprotocal::CROSS_TRANSFER_TYPE_HIDDEN, 0, (const char *)&tmp_req, sizeof(tmp_req));
		}
		else
		{
			if (NULL != scene)
			{
				SpecialCrossHusongShuijing * special_logic = dynamic_cast<SpecialCrossHusongShuijing*>(scene->GetSpecialLogic());
				if (NULL != special_logic)
				{
					special_logic->SyncShuijingInfo(role);
				}
			}
		}
	}
}