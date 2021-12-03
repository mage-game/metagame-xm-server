#include "scenemanager.h"
#include "map.h"
#include "gamelog.h"

#include "utility/logagent.h"
#include "internalcomm.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"
#include "protocal/msgentergs.h"
#include "servercommon/servercommon.h"
#include "scene.h"
#include "obj/character/role.h"
#include "item/knapsack.h"
#include "rmibackobjdef.h"

#include "servercommon/errornum.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/serverconfig/serverconfigpool.h"
#include "servercommon/userprotocal/msgsystem.h"
#include "servercommon/internalprotocal/gcprotocal.h"
#include "servercommon/internalprotocal/crossgameprotocal.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "servercommon/serverconfig/crossconfig.hpp"

#include "world.h"


#include "task/taskrecorder.h"
#include "globalconfig/globalconfig.h"
#include "globalconfig/otherconfig.h"
#include "monster/monsterpool.h"

#include "protocal/msgequipment.h"

#include "speciallogic/speciallogic.hpp"

#include "systemcmddef.h"

#include "item/itempool.h"
#include "other/cmdhandler/cmdhandler.hpp"

#include "scene/sceneregister/sceneregister.hpp"
#include "scene/worldshadow/worldshadow.hpp"
#include "scene/marryshadow/marryshadow.h"

#include "activityshadow/activityshadow.hpp"

#include "config/reloadconfigmanager.hpp"
#include "config/flycorrectionconfig.hpp"
#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/nationalbossconfig.hpp"
#include "config/activityconfig/shuijingconfig.hpp"
#include "other/vip/vipconfig.hpp"

#include "scene/speciallogic/campspecial/specialcampstation.hpp"
#include "scene/speciallogic/worldspecial/specialxianmengzhan.hpp"
#include "scene/speciallogic/worldspecial/specialnationalboss.hpp"
#include "scene/speciallogic/worldspecial/specialyizhandaodi.hpp"
#include "scene/speciallogic/guildspecial/specialguildboss.hpp"
#include "scene/speciallogic/worldspecial/specialyaoshouguangchang.hpp"
#include "scene/speciallogic/worldspecial/specialcrossxiuluotower.hpp"
#include "scene/speciallogic/worldspecial/specialmillionaire.hpp"
#include "scene/speciallogic/worldspecial/specialcrossmultiuserchallenge.hpp"
#include "scene/speciallogic/worldspecial/specialshuijing.hpp"

#include "chat/chatmanager.h"
#include "chat/channel.h"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"
#include "global/rank/rankmanager.hpp"
#include "global/globalrole/globalrolemanager.hpp"
#include "global/globalrole/commondata/commondataglobal.hpp"
#include "equipment/equipmentmanager.h"
#include "speciallogic/worldspecial/specialxingzuoyiji.hpp"
#include "item/hotkeyrecorder.h"
#include "friend/friendmanager.h"
#include "other/vip/vip.hpp"
#include "other/capability/capability.hpp"
#include "other/welfare/welfare.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "other/qingyuan/qingyuan.hpp"
#include "task/other/husongtask.hpp"
#include "other/commonactivity/commonactivityunit.hpp"
#include "other/cross/rolecross.h"
#include "other/multimount/multimount.hpp"
#include "other/daycounter/daycounter.hpp"
#include "global/worldstatus/worldstatus.hpp"
#include "speciallogic/worldspecial/specialcrossguildbattle.hpp"
#include "other/rank/rankshadow.h"
#include "speciallogic/worldspecial/specialcrossboss.hpp"
#include "speciallogic/teamspecial/specialequipteam.hpp"
#include "speciallogic/worldspecial/specialcross1v1.hpp"
#include "speciallogic/other/specialchallengefield.hpp"
#include "speciallogic/other/specialcrosslocalchallengefield.hpp"
#include "speciallogic/guildspecial/specialguildstation.hpp"
#include "speciallogic/worldspecial/specialzhuxie.hpp"
#include "speciallogic/worldspecial/specialterritorywar.hpp"
#include "speciallogic/worldspecial/specialtianjiangcaibao.h"
#include "speciallogic/worldspecial/specialwanglingexplore.h"
#include "speciallogic/guildspecial/specialguildfb.hpp"
#include "speciallogic/worldspecial/specialwuxingguaji.hpp"
#include "speciallogic/worldspecial/specialqunxianluandou.hpp"
#include "speciallogic/worldspecial/specialgongchengzhan.hpp"
#include "speciallogic/worldspecial/specialhunyan.hpp"
#include "speciallogic/worldspecial/specialsuoyaota.hpp"
#include "speciallogic/worldspecial/specialcrosshotspring.hpp"
#include "speciallogic/worldspecial/specialguildbattlenew.hpp"
#include "speciallogic/worldspecial/specialcombineserverboss.hpp"
#include "speciallogic/worldspecial/specialcrossdarknight.hpp"
#include "speciallogic/worldspecial/specialcrossfishing.hpp"
#include "speciallogic/doublesidefbspecial/specialnightfight.hpp"
#include "speciallogic/worldspecial/specialguildquestion.hpp"
#include "speciallogic/doublesidefbspecial/specialmessbattle.hpp"
#include "speciallogic/personspecial/specialarmordefend.hpp"
#include "servercommon/serverconfig/crossbossconfig.hpp"
#include "global/worldstatus2/worldstatus2.hpp"
#include "servercommon/serverconfig/crossmizangbossconfig.hpp"
#include "speciallogic/worldspecial/specialcrossyoumingboss.hpp"
#include "speciallogic/worldspecial/specialcrossmizangboss.hpp"
#include "servercommon/serverconfig/crossyoumingbossconfig.hpp"
#include "other/event/eventhandler.hpp"
#include "servercommon/serverconfig/crossguildbattleconfig.hpp"
#include "global/usercache/usercache.hpp"
#include "other/rolecrossrandactivity/rolecrossrandactivity.hpp"
#include "scene/speciallogic/worldspecial/specialgodmagicboss.hpp"
#include "scene/speciallogic/worldspecial/specialcrosshusongshuijing.hpp"

SceneManager::SceneManager()
	: m_world(0), m_game_time(0), 
	m_message_handler(),
	m_last_check_timeout(0), 
	m_check_timeout_interval(10000), 
	m_check_timeout(true), 
	m_heartbeat_dead_time(10000), m_last_chk_speedup_time(0), 
	m_is_forbid_sublock(false),
	m_is_micro_pc_open(false),
	m_is_monitor_auto_forbid(true)
{
	memset(m_on_cross_handler, 0, sizeof(m_on_cross_handler));

	this->InitCrossHandler();

	// --------------------------------------------------------------------------------------------------------------------------------------
}

SceneManager::~SceneManager()
{
	this->Release();
}

int SceneManager::GetSceneTypeWithID(int scene_id)
{
	std::map<int, int>::iterator type_it = m_scene_id_to_type_map.find(scene_id);
	if (type_it == m_scene_id_to_type_map.end())
	{
		return Scene::SCENE_TYPE_COMMON_SCENE; // 找不到就当作普通场景，一般不可能找不到
	}

	return type_it->second;
}

bool SceneManager::Init(const SceneManagerConfig& scm, int total_server, int server_index)
{
	if (total_server <= 0 || server_index < 0) return false;

	m_message_handler.Init(this);

	for (ConfigMapList::const_iterator iter = scm.configmanager.maps.begin(); iter != scm.configmanager.maps.end(); ++iter)
	{
		if (m_map_list.find(iter->map.id) != m_map_list.end())
		{
			gamelog::g_log_scenemanager.printf(LL_WARNING, "SceneManager On INIT, create map error, map id(%d) is exist!", iter->map.id);
			return false;
		}

		if (!SceneRegister::Instance().IsThisGSScene(total_server, server_index, iter->map.id)) continue;

		Map *map = new Map(iter->map);
		m_map_list[iter->map.id] = map;

	//	gamelog::g_log_scenemanager.printf(LL_INFO, "Init map[%d].", iter->map.id);
	}

	for (ConfigSceneList::const_iterator iter = scm.configmanager.scenes.begin(); iter != scm.configmanager.scenes.end(); ++iter)
	{
		if (!SceneRegister::Instance().IsThisGSScene(total_server, server_index, iter->scene.id)) continue;

		m_scene_id_to_type_map[iter->scene.id] = iter->scene_type;

		if (Scene::IsStaticScene(iter->scene_type) || iter->scene_type == Scene::SCENE_TYPE_BABY_BOSS || iter->scene_type == Scene::SCENE_TYPE_GODMAGIC_BOSS)
		{
			Scene *scene = 0;
			bool ret = this->CreateScene(iter->scene, 0, &scene, iter->scene_type, 0, false);
			if (!ret)
			{
				gamelog::g_log_scenemanager.printf(LL_WARNING, "Create scene[%d, %s]  fail.", iter->scene.id, iter->scene.name.c_str());
				return false;
			}
			//gamelog::g_log_scenemanager.printf(LL_INFO, "Init scene[%d, %s].", iter->scene.id, iter->scene.name.c_str());
		}
		else if (CrossConfig::Instance().IsHiddenServer() && Scene::IsCrossGuildBattleScene(iter->scene_type))
		{
			Scene *scene = 0;
			bool ret = this->CreateScene(iter->scene, CROSS_PUBLIC_SCENE_KEY, &scene, iter->scene_type, 0, false);
			if (!ret || NULL == scene)
			{
				gamelog::g_log_scenemanager.printf(LL_WARNING, "Create crossguildbattle scene[%d, %s]  fail.", iter->scene.id, iter->scene.name.c_str());
				return false;
			}

			SpecialCrossGuildBattle *cross_guildbattle_logical = (SpecialCrossGuildBattle *)scene->GetSpecialLogic();
			if (NULL != cross_guildbattle_logical)
			{
				cross_guildbattle_logical->OnServerStart();
			}
		}

		FBConfig fbconfig;
		fbconfig.config = iter->scene;
		fbconfig.scene_type = iter->scene_type;
		m_fb_config_list[iter->scene.id] = fbconfig;
	}

	this->StartSceneStatusLog(); // 启动场景状态日志

	m_check_timeout = ServerConfigPool::Instance().common_config.timeout.is_check_timeout;
	m_check_timeout_interval = ServerConfigPool::Instance().common_config.timeout.check_timeout_interval_ms;
	m_heartbeat_dead_time = ServerConfigPool::Instance().common_config.timeout.gameworld_heartbeat_dead_time_ms;
	m_is_micro_pc_open = ServerConfigPool::Instance().common_config.is_micro_pc_open;
	m_last_chk_speedup_time = EngineAdapter::Instance().Time();

	return true;
}

bool SceneManager::ReInit(const SceneManagerConfig &scm, int total_server, int server_index)
{
	if (total_server <= 0 || server_index < 0) return false;

	{
		for (ConfigMapList::const_iterator iter = scm.configmanager.maps.begin(); iter != scm.configmanager.maps.end(); ++iter)
		{
			if (!SceneRegister::Instance().IsThisGSScene(total_server, server_index, iter->map.id)) continue;

			MapList::iterator it_map = m_map_list.find(iter->map.id);
			if (it_map != m_map_list.end())
			{
				delete it_map->second;
				m_map_list.erase(it_map);
			}

			Map *map = new Map(iter->map);
			m_map_list[iter->map.id] = map;
		}
	}

	m_fb_config_list.clear();

	for (ConfigSceneList::const_iterator it = scm.configmanager.scenes.begin(); it != scm.configmanager.scenes.end(); ++it)
	{
		if (!SceneRegister::Instance().IsThisGSScene(total_server, server_index, it->scene.id)) continue;

		MapList::iterator it_map = m_map_list.find(it->scene.mapid);
		if (it_map == m_map_list.end())
		{
			continue;
		}

		FBConfig fbconfig;
		fbconfig.config = it->scene;
		fbconfig.scene_type = it->scene_type;
		m_fb_config_list[it->scene.id] = fbconfig;

		for (SceneList::Iterator it_scene = m_scene_list.Beg(); it_scene != m_scene_list.End(); ++it_scene)
		{
			Scene *scene = *it_scene;
			if (it->scene.id == scene->GetSceneID())
			{
				scene->ReInit(it->scene, it_map->second);
			}
		}
	}

	return true;
}

void RMIRoleSaveBackObjImplLocal::RoleSaveRet(int ret)
{
	if (ret != 0) // 处理失败情况
	{
		Error(log_user_id, ERROR_EXECUTE, ret);
	}
}

bool SceneManager::ChangeLocalSceneHelper(Scene *old_scene, Role *role, Scene *target_scene, const Posi &target_pos)
{
	old_scene->RoleLeaveScene(role->GetId());  // 离开当前场景

	RMIRoleSaveBackObjImplLocal *rsbo = new RMIRoleSaveBackObjImplLocal();
	rsbo->log_user_id = role->GetUserId();

	this->SaveRole(role, target_scene->GetSceneID(), target_pos, rsbo);

	static RoleInitParam p;
	role->GetRoleInitParam(&p);

	if (role->GetScene()->InStaticScene() && !CrossConfig::Instance().IsHiddenServer())	// 只有普通场景才会保存到last_scene中
	{
		p.last_scene_id = p.scene_id;
		p.last_scene_x = p.scene_x;
		p.last_scene_y = p.scene_y;
	}

	p.scene_id = target_scene->GetSceneID();
	p.scene_x = target_pos.x;
	p.scene_y = target_pos.y;

	static RoleOtherInitParam other_p;
	role->GetRoleOtherInitParam(&other_p);

	static ItemListParam item_list_p;

	int size = ItemNamespace::MAX_GRID_NUM;
	role->GetKnapsack()->GetItemList(&size, &item_list_p.item_list[0]);
	item_list_p.count = size;

	size = ItemNamespace::MAX_GRID_NUM - size;
	role->GetEquipmentManager()->GetEquipmentList(&size, &item_list_p.item_list[item_list_p.count]);
	item_list_p.count += size;

	HotkeyRecorder *hr = role->GetHotkeyRecorder();	
	static HotkeyListParam hotkey_p;
	hr->GetHotkeyList(&hotkey_p);

	PlatName pname;
	role->GetPlatName(pname);

	FriendManager *fm = role->GetFriendManager();
	static FriendListParam friend_p;
	fm->GetFriendList(&friend_p);

	static BlacklistsParam blacklists_param;
	fm->GetBlackList(&blacklists_param);

	static EnemyListParam enemy_p;
	fm->GetEnemyList(&enemy_p);

	static BusinessListParam busi; busi.count = 0;

	SkillManager *skill_manager = role->GetSkillManager();
	static SkillListParam skill_param;
	skill_manager->GetSkillList(&skill_param);

	Welfare *welfare = role->GetWelfare();
	static DailyFindListParam daily_find_param;
	welfare->GetDailyFindList(&daily_find_param);

	target_scene->RoleEnterScene(false, role->GetPlatSpid(), pname, role->GetUserId(), role->GetNetId(), p, other_p, item_list_p, hotkey_p, friend_p, blacklists_param, enemy_p, 
		busi, skill_param, daily_find_param);

	return true;
}

bool SceneManager::CreateScene(const ConfigScene &scene_config, int scene_key, Scene **scene_create, int scene_type, unsigned int scene_timeout, bool timeout_kick)
{
	MapList::iterator scene_map = m_map_list.find(scene_config.mapid);
	if (scene_map == m_map_list.end())
	{
		gamelog::g_log_scenemanager.printf(LL_WARNING, "SceneManager On INIT, create scene[id:%d] error, map id(%d) is NOT exist!", scene_config.id, scene_config.mapid);
		return false;
	}

	Scene *scene = new Scene(this);

	SceneIndex sceneid = (SceneIndex)m_scene_list.Insert(scene);
	bool ret = scene->Init(sceneid, scene_config, scene_map->second, scene_type, scene_key, scene_timeout, timeout_kick);
	if (!ret)
	{
		m_scene_list.Erase(sceneid);
		delete scene;
		return false;
	}

	m_sceneidmap[SceneIDKey(scene_config.id, scene_key)] = scene->GetIndex();

	*scene_create = scene;

	return true;
}

void SceneManager::DestroyScene()
{
	if (m_destroy_scene_list.size() != 0)
	{
		for (int i = 0; i < (int)m_destroy_scene_list.size(); ++i)
		{
			Scene *scene = GetSceneByIndex(m_destroy_scene_list[i]);
			if (NULL == scene) continue;

			delete scene;
			m_scene_list.Erase(m_destroy_scene_list[i]);
		}

		m_destroy_scene_list.clear();
	}
}

void SceneManager::Release()
{
	for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
	{
		delete (*i);
	}
	m_scene_list.Clear();

	for (MapList::iterator i = m_map_list.begin(); i != m_map_list.end(); ++i)
	{
		delete i->second;
	}
	m_map_list.clear();

	m_sceneidmap.clear();
	m_net_to_rolelocal.clear();
	m_uid_to_rolelocal.clear();
	m_message_handler.Release();
	m_remote_gs_list.clear();
}

