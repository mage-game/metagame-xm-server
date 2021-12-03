
#include "scenemanagerconfig.h"
#include "common/tinyxml/tinyxml.h"
#include "servercommon/configcommon.h"
#include "scene/scene.h"
#include "gamelog.h"
#include "obj/otherobj/trigger.h"
#include "scene/sceneregister/sceneregister.hpp"
#include "scene/map.h"
#include "servercommon/pugixml/pugixml_adapter.hpp"

bool SceneManagerConfig::CheckMap(int mapid, int x, int y) const
{
	if ( 0> x || 0 > y)
	{
		return false;
	}

        std::map<int, MapItemForCheck>::const_iterator iter = m_map_for_check.find(mapid);
	if (iter == m_map_for_check.end())
	{
		return false;
	}

	if (iter->second.width >= x || iter->second.height >= y)
	{
		return true;
	}

// 不用这个vector，改成用map，时间复杂度从O(N)改成O(Log(N))，提高效率
//	for (ConfigMapList::const_iterator map_iter = configmanager.maps.begin();
//		map_iter != configmanager.maps.end(); ++map_iter)
//	{
//		if (map_iter->map.id == mapid)
//		{
//			if (map_iter->map.width >= x
//				|| map_iter->map.height >= y)
//			{
//				return true;
//			}
//		}
//	}

	return false;
}

