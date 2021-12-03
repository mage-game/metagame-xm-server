#include <assert.h>

#include "common/platform/system.h"
#include "gamelog.h"

#include "internalcomm.h"
#include "gamecommon.h"
#include "scene.h"
#include "map.h"
#include "aoi/zone.h"
#include "scenemanager.h"
#include "rmibackobjdef.h"
#include "obj/character/role.h"
#include "obj/otherobj/scenearea.h"
#include "obj/otherobj/trigger.h"
#include "obj/otherobj/gatherobj.h"
#include "obj/otherobj/fallingitem.h"

#include "servercommon/internalprotocal/gcprotocal.h"
#include "servercommon/servercommon.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "servercommon/userprotocal/msgsystem.h"
#include "globalconfig/globalconfig.h"
#include "protocal/msgscene.h"
#include "protocal/msgfb.h"

#include "servercommon/serverconfig/serverconfigpool.h"

#include "speciallogic/speciallogic.hpp"

#include "world.h"

#include "servercommon/errornum.h"

#include "teamkeyregister/teamkeyregister.hpp"
#include "guildkeyregister/guildkeyregister.hpp"

#include "obj/character/monster.h"

#include "effect/skilleffect/effecthspecialshield.hpp"

#include "scene/asynreqregister/asynreqregister.hpp"
#include "protocal/msgactivity.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/activityconfig/shuijingconfig.hpp"
#include "monster/monsterpool.h"

#include "scene/generater/monstergenerater.h"

#include "servercommon/shenzhouweapondef.hpp"
#include "scene/generater/gathergenerater.h"
#include "other/shenzhouweapon/shenzhouweaponconfig.hpp"
#include "other/cross/rolecross.h"
#include "other/rolezhuanzhi/rolezhuanzhi.hpp"
#include "servercommon/serverconfig/crossguildbattleconfig.hpp"

Scene::Scene(SceneManager *scene_manager)
	: m_index(0), m_scene_id(0), m_scene_key(0), m_scene_manager(scene_manager), m_map(0), m_scene_type(SCENE_TYPE_COMMON_SCENE), 
	 m_forbid_pk(false), m_scene_create_time(0), m_scene_timeout(0), m_is_timeout_kick_role(false), m_is_delay_delete_scene(false), m_delay_delete_scene_time(0), m_monster_unique_key(0), m_special_logic(NULL),
	m_world_boss_last_brocast_time(0), m_guiji_scene_forbid_pk(false), m_scene_no_one_update_interval(0)
{
	memset(m_scene_name, 0, MAX_SCENE_NAME_LENGTH);
}

Scene::~Scene()
{
	this->Release();
	SpecialLogic::DestorySpecialLogic(m_special_logic);
}

bool Scene::Init(SceneIndex scene_index, const ConfigScene &configscene, Map *map, int scene_type, int scene_key, unsigned int scene_timeout, bool timeout_kick)
{
	assert(NULL != map);

	m_map = map;

	STRNCPY(m_scene_name, configscene.name.c_str(), sizeof(m_scene_name));
	
	m_scene_id = configscene.id;

	m_scene_type = scene_type;
	m_scene_key = scene_key;

	m_forbid_pk = configscene.is_forbid_pk;

	m_scene_create_time = EngineAdapter::Instance().Time();
	if (!this->InStaticScene()) 
	{
		m_scene_timeout = scene_timeout;
		if (0 != scene_timeout) m_is_timeout_kick_role = timeout_kick;	
	}

	m_special_logic = SpecialLogic::CreateSpecialLogic(this, scene_type);

	m_town_point.scene_id = configscene.townpoint.sceneid;
	m_town_point.pos.Init(configscene.townpoint.scenex, configscene.townpoint.sceney);

	m_index = scene_index;

	m_role_manager.SetScene(this);
	m_monster_manager.SetScene(this);
	m_fallingitem_manager.SetScene(this);
	m_trigger_manager.SetScene(this);
	m_gather_manager.SetScene(this);

	m_role_manager.Init();
	m_monster_manager.Init();
	m_fallingitem_manager.Init();
	m_trigger_manager.Init();
	m_gather_manager.Init();

	Axis ru_x, ru_y;
	m_map->GetArea(&ru_x, &ru_y);

	// zone matrix 单元区域
	static const int ZONE_UNIT_X = (int)GLOBALCONFIG->GetKeyConfig_ZoneMatrixX();		// 地图单元的划分距离
	static const int ZONE_UNIT_Y = (int)GLOBALCONFIG->GetKeyConfig_ZoneMatrixY();
	ZoneMatrix::CreateZoneMatrix(&m_zone_matrix, ru_x, ru_y, ZONE_UNIT_X, ZONE_UNIT_Y, &m_obj_manager);

	bool ret = m_scene_monster_logic.Init(this, configscene.monsterpoints, configscene.gathers);
	if (!ret)
	{
		gamelog::g_log_scene.printf(LL_ERROR, "Init scene_monster_logic Return FAIL!");
		return false;
	}
	
	m_transport_list.Init(configscene.doors);
	m_npc_list.Init(configscene.npcs);

	for(ConfigSceneTriggerAreaList::const_iterator area_iter = configscene.triggerareas.begin();
		area_iter != configscene.triggerareas.end(); ++area_iter)
	{
		SceneArea *area = new SceneArea();
		area->Init(Posi(area_iter->x, area_iter->y), area_iter->range);
		this->AddObj(area);
	}
	
	for(ConfigSceneTriggerList::const_iterator trigger_iter = configscene.triggers.begin();
		trigger_iter != configscene.triggers.end(); ++trigger_iter)
	{
		Trigger *trigger = new Trigger();
		trigger->SetPos(Posi(trigger_iter->x, trigger_iter->y));
		trigger->Init(trigger_iter->id, trigger_iter->range, trigger_iter->trigger_cond_type, trigger_iter->trigger_cond_param_list, trigger_iter->trigger_action_type, trigger_iter->trigger_action_param_list, trigger_iter->trigger_max_count);
		this->AddObj(trigger);
	}

	//gcprotocal::CreateChannel cc; // 创建场景聊天频道
	//cc.cid = GetChannelID();
	//cc.cooldown = CHANNEL_CD_TIME_SCENE;
	//cc.type = chatdef::CHANNEL_TYPE_SCENE;
	//STRNCPY(cc.name, m_scene_name, sizeof(cc.name));
	//InternalComm::Instance().NetSend(InternalComm::Instance().m_chat_server_id, (const char*)&cc, sizeof(gcprotocal::CreateChannel));

	/*int channel_id = this->GetChannelID();
	Channel *channel = ChannelManager::Instance().CreateChatChannel(channel_id, CHANNEL_CD_TIME_SCENE, chatdef::CHANNEL_TYPE_SCENE, m_scene_name);
	if (NULL == channel)
	{
		g_log_scene.printf(LL_DEBUG, "OnCreateChannel Fail, Channel is exist, cid:%d cooldown:%d type:%d name:%s .", 
			channel_id, CHANNEL_CD_TIME_SCENE, chatdef::CHANNEL_TYPE_SCENE, m_scene_name);
	}
	else
	{
		g_log_scene.printf(LL_DEBUG, "OnCreateChannel cid:%d cooldown:%d type:%d name:%s .", 
			channel_id, CHANNEL_CD_TIME_SCENE, chatdef::CHANNEL_TYPE_SCENE, m_scene_name);
	}*/

	gamelog::g_log_scene.printf(LL_DEBUG, "[%d:%d] %s start up.", m_scene_id, m_scene_key, m_scene_name);

	return true;
}