void SceneManager::OnRecv(const GSNetID &netid, IP userip, const char *data, int length)
{
	if (length < (int)sizeof(Protocol::MessageHeader)) 
	{
		EngineAdapter::Instance().NetDisconnect(netid, INVALID_USER_ID, "null", -1, "MsgHeaderSizeError");
		return;
	}

	NetSceneMap::iterator i = m_net_to_rolelocal.find(netid);
	if (i != m_net_to_rolelocal.end())
	{
		i->second.last_active_time = m_game_time;

		SceneList::Iterator iter = m_scene_list.Find(i->second.scene_index);
		if (iter != m_scene_list.End())
		{
			Scene *scene = *iter;
			Role *role = MessageHandler::GetRoleHelper(scene, i->second.obj_id, "SceneManager::OnRecv");
			if (NULL == role)
			{
				gamelog::g_log_debug.printf(LL_ERROR, "[SceneManager::OnRecv Error][scene_index:%d obj_id:%d] [gateway:%d netid:%d]", (int)i->second.scene_index, (int)i->second.obj_id,
					(int)netid.gateway_netid, (int)netid.netid);

				m_net_to_rolelocal.erase(netid);    // netid列表有，但是role不存在了，导致这个netid一直挂住，玩家进不去游戏 modify by xiaolong

				return; 
			}

			role->SetMsgTime(m_game_time, userip);

			m_message_handler.HandleMessage(netid, scene, role, i->second.obj_id, data, length);
		}
		else
		{
			gamelog::g_log_scenemanager.printf(LL_DEBUG, "SceneId:%d NOT found.", i->second.scene_index);
		}
	}
	else
	{
		m_message_handler.HandleMessageNoLogin(netid, userip, data, length);
	}
}

bool SceneManager::OnDisconnect(const GSNetID &netid, SceneIndex *p_scene_index, ObjID *p_obj_id)
{
	// gamelog::g_log_scenemanager.printf(LL_DEBUG, "SceneManager::OnDisconnect netid [%d, %d]", netid.gateway_netid, netid.netid);

	bool ret = false;

	NetSceneMap::iterator i = m_net_to_rolelocal.find(netid);
	if (i != m_net_to_rolelocal.end())
	{
		if (i->second.scene_index != SceneIndex(-1))
		{
			SceneList::Iterator iter = m_scene_list.Find(i->second.scene_index);
			if (iter != m_scene_list.End())
			{
				if (NULL != p_scene_index) *p_scene_index = i->second.scene_index;
				if (NULL != p_obj_id) *p_obj_id = i->second.obj_id;

				this->Logout(*iter, i->second.obj_id);
			
				ret = true;
			}
		}
		else
		{
			gamelog::g_log_debug.printf(LL_ERROR, "[SceneManager::OnDisconnect Error][scene_index:%d obj_id:%d]", (int)i->second.scene_index, (int)i->second.obj_id);
		}

		m_net_to_rolelocal.erase(netid);   // 无论怎样都要干掉netid 之前是在else分支里面 by xiaolong
	}

	return ret;
}

void SceneManager::OnGateWayDisconnect(NetID gateway_netid)
{
	gamelog::g_log_scenemanager.printf(LL_DEBUG, "SceneManager::OnGateWayDisconnect netid[%d]", gateway_netid);

	this->LogoutAllRole(gateway_netid);
}

void SceneManager::LogoutAllRole(NetID gateway_netid)
{
	struct DisItem
	{
		RoleLocal rl;
		GSNetID netid;
	};

	while (true)
	{
		static DisItem gsnetid_list[512];
		int dis_num = 0;

		for(NetSceneMap::iterator i = m_net_to_rolelocal.begin(); dis_num < 512 && i != m_net_to_rolelocal.end(); ++i)
		{
			if ((NetID)-1 == gateway_netid || i->first.gateway_netid == (unsigned short)(gateway_netid))
			{
				DisItem di;
				di.rl = i->second;
				di.netid = i->first;
				gsnetid_list[dis_num ++] = di;
			}
		}

		if (0 == dis_num)
		{
			break;
		}

		for (int i = 0; i < dis_num; ++i)
		{
			if (gsnetid_list[i].rl.scene_index != SceneIndex(-1))
			{
				SceneList::Iterator iter = m_scene_list.Find(gsnetid_list[i].rl.scene_index);
				if (iter != m_scene_list.End())
				{
					this->Logout(*iter, gsnetid_list[i].rl.obj_id);  // 这里有可能出现logout失败导致m_net_to_rolelocal没删除干净的情况 xiaolong
					continue;
				}
			}

			m_net_to_rolelocal.erase(gsnetid_list[i].netid);
		}
	}
}

void SceneManager::Update(unsigned long interval)
{
	m_game_time = EngineAdapter::Instance().GetGameTime();
	time_t now = EngineAdapter::Instance().Time();
	unsigned int now_dayid = EngineAdapter::Instance().DayID();

	this->DestroyScene();

	if (m_check_timeout && (m_game_time - m_last_check_timeout > m_check_timeout_interval))
	{
		this->EraseTimeOut();
		m_last_check_timeout = m_game_time;
	}

	if (now - m_last_chk_speedup_time >= 30)
	{
		m_last_chk_speedup_time = now;
		this->EraseSpeedup();
	}

	for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
	{
		(*i)->Update(interval, now, now_dayid);
	}

	this->DestroyScene(); // 这里不清理会导致 场景无效还是能进入场景 那么角色就直接被干掉 
}

void SceneManager::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
	{
		(*i)->OnDayChange(old_dayid, now_dayid);
	}
}

void SceneManager::RegisterRoleLocal(const GSNetID &netid, SceneIndex sceneid, ObjID obj_id)
{
	Scene *scene = GetSceneByIndex(sceneid);
	if (NULL == scene) return;

	Obj *role_obj = scene->GetObj(obj_id);
	if (NULL == role_obj) return;

	if (role_obj->GetObjType() != Obj::OBJ_TYPE_ROLE) return;

	Role *role = (Role*)role_obj;

	UserID user_id = role->GetUserId();

	////////////////////////////////////////////////////////////////////////////////////////////////////////////
	// 此处是为了debug加的日志，如果出现该日志，则表明又有Role影子的情况
	UIDSceneMap::iterator uid_role_item;
	if ((uid_role_item = m_uid_to_rolelocal.find(user_id)) != m_uid_to_rolelocal.end())
	{
		// 难道又出现Role影子的情况？！！
		gamelog::g_log_debug.printf(LL_ERROR, "[SceneManager::EnterScene Error][UserID(%d, %d) is already exist in SceneIndex:%d ObjID:%d]", 
									user_id.db_index, user_id.role_id, (int)uid_role_item->second.scene_index, (int)uid_role_item->second.obj_id);
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////

	RoleLocal rl(sceneid, obj_id, m_game_time);
	m_uid_to_rolelocal[user_id] = rl;
	m_net_to_rolelocal[netid] = rl;

// 	gglobalprotocal::GGlobalRoleEnterScene res;  // 同步到globalserver
// 	res.db_index = user_id.db_index;
// 	res.role_id = user_id.role_id;
// 	res.scene_id = scene->GetSceneID();
// 	res.scene_key = scene->GetSceneKey();
// 	res.obj_id = role->GetId();
// 	res.scene_index = scene->GetIndex();
// 	InternalComm::Instance().NetSend(InternalComm::Instance().m_golbal_server_id, (const char*)&res, sizeof(gglobalprotocal::GGlobalRoleEnterScene));
}

void SceneManager::UnregisterRoleLocal(const GSNetID &leaverole_netid, const UserID &leaverole_user_id)
{
	//Scene *scene = GetSceneByIndex(scene_index);
	//if (NULL == scene) 
	//{
	//	gamelog::g_log_debug.printf(LL_WARNING, "[SceneManager::LeaveScene Warring][NULL == scene del_sceneid(%d) delobj_id(%d) ]", scene_index, obj_id);
	//	return;
	//}

	//Obj *role_obj = scene->GetObj(obj_id);
	//if (NULL == role_obj)
	//{
	//	gamelog::g_log_debug.printf(LL_WARNING, "[SceneManager::LeaveScene Warring][NULL == role_obj del_sceneid(%d) delobj_id(%d) ]", scene_index, obj_id);
	//	return;
	//}

	//if (role_obj->GetObjType() != Obj::OBJ_TYPE_ROLE) return;

	//Role *role = (Role*)role_obj;

	//NetSceneMap::iterator i = m_net_to_rolelocal.find(netid);
	//if (i != m_net_to_rolelocal.end())
	//{
	//	if (i->second.scene_index != scene_index || i->second.obj_id != obj_id) 
	//	{
	//		gamelog::g_log_debug.printf(LL_WARNING, "[SceneManager::LeaveScene Warring][i != m_net_to_rolelocal.end() del_sceneid(%d) delobj_id(%d) now_sceneid(%d) now_delobj_id(%d)]",
	//			scene_index, obj_id, i->second.scene_index, i->second.obj_id);
	//	}
	//}
	m_net_to_rolelocal.erase(leaverole_netid);
	
	//UIDSceneMap::iterator iter = m_uid_to_rolelocal.find(role->GetUserId());
	//if (iter != m_uid_to_rolelocal.end())
	//{
	//	if (iter->second.scene_index != scene_index || iter->second.obj_id != obj_id)
	//	{
	//		gamelog::g_log_debug.printf(LL_WARNING, "[SceneManager::LeaveScene Warring][iter != m_uid_to_rolelocal.end() del_sceneid(%d) delobj_id(%d) now_sceneid(%d) now_delobj_id(%d)]",
	//			scene_index, obj_id, iter->second.scene_index, iter->second.obj_id);
	//	}
	//}
	
	m_uid_to_rolelocal.erase(leaverole_user_id);
}

void SceneManager::Logout(Scene *scene, ObjID obj_id)
{
	Obj *role_roj = scene->GetObj(obj_id);
	if (NULL == role_roj || role_roj->GetObjType() != Obj::OBJ_TYPE_ROLE)
	{
		return;
	}

	Role *role = (Role*)role_roj;
	//UserID user_id = role->GetUserId();

	role->OnRoleLogout(); // 要把状态处理完再下线

	scene->RoleLeaveScene(obj_id, true);

	role->Save(true); // 存档
}

void SceneManager::Logout(const UserID &user_id)
{
	UIDSceneMap::iterator i = m_uid_to_rolelocal.find(user_id);
	if (i != m_uid_to_rolelocal.end())
	{
		if (i->second.scene_index != SceneIndex(-1))
		{
			SceneList::Iterator iter = m_scene_list.Find(i->second.scene_index);
			SceneList::Iterator end_iter = m_scene_list.End();
			if (iter != end_iter) // 找到直接退出
			{
				this->Logout(*iter, i->second.obj_id);
			}
			else // 找不到容错 不然 m_uid_to_rolelocal会保存user_id的映射 下次再也进不了游戏 
			{
				Role *role = this->GetRole(user_id);
				if (NULL == role) // 再找一次 找不到拉倒 直接干
				{
					gamelog::g_log_debug.printf(LL_ERROR,
						"[SceneManager::Logout(UserID:%d,%d) Error][scene_index:%d obj_id:%d] delete uid_local", 
						user_id.db_index, user_id.role_id, (int)i->second.scene_index, (int)i->second.obj_id);
				
					m_uid_to_rolelocal.erase(user_id);		
				}
				else // 竟然还有role 严重了 
				{
					gamelog::g_log_debug.printf(LL_ERROR,
						"[SceneManager::Logout(UserID:%d,%d) Error][scene_index:%d obj_id:%d] exist", 
						user_id.db_index, user_id.role_id, (int)i->second.scene_index, (int)i->second.obj_id);
				}
			}
		}
		else
		{
			gamelog::g_log_debug.printf(LL_ERROR, "[SceneManager::Logout(UserID) Error][scene_index:%d obj_id:%d]", (int)i->second.scene_index, (int)i->second.obj_id);
			m_uid_to_rolelocal.erase(i);
		}
	}
}

void SceneManager::OnRoleLogin(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	long long uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());

	if (m_cross_user_map.find(uuid) != m_cross_user_map.end())
	{
		m_cross_user_map.erase(uuid);

		// 私聊在线状态同步
		ChatManager::Instance().CheckChangeRoleOnline(uuid);
	}
}

int SceneManager::GetSceneIDBySceneType(int scene_type)
{
	for (FBConfigList::iterator it = m_fb_config_list.begin(), end = m_fb_config_list.end(); it != end; ++it)
	{
		if (scene_type == it->second.scene_type)
		{
			return it->first;
		}
	}

	return 0;
}

int SceneManager::GetSceneList(int *scene_list, int max_num)
{
	int count = 0;

	for (SceneIDMap::iterator i = m_sceneidmap.begin(); count < max_num && i != m_sceneidmap.end(); ++i)
	{
		scene_list[count ++] = i->first.scene_id;
	}

	for (FBConfigList::iterator i = m_fb_config_list.begin(); count < max_num && i != m_fb_config_list.end(); ++i)
	{
		scene_list[count ++] = i->first; // 将副本也注册到loginserver
	}

	return count;
}

Scene * SceneManager::GetSceneByIndex(SceneIndex sid)
{
	if (m_scene_list.Exist(sid))
	{
		return m_scene_list[sid];
	}

	return NULL;
}

Scene * SceneManager::GetSceneById(int id, int key)
{
	SceneIDMap::iterator i = m_sceneidmap.find(SceneIDKey(id, key));
	if (i != m_sceneidmap.end())
	{
		return this->GetSceneByIndex(i->second);
	}

	return NULL;
}

bool SceneManager::CheckIsDelayDeleteScene(int scene_id)
{
	FBConfigList::iterator it = m_fb_config_list.find(scene_id);
	if (it != m_fb_config_list.end())
	{
		return Scene::IsDelayDeleteScene(it->second.scene_type, scene_id);
	}

	return false;
}

bool SceneManager::CheckIsTeamScene(int scene_id)
{
	FBConfigList::iterator it = m_fb_config_list.find(scene_id);
	if (it != m_fb_config_list.end())
	{
		return Scene::IsTeamScene(it->second.scene_type, scene_id);
	}

	return false;
}

bool SceneManager::CheckIsGuildScene(int scene_id)
{
	FBConfigList::iterator it = m_fb_config_list.find(scene_id);
	if (it != m_fb_config_list.end())
	{
		return Scene::IsGuildScene(it->second.scene_type, scene_id);
	}

	return false;
}

bool SceneManager::CheckIsYaoShouGuangChangScene(int scene_id)
{
	FBConfigList::iterator it = m_fb_config_list.find(scene_id);
	if (it != m_fb_config_list.end())
	{
		return Scene::IsYaoShouGuangChangScene(it->second.scene_type, scene_id);
	}

	return false;
}

bool SceneManager::CheckIsSuoYaoTaScene(int scene_id)
{
	FBConfigList::iterator it = m_fb_config_list.find(scene_id);
	if (it != m_fb_config_list.end())
	{
		return Scene::IsSuoYaoTaScene(it->second.scene_type, scene_id);
	}

	return false;
}

bool SceneManager::CheckIsBossScene(int scene_id)
{
	FBConfigList::iterator it = m_fb_config_list.find(scene_id);
	if (it != m_fb_config_list.end())
	{
		return Scene::SCENE_TYPE_SHANGGU_BOSS == it->second.scene_type;
	}

	return false;
}

SpecialCampStation * SceneManager::GetCampStationSpecialLogic(int camp_type)
{
	int scene_id = GlobalConfig::Instance()->GetSceneIdByCampType(camp_type);
	int scene_camp_type = GlobalConfig::Instance()->GetCampTypeOfScene(scene_id);

	if (scene_id > 0 && camp_type == scene_camp_type)
	{
		Scene *scene = this->GetSceneById(scene_id, 0);
		if (NULL != scene)
		{
			SpecialLogic *special_logic = scene->GetSpecialLogic();
			if (nullptr != special_logic && Scene::SCENE_TYPE_COMMON_SCENE == special_logic->GetSceneType())
			{
				return dynamic_cast<SpecialCampStation*>(special_logic);
			}
		}
	}

	return NULL;
}

SpecialGuildStation * SceneManager::GetGuildStationSpecialLogic(GuildID guild_id, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_GUILD_STATION);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, guild_id);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, guild_id, &scene);
		}

		if (NULL != scene)
		{
			SpecialLogic *special_logic = scene->GetSpecialLogic();
			if (nullptr != special_logic && Scene::SCENE_TYPE_GUILD_STATION == special_logic->GetSceneType())
			{
				return dynamic_cast<SpecialGuildStation*>(special_logic);
			}
		}
	}

	return NULL;
}

