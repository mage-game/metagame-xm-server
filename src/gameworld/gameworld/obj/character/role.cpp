#include "role.h"
#include "gamecommon.h" 
#include "internalcomm.h"
#include "common/platform/system.h"

#include "servercommon/crossdef.hpp"
#include "servercommon/struct/roleinitparam.h"
#include "servercommon/struct/roleotherinitparam.hpp"
#include "servercommon/struct/skilllistparam.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/struct/hotkeylistparam.h"
#include "servercommon/struct/businessparam.h"
#include "servercommon/struct/roleallparam.hpp"
#include "servercommon/commondata.hpp"

#include "scene/aoi/zonematrix.h"
#include "scene/scenemanager.h"
#include "scene/scene.h"
#include "scene/map.h"

#include "obj/character/battlefieldshenshi.hpp"
#include "obj/otherobj/fallingitem.h"
#include "obj/otherobj/gatherobj.h"
#include "obj/otherobj/effectobj.h"
#include "obj/otherobj/bigcoinobj.hpp"
#include "obj/otherobj/trigger.h"
#include "obj/character/monster.h"
#include "obj/character/monsterhalo.hpp"
#include "obj/character/marryobj.hpp"
#include "obj/character/roleshadow.hpp"
#include "obj/otherobj/worldeventobj.h"

#include "protocal/msgfight.h"
#include "protocal/msgnpc.h"
#include "protocal/msgrole.h"
#include "protocal/msgscene.h"
#include "protocal/msgother.h"
#include "protocal/msgroleother.h"
#include "protocal/msgfb.h"
#include "protocal/msgitem.h"

#include "chat/chatmanager.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "servercommon/internalprotocal/gcprotocal.h"
#include "servercommon/internalprotocal/lgprotocal.h"
#include "servercommon/errornum.h"
#include "servercommon/noticenum.h"
#include "servercommon/servercommon.h"
#include "rmibackobjdef.h"

#include "global/fishpool/fishpoolmanager.hpp"
#include "global/team/team.hpp"
#include "global/offlineregister/offlineregistermanager.hpp"
#include "globalconfig/globalconfig.h"
#include "global/cross/crossusershadow.hpp"
#include "global/cross/crossuserregister.hpp"
#include "global/cross/crossactivity/crossrecordmanager.hpp"
#include "global/usercache/usercache.hpp"
#include "global/guild/guild.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/rank/rankmanager.hpp"
#include "global/team/teammanager.hpp"
#include "global/activity/activitymanager.hpp"
#include "global/camp/campmanager.hpp"
#include "global/specialactivity/specialactivitymanager.hpp"
#include "global/rank/topranker.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "global/challengefield/challengefield.hpp"
#include "global/crosschallengefield/crosschallengefield.hpp"
#include "global/mail/mailmanager.hpp"
#include "global/trade/p2ptrademanager.h"
#include "global/randactivity/randactivitymanager.hpp"
#include "global/worldevent/worldeventmanager.hpp"
#include "global/roleboss/rolebossmanager.h"
#include "global/bossfamily/bossfamilymanager.hpp"
#include "global/globalrole/lovetree/lovetree.hpp"
#include "global/globalrole/qingyuanglobal/qingyuanglobal.hpp"
#include "global/globalrole/commondata/commondataglobal.hpp"

#include "gamelog.h"
#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/guilddef.hpp"
#include "servercommon/campdef.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "servercommon/userprotocal/msgcrosscommon.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "servercommon/serverconfig/crosscommonconfig.hpp"

#include "item/itempool.h"
#include "equipment/equipmentfunction.h"

#include "scene/speciallogic/speciallogic.hpp"
#include "scene/scene.h"
#include "scene/scenemanager.h"
#include "scene/sceneregister/sceneregister.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "scene/worldshadow/worldshadow.hpp"
#include "scene/guildshadow/guildshadow.hpp"
#include "scene/marryshadow/marryshadow.h"

#include "scene/speciallogic/worldspecial/yaoshouguangchangmgr.hpp"
#include "scene/speciallogic/worldspecial/suoyaotamgr.hpp"
#include "scene/speciallogic/teamspecial/zhuaguimanager.hpp"
#include "scene/speciallogic/worldspecial/jinghuahusongmgr.hpp"
#include "scene/speciallogic/worldspecial/huangchenghuizhanmgr.hpp"
#include "scene/speciallogic/worldspecial/campdefendmanager.hpp"
#include "scene/speciallogic/worldspecial/specialcrossfishing.hpp"
#include "scene/speciallogic/guildspecial/specialguildstation.hpp"

#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effectpercentbuff.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectotherbuff.hpp"

#include "world.h"
#include "serverlogic.h"
#include "gstr.h"

#include "monster/monsterpool.h"
#include "friend/friendblessmanager.h"
#include "npc/transmit/fbmanager.hpp"

#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"
#include "other/queryhandler/queryhandler.hpp"
#include "other/monitor/systemmonitor.hpp"
#include "other/advancenotice/advancenoticeconfig.hpp"

#include "global/globalrole/globalrolemanager.hpp"
#include "global/globalrole/litttlepet/littlepet.hpp"

#include "scene/teamkeyregister/teamkeyregister.hpp"
#include "global/worldboss/worldbossbaseconfig.hpp"
#include "ai/monsterai/monsteraibase.hpp"
#include "servercommon/struct/xunzhangparam.hpp"
#include "global/tuodan/tuodanmanager.hpp"
#include "scene/speciallogic/worldspecial/specialxingzuoyiji.hpp"

#include "roleshadow.hpp"

#include "other/role_module.hpp"

#include "config/sharedconfig/sharedconfig.h"
#include "config/logicconfigmanager.hpp"
#include "config/restconfig.hpp"
#include "config/joinlimitconfig.hpp"
#include "config/fbsceneconfig.hpp"
#include "config/zhuanzhiconfig.hpp"
#include "config/flycorrectionconfig.hpp"
#include "config/activityconfig/guajiactivityconfig.hpp"
#include "config/expbottleconfig.hpp"
#include "config/spaceconfig.hpp"
#include "config/activityconfig/gongchengzhanconfig.hpp"
#include "other/vip/vipconfig.hpp"
#include "other/camp/campconfig.hpp"
#include "other/lieming/liemingconfig.hpp"
#include "other/goldvip/goldvipconfig.hpp"
#include "other/jinglingadvantage/jinglingadvantage.hpp"
#include "other/fb/rolepreciousboss.hpp"
#include "other/imgfuling/imgfuling.hpp"
#include "global/combineserveractivity/combineserverequip.hpp"
#include "role_module/i_role_module.hpp"
#include "other/jingjie/jingjieconfig.hpp"
#include "other/jinjietalent/jinjietalent.hpp"
#include "other/fb/rolefbarmordefend.hpp"
#include "other/heshenluoshu/heshenluoshu.hpp"
#include "global/bossshanggu/bossshanggumanager.hpp"
#include "other/seal/seal.hpp"
#include "servercommon/serverconfig/crossbossconfig.hpp"
#include "scene/speciallogic/worldspecial/specialcrossboss.hpp"
#include "other/jinjiesysreward/jinjiesysrewardmgr.hpp"
#include "other/shenqi/shenqi.hpp"
#include "other/molong/molong.h"
#include "gameworld/global/activity/activitymsgmanager.h"
#include "global/globalrole/spousehome/spousehome.hpp"
#include "skill/skillpool.hpp"
#include "servercommon/serverconfig/crossmizangbossconfig.hpp"
#include "scene/speciallogic/worldspecial/specialcrossyoumingboss.hpp"
#include "servercommon/serverconfig/crossyoumingbossconfig.hpp"
#include "scene/speciallogic/worldspecial/specialcrossmizangboss.hpp"
#include "config/agentadapterconfig.hpp"
#include "global/crossrandactivity/crossramanagerlocal.hpp"
#include "other/upgradecardbuy/upgradecardbuy.hpp"
#include "other/huguozhili/huguozhili.hpp"
#include "global/fightback/fightbackmanager.hpp"
#include "other/tianshuxunzhu/tianshuxunzhu.hpp"
#include "global/droplog/droplogmanager.hpp"
#include "global/cross/crossmanager.h"
#include "other/cross/rolecross.h"
#include "servercommon/serverconfig/husongshuijingconfig.hpp"
#include "scene/speciallogic/worldspecial/specialcrosshusongshuijing.hpp"

const static int FAKE_PRIVATE_CHAT_INTERVAL_S = 20 * 60; // 陌生人聊天发送间隔

Role::Role()
	: Character(OBJ_TYPE_ROLE), m_user_id(), m_uid(0), m_userip(0), m_netid(-1, -1), m_avatar(0), m_sex(FEMALE), m_camp(0), m_camp_post(0), m_prof(0),
	m_level(0), m_exp(0), m_max_exp(0), m_avator_timestamp(0), m_is_change_avatar(0), m_aoi_handle(-1), m_enter_scene_time(0), m_birth_pos(0, 0), 
	m_last_scene_id(0), m_last_scene_pos(0, 0), m_online_time(0), m_today_online_time(0), m_create_time(0), m_last_online_stati_time(0), m_last_today_online_stati_time(0), m_offline_day_num(0), m_lastday_online_time(0), m_forbid_talk_time(0),
	m_authority_type(0), m_attack_mode(ATTACK_MODE_PEACE), m_last_attack_me_user_id(INVALID_USER_ID), m_last_kill_me_time(0), m_last_guild_sos_time(0), m_update_last_time(0), m_next_save_time(0), m_last_msg_from_client_time(0), m_last_msg_from_client_timestamp(0), m_last_online_dayid(0), 
	m_last_reset_module_data_dayid(0), m_last_reset_module_data_timestamp(0), m_next_reset_module_data_timestamp(0), m_guild_id(INVALID_GUILD_ID), m_guild_post(0), m_guild_gongxian(0), m_guild_total_gongxian(0),
	m_evil(0),  m_xianhun(0), m_yuanli(0), m_nv_wa_shi(0), m_lingjing(0), m_chengjiu(0), m_hunli(0), m_role_status(ROLE_STATUS_CHARACTER), m_gather_obj_id(INVALID_OBJ_ID), m_gather_time(0), m_gather_count(0),
	m_rest_next_recovery_time(0), m_rest_next_reward_time(0), m_rest_partner_obj_id(INVALID_OBJ_ID), m_shuangxiu_next_intimacy_time(0), m_namecolor_last_dec_evil_time(0), m_name_color(NAME_COLOR_WHITE), m_move_mode(MOVE_MODE_NORMAL), m_move_mode_param(0),
	m_jump_src_pos(0, 0), m_jump_last_time(0), m_gm_change_maxhp(0), m_gm_change_gongji(0), m_gm_change_movespeed(0), m_gm_change_fangyu(0), m_gm_change_mingzhong(0), m_gm_change_shanbi(0), m_gm_change_baoji(0), m_gm_change_jianren(0), 
	m_gm_change_fujia_shanghai(0), m_gm_change_dikang_shanghai(0), m_gm_change_per_jingzhun(0), m_gm_change_per_baoji(0), m_gm_change_per_kangbao(0), m_gm_change_per_pofang(0), m_gm_change_per_mianshang(0), m_gm_change_constant_zengshang(0), m_gm_change_constant_mianshang(0), m_gm_change_capability_attr(0), 
	m_last_singlechat_monitor_time(0), m_last_worldchat_monitor_time(0), m_last_mail_monitor_time(0), m_mail_monitor_start_time(0), m_mail_send_count(0), m_has_send_all_info(false), 
	m_is_zombie(false), m_isforce_kickdelay(false), m_honour(0), m_is_mount_bianshen(false), m_mount_bianshen_time(0), m_is_reserve_marry(0), m_last_hunyan_bless_marry_start_time(0), m_last_marry_time(0), m_last_divorce_time(0),
	m_last_create_guild_time(0), m_speedup_hello_count(0), m_speedup_hello_total_count(0), m_first_speedup_hello_time(0), m_login_timestamp(0), m_check_week_camp_buff_next_timestamp(0), m_top_dps_flag(false), m_first_hit_flag(false), m_first_hit_have_died(false),
	m_need_send_attrinfo(false), m_cooldown_owe_time(0), m_tick_attack_count(0), m_tick_attack_recal_count(0), m_next_attack_extra_hurt_percent(0), m_wait_scene_frame_num(0), m_role_module(NULL), m_role_other_mgr(nullptr)
{
	memset(m_plat_name, 0, sizeof(PlatName));
	memset(m_guild_name, 0, sizeof(GuildName));

	memset(m_last_singlechat_monitor_msg, 0, sizeof(m_last_singlechat_monitor_msg));
	memset(m_last_worldchat_monitor_msg, 0, sizeof(m_last_worldchat_monitor_msg));
	memset(m_last_mail_monitor_subject, 0, sizeof(m_last_mail_monitor_subject));

	m_role_module = new RoleModule();
	m_role_other_mgr = new RoleOtherMgr();

	this->InitModules();
	this->SetSubSystemRole();
}

Role::Role(const GSNetID &_netid, const GameName _name)
	: Character(OBJ_TYPE_ROLE, _name), m_user_id(), m_uid(0), m_userip(0), m_netid(_netid), m_avatar(0), m_sex(FEMALE), m_camp(0), m_camp_post(0), m_prof(0), 
	m_level(0), m_exp(0), m_max_exp(0), m_avator_timestamp(0), m_is_change_avatar(0), m_aoi_handle(-1), m_enter_scene_time(0), m_birth_pos(0, 0),
	m_last_scene_id(0), m_last_scene_pos(0, 0), m_online_time(0), m_today_online_time(0), m_create_time(0), m_last_online_stati_time(0), m_last_today_online_stati_time(0), m_offline_day_num(0), m_lastday_online_time(0), m_forbid_talk_time(0),
	m_authority_type(0), m_attack_mode(ATTACK_MODE_PEACE), m_last_attack_me_user_id(INVALID_USER_ID), m_last_kill_me_time(0), m_last_guild_sos_time(0), m_update_last_time(0), m_next_save_time(0), m_last_msg_from_client_time(0), m_last_msg_from_client_timestamp(0), m_last_online_dayid(0),
	m_last_reset_module_data_dayid(0), m_last_reset_module_data_timestamp(0), m_next_reset_module_data_timestamp(0), m_guild_id(INVALID_GUILD_ID), m_guild_post(0), m_guild_gongxian(0), m_guild_total_gongxian(0),
	m_evil(0), m_xianhun(0), m_yuanli(0), m_nv_wa_shi(0), m_lingjing(0), m_chengjiu(0), m_hunli(0), m_role_status(ROLE_STATUS_CHARACTER), m_gather_obj_id(INVALID_OBJ_ID), m_gather_time(0), m_gather_count(0),
	m_rest_next_recovery_time(0), m_rest_next_reward_time(0), m_rest_partner_obj_id(INVALID_OBJ_ID), m_shuangxiu_next_intimacy_time(0), m_namecolor_last_dec_evil_time(0), m_name_color(NAME_COLOR_WHITE), m_move_mode(MOVE_MODE_NORMAL), m_move_mode_param(0),
	m_jump_src_pos(0, 0), m_jump_last_time(0), m_gm_change_maxhp(0), m_gm_change_gongji(0), m_gm_change_movespeed(0), m_gm_change_fangyu(0), m_gm_change_mingzhong(0), m_gm_change_shanbi(0), m_gm_change_baoji(0), m_gm_change_jianren(0),
	m_gm_change_fujia_shanghai(0), m_gm_change_dikang_shanghai(0), m_gm_change_per_jingzhun(0), m_gm_change_per_baoji(0), m_gm_change_per_kangbao(0), m_gm_change_per_pofang(0), m_gm_change_per_mianshang(0), m_gm_change_constant_zengshang(0), m_gm_change_constant_mianshang(0), m_gm_change_capability_attr(0),
	m_last_singlechat_monitor_time(0), m_last_worldchat_monitor_time(0), m_last_mail_monitor_time(0), m_mail_monitor_start_time(0), m_mail_send_count(0), m_has_send_all_info(false),  
	m_is_zombie(false), m_isforce_kickdelay(false), m_honour(0), m_is_mount_bianshen(false), m_mount_bianshen_time(0), m_is_reserve_marry(0), m_last_hunyan_bless_marry_start_time(0), m_last_marry_time(0), m_last_divorce_time(0),  
	m_last_create_guild_time(0), m_speedup_hello_count(0), m_speedup_hello_total_count(0), m_first_speedup_hello_time(0), m_login_timestamp(0), m_check_week_camp_buff_next_timestamp(0), m_top_dps_flag(false), m_first_hit_flag(false), m_first_hit_have_died(false), 
	m_need_send_attrinfo(false), m_cooldown_owe_time(0), m_tick_attack_count(0), m_tick_attack_recal_count(0), m_next_attack_extra_hurt_percent(0), m_wait_scene_frame_num(0), m_role_module(NULL), m_role_other_mgr(nullptr)
{
	memset(m_plat_name, 0 , sizeof(PlatName));
	memset(m_guild_name, 0, sizeof(GuildName));

	memset(m_last_singlechat_monitor_msg, 0, sizeof(m_last_singlechat_monitor_msg));
	memset(m_last_worldchat_monitor_msg, 0, sizeof(m_last_worldchat_monitor_msg));
	memset(m_last_mail_monitor_subject, 0, sizeof(m_last_mail_monitor_subject));

	m_role_module = new RoleModule();
	m_role_other_mgr = new RoleOtherMgr();

	this->InitModules();
	this->SetSubSystemRole();
}

void Role::InitModules()
{
	//m_role_module_list.push_back(new IRoleModule());
}

Role::~Role()
{
	delete m_role_module;
	delete m_role_other_mgr;

	//for (IRoleModule * mod : m_role_module_list)
	//{
	//	delete mod;
	//}
}

void Role::SetSubSystemRole()
{
	m_role_other_mgr->SetRole(this);
	//for (IRoleModule *module : m_role_module_list)
	//{
	//	module->SetRole(this);
	//}

	m_role_module->knapsack.SetRole(this);
	m_role_module->newtask_manager.SetRole(this);

	m_role_module->task_recorder.SetRole(this);
	m_role_module->hotkey_recorder.SetRole(this);
	m_role_module->friend_manager.SetRole(this);

	m_role_module->team_info.SetRole(this);
	m_role_module->rank_shadow.SetRole(this);
	m_role_module->mentality.SetRole(this);
	m_role_module->vip.SetRole(this);
	m_role_module->role_chestshop.SetRole(this);
	m_role_module->roleshop.SetRole(this);
	m_role_module->capablity.SetRole(this);
	m_role_module->daycounter.SetRole(this);
	m_role_module->zhanshendian.SetRole(this);
	m_role_module->title_manager.SetRole(this);
	m_role_module->active_degree.SetRole(this);
	m_role_module->welfare.SetRole(this);
	m_role_module->role_hang_up.SetRole(this);
	m_role_module->turntable.SetRole(this);
	
	m_role_module->role_evaluate.SetRole(this);
	m_role_module->role_status_manager.SetRole(this);
	m_role_module->monitor.SetRole(this);
	m_role_module->sublock.SetRole(this);

	m_role_module->mount_manager.SetRole(this);
	m_role_module->fightmount_manager.SetRole(this);
	m_role_module->stone_manager.SetRole(this);
	m_role_module->role_activity.SetRole(this);
	m_role_module->equip_suit.SetRole(this);
	m_role_module->shen_zhuang.SetRole(this);

	m_role_module->daily_task.SetRole(this);
	m_role_module->husong_task.SetRole(this);
	m_role_module->equip_fb.SetRole(this);
	m_role_module->maze.SetRole(this);
	m_role_module->wing.SetRole(this);
	m_role_module->halo.SetRole(this);
	m_role_module->shengong.SetRole(this);
	m_role_module->shenyi.SetRole(this);
	m_role_module->footprint.SetRole(this);
	m_role_module->cloak_manager.SetRole(this);
	m_role_module->guild_task.SetRole(this);

	m_role_module->yaojiang.SetRole(this);
	m_role_module->special_appearance.SetRole(this);
	m_role_module->dating_task.SetRole(this);
	m_role_module->guild_xianshu.SetRole(this);
	m_role_module->role_supply.SetRole(this);
	m_role_module->story_fb.SetRole(this);
	m_role_module->mojie_fb.SetRole(this);
	m_role_module->challenge_fb.SetRole(this);
	m_role_module->role_phase_fb.SetRole(this);
	m_role_module->patafb.SetRole(this);
	m_role_module->vipfb.SetRole(this);
	m_role_module->convert_record.SetRole(this);
	m_role_module->discount_buy.SetRole(this);
	m_role_module->zero_gift.SetRole(this);
	m_role_module->month_card.SetRole(this);
	m_role_module->wabao.SetRole(this);
	m_role_module->cisha_task.SetRole(this);
	m_role_module->jilian.SetRole(this);
	m_role_module->qingyuan.SetRole(this);
	m_role_module->baby.SetRole(this);
	m_role_module->trade_register.SetRole(this);
	m_role_module->touzijihua.SetRole(this);
	m_role_module->roll_money.SetRole(this);
	m_role_module->role_world_event.SetRole(this);
	m_role_module->common_activity_unit_mgr.SetRole(this);
	m_role_module->role_goal.SetRole(this);
	m_role_module->shizhuang.SetRole(this);
	m_role_module->role_cross.SetRole(this);
	m_role_module->role_guild.SetRole(this);
	m_role_module->shengwang_mgr.SetRole(this);
	m_role_module->chengjiu_mgr.SetRole(this);
	m_role_module->dailyfb.SetRole(this);
	m_role_module->lifeskill_mgr.SetRole(this);
	m_role_module->paohuan_task.SetRole(this);
	m_role_module->gongxun_mgr.SetRole(this);
	m_role_module->tuhaojin.SetRole(this);
	m_role_module->bigchatface.SetRole(this);
	m_role_module->pet.SetRole(this);
	m_role_module->multi_mount.SetRole(this);
	m_role_module->personalize_window.SetRole(this);
	m_role_module->xunzhang_manager.SetRole(this);
	m_role_module->zhibao_manager.SetRole(this);
	m_role_module->shenbing_manager.SetRole(this);
	m_role_module->mojie_manager.SetRole(this);
	m_role_module->cardzu.SetRole(this);
	m_role_module->lieming_mgr.SetRole(this);
	m_role_module->huashen.SetRole(this);
	m_role_module->magic_card.SetRole(this);
	m_role_module->mitama.SetRole(this);
	m_role_module->xiannvshouhu_manager.SetRole(this);
	m_role_module->jinglingfazhen_manager.SetRole(this);
	m_role_module->jinglingguanghuan_manager.SetRole(this);
	m_role_module->zhuansheng_equip.SetRole(this);
	m_role_module->chinese_zodiac.SetRole(this);
	m_role_module->friend_gift_manager.SetRole(this);
	m_role_module->gold_vip.SetRole(this);
	m_role_module->famous_man.SetRole(this);
	m_role_module->followboss_manager.SetRole(this);
	m_role_module->shenge_system.SetRole(this);

	m_role_module->equip_manager.SetRole(this);
	m_role_module->friend_exp_bottle.SetRole(this);
	m_role_module->jingling_mgr.SetRole(this);
	m_role_module->magical_precious_manager.SetRole(this);
	m_role_module->mysterious_shop_in_mall.SetRole(this);
	m_role_module->towerdefend_fb.SetRole(this);
	m_role_module->runesystem_manger.SetRole(this);
	m_role_module->shenzhou_weapon_manger.SetRole(this);
	m_role_module->xiannv_manager.SetRole(this);
	m_role_module->shenyin.SetRole(this);
	m_role_module->element_heart.SetRole(this);
	m_role_module->shengxing_zhuli.SetRole(this);
	m_role_module->upgrade_rush.SetRole(this);
	m_role_module->fabao.SetRole(this);
	m_role_module->role_challenge_field.SetRole(this);
	m_role_module->jingjie.SetRole(this);
	m_role_module->talent.SetRole(this);
	m_role_module->jinjie_talent.SetRole(this);
	m_role_module->role_am_fb.SetRole(this);
	m_role_module->heshen_luoshu.SetRole(this);
	m_role_module->shengqi.SetRole(this);
	m_role_module->m_imp_guard.SetRole(this);
	m_role_module->boss_card.SetRole(this);
	m_role_module->role_zhuanzhi.SetRole(this);
	m_role_module->zhuanzhi_task.SetRole(this);
	m_role_module->yaoshi_manager.SetRole(this);
	m_role_module->toushi_manager.SetRole(this);
	m_role_module->qilinbi_manager.SetRole(this);
	m_role_module->mask_manager.SetRole(this);
	m_role_module->zhuanzhi_equip.SetRole(this);
	m_role_module->seal.SetRole(this);
	m_role_module->dressing_room_mgr.SetRole(this);
	m_role_module->role_big_small_goal.SetRole(this);
	m_role_module->shen_qi.SetRole(this);
	m_role_module->mojie_skill.SetRole(this);
	m_role_module->redequip_collect.SetRole(this);
	m_role_module->orang_equip_collect.SetRole(this);
	m_role_module->molong.SetRole(this);
	m_role_module->xianzunka.SetRole(this);
	m_role_module->equip_baptize.SetRole(this);
	m_role_module->role_personboss_fb.SetRole(this);
	m_role_module->m_greate_soldier.SetRole(this);
	m_role_module->lottery_count_module.SetRole(this);
	m_role_module->cross_rand_activity.SetRole(this);
	m_role_module->upgrade_card_buy.SetRole(this);
	m_role_module->huguozhili.SetRole(this);
	m_role_module->baizhan_equip.SetRole(this);
	m_role_module->little_helper.SetRole(this);
	m_role_module->myth.SetRole(this);
	m_role_module->zodiac.SetRole(this);
	m_role_module->cross_equip.SetRole(this);
}

void Role::Init(bool is_micro_pc, 
				int plat_spid,
				PlatName pname, 
				const UserID &user_id,
				const RoleInitParam &param,
				const RoleOtherInitParam &other_p,
				const ItemListParam &itemlist,
				const HotkeyListParam &hotkey,
				const FriendListParam &friends,
				const BlacklistsParam &balcklists_param,
				const EnemyListParam &enemy,
				const BusinessListParam &business_param,
				const SkillListParam &skill_param,
				const DailyFindListParam &daily_find_param)
{
	m_role_module->common_data_param = other_p.common_data_param;
	m_role_module->history_cap_param = other_p.history_camp_param;
	const OtherModuleDataParam &other_module_param = other_p.other_module_param;

	UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(m_role_module->common_data_param.lover_uid);
	if (NULL != user_node)
	{
		user_node->GetName(m_role_module->common_data_param.lover_name);
	}

	m_role_module->common_data_param.plat_spid = plat_spid;
	m_user_id = user_id;
	m_uid = UidToInt(m_user_id);
	m_avatar = param.avatar;
	m_sex = param.sex;
	m_camp = param.camp;
	m_camp_post = param.camp_post;
	m_prof = param.prof >= PROF_TYPE_MAX ? ((param.prof % PROF_TYPE_PROF_ZHUANZHI_BASE) + (MAX_PROF_LEVEL * PROF_TYPE_PROF_ZHUANZHI_BASE)) : param.prof;    // 避免开发服满转职职业报错，清档前可改
	m_level = param.level;
	m_avator_timestamp = param.avatar_timestamp;
	m_is_change_avatar = param.is_change_avatar;

	m_exp = param.exp;
	m_max_exp = param.max_exp;
	if (m_max_exp <= 0) m_max_exp = GLOBALCONFIG->GetRoleMaxExp(m_level);
	if (m_exp >= m_max_exp) m_exp = m_max_exp - 1;

	Posi pos = Posi(param.scene_x, param.scene_y);
	if (MarryShadow::Instance().IsMarryUser(m_user_id))
	{
		MarryShadow::Instance().GetMarryobjPos(&pos.x, &pos.y);
	}

	SetPos(pos);

	time_t now = EngineAdapter::Instance().Time();

	m_last_scene_id = param.last_scene_id;
	m_last_scene_pos.Init(param.last_scene_x, param.last_scene_y);
	m_online_time = param.online_time;
	m_today_online_time = param.today_online_time;
	m_lastday_online_time = param.lastday_online_time;
	m_create_time = (time_t)param.create_time;
	m_last_online_stati_time = now;
	m_last_today_online_stati_time = now;
	m_forbid_talk_time = (time_t)param.forbid_talk_time;
	m_last_online_dayid = param.last_online_dayid;
	m_last_reset_module_data_dayid = other_module_param.last_reset_role_module_data_dayi;
	m_last_reset_module_data_timestamp = other_module_param.last_reset_role_module_data_timestamp;

	unsigned int now_dayid = static_cast<unsigned int>(EngineAdapter::Instance().DayID());
	
	if (0 == m_last_reset_module_data_dayid) m_last_reset_module_data_dayid = now_dayid;
	if (0 == m_last_reset_module_data_timestamp) m_last_reset_module_data_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	F_STRNCPY(m_plat_name, pname, sizeof(m_plat_name));

	const Attribute ROLE_GLOBAL_CD = GLOBALCONFIG->GetRoleGlobalCooldown();

	CharIntAttrs int_attribute;
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GLOBAL_COOLDOWN, ROLE_GLOBAL_CD - 50); // 容错50毫秒
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP, param.hp);
	int_attribute.SetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP, param.mp);
	Character::Init(int_attribute, skill_param, other_p.skill_other_param, other_p.effect_buff_len, other_p.effect_buff);

	int knapsack_param_count = 0;
	const ItemListParam::ItemListData *knapsack_param_list[ItemNamespace::MAX_GRID_NUM] = {0};

	int equip_param_count = 0;
	const ItemListParam::ItemListData *equip_param_list[ItemNamespace::MAX_GRID_NUM] = {0};

	for (int i = 0; i < itemlist.count; i++) // 将属于背包和装备的数据指针分别筛选出来
	{
		if (itemlist.item_list[i].index < ItemNamespace::EQUIPMENT_INDEX_BEGIN)
		{
			knapsack_param_list[knapsack_param_count ++] = &itemlist.item_list[i];
		}
		else
		{
			equip_param_list[equip_param_count ++] = &itemlist.item_list[i];
		}
	}

	m_role_module->knapsack.Init(this, knapsack_param_count, knapsack_param_list, param.knapsack_valid_num, param.storage_valid_num, other_p.other_module_param.knapsack_grid_auto_add_count,
		param.gold, param.gold_bind, param.coin, param.coin_bind);
	m_role_module->mojie_fb.Init(this, other_p.mojiefb_param);
	m_role_module->friend_gift_manager.Init(this, other_p.friend_gift_param);
	m_role_module->gold_vip.Init(this, other_p.gold_vip_param);
	

	m_role_module->task_recorder.Init(this, other_p.taskrecord_buff_len, other_p.taskrecord_buff);
	m_role_module->newtask_manager.Init(this, other_p.task_param);
	m_role_module->daily_task.Init(this, other_module_param.daily_task_can_fetch_flag, other_module_param.daily_task_fetch_flag, other_module_param.daily_task_exp_ball_valid_time, other_module_param.daily_task_exp_ball_exp);

	m_role_module->hotkey_recorder.Init(this, hotkey);
	m_role_module->friend_manager.Init(this, friends, balcklists_param, enemy, param.all_give_flower_num, param.day_give_flower_num, param.all_charm, other_module_param.week_add_charm, other_module_param.day_add_charm);
	m_role_module->zhanshendian.Init(this, other_module_param.zhanshendian_pass_level, other_module_param.zhanshendian_yesterday_pass_level, 
		other_module_param.m_zhanshendian_fetch_day_reward_timestamp);
	m_role_module->shizhuang.InitParam(this, other_p.shizhuang_param);
	m_role_module->patafb.Init(this, other_module_param.patafb_pass_level, other_module_param.patafb_today_level);
	m_role_module->rank_shadow.Init(this);
	m_role_module->team_info.Init(this, param.team_index, param.team_seq, param.is_auto_join_team);

	m_role_module->vip.Init(this, other_p.vip_param);
	m_role_module->capablity.Init(this, param.max_capability);
	m_role_module->daycounter.Init(this, param.day_count_data);
	m_role_module->role_status_manager.Init(this);
	m_role_module->welfare.Init(this, other_p.welfare_param, daily_find_param);
	m_role_module->mentality.Init(this, other_p.mentality_param);
	m_role_module->sublock.Init(this, param.sublock_pw, param.sublock_has_lock, param.sublock_today_error_times);
	m_role_module->mount_manager.Init(this, other_p.mount_param);
	m_role_module->fightmount_manager.Init(this, other_p.fight_mount_param);
	m_role_module->stone_manager.Init(this, other_p.stone_param);
	m_role_module->guild_xianshu.Init(this, other_p.guild_xianshu_param);
	m_role_module->wing.Init(this, other_p.wing_param);
	m_role_module->halo.Init(this, other_p.halo_param);
	m_role_module->shengong.Init(this, other_p.shengong_param);
	m_role_module->shenyi.Init(this, other_p.shenyi_param);
	m_role_module->footprint.Init(this, other_p.footprint_param);
	m_role_module->cloak_manager.Init(this, other_p.cloak_param);
	m_role_module->husong_task.Init(this, other_p.husong_param);
	m_role_module->role_activity.Init(this, other_p.roleactivity_param);

	m_role_module->equip_fb.Init(this, other_p.newequipfb_param, other_p.equipfb_param.team_equipfb_info);
	m_role_module->guild_task.Init(this, other_p.guild_task_param);
	m_role_module->role_supply.Init(this, other_p.role_supply_param);
	m_role_module->story_fb.Init(this, other_p.storyfb_param);
	
	m_role_module->chinese_zodiac.InitParam(this, other_p.chinese_zodiac_param);
	m_role_module->challenge_fb.Init(this, other_p.challengefb_param);
	m_role_module->role_phase_fb.Init(this, other_p.phase_fb_param);
	m_role_module->vipfb.Init(this, other_p.vipfb_param);
	m_role_module->convert_record.Init(this, other_p.convert_record_param);
	m_role_module->discount_buy.Init(this, other_p.discount_buy_param);
	m_role_module->zero_gift.Init(this, other_p.zero_gift_param);
	m_role_module->qingyuan.Init(this, other_p.qingyuan_param, param.m_marry_type, param.m_lover_id, param.m_shici_type, param.m_marry_req_id, param.m_is_sure);
	m_role_module->baby.InitParam(this, other_p.baby_param);
	m_role_module->shenge_system.InitParam(this, other_p.shenge_system_param);
	m_role_module->month_card.Init(this, other_module_param.monthcard_active_timestamp, other_module_param.monthcard_last_day,
		other_module_param.monthcard_next_reward_day_idx, other_module_param.monthcard_buy_times);
	m_role_module->wabao.Init(this, other_module_param.wabao_scene_id, other_module_param.wabao_pos_x, other_module_param.wabao_pos_y,
		other_module_param.wabao_baotu_count, other_module_param.wabao_reward_type, other_module_param.baotu_fetch_flag, other_module_param.shouhuzhe_time);

	m_role_module->jilian.Init(this, other_module_param.jilian_type, other_module_param.jilian_is_working, 
		other_module_param.jilian_progress, other_module_param.jilian_once_join, other_module_param.jilian_qte_succ_times);

	m_role_module->touzijihua.Init(this, other_module_param.touzijihua_active_plan_0, other_module_param.touzijihua_active_plan_1, other_module_param.touzijihua_active_plan_2, other_module_param.touzijihua_active_plan_3,
		other_module_param.touzhijihua_active_highest_plan,	other_module_param.touzijihua_plan_0_reward_flag, other_module_param.touzijihua_plan_1_reward_flag, other_module_param.touzijihua_plan_2_reward_flag, other_module_param.touzijihua_plan_3_reward_flag,
		other_module_param.new_touzijihua_buy_time, other_module_param.new_touzijihua_reward_flag, other_module_param.new_touzijihua_vip_reward_flag, 
		other_module_param.new_touzijihua_gold_bind_flag, other_module_param.new_touzijihua_first_reward_flag, other_module_param.foundation_next_reward_index,
		other_module_param.touzijihua_active_fb_plan,other_module_param.touzijihua_fb_plan_reward_flag,other_module_param.touzijihua_active_boss_plan,other_module_param.touzijihua_boss_plan_reward_flag,other_module_param.touzijihua_kill_boss_num, 
		other_module_param.touzijihua_kill_shenyun_boss_num, other_module_param.touzijihua_active_shenyun_boss_plan, other_module_param.touzijihua_shenyun_boss_plan_reward_flag);

	m_role_module->roll_money.Init(this, other_module_param.roll_money_gold_fetch_reward_times, other_module_param.roll_money_coin_fetch_reward_times,
		other_module_param.roll_money_gold_roll_times, other_module_param.roll_money_coin_roll_times, other_module_param.roll_money_gold_num_0, 
		other_module_param.roll_money_gold_num_1, other_module_param.roll_money_gold_num_2, other_module_param.roll_money_coin_num_0, 
		other_module_param.roll_money_coin_num_1, other_module_param.roll_money_coin_num_2, other_module_param.roll_money_coin_num_3,
		other_module_param.roll_money_coin_num_4, other_module_param.roll_money_coin_num_5, other_module_param.roll_money_coin_num_6, 
		other_module_param.roll_money_already_roll_gold_num);

	m_role_module->turntable.Init(this, other_module_param.turntable_lucky_value);
	m_role_module->maze.Init(this,other_p.maze_param);
	m_role_module->yaojiang.Init(this);

	m_role_module->role_world_event.Init(this, other_module_param.world_event_fetch_reward_flag, other_module_param.world_event_find_npc_score, other_module_param.world_event_kill_rand_boss_score,
		other_module_param.world_event_shenmozhixi_score, other_module_param.world_event_xukongzhimen_score, other_module_param.world_event_quanmintafang_score, other_module_param.world_event_jiqingpk_score,
		other_module_param.world_event_tiannvsanhua_score, other_module_param.world_event_find_npc_flag, other_module_param.world_event_light_word_flag, other_module_param.world_event_curr_find_npc_id);

	m_role_module->common_activity_unit_mgr.Init(this, other_p.common_activity_param);

	m_role_module->role_cross.InitParam(this, other_p.role_cross_param);
	m_role_module->role_guild.Init(this, other_p.role_guild_param);
	m_role_module->shengwang_mgr.Init(this, other_p.shengwang_param);
	m_role_module->chengjiu_mgr.Init(this, other_p.chengjiu_param);
	m_role_module->dailyfb.Init(this, other_p.dailyfb_param);
	m_role_module->lifeskill_mgr.Init(this, other_p.lifeskill_param);
	m_role_module->paohuan_task.Init(this);
	m_role_module->tuhaojin.Init(this, other_p.tuhaojin_param);
	m_role_module->bigchatface.Init(this, other_p.bigchatface_param);
	m_role_module->pet.Init(this, other_p.pet_param);
	m_role_module->multi_mount.Init(this, other_p.multi_mount_param);
	m_role_module->personalize_window.InitParam(this, other_p.personalize_window_param);
	m_role_module->xunzhang_manager.Init(this, other_p.xunzhang_param);
	m_role_module->zhibao_manager.Init(this, other_p.zhibao_param);
	m_role_module->shenbing_manager.Init(this, other_p.shenbing_param);
	m_role_module->mojie_manager.Init(this, other_p.mojie_param);
	m_role_module->equip_suit.Init(this, other_p.equipsuit_param);
	m_role_module->shen_zhuang.Init(this, other_p.shenzhuang_param);
	m_role_module->cardzu.InitParam(this, other_p.cardzu_param);
	//m_role_module->lieming_mgr.Init(this, other_p.lieming_param, other_p.lieming_bag_param);
	m_role_module->lieming_mgr.Init(this, other_p.lieming_param2);
	m_role_module->huashen.Init(this, other_p.huashen_param);
	m_role_module->magic_card.InitParam(this, other_p.magic_card_param);
	m_role_module->mitama.InitParam(this, other_p.mitama_param);
	m_role_module->xiannvshouhu_manager.Init(this, other_p.xiannv_shouhu_param);
	m_role_module->jinglingfazhen_manager.Init(this, other_p.jingling_fazhen_param);
	m_role_module->jinglingguanghuan_manager.Init(this, other_p.jingling_guanghuan_param);
	m_role_module->zhuansheng_equip.InitParam(this, other_p.zhuansheng_equip_param);
	m_role_module->followboss_manager.Init(this, other_p.follow_boss_param);
	m_role_module->role_goal.Init(this, other_p.role_goal_param);

	m_role_module->equip_manager.Init(this, equip_param_count, equip_param_list, m_role_module->common_data_param.cross_equip_itemid, m_role_module->common_data_param.cross_equip_gain_timestamp, other_p.equipment_param);
	m_role_module->friend_exp_bottle.Init(this, other_p.friend_exp_bottle_param); 
	m_role_module->jingling_mgr.Init(this, other_p.jingling_param);
	m_role_module->magical_precious_manager.Init(this, other_p.magical_precious_param);
	m_role_module->mysterious_shop_in_mall.Init(this, other_p.mysterious_shop_in_mall_param, other_p.discountbuy_param);
	m_role_module->towerdefend_fb.Init(this, other_p.towerdefend_param);
	m_role_module->shenzhou_weapon_manger.InitParam(this, other_p.shenzhou_weapon_param);
	m_role_module->runesystem_manger.InitParam(this, other_p.rune_system_param);
	m_role_module->xiannv_manager.Init(this, other_p.xiannv_param);
	m_role_module->tuitu_fb.Init(this, other_p.tuitu_fb_param);
	m_role_module->precious_boss.Init(this, other_p.precious_boss_param);
	m_role_module->shenshou.Init(this, other_p.shenshou_param);
	m_role_module->img_fuling.Init(this, other_p.img_fuling_param);
	m_role_module->csa_equip.Init(this, other_p.csa_equip_param);
	m_role_module->multi_mount.Init2(this, other_p.couple_mount_param);
	m_role_module->shenyin.InitParam(this, other_p.shenyin_param);
	m_role_module->element_heart.InitParam(this, other_p.element_heart_param);
	m_role_module->shengxing_zhuli.InitParam(other_p.shengxing_zhuli_param);
	m_role_module->upgrade_rush.Init(this, other_module_param.upgrade_return_end_time_list);
	m_role_module->fabao.InitParam(this, other_p.fabao_param);
	m_role_module->feixian_manager.Init(this, other_p.feixian_param);
	m_role_module->talent.InitParam(this, other_p.talent_param);
	m_role_module->jinjie_talent.InitParam(this, other_p.jinjie_talent_param);
	m_role_module->role_am_fb.Init(this, other_p.armor_fb_param);
	m_role_module->heshen_luoshu.Init(this, other_p.heshen_luoshu_param);
	m_role_module->shengqi.Init(this, other_p.shengqi_param);
	m_role_module->m_imp_guard.Init(this);
	m_role_module->boss_card.Init(this, other_p.bosscard_param);
	m_role_module->zhuanzhi_task.Init(this, other_p.task_zhuanzhi_param);
	m_role_module->role_zhuanzhi.Init(this, other_module_param.zhuanzhi_tianming, other_module_param.zhuanzhi_canglong, other_module_param.zhuanzhi_shengxiao, other_module_param.zhuanzhi_yuanqi, other_module_param.zhuanzhi_shenqi, other_module_param.zhuanzhi_xingxiu);
	m_role_module->cross_role_global.InitParam(this, other_p.cross_global_data_param);
	m_role_module->yaoshi_manager.Init(this, other_p.yaoshi_param);
	m_role_module->toushi_manager.Init(this, other_p.toushi_param);
	m_role_module->qilinbi_manager.Init(this, other_p.qilinbi_param);
	m_role_module->mask_manager.Init(this, other_p.mask_param);
	m_role_module->zhuanzhi_equip.Init(this, other_p.zhuanzhi_equip_param);
	m_role_module->seal.Init(this, other_p.seal_param);
	m_role_module->dressing_room_mgr.InitParam(other_p.dressing_room_param);
	m_role_module->upgrade_manager.Init(this, other_p.upgrade_manager_param);
	m_role_module->role_big_small_goal.InitParam(this, other_p.role_big_small_goal_param);
	m_role_module->shen_qi.InitParam(other_p.shenqi_param);
	m_role_module->qingyuan.Init(other_p.profess_param);
	m_role_module->redequip_collect.InitParam(this, other_p.red_equip_collect_param);
	m_role_module->orang_equip_collect.InitParam(this, other_p.orange_equip_collect_param);
	m_role_module->molong.Init(other_module_param.molong_curr_loop, other_module_param.molong_accumulate_consume_gold, other_module_param.molong_today_consume_gold,
		other_module_param.molong_total_move_step, other_module_param.molong_today_move_step, other_module_param.molong_rank_grade, other_module_param.molong_rank_cumulate_gold);
	m_role_module->xianzunka.InitParam(this, other_p.xianzunka_param);
	m_role_module->equip_baptize.InitParam(this, other_p.equipbaptize_param);
	m_role_module->role_personboss_fb.Init(this, other_p.personboss_param);
	m_role_module->m_greate_soldier.InitParam(other_p.greate_soldier_param);
	m_role_module->lottery_count_module.Init(this, other_p.role_lottery_count_param);
	m_role_module->upgrade_card_buy.InitParam(other_p.upgrade_card_buy_param);
	m_role_module->huguozhili.Init(other_p.huguozhili_param);
	m_role_module->baizhan_equip.Init(this, other_p.baizhan_equip_param);
	m_role_module->myth.InitParam(this, other_p.myth_param);
	m_role_module->zodiac.Init(this, other_p.zodiac_param);
	m_role_module->cross_equip.InitParam(other_p.cross_equip_param);

	m_role_other_mgr->InitParam(other_p);
	//for (IRoleModule *module : m_role_module_list)
	//{
	//	module->InitParam(other_p);
	//}

	static const unsigned int ROLE_SAVE_TIME = GLOBALCONFIG->GetKeyConfig_RoleSaveTime();
	m_next_save_time = now + ROLE_SAVE_TIME;

	m_last_msg_from_client_time = EngineAdapter::Instance().GetGameTime();
	m_last_msg_from_client_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	m_authority_type = param.authority_type;
	m_attack_mode = param.attack_mode;
	
	{
		m_guild_id = param.guild_id;
		F_STRNCPY(m_guild_name, param.guild_name, sizeof(m_guild_name));
		m_guild_post = param.guild_post;
		m_guild_gongxian = param.guild_gongxian;
		m_guild_total_gongxian = param.guild_total_gongxian;
	}

	m_role_module->title_manager.Init(this, other_p.title_param); //使用这套称号机制而获得称号的都需写在前面，第一公会的称号需要判断guild_id，所以需要放在m_guild_id初始化之后
	
	m_evil = param.evil;
	m_last_kill_me_time = param.last_kill_me_time;
	m_yuanli = param.yuanli;
	m_nv_wa_shi = param.nv_wa_shi;
	m_lingjing = param.lingjing;
	m_chengjiu = param.chengjiu;
	m_hunli = param.fenchen;

	if (m_evil > 0)	m_namecolor_last_dec_evil_time = now;

	m_move_mode = param.move_mode;
	m_move_mode_param = param.move_mode_param;

	m_gm_change_maxhp = param.gm_change_maxhp;
	m_gm_change_gongji = param.gm_change_gongji;
	m_gm_change_movespeed = param.gm_change_movespeed;
	m_gm_change_fangyu = param.gm_change_fangyu;
	m_gm_change_mingzhong = param.gm_chnage_mingzhong;
//	m_gm_change_shanbi = param.gm_chnage_shanbi;
// 	m_gm_change_baoji = param.gm_chnage_baoji;
// 	m_gm_change_jianren = param.gm_chnage_jianren;
// 	m_gm_change_fujia_shanghai = param.gm_change_fujia_shanghai;
// 	m_gm_change_dikang_shanghai = param.gm_change_dikang_shanghai;
// 	m_gm_change_per_jingzhun = param.gm_change_per_jingzhun;
// 	m_gm_change_per_baoji = param.gm_change_per_baoji;
// 	m_gm_change_per_kangbao = param.gm_change_per_kangbao;
// 	m_gm_change_per_pofang = param.gm_change_per_pofang;
// 	m_gm_change_per_mianshang = param.gm_change_per_mianshang;
// 	m_gm_change_constant_zengshang = param.gm_change_constant_zengshang;
// 	m_gm_change_constant_mianshang = param.gm_change_constant_mianshang;
	m_gm_change_capability_attr = param.gm_change_capability_attr;

	m_has_send_all_info = (0 != param.has_send_all_info);
	m_honour = param.honour;

	m_is_mount_bianshen = param.is_mount_bianshen > 0 ? true : false;
	m_mount_bianshen_time = param.mount_bianshen_time;

	m_last_marry_time = m_role_module->common_data_param.last_marry_time;
	m_last_divorce_time = m_role_module->common_data_param.last_divorce_time;
	m_first_speedup_hello_time = EngineAdapter::Instance().Time();
	m_login_timestamp = param.login_timestamp;
	JinjieSysRewardMgr::Instance().InitParam(this, other_p.jinjiesys_reward_param);

	this->InitOtherParam(other_p);

	this->RefreshApperance(false);

	this->InitCommonData();

	m_role_module->special_appearance.Init(this, param.skill_appearance, param.skill_appearance_end_time, param.item_appearance, param.item_appearance_param, param.item_appearance_end_time,
		param.huashen_appearance, param.huashen_appearance_param, param.huashen_appearance_end_time,
		param.greate_soldier_appearance, param.greate_soldier_appearance_param, param.greate_soldier_appearance_end_time);

	m_role_module->cross_rand_activity.Init(this, other_p.role_cross_rand_activity_param);
	m_role_module->cross_goal.Init(this, other_p.cross_goal_param);

	//迁移到本函数返回后调用
	//this->OnInitFinish();
}

void Role::GetPlatName(PlatName pname)
{
	F_STRNCPY(pname, m_plat_name, sizeof(PlatName));
}

void Role::GetRoleInitParam(RoleInitParam *param)
{
	time_t now = EngineAdapter::Instance().Time();

	int online_time_delta = static_cast<int>(now - m_last_online_stati_time);
	m_online_time += online_time_delta;
	m_last_online_stati_time = now;

	int today_online_time_delta = static_cast<int>(now - m_last_today_online_stati_time);
	m_today_online_time += today_online_time_delta;
	m_last_today_online_stati_time = now;

	param->role_id = m_user_id.role_id;
	F_STRNCPY(param->role_name, m_name, sizeof(param->role_name));
	param->avatar = m_avatar;
	param->sex = m_sex;
	param->camp = m_camp;
	param->camp_post = m_camp_post;
	param->prof = m_prof;
	param->level = m_level;
	param->exp = m_exp;
	param->max_exp = m_max_exp;
	param->vip_level = m_role_module->vip.GetVipLevel();
	param->avatar_timestamp = m_avator_timestamp;
	param->is_change_avatar = m_is_change_avatar;

	param->hp = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	param->max_hp = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	param->mp = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP);
	param->max_mp = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP);
	param->gongji = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI);
	param->fangyu = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU);
	param->mingzhong = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG);
	param->shanbi = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI);
	param->baoji = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI);
	param->jianren = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN);
	param->move_speed = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED);
	param->fujia_shanghai = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI);
	param->dikang_shanghai = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI);
	param->per_jingzhun = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN);
	param->per_baoji = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI);
	param->per_kangbao = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO);
	param->per_pofang = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG);
	param->per_mianshang = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG);
	param->constant_zengshang = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG);
	param->constant_mianshang = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG);
	param->pvp_zengshang = m_int_attrs.GetAttr(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER);
	param->pvp_mianshang = m_int_attrs.GetAttr(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER);
	param->pve_zengshang = m_int_attrs.GetAttr(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER);
	param->pve_mianshang = m_int_attrs.GetAttr(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER);
	param->huixinyiji = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI);
	param->huixinyiji_hurt_per = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER);

	param->per_baoji_hurt = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT);
	param->per_kangbao_hurt = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO_HURT);
	param->zhufuyiji_per = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_ZHUFUYIJI_PER);
	param->gedang_per = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_PER);
	param->gedang_dikang_per = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_DIKANG_PER);
	param->gedang_jianshang = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_JIANSHANG_PER);
	param->skill_zengshang = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER);
	param->skill_jianshang = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER);
	param->mingzhong_per = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG_PER);
	param->shanbi_per = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI_PER);
	param->skill_cap_per = m_int_attrs.GetAttr(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_SKILL_CAP_PER);

	if (NULL != m_scene) 
	{
		param->scene_id = m_scene->GetSceneID();
	}

	param->last_scene_id = m_last_scene_id;
	param->scene_x = m_posi.x;
	param->scene_y = m_posi.y;
	param->online_time = m_online_time;
	param->today_online_time = m_today_online_time;
	param->lastday_online_time = m_lastday_online_time;
	param->create_time = (long long)m_create_time;
	param->last_scene_x = m_last_scene_pos.x;
	param->last_scene_y = m_last_scene_pos.y;

	Money *money = m_role_module->knapsack.GetMoney();
	param->gold = money->GetGold();
	param->gold_bind = money->GetGoldBind();
	param->coin = money->GetCoin();
	param->coin_bind = money->GetCoinBind();
	
	param->knapsack_valid_num = m_role_module->knapsack.GetCurrKnapsackValidNum();
	param->storage_valid_num = m_role_module->knapsack.GetCurrStorageValidNum();
	param->authority_type = m_authority_type;
	
	param->forbid_talk_time = (long long)m_forbid_talk_time;
	param->last_online_dayid = m_last_online_dayid;
	param->attack_mode = m_attack_mode;

	{
		param->guild_id = m_guild_id;
		F_STRNCPY(param->guild_name, m_guild_name, sizeof(param->guild_name));
		param->guild_post = m_guild_post;
		param->guild_gongxian = m_guild_gongxian;
		param->guild_total_gongxian = m_guild_total_gongxian;
	}

	param->evil = m_evil;
	param->last_kill_me_time = m_xianhun;
	param->yuanli = m_yuanli;
	param->nv_wa_shi = m_nv_wa_shi;
	param->lingjing = m_lingjing;
	param->chengjiu = m_chengjiu;
	param->fenchen = m_hunli;

	m_role_module->daycounter.GetCountData(param->day_count_data);
	param->capability = m_role_module->capablity.GetTotalCapability();
	param->max_capability = m_role_module->capablity.GetMaxCapablity();

	m_role_module->sublock.GetInitParam(param->sublock_pw, &param->sublock_has_lock, &param->sublock_today_error_times);
	
	m_role_module->team_info.GetInitParam(&param->team_index, &param->team_seq, &param->is_auto_join_team);
	m_role_module->friend_manager.GetInitParam(&param->all_give_flower_num, &param->day_give_flower_num, &param->all_charm);
	
	param->move_mode = m_move_mode;
	param->move_mode_param = m_move_mode_param;

	param->gm_change_maxhp = m_gm_change_maxhp;
	param->gm_change_gongji = m_gm_change_gongji;
	param->gm_change_movespeed = m_gm_change_movespeed;
	param->gm_change_fangyu = m_gm_change_fangyu;
	param->gm_chnage_mingzhong = m_gm_change_mingzhong;
//	param->gm_chnage_shanbi = m_gm_change_shanbi;
// 	param->gm_chnage_baoji = m_gm_change_baoji;
// 	param->gm_chnage_jianren = m_gm_change_jianren;
// 	param->gm_change_fujia_shanghai = m_gm_change_fujia_shanghai;
// 	param->gm_change_dikang_shanghai = m_gm_change_dikang_shanghai;
// 	param->gm_change_per_jingzhun = m_gm_change_per_jingzhun;
// 	param->gm_change_per_baoji = m_gm_change_per_baoji;
// 	param->gm_change_per_kangbao = m_gm_change_per_kangbao;
// 	param->gm_change_per_pofang = m_gm_change_per_pofang;
// 	param->gm_change_per_mianshang = m_gm_change_per_mianshang;
// 	param->gm_change_constant_zengshang = m_gm_change_constant_zengshang;
// 	param->gm_change_constant_mianshang = m_gm_change_constant_mianshang;
	param->gm_change_capability_attr = m_gm_change_capability_attr;

	param->has_send_all_info = (m_has_send_all_info ? 1 : 0);

	param->honour = m_honour;

	param->is_mount_bianshen = m_is_mount_bianshen ? 1 : 0;
	param->mount_bianshen_time = m_mount_bianshen_time;

	m_role_module->common_data_param.last_marry_time = m_last_marry_time;
	m_role_module->common_data_param.last_divorce_time = m_last_divorce_time;
	
	param->login_timestamp = m_login_timestamp;

	m_role_module->qingyuan.GetRoleInitParam(&param->m_marry_type, &param->m_lover_id, &param->m_shici_type, &param->m_marry_req_id, &param->m_is_sure);

	m_role_module->special_appearance.GetInitParam(&param->skill_appearance, &param->skill_appearance_end_time, &param->item_appearance, &param->item_appearance_param, &param->item_appearance_end_time,
		&param->huashen_appearance, &param->huashen_appearance_param, &param->huashen_appearance_end_time,
		&param->greate_soldier_appearance, &param->greate_soldier_appearance_param, &param->greate_soldier_appearance_end_time);
}

void Role::InitOtherParam(const RoleOtherInitParam &other_p)
{
	m_role_module->role_chestshop.Init(this, other_p.chest_buff, other_p.chest_buff_len);
	m_role_module->roleshop.Init(this, other_p.roleshop_param);
	m_role_module->active_degree.Init(this, other_p.active_degree_param);
	m_role_module->role_hang_up.Init(this, other_p.rolehangup_param);
	m_role_module->knapsack.GetItemColddownManager()->Init(this, other_p.itemcolddown_param);
	m_role_module->role_evaluate.Init(this, other_p.evaluate_param);
	m_role_module->monitor.Init(this, other_p.monitor_param);
	
	m_last_singlechat_monitor_time = other_p.last_singlechat_monitor_time;
	F_STRNCPY(m_last_singlechat_monitor_msg, other_p.last_singlechat_monitor_msg, sizeof(m_last_singlechat_monitor_msg));
	m_last_worldchat_monitor_time = other_p.last_worldchat_monitor_time;
	F_STRNCPY(m_last_worldchat_monitor_msg, other_p.last_worldchat_monitor_msg, sizeof(m_last_worldchat_monitor_msg));
	m_last_mail_monitor_time = other_p.last_mail_monitor_time;
	F_STRNCPY(m_last_mail_monitor_subject, other_p.last_mail_monitor_subject, sizeof(m_last_mail_monitor_subject));
}

void Role::GetRoleOtherInitParam(RoleOtherInitParam *other_p)
{
	OtherModuleDataParam &other_module_param = other_p->other_module_param;

	m_role_module->mentality.GetOtherInitParam(&other_p->mentality_param);

	m_role_module->vip.GetOtherInitParam(&other_p->vip_param);
	m_role_module->role_chestshop.GetInitParam(other_p->chest_buff, &other_p->chest_buff_len);
	m_effect_list.GetInitParam(other_p->effect_buff, &other_p->effect_buff_len);
	m_role_module->roleshop.GetInitParam(&other_p->roleshop_param);
	m_role_module->active_degree.GetInitParam(&other_p->active_degree_param);
	m_role_module->welfare.GetInitParam(&other_p->welfare_param);
	m_role_module->role_hang_up.GetInitParam(&other_p->rolehangup_param);
	m_role_module->knapsack.GetItemColddownManager()->GetItemColddownParam(&other_p->itemcolddown_param);

	m_role_module->role_evaluate.GetEvaluateParam(&other_p->evaluate_param);
	m_role_module->monitor.GetMonitorParam(&other_p->monitor_param);
	m_role_module->mount_manager.GetOtherInitParam(&other_p->mount_param);
	m_role_module->fightmount_manager.GetOtherInitParam(&other_p->fight_mount_param);
	m_role_module->title_manager.GetOtherInitParam(&other_p->title_param);
	m_role_module->husong_task.GetInitParam(&other_p->husong_param);

	m_role_module->wing.GetOtherInitParam(&other_p->wing_param);
	m_role_module->halo.GetOtherInitParam(&other_p->halo_param);
	m_role_module->shengong.GetOtherInitParam(&other_p->shengong_param);
	m_role_module->shenyi.GetOtherInitParam(&other_p->shenyi_param);
	m_role_module->footprint.GetOtherInitParam(&other_p->footprint_param);
	m_role_module->cloak_manager.GetOtherInitParam(&other_p->cloak_param);

	m_role_module->role_activity.GetInitParam(&other_p->roleactivity_param);
	m_role_module->equip_fb.GetInitParam(&other_p->newequipfb_param, &other_p->equipfb_param.team_equipfb_info);
	m_role_module->zhanshendian.GetInitParam(&other_module_param.zhanshendian_pass_level, 
		&other_module_param.zhanshendian_yesterday_pass_level, &other_module_param.m_zhanshendian_fetch_day_reward_timestamp);

	m_role_module->stone_manager.GetInitParam(&other_p->stone_param);
	m_role_module->guild_xianshu.GetInitParam(&other_p->guild_xianshu_param);
	m_role_module->task_recorder.GetInitParam(other_p->taskrecord_buff, &other_p->taskrecord_buff_len);
	m_role_module->newtask_manager.GetOtherInitParam(&other_p->task_param);
	m_role_module->daily_task.GetInitParam(&other_module_param.daily_task_can_fetch_flag, &other_module_param.daily_task_fetch_flag, &other_module_param.daily_task_exp_ball_valid_time, &other_module_param.daily_task_exp_ball_exp);

	m_role_module->guild_task.GetInitParam(&other_p->guild_task_param);

	m_role_module->role_supply.GetInitParam(&other_p->role_supply_param);
	m_role_module->story_fb.GetInitParam(&other_p->storyfb_param);
	m_role_module->mojie_fb.GetInitParam(&other_p->mojiefb_param);
	m_role_module->turntable.GetInitParam(other_module_param.turntable_lucky_value);
	m_role_module->maze.GetOtherInitParam(&other_p->maze_param);
	m_role_module->challenge_fb.GetInitParam(&other_p->challengefb_param);
	m_role_module->role_phase_fb.GetInitParam(&other_p->phase_fb_param);
	m_role_module->patafb.GetInitParam(&other_module_param.patafb_pass_level, &other_module_param.patafb_today_level);
	m_role_module->vipfb.GetInitParam(&other_p->vipfb_param);
	m_role_module->convert_record.GetParam(&other_p->convert_record_param);
	m_role_module->discount_buy.GetInitParam(&other_p->discount_buy_param);
	m_role_module->zero_gift.GetInitParam(&other_p->zero_gift_param);
	m_role_module->qingyuan.GetInitParam(&other_p->qingyuan_param);
	m_role_module->baby.GetInitParam(&other_p->baby_param);
	m_role_module->shenge_system.GetInitParam(&other_p->shenge_system_param);
	m_role_module->month_card.GetInitParam(&other_module_param.monthcard_active_timestamp, &other_module_param.monthcard_last_day,
		&other_module_param.monthcard_next_reward_day_idx, &other_module_param.monthcard_buy_times);
	m_role_module->wabao.GetInitParam(&other_module_param.wabao_scene_id, &other_module_param.wabao_pos_x, &other_module_param.wabao_pos_y,
		&other_module_param.wabao_baotu_count, &other_module_param.wabao_reward_type, &other_module_param.baotu_fetch_flag, &other_module_param.shouhuzhe_time);

	m_role_module->jilian.GetInitParam(&other_module_param.jilian_type, &other_module_param.jilian_is_working, 
		&other_module_param.jilian_progress, &other_module_param.jilian_once_join, &other_module_param.jilian_qte_succ_times);

	m_role_module->touzijihua.GetInitParam(&other_module_param.touzijihua_active_plan_0, &other_module_param.touzijihua_active_plan_1, &other_module_param.touzijihua_active_plan_2, &other_module_param.touzijihua_active_plan_3,
		&other_module_param.touzhijihua_active_highest_plan, &other_module_param.touzijihua_plan_0_reward_flag, &other_module_param.touzijihua_plan_1_reward_flag, &other_module_param.touzijihua_plan_2_reward_flag, &other_module_param.touzijihua_plan_3_reward_flag,
		&other_module_param.new_touzijihua_buy_time, &other_module_param.new_touzijihua_reward_flag, &other_module_param.new_touzijihua_vip_reward_flag, 
		&other_module_param.new_touzijihua_gold_bind_flag, &other_module_param.new_touzijihua_first_reward_flag, other_module_param.foundation_next_reward_index,
		&other_module_param.touzijihua_active_fb_plan, &other_module_param.touzijihua_fb_plan_reward_flag, &other_module_param.touzijihua_active_boss_plan, &other_module_param.touzijihua_boss_plan_reward_flag, &other_module_param.touzijihua_kill_boss_num,
		&other_module_param.touzijihua_kill_shenyun_boss_num, &other_module_param.touzijihua_active_shenyun_boss_plan, &other_module_param.touzijihua_shenyun_boss_plan_reward_flag);

	m_skill_manager.GetInitParam(&other_p->skill_other_param);

	m_role_module->roll_money.GetInitParam(&other_module_param.roll_money_gold_fetch_reward_times, &other_module_param.roll_money_coin_fetch_reward_times,
		&other_module_param.roll_money_gold_roll_times, &other_module_param.roll_money_coin_roll_times, &other_module_param.roll_money_gold_num_0, 
		&other_module_param.roll_money_gold_num_1, &other_module_param.roll_money_gold_num_2, &other_module_param.roll_money_coin_num_0, 
		&other_module_param.roll_money_coin_num_1, &other_module_param.roll_money_coin_num_2, &other_module_param.roll_money_coin_num_3,
		&other_module_param.roll_money_coin_num_4, &other_module_param.roll_money_coin_num_5, &other_module_param.roll_money_coin_num_6, 
		&other_module_param.roll_money_already_roll_gold_num);

	other_module_param.last_reset_role_module_data_dayi = m_last_reset_module_data_dayid;
	other_module_param.last_reset_role_module_data_timestamp = m_last_reset_module_data_timestamp;

	m_role_module->role_world_event.GetInitParam(&other_module_param.world_event_fetch_reward_flag, &other_module_param.world_event_find_npc_score, &other_module_param.world_event_kill_rand_boss_score,
		&other_module_param.world_event_shenmozhixi_score, &other_module_param.world_event_xukongzhimen_score, &other_module_param.world_event_quanmintafang_score, &other_module_param.world_event_jiqingpk_score,
		&other_module_param.world_event_tiannvsanhua_score, &other_module_param.world_event_find_npc_flag, &other_module_param.world_event_light_word_flag, &other_module_param.world_event_curr_find_npc_id);

	m_role_module->common_activity_unit_mgr.GetInitParam(&other_p->common_activity_param);

	m_role_module->role_goal.GetInitParam(&other_p->role_goal_param);
	m_role_module->shizhuang.GetInitParam(&other_p->shizhuang_param);
	m_role_module->role_cross.GetInitParam(&other_p->role_cross_param);
	m_role_module->role_guild.GetInitParam(&other_p->role_guild_param);
	m_role_module->shengwang_mgr.GetInitParam(&other_p->shengwang_param);
	m_role_module->chengjiu_mgr.GetInitParam(&other_p->chengjiu_param);
	m_role_module->dailyfb.GetInitParam(&other_p->dailyfb_param);
	m_role_module->lifeskill_mgr.GetInitParam(&other_p->lifeskill_param);
	m_role_module->tuhaojin.GetInitParam(&other_p->tuhaojin_param);
	m_role_module->bigchatface.GetInitParam(&other_p->bigchatface_param);
	m_role_module->pet.GetInitParam(&other_p->pet_param);
	m_role_module->multi_mount.GetInitParam(&other_p->multi_mount_param);
	m_role_module->personalize_window.GetInitParam(&other_p->personalize_window_param);
	m_role_module->xunzhang_manager.GetInitParam(&other_p->xunzhang_param);
	m_role_module->zhibao_manager.GetInitParam(&other_p->zhibao_param);
	m_role_module->shenbing_manager.GetInitParam(&other_p->shenbing_param);
	m_role_module->mojie_manager.GetInitParam(&other_p->mojie_param);
	m_role_module->equip_suit.GetInitParam(&other_p->equipsuit_param);
	m_role_module->shen_zhuang.GetInitParam(&other_p->shenzhuang_param);
	m_role_module->friend_gift_manager.GetInitParam(&other_p->friend_gift_param);
	m_role_module->gold_vip.GetOtherInitParam(&other_p->gold_vip_param);
	m_role_module->cardzu.GetInitParam(&other_p->cardzu_param);
	//m_role_module->lieming_mgr.GetInitParam(&other_p->lieming_param, &other_p->lieming_bag_param);
	m_role_module->lieming_mgr.GetInitParam(&other_p->lieming_param2);
	m_role_module->huashen.GetInitParam(&other_p->huashen_param);
	m_role_module->magic_card.GetInitParam(&other_p->magic_card_param);
	m_role_module->mitama.GetInitParam(&other_p->mitama_param);
	m_role_module->xiannvshouhu_manager.GetOtherInitParam(&other_p->xiannv_shouhu_param);
	m_role_module->jinglingfazhen_manager.GetOtherInitParam(&other_p->jingling_fazhen_param);
	m_role_module->jinglingguanghuan_manager.GetOtherInitParam(&other_p->jingling_guanghuan_param);
	m_role_module->zhuansheng_equip.GetInitParam(&other_p->zhuansheng_equip_param);
	m_role_module->chinese_zodiac.GetInitParam(&other_p->chinese_zodiac_param);
	m_role_module->followboss_manager.GetInitParam(&other_p->follow_boss_param);

	m_role_module->equip_manager.GetInitParam(&m_role_module->common_data_param.cross_equip_itemid, &m_role_module->common_data_param.cross_equip_gain_timestamp, &other_p->equipment_param);
	m_role_module->friend_exp_bottle.GetInitParam(&other_p->friend_exp_bottle_param); 
	m_role_module->jingling_mgr.GetInitParam(&other_p->jingling_param);
	m_role_module->magical_precious_manager.GetInitParam(&other_p->magical_precious_param);
	m_role_module->mysterious_shop_in_mall.GetInitParam(&other_p->mysterious_shop_in_mall_param, &other_p->discountbuy_param);
	m_role_module->towerdefend_fb.GetInitParam(&other_p->towerdefend_param);
	m_role_module->runesystem_manger.GetInitParam(&other_p->rune_system_param);
	m_role_module->shenzhou_weapon_manger.GetInitParam(&other_p->shenzhou_weapon_param);
	m_role_module->xiannv_manager.GetInitParam(&other_p->xiannv_param);
	m_role_module->tuitu_fb.GetInitParam(&other_p->tuitu_fb_param);
	m_role_module->precious_boss.GetInitParam(&other_p->precious_boss_param);
	m_role_module->shenshou.GetInitParam(&other_p->shenshou_param);
	m_role_module->img_fuling.GetInitParam(&other_p->img_fuling_param);
	m_role_module->csa_equip.GetInitParam(&other_p->csa_equip_param);
	m_role_module->multi_mount.GetInitParam2(&other_p->couple_mount_param);
	m_role_module->shenyin.GetInitParam(&other_p->shenyin_param);
	m_role_module->element_heart.GetInitParam(&other_p->element_heart_param);
	m_role_module->shengxing_zhuli.GetInitParam(&other_p->shengxing_zhuli_param);
	m_role_module->upgrade_rush.GetInitParam(other_module_param.upgrade_return_end_time_list);
	m_role_module->fabao.GetInitParam(&other_p->fabao_param);
	m_role_module->feixian_manager.GetInitParam(&other_p->feixian_param);
	m_role_module->talent.GetInitParam(&other_p->talent_param);
	m_role_module->jinjie_talent.GetInitParam(&other_p->jinjie_talent_param);
	m_role_module->role_am_fb.GetInitParam(&other_p->armor_fb_param);

	m_role_module->heshen_luoshu.GetInitParam(&other_p->heshen_luoshu_param);
	m_role_module->shengqi.GetInitParam(&other_p->shengqi_param);
	m_role_module->boss_card.GetInitParam(&other_p->bosscard_param);
	m_role_module->zhuanzhi_task.GetInitParam(&other_p->task_zhuanzhi_param);
	m_role_module->role_zhuanzhi.GetInitParam(&other_module_param.zhuanzhi_tianming, &other_module_param.zhuanzhi_canglong, &other_module_param.zhuanzhi_shengxiao, &other_module_param.zhuanzhi_yuanqi, &other_module_param.zhuanzhi_shenqi, &other_module_param.zhuanzhi_xingxiu);

	m_role_module->cross_role_global.GetInitParam(&other_p->cross_global_data_param);
	m_role_module->yaoshi_manager.GetOtherInitParam(&other_p->yaoshi_param);
	m_role_module->toushi_manager.GetOtherInitParam(&other_p->toushi_param);
	m_role_module->qilinbi_manager.GetOtherInitParam(&other_p->qilinbi_param);
	m_role_module->mask_manager.GetOtherInitParam(&other_p->mask_param);
	m_role_module->zhuanzhi_equip.GetInitParam(&other_p->zhuanzhi_equip_param);
	m_role_module->seal.GetInitParam(&other_p->seal_param);
	m_role_module->dressing_room_mgr.GetInitParam(&other_p->dressing_room_param);
	
	m_role_module->upgrade_manager.GetInitParam(&other_p->upgrade_manager_param);
	m_role_module->role_big_small_goal.GetInitParam(&other_p->role_big_small_goal_param);
	m_role_module->shen_qi.GetInitParam(&other_p->shenqi_param);
	m_role_module->qingyuan.GetInitParam(&other_p->profess_param);
	m_role_module->redequip_collect.GetInitParam(&other_p->red_equip_collect_param);
	m_role_module->orang_equip_collect.GetInitParam(&other_p->orange_equip_collect_param);
	m_role_module->molong.GetInitParam(&other_module_param.molong_curr_loop, &other_module_param.molong_accumulate_consume_gold, &other_module_param.molong_today_consume_gold,
		&other_module_param.molong_total_move_step, &other_module_param.molong_today_move_step, &other_module_param.molong_rank_grade, &other_module_param.molong_rank_cumulate_gold);
	m_role_module->xianzunka.GetInitParam(&other_p->xianzunka_param);
	m_role_module->equip_baptize.GetInitParam(&other_p->equipbaptize_param);
	m_role_module->role_personboss_fb.GetInitParam(&other_p->personboss_param);
	m_role_module->m_greate_soldier.GetInitParam(&other_p->greate_soldier_param);
	m_role_module->lottery_count_module.GetInitParam(&other_p->role_lottery_count_param);
	m_role_module->cross_rand_activity.GetInitParam(&other_p->role_cross_rand_activity_param);
	m_role_module->upgrade_card_buy.GetInitParam(&other_p->upgrade_card_buy_param);
	m_role_module->huguozhili.GetInitParam(&other_p->huguozhili_param);
	m_role_module->cross_goal.GetInitParam(&other_p->cross_goal_param);
	m_role_module->baizhan_equip.GetInitParam(&other_p->baizhan_equip_param);
	m_role_module->myth.GetInitParam(&other_p->myth_param);
	m_role_module->zodiac.GetInitParam(&other_p->zodiac_param);
	m_role_module->cross_equip.GetInitParam(&other_p->cross_equip_param);

	m_role_other_mgr->GetInitParam(other_p);
	//for (IRoleModule *module : m_role_module_list)
	//{
	//	module->GetParam(*other_p);
	//}

	other_p->last_singlechat_monitor_time = m_last_singlechat_monitor_time;
	F_STRNCPY(other_p->last_singlechat_monitor_msg, m_last_singlechat_monitor_msg, sizeof(other_p->last_singlechat_monitor_msg));
	other_p->last_worldchat_monitor_time = m_last_worldchat_monitor_time;
	F_STRNCPY(other_p->last_worldchat_monitor_msg, m_last_worldchat_monitor_msg, sizeof(other_p->last_worldchat_monitor_msg));
	other_p->last_mail_monitor_time = m_last_mail_monitor_time;
	F_STRNCPY(other_p->last_mail_monitor_subject, m_last_mail_monitor_subject, sizeof(other_p->last_mail_monitor_subject));

	other_module_param.week_add_charm = m_role_module->friend_manager.GetWeekAddCharm();
	other_p->common_data_param = m_role_module->common_data_param;   
	other_p->history_camp_param = m_role_module->history_cap_param;
	other_module_param.day_add_charm = m_role_module->friend_manager.GetDayAddCharm();
	other_p->jinjiesys_reward_param = m_role_module->jinjiesys_reward_param;

	other_module_param.knapsack_grid_auto_add_count = m_role_module->knapsack.GetValidGridAutoAddCount();

}


CommonDataParam *Role::GetCommonDataParam()
{
	return &m_role_module->common_data_param;
}

void Role::GetCommonDataParam(CommonDataParam *param)
{
	if (NULL != param)
	{
		*param = m_role_module->common_data_param;
	}
}

UpgradeManager *Role::GetUpgradeManager()
{
	return &m_role_module->upgrade_manager;
}

EquipmentManager *Role::GetEquipmentManager()
{
	return &m_role_module->equip_manager;
}

FeixianManager *Role::GetFeixianManager()
{
	return &m_role_module->feixian_manager;
}

FriendExpBottle *Role::GetFriendExpBottle()
{
	return &m_role_module->friend_exp_bottle;
}

RoleGuild *Role::GetRoleGuild()
{
	return &m_role_module->role_guild;
}

RoleFBTowerDefend *Role::GetFBTowerDefend()
{
	return &m_role_module->towerdefend_fb;
}

RuneSystem *Role::GetRuneSystem()
{
	return &m_role_module->runesystem_manger;
}

ShenzhouWeapon *Role::GetShenzhouWeapon()
{
	return &m_role_module->shenzhou_weapon_manger;
}

XiannvManager *Role::GetXiannvManager()
{
	return &m_role_module->xiannv_manager;
}

JingLingManager *Role::GetJingLingMgr()
{
	return &m_role_module->jingling_mgr;
}

MagicalPrecious *Role::GetMagicalPreciousManager()
{
	return &m_role_module->magical_precious_manager;
}

MysteriousShopInMall *Role::GetMysteriousShopInMall()
{
	return &m_role_module->mysterious_shop_in_mall;
}

RoleTuituFb	*Role::GetTuituFb()
{
	return &m_role_module->tuitu_fb;
}

Knapsack *Role::GetKnapsack()
{
	return &m_role_module->knapsack;
}

NewTaskManager *Role::GetTaskManager()
{
	return &m_role_module->newtask_manager;
}

TaskRecorder *Role::GetTaskRecorder()
{
	return &m_role_module->task_recorder;
}

HotkeyRecorder *Role::GetHotkeyRecorder()
{
	return &m_role_module->hotkey_recorder;
}

FriendManager *Role::GetFriendManager()
{
	return &m_role_module->friend_manager;
}

TeamInfo *Role::GetTeamInfo()
{
	return &m_role_module->team_info;
}

RankShadow *Role::GetRankShadow()
{
	return &m_role_module->rank_shadow;
}

Mentality *Role::GetMentality()
{
	return &m_role_module->mentality;
}

Vip *Role::GetVip()
{
	return &m_role_module->vip;
}

RoleChestShop *Role::GetRoleChestShop()
{
	return &m_role_module->role_chestshop;
}

RoleShop *Role::GetRoleShop()
{
	return &m_role_module->roleshop;
}

Capability *Role::GetCapability()
{
	return &m_role_module->capablity;
}

DayCounter *Role::GetDayCounter()
{
	return &m_role_module->daycounter;
}

TitleManager *Role::GetTitleManager()
{
	return &m_role_module->title_manager;
}

ActiveDegree *Role::GetActiveDegree()
{
	return &m_role_module->active_degree;
}

Welfare *Role::GetWelfare()
{
	return &m_role_module->welfare;
}

Hangup *Role::GetHangup()
{
	return &m_role_module->role_hang_up;
}

RoleEvaluate *Role::GetRoleEvaluate()
{
	return &m_role_module->role_evaluate;
}

RoleStatusManager *Role::GetRoleStatusManager()
{
	return &m_role_module->role_status_manager;
}

Monitor *Role::GetMonitor()
{
	return &m_role_module->monitor;
}

SubLock *Role::GetSublock()
{
	return &m_role_module->sublock;
}

MountManager *Role::GetMountManager()
{
	return &m_role_module->mount_manager;
}

FightMountManager *Role::GetFightMountManager()
{
	return &m_role_module->fightmount_manager;
}

StoneManager *Role::GetStoneManager()
{
	return &m_role_module->stone_manager;
}

RoleActivity *Role::GetRoleActivity()
{
	return &m_role_module->role_activity;
}

DailyTask *Role::GetDailyTask()
{
	return &m_role_module->daily_task;
}

HusongTask *Role::GetHusongTask()
{
	return &m_role_module->husong_task;
}

RoleFBEquip *Role::GetFBEquip()
{
	return &m_role_module->equip_fb;
}

MazeManager *Role::GetMaze()
{
	return &m_role_module->maze;
}

WingManager *Role::GetWing()
{
	return &m_role_module->wing;
}

HaloManager *Role::GetHalo()
{
	return &m_role_module->halo;
}

ShengongManager *Role::GetShengong()
{
	return &m_role_module->shengong;
}

ShenyiManager *Role::GetShenyi()
{
	return &m_role_module->shenyi;
}

FootPrint *Role::GetFootPrint()
{
	return &m_role_module->footprint;
}

CloakManager *Role::GetCloakManager()
{
	return &m_role_module->cloak_manager;
}

GuildTask *Role::GetGuildTask()
{
	return &m_role_module->guild_task;
}

ZhuanzhiTask *Role::GetZhuanzhiTask()
{
	return &m_role_module->zhuanzhi_task;
}

MojieManager *Role::GetMojie()
{
	return &m_role_module->mojie_manager;
}

EquipSuit *Role::GetEquipSuit()
{
	return &m_role_module->equip_suit;
}

ShenZhuang *Role::GetShenZhuang()
{
	return &m_role_module->shen_zhuang;
}

RoleYaojiang *Role::GetRoleYaojiang()
{
	return &m_role_module->yaojiang;
}

SpecialAppearance *Role::GetSpecialAppearance()
{
	return &m_role_module->special_appearance;
}

DatingTask *Role::GetDatingTask()
{
	return &m_role_module->dating_task;
}

GuildXianshu *Role::GetGuildXianshu()
{
	return &m_role_module->guild_xianshu;
}

RoleSupply *Role::GetRoleSupply()
{
	return &m_role_module->role_supply;
}

RoleStoryFB *Role::GetFBStory()
{
	return &m_role_module->story_fb;
}

RoleMojieFB *Role::GetFBMojie()
{
	return &m_role_module->mojie_fb;
}

RoleFBChallenge *Role::GetFBChallenge()
{
	return &m_role_module->challenge_fb;
}

RolePhaseFB *Role::GetRoleFBPhase()
{
	return &m_role_module->role_phase_fb;
}

RolePataFB *Role::GetRolePataFB()
{
	return &m_role_module->patafb;
}

RoleVipFB *Role::GetRoleVipFB()
{
	return &m_role_module->vipfb;
}

RoleConvertRecord *Role::GetConvertRecord()
{
	return &m_role_module->convert_record;
}

DiscountBuy *Role::GetDiscountBuy()
{
	return &m_role_module->discount_buy;
}

ZeroGift *Role::GetZeroGift()
{
	return &m_role_module->zero_gift;
}

MonthCard *Role::GetMonthCard()
{
	return &m_role_module->month_card;
}

TurnTable *Role::GetTurnTable()
{
	return &m_role_module->turntable;
}

Wabao *Role::GetWabao()
{
	return &m_role_module->wabao;
}

CishaTask *Role::GetCishaTask()
{
	return &m_role_module->cisha_task;
}

Jilian *Role::GetJilian()
{
	return &m_role_module->jilian;
}

Qingyuan *Role::GetQingyuan()
{
	return &m_role_module->qingyuan;
}

Baby *Role::GetBaby()
{
	return  &m_role_module->baby;
}

TradeRegister *Role::GetTradeRegister()
{
	return &m_role_module->trade_register;
}

RoleZhanshenDian *Role::GetZhanShenDian()
{
	return &m_role_module->zhanshendian;
}

Touzijihua *Role::GetTouzijihua()
{
	return &m_role_module->touzijihua;
}

RollMoney *Role::GetRollMoney()
{
	return &m_role_module->roll_money;
}

RoleWorldEvent *Role::GetRoleWorldEvent()
{
	return &m_role_module->role_world_event;
}

CommonActivityUnitManager *Role::GetCommonActivityUnitMgr()
{
	return &m_role_module->common_activity_unit_mgr;
}

RoleGoal *Role::GetRoleGoal()
{
	return &m_role_module->role_goal;
}

Shizhuang *Role::GetShizhuang()
{
	return &m_role_module->shizhuang;
}

RoleCross *Role::GetRoleCross()
{
	return &m_role_module->role_cross;
}

ShengWang *Role::GetShengWangMgr()
{
	return &m_role_module->shengwang_mgr;
}

ChengJiu *Role::GetChengJiuMgr()
{
	return &m_role_module->chengjiu_mgr;
}

RoleDailyFb *Role::GetRoleDailyFb()
{
	return &m_role_module->dailyfb;
}

LifeSkillMgr *Role::GetLifeSkillMgr()
{
	return &m_role_module->lifeskill_mgr;
}

GongXun *Role::GetGongXunMgr()
{
	return &m_role_module->gongxun_mgr;
}

PaoHuanTask *Role::GetPaoHuanTask()
{
	return &m_role_module->paohuan_task;
}

TuHaoJin *Role::GetTuHaoJin()
{
	return &m_role_module->tuhaojin;
}

BigChatFace *Role::GetBigChatFace()
{
	return &m_role_module->bigchatface;
}

Pet *Role::GetPet()
{
	return &m_role_module->pet;
}

MultiMount *Role::GetMultiMount()
{
	return &m_role_module->multi_mount;
}

PersonalizeWindow *Role::GetPersonalizeWindow()
{
	return &m_role_module->personalize_window;
}

XunZhangManager *Role::GetXunZhangManager()
{
	return &m_role_module->xunzhang_manager;
}

ZhiBaoManager *Role::GetZhiBaoManager()
{
	return &m_role_module->zhibao_manager;
}

ShenBingManager *Role::GetShenBingManager()
{
	return &m_role_module->shenbing_manager;
}

Cardzu *Role::GetCardzu()
{
	return &m_role_module->cardzu;
}

LieMingManager *Role::GetLieMingMgr()
{
	return &m_role_module->lieming_mgr;
}

HuaShen *Role::GetHuaShen()
{
	return &m_role_module->huashen;
}

MagicCard *Role::GetMagicCard()
{
	return &m_role_module->magic_card;
}

Mitama *Role::GetMitama()
{
	return &m_role_module->mitama;
}

XiannvShouhuManager *Role::GetXiannvShouhu()
{
	return &m_role_module->xiannvshouhu_manager;
}

JinglingGuanghuanManager *Role::GetJinglingGuanghuan()
{
	return &m_role_module->jinglingguanghuan_manager;
}

JinglingFazhenManager *Role::GetJinglingFazhen()
{
	return &m_role_module->jinglingfazhen_manager;
}

ZhuanShengEquip *Role::GetZhuanShengEquip()
{
	return  &m_role_module->zhuansheng_equip;
}

CrossRoleGlobal *Role::GetCrossRoleGlobal()
{
	return &m_role_module->cross_role_global;
}

ChineseZodiac *Role::GetChineseZodiac()
{
	return  &m_role_module->chinese_zodiac;
}

FriendGiftManager *Role::GetFriendGiftManager()
{
	return  &m_role_module->friend_gift_manager;
}

GoldVip *Role::GetGoldVip()
{
	return &m_role_module->gold_vip;
}

FamousMan *Role::GetFamousMan()
{
	return &m_role_module->famous_man;
}

FollowBossManager *Role::GetFollowBossManager()
{
	return &m_role_module->followboss_manager;
}

ShengeSystem *Role::GetShengeSystem()
{
	return &m_role_module->shenge_system;
}

RolePreciousBoss *Role::GetRolePreciousBoss()
{
	return &m_role_module->precious_boss;
}

Shenshou *Role::GetShenshou()
{
	return &m_role_module->shenshou;
}

ImgFuling *Role::GetImgFuling()
{
	return &m_role_module->img_fuling;
}

CombineServerActivityEquip *Role::GetCSAEquip()
{
	return &m_role_module->csa_equip;
}

ShenYin * Role::GetShenYin()
{
	return &m_role_module->shenyin;
}

ElemnetHeart *Role::GetElementHeart()
{
	return &m_role_module->element_heart;
}

ShengxingzhuliManager *Role::GetShengxingzhuliManager()
{
	return &m_role_module->shengxing_zhuli;
}

FabaoManager* Role::GetFabaoManager()
{
	return &m_role_module->fabao;
}

RoleChallengeField * Role::GetRoleChallengeField()
{
	return &m_role_module->role_challenge_field;
}

RoleFBArmorDefend* Role::GetRoleFbArmorDefend()
{
	return &m_role_module->role_am_fb;
}

ImpGuard *Role::GetImpGuard()
{
	return &m_role_module->m_imp_guard;
}

RoleZhuanZhi *Role::GetRoleZhuanZhi()
{
	return &m_role_module->role_zhuanzhi;
}

BossCard *Role::GetBossCard()
{
	return &m_role_module->boss_card;
}

YaoShiManager * Role::GetYaoShiManager()
{
	return &m_role_module->yaoshi_manager;
}

TouShiManager * Role::GetTouShiManager()
{
	return &m_role_module->toushi_manager;
}

QiLinBiManager * Role::GetQiLinBiManager()
{
	return &m_role_module->qilinbi_manager;
}

MaskManager * Role::GetMaskManager()
{
	return &m_role_module->mask_manager;
}

ZhuanzhiEquip * Role::GetZhuanzhiEquip()
{
	return &m_role_module->zhuanzhi_equip;
}

int Role::GetJingJieLevel()
{
	return m_role_module->common_data_param.jingjie_level;
}

bool Role::SetJingJieLevel(int level)
{
	if (level < 0 || level > MAX_JINGJIE_LEVEL)
	{
		return false;
	}
	m_role_module->common_data_param.jingjie_level = level;
	return true;
}

JingJie* Role::GetJingJie() 	
{
	return &m_role_module->jingjie;
}

Talent* Role::GetTalent()
{
	return &m_role_module->talent;
}

JinJieTalent* Role::GetJinJieTalent()
{
	return &m_role_module->jinjie_talent;
}

HeShengLuoShu* Role::GetHeShengLuoShu()
{
	return &m_role_module->heshen_luoshu;
}

void Role::SetMsgTime(unsigned long now, IP userip) 
{
	m_last_msg_from_client_time = now;
	m_last_msg_from_client_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	m_userip = userip;
}

bool Role::GetShangGuBossConcern(int layer, int index)
{
	if (index < 0 || layer < 0 || layer >= CommonDataParam::SHANGGU_BOSS_MAX_LAYER)
	{
		return false;
	}

	return 0 != (m_role_module->common_data_param.shanggu_boss_concern_flag_list[layer] & ((long long)1 << index));
}

void Role::SetShangGuBossConcern(int layer, int index, bool is_set)
{
	if (index < 0 || layer < 0 || layer >= CommonDataParam::SHANGGU_BOSS_MAX_LAYER)
	{
		return;
	}
	if (is_set)
	{
		m_role_module->common_data_param.shanggu_boss_concern_flag_list[layer] |= ((long long)1 << index);
		BossShangGuManager::Instance().RegisterConcernBoss(m_uid, layer, index);
	}
	else
	{
		m_role_module->common_data_param.shanggu_boss_concern_flag_list[layer] &= ~((long long)1 << index);
		BossShangGuManager::Instance().UnRegisterConcernBoss(m_uid, layer, index);
	}
}

void Role::SetAvatarTimeStamp(long long avatar_timestamp)
{ 
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	// 不是恢复默认头像则需要检查是否开放头像更换功能
	if (0 != avatar_timestamp && LOGIC_CONFIG->GetAgentAdapterConfig().IsForbidChagneAvatar())
	{
		return;
	}

	m_avator_timestamp = avatar_timestamp; 

	this->OnRoleBaseInfoChange();

	if (0 == m_is_change_avatar)
	{
		m_is_change_avatar = 1;

		long long add_money = GLOBALCONFIG->GetFirstChangeAvatarReward();
		this->GetKnapsack()->GetMoney()->Add(0, add_money, 0, 0, "FirstSetAvatarTimeStamp");
		
		Protocol::SCAvatarTimeStampInfo cmd;
		cmd.is_change_avatar = m_is_change_avatar;
		cmd.reserve_1 = 0;
		cmd.reserve_2 = 0;

		EngineAdapter::Instance().NetSend(this->GetNetId(), (const char*)&cmd, sizeof(cmd));
	}

	EventHandler::Instance().OnCustomAvatarChange(this, m_avator_timestamp);
}


bool Role::IsInCross()
{
	return IsCrossUser(m_user_id);
}

// 获取唯一角色ID
UniqueUserID Role::GetUniqueUserID()
{
	// 直接用原始uuid(隐藏服里user_id会不一样，不用user_id来组合生成)
	return m_role_module->role_cross.GetCrossOriginalUniqueUserID();
}

void Role::GetCrossRoleParam(RoleAllParam *all_param)
{
	this->GetRoleInitParam(&all_param->role_param);
	this->GetRoleOtherInitParam(&all_param->other_param);

	int size = ItemNamespace::MAX_GRID_NUM;
	m_role_module->knapsack.GetItemList(&size, &all_param->item_param.item_list[0]);
	all_param->item_param.count = size;

	size = ItemNamespace::MAX_GRID_NUM - size;
	this->GetEquipmentManager()->GetEquipmentList(&size, &all_param->item_param.item_list[all_param->item_param.count]);
	all_param->item_param.count += size;

	m_role_module->hotkey_recorder.GetHotkeyList(&all_param->hotkey_param);
	m_role_module->friend_manager.GetFriendList(&all_param->friend_param);
	m_role_module->friend_manager.GetEnemyList(&all_param->enemy_param);
	m_role_module->friend_manager.GetBlackList(&all_param->black_param);
	m_skill_manager.GetSkillList(&all_param->skill_param);
	m_role_module->welfare.GetDailyFindList(&all_param->daily_find_param);

	all_param->role_param.gold = this->GetKnapsack()->GetMoney()->GetGold();
	all_param->role_param.gold_bind = this->GetKnapsack()->GetMoney()->GetGoldBind();
	all_param->role_param.coin = this->GetKnapsack()->GetMoney()->GetCoin();
	all_param->role_param.coin_bind = this->GetKnapsack()->GetMoney()->GetCoinBind();

	//all_param->role_param.guild_id = INVALID_GUILD_ID;
	//memset(all_param->role_param.guild_name, 0, sizeof(all_param->role_param.guild_name));
	//all_param->role_param.guild_post = GUILD_POST_INVALID;

	//all_param->other_param.itemcolddown_param.count = 0;

	//all_param->friend_param.count = 0;
	//all_param->enemy_param.count = 0;
	//all_param->business_param.count = 0;
	//all_param->daily_find_param.count = 0;

	GameName tmp_name; memset(tmp_name, 0, sizeof(tmp_name));
	SNPRINTF(tmp_name, sizeof(tmp_name), "%s_s%d", all_param->role_param.role_name, m_user_id.db_index);
	SNPRINTF(all_param->role_param.role_name, sizeof(all_param->role_param.role_name), "%s", tmp_name);
}

// 增加跨服荣誉
bool Role::AddCrossHonor(int honor_val, const char *type)
{
	if (0 == honor_val || NULL == type) return true;

	long long tmp_cross_honor = m_role_module->common_data_param.cross_honor;
	tmp_cross_honor += honor_val;

	if (tmp_cross_honor < 0) return false;

	if (tmp_cross_honor > MAX_CROSSHONOR_VAL) tmp_cross_honor = MAX_CROSSHONOR_VAL;

	m_role_module->common_data_param.cross_honor = static_cast<int>(tmp_cross_honor);

	static Protocol::SCCrossHonorChange chc;
	chc.honor = m_role_module->common_data_param.cross_honor;
	chc.delta_honor = honor_val;
	EngineAdapter::Instance().NetSend(m_netid, (const char *)&chc, sizeof(chc));

	ROLE_LOG_QUICK6(LOG_TYPE_CROSS_HONOR, this, m_role_module->common_data_param.cross_honor, honor_val, type, NULL);

	return true;
}

int Role::GetCrossHonor()
{
	return m_role_module->common_data_param.cross_honor;
}

int Role::GetRightWorldAnswerNum()
{
	return m_role_module->common_data_param.world_question_right_num;
}

unsigned int Role::GetRightWorldAnswerNumChangeTime()
{
	return m_role_module->common_data_param.world_question_right_num_change_time;
}

// 登录
void Role::OnLogin(long long last_save_time, IP userip, int plat_spid)			// 新角色的last_save_time是login给过来的 有可能比当前时间小
{
	m_userip = userip;
	m_login_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	m_role_module->common_data_param.next_fake_private_chat_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time() + FAKE_PRIVATE_CHAT_INTERVAL_S);

	{
		gamelog::g_log_login.printf(LL_DEBUG, "[Role::OnLogin][%d][%s]", UidToInt(m_user_id), m_name);
		ROLE_LOG_QUICK10(LOG_TYPE_LOGIN, this, this->GetIP(), plat_spid, NULL, NULL, this->GetLevel(), this->GetRoleActivity()->GetHistoryChongZhi(), this->GetCamp(), this->GetCapability()->GetTotalCapability());
		gamelog::g_log_global_user_state.printf(LL_INFO, "[LoginServer::OnGlobalUserLogin Success] user[%s, %d] ip[%u] is_micro_pc[%d].", m_name, this->GetUID(), this->GetIP(), 1);

#ifdef _DEBUG
		printf("Role Login: %d \n", m_uid);
#endif
	}
	
	// 让以前global的东西先执行
	UserCacheManager::Instance().UpdateUserNameCache(this, true); // 这行放最前边，组队和好友需要用到用户在线状态信息
	TeamManager::Instance().OnUserLogin(this);
	GuildManager::Instance().OnUserLogin(this);
	ActivityManager::Instance().OnUserLogin(this);
	OfflineRegisterManager::Instance().OnUserFirstEnterScene(this);
	CampManager::Instance().OnUserLogin(this);
	SpecialActivityManager::Instance().OnUserLogin(this);
	RankManager::Instance().OnUserLogin(this);
	ChallengeField::Instance().OnUserLogin(this);
	CrossChallengeField::Instance().OnUserLogin(this);
	UserMailManager::Instance().OnUserLogin(this);
	ChatManager::Instance().OnUserLogin(this);
	RandActivityManager::Instance().OnUserLogin(this);
	FishPoolManager::Instance().OnUserLogin(this);
	FBManager::Instance().OnUserLogin(this);
	WorldEventManager::Instance().OnUserLogin(this);
	ActivityShadow::Instance().OnUserLogin(this);
	SystemMonitor::Instance().OnRoleLogin(this);
	RoleBossManager::Instance().OnRoleLogin(this);
	CrossUserShadow::Instance().OnUserLogin(this);
	CrossUserRegister::Instance().OnUserLogin(this);
	CrossRecordManager::Instance().OnUserLogin(this);
	YaoShouGuangChangMgr::Instance().OnRoleLogin(this);
	SuoYaoTaMgr::Instance().OnRoleLogin(this);
	ZhuaGuiMgr::Instance().OnRoleLogin(this);
	JingHuaHuSongMgr::Instance().OnRoleLogin(this);
	HuangchenghuizhanMgr::Instance().OnRoleLogin(this);
	BossFamilyManager::Instance().OnRoleLogin(this);
	GlobalRoleManager::Instance().OnRoleLogin(this);
	CrossRAManagerLocal::Instance().OnUserLogin(this);
	JinjieSysRewardMgr::Instance().OnRoleLogin(this);

	m_has_send_all_info = false;

	m_role_module->sublock.OnRoleLogin();
	m_role_module->role_activity.OnRoleLogin();
	m_role_module->welfare.OnRoleLogin(last_save_time);
	m_role_module->month_card.OnLogin();
	m_role_module->active_degree.OnUserLogin();
	m_role_module->role_cross.OnLogin();
	m_role_module->shizhuang.OnLogin();
	m_role_module->mount_manager.OnLogin();
	m_role_module->wing.OnLogin();
	m_role_module->halo.OnLogin();
	m_role_module->shengong.OnLogin();
	m_role_module->shenyi.OnLogin();
	m_role_module->cloak_manager.OnLogin();
	m_role_module->fightmount_manager.OnLogin();
	m_role_module->multi_mount.OnRoleLogin();
	m_role_module->title_manager.OnRoleLogin();
	m_role_module->role_guild.OnRoleLogin();
	m_role_module->chinese_zodiac.OnRoleLogin();
	m_role_module->qingyuan.OnRoleLogin((unsigned int)last_save_time);

	m_role_module->friend_exp_bottle.OnRoleLogin();
	m_role_module->mysterious_shop_in_mall.OnRoleLogin();
	m_role_module->towerdefend_fb.OnLogin();
	m_role_module->runesystem_manger.OnRoleLogin((unsigned int)last_save_time);
	m_role_module->shenzhou_weapon_manger.SendBoxInfo();
	m_role_module->paohuan_task.SendPaohuanTaskInfo();
	m_role_module->shenshou.CheckFirstHuanlingRefresh();
	m_role_module->img_fuling.CheckFirstLoadImg();
	m_role_module->knapsack.SendKnapsackGridExtendAuto();
	m_role_module->upgrade_rush.OnRoleLogin();
	m_role_module->fabao.OnLogin();
	m_role_module->role_challenge_field.SendRoleCFBestRankBreakInfo();
	m_role_module->jingjie.OnLogin();
	m_role_module->jingling_mgr.OnLogin();
	m_role_module->m_imp_guard.OnRoleLogin();
	m_role_module->yaoshi_manager.OnRoleLogin();
	m_role_module->toushi_manager.OnRoleLogin();
	m_role_module->qilinbi_manager.OnRoleLogin();
	m_role_module->mask_manager.OnRoleLogin();
	m_role_module->jinjie_talent.OnLogin();
	m_role_module->shengqi.OnRoleLogin();	
	m_role_module->dressing_room_mgr.OnRoleLogin();
	m_role_module->upgrade_manager.OnLogin();
	m_role_module->discount_buy.OnLogin();
	m_role_module->redequip_collect.OnRoleLogin();
	m_role_module->orang_equip_collect.OnRoleLogin();
	m_role_module->role_big_small_goal.OnRoleLogin();
	m_role_module->shenge_system.OnRoleLogin();
	m_role_module->shenyin.OnRoleLogin();
	m_role_module->patafb.OnRoleLogin();
	m_role_module->shenzhou_weapon_manger.OnRoleLogin();
	m_role_module->chengjiu_mgr.OnLogin();
	m_role_module->xiannv_manager.OnLogin();
	m_role_module->jinglingfazhen_manager.OnLogin();
	m_role_module->equip_baptize.OnRoleLogin();
	m_role_module->role_personboss_fb.OnRoleLogin();
	m_role_module->touzijihua.OnRoleLogin();
	m_role_module->m_greate_soldier.OnRoleLogin((unsigned int)last_save_time);
	m_role_module->lottery_count_module.OnRoleLogin();
	m_role_module->convert_record.OnRoleLogin();
	m_role_module->cross_rand_activity.OnRoleLogin();
	m_role_module->upgrade_card_buy.OnRoleLogin();
	m_role_module->footprint.OnLogin();
	m_role_module->cross_goal.OnUserLogin();

	m_role_other_mgr->OnRoleLogin();
	//for (IRoleModule *module : m_role_module_list)
	//{
	//	module->OnRoleLogin();
	//}

	/*int fb_key = ActivityShadow::Instance().GetFBKeyByRoomIndex(0);
	SpecialXingzuoYiji *xingzuoyiji_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialXingzuoYiji(fb_key, true);
	if (NULL != xingzuoyiji_logic)
	{
		xingzuoyiji_logic->SendtoRoleNotInScene(this);
	}*/

	this->AutoFetchAdvanceNoticDayReward();
	this->CheckGmTestAccount(last_save_time);
	this->SyncKillCrossBossRankSkill();
	TianShuXunZhu::OnRoleLogin(this);

	if (m_role_module->role_activity.GetParam()->current_lun_reward_gold > 0)
	{
		m_role_module->role_activity.OnRAFetchCornucopiaRewardInfo();
	}

	this->SyncCommonInfo(Protocol::SCIT_TODAY_FREE_RELIVE_NUM, m_role_module->common_data_param.today_use_free_relive_num);
	
	{
		// 更新全量缓存
		this->UpdateUserCache();
	}

	m_offline_day_num = GetDayIndex(static_cast<time_t>(last_save_time), EngineAdapter::Instance().Time());

	if (0 == m_online_time)
	{
		m_honour = GLOBALCONFIG->GetHonourInitValue();
	}
	else
	{ 
		if (GetWeekIndex(last_save_time, EngineAdapter::Instance().Time()) > 0)
		{
			const tm *now_tm = EngineAdapter::Instance().LocalTime();
			if (NULL != now_tm && 1 != now_tm->tm_wday)					// 周一的时候由OnDayChange触发
			{
				this->OnWeekChange();
			}
		}

		if (GetMonthIndex(last_save_time, EngineAdapter::Instance().Time()) > 0)
		{
			const tm *now_tm = EngineAdapter::Instance().LocalTime();
			if (NULL != now_tm && 1 != now_tm->tm_mday)					// 一号的时候由OnDayChange触发
			{
				this->OnMonthChange();
			}
		}
	}

	EngineAdapter::Instance().NetHasCheck(this->GetNetId());

	{
		// 如果是登录的话enterscene的时候不能获知自己是否在队伍，所以需要在这里补做更新自己队伍信息的操作
		if (m_role_module->team_info.InTeam())
		{
			TeamManager::Instance().OnUserChangeScene(m_role_module->team_info.GetTeamIndex(), m_user_id, m_scene->GetSceneID(), m_scene->GetSceneKey(), m_obj_id);
		}
	}

	// 检查是否重新进入跨服
	m_role_module->role_cross.CheckCrossServerCross3v3MatchingState();
	m_role_module->role_cross.CheckCanRetransferToCrossOnRoleLogin();
}

void Role::OnLoginFinish(long long last_save_time)
{
	// ...

	// 检查BOSS场景的切回主城，执行了Goto，注意this指针的使用
	BossFamilyManager::Instance().OnCheckComeBackWithOnLogin(this, last_save_time);
}

// 禁止发言
void Role::ForbidTalk(time_t forbid_time, const char *reason, const char *last_msg)
{
	if (NULL == reason || NULL == last_msg) return;

	m_forbid_talk_time = EngineAdapter::Instance().Time() + forbid_time;

	gamelog::g_log_forbid.printf(LL_INFO, "[Role::ForbidTalk][%d][%s] plat_name:%s forbid_time:%u reason:%s last_msg:%s", 
		UidToInt(m_user_id), (const char*)m_name, (const char*)m_plat_name, (unsigned int)forbid_time, (const char*)reason, (const char*)last_msg);

	EventHandler::Instance().OnForbidRole(this->GetUID());

	ROLE_LOG_QUICK6(LOG_TYPE_FORBID_TALK, this, forbid_time, 0, reason, last_msg);
}

// 禁止角色
void Role::ForbidRole(const char *reason, int forbit_time)
{
	if (NULL == reason || forbit_time <= 0) return;

	lgprotocal::RoleForbid rf;
	this->GetPlatName(rf.plat_name);
	STRNCPY(rf.reason, reason, sizeof(rf.reason));
	rf.forbit_time_s = forbit_time;
	InternalComm::Instance().NetSend(InternalComm::Instance().m_login_server_id, (const char *)&rf, sizeof(lgprotocal::RoleForbid));

	gamelog::g_log_forbid.printf(LL_INFO, "[Role::ForbidRole][%d][%s] plat_name:%s reason:%s", 
		UidToInt(m_user_id), (const char*)m_name, (const char*)m_plat_name, (const char*)reason);

	EventHandler::Instance().OnForbidRole(this->GetUID());

	ROLE_LOG_QUICK6(LOG_TYPE_FORBID_ROLE, this, 0, 0, reason, NULL);
}

// 禁止发言
bool Role::IsForbidTalk()
{
	if (m_forbid_talk_time >= EngineAdapter::Instance().Time())
	{
		this->NoticeNum(errornum::EN_FORBID_TALK);

		return true;
	}

	return false;
}

bool Role::SystemMsg(const char *msg, short length, short msg_type)
{
	if (NULL == msg || length < 0 || length > PROTOCAL_MAX_SYSTEM_MSG_LENGTH) return false;

	static Protocol::SCSystemMsg sm;

	sm.send_time = (unsigned int)EngineAdapter::Instance().Time();
	sm.msg_type = msg_type;
	sm.msg_length = length;
	memcpy(sm.msg, msg, length);
	sm.color = 0;
	sm.display_pos = 0;

	int sendlen = sizeof(Protocol::SCSystemMsg) - PROTOCAL_MAX_SYSTEM_MSG_LENGTH + length;
	return EngineAdapter::Instance().NetSend(m_netid, (const char *)&sm, sendlen);
}

// 是敌人
bool Role::IsEnemy(Obj *obj)
{
	return m_scene->GetSpecialLogic()->SpecialIsEnemy(this, obj);
}

// 是友盟
bool Role::IsFriend(Obj *obj, bool check_alive)
{
	return m_scene->GetSpecialLogic()->SpecialIsFriend(this, obj, check_alive);
}

// 角色是敌人
bool Role::RoleIsEnemy(Role *target_role)
{
	if (target_role == this)
	{
		return false;
	}

	// 反击对象放在最前面判断
	if (FightBackManager::Instance().IsFightBackObj(this, target_role))
	{
		return true;
	}

	if (m_scene->ForbidPK()) 
	{
		return false;
	}

	char fishing_station = target_role->GetRoleActivity()->GetFishingStation();
	if (0 != fishing_station)
	{
		return false;
	}

	if (NAME_COLOR_WHITE == target_role->GetNameColor())
	{
		const tm *local_time = EngineAdapter::Instance().LocalTime();
		if (NULL != local_time && local_time->tm_hour >= 0 && local_time->tm_hour < 8)
		{
			if (LOGIC_CONFIG->GetGuajiActivityConfig().IsGuaJiSceneID(m_scene->GetSceneID()))
			{
				return false;
			}
		}
	}

	if (MOVE_MODE_FLY == target_role->GetMoveMode()) return false;

	// 新手保护等级, // 2019-04-02普通非活动场景才有新手保护
	const int XINSHOU_PROTECTED_LEVEL = GLOBALCONFIG->GetRoleXinShouProtectedLevel();
	if (target_role->GetLevel() < XINSHOU_PROTECTED_LEVEL && m_scene->InCommonScene() && !Scene::IsBossScene(m_scene->GetSceneID()))
	{
		return false;
	}
	
	// 活跃boss第一层，强制和平模式
	//if (Scene::IsActiveBossSceneOneLayer(m_scene->GetSceneID()))
	//{
	//	return false;
	//}

	// 是否是巡游状态
	if (target_role->GetQingyuan()->IsInXunyouStatus())
	{
		return false;
	}

	// PK模式
	switch (m_attack_mode)
	{
		case ATTACK_MODE_PEACE:
			{
				if (target_role->m_name_color > NAME_COLOR_WHITE)
				{
					// 可攻击红名玩家
					return true;
				}
				else
				{
					return false;
				}
			}
			break;

		case ATTACK_MODE_ALL:
			{
				return true;
			}
			break;

		case ATTACK_MODE_GUILD:
			{
				if (target_role->m_name_color > NAME_COLOR_WHITE)
				{
					// 可攻击红名玩家
					return true;
				}

				if (INVALID_GUILD_ID == m_guild_id || target_role->GetGuildID() != m_guild_id)
				{
					//仙盟模式允许打队友
					//if (!m_role_module->team_info.InTeam() || !target_role->GetTeamInfo()->InTeam() || (m_role_module->team_info.GetTeamIndex() != target_role->GetTeamInfo()->GetTeamIndex()))
					{
						return true;
					}
				}
			}
			break;
		
		case ATTACK_MODE_TEAM:
			{
				if (!m_role_module->team_info.InTeam() || !target_role->GetTeamInfo()->InTeam() || (m_role_module->team_info.GetTeamIndex() != target_role->GetTeamInfo()->GetTeamIndex()))
				{
					return true;
				}
			}
			break;

		case ATTACK_MODE_NAMECOLOR:
			{
				if (target_role->m_name_color > NAME_COLOR_WHITE)
				{
					return true;
				}
			}
			break;

		case ATTACK_MODE_CAMP:
			{
				if (CAMP_TYPE_INVALID == m_camp || target_role->GetCamp() != m_camp)
				{
					return true;
				}
			}
			break;

		case ATTACK_MODE_SAME_SERVER:
		{
			if (!this->GetRoleCross()->IsFromSameGameServer(target_role))
			{
				return true;
			}
		}
		break;

		case ATTACK_MODE_ENEMY:
		{
			if (this->GetFriendManager()->IsEnemy(target_role->GetUniqueUserID()))
			{
				return true;
			}
			else
			{
				return false;
			}
		}
		break;
	}

	return false;
}

// 角色影子是敌人
bool Role::RoleShadowIsEnemy(RoleShadow *role_shadow)
{
	if (NULL == role_shadow) return false;

	if (ROLE_SHADOW_TYPE_CAMPDEFEND == role_shadow->GetRoleShadowType())
	{
		return CampDefendMgr::Instance().IsDiaoxiangCanBeAttacked(role_shadow, this);
	}

	if (ROLE_SHADOW_TYPE_CLONE_ROLE == role_shadow->GetRoleShadowType())
	{
		return this->GetUID() != UidToInt(role_shadow->GetUserId());
	}

	return true;
}

void Role::SetTopDpsFlag(bool top_dps_flag)
{
	m_top_dps_flag = top_dps_flag;
	Protocol::SCBossDpsTitle fhi;
	fhi.obj_id = m_obj_id;
	fhi.first_hurt_flag = m_top_dps_flag ? 1 : 0;
	m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&fhi, sizeof(fhi));
}

void Role::SetFirstHitFlag(bool first_hit_flag)
{
	if (first_hit_flag != m_first_hit_flag)
	{
		Protocol::SCBossFirstHurtInfo fhi;

		fhi.obj_id = m_obj_id;
		fhi.first_hurt_flag = first_hit_flag ? 1 : 0;

		m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&fhi, sizeof(fhi));
	}

	m_first_hit_flag = first_hit_flag;
	m_first_hit_have_died = false;
}

void Role::NotifyFBFinish()
{
	Protocol::SCFBFinish fbfinish;
	EngineAdapter::Instance().NetSend(m_netid, (const char*)&fbfinish, sizeof(fbfinish));
}

// 角色是友盟
bool Role::RoleIsFriend(Role *target_role)
{
	if (target_role == this) return true;
	// if (target_role->GetUserId() == m_last_attack_me_user_id) return false;	// 最后一击打我的人 任何时候都不是我的朋友

	return !RoleIsEnemy(target_role);
}

bool Role::CheckMapValidate()
{
	return (MOVE_MODE_NORMAL == m_move_mode && m_level > 10);
}

bool Role::CheckMoveFrequency()
{
	return MOVE_MODE_JUMP != m_move_mode && MOVE_MODE_JUMP2 != m_move_mode;
}

float Role::GetMoveRate()
{
	if (MOVE_MODE_JUMP == m_move_mode)
	{
		return 1.8f;
	}
	else if (MOVE_MODE_JUMP2 == m_move_mode)
	{
		return 5.0f;
	}

	return 1.0f;
}

int Role::GetPutOnLevel()
{
	EffectOtherBuff *buff = this->GetEffectList()->FindOtherEffectBuff(EffectBase::PRODUCT_ID_INC_PUTON_LEVEL);
	int level = m_level;
	if (NULL != buff)
	{
		level += buff->GetIncPutOnLevel();
	}

	return level;
}

// 设置阵营
void Role::SetCamp(char camp_type, bool is_random)
{
	if (camp_type <= CAMP_TYPE_INVALID || camp_type >= CAMP_TYPE_MAX) return;
	if (CAMP_TYPE_INVALID != m_camp) return;	

	if (is_random)
	{
		m_role_module->knapsack.GetMoney()->AddGoldBind(GLOBALCONFIG->GetRandomCampGoldBind(), "RandomCamp");
	}

	m_camp = camp_type;

	EventHandler::Instance().OnChangeCamp(this, m_camp);

	this->OnRoleBaseInfoChange();

	Protocol::SCRoleAddCamp add_camp;
	add_camp.obj_id = m_obj_id;
	add_camp.camp = m_camp;
	add_camp.reserved = 0;
	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&add_camp, sizeof(add_camp));

	int notice_num = 0;
	if (CAMP_TYPE_FEIXING == m_camp)
	{
		notice_num = noticenum::NT_SELECT_CAMP_1_SUCC;
	}
	else if (CAMP_TYPE_ZHURI == m_camp)
	{
		notice_num = noticenum::NT_SELECT_CAMP_2_SUCC;
	}
	else if (CAMP_TYPE_ZHUIYUE == m_camp)
	{
		notice_num = noticenum::NT_SELECT_CAMP_3_SUCC;
	}

	if (notice_num > 0)
	{
		this->NoticeNum(notice_num);
	}

	CampManager::Instance().OnRoleJoinCamp(m_camp);

	// 角色加入阵营频道
	if (m_camp != CAMP_TYPE_INVALID)
	{
		int cid = chatdef::CHANNEL_ID_CAMP_BEGIN + m_camp;
		ChatManager::Instance().AddToChannel(cid, m_uid);
	}
}

// 设置职业
void Role::SetProf(char prof)
{
	if (prof <= PROF_TYPE_INVALID || prof >= PROF_TYPE_PROF_NO_LIMIT) return;

	if (PROF_TYPE_INVALID != m_prof)
	{
		this->NoticeNum(errornum::EN_ROLE_HAS_CHOOSE_PROF);
		return; 
	}

	m_prof = prof;
	EventHandler::Instance().OnChangeProf(this, m_prof);

	m_skill_manager.InitDefaultSkill();
	m_skill_manager.SendSkillListInfo();

	this->AddNuqi(GLOBALCONFIG->GetMaxNuqi());

	this->ReCalcAttr(CharIntAttrs::RECALC_REASON_SELF, false, true);

	this->OnRoleBaseInfoChange();

	Protocol::SCRoleChangeProf chg_prof;
	chg_prof.obj_id = m_obj_id;
	chg_prof.prof = m_prof;
	chg_prof.maxhp = 0;
	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&chg_prof, sizeof(chg_prof));

	int notice_num = 0;
	if (1 == m_prof)
	{
		notice_num = noticenum::NT_SELECT_PROF_1_SUCC;
	}
	else if (2 == m_prof)
	{
		notice_num = noticenum::NT_SELECT_PROF_2_SUCC;
	}
	else if (3 == m_prof)
	{
		notice_num = noticenum::NT_SELECT_PROF_3_SUCC;
	}
	else if (4 == m_prof)
	{
		notice_num = noticenum::NT_SELECT_PROF_4_SUCC;
	}

	if (notice_num > 0)
	{
		this->NoticeNum(notice_num);
	}

	gamelog::g_log_debug.printf(LL_DEBUG, "Role::SetProf:%d %s[%d %d]", m_prof, m_name, m_user_id.db_index, m_user_id.role_id);
}

// GM设置职业
void Role::GMSetProf(char prof)
{
	if (prof <= PROF_TYPE_INVALID || prof >= PROF_TYPE_PROF_NO_LIMIT) return;

	m_prof = prof;
	EventHandler::Instance().OnChangeProf(this, m_prof);

	m_skill_manager.InitDefaultSkill();
	m_skill_manager.SendSkillListInfo();

	this->AddNuqi(GLOBALCONFIG->GetMaxNuqi());

	this->ReCalcAttr(CharIntAttrs::RECALC_REASON_SELF, false, true);

	this->OnRoleBaseInfoChange();

	Protocol::SCRoleChangeProf chg_prof;
	chg_prof.obj_id = m_obj_id;
	chg_prof.prof = m_prof;
	chg_prof.maxhp = 0;
	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&chg_prof, sizeof(chg_prof));

	int notice_num = 0;
	if (1 == m_prof)
	{
		notice_num = noticenum::NT_SELECT_PROF_1_SUCC;
	}
	else if (2 == m_prof)
	{
		notice_num = noticenum::NT_SELECT_PROF_2_SUCC;
	}
	else if (3 == m_prof)
	{
		notice_num = noticenum::NT_SELECT_PROF_3_SUCC;
	}
	else if (4 == m_prof)
	{
		notice_num = noticenum::NT_SELECT_PROF_4_SUCC;
	}

	if (notice_num > 0)
	{
		this->NoticeNum(notice_num);
	}

	gamelog::g_log_debug.printf(LL_DEBUG, "Role::SetProf:%d %s[%d %d]", m_prof, m_name, m_user_id.db_index, m_user_id.role_id);
}

// 设置性别
bool Role::SetSex(char sex)
{
	if (sex < FEMALE || sex >= SEX_MAX || sex == m_sex) return false;

	if (this->IsMarry()) 
	{
		this->NoticeNum(errornum::EN_ROLE_IS_MARRY_CAN_NOT_CHANGE_SEX);
		return false;
	}

	m_sex = sex;

	{
		this->GetEquipmentManager()->ChangeAllItemToOtherSex();
		this->GetKnapsack()->ChangeAllItemToOtherSex();
	}

	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_role_set_sex_notice, 
			UidToInt(m_user_id), m_name, m_camp);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
		}
	}

	this->OnRoleBaseInfoChange();

	Protocol::SCRoleChangeSex chg_sex;
	chg_sex.obj_id = m_obj_id;
	chg_sex.sex = m_sex;
	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&chg_sex, sizeof(chg_sex));

	this->GetFriendManager()->OnRoleSetSex();

	TuodanManager::Instance().OnSetSex(this);

	return true;
}

// 设置公会幸运色
int Role::GetGuildLuckyColor()
{
	if (m_guild_id > 0)
	{
		Guild *guild = GuildManager::Instance().GetGuild(m_guild_id);
		if (NULL != guild)
		{
			return guild->GetMemberLuckyColor(m_user_id);
		}
	}

	return 0;
}

unsigned long long Role::GetGuildAvatar()
{
	if (m_guild_id > 0)
	{
		Guild *guild = GuildManager::Instance().GetGuild(m_guild_id);
		if (NULL != guild)
		{
			return guild->GetGuildAvatar();
		}
	}

	return 0;
}

// 转职
void Role::Zhuanzhi()
{
	if (m_prof <= PROF_TYPE_INVALID || m_prof + PROF_TYPE_PROF_ZHUANZHI_BASE >= PROF_TYPE_MAX) return;

	m_prof = m_prof + PROF_TYPE_PROF_ZHUANZHI_BASE;

	m_skill_manager.OnZhuanzhi();
	this->OnRoleBaseInfoChange();

	EventHandler::Instance().OnZhuanzhi(this);

	Protocol::SCRoleChangeProf chg_prof;
	chg_prof.obj_id = m_obj_id;
	chg_prof.prof = m_prof;
	chg_prof.maxhp = 0;
	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&chg_prof, sizeof(chg_prof));

	//this->NoticeNum(noticenum::NT_ZHUANZHI_SUCC);

	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_zhuanzhi,
					UidToInt(m_user_id), m_name, m_camp, m_prof);
	if (length > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
	}

	gamelog::g_log_debug.printf(LL_DEBUG, "Role::Zhuanzhi:%d %s[%d %d]", m_prof, m_name, m_user_id.db_index, m_user_id.role_id);
}


// 增加公会贡献
bool Role::AddGuildGongxian(int gongxian, const char *type)
{
	if (0 == gongxian || NULL == type) return false;

	m_guild_gongxian += gongxian;
	if (m_guild_gongxian <= 0) m_guild_gongxian = 0;

	if (gongxian > 0)
	{
		m_guild_total_gongxian += gongxian;
		m_role_module->role_guild.SetDailyGongXian(m_role_module->role_guild.GetDailyGongXian() + gongxian);
		m_role_module->role_guild.SendRoleGuildInfo();
	}

	this->NotifyGuildInfoToClient();
	this->SyncGuildInfoToGlobal();

	EventHandler::Instance().OnGuildGongXian(this, 0);

	gamelog::g_log_roledata.printf(LL_INFO, "[AddGuildGongxian][user[%d %s], type:%s, add_gongxian:%d now:%d total:%d]",
		UidToInt(m_user_id), m_name, type, gongxian, m_guild_gongxian, m_guild_total_gongxian);

	return true;
}

// 角色基础信息改变
void Role::OnRoleBaseInfoChange()
{
	// usercache依赖
	{
		m_role_module->common_data_param.qingyuan_capablity = m_role_module->capablity.GetRoleCapability(CAPABILITY_TYPE_QINGYUAN) + m_role_module->capablity.GetRoleCapability(CAPABILITY_TYPE_LOVE_TREE);
		m_role_module->common_data_param.baby_capablity = m_role_module->capablity.GetRoleCapability(CAPABILITY_TYPE_BABY);
		m_role_module->common_data_param.little_pet_capablity = m_role_module->capablity.GetRoleCapability(CAPABILITY_TYPE_LITTLE_PET);
	}
	// 更新角色缓存，必须放最前面，后边依赖角色缓存信息
	this->UpdateUserCache();

	// 排行榜
	RankManager::Instance().SyncPersonBaseInfo(this);

	// 组队
	TeamManager::Instance().SyncPersonBaseInfo(this);

	// 跨服记录
	CrossRecordManager::Instance().OnSyncRoleBaseInfo(this);

	// 其他 待定
}

// 同步公会信息到全局
void Role::SyncGuildInfoToGlobal()
{
	if (INVALID_GUILD_ID != m_guild_id)
	{
		Guild *guild = GuildManager::Instance().GetGuild(m_guild_id);
		if (NULL != guild)
		{
			GuildMember *member = guild->GetGuildMemberManager().GetMember(m_user_id);
			if (NULL != member)
			{
				member->OnSyncInfo(m_guild_gongxian, m_guild_total_gongxian);
			}
		}
	}
}

// 角色升级
bool Role::OnUpgradeLevel(int to_level)
{
	if (to_level <= 0 || to_level > MAX_ROLE_LEVEL_VERSION_LIMIT) return false;
	
	const bool is_bopen = DropLogManager::Instance().IsBopenBegin();
	if (is_bopen)
	{
		DropLogManager::Instance().SetBopenBegin(false);
	}

	int old_level = m_level;
	m_level = to_level; 

	if (1 == old_level)
	{
		WorldStatus::Instance().OnFakeFirstChongzhiSystemMsg();
	}

	m_exp = 0;
	m_max_exp = GLOBALCONFIG->GetRoleMaxExp(m_level);

	Protocol::SCChaLevelUpgrade clu;
	clu.obj_id = m_obj_id;
	clu.level = m_level;
	clu.exp = m_exp;
	clu.max_exp = m_max_exp;
	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, &clu, sizeof(Protocol::SCChaLevelUpgrade));

	m_role_module->mount_manager.OnRoleLevelChange(m_level);
	m_role_module->fightmount_manager.OnRoleLevelChange(m_level);
	m_role_module->wing.OnRoleLevelChange(m_level);
	m_role_module->halo.OnRoleLevelChange(m_level);
	m_role_module->shengong.OnRoleLevelChange(m_level);
	m_role_module->shenyi.OnRoleLevelChange(m_level);
	m_role_module->footprint.OnRoleLevelChange(m_level);
	m_role_module->cloak_manager.OnRoleLevelChange(m_level);
	m_role_module->xiannv_manager.OnRoleLevelChange(m_level);
	m_skill_manager.OnRoleLevelChange(m_level);
	m_role_module->role_goal.OnRoleLevelChange(m_level);
	m_role_module->wabao.OnRoleLevelChange(old_level, m_level);
	m_role_module->friend_gift_manager.OnRoleLevelChange(m_level);
	m_role_module->friend_exp_bottle.OnRoleUpLevel(m_level);
	m_role_module->magical_precious_manager.OnRoleLevelChange(m_level);
	m_role_module->shenshou.CheckFirstHuanlingRefresh();
	m_role_module->yaoshi_manager.OnRoleLevelChange(m_level);
	m_role_module->toushi_manager.OnRoleLevelChange(m_level);
	m_role_module->qilinbi_manager.OnRoleLevelChange(m_level);
	m_role_module->mask_manager.OnRoleLevelChange(m_level);
	m_role_module->jingling_mgr.OnRoleLevelChange(m_level);
	m_role_module->shenge_system.OnRoleLevelChange(m_level);
	m_role_module->shenyin.OnRoleLevelChange(m_level);
	m_role_module->shenzhou_weapon_manger.OnRoleLevelChange(m_level);
	m_role_module->shenshou.OnRoleLevelChange(m_level);
	m_role_module->runesystem_manger.OnRoleLevelChange(m_level);
	m_role_module->redequip_collect.OnRoleLevelUp(m_level);
	m_role_module->role_activity.OnRoleLevelChange(m_level);
	m_role_module->m_greate_soldier.OnRoleLevelChange(m_level);
	m_role_module->shengqi.OnRoleLevelChange(m_level);
	m_role_module->chinese_zodiac.OnRoleLevelChange(m_level);
	JinjieSysRewardMgr::Instance().OnRoleLevelChange(this, m_level);
	m_role_module->role_big_small_goal.OnRoleLevelChange(m_level);
	m_role_module->convert_record.OnRoleLevelChange(m_level);
	
	m_role_module->touzijihua.SendInfo();
	LOGIC_CONFIG->GetLieMingCfg().ActiveSoltByRoleLevel(this, m_level);

	this->AutoFetchAdvanceNoticDayReward();

	this->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT, false, false);
	this->ReCalcAttr(CharIntAttrs::RECALC_REASON_SELF, false, true);
		
	this->OnRoleBaseInfoChange();
	
	EventHandler::Instance().OnRoleUpgradeLevel(this, old_level, m_level);

	if (is_bopen && !DropLogManager::Instance().IsBopenBegin())
	{
		DropLogManager::Instance().SetBopenBegin(true);
	}

	ROLE_LOG_QUICK6(LOG_TYPE_UPGRADE_LEVEL, this, m_level, this->GetCapability()->GetTotalCapability(), NULL, NULL);

	return true;
}

// 进入场景
void Role::OnEnterScene()
{
	int scene_id = m_scene->GetSceneID();

	if (NULL != m_scene && !m_scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, m_posi.x, m_posi.y))
	{
		Posi realive_pos(0, 0); m_scene->GetTownPoint(this, &realive_pos);
		if (0 != realive_pos.x && 0 != realive_pos.y)
		{
			this->SetPos(Posi(realive_pos.x, realive_pos.y));
		}
	}

	m_birth_pos = m_posi;

	// 发送进入场景消息通知
	Protocol::SCEnterScene es;
	es.sceneid = scene_id;
	es.obj_id = m_obj_id;
	es.open_line = 0;
	SceneLineInfo *line_info = SceneRegister::Instance().Instance().GetSceneLineInfo(scene_id);
	if (NULL != line_info)
	{
		es.open_line = line_info->open_line;
	}
	es.pos_x = (short)m_posi.x;
	es.pos_y = (short)m_posi.y;
	es.hp = this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	es.max_hp = this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	es.scene_key = m_scene->GetSceneKey();
	EngineAdapter::Instance().NetSend(m_netid, (const char*)&es, sizeof(Protocol::SCEnterScene));

	Character::OnEnterScene();

	// role关注区域
	static const int CLIENT_SCREEN_X = (int)GLOBALCONFIG->GetKeyConfig_RoleAoiX();			// 真实屏幕50 该数字用于计算Role的AOI进入范围
	static const int CLIENT_SCREEN_Y = (int)GLOBALCONFIG->GetKeyConfig_RoleAoiY();			// 真实屏幕30
	static const int ROLE_AOI_LEAVE_MORE_THEN_ENTER = (int)GLOBALCONFIG->GetKeyConfig_RoleAoiLeaveMoreThenEnter();	// Role的AOI离开大于进入范围的值

	int screen_x = CLIENT_SCREEN_X;
	int screen_y = CLIENT_SCREEN_Y;

	const FbSceneConfig* fbconfig =  LOGIC_CONFIG->GetFbSceneConfig().GetFbSceneCfg(m_scene->GetSceneType());
	if (fbconfig)
	{
		screen_x *= (fbconfig->view_double + 1);
		screen_y *= (fbconfig->view_double + 1);
	}

	m_aoi_handle = m_scene->GetZoneMatrix()->CreateRectAOI(m_obj_id, m_posi, Posi(screen_x/2, screen_y/2), 
		Posi(screen_x/2 + ROLE_AOI_LEAVE_MORE_THEN_ENTER, screen_y/2 + ROLE_AOI_LEAVE_MORE_THEN_ENTER));

	//m_team_info.SendTeamMemberPosToMe(); m_team_info.SendMyPosToMember();

	{
		// 进入场景的时候需要修正队伍信息  
		if (m_role_module->team_info.InTeam())  
		{
			TeamManager::Instance().OnUserChangeScene(m_role_module->team_info.GetTeamIndex(), m_user_id, m_scene->GetSceneID(), m_scene->GetSceneKey(), m_obj_id);
		}
	}

	{
		this->CheckTaskAppearnStatusWithEnterScene();
	}

	// EventHandler::Instance().OnGetTo(this, scene_id, m_posi);

	if (!m_scene->InStaticScene()) this->GetRoleStatusManager()->OnEnterFB();

	m_enter_scene_time = EngineAdapter::Instance().GetGameTime();

	if (!m_scene->InStaticScene()) m_scene->OnTrigger(TRIGGER_COND_TYPE_ENTER_SCENE, this);

	{
		if (LOGIC_CONFIG->GetJoinLimitConfig().CanJoinOther(JLMT_OTHER_CHALLENGE_FIELD, this))
		{
			ChallengeField::Instance().OnSyncChallengeParam(this);
		}

		CrossChallengeField::Instance().OnSyncChallengeParam(this);

		this->SetNameColor((char)GLOBALCONFIG->GetNameColorByEvil(m_evil));
		ActivityManager::Instance().OnUserEnterScene(this);
		TopRanker::Instance().OnEnterScene(this);
		WorldStatus::Instance().OnEnterScene(this);
		CampDefendMgr::Instance().OnEnterScene(this);

		m_role_module->newtask_manager.OnEnterScene(scene_id);
		m_role_module->newtask_manager.OnEnterSceneType(m_scene->GetSceneType());
	}
	m_role_module->multi_mount.OnEnterScene(scene_id);
	m_role_module->qingyuan.OnEnterScene(scene_id);
	m_role_module->patafb.OnEnterScene(scene_id);
	m_role_module->redequip_collect.OnEnterScene(scene_id);
	m_role_module->orang_equip_collect.OnEnterScene(scene_id);

	if (!IsAlive())
	{
		ReAlive(100, 100, GetPos(), true);
	}


}

// 离开场景
void Role::OnLeaveScene()
{
	if (MOVE_MODE_JUMP == m_move_mode)
	{
		this->SetMoveMode(MOVE_MODE_NORMAL);
	}

	this->ResetRoleStatus();
	this->ClearFightBuffEffect(EffectBase::CLEAR_REASON_LEAVE_SCENE, false, true);

	// 因角色logout函数不一定能调用到 （譬如 切换场景 不成功 角色不会存在于任何gameworld上）
	{
		//m_role_module->rank_shadow.OnRoleLeaveScene();
		ActivityManager::Instance().OnUserLeaveScene(this);
		P2PTradeManager::Instance().OnUserLeaveScene(this);
		
		if (LOGIC_CONFIG->GetJoinLimitConfig().CanJoinOther(JLMT_OTHER_CHALLENGE_FIELD, this))
		{
			ChallengeField::Instance().OnSyncChallengeParam(this);
		}

		CrossChallengeField::Instance().OnSyncChallengeParam(this);
	}

	m_role_module->team_info.SendMyPosToMember(true);
	m_role_module->jilian.OnRoleLeaveScene();
	m_role_module->paohuan_task.OnRoleLeaveScene();

	m_scene->GetZoneMatrix()->DestroyAOI(m_aoi_handle);
	Character::OnLeaveScene();

	/*Protocol::SCLeaveScene ls;
	ls.sceneid = m_scene->GetSceneID();
	EngineAdapter::Instance().NetSend(m_netid, (const char*)&ls, sizeof(Protocol::SCLeaveScene));*/
}

void Role::OnAOIRoleEnter(Obj *obj)
{
	Role *role = (Role*)obj;

	if (role->IsZombie() && role->GetLevel() <= 1) return;	// 屏蔽僵尸号

	role->SendEnterInfoToRole(this);

	if (NULL != m_scene && m_scene->GetSpecialLogic())
	{
		m_scene->GetSpecialLogic()->OnAOIRoleEnter(this, role);
	}
}

void Role::OnAOIMonsterEnter(Obj *obj)
{
	Monster *monster = (Monster*)obj;

	static Protocol::SCVisibleObjEnterMonster voem;
	voem.dir = monster->GetDir();
	voem.status_type = monster->GetMonsterAI()->GetStatusType();
	voem.obj_id = monster->GetId();

	voem.hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	voem.max_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);

	voem.level = monster->GetLevel();
	voem.monster_id = monster->GetMonsterId();
	voem.speed = (short)monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED);
	voem.pos_x = (short)monster->GetPos().x;
	voem.pos_y = (short)monster->GetPos().y;
	voem.distance = monster->GetDirDistance();

	//voem.buff_mark = monster->GetEffectList()->GetSpecialBuffMark();
	monster->GetEffectList()->GetSpecialBuffMark(voem.buff_mark);
	voem.special_param = monster->GetSpecialParam();
	voem.monster_key = monster->GetMonsterKey();

	EngineAdapter::Instance().NetSend(m_netid, (const char *)&voem, sizeof(Protocol::SCVisibleObjEnterMonster));

	// 首刀
	UniqueUserID first_hit_user_id = monster->GetMonsterAI()->GetFirstHitUser();
	if (INVALID_UNIQUE_USER_ID != first_hit_user_id)
	{
		Role *first_hit_role = m_scene->GetRoleByUUID(first_hit_user_id);
		if (first_hit_role != nullptr)
		{
			static Protocol::SCMonsterFirstHitInfo mfhi;

			mfhi.obj_id = obj->GetId();
			mfhi.is_show = 1;
			first_hit_role->GetName(mfhi.first_hit_user_name);
			first_hit_role->GetGuildName(mfhi.guild_name);
			mfhi.first_hit_user_obj_id = first_hit_role->GetId();
			mfhi.reserve_sh = 0;

			EngineAdapter::Instance().NetSend(m_netid, (const char *)&mfhi, sizeof(mfhi));
		}
	}

	// 掉落归属信息
	if (monster->GetDropOwnerUid() > 0 || INVALID_TEAM_INDEX != monster->GetDropOwnerTeamIndex())
	{
		monster->NotifyDropOwner(this);
	}

}

// 掉落物出现
void Role::OnAOIFallingEnter(Obj *obj)
{
	FallingItem *fallingitem = (FallingItem *)obj;

	Protocol::SCVisibleObjEnterFalling voef;
	voef.obj_id = fallingitem->GetId();
	voef.item_id = fallingitem->GetItemId();
	voef.obj_pos_x = (short)fallingitem->GetObjPosi().x;
	voef.obj_pos_y = (short)fallingitem->GetObjPosi().y;
	voef.pos_x = (short)fallingitem->GetPos().x;
	voef.pos_y = (short)fallingitem->GetPos().y;
	voef.owner_role_id = UidToInt(fallingitem->GetOwner());
	voef.coin_or_gold = fallingitem->GetCoinOrGold();
	voef.monster_id = fallingitem->GetMonsterId();
	voef.item_num = fallingitem->GetItemNum();
	voef.drop_time = fallingitem->GetDropTime();
	voef.create_interval = static_cast<short>(EngineAdapter::Instance().Time() - voef.drop_time); // 小于x秒为新创建的掉落物
	voef.is_create = fallingitem->IsCreate() ? 1 : 0;
	voef.is_buff_falling = fallingitem->IsBuffFalling() ? 1 : 0;
	voef.reserve = 0;
	voef.buff_appearan = fallingitem->GetBuffAppearan();

	EngineAdapter::Instance().NetSend(m_netid, (const char *)&voef, sizeof(voef));
}

void Role::OnAOIGatherEnter(Obj *obj)
{
	GatherObj *gather_obj = (GatherObj*)obj;

	Protocol::SCVisibleObjEnterGather voeg;
	voeg.obj_id = gather_obj->GetId();
	voeg.gather_id = gather_obj->GatherId();
	voeg.pos_x = (short)gather_obj->GetPos().x;
	voeg.pos_y = (short)gather_obj->GetPos().y;
	voeg.special_gather_type = (short)gather_obj->GetSpecialGatherType();

	if (SPECIAL_GATHER_TYPE_JINGHUA == voeg.special_gather_type)
	{
		voeg.param_0 = JingHuaHuSongMgr::Instance().GetRemainGatherTimes();
	}
	else if (SPECIAL_GATHER_TYPE_GUILD_BONFIRE == voeg.special_gather_type)
	{
		voeg.param_0 = gather_obj->GetBelongGuildID();
		gather_obj->GetBelongGuildName(voeg.param_2);
	}

	else if (SPEICAL_GATHER_TYPE_CROSS_FISHING == voeg.special_gather_type)
	{
		UNSTD_STATIC_CHECK(sizeof(GuildName) == sizeof(GameName));
		gather_obj->GetBelongName(voeg.param_2);
		voeg.param_3 = gather_obj->GetData();

		bool is_can_stealfish = false;
		SpecialCrossFishing* special_fishing = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossFishing(COMMON_SCENE_KEY, false);
		if (NULL != special_fishing)
		{
			is_can_stealfish = special_fishing->IsCanStealFish(this, voeg.param_3);
		}

		voeg.param_0 = (is_can_stealfish ? 1 : 0);
	}
	else if(SPEICAL_GATHER_TYPE_CROSS_GUILD_BATTLE_BOSS == voeg.special_gather_type)
	{
		gather_obj->GetBelongGuildName(voeg.param_2);
	}
	else if (SPECIAL_GATHER_TYPE_HUSONG_SHUIJING == voeg.special_gather_type)
	{
		const HusongSJOtherCfg other_cfg = HusongShuijingConfig::Instance().GetOtherCfg();
		SpecialCrossHusongShuijing * special_logic = World::GetInstWorld()->GetSceneManager()->GetSpcialCrossHusongShuijing(other_cfg.scene_id, COMMON_SCENE_KEY, false);
		if (NULL != special_logic)
		{
			voeg.param_0 = special_logic->GetRemainGatherTimes(voeg.obj_id);
		}
	}

	EngineAdapter::Instance().NetSend(m_netid, (const char *)&voeg, sizeof(Protocol::SCVisibleObjEnterGather));
}

void Role::OnAOISceneAreaEnter(Obj *obj)
{

}

void Role::OnAOIEffectEnter(Obj *obj)
{
	EffectObj *effect_obj = (EffectObj*)obj;

	Protocol::SCVisibleObjEnterEffect voee;
	voee.obj_id = effect_obj->GetId();
	voee.product_id = effect_obj->GetProductId();
	voee.product_method = effect_obj->GetProductMethod();
	voee.pos_x = (short)effect_obj->GetPos().x;
	voee.pos_y = (short)effect_obj->GetPos().y;
	voee.birth_time = (unsigned int)effect_obj->GetBirthTime();
	voee.disappear_time = (unsigned int)effect_obj->GetDisapperTime();
	voee.param1 = effect_obj->GetParam1();
	voee.param2 = effect_obj->GetParam2();
	voee.src_pos_x = effect_obj->GetSrcPos().x;
	voee.src_pos_y = effect_obj->GetSrcPos().y;

	EngineAdapter::Instance().NetSend(m_netid, (const char *)&voee, sizeof(voee));
}

void Role::OnAOITriggerEnter(Obj *obj)
{
	Trigger *trigger = (Trigger *)obj;

	if (!trigger->IsHiden())
	{
		Protocol::SCVisibleObjEnterTrigger voet;
		voet.obj_id = trigger->GetId();
		voet.action_type = trigger->GetActionType();
		voet.param0 = trigger->GetActionParam(0);
		voet.param1 = trigger->GetActionParam(1);
		voet.pos_x = trigger->GetPos().x;
		voet.pos_y = trigger->GetPos().y;
		voet.affiliation = trigger->GetAffiliation();
		trigger->GetName(voet.trigger_name);

		EngineAdapter::Instance().NetSend(m_netid, (const char *)&voet, sizeof(voet));
	}

	if (NULL != m_scene)
	{
		m_scene->GetSpecialLogic()->OnAOITriggerEnter(this);
	}
}

void Role::OnAOIBigCoinEnter(Obj *obj)
{
	BigCoinObj *bigcoin_obj = (BigCoinObj*)obj;
	
	Protocol::SCVisibleObjEnterBigCoin voebc;
	voebc.obj_id = bigcoin_obj->GetId();
	voebc.type = bigcoin_obj->GetType();
	voebc.value = bigcoin_obj->GetValue();
	voebc.pos_x = bigcoin_obj->GetPos().x;
	voebc.pos_y = bigcoin_obj->GetPos().y;

	EngineAdapter::Instance().NetSend(m_netid, (const char *)&voebc, sizeof(voebc));
}

void Role::OnAOIBattleFieldShenShiEnter(Obj *obj)
{
	BattleFieldShenShi *battlefield_shenshi = (BattleFieldShenShi*)obj;

	Protocol::SCVisibleObjEnterBattleFieldShenShi voebfss;

	voebfss.has_owner = (battlefield_shenshi->HaveOwner()) ? 1 : 0;
	voebfss.hp = battlefield_shenshi->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	voebfss.max_hp = battlefield_shenshi->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	voebfss.pos_x = (short)battlefield_shenshi->GetPos().x;
	voebfss.pos_y = (short)battlefield_shenshi->GetPos().y;
	voebfss.obj_id = battlefield_shenshi->GetId();
	voebfss.owner_obj_id = battlefield_shenshi->GetOwnerObjId();
	voebfss.owner_uid = UidToInt(battlefield_shenshi->GetOwnerUserId());

	EngineAdapter::Instance().NetSend(m_netid, (const char *)&voebfss, sizeof(voebfss));
}

void Role::OnAOIMonsterHaloEnter(Obj *obj)
{
	MonsterHalo *halo = (MonsterHalo *)obj;

	Protocol::SCVisibleObjEnterMonsterHalo voemh;
	voemh.obj_id = halo->GetId();
	voemh.pos_x = (short)halo->GetPos().x;
	voemh.pos_y = (short)halo->GetPos().y;
	voemh.owner_obj_id = halo->GetOwnerObjID();
	voemh.effect_type = halo->GetEffectType();

	EngineAdapter::Instance().NetSend(m_netid, (const char *)&voemh, sizeof(voemh));
}

void Role::OnAOIMarryObjEnter(Obj *obj)
{
	MarryObj *marry_obj = (MarryObj*)obj;

	Protocol::SCVisibleObjEnterMarryObj voemo;
	voemo.obj_id = marry_obj->GetId();
	voemo.marry_seq = marry_obj->GetMarrySeq();
	voemo.pos_x = marry_obj->GetPos().x;
	voemo.pos_y = marry_obj->GetPos().y;
	voemo.dir = marry_obj->GetDir();
	voemo.distance = marry_obj->GetDirDistance();
	voemo.speed = (int)marry_obj->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED);

	EngineAdapter::Instance().NetSend(m_netid, (const char *)&voemo, sizeof(voemo));
}


void Role::OnAOIRoleShadowEnter(Obj *obj)
{
	RoleShadow *rs = (RoleShadow *)obj;

	Protocol::SCVisibleObjEnterRoleShadow voers;
	voers.obj_id = rs->GetId();
	voers.reserved = 0;
	voers.role_id = UidToInt(rs->GetUserId());
	rs->GetName(voers.role_name);
	voers.level = rs->GetLevel();
	voers.avatar = rs->GetAvatar();
	voers.prof = rs->GetProf();
	voers.sex = rs->GetSex();
	voers.camp = rs->GetCamp();
	voers.hp = rs->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	voers.max_hp = rs->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	voers.speed = rs->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED);
	voers.pos_x = static_cast<short>(rs->GetPos().x);
	voers.pos_y = static_cast<short>(rs->GetPos().y);
	voers.dir = rs->GetDir();
	voers.distance = rs->GetDirDistance();
	voers.plat_type = rs->GetPlatType();

	voers.appearance = rs->GetAppearance();

	voers.vip_level = rs->GetVipLevel();
	voers.guild_post = rs->GetGuildPost();

	voers.guild_id = rs->GetGuildID();
	rs->GetGuildName(voers.guild_name);
	voers.avatar_timestamp = rs->GetAvatarTimestamp();

	voers.shadow_type = rs->GetRoleShadowType();
	voers.shadow_param = m_scene->GetSpecialLogic()->GetRoleShadowSpecialParam(rs);
	if (ROLE_SHADOW_TYPE_CAMPDEFEND == voers.shadow_type)
	{
		voers.shadow_param = CampDefendMgr::Instance().GetDiaoXiangStatus(rs);
	}

	EngineAdapter::Instance().NetSend(m_netid, (const char *)&voers, sizeof(voers));
}

void Role::OnAOIWorldEventObjEnter(Obj *obj)
{
	WorldEventObj *world_event_obj = (WorldEventObj*)obj;

	Protocol::SCVisibleObjEnterWorldEventObj voeweo;
	voeweo.obj_id = world_event_obj->GetId();
	voeweo.world_event_id = world_event_obj->GetWorldEventID();
	voeweo.pos_x = (short)world_event_obj->GetPos().x;
	voeweo.pos_y = (short)world_event_obj->GetPos().y;
	voeweo.hp = world_event_obj->GetHp();
	voeweo.max_hp = world_event_obj->GetMaxHp();
	EngineAdapter::Instance().NetSend(m_netid, (const char *)&voeweo, sizeof(voeweo));
}

typedef void (Role::*ObjEnterFun)(Obj *obj);
const ObjEnterFun g_obj_enter_fun[] = {
	0,
	&Role::OnAOIRoleEnter,
	&Role::OnAOIMonsterEnter,
	&Role::OnAOIFallingEnter,
	&Role::OnAOIGatherEnter,
	&Role::OnAOISceneAreaEnter,
	&Role::OnAOIEffectEnter,
	&Role::OnAOITriggerEnter,
	&Role::OnAOIBigCoinEnter,
	&Role::OnAOIBattleFieldShenShiEnter,
	&Role::OnAOIMonsterHaloEnter, 
	&Role::OnAOIMarryObjEnter,
	&Role::OnAOIRoleShadowEnter,
	&Role::OnAOIWorldEventObjEnter,
};

UNSTD_STATIC_CHECK(sizeof(g_obj_enter_fun) / sizeof(g_obj_enter_fun[0]) == Obj::OBJ_TYPE_COUNT);

void Role::OnAOIEnter(ObjID obj_id)
{
	if (NULL == m_scene || obj_id == m_obj_id) return;

	Obj *obj = (Obj *)m_scene->GetObj(obj_id);
	if (NULL == obj) return;

	ObjEnterFun enter_fun = g_obj_enter_fun[int(obj->GetObjType())];
	if (NULL == enter_fun) return;

	(this->*enter_fun)(obj);

	++gamecommon::g_game_stat.enter_count;
}

void Role::OnAOILeave(ObjID obj_id)
{
	if (obj_id == m_obj_id) return;

	++gamecommon::g_game_stat.leave_count;

	Obj *obj = m_scene->GetObj(obj_id);
	if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
	{
		Role *role = (Role *)obj;
		if (role->IsZombie() && role->GetLevel() <= 1) return;	// 屏蔽僵尸号
	}

	Protocol::SCVisibleObjLeave voj;
	voj.obj_id = obj_id;
	voj.reserved = 0;
	EngineAdapter::Instance().NetSend(m_netid, (const char *)&voj, sizeof(Protocol::SCVisibleObjLeave));

	// printf("obj leave: %d\n", obj_id);
}

bool Role::CanCharacterAction(bool is_notify_client)
{
	return Character::CanCharacterAction(is_notify_client);
}

bool Role::Move(Axis _x, Axis _y, Dir _dir, Scalar distance, short height, bool check_valid, short press_onward)
{
	if (NULL == m_scene) return false;

	//printf("..... x %d y %d dir %f dis %f\n", _x, _y, _dir, distance);
	
	if (Character::Move(_x, _y, _dir, distance, height, check_valid, press_onward))
	{
		m_scene->GetZoneMatrix()->MoveAOI(m_aoi_handle);
		ResetRoleStatus(INVALID_OBJ_ID, 0, RESET_ROLE_STATUS_REASON_MOVE);
		m_role_module->team_info.SendMyPosToMember();

		return true;
	}

	return false;
}
 
bool Role::ResetPos(Axis x, Axis y, short reset_type)
{
	if (Move(x, y, Dir_S, 0, 0, false))
	{
		m_last_check_valid_time = 0;

		Protocol::SCResetPost rp;
		rp.x = m_posi.x;
		rp.y = m_posi.y;
		rp.reset_type = reset_type;
		rp.reserved = 0;
		return EngineAdapter::Instance().NetSend(m_netid, (const char *)&rp, sizeof(Protocol::SCResetPost));
	}

	return false;
}

bool Role::SetDir(Dir _dir, Scalar distance)
{
	if (Character::SetDir(_dir, distance))
	{
		ResetRoleStatus();

		return true;
	}

	return false;
}

void Role::Update(unsigned long interval, time_t now_second, unsigned int now_dayid)
{
	if (NULL == m_scene) return;
	
	Character::Update(interval, now_second, now_dayid);

	this->UpdateEnergy(now_second);
	this->CheckFakePrivateChat(now_second);
	this->CheckWeekCampBuff(now_second);
	this->IntervalRecoverHp(interval, now_second);

	//复活疲劳
	this->UpdateReliveTire(now_second);

	if (this->IsAlive())
	{
		m_scene->GetZoneMatrix()->MoveAOI(m_aoi_handle);

		if (m_role_status != ROLE_STATUS_CHARACTER)
		{
			switch (m_role_status)
			{
			case ROLE_STATUS_GATHER:
				UpdateGather(interval);
				break;

			case ROLE_STATUS_REST:
				// UpdateRest(now_second);
				break;

			default:
				break;
			}
		}
	}
	else if (LocalConfig::Instance().IsShenheServer())
	{
		this->ReAlive(100);
	}

	if (MOVE_MODE_JUMP == m_move_mode && 0 != m_jump_last_time && now_second >= m_jump_last_time + 30)
	{
		if (m_level >= GLOBALCONFIG->GetResetJumpPosMinLevel())
		{
			this->SetPos(m_jump_src_pos);
		}
		this->SetMoveMode(MOVE_MODE_NORMAL);
	}

	if (MOVE_MODE_JUMP2 == m_move_mode && 0 != m_jump_last_time && now_second >= m_jump_last_time + 30)
	{
		this->SetMoveMode(MOVE_MODE_NORMAL);
	}

	if (now_second >= m_update_last_time + 15)
	{
		m_role_module->newtask_manager.Update((unsigned int)now_second);
		m_role_module->equip_manager.Update((unsigned int)now_second);
		m_role_module->knapsack.Update((unsigned int)now_second);

		m_update_last_time = now_second;
	}

	this->UpdateNameColor(now_second);
	this->UpdateSpecialAppearance(now_second);
	this->UpdateNewhandGuide((unsigned int)now_second);

	m_role_module->knapsack.GetMoney()->GetAccountGoldFromDB((unsigned int)now_second);
	m_role_module->rank_shadow.Update(interval, now_second);
	m_role_module->mentality.Update((unsigned int)now_second);
	m_role_module->active_degree.Update((unsigned int)now_second);
	m_role_module->friend_manager.Update((unsigned int)now_second);
	m_role_module->monitor.Update(now_second);
	
	m_role_module->mount_manager.Update((unsigned int)now_second);
	m_role_module->wing.Update((unsigned int)now_second);
	m_role_module->shengong.Update((unsigned int)now_second);
	m_role_module->shenyi.Update((unsigned int)now_second);
	m_role_module->mojie_manager.Update((unsigned int)now_second);
	m_role_module->fightmount_manager.Update((unsigned int)now_second);
	m_role_module->halo.Update((unsigned int)now_second);
	m_role_module->footprint.Update((unsigned int)now_second);
	m_role_module->cloak_manager.Update((unsigned int)now_second);
	m_role_module->title_manager.Update((unsigned int)now_second);
	m_role_module->shenbing_manager.Update((unsigned int)now_second);

	m_role_module->role_status_manager.Update((unsigned int)now_second);
	m_role_module->daily_task.Update((unsigned int)now_second);
	m_role_module->husong_task.Update((unsigned int)now_second);
	m_role_module->role_activity.Update((unsigned int)now_second);
	m_role_module->special_appearance.Update(interval, (unsigned int)now_second);
	m_role_module->role_supply.Update((unsigned int)now_second);
	m_role_module->jilian.Update((unsigned int)now_second);
	m_skill_manager.Update(now_second);
	m_role_module->paohuan_task.Update(now_second);
	m_role_module->qingyuan.Update((unsigned int)now_second);
	m_role_module->multi_mount.Update((unsigned int) now_second);
	m_role_module->welfare.Update(interval, (unsigned int) now_second);
	m_role_module->vip.Update((unsigned int)now_second);
	m_role_module->shenge_system.Update(interval, (unsigned int)now_second);
	m_role_module->wabao.Update((unsigned int)now_second);
	m_role_module->zero_gift.Update((unsigned int)now_second);

	m_role_module->friend_exp_bottle.Update((unsigned int)now_second);
	m_role_module->magical_precious_manager.Update((unsigned int)now_second);
	m_role_module->mysterious_shop_in_mall.Update((unsigned int)now_second);
	m_role_module->runesystem_manger.Update((unsigned int)now_second);
	m_role_module->shenzhou_weapon_manger.Update(interval, now_second);
	m_role_module->xiannv_manager.Update((unsigned int)now_second);
	m_role_module->shizhuang.Update((unsigned int)now_second);
	m_role_module->element_heart.Update((unsigned int)now_second);
	m_role_module->shenyin.Update((unsigned int)now_second);
	m_role_module->fabao.Update((unsigned int)now_second);
	m_role_module->talent.Update(interval, (unsigned int)now_second);
	m_role_module->m_imp_guard.Update(interval, (unsigned int)now_second);
	m_role_module->yaoshi_manager.Update((unsigned int)now_second);
	m_role_module->toushi_manager.Update((unsigned int)now_second);
	m_role_module->qilinbi_manager.Update((unsigned int)now_second);
	m_role_module->mask_manager.Update((unsigned int)now_second);
	m_role_module->zhuanzhi_equip.Update((unsigned int)now_second);
	m_role_module->role_zhuanzhi.Update(interval, (unsigned int)now_second);
	m_role_module->upgrade_manager.Update(now_second);
	m_role_module->mojie_skill.Update((unsigned int)now_second, (unsigned int)interval);
	m_role_module->xianzunka.Update((unsigned int)now_second);
	m_role_module->m_greate_soldier.Update((unsigned int)now_second, (unsigned int)interval);
	m_role_module->cross_rand_activity.Update((unsigned int)now_second);
	m_role_module->huguozhili.Update((unsigned int)now_second);
	m_role_module->cross_goal.Update((unsigned int)now_second);
	
	m_role_other_mgr->Update(now_second, interval);
	//for (IRoleModule *module : m_role_module_list)
	//{
	//	module->Update(now_second);
	//}

	JingHuaHuSongMgr::Instance().CheckJingHuaHusongTime(this, now_second);
	JinglingAdvantage::Instance().Update((unsigned int)now_second);

	if (m_last_online_dayid != now_dayid) // 所有的OnDayChange函数只能在这里调用
	{
		this->OnDayChange(m_last_online_dayid, now_dayid);

		const tm *now_tm = EngineAdapter::Instance().LocalTime();
		if (NULL != now_tm && 1 == now_tm->tm_wday)
		{
			this->OnWeekChange();
		}

		if (NULL != now_tm && 1 == now_tm->tm_mday)					
		{
			this->OnMonthChange();
		}

		m_last_online_dayid = now_dayid;
	}

	{
		// 重置角色数据相关逻辑
		if (m_next_reset_module_data_timestamp > 0 && now_second >= m_next_reset_module_data_timestamp)
		{
			this->OnResetModuleData(0, now_dayid);

			m_last_reset_module_data_dayid = now_dayid;
			m_last_reset_module_data_timestamp = static_cast<unsigned int>(now_second);

			this->RecalNextResetDataTimeStamp();
		}
	}
	
	if (m_next_save_time < now_second)
	{
		static const unsigned int hearbeat_dead_time = ServerConfigPool::Instance().common_config.timeout.gameworld_heartbeat_dead_time_ms;
		unsigned long now = EngineAdapter::Instance().GetGameTime();
		if (now - m_last_msg_from_client_time > hearbeat_dead_time)
		{
			m_next_save_time += 20;	// 暂时先这么着了 等实际发现问题了 再改好
			gamelog::g_log_debug.printf(LL_WARNING, "[Role::Update Warring][User(%d, %d) Save NOT Active more then hearbeat_dead_time:%d]", 
				m_user_id.db_index, m_user_id.role_id, (int)hearbeat_dead_time);
		}
		else
		{
			this->Save();
		}
	}

	{
		if (0 != m_mount_bianshen_time && now_second >= m_mount_bianshen_time)
		{
			m_is_mount_bianshen = false;
			m_mount_bianshen_time = 0;

			this->SendMountBianshenInfo(true);
		}
	}

	{
		// 等待场景帧数检测,角色卡在跨服中转场景一定帧时间则请求回到原服
		if (this->IsInCross() && CrossCommonConfig::Instance().IsCrossWaitScene(m_scene->GetSceneID()))
		{
			if (++m_wait_scene_frame_num > 3)
			{
				m_wait_scene_frame_num = 0;
				this->BackToOriginalServer();

				gamelog::g_log_debug.printf(LL_WARNING, "Force Role(%d, %s, %d) scene[%d] back to original server.",
					GetUID(), GetName(), UidToInt(GetUniqueUserID().user_id), m_scene->GetSceneID());
			}
		}
		else
		{
			m_wait_scene_frame_num = 0;
		}
	}

	if (m_need_send_attrinfo)
	{
		m_need_send_attrinfo = false;
		//参数废弃不用
		this->SendAttributeInfo(0);
	}

	//每帧初始化次变量. 用做帧统计次数
	m_tick_attack_count = 0;
	m_tick_attack_recal_count = 0;
// 	if (0 == m_common_data_param.has_send_welcome_mail)
// 	{
// 		m_common_data_param.has_send_welcome_mail = 1;
// 
// 		static MailContentParam reward_mail;  reward_mail.Reset();
// 		int l1 = SNPRINTF(reward_mail.contenttxt, sizeof(reward_mail.contenttxt), gamestring::g_welcome_mail_content);
// 		if (l1 > 0)
// 		{
// 			MailRoute::MailToUser(m_user_id, SYSTEM_MAIL_REASON_INVALID, reward_mail);
// 		}
// 	}
}

void Role::IntervalRecoverHp(unsigned long interval, time_t now_second)
{
	if (NULL != m_scene && !m_scene->GetSpecialLogic()->SpecialCanRecoverHp(this))
	{
		return;
	}

	if (!this->IsAlive()) return;

	CommonDataParam	*common_data = this->GetCommonDataParam();
	if (NULL == common_data) return;
	if (common_data->next_recover_hp_time > static_cast<unsigned int>(now_second)) return;

	Attribute max_hp = this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	Attribute cur_hp = this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	if (cur_hp >= max_hp) return;

	// 星座秘籍回血加成
	double other_add_per = this->GetChineseZodiac()->GetHuiXuePercent() * ROLE_ATTR_PER;

	double recover_hp_fight_per = GLOBALCONFIG->GetRecoverHpFightPer() + GLOBALCONFIG->GetRecoverHpFightPer() * other_add_per;
	double recover_hp_per = GLOBALCONFIG->GetRecoverHpPer() + GLOBALCONFIG->GetRecoverHpPer() * other_add_per;

	if (LocalConfig::Instance().IsShenheServer())
	{
		recover_hp_fight_per = 10000;
		recover_hp_per = 10000;
	}

	Attribute recover_hp = 0;
	if (this->GetRoleStatusManager()->IsInFightStatus())
	{
		recover_hp = static_cast<Attribute>(max_hp * (recover_hp_fight_per * ROLE_ATTR_PER));
	}
	else
	{
		recover_hp = static_cast<Attribute>(max_hp * (recover_hp_per * ROLE_ATTR_PER));
	}

	common_data->next_recover_hp_time = static_cast<unsigned int>(now_second) + GLOBALCONFIG->GetRecoverHpInterval();

	if (recover_hp > max_hp - cur_hp)
	{
		recover_hp = max_hp - cur_hp;
	}
	
	if(recover_hp == 0) return;

	this->ChangeHp(&recover_hp, true);
}

void RMIRoleSaveBackObjectImplRole::RoleSaveRet(int ret)
{
	if (0 != ret)
	{
		Error(log_user_id, ERROR_EXECUTE, ret);
	}
}

bool Role::Save(bool is_logout)
{
	static RoleInitParam role_param;
	static RoleOtherInitParam other_p;
	static ItemListParam itemlist_param;
	static HotkeyListParam hotkey_param;
	static FriendListParam friend_param;
	static BlacklistsParam black_param;
	static EnemyListParam enemy_param;
	static BusinessListParam busi;
	static SkillListParam skill_param;
	static DailyFindListParam daily_find_param;

	GetRoleInitParam(&role_param);
	GetRoleOtherInitParam(&other_p);

	int size = ItemNamespace::MAX_GRID_NUM;
	m_role_module->knapsack.GetChangeItemList(&size, &itemlist_param.item_list[0]);
	itemlist_param.count = size;

	size = ItemNamespace::MAX_GRID_NUM - size;
	this->GetEquipmentManager()->GetChangeEquipmentList(&size, &itemlist_param.item_list[itemlist_param.count]);
	itemlist_param.count += size;

	m_role_module->hotkey_recorder.GetChangeHotKeyList(&hotkey_param);
	m_role_module->friend_manager.GetChangeFriendList(&friend_param);
	m_role_module->friend_manager.GetChangeBlackList(&black_param);
	m_role_module->friend_manager.GetChangeEnemyList(&enemy_param);
	m_skill_manager.GetChangeSkillList(&skill_param);
	m_role_module->welfare.GetChangeDailyFindList(&daily_find_param);
	
	unsigned int ROLE_SAVE_TIME = GLOBALCONFIG->GetKeyConfig_RoleSaveTime();
	m_next_save_time = EngineAdapter::Instance().Time() + ROLE_SAVE_TIME;

	RMIRoleSaveBackObjectImplRole *rsbo = new RMIRoleSaveBackObjectImplRole();
	rsbo->log_user_id = m_user_id;

	RMIRoleClient rc;
	rc.__bind_session(InternalComm::Instance().GetDBHandler().GetRoleDB());
	bool ret = rc.RoleSaveAsyn(UidToInt(m_user_id), role_param, other_p, itemlist_param, hotkey_param,
		friend_param, black_param, enemy_param, busi, skill_param, daily_find_param, rsbo, is_logout, m_role_module->role_cross.GetAllowToCrossFlag());
	if (ret)
	{
		m_role_module->knapsack.ClearDirtyMark();
		m_role_module->equip_manager.ClearDirtyMark();
		m_role_module->hotkey_recorder.ClearDirtyMark();
		m_role_module->friend_manager.ClearDirtyMark();
		m_skill_manager.ClearDirtyMark();
		m_role_module->welfare.ClearDirtyMark();
	}

	if (CrossConfig::Instance().IsHiddenServer())
	{
		static RoleAllParam all_param;
		all_param.Reset();

		all_param.role_param = role_param;
		all_param.other_param = other_p;
		all_param.item_param = itemlist_param;
		all_param.hotkey_param = hotkey_param;
		all_param.friend_param = friend_param;
		all_param.black_param = black_param;
		all_param.enemy_param = enemy_param;
		all_param.business_param = busi;
		all_param.skill_param = skill_param;
		all_param.daily_find_param = daily_find_param;

		int fix_pos = -1;
		for (int i = 0; i < static_cast<int>(sizeof(GameName)); i++)
		{
			if (0 == all_param.role_param.role_name[i])
			{
				break;
			}
			else if ('_' == all_param.role_param.role_name[i])
			{
				fix_pos = i;
			}
		}

		if (fix_pos >= 0 && fix_pos < static_cast<int>(sizeof(GameName)))
		{
			all_param.role_param.role_name[fix_pos] = 0;
		}

		static crossgameprotocal::CrossGameSyncRoleData cgsrd;
		cgsrd.is_logout = is_logout ? 1 : 0;
		cgsrd.uuid = UniqueUserIDToLongLong(m_role_module->role_cross.GetCrossOriginalUniqueUserID());
		
		TLVSerializer outstream;
		outstream.Reset(cgsrd.role_all_data, sizeof(cgsrd.role_all_data));
		if (all_param.Serialize(outstream, true))
		{
			cgsrd.data_len = outstream.Size();

			int send_len = sizeof(cgsrd) - sizeof(cgsrd.role_all_data) + cgsrd.data_len;
			InternalComm::Instance().SendToCross((const char*)&cgsrd, send_len);
		}
	}

	return ret;
}

void Role::OnRoleLogout()
{
	{
		m_role_module->common_data_param.last_logout_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());

		int online_time_s = static_cast<unsigned int>(EngineAdapter::Instance().Time() - m_login_timestamp);

		ROLE_LOG_QUICK10(LOG_TYPE_LOGOUT, this, online_time_s, this->GetCapability()->GetTotalCapability(), NULL, NULL, m_role_module->knapsack.GetMoney()->GetGold(), m_role_module->knapsack.GetMoney()->GetGoldBind(), this->GetLevel(), this->GetRoleActivity()->GetHistoryChongZhi());
		gamelog::g_log_login.printf(LL_DEBUG, "[Role::OnRoleLogout][%d][%s]", UidToInt(m_user_id), m_name);
		gamelog::g_log_global_user_state.printf(LL_INFO, "[LoginServer::OnGlobalUserLogout ret:%d] user[%s, %d] ip[%u] is_micro_pc[%d].", 0, m_name, this->GetUID(), this->GetIP(), 1);

#ifdef _DEBUG
		printf("Role Logout: %d \n", m_uid);
#endif

	}

	FixMoveMode();
	UserCacheManager::Instance().UpdateUserNameCache(this, false); // 这一行要放最前面，组队和好友的协议依赖玩家的在线状态
	TeamManager::Instance().OnUserLogout(this);
	GuildManager::Instance().OnUserLogout(this);
	ActivityManager::Instance().OnUserLogout(this);
	SpecialActivityManager::Instance().OnUserLogout(this);
	ChallengeField::Instance().OnUserLogout(this);
	CrossChallengeField::Instance().OnUserLogout(this);
	ChatManager::Instance().OnUserLogout(this);
	RandActivityManager::Instance().OnUserLogout(this);
	FishPoolManager::Instance().OnUserLogout(this);
	QueryHandler::Instance().OnUserLogout(this);
	SystemMonitor::Instance().OnRoleLogout(this);
	CrossUserRegister::Instance().OnUserLogout(this);
	m_role_module->role_cross.OnLogout();
	m_role_module->multi_mount.OnLogout();
	m_role_module->friend_exp_bottle.OnRoleLogout();
	GlobalRoleManager::Instance().OnRoleLogout(this);
	m_role_module->m_imp_guard.OnRoleLogout();
	m_role_module->welfare.OnRoleLogout();
	m_role_module->m_greate_soldier.OnRoleLogout();
	CrossRAManagerLocal::Instance().OnUserLogout(this);
	BossFamilyManager::Instance().OnRoleLogout(this);

	m_role_other_mgr->OnRoleLogout();
	//for (IRoleModule *module : m_role_module_list)
	//{
	//	module->OnRoleLogout();
	//}
	
	int scene_type = m_scene->GetSceneType();
	if (Scene::SCENE_TYPE_TAOSHOUJITAN_TEAM_FB == scene_type || Scene::SCENE_TYPE_LOVERS_FB == scene_type ||
		Scene::SCENE_TYPE_MIGONGXIANFU_TEAM == scene_type || Scene::SCENE_TYPE_TEAM_ZHUAGUI == scene_type || 
		Scene::SCENE_TYPE_TEAM_EQUIP_FB == scene_type || Scene::SCENE_TYPE_EXPFB == scene_type ||
		Scene::SCENE_TYPE_TEAM_EQUIPFB == scene_type || Scene::SCENE_TYPE_TEAM_TOWERDEFEND == scene_type)
	{
		TeamKeyRegister::Instance().RegisterTeamKey(m_user_id, m_scene->GetSceneID(), m_scene->GetSceneKey(), m_scene->GetCreateTime());
	}

	ActivityMsgManager::Instance().OnRoleLogout(this);
}

void Role::SendInfoMsg()
{
	Protocol::SCRoleInfoAck ria;

	ria.sex = m_sex;
	ria.prof = m_prof;
	ria.camp = m_camp;
	ria.authority_type = m_authority_type;
	ria.level = m_level;
	ria.uuserverid = GetCommonDataParam()->uu_last_login_serverid;

	ria.hp = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	ria.base_max_hp = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP);
	ria.mp = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP);
	ria.base_max_mp = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXMP);

	ria.base_gongji = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI);
	ria.base_fangyu = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU);
	ria.base_mingzhong = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG);
	ria.base_shanbi = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI);
	ria.base_baoji = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI);
	ria.base_jianren = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN);
	ria.base_move_speed = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED);
	ria.base_fujia_shanghai =  m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FUJIA_SHANGHAI);
	ria.base_dikang_shanghai =  m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_DIKANG_SHANGHAI);
	ria.base_per_jingzhun =  m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_JINGZHUN);
	ria.base_per_baoji =  m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI);
	ria.base_per_kangbao =  m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO);
	ria.base_per_pofang =  m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_POFANG);
	ria.base_per_mianshang =  m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_MIANSHANG);
	ria.base_constant_zengshang =  m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG);
	ria.base_constant_mianshang =  m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_CONSTANT_MIANSHANG);
	ria.base_huixinyiji = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI);
	ria.base_huixinyiji_hurt_per = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER);
	ria.base_per_baoji_hurt = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_BAOJI_HURT);
	ria.base_per_kangbao_hurt = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_PER_KANGBAO_HURT);
	ria.base_zhufuyiji_per = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_ZHUFUYIJI_PER);
	ria.base_gedang_per = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_PER);
	ria.base_gedang_dikang_per = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_DIKANG_PER);
	ria.base_gedang_jianshang = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GEDANG_JIANSHANG_PER);
	ria.base_skill_zengshang = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER);
	ria.base_skill_jianshang = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER);
	ria.base_mingzhong_per = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG_PER);
	ria.base_shanbi_per = m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI_PER);

	ria.fight_max_hp = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	ria.fight_max_mp = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP);
	ria.fight_gongji = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI);
	ria.fight_fangyu = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU);
	ria.fight_mingzhong = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG);
	ria.fight_shanbi = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI);
	ria.fight_baoji = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI);
	ria.fight_jianren = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_JIANREN);
	ria.fight_move_speed = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED);
	ria.fight_fujia_shanghai = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI);
	ria.fight_dikang_shanghai = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_DIKANG_SHANGHAI);
	ria.fight_per_jingzhun = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_JINGZHUN);
	ria.fight_per_baoji = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI);
	ria.fight_per_kangbao = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_KANGBAO);
	ria.fight_per_pofang = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_POFANG);
	ria.fight_per_mianshang = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG);
	ria.fight_constant_zengshang = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_CONSTANT_ZENGSHANG);
	ria.fight_constant_mianshang = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_CONSTANT_MIANSHANG);
	ria.pvp_zengshang = m_int_attrs.GetAttr(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_ZENGSHANG_PER);
	ria.pvp_mianshang = m_int_attrs.GetAttr(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVP_JIANSHANG_PER);
	ria.pve_zengshang = m_int_attrs.GetAttr(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_ZENGSHANG_PER);
	ria.pve_mianshang = m_int_attrs.GetAttr(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_PVE_JIANSHANG_PER);
	ria.fight_huixinyiji = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HUIXINYIJI);
	ria.fight_huixinyiji_hurt_per = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HUIXINYIJI_HURT_PER);
	ria.fight_per_baoji_hurt = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI_HURT);
	ria.fight_per_kangbao_hurt = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_KANGBAO_HURT);
	ria.fight_zhufuyiji_per = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_ZHUFUYIJI_PER);
	ria.fight_gedang_per = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GEDANG_PER);
	ria.fight_gedang_dikang_per = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GEDANG_DIKANG_PER);
	ria.fight_gedang_jianshang = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GEDANG_JIANSHANG_PER);
	ria.fight_skill_zengshang = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SKILL_ZENGSHANG_PER);
	ria.fight_skill_jianshang = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SKILL_JIANSHANG_PER);
	ria.fight_mingzhong_per = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG_PER);
	ria.fight_shanbi_per = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI_PER);
	ria.skill_cap_per = m_int_attrs.GetAttr(CharIntAttrs::SPEICAL_CHARINTATTR_TYPE_SKILL_CAP_PER);

	ria.exp = m_exp;
	ria.max_exp = m_max_exp;

	ria.attack_mode = m_attack_mode;
	ria.name_color = m_name_color;
	ria.move_mode = m_move_mode;
	ria.move_mode_param = m_move_mode_param;
	ria.xiannv_huanhua_id = this->GetXiannvManager()->GetXiannvHuanhuaId();
	ria.jump_remain_times = m_role_module->common_data_param.jump_remain_times;
	ria.jump_last_recover_time = m_role_module->common_data_param.jump_last_recover_time;
	ria.create_role_time = static_cast<unsigned int>(m_create_time);
	ria.capability = this->GetCapability()->GetTotalCapability();

	ria.appearance = m_appearance;

	m_role_module->jingling_mgr.GetUsedJingLingInfo(&ria.used_jingling_id, &ria.used_jingling_level, ria.jingling_name, &ria.jingling_use_imageid, &ria.jingling_phantom_img);
	//ria.buff_mark = this->GetEffectList()->GetSpecialBuffMark();
	this->GetEffectList()->GetSpecialBuffMark(ria.buff_mark);
	ria.evil = m_evil;
	ria.xianhun = m_xianhun;
	ria.yuanli = m_yuanli;
	ria.nv_wa_shi = m_nv_wa_shi;
	ria.lingjing = m_lingjing;
	ria.chengjiu = m_chengjiu;
	ria.fenchen = m_hunli;
	ria.guanghui = this->GetRoleShop()->GetChestShopGuanghui();

	ria.guild_id = m_guild_id;
	F_STRNCPY(ria.guild_name, m_guild_name, sizeof(GuildName));
	ria.last_leave_guild_time = m_role_module->role_guild.GetLastLeaveGuildTime();
	ria.guild_post = m_guild_post;

	ria.is_team_leader = 0;
	if (m_role_module->team_info.InTeam())
	{
		Team *team = m_role_module->team_info.GetMyTeam();
		if (NULL != team && team->IsLeader(m_user_id))
		{
			ria.is_team_leader = 1;
		}
	}
	ria.mount_appeid = this->GetMountManager()->GetMountAppeID();
//	ria.mount_flyup_use_img = this->GetMountManager()->GetFlyupImgID();

	const MultiMountBindItem *multi_mount_item = this->GetMultiMount()->GetMountOnInfo();
	if (NULL != multi_mount_item)
	{
		ria.multi_mount_res_id = multi_mount_item->res_id;
		ria.multi_mount_is_owner = (multi_mount_item->owner_uid == m_uid ? 1 : 0);
		ria.multi_mount_partner_uid = (multi_mount_item->owner_uid == m_uid ? multi_mount_item->follower_uid : multi_mount_item->owner_uid);
	}
	else
	{
		ria.multi_mount_res_id = -1;
		ria.multi_mount_is_owner = 0;
		ria.multi_mount_partner_uid = 0;
	}
	
	this->GetHusongTask()->GetHusongInfo(&ria.husong_taskid, &ria.husong_color);

	ria.is_change_avatar = m_is_change_avatar;

	ria.nuqi = m_role_module->common_data_param.nuqi;
	ria.honour = m_honour;
	ria.energy = m_role_module->common_data_param.energy;

	ria.guild_gongxian = m_guild_gongxian;
	ria.guild_total_gongxian = m_guild_total_gongxian;
	
	m_role_module->chengjiu_mgr.GetChengJiuTitleLevel(&ria.chengjiu_title_level);

	m_role_module->shengwang_mgr.GetXianJieLevel(&ria.xianjie_level);
	
	ria.day_revival_times = m_role_module->common_data_param.m_day_revival_times;
	ria.cross_honor = m_role_module->common_data_param.cross_honor;
	ria.shengwang = m_role_module->common_data_param.shengwang;
	ria.avatar_timestamp = m_avator_timestamp;
	ria.lover_uid = m_role_module->common_data_param.lover_uid;
	this->GetLoverName(ria.lover_name); 
	ria.m_last_marry_time = m_last_marry_time;
	ria.role_uuid = UniqueUserIDToLongLong(GetRoleCross()->GetCrossOriginalUniqueUserID());
	ria.jinghua_husong_status = (char)m_role_module->common_data_param.jinghua_husong_status;

	ria.gongxun = m_role_module->common_data_param.day_gongxun;
	ria.fight_mount_appeid = this->GetFightMountManager()->GetFightMountAppeID();
	ria.wuqi_color = this->GetZhuanzhiEquip()->GetEquipNoticeColor(E_INDEX_ZHUANZHI::E_INDEX_WUQI);
	this->GetImpGuard()->GetImpUseAppeId(ria.imp_appe_id);
	ria.lover_qingyuan_capablity = 0;
	ria.lover_baby_capablity = 0;
	ria.lover_little_pet_capablity = 0;

	if (this->IsMarry())
	{
		UserCacheNode *lover_user_node = UserCacheManager::Instance().GetUserNode(this->GetLoverUid());
		if (nullptr != lover_user_node)
		{
			ria.lover_qingyuan_capablity = lover_user_node->qingyuan_cap;
			ria.lover_baby_capablity = lover_user_node->baby_cap;
			ria.lover_little_pet_capablity = lover_user_node->little_pet_cap;
		}
	}

	if (CrossConfig::Instance().IsHiddenServer())
	{
		ria.little_pet_using_id = this->GetCrossRoleGlobal()->GetUsingPetId();
	}
	else
	{
		LittlePet *m_little_pet = GLOBAL_ROLE_MGR.GetLittlePet(this->GetUID());
		if (NULL != m_little_pet)
		{
			ria.little_pet_using_id = m_little_pet->GetUsingPetId();
			m_little_pet->GetCurPetName(ria.little_pet_name);
		}
	}

	m_role_module->pet.GetPetInfo(&ria.pet_id, &ria.pet_level, &ria.pet_grade, ria.pet_name, &ria.user_pet_special_img);

	EngineAdapter::Instance().NetSend(m_netid, (const char *)&ria, sizeof(ria));
}

void Role::BackToOriginalServer()
{
	bool is_hidden_server = CrossConfig::Instance().IsHiddenServer();
	if (is_hidden_server)
	{
		Protocol::SCReturnOriginalServer ros;
		EngineAdapter::Instance().NetSend(m_netid, (const char *)&ros, sizeof(ros));
	}
}

void Role::SetMarryInfo(int lover_uid, GameName lover_name, time_t marry_time)
{
	lover_name[sizeof(GameName) - 1] = 0;

	m_role_module->common_data_param.lover_uid = lover_uid;
	F_STRNCPY(m_role_module->common_data_param.lover_name, lover_name, sizeof(m_role_module->common_data_param.lover_name));
	if (0 != marry_time) m_last_marry_time = static_cast<unsigned int>(marry_time);

	LittlePet * role_pet = GLOBAL_ROLE_MGR.GetLittlePet(this->GetUID());
	if (NULL != role_pet) role_pet->SetLoverId(lover_uid);

	LoveTree * role_tree = GLOBAL_ROLE_MGR.GetLoveTree(this->GetUID());
	if (NULL != role_tree) role_tree->SetLoverId(lover_uid);

	QingyuanGlobal * qingyuan_global = GLOBAL_ROLE_MGR.GetQingyuanGlobal(this->GetUID());
	if (NULL != qingyuan_global) qingyuan_global->SetLoverId(lover_uid);

	SpouseHome * role_spouse_home = GLOBAL_ROLE_MGR.GetSpouseHomeGlobal(this->GetUID());
	if (nullptr != role_spouse_home) role_spouse_home->SetLoverId(lover_uid);

	static Protocol::SCRoleMarryInfoChange rmic;
	rmic.lover_uid = m_role_module->common_data_param.lover_uid;
	rmic.obj_id = m_obj_id;
	rmic.reserved = 0;
	F_STRNCPY(rmic.lover_name, m_role_module->common_data_param.lover_name, sizeof(rmic.lover_name));
	rmic.last_marry_time = m_last_marry_time;

	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&rmic, sizeof(rmic));

	this->Save();

	m_role_module->shizhuang.CheckActiveByCondition(SHIZHUANG_CHECK_ACTIVE_TYPE_GCZ);
}

bool Role::IsMarry()
{
	return 0 != m_role_module->common_data_param.lover_uid;
}

int Role::GetLoverUid() {
	return m_role_module->common_data_param.lover_uid;
}

void Role::GetLoverName(GameName lover_name)
{
	F_STRNCPY(lover_name, m_role_module->common_data_param.lover_name, sizeof(GameName));
}

void Role::SetDivorce(unsigned int divorce_time)
{
	m_last_divorce_time = divorce_time;

	GameName name = {0};
	unsigned int last_marry_time = m_last_marry_time;

	// 修正离婚时间
	if (0 == last_marry_time)
	{
		last_marry_time = divorce_time;
	}

	this->SetMarryInfo(0, name, last_marry_time);
}

bool Role::IsFetchWorldBossWeekReward()
{
	return 0 != m_role_module->common_data_param.is_fetch_worldboss_week_reward;
}

void Role::SetFetchWorldBossWeekReward(char flag)
{
	m_role_module->common_data_param.is_fetch_worldboss_week_reward = flag;
}

void Role::GenerateShenheNewRoleParam(const RoleInitParam& role_init_param, const RoleOtherInitParam& other_p)
{
	
}

void Role::ShenheServerRandomRole()
{
	int rand_num1 = RandomNum(INT_MAX);
	//int rand_num2 = RandomNum(INT_MAX);

	int rand_level = rand_num1 % 30 + 50;
	this->GMSetLevel(rand_level);
/*
	const int shizhuang_wuqi_index = rand_num1 % 17 + 1; // 暂时只有(1 ~ 18) SHIZHUANG_MAX_INDEX
	m_role_module->shizhuang.ActiveSpecialImg(SHIZHUANG_TYPE_WUQI, rand_num1 % shizhuang_wuqi_index);
	const int shizhuang_body_index = rand_num1 % 17 + 1; // 暂时只有(1 ~ 18) SHIZHUANG_MAX_INDEX
	m_role_module->shizhuang.ActiveSpecialImg(SHIZHUANG_TYPE_BODY, rand_num2 % shizhuang_body_index);
	
	const int wing_id = rand_num1 % 11 + 1; // WingParam::MAX_IMAGE_ID + 1;
	m_role_module->wing.ActiveImage(wing_id);
	if (rand_num2 % 3 > 0)
	{
		const int special_img = rand_num2 % 29; // WingParam::MAX_SPECIAL_IMAGE_ID
		m_role_module->wing.ActiveSpecialImage(special_img);
	}

	if (rand_num2 % 2 > 0)
	{
		const int mount_special_img_id = rand_num1 % 28 + 1; // 本来最大ID是MountParam::MAX_SPECIAL_IMAGE_ID，不过配置只配到28个形象
		m_role_module->mount_manager.ActiveSpecialImage(mount_special_img_id);
		m_role_module->mount_manager.GoonMount(true);
	}
	else
	{
		const int mount_special_img_id = rand_num1 % 3 + 1; // 本来最大ID是FightMountParam::MAX_SPECIAL_IMAGE_ID，不过配置只配到3个形象
		m_role_module->fightmount_manager.ActiveSpecialImage(mount_special_img_id);
		m_role_module->fightmount_manager.GoonFightMount(true);
	}

	const int halo_img_id = rand_num1 % 11 + 1; // HaloParam::MAX_SPECIAL_IMAGE_ID
	m_role_module->halo.ActiveSpecialImage(halo_img_id);

	const int jingling_id = rand_num1 % 6; // 精灵最大id为JINGLING_MAX_IMAGE_ID
	m_role_module->jingling_mgr.ActiveImage(jingling_id);
	if (rand_num2 % 2 > 0) // 随机决定是否使用特殊形象
	{
		const int special_img = rand_num2 % JINGLING_PTHANTOM_MAX_TYPE; 
		m_role_module->jingling_mgr.UsePhantomImg(special_img);
	}
	
	const int xiannv_id = rand_num1 % MAX_XIANNV_COUNT; // 仙女
	m_role_module->xiannv_manager.ActiveXiannv(xiannv_id);
	if (rand_num2 % 2 > 0) // 随机决定是否使用特殊形象
	{
		const int huanhua_id = rand_num2 % 7; // MAX_XIANNV_HUANHUA_COUNT
		m_role_module->xiannv_manager.ActiveHuanHua(huanhua_id);
	}

	m_role_module->cloak_manager.ActiveCloak();
	const int cloak_img_id = rand_num1 % 2; // CloakParam::MAX_SPECIAL_IMAGE_ID;
	m_role_module->cloak_manager.ActiveSpecialImage(cloak_img_id);
*/

	// 跳任务，这代码直接从gm那边拷贝的
	{
		TaskID task_id = 450;

		int count = 0;
		int task_list[1000]; memset(task_list, 0, sizeof(task_list));
		const TaskCfg *task = TASKPOOL->GetCfgByTaskID(task_id);

		while (NULL != task)
		{
			if (count < 1000)
			{
				task_list[count++] = task->pretaskid;
			}
			else
			{
				break;
			}

			task = TASKPOOL->GetCfgByTaskID(task->pretaskid);
		}

		for (int i = count - 1; i >= 0; --i)
		{
			this->GetTaskManager()->GMAddTask(task_list[i]);
			for (int try_count = 0; try_count < 256; ++try_count)
			{
				int  task_id = 0;
				bool ret = this->GetTaskManager()->GetTaskIdByType(TASK_TYPE_BRANCH, &task_id);
				if (!ret && !this->GetTaskManager()->GetTaskIdByType(TASK_TYPE_TRUNK, &task_id))
				{
					break;
				}

				this->GetTaskManager()->ForceCompleteTask(task_id);
				this->GetTaskManager()->GMCommitTask(task_id);
			}
		}

		this->GetTaskManager()->GMAddTask(task_id);
	}
}

void Role::NextAttackExtHurtPercent(int hurt_percent)
{
	m_next_attack_extra_hurt_percent = hurt_percent;
}

void Role::NoticeNum(int notice_num)
{
	if (LocalConfig::Instance().IsShenheServer())
	{
		return;
	}
	Protocol::SCNoticeNum en;
	en.notice_num = notice_num;
	EngineAdapter::Instance().NetSend(m_netid, (const char *)&en, sizeof(Protocol::SCNoticeNum));
}

void Role::NoticeItemLack(int item_id, int nun)
{
	Protocol::SCLackItem li;
	li.item_id = item_id;
	li.item_count = nun;
	EngineAdapter::Instance().NetSend(m_netid, (const char*)&li, sizeof(li));
}

long long Role::GetAddonExp(long long exp, int exp_add_reason, int *add_percent)
{
	if (exp <= 0 || exp_add_reason < EXP_ADD_REASON_DEFAULT || exp_add_reason >= EXP_ADD_REASON_MAX || nullptr == add_percent) return exp;
	double total_per = this->GetExpTotalPer(exp_add_reason, add_percent);
	long long addon_exp = static_cast<long long>(exp * (total_per * 0.01));
	return addon_exp;
}

bool Role::AddExp(long long exp, const char *type, int reason, long long *real_add_exp, int add_percent)
{
	if (exp <= 0 || NULL == type || reason < EXP_ADD_REASON_DEFAULT || reason >= EXP_ADD_REASON_MAX) return false;

	long long add_exp = this->GetAddonExp(exp, reason, &add_percent);
	exp = add_exp;

	int round_count = 0;
	int to_level = m_level;

	while (exp > 0 && round_count ++ < 1000)
	{
		if (m_exp + exp >= m_max_exp)
		{
				long long old_exp = m_exp;

				exp -= (m_max_exp - m_exp);
				m_exp = m_max_exp;

				if (to_level >= MAX_ROLE_LEVEL_VERSION_LIMIT) break;

				to_level++;
				m_exp = 0;
				m_max_exp = GLOBALCONFIG->GetRoleMaxExp(to_level);

				const int ROLE_UPGRADE_LOG_LEVEL = GLOBALCONFIG->GetRoleUpgradeLogLevel();
				if (NULL != m_scene && to_level >= ROLE_UPGRADE_LOG_LEVEL)
				{
					gamelog::g_log_role_upgrade.printf(LL_INFO, "role[%d %s] upgrade [level:%d time:%u scene:[%d %s] pos:[%d %d] old_exp:%lld add_exp:%lld type:%s]", 
						UidToInt(m_user_id), m_name, to_level, static_cast<unsigned int>(EngineAdapter::Instance().Time()), 
						m_scene->GetSceneID(), m_scene->GetSceneName(), m_posi.x, m_posi.y, old_exp, add_exp, type);
				}
			}
		else 
		{
			m_exp += exp;
			exp = 0;
			break;
		}
	}

	if (to_level != m_level)
	{
		long long temp_exp = m_exp;
		this->OnUpgradeLevel(to_level);
		m_exp = temp_exp;
	}

	if (add_exp > exp) 
	{
		Protocol::SCChaExpChange cec;
		cec.reason = (short)reason;
		cec.exp = m_exp;
		cec.delta = add_exp;
		cec.add_percent = add_percent;
		EngineAdapter::Instance().NetSend(m_netid, (const char *)&cec, sizeof(Protocol::SCChaExpChange));
	}

	{
		if (NULL != m_scene) m_scene->GetSpecialLogic()->OnAddExp(this, add_exp, reason);
	}

	if (NULL != real_add_exp) *real_add_exp = add_exp;

	return true;
}

void Role::ReduceExp(long long exp, const char *type, long long *reduce_exp) // 减经验，但是不会掉级，最多把当前经验减到0
{
	if (exp <= 0 || NULL == type)
	{
		return;
	}

	long long old_exp = m_exp;

	m_exp -= exp;
	if (m_exp < 0)
	{
		m_exp = 0;
	}

	long long delta_exp = old_exp - m_exp;
	if (delta_exp > 0) 
	{
		Protocol::SCChaExpChange cec;
		cec.reason = 0; //暂无用
		cec.exp = m_exp;
		cec.delta = -delta_exp;
		cec.add_percent = 100;
		EngineAdapter::Instance().NetSend(m_netid, (const char *)&cec, sizeof(Protocol::SCChaExpChange));
	}

	gamelog::g_log_other.printf(LL_INFO, "Role::ReduceExp user[%d, %s] level[%d], old_exp[%lld], curr_exp[%lld]", 
		m_uid, m_name, m_level, old_exp, m_exp);

	if (NULL != reduce_exp) *reduce_exp = delta_exp;
}

double Role::GetExpExtraPer(int exp_addon_reason)
{
	double extra_exp_per = 0.0;

	//默认只是世界有加成
	//bool can_add_world_extra = true;        //世界加成
	bool can_add_vip_extra = false;         // vip额外加成
	bool can_add_buff_extra = false;        // buff加成
	bool can_add_campdefendwin = false;     // 师门雕像
	bool can_add_mojie = false;             // 特戒
	bool can_add_xianpin_extra = false;     // 仙品属性
	bool can_add_xunzhang_extra = false;    // 勋章
	bool can_add_glod_vip_extra = false;    // 黄金vip
	bool can_add_role_goal_extra = false;   // 人物目标
	bool can_add_rune_extra = false;        // 符文系统
	bool can_add_shenge_system = false;     // 星辉系统
	bool can_add_xianzunka = false;         // 仙尊卡
	bool can_imp_guard_extra = false;       // 小鬼守护
	bool can_add_zhuanzhi_extra = false;    // 转职
	bool can_add_mojieskill = false;        // 魔戒
	bool can_add_tianshuxunzhu = false;		// 天书寻主

	// 杀怪及经验副本 全都加
	if(Role::EXP_ADD_REASON_KILL_MONSTER == exp_addon_reason 
		|| Role::EXP_ADD_REASON_WATCH == exp_addon_reason 
		|| Role::EXP_ADD_EXP_FB == exp_addon_reason)
	{
		//can_add_world_extra = true;
		can_add_vip_extra = true;
		can_add_buff_extra = true;
		can_add_campdefendwin = true;
		can_add_mojie = true;
		can_add_xianpin_extra = true;
		can_add_xunzhang_extra = true;
		can_add_glod_vip_extra = true;
		can_add_role_goal_extra = true;
		can_add_rune_extra = true;
		can_add_shenge_system = true;
		can_add_xianzunka = true;
		can_imp_guard_extra = true;
		can_add_zhuanzhi_extra = true;
		can_add_mojieskill = true;
		can_add_tianshuxunzhu = true;
	}

	// vip额外经验加成
	if (can_add_vip_extra)
	{
		int vip_extra_percent = LOGIC_CONFIG->GetVipConfig().GetAuthParam(m_role_module->vip.GetVipLevel(), VAT_VIP_EXTRA_EXP);
		if (vip_extra_percent > 0)
		{
			extra_exp_per += vip_extra_percent;
		}
	}
	
	// buff额外经验加成
	if (can_add_buff_extra)
	{
		EffectOtherBuff *buff = this->GetEffectList()->FindOtherEffectBuff(EffectBase::PRODUCT_ID_INC_EXTRAL_EXP_PER);
		if (NULL != buff)
		{
			extra_exp_per += buff->GetIncExtraExpPer();
		}
	}

	// 师门雕象加成
	if (can_add_campdefendwin)
	{
		int exist_diaoxiang_num = WorldStatus::Instance().GetCampExistsDiaoXiangNum(this->GetCamp());
		int campdefndwin_addpercent = exist_diaoxiang_num * LOGIC_CONFIG->GetCampCfg().GetOtherCfg().dx_addextraexp_once;
		if (campdefndwin_addpercent > 0)
		{
			extra_exp_per += campdefndwin_addpercent;
		}
	}

	// 仙品属性经验额外加成
	if (can_add_xianpin_extra)
	{
		double equipment_xinpin_add = this->GetEquipmentManager()->EquipXianPinGetValue(Equipment::E_XIANPIN_ADD_KILL_MONSTER_EXP_REWARD_PER) * 0.01;
		if (equipment_xinpin_add > 0)
		{
			extra_exp_per += equipment_xinpin_add;
		}

		// 转职装备
		double zhuanzhi_equip_add = this->GetZhuanzhiEquip()->EquipXianPinFixValue(Equipment::E_XIANPIN_ADD_KILL_MONSTER_EXP_REWARD_PER) * 0.0001;
		if (zhuanzhi_equip_add > 0)
		{
			extra_exp_per += zhuanzhi_equip_add;
		}
	}

	if(can_add_zhuanzhi_extra)
	{
		// 转职
		extra_exp_per += this->GetRoleZhuanZhi()->GetRoleZhuanZhiSkill().GetAddExpPer();
	}

	// 魔戒技能加成
	if (can_add_mojie)
	{
		double exp_per = m_role_module->mojie_fb.GetJingLingPianExpPer(MOJIEFB_SKILL_SHEHUN);
		if(exp_per > 0)
		{
			extra_exp_per += (float)exp_per;
		}
	}

	// 护送双倍加成
	if (EXP_ADD_REASON_HUSONG == exp_addon_reason && m_role_module->husong_task.IsHusongDoubleTime())
	{
		extra_exp_per += 100.0;
	}

	// 勋章属性经验额外加成
	if (can_add_xunzhang_extra)
	{
		double xunzhang_add = this->GetXunZhangManager()->XunZhangGetAttributeValue(XUNZHANG_ADD_KILL_MONSTER_EXP_REWARD_PER) * 0.01;
		if (xunzhang_add > 0)
		{
			extra_exp_per += xunzhang_add;
		}
	}

	// 黄金vip经验额外加成
	if (can_add_glod_vip_extra)
	{
		if (this->GetGoldVip()->IsvalidGoldVip())
		{
			double glod_add = LOGIC_CONFIG->GetGoldVipCfg().GetGoldVipExpAdd() * 0.01;
			if (glod_add > 0)
			{
				extra_exp_per += glod_add;
			}
		}
	}

	// 角色目标加成
	if (can_add_role_goal_extra)
	{
		double goal_add = this->GetRoleGoal()->GetGoalExpAdd() * 0.01;
		if (goal_add)
		{
			extra_exp_per += goal_add;
		}
	}

	// 符文加成
	if (can_add_rune_extra)
	{
		double rune_add = static_cast<double>(this->GetRuneSystem()->GetSpecialEffectValuPercentByType(RUNE_SPECIAL_EFFECT_TYPE_ADD_KILL_MONSTER_EXP) * 100);
		if (rune_add)
		{
			extra_exp_per += rune_add;
		}
	}

	if (NULL != m_scene)
	{
		int special_add = m_scene->GetSpecialLogic()->GetSpecialExpAdd(this, exp_addon_reason);
		if (0 != special_add)
		{
			extra_exp_per += (double)special_add;
		}
	}

	// 神格系统
	{
		int rune_system_per = m_role_module->shenge_system.GetAddExpPercent();
		if (can_add_shenge_system && rune_system_per > 0)
		{
			extra_exp_per += rune_system_per;
		}
	}

	// 仙尊卡
	if (can_add_xianzunka)
	{
		extra_exp_per += m_role_module->xianzunka.GetAddExpPer() * 0.01;
	}

	// 小鬼守护
	if (can_imp_guard_extra)
	{
		int imp_guard_add_per = m_role_module->m_imp_guard.GetAddPerExp();
		if (imp_guard_add_per > 0)
		{
			extra_exp_per += imp_guard_add_per;
		}
	}

	// 传世名剑-技能
	if (can_add_mojieskill)
	{
		double mojieskill_add_per = m_role_module->mojie_skill.GetExpBuffPercent() * 0.01;
		if (mojieskill_add_per > 0)
		{
			extra_exp_per += mojieskill_add_per;
		}
	}

	if (can_add_tianshuxunzhu)
	{
		double tianshuxunzhu_add_per = TianShuXunZhu::GetExtraExp(this) *0.01;
		if (tianshuxunzhu_add_per > 0)
		{
			extra_exp_per += tianshuxunzhu_add_per;
		}
	}

	return extra_exp_per;
}

double Role::GetExpTotalPer(int exp_add_reason, int *add_percent)
{
	double initial_per = 100.0;//初始加成是100%
	if (exp_add_reason < EXP_ADD_REASON_DEFAULT || exp_add_reason >= EXP_ADD_REASON_MAX) return initial_per;

	double extra_per = this->GetExpExtraPer(exp_add_reason);
	
	double total_per = initial_per + extra_per;//总加成 = 初始加成 + 额外加成
	double world_extra_percent = WorldShadow::Instance().GetWorLeveldExtraExpPerByConfig(m_level, exp_add_reason);//世界等级差对经验加成的加成
	total_per = total_per * (1.0 + world_extra_percent);//总的经验加成 * （1 + 世界经验加成），所以这个世界经验加成可以理解为总经验加成的“加成”
	if (NULL != add_percent)
	{
		int protocol_extra_per = static_cast<int>(round(total_per - initial_per));//因为世界等级差加成要在总加成的基础上算的，算完后要减初始加成，再发给客户端
		*add_percent += protocol_extra_per;
		extra_per = static_cast<double>(*add_percent);
		total_per = initial_per + extra_per;	//额外加成变化了，总加成也要跟着变
	}
	return total_per;
}

long long Role::RecalcExp(long long exp)
{
	double min_val = 10000 * std::pow(m_level, 2) - 600000 * m_level + 150000000;
	double max_val = 1000000000 * std::pow(2.72, (0.0095 * (m_level - 400)));

	return static_cast<long long>(exp * (std::max)(min_val, max_val) * ROLE_ATTR_PER);
}

bool Role::ZhuanSheng()
{
	return false;
	if(m_level % ZHUANSHENG_LEVEL_STEP != 0 || m_level / ZHUANSHENG_LEVEL_STEP > MAX_ZHUAN_SHENG_LEVEL)
	{
		this->NoticeNum(errornum::EN_ITEM_LEVEL_LIMIT);
		return false;
	}

	RoleZhuanShengCfg * zhuanshen_cfg = GLOBALCONFIG->GetRoleZhuanShengCfg(m_level);
	if (NULL == zhuanshen_cfg) return false;

	Knapsack *knapsack = GetKnapsack();
	if (knapsack->IsLock())
	{
		NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return false;
	}

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();

	{
		// 进阶符
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = zhuanshen_cfg->consume_item.item_id;
		stuff_list[0].num = zhuanshen_cfg->consume_item.num;
		stuff_list[0].buyable = false;

		if (!ItemExtern::GetConsumeListByOrder(this, false, 1, stuff_list, &consume_list, true))
		{
			return false;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(this, consume_list, "UpRoleZhuanShenLevel"))
	{
		return false;
	}
	
	long long old_exp = m_exp;
	int old_level = m_level;

	//直接升1级
	if (!this->OnUpgradeLevel(m_level + 1))
	{
		return false;	
	};
	
	this->NoticeNum(noticenum::NT_ZHUANSHEN_SUCC);
	//补回升级之前的经验
	AddExp(old_exp, "ZhuanSheng", EXP_ADD_REASON_ZHUAN_SHENG);

	if (zhuanshen_cfg->is_broadcast)
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_zhuansheng_notice, 
			this->GetUID(), this->GetName(), (int)this->GetCamp(), zhuanshen_cfg->zhuansheng_level);

		if (length > 0) 
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
		}
	}

	//打印日志
	{
		gamelog::g_log_equipment.printf(LL_INFO,"[Role::ZhuanSheng Succ][user[%s %d] old_level:%d new_level:%d"
			"consume_item:%d consume_num:%d consume_is_bind:%d",
			GetName(), UidToInt(GetUserId()), old_level, m_level,
			(int)zhuanshen_cfg->consume_item.item_id, (int)zhuanshen_cfg->consume_item.num, (int)zhuanshen_cfg->consume_item.is_bind);
	}

	return true;
}

void Role::TransformExp(int seq, int item_num)
{
	if (1 != item_num && 10 != item_num) // 仅仅接受1和10的数量 防止客户端乱发
	{
		return;
	}

	const ExpBottleItemCfg *item_cfg = LOGIC_CONFIG->GetExpBottleConfig().GetExpBottleItemCfg(seq);
	if (NULL != item_cfg)
	{
		const ExpBottleLvCfg *lv_cfg = LOGIC_CONFIG->GetExpBottleConfig().GetExpBottleLvCfg(m_level);
		if (NULL != lv_cfg)
		{
			if (!m_role_module->knapsack.HaveSpace())
			{
				this->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
				return;
			}

			int transform_count = m_role_module->common_data_param.expbottle_transform_count;

			int left_count = lv_cfg->transform_maxcount - transform_count;
			if (left_count <= 0)
			{
				this->NoticeNum(errornum::EN_EXPBOTTLE_TRANSFORM_LIMIT);
				return;
			}

			if (item_num > left_count)  // 修正数量
			{
				item_num = left_count;
			}

			int need_exp = item_cfg->cost_exp * item_num;

			if (this->GetExp() < need_exp)
			{
				this->NoticeNum(errornum::EN_EXPBOTTLE_EXP_LIMIT);
				return;
			}

			this->ReduceExp(need_exp, "Role::TransformExp", NULL);
			m_role_module->common_data_param.expbottle_transform_count += item_num;

			m_role_module->knapsack.Put(ItemConfigData(item_cfg->bottle_itemid, 0, item_num), PUT_REASON_EXP_BOTTLE);

			this->SendExpBottleInfo();
		}
	}
}

void Role::SendExpBottleInfo()
{
	static Protocol::SCExpBottleInfo ebi;
	ebi.expbottle_use_count = m_role_module->common_data_param.expbottle_use_count;
	ebi.expbottle_transform_count = m_role_module->common_data_param.expbottle_transform_count;
	EngineAdapter::Instance().NetSend(m_netid, (const char *)&ebi, sizeof(ebi));
}

void Role::SendMoveChessInfo()
{
	static Protocol::SCMoveChessInfo ebi;
	ebi.move_chess_free_times = m_role_module->common_data_param.move_chess_free_times;
	ebi.move_chess_reset_times = m_role_module->common_data_param.move_chess_reset_times;
	ebi.move_chess_cur_step = m_role_module->common_data_param.move_chess_cur_step;
	ebi.move_chess_next_free_time = m_role_module->common_data_param.move_chess_next_free_time;

	EngineAdapter::Instance().NetSend(m_netid, (const char *)&ebi, sizeof(ebi));
}

void Role::SendMillionaireInfo()
{
	static Protocol::SCMillionaireInfo ebi;

	if (ActivityShadow::Instance().IsActivtyOpen(ACTIVITY_TYPE_MILLIONAIRE))
	{
		if (m_role_module->common_data_param.millionaire_last_join_time != ActivityShadow::Instance().GetActivityBeginTime(ACTIVITY_TYPE_MILLIONAIRE))
		{
			m_role_module->common_data_param.millionaire_last_join_time = ActivityShadow::Instance().GetActivityBeginTime(ACTIVITY_TYPE_MILLIONAIRE);
			m_role_module->common_data_param.millionaire_gather_count = 0; 
			m_role_module->common_data_param.is_turn_millionaire = 0;
			m_role_module->common_data_param.is_turn = 0;
			m_role_module->common_data_param.millionaire_reward_index = -1;
			m_role_module->common_data_param.millionaire_use_skill_times = 0;
			m_role_module->common_data_param.millionaire_valid_perform_skill_time = 0;
		}
	}

	ebi.gather_total_times = m_role_module->common_data_param.millionaire_gather_count;
	ebi.is_millionaire = m_role_module->common_data_param.is_turn_millionaire;
	ebi.is_turn = m_role_module->common_data_param.is_turn;
	ebi.reward_index = m_role_module->common_data_param.millionaire_reward_index;
	ebi.millionaire_use_skill_times = m_role_module->common_data_param.millionaire_use_skill_times;
	ebi.millionaire_valid_perform_skill_time = m_role_module->common_data_param.millionaire_valid_perform_skill_time;

	EngineAdapter::Instance().NetSend(m_netid, (const char *)&ebi, sizeof(ebi));
}

void Role::SendWorldBossWearyInfo()
{
	static Protocol::SCWorldBossWearyInfo cmk;

	cmk.worldboss_weary = m_role_module->common_data_param.mikuboss_relive_weary;
	cmk.worldboss_weary_last_die_time = m_role_module->common_data_param.miku_relive_weary_last_die_time;
	cmk.worldboss_weary_last_relive_time = m_role_module->common_data_param.miku_relive_weary_last_relive_time;

	EngineAdapter::Instance().NetSend(m_netid, (const char *)&cmk, sizeof(cmk));
}

bool Role::CanRealiveWithMiKuTire()
{
	if (m_role_module->common_data_param.mikuboss_relive_weary >= 5 &&
		EngineAdapter::Instance().Time() - m_role_module->common_data_param.miku_relive_weary_last_relive_time <= 60)           // 疲劳值已满不可立即回城复活
	{
		return false;
	}

	return true;
}

void Role::DieOnMiKuScene(bool is_die)
{
	//策划要求屏蔽复活疲劳的功能,先留着代码，需要再开回来

	/*unsigned int now_time = (unsigned int)EngineAdapter::Instance().Time();
	if (is_die)
	{
		m_role_module->common_data_param.miku_relive_weary_last_die_time = now_time;
	}
	else
	{
		++m_role_module->common_data_param.mikuboss_relive_weary;
		m_role_module->common_data_param.miku_relive_weary_last_relive_time = now_time;
	}
	
	this->SendWorldBossWearyInfo();*/
}

void Role::AddExpBottleUseCount(int num)
{
	m_role_module->common_data_param.expbottle_use_count += num;;

	this->SendExpBottleInfo();
}

void Role::ResetExpBottleDayCount()
{
	m_role_module->common_data_param.expbottle_use_count = 0;
	m_role_module->common_data_param.expbottle_transform_count = 0;

	this->SendExpBottleInfo();
}

void Role::ResetMoveChessDayCount()
{
	m_role_module->common_data_param.move_chess_free_times = 0;
	m_role_module->common_data_param.move_chess_reset_times = 0;
	m_role_module->common_data_param.move_chess_cur_step = 0;
	m_role_module->common_data_param.move_chess_next_free_time = 0;
	this->SendMoveChessInfo();
}


void Role::GMSetLevel(int level)
{
	if (level > 0 && level <= MAX_ROLE_LEVEL)
	{
		this->OnUpgradeLevel(level);
	}
}

void Role::GMAddRoleOnLineTime(int seconds)
{
	m_online_time += seconds;
	if (m_online_time < 0)
	{
		m_online_time = 0;
	}

	m_today_online_time += seconds;
	if (m_today_online_time < 0)
	{
		m_today_online_time = 0;
	}

	this->GetWelfare()->SendWelfareInfo();
}

bool Role::StartGather(ObjID obj_id, int gather_count)
{
	if (NULL == m_scene) return false;

	Obj *gather_obj = m_scene->GetObj(obj_id);
	if (NULL == gather_obj || gather_obj->GetObjType() != Obj::OBJ_TYPE_GATHER)
	{
		return false;
	}

	static const int MAX_GATHER_DISTANCE = GLOBALCONFIG->GetGatherMaxDistance();
	if ((m_posi - gather_obj->GetPos()).IsLongerThan(MAX_GATHER_DISTANCE)) // 检查采集距离
	{
		this->NoticeNum(errornum::EN_GATHER_TOO_FAR);
		return false;
	}

	if (m_role_status == ROLE_STATUS_REST) ResetRoleStatus();

	GatherObj *gather = (GatherObj*)gather_obj;
	if (!CanCharacterAction())
	{
		this->NoticeNum(errornum::EN_STATUS_CAN_NOT_GATHER);
		return false;
	}
	
	if (!m_role_module->role_status_manager.CanGather(gather))
	{
		return false;
	}

	if (m_role_status == ROLE_STATUS_GATHER)
	{
		if (m_gather_obj_id == obj_id)
		{
			return false;
		}
		ResetRoleStatus();
	}

	if (!CanRoleAction())
	{
		NoticeNum(errornum::EN_STATUS_CAN_NOT_GATHER);
		return false;
	}

	ActionStop();
	
	// 2019-04-13 采集打断隐身状态
	if (m_effect_list.IsInvisible())
	{
		m_effect_list.FindAndRemoveEffect(EffectBase::EFFECT_TYPE_INVISIBLE, EBT_INVISIBLE, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SKILL, 0);
	}

	float inc_gather_per = 0.0f;
	EffectOtherBuff *buff = this->GetEffectList()->FindOtherEffectBuff(EffectBase::PRODUCT_ID_GATHER_SPEED);
	if (NULL != buff)
	{
		inc_gather_per = buff->GetIncGatherSpeedPer() * SKILL_ATTR_PER;
	}
	
	m_gather_obj_id = obj_id;
	m_gather_time = static_cast<int>(gather->GatherTime(m_user_id) / (1.0 + inc_gather_per));
	m_gather_count = gather_count;
	m_role_status = ROLE_STATUS_GATHER;

	if (INVALID_OBJ_ID == gather->GetGatherIngObjId())
	{
		gather->SetGatherIngObjId(this->GetId());
	}

	// 发送开始收集的消息

	// 广播
	Protocol::SCStartGather sg;
	sg.role_obj_id = m_obj_id;
	sg.gather_obj_id = obj_id;
	m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&sg, sizeof(Protocol::SCStartGather));

	// 发给个人
	Protocol::SCStartGatherTimer sgt;
	sgt.gather_time = (unsigned int)m_gather_time;
	EngineAdapter::Instance().NetSend(m_netid, (const char *)&sgt, sizeof(Protocol::SCStartGatherTimer));

	EventHandler::Instance().OnStartGather(this, gather->GatherId(), gather->GetId(), gather->GetPos());

	return true;
}

bool Role::OnRest(bool is_force)
{
	if (NULL == m_scene) return false;

	if (this->GetLevel() < GLOBALCONFIG->GetRestLimitLevel())
	{
		this->NoticeNum(errornum::EN_ROLE_REST_LEVEL_NOT_ENOUGH);
		return false;
	}

	if (m_role_status == ROLE_STATUS_REST)
	{
		this->ResetRoleStatus();
	}
	else
	{
		if (!m_role_module->role_status_manager.CanRest()) return false;

		this->ActionStop();
		
		time_t now = EngineAdapter::Instance().Time();
		m_rest_next_recovery_time = now + GLOBALCONFIG->GetRestRecoveryInterval();
		m_rest_next_reward_time = now + GLOBALCONFIG->GetRestRewardInterval();
		m_shuangxiu_next_intimacy_time = now + GLOBALCONFIG->GetShuangxiuIntimacyInterval();

		m_role_status = ROLE_STATUS_REST;

		Protocol::SCRestStatus rs;
		rs.obj_id = m_obj_id;
		rs.start_or_stop = 1;	// 开始
		rs.partner_obj_id = INVALID_OBJ_ID;
		rs.reserved = 0;
		m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&rs, sizeof(Protocol::SCRestStatus));
	}

	return true;
}

void Role::OnRestInvite(ObjID obj_id)
{
	if (NULL == m_scene) return;

	if (!m_role_module->role_status_manager.CanRest()) return;

	if (m_level < GLOBALCONFIG->GetRestLimitLevel())
	{
		this->NoticeNum(errornum::EN_ROLE_SHUANGXIU_LEVEL_NOT_ENOUGH);
		return;
	}

	if (this->HasRestPartner())
	{
		this->NoticeNum(errornum::EN_ROLE_ALREADY_HAS_REST_PARNTER);
		return;
	}

	Obj *receive_invite_obj = m_scene->GetObj(obj_id);
	if (NULL == receive_invite_obj || Obj::OBJ_TYPE_ROLE != receive_invite_obj->GetObjType())
	{
		this->NoticeNum(errornum::EN_REST_OTHER_NOT_ONLINE);
		return;
	}

	Role *receive_invite_role = (Role *)receive_invite_obj;

	if (!receive_invite_role->GetRoleStatusManager()->CanRest(false))
	{
		this->NoticeNum(errornum::EN_REST_OTHER_CAN_NOT_REST);
		return;
	}

	if (receive_invite_role->GetLevel() < GLOBALCONFIG->GetRestLimitLevel())
	{
		this->NoticeNum(errornum::EN_ROLE_OTHER_SHUANGXIU_LEVEL_NOT_ENOUGH);
		return;
	}

	if (receive_invite_role->HasRestPartner())
	{
		this->NoticeNum(errornum::EN_REST_ALREADY_HAS_PARNTER);
		return;
	}

	Protocol::SCRestInviteNotify invite_notify;
	invite_notify.obj_id = m_obj_id;
	this->GetName(invite_notify.role_name);
	invite_notify.reserved = 0;
	EngineAdapter::Instance().NetSend(receive_invite_role->GetNetId(), (const char*)&invite_notify, sizeof(invite_notify));

	this->NoticeNum(noticenum::NT_REST_INVITE_SUCC);
}

void Role::OnRestInviteAck(ObjID obj_id, bool is_accept)
{
	if (NULL == m_scene) return;

	if (!m_role_module->role_status_manager.CanRest()) return;

	if (m_level < GLOBALCONFIG->GetRestLimitLevel())
	{
		this->NoticeNum(errornum::EN_ROLE_SHUANGXIU_LEVEL_NOT_ENOUGH);
		return;
	}

	if (this->HasRestPartner()) 
	{
		this->NoticeNum(errornum::EN_ROLE_ALREADY_HAS_REST_PARNTER);
		return;
	}

	Obj *obj_ack_to = m_scene->GetObj(obj_id);
	if (NULL == obj_ack_to || Obj::OBJ_TYPE_ROLE != obj_ack_to->GetObjType())
	{
		this->NoticeNum(errornum::EN_REST_OTHER_NOT_ONLINE);
		return;
	}

	Role *inviting_role = (Role *)obj_ack_to;

	Protocol::SCRestInviteResult invite_result;
	invite_result.beinviter_obj_id = m_obj_id;
	this->GetName(invite_result.role_name);

	if (is_accept)
	{
		if (!inviting_role->GetRoleStatusManager()->CanRest(false))
		{
			this->NoticeNum(errornum::EN_REST_OTHER_CAN_NOT_REST);
			return;
		}

		if (inviting_role->GetLevel() < GLOBALCONFIG->GetRestLimitLevel())
		{
			this->NoticeNum(errornum::EN_ROLE_OTHER_SHUANGXIU_LEVEL_NOT_ENOUGH);
			return;
		}

		if (inviting_role->HasRestPartner())
		{
			this->NoticeNum(errornum::EN_REST_ALREADY_HAS_PARNTER);
			return;
		}

		invite_result.is_accept = 1;
		EngineAdapter::Instance().NetSend(inviting_role->GetNetId(), (const char*)&invite_result, sizeof(invite_result));
	}
	else
	{
		if (!inviting_role->HasRestPartner())
		{
			invite_result.is_accept = 0;
			EngineAdapter::Instance().NetSend(inviting_role->GetNetId(), (const char*)&invite_result, sizeof(invite_result));
		}
	}
}

void Role::OnRestStartAffirm(ObjID obj_id)
{
	if (NULL == m_scene) return;

	if (!m_role_module->role_status_manager.CanRest()) return;

	if (m_level < GLOBALCONFIG->GetRestLimitLevel())
	{
		this->NoticeNum(errornum::EN_ROLE_SHUANGXIU_LEVEL_NOT_ENOUGH);
		return;
	}

	if (this->HasRestPartner()) 
	{
		this->NoticeNum(errornum::EN_ROLE_ALREADY_HAS_REST_PARNTER);
		return;
	}

	Obj *partner_obj = m_scene->GetObj(obj_id);
	if (NULL == partner_obj || Obj::OBJ_TYPE_ROLE != partner_obj->GetObjType())
	{
		this->NoticeNum(errornum::EN_REST_OTHER_NOT_ONLINE);
		return;
	}

	Role *partner_role = (Role *)partner_obj;

	if (!partner_role->GetRoleStatusManager()->CanRest(false))
	{
		this->NoticeNum(errornum::EN_REST_OTHER_CAN_NOT_REST);
		return;
	}

	if (partner_role->GetLevel() < GLOBALCONFIG->GetRestLimitLevel())
	{
		this->NoticeNum(errornum::EN_ROLE_SHUANGXIU_LEVEL_NOT_ENOUGH);
		return;
	}

	if (partner_role->HasRestPartner()) 
	{
		this->NoticeNum(errornum::EN_ROLE_ALREADY_HAS_REST_PARNTER);
		return;
	}

	this->ResetRoleStatus();
	this->OnRest(true);
	this->SetRestPartner(obj_id);

	partner_role->ResetRoleStatus();
	partner_role->OnRest(true);
	partner_role->SetRestPartner(m_obj_id);
}

void Role::SetRestPartner(ObjID obj_id, int start_or_stop)
{
	Protocol::SCRestStatus rs;
	rs.obj_id = m_obj_id;
	rs.start_or_stop = start_or_stop;
	rs.partner_obj_id = (0 != start_or_stop ? obj_id : m_rest_partner_obj_id);
	rs.reserved = 0;
	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&rs, sizeof(Protocol::SCRestStatus));

	m_rest_partner_obj_id = obj_id;
}

void Role::RefreshApperance(bool notify, short wuqi_use_type, short body_use_type)
{
	if (wuqi_use_type != APPEARANCE_WUQI_USE_TYPE_INVALID)
	{
		m_role_module->common_data_param.appearacne_wuqi_use_type =static_cast<char>(wuqi_use_type);
	}

	if (body_use_type != APPEARANCE_BODY_USE_TYPE_INVALID)
	{
		m_role_module->common_data_param.appearacne_body_use_type =static_cast<char>( body_use_type);
	}

	RoleAppearance appearance = m_appearance;

	m_appearance.Reset();
	m_appearance.shizhuang_wuqi = m_role_module->common_data_param.appearacne_wuqi_use_type;
	m_appearance.shizhuang_body = m_role_module->common_data_param.appearacne_body_use_type;
	m_role_module->equip_manager.GetRoleAppearance(&m_appearance);
	m_role_module->shizhuang.GetRoleAppearance(&m_appearance);
	m_role_module->mount_manager.GetRoleAppearance(&m_appearance);
	m_role_module->fightmount_manager.GetRoleAppearance(&m_appearance);
	m_role_module->wing.GetRoleAppearance(&m_appearance);
	m_role_module->halo.GetRoleAppearance(&m_appearance);
	m_role_module->shengong.GetRoleAppearance(&m_appearance);
	m_role_module->shenyi.GetRoleAppearance(&m_appearance);
	m_role_module->footprint.GetRoleAppearance(&m_appearance);
	m_role_module->cloak_manager.GetRoleAppearance(&m_appearance);
	m_role_module->xiannvshouhu_manager.GetRoleAppearance(&m_appearance);
	m_role_module->jinglingguanghuan_manager.GetRoleAppearance(&m_appearance);
	m_role_module->jinglingfazhen_manager.GetRoleAppearance(&m_appearance);
	//m_role_module->zhibao_manager.GetRoleAppearance(&m_appearance);            // 至宝外观其实就是法宝外观，先在法宝里给外观赋值法宝形象id
	m_role_module->fabao.GetRoleAppearance(&m_appearance);
	m_role_module->jingjie.GetRoleAppearance(&m_appearance);
	m_role_module->yaoshi_manager.GetRoleAppearance(&m_appearance);
	m_role_module->toushi_manager.GetRoleAppearance(&m_appearance);
	m_role_module->qilinbi_manager.GetRoleAppearance(&m_appearance);
	m_role_module->mask_manager.GetRoleAppearance(&m_appearance);
	m_role_module->upgrade_manager.GetRoleAppearance(&m_appearance);
	m_role_module->shen_qi.GetRoleAppearance(&m_appearance);

	if (notify && !m_appearance.IsEquil(appearance))
	{
		this->BroadcastAppearanceInfo();
	}
}

void Role::BroadcastAppearanceInfo()
{
	static Protocol::SCRoleVisibleChange voc;
	voc.obj_id = this->GetId();
	voc.reserved = 0;
	voc.appearance = m_appearance;
	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(this->GetObserHandle(), (void*)&voc, sizeof(voc));
}

bool Role::AddEvil(int evil)
{
	if (0 == evil) return false;
	if (evil < 0 && m_evil <= 0) return false;

	int old_evil = m_evil;
	m_evil += evil;
	if (m_evil <= 0) m_evil = 0;
	if (m_evil > GLOBALCONFIG->GetNameColorMaxEvil()) m_evil = GLOBALCONFIG->GetNameColorMaxEvil();

	this->SetNameColor((char)GLOBALCONFIG->GetNameColorByEvil(m_evil));

	Protocol::SCRoleEvilChange rec;
	rec.evil = m_evil;
	EngineAdapter::Instance().NetSend(m_netid, (const char*)&rec, sizeof(rec));

	{
		if (old_evil <= 0 && m_evil > 0)
		{
			m_namecolor_last_dec_evil_time = EngineAdapter::Instance().Time();
		}

		if (m_evil <= 0)
		{
			m_namecolor_last_dec_evil_time = 0;
		}
	}

	return true;
}

void Role::SetNameColor(int name_color)
{
	if (name_color != m_name_color)
	{
		m_name_color = name_color;
		this->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTR_PER, EBT_INVALID, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_NAME_COLOR, EffectBase::PRODUCT_ID_NAMECOLOR);

		int reduce_hp_per = GLOBALCONFIG->GetReduceHpPercentByNameColor(m_name_color);
		int reduce_gongji_per = GLOBALCONFIG->GetReduceHpPercentByNameColor(m_name_color);
		if (reduce_hp_per > 0 && reduce_gongji_per > 0)
		{
			EffectPercentBuff *effect = new EffectPercentBuff(INVALID_OBJ_ID, EffectBase::PRODUCT_ID_NAMECOLOR, 0, 1, EffectBase::PRODUCT_METHOD_NAME_COLOR);
			effect->AddAttrEffect(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, -reduce_hp_per);
			effect->AddAttrEffect(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, -reduce_gongji_per);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
			effect->SetClientEffectType(EffectBase::ECT_OTHER_HMCF);
			effect->SetSave(true);
			this->AddEffect(effect);
		}

		Protocol::SCRoleNameColorChange rncc;
		rncc.obj_id = m_obj_id;
		rncc.name_color = m_name_color;
		rncc.reserved = 0;
		if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&rncc, sizeof(rncc));
	}
}

bool Role::AddXianhun(int xianhun, const char *type)
{
	if (0 == xianhun || NULL == type) return true;

	long long tmp_xianhun = m_xianhun;
	tmp_xianhun += xianhun;

	if (tmp_xianhun < 0) return false;
	if (tmp_xianhun > MAX_XIANHUN_VAL) tmp_xianhun = MAX_XIANHUN_VAL;
	
	m_xianhun = static_cast<int>(tmp_xianhun);

	if (m_xianhun < 0) m_xianhun = 0;

	Protocol::SCRoleXianhun scxh;
	scxh.xianhun = m_xianhun;
	EngineAdapter::Instance().NetSend(m_netid, (const char*)&scxh, sizeof(scxh));

	gamelog::g_log_roledata.printf(LL_INFO, "[Role::AddXianhun Succ] [user[%d %s], type:%s, add_xianhun:%d, left_xianhun:%d]", 
		UidToInt(m_user_id), m_name, type, xianhun, m_xianhun);

	ROLE_LOG_QUICK6(LOG_TYPE_XIANHUN, this, xianhun, m_xianhun, type, NULL);

	return true;
}


bool Role::AddYuanli(int yuanli, const char *type)
{
	if (0 == yuanli || NULL == type) return true;

	long long tmp_yuanli = m_yuanli;
	tmp_yuanli += yuanli;

	if (tmp_yuanli < 0) return false;
	if (tmp_yuanli > MAX_YUANLI_VAL) tmp_yuanli = MAX_YUANLI_VAL;

	m_yuanli = static_cast<int>(tmp_yuanli);

	if (m_yuanli < 0) m_yuanli = 0;

	Protocol::SCRoleYuanli scyl;
	scyl.yuanli = m_yuanli;
	EngineAdapter::Instance().NetSend(m_netid, (const char*)&scyl, sizeof(scyl));

	gamelog::g_log_roledata.printf(LL_INFO, "[Role::AddYuanli Succ] [user[%d %s], type:%s, add_yuanli:%d, left_yuanli:%d]", 
		UidToInt(m_user_id), m_name, type, yuanli, m_yuanli);

	ROLE_LOG_QUICK6(LOG_TYPE_YUANLI, this, yuanli, m_yuanli, type, NULL);

	return true;
}

bool Role::AddNvWaShi(int nv_wa_shi, const char *type)
{
	if (0 == nv_wa_shi || NULL == type) return true;

	long long tmp_nv_wa_shi = m_nv_wa_shi;
	tmp_nv_wa_shi += nv_wa_shi;

	if (tmp_nv_wa_shi < 0) return false;
	if (tmp_nv_wa_shi > MAX_NVWASHI_VAL) tmp_nv_wa_shi = MAX_NVWASHI_VAL;

	m_nv_wa_shi = static_cast<int>(tmp_nv_wa_shi);

	if (m_nv_wa_shi < 0) m_nv_wa_shi = 0;

	Protocol::SCRoleNvWaShi scnws;
	scnws.nv_wa_shi = m_nv_wa_shi;
	EngineAdapter::Instance().NetSend(m_netid, (const char*)&scnws, sizeof(scnws));

	gamelog::g_log_other.printf(LL_INFO, "AddNvWaShi user[%d, %s], type:%s, level[%d], nvwashi[%d]", m_uid, m_name, type, m_level, nv_wa_shi);

	ROLE_LOG_QUICK6(LOG_TYPE_NV_WA_SHI, this, nv_wa_shi, m_nv_wa_shi, type, NULL);

	return true;
}

bool Role::AddLingJing(int lingjing, const char *type)
{
	m_lingjing += lingjing;
	m_lingjing = m_lingjing < 0 ? 0 : m_lingjing;

	Protocol::SCRoleLingJing msg;
	msg.lingjing = m_lingjing;
	EngineAdapter::Instance().NetSend(m_netid, (const char*)&msg, sizeof(msg));

	gamelog::g_log_other.printf(LL_INFO, "AddLingJing user[%d, %s], type:%s, level[%d], lingjing[%d]", m_uid, m_name, type, m_level, lingjing);

	ROLE_LOG_QUICK6(LOG_TYPE_LINGJING, this, lingjing, m_lingjing, type, NULL);

	return true;
}

bool Role::AddChengJiu(int chengjiu, const char *type)
{
	int old_chengjiu = m_chengjiu;

	m_chengjiu += chengjiu;
	m_chengjiu = m_chengjiu < 0 ? 0 : m_chengjiu;

	Protocol::SCRoleChengJiu msg;
	msg.chengjiu = m_chengjiu;
	EngineAdapter::Instance().NetSend(m_netid, (const char*)&msg, sizeof(msg));

	EventHandler::Instance().OnAddChengJiu(this, old_chengjiu, m_chengjiu);

	gamelog::g_log_other.printf(LL_INFO, "AddChengJiu user[%d, %s], type:%s, level[%d], chengjiu[%d]", m_uid, m_name, type, m_level, chengjiu);
	
	ROLE_LOG_QUICK6(LOG_TYPE_CHENGJIU, this, chengjiu, m_chengjiu, type, NULL);

	return true;
}

void Role::AddOrResetDayRevivalTimes(bool is_reset)
{
	if (is_reset)
	{
		m_role_module->common_data_param.m_day_revival_times = 0;
	}
	else
	{
		++ m_role_module->common_data_param.m_day_revival_times;
	}

	Protocol::SCRoleDayRevivalTimes msg;
	msg.day_revival_times = m_role_module->common_data_param.m_day_revival_times;
	EngineAdapter::Instance().NetSend(m_netid, (const char*)&msg, sizeof(msg));
}

unsigned short Role::GetDayRevivalTimes()
{
	return m_role_module->common_data_param.m_day_revival_times;
}

bool Role::AddHunli(int hunli, const char *type)
{
	m_hunli += hunli;
	m_hunli = m_hunli < 0 ? 0 : m_hunli;

	Protocol::SCRoleHunli msg;
	msg.hunli = m_hunli;
	EngineAdapter::Instance().NetSend(m_netid, (const char*)&msg, sizeof(msg));

	gamelog::g_log_other.printf(LL_INFO, "AddHunli user[%d, %s], type %s, level[%d], hunli[%d]", m_uid, m_name, type, m_level, hunli);

	ROLE_LOG_QUICK6(LOG_TYPE_HUNLI, this, hunli, m_hunli, type, NULL);

	return true;
}

int Role::GetNuqi()
{
	return m_role_module->common_data_param.nuqi;
}

void Role::AddNuqi(int nuqi)
{
	int old_nuqi = m_role_module->common_data_param.nuqi;

	m_role_module->common_data_param.nuqi += nuqi;

	if (m_role_module->common_data_param.nuqi < 0) m_role_module->common_data_param.nuqi = 0;
	if (m_role_module->common_data_param.nuqi > GLOBALCONFIG->GetMaxNuqi())
	{
		m_role_module->common_data_param.nuqi = GLOBALCONFIG->GetMaxNuqi();
	}

	if (old_nuqi != m_role_module->common_data_param.nuqi)
	{
		Protocol::SCRoleNuqi scnuqi;
		scnuqi.nuqi = m_role_module->common_data_param.nuqi;
		EngineAdapter::Instance().NetSend(m_netid, (const char*)&scnuqi, sizeof(scnuqi));
	}
}

int Role::GetShengwang()
{
	return m_role_module->common_data_param.shengwang;
}

bool Role::AddShengwang(int shengwang, const char *type)
{
	if (0 == shengwang || NULL == type)
	{
		return true;
	}

	int old_shengwang = m_role_module->common_data_param.shengwang;

	m_role_module->common_data_param.shengwang += shengwang;
	if (m_role_module->common_data_param.shengwang < 0)
	{
		m_role_module->common_data_param.shengwang = 0;
	}

	if (old_shengwang != m_role_module->common_data_param.shengwang)
	{
		Protocol::SCRoleShengwang rsw;
		rsw.shengwang = m_role_module->common_data_param.shengwang;
		EngineAdapter::Instance().NetSend(m_netid, (const char*)&rsw, sizeof(rsw));
	}

	EventHandler::Instance().OnAddShengwang(this, shengwang);

	gamelog::g_log_other.printf(LL_INFO, "AddShengwang user[%d, %s], type %s, level[%d], shengwang[%d]", m_uid, m_name, type, m_level, shengwang);

	ROLE_LOG_QUICK6(LOG_TYPE_ROLE_SHENGWANG, this, shengwang, m_role_module->common_data_param.shengwang, type, NULL);

	return true;
}

int Role::GetGongXun()
{
	return m_role_module->common_data_param.day_gongxun;
}

bool Role::AddGongxun(const int gongxun, const char *type)
{
	if (0 == gongxun)
	{
		return true;
	}

	m_role_module->common_data_param.day_gongxun += gongxun;	
	if (m_role_module->common_data_param.day_gongxun < 0)
	{
		m_role_module->common_data_param.day_gongxun = 0;
	}

	this->SyncGongXunInfo();

	gamelog::g_log_other.printf(LL_INFO, "AddGongxun user[%d, %s], type %s, [%d]", m_uid, m_name, type, gongxun);

	return true;
}

void Role::ResetGongXun()
{
	m_role_module->common_data_param.day_gongxun = 0;

	this->SyncGongXunInfo();
}

void Role::SyncGongXunInfo()
{
	Protocol::SCRoleGongxun cmd;
	cmd.gongxun = this->GetGongXun();
	EngineAdapter::Instance().NetSend(this->GetNetId(), (const char*)&cmd, sizeof(cmd));
}

void Role::SendAddGuildGongziInfo(int gongzi)
{
	Protocol::SCNotifyGuildSuper ngs;
	ngs.notify_type = Protocol::GUILD_NOTIFY_TYPE_GET_GONGZI;
	ngs.notify_param = gongzi;
	ngs.notify_param1 = 0;

	EngineAdapter::Instance().NetSend(this->GetNetId(), (const char*)&ngs, sizeof(ngs));
}

void Role::SetMoveMode(int move_mode, const Posi &longjump_dest_pos, char move_mode_param)
{
	if (move_mode < 0 || move_mode >= MOVE_MODE_MAX) return;

// 	if (MOVE_MODE_FLY == move_mode)
// 	{
// 		// 屏蔽飞行
// 		return;
// 	}

	if (MOVE_MODE_NORMAL != move_mode)
	{
		if (MOVE_MODE_FLY == move_mode && MOVE_MODE_NORMAL != m_move_mode)
		{
			return;
		}

		if (this->GetHusongTask()->IsHusongStatus() || JingHuaHuSongMgr::Instance().IsHusongStatus(this))
		{
			return;
		}

		if (MOVE_MODE_JUMP == move_mode)
		{
			if (MOVE_MODE_FLY == m_move_mode || m_level < GLOBALCONFIG->GetJumpLimitLevel())
			{
				return;
			}

			unsigned int time_now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
			int limit_times = GLOBALCONFIG->GetJumpLimitTimes();

			int recover_time = GLOBALCONFIG->GetJumpRecoverTime();
			if (recover_time > 0)
			{
				int add_times = (time_now - m_role_module->common_data_param.jump_last_recover_time) / recover_time;
				m_role_module->common_data_param.jump_remain_times += add_times;

				if (add_times > 0 && m_role_module->common_data_param.jump_remain_times < limit_times)
				{
					m_role_module->common_data_param.jump_last_recover_time += (add_times * recover_time);
				}
			}
			else
			{
				m_role_module->common_data_param.jump_remain_times = limit_times;
			}

			if (m_role_module->common_data_param.jump_remain_times <= 0)
			{
				return;
			}
			
			if (m_role_module->common_data_param.jump_remain_times >= limit_times)
			{
				m_role_module->common_data_param.jump_remain_times = limit_times;
				m_role_module->common_data_param.jump_last_recover_time = time_now;
			}

			m_role_module->common_data_param.jump_remain_times--;
		}
	}

	char old_move_mode = m_move_mode;

	m_move_mode = static_cast<char>(move_mode);
	m_move_mode_param = move_mode_param;

	if (MOVE_MODE_FLY == m_move_mode)
	{
		m_role_module->role_status_manager.OnFly();
	}
	else if (MOVE_MODE_JUMP == m_move_mode || MOVE_MODE_JUMP2 == m_move_mode)
	{
		//记录有效的起跳点.如果飞行中途下线.
		if (GetScene()->GetMap()->Validate(m_obj_type, GetPos().x, GetPos().y)) 
		{
			m_jump_src_pos = GetPos();
		}
		m_jump_last_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	}

	if (MOVE_MODE_FLY == old_move_mode || MOVE_MODE_FLY == m_move_mode)
	{
		this->ReCalcAttr(CharIntAttrs::RECALC_REASON_SELF);
	}

	static Protocol::SCObjMoveMode omm;
	omm.obj_id = m_obj_id;
	omm.move_mode = m_move_mode;
	omm.move_mode_param = move_mode_param;
	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&omm, sizeof(omm));
}

void Role::FixMoveMode()
{
	switch (GetMoveMode())
	{
	case Role::MOVE_MODE_JUMP:
	case Role::MOVE_MODE_JUMP2:
		ResetPos(GetJumpSrcPos().x, GetJumpSrcPos().y);
		m_move_mode = Role::MOVE_MODE_NORMAL;
		break;
	default:
		break;
	}

}

void Role::SetAcceptTaskAppearn(char appearn, int appearn_param) {
	if (appearn < CHANGE_MODE_TASK_TYPE_INVALID || appearn >= CHANGE_MODE_APPEARN_MAX) return;

	m_role_module->common_data_param.accept_task_change_appearn = appearn;
	m_role_module->common_data_param.accept_task_change_appearn_param = appearn_param;
	this->NotifyAcceptTaskAppearn();

	switch (appearn)
	{
	case CHANGE_MODE_TASK_TYPE_FLY:
		if (appearn_param == 0)
		{
			m_role_module->newtask_manager.RemoveFlyTaskEffetc();
		}
		else
		{
			m_role_module->newtask_manager.AddFlyTaskEffect();
		}
		break;
	default:
		if (appearn_param == 0)
		{
			m_role_module->newtask_manager.RemoveBianshenTaskEffetc();
		}
		else
		{
			m_role_module->newtask_manager.AddBianshenTaskEffect();
		}
		break;
	}

	
}

char Role::GetAcceptTaskAppearn() {
	return m_role_module->common_data_param.accept_task_change_appearn;
}

int Role::GetAcceptTaskAppearnParam() {
	return m_role_module->common_data_param.accept_task_change_appearn_param;
}

void Role::NotifyAcceptTaskAppearn() {
	static Protocol::SCRoleAccetpTaskAppearn omm;
	omm.obj_id = m_obj_id;
	omm.task_appearn = m_role_module->common_data_param.accept_task_change_appearn;
	omm.reserve = 0;
	omm.task_appearn_param_1 = m_role_module->common_data_param.accept_task_change_appearn_param;
	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&omm, sizeof(omm));
}

bool Role::IsTaskAppearnStatus() {
	return 0 != m_role_module->common_data_param.accept_task_change_appearn_param;
}

void Role::CheckTaskAppearnStatusWithEnterScene()
{
	if (nullptr == m_scene)
	{
		return;
	}

	// 进入副本要取消任务变身状态，回到普通场景再恢复
	if (!m_scene->InStaticScene())
	{
		if (this->IsTaskAppearnStatus())
		{
			char appearn = m_role_module->common_data_param.accept_task_change_appearn;
			m_role_module->common_data_param.last_accept_task_change_appearn = appearn;
			m_role_module->common_data_param.last_accept_task_change_appearn_param = m_role_module->common_data_param.accept_task_change_appearn_param;
			this->SetAcceptTaskAppearn(appearn, 0);  // 取消状态
		}
		else
		{
			m_role_module->common_data_param.last_accept_task_change_appearn = 0;
			m_role_module->common_data_param.last_accept_task_change_appearn_param = 0;
		}
	}
	else
	{
		int last_appearn = m_role_module->common_data_param.last_accept_task_change_appearn;
		int last_appearn_param = m_role_module->common_data_param.last_accept_task_change_appearn_param;

		m_role_module->common_data_param.last_accept_task_change_appearn = 0;
		m_role_module->common_data_param.last_accept_task_change_appearn_param = 0;

		if (last_appearn_param > 0 &&
			0 == m_role_module->common_data_param.accept_task_change_appearn_param &&
			this->GetTaskManager()->HasTaskBianshen(last_appearn, last_appearn_param))
		{
			this->SetAcceptTaskAppearn(last_appearn, last_appearn_param);  // 设回状态
		}
	}
}


void Role::ResetRoleStatus(ObjID attacker, Attribute delta_hp, short reset_reason)
{
	if (NULL == m_scene) return; 

	bool reset_status = true;

	if (m_role_status != ROLE_STATUS_CHARACTER)
	{
		switch (m_role_status)
		{
		case ROLE_STATUS_GATHER:
			{
				if (INVALID_OBJ_ID != attacker)
				{
					Obj *obj = m_scene->GetObj(attacker);
					if (NULL != obj && Obj::OBJ_TYPE_MONSTER != obj->GetObjType())
					{
						if (NULL != m_scene->GetSpecialLogic() && !m_scene->GetSpecialLogic()->SpecialStopGahterOnAttack(this))
						{
							reset_status = false;
						}

						if (NULL != this->GetEffectList()->FindOtherEffectBuff(EffectBase::PRODUCT_ID_GATHER_NOT_STOP))
						{
							reset_status = false;
						}
					}
					else
					{
						reset_status = false;
					}
				}

				// 用于某些场景无敌采集
				if (this->IsAlive() &&
					reset_reason != RESET_ROLE_STATUS_REASON_GATHER_SUCC && 
					reset_reason != RESET_ROLE_STATUS_REASON_MOVE &&
					NULL != m_scene->GetSpecialLogic() && 
					!m_scene->GetSpecialLogic()->SpecialStopGahter(this))
				{
					reset_status = false;
				}

				if (reset_status)
				{
					if (NULL != m_scene && Scene::SCENE_TYPE_CROSS_BOSS == m_scene->GetSceneType())
					{
						SpecialCrossBoss *cross_boss = dynamic_cast<SpecialCrossBoss*>(m_scene->GetSpecialLogic());
						if (NULL != cross_boss)
						{
							cross_boss->OnStopGather(this);
						}
					}
					if (NULL != m_scene && Scene::SCENE_TYPE_CROSS_MIZANG_BOSS == m_scene->GetSceneType())
					{
						SpecialCrossMiZangBoss *cross_boss = dynamic_cast<SpecialCrossMiZangBoss*>(m_scene->GetSpecialLogic());
						if (NULL != cross_boss)
						{
							cross_boss->OnStopGather(this);
						}
					}
					if (NULL != m_scene && Scene::SCENE_TYPE_CROSS_YOUMING_BOSS == m_scene->GetSceneType())
					{
						SpecialCrossYouMingBoss *cross_boss = dynamic_cast<SpecialCrossYouMingBoss*>(m_scene->GetSpecialLogic());
						if (NULL != cross_boss)
						{
							cross_boss->OnStopGather(this);
						}
					}

					Obj *obj = m_scene->GetObj(m_gather_obj_id);
					if (NULL != obj && Obj::OBJ_TYPE_GATHER == obj->GetObjType())
					{
						GatherObj *gather_obj = (GatherObj *)obj;
						if (NULL != gather_obj)
						{
							// 重置有人采集中状态
							gather_obj->SetGatherIngObjId(INVALID_OBJ_ID);
						}
					}

					m_gather_obj_id = INVALID_OBJ_ID;
					m_gather_time = 0;
					m_gather_count = 0;

					Protocol::SCStopGather sg;
					sg.role_obj_id = m_obj_id;
					sg.reset_reason = reset_reason;
					m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&sg, sizeof(Protocol::SCStopGather));
				}
			}
			break;

		case ROLE_STATUS_REST:
			{
				m_rest_next_recovery_time = 0;
				m_rest_next_reward_time = 0;
				m_shuangxiu_next_intimacy_time = 0;

				if (this->HasRestPartner())
				{
					Obj *partner_obj = m_scene->GetObj(m_rest_partner_obj_id);
					if (NULL != partner_obj && Obj::OBJ_TYPE_ROLE == partner_obj->GetObjType())
					{
						Role *partner_role = (Role *)partner_obj;

						//if (NULL != partner_role->GetScene() && partner_role->GetScene()->GetSceneID() == GLOBALCONFIG->GetHotSpringSceneID())
						//{
						//	// 温泉双方都终止
						//	partner_role->m_rest_next_recovery_time = 0;
						//	partner_role->m_rest_next_reward_time = 0;
						//	partner_role->m_shuangxiu_next_intimacy_time = 0;

						//	partner_role->SetRestPartner(INVALID_OBJ_ID, 0);

						//	partner_role->m_role_status = ROLE_STATUS_CHARACTER;
						//}

						partner_role->SetRestPartner(INVALID_OBJ_ID, 1); // 普通对方变打坐
					}
				}

				this->SetRestPartner(INVALID_OBJ_ID, 0);
			}
			break;
		}

		if (reset_status)
		{
			m_role_status = ROLE_STATUS_CHARACTER;
		}
	}
}

void Role::SyncSCGulidBossRedbagInfo()
{
	Protocol::SCGulidBossRedbagInfo pro;
	pro.daily_use_guild_relive_times = this->GetDailyUseGuildReliveTimes();
	pro.daily_boss_redbag_reward_fetch_flag = this->GetDailyBossRedbagRewardFetchFlag();
	pro.reverse = 0;

	EngineAdapter::Instance().NetSend(m_netid, (const char*)&pro, sizeof(pro));
}

void Role::OnSyncGuildInfo(GuildID guild_id, const GuildName guild_name, int guild_post)
{
	if (this->IsInCross())
	{
		return;
	}

	int old_guild_id = m_guild_id;
	char old_guild_post = m_guild_post;

	m_guild_id = guild_id;
	m_guild_post = (char)guild_post;
	F_STRNCPY(m_guild_name, guild_name, sizeof(m_guild_name));

	if (INVALID_GUILD_ID != old_guild_id && INVALID_GUILD_ID == m_guild_id)
	{
		this->OnLeaveGuild(old_guild_id); 
	}

	if (INVALID_GUILD_ID == old_guild_id && INVALID_GUILD_ID != m_guild_id)
	{
		this->OnJoinGuild(m_guild_id);
		Guild *guild = GuildManager::Instance().GetGuild(m_guild_id);
		if (NULL != guild)
		{
			guild->SyncGulidReliveTimesInfo(this);
		}
	}

	if (old_guild_id != m_guild_id || old_guild_post != m_guild_post)
	{
		this->NotifyGuildInfoToClient(true);
	}
}

void Role::OnJoinGuild(GuildID guild_id)
{
	m_role_module->role_guild.OnJoinGuild();
	EventHandler::Instance().OnJoinGuild(this, guild_id);
}

void Role::OnLeaveGuild(GuildID old_guild_id)
{
	m_guild_gongxian = 0;
	m_guild_total_gongxian = 0;

	if (NULL != m_scene && Scene::SCENE_TYPE_GUILD_STATION == m_scene->GetSceneType())
	{
		//m_scene->GetSpecialLogic()->DelayKickOutRole(this);
		SpecialGuildStation *special_logic = dynamic_cast<SpecialGuildStation*>(m_scene->GetSpecialLogic());
		if (nullptr != special_logic)
			special_logic->DelayKickOutRole(this);
	}

	m_role_module->role_guild.OnLeaveGuild();
	
	EventHandler::Instance().OnLeaveGuild(this, old_guild_id);
}

void Role::NotifyGuildInfoToClient(bool boardcast)
{
	static Protocol::SCRoleGuildInfoChange rgic;
	rgic.obj_id = m_obj_id;
	rgic.reserved = 0;
	rgic.guild_post = m_guild_post;
	rgic.guild_id = m_guild_id;
	F_STRNCPY(rgic.guild_name, m_guild_name, sizeof(rgic.guild_name));

	rgic.guild_gongxian = m_guild_gongxian;
	rgic.guild_total_gongxian = m_guild_total_gongxian;
	rgic.last_leave_guild_time = m_role_module->role_guild.GetLastLeaveGuildTime();
	
	if (boardcast)
	{
		if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&rgic, sizeof(rgic));
	}
	else 
	{
		EngineAdapter::Instance().NetSend(m_netid, (const char*)&rgic, sizeof(rgic));
	}
}

void Role::SendQuestionRightAnswerNum()
{
	Protocol::SCQuestionRightAnswerNum cmd;
	cmd.world_right_answer_num = m_role_module->common_data_param.world_question_right_num;
	cmd.guild_right_answer_num = m_role_module->common_data_param.guild_question_right_num;

	EngineAdapter::Instance().NetSend(this->GetNetId(), (const char*)&cmd, sizeof(Protocol::SCQuestionRightAnswerNum));
}

void Role::UpdateGather(unsigned int interval)
{
	Obj *gather_obj = m_scene->GetObj(m_gather_obj_id);
	if (NULL == gather_obj || gather_obj->GetObjType() != Obj::OBJ_TYPE_GATHER || ((GatherObj*)gather_obj)->HasGather() || this->IsXuanYun())
	{
		ResetRoleStatus();
		return;
	}

	m_gather_time -= interval;
	if (m_gather_time < 0)
	{
		GatherObj *gather = (GatherObj*)gather_obj;
		gather->Gather(this);

		--m_gather_count;
		if (m_gather_count > 0 && !gather->HasGather())
		{
			m_gather_time = gather->GatherTime(m_user_id);

			Protocol::SCStartGatherTimer sgt;
			sgt.gather_time = (unsigned int)m_gather_time;
			EngineAdapter::Instance().NetSend(m_netid, (const char *)&sgt, sizeof(Protocol::SCStartGatherTimer));
		}
		else
		{
			ResetRoleStatus(INVALID_OBJ_ID, 0, RESET_ROLE_STATUS_REASON_GATHER_SUCC);
		}
	}
}

void Role::UpdateRest(time_t now)
{
	if (NULL == m_scene) return;

	if (now >= m_rest_next_recovery_time)
	{
		m_rest_next_recovery_time = now + GLOBALCONFIG->GetRestRecoveryInterval();

		{
			const int RECOVERY_HP_PER = GLOBALCONFIG->GetRestRecoveryHpPer();
			Attribute max_hp = this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			Attribute change_hp = max_hp * RECOVERY_HP_PER / 100;
			this->ChangeHp(&change_hp, true);
		}
	}

	if (now >= m_rest_next_reward_time)
	{
		m_rest_next_reward_time = now + GLOBALCONFIG->GetRestRewardInterval();

		long long add_exp = 0;
		int add_xianhun = 0, add_yuanli = 0;
		if (!LOGIC_CONFIG->GetRestConfig().GetRestReward(m_level, &add_exp, &add_xianhun, &add_yuanli)) return;

		int shuangxiu_add_percent = 0;
		int lover_add_percent = 0;

		if (this->HasRestPartner())
		{
			shuangxiu_add_percent += GLOBALCONFIG->GetShuangXiuAddPer();
		}

		add_exp += (add_exp * (shuangxiu_add_percent + lover_add_percent) / 100);				// 经验加成 = 双修加成
		this->AddExp(add_exp, "RestUpdate", EXP_ADD_REASON_DEFAULT);

		add_xianhun += (add_xianhun * shuangxiu_add_percent / 100);												// 仙魂加成 = 双修加成
		this->AddXianhun(add_xianhun, "RestUpdate");

		add_yuanli += (add_yuanli * (shuangxiu_add_percent + lover_add_percent) / 100);							// 元力加成 = 双修加成 
		this->AddYuanli(add_yuanli, "RestUpdate");
	}

	if (now >= m_shuangxiu_next_intimacy_time && this->HasRestPartner())
	{
		const int INTIMACY_INTERVAL = GLOBALCONFIG->GetShuangxiuIntimacyInterval();
		const int INTIMACY_ADD = GLOBALCONFIG->GetShuangxiuIntimacyAdd();

		m_shuangxiu_next_intimacy_time = now + INTIMACY_INTERVAL;

		Obj *partner_obj = m_scene->GetObj(m_rest_partner_obj_id);
		if (NULL != partner_obj && Obj::OBJ_TYPE_ROLE == partner_obj->GetObjType())
		{
			Role *partner_role = (Role *)partner_obj;
			
			FriendManager::IntimacyIncrease(this, partner_role, INTIMACY_ADD, "Rest Add", false);

			partner_role->SetShuangxiuNextIntimacyTime(m_shuangxiu_next_intimacy_time);
		}		
	}

}

void Role::UpdateNameColor(time_t now)
{
	const int UPDATE_INTERVAL_S = GLOBALCONFIG->GetNameColorUpdateIntervalS();
	const int UPDATE_DEC_EVIL = -GLOBALCONFIG->GetNameColorUpdateDecEvil();

	if (0 != m_namecolor_last_dec_evil_time && now >= m_namecolor_last_dec_evil_time + UPDATE_INTERVAL_S)
	{
		m_namecolor_last_dec_evil_time = now;

		this->AddEvil(UPDATE_DEC_EVIL);
	}
}

bool Role::IsProfSuitable(char prof)
{
	return PROF_TYPE_PROF_NO_LIMIT == prof || m_prof == prof || (this->GetBaseProf() == prof % PROF_TYPE_PROF_ZHUANZHI_BASE && this->GetProfLevel() >= prof / PROF_TYPE_PROF_ZHUANZHI_BASE);
}

bool Role::IsSexSuitable(char sex)
{
	return SEX_MAX == sex || m_sex == sex;
}

bool Role::SetAuthority(char authority_type, bool save_now)
{ 
	if (authority_type < AUTHORITY_TYPE_INVALID || authority_type >= AUTHORITY_TYPE_MAX)
	{
		return false;
	}
	
	char old_auth = m_authority_type;

	m_authority_type = authority_type;

	if (save_now) this->Save();

	if (AUTHORITY_TYPE_GUIDER != old_auth && AUTHORITY_TYPE_GUIDER == m_authority_type)
	{
		this->NoticeNum(noticenum::NT_NEWHAND_GUILD_SUCC);
	}

	{
		Protocol::SCRoleAuthority ra;
		ra.obj_id = m_obj_id;
		ra.authority_type = m_authority_type;
		EngineAdapter::Instance().NetSend(m_netid, (const char*)&ra, sizeof(ra));
	}

	this->OnRoleBaseInfoChange();

	ROLE_LOG_QUICK6(LOG_TYPE_CHANGE_AUTHORITY_TYPE, this, old_auth, m_authority_type, NULL, NULL);

	return true;
}

void Role::StopAttack()
{
	Character::StopAttack();
}

bool Role::PerformSkill(int skill_index, const Posi &pos, ObjID target_id, bool is_specialskill, int skill_data)
{
	if (!CanCharacterAction(true)) return false;

	if (!m_role_module->role_status_manager.CanPerformSkill())
	{
		return false;
	}

	if (!m_role_module->role_status_manager.CanFight(target_id))
	{
		return false;
	}

	// 检查全局CD
	unsigned long pi_time = ServerLogic::GetInstServerLogic()->GetPiTime();
	if ((m_cooldown_end_time > pi_time && m_cooldown_owe_time + m_cooldown_end_time - pi_time >= 500))
	{
		return false;
	}
	int owe_change_time = (int)((long long)m_cooldown_end_time - (long long)pi_time);

	if (is_specialskill) // 1：如果玩家是特殊技能
	{
		m_scene->GetSpecialLogic()->PerformSkill(this, skill_index, pos, target_id);

		this->IncAttackSeq();
	}
	else 
	{
		// 不知道为什么沉默的时候要自动转换技能 2019-02-27
		// 修改：暂时先实现策划的需求，沉默时不能释放普通攻击以外的攻击。
		// 保留：魔法不够的时候转为普通攻击，其实现在没啥用
		//if (this->IsChenMo() ||
		//	(m_scene->GetSpecialLogic()->SpecialIsCostMp(this) && this->GetSkillManager()->GetSkillCostMp(skill_index) > m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP)))
		//{
		//	skill_index = this->GetSkillManager()->GetDefaultSkill();
		//}

		if (this->IsChenMo())
		{
			this->NoticeNum(errornum::EN_ROLE_FIGHT_CHENMO_STATUS);
			return false;
		}

		if ((m_scene->GetSpecialLogic()->SpecialIsCostMp(this) && this->GetSkillManager()->GetSkillCostMp(skill_index) > m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP)))
		{
			// 没有魔法，则只能使用普通技能
			skill_index = this->GetSkillManager()->GetDefaultSkill();
		}

		if (!m_scene->GetSpecialLogic()->SpecialCanPerformCommonSkill(this))
		{
			return false;
		}

		//if (m_effect_list.IsInvisible())											// 隐身不能释放角色技能
		//{
		//	return false;
		//}
		// 2019-04-13 隐身时释放技能打断隐身状态
		if (m_effect_list.IsInvisible())
		{
			m_effect_list.FindAndRemoveEffect(EffectBase::EFFECT_TYPE_INVISIBLE, EBT_INVISIBLE, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_SKILL, 0);
		}

		if (Character::PerformSkill(skill_index, pos, target_id, -1, 0, skill_data))// 2：处理普通技能
		{
			m_cooldown_owe_time += owe_change_time;
			if (m_cooldown_owe_time < 0) m_cooldown_owe_time = 0;

			ResetRoleStatus();
		}
	}

	return true;
}

void Role::OnKillOtherRole(Role * target_role)
{
	m_role_module->talent.OnKillOtherRole(target_role);
	m_role_module->role_zhuanzhi.OnKillOtherRole(target_role);
	m_role_module->shenzhou_weapon_manger.OnKillOtherRole(target_role);
}

void Role::OnDie(ObjID killer_objid)
{
	this->ResetRoleStatus();
	this->ClearFightBuffEffect(EffectBase::CLEAR_REASON_DIE);

	m_role_module->multi_mount.MountOn(false); // 双人坐骑下马

	Character::OnDie(killer_objid);

	m_last_kill_me_user_info.Reset();

	Role *killer_role = m_scene->GetPrincipal(killer_objid);
	if (NULL != killer_role)
	{
		m_role_module->shen_zhuang.OnDie(killer_role);
		EventHandler::Instance().OnKillRole(this, killer_role);
	}

	if (m_scene->InStaticScene() || Scene::SCENE_TYPE_BABY_BOSS == m_scene->GetSceneType())
	{
		Role *killer_role = m_scene->GetPrincipal(killer_objid);
		if (NULL != killer_role)
		{
			// 神器-神兵触发特殊形象效果
			this->GetShenqi()->OnDie(killer_role);

			if (m_role_module->role_status_manager.CanRecordLastKillMeUserInfo())
			{
				m_last_kill_me_user_info.user_id = killer_role->GetUserId(); killer_role->GetName(m_last_kill_me_user_info.user_name); m_last_kill_me_user_info.camp = killer_role->GetCamp();
			}

			// 保证调用顺序，先调用被杀者，在调用击杀者
			m_role_module->friend_manager.AddEnemy(killer_role->GetUniqueUserID(), true, "Be Kill Add Enemy");
			killer_role->GetFriendManager()->AddEnemy(this->GetUniqueUserID(), false, "Kill Add Enemy");

			//if (!GLOBALCONFIG->IsRedNameColor(m_evil) && ATTACK_MODE_PEACE == m_attack_mode &&  // killer杀了本国非红名和平模式下的玩家
			//	!HuangchenghuizhanMgr::Instance().IsInHuangChengHuizhan(this) &&				// 在皇城活动中杀人不加罪恶值
			//	!CampDefendMgr::Instance().IsInCampDefendingScene(this))						// 在雕象守卫战中杀人	不加罪恶值	
			//{
			//	int level_interval = killer_role->GetLevel() - m_level; 
			//	if (level_interval < 40) level_interval = 40;
			//	int kill_add_evil = static_cast<int>((level_interval - 40) * 0.1f + 1) * 10;
			//	killer_role->AddEvil(kill_add_evil);
			//}

			if (!GLOBALCONFIG->IsRedNameColor(m_evil) && ATTACK_MODE_PEACE == m_attack_mode &&  // killer杀了本国非红名和平模式下的玩家
				!FightBackManager::Instance().IsFightBackObj(killer_role, this) &&				// 如果本人是killer反击对象，killer不加罪恶值
				!HuangchenghuizhanMgr::Instance().IsInHuangChengHuizhan(this) &&				// 在皇城活动中杀人不加罪恶值
				!CampDefendMgr::Instance().IsInCampDefendingScene(this))						// 在雕象守卫战中杀人	不加罪恶值	
			{
				int level_interval = killer_role->GetLevel() - m_level;

				if (level_interval >= 50)														//策划需求，等级差50级以内不加罪恶值
				{
					int kill_add_evil = static_cast<int>((level_interval - 50) * 0.1f + 1) * 10;

					killer_role->AddEvil(kill_add_evil);
				}
			}

			{
				GuildName killer_guild_name = {0};
				killer_role->GetGuildName(killer_guild_name);

				//公会成员被杀通告
				{
					RoleGuildParam * role_guild_param = this->GetRoleGuild()->GetRoleGuildParam();
					if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) > role_guild_param->be_kill_last_notice_time + 60)
					{
						role_guild_param->be_kill_last_notice_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
						Guild *guild = GuildManager::Instance().GetGuild(m_guild_id);
						if (NULL != guild)
						{
							int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_guild_memberkilled, 
								UidToInt(this->GetUserId()), (const char*)this->GetName(), (int)this->GetCamp(), 
								killer_role->GetGuildID(), killer_guild_name, (int)killer_role->GetCamp(),
								UidToInt(killer_role->GetUserId()), (const char*)killer_role->GetName(), (int)killer_role->GetCamp(),
								m_scene->GetSceneName(), this->GetPos().x, this->GetPos().y, m_scene->GetSceneID(), m_scene->GetSceneKey(), killer_role->IsInCross() ? 1 : 0);

							if (length > 0)
							{
								static Protocol::SCSystemMsg sm;
								int sendlen = 0;
								if (World::GetInstWorld()->GetSceneManager()->CreateSystemMsg(&sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_ONLY_CHAT_GUILD))
								{
									guild->SendMessageToGuild(Protocol::GUILD_NOTIFY_TYPE_MEMBER_SOS, (const char *)&sm, sendlen);	
								}
							}
						}
					}
				}
			}

			{
				JingHuaHuSongMgr::Instance().OnRoleDie(this, killer_role);
			}

			// 传闻
			{
				static const unsigned int KILL_ME_BROADCAST_INTERVAL = 180;
				unsigned int now_sce = static_cast<unsigned int>(EngineAdapter::Instance().Time());
				if (now_sce > m_last_kill_me_time + KILL_ME_BROADCAST_INTERVAL)
				{
					int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_be_killed_by_role_in_static_scene,
						UidToInt(m_last_kill_me_user_info.user_id), m_last_kill_me_user_info.user_name, (int)m_last_kill_me_user_info.camp,
						m_uid, m_name, (int)m_camp);
					if (length > 0) 
					{
						World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
					}
					
					m_last_kill_me_time = now_sce;
				}
			}

			CampDefendMgr::Instance().OnRoleDie(this, killer_role);
		}

		{
			m_role_module->husong_task.OnDie(killer_objid);
			m_role_module->role_activity.OnDie(killer_objid);
		}
	}
	else if (Scene::SCENE_TYPE_CROSS_BOSS == m_scene->GetSceneType() ||
			 Scene::SCENE_TYPE_CROSS_MIZANG_BOSS == m_scene->GetSceneType() ||
			 Scene::SCENE_TYPE_GODMAGIC_BOSS == m_scene->GetSceneType())
	{
		// 策划需求2019-04-12，这几个BOSS里击杀要算仇人
		// 保证调用顺序，先调用被杀者，在调用击杀者
		Role *killer_role = m_scene->GetPrincipal(killer_objid);
		if (NULL != killer_role)
		{
			m_role_module->friend_manager.AddEnemy(killer_role->GetUniqueUserID(), true, "Be Kill Add Enemy");
			killer_role->GetFriendManager()->AddEnemy(this->GetUniqueUserID(), false, "Kill Add Enemy");
		}
	}
	else
	{
		static ObjID obj_list[256] = {0};
		static const int MONSTER_MAX_AOI_RANGE = MONSTERPOOL->GetMonsterMaxAoiRange();

		int count = m_scene->GetZoneMatrix()->GetObjByArea(m_posi, MONSTER_MAX_AOI_RANGE, MONSTER_MAX_AOI_RANGE, obj_list, sizeof(obj_list) / sizeof(obj_list[0]));
		for (int i = 0; i < count; i++)
		{
			Obj *obj = m_scene->GetObj(obj_list[i]);
			if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
			{
				Monster *monster = (Monster*)obj;
				monster->OnNearRoleDie(m_obj_id, m_posi);
			}
		}
	}

	if (Scene::IsBossScene(m_scene->GetSceneID()))
	{
		//Role *killer_role = m_scene->GetPrincipal(killer_objid);
		if (NULL != killer_role)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_be_killed_in_boss_scene,
				UidToInt(m_last_kill_me_user_info.user_id), m_last_kill_me_user_info.user_name, (int)m_last_kill_me_user_info.camp,
				m_uid, m_name, (int)m_camp);
			if (length > 0) 
			{
				int sendlen = 0;
				static Protocol::SCSystemMsg boss_scene_sm;

				if (SceneManager::CreateSystemMsg(&boss_scene_sm, &sendlen, gamestring::GAMESTRING_BUF, length, SYS_MSG_ACTIVE_NOTICE))
				{
					m_scene->SendToRole((const char*)&boss_scene_sm, sendlen);
				}
			}
		}
	}
	if (Scene::IsDabaoBossScene(m_scene->GetSceneID()))
	{
		BossFamilyManager::Instance().OnRoleDie(Protocol::BOSS_ENTER_TYPE_BOSS_DABAO, this);
	}

	if (Scene::IsBossDongKuScene(m_scene->GetSceneID()))
	{
		this->DieOnMiKuScene(true);
	}

	if (Scene::IsBossFamilyScene(m_scene->GetSceneID()))
	{
		if (NULL != killer_role)
		{
			ActivityShadow::Instance().OnCrossAddGuildEnemy(this, killer_role);
		}
	}

	if (m_first_hit_flag)
	{
		m_first_hit_have_died = true;
	}

	m_role_module->huguozhili.OnRoleDie();

	ActivityMsgManager::Instance().OnRoleDie(m_scene, this, killer_objid);
	FightBackManager::Instance().OnRoleDie(this);
}

void Role::SendBeKillMail()
{
	{
		// 被杀提醒邮件
		static MailSubject mail_subject = { 0 };
		static MailContentTxt mail_content_txt = { 0 };

		int length1 = SNPRINTF(mail_subject, sizeof(mail_subject), gamestring::g_bekilled_notice_subject);
		int length2 = SNPRINTF(mail_content_txt, sizeof(mail_content_txt), gamestring::g_bekilled_notice_content, UidToInt(m_last_kill_me_user_info.user_id), m_last_kill_me_user_info.user_name, m_last_kill_me_user_info.camp);

		if (length1 > 0 && length2 > 0)
		{
			if (m_last_kill_me_user_info.user_id != INVALID_USER_ID)
			{
				MailRoute::Instance().MailToUser(m_user_id, mail_subject, mail_content_txt);
			}
		}
	}
}

// 受击，此时角色尚未被扣血，可以在此进行伤害修正
void Role::OnAttackCalcHurt(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill, int fight_type)
{
	if (m_tick_attack_recal_count++ > 100)
	{
		// log
		return;
	}
	// 触发进入战斗状态
	Obj *attacker_obj = m_scene->GetObj(attacker);
	if (NULL != attacker_obj/* && Obj::OBJ_TYPE_ROLE == attacker_obj->GetObjType()*/)
	{
		this->OnStartFight();
	}

	ResetRoleStatus(attacker, delta_hp);

	if (m_scene->GetSpecialLogic()->SpecialCanPassiveSkillEffect(this, attacker, skill_id))
	{
		m_role_module->wing.OnAttack(attacker, skill_id, delta_hp, from_skill);
		m_role_module->shenyi.OnAttack(attacker, skill_id, delta_hp, from_skill);
		m_role_module->cloak_manager.OnAttack(attacker, skill_id, delta_hp, from_skill);
		m_role_module->shenzhou_weapon_manger.OnAttack(attacker, skill_id, delta_hp, from_skill);
		m_role_module->yaoshi_manager.OnAttack(attacker, skill_id, delta_hp, from_skill);
		m_role_module->qilinbi_manager.OnAttack(attacker, skill_id, delta_hp, from_skill);
		m_role_module->zhuanzhi_equip.OnAttack(attacker, skill_id, delta_hp, from_skill);
		m_role_module->mask_manager.CheckOnAttack();
		m_role_module->mojie_skill.OnAttackCalcHurt(attacker, delta_hp, fight_type);
		m_role_module->role_zhuanzhi.GetRoleZhuanZhiSkill().OnAttack(attacker, skill_id, delta_hp, from_skill);
		m_role_module->mount_manager.OnAttack(attacker, skill_id, delta_hp, from_skill);
		m_role_module->shizhuang.OnAttack(attacker, skill_id, delta_hp, from_skill);
		m_role_module->jingling_mgr.OnAttack(attacker, skill_id, delta_hp, from_skill);

		// 其他系统
		m_role_other_mgr->OnAttackCalcHurt(attacker, delta_hp);

		// 最后调用天神护体，它会限制最大伤害
		m_role_module->huguozhili.OnAttackCalcHurt(delta_hp, attacker);
	}

	Character::OnAttackCalcHurt(attacker, skill_id, delta_hp, from_skill, fight_type);
}

// 受击，此时角色已被扣血
void Role::OnAttack(ObjID attacker, UInt16 skill_id, Attribute delta_hp, bool from_skill)
{
	if (m_tick_attack_count++ > 100)
	{
		//log
		return;
	}

	Character::OnAttack(attacker, skill_id, delta_hp, from_skill);

	m_role_module->xiannv_manager.OnAttack(attacker, delta_hp);
	m_role_module->shenshou.OnRoleBeenAttack(attacker);
	m_role_module->img_fuling.OnAttack(delta_hp);
	m_role_module->talent.OnRoleBeenAttack(attacker, skill_id, delta_hp, from_skill);
	m_role_module->jinjie_talent.OnAttack(attacker, skill_id, delta_hp, from_skill);
	m_role_module->role_zhuanzhi.GetRoleZhuanZhiSkill().OnRoleBeenAttack(attacker, skill_id, delta_hp, from_skill);
	//m_role_module->fabao.OnRoleBeenAttack(attacker);
	m_role_module->footprint.OnAttack(attacker, skill_id, delta_hp, from_skill);
	m_role_module->fightmount_manager.OnAttack(attacker, skill_id, delta_hp, from_skill);

	//for (IRoleModule *module : m_role_module_list)
	//{
	//	module->OnAttack(attacker, skill_id, delta_hp);
	//}

	// other
	this->AddFightBackRole(attacker);
}

// 进入战斗
void Role::OnStartFight()
{
	// 修改战斗状态，记录最后一次的战斗时间
	m_role_module->role_status_manager.OnFight();
	m_role_module->shenge_system.OnStartFight();
}

void Role::OnStopFight()
{
	m_role_module->shenge_system.OnStopFight();
}

// 攻击，伤害他人
int Role::OnHurtOther(Character *target, int skill_id, long long &injure, int fight_type, int target_index)
{
	// 基础伤害值，用于系统的加成计算
	long long base_injure = injure;
	int passive_flag = Character::OnHurtOther(target, skill_id, injure, fight_type, target_index);

	if (m_scene->GetSpecialLogic()->SpecialCanPassiveSkillEffect(this, target->GetId(), skill_id))
	{
		//魔戒技能放在最前
		m_role_module->mojie_skill.OnHurtOther(target, injure, fight_type, target_index);

		//m_role_module->mount_manager.OnHurtOther(target, skill_id, base_injure, injure, target_index, passive_flag);
		//m_fightmount_manager.OnHurtOther(target, skill_id, base_injure, injure, target_index, passive_flag);
		m_role_module->shenyi.OnHurtOther(target, skill_id, base_injure, injure, target_index, passive_flag);
		m_role_module->shengong.OnHurtOther(target, skill_id, base_injure, injure, target_index, passive_flag);
		m_role_module->cloak_manager.OnHurtOther(target, skill_id, base_injure, injure, target_index, passive_flag);
		m_role_module->zhibao_manager.OnHurtOther(target, skill_id, base_injure, injure, target_index, passive_flag);
		m_role_module->mojie_fb.OnHurtOther(target, skill_id, base_injure, injure, target_index, passive_flag);
		m_role_module->shenbing_manager.OnHurtRole(target);
		m_role_module->shenge_system.OnHurtOther(target, base_injure, injure);
		m_role_module->shenzhou_weapon_manger.OnHurtOther(target, skill_id, base_injure, injure, target_index, passive_flag);
		m_role_module->xiannv_manager.OnHurtOther(target, base_injure, injure);
		m_role_module->jingling_mgr.OnHurtOther(target, skill_id, base_injure, injure, target_index, passive_flag);
		m_role_module->shenshou.OnHurtOther(target, base_injure);
		m_role_module->img_fuling.OnHurtOther(target, base_injure, injure, target_index);
		m_role_module->jingjie.OnHurtOther(target, skill_id, base_injure, injure, fight_type, target_index);
		m_role_module->talent.OnHurtOther(target, skill_id, base_injure, fight_type);
		m_role_module->jinjie_talent.OnHurtOther(target, base_injure);
		m_role_module->role_zhuanzhi.GetRoleZhuanZhiSkill().OnHurtOther(target, skill_id, base_injure, fight_type);
		m_role_module->zhuanzhi_equip.OnHurtOther(target, base_injure, injure);
		m_role_module->shizhuang.OnHurtOther(target, base_injure, injure);
		m_role_module->fabao.OnHurtOther(target, skill_id, base_injure, injure);
		m_role_module->halo.OnHurtOther(target, skill_id, base_injure, injure);
		m_role_module->mask_manager.OnHurtOther(target, skill_id, injure, target_index, passive_flag);
		m_role_module->toushi_manager.OnHurtOther(target, skill_id, base_injure, injure, target_index, passive_flag);
		m_role_module->wing.OnHurtOther(target, skill_id, base_injure, injure);
		m_role_module->mojie_manager.OnHurtOther(target, skill_id, base_injure, injure, target_index, passive_flag);
		// 在这里添加
		
		// 其他系统
		m_role_other_mgr->OnHurtOther(target, skill_id, injure, target_index, passive_flag);

		m_role_module->upgrade_manager.GetUpgradeSkill().OnHurtOther(target, skill_id, base_injure, fight_type);

		//for (IRoleModule *module : m_role_module_list)
		//{
		//	module->OnHurtOther(target, skill_id, injure);
		//}
		//魔戒技能对boss增伤放在最后
		m_role_module->mojie_skill.OnHurtBoss(target, injure);
	}

	// 世界事件
	if (Obj::OBJ_TYPE_MONSTER == target->GetObjType())
	{
		Monster *monster = (Monster *)target;

		WorldEventManager::Instance().OnHurtMonster(this, monster, base_injure);
	}
	else if (Obj::OBJ_TYPE_ROLE_SHADOW == target->GetObjType())
	{
		RoleShadow *role_shadow = (RoleShadow *)target;

		WorldEventManager::Instance().OnHurtRoleShadow(this, role_shadow, base_injure);
	}

	if (m_next_attack_extra_hurt_percent > 0)
	{
		Attribute target_max_hp = target->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		Attribute hurt = (Attribute)(target_max_hp * (m_next_attack_extra_hurt_percent / 100.0));
		EffectOnce *effect = new EffectOnce(this->GetId(), 0, -hurt, fight_type, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
		target->AddEffect(effect);

		m_next_attack_extra_hurt_percent = 0;
	}

	// 个人目标吸血
	this->GetRoleGoal()->OnAbsordBlood(base_injure);

	// 检查effect  放在最后
	m_effect_list.OnHurtOther(target, skill_id, base_injure);

	return passive_flag;
}

void Role::OnAfterHurtOther(Character *target, int skill_id, long long injure, bool from_skill, int fight_type)
{
	m_role_module->upgrade_manager.GetUpgradeSkill().OnAfterHurtOther(target, skill_id, injure, from_skill, fight_type);
}

void Role::CheckFakePrivateChat(time_t now_second)
{
	const static int FAKE_PRIVATE_CHAT_MAX_TIMES = 4;
	const static int FAKE_PRIVATE_CHAT_MAX_LEVEL = 40;
	const static int FAKE_PRIVATE_CHAT_MIN_LEVEL = 20;

	if (m_role_module->common_data_param.fake_private_chat_times < FAKE_PRIVATE_CHAT_MAX_TIMES && 
		m_level <= FAKE_PRIVATE_CHAT_MAX_LEVEL && m_level >= FAKE_PRIVATE_CHAT_MIN_LEVEL &&
		now_second >= m_role_module->common_data_param.next_fake_private_chat_timestamp)
	{
		m_role_module->common_data_param.next_fake_private_chat_timestamp = static_cast<unsigned int>(now_second + FAKE_PRIVATE_CHAT_INTERVAL_S);

		static int rand_role_list[SCENE_MANAGER_RANDOM_GET_ROLE_MAX_COUNT]; memset(rand_role_list, 0, sizeof(rand_role_list));					
		int rand_role_count = World::GetInstWorld()->GetSceneManager()->RandomGetRoleList(rand_role_list);	
		
		for (int i = 0; i < rand_role_count; ++ i)
		{
			if (rand_role_list[i] != m_uid)
			{
				UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(rand_role_list[i]);
				if (NULL != user_node && user_node->level <= 45)
				{
					static Protocol::SCFakePrivateChat fpc;
					fpc.from_uid = rand_role_list[i];
					user_node->GetName(fpc.username);
					fpc.sex = user_node->sex;
					fpc.camp = user_node->camp;
					fpc.vip_level = user_node->vip_level;
					fpc.prof = user_node->prof;
					fpc.level = user_node->level;
					fpc.avatar_timestamp = user_node->avatar_timestamp;
					EngineAdapter::Instance().NetSend(m_netid, (const char *)&fpc, sizeof(fpc)); 

					++ m_role_module->common_data_param.fake_private_chat_times;

					return;
				}
			}
		}
	}
}

bool Role::UseGoldCard(int gold_num)
{
	if (this->GetKnapsack()->GetMoney()->AddGold(gold_num, "GoldCard"))
	{
		EventHandler::Instance().OnRMBBuy(this, gold_num);
		EventHandler::Instance().OnAddChongzhi(this, gold_num);
		this->GetRoleActivity()->CheckChongzhiExtraRewardByNum(gold_num);
		this->SetAuthority(AUTHORITY_TYPE_TEST);
		return true;
	}
	
	return false;
}

bool Role::ReAlive(int hp_per, int mp_per, Posi realive_pos, bool is_immediately)
{
	if (this->IsAlive()) return false;

	Attribute blood = (Attribute)(m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * (hp_per / 100.0f));
	Attribute delta_blood = blood - m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	ChangeHp(&delta_blood, true, CharIntAttrs::ATTR_NOTIFY_REASON_NORMAL, true);

	/*if (0 != mp_per)
	{
		Attribute mp_to = (Attribute)(m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP) * (mp_per / 100.0f));
		Attribute cur_mp = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP);
		if (mp_to > cur_mp)
		{
			Attribute delta_mp = mp_to - cur_mp;
			ChangeMp(&delta_mp, true);
		}
	}*/

	m_dead_time = 0;

	if (realive_pos.x != -1 || realive_pos.y != -1) // 这里不必用ResetPost因为下面有重生消息
	{
		Move(realive_pos.x, realive_pos.y, Dir_S, 0, 0, false);
	}
	
	if (NULL != m_scene)
	{
		Protocol::SCRoleReAlive rra;
		rra.obj_id = m_obj_id;
		rra.pos_x = m_posi.x;
		rra.pos_y = m_posi.y;
		m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void*)&rra, sizeof(Protocol::SCRoleReAlive));

		if (is_immediately)
		{
			static ObjID obj_list[1024] = {0};
			static const int MONSTER_MAX_AOI_RANGE = MONSTERPOOL->GetMonsterMaxAoiRange();

			int count = m_scene->GetZoneMatrix()->GetObjByArea(m_posi, MONSTER_MAX_AOI_RANGE, MONSTER_MAX_AOI_RANGE, obj_list, 1024);
			for (int i = 0; i < count; i++)
			{
				Obj *obj = m_scene->GetObj(obj_list[i]);
				if (NULL != obj && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
				{
					Monster *monster = (Monster*)obj;
					monster->OnNearRoleRealive(m_obj_id, m_posi);
				}
			}
		}
	}

	this->GetShenqi()->OnRealive();
	return true;
}

bool Role::ReAliveBackHome()
{
	Posi pos(0, 0);
	int target_scene_id = 0;

	if (!this->GetRoleStatusRealivePos(&target_scene_id, &pos))
	{
		target_scene_id = m_scene->GetTownPoint(this, &pos);
	}
	// 复活时检查神器是否改变角色形象
	this->GetShenqi()->OnRealive();

	return this->ReAliveTargetScene(target_scene_id, pos);
}

bool Role::ReAliveTargetScene(int scene_id, const Posi &pos)
{
	if (this->IsAlive()) return false;

	const int REALIVE_RECOVER_COMMON_SCENE_PERCENT = GLOBALCONFIG->GetRealiveRecoverCommonScenePercent();
	const int REALIVE_RECOVER_FB_PERCENT = GLOBALCONFIG->GetRealiveRecoverFbPercent();

	if (scene_id == m_scene->GetSceneID())
	{
		if (HuangchenghuizhanMgr::Instance().IsInHuangChengHuizhan(this))
		{
			ReAlive(50, 50, pos);
		}
		else if (m_scene->InStaticScene())
		{
			ReAlive(REALIVE_RECOVER_COMMON_SCENE_PERCENT, REALIVE_RECOVER_COMMON_SCENE_PERCENT, pos);
		}
		else
		{
			ReAlive(REALIVE_RECOVER_FB_PERCENT, REALIVE_RECOVER_FB_PERCENT, pos);
		}
	}
	else
	{
		Attribute blood = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP) * REALIVE_RECOVER_COMMON_SCENE_PERCENT / 100;
		Attribute delta_blood = blood - m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
		ChangeHp(&delta_blood, true, CharIntAttrs::ATTR_NOTIFY_REASON_NORMAL, true);

		/*Attribute mp = m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP) * REALIVE_RECOVER_COMMON_SCENE_PERCENT / 100;
		Attribute delta_mp = mp - m_int_attrs.GetAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP);
		if (delta_mp > 0)
		{
			ChangeMp(&delta_mp);
		}*/

		m_dead_time = 0;

		return World::GetInstWorld()->GetSceneManager()->GoTo(this, scene_id, -1, pos);
	}

	return true;
}

bool Role::ForceReturnReAlivePosi()
{
	if (nullptr == m_scene)
	{
		return false;
	}

	Posi pos(0, 0);
	int target_scene_id = 0;

	if (!this->GetRoleStatusRealivePos(&target_scene_id, &pos))
	{
		target_scene_id = m_scene->GetTownPoint(this, &pos);
	}

	{
		// log
		gamelog::g_log_scenestatus.printf(LL_WARNING,
			"Role::ForceReturnReAlivePosi() role(%d, %s), sceneId[%d], target_sceneId[%d], pos[%d,%d]",
			this->GetUID(), this->GetName(), m_scene->GetSceneID(), target_scene_id, (int)pos.x, (int)pos.y);
	}

	// set pos
	if (target_scene_id == m_scene->GetSceneID())
	{
		this->ResetPos(pos.x, pos.y);
	}
	else
	{
		World::GetInstWorld()->GetSceneManager()->GoTo(this, target_scene_id, -1, pos);
	}

	return true;
}

void Role::SetAttackMode(int attack_mode, bool is_force, bool is_fanji)
{
	if (attack_mode < ATTACK_MODE_PEACE || attack_mode >= ATTACK_MODE_MAX || attack_mode == m_attack_mode) return;

	if (attack_mode == ATTACK_MODE_SAME_SERVER && !CrossConfig::Instance().IsHiddenServer()) return;

	Protocol::SCSetAttackMode sam;
	sam.obj_id = m_obj_id;
	sam.result = Protocol::SET_ATTACK_MODE_SUCC;
	sam.attack_mode = attack_mode;
	sam.last_peace_mode_time = 0;

	if (!is_force && !m_role_module->role_status_manager.CanChangeAttackMode(attack_mode))
	{
		sam.result = Protocol::SET_ATTACK_MODE_STATUS_LIMIT;
		EngineAdapter::Instance().NetSend(this->GetNetId(), (const char *)&sam, sizeof(sam));
		return;
	}

	const int XINSHOU_PROTECTED_LEVEL = GLOBALCONFIG->GetRoleXinShouProtectedLevel();
	if (m_level < XINSHOU_PROTECTED_LEVEL)
	{
		sam.result = Protocol::SET_ATTACK_MODE_PROTECT_LEVEL;
		EngineAdapter::Instance().NetSend(this->GetNetId(), (const char *)&sam, sizeof(sam));
		return;
	}

	char old_attack_mode = m_attack_mode;

	m_attack_mode = attack_mode;

	switch(m_attack_mode)
	{
	case ATTACK_MODE_PEACE:
		{
			this->NoticeNum(noticenum::NT_SETATTACKMODE_PEACE);
		}
		break;

	case ATTACK_MODE_TEAM:
		{
			this->NoticeNum(noticenum::NT_SETATTACKMODE_TEAM);
		}
		break;

	case ATTACK_MODE_GUILD:
		{
			this->NoticeNum(noticenum::NT_SETATTACKMODE_GUILD);
		}
		break;
	
	case ATTACK_MODE_ALL:
		{
			if (is_fanji && ATTACK_MODE_PEACE == old_attack_mode)
			{
				this->NoticeNum(noticenum::NT_ATTACK_MODE_FANJI);
			}
			else
			{
				this->NoticeNum(noticenum::NT_SETATTACKMODE_ALL);
			}	
		}
		break;

	case ATTACK_MODE_NAMECOLOR:
		{
			this->NoticeNum(noticenum::NT_SETATTACKMODE_NAMECOLOR);
		}
		break;

	case ATTACK_MODE_CAMP:
		{
			this->NoticeNum(noticenum::NT_SETATTACKMODE_CAMP);
		}
		break;

	case ATTACK_MODE_SAME_SERVER:
		{
			gstr::SendNotice(this, "setattack_mode_same_server");
		}
		break;

	case ATTACK_MODE_ENEMY:
		{
			gstr::SendNotice(this, "setattack_mode_enemy");
		}
		break;
	}
	
	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void*)&sam, sizeof(sam));
}

void Role::ResetGMChangeAttribute()
{
	m_gm_change_maxhp = 0;
	m_gm_change_gongji = 0;
	m_gm_change_movespeed = 0;
	m_gm_change_fangyu = 0;
	m_gm_change_mingzhong = 0;
	m_gm_change_shanbi = 0;
	m_gm_change_baoji = 0;
	m_gm_change_jianren = 0;
	m_gm_change_fujia_shanghai = 0;
	m_gm_change_dikang_shanghai = 0;
	m_gm_change_per_jingzhun = 0;
	m_gm_change_per_baoji = 0;
	m_gm_change_per_kangbao = 0;
	m_gm_change_per_pofang = 0;
	m_gm_change_per_mianshang = 0;
	m_gm_change_constant_zengshang = 0;
	m_gm_change_constant_mianshang = 0;

	this->ReCalcAttr(CharIntAttrs::RECALC_REASON_INVALID, true);
	this->SendAttributeInfo();
}

void Role::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	{ 
		int pass_day_num = (m_offline_day_num > 0) ? m_offline_day_num : 1;		// 离线天数

		m_lastday_online_time = (pass_day_num > 1) ? 0 : m_today_online_time;
		m_offline_day_num = 0;

		m_today_online_time = 0;
		m_last_today_online_stati_time = EngineAdapter::Instance().Time(); 
	}

	m_role_module->role_activity.SendServerTime();
	m_role_module->month_card.OnDayChange(old_dayid, now_dayid);
	m_role_module->welfare.OnDayChange(old_dayid, now_dayid);
	m_role_module->qingyuan.OnDayChange(old_dayid, now_dayid);
	m_role_module->baby.OnDayChange(old_dayid,now_dayid);
	m_role_module->active_degree.OnDayChange(old_dayid, now_dayid);
	m_role_module->role_cross.OnDayChange(old_dayid, now_dayid);
	m_role_module->role_guild.OnDayChange(old_dayid, now_dayid);
	m_role_module->pet.OnDayChange(old_dayid, now_dayid);
	m_role_module->magic_card.OnDayChange(old_dayid, now_dayid);
	m_role_module->zhuansheng_equip.OnDayChange(old_dayid, now_dayid);
	m_role_module->gold_vip.OnDayChange(old_dayid, now_dayid);
	m_role_module->role_goal.OnDayChange(old_dayid, now_dayid);
	m_role_module->vip.OnDayChange(old_dayid, now_dayid);
	m_role_module->role_evaluate.OnDayChange(old_dayid, now_dayid);
	m_role_module->friend_manager.OnDayChange(old_dayid, now_dayid);
	m_role_module->friend_gift_manager.OnDayChange(old_dayid, now_dayid);
	m_role_module->monitor.OnDayChange(old_dayid, now_dayid);
	m_role_module->sublock.OnDayChange(old_dayid, now_dayid);
	m_role_module->roleshop.OnDayChange(old_dayid, now_dayid);
	m_role_module->zhanshendian.OnDayChange(old_dayid, now_dayid);
	m_role_module->shengwang_mgr.OnDayChange(old_dayid, now_dayid);

	m_role_module->daycounter.OnDayChange(old_dayid, now_dayid);				// 需要oldday数据的放在OnDayChange前面，需要newday数据放在后面

	m_role_module->role_phase_fb.OnDayChange(old_dayid, now_dayid);
	m_role_module->dailyfb.OnDayChange(old_dayid, now_dayid);
	m_role_module->vipfb.OnDayChange(old_dayid, now_dayid);
	m_role_module->patafb.OnDayChange(old_dayid, now_dayid);
	m_role_module->story_fb.OnDayChange(old_dayid, now_dayid);
	m_role_module->mount_manager.OnDayChange(old_dayid, now_dayid);
	m_role_module->fightmount_manager.OnDayChange(old_dayid, now_dayid);
	m_role_module->daily_task.OnDayChange(old_dayid, now_dayid);
	m_role_module->equip_fb.OnDayChange(old_dayid, now_dayid);
	m_role_module->maze.OnDayChange(old_dayid, now_dayid);
	m_role_module->guild_task.OnDayChange(old_dayid, now_dayid);
	m_role_module->challenge_fb.OnDayChange(old_dayid, now_dayid);
	m_role_module->convert_record.OnDayChange(old_dayid, now_dayid);
	m_role_module->cardzu.OnDayChange(old_dayid, now_dayid);
	m_role_module->task_recorder.OnDayChange(old_dayid, now_dayid);
	m_role_module->wabao.OnDayChange(old_dayid, now_dayid);
	m_role_module->cisha_task.OnDayChange(old_dayid, now_dayid);
	m_role_module->roll_money.OnDayChange(old_dayid, now_dayid);
	m_role_module->role_world_event.OnDayChange(old_dayid, now_dayid);
	m_role_module->paohuan_task.OnDayChange(old_dayid, now_dayid);
	m_role_module->gongxun_mgr.OnDayChange(old_dayid, now_dayid);
	m_role_module->chengjiu_mgr.OnDayChange(old_dayid, now_dayid);
	m_role_module->shenge_system.OnDayChange(old_dayid, now_dayid);
	m_role_module->upgrade_manager.OnDayChange(old_dayid, now_dayid);

	m_role_module->towerdefend_fb.OnDayChange(old_dayid, now_dayid);
	m_role_module->runesystem_manger.OnDayChange(old_dayid, now_dayid);
	m_role_module->shenzhou_weapon_manger.OnDayChange(old_dayid, now_dayid);
	m_role_module->xiannv_manager.OnDayChange(old_dayid, now_dayid);
	m_role_module->tuitu_fb.OnDayChange(old_dayid, now_dayid);
	m_role_module->jingling_mgr.OnDayChange(old_dayid, now_dayid);
	m_role_module->precious_boss.OnDayChange(old_dayid, now_dayid);
	m_role_module->shen_zhuang.OnDayChange(old_dayid, now_dayid);
	//m_role_module->element_heart.OnDayChange(old_dayid, now_dayid);
	m_role_module->shengxing_zhuli.OnDayChange(old_dayid, now_dayid);
	m_role_module->jinjie_talent.OndayChange(old_dayid, now_dayid);
	m_role_module->yaoshi_manager.OnDayChange(old_dayid, now_dayid);
	m_role_module->toushi_manager.OnDayChange(old_dayid, now_dayid);
	m_role_module->qilinbi_manager.OnDayChange(old_dayid, now_dayid);
	m_role_module->wing.OnDayChange(old_dayid, now_dayid);
	m_role_module->jinglingguanghuan_manager.OnDayChange(old_dayid, now_dayid);
	m_role_module->jinglingfazhen_manager.OnDayChange(old_dayid, now_dayid);
	m_role_module->halo.OnDayChange(old_dayid, now_dayid);
	m_role_module->shengong.OnDayChange(old_dayid, now_dayid);
	m_role_module->shenyi.OnDayChange(old_dayid, now_dayid);
	m_role_module->xiannvshouhu_manager.OnDayChange(old_dayid, now_dayid);
	m_role_module->footprint.OnDayChange(old_dayid, now_dayid);
	m_role_module->cloak_manager.OnDayChange(old_dayid, now_dayid);
	m_role_module->fabao.OnDayChange(old_dayid, now_dayid);
	m_role_module->mask_manager.OnDayChange(old_dayid, now_dayid);
	m_role_module->shizhuang.OnDayChange(old_dayid, now_dayid);
	m_role_module->role_am_fb.OnDayChange(old_dayid, now_dayid);
	m_role_module->knapsack.OnDayChange(old_dayid, now_dayid);
	m_role_module->shenshou.OnDayChange(old_dayid, now_dayid);
	m_role_module->shenyin.OnDayChange(old_dayid, now_dayid);

	m_role_module->role_activity.OnDayChange(old_dayid, now_dayid);			// 这个要放在m_welfare和m_active_degree之后，封测活动对登录天数和在线时间有依赖
	m_role_module->discount_buy.OnDayChange(old_dayid, now_dayid);
	m_role_module->capablity.OnDayChange();
	m_role_module->zero_gift.OnDayChange();
	m_role_module->redequip_collect.OnDayChange();
	m_role_module->molong.OnDayChange(old_dayid, now_dayid);
	m_role_module->xianzunka.OnDayChange(old_dayid, now_dayid);
	m_role_module->equip_baptize.OnDayChange(old_dayid, now_dayid);
	m_role_module->role_personboss_fb.OnDayChange(old_dayid, now_dayid);
	m_role_module->m_greate_soldier.OnDayChange(old_dayid, now_dayid);
	m_role_module->lottery_count_module.OnDayChange(old_dayid, now_dayid);
	m_role_module->shengqi.OnDayChange(old_dayid, now_dayid);
	m_role_module->chinese_zodiac.OnDayChange(old_dayid, now_dayid);
	m_role_module->m_greate_soldier.OnDayChange(old_dayid, now_dayid);
	m_role_module->cross_rand_activity.OnDayChange(old_dayid, now_dayid);
	m_role_module->upgrade_card_buy.OnDayChange(old_dayid, now_dayid);
	JinjieSysRewardMgr::Instance().OnDayChange(this, old_dayid, now_dayid);
	m_role_module->role_big_small_goal.OnDayChange(old_dayid, now_dayid);
	m_role_module->cross_goal.OnDayChange(old_dayid, now_dayid);
	m_role_module->mysterious_shop_in_mall.OnDayChange(old_dayid, now_dayid);
	m_role_module->cross_equip.OnDayChange(old_dayid, now_dayid);
	
	m_role_other_mgr->OnDayChange(old_dayid, now_dayid);
	//ClockReturnDayItem *old_day = (ClockReturnDayItem*)&old_dayid;
	//ClockReturnDayItem *now_day = (ClockReturnDayItem*)&now_dayid;
	//for (IRoleModule *module : m_role_module_list)
	//{
	//	module->OnDayChange(*old_day, *now_day);
	//}

	m_role_module->common_data_param.space_day_cai_addhuoli = 0;			// 每日踩一踩活力值清零

	m_role_module->common_data_param.zhuagui_day_gethunli = 0;				// 每日抓鬼 获得活力清零
	m_role_module->common_data_param.zhuagui_day_catch_count = 0;			// 每日抓鬼个数清零

	m_role_module->common_data_param.day_hunyan_gather_bind_coin = 0;
	m_role_module->common_data_param.day_hunyan_add_nvwashi = 0;
	m_role_module->common_data_param.jinghua_husong_commit_count = 0;

	m_role_module->common_data_param.day_hunyan_fa_blessgift_count = 0;

	m_role_module->common_data_param.millionaire_gather_count = 0;
	m_role_module->common_data_param.is_turn_millionaire = 0;
	m_role_module->common_data_param.is_turn = 0;
	m_role_module->common_data_param.millionaire_valid_perform_skill_time = 0;
	m_role_module->common_data_param.millionaire_use_skill_times = 0;

	m_role_module->common_data_param.zhuagui_get_drop_time = 0;
	m_role_module->common_data_param.zhuagui_kill_boss_count = 0;
	m_role_module->common_data_param.zhuagui_get_couple_drop_time = 0;

	m_role_module->common_data_param.miku_boss_weary = 0;

	// 切天不清0
// 	m_role_module->common_data_param.dabao_angry_value = 0;
// 	m_role_module->common_data_param.dabao_kick_time = 0;

	m_role_module->common_data_param.millionaire_last_join_time = 0;
	m_role_module->common_data_param.millionaire_reward_index = -1;
	m_role_module->common_data_param.water_self_love_tree_time = 0;
	m_role_module->common_data_param.water_other_love_tree_time = 0;

	m_role_module->common_data_param.team_equip_fb_day_count = 0;
	m_role_module->common_data_param.team_equip_fb_day_buy_count = 0;

	m_role_module->common_data_param.daily_use_guild_relive_times = 0;
	m_role_module->common_data_param.daily_guild_redbag_reward_fetch_flag = 0;
	m_role_module->common_data_param.daily_use_free_song_hua_times = 0;

	m_role_module->common_data_param.today_use_free_relive_num = 0;

	m_role_module->common_data_param.world_question_right_num = 0;
	m_role_module->common_data_param.world_question_right_num_change_time = 0;
	m_role_module->common_data_param.guild_question_right_num = 0;

	m_role_module->common_data_param.today_putong_hunyan_times = 0;

	m_role_module->common_data_param.collect_drop_num = 0;

	m_role_module->common_data_param.jingling_advantage_gather_blue = 0;
	m_role_module->common_data_param.guild_signin_fetch_reward_flag = 0;
	m_role_module->common_data_param.guild_battle_fetch_daily_reward = 0;

	m_role_module->common_data_param.active_boss_weary = 0;

	memset(m_role_module->common_data_param.today_boss_family_kill_info, 0, sizeof(m_role_module->common_data_param.today_boss_family_kill_info));

	BossFamilyManager::Instance().SendActiveBossInfo(this);
	BossFamilyManager::Instance().SendMikuBossInfo(this);
	FBManager::Instance().SendTeamEquipFBCountInfo(this);
	this->AddOrResetDayRevivalTimes(true);
	this->ResetExpBottleDayCount();
	this->ResetMoveChessDayCount();
	this->SyncSCGulidBossRedbagInfo();
	ZhuaGuiMgr::Instance().SyncZhuaGuiInfo(this);
	JingHuaHuSongMgr::Instance().SyncRoleHusongInfo(this);
	JinglingAdvantage::Instance().SynJinglingCountToRole(this);
	this->SyncCommonInfo(Protocol::SCIT_TODAY_FREE_RELIVE_NUM, m_role_module->common_data_param.today_use_free_relive_num);

	m_role_module->common_data_param.day_guild_red_paper_consume_gold_num = 0;
	RedPaperManager::Instance().SendRedPaperRoleInfo(this);

	m_role_module->common_data_param.recored_use_norex_num = 0;
	memset(m_role_module->common_data_param.use_norex_day_times, 0, sizeof(m_role_module->common_data_param.use_norex_day_times));
	this->SyncNorexItemDayTimes();

	{
		m_role_module->common_data_param.red_paper_lucky_value = 0;
		m_check_week_camp_buff_next_timestamp = EngineAdapter::Instance().Time() + 1;
	}

	this->AutoFetchAdvanceNoticDayReward();

	BossFamilyManager::Instance().SendDabaoBossInfo(this);
	m_role_module->common_data_param.baby_boss_enter_times = 0;
//	m_role_module->common_data_param.baby_boss_angry_value = 0;
	BossFamilyManager::Instance().SendBabyBossRoleInfo(this);
	m_role_module->common_data_param.godmagic_today_kill_boss_num = 0;
	m_role_module->common_data_param.today_gather_ordinary_crystal_num = 0;
	m_role_module->common_data_param.today_gather_treasure_crystal_num = 0;
	BossFamilyManager::Instance().SendGodmagicBossRoleInfo(this);
	m_role_module->little_helper.OnDayChange(old_dayid, now_dayid);

	m_role_module->huguozhili.OnDayChange(old_dayid, now_dayid);

	CommonDataGlobal *common_global = GlobalRoleManager::Instance().GetCommonDataGlobal(m_uid);
	if (nullptr != common_global)
	{
		common_global->CheckReMoveInvalidId();
	}
}

void Role::OnWeekChange()
{
	this->SetHonour(GLOBALCONFIG->GetHonourInitValue(), Protocol::SCRoleCampHonour::REASON_WEEK_CHANGE);
	m_role_module->role_activity.OnWeekChange();
	m_role_module->role_cross.OnWeekChange();
	m_role_module->shizhuang.OnWeekChange();
	m_role_module->friend_manager.OnWeekChange();
	m_role_module->role_guild.OnWeekChange();
	m_role_module->vip.OnWeekChange();
	m_role_module->common_data_param.week_kill_boss_count = 0;
	m_role_module->common_data_param.is_fetch_worldboss_week_reward = 0;

	m_role_module->cross_rand_activity.OnWeekChange();

	m_role_other_mgr->OnWeekChange();
	//for (IRoleModule *module : m_role_module_list)
	//{
	//	module->OnWeekChange();
	//}
}

void Role::OnMonthChange()
{
	m_role_module->welfare.OnMonthChange();

	m_role_other_mgr->OnMonthChange();
	//for (IRoleModule *module : m_role_module_list)
	//{
	//	module->OnMonthChange();
	//}
}

void Role::OnResetModuleData(unsigned int old_dayid, unsigned int now_dayid)
{
	this->GetKnapsack()->OnResetData();
}

void Role::OnExtraResetModuleData(unsigned int old_dayid, unsigned int now_dayid)
{

}

void Role::SendPersonalMsg(int str_len, const char *str)
{
	if (NULL == str || str_len <= 0) return;

	static Protocol::SCSystemMsg sm;
	int sendlen = 0;

	if (SceneManager::CreateSystemMsg(&sm, &sendlen, str, str_len, SYS_MSG_CENTER_PERSONAL_NOTICE))
	{
		EngineAdapter::Instance().NetSend(m_netid, (const char *)&sm, sendlen);
	}
}

void Role::SendLackItemMsg(ItemID item_id)
{
	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_errornotice_lack_item, item_id);
	if (length > 0) 
	{
		this->SendPersonalMsg(length, gamestring::GAMESTRING_BUF);
	}
}

bool Role::CheckEnterSceneLevelLimit(int require_level)
{
	if (this->GetLevel() >= require_level) return true;
	
	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_errornotice_enter_scene_limit_level, require_level);
	if (length > 0) 
	{
		this->SendPersonalMsg(length, gamestring::GAMESTRING_BUF);
	}

	return false;
}

bool Role::CheckEnterFbLevelLimit(int require_level)
{
	if (this->GetLevel() >= require_level) return true;

	int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_errornotice_enter_fb_limit_level, require_level);
	if (length > 0) 
	{
		this->SendPersonalMsg(length, gamestring::GAMESTRING_BUF);
	}

	return false;
}

void Role::OnResetNameReq(GameName name, bool is_item_reset)
{
	name[sizeof(GameName) - 1] = 0;

	InvalidCharFilte(name);
	RoleNameCatPostfix(name, m_user_id.db_index);

	RMIQueryNameExistBackObjectImpl *impl = new RMIQueryNameExistBackObjectImpl();
	impl->user_id = m_user_id;
	F_STRNCPY(impl->role_name, name, sizeof(GameName));
	impl->is_item_reset = is_item_reset;

	RMILoginClient rc;
	rc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());
	rc.QueryNameExistAsyn((const char*)name, impl);
}

bool Role::ResetName(GameName name, bool is_item_reset)
{
	GameName old_name;
	this->GetName(old_name);

	if (is_item_reset)
	{
		Knapsack *knapsack = this->GetKnapsack();
		if (knapsack->IsLock())
		{
			this->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
			return false;
		}

		// 获取可消耗的背包材料列表

		static ItemExtern::ItemConsumeList consume_list; 
		consume_list.Reset();

		int RESET_NAME_ITEM_ID = GLOBALCONFIG->GetResetNameItemId();
		short stuff_count = 0;

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = static_cast<unsigned short>(RESET_NAME_ITEM_ID);
		stuff_list[stuff_count].num = 1;
		stuff_list[stuff_count].buyable = false;

		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(this, false, stuff_count, stuff_list, &consume_list))
		{
			this->NoticeNum(errornum::EN_FUNC_STUFF_NOT_ENOUGH);
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(this, consume_list, "ResetName"))
		{
			return false;
		}
	}

	F_STRNCPY(m_name, name, sizeof(GameName));

	{
		// 同步数据其他模块
		this->OnRoleBaseInfoChange();
		UserCacheManager::Instance().OnResetName(this, old_name);
		QueryHandler::Instance().OnBaseInfoChange(this);
		GuildManager::Instance().UserResetName(m_user_id,m_name);
		CrossRecordManager::Instance().OnUserResetName(m_user_id, m_name);
	}

	this->GetFriendManager()->OnRoleResetName();

	// 通知客户端
	Protocol::SCRoleResetName rrn;
	rrn.obj_id = m_obj_id;
	F_STRNCPY(rrn.name, m_name, sizeof(GameName));
	if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, &rrn, sizeof(rrn));

	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_role_rename_notice, 
			UidToInt(m_user_id), old_name, m_camp, UidToInt(m_user_id), m_name, m_camp);
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
		}
	}

	{
		static MailSubject subject;
		static MailContentTxt contenttxt;

		int length1 = SNPRINTF(subject, sizeof(subject), gamestring::g_role_rename_subject);
		int length2 = SNPRINTF(contenttxt, sizeof(contenttxt), gamestring::g_role_rename_content, old_name, m_name);
		if (length1 > 0 && length2 > 0)
		{
			this->GetFriendManager()->SystemMailToAllFriend(subject, contenttxt);
		}
	}

	if (INVALID_USER_ID != IntToUid(m_role_module->common_data_param.lover_uid))
	{
		Role *lover_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(m_role_module->common_data_param.lover_uid));
		if (NULL != lover_role)
		{
			lover_role->SetMarryInfo(m_uid, m_name);
		}
	}

	EventHandler::Instance().OnResetName(this, m_name);

	gamelog::g_log_role_rename.printf(LL_INFO, "[Role::ResetName Succ][user[%s %d] old_name:%s]", m_name, UidToInt(m_user_id), old_name);

	return true;
}


void Role::ChangeCamp(int camp)
{
	if (camp <= CAMP_TYPE_INVALID || camp >= CAMP_TYPE_MAX)
	{
		return;
	}

	if (m_camp <= CAMP_TYPE_INVALID || m_camp >= CAMP_TYPE_MAX)
	{
		this->NoticeNum(errornum::EN_CHANGE_CAMP_NOCAMP_LIMIT);
		return;
	}

	if (m_camp == camp)
	{
		return;
	}

	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_CHANGE_CAMP, &limit_unit);

	if (is_limit && m_level < limit_unit.level)
	{
		this->NoticeNum(errornum::EN_CHANGE_CAMP_LEVEL_LIMIT);
		return;
	}

	if (GetDayIndex(this->GetServerStartTime(), EngineAdapter::Instance().Time()) <= 0)
	{
		this->NoticeNum(errornum::EN_CHANGE_CAMP_OPEN_DAY_LIMIT);
		return;
	}

	if (m_role_module->husong_task.IsHusongStatus() || m_role_module->newtask_manager.HasTaskType(TASK_TYPE_CISHA))
	{
		this->NoticeNum(errornum::EN_CHANGE_CAMP_LIMIT_HAS_TASK);
		return;
	}

// 	if (m_wabao.IsStarted())
// 	{
// 		this->NoticeNum(errornum::EN_CHANGE_CAMP_LIMIT_HAS_WABAO);
// 		return;
// 	}

	if (CampDefendMgr::Instance().IsCampDefending() || CampDefendMgr::Instance().IsCampDefendStandBy())
	{
		this->NoticeNum(errornum::EN_CHANGE_CAMP_LIMIT_CAMPDEFEND);
		return;
	}

	if (Scene::IsCampNormalDuoBaoScene(this->GetScene()->GetSceneID()) ||
		Scene::SCENE_TYPE_CAMPGAOJI_DUOBAO == this->GetScene()->GetSceneType())
	{
		this->NoticeNum(errornum::EN_CHANGE_CAMP_LIMIT_DUOBAOSCENE);
		return;
	}

 	if (static_cast<unsigned int>(EngineAdapter::Instance().Time()) < m_role_module->common_data_param.m_next_canchangecamp_timestamp)
 	{
 		this->NoticeNum(errornum::EN_CHANGE_CAMP_LIMIT_TIME);
 		return;
 	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	{
		// 检查
		const static ItemID CHANGE_CAMP_STUFF_ID = 27583;

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = CHANGE_CAMP_STUFF_ID;
		stuff_list[0].num = 1;
		stuff_list[0].buyable = true;

		if (!ItemExtern::GetConsumeListByOrder(this, false, 1, stuff_list, &consume_list, true))
		{
			return;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(this, consume_list, "Role::ChangeCamp"))
		{
			return;
		}
		}

	m_role_module->common_data_param.m_next_canchangecamp_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + SECOND_PER_DAY; // 1天

	char old_camp = m_camp;
	m_camp = camp;

	{
		// 通知客户端
		Protocol::SCRoleChangeCamp rcc;
		rcc.obj_id = m_obj_id;
		rcc.camp = m_camp;
		if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, &rcc, sizeof(rcc));
	}

	{
		// 各种同步数据
		this->OnRoleBaseInfoChange();

		if (LOGIC_CONFIG->GetJoinLimitConfig().CanJoinOther(JLMT_OTHER_CHALLENGE_FIELD, this))
		{
			ChallengeField::Instance().OnSyncChallengeParam(this);
		}

		CrossChallengeField::Instance().OnSyncChallengeParam(this);

		// 转阵营时如果是前3名的，清掉相关数据
		{
			WorldStatusCommonDataParam &world_common_p = WorldStatus::Instance().GetCommonData();
			
			for (int i = 0; i < MAX_CAMP_RECORD_RANK3_NUM; ++ i)
			{
				if (world_common_p.camp_toprank3_uid[i] == this->GetUID())
				{
					world_common_p.camp_toprank3_uid[i] = 0;
					WorldStatus::Instance().SetCampDiaoXiangStatus(i, DIAO_XIANG_STATUS_DESTORY);
					CampManager::Instance().CheckRoleRank3Reward(this);
					CampDefendMgr::Instance().UpdateDiaoXiang();

					break;
				}
			}
		}

		QueryHandler::Instance().OnBaseInfoChange(this);
		CampManager::Instance().OnRoleChangeCamp(old_camp, m_camp);
		m_role_module->newtask_manager.CheckCishaTaskValid(false);
		CampDefendMgr::Instance().OnRoleChangeCamp(this);

		{
			m_role_module->rank_shadow.ForceSyncData();

			PersonRank *person_rank = RankManager::Instance().GetPersonRank();
			if (NULL != person_rank)
			{
				person_rank->ForceSnapshot();
			}
		}
	}

	{
		// 发传闻
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_role_change_camp_notice, 
			UidToInt(m_user_id), m_name, static_cast<int>(m_camp), static_cast<int>(old_camp), static_cast<int>(m_camp));
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
		}
	}
}

void Role::ChangeProf(char prof, char sex)
{
	if (m_prof <= PROF_TYPE_INVALID || m_prof >= PROF_TYPE_MAX)
	{
		return;
	}

	if (prof <= PROF_TYPE_INVALID || prof >= PROF_TYPE_MAX || prof == m_prof)
	{
		return;
	}

	if (m_prof > PROF_TYPE_INVALID && m_prof < PROF_TYPE_PROF_NO_LIMIT)
	{
		if (prof <= PROF_TYPE_INVALID || prof >= PROF_TYPE_PROF_NO_LIMIT)
		{
			return;
		}
	}

	if (m_prof > PROF_TYPE_PROF_ZHUANZHI_BASE && m_prof < PROF_TYPE_MAX)
	{
		if (prof <= PROF_TYPE_PROF_ZHUANZHI_BASE || prof >= PROF_TYPE_MAX)
		{
			return;
		}
	}

	if (sex < 0 || sex >= SEX_MAX)
	{
		return;
	}

	if (sex != m_sex && this->IsMarry())
	{
		this->NoticeNum(errornum::EN_ZHUANZHI_MARRY_LIMIT);
		return;
	}

	JoinLimitUnit limit_unit;
	bool is_limit = LOGIC_CONFIG->GetJoinLimitConfig().GetOtherJoinLimit(JLMT_OTHER_ZHUANZHI, &limit_unit);

	if (is_limit && m_level < limit_unit.level)
	{
		this->NoticeNum(errornum::EN_ZHUANZHI_LEVEL_LIMIT);
		return;
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();
	{
		// 检查
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = LOGIC_CONFIG->GetZhuanzhiCfg().GetZhuanzhiStuffID();
		stuff_list[0].num = 1;
		stuff_list[0].buyable = true;

		if (!ItemExtern::GetConsumeListByOrder(this, false, 1, stuff_list, &consume_list, true))
		{
			return;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(this, consume_list, "Role::ChangeProf"))
		{
			return;
		}
		}

	{
		char old_sex = m_sex;
		char old_prof = m_prof;

		m_sex = sex;
		m_prof = prof;

		Protocol::SCChangeProfResult rcp;
		rcp.obj_id = m_obj_id;
		rcp.prof = m_prof;
		rcp.sex = m_sex;
		if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, &rcp, sizeof(rcp));

		m_skill_manager.OnChangeProf(old_prof);
		m_role_module->knapsack.OnChangeProf();
		m_role_module->equip_manager.OnChangeProf();

		if (old_sex != m_sex)
		{
			m_role_module->knapsack.ChangeAllItemToOtherSex();
			EventHandler::Instance().OnChangeSex(this, old_sex);
		}

		{
			// 各种同步数据
			this->OnRoleBaseInfoChange();

			if (LOGIC_CONFIG->GetJoinLimitConfig().CanJoinOther(JLMT_OTHER_CHALLENGE_FIELD, this))
			{
				ChallengeField::Instance().OnSyncChallengeParam(this);
			}

			CrossChallengeField::Instance().OnSyncChallengeParam(this);

			QueryHandler::Instance().OnBaseInfoChange(this);
		}

		{
			// 发传闻
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_change_prof, 
				UidToInt(m_user_id), m_name, static_cast<int>(m_camp), old_prof, m_prof);
			if (length > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
			}
		}
	}
}

void Role::OnSingleChatMonitor(unsigned int msg_time, int msg_length, const char *msg)
{
	if (NULL == msg || msg_length <= 0 || msg_length >= MAX_CHAT_LENGTH) return;

	m_last_singlechat_monitor_time = msg_time;

	if (msg_length >= MAX_SINGLE_CHAT_MONITOR_LENGTH) msg_length = MAX_SINGLE_CHAT_MONITOR_LENGTH - 1;

	memcpy(m_last_singlechat_monitor_msg, msg, msg_length);

	m_last_singlechat_monitor_msg[msg_length] = 0;

	if (World::GetInstWorld()->GetSceneManager()->IsMonitorAutoForbid()) 
	{
		this->ForbidTalk(3600, "SingleChatMonitor", m_last_singlechat_monitor_msg);
	}
}

void Role::OnWorldChatMonitor(unsigned int msg_time, int msg_length, const char *msg)
{
	if (NULL == msg || msg_length <= 0 || msg_length >= MAX_CHAT_LENGTH) return;

	m_last_worldchat_monitor_time = msg_time;

	if (msg_length >= MAX_WORLD_CHAT_MONITOR_LENGTH) msg_length = MAX_WORLD_CHAT_MONITOR_LENGTH - 1;

	memcpy(m_last_worldchat_monitor_msg, msg, msg_length);

	m_last_worldchat_monitor_msg[msg_length] = 0;

	if (World::GetInstWorld()->GetSceneManager()->IsMonitorAutoForbid()) 
	{
		this->ForbidTalk(3600, "WorldChatMonitor", m_last_worldchat_monitor_msg);
	}
}

void Role::OnGuildChatMonitor(unsigned int msg_time, int msg_length, const char *msg)
{
	if (NULL == msg || msg_length <= 0 || msg_length >= MAX_CHAT_LENGTH) return;

	if (msg_length >= MAX_GUILD_CHAT_MONITOR_LENGTH) msg_length = MAX_GUILD_CHAT_MONITOR_LENGTH - 1;

	static char last_guildchat_monitor_msg[MAX_GUILD_CHAT_MONITOR_LENGTH] = {0};
	memcpy(last_guildchat_monitor_msg, msg, msg_length);
	last_guildchat_monitor_msg[msg_length] = 0;

	if (World::GetInstWorld()->GetSceneManager()->IsMonitorAutoForbid()) 
	{
		this->ForbidTalk(3600, "GuildChatMonitor", last_guildchat_monitor_msg);
	}
}

void Role::OnMailMonitor(MailSubject subject)
{
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now > m_mail_monitor_start_time + MAIL_SEND_MONITOR_INTERVAL)
	{
		m_mail_monitor_start_time = now;
		m_mail_send_count = 1;

		return;
	}

	m_mail_send_count++;

	if (m_mail_send_count > MAIL_SEND_MONITOR_MAX_COUNT)
	{
		m_last_mail_monitor_time = now;
		m_mail_send_count = 1;

		UNSTD_STATIC_CHECK(MAX_MAIL_MONITOR_SUBJECT_LENGTH == MAX_SUBJECT_LENTH);

		F_STRNCPY(m_last_mail_monitor_subject, subject, sizeof(m_last_mail_monitor_subject));

		this->ForbidRole("Mail Monitor", 60);
	}
}

void Role::OnKillOtherCampGetHonour(const UserID &bekill_user_id, int delta_level)
{
	Role *bekill_role = World::GetInstWorld()->GetSceneManager()->GetRole(bekill_user_id);
	if (NULL != bekill_role && bekill_role->GetHonour() > 0)
	{
		int bekill_role_honour = bekill_role->GetHonour();
		int delta_hounor = (int)(bekill_role_honour * (GLOBALCONFIG->GetKillOtherCampHounorPer() / 100.0f));

		if (m_role_module->daycounter.DayCountCheck(DayCounter::DAYCOUNT_KILL_OTHER_CAMP_COUNT, GLOBALCONFIG->GetDoubleHonourKillCount()))
		{
			m_role_module->daycounter.DayCountIncrease(DayCounter::DAYCOUNT_KILL_OTHER_CAMP_COUNT);
			delta_hounor *= 2;
		}

		if (delta_hounor <= 0) delta_hounor = 1;
		if (delta_hounor > bekill_role_honour) delta_hounor = bekill_role_honour;

		this->SetHonour(m_honour + delta_hounor);
		bekill_role->SetHonour(bekill_role_honour - delta_hounor);
	}
}

void Role::SetHonour(int honour, int reason)
{
	m_honour = honour;

	if (m_honour < 0) m_honour = 0;

	Protocol::SCRoleCampHonour rch;
	rch.honour = m_honour;
	rch.reason = reason;
	EngineAdapter::Instance().NetSend(m_netid, (const char*)&rch, sizeof(rch));
}

bool Role::GetRoleStatusRealivePos(int *scene_id, Posi *pos)
{
	if (NULL == scene_id || NULL == pos) return false;

	//if (m_husong_task.IsHusongStatus())  以前护送死了，需要去特定的点，现在不需要了
	//{
	//	*scene_id = LOGIC_CONFIG->GetHuSongConfig().GetRealivePos(pos);
	//	return true;
	//}

	if (Scene::IsWorldBossScene(m_scene->GetSceneID())) 
	{
		const WorldBossReliveCfg * relive_cfg = LOGIC_CONFIG->GetWorldBossConfig().GetWorldBossReliveInfo(m_scene->GetSceneID());
		if(NULL == relive_cfg) return false;

		*scene_id = relive_cfg->relive_scene_id;
		*pos = relive_cfg->relive_pos;

		return true;
	}

	if (Scene::IsBossFamilyScene(m_scene->GetSceneID()))
	{
		*scene_id = this->GetLastScene(pos);
		if (*scene_id > 0)
		{
			return true;
		}
	}

	return false;
}

void Role::UpdateSpecialAppearance(time_t now)
{
	//switch (m_special_appearance)
	//{
	//case SPECIAL_APPEARANCE_TYPE_TEST_1:
	//	{
	//		if (0 != m_next_transform_appearance_time && now >= m_next_transform_appearance_time)
	//		{
	//			this->SetSpecialAppearance(SPECIAL_APPEARANCE_TYPE_NORMAL, 0);
	//			//EventHandler::Instance().OnBianShenEnd(this);
	//		}
	//	}
	//	break;

	//default:
	//	break;
	//}
}

void Role::OnQTEEffectDone()
{
	
}

void Role::CheckQTEEffect(Character *target, bool is_gongji)
{
}

int Role::GetHurtIncPercent(Character *target)
{
	if (NULL == target)
	{
		return 0;
	}

	int add_hurt_per = 0;

	if (Obj::OBJ_TYPE_ROLE == target->GetObjType())
	{
		Role *target_role = static_cast<Role *>(target);

		add_hurt_per += this->GetJingJie()->GetIncreaHurtPer(target_role);
	}

	return add_hurt_per;
}

int Role::GetHurtDecPercent(Character *performer)
{
	if (NULL == performer)
	{
		return 0;
	}

	int dec_percent = 0;

	dec_percent += m_role_module->shenge_system.GetDecHurtPercent();

	return dec_percent;
}

bool Role::IsMianyiDeadlyAttack()
{
	if (this->GetRoleZhuanZhi()->GetRoleZhuanZhiSkill().IsMianyiDeadlyAttack())
	{
		return true;
	}

	if (this->GetMojie()->IsMianyiDeadlyAttack())
	{
		return true;
	}

	return false;
}

void Role::SetZombie(bool is_zombie)
{
	if (is_zombie && m_level > 1) return;

	if (m_is_zombie != is_zombie)
	{
		m_is_zombie = is_zombie;

		if (is_zombie)
		{
			Protocol::SCVisibleObjLeave voj;
			voj.obj_id = m_obj_id;
			voj.reserved = 0;
			if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&voj, sizeof(voj));
		}
		else
		{
			this->SendEnterInfoToRole(NULL);
		}
	}
}

void Role::SendEnterInfoToRole(Role *role)
{
	static Protocol::SCVisibleObjEnterRole esr;

	esr.dir = this->GetDir();
	esr.move_mode_param = this->GetMoveModeParam();
	esr.obj_id = this->GetId();
	esr.role_status = this->GetRoleStatus();

	esr.uuserverid = GetCommonDataParam()->uu_last_login_serverid;
	esr.role_uuid = UniqueUserIDToLongLong(GetRoleCross()->GetCrossOriginalUniqueUserID());
	esr.role_id = UidToInt(this->GetUserId());
	this->GetName(esr.role_name);

	esr.hp = this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	esr.max_hp = this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);

	esr.mp = this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP);
	esr.max_mp = this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXMP);

	esr.level = this->GetLevel();
	esr.camp = this->GetCamp();
	esr.prof = (char)this->GetProf();
	esr.sex = this->GetSex();
	esr.vip_level = this->GetVip()->GetVipLevel();

	esr.speed = this->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED);
	esr.pos_x = (short)this->GetPos().x;
	esr.pos_y = (short)this->GetPos().y;
	esr.distance = this->GetDirDistance();

	esr.attack_mode = this->GetAttackMode();
	esr.name_color = this->GetNameColor();
	{
		// 反击对象
		if (FightBackManager::Instance().IsFightBackObj(role, this))
		{
			esr.is_fightback_obj = (char)1;
		}
		else
		{
			esr.is_fightback_obj = (char)0;
		}
	}
	esr.move_mode = this->GetMoveMode();
	esr.authority_type = this->GetAuthority();

	esr.rest_partner_obj_id = this->GetRestPartnerID();
	esr.guild_post = this->GetGuildPost();

	esr.mount_appeid = this->GetMountManager()->GetMountAppeID();

	this->GetShengWangMgr()->GetXianJieLevel(&esr.xianjie_level);

	this->GetChengJiuMgr()->GetChengJiuTitleLevel(&esr.chengjiu_title_level);
	
	this->GetHusongTask()->GetHusongInfo(&esr.husong_task_id, &esr.husong_color);

	m_role_module->pet.GetPetInfo(&esr.pet_id, &esr.pet_level, &esr.pet_grade, esr.pet_name, &esr.user_pet_special_img);

	esr.appearance = this->GetAppearance();

	esr.use_xiannv_id = this->GetXiannvManager()->GetFightOutXiannv();

	m_role_module->jingling_mgr.GetUsedJingLingInfo(&esr.used_jingling_id, &esr.use_jingling_level, esr.jingling_name, &esr.jingling_use_imageid, &esr.jingling_phantom_img);
	esr.top_dps_flag = m_top_dps_flag ? 1 : 0;
	esr.first_hurt_flag = m_first_hit_flag ? 1 : 0;

	esr.avatar_timestamp = this->GetAvatarTimeStamp();
	esr.pata_pass_level = this->GetRolePataFB()->GetPassLevel();

	if (CrossConfig::Instance().IsHiddenServer())
	{
		esr.little_pet_using_id = this->GetCrossRoleGlobal()->GetUsingPetId();
	}
	else
	{
		LittlePet *m_little_pet = GLOBAL_ROLE_MGR.GetLittlePet(this->GetUID());
		if (NULL != m_little_pet)
		{
			esr.little_pet_using_id = m_little_pet->GetUsingPetId();
			m_little_pet->GetCurPetName(esr.little_pet_name);
		}
	}

	esr.fight_mount_appeid = this->GetFightMountManager()->GetFightMountAppeID();

	this->GetXiannvManager()->GetXiannvInfo(esr.xiannv_name);

	esr.xiannv_huanhua_id = this->GetXiannvManager()->GetXiannvHuanhuaId();
	esr.combine_server_equip_active_special = this->GetCSAEquip()->GetActiveSpecial();
	esr.wuqi_color = this->GetZhuanzhiEquip()->GetEquipNoticeColor(E_INDEX_ZHUANZHI::E_INDEX_WUQI);
	esr.total_capability = this->GetCapability()->GetTotalCapability();
	esr.total_jingjie = this->GetJingJieLevel();
	esr.is_in_xunyou = (this->GetQingyuan()->IsInXunyouStatus() ? 1 : 0);
	m_role_module->m_imp_guard.GetImpUseAppeId(esr.imp_appe_id);

	const MultiMountBindItem *multi_mount_item = this->GetMultiMount()->GetMountOnInfo();
	if (NULL != multi_mount_item)
	{
		esr.multi_mount_res_id = multi_mount_item->res_id;
		esr.multi_mount_is_owner = (multi_mount_item->owner_uid == m_uid ? 1 : 0);
		esr.multi_mount_partner_uid = (multi_mount_item->owner_uid == m_uid ? multi_mount_item->follower_uid : multi_mount_item->owner_uid);
	}
	else
	{
		esr.multi_mount_res_id = 0;
		esr.multi_mount_is_owner = 0;
		esr.multi_mount_partner_uid = 0;
	}

	esr.baojia_use_image_id = this->GetShenqi()->GetBaojiaCurImageId();
	esr.shenbing_use_image_id = this->GetShenqi()->GetShenbingCurImageId();

	esr.attach_mask = 0;

	char *attach_offset_ptr = esr.attach_data;
	unsigned int attach_len = 0;

	{
		{
			if (0 != this->GetGuildID())
			{
				*(GuildID*)attach_offset_ptr = this->GetGuildID(); attach_offset_ptr += sizeof(GuildID);
				this->GetGuildName(attach_offset_ptr); attach_offset_ptr += sizeof(GuildName);
				*(unsigned long long*)attach_offset_ptr = this->GetGuildAvatar(); attach_offset_ptr += sizeof(long long);
				esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_GUILD);
			}
		}

		{
			unsigned short used_title_list[MAX_USE_TITLE_COUNT]; memset(used_title_list, 0, sizeof(used_title_list));
			int use_title_count = this->GetTitleManager()->GetUseTitleList(used_title_list);

			if (use_title_count > 0) 
			{
				UNSTD_STATIC_CHECK(MAX_USE_TITLE_COUNT >= 3);

				*(short*)attach_offset_ptr = used_title_list[0]; attach_offset_ptr += sizeof(short);
				*(short*)attach_offset_ptr = used_title_list[1]; attach_offset_ptr += sizeof(short);
				*(short*)attach_offset_ptr = used_title_list[2]; attach_offset_ptr += sizeof(short);
				esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_USED_TITLE);
			}
		}

		{
			if (ActivityManager::Instance().IsActivityOpen(ACTIVITY_TYPE_MILLIONAIRE))
			{
				*(char*)attach_offset_ptr = this->GetCommonDataParam()->is_turn_millionaire; attach_offset_ptr += sizeof(char);
			}
			else
			{
				*(char*)attach_offset_ptr = 0; attach_offset_ptr += sizeof(char);
			}
			esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_MILLIONARE);
		}

		{
			//*(long long*)attach_offset_ptr = this->GetEffectList()->GetSpecialBuffMark();
			//if (0 != *(long long*)attach_offset_ptr)
			//{
			//	attach_offset_ptr += sizeof(long long);
			//	esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_BUFF_MARK);
			//}

			this->GetEffectList()->GetSpecialBuffMark((unsigned char *)attach_offset_ptr);
			{
				attach_offset_ptr += sizeof(unsigned char) * SPECIAL_BUFF_MAX_BYTE;
				esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_BUFF_MARK);
			}
		}

		{
			*(int*)attach_offset_ptr = m_scene->GetSpecialLogic()->GetSpecialParam(this);

			attach_offset_ptr += sizeof(int);
			esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_SPECIAL_PARAM);
		}

		{
			*(short *)attach_offset_ptr = this->GetHeight();
			if (0 != *(short*)attach_offset_ptr)
			{
				attach_offset_ptr += sizeof(short);
				esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_HEIGHT);
			}
		}

		{
			*(short*)attach_offset_ptr = m_role_module->special_appearance.GetShowAppearance(); attach_offset_ptr += sizeof(short);
			*(int *)attach_offset_ptr = m_role_module->special_appearance.GetShowAppearanceParam(); attach_offset_ptr += sizeof(int);
			esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_SPECIAL_APPEARANCE);
		}

		{
			*(int*)attach_offset_ptr = this->GetEffectList()->GetBianshenAppearance();

			attach_offset_ptr += sizeof(int);
			esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_BIANSHEN_APPEARANCE);
		}

		{
			if (0 != this->GetLoverUid())
			{
				this->GetLoverName(attach_offset_ptr); attach_offset_ptr += sizeof(GameName);
				esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_MARRY_INFO);
			}
		}

		{
			if (m_role_module->jilian.IsWorking())
			{
				*(char *)attach_offset_ptr = m_role_module->jilian.GetJilianType(); attach_offset_ptr += sizeof(char);
				esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_JILIAN_TYPE);
			}
		}

		{
			if (JingHuaHuSongMgr::Instance().IsHusongStatus(this))
			{
				*(short *)attach_offset_ptr = m_role_module->common_data_param.jinghua_husong_status; attach_offset_ptr += sizeof(short);
				esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_JINGHUA_HUSONG);
			}
		}

		if (m_role_module->title_manager.GetUsetJingLingTitle() > 0)
		{
			*(unsigned short *)attach_offset_ptr = m_role_module->title_manager.GetUsetJingLingTitle(); attach_offset_ptr += sizeof(unsigned short);
			esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_JINGLING_TITLE);
		}

		{
			if (m_role_module->qingyuan.IsInCoupleHaloEffect())
			{
				*(int *)attach_offset_ptr = m_role_module->qingyuan.GetCoupleHaloType(); attach_offset_ptr += sizeof(int);
				esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_COUPLE_HALO);

				*(int *)attach_offset_ptr = m_role_module->common_data_param.lover_uid; attach_offset_ptr += sizeof(int);
				esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_COUPLE_UID);
			}
		}

// 		{
// 			*(char*)attach_offset_ptr = m_role_module->role_activity.GetFishingStation(); attach_offset_ptr += sizeof(char);
// 			esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_FISHING_STATION);
// 		}
		{
			*(char *)attach_offset_ptr = m_role_module->common_data_param.accept_task_change_appearn; attach_offset_ptr += sizeof(char);
			esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_TASK_APPEARANCE_TYPE);
		}

		{
			*(int *)attach_offset_ptr = m_role_module->common_data_param.accept_task_change_appearn_param; attach_offset_ptr += sizeof(int);
			esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_TASK_APPEARANCE_PARAM);
		}

		{
			*(unsigned short *)attach_offset_ptr = m_gather_obj_id; attach_offset_ptr += sizeof(unsigned short);
			esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_GATHER_ID);
		}

		{
			*(short *)attach_offset_ptr = this->GetFlyHeight();
			if (0 != *(short*)attach_offset_ptr)
			{
				attach_offset_ptr += sizeof(short);
				esr.attach_mask |= (1 << Protocol::SCVisibleObjEnterRole::ATTACH_TYPE_FLY_HEIGHT);
			}
		}

		attach_len = (unsigned int)(attach_offset_ptr - esr.attach_data);
	}

	int send_len = sizeof(esr) - Protocol::SCVisibleObjEnterRole::ATTACH_DATA_LENGTH + attach_len;

	if (NULL != role)
	{
		EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&esr, send_len);
	}
	else
	{
		if (NULL != m_scene) m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&esr, send_len);
	}
}

void Role::OnInitFinish()
{
	m_role_module->newtask_manager.OnRoleInitFinish();
	this->RecalNextResetDataTimeStamp();
	m_role_module->role_zhuanzhi.OnInitFinish();
	m_role_module->upgrade_manager.OnInitFinish();
	m_role_module->zhuanzhi_equip.OnInitFinish();
	m_role_module->mojie_skill.OnInitFinish();

	//同步
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		GetCommonDataParam()->uuid = UniqueUserIDToLongLong(GetRoleCross()->GetCrossOriginalUniqueUserID());
		long long uuserverid = LocalConfig::Instance().GetPlatType();
		GetCommonDataParam()->uu_last_login_serverid = (uuserverid << 32) + LocalConfig::Instance().GetMergeServerId();
	}


}

void Role::RecalNextResetDataTimeStamp()
{
	time_t last_reset_module_data_timestamp = m_last_reset_module_data_timestamp;

#ifdef _WIN32
	const tm *last_reset_tm = ::localtime(&last_reset_module_data_timestamp);
#else	
	struct tm local_time_item;
	struct tm *last_reset_tm = &local_time_item;
	::localtime_r(&last_reset_module_data_timestamp, &local_time_item);
#endif

	if (NULL != last_reset_tm)
	{
		time_t zero_timestamp = GetZeroTime(last_reset_module_data_timestamp);
		if (last_reset_tm->tm_hour >= ROLE_RESET_MODULE_DATA_TM_HOUR)
		{
			m_next_reset_module_data_timestamp = zero_timestamp + SECOND_PER_DAY + ROLE_RESET_MODULE_DATA_TM_HOUR * SECOND_PER_HOUR;
		}
		else
		{
			m_next_reset_module_data_timestamp = zero_timestamp + ROLE_RESET_MODULE_DATA_TM_HOUR * SECOND_PER_HOUR;
		}
	}
}

void Role::CheckWeekCampBuff(time_t now_second)
{
// 	if (m_camp <= CAMP_TYPE_INVALID || m_camp >= CAMP_TYPE_MAX) return;
// 
// 	if (NULL != m_scene && NULL != m_scene->GetSpecialLogic() && !m_scene->GetSpecialLogic()->SpecialCanCampWeekBuffEffect(this))
// 	{
// 		return;
// 	}
// 
// 	if (now_second >= m_check_week_camp_buff_next_timestamp)
// 	{
// 		const static int CHECK_WEEK_CAMP_BUFF_INTERVAL_S = 300;
// 
// 		m_check_week_camp_buff_next_timestamp = now_second + CHECK_WEEK_CAMP_BUFF_INTERVAL_S;
// 
// 		long long camp_total_capability_list[CAMP_TYPE_MAX]; memset(camp_total_capability_list, 0, sizeof(camp_total_capability_list));
// 		CampManager::Instance().GetCampTotalCapability(camp_total_capability_list);
// 
// 		int strongest_camp = CAMP_TYPE_INVALID;
// 		for (int camp_type = CAMP_TYPE_INVALID + 1; camp_type < CAMP_TYPE_MAX; ++ camp_type)
// 		{
// 			if (camp_total_capability_list[camp_type] > camp_total_capability_list[strongest_camp])
// 			{
// 				strongest_camp = camp_type;
// 			}
// 		}
// 
// 		if (CAMP_TYPE_INVALID != strongest_camp && m_camp != strongest_camp)
// 		{
// 			long long capability_detal = camp_total_capability_list[strongest_camp] - camp_total_capability_list[(int)m_camp];
// 			long long capability_base = camp_total_capability_list[strongest_camp];
// 
// 			if (capability_base > 0)
// 			{
// 				int week_rate = static_cast<int>(0.5f * capability_detal * 100.0f / capability_base);
// 				int add_percent = 0;
// 
// 				if (week_rate >= 10 && week_rate < 15)
// 				{
// 					add_percent = 10;
// 				}
// 				else if (week_rate >= 15 && week_rate < 20)
// 				{
// 					add_percent = 15;
// 				}
// 				else if (week_rate >= 20)
// 				{
// 					add_percent = 20;
// 				}
// 
// 				if (add_percent <= 0)
// 				{
// 					return;
// 				}
// 
// 				const static UInt16 WEEK_CAMP_BUFF_SKILL_ID = 65535;
// 
// 				EffectBuff *effect_buff = new EffectBuff(INVALID_OBJ_ID, WEEK_CAMP_BUFF_SKILL_ID, (CHECK_WEEK_CAMP_BUFF_INTERVAL_S + 2) * 1000, 1, EffectBase::PRODUCT_METHOD_SKILL);
// 				effect_buff->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
// 
// 				int fangyu_add_value =  m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) * add_percent / 100;
// 				if (fangyu_add_value < 0) fangyu_add_value = 0;
// 				effect_buff->AddFangYuEffect(fangyu_add_value, add_percent);
// 
// 				int hp_add_value =  m_int_attrs.GetAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP) * add_percent / 100;
// 				if (hp_add_value < 0) hp_add_value = 0;
// 				effect_buff->AddAttrEffect(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, hp_add_value, add_percent);
// 
// 				this->AddEffect(effect_buff);
// 			}
// 		} 
// 	}
}

void Role::CheckGmTestAccount(long long last_save_timestamp)
{
	if (AUTHORITY_TYPE_TEST == m_authority_type && LOGIC_CONFIG->GetAgentAdapterConfig().IsForbidGmAccountOfflineTooLong())
	{
		long long now_second = static_cast<long long>(EngineAdapter::Instance().Time());

		const AgentAdapterOtherCfg &other_cfg = LOGIC_CONFIG->GetAgentAdapterConfig().GetOtherCfg();
		if (now_second - last_save_timestamp >= other_cfg.gm_account_forbid_offline_interval)
		{
			this->ForbidRole("Role::CheckGmTestAccount");
		}
	}
}

void Role::SendNewhandGuideEndtime()
{
	unsigned int end_time = static_cast<unsigned int>(SHAREDCONFIG->GetOpenServerConfig().GetNewhandGuideEndTime());
	unsigned int now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	if (now_time < end_time)
	{
		Protocol::SCNewhandEndTime scmsg;
		scmsg.newhand_end_time = end_time;

		EngineAdapter::Instance().NetSend(m_netid, (const char *)&scmsg, sizeof(scmsg));
	}
}

void Role::OnMountBianshenOpera(bool is_bianshen)
{
	if (m_level < MOUNT_BIANSHEN_BEGIN_LEVEL || m_level >= MOUNT_BIANSHEN_END_LEVEL) return;

	if (m_mount_bianshen_time <= 0)
	{
		this->NoticeNum(errornum::EN_MOUNT_BIANSHEN_TIME_LIMIT);
		return;
	}

	m_is_mount_bianshen = is_bianshen;

	this->SendMountBianshenInfo(true);
}

void Role::SendMountBianshenInfo(bool is_brocast)
{
	/*
	Protocol::SCMountBianshenInfo mbsi;
	mbsi.uid = UidToInt(m_user_id);
	mbsi.obj_id = m_obj_id;
	mbsi.is_mount_bianshen = m_is_mount_bianshen ? 1 : 0;
	mbsi.mount_bianshen_time = m_mount_bianshen_time;

	if (is_brocast)
	{
		if (NULL != this->GetScene()) this->GetScene()->GetZoneMatrix()->NotifyAreaMsg(this->GetObserHandle(), (void*)&mbsi, sizeof(mbsi));
	}
	else
	{
		EngineAdapter::Instance().NetSend(m_netid, (const char*)&mbsi, sizeof(mbsi));
	}
	*/
}

void Role::UpdateNewhandGuide(unsigned int now)
{
	if (AUTHORITY_TYPE_GUIDER == m_authority_type)
	{
		unsigned int end_time = static_cast<unsigned int>(SHAREDCONFIG->GetOpenServerConfig().GetNewhandGuideEndTime());

		if (now >= end_time)
		{
			this->SetAuthority(AUTHORITY_TYPE_INVALID);
		}
	}
}

void Role::StatMoveReq()
{
	/*if (0 == m_move_stat_begin_time_1s)
	{
		m_move_stat_begin_time_1s = EngineAdapter::Instance().Time();
	}

	if (0 == m_move_stat_begin_time_5s)
	{
		m_move_stat_begin_time_5s = EngineAdapter::Instance().Time();
	}

	++m_move_count_1s;
	++m_move_count_5s;*/
}

void Role::OnSpeedupHello()
{
	++ m_speedup_hello_count;

	++ m_speedup_hello_total_count;
}

void Role::ClearSpeedupHello()
{
	m_speedup_hello_count = 0;
}

int Role::GetSpeedupHelloCount()
{
	return m_speedup_hello_count;
}

int Role::GetSpeedupHelloTotalCount()
{
	return m_speedup_hello_total_count;
}

time_t Role::GetFirstSpeedupHelloTime()
{
	return m_first_speedup_hello_time;
}

void Role::OnWeddingRingSkill(int ring_level, int target_obj_id)
{
	
}

void Role::OnQueryMyPosition(short reason, int from_uid)
{
	if (NULL == m_scene)
	{
		return;
	}

	ggprotocal::RspPosition rspmsg;
	rspmsg.reason = reason;
	rspmsg.is_static_scene = m_scene->InStaticScene() ? 1 : 0;
	rspmsg.my_uid = UidToInt(m_user_id);
	rspmsg.my_scene_id = m_scene->GetSceneID();
	rspmsg.my_scene_key = m_scene->GetSceneKey();
	rspmsg.pos_x = m_posi.x;
	rspmsg.pos_y = m_posi.y;

	UserID target_user_id = IntToUid(from_uid);
	World::GetInstWorld()->GetSceneManager()->SendMessageToGameWorld(target_user_id, (const char *)&rspmsg, sizeof(rspmsg));
}

void Role::OnRspPosition(short reason, bool is_static_scene, int my_scene_id, int my_scene_key, int pos_x, int pos_y)
{
	if (ggprotocal::QueryPosition::QP_REASON_LOVER_SKILL == reason)
	{
		if (!is_static_scene)
		{
			this->NoticeNum(errornum::EN_LOVER_SCENE_CANNOT_TRANSMIT);
			return;
		}

		int target_scene_id = my_scene_id;
		int x = pos_x;
		int y = pos_y;
		CorrectionSceneCfg * correction_scene_cfg = LOGIC_CONFIG->GetFlyCorrectionConfig().GetCorrectionSceneCfg(target_scene_id);
		if (NULL != correction_scene_cfg)
		{
			if (0 == correction_scene_cfg->ignore_camp || m_camp != correction_scene_cfg->ignore_camp)
			{
				target_scene_id = correction_scene_cfg->correction_scene_id;
				x = correction_scene_cfg->correction_x;
				y = correction_scene_cfg->correction_y;
			}
		}

		if (!SceneRegister::Instance().CheckSceneLevelLimit(this, target_scene_id)) 
		{
			return;
		}

		if (!this->GetRoleStatusManager()->CanFlyByShoe(target_scene_id))
		{
			return;
		}

		this->SendWeddingRingSkillInfo();

		World::GetInstWorld()->GetSceneManager()->GoTo(this, target_scene_id, my_scene_key, Posi(x, y));
	}
}

void Role::SendWeddingRingSkillInfo()
{

}

void Role::UpdateUserCache()
{
	UserCacheNode cachenode;
	cachenode.uuid = UniqueUserIDToLongLong(GetRoleCross()->GetCrossOriginalUniqueUserID());
	cachenode.uid = m_uid;

	this->GetName(cachenode.user_name);
	cachenode.sex = m_sex;
	cachenode.camp = m_camp;
	cachenode.prof = m_prof;
	cachenode.level = m_level;
	cachenode.vip_level = m_role_module->vip.GetVipLevel();
	cachenode.capability = m_role_module->capablity.GetTotalCapability();
	cachenode.avatar_timestamp = m_avator_timestamp;
	cachenode.last_logout_timestamp = m_role_module->common_data_param.last_logout_timestamp;
	cachenode.hunjie_item_id = m_role_module->common_data_param.hunjie_item_id;
	cachenode.hunjie_star_level = m_role_module->common_data_param.hunjie_star_level;
	cachenode.hunyan_buy_fb_join_times = m_role_module->common_data_param.hunyan_buy_fb_join_times;
	cachenode.last_buy_hunyan_fb_times_dayid = m_role_module->common_data_param.last_buy_hunyan_fb_times_dayid;
	cachenode.cur_use_avatar_type = m_role_module->common_data_param.cur_use_avatar_type;
	cachenode.cur_use_bubble_type = m_role_module->common_data_param.cur_use_bubble_type;
	cachenode.lover_id = m_role_module->common_data_param.lover_uid;
	cachenode.qingyuan_cap = m_role_module->common_data_param.qingyuan_capablity;
	cachenode.baby_cap = m_role_module->common_data_param.baby_capablity;
	cachenode.little_pet_cap = m_role_module->common_data_param.little_pet_capablity;
	cachenode.shenbing_cur_use_image = m_role_module->common_data_param.shenbing_use_image_id;
	cachenode.baojia_cur_use_image = m_role_module->common_data_param.baojia_use_image_id;
	cachenode.limit_vip_time = m_role_module->vip.GetTimeLimitVipTime();
	cachenode.guild_id = m_guild_id;
	cachenode.has_xianzunka_flag = m_role_module->xianzunka.SetAndGetHasXianzunkaFlag();
	
	UserCacheManager::Instance().UpdateCacheNode(cachenode);
}

void Role::InitCommonData()
{
	if (0 == (m_role_module->common_data_param.init_flag & (1 << CommonDataParam::INITFLAG_NUQI)))
	{
		m_role_module->common_data_param.nuqi = GLOBALCONFIG->GetMaxNuqi();
		m_role_module->common_data_param.init_flag |= (1 << CommonDataParam::INITFLAG_NUQI);
	}

	if (0 == (m_role_module->common_data_param.init_flag & (1 << CommonDataParam::INITFLAG_ENERGY)))
	{
		m_role_module->common_data_param.energy = GLOBALCONFIG->GetMaxEnergy();
		m_role_module->common_data_param.init_flag |= (1 << CommonDataParam::INITFLAG_ENERGY);
		m_role_module->common_data_param.last_give_energy = (unsigned int)(EngineAdapter::Instance().Time());
	}

	if (0 == m_role_module->common_data_param.last_logout_timestamp)
	{
		m_role_module->common_data_param.last_logout_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	}
}

void Role::UpdateEnergy(time_t now)
{
	int GIVE_INTERVAL = GLOBALCONFIG->GetGiveEnergyIntervalS();
	int GIVE_PER_TIME = GLOBALCONFIG->GetGiveEnergyPerTime();

	if (GIVE_INTERVAL <= 0 || GIVE_PER_TIME <= 0)
	{
		return;
	}

	int passed_s = (int)(now - m_role_module->common_data_param.last_give_energy);
	int pass_interval = passed_s / GIVE_INTERVAL;

	if (pass_interval <= 0)
	{
		return;
	}

	int give_count = pass_interval * GIVE_PER_TIME;
	if (give_count > 0)
	{
		if (give_count + m_role_module->common_data_param.energy > GLOBALCONFIG->GetMaxEnergy())
		{
			give_count = GLOBALCONFIG->GetMaxEnergy() - m_role_module->common_data_param.energy;
		}

		if (give_count > 0)
		{
			this->AddEnergy(give_count, "UpdateEnergy");
		}

		m_role_module->common_data_param.last_give_energy = (unsigned int)now;
	}
}

int Role::GetEnergy()
{
	return m_role_module->common_data_param.energy;
}

void Role::AddEnergy(int energy, const char *type)
{
	if (NULL == type)
	{
		return;
	}

	int old_energy = m_role_module->common_data_param.energy;

	m_role_module->common_data_param.energy += energy;

	if (m_role_module->common_data_param.energy < 0)
	{
		m_role_module->common_data_param.energy = 0;
	}

	if (m_role_module->common_data_param.energy > 30000)
	{
		m_role_module->common_data_param.energy = 30000;
	}

	gamelog::g_log_roledata.printf(LL_INFO, "[Role::AddEnergy Succ][user[%d %s], type:%s, add_energy:%d, left_energy:%d, last_give_time:%u]", 
		UidToInt(m_user_id), m_name, type, energy, m_role_module->common_data_param.energy, m_role_module->common_data_param.last_give_energy);

	if (old_energy != m_role_module->common_data_param.energy)
	{
		Protocol::SCRoleEnergy scnuqi;
		scnuqi.energy = m_role_module->common_data_param.energy;
		EngineAdapter::Instance().NetSend(m_netid, (const char*)&scnuqi, sizeof(scnuqi));
	}
}

bool Role::AddEnergyIgnoreOverRange(int energy, const char *type)
{
	if (NULL == type)
	{
		return false;
	}

	if (energy <= 0)
	{
		return false;
	}

	if (m_role_module->common_data_param.energy >= GLOBALCONFIG->GetMaxEnergy())
	{
		this->NoticeNum(errornum::EN_ENERGY_IS_FULL);
		return false;
	}

	int old_energy = m_role_module->common_data_param.energy;
	int new_energy = m_role_module->common_data_param.energy += energy;

	if (new_energy > 30000)
	{
		new_energy = 30000;
	}

	m_role_module->common_data_param.energy = new_energy;

	gamelog::g_log_roledata.printf(LL_INFO, "[Role::AddEnergyIgnoreOverRange Succ][user[%d %s], type:%s, add_energy:%d, left_energy:%d, last_give_time:%u]", 
		UidToInt(m_user_id), m_name, type, energy, m_role_module->common_data_param.energy, m_role_module->common_data_param.last_give_energy);

	if (old_energy != m_role_module->common_data_param.energy)
	{
		Protocol::SCRoleEnergy scnuqi;
		scnuqi.energy = m_role_module->common_data_param.energy;
		EngineAdapter::Instance().NetSend(m_netid, (const char*)&scnuqi, sizeof(scnuqi));
	}

	return true;
}

void Role::AddEnergyForce(int energy, const char *type)
{
	if (NULL == type)
	{
		return;
	}

	if (energy <= 0)
	{
		return;
	}

	int old_energy = m_role_module->common_data_param.energy;
	int new_energy = m_role_module->common_data_param.energy += energy;

	if (new_energy > 30000)
	{
		new_energy = 30000;
	}

	m_role_module->common_data_param.energy = new_energy;

	gamelog::g_log_roledata.printf(LL_INFO, "[Role::AddEnergyForce Succ][user[%d %s], type:%s, add_energy:%d, left_energy:%d, last_give_time:%u]", 
		UidToInt(m_user_id), m_name, type, energy, m_role_module->common_data_param.energy, m_role_module->common_data_param.last_give_energy);

	if (old_energy != m_role_module->common_data_param.energy)
	{
		Protocol::SCRoleEnergy scnuqi;
		scnuqi.energy = m_role_module->common_data_param.energy;
		EngineAdapter::Instance().NetSend(m_netid, (const char*)&scnuqi, sizeof(scnuqi));
	}
}

int Role::GetRealTimeOnlineS()
{
	int online_time_delta = (int)(EngineAdapter::Instance().Time() - m_last_online_stati_time);

	return (m_online_time + online_time_delta);
}

int Role::GetTodayRealTimeOnlineS()
{
	int online_time_delta = (int)(EngineAdapter::Instance().Time() - m_last_today_online_stati_time);

	return (m_today_online_time + online_time_delta);
}

bool Role::IsTeamEquipFBPass(int layer) 
{
	return (m_role_module->common_data_param.team_equip_fb_pass_flag & (1 << layer)) != 0;
}

void Role::OnTeamEquipFBPassLayer(int layer)
{
	m_role_module->common_data_param.team_equip_fb_pass_flag |= (1 << layer);
}

int Role::GetTeamEquipFBDayCount()
{
	return m_role_module->common_data_param.team_equip_fb_day_count;
}

int Role::GetTeamEquipFBDayBuyCount()
{
	return m_role_module->common_data_param.team_equip_fb_day_buy_count;
}

void Role::OnKillWorldBoss()
{
	++m_role_module->common_data_param.week_kill_boss_count;
}

int Role::GetKillWorldBossCount()
{
	return m_role_module->common_data_param.week_kill_boss_count;
}

void Role::AddRedPaperConsumeGold(int gold_num, const char *type)
{
	if (NULL == type) return;

	m_role_module->common_data_param.red_paper_consume_gold_num += gold_num;

	gamelog::g_log_roledata.printf(LL_INFO, "[Role::AddRedPaperConsumeGold Succ] [user[%d %s], type:%s, add_gold:%d, result_gold:%d]",
		UidToInt(m_user_id), m_name, type, gold_num, m_role_module->common_data_param.red_paper_consume_gold_num);
}

long long Role::GetRedPaperConsumeGold()
{
	return m_role_module->common_data_param.red_paper_consume_gold_num;
}

void Role::AddRedPaperLuckyValue(int add_value)
{
	m_role_module->common_data_param.red_paper_lucky_value += add_value;
}

int Role::GetRedPaperLuckyValue()
{
	return m_role_module->common_data_param.red_paper_lucky_value;
}

void Role::AddDayHunyanGatherBindCoin(int add_val)
{
	m_role_module->common_data_param.day_hunyan_gather_bind_coin += add_val;
}

int Role::GetDayHunyanGatherBindCoin()
{
	return m_role_module->common_data_param.day_hunyan_gather_bind_coin;
}

void Role::AddDayHunyanGatherNvWaShi(int add_val)
{
	m_role_module->common_data_param.day_hunyan_add_nvwashi += add_val;
}

int Role::GetDayHunyanGatherNvWaShi()
{
	return m_role_module->common_data_param.day_hunyan_add_nvwashi;
}

void Role::OnChangeCapability(int cap_type)
{
	int history_cap_type = Capability::GetHistoryType(cap_type);
	if (history_cap_type >= 0 && history_cap_type < HISTORY_CAPABILITY_TYPE_MAX)
	{
		int curr_cap = m_role_module->capablity.GetRoleCapability(cap_type);
		{
			if (HISTORY_CAPABILITY_TYPE_EQUIPMENT == history_cap_type)
			{
				curr_cap = m_role_module->capablity.GetRoleCapability(CAPABILITY_TYPE_EQUIPMENT);
			}
		}

		if (curr_cap > m_role_module->history_cap_param.history_cap_list[history_cap_type].max_cap)
		{
			m_role_module->history_cap_param.history_cap_list[history_cap_type].max_cap = curr_cap;
			m_role_module->history_cap_param.history_cap_list[history_cap_type].reach_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
		}
	}

	// 总战力不要挪到上面的括号里面 因为总战力变化是传cap_type=0过来的 GetHistoryType就会进不去
	int total_cap = m_role_module->capablity.GetMaxCapablity();
	if (total_cap > m_role_module->history_cap_param.history_cap_list[HISTORY_CAPABILITY_TYPE_TAOTAL].max_cap)
	{
		m_role_module->history_cap_param.history_cap_list[HISTORY_CAPABILITY_TYPE_TAOTAL].max_cap = total_cap;
		m_role_module->history_cap_param.history_cap_list[HISTORY_CAPABILITY_TYPE_TAOTAL].reach_timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	}
}

const HistoryCapItem * Role::GetHistoryMaxCapability(int history_cap_type)
{
	if (history_cap_type >= 0 && history_cap_type < HISTORY_CAPABILITY_TYPE_MAX)
	{
		return &m_role_module->history_cap_param.history_cap_list[history_cap_type];
	}

	return NULL;
}

int Role::GetPlatSpid()
{
	return m_role_module->common_data_param.plat_spid;
}

void Role::SendOperateResult(short oprate, short result, int param1, int param2)
{
	Protocol::SCOperateResult scret;
	scret.operate = oprate;
	scret.result = result;
	scret.param1 = param1;
	scret.param2 = param2;

	EngineAdapter::Instance().NetSend(m_netid, (const char*)&scret, sizeof(scret));
}

short Role::GetUseNorexItemDayTimes(ItemID item_id)
{
	for (int i = 0; i < m_role_module->common_data_param.recored_use_norex_num && i < ItemNamespace::MAX_USE_NOREXITEM_RECORD_NUM; ++ i)
	{
		if (item_id == m_role_module->common_data_param.use_norex_day_times[i].item_id)
		{
			return m_role_module->common_data_param.use_norex_day_times[i].times;
		}
	}
	
	return 0;
}

void Role::AddUseNorexItemDayTimes(ItemID item_id, short times)
{
	if (item_id <= 0 || times <= 0) return;

	for (int i = 0; i < m_role_module->common_data_param.recored_use_norex_num; ++ i)
	{
		if (item_id == m_role_module->common_data_param.use_norex_day_times[i].item_id)
		{
			m_role_module->common_data_param.use_norex_day_times[i].times += times;
			this->SyncNorexItemDayTimes();

			return;
		}
	}

	if (m_role_module->common_data_param.recored_use_norex_num + 1 < ItemNamespace::MAX_USE_NOREXITEM_RECORD_NUM)
	{
		m_role_module->common_data_param.use_norex_day_times[m_role_module->common_data_param.recored_use_norex_num].item_id = item_id;
		m_role_module->common_data_param.use_norex_day_times[m_role_module->common_data_param.recored_use_norex_num].times += times;
		++ m_role_module->common_data_param.recored_use_norex_num;
		this->SyncNorexItemDayTimes();
	}
}

void Role::SyncNorexItemDayTimes()
{
	Protocol::SCRoleNorexItemUseTimes cmd;
	cmd.count = m_role_module->common_data_param.recored_use_norex_num;

	for (int i = 0; i < cmd.count && i < ItemNamespace::MAX_USE_NOREXITEM_RECORD_NUM; ++ i)
	{
		cmd.use_norex_day_times[i].item_id = m_role_module->common_data_param.use_norex_day_times[i].item_id;
		cmd.use_norex_day_times[i].times = m_role_module->common_data_param.use_norex_day_times[i].times;
	}

	unsigned int sendlen = sizeof(cmd) - sizeof(cmd.use_norex_day_times[0]) * (ItemNamespace::MAX_USE_NOREXITEM_RECORD_NUM - cmd.count);
	EngineAdapter::Instance().NetSend(this->GetNetId(), (const char *)&cmd, sendlen);
}

bool Role::IsIgnoreChatOrMailLevelLimit()
{
	return m_role_module->role_activity.GetHistoryChongZhi() >= GLOBALCONFIG->GetIgnoreLevelLimitChongzhi();
}

void Role::SpaceCaiOrBeCai(int cai_count, bool is_caier)
{
	if (cai_count <= 0)
	{
		return;
	}
	
	int cai_add_renqi = 0;
	if (!is_caier)
	{
		cai_add_renqi = LOGIC_CONFIG->GetSpaceConfig().GetCaiAddRenqi();
		m_role_module->common_data_param.space_renqi += (cai_count * cai_add_renqi);
	}

	const int cai_day_addhuoli_limit = LOGIC_CONFIG->GetSpaceConfig().GetCaiDayHuoliLimit();
	const int cai_addhuoli = LOGIC_CONFIG->GetSpaceConfig().GetCaiAddHuoli();

	int add_huoli = cai_addhuoli * cai_count;
	if (m_role_module->common_data_param.space_day_cai_addhuoli + add_huoli > cai_day_addhuoli_limit)
	{
		add_huoli = cai_day_addhuoli_limit - m_role_module->common_data_param.space_day_cai_addhuoli;
	}

	if (add_huoli <= 0)
	{
		return;
	}

	m_role_module->common_data_param.space_huoli += add_huoli;
	m_role_module->common_data_param.space_day_cai_addhuoli += add_huoli;

	gamelog::g_log_other.printf(LL_INFO, "SpaceCaiOrBeCai user[%d, %s], level[%d], cai_count[%d] addrenqi[%d], addhuoli[%d]", 
		m_uid, m_name, m_level, cai_count, cai_count * cai_add_renqi, add_huoli);

	this->SendSpaceSelfInfo();
}

void Role::SpaceGiftOrBeGift(int gift_count, bool is_sender)
{
	if (gift_count <= 0)
	{
		return;
	}
	
	int gift_add_renqi = 0;
	if (!is_sender)
	{
		gift_add_renqi = LOGIC_CONFIG->GetSpaceConfig().GetGiftAddRenqi();
		m_role_module->common_data_param.space_renqi += (gift_count * gift_add_renqi);

		m_role_module->common_data_param.space_getgift_count += gift_count;
	}

	const int gift_addhuoli = LOGIC_CONFIG->GetSpaceConfig().GetGiftAddHuoLiZhi();
	m_role_module->common_data_param.space_huoli += (gift_addhuoli * gift_count);

	gamelog::g_log_other.printf(LL_INFO, "SpaceGiftOrBeGift user[%d, %s], level[%d], gift_count[%d] addrenqi[%d], addhuoli[%d]", 
		m_uid, m_name, m_level, gift_count, gift_count * gift_add_renqi, gift_addhuoli * gift_count);

	this->SendSpaceSelfInfo();
}

bool Role::HuoliMoreThan(int huoli_cost)
{
	return m_role_module->common_data_param.space_huoli >= huoli_cost;
}

bool Role::UseHuoli(int huoli_cost, const char *reason)
{
	if (m_role_module->common_data_param.space_huoli >= huoli_cost)
	{
		m_role_module->common_data_param.space_huoli -= huoli_cost; 

		gamelog::g_log_other.printf(LL_INFO, "UseHuoli user[%d, %s], level[%d], huoli_cost[%d] reason[%s]", m_uid, m_name, m_level, huoli_cost, reason);
		
		this->SendSpaceSelfInfo();

		return true;
	}

	return false;
}

bool Role::AddHuoli(int huoli, const char *reason)
{
	if (huoli <= 0) 
	{
		return false;
	}

	m_role_module->common_data_param.space_huoli += huoli;

	gamelog::g_log_other.printf(LL_INFO, "AddHuoli user[%d, %s], level[%d], huoli[%d] reason[%s]", m_uid, m_name, m_level, huoli, reason);

	this->SendSpaceSelfInfo();

	return true;
}


void Role::SendSpaceSelfInfo()
{
	static Protocol::SCSpaceSelfInfo ssi;
	ssi.space_renqi = m_role_module->common_data_param.space_renqi;
	ssi.space_getgift_count = m_role_module->common_data_param.space_getgift_count;
	ssi.space_day_cai_addhuoli = m_role_module->common_data_param.space_day_cai_addhuoli;
	ssi.space_huoli = m_role_module->common_data_param.space_huoli;
	F_STRNCPY(ssi.space_xinqing, m_role_module->common_data_param.space_xinqing, sizeof(ssi.space_xinqing));
	EngineAdapter::Instance().NetSend(m_netid, (const char*)&ssi, sizeof(ssi));
}

void Role::UpdateSpaceXinqing(SpaceMessage xinqing)
{
	F_STRNCPY(m_role_module->common_data_param.space_xinqing, xinqing, sizeof(SpaceMessage));

	this->SendSpaceSelfInfo();
}

void Role::SyncCommonInfo(int info_type, int param1, int param2 , int param3, int param4)
{
	static Protocol::SCCommonInfo cmd;
	cmd.info_type = info_type;
	cmd.param1 = param1;
	cmd.param2 = param2;
	cmd.param3 = param3;
	cmd.param4 = param4;
	EngineAdapter::Instance().NetSend(m_netid, (const char*)&cmd, sizeof(cmd));
}

bool Role::IsInCanTradingTime()
{
	if (m_role_module->active_degree.GetDayOnlineTime() >= GLOBALCONFIG->GetTradeLoginTimeLimit())
	{
		return true;
	}

	return false;
}

unsigned short Role::GetEquipXunbaoTimes()
{
	return m_role_module->common_data_param.equip_xunbao_times;
}

unsigned short Role::GetJLXunbaoTimes()
{
	return m_role_module->common_data_param.jl_xunbao_times;
}

unsigned short Role::GetHCHZKillCount() 
{
	return m_role_module->common_data_param.hchz_kill_count;
}

unsigned short Role::GetGCZKillCount()
{
	return m_role_module->common_data_param.gcz_kill_count;
}

int Role::GetRareChestShopTimes()
{
	return m_role_module->common_data_param.rare_chestshop_times;
}

void Role::AddEquipXunBaoTimes(const short times)
{
	if (times <= 0) return;

	int tmp_times = m_role_module->common_data_param.equip_xunbao_times + times;
	if (tmp_times > USHRT_MAX)
	{
		tmp_times = USHRT_MAX;
	}

	m_role_module->common_data_param.equip_xunbao_times = (unsigned short)tmp_times;
}

void Role::AddJLXunbaoTimes(const short times)
{
	if (times <= 0) return;

	int tmp_times = m_role_module->common_data_param.jl_xunbao_times + times;
	if (tmp_times > USHRT_MAX)
	{
		tmp_times = USHRT_MAX;
	}

	m_role_module->common_data_param.jl_xunbao_times = (unsigned short)tmp_times;
}

void Role::AddHCHZKillCount(const short count)
{
	if (count <= 0) return;

	int tmp_count = m_role_module->common_data_param.hchz_kill_count + count;
	if (tmp_count > USHRT_MAX)
	{
		tmp_count = USHRT_MAX;
	}

	m_role_module->common_data_param.hchz_kill_count = (unsigned short)tmp_count;
}

void Role::AddGCZKillCount(const short count)
{
	if (count <= 0) return;

	int tmp_count = m_role_module->common_data_param.gcz_kill_count + count;
	if (tmp_count > USHRT_MAX)
	{
		tmp_count = USHRT_MAX;
	}

	m_role_module->common_data_param.gcz_kill_count = (unsigned short)tmp_count;
}


void Role::AddRareChestShopTimes(const int times)
{
	if (times <= 0)
	{
		return;
	}

	int temp = m_role_module->common_data_param.rare_chestshop_times + times;
	if (temp >= INT_MAX)
	{
		temp = INT_MAX;
	}
	m_role_module->common_data_param.rare_chestshop_times = temp;
}

void Role::ResetRareChestShopTimes()
{
	m_role_module->common_data_param.rare_chestshop_times = 0;
}

bool Role::AddZhuaGuiScore(int score, const char *type)
{
	if (m_role_module->common_data_param.zhuagui_score + score < 0)
	{
		return false;
	}

	m_role_module->common_data_param.zhuagui_score += score;

	if (m_role_module->common_data_param.zhuagui_score > TWO_BILLION) m_role_module->common_data_param.zhuagui_score = TWO_BILLION;

	gamelog::g_log_other.printf(LL_INFO, "AddZhuaGuiScore user[%d, %s], type:%s, zhuagui_score[%d]", UidToInt(this->GetUserId()), this->GetName(), type, m_role_module->common_data_param.zhuagui_score);

	ROLE_LOG_QUICK6(LOG_TYPE_LINGJING, this, score, m_role_module->common_data_param.zhuagui_score, type, NULL);

	ZhuaGuiMgr::Instance().SyncZhuaGuiInfo(this);

	return true;
}

bool Role::ReduceBossWeary(int boss_type, int reduce_weary)
{
	if (reduce_weary <= 0)
	{
		return false;
	}

	if (boss_type == Protocol::BOSS_ENTER_TYPE_BOSS_MIKU)    // 小于0也继续让它使用继续减
	{
		return this->DecMikuBossWeary(reduce_weary);
	}
	else if (boss_type == Protocol::BOSS_ENTER_TYPE_BOSS_ACTIVE)
	{
		return this->DecActiveBossWeary(reduce_weary);
	}

	return false;
}

int Role::GetDailyUseGuildReliveTimes()
{
	return m_role_module->common_data_param.daily_use_guild_relive_times;
}

short Role::GetDailyBossRedbagRewardFetchFlag()
{
	return m_role_module->common_data_param.daily_guild_redbag_reward_fetch_flag;
}

void Role::AddDailyUseGuildReliveTimes()
{
	m_role_module->common_data_param.daily_use_guild_relive_times++;
}

void Role::ChangeDailyBossRedbagRewardFetchFlag(int index) 
{
	m_role_module->common_data_param.daily_guild_redbag_reward_fetch_flag |= (1 << index);
}

void Role::FetchAdvanceNoticReward(int id)
{
	if (id != m_role_module->common_data_param.advance_notice_last_fectch_id + 1)
	{
		return;
	}

	const AdvanceNoticeGuideCfg *advance_cfg = LOGIC_CONFIG->GetAdvanceNoticeCfg().GetAdvanceNoticeCfg(id);
	if (NULL == advance_cfg) return;

	if (this->GetLevel() < advance_cfg->end_level)
	{
		return;
	}

	m_role_module->knapsack.PutListOrMail(advance_cfg->reward_item, PUT_REASON_ADVANCE_NOTICE_REWARD);
	m_role_module->common_data_param.advance_notice_last_fectch_id = id;

	this->SendAdvanceNoticInfo();
}

void Role::FetchAdvanceNoticDayReward(int id)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	if (id <= 0 || id >= CommonDataParam::NOTICE_DAY_MAX_ID)
	{
		return;
	}

	bool is_fetch = BitmapFunction::IsBitSet(m_role_module->common_data_param.notice_day_fectch_flag_list, sizeof(m_role_module->common_data_param.notice_day_fectch_flag_list), id);
	if (is_fetch)
	{
		this->NoticeNum(errornum::EN_GUILD_FETCH_REWARD_BEFORE);
		return;
	}

	const AdvanceNoticeDayCfg *advance_cfg = LOGIC_CONFIG->GetAdvanceNoticeCfg().GetAdvanceNoticeDayCfg(id);
	if (NULL == advance_cfg) return;

	int open_day_index = this->GetOpenServerDay();
	if (open_day_index < advance_cfg->open_day)
	{
		return;
	}

	if (this->GetLevel() < advance_cfg->level_limit)
	{
		return;
	}

	m_role_module->knapsack.PutListOrMail(advance_cfg->reward_item_list, PUT_REASON_ADVANCE_NOTICE_REWARD);

	BitmapFunction::SetBit(m_role_module->common_data_param.notice_day_fectch_flag_list, sizeof(m_role_module->common_data_param.notice_day_fectch_flag_list), id);

	this->SendAdvanceNoticDayInfo();
}

void Role::AutoFetchAdvanceNoticDayReward()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	// 昨天开服多少天
	int yesterday_open_day_index = GetDayIndex(this->GetServerStartTime(), EngineAdapter::Instance().Time());

	for (int id = 1; id < CommonDataParam::NOTICE_DAY_MAX_ID; ++id)
	{
		const AdvanceNoticeDayCfg *advance_cfg = LOGIC_CONFIG->GetAdvanceNoticeCfg().GetAdvanceNoticeDayCfg(id);
		if (NULL == advance_cfg) return;

		if (yesterday_open_day_index < advance_cfg->open_day) continue;

		if (this->GetLevel() < advance_cfg->level_limit)
		{
			continue;
		}

		bool is_fetch = BitmapFunction::IsBitSet(m_role_module->common_data_param.notice_day_fectch_flag_list, sizeof(m_role_module->common_data_param.notice_day_fectch_flag_list), id);
		if (is_fetch)
		{
			continue;
		}

		this->FetchAdvanceNoticDayReward(id);
	}
}

void Role::SendAdvanceNoticInfo()
{
	Protocol::SCAdvanceNoticeInfo cmd;
	cmd.notice_type = Protocol::SCAdvanceNoticeInfo::ADVANCE_NOTICE_TYPE_LEVEL;
	cmd.last_fecth_id = m_role_module->common_data_param.advance_notice_last_fectch_id;

	EngineAdapter::Instance().NetSend(m_netid, (const char*)&cmd, sizeof(Protocol::SCAdvanceNoticeInfo));
}

void Role::SendAdvanceNoticDayInfo()
{
	Protocol::SCAdvanceNoticeInfo cmd;
	cmd.notice_type = Protocol::SCAdvanceNoticeInfo::ADVANCE_NOTICE_TYPE_DAY;
	cmd.last_fecth_id = 0;

	UNSTD_STATIC_CHECK(sizeof(cmd.notice_day_fectch_flag_list) == sizeof(m_role_module->common_data_param.notice_day_fectch_flag_list));
	memcpy(cmd.notice_day_fectch_flag_list, m_role_module->common_data_param.notice_day_fectch_flag_list, sizeof(cmd.notice_day_fectch_flag_list));

	EngineAdapter::Instance().NetSend(m_netid, (const char*)&cmd, sizeof(Protocol::SCAdvanceNoticeInfo));
}

void Role::SendFishingStation()
{
	static Protocol::SCRAFishStatusInfo fai;

	fai.fish_status = this->GetRoleActivity()->GetFishingStation();
	fai.obj_id = m_obj_id;

	m_scene->GetZoneMatrix()->NotifyAreaMsg(m_obser_handle, (void *)&fai, sizeof(Protocol::SCRAFishStatusInfo));
}

UpgradeRush* Role::GetUpgradeRush() { return &m_role_module->upgrade_rush; }

Shengqi * Role::GetShengqi()
{
	return &m_role_module->shengqi;
}

void Role::MsgReply(noticenum::reply_type typ, int value)
{
	static Protocol::SCRoleMsgReply reply;
	reply.typ = typ;
	reply.value = value;
	EngineAdapter::Instance().NetSend(m_netid, (const char*)&reply, sizeof(Protocol::SCRoleMsgReply));
}

Seal *Role::GetSeal()
{
	return &m_role_module->seal;
}

DressingRoomMgr * Role::GetDressingRoomMgr()
{
	return &m_role_module->dressing_room_mgr;
}

JinjieSysRewardParam * Role::GetJinjieSysRewardParam()
{
	return &m_role_module->jinjiesys_reward_param;
}

RoleBigSmallGoal * Role::GetRoleBigSmallGoal()
{
	return &m_role_module->role_big_small_goal;
}

Shenqi *Role::GetShenqi()
{
	return &m_role_module->shen_qi;
}

MojieSkill * Role::GetMojieSkill()
{
	return &m_role_module->mojie_skill;
}

RedEquipCollect * Role::GetRedEquipCollect()
{
	return &m_role_module->redequip_collect;
}

OrangeEquipCollect * Role::GetOrangeEquipCollect()
{
	return &m_role_module->orang_equip_collect;
}

Molong * Role::GetMolong()
{
	return &m_role_module->molong;
}

XianZunKa * Role::GetXianZunKa()
{
	return &m_role_module->xianzunka;
}

EquipBaptize * Role::GetEquipBaptize()
{
	return &m_role_module->equip_baptize;
}

RolePersonBossFB * Role::GetRolePersonBossFb()
{	
	return &m_role_module->role_personboss_fb;
}

GreateSoldier * Role::GetGreateSoldier()
{
	return &m_role_module->m_greate_soldier;
}

LotteryCount * Role::GetLotteryCount()
{
	return &m_role_module->lottery_count_module;
}

RoleCrossRandActivity * Role::GetRoleCrossRandActivity()
{
	return &m_role_module->cross_rand_activity;
}

UpgradeCardBuy * Role::GetUpgradeCardBuy()
{
	return &m_role_module->upgrade_card_buy;
}

Huguozhili * Role::GetHuguozhili()
{
	return &m_role_module->huguozhili;
}

CrossGoal * Role::GetCrossGoal()
{
	return &m_role_module->cross_goal;
}

BaizhanEquip * Role::GetBaizhanEquip()
{
	return &m_role_module->baizhan_equip;
}

CrossEquip * Role::GetCrossEquip()
{
	return &m_role_module->cross_equip;
}

LittleHelper * Role::GetLittleHelper()
{
	return &m_role_module->little_helper;
}

Myth *Role::GetMyth()
{
	return &m_role_module->myth;
}

Zodiac * Role::GetZodiac()
{
	return &m_role_module->zodiac;
}

void Role::UpdateReliveTire(time_t now)
{
	// 精英boss
	if (0 != m_role_module->common_data_param.miku_relive_weary_last_relive_time && (unsigned int)now - m_role_module->common_data_param.miku_relive_weary_last_relive_time >= 5 * 60)         // 五分钟后清楚疲劳
	{
		m_role_module->common_data_param.mikuboss_relive_weary = 0;
		m_role_module->common_data_param.miku_relive_weary_last_relive_time = 0;
		this->SendWorldBossWearyInfo();
	}

	// 跨服boss
	if (m_role_module->common_data_param.cross_boss_relive_tire > 0 && now >= m_role_module->common_data_param.cross_boss_relive_tire_reset_time)
	{
		m_role_module->common_data_param.cross_boss_relive_tire = 0;
		this->SendReliveTireInfo();
	}

	// 跨服秘藏boss
	if (m_role_module->common_data_param.cross_mizang_boss_relive_tire > 0 && now >= m_role_module->common_data_param.cross_mizang_boss_relive_tire_reset_time)
	{
		m_role_module->common_data_param.cross_mizang_boss_relive_tire = 0;
		this->SendCrossMizangBossReliveTireInfo();
	}

	// 跨服幽冥boss
	if (m_role_module->common_data_param.cross_youming_boss_relive_tire > 0 && now >= m_role_module->common_data_param.cross_youming_boss_relive_tire_reset_time)
	{
		m_role_module->common_data_param.cross_youming_boss_relive_tire = 0;
		this->SendCrossYoumingBossReliveTireInfo();
	}
}

int Role::GetTeamIndex()
{
	return m_role_module->team_info.GetTeamIndex();
}

void Role::SetUseAvatarType(int avatar_type)
{
	m_role_module->common_data_param.cur_use_avatar_type = avatar_type;
}

void Role::SetUseBubbleType(int bubble_type)
{
	m_role_module->common_data_param.cur_use_bubble_type = bubble_type;
}

void Role::DieOnReliveTireScene(bool is_die)
{
	if (is_die)     //死亡
	{
		m_role_module->common_data_param.cross_boss_can_realive_time = (unsigned int)(EngineAdapter::Instance().Time());
		if (m_role_module->common_data_param.cross_boss_relive_tire >= CrossBossConfig::Instance().GetOtherCfg().relive_tire_max_level)
		{
			m_role_module->common_data_param.cross_boss_can_realive_time += CrossBossConfig::Instance().GetOtherCfg().relive_tire_time_s;
		}
	}
	else          // 复活
	{
		++m_role_module->common_data_param.cross_boss_relive_tire;
		m_role_module->common_data_param.cross_boss_relive_tire_reset_time = (unsigned int)(EngineAdapter::Instance().Time() + CrossBossConfig::Instance().GetOtherCfg().relive_tire_clear_time_s);
	}
	this->SendReliveTireInfo();
}

bool Role::CanRealiveWithCurTire() const
{
	if (m_role_module->common_data_param.cross_boss_relive_tire >= CrossBossConfig::Instance().GetOtherCfg().relive_tire_max_level &&
		(unsigned int)EngineAdapter::Instance().Time() < m_role_module->common_data_param.cross_boss_can_realive_time)
	{
		return false;
	}

	return true;
}

void Role::SendReliveTireInfo()
{
	static Protocol::SCReliveTire wbrt;
	wbrt.relive_tire_value = m_role_module->common_data_param.cross_boss_relive_tire;
	wbrt.tire_buff_end_time = m_role_module->common_data_param.cross_boss_relive_tire_reset_time;
	wbrt.tire_can_relive_time = m_role_module->common_data_param.cross_boss_can_realive_time;

	EngineAdapter::Instance().NetSend(m_netid, (const char*)&wbrt, sizeof(wbrt));
}

void Role::SendContinueKillInfo()
{
	static Protocol::SCContinueKillInfo kill_info;
	kill_info.kill_count = m_role_module->common_data_param.continue_kill_count;
	kill_info.trigger_continue_kill_invalid_timestamp = m_role_module->common_data_param.trigger_continue_kill_invalid_timestamp;
	EngineAdapter::Instance().NetSend(m_netid, (const char *)&kill_info, sizeof(kill_info));
}

void Role::SyncChengZhuSkill(int is_chengzhu)
{
	const GczOtherCfg &other_cfg = LOGIC_CONFIG->GetGongChengZhanConfig().GetOtherCfg();

	if (1 == is_chengzhu)
	{
		// 添加城主技能id
		if (NULL != SKILLPOOL->GetSkill(other_cfg.chengzhu_skill_id))
		{
			SkillManager::SkillItem real_skill_item;
			if (!m_skill_manager.GetSkillByID(other_cfg.chengzhu_skill_id, &real_skill_item))
			{
				m_skill_manager.AddSkill(other_cfg.chengzhu_skill_id, 1);
			}
		}
	}
	else
	{
		m_skill_manager.RemoveSkill(other_cfg.chengzhu_skill_id);
	}
}

void Role::SyncKillCrossBossRankSkill()
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	GuildOtherConfig & other_cfg = LOGIC_CONFIG->GetGuildConfig().GetGuildOtherConfig();

	if (GetUID() == WorldStatus::Instance().GetKillCrossBossRankMengzhu())
	{
		if (NULL != SKILLPOOL->GetSkill(other_cfg.kill_cross_boss_mengzhu_skill))
		{
			SkillManager::SkillItem real_skill_item;
			if (!m_skill_manager.GetSkillByID(other_cfg.kill_cross_boss_mengzhu_skill, &real_skill_item))
			{
				m_skill_manager.AddSkill(other_cfg.kill_cross_boss_mengzhu_skill,1);
			}
		}
	}
	else
	{
		m_skill_manager.RemoveSkill(other_cfg.kill_cross_boss_mengzhu_skill);
	}
}

bool Role::CanCrossMizangBossRealiveWithCurTire() const
{
	if (m_role_module->common_data_param.cross_mizang_boss_relive_tire >= CrossMizangBossConfig::Instance().GetOtherCfg().relive_tire_max_level &&
		(unsigned int)EngineAdapter::Instance().Time() < m_role_module->common_data_param.cross_mizang_boss_can_realive_time)
	{
		return false;
	}

	return true;
}

void Role::DieOnCrossMizangBossScene(bool is_die)
{
	if (is_die)     //死亡
	{
		m_role_module->common_data_param.cross_mizang_boss_can_realive_time = (unsigned int)(EngineAdapter::Instance().Time());
		if (m_role_module->common_data_param.cross_mizang_boss_relive_tire >= CrossMizangBossConfig::Instance().GetOtherCfg().relive_tire_max_level)
		{
			m_role_module->common_data_param.cross_mizang_boss_can_realive_time += CrossMizangBossConfig::Instance().GetOtherCfg().relive_tire_time_s;
		}
	}
	else          // 复活
	{
		++m_role_module->common_data_param.cross_mizang_boss_relive_tire;
		m_role_module->common_data_param.cross_mizang_boss_relive_tire_reset_time = (unsigned int)(EngineAdapter::Instance().Time() + CrossMizangBossConfig::Instance().GetOtherCfg().relive_tire_clear_time_s);
	}
	this->SendCrossMizangBossReliveTireInfo();
}

void Role::SendCrossMizangBossReliveTireInfo()
{
	static Protocol::SCMiZangBossReliveTire wbrt;
	wbrt.relive_tire_value = m_role_module->common_data_param.cross_mizang_boss_relive_tire;
	wbrt.tire_buff_end_time = m_role_module->common_data_param.cross_mizang_boss_relive_tire_reset_time;
	wbrt.tire_can_relive_time = m_role_module->common_data_param.cross_mizang_boss_can_realive_time;

	EngineAdapter::Instance().NetSend(m_netid, (const char*)&wbrt, sizeof(wbrt));
}

bool Role::CanCrossYoumingBossRealiveWithCurTire() const
{
	if (m_role_module->common_data_param.cross_youming_boss_relive_tire >= CrossYoumingBossConfig::Instance().GetOtherCfg().relive_tire_max_level &&
		(unsigned int)EngineAdapter::Instance().Time() < m_role_module->common_data_param.cross_youming_boss_can_realive_time)
	{
		return false;
	}

	return true;
}

void Role::DieOnCrossYoumingBossScene(bool is_die)
{
	if (is_die)     //死亡
	{
		m_role_module->common_data_param.cross_youming_boss_can_realive_time = (unsigned int)(EngineAdapter::Instance().Time());
		if (m_role_module->common_data_param.cross_youming_boss_relive_tire >= CrossYoumingBossConfig::Instance().GetOtherCfg().relive_tire_max_level)
		{
			m_role_module->common_data_param.cross_youming_boss_can_realive_time += CrossYoumingBossConfig::Instance().GetOtherCfg().relive_tire_time_s;
		}
	}
	else          // 复活
	{
		++m_role_module->common_data_param.cross_youming_boss_relive_tire;
		m_role_module->common_data_param.cross_youming_boss_relive_tire_reset_time = (unsigned int)(EngineAdapter::Instance().Time() + CrossYoumingBossConfig::Instance().GetOtherCfg().relive_tire_clear_time_s);
	}
	this->SendCrossYoumingBossReliveTireInfo();
}

void Role::SendCrossYoumingBossReliveTireInfo()
{
	static Protocol::SCYouMingBossReliveTire wbrt;
	wbrt.relive_tire_value = m_role_module->common_data_param.cross_youming_boss_relive_tire;
	wbrt.tire_buff_end_time = m_role_module->common_data_param.cross_youming_boss_relive_tire_reset_time;
	wbrt.tire_can_relive_time = m_role_module->common_data_param.cross_youming_boss_can_realive_time;

	EngineAdapter::Instance().NetSend(m_netid, (const char*)&wbrt, sizeof(wbrt));
}

int Role::GetKillBossNum(int boss_type)
{
	switch (boss_type)
	{
	case MonsterInitParam::BOSS_TYPE_TASK_MIKU:
		{
			return m_role_module->common_data_param.kill_miku_boss_count;
		}
		break;

	case MonsterInitParam::BOSS_TYPE_TASK_ACTIVE:
		{
			return m_role_module->common_data_param.kill_active_boss_count;
		}
		break;
	}

	return 0;
}

void Role::AddKillBossNum(int boss_type, int num)
{
	if (num <= 0)
	{
		return;
	}

	switch (boss_type)
	{
	case MonsterInitParam::BOSS_TYPE_TASK_MIKU:
	{
		m_role_module->common_data_param.kill_miku_boss_count += num;
	}
	break;

	case MonsterInitParam::BOSS_TYPE_TASK_ACTIVE:
	{
		m_role_module->common_data_param.kill_active_boss_count += num;
	}
	break;
	}
}

void Role::OnReqWorldAcquisition(Protocol::CSWorldAcquisitionLogReq *req)
{
	if (nullptr == req) return;
	
	WorldAcquisitionLogItem log_item;
	
	log_item.log_type = req->log_type;
	log_item.log_str_id = req->log_str_id;
	log_item.item_id = req->item_id;
	log_item.timestamp = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	log_item.role_uid = m_uid;
	UNSTD_STATIC_CHECK(sizeof(log_item.xianpin_type_list) == sizeof(Protocol::CSWorldAcquisitionLogReq::xianpin_type_list));
	memcpy(log_item.xianpin_type_list, req->xianpin_type_list, sizeof(log_item.xianpin_type_list));
	
	WorldStatus2::Instance().AddWorldAcquisitionLog(log_item);
	WorldStatus2::Instance().SendWorldAcquisitionLog(Protocol::SCWorldAcquisitionLog::NOTICE_TYPE_ADD, nullptr);
}

void Role::AddFightBackRole(ObjID attacker)
{
	// 普通非BOSS场景才生效
	if (!m_scene->InCommonScene() || Scene::IsBossScene(m_scene->GetSceneID()))
	{
		return;
	}

	// 本人需要处于和平非红名状态
	if (GLOBALCONFIG->IsRedNameColor(m_evil) || ATTACK_MODE_PEACE != m_attack_mode)
	{
		return;
	}

	if (attacker == this->GetId())
	{
		return;
	}

	Obj *attack_obj = m_scene->GetObj(attacker);
	if (NULL == attack_obj || Obj::OBJ_TYPE_ROLE != attack_obj->GetObjType())
	{
		return;
	}

	Role *attacker_role = dynamic_cast<Role *>(attack_obj);
	if (NULL == attacker_role)
	{
		return;
	}

	// 如果攻击者是和平模式则不加入列表 
	// 假设A攻击了B，A则成为B的反击对象，B虽处于和平模式，B可以反击A。如果此时A切换了和平，B又会成为A的反击对象。造成了互相反击。所以这里直接截断
	if (ATTACK_MODE_PEACE == attacker_role->GetAttackMode())
	{
		return;
	}

	FightBackManager::Instance().AddFightBackRole(this, attacker_role);
}

bool Role::AddActiveBossWeary(short weary)
{
	if (weary <= 0)
	{
		return false;
	}
	short old_weary = m_role_module->common_data_param.active_boss_weary;
	short new_weary = old_weary + weary;
	//可以小于0
	if (new_weary < old_weary)
	{
		return false;
	}
	m_role_module->common_data_param.active_boss_weary = new_weary;
	BossFamilyManager::Instance().SendActiveBossInfo(this);
	m_role_module->little_helper.SendRefreshInfo(Protocol::LITTLE_HELPER_COMPLETE_TYPE_EASY_BOSS);
	return true;
}

bool Role::DecActiveBossWeary(short weary)
{
	if (weary <= 0)
	{
		return false;
	}
	short old_weary = m_role_module->common_data_param.active_boss_weary;
	short new_weary = old_weary - weary;
	//可以小于0
	if (new_weary > old_weary)
	{
		return false;
	}
	m_role_module->common_data_param.active_boss_weary = new_weary;
	BossFamilyManager::Instance().SendActiveBossInfo(this);
	m_role_module->little_helper.SendRefreshInfo(Protocol::LITTLE_HELPER_COMPLETE_TYPE_EASY_BOSS);
	return true;
}

short Role::GetActiveBossWeary()
{
	return m_role_module->common_data_param.active_boss_weary;
}

bool Role::AddMikuBossWeary(short weary)
{
	if (weary <= 0)
	{
		return false;
	}
	short old_weary = m_role_module->common_data_param.miku_boss_weary;
	short new_weary = old_weary + weary;
	//可以小于0
	if (new_weary < old_weary)
	{
		return false;
	}
	m_role_module->common_data_param.miku_boss_weary = new_weary;
	BossFamilyManager::Instance().SendBossRoleInfo(this);
	m_role_module->little_helper.SendRefreshInfo(Protocol::LITTLE_HELPER_COMPLETE_TYPE_DIFFICULT_BOSS);
	return true;
}

bool Role::DecMikuBossWeary(short weary)
{
	if (weary <= 0)
	{
		return false;
	}
	short old_weary = m_role_module->common_data_param.miku_boss_weary;
	short new_weary = old_weary - weary;
	//可以小于0
	if (new_weary > old_weary)
	{
		return false;
	}
	m_role_module->common_data_param.miku_boss_weary = new_weary;
	BossFamilyManager::Instance().SendBossRoleInfo(this);
	m_role_module->little_helper.SendRefreshInfo(Protocol::LITTLE_HELPER_COMPLETE_TYPE_DIFFICULT_BOSS);
	return true;
}

short Role::GetMikuBossWeary()
{
	return m_role_module->common_data_param.miku_boss_weary;
}

bool Role::OnUserRefreshBossCard(int boss_id)
{
	if (nullptr == m_scene)
	{
		return false;
	}

	int scene_type = m_scene->GetSceneType();
	int scene_id = m_scene->GetSceneID();

	if (Scene::IsActiveBossScene(scene_id))
	{
		return BossFamilyManager::Instance().OnUseBossRefreshCardActiveBoss(this, boss_id);
	}

	else if (Scene::IsBossDongKuScene(scene_id))
	{
		return BossFamilyManager::Instance().OnUseBossRefreshCardMikuBoss(this, boss_id);
	}

	else
	{
		switch (scene_type)
		{
			case Scene::SCENE_TYPE_CROSS_BOSS:
			{
				SpecialCrossBoss *special_logic = dynamic_cast<SpecialCrossBoss *>(m_scene->GetSpecialLogic());
				if (nullptr != special_logic && Scene::SCENE_TYPE_CROSS_BOSS == special_logic->GetSceneType())
				{
					return special_logic->OnUseBossRefreshCard(this, boss_id);
				}
			}
			break;

			case Scene::SCENE_TYPE_CROSS_MIZANG_BOSS:
			{
				SpecialCrossMiZangBoss *special_logic = dynamic_cast<SpecialCrossMiZangBoss *>(m_scene->GetSpecialLogic());
				if (nullptr != special_logic && Scene::SCENE_TYPE_CROSS_MIZANG_BOSS == special_logic->GetSceneType())
				{
					return special_logic->OnUseBossRefreshCard(this, boss_id);
				}
			}
			break;

		}
	}
	
	return false;
}

bool Role::GetBossFamilyKillCount(unsigned short monster_id, int &index, unsigned short &kill_count)
{
	for (int i = 0; i < CommonDataParam::BOSS_FAMILY_INFO_MAX_COUNT; ++i)
	{
		CommonDataParam::TodayBossFamilyKillInfo &info = m_role_module->common_data_param.today_boss_family_kill_info[i];
		if (info.monster_id == 0)
		{
			info.monster_id = monster_id;
			kill_count = info.kill_count;
			index = i;
			return true;
		}
		if (info.monster_id == monster_id)
		{
			kill_count = info.kill_count;
			index = i;
			return true;
		}	
	}
	return false;
}

bool Role::AddBossFamilyKillCount(unsigned short monster_id, int index, unsigned short count)
{
	if (monster_id == 0 || index < 0 || index >= CommonDataParam::BOSS_FAMILY_INFO_MAX_COUNT || count <= 0)
	{
		return false;
	}
	CommonDataParam::TodayBossFamilyKillInfo &info = m_role_module->common_data_param.today_boss_family_kill_info[index];
	if (info.monster_id != monster_id)
	{
		return false;
	}
	unsigned short new_count = info.kill_count + count;
	if (new_count < info.kill_count)
	{
		return false;
	}
	info.kill_count = new_count;
	return true;
}

int Role::GetOpenServerDay()
{
	return CrossConfig::Instance().IsHiddenServer() ? CrossManager::Instance().GetGameServerDayIndex(m_role_module->role_cross.GetOriginalUniqueServerID()) + 1 :
														GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), EngineAdapter::Instance().Time()) + 1;
}

int Role::GetOpenServerDay(const time_t time)
{
	return CrossConfig::Instance().IsHiddenServer() ? GetDayIndex(CrossManager::Instance().GetGameServerStartTime(m_role_module->role_cross.GetOriginalUniqueServerID()), time) + 1 :
														GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime(), time) + 1;
}

time_t Role::GetServerStartTime()
{
	return CrossConfig::Instance().IsHiddenServer() ? CrossManager::Instance().GetGameServerStartTime(m_role_module->role_cross.GetOriginalUniqueServerID()) :
														SHAREDCONFIG->GetOpenServerConfig().GetServerRealStartTime();
}

bool Role::SetHasXianzunkaFlag(int card_type)
{
	if (card_type < XIANZUNKA_TYPE_SILVER || card_type >= XIANZUNKA_TYPE_MAX)
	{
		return false;
	}
	m_role_module->common_data_param.has_xianzunka_flag |= (1 << card_type);
	return true;
}

short Role::GetHasXianzunkaFlag()
{
	return m_role_module->common_data_param.has_xianzunka_flag;
}