bool Scene::ReInit(const ConfigScene &configscene, Map *map)
{
	assert(NULL != map);

	m_map = map;

	if (configscene.id != m_scene_id) return false;

	m_town_point.scene_id = configscene.townpoint.sceneid;
	m_town_point.pos.Init(configscene.townpoint.scenex, configscene.townpoint.sceney);

	// 清除怪和采集点
	for (ObjManager::ObjIterator it = m_obj_manager.FirstObj(); it != m_obj_manager.LastObj(); ++it)
	{
		Obj *obj = *it;
		if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
		{
			Monster *monster = (Monster *)obj;
			if (monster->HasGenerater())
			{
				monster->SetGenerater(NULL);
				monster->ForceSetDead();
			}
		}
		else if (Obj::OBJ_TYPE_GATHER == obj->GetObjType())
		{
			GatherObj *gather = (GatherObj *)obj;
			if (gather->HasGenerater())
			{
				gather->SetGenerater(NULL);
				this->DeleteObj(gather->GetId());
			}
		}
	}

	bool ret = m_scene_monster_logic.Init(this, configscene.monsterpoints, configscene.gathers);
	if (!ret)
	{
		gamelog::g_log_scene.printf(LL_ERROR, "Scene::ReInit scene_monster_logic Return FAIL!");
		return false;
	}

	m_transport_list.Init(configscene.doors);
	m_npc_list.Init(configscene.npcs);

	return true;
}

void Scene::Release()
{
	/*int cid = this->GetChannelID();
	ChannelManager::Instance().DestroyChatChannel(cid);*/

	m_index = 0;
	m_zone_matrix.DestriyZoneMatrix();

	for (ObjManager::ObjIterator i = m_obj_manager.FirstObj(); i != m_obj_manager.LastObj(); ++i)
	{
		delete (*i);
	}
	m_obj_manager.Clear();

	m_scene_monster_logic.Release();

	m_role_manager.Release();
	m_monster_manager.Release();
	m_fallingitem_manager.Release();
	m_trigger_manager.Release();
	m_gather_manager.Release();

	m_remove_obj_list.clear();

//	gamelog::g_log_scene.printf(LL_DEBUG, "[%d:%d] %s release.", m_scene_id, m_scene_key, m_scene_name);
}

// 添加对象
void Scene::AddObj(Obj *obj)
{
	obj->SetScene(this);

	switch (obj->GetObjType())
	{
	case Obj::OBJ_TYPE_INVALID:
		gamelog::g_log_scene.printf(LL_WARNING, "Scene:%s Add Invalid Type Obj", m_scene_name);
		break;

	case Obj::OBJ_TYPE_ROLE:
		m_role_manager.Add(obj);
		break;

	case Obj::OBJ_TYPE_MONSTER:
		m_monster_manager.Add(obj);
		break;

	case Obj::OBJ_TYPE_FALLINGITEM:
		m_fallingitem_manager.Add(obj);
		break;

	case Obj::OBJ_TYPE_TRIGGER:
		m_trigger_manager.Add(obj);
		break;

	case Obj::OBJ_TYPE_GATHER:
		m_gather_manager.Add(obj);
		break;

	default:
		break;
	}

	m_obj_manager.Add(obj);
	m_special_logic->OnAddObj(obj);
	obj->OnEnterScene();
}