bool SceneManagerConfig::Load(const std::string &dir,const std::string &configname, int total_server, int server_index)
{
	if (total_server <= 0 || server_index < 0) return false;

	char errinfo[1024] = {0};

	PugiXmlDocument document;
	PUGI_XML_LOAD_AND_CHECK(document, (dir + configname), "SceneManagerConfig", lasterr);

	PugiXmlNode RootElement = document.document_element();
	PugiXmlNode currentElement;

	if (!RootElement)
	{
		lasterr = configname + ": xml node error in root";
		return false;
	}

	//开始加载

	//处理maps
	currentElement = RootElement.child("maps");
	if (!currentElement)
	{
		lasterr = configname + ": xml node error in node [maps]. ";
		return false;
	}
	else
	{
		PugiXmlNode subElement = currentElement.first_child();
		while (!subElement.empty())
		{
			ConfigMapItem item;

			if (!PugiGetSubNodeValue(subElement, "path", item.path))
			{
				lasterr = configname + ": xml node error in node [scenemanager->maps->path] ";
				return false;
			}

			if (!item.map.Load(dir + item.path))
			{
				lasterr = dir + item.path + ":" + item.map.GetError();
				return false;
			}
	
			if (item.map.id >= 10000)
			{
				lasterr = dir + item.path + " id >= 10000";
				return false;
			}

			//if (101 == item.map.id || 102 == item.map.id || 103 == item.map.id)  // 新手村、神木林、问剑平原进行复制 
			//{
			//	int old_mapid = item.map.id;  // 保存旧ID

			//	for (int i = 0; i < 3; i++)
			//	{
			//		int new_mapid = old_mapid * 100 + i;
			//		if (SceneRegister::Instance().IsThisGSScene(total_server, server_index, new_mapid))
			//		{
			//			item.map.id = new_mapid;
			//			configmanager.maps.push_back(item);
			//			gamelog::g_log_world.printf(LL_INFO, "Load Clone Map [%d]->[%d] Config Success.", old_mapid, new_mapid);
			//		}
			//	}

			//	item.map.id = old_mapid;  // 恢复ID
			//}

			if (!SceneRegister::Instance().IsThisGSScene(total_server, server_index, item.map.id))
			{
				subElement = subElement.next_sibling();
				continue;
			}

//			for(ConfigMapList::const_iterator mapiter = configmanager.maps.begin();
//				mapiter != configmanager.maps.end(); ++mapiter)
//			{
//				if (item.map.id == mapiter->map.id)
//				{
//					sprintf(errinfo,"%s: %s map id[%d] repeat.",configname.c_str(), item.path.c_str(), item.map.id);
//					lasterr = errinfo;
//					return false;
//				}
//			} 用下面的代码替换上面这堆，稳定后可以把上面这堆注释删除
                        std::map<int, MapItemForCheck>::iterator mapiter = m_map_for_check.find(item.map.id);
			if (mapiter != m_map_for_check.end())
			{
				SNPRINTF(errinfo, sizeof(errinfo), "%s: %s map id[%d] repeat.", configname.c_str(), item.path.c_str(), item.map.id);
				lasterr = errinfo;
				return false;
			}

			//gamelog::g_log_world.printf(LL_INFO, "Load Map[%d] Config Success.", item.map.id);
			configmanager.maps.push_back(item);

			MapItemForCheck &item_for_check = m_map_for_check[item.map.id];
			item_for_check.id = item.map.id;
			item_for_check.width = item.map.width;
			item_for_check.height = item.map.height;
			subElement = subElement.next_sibling();
		}
	}

	//处理scenes
	currentElement = RootElement.child("scenes");
	if (!currentElement)
	{
		lasterr = configname + ": xml node error in node [scenes]. ";
		return false;
	}
	else
	{
		PugiXmlNode subElement = currentElement.first_child();
		while (!subElement.empty())
		{
			ConfigSceneItem item;

			if ( !PugiGetSubNodeValue(subElement, "path", item.path) )
			{
				lasterr = configname + ": xml node error in node [scenemanager->scenes->path] ";
				return false;
			}
			if ( !PugiGetSubNodeValue(subElement, "scene_type", item.scene_type) )
			{
				lasterr = configname + ": xml node error in node [scenemanager->scenes->scene_type] ";
				return false;
			}

			if (item.scene_type < 0 || item.scene_type >= Scene::SCENE_TYPE_COUNT)
			{
				lasterr = configname + ": xml node error in node [scenemanager->scenes->scene_type], scene_type unknow.";
				return false;
			}

			if ( !item.scene.Load(dir,item.path) )
			{
				lasterr = item.scene.GetError();
				return false;
			}

			if (Scene::SCENE_TYPE_COMMON_SCENE != item.scene_type) // 如果是副本复活点必须是本场景
			{
				if (item.scene.id != item.scene.townpoint.sceneid)
				{
					printf("townpoint error scene_id:%d townpoint_scene_id:%d\n", item.scene.id, item.scene.townpoint.sceneid);
					item.scene.townpoint.sceneid = item.scene.id;
				}
			}

			if (!this->IsMatch(item.scene_type, item.scene.id))
			{
				char err_str[256] = {0};
				sprintf(err_str, " scene_type:%d not match scene_id:%d", item.scene_type, item.scene.id);

				lasterr = configname + err_str;
				return false;
			}

			SceneRegister::Instance().AddScene(item.scene.id);

			for(ConfigMapList::const_iterator mapiter = configmanager.maps.begin(); // 复活坐标不能在阻挡区
				mapiter != configmanager.maps.end(); ++mapiter)
			{
				if (item.scene.mapid == mapiter->map.id)
				{
					Map tmp_map(mapiter->map);

					if (!tmp_map.Validate(Obj::OBJ_TYPE_ROLE, item.scene.townpoint.scenex, item.scene.townpoint.sceney))
					{
						char err_str[256] = {0};
						sprintf(err_str, "scene_id:%d towerpoint[%d, %d] in invalid point(is save area %c)", 
								item.scene.id, item.scene.townpoint.scenex, item.scene.townpoint.sceney, tmp_map.GetType(item.scene.townpoint.scenex, item.scene.townpoint.sceney));

						lasterr = configname + err_str;
						return false;
					}
				}
			}

			//if (101 == item.scene.id || 102 == item.scene.id || 103 == item.scene.id)  // 新手村、神木林、问剑平原进行复制
			//{
			//	int old_sceneid = item.scene.id;  // 保存旧ID
			//	int old_mapid = item.scene.mapid;

			//	for (int i = 0; i < 3; i++)
			//	{
			//		int new_sceneid = old_sceneid * 100 + i;
			//		int new_mapid = old_mapid * 100 + i;
			//		if (SceneRegister::Instance().IsThisGSScene(total_server, server_index, new_sceneid))
			//		{
			//			item.scene.id = new_sceneid;
			//			item.scene.mapid = new_mapid;

			//			ConfigSceneDoorList old_doors = item.scene.doors;	// 保存旧传送点

			//			for (ConfigSceneDoorList::iterator door_it = item.scene.doors.begin(); door_it != item.scene.doors.end(); ++door_it)
			//			{
			//				if (101 == door_it->targetscene.sceneid || 102 == door_it->targetscene.sceneid || 103 == door_it->targetscene.sceneid)
			//				{
			//					door_it->targetscene.sceneid = door_it->targetscene.sceneid * 100 + i;
			//				}
			//			}

			//			configmanager.scenes.push_back(item);

			//			item.scene.doors = old_doors;	// 恢复传送点
			//			gamelog::g_log_world.printf(LL_INFO, "Load Clone Scene [%d]->[%d] Config Success.", old_sceneid, new_sceneid);
			//		}
			//	}

			//	item.scene.id = old_sceneid;  // 恢复ID
			//	item.scene.mapid = old_mapid;
			//}

			if (!SceneRegister::Instance().IsThisGSScene(total_server, server_index, item.scene.id))
			{
				subElement = subElement.next_sibling();
				continue;
			}

			for(ConfigSceneList::const_iterator scene_iter = configmanager.scenes.begin();
				scene_iter != configmanager.scenes.end(); ++scene_iter)
			{
				if (scene_iter->scene.id == item.scene.id)
				{
					sprintf(errinfo,"%s: %s scene id[%d] repeat.",configname.c_str(), item.path.c_str(), item.scene.id);
					lasterr = errinfo;
					return false;
				}
			}

			//gamelog::g_log_world.printf(LL_INFO, "Load Scene[%d] Config Success.", item.scene.id);
			configmanager.scenes.push_back(item);
			subElement = subElement.next_sibling();
		}
	}

	// 检查场景和地图配置
	for(ConfigSceneList::const_iterator iter = configmanager.scenes.begin();
		iter != configmanager.scenes.end(); ++iter)
	{
		// 检查npc相关
		for(ConfigSceneNpcList::const_iterator npc_iter = iter->scene.npcs.begin();
			npc_iter != iter->scene.npcs.end(); ++npc_iter)
		{
			if (!CheckMap(iter->scene.mapid,npc_iter->x,npc_iter->y))
			{
				SNPRINTF(errinfo, sizeof(errinfo), "%s Scene[%d] : the [x:%d y:%d] of npc is error where npc id is %d.", configname.c_str(), iter->scene.id, npc_iter->x, npc_iter->y, npc_iter->id);
				lasterr = errinfo;
				return false;
			}

			//for(ConfigSceneNpcFuntionList::const_iterator npcfunc_iter = npc_iter->functions.begin();
			//	npcfunc_iter != npc_iter->functions.end(); ++ npcfunc_iter)
			//{
			//	if (npcfunc_iter->npc_type != NPCFunction::FTYPE_TRANSMIT)
			//	{
			//		continue;
			//	}

			//	int mapid = -1;
			//	for(ConfigSceneList::const_iterator npcfunc_scenen_iter = configmanager.scenes.begin();
			//		npcfunc_scenen_iter != configmanager.scenes.end(); ++npcfunc_scenen_iter)
			//	{
			//		if ((int)npcfunc_iter->transportpoint.scene_id == npcfunc_scenen_iter->scene.id)
			//		{
			//			mapid = npcfunc_scenen_iter->scene.mapid;
			//			break;
			//		}
			//	}

			//	if (!CheckMap(mapid, npcfunc_iter->transportpoint.scene_x, npcfunc_iter->transportpoint.scene_y))
			//	{
			//		sprintf(errinfo,"%s : the [x:%d y:%d] of transport point of npc function is error where npc id is %d .",configname.c_str(),npc_iter->x,npc_iter->y, npc_iter->id);
			//		lasterr = errinfo;
			//		return false;
			//	}
			//}
		}

		//// 检查传送门坐标
// 		for(ConfigSceneDoorList::const_iterator door_iter = iter->scene.doors.begin();
// 			door_iter != iter->scene.doors.end(); ++door_iter)
// 		{
// 			if (!CheckMap(iter->scene.mapid, door_iter->x,door_iter->y))
// 			{
// 				sprintf(errinfo,"%s: the position of door[id:%d x:%d y:%d] on scene[id:%d] is error .",
// 					configname.c_str(), door_iter->id, door_iter->x,door_iter->y, iter->scene.id);
// 				lasterr = errinfo;
// 				return false;
// 			}
// 
// 			int doormapid = -1;
// 			for(ConfigSceneList::const_iterator door_scenen_iter = configmanager.scenes.begin();
// 				door_scenen_iter != configmanager.scenes.end(); ++door_scenen_iter)
// 			{
// 				if (iter->scene.townpoint.sceneid == door_scenen_iter->scene.id)
// 				{
// 					doormapid = door_scenen_iter->scene.mapid;
// 					break;
// 				}
// 			}
// 
// 			if (!CheckMap(doormapid, door_iter->targetscene.x,door_iter->targetscene.y))
// 			{
// 				sprintf(errinfo,"%s : the [x:%d y:%d] of target scene of scene door is error where door id is %d.",
// 					configname.c_str(), door_iter->targetscene.x,door_iter->targetscene.y, door_iter->id);
// 				lasterr = errinfo;
// 				return false;
// 			}
// 		}

		// 检查触发区域坐标
		for(ConfigSceneTriggerAreaList::const_iterator area_iter = iter->scene.triggerareas.begin();
			area_iter != iter->scene.triggerareas.end(); ++area_iter)
		{
			if (!CheckMap(iter->scene.mapid,area_iter->x,area_iter->y))
			{
				SNPRINTF(errinfo, sizeof(errinfo), "%s Scene[%d] : the [x:%d y:%d] of trigger area is error.", configname.c_str(), iter->scene.id, area_iter->x, area_iter->y);
				lasterr = errinfo;
				return false;
			}
			if (!CheckMap(iter->scene.mapid,area_iter->x+area_iter->range,area_iter->y+area_iter->range))
			{
				SNPRINTF(errinfo, sizeof(errinfo), "%s Scene[%d] : the [range:%d] of trigger area is error.", configname.c_str(), iter->scene.id, area_iter->range);
				lasterr = errinfo;
				return false;
			}
		}

		//// 检查复活点坐标
		//int relivemapid = -1;
		//for(ConfigSceneList::const_iterator relive_scenen_iter = configmanager.scenes.begin();
		//	relive_scenen_iter != configmanager.scenes.end(); ++relive_scenen_iter)
		//{
		//	if (iter->scene.townpoint.sceneid == relive_scenen_iter->scene.id)
		//	{
		//		relivemapid = relive_scenen_iter->scene.mapid;
		//		break;
		//	}
		//}
		//if (-1 == relivemapid || !CheckMap(relivemapid,iter->scene.townpoint.scenex,iter->scene.townpoint.sceney))
		//{
		//	sprintf(errinfo,"%s : the [x:%d y:%d] of relive point in scene[%d] is error.",configname.c_str(),
		//		iter->scene.townpoint.scenex,iter->scene.townpoint.sceney, iter->scene.townpoint.sceneid);
		//	lasterr = errinfo;
		//	return false;
		//}

		// 检查怪物生长点坐标
		for (ConfigSceneMonsterPointList::const_iterator monsterpoint_iter = iter->scene.monsterpoints.begin();
			monsterpoint_iter != iter->scene.monsterpoints.end(); ++monsterpoint_iter)
		{
			if (!CheckMap(iter->scene.mapid, monsterpoint_iter->x, monsterpoint_iter->y))
			{
				SNPRINTF(errinfo, sizeof(errinfo), "%s Scene[%d] : the [x:%d y:%d] of monster generater point is error.", configname.c_str(), iter->scene.id, 
					monsterpoint_iter->x, monsterpoint_iter->y);
				lasterr = errinfo;
				return false;
			}
		}

		// 检查采集点坐标
		for (ConfigSceneGatherList::const_iterator gather_iter = iter->scene.gathers.begin();
			gather_iter != iter->scene.gathers.end(); ++gather_iter)
		{
			if (!CheckMap(iter->scene.mapid, gather_iter->x, gather_iter->y))
			{
				SNPRINTF(errinfo, sizeof(errinfo), "%s Scene[%d] : the [x:%d y:%d] id[%d] of gather generater point is error.", configname.c_str(), iter->scene.id, 
					gather_iter->x, gather_iter->y, gather_iter->id);
				lasterr = errinfo;
				return false;
			}
		}

		// 检查触发器相关
		for(ConfigSceneTriggerList::const_iterator trigger_iter = iter->scene.triggers.begin();
			trigger_iter != iter->scene.triggers.end(); ++ trigger_iter)
		{
			std::string check_trigger_err;
			if (!Trigger::CheckTriggerConfig(*trigger_iter, this, iter->scene.mapid, &check_trigger_err))
			{
				SNPRINTF(errinfo, sizeof(errinfo), "%s Scene[%d] : %s", configname.c_str(), iter->scene.id, check_trigger_err.c_str());
				lasterr = errinfo;
				return false;
			}
		}
	}

	return true;
}

bool SceneManagerConfig::IsMatch(int scene_type, int scene_id) const
{
// 	switch (scene_type)
// 	{
// 	case Scene::SCENE_TYPE_GUILD_STATION:
// 		{
// 			if (107 != scene_id) return false;
// 		}
// 		break;
// 
// 	case Scene::SCENE_TYPE_ZHUXIE:
// 		{
// 			if (1202 != scene_id) return false;
// 		}
// 		break;
// 
// 	case Scene::SCENE_TYPE_QUNXIANLUANDOU:
// 		{
// 			if (1001 != scene_id) return false;
// 		}
// 		break;
// 	}

	return true;
}
