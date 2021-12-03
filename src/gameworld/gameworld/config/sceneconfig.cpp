
#include "sceneconfig.h"
#include "servercommon/configcommon.h"
#include "servercommon/servercommon.h"

#include "item/itempool.h"
#include "monster/monsterpool.h"

#include "checkresourcecenter.hpp"

#include "scene/scene.h"
#include "scene/sceneregister/sceneregister.hpp"

bool ConfigScene::Load(const std::string &dir,const std::string &filename)
{
	std::string configname = dir + filename;
	PugiXmlDocument m_document;
	char errinfo[1024];
	PUGI_XML_LOAD_AND_CHECK(m_document, configname, "ConfigScene", lasterr);

	PugiXmlNode RootElement = m_document.document_element();
	PugiXmlNode currentElement;

	if ( !RootElement )
	{
		lasterr = configname + ": xml node error in root";
		return false;
	}

	//开始加载
	if ( !PugiGetSubNodeValue(RootElement, "id", id) )
	{
		lasterr = configname + ": xml node error in node [id] ";
		return false;
	}

	if ( !PugiGetSubNodeValue(RootElement, "name", name ) )
	{
		lasterr = configname + ": xml node error in node [name] ";
		return false;
	}

	if ( !PugiGetSubNodeValue(RootElement, "mapid", mapid) )
	{
		lasterr = configname + ": xml node error in node [mapid] ";
		return false;
	}

	if ( PugiGetSubNodeValue(RootElement, "levellimit", levellimit) )
	{
		SceneRegister::Instance().AddSceneLevelLimit(id, levellimit);
	}

	if ( !PugiGetSubNodeValue(RootElement, "istax", istax) )
	{
		lasterr = configname + ": xml node error in node [istax] ";
		return false;
	}

	if ( !PugiGetSubNodeValue(RootElement, "speciallogic_type", speciallogic_type) || speciallogic_type < 0)
	{
		lasterr = configname + ": xml node error in node [speciallogic_type] ";
		return false;
	}

	if ( !PugiGetSubNodeValue(RootElement, "scene_timeout", scene_timeout) )
	{
		lasterr = configname + ": xml node error in node [scene_timeout] ";
		return false;
	}

	if ( !PugiGetSubNodeValue(RootElement, "is_timeout_kick", is_timeout_kick_user) )
	{
		lasterr = configname + ": xml node error in node [is_timeout_kick] ";
		return false;
	}

	if ( !PugiGetSubNodeValue(RootElement, "is_forbid_pk", is_forbid_pk) )
	{
	//	lasterr = configname + ": xml node error in node [is_forbid_pk] ";
	//	return false;
	}

	//处理npcs，可为空
	currentElement = RootElement.child("npcs");
	if ( currentElement )
	{
		PugiXmlNode subElement = currentElement.first_child();
		while( !subElement.empty() )
		{
			ConfigSceneNpc npc;

			if ( !PugiGetSubNodeValue(subElement, "x", npc.x ) || 0 > npc.x)
			{
				lasterr = configname + ": xml node error in node [npcs->npc->x] ";
				return false;
			}

			if ( !PugiGetSubNodeValue(subElement, "y", npc.y ) || 0 > npc.y)
			{
				lasterr = configname + ": xml node error in node [npcs->npc->y] ";
				return false;
			}

			if ( !PugiGetSubNodeValue(subElement, "id", npc.id ) || 0 == npc.id)
			{
				lasterr = configname + ": xml node error in node [npcs->npc->id] ";
				return false;
			}

			// 检查npc是否重复
			ConfigSceneNpcList::iterator tmpnpc = npcs.begin();
			for (; tmpnpc!=npcs.end(); ++tmpnpc)
			{
				if (tmpnpc->id == npc.id) break;
			}
			if (tmpnpc != npcs.end())
			{
				sprintf(errinfo,"%s:\nxml node error in node [npcs->npc->id]: the npc id[%d] is repeat!", configname.c_str(), npc.id);
				lasterr = errinfo;
				return false;
			}

			// 加载npc
			/*std::string npccofnigname;
			BaseTypeToString(npc.id,npccofnigname);
			std::string npcconfig = dir+"npc/"+npccofnigname+".xml";
			if (!npc.Load(npcconfig,lasterr))
			{
				lasterr = configname + ":" + lasterr;
				return false;
			}*/

			npcs.push_back(npc);
			subElement = subElement.next_sibling();
		}
	}

	//处理doors，可为空
	currentElement = RootElement.child("doors");
	if ( currentElement )
	{
		PugiXmlNode subElement = currentElement.first_child();
		while( !subElement.empty())
		{
			ConfigSceneDoor door;

			if (!PugiGetSubNodeValue(subElement, "id", door.id ))
			{
				lasterr = configname + ": xml node error in node [doors->door->id] ";
				return false;
			}

			if (!PugiGetSubNodeValue(subElement, "target_scene_id", door.target_scene_id))
			{
				// todo:先不检查目标场景
// 				lasterr = configname + ": xml node error in node [doors->door->target_scene_id] ";
// 				return false;
			}

			if (!PugiGetSubNodeValue(subElement, "target_door_id", door.target_door_id))
			{
				// todo:先不检查目标点
// 				lasterr = configname + ": xml node error in node [doors->door->target_door_id] ";
// 				return false;
			}

			if (!PugiGetSubNodeValue(subElement, "x", door.pos.x ) || door.pos.x <= 0)
			{
				lasterr = configname + ": xml node error in node [doors->door->x] ";
				return false;
			}

			if (!PugiGetSubNodeValue(subElement, "y", door.pos.y ) || door.pos.y <= 0)
			{
				lasterr = configname + ": xml node error in node [doors->door->y] ";
				return false;
			}

			// 检查传送门是否重复
			ConfigSceneDoorList::iterator tmpiter = doors.begin();
			for (; tmpiter!=doors.end(); ++tmpiter)
			{
				if (tmpiter->id == door.id) break;
			}

			if (tmpiter != doors.end())
			{
				sprintf(errinfo,"%s:\nxml node error in node [doors->door->id]: the door id[%d] is repeat!", configname.c_str(), door.id);
				lasterr = errinfo;
				return false;
			}

			doors.push_back(door);
			SceneRegister::Instance().AddDoor(id, door);

			subElement = subElement.next_sibling();
		}
	}

	// 处理triggerareas
	currentElement = RootElement.child("triggerareas");
	if ( currentElement )
	{
		PugiXmlNode subElement = currentElement.first_child();
		while( !subElement.empty())
		{
			ConfigSceneTriggerArea triggerarea;

			if ( !PugiGetSubNodeValue(subElement, "x", triggerarea.x ) )
			{
				lasterr = configname + ": xml node error in node [triggerareas->triggerarea->x] ";
				return false;
			}

			if ( !PugiGetSubNodeValue(subElement, "y", triggerarea.y ) )
			{
				lasterr = configname + ": xml node error in node [triggerareas->triggerarea->y] ";
				return false;
			}

			if ( !PugiGetSubNodeValue(subElement, "range", triggerarea.range ) )
			{
				lasterr = configname + ": xml node error in node [triggerareas->triggerarea->range] ";
				return false;
			}

			triggerareas.push_back(triggerarea);
			subElement = subElement.next_sibling();
		}
	}


	//处理relivepoint
	currentElement = RootElement.child("townpoint");
	if ( !currentElement )
	{
		lasterr = configname + ": xml node error in node [townpoint] ";
		return false;
	}

	if ( !PugiGetSubNodeValue(currentElement, "sceneid", townpoint.sceneid ) || townpoint.sceneid <= 0 )
	{
		lasterr = configname + ": xml node error in node [townpoint->sceneid] ";
		return false;
	}
	CheckResourceCenter::Instance().GetSceneCheck()->Add(townpoint.sceneid);

	if ( !PugiGetSubNodeValue(currentElement, "scenex", townpoint.scenex ) )
	{
		lasterr = configname + ": xml node error in node [townpoint->scenex] ";
		return false;
	}
	if ( !PugiGetSubNodeValue(currentElement, "sceney", townpoint.sceney ) )
	{
		lasterr = configname + ": xml node error in node [townpoint->sceney] ";
		return false;
	}

	//处理gathers，可为空
	currentElement = RootElement.child("gatherpoints");
	if (currentElement)
	{
		PugiXmlNode subElement = currentElement.child("gather");
		while (!subElement.empty())
		{
			ConfigSceneGather gather;

			if ( !PugiGetSubNodeValue(subElement, "id", gather.id ) )
			{
				lasterr = configname + ": xml node error in node [gatherpoints->gather->id] ";
				return false;
			}
			if ( !PugiGetSubNodeValue(subElement, "x", gather.x ) )
			{
				lasterr = configname + ": xml node error in node [gatherpoints->gather->x] ";
				return false;
			}
			if ( !PugiGetSubNodeValue(subElement, "y", gather.y ) )
			{
				lasterr = configname + ": xml node error in node [gatherpoints->gather->y] ";
				return false;
			}
			if ( !PugiGetSubNodeValue(subElement, "create_interval", gather.create_interval ) || 0 == gather.create_interval)
			{
				lasterr = configname + ": xml node error in node [gatherpoints->gather->create_interval] ";
				return false;
			}
			if ( !PugiGetSubNodeValue(subElement, "gather_time", gather.gather_time ) || 0 == gather.gather_time)
			{
				lasterr = configname + ": xml node error in node [gatherpoints->gather->gather_time] ";
				return false;
			}
			if ( !PugiGetSubNodeValue(subElement, "evil_add", gather.evil_add ) || 0 > gather.evil_add)
			{
				lasterr = configname + ": xml node error in node [gatherpoints->gather->evil_add] ";
				return false;
			}
			if ( !PugiGetSubNodeValue(subElement, "disappear_after_gather", gather.disappear_after_gather ))
			{
				lasterr = configname + ": xml node error in node [gatherpoints->gather->disappear_after_gather] ";
				return false;
			}

			gathers.push_back(gather);
			subElement = subElement.next_sibling();
		}
	}

	//处理monsterpoints，可为空，必须在加载了monsterdropitem和monsters之后
	currentElement = RootElement.child("monsterpoints");
	if ( currentElement )
	{
		PugiXmlNode subElement = currentElement.first_child();
		while( !subElement.empty() )
		{
			ConfigSceneMonsterPoint point;

			if ( !PugiGetSubNodeValue(subElement, "x", point.x ) )
			{
				lasterr = configname + ": xml node error in node [monsterpoints->point->x] ";
				return false;
			}

			if ( !PugiGetSubNodeValue(subElement, "y", point.y ) )
			{
				lasterr = configname + ": xml node error in node [monsterpoints->point->y] ";
				return false;
			}

			if ( !PugiGetSubNodeValue(subElement, "interval", point.interval ) )
			{
				lasterr = configname + ": xml node error in node [monsterpoints->point->interval] ";
				return false;
			}

			if ( !PugiGetSubNodeValue(subElement, "num", point.num ) )
			{
				lasterr = configname + ": xml node error in node [monsterpoints->point->num] ";
				return false;
			}

			if ( !PugiGetSubNodeValue(subElement, "histroytotalnum", point.histroytotalnum ) )
			{
				lasterr = configname + ": xml node error in node [monsterpoints->point->histroytotalnum] ";
				return false;
			}

			if ( !PugiGetSubNodeValue(subElement, "monsterid", point.monsterid ) )
			{
				lasterr = configname + ": xml node error in node [monsterpoints->point->monsterid] ";
				return false;
			}
			// 怪物是否存在
			if (!MONSTERPOOL->IsMonsterExist(point.monsterid))
			{
				sprintf(errinfo,"%s:\nxml node error in node [monsterpoints->point->monsterid]: the monster id[%d] is no exist!", configname.c_str(), point.monsterid);
				lasterr = errinfo;
				return false;
			}

			if (Scene::IsBossDongKuScene(id) || Scene::IsDabaoBossScene(id) || Scene::IsCampNormalDuoBaoScene(id) || Scene::IsActiveBossScene(id)) // 只限制Boss洞窟地图、打宝地图、活跃boss
			{
				const MonsterInitParam * monster_param = MONSTERPOOL->GetMonsterParam(point.monsterid);
				if (NULL != monster_param && MonsterInitParam::MONSTER_TYPE_BOSS == monster_param->monster_type)
				{
					if ((point.interval / 1000) < 60)  // 如果是直接在地图上刷boss，防止策划配错，导致boss被无限的刷 
					{
						sprintf(errinfo,"%s:\nxml node error in node [monsterpoints->point->monsterid]: the monster id[%d] is boss but interval is invalid!", 
							configname.c_str(), point.monsterid);
						lasterr = errinfo;
						return false;
					}
				}
			}

			monsterpoints.push_back(point);
			subElement = subElement.next_sibling();
		}
	}

	//处理triggers
	currentElement = RootElement.child("triggers");
	if (NULL != currentElement)
	{
		PugiXmlNode subElement = currentElement.first_child();
		while (!subElement.empty())
		{
			ConfigSceneTrigger trigger;

			if (!PugiGetSubNodeValue(subElement, "id", trigger.id))
			{
				lasterr = configname + ": xml node error in node [triggers->trigger->id] ";
				return false;
			}

			if (!PugiGetSubNodeValue(subElement, "x", trigger.x))
			{
				lasterr = configname + ": xml node error in node [triggers->trigger->x] ";
				return false;
			}

			if (!PugiGetSubNodeValue(subElement, "y", trigger.y))
			{
				lasterr = configname + ": xml node error in node [triggers->trigger->y] ";
				return false;
			}

			if (!PugiGetSubNodeValue(subElement, "range", trigger.range))
			{
				lasterr = configname + ": xml node error in node [triggers->trigger->range] ";
				return false;
			}

			if (!PugiGetSubNodeValue(subElement, "trigger_cond_type", trigger.trigger_cond_type))
			{
				lasterr = configname + ": xml node error in node [triggers->trigger->trigger_cond_type] ";
				return false;
			}

			{
				PugiXmlNode ParamListElement = subElement.child("trigger_cond_param_list");
				if (ParamListElement.empty())
				{
					lasterr = configname + ": xml node error in node [triggers->trigger->trigger_cond_param_list] ";
					return false;
				}

				int count = 0;
				PugiXmlNode ParamElement = ParamListElement.child("param");
				while (!ParamElement.empty())
				{
					if (count >= TRIGGER_COND_PARAM_MAX)
					{
						lasterr = configname + ": xml node error in node [triggers->trigger->trigger_cond_param_list->param], too many";
						return false;
					}

					if (!PugiGetNodeValue(ParamElement, trigger.trigger_cond_param_list[count]))
					{
						lasterr = configname + ": xml node error in node [triggers->trigger->trigger_cond_param_list->param] ";
						return false;
					}

					++count;
					ParamElement = ParamElement.next_sibling();
				}
			}
			
			if (!PugiGetSubNodeValue(subElement, "trigger_action_type", trigger.trigger_action_type))
			{
				lasterr = configname + ": xml node error in node [triggers->trigger->trigger_action_type] ";
				return false;
			}

			{
				PugiXmlNode ParamListElement = subElement.child("trigger_action_param_list");
				if (ParamListElement.empty())
				{
					lasterr = configname + ": xml node error in node [triggers->trigger->trigger_action_param_list] ";
					return false;
				}

				int count = 0;
				PugiXmlNode ParamElement = ParamListElement.child("param");
				while (!ParamElement.empty())
				{
					if (count >= TRIGGER_ACTION_PARAM_MAX)
					{
						lasterr = configname + ": xml node error in node [triggers->trigger->trigger_action_param_list->param], too many";
						return false;
					}

					if (!PugiGetNodeValue(ParamElement, trigger.trigger_action_param_list[count]))
					{
						lasterr = configname + ": xml node error in node [triggers->trigger->trigger_action_param_list->param] ";
						return false;
					}
					
					++count;
					ParamElement = ParamElement.next_sibling();
				}
			}

			if (!PugiGetSubNodeValue(subElement, "trigger_max_count", trigger.trigger_max_count))
			{
				lasterr = configname + ": xml node error in node [triggers->trigger->trigger_max_count] ";
				return false;
			}
			
			triggers.push_back(trigger);
			subElement = subElement.next_sibling();
		}
	}

	return true;
}