// 删除对象
bool Scene::DeleteObj(ObjID obj_id)
{
	if (!m_obj_manager.Exist(obj_id))
	{
		return false;
	}

	Obj *obj = m_obj_manager.GetObj(obj_id);
	if (NULL != obj && obj->IsValid())
	{
		if (!this->InStaticScene() && Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
		{
			Monster *monster = (Monster*)obj;

			{
				monster->IsBossMonster() ?  ++ m_fbsene_dyninfo.kill_boss : ++ m_fbsene_dyninfo.kill_monster;

				SceneFBDynInfo::KillMonsterCountMapIt it = m_fbsene_dyninfo.kill_monster_count_map.find(monster->GetMonsterId());
				if (it != m_fbsene_dyninfo.kill_monster_count_map.end())
				{
					++ it->second;
				}
				else
				{
					m_fbsene_dyninfo.kill_monster_count_map[monster->GetMonsterId()] = 1;
				}
			}
		}

		obj->OnLeaveScene();
		obj->SetInvalid(); 

		m_remove_obj_list.push_back(obj_id); // 延迟删除操作 让Obj可以调用DeleteObj将自己移除
	}
	else	
	{
		gamelog::g_log_debug.printf(LL_WARNING, "[Scene::DeleteObj Warring][sceneindex(%d) obj_id(%d) delete repeat]", m_index, obj_id);
	}

	return true;
}

// 获取场景状态
void Scene::GetSceneStatus(int *obj_num, int *role_num, int *monster_num, int *pet_num, int *drop_item_num) const
{
	if (NULL != obj_num) *obj_num = (int)m_obj_manager.Size();
	if (NULL != role_num) *role_num = (int)m_role_manager.Size();
	if (NULL != monster_num) *monster_num = (int)m_monster_manager.Size();
	if (NULL != drop_item_num) *drop_item_num = (int)m_fallingitem_manager.Size();
}

// 根据id获取角色
Role * Scene::GetPrincipal(ObjID obj_id)
{
	Obj *obj = this->GetObj(obj_id);
	if (NULL != obj)
	{
		if (Obj::OBJ_TYPE_ROLE == obj->GetObjType())
		{
			return (Role*)obj;
		}		
	}

	return NULL;
}

// 根据id获取采集物
GatherObj *Scene::GetGatherObj(ObjID obj_id)
{
	Obj *obj = this->GetObj(obj_id);
	if (NULL != obj)
	{
		if (Obj::OBJ_TYPE_GATHER == obj->GetObjType())
		{
			return (GatherObj*)obj;
		}		
	}

	return NULL;
}

Obj * Scene::GetObj(ObjID id)
{
	if (m_obj_manager.Exist(id))
	{
		Obj *obj = m_obj_manager.GetObj(id);
		
		if (!obj->IsValid()) return NULL; 

		return obj;
	}

	return NULL;
}

void RMIRoleInitBackObjectImpl::RoleInitRet(int ret, const RoleInitParam &p, 
									const RoleOtherInitParam &other_p,
									const ItemListParam &item_list_param,
									const HotkeyListParam &hotkey,
									const FriendListParam &friends,
									const BlacklistsParam &blacklists_param,
									const EnemyListParam &enemy,
									const BusinessListParam &business_param,
									const SkillListParam &skill_param,
									const DailyFindListParam &daily_find_param,
									long long last_save_time)
{
	if (0 != ret)
	{
		return;
	}

	if (!AsynReqRegister::Instance().OnAsynRspCheckNetidExist(netid, inc_key))
	{
		return;
	}

	// 当数据库回来之后，如果已经有一个号存在了，那么不能让他进来，不然出现影子，会被刷元宝道具之类的
	if (NULL != scene_manager->GetRole(user_id))
	{
		printf("RMIRoleInitBackObjectImpl::RoleInitRet role reapet !!! [error] %d \n", UidToInt(user_id));
		return;
	}

	Scene *scene = scene_manager->GetSceneByIndex(scene_index);
	if (NULL == scene)
	{
		return;
	}

	if (LocalConfig::Instance().IsShenheServer() && p.online_time <= 0)
	{
		//scene = scene_manager->GetRandomStaticScene();
	}

	RoleInitParam role_param = p;
	if (scene->GetSceneID() != p.scene_id)
	{
		role_param.scene_x = p.last_scene_x;
		role_param.scene_y = p.last_scene_y;
	}

	// 落在障碍区或者跳跃区自动回到回城点
	if ((!scene->GetMap()->Validate(Obj::OBJ_TYPE_ROLE, role_param.scene_x, role_param.scene_y))
		|| scene->GetMap()->JumpArea(role_param.scene_x, role_param.scene_y))
	{
		Posi tp_pos;
		int tp_scene = scene->GetTownPoint(NULL, &tp_pos);
		if (tp_scene != role_param.scene_id)
		{
			Scene *temp_scene = scene_manager->GetSceneById(tp_scene, 0);
			if (NULL != temp_scene)
			{
				role_param.scene_id = tp_scene;
				scene = temp_scene;
			}
		}

		role_param.scene_x = tp_pos.x;
		role_param.scene_y = tp_pos.y;
	}

	Role *role = 0;
	scene->RoleEnterScene(is_micro_pc, plat_spid, pname, user_id, netid, role_param, other_p, item_list_param, hotkey, friends, blacklists_param, enemy, 
		business_param, skill_param, daily_find_param, is_login, &role);

	if (is_login && NULL != role) // 是第一次登陆
	{
		role->OnLogin(last_save_time, userip, plat_spid);
		
		if (LocalConfig::Instance().IsShenheServer())
		{
			if (p.online_time <= 0)
			{
				role->ShenheServerRandomRole();
				Role::GenerateShenheNewRoleParam(p, other_p);
				role->Zhuanzhi();
				role->GetKnapsack()->ClearAll();
			}
			//scene_manager->GotoRandomStaticScene(role);
		}

		role->OnLoginFinish(last_save_time); // 登录完成，里面可能会执行SceneManager::GoTo，注意！！接下来role不可再使用

		RMIRoleSaveStatusBackObjectImpl *rssbo = new RMIRoleSaveStatusBackObjectImpl();
		rssbo->log_user_id = user_id;

		RMIRoleClient rc;
		rc.__bind_session(InternalComm::Instance().GetDBHandler().GetRoleDB());
		rc.RoleSaveStatusAsyn(UidToInt(user_id), false, is_micro_pc, rssbo);
	}
}

void RMIRoleSaveStatusBackObjectImpl::RoleSaveStatusRet(int ret)
{
	if (0 != ret)
	{
		Error(log_user_id, ERROR_EXECUTE, ret);
	}
}

void RMIQueryNameExistBackObjectImpl::QueryNameExistRet(int result)
{
	Role *role = World::GetInstWorld()->GetSceneManager()->GetRole(user_id);
	if (NULL == role) return;

	if (0 == result)
	{
		if (role->ResetName(role_name, is_item_reset))
		{
			RMIResetNameBackObjectImpl *impl = new RMIResetNameBackObjectImpl();
			impl->user_id = user_id;
			F_STRNCPY(impl->role_name, role_name, sizeof(GameName));

			RMILoginClient rc;
			rc.__bind_session(InternalComm::Instance().GetDBHandler().GetAcounterDB());

			rc.ResetNameAsyn(user_id.db_index, UidToInt(user_id), (const char*)role_name, impl);

			return;
		}
	}
	else
	{
		role->NoticeNum(errornum::EN_ROLE_NAME_REPEAT);
	}
}

void RMIResetNameBackObjectImpl::ResetNameRet(int result)
{
	if (0 != result)
	{
		// 失败
		gamelog::g_log_debug.printf(LL_INFO, "[RMIResetNameBackObjectImpl::ResetNameRet Fail_RoleNameMap][user[%s %d] result:%d]", role_name, UidToInt(user_id), result);
	}
}

bool Scene::RoleEnterSceneAsyn(bool is_micro_pc, const GSNetID &netid, IP userip, const UserID &user_id, PlatName pname, bool is_login, int plat_spid)
{
	RMIRoleInitBackObjectImpl *ribo = new RMIRoleInitBackObjectImpl();
	ribo->scene_manager = m_scene_manager;
	ribo->scene_index = m_index;
	ribo->scene_id = m_scene_id;
	ribo->netid = netid;
	ribo->userip = userip;
	ribo->user_id = user_id;
	ribo->plat_spid = plat_spid;
	ribo->is_login = is_login;
	F_STRNCPY(ribo->pname, pname, sizeof(ribo->pname));
	ribo->is_micro_pc = is_micro_pc;

	RMIRoleClient rc;
	rc.__bind_session(InternalComm::Instance().GetDBHandler().GetRoleDB());
	bool ret = rc.RoleInitAsyn(UidToInt(user_id), ribo);
	if (ret)
	{
		ribo->inc_key = AsynReqRegister::Instance().OnAsynReq(netid);
	}

	return ret;
}

void Scene::RoleEnterScene(bool is_micro_pc,
						   int plat_spid,
						   PlatName pname, 
						   const UserID & uid, 
						   const GSNetID &netid, 
						   const RoleInitParam &param, 
						   const RoleOtherInitParam &other_p,
						   const ItemListParam &item_list_param,
						   const HotkeyListParam &hotkey,
						   const FriendListParam &friends,
						   const BlacklistsParam &blacklists_param,
						   const EnemyListParam &enemy,
						   const BusinessListParam &business_param,
						   const SkillListParam &skill_param,
						   const DailyFindListParam &daily_find_param,
						   bool is_login,
						   Role **p_role)
{
	Role *role = new Role(netid, param.role_name);

	role->Init(is_micro_pc, plat_spid, pname, uid, param, other_p, item_list_param, hotkey, friends, blacklists_param, enemy, business_param,
			skill_param, daily_find_param);

	role->OnInitFinish();

	//addobj  之前必须算好属性.
	role->ReCalcAttr(CharIntAttrs::RECALC_REASON_INVALID, true, 0 == role->GetOnlineTime());

	this->AddObj(role);

	// 执行完RegisterRoleLocal之后SceneManager的m_net_to_rolelocal和m_uid_to_rolelocal才响应地有了映射。
	// 依赖这2个映射的函数如：SceneManager::GetRole才能正确找到Role对象
	// 才能保证在m_special_logic->OnRoleEnterScene中一定能get到role
	m_scene_manager->RegisterRoleLocal(netid, m_index, role->GetId());

	m_special_logic->OnRoleEnterScene(role);

	if (NULL != p_role) *p_role = role;

	this->SyncSceneTimeOut(role);

	if (is_login)
	{
		if (m_scene_manager->CheckIsDelayDeleteScene(m_scene_id))
		{
			m_is_delay_delete_scene = false; m_delay_delete_scene_time = 0;
		}
	}

	role->OnRoleBaseInfoChange();
}

// 角色离开场景
bool Scene::RoleLeaveScene(ObjID obj_id, bool is_logout)
{
	Obj *obj = this->GetObj(obj_id);
	if (NULL == obj || Obj::OBJ_TYPE_ROLE != obj->GetObjType())
	{
		gamelog::g_log_scene.printf(LL_WARNING, "Scene: [%d]%s OnLogout uid:%d is not exist in scene.", m_index, m_scene_name, obj_id);
		return false;
	}

	Role *role = (Role*)obj;
	
	GSNetID leaverole_netid = role->GetNetId();
	UserID leaverole_userid = role->GetUserId();
	
	m_special_logic->OnRoleLeaveScene(role, is_logout); // 离开场景的时候 回调保存下数据 其实可以放到角色身上 TODO  

	this->DeleteObj(obj_id);

	// 执行完LeaveScene之后SceneManager的m_net_to_rolelocal和m_uid_to_rolelocal的映射就被删除了。
	// 依赖这2个映射的函数如：SceneManager::GetRole就无法正确找到Role对象 
	// 所以小龙决定把UnregisterRoleLocal放到DeleteObj后面，这样才能在obj->OnLeaveScene触发的所有逻辑保证能get到role
	// UnregisterRoleLocal意味这个role彻底消失，不能再后续再添加任何逻辑
	// role不能再使用啦------------------------------------------------------------
	m_scene_manager->UnregisterRoleLocal(leaverole_netid, leaverole_userid);

	if (is_logout)
	{
		if (m_scene_manager->CheckIsDelayDeleteScene(m_scene_id))
		{
			m_is_delay_delete_scene = true; m_delay_delete_scene_time = EngineAdapter::Instance().Time() + 300;		// 保留5分钟
		}
		else 
		{
			//最新：组队副本下线后，重连不回到该场景。因为下线后，组队系统那里在那些组队类型下会认为离队。如果不这样，组队副本里就可能出现多个队伍的情况
// 			if (m_scene_manager->CheckIsTeamScene(m_scene_id))
// 			{
// 				// 在组队场景下线，重连还是能回到该场景，也合理 也就是在组队副本离队不影响重入该副本
// 				// 组队的逻辑跟场景重入逻辑 本来就应该 分离 
// 				TeamKeyRegister::Instance().RegisterTeamKey(leaverole_userid, m_scene_id, m_scene_key, (unsigned int)m_scene_create_time);
// 			}
			
			if (m_scene_manager->CheckIsGuildScene(m_scene_id))
			{
				GuildKeyRegister::Instance().RegisterGuildKey(leaverole_userid, m_scene_id, m_scene_key, (unsigned int)m_scene_create_time);
			}
		}
	}

	return true;
}

void Scene::Update(unsigned long interval, time_t now_second, unsigned int now_dayid)
{
	this->RemoveObj();
	this->UpdateObj(interval, now_second, now_dayid);
	m_scene_monster_logic.Update(interval);
	this->RemoveObj();

	if (m_is_timeout_kick_role && (m_scene_create_time + m_scene_timeout < now_second))
	{
		if (!m_special_logic->OnTimeout())
		{
			int role_num = m_role_manager.Size();  // fb 时间到 直接刷到 last_scene 

			for (int i = 0; i < role_num; ++i)
			{
				Role *role = (Role*)m_role_manager.GetObjByIndex(i);
				if (NULL == role) continue;

				Posi last_scene_pos;
				int last_scene_id = role->GetLastScene(&last_scene_pos);

				gamelog::g_log_debug.printf(LL_DEBUG, "Scene::Update m_is_timeout_kick_role: %s[%d %d]", role->GetName(), role->GetUserId().db_index, role->GetUserId().role_id);

				m_scene_manager->GoTo(role, last_scene_id, -1, last_scene_pos);
			}
		}
	}

	m_special_logic->Update(interval, now_second);

	if (0 == m_role_manager.Size() && m_special_logic->CanDestroy() && m_is_delay_delete_scene && now_second >= m_delay_delete_scene_time)
	{
		m_is_delay_delete_scene = false; m_scene_manager->DestroyFB(m_index);
	}

	// 婚宴副本 仙盟副本 不断检查场景是否可销毁 (要小心这类的场景)
	if (Scene::SCENE_TYPE_HUN_YAN_FB == m_scene_type || Scene::SCENE_TYPE_GUILD_FB == m_scene_type || 
		Scene::SCENE_TYPE_YAOSHOU_GUANGCHANG == m_scene_type || Scene::SCENE_TYPE_SUOYAOTA == m_scene_type ||
		Scene::SCENE_TYPE_FUN_TYPE_MOUNT == m_scene_type || Scene::SCENE_TYPE_FUN_TYPE_WING == m_scene_type ||
		Scene::SCENE_TYPE_FUN_TYPE_XIANNV == m_scene_type || Scene::SCENE_TYPE_FIGHTING == m_scene_type||
		Scene::SCENE_TYPE_CROSS_DARK_NIGHT == m_scene_type || Scene::SCENE_TYPE_FISHING == m_scene_type || 
		Scene::SCENE_TYPE_MILLIONAIRE == m_scene_type || Scene::SCENE_TYPE_NIGHT_FIGHT_FB == m_scene_type ||
		Scene::SCENE_TYPE_MESS_BATTLE_FB == m_scene_type || Scene::SCENE_TYPE_GUILD_QUESTION == m_scene_type || 
		Scene::SCENE_TYPE_CROSS_LIEKUN_FB == m_scene_type || Scene::SCENE_TYPE_CROSS_BIANJINGZHIDI == m_scene_type)
	{
		if (0 == m_role_manager.Size() && m_special_logic->CanDestroy())
		{
			m_scene_manager->DestroyFB(m_index);
		}
	}

	if (static_cast<unsigned int>(now_second) > m_world_boss_last_brocast_time)
	{
		m_world_boss_last_brocast_time = static_cast<unsigned int>(now_second) + 1;
		this->SendWorldBossHpInfo();

		if (this->IsGuajiScene(m_scene_id))
		{
			m_guiji_scene_forbid_pk = false;
			unsigned int now_hour = GetTodaySecond(static_cast<unsigned int>(now_second)) / SECOND_PER_HOUR;
			if (0 <= now_hour && now_hour < 6)
			{
				m_guiji_scene_forbid_pk = true;
			}
		}
	}
}

void Scene::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{
	m_special_logic->OnDayChange(old_dayid, now_dayid);
}

void Scene::UpdateObj(unsigned long interval, time_t now_second, unsigned now_dayid)
{
	if ((int)m_role_manager.Size() <= 0)
	{
		m_scene_no_one_update_interval += interval;
		if (m_scene_no_one_update_interval >= SCENE_NO_ONE_UPDATE_INTERVAL)
		{
			m_scene_no_one_update_interval = 0;
		}
		else
		{
			return;
		}
	}

	for (ObjManager::ObjIterator i = m_obj_manager.FirstObj(); i != m_obj_manager.LastObj(); ++i)
	{
		(*i)->Update(interval, now_second, now_dayid);
	}
}

void Scene::RemoveObj()
{
	if (m_remove_obj_list.size() != 0)
	{
		for (int i = 0; i < (int)m_remove_obj_list.size(); ++i)
		{
			if (!m_obj_manager.Exist(m_remove_obj_list[i]))
			{
				continue;
			}

			Obj *obj = m_obj_manager.GetObj(m_remove_obj_list[i]);
			if (NULL == obj) continue;

			m_special_logic->OnRemoveObj(obj);

			m_obj_manager.Remove(m_remove_obj_list[i]);

			switch (obj->GetObjType())
			{
			case Obj::OBJ_TYPE_INVALID:
				gamelog::g_log_scene.printf(LL_WARNING, "Scene:%s Remove Invalid Type Obj", m_scene_name);
				break;
			case Obj::OBJ_TYPE_ROLE:
				m_role_manager.Remove(obj);
				break;
			case Obj::OBJ_TYPE_MONSTER:
				m_monster_manager.Remove(obj);
				break;
			case Obj::OBJ_TYPE_FALLINGITEM:
				m_fallingitem_manager.Remove(obj);
				break;
			case Obj::OBJ_TYPE_TRIGGER:
				m_trigger_manager.Remove(obj);
				break;
			case Obj::OBJ_TYPE_GATHER:
				m_gather_manager.Remove(obj);
				break;
			default:
				break;
			}

			delete obj;
		}

		m_remove_obj_list.clear();

		if (!this->InStaticScene())
		{
			if (0 == m_role_manager.Size() && m_special_logic->CanDestroy() && !m_is_delay_delete_scene)
			{
				m_scene_manager->DestroyFB(m_index); 
			}
		}
	}
}

// 获取聊天频道
int Scene::GetChannelID()
{
	return chatdef::CHANNEL_ID_SCENE_BEGIN + EngineAdapter::Instance().m_server_index * 10000 + m_index;
}

void Scene::SyncSceneTimeOut(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (this->InStaticScene()) return;

	static Protocol::SCSceneFbTimeOut sfdi;
	sfdi.time_out = (0 == m_scene_timeout ? 0 : static_cast<unsigned int>(m_scene_create_time + m_scene_timeout));

	EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&sfdi, sizeof(sfdi));
}