SpecialZhuXie * SceneManager::GetSpecialZhuXie(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_ZHUXIE);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialZhuXie *special_logic = dynamic_cast<SpecialZhuXie*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_ZHUXIE == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialTerritoryWar * SceneManager::GetSpecialTerritoryWar(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_TERRITORY_WAR);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialTerritoryWar *special_logic = dynamic_cast<SpecialTerritoryWar*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_TERRITORY_WAR == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialTianJiangCaiBao * SceneManager::GetSpecialTianJiangCaiBao(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_TIANJIANGCAIBAO);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialTianJiangCaiBao *special_logic = dynamic_cast<SpecialTianJiangCaiBao*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_TIANJIANGCAIBAO == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialWangLingExplore* SceneManager::GetSpecialWangLingExplore(int fb_key, bool is_create /*= false*/)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_WANGLINGEXPLORE);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialWangLingExplore *special_logic = dynamic_cast<SpecialWangLingExplore*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_WANGLINGEXPLORE == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialCrossXiuluoTower * SceneManager::GetSpecialCrossXiuluoTower(int scene_id, int fb_key, bool is_create)
{
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialCrossXiuluoTower *special_logic = dynamic_cast<SpecialCrossXiuluoTower*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_CROSS_XIULUO_TOWER == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialMillionaire * SceneManager::GetSpecialMillionaire(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_MILLIONAIRE);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialMillionaire *special_logic = dynamic_cast<SpecialMillionaire*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_MILLIONAIRE == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialNationalBoss * SceneManager::GetSpecialNationBoss(int fb_key, bool is_create)
{
	int scene_id = LOGIC_CONFIG->GetNationalBossConfig().GetSceneID();
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialNationalBoss *special_logic = dynamic_cast<SpecialNationalBoss*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_NATIONAL_BOSS == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialYiZhanDaoDi * SceneManager::GetSpecialYiZhanDaoDi(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_YIZHANDAODI);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialYiZhanDaoDi *special_logic = dynamic_cast<SpecialYiZhanDaoDi*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_YIZHANDAODI == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialGuildFb * SceneManager::GetSpecialGuildFb(int scene_id, int fb_key, bool is_create)
{
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialGuildFb *special_logic = dynamic_cast<SpecialGuildFb*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_GUILD_FB == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialWuXingGuaJi * SceneManager::GetSpecialWuXingGuaJi(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_WUXINGGUAJI);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialWuXingGuaJi *special_logic = dynamic_cast<SpecialWuXingGuaJi*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_WUXINGGUAJI == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialQunXianLuanDou * SceneManager::GetSpecialQunXianLuanDou(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_QUNXIANLUANDOU);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialQunXianLuanDou *special_logic = dynamic_cast<SpecialQunXianLuanDou*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_QUNXIANLUANDOU == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialGongChengZhan * SceneManager::GetSpecialGongChengZhan(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_GONGCHENGZHAN);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialGongChengZhan *special_logic = dynamic_cast<SpecialGongChengZhan*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_GONGCHENGZHAN == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialXianMengZhan * SceneManager::GetSpecialXianMengZhan(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_XIANMENGZHAN);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialXianMengZhan *special_logic = dynamic_cast<SpecialXianMengZhan*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_XIANMENGZHAN == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialHunyan * SceneManager::GetSpecialHunyan(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_HUN_YAN_FB);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialHunyan *special_logic = dynamic_cast<SpecialHunyan*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_HUN_YAN_FB == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialCrossMultiuserChallenge * SceneManager::GetSpecialCrossMultiuserChallenge(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_CROSS_MULTIUSER_CHALLENGE);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialCrossMultiuserChallenge *special_logic = dynamic_cast<SpecialCrossMultiuserChallenge*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_CROSS_MULTIUSER_CHALLENGE == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialCross1v1 * SceneManager::GetSpecialCross1v1Challenge(int fb_key, bool is_create, unsigned int time_out/* = 0*/)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_CROSS_1V1);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene, true, time_out);
		}

		if (NULL != scene)
		{
			SpecialCross1v1 *special_logic = dynamic_cast<SpecialCross1v1*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_CROSS_1V1 == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialGuildBoss * SceneManager::GetSpecialGuildBoss(int scene_id, int fb_key, bool is_create)
{
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialGuildBoss *special_logic = dynamic_cast<SpecialGuildBoss*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_GUILD_BOSS == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialYaoShouGuangChang * SceneManager::GetSpecialYaoShouGuangChang(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_YAOSHOU_GUANGCHANG);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialYaoShouGuangChang *special_logic = dynamic_cast<SpecialYaoShouGuangChang*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_YAOSHOU_GUANGCHANG == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialSuoYaoTa * SceneManager::GetSpecialSuoYaoTa(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_SUOYAOTA);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialSuoYaoTa *special_logic = dynamic_cast<SpecialSuoYaoTa*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_SUOYAOTA == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialShuiJing * SceneManager::GetSpecialShuiJing(int fb_key, bool is_create)
{
	int scene_id = LOGIC_CONFIG->GetShuiJingCfg().GetOtherCfg().scene_id;
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialShuiJing *special_logic = dynamic_cast<SpecialShuiJing*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_SHUIJING == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialCrossHotspring * SceneManager::GetSpecialCrossHotspring(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_HOTSPRING);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialCrossHotspring *special_logic = dynamic_cast<SpecialCrossHotspring*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_HOTSPRING == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialGuildBattleNew * SceneManager::GetSpecialGuildBattle(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_GUILDBATTLE);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialGuildBattleNew *special_logic = dynamic_cast<SpecialGuildBattleNew*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_GUILDBATTLE == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialXingzuoYiji * SceneManager::GetSpecialXingzuoYiji(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_XINGZUOYIJI);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialXingzuoYiji *special_logic = dynamic_cast<SpecialXingzuoYiji*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_XINGZUOYIJI == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialCombineServerBoss * SceneManager::GetSpecialCSABoss(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_CSA_BOSS);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialCombineServerBoss *special_logic = dynamic_cast<SpecialCombineServerBoss*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_CSA_BOSS == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}
SpecialCrossDarkNight * SceneManager::GetSpecialCrossDarkNight(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_CROSS_DARK_NIGHT);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialCrossDarkNight *special_logic = dynamic_cast<SpecialCrossDarkNight*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_CROSS_DARK_NIGHT == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialCrossFishing * SceneManager::GetSpecialCrossFishing(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_FISHING);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialCrossFishing *special_logic = dynamic_cast<SpecialCrossFishing*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_FISHING == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialNightFight * SceneManager::GetSpecialNightFight(int scene_id, int fb_key, bool is_create)
{
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialNightFight *special_logic = dynamic_cast<SpecialNightFight*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_NIGHT_FIGHT_FB == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialGuildQuestion * SceneManager::GetSpecialGuildQuestion(int scene_id, int fb_key, bool is_create)
{
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialGuildQuestion *special_logic = dynamic_cast<SpecialGuildQuestion*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_GUILD_QUESTION == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}
 
 	return NULL;
 }
 
SpecialMessBattle * SceneManager::GetSpecialMessBattle(int scene_id, int fb_key, bool is_create)
{
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialMessBattle *special_logic = dynamic_cast<SpecialMessBattle*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_MESS_BATTLE_FB == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialCrossBoss *SceneManager::GetSpecialCrossBoss(int scene_id, int fb_key, bool is_create)
{
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialCrossBoss *special_logic = dynamic_cast<SpecialCrossBoss*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_CROSS_BOSS == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialChallengeField *SceneManager::GetSpecialChallengeField(int scene_id, int fb_key)
{
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);

		if (NULL != scene)
		{
			SpecialChallengeField *special_logic = dynamic_cast<SpecialChallengeField*>(scene->GetSpecialLogic());
			if (NULL != special_logic && Scene::SCENE_TYPE_CHALLENGE_FIELD == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialCrossLocalChallengeField * SceneManager::GetSpecialCrossLocalChallengeField(int scene_id, int fb_key)
{
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);

		if (NULL != scene)
		{
			SpecialCrossLocalChallengeField *special_logic = dynamic_cast<SpecialCrossLocalChallengeField*>(scene->GetSpecialLogic());
			if (NULL != special_logic && Scene::SCENE_TYPE_CROSS_CHALLENGE_FIELD == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialArmorDefend * SceneManager::GetSpecialArmorDefend(int scene_id, int fb_key, bool is_create /*= false*/)
{
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialArmorDefend *special_logic = dynamic_cast<SpecialArmorDefend*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_ARMOR_DEFEND == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialCrossMiZangBoss *SceneManager::GetSpecialCrossMizangBoss(int scene_id, int fb_key, bool is_create)
{
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialCrossMiZangBoss *special_logic = dynamic_cast<SpecialCrossMiZangBoss*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_CROSS_MIZANG_BOSS == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialCrossYouMingBoss *SceneManager::GetSpecialCrossYoumingBoss(int scene_id, int fb_key, bool is_create)
{
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialCrossYouMingBoss *special_logic = dynamic_cast<SpecialCrossYouMingBoss*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_CROSS_YOUMING_BOSS == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialGodmagicBoss * SceneManager::GetSpeciaGodmagicBoss(int scene_id, int fb_key, bool is_create)
{
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialGodmagicBoss *special_logic = dynamic_cast<SpecialGodmagicBoss*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_GODMAGIC_BOSS == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

SpecialGiftHarvest * SceneManager::GetSpecialGiftHarvest(int fb_key, bool is_create)
{
	int scene_id = this->GetSceneIDBySceneType(Scene::SCENE_TYPE_GIFT_HARVEST);
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialLogic *special_logic = scene->GetSpecialLogic();
			if (Scene::SCENE_TYPE_GIFT_HARVEST == special_logic->GetSceneType())
			{
				return (SpecialGiftHarvest*)special_logic;
			}
		}
	}

	return NULL;
}

SpecialCrossHusongShuijing * SceneManager::GetSpcialCrossHusongShuijing(int scene_id, int fb_key, bool is_create)
{
	if (scene_id > 0)
	{
		Scene *scene = this->GetSceneById(scene_id, fb_key);
		if (NULL == scene)
		{
			if (is_create) this->CreateFB(scene_id, fb_key, &scene);
		}

		if (NULL != scene)
		{
			SpecialCrossHusongShuijing *special_logic = dynamic_cast<SpecialCrossHusongShuijing*>(scene->GetSpecialLogic());
			if (nullptr != special_logic && Scene::SCENE_TYPE_CROSS_HUSONG_SHUIJING == special_logic->GetSceneType())
			{
				return special_logic;
			}
		}
	}

	return NULL;
}

int SceneManager::GetNpcScenes(UInt16 npc_id, Scene **scene_array, int max_num)
{
	int n_npc_scene_count = 0;

	for (SceneList::Iterator it = m_scene_list.Beg(); it != m_scene_list.End() && n_npc_scene_count < max_num; ++it)
	{
		Scene *scene = *it;
		if (NULL != scene && 0 == scene->GetSceneKey() && NULL != scene->GetNPCList()->GetNpc(npc_id))
		{
			scene_array[n_npc_scene_count ++] = scene;
		}
	}

	return n_npc_scene_count;
}

void RMIRoleSaveBackObjImplRemote::RoleSaveRet(int ret)
{
	if (0 == ret) // 切换到远程场景
	{
		Protocol::SCChangeGS cgs;
		cgs.scene_id = target_scene_id;
		cgs.scene_key = target_scene_key;
		cgs.last_scene_id = scene_id;
		cgs.uid = UidToInt(uid);
		cgs.time = (unsigned int)EngineAdapter::Instance().Time();
		cgs.is_login = 0;
		GetSessionKey(InternalComm::Instance().m_session_key_base, target_scene_id, target_scene_key, scene_id, cgs.uid, 
			role_name, cgs.time, 0, pname, cgs.key);
		cgs.gs_index = gs_index;

		EngineAdapter::Instance().NetSend(netid, (const char *)&cgs, sizeof(Protocol::SCChangeGS));
	}
	else // 处理失败情况
	{
		Error(log_user_id, ERROR_EXECUTE, ret);
	}
}

bool SceneManager::GoTo(Role *role, int target_scene_id, int target_scene_key, const Posi &target_pos)
{
	if (NULL == role) return false;

	Scene *scene = role->GetScene();
	if (NULL == scene) return false;

	bool target_is_static_scene = Scene::IsStaticScene(this->GetSceneTypeWithID(target_scene_id)); // 是否前往普通场景
	if (!target_is_static_scene)
	{
		role->GetMultiMount()->MountOn(false); // 去非普通场景之前先下双人坐骑

		// 进入副本，并且进的不是组队本就退出跨服组队
		if (Scene::SCENE_TYPE_CROSS_TEAM_FB != this->GetSceneTypeWithID(target_scene_id))
		{
			role->GetRoleCross()->OnCrossTeamFBOption(CROSS_TEAM_FB_STATE_OPTION_QUIT, 0, 0, 0);
		}
	}

	MultiMountBindItem *bind_info = role->GetMultiMount()->GetMountOnInfo();
	if (NULL != bind_info && bind_info->owner_uid != role->GetUID() && target_is_static_scene)
	{
		// 非主人不给飞到普通场景
		return false;
	}

	// 双人坐骑的跟随者
	//Role *follower_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(bind_info->follower_uid));
	Role *follower_role = NULL;
	if (NULL != bind_info) 
	{
		follower_role = scene->GetRoleByUID(bind_info->follower_uid);
	}

	if (target_scene_key < 0)
	{
		SceneLineInfo *line_info = SceneRegister::Instance().Instance().GetSceneLineInfo(target_scene_id);
		if (NULL == line_info || line_info->max_line <= 0)
		{
			target_scene_key = 0;
		}
		else
		{
			for (int i = 0; i < line_info->open_line && i < 100; i++)
			{
				Scene *temp_scene = this->GetSceneById(target_scene_id, i);
				if (NULL != temp_scene && (int)temp_scene->RoleNum() < line_info->line_role_count)
				{
					target_scene_key = i;
					break;
				}
			}

			if (target_scene_key < 0)
			{
				if (line_info->open_line < line_info->max_line)
				{
					target_scene_key = line_info->open_line;
					line_info->open_line++;
				}
				else
				{
					target_scene_key = RandomNum(line_info->max_line);
				}
			}
		}
	}
	else if (target_is_static_scene && target_scene_key > 0)
	{
		SceneLineInfo *line_info = SceneRegister::Instance().Instance().GetSceneLineInfo(target_scene_id);
		if (NULL == line_info || target_scene_key >= line_info->open_line)
		{
			target_scene_key = 0;
		}
	}

	bool return_ret = true;

	if (scene->GetSceneID() == target_scene_id && scene->GetSceneKey() == target_scene_key)
	{
		if (NULL != follower_role) 
		{
			return_ret = follower_role->ResetPos(target_pos.x, target_pos.y);
		}

		return return_ret && role->ResetPos(target_pos.x, target_pos.y);
	}
	
	if (NULL != follower_role)
	{
		return_ret = this->ChangeScene(scene, follower_role->GetId(), target_scene_id, target_scene_key, target_pos);
	}

	return return_ret && this->ChangeScene(scene, role->GetId(), target_scene_id, target_scene_key, target_pos);
}

bool SceneManager::FlyByShoe(Role *role, int target_scene_id, int target_scene_key, int x, int y, int is_auto_buy, int is_force)
{
	CorrectionSceneCfg * correction_scene_cfg = LOGIC_CONFIG->GetFlyCorrectionConfig().GetCorrectionSceneCfg(target_scene_id);
	if (NULL != correction_scene_cfg)
	{
		if (0 == correction_scene_cfg->ignore_camp || role->GetCamp() != correction_scene_cfg->ignore_camp)
		{
			target_scene_id = correction_scene_cfg->correction_scene_id;
			x = correction_scene_cfg->correction_x;
			y = correction_scene_cfg->correction_y;
		}
	}

	if (NULL == role || !role->GetRoleStatusManager()->CanFlyByShoe(target_scene_id)) return false;

	{
		// 检查双人坐骑的另一半是否足够等级进入目标场景
		MultiMountBindItem *bind_info = role->GetMultiMount()->GetMountOnInfo();
		if (NULL != bind_info && bind_info->follower_uid != role->GetUID())
		{
			Role *follower_role = role->GetScene()->GetRoleByUID(bind_info->follower_uid);
			if (NULL != follower_role && !SceneRegister::Instance().CheckSceneLevelLimit(follower_role, target_scene_id))
			{
				follower_role->NoticeNum(errornum::EN_SCENE_TRANSPORT_LEVEL_LIMIT);
				role->NoticeNum(errornum::EN_FOLLOWER_LEVEL_NOT_ENOUGH);
				return false;
			}
		}
	}

	if (!SceneRegister::Instance().CheckSceneLevelLimit(role, target_scene_id)) 
	{
		role->NoticeNum(errornum::EN_SCENE_TRANSPORT_LEVEL_LIMIT);
		return false;
	}

	if (target_scene_key < 0)
	{
		if (NULL != role->GetScene() && role->GetScene()->GetSceneID() == target_scene_id)
		{
			target_scene_key = role->GetScene()->GetSceneKey();
		}
	}

	if (role->GetHusongTask()->IsHusongStatus())
	{
		target_scene_key = 0;
	}

	if (is_force == 0 && 0 == LOGIC_CONFIG->GetVipConfig().GetAuthParam(role->GetVip()->GetVipLevel(), VAT_TRANSFER))
	{
		static ItemExtern::ItemConsumeList consume_list; 
		consume_list.Reset();

		int flyshoe_item_id = LOGIC_CONFIG->GetOtherCfg().GetOtherCfg().flyshoe_item_id;

		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = flyshoe_item_id;
		stuff_list[0].num = 1;
		stuff_list[0].buyable = is_auto_buy != 0 ? true : false;

		if (!ItemExtern::GetConsumeListByOrder(role, false, 1, stuff_list, &consume_list, true))
		{
			return false;
		}

		// 消耗
		if (!ItemExtern::ConsumeItem(role, consume_list, "SceneManager::FlyByShoe"))
		{
			return false;
		}
	}

	this->GoTo(role, target_scene_id, target_scene_key, Posi(x,y));

	return true;
}

bool SceneManager::ChangeScene(Scene *scene, ObjID obj_id, int target_scene_id, int target_scene_key, const Posi &target_pos)
{
	Obj *obj = scene->GetObj(obj_id);
	if (NULL == obj || obj->GetObjType() != Obj::OBJ_TYPE_ROLE)
	{
		return false;
	}

	Role *role = (Role*)obj;

	Scene * target_scene = this->GetSceneById(target_scene_id, target_scene_key);
	if (NULL != target_scene) // scene 在本地
	{
		return this->ChangeLocalSceneHelper(scene, role, target_scene, target_pos);
	}
	else
	{
		Scene *fb_scene = NULL;
		bool ret = this->CreateFB(target_scene_id, target_scene_key, &fb_scene);
		if (ret)
		{
			return this->ChangeLocalSceneHelper(scene, role, fb_scene, target_pos);
		}
	}

	return false;
}

bool SceneManager::SaveRole(Role *role, int target_scene_id, const Posi &target_pos, RMIRoleSaveBackObject *rsbo)
{
	static RoleInitParam role_param;
	role->GetRoleInitParam(&role_param);

	if (role->GetScene()->InStaticScene())	// 只有普通场景才会保存到last_scene中
	{
		role_param.last_scene_id = role_param.scene_id;
		role_param.last_scene_x = role_param.scene_x;
		role_param.last_scene_y = role_param.scene_y;
	}

	role_param.scene_id = target_scene_id;
	role_param.scene_x = target_pos.x;
	role_param.scene_y = target_pos.y;

	static RoleOtherInitParam role_other_param;
	role->GetRoleOtherInitParam(&role_other_param);

	static ItemListParam itemlist_param;

	Knapsack *knapsack = role->GetKnapsack();
	int size = ItemNamespace::MAX_GRID_NUM;
	knapsack->GetChangeItemList(&size, &itemlist_param.item_list[0]);
	itemlist_param.count = size;

	EquipmentManager *equip_manager = role->GetEquipmentManager();
	size = ItemNamespace::MAX_GRID_NUM - size;
	equip_manager->GetChangeEquipmentList(&size, &itemlist_param.item_list[itemlist_param.count]);
	itemlist_param.count += size;

	HotkeyRecorder *hotkey = role->GetHotkeyRecorder();
	static HotkeyListParam hotkey_param;
	hotkey->GetChangeHotKeyList(&hotkey_param);

	FriendManager *friendmanager = role->GetFriendManager();
	static FriendListParam friends_param;
	friendmanager->GetChangeFriendList(&friends_param);

	static BlacklistsParam blacklists_param;
	friendmanager->GetChangeBlackList(&blacklists_param);

	static EnemyListParam enemy_param;
	friendmanager->GetChangeEnemyList(&enemy_param);
	
	static BusinessListParam busi; busi.count = 0;

	SkillManager *skill_manager = role->GetSkillManager();
	static SkillListParam skill_param;
	skill_manager->GetChangeSkillList(&skill_param);

	Welfare *welfare = role->GetWelfare();
	static DailyFindListParam daily_find_param;
	welfare->GetChangeDailyFindList(&daily_find_param);

	RMIRoleClient rc;
	rc.__bind_session(InternalComm::Instance().GetDBHandler().GetRoleDB());
	bool ret = rc.RoleSaveAsyn(UidToInt(role->GetUserId()), role_param, role_other_param, itemlist_param, hotkey_param, 
		friends_param, blacklists_param, enemy_param, busi, skill_param, daily_find_param, rsbo);
	if (ret)
	{
		knapsack->ClearDirtyMark();
		equip_manager->ClearDirtyMark();
		hotkey->ClearDirtyMark();
		friendmanager->ClearDirtyMark();
		skill_manager->ClearDirtyMark();
		welfare->ClearDirtyMark();
	}

	if (CrossConfig::Instance().IsHiddenServer())
	{
		static RoleAllParam all_param;
		all_param.Reset();

		all_param.role_param = role_param;
		all_param.other_param = role_other_param;
		all_param.item_param = itemlist_param;
		all_param.hotkey_param = hotkey_param;
		all_param.friend_param = friends_param;
		all_param.black_param = blacklists_param;
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
		cgsrd.is_logout = 0;
		cgsrd.uuid = UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
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

void SceneManager::OnCrossGameSyncRoleData(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameSyncRoleData *cgsrd = (crossgameprotocal::CrossGameSyncRoleData *)msg;

 	RMIRoleSaveBackObjectImplRole *rsbo = new RMIRoleSaveBackObjectImplRole();
 	rsbo->log_user_id = LongLongToUniqueUserID(cgsrd->uuid).user_id;
 
	static RoleAllParam role_all_param;
	role_all_param.Reset();
	{
		TLVUnserializer instream;
		instream.Reset(cgsrd->role_all_data, cgsrd->data_len);
		if (!role_all_param.Unserialize(instream, true))
		{
			gamelog::g_log_scenemanager.printf(LL_WARNING, "[SceneManager::OnCrossGameSyncRoleData RoleAllParam Unserialize fail] uuid:%lld", cgsrd->uuid);
			return;
		}
	}

 	RMIRoleClient rc;
 	rc.__bind_session(InternalComm::Instance().GetDBHandler().GetRoleDB());
 	rc.RoleSaveAsyn(UidToInt(rsbo->log_user_id), role_all_param.role_param, role_all_param.other_param, role_all_param.item_param, role_all_param.hotkey_param,
 		role_all_param.friend_param, role_all_param.black_param, role_all_param.enemy_param, role_all_param.business_param, role_all_param.skill_param, role_all_param.daily_find_param, rsbo, false);

	if (cgsrd->is_logout > 0 && m_cross_user_map.find(cgsrd->uuid) != m_cross_user_map.end())
	{
		m_cross_user_map.erase(cgsrd->uuid);

		// 私聊在线状态同步
		ChatManager::Instance().CheckChangeRoleOnline(cgsrd->uuid);
	}
}

void SceneManager::OnCrossGameChannelChat(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameChannelChat *cgcc = (crossgameprotocal::CrossGameChannelChat *)msg;

	ChatManager::Instance().OnCrossChannelChat(cgcc->channel_type, cgcc->role_camp, cgcc->guild_id, (const char *)cgcc + sizeof(crossgameprotocal::CrossGameChannelChat), cgcc->protocal_len);
}

void SceneManager::OnCrossGameRankInfoReq(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameRankInfoReq *cgrir = (crossgameprotocal::CrossGameRankInfoReq *)msg;

	RankManager::Instance().OnCrossRankInfoReq(cgrir);
}

void SceneManager::OnCrossGameSingleChat(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::CrossGameSingleChat *cgsc = (crossgameprotocal::CrossGameSingleChat *)msg;

	ChatManager::Instance().OnCrossSingleChat(cgsc->to_uuid, (const char *)cgsc + sizeof(crossgameprotocal::CrossGameSingleChat), length - sizeof(crossgameprotocal::CrossGameSingleChat));
}

void SceneManager::OnCrossGameLocalSpeaker(NetID net_id, const char *msg, int length)
{
//	crossgameprotocal::CrossGameSpeaker *cgs = (crossgameprotocal::CrossGameSpeaker *)msg;

	Protocol::SCSpeaker * sp = (Protocol::SCSpeaker *)(msg + sizeof(crossgameprotocal::CrossGameSpeaker));
	int sp_len = length - sizeof(crossgameprotocal::CrossGameSpeaker);

	if (sp->speaker_type == Protocol::CSSpeaker::SPEAKER_TYPE_CROSS)
	{
		//跨服消息,发送时 发起服会SendToAllGateway. 这里是指其他服,  接到消息只需发本地所有玩家即可
		World::GetInstWorld()->SendToAllGateway((const char *)sp, sp_len);
	}
	else 
	{
		//本服消息如果在跨服发起.跨服不会发出任何消息.  这里是本地服, 需发送所有本服的在线玩家(包括当前在跨服)
		World::GetInstWorld()->SendToAllGatewayAndCrossUser((const char *)sp, sp_len);
	}
	ChatManager::Instance().AddWorldChatBoard((const char *)sp, sp_len, true);
}

bool SceneManager::CreateFB(int fb_scene_id, int fb_scene_key, Scene **scene, bool check_key, unsigned int scene_timeout)
{
	if (check_key && 0 == fb_scene_key) return false; // FB 不允许使用0，规范化以减少错误发生的可能

	FBConfigList::iterator i = m_fb_config_list.find(fb_scene_id);
	if (i == m_fb_config_list.end()) return false;

	bool timeout_kick = i->second.config.is_timeout_kick_user; // 计时场景时间到是否踢人
	if (0 == scene_timeout)	// 如果非0则以外面传入为准 如果fb需要倒计时 则此处将scene_timeout赋值
	{
		scene_timeout = i->second.config.scene_timeout;
	}
	
	Scene *fb_scene = 0;
	bool ret = CreateScene(i->second.config, fb_scene_key, &fb_scene, i->second.scene_type, scene_timeout, timeout_kick);
	if (!ret) return false;

	*scene = fb_scene;

	return true;
}

bool SceneManager::DestroyFB(SceneIndex scene_index)
{
	Scene *scene = GetSceneByIndex(scene_index);
	if (NULL == scene)
	{
		return false;
	}

	if (0 != scene->RoleNum())
	{
		return false;
	}

	m_destroy_scene_list.push_back(scene_index);
	m_sceneidmap.erase(SceneIDKey(scene->GetSceneID(), scene->GetSceneKey()));

	return true;
}

bool SceneManager::UserExist(const UserID &user_id)
{
	return m_uid_to_rolelocal.find(user_id) != m_uid_to_rolelocal.end();
}

bool SceneManager::UserCrosserExist(const UserID &user_id)
{
	const int PLAT_TYPE = LocalConfig::Instance().GetPlatType();
	long long uuid = UniqueUserIDToLongLong(UniqueUserID(PLAT_TYPE, user_id));

	return m_cross_user_map.find(uuid) != m_cross_user_map.end();
}

bool SceneManager::UserCrosserExist(const long long uuid)
{
	return m_cross_user_map.find(uuid) != m_cross_user_map.end();
}

bool SceneManager::IsOnLine(long long uuid)
{
	bool is_online = true;
	std::set<Role*> role_list;
	this->GetAllRole(role_list, [uuid](Role *cmp_role) -> bool {
		return UniqueUserIDToLongLong(cmp_role->GetRoleCross()->GetCrossOriginalUniqueUserID()) == uuid;
	});

	std::map<long long, CrossUserInfo> &cross_user_map = this->GetCrossUserMap();
	auto cross_it = cross_user_map.find(uuid);

	if (role_list.empty() && (cross_it == cross_user_map.end()))
	{
		is_online = false;
	}

	return is_online;
}

int SceneManager::GetOnLineType(long long uuid)
{
	int online_type = ROLE_ONLINE_TYPE_OFF;
	std::set<Role*> role_list;
	this->GetAllRole(role_list, [uuid](Role *cmp_role) -> bool {
		return UniqueUserIDToLongLong(cmp_role->GetRoleCross()->GetCrossOriginalUniqueUserID()) == uuid;
	});

	std::map<long long, CrossUserInfo> &cross_user_map = this->GetCrossUserMap();
	auto cross_it = cross_user_map.find(uuid);

	if (!role_list.empty() && (cross_it == cross_user_map.end()) && !CrossConfig::Instance().IsHiddenServer())
	{
		UniqueUserID uuser_id = LongLongToUniqueUserID(uuid);
		ChatUser* chat_user = ChatManager::Instance().GetChatUser(UidToInt(uuser_id.user_id));
		if (chat_user)
		{
			online_type = ROLE_ONLINE_TYPE_ON;
		}
	}

	if (role_list.empty() && (cross_it != cross_user_map.end()))
	{
		online_type = ROLE_ONLINE_TYPE_CROSS;
	}

	return online_type;
}

void SceneManager::EraseTimeOut()
{
	struct TmpTimeoutRecd
	{
		TmpTimeoutRecd() : scene(NULL), obj_id(0) {}

		Scene *scene;
		ObjID obj_id;
	};

	TmpTimeoutRecd timeout_recorder[32];
	int timeout_count = 0;

	GSNetID disconnect_netid[32];
	Role *disconnect_role[32] = {NULL};
	int disconn_count = 0;

	for (NetSceneMap::iterator i = m_net_to_rolelocal.begin(); timeout_count < 32 && disconn_count < 32 && i != m_net_to_rolelocal.end(); ++i)
	{
		// 这里之前有严重的bug game_time有可能小于last_active_time 因为对时导致game_time变小 一减就变成负的 unsigned 就跪了
		if ((m_game_time > i->second.last_active_time) && (m_game_time - i->second.last_active_time > m_heartbeat_dead_time))
		{
			UserID user_id;

			Role *role = GetRole(i->second.scene_index, i->second.obj_id);
			if (NULL != role)
			{
				user_id = role->GetUserId();
			}

			gamelog::g_log_scenemanager.printf(LL_DEBUG, "EraseTimeOut user_id[%d, %d] obj_id:%d.", user_id.db_index, user_id.role_id, i->second.obj_id);

			GSNetID netid = i->first;
			if (i->second.scene_index != SceneIndex(-1))
			{
				SceneList::Iterator iter = m_scene_list.Find(i->second.scene_index);
				if (iter != m_scene_list.End())
				{
					timeout_recorder[timeout_count].scene = *iter;
					timeout_recorder[timeout_count].obj_id = i->second.obj_id;
					++ timeout_count;
				}
			}

			disconnect_netid[disconn_count] = netid;
			disconnect_role[disconn_count] = role;
			++ disconn_count;
		}
	}

	for (int i = 0; i < timeout_count; ++i)
	{
		this->Logout(timeout_recorder[i].scene, timeout_recorder[i].obj_id);
	}

	unsigned int kick_out_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = 0; i < disconn_count; ++i)
	{
		Role *role = disconnect_role[i];
		if (NULL != role)
		{
			static char reason_str[128] = {0};

			SNPRINTF(reason_str, sizeof(reason_str), "EraseTimeOut_%d_%u_%u", role->GetRoleStatusManager()->GetRoleActionStatus(), role->GetLastMsgFromClientTimestamp(), kick_out_time);

			EngineAdapter::Instance().NetDisconnect(disconnect_netid[i], role->GetUserId(), role->GetName(), NULL != role->GetScene() ? role->GetScene()->GetSceneID() : -1, reason_str);
		}
		else
		{
			EngineAdapter::Instance().NetDisconnect(disconnect_netid[i], INVALID_USER_ID, "null", -1, "EraseTimeOut");
		}
	}

	for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
	{
		Scene *scene = *i;
		scene->CheckRoleTimeOut(m_game_time);
	}
}

void SceneManager::RoleTimeOut(Scene *scene, Role *role, const char *reason, int param1)
{
	gamelog::g_log_debug.buff_printf("[Scene::CheckRoleTimeOut Error][name:%s UserID(%d, %d) SceneIndex:%d ObjID:%d param1:%d reason %s]", 
			role->GetName(), role->GetUserId().db_index, role->GetUserId().role_id, (int)scene->GetIndex(), (int)role->GetId(), param1, reason);

	UIDSceneMap::iterator uid_i = m_uid_to_rolelocal.find(role->GetUserId());
	if (uid_i != m_uid_to_rolelocal.end())
	{
		gamelog::g_log_debug.buff_printf("[m_uid_to_rolelocal SceneIndex:%d ObjID:%d]", (int)uid_i->second.scene_index, (int)uid_i->second.obj_id);
	}
	else
	{
		gamelog::g_log_debug.buff_printf("[m_uid_to_rolelocal can NOT find]");
	}

	NetSceneMap::iterator net_i = m_net_to_rolelocal.find(role->GetNetId());
	if (net_i != m_net_to_rolelocal.end())
	{
		gamelog::g_log_debug.buff_printf("[m_net_to_rolelocal  SceneIndex:%d ObjID:%d]", (int)net_i->second.scene_index, (int)net_i->second.obj_id);
	}
	else
	{
		gamelog::g_log_debug.buff_printf("[m_net_to_rolelocal can NOT find]");
	}
	gamelog::g_log_debug.commit_buff(LL_ERROR);

	this->Logout(scene, role->GetId());

	EngineAdapter::Instance().NetDisconnect(role->GetNetId(), role->GetUserId(), role->GetName(), scene->GetSceneID(), reason);
}

void SceneManager::EraseSpeedup()
{
	for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
	{
		Scene *scene = *i;
		scene->CheckRoleSpeedup();
	}
}

void SceneManager::AddGS(int scene, int index)
{
	GS gs;
	gs.index = index;
	m_remote_gs_list[scene] = gs;
}

void SceneManager::DeleteGS(int scene)
{
	m_remote_gs_list.erase(scene);
}

bool SceneManager::GetGS(int scene, int *index)
{
	RemoteGSList::iterator i = m_remote_gs_list.find(scene);
	if (i == m_remote_gs_list.end())
	{
		return false;
	}

	*index = i->second.index;

	return true;
}

bool SceneManager::SendMessageToGameWorld(const UserID &user_id, const char *data, int len, bool not_find_sendtoG)
{
	UIDSceneMap::iterator iter = m_uid_to_rolelocal.find(user_id);
	if (iter != m_uid_to_rolelocal.end())
	{
		SceneList::Iterator scene_iter = m_scene_list.Find(iter->second.scene_index);
		if (scene_iter != m_scene_list.End())
		{
			m_message_handler.HandleUIDMessage(user_id, (*scene_iter), iter->second.obj_id, data, len);
			return true;
		}
		else
		{
			gamelog::g_log_scenemanager.printf(LL_WARNING, "SendMessageTo: SceneId:%d NOT found.", iter->second.scene_index);
			return false;
		}
	}

	return false;
}

bool SceneManager::SendToUser(const UserID &user_id, const char *data, int length)
{
	Role *role = this->GetRole(user_id);
	if (NULL == role)
	{
		return false;
	}

	bool ret = EngineAdapter::Instance().NetSend(role->GetNetId(), data, length);
	return ret;
}

bool SceneManager::SendToUserByGlobal(const UserID &user_id, const char *data, int length)
{
	if (length > 6844) return false;

	gglobalprotocal::GGlobalMsgToUser um;
	um.db_index = user_id.db_index;
	um.role_id = user_id.role_id;

	static char msg_tmp[6144 + sizeof(gglobalprotocal::GGlobalMsgToUser)] = {0};
	gglobalprotocal::GGlobalMsgToUser *p_msg = (gglobalprotocal::GGlobalMsgToUser *)msg_tmp;
	*p_msg = um;
	memcpy(msg_tmp + sizeof(gglobalprotocal::GGlobalMsgToUser), data, length);

	return InternalComm::Instance().NetSend(InternalComm::Instance().m_golbal_server_id, msg_tmp, length + sizeof(gglobalprotocal::GGlobalMsgToUser));
}

void SceneManager::SendToAllUser(const char *data, unsigned int length, std::function<bool(Role*)> filter)
{
	std::set<Role*> role_list;
	this->GetAllRole(role_list);

	for (auto role : role_list)
	{
		if (filter && !filter(role))
		{
			continue;
		}

		EngineAdapter::Instance().NetSend(role->GetNetId(), data, length);
	}
}

void SceneManager::SendToVipPhaseUser(const char *msg, int length, int vip_level_min, int vip_level_max)
{
	this->SendToAllUser(msg, length, [vip_level_min, vip_level_max](Role *role) -> bool
	{
		int role_vip_level = role->GetVip()->GetVipLevel();
		return role_vip_level >= vip_level_min && role_vip_level <= vip_level_max;
	});
}

void SceneManager::GetAllRole(std::set<Role*> &role_list, std::function<bool(Role*)> filter)
{
	for (auto it = m_uid_to_rolelocal.begin(); it != m_uid_to_rolelocal.end(); ++it)
	{
		Role *role = this->GetRole(it->first);
		if (NULL != role)
		{
			if (filter && !filter(role))
			{
				continue;
			}

			role_list.insert(role);
		}
	}
}

bool SceneManager::NoticeToUser(const UserID &user_id, int notice_num)
{
	Role *role = this->GetRole(user_id);
	if (NULL == role)
	{
		return false;
	}

	role->NoticeNum(notice_num);
	return true;
}

Obj * SceneManager::GetObj(SceneIndex scene_index, ObjID obj_id)
{
	SceneList::Iterator iter = m_scene_list.Find(scene_index);
	if (iter != m_scene_list.End())
	{
		return (*iter)->GetObj(obj_id);
	}

	return NULL;
}

Role * SceneManager::GetRole(SceneIndex scene_index, ObjID obj_id)
{
	SceneList::Iterator iter = m_scene_list.Find(scene_index);
	if (iter != m_scene_list.End())
	{
		Obj *obj = (*iter)->GetObj(obj_id);
		if (NULL != obj && Obj::OBJ_TYPE_ROLE == obj->GetObjType())
		{
			return (Role*)obj;
		}
	}

	return NULL;
}

Role * SceneManager::GetRole(const UserID &user_id)
{
	if (INVALID_USER_ID == user_id) return NULL; 

	UIDSceneMap::iterator iter = m_uid_to_rolelocal.find(user_id);
	if (iter != m_uid_to_rolelocal.end())
	{
		SceneList::Iterator scene_iter = m_scene_list.Find(iter->second.scene_index);
		if (scene_iter != m_scene_list.End())
		{
			Role *role = MessageHandler::GetRoleHelper((*scene_iter), iter->second.obj_id, "MessageHandler::HandleUIDMessage");
			if (NULL != role && role->GetUserId() == user_id)
			{
				return role;
			}
		}
	}

	return NULL;
}

Role * SceneManager::GetRoleByUUID(long long uuid)
{
	for (SceneList::Iterator iter = m_scene_list.Beg(); iter != m_scene_list.End(); ++ iter)
	{
		Scene *scene = *iter;
		for (int i = 0; i < (int)scene->RoleNum(); ++ i)
		{
			Role *role = scene->GetRoleByIndex(i);
			if (NULL != role && UniqueUserIDToLongLong(role->GetRoleCross()->GetCrossOriginalUniqueUserID()) == uuid)
			{
				return role;
			}
		}
	}

	return NULL;
}

Role * SceneManager::GetRole(GameName role_name)
{
	for (SceneList::Iterator iter = m_scene_list.Beg(); iter != m_scene_list.End(); ++ iter)
	{
		Scene *scene = *iter;
		for (int i = 0; i < (int)scene->RoleNum(); ++ i)
		{
			Role *role = scene->GetRoleByIndex(i);
			if (NULL != role && 0 == strcmp(role_name, role->GetName()))
			{
				return role;
			}
		}
	}

	return NULL;
}

void SceneManager::OnCrossServerRecv(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::MessageHeader *header = (crossgameprotocal::MessageHeader*)msg;

	int msg_type_index = header->msg_type - crossgameprotocal::MT_CROSS_GAME_BEGIN;
	if (msg_type_index < 0 || msg_type_index >= (int)sizeof(m_on_cross_handler) / (int)sizeof(m_on_cross_handler[0]) ||
		NULL == m_on_cross_handler[msg_type_index])
	{
		return;
	}

	(this->*(m_on_cross_handler[msg_type_index]))(net_id, (const char *)header, length);
}

void SceneManager::OnChatServerRecv(NetID net_id, const char *msg, int length)
{
	gcprotocal::MessageHeader *header = (gcprotocal::MessageHeader*)msg;

	if (gcprotocal::MT_SINGLE_CHAT_MONITOR == header->msg_type)
	{
		gcprotocal::SingleChatMonitor *single_chat_monitor = (gcprotocal::SingleChatMonitor*)msg;

		int sendlen = sizeof(gcprotocal::SingleChatMonitor) - (MAX_CHAT_LENGTH - single_chat_monitor->msg_length);
		if (sendlen != length) return;

		UserID user_id = IntToUid(single_chat_monitor->uid);

		Role *role = this->GetRole(user_id);
		if (NULL != role)
		{
			role->OnSingleChatMonitor(single_chat_monitor->msg_time, single_chat_monitor->msg_length, single_chat_monitor->msg);
		}
	}
	else if (gcprotocal::MT_WORLD_CHAT_MONITOR == header->msg_type)
	{
		gcprotocal::WorldChatMonitor *world_chat_monitor = (gcprotocal::WorldChatMonitor*)msg;

		int sendlen = sizeof(gcprotocal::WorldChatMonitor) - (MAX_CHAT_LENGTH - world_chat_monitor->msg_length);
		if (sendlen != length) return;

		UserID user_id = IntToUid(world_chat_monitor->uid);

		Role *role = this->GetRole(user_id);
		if (NULL != role)
		{
			role->OnWorldChatMonitor(world_chat_monitor->msg_time, world_chat_monitor->msg_length, world_chat_monitor->msg);
		}
	}
	else if (gcprotocal::MT_GUILD_CHAT_MONITOR == header->msg_type)
	{
		gcprotocal::GuildChatMonitor *guild_chat_monitor = (gcprotocal::GuildChatMonitor*)msg;

		int sendlen = sizeof(gcprotocal::GuildChatMonitor) - (MAX_CHAT_LENGTH - guild_chat_monitor->msg_length);
		if (sendlen != length) return;

		UserID user_id = IntToUid(guild_chat_monitor->uid);

		Role *role = this->GetRole(user_id);
		if (NULL != role)
		{
			role->OnGuildChatMonitor(guild_chat_monitor->msg_time, guild_chat_monitor->msg_length, guild_chat_monitor->msg);
		}
	}
}

void SceneManager::OnGlobalUserMsg(NetID net_id, const char *msg, int length)
{
	gglobalprotocal::GGlobalUserMsg *um = (gglobalprotocal::GGlobalUserMsg*)msg;
	this->SendMessageToGameWorld(UserID(um->db_index, um->role_id), msg + sizeof(gglobalprotocal::GGlobalUserMsg), length - sizeof(gglobalprotocal::GGlobalUserMsg), false);
}

class SceneLogTimer : public ITimerCallback
{
public:
	SceneManager *scene_manager;

	SceneLogTimer() : scene_manager(NULL) {}
	virtual ~SceneLogTimer(){}

	virtual void OnTimer()
	{
		scene_manager->StartSceneStatusLog();
	}

	virtual void Free()
	{
		delete this;
	}
};

void SceneManager::StartSceneStatusLog()
{
	gamelog::g_log_scenestatus.buff_printf("[SceneManager::StartSceneStatusLog Succ][scene_count:%d scene_id:obj:role:monster:pet:drop_item[", (int)m_scene_list.Size());
	for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
	{
		int obj_num = 0, role_num = 0, monster_num = 0, pet_num = 0, drop_item_num = 0;
		(*i)->GetSceneStatus(&obj_num, &role_num, &monster_num, &pet_num, &drop_item_num);
		gamelog::g_log_scenestatus.buff_printf(" %d:%d:%d:%d:%d:%d",(*i)->GetSceneID(), obj_num, role_num, monster_num, pet_num, drop_item_num);
	}
	gamelog::g_log_scenestatus.buff_printf("]]");
	gamelog::g_log_scenestatus.commit_buff(LL_INFO);

	SceneLogTimer *timer = new SceneLogTimer();
	timer->scene_manager = this;

	static const int LOG_INTERVAL = (int)ServerConfigPool::Instance().common_config.game_world_scene_status_log_interval_s;
	EngineAdapter::Instance().CreateTimerSecond(LOG_INTERVAL, timer);
}

bool SceneManager::CreateSystemMsg(Protocol::SCSystemMsg *sm, int *sendlen, const char *msg, int length, int msg_type, int color, int display_pos)
{
	if (length < 0 || length > PROTOCAL_MAX_SYSTEM_MSG_LENGTH || NULL == sm || NULL == sendlen) return false;

	sm->send_time = (unsigned int)EngineAdapter::Instance().Time();
	sm->msg_type = (short)msg_type;
	sm->msg_length = (short)length;
	memcpy(sm->msg, msg, length);
	sm->color = 0;
	sm->display_pos = 0;

	*sendlen = sizeof(Protocol::SCSystemMsg) - PROTOCAL_MAX_SYSTEM_MSG_LENGTH + length;

	return true;
}

bool SceneManager::SystemMsgToGuild(int guild_id, const char *msg, int length, int msg_type, int color, int display_pos)
{
	if (INVALID_GUILD_ID == guild_id)
	{
		return false;
	}

	Guild *guild = GuildManager::Instance().GetGuild(guild_id);
	if (NULL == guild)
	{
		return false;
	}

	static Protocol::SCSystemMsg sm;
	int sendlen = 0;

	if (this->CreateSystemMsg(&sm, &sendlen, msg, length, msg_type, color, display_pos))
	{
		return guild->GetGuildMemberManager().OnSystemMsg((const char *)&sm, sendlen);	
	}

	return false;
}

bool SceneManager::SystemMsgToCamp(int camp_type, const char *msg, int length, int msg_type, int color, int display_pos)
{
	if (camp_type <= CAMP_TYPE_INVALID || camp_type >= CAMP_TYPE_MAX)
	{
		return false;
	}

	Channel *channel = ChatManager::Instance().GetChatChannel(chatdef::CHANNEL_ID_CAMP_BEGIN + camp_type); // 频道是否存在
	if (NULL == channel)
	{
		return false;
	}

	static Protocol::SCSystemMsg sm;
	int sendlen = 0;

	if (this->CreateSystemMsg(&sm, &sendlen, msg, length, msg_type, color, display_pos))
	{
		channel->OnChannelSystemMsg((const char *)&sm, sendlen);
		return true;
	}

	return false;
}

bool SceneManager::SystemMsgThread(const char *msg, int length, int msg_type, int color, int display_pos)
{
	static Protocol::SCSystemMsg sm;

	int sendlen = 0;

	if (this->CreateSystemMsg(&sm, &sendlen, msg, length, msg_type, color, display_pos))
	{
		for (CrossUserMapIt it = m_cross_user_map.begin(); it != m_cross_user_map.end(); it++)
		{
			InternalComm::Instance().UserProtocalTransferToGame(it->first, (const char*)&sm, sendlen);
		}

		return m_world->SendToAllGateway((const char*)&sm, sendlen);
	}

	return false;
}

bool SceneManager::SystemMsgToAllGameWorld(const char *msg, int length, int msg_type, int color, int display_pos)
{
	static Protocol::SCSystemMsg sm;

	int sendlen = 0;

	if (this->CreateSystemMsg(&sm, &sendlen, msg, length, msg_type, color, display_pos))
	{
		InternalComm::Instance().UserProtocalTransferToGame(0, (const char*)&sm, sendlen);

		if (!CrossConfig::Instance().IsHiddenServer())
		{
			return m_world->SendToAllGateway((const char*)&sm, sendlen);
		}
		else
		{
			return true;
		}
	}

	return false;
}

void SceneManager::OnReloadConfig(const char *msg, int length)
{
	gglobalprotocal::GSystemCmdReload *gscr = (gglobalprotocal::GSystemCmdReload*)msg;
	ReloadConfigManager::Instance().ReloadConfig(gscr->config_type, gscr->param1, gscr->param2);
}

void SceneManager::OnSyncGuildInfo(NetID net_id, const char *msg, int length)
{
	
}

void SceneManager::OnSyncWorldLevel(NetID net_id, const char *msg, int length)
{
	gglobalprotocal::SyncWorldLevelInfo *wli = (gglobalprotocal::SyncWorldLevelInfo*)msg;
	WorldShadow::Instance().OnSyncWorldLevelInfo(wli);
}

void SceneManager::OnForbidSublock(const char *msg, int length)
{
	m_is_forbid_sublock = true;
}

void SceneManager::OnForceSetMicroPcFlag(NetID net_id, const char *msg, int length)
{
	gglobalprotocal::ForceSetMicroPcFlag *fsmpc = (gglobalprotocal::ForceSetMicroPcFlag *)msg;

	m_is_micro_pc_open = (0 != fsmpc->is_micro_pc_open);
}

void SceneManager::OnForceSetAutoForbid(NetID net_id, const char *msg, int length)
{
	gglobalprotocal::ForceSetAutoForbid *fsaf = (gglobalprotocal::ForceSetAutoForbid *)msg;

	m_is_monitor_auto_forbid = (0 != fsaf->is_auto_forbid);
}

void SceneManager::OnSyncMarryInfo(NetID net_id, const char *msg, int length)
{
	gglobalprotocal::MarryInfo *mi = (gglobalprotocal::MarryInfo*)msg;

	MarryShadow::Instance().OnSyncMarryInfo(mi->marry_info);
}

void SceneManager::OnCmdToWorldCount(const char *msg, int length)
{
	gglobalprotocal::SystemCmdToWorldCount *sctwc = (gglobalprotocal::SystemCmdToWorldCount*)msg;

	if (sctwc->count_type <= systemcmd::RCT_INVALID || sctwc->count_type >= systemcmd::RCT_MAX) return;

	for (SceneList::Iterator it = m_scene_list.Beg(); it != m_scene_list.End(); ++it)
	{
		Scene *scene = *it;

		int role_num = scene->RoleNum();
		for (int i = 0; i < role_num; ++i)
		{
			Role *role = scene->GetRoleByIndex(i);
			if (NULL != role)
			{
				CmdHandler::Instance().OnCmdToRoleCount(role, sctwc->count_type, sctwc->count, true);
			}
		}
	}

	gamelog::g_log_scenestatus.printf(LL_INFO, "[OnCmdToWorldCount, count_type:%d, count:%d]", sctwc->count_type, sctwc->count);
}

void SceneManager::OnCmdToWorldFbCount(const char *msg, int length)
{
	gglobalprotocal::SystemCmdToWorldFbCount *sctwfc = (gglobalprotocal::SystemCmdToWorldFbCount*)msg;

	if (sctwfc->day_count_id < DayCounter::DAYCOUNT_ID_FB_START || sctwfc->day_count_id > DayCounter::DAYCOUNT_ID_FB_END) return;

	for (SceneList::Iterator it = m_scene_list.Beg(); it != m_scene_list.End(); ++it)
	{
		Scene *scene = *it;

		int role_num = scene->RoleNum();
		for (int i = 0; i < role_num; ++i)
		{
			Role *role = scene->GetRoleByIndex(i);
			if (NULL != role)
			{
				CmdHandler::Instance().OnCmdToRoleFbCount(role, sctwfc->day_count_id, sctwfc->count, true);
			}
		}
	}

	gamelog::g_log_scenestatus.printf(LL_INFO, "[OnCmdToWorldCount, day_count_id:%d, count:%d]", sctwfc->day_count_id, sctwfc->count);
}

void SceneManager::OnSyncActivityStatus(ActivityStatus *as)
{
	ActivityShadow::Instance().OnSyncActivityStatus(as);

	{
		// 通用活动状态切换
		if (SPECIAL_ACTIVITY_TYPE_COMMON_ACTIVITY == as->activity_type && ACTIVITY_STATUS_OPEN == as->status)
		{
			for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
			{
				Scene *scene = *i;
				if (NULL == scene) continue;

				for (int j = 0; j < (int)scene->RoleNum(); ++ j)
				{
					Role *temp_role = scene->GetRoleByIndex(j);
					if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
					{
						temp_role->GetCommonActivityUnitMgr()->CheckJoinInSameActivity(true);
					}
				}
			}
		}
	}

	{
		// 跨服随机活动状态切换
		if (as->activity_type >= CROSS_RAND_ACTIVITY_TYPE_BEGIN && as->activity_type < CROSS_RAND_ACTIVITY_TYPE_END)
		{
			for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
			{
				Scene *scene = *i;
				if (NULL == scene) continue;

				for (int j = 0; j < (int)scene->RoleNum(); ++j)
				{
					Role *temp_role = scene->GetRoleByIndex(j);
					if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
					{
						if (ACTIVITY_STATUS_OPEN == as->status)
						{
							temp_role->GetRoleCrossRandActivity()->OnActivityOpen(as->activity_type);
						}
						else if (ACTIVITY_STATUS_CLOSE == as->status)
						{
							temp_role->GetRoleCrossRandActivity()->OnActivityClose(as->activity_type);
						}
					}
				}
			}
		}
	}

	{
		if (RAND_ACTIVITY_TYPE_LOGIN_GIFT == as->activity_type && ACTIVITY_STATUS_OPEN == as->status)
		{
			for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
			{
				Scene *scene = *i;
				if (NULL == scene) continue;

				for (int j = 0; j < (int)scene->RoleNum(); ++ j)
				{
					Role *temp_role = scene->GetRoleByIndex(j);
					if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
					{
						temp_role->GetRoleActivity()->OnRALoginGiftLogin();
					}
				}
			}
		}
	}

	{
		if (RAND_ACTIVITY_TYPE_LOGIN_GIFT_0 == as->activity_type && ACTIVITY_STATUS_OPEN == as->status)
		{
			for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
			{
				Scene *scene = *i;
				if (NULL == scene) continue;

				for (int j = 0; j < (int)scene->RoleNum(); ++j)
				{
					Role *temp_role = scene->GetRoleByIndex(j);
					if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
					{
						temp_role->GetRoleActivity()->OnRALoginGiftLogin_0();
					}
				}
			}
		}
	}

	if (SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER == as->activity_type && ACTIVITY_STATUS_OPEN == as->status)
	{
		for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
		{
			Scene *scene = *i;
			if (NULL == scene) continue;

			for (int j = 0; j < (int)scene->RoleNum(); ++ j)
			{
				Role *temp_role = scene->GetRoleByIndex(j);
				if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
				{
					temp_role->GetRoleActivity()->OnCSALoginGiftLogin();
				}
			}
		}
	}

	if (SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER == as->activity_type)
	{
		if (ACTIVITY_STATUS_OPEN == as->status)
		{
			for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
			{
				Scene *scene = *i;
				if (NULL != scene)
				{
					int role_num = scene->RoleNum();
					for (int j = 0; j < role_num; ++ j)
					{
						Role *temp_role = scene->GetRoleByIndex(j);
						if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
						{
							temp_role->GetRoleActivity()->OnCombineServerActivityOpen();
						}
					}
				}
			}
		}
	}

	if (ACTIVITY_TYPE_XINGZUOYIJI == as->activity_type)
	{
		if (ACTIVITY_STATUS_OPEN == as->status)
		{
			for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
			{
				Scene *scene = *i;
				if (NULL != scene)
				{
					int role_num = scene->RoleNum();
					for (int j = 0; j < role_num; ++j)
					{
						int fb_key = ActivityShadow::Instance().GetFBKeyByRoomIndex(0);
						SpecialXingzuoYiji *xingzuoyiji_logic = this->GetSpecialXingzuoYiji(fb_key, true);

						Role *temp_role = scene->GetRoleByIndex(j);
						if (NULL != temp_role && NULL != xingzuoyiji_logic && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
						{
							xingzuoyiji_logic->SendtoRoleNotInScene(temp_role);
						}
					}
				}
			}
		}
	}

	//add by wwd
	{
		if ((RAND_ACTIVITY_TYPE_BEGIN < as->activity_type && as->activity_type < RAND_ACTIVITY_TYPE_END) && (ACTIVITY_STATUS_OPEN == as->status))
		{
			for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
			{
				Scene *scene = *i;
				if (NULL != scene)
				{
					int role_num = scene->RoleNum();
					for (int j = 0; j < role_num; ++j)
					{
						Role *temp_role = scene->GetRoleByIndex(j);
						if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
						{
							temp_role->GetRoleActivity()->CheckRandActivity(as->activity_type);
						}
					}
				}
			}
		}
	}
	if (RAND_ACTIVITY_TYPE_HUANLE_YAOJIANG2 == as->activity_type)
	{
		if (ACTIVITY_STATUS_OPEN == as->status)
		{
			for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
			{
				Scene *scene = *i;
				if (NULL != scene)
				{
					int role_num = scene->RoleNum();
					for (int j = 0; j < role_num; ++j)
					{
						Role *temp_role = scene->GetRoleByIndex(j);
						if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
						{
							temp_role->GetRoleActivity()->SendRAHuanLeYaoJiang2Info();
						}
					}
				}
			}
		}
	}

	if (RAND_ACTIVITY_TYPE_COUPLE_HALO_SELL == as->activity_type)
	{
		if (ACTIVITY_STATUS_OPEN == as->status)
		{
			for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
			{
				Scene *scene = *i;
				if (NULL != scene)
				{
					int role_num = scene->RoleNum();
					for (int j = 0; j < role_num; ++j)
					{
						Role *temp_role = scene->GetRoleByIndex(j);
						if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
						{
							temp_role->GetQingyuan()->CheckTejiaHaloActivity(false, 0);
						}
					}
				}
			}
		}
	}

	if (ACTIVITY_TYPE_GUILD_QUESTION == as->activity_type)
	{
		if (ACTIVITY_STATUS_OPEN == as->status)
		{
			std::set<GuildID> guild_notice_set;
			guild_notice_set.clear();
			for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
			{
				Scene *scene = *i;
				if (NULL != scene)
				{
					int role_num = scene->RoleNum();
					for (int j = 0; j < role_num; ++j)
					{
						Role *temp_role = scene->GetRoleByIndex(j);
						if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
						{
							//江湖传闻
							GuildID guild_id = temp_role->GetGuildID();
							if (INVALID_GUILD_ID != guild_id)
							{
								if (guild_notice_set.find(guild_id) == guild_notice_set.end())
								{
									EventHandler::Instance().OnAddGuildRumorEvent(guild_id, NULL, NULL, GUILD_EVENT_TYPE_RUMOR_GUILD_QUESTION);

									guild_notice_set.insert(guild_id);
								}
							}
						}
					}
				}
			}
		}
	}

	if (ACTIVITY_TYPE_GONGCHENGZHAN == as->activity_type)
	{
		if (ACTIVITY_STATUS_OPEN == as->status)
		{
			std::set<GuildID> guild_notice_set;
			guild_notice_set.clear();
			for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
			{
				Scene *scene = *i;
				if (NULL != scene)
				{
					int role_num = scene->RoleNum();
					for (int j = 0; j < role_num; ++j)
					{
						Role *temp_role = scene->GetRoleByIndex(j);
						if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
						{
							//江湖传闻
							GuildID guild_id = temp_role->GetGuildID();
							if (INVALID_GUILD_ID != guild_id)
							{
								if (guild_notice_set.find(guild_id) == guild_notice_set.end())
								{
									EventHandler::Instance().OnAddGuildRumorEvent(guild_id, NULL, NULL, GUILD_EVENT_TYPE_RUMOR_GONGCHENG_START);

									guild_notice_set.insert(guild_id);
								}
							}
						}
					}
				}
			}
		}
	}

	if (CROSS_ACTIVITY_TYPE_CROSS_GUILDBATTLE == as->activity_type)
	{
		if (ACTIVITY_STATUS_OPEN == as->status)
		{
			std::set<GuildID> guild_notice_set;
			guild_notice_set.clear();
			for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
			{
				Scene *scene = *i;
				if (NULL != scene)
				{
					int role_num = scene->RoleNum();
					for (int j = 0; j < role_num; ++j)
					{
						Role *temp_role = scene->GetRoleByIndex(j);
						if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
						{
							//江湖传闻
							GuildID guild_id = temp_role->GetGuildID();
							if (INVALID_GUILD_ID != guild_id)
							{
								if (guild_notice_set.find(guild_id) == guild_notice_set.end())
								{
									EventHandler::Instance().OnAddGuildRumorEvent(guild_id, NULL, NULL, GUILD_EVENT_TYPE_RUMOR_CROSS_GUILD_BATTLE);

									guild_notice_set.insert(guild_id);
								}
							}
						}
					}
				}
			}
		}
	}

	if (ACTIVITY_TYPE_GUILDBATTLE == as->activity_type)
	{
		if (ACTIVITY_STATUS_OPEN == as->status)
		{
			std::set<GuildID> guild_notice_set;
			guild_notice_set.clear();
			for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
			{
				Scene *scene = *i;
				if (NULL != scene)
				{
					int role_num = scene->RoleNum();
					for (int j = 0; j < role_num; ++j)
					{
						Role *temp_role = scene->GetRoleByIndex(j);
						if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
						{
							//江湖传闻
							GuildID guild_id = temp_role->GetGuildID();
							if (INVALID_GUILD_ID != guild_id)
							{
								if (guild_notice_set.find(guild_id) == guild_notice_set.end())
								{
									EventHandler::Instance().OnAddGuildRumorEvent(guild_id, NULL, NULL, GUILD_EVENT_TYPE_RUMOR_GUILD_QIANGQI);

									guild_notice_set.insert(guild_id);
								}
							}
						}
					}
				}
			}
		}
	}

	if (ACTIVITY_TYPE_GUILD_YUNBIAO == as->activity_type)
	{
		if (ACTIVITY_STATUS_OPEN == as->status)
		{
			std::set<GuildID> guild_notice_set;
			guild_notice_set.clear();
			for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
			{
				Scene *scene = *i;
				if (NULL != scene)
				{
					int role_num = scene->RoleNum();
					for (int j = 0; j < role_num; ++j)
					{
						Role *temp_role = scene->GetRoleByIndex(j);
						if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
						{
							//江湖传闻
							GuildID guild_id = temp_role->GetGuildID();
							if (INVALID_GUILD_ID != guild_id)
							{
								if (guild_notice_set.find(guild_id) == guild_notice_set.end())
								{
									EventHandler::Instance().OnAddGuildRumorEvent(guild_id, NULL, NULL, GUILD_EVENT_TYPE_RUMOR_GUILD_YUNBIAO);

									guild_notice_set.insert(guild_id);
								}
							}
						}
					}
				}
			}
		}
	}

	if (ACTIVITY_TYPE_GUILD_FB == as->activity_type)
	{
		if (ACTIVITY_STATUS_OPEN == as->status)
		{
			std::set<GuildID> guild_notice_set;
			guild_notice_set.clear();
			for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
			{
				Scene *scene = *i;
				if (NULL != scene)
				{
					int role_num = scene->RoleNum();
					for (int j = 0; j < role_num; ++j)
					{
						Role *temp_role = scene->GetRoleByIndex(j);
						if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
						{
							//江湖传闻
							GuildID guild_id = temp_role->GetGuildID();
							if (INVALID_GUILD_ID != guild_id)
							{
								if (guild_notice_set.find(guild_id) == guild_notice_set.end())
								{
									EventHandler::Instance().OnAddGuildRumorEvent(guild_id, NULL, NULL, GUILD_EVENT_TYPE_RUMOR_GUILD_FB);

									guild_notice_set.insert(guild_id);
								}
							}
						}
					}
				}
			}
		}
	}
	// end

	// 累计充值活动切换
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_TOTAL_CHARGE, as, RACheckTotalChargeGiveReward);		
	// 消费返利活动切换
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_CONSUME_GOLD_FANLI, as, RACheckConsumeGoldFanliGiveReward);
	// 累计消费
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_TOTAL_CONSUME_GOLD, as, RACheckTotalConsumeGoldGiveReward);
	// BOSS击杀
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_KILL_BOSS, as, RACheckKillBossReward);
	// 奇珍异宝活动切换
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_CHESTSHOP, as, RACheckChestshopGiveReward);
	// 全民祈福活动切换
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_QUANMIN_QIFU, as, RACheckQuanminQifuGiveReward);
	// 宝石升级活动
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_QUANMIN_QIFU, as, RACheckStoneTotalLevelGiveReward);
	// 坐骑进阶
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_MOUNT_UPGRADE, as, RACheckMountUpgradeGiveReward);
	// 根骨活动
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_MENTALITY_TOTAL_LEVEL, as, RACheckMentalityTotalLevelGiveReward);
	// 羽翼活动
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_WING_UPGRADE, as, RACheckWingUpgradeGiveReward);
	// 登录奖励（活动结束时）
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_LOGIN_GIFT, as, RACheckLoginGiftGiveReward);
	// 登录奖励（活动结束时）
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_LOGIN_GIFT_0, as, RACheckLoginGiftGiveReward_0);
	// 比拼综合战力
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL, as, RACheckBipinCapTotalGiveReweard);
	// 比拼装备战力
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP, as, RACheckBipinCapEquipGiveReweard);
	// 比拼仙女战力
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_BP_CAPABILITY_XIANNV, as, RACheckBipinCapXiaNvGiveReweard);
	// 比拼经脉战力
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGMAI, as, RACheckBipinCapJingMaiGiveReweard);
	// 比拼羽翼战力
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING, as, RACheckBipinCapWingGiveReweard);
	// 比拼坐骑战力
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT, as, RACheckBipinCapMountGiveReweard);
	// 累计充值（天数）活动
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_TOTAL_CHARGE_DAY, as, RACheckTotalChargeDayGiveReward);
	// 比拼神装战力
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG, as, RACheckBipinCapShenzhuangGiveReweard);
	// 比拼精灵战力
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING, as, RACheckBipinCapJinglingGiveReweard);
	// 比拼精炼战力
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH, as, RACheckBipinCapWashGiveReweard);
	// 珍宝阁
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_ZHENBAOGE, as, OnRaZhenbageResetflushtimestamp);
	// 被动变身榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_PASSIVE_RANK, as, RACheckSpecialAppearacePassiveJoinReward);
	// 变身榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_SPECIAL_APPEARANCE_RANK, as, RACheckSpecialAppearaceJoinReward);
	// 翻翻乐
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_FANFAN, as, RACheckFanFanGiveReward);
	// 连续充值
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI, as, RACheckContinueChongzhiGiveReward);
	// 连充特惠初
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_CHU, as, RACheckContinueChongzhiGiveRewardChu);
	// 连充特惠高
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_CONTINUE_CHONGZHI_GAO, as, RACheckContinueChongzhiGiveRewardGao);
	// 连续消费
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_CONTINUE_CONSUME, as, RACheckContinueConsumeGiveReward);
	//军歌嘹亮
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_ARMY_DAY, as, OnRaCheckArmyDayWinnerGiveReward);
	//循环充值
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_CIRCULATION_CHONGZHI, as, RACheckCirculationChongzhiGiveReward);
	// 珍宝阁2
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_ZHENBAOGE2, as, OnRaZhenbage2Resetflushtimestamp);
	// 冲级大礼
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_ROLE_UPLEVEL, as, RACheckRoleUplevelGiveReweard);
	// 勇者之塔
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_PATA, as, RACheckPataGiveReweard);
	// 经验副本
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_EXP_FB, as, RACheckExpFbGiveReweard);
	// 坐骑进阶
	//TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_MOUNT, as, RACheckUpgradeMountGiveReweard);
	// 光环进阶
	//TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_HALO, as, RACheckUpgradeHaloGiveReweard);
	// 羽翼进阶
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_WING, as, RACheckUpgradeWingGiveReweard);
	// 神弓进阶
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG, as, RACheckUpgradeShengongGiveReweard);
	// 神翼进阶
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_SHENYI, as, RACheckUpgradeShenyiGiveReweard);
	// 首充团购
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_FIRST_CHARGE_TUAN, as, RACheckFirstChargeTuanUpgradeGiveReweard);
	// 每日累计充值
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_DAY_TOTAL_CHARGE, as, RACheckDayTotalChargeUpgradeGiveReweard);
	// 全服坐骑进阶
	//TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_TOTAL, as, RACheckMountUpgradeTotalGiveReweard);
	//// 全服光环进阶
	//TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_HALO_TOTAL, as, RACheckHaloUpgradeTotalGiveReweard);
	// 全服羽翼进阶
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_WING_TOTAL, as, RACheckWingUpgradeTotalGiveReweard);
	// 全服神弓进阶
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_TOTAL, as, RACheckShengongUpgradeTotalGiveReweard);
	// 全服神翼进阶
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_TOTAL, as, RACheckShenyiUpgradeTotalGiveReweard);
	// 坐骑进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_MOUNT_RANK, as, RACheckMountUpgradeRankGiveReweard);
	// 光环进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_HALO_RANK, as, RACheckHaloUpgradeRankGiveReweard);
	// 羽翼进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_WING_RANK, as, RACheckWingUpgradeRankGiveReweard);
	// 神弓进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_SHENGONG_RANK, as, RACheckShengongUpgradeRankGiveReweard);
	// 神翼进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_SHENYI_RANK, as, RACheckShenyiUpgradeRankGiveReweard);
	// 时装进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK, as, RACheckShizhuangUpgradeRankGiveReweard);
	// 神兵进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_SHENBIN_RANK, as, RACheckShenbinUpgradeRankGiveReweard);
	// 法宝进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK, as, RACheckFabaoUpgradeRankGiveReweard);
	// 足迹进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_SHIZHUANG_RANK, as, RACheckFootprintUpgradeRankGiveReweard);
	// 麒麟臂进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_QILINBI_RANK, as, RACheckQilinbiUpgradeRankGiveReweard);
	// 头饰进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_TOUSHI_RANK, as, RACheckToushiUpgradeRankGiveReweard);
	// 腰饰进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_YAOSHI_RANK, as, RACheckYaoshiUpgradeRankGiveReweard);
	// 灵童进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_LINGTONG_RANK, as, RACheckLingtongUpgradeRankGiveReweard);
	// 灵弓进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_LINGGONG_RANK, as, RACheckLinggongUpgradeRankGiveReweard);
	// 灵骑进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_LINGQI_RANK, as, RACheckLingqiUpgradeRankGiveReweard);
	// 全身装备强化总等级
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL, as, RACheckEquipStrengthTotalLevelGiveReweard);
	// 全身宝石总等级
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL, as, RACheckStoneTotalLevelGiveReweard);
	// 全身装备强化总等级榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_EQUIP_STRENGTH_TOTAL_LEVEL_RANK, as, RACheckEquipStrengthTotalLevelRankGiveReweard);
	// 全身宝石总等级榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_STONE_TOTAL_LEVEL_RANK, as, RACheckStoneTotalLevelRankGiveReweard);
	// Boss猎手
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_BOSS_KILLER, as, RACheckBossKillerGiveReweard);
	// 全服女神比拼
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_XIANNV_RANK, as, RACheckXianNvTotalRankGiveReweard);
	// 全服精灵比拼
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_JINGLING_RANK, as, RACheckJingLingTotalRankGiveReweard);
	// 全服战骑比拼
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_FIGHT_MOUNT_RANK, as, RACheckFightMountTotalRankGiveReweard);
	// 全服综合战力比拼
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_TOTAL_CAP_RANK, as, RACheckRoleTotalCapRankGiveReweard);
	// 单返豪礼
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_SINGLE_CHONGZHI, as, OnRASingleChongZhiGiveReward);
	// 聚划算
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_XIANYUAN_TREAS, as, RACheckXianyuanTreasMailReward);
	// 新累计充值活动切换
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE, as, RACheckNewTotalChargeGiveReward);
	// 植树参与奖
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_PLANTING_TREE, as, RACheckPlantingTreeCanyuReward);
	// 限时礼包
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_TIME_LIMIT_GIFT, as, RACheckTimeLimitGiftReward);
	//版本活动累计充值
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_VERSIONS_GRAND_TOTAL_CHARGE, as, RACheckVersionTotalChargeGiveReward);
	// 版本活动连续充值
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_VERSIONS_CONTINUE_CHARGE, as, RACheckVersionContinueChongzhiGiveReward);
	// 线下单笔充值0
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_OFFLINE_SINGLE_CHARGE_0, as, CheckRAOfflineSingleChargeReward0);
	// 每日好礼
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_EVERYDAY_NICE_GIFT, as, OnRAEverydayNiceGiftReissueReward);
	// 节日守护
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_HOLIDAY_GUARD, as, OnHolidayGuardKillMonsterGiveReward);
	// 极限挑战
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_EXTREME_CHALLENGE, as, OnRAExtremeChallengeGiveReward);
	// 飞宠进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_FLYPET_RANK, as, RACheckFlypetUpgradeRankGiveReweard);
	// 尾焰进阶榜
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_UPGRADE_WEIYAN_RANK, as, RACheckWeiyanUpgradeRankGiveReweard);
	// 消费返礼
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_CONSUME_GOLD_REWARD, as, RACheckConsumeGoldRewardAutoFetch);
	// 狂嗨庆典
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_KUANG_HAI_QING_DIAN, as, CheckRAKuangHaiQingDianGiveReward);
	// 新累计充值1活动切换
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE1, as, RACheckNewTotalCharge1GiveReward);
	// 新累计充值2活动切换
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE2, as, RACheckNewTotalCharge2GiveReward);
	// 新累计充值3活动切换
	TRY_GIVE_REWRAD_ON_OPEN_RAND_ACT(RAND_ACTIVITY_TYPE_NEW_TOTAL_CHARGE3, as, RACheckNewTotalCharge3GiveReward);
}