unsigned int Scene::GetSceneTimeOutStamp()
{
	unsigned int time_out = (0 == m_scene_timeout ? 0 : static_cast<unsigned int>(m_scene_create_time + m_scene_timeout));
	return time_out;
}

int Scene::GetTownPoint(Role *role, Posi *pos)
{
	if (NULL != pos) *pos = m_town_point.pos;

	return m_town_point.scene_id;
}

void Scene::GetMonsterGeneraterInfo(Role *role)
{
	if (this->IsBossDongKuScene(m_scene_id) || this->IsDabaoBossScene(m_scene_id) || this->IsCampNormalDuoBaoScene(m_scene_id)  || 
		this->IsActiveBossScene(m_scene_id) || this->InCommonScene() || this->IsBossFamilyScene(m_scene_id) ||
		this->IsJuLingScene(m_scene_id) || this->IsBabyTrialScene(m_scene_id))
	{
		SceneMonsterLogic::MonsterGeneraterList &monster_gen_list = m_scene_monster_logic.GetMonsterGenList();

		static Protocol::SCMonsterGeneraterList mgl;
		mgl.boss_count = 0;

		for (SceneMonsterLogic::MonsterGeneraterListIt it = monster_gen_list.begin(); it != monster_gen_list.end(); ++ it)
		{
			MonsterGenerater * monster_generater = *it;
			if (NULL != monster_generater && mgl.boss_count < Protocol::SCENE_MONSTER_GENERATER_MAX_NUM)
			{
				const MonsterInitParam *monster_p = MONSTERPOOL->GetMonsterParam(monster_generater->GetMonsterID());
				if (NULL != monster_p && MonsterInitParam::MONSTER_TYPE_BOSS == monster_p->monster_type)
				{
					mgl.boss_list[mgl.boss_count].boss_id = monster_generater->GetMonsterID();
					mgl.boss_list[mgl.boss_count].next_refresh_time = monster_generater->GetMonsterNextFlushTime();
					++ mgl.boss_count;
				}
			}
		}

		int len = sizeof(mgl) - (Protocol::SCENE_MONSTER_GENERATER_MAX_NUM - mgl.boss_count) * sizeof(mgl.boss_list[0]);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&mgl, len);
	}
}