void SceneManager::OnRADayChongzhiRankGiveReward()
{
	for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
	{
		Scene *scene = *i;
		if (NULL == scene) continue;

		for (int j = 0; j < (int)scene->RoleNum(); ++ j)
		{
			Role *temp_role = scene->GetRoleByIndex(j);
			if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
			{
				temp_role->GetRoleActivity()->RAResetDayChongzhiRankGoldNum();
			}
		}
	}
}

void SceneManager::OnRADayConsumeRankGiveReward()
{
	for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
	{
		Scene *scene = *i;
		if (NULL == scene) continue;

		for (int j = 0; j < (int)scene->RoleNum(); ++ j)
		{
			Role *temp_role = scene->GetRoleByIndex(j);
			if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
			{
				temp_role->GetRoleActivity()->RAResetDayConsumeRankGoldNum();
			}
		}
	}
}

void SceneManager::OnRAGoldenPigSendBossState()
{
	if (!ActivityShadow::Instance().IsRandActivityOpen(RAND_ACTIVITY_TYPE_GOLDEN_PIG))
	{
		return;
	}

	for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)
	{
		Scene *scene = *i;
		if (NULL == scene) continue;

		for (int j = 0; j < (int)scene->RoleNum(); ++j)
		{
			Role *temp_role = scene->GetRoleByIndex(j);
			if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
			{
				temp_role->GetRoleActivity()->RAGoldenPigSendBossState();
			}
		}
	}
}