void Scene::GetGatherGeneraterInfo(Role *role)
{
	static Protocol::SCGatherGeneraterList mgl;
	mgl.gather_count = 0;

	const ShenzhouOtherCfg &other_cfg = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetOtherCfg();
	if (this->IsStaticScene(m_scene_type) && m_scene_id == other_cfg.gather_scene_id)
	{
		int gather_obj_num = this->GatherObjNum();
		for (int i = 0; i < gather_obj_num; i++)
		{
			GatherObj *gather_obj = this->GetGatherObjByIndex(i);
			if (NULL != gather_obj && !gather_obj->HasGather())
			{
				mgl.gather_list[mgl.gather_count].gather_id = gather_obj->GatherId();
				mgl.gather_list[mgl.gather_count].next_refresh_time = 0;
				const Posi &gather_pos = gather_obj->GetPos();
				mgl.gather_list[mgl.gather_count].pos_x = gather_pos.x;
				mgl.gather_list[mgl.gather_count].pos_y = gather_pos.y;
				++mgl.gather_count;
			}
		}

		int len = sizeof(mgl) - (Protocol::SCENE_GATHER_GENERATER_MAX_NUM - mgl.gather_count) * sizeof(mgl.gather_list[0]);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&mgl, len);
	}
	else if (this->IsShuijingScene(m_scene_id))
	{
		SceneMonsterLogic::GatherGeneraterList &gather_gen_list = m_scene_monster_logic.GetGatherGeneraterList();

		for (SceneMonsterLogic::GatherGeneraterListIt it = gather_gen_list.begin(); it != gather_gen_list.end() && mgl.gather_count < Protocol::SCENE_GATHER_GENERATER_MAX_NUM; ++it)
		{
			GatherGenerater *gather_generater = *it;
			if (NULL != gather_generater)
			{
					mgl.gather_list[mgl.gather_count].gather_id = gather_generater->GetGatherId();
					mgl.gather_list[mgl.gather_count].next_refresh_time = gather_generater->GetGatherNextFlushTime();
					Posi gather_pos = gather_generater->GetGatherPosi();
					mgl.gather_list[mgl.gather_count].pos_x = gather_pos.x;
					mgl.gather_list[mgl.gather_count].pos_y = gather_pos.y;
					++mgl.gather_count;
			}
		}

		int len = sizeof(mgl) - (Protocol::SCENE_GATHER_GENERATER_MAX_NUM - mgl.gather_count) * sizeof(mgl.gather_list[0]);
		EngineAdapter::Instance().NetSend(role->GetNetId(), (char*)&mgl, len);
	}
}

bool Scene::ResetScene(Role *role, bool reset_pos)
{
	if (SCENE_TYPE_COMMON_SCENE == m_scene_type) return false;	// 普通场景一定不能重置

	if (SCENE_TYPE_PATA_FB != m_scene_type 
		&& SCENE_TYPE_ZHANSHENDIAN_FB != m_scene_type 
		&& SCENE_TYPE_GUILD_MONSTER != m_scene_type 
		&& SCENE_TYPE_WUSHUANG_FB != m_scene_type
		&& SCENE_TYPE_RUNE_TOWER != m_scene_type
		&& SCENE_TYPE_TUITU_FB != m_scene_type) 
	{
		return false;
	}

	if (NULL == m_special_logic || !m_special_logic->IsAllMonsterDead()) //怪物死亡才可以重置
	{
		return false;
	}
	
	Posi town_pos;
	int town_scene_id = this->GetTownPoint(role, &town_pos);
	if (reset_pos && town_scene_id == m_scene_id) 
	{
		role->ResetPos(town_pos.x, town_pos.y);
	}
	
	m_special_logic->Reset();
	m_scene_monster_logic.Reset();
	
	m_scene_create_time = EngineAdapter::Instance().Time();
	m_fbsene_dyninfo.Reset();
	
	this->SyncSceneTimeOut(role);

	gamelog::g_log_debug.printf(LL_DEBUG, "Scene::ResetScene: %s[%d]", role->GetName(), UidToInt(role->GetUserId()));

	return true;
}

// 转到重生点
bool Scene::GoToTownPoint(Role *role)
{
	Posi tp_pos;
	int tp_scene = this->GetTownPoint(role, &tp_pos);

	gamelog::g_log_debug.printf(LL_DEBUG, "Scene::GoToTownPoint: %s[%d %d]", role->GetName(), role->GetUserId().db_index, role->GetUserId().role_id);

	return m_scene_manager->GoTo(role, tp_scene, -1, tp_pos);
}

bool Scene::ForbidPK()
{
	if (!m_forbid_pk)
	{
		if (this->IsGuajiScene(m_scene_id))
		{
			return m_guiji_scene_forbid_pk;
		}
	}

	return m_forbid_pk;
}

bool Scene::InCommonScene()
{
	return m_scene_type == SCENE_TYPE_COMMON_SCENE;
}

bool Scene::InStaticScene()
{
	return Scene::IsStaticScene(m_scene_type);
}

bool Scene::IsStaticScene(int scene_type)
{
	return SCENE_TYPE_COMMON_SCENE == scene_type || SCENE_TYPE_CAMP_STATION == scene_type ||  SCENE_TYPE_VIP_FB == scene_type;
}

bool Scene::IsCrossGuildBattleScene(int scene_type)
{
	return SCENE_TYPE_CROSS_GUILDBATTLE == scene_type;
}

bool Scene::IsDelayDeleteScene(int scene_type, int scene_id)
{
	UNSTD_STATIC_CHECK(88 == Scene::SCENE_TYPE_COUNT);


	if (SCENE_TYPE_COINFB == scene_type || SCENE_TYPE_EXPFB == scene_type || SCENE_TYPE_PERSONAL_TOWERDEFEND == scene_type ||
		SCENE_TYPE_PHASE_FB == scene_type || SCENE_TYPE_CHALLENGE_FB == scene_type || SCENE_TYPE_GUILD_MONSTER == scene_type || 
		SCENE_TYPE_STORY_FB == scene_type || SCENE_TYPE_ZHANSHENDIAN_FB == scene_type || SCENE_TYPE_SHENMOZHIXI_FB == scene_type ||
		SCENE_TYPE_ZHUANZHI_PERSONAL == scene_type || SCENE_TYPE_WUSHUANG_FB == scene_type || SCENE_TYPE_PATA_FB == scene_type ||
		SCENE_TYPE_CAMPGAOJI_DUOBAO == scene_type || SCENE_TYPE_MO_JIE == scene_type ||
		SCENE_TYPE_RUNE_TOWER == scene_type || SCENE_TYPE_DAILY_TASK_FB == scene_type || SCENE_TYPE_TUITU_FB == scene_type ||
		SCENE_TYPE_PERSONAL_EQUIPFB == scene_type || SCENE_TYPE_ARMOR_DEFEND == scene_type || SCENE_TYPE_PERSON_BOSS == scene_type ||
		SCENE_TYPE_XINMO_FB == scene_type || SCENE_TYPE_BUILD_TOWER == scene_type || SCENE_TYPE_HOLIDAY_GUARD_FB == scene_type)
	{
		return true;
	}

	return false;
}

bool Scene::IsPersonalScene(int scene_type, int scene_id)
{
	return Scene::IsDelayDeleteScene(scene_type, scene_id);
}

bool Scene::IsTeamScene(int scene_type, int scene_id)
{
	UNSTD_STATIC_CHECK(88 == Scene::SCENE_TYPE_COUNT);

	if (SCENE_TYPE_TAOSHOUJITAN_TEAM_FB == scene_type || SCENE_TYPE_LOVERS_FB == scene_type ||
		SCENE_TYPE_MIGONGXIANFU_TEAM == scene_type || SCENE_TYPE_TEAM_ZHUAGUI == scene_type || 
		SCENE_TYPE_TEAM_EQUIP_FB == scene_type || SCENE_TYPE_TEAM_TOWERDEFEND == scene_type ||
		SCENE_TYPE_TEAM_EQUIPFB == scene_type || SCENE_TYPE_EXPFB == scene_type)
	{
		return true;
	}

	return false;
}

bool Scene::IsGuildScene(int scene_type, int scene_id)
{

	UNSTD_STATIC_CHECK(88 == Scene::SCENE_TYPE_COUNT);

	if (SCENE_TYPE_GUILD_STATION == scene_type)
	{
		return true;
	}

	if (SCENE_TYPE_GUILD_FB == scene_type)
	{
		return true;
	}
	
	return false;
}

bool Scene::IsGuajiScene(int scene_id)
{
	if (scene_id >= 2000 && scene_id <= 2019)
	{
		return true;
	}

	return false;
}

bool Scene::IsWorldBossScene(int scene_id)
{
	if (scene_id >= 200 && scene_id <= 249)
	{
		return true;
	}

	return false;
}

bool Scene::IsWorldBossReliveScene(int scene_id)
{
	if (scene_id >= 250 && scene_id <= 299)
	{
		return true;
	}

	return false;
}

bool Scene::IsBossDongKuScene(int scene_id)
{
	if (scene_id >= 9010 && scene_id <= 9019)
	{
		return true;
	}

	return false;
}

bool Scene::IsDabaoBossScene(int scene_id)
{
	if (scene_id >= 9020 && scene_id <= 9029)
	{
		return true;
	}

	return false;
}

bool Scene::IsActiveBossScene(int scene_id)
{
	if (scene_id >= 9040 && scene_id <= 9049)
	{
		return true;
	}

	return false;
}

bool Scene::IsPerciousBossScene(int scene_id)
{
	if (scene_id == 1250)
	{
		return true;
	}

	return false;
}

bool Scene::IsActiveBossSceneOneLayer(int scene_id)
{
	if (scene_id == 9040)
	{
		return true;
	}

	return false;
}

bool Scene::IsCampNormalDuoBaoScene(int scene_id)
{
	if (scene_id >= 900 && scene_id <= 929)
	{
		return true;
	}

	return false;
}