bool SceneManager::RandomGetRoleList(Role *role, Protocol::RoleInfo role_list[Protocol::MAX_RANDOM_ROLE_LIST_NUM], int *get_num)
{
	if (NULL == role || NULL == role_list || NULL == get_num)
	{
		return false;
	}

	if (m_uid_to_rolelocal.empty()) 
	{
		return false;
	}
	
	*get_num = 0;

	std::set<UserID> check_userid_list;

	int random_count = 0, temp_num = 0;
	while (random_count ++ < 128 && temp_num < Protocol::MAX_RANDOM_ROLE_LIST_NUM)
	{
		int rand_index = RandomNum(static_cast<int>(m_uid_to_rolelocal.size()));

		UIDSceneMap::iterator it = m_uid_to_rolelocal.begin();
		std::advance(it, rand_index);

		Role *friend_role = this->GetRole(it->first);
		if (NULL != friend_role)
		{
			if (check_userid_list.end() != check_userid_list.find(friend_role->GetUserId())) continue;
			check_userid_list.insert(friend_role->GetUserId());

			static const int RANDOM_FRIEND_LEVEL_INTERVAL = 8;

			if (friend_role->GetUserId() == role->GetUserId()) continue;
			if (friend_role->GetLevel() < role->GetLevel() - RANDOM_FRIEND_LEVEL_INTERVAL) continue;
			if (role->GetFriendManager()->IsFriend(friend_role->GetUserId(), NULL)) continue;

			role_list[temp_num].uid = friend_role->GetUID();
			role_list[temp_num].avatar = friend_role->GetAvatar();
			role_list[temp_num].camp = friend_role->GetCamp();
			role_list[temp_num].capability = friend_role->GetCapability()->GetTotalCapability();
			role_list[temp_num].level = friend_role->GetLevel();
			role_list[temp_num].prof = friend_role->GetProf();
			friend_role->GetName(role_list[temp_num].role_name);
			role_list[temp_num].sex = friend_role->GetSex();
			role_list[temp_num].avatar_timestamp = friend_role->GetAvatarTimeStamp();

			++ temp_num;
		}
	}

	*get_num = temp_num;
	
	return true;
}

int SceneManager::RandomGetRoleList(int role_list[SCENE_MANAGER_RANDOM_GET_ROLE_MAX_COUNT])
{
	if (NULL == role_list || m_uid_to_rolelocal.empty())
	{
		return 0;
	}

	std::set<UserID> uid_repeat_checker;

	int random_count = 0, role_count = 0;
	while (random_count ++ < 128 && role_count < SCENE_MANAGER_RANDOM_GET_ROLE_MAX_COUNT)
	{
		int rand_index = RandomNum(static_cast<int>(m_uid_to_rolelocal.size()));

		UIDSceneMap::iterator it = m_uid_to_rolelocal.begin();
		std::advance(it, rand_index);

		Role *role = this->GetRole(it->first);
		if (NULL == role)
		{
			continue;
		}

		if (uid_repeat_checker.end() != uid_repeat_checker.find(role->GetUserId())) 
		{
			continue;
		}

		uid_repeat_checker.insert(role->GetUserId());

		role_list[role_count] = role->GetUID();

		++ role_count;
	}

	return role_count;
}

int SceneManager::GetOnlineRoleNum()
{
	return static_cast<int>(m_net_to_rolelocal.size());
}

int SceneManager::GetMaxRoleNum()
{
	return GLOBALCONFIG->GetServerMaxRoleNum(); 
}