bool Scene::IsJuLingScene(int scene_id)
{
	if (5004 == scene_id)
	{
		return true;
	}

	return false;
}

bool Scene::IsBabyTrialScene(int scene_id)
{
	if (5005 == scene_id)
	{
		return true;
	}

	return false;
}

bool Scene::IsQinYuanScene(int scene_id)
{
	if (5006 == scene_id)
	{
		return true;
	}

	return false;
}

bool Scene::IsCangBaoMiJing(int scene_id)
{
	if (8001 == scene_id)
	{
		return true;
	}

	return false;
}
bool Scene::IsShiFangShiJie(int scene_id)
{
	if (200 <= scene_id && scene_id <= 209)
	{
		return true;
	}

	return false;
}

bool Scene::IsBossFamilyScene(int scene_id)
{
	if ((scene_id >= 9000 && scene_id <= 9009) || (scene_id >= 9210 && scene_id <= 9219))
	{
		return true;
	}

	return false;
}

bool Scene::IsShangGuYiJi(int scene_id)
{
	int shangguyiji_scene_id = LOGIC_CONFIG->GetShenzhouWeaponCfg().GetOtherCfg().gather_scene_id;
	if (shangguyiji_scene_id == scene_id)
	{
		return true;
	}
	return false;
}

bool Scene::IsBossScene(int scene_id)
{
	return IsBossFamilyScene(scene_id) || IsBossDongKuScene(scene_id) || IsDabaoBossScene(scene_id) ||
		IsActiveBossScene(scene_id) || IsWorldBossScene(scene_id) || IsPerciousBossScene(scene_id);
}

bool Scene::IsShuijingScene(int scene_id)
{
	return scene_id == LOGIC_CONFIG->GetShuiJingCfg().GetShuijingScenId();
}

bool Scene::IsYaoShouGuangChangScene(int scene_type, int scene_id)
{
	if (SCENE_TYPE_YAOSHOU_GUANGCHANG == scene_type)
	{
		return true;
	}

	return false;
}

bool Scene::IsSuoYaoTaScene(int scene_type, int scene_id)
{
	if (SCENE_TYPE_SUOYAOTA == scene_type)
	{
		return true;
	}

	return false;
}

bool Scene::SendToRole(const char *msg, int length, RoleFilterFunc role_filter, void *func_param)
{
	int role_num = (int)m_role_manager.Size();

	static const int MAX_ROLE_NUM_IN_SCENE = 8196;
	static GSNetID send_netid[MAX_ROLE_NUM_IN_SCENE];

	if (role_num > MAX_ROLE_NUM_IN_SCENE) role_num = MAX_ROLE_NUM_IN_SCENE;

	int send_num = 0;
	if (NULL == role_filter)
	{
		for (int i = 0; i < role_num; ++i)
		{
			Role *role = (Role*)m_role_manager.GetObjByIndex(i);
			if (NULL != role)
			{
				send_netid[send_num++] = role->GetNetId();
			}
		}
	}
	else
	{
		for (int i = 0; i < role_num; ++i)
		{
			Role *role = (Role*)m_role_manager.GetObjByIndex(i);
			if (NULL != role)
			{
				if (role_filter(role, func_param))
				{
					send_netid[send_num++] = role->GetNetId();
				}
			}
		}
	}

	return EngineAdapter::Instance().NetMulSend(send_netid, send_num, msg, length);
}

bool Scene::SendToRole(const char *msg, int length, std::function<bool(Role *)> filter_func)
{
	int role_num = (int)m_role_manager.Size();

	static const int MAX_ROLE_NUM_IN_SCENE = 8196;
	static GSNetID send_netid[MAX_ROLE_NUM_IN_SCENE];

	if (role_num > MAX_ROLE_NUM_IN_SCENE) role_num = MAX_ROLE_NUM_IN_SCENE;

	int send_num = 0;
	if (NULL == filter_func)
	{
		for (int i = 0; i < role_num; ++i)
		{
			Role *role = (Role*)m_role_manager.GetObjByIndex(i);
			if (NULL != role)
			{
				send_netid[send_num++] = role->GetNetId();
			}
		}
	}
	else
	{
		for (int i = 0; i < role_num; ++i)
		{
			Role *role = (Role*)m_role_manager.GetObjByIndex(i);
			if (NULL != role)
			{
				if (filter_func(role))
				{
					send_netid[send_num++] = role->GetNetId();
				}
			}
		}
	}

	return EngineAdapter::Instance().NetMulSend(send_netid, send_num, msg, length);
}

void Scene::NotifySystemMsg(char *str_buff, int str_len, int notice_type)
{
	if (NULL != str_buff && str_len > 0)
	{
		int sendlen = 0;
		static Protocol::SCSystemMsg sm;

		if (SceneManager::CreateSystemMsg(&sm, &sendlen, str_buff, str_len, notice_type))
		{
			this->SendToRole((const char*)&sm, sendlen);
		}
	}
}

void Scene::SendWorldBossHpInfo()
{
	if (!this->IsWorldBossScene(m_scene_id))
	{
		return;
	}

	static Protocol::SCWorldBossHPInfo boss_hp_info;
	boss_hp_info.boss_id = 0;
	boss_hp_info.cur_hp = 0;
	boss_hp_info.max_hp = 0;
	boss_hp_info.shield_hp = 0;

	for (unsigned int i = 0; i < m_monster_manager.Size(); i++)
	{
		Monster *worldboss = this->GetMonsterByIndex(i);
		if (NULL != worldboss && worldboss->IsWorldBossMonster())
		{
			boss_hp_info.boss_id = worldboss->GetMonsterId();
			boss_hp_info.cur_hp = worldboss->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			boss_hp_info.max_hp = worldboss->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			boss_hp_info.shield_hp = 0;

			EffectSpecialShield *shield_effect = worldboss->GetEffectList()->GetEffectSpecialShield();
			if (NULL != shield_effect)
			{
				boss_hp_info.shield_hp = shield_effect->GetLeftEffectiveTimes();
			}

			break;
		}
	}

	this->SendToRole((const char*)&boss_hp_info, sizeof(boss_hp_info));
}

void Scene::CheckRoleTimeOut(unsigned long now)
{
	int role_num = (int)m_role_manager.Size();

	for (int i = 0; i < role_num; ++i)
	{
		static const unsigned int hearbeat_dead_time = ServerConfigPool::Instance().common_config.timeout.gameworld_heartbeat_dead_time_ms;

		Role *role = (Role*)m_role_manager.GetObjByIndex(i);
		unsigned long last_client_msg_time = role->GetLastMsgFromClientTime();

		// 这里之前有严重的bug game_time有可能小于last_active_time 因为对时导致game_time变小 一减就变成负的 unsigned 就跪了
		if ((now > last_client_msg_time) && (now - last_client_msg_time > (unsigned long)(hearbeat_dead_time * 1.5f)))	// 暂时先这样，等确定了问题后再改
		{
			m_scene_manager->RoleTimeOut(this, role, "CheckRoleTimeout");
		}
	}
}