void SceneManager::OnCrossUserAdd(int uid, int cross_type)
{
	static const int PLAT_TYPE = LocalConfig::Instance().GetPlatType();
	long long uuid = UniqueUserIDToLongLong(UniqueUserID(PLAT_TYPE, IntToUid(uid)));

	if (m_cross_user_map.find(uuid) == m_cross_user_map.end())
	{
		CrossUserInfo user_info;
		user_info.uuid = uuid;
		user_info.camp = 0;
		user_info.cross_type = cross_type;
		m_cross_user_map[user_info.uuid] = user_info;
	}

	// 私聊在线状态同步
	ChatManager::Instance().CheckChangeRoleOnline(uuid);
}

void SceneManager::OnDisconnectFromCrossServer(const int cross_type)
{
	///////////////////////////////// 清除处于跨服状态角色标记 /////////////////////////////////////////
	if (cross_type > 0)
	{
		for (CrossUserMapIt it = m_cross_user_map.begin(); it != m_cross_user_map.end();)
		{
			if (it->second.cross_type <= 0 || cross_type == it->second.cross_type)
			{
				long long uuid = it->first;
				m_cross_user_map.erase(it++);

				// 在线状态同步
				ChatManager::Instance().CheckChangeRoleOnline(uuid);
			}
			else
			{
				it++;
			}
		}
	}
	else
	{
		CrossUserMap tmp_cross_user_map = m_cross_user_map;

		m_cross_user_map.clear();

		// 私聊在线状态同步
		for (auto &it : tmp_cross_user_map)
		{
			ChatManager::Instance().CheckChangeRoleOnline(it.first);
		}
	}


	////////////////////////////// 清除角色的活动数据 ////////////////////////////////////////////

	if (!CrossConfig::Instance().IsHiddenServer())
	{
		for (auto it = m_uid_to_rolelocal.begin(); it != m_uid_to_rolelocal.end(); ++it)
		{
			Role *role = this->GetRole(it->first);
			if (NULL != role)
			{
				role->GetRoleCross()->OnClearCross1v1Matching(cross_type);
				role->GetRoleCross()->OnClearCross3v3Matching(cross_type);
			}
		}
	}
}

void SceneManager::OnCrossGuildBattleInfoSync(NetID net_id, const char *data, int length)
{
// 	if (CrossConfig::Instance().IsHiddenServer())
// 	{
// 		return;
// 	}

	crossgameprotocal::CrossGameGuildBattleInfoSync *cmd = (crossgameprotocal::CrossGameGuildBattleInfoSync *)data;

	std::set<GuildID> guild_id_list;
	CrossGuildBattleGuildItem cross_guildbattle_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	CrossGuildBattleTuanzhangInfo cross_guildbattle_tuanzhang_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	CrossGuildBattleApperanceInfo cross_guildbattle_apperance_info_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM];
	for (int i = 0; i < CROSS_GUILDBATTLE_MAX_SCENE_NUM; ++i)
	{
		if (INVALID_UNIQUE_USER_ID != cmd->owner_list[i].guild_id)
		{
			cross_guildbattle_list[i].plat_type = cmd->owner_list[i].guild_id.plat_type;
			cross_guildbattle_list[i].guild_id = UidToInt(cmd->owner_list[i].guild_id.user_id);
			F_STRNCPY(cross_guildbattle_list[i].guild_tuanzhang_name, cmd->owner_list[i].guild_tuanzhang_name, sizeof(GameName));
			F_STRNCPY(cross_guildbattle_list[i].guild_name, cmd->owner_list[i].guild_name, sizeof(GameName));
			cross_guildbattle_tuanzhang_info_list[i].tuanzhang_prof = cmd->owner_list[i].prof;
			cross_guildbattle_tuanzhang_info_list[i].tuanzhang_sex = cmd->owner_list[i].sex;
			cross_guildbattle_apperance_info_list[i].wuqi_id = cmd->owner_list[i].wuqi_id;
			cross_guildbattle_apperance_info_list[i].shizhuang_wuqi = cmd->owner_list[i].shizhuang_wuqi;
			cross_guildbattle_apperance_info_list[i].shizhuang_wuqi_is_special = cmd->owner_list[i].shizhuang_wuqi_is_special;
			cross_guildbattle_apperance_info_list[i].shizhuang_body = cmd->owner_list[i].shizhuang_body;
			cross_guildbattle_apperance_info_list[i].shizhuang_body_is_special = cmd->owner_list[i].shizhuang_body_is_special;
			cross_guildbattle_apperance_info_list[i].halo_used_imageid = cmd->owner_list[i].halo_used_imageid;
			cross_guildbattle_apperance_info_list[i].qilinbi_used_imageid = cmd->owner_list[i].qilinbi_used_imageid;
			cross_guildbattle_apperance_info_list[i].toushi_used_imageid = cmd->owner_list[i].toushi_used_imageid;
			cross_guildbattle_apperance_info_list[i].mask_used_imageid = cmd->owner_list[i].mask_used_imageid;

			guild_id_list.insert(cross_guildbattle_list[i].guild_id);

			if (cross_guildbattle_list[i].guild_id > 0 && cross_guildbattle_list[i].plat_type == LocalConfig::Instance().GetPlatType())
			{
				Guild *guild = GuildManager::Instance().GetGuild(cross_guildbattle_list[i].guild_id);
				if (NULL != guild) // 给帮派每个人直接发奖励
				{
					int scene_id_list[CROSS_GUILDBATTLE_MAX_SCENE_NUM] = { 0 };
					CrossGuildBattleConfig::Instance().GetGuildBattleSceneIdList(scene_id_list);			//获取场景id列表

					guild->SendBattleWinReward(i, scene_id_list[i]);

					for(auto it : cmd->user_core_rank_list)
					{
						if (UidToInt(it.guild_id.user_id) == cross_guildbattle_list[i].guild_id)
						{
							UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(it.role_id.user_id);
							if (user_node != NULL)
							{
								//江湖传闻(成功)
								EventHandler::Instance().OnAddGuildRumorEvent(guild->GetGuildID(), user_node->user_name, NULL, GUILD_EVENT_TYPE_RUMOR_CROSS_GUILD_BATTLE_SUCC, scene_id_list[i]);
							}
						}
					}
				}
			}
		}
	}

	//其他参与帮派
	{
		for (auto it : cmd->user_core_rank_list)
		{
			if (it.guild_id.plat_type == LocalConfig::Instance().GetPlatType())
			{
				bool is_find = false;
				for (auto win_gild : cross_guildbattle_list)					//查询是否胜利帮派，胜利帮派无需再播报江湖传闻
				{
					if (win_gild.guild_id == UidToInt(it.guild_id.user_id))
					{
						is_find = true;
						break;
					}
				}

				if (!is_find)
				{
					UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(it.role_id.user_id);
					if (user_node != NULL)
					{
						EventHandler::Instance().OnAddGuildRumorEvent(UidToInt(it.guild_id.user_id), user_node->user_name, NULL, GUILD_EVENT_TYPE_RUMOR_CROSS_GUILD_BATTLE_FAIL);
					}
				}
			}
		}
	}

	WorldStatus::Instance().SetCrossGuildBattleInfo(cross_guildbattle_list);
	WorldStatus::Instance().SetCrossGuildBattleTuanzhangInfo(cross_guildbattle_tuanzhang_info_list);
	WorldStatus::Instance().SetCrossGuildBattleApperanceInfo(cross_guildbattle_apperance_info_list);

	GuildManager::Instance().SendGuildBattleJoinReward(guild_id_list);
}

void SceneManager::OnCrossGuildBattleGetRankInfoResp(NetID net_id, const char *data, int length)
{
	if (CrossConfig::Instance().IsHiddenServer())
	{
		return;
	}

	crossgameprotocal::CrossGameGetGuildBattleRankLogResp *resp_msg = (crossgameprotocal::CrossGameGetGuildBattleRankLogResp *)data;
	Role *role = this->GetRole(resp_msg->uni_user_id.user_id);
	if (NULL == role)
	{
		return;
	}

	role->GetRoleCross()->RespCrossGuildBattleGetRankLog(resp_msg->scene_list);
}

void SceneManager::ForceAllOnlineRoleSyncData()
{
	for (SceneList::Iterator i = m_scene_list.Beg(); i != m_scene_list.End(); ++i)\
	{
		Scene *scene = *i; 
		if (NULL == scene) continue; 
		for (int j = 0; j < (int)scene->RoleNum(); ++j)
		{
			Role *temp_role = scene->GetRoleByIndex(j);
			if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())\
			{
				temp_role->GetRankShadow()->UpdateFunc();
			}
		}
	}
}

Scene *SceneManager::GetRandomStaticScene()
{
	// 指定在这个列表里的场景才拿来随机，因为服务端有些场景已经对客户端来说无效了
	std::set<int> valid_scene_id_set;
	valid_scene_id_set.insert(404);
	valid_scene_id_set.insert(103);
	valid_scene_id_set.insert(105);
	valid_scene_id_set.insert(109);
	valid_scene_id_set.insert(130);
	valid_scene_id_set.insert(140);
	valid_scene_id_set.insert(150);
	valid_scene_id_set.insert(160);
	valid_scene_id_set.insert(164);
	valid_scene_id_set.insert(200);
	valid_scene_id_set.insert(403);
	valid_scene_id_set.insert(1003);
	valid_scene_id_set.insert(1201);
	valid_scene_id_set.insert(1140);
	valid_scene_id_set.insert(1700);
	valid_scene_id_set.insert(1710);
	valid_scene_id_set.insert(1740);
	valid_scene_id_set.insert(8050);

	//	int index = RandomNum(m_scene_list.Size());
	//	return this->GetSceneByIndex(index);
	static const int MAX_STATIC_SCENE_COUNT = 100;

	int scene_count = 0;
	Scene *scene_list[MAX_STATIC_SCENE_COUNT];
	for (SceneList::Iterator it = m_scene_list.Beg(); it != m_scene_list.End() && scene_count < MAX_STATIC_SCENE_COUNT; ++it)
	{
		Scene *scene = *it;
		if (NULL != scene && scene->InStaticScene() && valid_scene_id_set.find(scene->GetSceneID()) != valid_scene_id_set.end())
		{
			scene_list[scene_count++] = scene;
		}
	}

	if (scene_count <= 0)
	{
		return NULL;
	}

	int index = RandomNum(scene_count);
	if (index < 0 || index >= MAX_STATIC_SCENE_COUNT)
	{
		index = 0;
	}

	return scene_list[index];
}

bool SceneManager::GotoRandomStaticScene(Role *role)
{
	Posi town_point;
	this->GoTo(role, 1001, role->GetUID(), town_point);
	return true;

	// 指定在这个列表里的场景才拿来随机，因为服务端有些场景已经对客户端来说无效了
	std::set<int> valid_scene_id_set;
	valid_scene_id_set.insert(404);
	valid_scene_id_set.insert(103);
	valid_scene_id_set.insert(105);
	valid_scene_id_set.insert(109);
	valid_scene_id_set.insert(130);
	valid_scene_id_set.insert(140);
	valid_scene_id_set.insert(150);
	valid_scene_id_set.insert(160);
	valid_scene_id_set.insert(164);
	valid_scene_id_set.insert(200);
	valid_scene_id_set.insert(403);
	valid_scene_id_set.insert(1003);
	valid_scene_id_set.insert(1201);
	valid_scene_id_set.insert(1140);
	valid_scene_id_set.insert(1700);
	valid_scene_id_set.insert(1710);
	valid_scene_id_set.insert(1740);
	valid_scene_id_set.insert(8050);

	//	int index = RandomNum(m_scene_list.Size());
	//	return this->GetSceneByIndex(index);
	static const int MAX_STATIC_SCENE_COUNT = 100;

	int scene_count = 0;
	Scene *scene_list[MAX_STATIC_SCENE_COUNT];
	for (SceneList::Iterator it = m_scene_list.Beg(); it != m_scene_list.End() && scene_count < MAX_STATIC_SCENE_COUNT; ++it)
	{
		Scene *scene = *it;
		if (NULL != scene && scene->InStaticScene() && valid_scene_id_set.find(scene->GetSceneID()) != valid_scene_id_set.end())
		{
			scene_list[scene_count++] = scene;
		}
	}

	if (scene_count <= 0)
	{
		return false;
	}

	int index = RandomNum(scene_count);
	if (index < 0 || index >= MAX_STATIC_SCENE_COUNT)
	{
		index = 0;
	}

	scene_list[index]->GetTownPoint(role, &town_point);
	this->GoTo(role, scene_list[index]->GetSceneID(), scene_list[index]->GetSceneKey(), town_point);

	return true;
}

void SceneManager::DestroyTeamEquipFB(int fb_key, bool real_destroy)
{
	for (SceneList::Iterator it = m_scene_list.Beg(); it != m_scene_list.End(); ++it)
	{
		Scene *scene = *it;
		if (Scene::SCENE_TYPE_TEAM_EQUIPFB == scene->GetSceneType() && fb_key == scene->GetSceneKey())
		{
			if (real_destroy)
			{
				this->DestroyFB(scene->GetIndex());
			}
			else
			{
				for (int i = 0; i < static_cast<int>(scene->RoleNum()); ++i)
				{
					Role *role = scene->GetRoleByIndex(i);
					if (NULL == role) continue;

					Posi last_scene_pos;
					int last_scene_id = role->GetLastScene(&last_scene_pos);

					this->GoTo(role, last_scene_id, 0, last_scene_pos);
				}
			}
		}
	}
}

void SceneManager::OnUseGoldInHiddenGs(NetID net_id, const char *msg, int length)
{
	crossgameprotocal::UseGoldInHiddenGs *ugihg = (crossgameprotocal::UseGoldInHiddenGs *)msg;

	// quick log
	{
		Money::QuickLogParam param;
		param.uid = ugihg->uid;
		F_STRNCPY(param.user_name, ugihg->user_name, sizeof(GameName));
		F_STRNCPY(param.plat_name, ugihg->plat_name, sizeof(PlatName));
		param.n1 = ugihg->use_gold;
		param.n2 = ugihg->cur_gold;

		UNSTD_STATIC_CHECK(sizeof(param.type) == sizeof(ugihg->type));
		F_STRNCPY(param.type, ugihg->type, sizeof(param.type));

		int log_type = ugihg->is_use_bind ? LOG_TYPE_GOLD_BIND_USE : LOG_TYPE_GOLD_USE;

		Money::QuickLogByCross(log_type, param);
	}

	// consume sys
	if (ugihg->is_consume_sys)
	{
		// 记录到 global role
		CommonDataGlobal *common_global = GlobalRoleManager::Instance().GetCommonDataGlobal(ugihg->uid);
		if (nullptr != common_global)
		{
			common_global->AddUseGoldOnCross(static_cast<int>(ugihg->use_gold));
		}

		//CONSOLE_LOG("OnUseGoldInHiddenGs sys_consume_gold_in_cross: %d", g_common_data->sys_consume_gold_in_cross);
	}
}

void SceneManager::OnSyncCrossSeasonChange(int cross_activity)
{
	// 给全服在线玩家
	for (auto it = m_uid_to_rolelocal.begin(); it != m_uid_to_rolelocal.end(); ++it)
	{
		Role *role = this->GetRole(it->first);
		if (NULL != role)
		{
			switch (cross_activity)
			{
			case CROSS_ACTIVITY_TYPE_CROSS_1V1:
			{
				role->GetRoleCross()->OnCross1v1SeasonChange();
				break;
			}
			case CROSS_ACTIVITY_TYPE_CROSS_MULTIUSER_CHALLENGE:
			{
				role->GetRoleCross()->OnCross3v3SeasonChange();
				break;
			}
			
			default:
				break;
			}
		}
	}
}

void SceneManager::OnCrossBossInfoReq(crossgameprotocal::GameGameBossInfoReq *ggbir)
{
	if (ggbir == nullptr)
	{
		return;
	}

	if (ggbir->req_type == Protocol::CROSS_BOSS_OPERA_TYPE_GET_FLUSH_INFO && ggbir->param_1 == 0)
	{
		static Protocol::SCCrossBossBossInfoAck cbbia;

		int scene_id_list[CROSS_BOSS_SCENE_MAX] = { 0 };
		cbbia.scene_count = CrossBossConfig::Instance().GetSceneIdList(scene_id_list);
		for (int i = 0; i < cbbia.scene_count && i < CROSS_BOSS_SCENE_MAX; ++i)
		{
			cbbia.scene_list[i].layer = i;
			cbbia.scene_list[i].boss_count = 0;
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY);
			if (NULL != scene && Scene::SCENE_TYPE_CROSS_BOSS == scene->GetSceneType())
			{
				SpecialCrossBoss *cross_boss = dynamic_cast<SpecialCrossBoss*>(scene->GetSpecialLogic());
				if (nullptr == cross_boss)
				{
					continue;
				}
				for (int k = 0; k < cross_boss->GetBossCount() && k < MAX_CROSS_BOSS_PER_SCENE; k++)
				{
					Protocol::SCCrossBossBossInfoAck::BossInfo &boss_info = cbbia.scene_list[i].boss_list[k];
					cross_boss->GetCrossBossInfo(k, boss_info, cbbia.scene_list[i].left_treasure_crystal_count, cbbia.scene_list[i].left_monster_count);
					cbbia.scene_list[i].boss_count++;
				}
			}
		}
		int send_length = sizeof(cbbia) - (CROSS_BOSS_SCENE_MAX - cbbia.scene_count) * sizeof(cbbia.scene_list[0]);
		InternalComm::Instance().UserProtocalTransferToGame(ggbir->uuid, (const char *)&cbbia, send_length);
		return;
	}

	if (ggbir->req_type == Protocol::CROSS_BOSS_OPERA_TYPE_DROP_RECORD)
	{
		this->SendCrossBossDropRecord(ggbir->uuid);
		return;
	}

	const CrossBossLayerCfg *layer_cfg = CrossBossConfig::Instance().GetCrossBossLayerCfg(ggbir->param_1);
	if (NULL == layer_cfg)
	{
		return;
	}

	SpecialCrossBoss *cross_boss = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossBoss(layer_cfg->scene_id, CROSS_PUBLIC_SCENE_KEY);
	if (NULL == cross_boss)
	{
		return;
	}

	cross_boss->OnSyncDataToCrossSever(ggbir);
}

void SceneManager::OnCrossMizangBossInfoReq(crossgameprotocal::GameGameBossInfoReq *ggbir)
{
	if (ggbir == nullptr)
	{
		return;
	}

	if (ggbir->req_type == Protocol::CROSS_MIZANG_BOSS_OPERA_TYPE_GET_FLUSH_INFO && ggbir->param_1 == 0)
	{
		static Protocol::SCCrossMizangBossBossInfoAck cbbia;

		int scene_id_list[CROSS_MIZANG_BOSS_SCENE_MAX] = { 0 };
		cbbia.scene_count = CrossMizangBossConfig::Instance().GetSceneIdList(scene_id_list);
		for (int i = 0; i < cbbia.scene_count && i < CROSS_MIZANG_BOSS_SCENE_MAX; ++i)
		{
			cbbia.scene_list[i].layer = i;
			cbbia.scene_list[i].boss_count = 0;
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY);
			if (NULL != scene && Scene::SCENE_TYPE_CROSS_MIZANG_BOSS == scene->GetSceneType())
			{
				SpecialCrossMiZangBoss *cross_boss = dynamic_cast<SpecialCrossMiZangBoss*>(scene->GetSpecialLogic());
				if (nullptr == cross_boss)
				{
					continue;
				}
				for (int k = 0; k < cross_boss->GetBossCount() && k < MAX_CROSS_MIZANG_BOSS_PER_SCENE; k++)
				{
					Protocol::SCCrossMizangBossBossInfoAck::BossInfo &boss_info = cbbia.scene_list[i].boss_list[k];
					cross_boss->GetCrossBossInfo(k, boss_info, cbbia.scene_list[i].left_treasure_crystal_count, cbbia.scene_list[i].left_monster_count);
					cbbia.scene_list[i].boss_count++;
				}
			}
		}
		int send_length = sizeof(cbbia) - (CROSS_MIZANG_BOSS_SCENE_MAX - cbbia.scene_count) * sizeof(cbbia.scene_list[0]);
		InternalComm::Instance().UserProtocalTransferToGame(ggbir->uuid, (const char *)&cbbia, send_length);
		return;
	}

	if (ggbir->req_type == Protocol::CROSS_MIZANG_BOSS_OPERA_TYPE_DROP_RECORD)
	{
		this->SendCrossBossDropRecord(ggbir->uuid);
		return;
	}

	const CrossMizangBossLayerCfg *layer_cfg = CrossMizangBossConfig::Instance().GetCrossBossLayerCfg(ggbir->param_1);
	if (NULL == layer_cfg)
	{
		return;
	}

	SpecialCrossMiZangBoss *cross_boss = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossMizangBoss(layer_cfg->scene_id, CROSS_PUBLIC_SCENE_KEY);
	if (NULL == cross_boss)
	{
		return;
	}

	cross_boss->OnSyncDataToCrossSever(ggbir);
}

void SceneManager::OnCrossYoumingBossInfoReq(crossgameprotocal::GameGameBossInfoReq *ggbir)
{
	if (ggbir == nullptr)
	{
		return;
	}

	if (ggbir->req_type == Protocol::CROSS_YOUMING_BOSS_OPERA_TYPE_GET_FLUSH_INFO && ggbir->param_1 == 0)
	{
		static Protocol::SCCrossYouMingBossBossInfoAck cbbia;

		int scene_id_list[CROSS_YOUMING_BOSS_SCENE_MAX] = { 0 };
		cbbia.scene_count = CrossYoumingBossConfig::Instance().GetSceneIdList(scene_id_list);
		for (int i = 0; i < cbbia.scene_count && i < CROSS_YOUMING_BOSS_SCENE_MAX; ++i)
		{
			cbbia.scene_list[i].layer = i;
			cbbia.scene_list[i].boss_count = 0;
			Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(scene_id_list[i], CROSS_PUBLIC_SCENE_KEY);
			if (NULL != scene && Scene::SCENE_TYPE_CROSS_YOUMING_BOSS == scene->GetSceneType())
			{
				SpecialCrossYouMingBoss *cross_boss = dynamic_cast<SpecialCrossYouMingBoss*>(scene->GetSpecialLogic());
				if (nullptr == cross_boss)
				{
					continue;
				}
				for (int k = 0; k < cross_boss->GetBossCount() && k < MAX_CROSS_YOUMING_BOSS_PER_SCENE; k++)
				{
					Protocol::SCCrossYouMingBossBossInfoAck::BossInfo &boss_info = cbbia.scene_list[i].boss_list[k];
					cross_boss->GetCrossBossInfo(k, boss_info, cbbia.scene_list[i].left_treasure_crystal_count, cbbia.scene_list[i].left_monster_count);
					cbbia.scene_list[i].boss_count++;
				}
			}
		}
		int send_length = sizeof(cbbia) - (CROSS_YOUMING_BOSS_SCENE_MAX - cbbia.scene_count) * sizeof(cbbia.scene_list[0]);
		InternalComm::Instance().UserProtocalTransferToGame(ggbir->uuid, (const char *)&cbbia, send_length);
		return;
	}

	if (ggbir->req_type == Protocol::CROSS_YOUMING_BOSS_OPERA_TYPE_DROP_RECORD)
	{
		this->SendCrossBossDropRecord(ggbir->uuid);
		return;
	}

	const CrossYoumingBossLayerCfg *layer_cfg = CrossYoumingBossConfig::Instance().GetCrossBossLayerCfg(ggbir->param_1);
	if (NULL == layer_cfg)
	{
		return;
	}

	SpecialCrossYouMingBoss *cross_boss = World::GetInstWorld()->GetSceneManager()->GetSpecialCrossYoumingBoss(layer_cfg->scene_id, CROSS_PUBLIC_SCENE_KEY);
	if (NULL == cross_boss)
	{
		return;
	}

	cross_boss->OnSyncDataToCrossSever(ggbir);
}

void SceneManager::SendCrossBossDropRecord(long long uid)
{
	std::deque<CrossBossDropRecordItem> &m_drop_record_list = WorldStatus2::Instance().GetCrossBossDropRecord();

	static Protocol::SCCrossBossDropRecord cbdr;
	cbdr.record_count = 0;
	for (std::deque<CrossBossDropRecordItem>::const_iterator it = m_drop_record_list.begin(); it != m_drop_record_list.end(); ++it)
	{
		if (cbdr.record_count >= CROSS_BOSS_MAX_DROP_RECROD)
		{
			break;
		}

		cbdr.dorp_record_list[cbdr.record_count].uuid = it->uuid;
		F_STRNCPY(cbdr.dorp_record_list[cbdr.record_count].role_name, it->name, sizeof(GameName));
		cbdr.dorp_record_list[cbdr.record_count].pickup_timestamp = it->pickup_timestamp;
		cbdr.dorp_record_list[cbdr.record_count].scene_id = it->scene_id;
		cbdr.dorp_record_list[cbdr.record_count].monster_id = it->monster_id;
		cbdr.dorp_record_list[cbdr.record_count].item_id = it->item_id;
		cbdr.dorp_record_list[cbdr.record_count].item_num = it->item_wrapper.num;
		ARRAYCPY(cbdr.dorp_record_list[cbdr.record_count].xianpin_type_list, it->item_wrapper.param_data.xianpin_type_list);
		cbdr.record_count++;
	}

	int length = sizeof(cbdr) - (CROSS_BOSS_MAX_DROP_RECROD - cbdr.record_count) * sizeof(cbdr.dorp_record_list[0]);
	InternalComm::Instance().UserProtocalTransferToGame(uid, (const char *)&cbdr, length);
}

int SceneManager::GetAllServerRoleLevelNum(int min_level, int max_level)
{
	if (!CrossConfig::Instance().IsHiddenServer())
	{
		return static_cast<int>(m_net_to_rolelocal.size());
	}

	if (min_level <= 0) min_level = 1;
	if (max_level > MAX_ROLE_LEVEL) max_level = MAX_ROLE_LEVEL;

	int role_count = 0;

	for (const auto &it : m_unique_server_role_num)
	{
		for (int lv = min_level; lv <= max_level; lv++)
		{
			role_count += it.second.role_num_list[lv];
		}
	}

	return role_count;
}