void Scene::CheckRoleSpeedup()
{
	if (!GLOBALCONFIG->IsOpenSpeedupHelloCheck())
	{
		return;
	}

	int role_num = (int)m_role_manager.Size();

#ifndef _DEBUG

	time_t now_t = EngineAdapter::Instance().Time();

	for (int i = 0; i < role_num; ++i)
	{
		Role *role = (Role*)m_role_manager.GetObjByIndex(i);
		if (NULL == role) continue;

		int speed_msg_count = role->GetSpeedupHelloCount();
		role->ClearSpeedupHello();

		if (speed_msg_count > GLOBALCONFIG->GetSpeedupHelloMax())
		{
			// 如果30S内收到太多的包，检测累计收了多少个
			int speed_msg_total_count = role->GetSpeedupHelloTotalCount(); 
			time_t first_hello_time = role->GetFirstSpeedupHelloTime();

			int limit_hello_count = static_cast<int>((now_t - first_hello_time) / 1.5f);

			int real_limit_hello_count = static_cast<int>(limit_hello_count * GLOBALCONFIG->GetSpeedupHelloAllPackPercentLimit() / 100.0);
			if (speed_msg_total_count >= real_limit_hello_count)
			{
				// 经过确认，累计的量也真的太大了  禁之踢之
				role->ForbidRole("SpeedUp", GLOBALCONFIG->GetSpeedupHelloCountForbitTimeS());
				m_scene_manager->RoleTimeOut(this, role, "SpeedUp", speed_msg_count);
			}
		}
	}

#else

	for (int i = 0; i < role_num; ++i)
	{
		Role *role = (Role*)m_role_manager.GetObjByIndex(i);
		if (NULL == role) continue;

		role->ClearSpeedupHello();
	}

#endif
}

void Scene::OnTrigger(int trigger_cond_type, Role *role, int trigger_cond_param0, int trigger_cond_param1)
{
	int trigger_num = static_cast<int>(m_trigger_manager.Size());
	for (int i = 0; i < trigger_num; ++ i)
	{
		Obj *obj = m_trigger_manager.GetObjByIndex(i);
		if (NULL != obj && Obj::OBJ_TYPE_TRIGGER == obj->GetObjType())
		{
			Trigger *trigger = (Trigger*)obj;
			trigger->OnTrigger(trigger_cond_type, role, trigger_cond_param0, trigger_cond_param1);
		}
	}
}

bool Scene::HasHiddenMonster()
{
	int trigger_num = static_cast<int>(m_trigger_manager.Size());
	for (int i = 0; i < trigger_num; ++ i)
	{
		Obj *obj = m_trigger_manager.GetObjByIndex(i);
		if (NULL != obj && Obj::OBJ_TYPE_TRIGGER == obj->GetObjType())
		{
			Trigger *trigger = (Trigger*)obj;
			if (trigger->HasHiddenMonster())
			{
				return true;
			}
		}
	}

	return false;
}

// 根据index获取角色
Role * Scene::GetRoleByIndex(int index)
{
	Role *role = (Role*)m_role_manager.GetObjByIndex(index);
	if (NULL != role && role->IsValid())
	{
		return role;
	}

	return NULL;
}

// 根据uid获取角色
Role * Scene::GetRoleByUID(int uid)
{
	for (int j = 0; j < (int)this->RoleNum(); ++ j)
	{
		Role *temp_role = this->GetRoleByIndex(j);
		if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType() && uid == temp_role->GetUID())
		{
			return temp_role;
		}
	}

	return NULL;
}

Role * Scene::GetRoleByUUID(const UniqueUserID &uuid)
{
	for (int j = 0; j < (int)this->RoleNum(); ++ j)
	{
		Role *temp_role = this->GetRoleByIndex(j);
		if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType() && uuid == temp_role->GetRoleCross()->GetCrossOriginalUniqueUserID())
		{
			return temp_role;
		}
	}

	return NULL;
}

Role * Scene::RandGetOneRole(int exclude_uid)
{
	if (this->RoleNum() <= 0) return NULL;

	int role_index = RandomNum(this->RoleNum());
	Role *temp_role = this->GetRoleByIndex(role_index);
	if (NULL != temp_role && Obj::OBJ_TYPE_ROLE == temp_role->GetObjType())
	{
		if (0 != exclude_uid && exclude_uid == UidToInt(temp_role->GetUserId()))
		{
			if (role_index < static_cast<int>(this->RoleNum() - 1))
			{
				++ role_index;
				return this->GetRoleByIndex(role_index);
			}
			else if (role_index > 0)
			{
				-- role_index;
				return this->GetRoleByIndex(role_index);
			}

			return NULL;
		}

		return temp_role;
	}

	return NULL;
}

// 根据index获取怪物
Monster * Scene::GetMonsterByIndex(int index)
{
	Monster *monster = (Monster*)m_monster_manager.GetObjByIndex(index);
	if (NULL != monster && monster->IsValid())
	{
		return monster;
	}

	return NULL;
}

Monster *Scene::GetMonster(ObjID obj_id)
{
	Obj *obj = this->GetObj(obj_id);
	if (NULL != obj)
	{
		if (Obj::OBJ_TYPE_MONSTER == obj->GetObjType())
		{
			return (Monster*)obj;
		}
	}

	return NULL;
}

// 根据index获取掉落物
FallingItem * Scene::GetFallingItemByIndex(int index)
{
	FallingItem *falling_item = (FallingItem*)m_fallingitem_manager.GetObjByIndex(index);
	if (NULL != falling_item && falling_item->IsValid())
	{
		return falling_item;
	}

	return NULL;
}

GatherObj * Scene::GetGatherObjByIndex(int index)
{
	GatherObj *gather = (GatherObj*)m_gather_manager.GetObjByIndex(index);
	if (NULL != gather && gather->IsValid())
	{
		return gather;
	}

	return NULL;
}

RoleShadow * Scene::GetRoleShadow(ObjID obj_id)
{
	Obj *obj = this->GetObj(obj_id);
	if (NULL != obj)
	{
		if (Obj::OBJ_TYPE_ROLE_SHADOW == obj->GetObjType())
		{
			return (RoleShadow*)obj;
		}
	}

	return NULL;
}

void Scene::DeleteGatherObjByGatherId(int gather_id)
{
	for (ObjManager::ObjIterator i = m_obj_manager.FirstObj(); i != m_obj_manager.LastObj(); ++i)
	{
		if (Obj::OBJ_TYPE_GATHER == (*i)->GetObjType())
		{
			GatherObj *gather_obj = (GatherObj *)(*i);
			if (gather_obj->GatherId() == gather_id)
			{
				this->DeleteObj(gather_obj->GetId());
			}
		}
	}
}

bool Scene::IsDropRecordBossScene()
{
	if (m_scene_type == Scene::SCENE_TYPE_BABY_BOSS || m_scene_type == Scene::SCENE_TYPE_SHANGGU_BOSS
		|| m_scene_type == Scene::SCENE_TYPE_PERSON_BOSS || m_scene_type == Scene::SCENE_TYPE_GODMAGIC_BOSS)
	{
		return true;
	}

	if (this->IsWorldBossScene(m_scene_id) || this->IsBossDongKuScene(m_scene_id) ||
		this->IsBossFamilyScene(m_scene_id) || this->IsActiveBossScene(m_scene_id) ||
		this->IsDabaoBossScene(m_scene_id))
	{
		return true;
	}

	return false;
}