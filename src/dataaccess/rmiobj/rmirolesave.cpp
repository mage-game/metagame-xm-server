#include "rmirole.h"

#include "databaselayer.h"
#include "servercommon/servercommon.h"
#include "servercommon/struct/roleinitparam.h"
#include "servercommon/struct/roleotherinitparam.hpp"
#include "servercommon/struct/skilllistparam.h"
#include "servercommon/struct/itemlistparam.h"

#include "servercommon/struct/hotkeylistparam.h"
#include "servercommon/struct/friendparam.h"
#include "servercommon/struct/enemyparam.h"
#include "servercommon/struct/mountparam.hpp"
#include "servercommon/struct/dailyfindlistparam.hpp"
#include "servercommon/guilddef.hpp"
#include "servercommon/jinglingdef.hpp"
#include "servercommon/crossgoaldef.hpp"

#include "servercommon/darmi/darole.h"
#include "common/bintohex.h"
#include "engineadapter.h"

#include "servercommon/roleattrvermanager/roleattrvermanager.h"
#include "servercommon/packetstream.hpp"
#include "dataaccesslog.hpp"

#include "rmidef.hpp"
#include "crossrolecache/crossrolecachemanager.hpp"
#include "servercommon/mythdef.hpp"

USE_DATAACCESS_VAR();
USE_DATAACCESS_ROLE_VAR();
OLD_USE_INTERNAL_NETWORK();

extern char g_struct_code_buffer[MAX_STRUCT_CODE_BUFFER_LEN];

int RMIRoleObject::SaveTableRole(IStatement *stmt, int role_id, bool is_logout, const RoleInitParam &role_p, const RoleOtherInitParam &other_p, int *error)
{
	int ret = 0;
	{
		DBCommand *rolecommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE);

		DataNodeList role_update;
		role_update.reserve(tabledef::TABLE_ROLE_MAXINDEX - 1);

		DataNode name_node(tabledef::ROLE_ROLE_NAME);
		name_node.SetString(role_p.role_name);
		role_update.push_back(name_node);

		DataNode avatar_node(tabledef::ROLE_AVATAR);
		avatar_node.SetByte(role_p.avatar);
		role_update.push_back(avatar_node);

		DataNode sex_node(tabledef::ROLE_SEX);
		sex_node.SetByte(role_p.sex);
		role_update.push_back(sex_node);

		DataNode country(tabledef::ROLE_COUNTRY);
		country.SetByte(role_p.camp);
		role_update.push_back(country);

		DataNode sid_node(tabledef::ROLE_SCENE_ID);
		sid_node.SetInt32(role_p.scene_id);
		role_update.push_back(sid_node);

		DataNode last_sid_node(tabledef::ROLE_LAST_SCENE_ID);
		last_sid_node.SetInt32(role_p.last_scene_id);
		role_update.push_back(last_sid_node);

		DataNode level_node(tabledef::ROLE_LEVEL);
		level_node.SetInt32(role_p.level);
		role_update.push_back(level_node);

		DataNode pro_node(tabledef::ROLE_PROFESSIONAL);
		pro_node.SetInt32(role_p.prof);
		role_update.push_back(pro_node);

		DataNode online_time(tabledef::ROLE_ONLINE_TIME);
		online_time.SetInt32(role_p.online_time);
		role_update.push_back(online_time);

		DataNode is_online_node(tabledef::ROLE_IS_ONLINE);
		is_online_node.SetByte(is_logout ? 0 : 1);
		role_update.push_back(is_online_node);

		// 不需要存储 ROLE_CREATE_TIME

		DataNode last_save_time(tabledef::ROLE_LAST_SAVE_TIME);
		last_save_time.SetLong64(EngineAdapter::Instance().Time());
		role_update.push_back(last_save_time);

		DataNode is_chongzhi(tabledef::ROLE_IS_CHONGZHI);
		is_chongzhi.SetByte(other_p.welfare_param.chongzhi_count > 0 ? 1 : 0);
		role_update.push_back(is_chongzhi);

		DataNode is_micro_pc(tabledef::ROLE_IS_MICRO_PC);
		is_micro_pc.SetByte(0);
		role_update.push_back(is_micro_pc);

		DataNode plat_spid(tabledef::ROLE_PLAT_SPID);
		plat_spid.SetInt32(other_p.common_data_param.plat_spid);
		role_update.push_back(plat_spid);

		CmpNodeList condition;
		CmpNode id_node(tabledef::ROLE_ROLE_ID);
		id_node.SetInt32(role_id);
		condition.push_back(id_node);

		int count = rolecommand->Update(stmt, condition, role_update);
		if (count < 1)
		{
			dataaccesslog::g_log_role.printf(LL_CRITICAL, "SaveTableRole rolecommand->Update uid:%d", role_id);
			*error |= dataaccessrmi::role::ROLE_ERROR;
			ret = -1;
		}
	}

	{
		DBCommand *roleattrcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL);

		DataNodeList attr_list;
		attr_list.reserve(tabledef::TABLE_ROLE_ATTR_DETAIL_MAXINDEX - 2);
		
		static char buff[RMIRoleAttrVerManager::ROLE_ATTR_MAX_BUFFER_LENGTH];
		TLVSerializer attr_data;
		attr_data.Reset(buff, RMIRoleAttrVerManager::ROLE_ATTR_MAX_BUFFER_LENGTH);

		if (!role_p.Serialize(attr_data, true))
		{
			dataaccesslog::g_log_role.printf(LL_CRITICAL, "SaveTableRole role_p.Serialize uid:%d", role_id);
			*error |= dataaccessrmi::role::ROLE_ATTR_DETAIL_ERROR;
			ret = -2;
		}
		else 
		{
			static char hex_buff[RMIRoleAttrVerManager::ROLE_ATTR_MAX_BUFFER_LENGTH * 2 + 1] = {0};
			BinToHex(buff, attr_data.Size(), hex_buff);
			hex_buff[attr_data.Size() * 2] = 0;

			DataNode attr_data_node(tabledef::ROLE_ATTR_DETAIL_ATTR_DATA);
			attr_data_node.SetString(hex_buff);
			attr_list.push_back(attr_data_node);

			DataNode name_node(tabledef::ROLE_ATTR_DETAIL_ROLE_NAME);
			name_node.SetString(role_p.role_name);
			attr_list.push_back(name_node);

			DataNode level_node(tabledef::ROLE_ATTR_DETAIL_LEVEL);
			level_node.SetInt32(role_p.level);
			attr_list.push_back(level_node);

			DataNode sex_node(tabledef::ROLE_ATTR_DETAIL_SEX);
			sex_node.SetByte(role_p.sex);
			attr_list.push_back(sex_node);

			DataNode prof_node(tabledef::ROLE_ATTR_DETAIL_PROF);
			prof_node.SetByte(role_p.prof);
			attr_list.push_back(prof_node);

			DataNode camp_node(tabledef::ROLE_ATTR_DETAIL_CAMP);
			camp_node.SetByte(role_p.camp);
			attr_list.push_back(camp_node);

			DataNode avatar_node(tabledef::ROLE_ATTR_DETAIL_AVATAR);
			avatar_node.SetByte(role_p.avatar);
			attr_list.push_back(avatar_node);

			DataNode avator_timestamp_node(tabledef::ROLE_ATTR_DETAIL_AVATAR_TIMESTAMP);
			avator_timestamp_node.SetLong64(role_p.avatar_timestamp);
			attr_list.push_back(avator_timestamp_node);

			DataNode vip_type_node(tabledef::ROLE_ATTR_DETAIL_VIP_TYPE);
			vip_type_node.SetByte(other_p.vip_param.vip_level);
			attr_list.push_back(vip_type_node);

			DataNode evil_node(tabledef::ROLE_ATTR_DETAIL_EVIL);
			evil_node.SetInt32(role_p.evil);
			attr_list.push_back(evil_node);

			DataNode gold_node(tabledef::ROLE_ATTR_DETAIL_GOLD);
			gold_node.SetLong64(role_p.gold); 
			attr_list.push_back(gold_node);

			DataNode gold_bind_node(tabledef::ROLE_ATTR_DETAIL_GOLD_BIND);
			gold_bind_node.SetLong64(role_p.gold_bind); 
			attr_list.push_back(gold_bind_node);

			DataNode coin_node(tabledef::ROLE_ATTR_DETAIL_COIN);
			coin_node.SetLong64(role_p.coin); 
			attr_list.push_back(coin_node);

			DataNode coin_bind_node(tabledef::ROLE_ATTR_DETAIL_COIN_BIND);
			coin_bind_node.SetLong64(role_p.coin_bind); 
			attr_list.push_back(coin_bind_node);

			DataNode hp_node(tabledef::ROLE_ATTR_DETAIL_HP);
			hp_node.SetLong64(role_p.hp);
			attr_list.push_back(hp_node);

			DataNode max_hp_node(tabledef::ROLE_ATTR_DETAIL_MAX_HP);
			max_hp_node.SetLong64(role_p.max_hp);
			attr_list.push_back(max_hp_node);

			DataNode mp_node(tabledef::ROLE_ATTR_DETAIL_MP);
			mp_node.SetLong64(role_p.mp);
			attr_list.push_back(mp_node);

			DataNode max_mp_node(tabledef::ROLE_ATTR_DETAIL_MAX_MP);
			max_mp_node.SetLong64(role_p.max_mp);
			attr_list.push_back(max_mp_node);

			DataNode gongji_node(tabledef::ROLE_ATTR_DETAIL_GONGJI);
			gongji_node.SetLong64(role_p.gongji);
			attr_list.push_back(gongji_node);

			DataNode fangyu_node(tabledef::ROLE_ATTR_DETAIL_FANGYU);
			fangyu_node.SetLong64(role_p.fangyu);
			attr_list.push_back(fangyu_node);

			DataNode mingzhong_node(tabledef::ROLE_ATTR_DETAIL_MINGZHONG);
			mingzhong_node.SetLong64(role_p.mingzhong);
			attr_list.push_back(mingzhong_node);

			DataNode shanbi_node(tabledef::ROLE_ATTR_DETAIL_SHANBI);
			shanbi_node.SetLong64(role_p.shanbi);
			attr_list.push_back(shanbi_node);

			DataNode baoji_node(tabledef::ROLE_ATTR_DETAIL_BAOJI);
			baoji_node.SetLong64(role_p.baoji);
			attr_list.push_back(baoji_node);

			DataNode jianren_node(tabledef::ROLE_ATTR_DETAIL_JIANREN);
			jianren_node.SetLong64(role_p.jianren);
			attr_list.push_back(jianren_node);
			
			// other attr

			DataNode capability_node(tabledef::ROLE_ATTR_DETAIL_CAPABILITY);
			capability_node.SetInt32(role_p.capability);
			attr_list.push_back(capability_node);

			DataNode max_capability_node(tabledef::ROLE_ATTR_DETAIL_MAX_CAPABILITY);
			max_capability_node.SetInt32(role_p.max_capability);
			attr_list.push_back(max_capability_node);

			DataNode authority_type_node(tabledef::ROLE_ATTR_DETAIL_AUTHORITY_TYPE);
			authority_type_node.SetByte(role_p.authority_type);
			attr_list.push_back(authority_type_node);

			DataNode forbid_talk_time_node(tabledef::ROLE_ATTR_DETAIL_FORBID_TALK_TIME);
			forbid_talk_time_node.SetLong64(role_p.forbid_talk_time);
			attr_list.push_back(forbid_talk_time_node);

			DataNode sublock_pw(tabledef::ROLE_ATTR_DETAIL_SUBLOCK_PW);
			sublock_pw.SetString(role_p.sublock_pw);
			attr_list.push_back(sublock_pw);

			DataNode guild_id_node(tabledef::ROLE_ATTR_DETAIL_GUILD_ID);
			guild_id_node.SetInt32(role_p.guild_id);
			attr_list.push_back(guild_id_node);

			DataNode today_online_time_node(tabledef::ROLE_ATTR_DETAIL_TODAY_ONLINE_TIME);
			today_online_time_node.SetInt32(role_p.today_online_time);
			attr_list.push_back(today_online_time_node);

			DataNode lastday_online_time_node(tabledef::ROLE_ATTR_DETAIL_LASTDAY_ONLINE_TIME);
			lastday_online_time_node.SetInt32(role_p.lastday_online_time);
			attr_list.push_back(lastday_online_time_node);


			DataNode rolechest_data_len_node(tabledef::ROLE_ATTR_DETAIL_ROLECHEST_DATA_LEN);
			rolechest_data_len_node.SetInt32(other_p.chest_buff_len);
			attr_list.push_back(rolechest_data_len_node);

			{
				static MentalityParamHex mentality_hex;
				BinToHex((char*)&other_p.mentality_param, sizeof(other_p.mentality_param), mentality_hex);
				mentality_hex[sizeof(mentality_hex) - 1] = '\0';

				DataNode mentality_node(tabledef::ROLE_ATTR_DETAIL_MENTALITY_DATA);
				mentality_node.SetString(mentality_hex);
				attr_list.push_back(mentality_node);
			}
		
			{
				static VipParamHex vip_hex;
				BinToHex((char*)&other_p.vip_param, sizeof(other_p.vip_param), vip_hex);
				vip_hex[sizeof(vip_hex) - 1] = '\0';

				DataNode vip_node(tabledef::ROLE_ATTR_DETAIL_VIP_DATA);
				vip_node.SetString(vip_hex);
				attr_list.push_back(vip_node);
			}

			{
				static RoleShopParamHex roleshop_hex;
				BinToHex((char*)&other_p.roleshop_param, sizeof(other_p.roleshop_param), roleshop_hex);
				roleshop_hex[sizeof(roleshop_hex) - 1] = '\0';

				DataNode roleshop_node(tabledef::ROLE_ATTR_DETAIL_ROLESHOP_DATA);
				roleshop_node.SetString(roleshop_hex);
				attr_list.push_back(roleshop_node);
			}
			
			{
				static ActiveDegreeHex active_degree_hex;
				BinToHex((char*)&other_p.active_degree_param, sizeof(other_p.active_degree_param), active_degree_hex);
				active_degree_hex[sizeof(active_degree_hex) - 1] = '\0';

				DataNode active_degree_node(tabledef::ROLE_ATTR_DETAIL_ACTIVE_DEGREE_DATA);
				active_degree_node.SetString(active_degree_hex);
				attr_list.push_back(active_degree_node);
			}

			{
				static WelfareParamHex welfare_hex;
				BinToHex((char*)&other_p.welfare_param, sizeof(other_p.welfare_param), welfare_hex);
				welfare_hex[sizeof(welfare_hex) - 1] = '\0';

				DataNode welfare_node(tabledef::ROLE_ATTR_DETAIL_WELFARE_DATA);
				welfare_node.SetString(welfare_hex);
				attr_list.push_back(welfare_node);
			}

			{
				static RoleHangupSetDataHex role_hangupset_data_hex;
				BinToHex((char*)&other_p.rolehangup_param.hangup_set, sizeof(other_p.rolehangup_param.hangup_set), role_hangupset_data_hex);
				role_hangupset_data_hex[sizeof(role_hangupset_data_hex) - 1] = '\0';

				DataNode rolehangup_node(tabledef::ROLE_ATTR_DETAIL_ROLEHANGUP_DATA);
				rolehangup_node.SetString(role_hangupset_data_hex);
				attr_list.push_back(rolehangup_node);
			}

			{
				static ItemColddownParamHex itemcolddown_hex;
				BinToHex((char*)&other_p.itemcolddown_param, sizeof(other_p.itemcolddown_param), itemcolddown_hex);
				itemcolddown_hex[sizeof(itemcolddown_hex) - 1] = '\0';

				DataNode itemcolddown_node(tabledef::ROLE_ATTR_DETAIL_ITEMCOLDDOWN_DATA);
				itemcolddown_node.SetString(itemcolddown_hex);
				attr_list.push_back(itemcolddown_node);
			}

			{
				static EvaluateParamHex evaluate_hex;
				BinToHex((char*)&other_p.evaluate_param, sizeof(other_p.evaluate_param), evaluate_hex);
				evaluate_hex[sizeof(evaluate_hex) - 1] = '\0';

				DataNode evaluate_node(tabledef::ROLE_ATTR_DETAIL_EVALUATE_DATA);
				evaluate_node.SetString(evaluate_hex);
				attr_list.push_back(evaluate_node);
			}

			{
				static RoleCustomSetDataHex role_customset_data_hex;
				BinToHex((char*)&other_p.rolehangup_param.custom_set, sizeof(other_p.rolehangup_param.custom_set), role_customset_data_hex);
				role_customset_data_hex[sizeof(role_customset_data_hex) - 1] = '\0';

				DataNode rolecustom_node(tabledef::ROLE_ATTR_DETAIL_ROLECUSTOM_DATA);
				rolecustom_node.SetString(role_customset_data_hex);
				attr_list.push_back(rolecustom_node);
			}

			{
				static MonitorParamHex monitor_hex;
				BinToHex((char*)&other_p.monitor_param, sizeof(other_p.monitor_param), monitor_hex);
				monitor_hex[sizeof(monitor_hex) - 1] = '\0';

				DataNode monitor_node(tabledef::ROLE_ATTR_DETAIL_MONITOR_DATA);
				monitor_node.SetString(monitor_hex);
				attr_list.push_back(monitor_node);
			}

			UPDATE_BINARY_DATA_TO_LIST(title_node, tabledef::ROLE_ATTR_DETAIL_TITLE_DATA, other_p.title_param, TitleParamHex, attr_list); 

			UPDATE_BINARY_DATA_TO_LIST(husong_node, tabledef::ROLE_ATTR_DETAIL_HUSONGTASK_DATA, other_p.husong_param, HusongTaskParamHex, attr_list); 

			UPDATE_BINARY_DATA_TO_LIST(common_node, tabledef::ROLE_ATTR_DETAIL_COMMON_DATA, other_p.common_data_param, CommonDataParamHex, attr_list); 

			UPDATE_BINARY_DATA_TO_LIST(wing_node, tabledef::ROLE_ATTR_DETAIL_WING_DATA, other_p.wing_param, WingParamHex, attr_list);

			UPDATE_BINARY_DATA_TO_LIST(roleactivity_node, tabledef::ROLE_ATTR_DETAIL_ROLEACTIVITY_DATA, other_p.roleactivity_param, RoleActivityParamHex, attr_list);

			UPDATE_BINARY_DATA_TO_LIST(fbtowerdefend_node, tabledef::ROLE_ATTR_DETAIL_FBTOWERDEFEND_DATA, other_p.towerdefend_param, TowerDefendParamHex, attr_list)

			{
				DataNode last_single_chat_monitor_time_node(tabledef::ROLE_ATTR_DETAIL_LAST_SINGLE_CHAT_MONITOR_TIME);
				last_single_chat_monitor_time_node.SetLong64(other_p.last_singlechat_monitor_time);
				attr_list.push_back(last_single_chat_monitor_time_node);
				
				DataNode last_single_chat_monitor_msg_node(tabledef::ROLE_ATTR_DETAIL_LAST_SINGLE_CHAT_MONITOR_MSG);
				last_single_chat_monitor_msg_node.SetString(other_p.last_singlechat_monitor_msg);
				attr_list.push_back(last_single_chat_monitor_msg_node);
			}

			{
				DataNode last_world_chat_monitor_time_node(tabledef::ROLE_ATTR_DETAIL_LAST_WORLD_CHAT_MONITOR_TIME);
				last_world_chat_monitor_time_node.SetLong64(other_p.last_worldchat_monitor_time);
				attr_list.push_back(last_world_chat_monitor_time_node);

				DataNode last_world_chat_monitor_msg_node(tabledef::ROLE_ATTR_DETAIL_LAST_WORLD_CHAT_MONITOR_MSG);
				last_world_chat_monitor_msg_node.SetString(other_p.last_worldchat_monitor_msg);
				attr_list.push_back(last_world_chat_monitor_msg_node);
			}

			{
				DataNode last_mail_monitor_time_node(tabledef::ROLE_ATTR_DETAIL_LAST_MAIL_MONITOR_TIME);
				last_mail_monitor_time_node.SetLong64(other_p.last_mail_monitor_time);
				attr_list.push_back(last_mail_monitor_time_node);

				DataNode last_mail_monitor_subject_node(tabledef::ROLE_ATTR_DETAIL_LAST_MAIL_MONITOR_SUBJECT);
				last_mail_monitor_subject_node.SetString(other_p.last_mail_monitor_subject);
				attr_list.push_back(last_mail_monitor_subject_node);
			}

			CmpNodeList condition;
			CmpNode id_node(tabledef::ROLE_ATTR_DETAIL_ROLE_ID);
			id_node.SetInt32(role_id);
			condition.push_back(id_node);
			
			int count = roleattrcommand->Update(stmt, condition, attr_list);
			if (count < 1)
			{
				dataaccesslog::g_log_role.printf(LL_CRITICAL, "SaveTableRole roleattrcommand->Update uid:%d", role_id);
				*error |= dataaccessrmi::role::ROLE_ATTR_DETAIL_ERROR;
				ret = -3;
			}
		}
	}

	{
		DBCommand *roleattrcommand2 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL2);

		DataNodeList attr_list;
		attr_list.reserve(tabledef::TABLE_ROLE_ATTR_DETAIL2_MAXINDEX - 2);

		{
			static char chest_hex_buff[RoleOtherInitParam::ROLE_CHEST_SHOP_BUFF_LENGTH * 2 + 1] = {0};
			BinToHex(other_p.chest_buff, other_p.chest_buff_len, chest_hex_buff);
			chest_hex_buff[other_p.chest_buff_len * 2] = 0;

			DataNode chest_data_node(tabledef::ROLE_ATTR_DETAIL2_ROLECHEST_DATA);
			chest_data_node.SetString(chest_hex_buff);
			attr_list.push_back(chest_data_node);
		}

		{
			static char effect_hex_buff[RoleOtherInitParam::MAX_EFFECT_NUM * RoleOtherInitParam::EFFECT_BUFFER_LENGTH * 2 + 1] = {0};
			BinToHex(other_p.effect_buff, other_p.effect_buff_len, effect_hex_buff);
			effect_hex_buff[other_p.effect_buff_len * 2] = 0;

			DataNode effect_data_node(tabledef::ROLE_ATTR_DETAIL2_EFFECT_DATA);
			effect_data_node.SetString(effect_hex_buff);
			attr_list.push_back(effect_data_node);
		}

		{
			static EquipFBParamHex equipfb_hex;
			BinToHex((char*)&other_p.equipfb_param, sizeof(other_p.equipfb_param), equipfb_hex);
			equipfb_hex[sizeof(equipfb_hex) - 1] = '\0';

			DataNode equipfb_node(tabledef::ROLE_ATTR_DETAIL2_EQUIPFB_DATA);
			equipfb_node.SetString(equipfb_hex);
			attr_list.push_back(equipfb_node);
		}

		UPDATE_BINARY_DATA_TO_LIST(guild_task_node, tabledef::ROLE_ATTR_DETAIL2_GUILD_TASK_DATA, other_p.guild_task_param, GuildTaskParamHex, attr_list);

		UPDATE_BINARY_DATA_TO_LIST(maze_node, tabledef::ROLE_ATTR_DETAIL2_MAZE_DATA, other_p.maze_param, MazeParamHex, attr_list);

		UPDATE_BINARY_DATA_TO_LIST(expfb_node, tabledef::ROLE_ATTR_DETAIL2_STORY_FB_DATA, other_p.storyfb_param, StoryFBParamHex, attr_list);

		UPDATE_BINARY_DATA_TO_LIST(convert_record_node, tabledef::ROLE_ATTR_DETAIL2_CONVERT_RECORD_DATA, other_p.convert_record_param, ConvertRecordParamHex, attr_list);

		UPDATE_BINARY_DATA_TO_LIST(neq_node, tabledef::ROLE_ATTR_DETAIL2_NEWEQUIPFB_DATA, other_p.newequipfb_param, NewEquipFBParamHex, attr_list);

		UPDATE_BINARY_DATA_TO_LIST(xiannv_node, tabledef::ROLE_ATTR_DETAIL2_XIANNV_DATA, other_p.xiannv_param, XiannvParamHex, attr_list);

		UPDATE_BINARY_DATA_TO_LIST(stone_node, tabledef::ROLE_ATTR_DETAIL2_STONE_DATA, other_p.stone_param, StoneParamHex, attr_list);

		UPDATE_BINARY_DATA_TO_LIST(guild_xianshu_node, tabledef::ROLE_ATTR_DETAIL2_GUILD_XIANSHU_DATA, other_p.guild_xianshu_param, GuildXianshuParamHex, attr_list);

		{
			static char taskrecord_hex_buff[MAX_TASK_RECORD_BUFF_LEN * 2 + 1] = {0};
			BinToHex(other_p.taskrecord_buff, other_p.taskrecord_buff_len, taskrecord_hex_buff);
			taskrecord_hex_buff[other_p.taskrecord_buff_len * 2] = 0;

			DataNode taskrecord_data_node(tabledef::ROLE_ATTR_DETAIL2_TASKRECORD_DATA);
			taskrecord_data_node.SetString(taskrecord_hex_buff);
			attr_list.push_back(taskrecord_data_node);
		}

		UPDATE_BINARY_DATA_TO_LIST(task_node, tabledef::ROLE_ATTR_DETAIL2_TASKINFO_DATA, other_p.task_param, TaskParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(role_supply_node, tabledef::ROLE_ATTR_DETAIL2_ROLE_SUPPLY_DATA, other_p.role_supply_param, RoleSupplyParamHex, attr_list);

		CmpNodeList condition;
		CmpNode id_node(tabledef::ROLE_ATTR_DETAIL2_ROLE_ID);
		id_node.SetInt32(role_id);
		condition.push_back(id_node);

		int count = roleattrcommand2->Update(stmt, condition, attr_list);
		if (count < 1)
		{
			dataaccesslog::g_log_role.printf(LL_CRITICAL, "SaveTableRole roleattrcommand2->Update uid:%d", role_id);
			*error |= dataaccessrmi::role::ROLE_ATTR_DETAIL_ERROR;
			ret = -4;
		}
	}

	// 人物表3
	{
		DBCommand *roleattrcommand3 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL3);

		DataNodeList attr_list;
		attr_list.reserve(tabledef::TABLE_ROLE_ATTR_DETAIL3_MAXINDEX - 2);

		UPDATE_BINARY_DATA_TO_LIST(qingyuan_node, tabledef::ROLE_ATTR_DETAIL3_QINGYUAN_DATA, other_p.qingyuan_param, QingyuanParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(role_goal_node, tabledef::ROLE_ATTR_DETAIL3_ROLE_GOAL_DATA, other_p.role_goal_param, RoleGoalParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(skill_other_node, tabledef::ROLE_ATTR_DETAIL3_SKILL_OTHER_DATA, other_p.skill_other_param, SkillOtherParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(history_cap_node, tabledef::ROLE_ATTR_DETAIL3_HISTORY_CAP_DATA, other_p.history_camp_param, HistoryCapParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(other_module_node, tabledef::ROLE_ATTR_DETAIL3_OTHER_MODULE_DATA, other_p.other_module_param, OtherModuleDataParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(role_guild_data, tabledef::ROLE_ATTR_DETAIL3_ROLE_GUILD_DATA, other_p.role_guild_param, RoleGuildParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(shengwang_data, tabledef::ROLE_ATTR_DETAIL3_SHENGWANG_DATA, other_p.shengwang_param, ShengWangParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(chengjiu_data, tabledef::ROLE_ATTR_DETAIL3_CHENGJIU_DATA, other_p.chengjiu_param, ChengJiuParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(dailyfb_data, tabledef::ROLE_ATTR_DETAIL3_DAILYFB_DATA, other_p.dailyfb_param, DailyFbParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(lifeskill_data, tabledef::ROLE_ATTR_DETAIL3_LIFESKILL_DATA, other_p.lifeskill_param, LifeSkillParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(baby_data, tabledef::ROLE_ATTR_DETAIL3_BABY_DATA, other_p.baby_param, BabyParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(shenge_system_data, tabledef::ROLE_ATTR_DETAIL3_SHENGE_SYSTEM_DATA, other_p.shenge_system_param, ShengeSystemParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(zero_gigt_node, tabledef::ROLE_ATTR_DETAIL3_ZERO_GIFT_DATA, other_p.zero_gift_param, ZeroGiftParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(mysterious_shop_in_mall_data, tabledef::ROLE_ATTR_DETAIL3_MYSTERIOUS_SHOP_IN_MALL_DATA, other_p.mysterious_shop_in_mall_param, MysteriousShopInMallParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(friend_exp_bottle_data, tabledef::ROLE_ATTR_DETAIL3_FRIEND_EXP_BOTTLE_DATA, other_p.friend_exp_bottle_param, FriendExpBottleParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(shenshou_node, tabledef::ROLE_ATTR_DETAIL3_SHENSHOU, other_p.shenshou_param, ShenshouParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(img_fuling_node, tabledef::ROLE_ATTR_DETAIL3_IMG_FULING_DATA, other_p.img_fuling_param, ImgFulingParamHex, attr_list);

		CmpNodeList condition;
		CmpNode id_node(tabledef::ROLE_ATTR_DETAIL3_ROLE_ID);
		id_node.SetInt32(role_id);
		condition.push_back(id_node);

		int count = roleattrcommand3->Update(stmt, condition, attr_list);
		if (count < 1)
		{
			dataaccesslog::g_log_role.printf(LL_CRITICAL, "SaveTableRole roleattrcommand3->Update uid:%d", role_id);
			*error |= dataaccessrmi::role::ROLE_ATTR_DETAIL_ERROR;
			ret = -4;
		}
	}

	// 人物表4
	{
		DBCommand *roleattrcommand4 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL4);

		DataNodeList attr_list;
		attr_list.reserve(tabledef::TABLE_ROLE_ATTR_DETAIL4_MAXINDEX - 2);

		UPDATE_BINARY_DATA_TO_LIST(tuhaojin_node, tabledef::ROLE_ATTR_DETAIL4_TUHAOJIN_DATA, other_p.tuhaojin_param, TuHaoJinParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(bigchatface_node, tabledef::ROLE_ATTR_DETAIL4_BIGCHATFACE_DATA, other_p.bigchatface_param, BigChatFaceParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(pet_node, tabledef::ROLE_ATTR_DETAIL4_PET_DATA, other_p.pet_param, PetParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(phasefb_node, tabledef::ROLE_ATTR_DETAIL4_PHASE_FB_DATA, other_p.phase_fb_param, PhaseFbParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(vipfb_node, tabledef::ROLE_ATTR_DETAIL4_VIP_FB_DATA, other_p.vipfb_param, VipFBParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(huashen_node, tabledef::ROLE_ATTR_DETAIL4_HUASHEN_DATA, other_p.huashen_param, HuashenParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(magic_card_node, tabledef::ROLE_ATTR_DETAIL4_MAGIC_CARD_DATA, other_p.magic_card_param, MagicCardParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(mitama_node, tabledef::ROLE_ATTR_DETAIL4_MITAMA_DATA, other_p.mitama_param, MitamaParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(equipment_node, tabledef::ROLE_ATTR_DETAIL4_EQUIPMENT_DATA, other_p.equipment_param, EquipmentGridParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(equipment_node, tabledef::ROLE_ATTR_DETAIL4_CHINESE_ZODIAC_DATA, other_p.chinese_zodiac_param, ChineseZodiacParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(equipment_node, tabledef::ROLE_ATTR_DETAIL4_FRIEND_GIFT, other_p.friend_gift_param, FriendGiftParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(followboss_node, tabledef::ROLE_ATTR_DETAIL4_FOLLOW_BOSS, other_p.follow_boss_param, FollowBossParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(goldvip_node, tabledef::ROLE_ATTR_DETAIL4_GOLD_VIP, other_p.gold_vip_param, GoldVipParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(rune_system_node, tabledef::ROLE_ATTR_DETAIL4_RUNE_SYSTEM, other_p.rune_system_param, RuneSystemParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(magical_precious_node, tabledef::ROLE_ATTR_DETAIL4_MAGICAL_PRECIOUS_DATA, other_p.magical_precious_param, MagicalPreciousParamHex, attr_list);

		CmpNodeList condition;
		CmpNode id_node(tabledef::ROLE_ATTR_DETAIL4_ROLE_ID);
		id_node.SetInt32(role_id);
		condition.push_back(id_node);

		int count = roleattrcommand4->Update(stmt, condition, attr_list);
		if (count < 1)
		{
			dataaccesslog::g_log_role.printf(LL_CRITICAL, "SaveTableRole roleattrcommand4->Update uid:%d", role_id);
			*error |= dataaccessrmi::role::ROLE_ATTR_DETAIL_ERROR;
			ret = -5;
		}
	}

	// 人物表5
	{
		DBCommand *roleattrcommand5 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL5);

		DataNodeList attr_list;
		attr_list.reserve(tabledef::TABLE_ROLE_ATTR_DETAIL5_MAXINDEX - 2);

		UPDATE_BINARY_DATA_TO_LIST(multi_mount_node, tabledef::ROLE_ATTR_DETAIL5_MULTI_MOUNT_DATA, other_p.multi_mount_param, MultiMountParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(personalize_window_node, tabledef::ROLE_ATTR_DETAIL5_PERSONALIZE_WINDOW_DATA, other_p.personalize_window_param, PersonalizeWindowParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(xunzhang_node, tabledef::ROLE_ATTR_DETAIL5_XUNZHANG, other_p.xunzhang_param, XunZhangParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(zhibao_node, tabledef::ROLE_ATTR_DETAIL5_ZHIBAO, other_p.zhibao_param, ZhiBaoParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(jingling_node, tabledef::ROLE_ATTR_DETAIL5_JINGLING, other_p.jingling_param, JingLingParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(xiannv_shouhu_node, tabledef::ROLE_ATTR_DETAIL5_XIANNV_SHOUHU, other_p.xiannv_shouhu_param, XiannvShouhuParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(jingling_fazhen_node, tabledef::ROLE_ATTR_DETAIL5_JINGLING_FAZHEN, other_p.jingling_fazhen_param, JinglingFazhenParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(jingling_guanghuan_node, tabledef::ROLE_ATTR_DETAIL5_JINGLING_GUANGHUAN, other_p.jingling_guanghuan_param, JinglingGuanghuanParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(shenzhou_weapon_data, tabledef::ROLE_ATTR_DETAIL5_SHENZHOU_WEAPON_DATA, other_p.shenzhou_weapon_param, ShenzhouWeaponParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(lieming_node, tabledef::ROLE_ATTR_DETAIL5_LIEMING, other_p.lieming_param, LieMingSlotParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(lieming_bag_node, tabledef::ROLE_ATTR_DETAIL5_LIEMING_BAG, other_p.lieming_bag_param, LieMingHunshouBagParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(cardzu_node, tabledef::ROLE_ATTR_DETAIL5_CARDZU, other_p.cardzu_param, CardzuParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(zhuansheng_equip_node, tabledef::ROLE_ATTR_DETAIL5_ZHUANSHENG_EQUIP, other_p.zhuansheng_equip_param, ZhuanShengEquipParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(shenbing_node, tabledef::ROLE_ATTR_DETAIL5_SHENBING, other_p.shenbing_param, ShenBingParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(tuitu_fb_node, tabledef::ROLE_ATTR_DETAIL5_TUITUFB_DATA, other_p.tuitu_fb_param, TuituFbParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(cloak_node, tabledef::ROLE_ATTR_DETAIL5_CLOAK, other_p.cloak_param, CloakParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(precious_boss_node, tabledef::ROLE_ATTR_DETAIL5_ROLE_PRECIOUS_BOSS, other_p.precious_boss_param, PreciousBossParamHex, attr_list);

		CmpNodeList condition;
		CmpNode id_node(tabledef::ROLE_ATTR_DETAIL5_ROLE_ID);
		id_node.SetInt32(role_id);
		condition.push_back(id_node);

		int count = roleattrcommand5->Update(stmt, condition, attr_list);
		if (count < 1)
		{
			dataaccesslog::g_log_role.printf(LL_CRITICAL, "SaveTableRole roleattrcommand5->Update uid:%d", role_id);
			*error |= dataaccessrmi::role::ROLE_ATTR_DETAIL_ERROR;
			ret = -6;
		}
	}

	// 人物表6
	{
		DBCommand *roleattrcommand6 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL6);

		DataNodeList attr_list;
		attr_list.reserve(tabledef::TABLE_ROLE_ATTR_DETAIL6_MAXINDEX - 2);

		UPDATE_BINARY_DATA_TO_LIST(tmp_node, tabledef::ROLE_ATTR_DETAIL6_COUPLE_MOUNT_DATA, other_p.couple_mount_param, CoupleMountParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(tmp_node, tabledef::ROLE_ATTR_DETAIL6_SHENYIN_DATA, other_p.shenyin_param, ShenYinParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(tmp_node, tabledef::ROLE_ATTR_DETAIL6_ELEMENT_HEART_DATA, other_p.element_heart_param, ElementHeartParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(tmp_node, tabledef::ROLE_ATTR_DETAIL6_SHENGXINGZHULI, other_p.shengxing_zhuli_param, ShengxingzhuliParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(tmp_node, tabledef::ROLE_ATTR_DETAIL6_SHIZHUANG_DATA, other_p.shizhuang_param, ShizhuangParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(tmp_node, tabledef::ROLE_ATTR_DETAIL6_FABAO_DATA, other_p.fabao_param, FabaoParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(tmp_node, tabledef::ROLE_ATTR_DETAIL6_FEIXIAN_DATA, other_p.feixian_param, FeiXianParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(tmp_node, tabledef::ROLE_ATTR_DETAIL6_TALENT_DATA, other_p.talent_param, TalentParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(tmp_node, tabledef::ROLE_ATTR_DETAIL6_JINJIE_TALENT_DATA, other_p.jinjie_talent_param, JinJieTalentParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(tmp_node, tabledef::ROLE_ATTR_DETAIL6_ARMOR_FB_DATA, other_p.armor_fb_param, ArmorDefendParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(tmp_node, tabledef::ROLE_ATTR_DETAIL6_BOSSCARD_DATA, other_p.bosscard_param, BossCardParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(tmp_node, tabledef::ROLE_ATTR_DETAIL6_TASK_ZHUANZHI_DATA, other_p.task_zhuanzhi_param, ZhuanzhiTaskParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(tmp_node, tabledef::ROLE_ATTR_DETAIL6_DISCOUNT_BUY_DATA, other_p.discount_buy_param, DiscountBuyParamHex, attr_list);

		UNSTD_STATIC_CHECK(15 == tabledef::TABLE_ROLE_ATTR_DETAIL6_MAXINDEX);

		CmpNodeList condition;
		CmpNode id_node(tabledef::ROLE_ATTR_DETAIL6_ROLE_ID);
		id_node.SetInt32(role_id);
		condition.push_back(id_node);

		int count = roleattrcommand6->Update(stmt, condition, attr_list);
		if (count < 1)
		{
			dataaccesslog::g_log_role.printf(LL_CRITICAL, "SaveTableRole roleattrcommand6->Update uid:%d", role_id);
			*error |= dataaccessrmi::role::ROLE_ATTR_DETAIL_ERROR;
			ret = -6;
		}
	}

	// 人物表7
	{
		DBCommand *roleattrcommand7 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL7);

		DataNodeList attr_list;
		attr_list.reserve(tabledef::TABLE_ROLE_ATTR_DETAIL7_MAXINDEX - 2);

		UPDATE_BINARY_DATA_TO_LIST(cross_global_data, tabledef::ROLE_ATTR_DETAIL7_CROSS_GLOBAL_DATA, other_p.cross_global_data_param, RoleCrossGlobalDataHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(zhuanzhi_equip_data, tabledef::ROLE_ATTR_DETAIL7_ZHUANZHI_EQUIP_DATA, other_p.zhuanzhi_equip_param, ZhuanzhiEquipParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(role_cross_node, tabledef::ROLE_ATTR_DETAIL7_ROLE_CROSS_DATA, other_p.role_cross_param, RoleCrossParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(heshenluoshu_data, tabledef::ROLE_ATTR_DETAIL7_HESHENLUOSHU_DATA, other_p.heshen_luoshu_param, HeShenLuoShuParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(shengqi_data, tabledef::ROLE_ATTR_DETAIL7_SHENGQI_DATA, other_p.shengqi_param, ShengqiParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(seal_data, tabledef::ROLE_ATTR_DETAIL7_SEAL_DATA, other_p.seal_param, SealParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(dressing_room_data, tabledef::ROLE_ATTR_DETAIL7_DRESSING_ROOM_DATA, other_p.dressing_room_param, DressingRoomParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(challengefb_node, tabledef::ROLE_ATTR_DETAIL7_CHALLENGE_FB_DATA, other_p.challengefb_param, ChallengeFBParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(role_cross_randactivity_data, tabledef::ROLE_ATTR_DETAIL7_ROLE_CROSS_RAND_ACTIVITY_DATA, other_p.role_cross_rand_activity_param, RoleCrossRandActivityParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(cross_goal_data, tabledef::ROLE_ATTR_DETAIL7_CROSS_GOAL_DATA, other_p.cross_goal_param, CrossGoalParamHex, attr_list);

		UNSTD_STATIC_CHECK(12 == tabledef::TABLE_ROLE_ATTR_DETAIL7_MAXINDEX);

		CmpNodeList condition;
		CmpNode id_node(tabledef::ROLE_ATTR_DETAIL7_ROLE_ID);
		id_node.SetInt32(role_id);
		condition.push_back(id_node);

		int count = roleattrcommand7->Update(stmt, condition, attr_list);
		if (count < 1)
		{
			dataaccesslog::g_log_role.printf(LL_CRITICAL, "SaveTableRole roleattrcommand7->Update uid:%d", role_id);
			*error |= dataaccessrmi::role::ROLE_ATTR_DETAIL_ERROR;
			ret = -7;
		}
	}

	// 人物表8
	{
		DBCommand *roleattrcommand8 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL8);

		DataNodeList attr_list;
		attr_list.reserve(tabledef::TABLE_ROLE_ATTR_DETAIL8_MAXINDEX - 2);

		UPDATE_BINARY_DATA_TO_LIST(upgrade_manager_data, tabledef::ROLE_ATTR_DETAIL8_UPGRADE_MANAGER_DATA, other_p.upgrade_manager_param, UpgradeManagerParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(shenqi_data, tabledef::ROLE_ATTR_DETAIL8_SHENQI_DATA, other_p.shenqi_param, ShenqiParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(minghun2_data, tabledef::ROLE_ATTR_DETAIL8_MINGHUN2_DATA, other_p.lieming_param2, LieMingParam2Hex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(profess_data, tabledef::ROLE_ATTR_DETAIL8_PERSON_PROFESS_DATA, other_p.profess_param, PersonProfessParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(xianzunka_data, tabledef::ROLE_ATTR_DETAIL8_XIANZUNKA_DATA, other_p.xianzunka_param, XianZunKaParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(equip_baptize_data, tabledef::ROLE_ATTR_DETAIL8_EQUIP_BAPTIZE_DATA, other_p.equipbaptize_param, EquipBaptizeParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(mojie_node, tabledef::ROLE_ATTR_DETAIL8_MOJIE, other_p.mojie_param, MojieParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(personboss_fb_node, tabledef::ROLE_ATTR_DETAIL8_FB_PERSONBOSS_DATA, other_p.personboss_param, PersonBossParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(greate_soldier_node, tabledef::ROLE_ATTR_DETAIL8_GREATE_SOLDIER_DATA, other_p.greate_soldier_param, GreateSoldierParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(lottery_count_node, tabledef::ROLE_ATTR_DETAIL8_LOTTERY_COUNT_DATA, other_p.role_lottery_count_param, RoleLotteryCountParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(upgrade_card_buy_data, tabledef::ROLE_ATTR_DETAIL8_UPGRADE_CARD_BUY_DATA, other_p.upgrade_card_buy_param, UpgradeCardBuyParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(myth_data, tabledef::ROLE_ATTR_DETAIL8_MYTH_DATA, other_p.myth_param, MythParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(zodiac_data, tabledef::ROLE_ATTR_DETAIL8_ZODIAC_DATA, other_p.zodiac_param, ZodiacParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(cross_equip_data, tabledef::ROLE_ATTR_DETAIL8_CROSS_EQUIP_DATA, other_p.cross_equip_param, CrossEquipParamHex, attr_list);

		UNSTD_STATIC_CHECK(16 == tabledef::TABLE_ROLE_ATTR_DETAIL8_MAXINDEX);

		CmpNodeList condition;
		CmpNode id_node(tabledef::ROLE_ATTR_DETAIL8_ROLE_ID);
		id_node.SetInt32(role_id);
		condition.push_back(id_node);

		int count = roleattrcommand8->Update(stmt, condition, attr_list);
		if (count < 1)
		{
			dataaccesslog::g_log_role.printf(LL_CRITICAL, "SaveTableRole roleattrcommand8->Update uid:%d", role_id);
			*error |= dataaccessrmi::role::ROLE_ATTR_DETAIL_ERROR;
			ret = -8;
		}
	}

	// 人物表9
	{
		DBCommand *roleattrcommand9 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL9);

		DataNodeList attr_list;
		attr_list.reserve(tabledef::TABLE_ROLE_ATTR_DETAIL9_MAXINDEX - 2);

		UPDATE_BINARY_DATA_TO_LIST(red_equip_collect_data, tabledef::ROLE_ATTR_DETAIL9_RED_EQUIP_COLLECT_DATA, other_p.red_equip_collect_param, RedEquipCollectParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(mount_node, tabledef::ROLE_ATTR_DETAIL9_MOUNT_DATA, other_p.mount_param, MountParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(footprint_node, tabledef::ROLE_ATTR_DETAIL9_FOOTPRINT, other_p.footprint_param, FootprintParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(halo_node, tabledef::ROLE_ATTR_DETAIL9_HALO, other_p.halo_param, HaloParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(huguozhili_node, tabledef::ROLE_ATTR_DETAIL9_HUGUOZHILI, other_p.huguozhili_param, HuguozhiliParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(baizhan_equip_data, tabledef::ROLE_ATTR_DETAIL9_BAIZHAN_EQUIP_DATA, other_p.baizhan_equip_param, BaizhanEquipParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(fight_mount_node, tabledef::ROLE_ATTR_DETAIL9_FIGHT_MOUNT_DATA, other_p.fight_mount_param, FightMountParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(mojiefb_node, tabledef::ROLE_ATTR_DETAIL9_MOJIEFB_DATA, other_p.mojiefb_param, MojieFBParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(equipsuit_node, tabledef::ROLE_ATTR_DETAIL9_EQUIPSUIT_DATA, other_p.equipsuit_param, EquipSuitParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(shenzhuang_node, tabledef::ROLE_ATTR_DETAIL9_SHENZHUANG, other_p.shenzhuang_param, ShenZhuangParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(csa_equip_node, tabledef::ROLE_ATTR_DETAIL9_CSA_EQUIP_DATA, other_p.csa_equip_param, CSAEquipParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(shengong_node, tabledef::ROLE_ATTR_DETAIL9_SHEN_GONG, other_p.shengong_param, ShengongParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(shenyi_node, tabledef::ROLE_ATTR_DETAIL9_SHEN_YI, other_p.shenyi_param, ShenyiParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(discountshop_data, tabledef::ROLE_ATTR_DETAIL9_DISCOUNTSHOP_DATA, other_p.discountbuy_param, DisCountShopParamHex, attr_list);

		UNSTD_STATIC_CHECK(16 == tabledef::TABLE_ROLE_ATTR_DETAIL9_MAXINDEX);

		CmpNodeList condition;
		CmpNode id_node(tabledef::ROLE_ATTR_DETAIL9_ROLE_ID);
		id_node.SetInt32(role_id);
		condition.push_back(id_node);

		int count = roleattrcommand9->Update(stmt, condition, attr_list);
		if (count < 1)
		{
			dataaccesslog::g_log_role.printf(LL_CRITICAL, "SaveTableRole roleattrcommand9->Update uid:%d", role_id);
			*error |= dataaccessrmi::role::ROLE_ATTR_DETAIL_ERROR;
			ret = -9;
		}
	}

	// 人物表10
	{
		DBCommand *roleattrcommand10 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL10);

		DataNodeList attr_list;
		attr_list.reserve(tabledef::TABLE_ROLE_ATTR_DETAIL10_MAXINDEX - 2);

		UPDATE_BINARY_DATA_TO_LIST(orange_equip_collect_data, tabledef::ROLE_ATTR_DETAIL10_ORANGE_EQUIP_COLLECT_DATA, other_p.orange_equip_collect_param, OrangeEquipCollectParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(yaoshi_data, tabledef::ROLE_ATTR_DETAIL10_YAOSHI_DATA, other_p.yaoshi_param, YaoShiParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(toushi_data, tabledef::ROLE_ATTR_DETAIL10_TOUSHI_DATA, other_p.toushi_param, TouShiParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(qilinbi_data, tabledef::ROLE_ATTR_DETAIL10_QILINBI_DATA, other_p.qilinbi_param, QiLinBiParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(qilinbi_data, tabledef::ROLE_ATTR_DETAIL10_MASK_DATA, other_p.mask_param, MaskParamHex, attr_list);

		UPDATE_BINARY_DATA_TO_LIST(tmp_node, tabledef::ROLE_ATTR_DETAIL10_JINJIESYS_REWARD_DATA, other_p.jinjiesys_reward_param, JinjieSysRewardParamHex, attr_list);
		UPDATE_BINARY_DATA_TO_LIST(tmp_node, tabledef::ROLE_ATTR_DETAIL10_ROLE_BIG_SMALL_GOAL_DATA, other_p.role_big_small_goal_param, RoleBigSmallGoalRewardParamHex, attr_list);

		BinToHexAutoDataPegex(attr_list, other_p.data_page_10, tabledef::ROLE_ATTR_DETAIL10_DATA_PAGE, tabledef::ROLE_ATTR_DETAIL10_DATA_PAGE_LEN);

		UNSTD_STATIC_CHECK(11 == tabledef::TABLE_ROLE_ATTR_DETAIL10_MAXINDEX);

		CmpNodeList condition;
		CmpNode id_node(tabledef::ROLE_ATTR_DETAIL10_ROLE_ID);
		id_node.SetInt32(role_id);
		condition.push_back(id_node);

		int count = roleattrcommand10->Update(stmt, condition, attr_list);
		if (count < 1)
		{
			dataaccesslog::g_log_role.printf(LL_CRITICAL, "SaveTableRole roleattrcommand10->Update uid:%d", role_id);
			*error |= dataaccessrmi::role::ROLE_ATTR_DETAIL_ERROR;
			ret = -10;
		}
	}

	if (is_logout) dataaccesslog::g_log_role.printf(LL_DEBUG, "SaveTableRoleByLogout uid:%d", role_id);
	
	return ret < 0 ? ret : 0;
}

int RMIRoleObject::SaveTableItemList(IStatement *stmt, int role_id, const ItemListParam &item_list_param)
{
	int ret = 0;
	{
		DBCommand *itemlistcommond = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ITEMLIST);

		for (int i = 0; i < item_list_param.count; ++i)
		{
			if ((int)item_list_param.item_list[i].change_state_item == structcommon::CS_NONE) continue;

			long long item_list_for_index = TranslateInt64(role_id, (int)item_list_param.item_list[i].index);

			switch (item_list_param.item_list[i].change_state_item)
			{
			case structcommon::CS_DELETE:
				{
					CmpNodeList cmp_node;

					CmpNode for_index_node(tabledef::ITEMLIST_FOR_INDEX);
					for_index_node.SetLong64(item_list_for_index);
					cmp_node.push_back(for_index_node);

					int count = itemlistcommond->Remove(stmt, cmp_node);
					if (count < 1)
					{  
						dataaccesslog::g_log_role.printf(LL_CRITICAL, "SaveTableItemList itemlistcommond->Remove index:%d uid:%d knapsack_for_index:%lld itemid:%d itemnum:%d", 
							item_list_param.item_list[i].index, role_id, item_list_for_index, item_list_param.item_list[i].item_wrapper.item_id, item_list_param.item_list[i].item_wrapper.num);
						ret = -1;
					}
				}
				break;

			case structcommon::CS_UPDATE:
				{
					DataNodeList attr_list;
					attr_list.reserve(tabledef::TABLE_ITEMLIST_MAXINDEX - 4);

					DataNode item_id(tabledef::ITEMLIST_ITEM_ID);
					item_id.SetInt32((int)item_list_param.item_list[i].item_wrapper.item_id);
					attr_list.push_back(item_id);

					DataNode is_bind(tabledef::ITEMLIST_IS_BIND);
					is_bind.SetByte((char)item_list_param.item_list[i].item_wrapper.is_bind);
					attr_list.push_back(is_bind);

					DataNode num(tabledef::ITEMLIST_NUM);
					num.SetInt32((int)item_list_param.item_list[i].item_wrapper.num);
					attr_list.push_back(num);

					DataNode invalid_time(tabledef::ITEMLIST_INVALID_TIME);
					invalid_time.SetLong64(item_list_param.item_list[i].item_wrapper.invalid_time);
					attr_list.push_back(invalid_time);

					DataNode gold_price(tabledef::ITEMLIST_GOLD_PRICE);
					gold_price.SetInt32(item_list_param.item_list[i].item_wrapper.gold_price);
					attr_list.push_back(gold_price);

					DataNode param(tabledef::ITEMLIST_PARAM);
					if (0 != item_list_param.item_list[i].item_wrapper.has_param)
					{
						static ItemParamDataStructHex hex_buff;

						BinToHex((const char *)(&item_list_param.item_list[i].item_wrapper.param_data), sizeof(item_list_param.item_list[i].item_wrapper.param_data), hex_buff);
						hex_buff[sizeof(hex_buff) - 1] = 0;

						param.SetString(hex_buff);
					}
					else
					{
						param.SetString("");
					}
					attr_list.push_back(param);

					CmpNodeList cmp_node;

					CmpNode for_index_node(tabledef::ITEMLIST_FOR_INDEX);
					for_index_node.SetLong64(item_list_for_index);
					cmp_node.push_back(for_index_node);

					int count = itemlistcommond->Update(stmt, cmp_node, attr_list);
					if (count < 1)
					{
						ret = -2; // 此时不break update不成功时则继续insert 以兼容某些错误
						dataaccesslog::g_log_role.printf(LL_CRITICAL, "SaveTableItemList itemlistcommond->Update index:%d uid:%d knapsack_for_index:%lld itemid:%d itemnum:%d", 
							item_list_param.item_list[i].index, role_id, item_list_for_index, (int)item_list_param.item_list[i].item_wrapper.item_id, (int)item_list_param.item_list[i].item_wrapper.num);
					}
					else
					{
						break;
					}
				}

			case structcommon::CS_INSERT:
				{
					const MataTableDef *itemlisttable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ITEMLIST);
					DataAdapter item_data = itemlisttable->GetPrototype();
					item_data.Malloc();

					item_data.m_data_area[tabledef::ITEMLIST_ROLE_ID].vint = role_id;
					item_data.m_data_area[tabledef::ITEMLIST_ITEMLIST_INDEX].vint = (int)item_list_param.item_list[i].index;
					item_data.m_data_area[tabledef::ITEMLIST_FOR_INDEX].vint64 = item_list_for_index;
					item_data.m_data_area[tabledef::ITEMLIST_ITEM_ID].vint = (int)item_list_param.item_list[i].item_wrapper.item_id;
					item_data.m_data_area[tabledef::ITEMLIST_IS_BIND].vchar = (char)item_list_param.item_list[i].item_wrapper.is_bind;
					item_data.m_data_area[tabledef::ITEMLIST_NUM].vint = (int)item_list_param.item_list[i].item_wrapper.num;
					item_data.m_data_area[tabledef::ITEMLIST_INVALID_TIME].vint64 = (long long)item_list_param.item_list[i].item_wrapper.invalid_time;
					item_data.m_data_area[tabledef::ITEMLIST_GOLD_PRICE].vint = item_list_param.item_list[i].item_wrapper.gold_price;

					if (0 != item_list_param.item_list[i].item_wrapper.has_param)
					{
						static ItemParamDataStructHex hex_buff;

						BinToHex((const char *)(&item_list_param.item_list[i].item_wrapper.param_data), sizeof(ItemParamDataStruct), hex_buff);
						hex_buff[sizeof(hex_buff) - 1] = 0;

						STRNCPY(item_data.m_data_area[tabledef::ITEMLIST_PARAM].vcharp, hex_buff, item_data.m_data_area[tabledef::ITEMLIST_PARAM].length);
					}

					if (DBCommand::RESULT_SUC != itemlistcommond->Save(stmt, &item_data))
					{
						dataaccesslog::g_log_role.printf(LL_CRITICAL, "SaveTableItemList itemlistcommond->Save index:%d uid:%d knapsack_for_index:%lld itemid:%d itemnum:%d", 
							item_list_param.item_list[i].index, role_id, item_list_for_index, (int)item_list_param.item_list[i].item_wrapper.item_id, (int)item_list_param.item_list[i].item_wrapper.num);
						ret = -3;
					}

					item_data.Free();
				}
				break;
			}
		}
	}

	return ret < 0 ? ret : 0;
}

int RMIRoleObject::SaveTableHotkey(IStatement *stmt, int role_id, const HotkeyListParam &hotkey)
{
	DBCommand *hotkeycommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_HOTKEY);

	int ret = 0;
	for(int i = 0; i < hotkey.count; ++i)
	{
		if(hotkey.hotkey_list[i].change_state == structcommon::CS_NONE) continue;

		long long hotkey_for_index = TranslateInt64(role_id, (int)hotkey.hotkey_list[i].index);

		switch(hotkey.hotkey_list[i].change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_list;

				CmpNode for_index_node(tabledef::HOTKEY_FOR_INDEX);
				for_index_node.SetLong64(hotkey_for_index);
				cmp_list.push_back(for_index_node);

				int count = hotkeycommand->Remove(stmt, cmp_list);
				if(count < 1)
				{
					ret = -1;
				}
			}
			break;
		case structcommon::CS_UPDATE:
			{
				DataNodeList hotkey_list;
				hotkey_list.reserve(tabledef::TABLE_HOTKEY_MAXINDEX - 4);

				DataNode hotkey_item_type_node(tabledef::HOTKEY_HOTKEY_ITEM_TYPE);
				hotkey_item_type_node.SetByte(hotkey.hotkey_list[i].item_type);
				hotkey_list.push_back(hotkey_item_type_node);

				DataNode hotkey_item_id_node(tabledef::HOTKEY_HOTKEY_ITEM_ID);
				hotkey_item_id_node.SetInt32(hotkey.hotkey_list[i].item_id);
				hotkey_list.push_back(hotkey_item_id_node);

				CmpNodeList cmp_list;

				CmpNode for_index_node(tabledef::HOTKEY_FOR_INDEX);
				for_index_node.SetLong64(hotkey_for_index);
				cmp_list.push_back(for_index_node);

				int count = hotkeycommand->Update(stmt, cmp_list, hotkey_list);
				if(count < 1)
				{
					ret = -2;
					// 此时不break，update不成功时则继续insert，以兼容某些错误
				}
				else
				{
					break;
				}
			}
		case structcommon::CS_INSERT:
			{
				const MataTableDef *hotkeytable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_HOTKEY);
				DataAdapter hotkey_item = hotkeytable->GetPrototype();
				hotkey_item.Malloc();
				hotkey_item.m_data_area[tabledef::HOTKEY_ROLE_ID].vint = role_id;
				hotkey_item.m_data_area[tabledef::HOTKEY_HOTKEY_INDEX].vint = hotkey.hotkey_list[i].index;
				hotkey_item.m_data_area[tabledef::HOTKEY_FOR_INDEX].vint64 = hotkey_for_index;
				hotkey_item.m_data_area[tabledef::HOTKEY_HOTKEY_ITEM_TYPE].vchar = hotkey.hotkey_list[i].item_type;
				hotkey_item.m_data_area[tabledef::HOTKEY_HOTKEY_ITEM_ID].vint = hotkey.hotkey_list[i].item_id;

				if(DBCommand::RESULT_SUC != hotkeycommand->Save(stmt, &hotkey_item))
				{
					ret = -3;
				}
				hotkey_item.Free();
			}
			break;
		}
	}

	return ret < 0 ? ret : 0;
}

int RMIRoleObject::SaveTableFrinend(IStatement *stmt, int role_id, const FriendListParam &friends)
{
	DBCommand *friendcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_FRIENDS);

	int ret = 0;
	for(int i = 0;i < friends.count; ++i)
	{
		if(friends.friend_list[i].change_state == structcommon::CS_NONE)continue;

		long long friend_for_index = TranslateInt64(role_id, friends.friend_list[i].index);

		switch(friends.friend_list[i].change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_list;

				CmpNode for_index_node(tabledef::FRIENDS_FOR_INDEX);
				for_index_node.SetLong64(friend_for_index);
				cmp_list.push_back(for_index_node);

				int count = friendcommand->Remove(stmt, cmp_list);
				if(count < 1)
				{
					ret = -1;
				}

			}
			break;
		case structcommon::CS_UPDATE:
			{
				DataNodeList friend_list;
				friend_list.reserve(tabledef::TABLE_FRIENDS_MAXINDEX - 3);

				DataNode firend_item_friend_uid(tabledef::FRIENDS_FRIEND_UID);
				firend_item_friend_uid.SetInt32(friends.friend_list[i].friend_uid);
				friend_list.push_back(firend_item_friend_uid);

				DataNode firend_item_friend_intimacy(tabledef::FRIENDS_INTIMACY);
				firend_item_friend_intimacy.SetInt32(friends.friend_list[i].intimacy);
				friend_list.push_back(firend_item_friend_intimacy);

				DataNode friend_item_friend_team_today_intimacy(tabledef::FRIENDS_TEAM_TODAY_INTIMACY);
				friend_item_friend_team_today_intimacy.SetInt32(friends.friend_list[i].team_today_intimacy);
				friend_list.push_back(friend_item_friend_team_today_intimacy);

				DataNode firend_item_friend_gift_count(tabledef::FRIENDS_GIFT_COUNT);
				firend_item_friend_gift_count.SetInt32(friends.friend_list[i].gift_count);
				friend_list.push_back(firend_item_friend_gift_count);

				DataNode friend_item_friend_index(tabledef::FRIENDS_FRIEND_INDEX);
				friend_item_friend_index.SetInt32(friends.friend_list[i].index);
				friend_list.push_back(friend_item_friend_index);

				DataNode friend_item_friend_favorable_impression(tabledef::FRIENDS_FAVORABLE_IMPRESSION);
				friend_item_friend_favorable_impression.SetInt32(friends.friend_list[i].favorable_impression);
				friend_list.push_back(friend_item_friend_favorable_impression);

				DataNode friend_item_friend_day_chat_count(tabledef::FRIENDS_DAY_CHAT_COUNT);
				friend_item_friend_day_chat_count.SetInt32(friends.friend_list[i].day_chat_count);
				friend_list.push_back(friend_item_friend_day_chat_count);

				DataNode friend_item_friend_add_timestamp(tabledef::FRIENDS_ADD_TIMESTAMP);
				friend_item_friend_add_timestamp.SetLong64(friends.friend_list[i].add_timestamp);
				friend_list.push_back(friend_item_friend_add_timestamp);

				CmpNodeList cmp_list;

				CmpNode for_index_node(tabledef::FRIENDS_FOR_INDEX);
				for_index_node.SetLong64(friend_for_index);
				cmp_list.push_back(for_index_node);

				int count = friendcommand->Update(stmt, cmp_list, friend_list);

				if(count < 1)
				{
					ret = -2;
					// 此时不break，update不成功时则继续insert，以兼容某些错误
				}
				else
				{
					break;
				}
			}
		case structcommon::CS_INSERT:
			{
				const MataTableDef	*friendtable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_FRIENDS);
				DataAdapter	friend_item	= friendtable->GetPrototype();
				friend_item.Malloc();
				friend_item.m_data_area[tabledef::FRIENDS_ROLE_ID].vint = role_id;
				friend_item.m_data_area[tabledef::FRIENDS_FRIEND_UID].vint = friends.friend_list[i].friend_uid;
				friend_item.m_data_area[tabledef::FRIENDS_FOR_INDEX].vint64 = friend_for_index;
				friend_item.m_data_area[tabledef::FRIENDS_INTIMACY].vint = friends.friend_list[i].intimacy;
				friend_item.m_data_area[tabledef::FRIENDS_TEAM_TODAY_INTIMACY].vint = friends.friend_list[i].team_today_intimacy;
				friend_item.m_data_area[tabledef::FRIENDS_GIFT_COUNT].vint = friends.friend_list[i].gift_count;
				friend_item.m_data_area[tabledef::FRIENDS_FRIEND_INDEX].vint = friends.friend_list[i].index;
				friend_item.m_data_area[tabledef::FRIENDS_FAVORABLE_IMPRESSION].vint = friends.friend_list[i].favorable_impression;
				friend_item.m_data_area[tabledef::FRIENDS_DAY_CHAT_COUNT].vint = friends.friend_list[i].day_chat_count;
				friend_item.m_data_area[tabledef::FRIENDS_ADD_TIMESTAMP].vint64 = friends.friend_list[i].add_timestamp;
			
				if(DBCommand::RESULT_SUC != friendcommand->Save(stmt, &friend_item))
				{
					ret = -3;
				}
				friend_item.Free();
			}
			break;
		}
	}

	return ret < 0 ? ret : 0;
}

int RMIRoleObject::SaveBlacklists(IStatement *stmt, int role_id, const BlacklistsParam &blacklists_param)
{
	DBCommand *blacklists_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_BLACK_LISTS);

	int ret = 0;
	for (int i = 0; i < blacklists_param.count; ++i)
	{
		if (blacklists_param.black_list[i].change_state == structcommon::CS_NONE) continue;

		long long blacklists_for_index = TranslateInt64(role_id, blacklists_param.black_list[i].index);

		switch (blacklists_param.black_list[i].change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_list;

				CmpNode for_index_node(tabledef::BLACK_LISTS_FOR_INDEX);
				for_index_node.SetLong64(blacklists_for_index);
				cmp_list.push_back(for_index_node);

				int count = blacklists_command->Remove(stmt, cmp_list);
				if(count < 1)
				{
					ret = -1;
				}

			}
			break;

		case structcommon::CS_UPDATE:
			{
				DataNodeList nodelist;
				nodelist.reserve(tabledef::TABLE_BLACK_LISTS_MAXINDEX - 4);

				DataNode blacklist_uid(tabledef::BLACK_LISTS_BLACK_UID);
				blacklist_uid.SetInt32(blacklists_param.black_list[i].uid);
				nodelist.push_back(blacklist_uid);

				DataNode hate_gamename(tabledef::BLACK_LISTS_BLACK_NAME);
				hate_gamename.SetString(blacklists_param.black_list[i].gamename);
				nodelist.push_back(hate_gamename);

				DataNode hate_sex(tabledef::BLACK_LISTS_SEX);
				hate_sex.SetByte(blacklists_param.black_list[i].sex);
				nodelist.push_back(hate_sex);

				DataNode hate_prof(tabledef::BLACK_LISTS_PROF);
				hate_prof.SetInt32(blacklists_param.black_list[i].prof);
				nodelist.push_back(hate_prof);

				CmpNodeList cmp_list;

				CmpNode for_index_node(tabledef::BLACK_LISTS_FOR_INDEX);
				for_index_node.SetLong64(blacklists_for_index);
				cmp_list.push_back(for_index_node);

				int count = blacklists_command->Update(stmt, cmp_list, nodelist);

				if (count < 1)
				{
					ret = -2;		// 此时不break，update不成功时则继续insert，以兼容某些错误
				}
				else
				{
					break;
				}
			}
		case structcommon::CS_INSERT:
			{
				const MataTableDef *table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_BLACK_LISTS);

				DataAdapter	item = table->GetPrototype();
				item.Malloc();
				item.m_data_area[tabledef::BLACK_LISTS_ROLE_ID].vint = role_id;
				item.m_data_area[tabledef::BLACK_LISTS_BLACK_INDEX].vint = blacklists_param.black_list[i].index;
				item.m_data_area[tabledef::BLACK_LISTS_FOR_INDEX].vint64 = blacklists_for_index;
				item.m_data_area[tabledef::BLACK_LISTS_BLACK_UID].vint = blacklists_param.black_list[i].uid;
				STRNCPY(item.m_data_area[tabledef::BLACK_LISTS_BLACK_NAME].vcharp, blacklists_param.black_list[i].gamename,
					item.m_data_area[tabledef::BLACK_LISTS_BLACK_NAME].length);
				item.m_data_area[tabledef::BLACK_LISTS_SEX].vchar = blacklists_param.black_list[i].sex;
				item.m_data_area[tabledef::BLACK_LISTS_PROF].vint = blacklists_param.black_list[i].prof;

				if (DBCommand::RESULT_SUC != blacklists_command->Save(stmt, &item))
				{
					ret = -3;
				}

				item.Free();
			}
			break;
		}
	}

	return ret < 0 ? ret : 0;
}

int RMIRoleObject::SaveTableEnemy(IStatement *stmt, int role_id, const EnemyListParam &enemy)
{
	DBCommand *enemycomand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ENEMY);

	int ret = 0;
	for(int i = 0; i < enemy.count; ++i)
	{
		if(enemy.enemy_list[i].change_state == structcommon::CS_NONE) continue;

		long long enemy_for_index = TranslateInt64(role_id,enemy.enemy_list[i].index);
		switch(enemy.enemy_list[i].change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_list;

				CmpNode for_index_node(tabledef::ENEMY_FOR_INDEX);
				for_index_node.SetLong64(enemy_for_index);
				cmp_list.push_back(for_index_node);

				int count = enemycomand->Remove(stmt, cmp_list);
				if(count < 1)
				{
					//return -1;
					ret = -1;
				}
			}
			break;
		case structcommon::CS_UPDATE:
			{
				DataNodeList enemy_list;
				enemy_list.reserve(tabledef::TABLE_ENEMY_MAXINDEX - 4);

				DataNode enemy_item_enemy_uid(tabledef::ENEMY_ENEMY_UID);
				enemy_item_enemy_uid.SetInt32(enemy.enemy_list[i].enemy_uid);
				enemy_list.push_back(enemy_item_enemy_uid);

				DataNode enemy_item_enemy_plat_type(tabledef::ENEMY_ENEMY_PLAT_TYPE);
				enemy_item_enemy_plat_type.SetInt32(enemy.enemy_list[i].enemy_plat_type);
				enemy_list.push_back(enemy_item_enemy_plat_type);

				DataNode enemy_item_enemy_kill_count(tabledef::ENEMY_KILL_COUNT);
				enemy_item_enemy_kill_count.SetInt32(enemy.enemy_list[i].kill_count);
				enemy_list.push_back(enemy_item_enemy_kill_count);

				DataNode enemy_item_last_kill_time(tabledef::ENEMY_LAST_KILL_TIME);
				enemy_item_last_kill_time.SetTime(enemy.enemy_list[i].last_kill_time);
				enemy_list.push_back(enemy_item_last_kill_time);

				DataNode enemy_item_enemy_be_kill_count(tabledef::ENEMY_BE_KILL_COUNT);
				enemy_item_enemy_be_kill_count.SetInt32(enemy.enemy_list[i].be_kill_count);
				enemy_list.push_back(enemy_item_enemy_be_kill_count);

				CmpNodeList cmp_list;

				CmpNode for_index_node(tabledef::ENEMY_FOR_INDEX);
				for_index_node.SetLong64(enemy_for_index);
				cmp_list.push_back(for_index_node);

				int count = enemycomand->Update(stmt, cmp_list, enemy_list);

				if(count < 1)
				{
					ret = -2;
					// 此时不break，update不成功时则继续insert，以兼容某些错误
				}
				else
				{
					break;
				}
			}
			break;
		case structcommon::CS_INSERT:
			{
				const MataTableDef *enemytable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ENEMY);
				DataAdapter enemy_item = enemytable->GetPrototype();
				enemy_item.Malloc();
				enemy_item.m_data_area[tabledef::ENEMY_ROLE_ID].vint = role_id;
				enemy_item.m_data_area[tabledef::ENEMY_FOR_INDEX].vint64 = enemy_for_index;
				enemy_item.m_data_area[tabledef::ENEMY_ENEMY_UID].vint = enemy.enemy_list[i].enemy_uid;
				enemy_item.m_data_area[tabledef::ENEMY_ENEMY_PLAT_TYPE].vint = enemy.enemy_list[i].enemy_plat_type;
				enemy_item.m_data_area[tabledef::ENEMY_KILL_COUNT].vint = enemy.enemy_list[i].kill_count;
				enemy_item.m_data_area[tabledef::ENEMY_LAST_KILL_TIME].vtime =  enemy.enemy_list[i].last_kill_time;
				enemy_item.m_data_area[tabledef::ENEMY_ENEMY_INDEX].vint =  enemy.enemy_list[i].index;
				enemy_item.m_data_area[tabledef::ENEMY_BE_KILL_COUNT].vint = enemy.enemy_list[i].be_kill_count;

				if(DBCommand::RESULT_SUC != enemycomand->Save(stmt, &enemy_item))
				{
					ret = -3;
				}
				enemy_item.Free();
			}
			break;
		}
	}

	return ret < 0 ? ret : 0;
}

int RMIRoleObject::SaveRoleSkill(IStatement *stmt, int role_id, const SkillListParam &skill_param)
{
	const MataTableDef *rollskilltable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLESKILL);
	DBCommand *roleskillcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLESKILL);

	int ret = 0;
	for (int i = 0; i < skill_param.count; ++i)
	{
		if (skill_param.skill_list[i].change_state == structcommon::CS_NONE) continue;

		long long skill_for_index = TranslateInt64(role_id, (int)skill_param.skill_list[i].index);
		
		switch (skill_param.skill_list[i].change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_list;

				CmpNode for_index_node(tabledef::ROLESKILL_FOR_INDEX);
				for_index_node.SetLong64(skill_for_index);
				cmp_list.push_back(for_index_node);

				int count = roleskillcommand->Remove(stmt, cmp_list);
				if (count < 1)
				{
					// return -1;
					ret = -1;
				}
			}
			break;

		case structcommon::CS_UPDATE:
			{
				DataNodeList skill_item_list;
				skill_item_list.reserve(tabledef::TABLE_ROLESKILL_MAXINDEX - 4);

				DataNode skill_id(tabledef::ROLESKILL_SKILL_ID);
				skill_id.SetInt32(skill_param.skill_list[i].skill_id);
				skill_item_list.push_back(skill_id);

				DataNode skill_level(tabledef::ROLESKILL_SKILL_LEVEL);
				skill_level.SetInt32(skill_param.skill_list[i].level);
				skill_item_list.push_back(skill_level);

				DataNode skill_last_perform_time(tabledef::ROLESKILL_LAST_PERFORM_TIME);
				skill_last_perform_time.SetLong64(skill_param.skill_list[i].last_perform_time);
				skill_item_list.push_back(skill_last_perform_time);

				DataNode skill_expl(tabledef::ROLESKILL_SKILL_EXP);
				skill_expl.SetInt32(skill_param.skill_list[i].exp);
				skill_item_list.push_back(skill_expl);

				CmpNodeList condition;

				CmpNode for_index_node(tabledef::ROLESKILL_FOR_INDEX);
				for_index_node.SetLong64(skill_for_index);
				condition.push_back(for_index_node);

				int count = roleskillcommand->Update(stmt, condition, skill_item_list);
				if (count < 1)
				{
					ret = -2;  // 此时不break，update不成功时则继续insert，以兼容某些错误
				}
				else
				{
					break;
				}
			}

		case structcommon::CS_INSERT:
			{
				DataAdapter skill_index_item = rollskilltable->GetPrototype();
				skill_index_item.Malloc();

				skill_index_item.m_data_area[tabledef::ROLESKILL_ROLE_ID].vint = role_id;
				skill_index_item.m_data_area[tabledef::ROLESKILL_SKILL_INDEX].vint = (int)skill_param.skill_list[i].index;
				skill_index_item.m_data_area[tabledef::ROLESKILL_FOR_INDEX].vint64 = skill_for_index;
				skill_index_item.m_data_area[tabledef::ROLESKILL_SKILL_ID].vint = (int)skill_param.skill_list[i].skill_id;
				skill_index_item.m_data_area[tabledef::ROLESKILL_SKILL_LEVEL].vint = skill_param.skill_list[i].level;
				skill_index_item.m_data_area[tabledef::ROLESKILL_LAST_PERFORM_TIME].vint64 = skill_param.skill_list[i].last_perform_time;
				skill_index_item.m_data_area[tabledef::ROLESKILL_SKILL_EXP].vint = skill_param.skill_list[i].exp;

				if (DBCommand::RESULT_SUC != roleskillcommand->Save(stmt, &skill_index_item))
				{
					ret = -1;
				}

				skill_index_item.Free();
			}
			break;

		default:
			break;
		}
	}

	return ret;
}

int RMIRoleObject::SaveDailyFind(IStatement *stmt, int role_id, const DailyFindListParam &daily_find_param)
{
	const MataTableDef *dailyfindtable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_DAILYFIND);
	DBCommand *dailyfindcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_DAILYFIND);

	int ret = 0;
	for (int i = 0; i < daily_find_param.count && i < DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS; ++i)
	{
		if (daily_find_param.daily_find_list[i].change_state == structcommon::CS_NONE) continue;

		long long dailyfind_for_index = TranslateInt64(role_id, (int)daily_find_param.daily_find_list[i].index);

		switch (daily_find_param.daily_find_list[i].change_state)
		{
		case structcommon::CS_DELETE:
			{
				CmpNodeList cmp_list;

				CmpNode for_index_node(tabledef::DAILYFIND_FOR_INDEX);
				for_index_node.SetLong64(dailyfind_for_index);
				cmp_list.push_back(for_index_node);

				int count = dailyfindcommand->Remove(stmt, cmp_list);
				if (count < 1)
				{
					// return -1;
					ret = -1;
				}
			}
			break;

		case structcommon::CS_UPDATE:
			{
				DataNodeList dailyfind_item_list;
				dailyfind_item_list.reserve(tabledef::TABLE_DAILYFIND_MAXINDEX - 4);

				DataNode find_time(tabledef::DAILYFIND_FIND_TIME);
				find_time.SetInt32(daily_find_param.daily_find_list[i].find_time);
				dailyfind_item_list.push_back(find_time);

				DataNode role_level(tabledef::DAILYFIND_ROLE_LEVEL);
				role_level.SetInt32(daily_find_param.daily_find_list[i].role_level);
				dailyfind_item_list.push_back(role_level);

				DataNode find_count(tabledef::DAILYFIND_FIND_COUNT);
				find_count.SetInt32(daily_find_param.daily_find_list[i].find_count);
				dailyfind_item_list.push_back(find_count);

				DataNode param(tabledef::DAILYFIND_PARAM);
				param.SetInt32(daily_find_param.daily_find_list[i].param);
				dailyfind_item_list.push_back(param);

				DataNode param1(tabledef::DAILYFIND_PARAM1);
				param1.SetLong64(daily_find_param.daily_find_list[i].param1);
				dailyfind_item_list.push_back(param1);

				CmpNodeList condition;

				CmpNode for_index_node(tabledef::DAILYFIND_FOR_INDEX);
				for_index_node.SetLong64(dailyfind_for_index);
				condition.push_back(for_index_node);

				int count = dailyfindcommand->Update(stmt, condition, dailyfind_item_list);
				if (count < 1)
				{
					ret = -2;  // 此时不break，update不成功时则继续insert，以兼容某些错误
				}
				else
				{
					break;
				}
			}

		case structcommon::CS_INSERT:
			{
				DataAdapter dailyfind_item = dailyfindtable->GetPrototype();
				dailyfind_item.Malloc();

				dailyfind_item.m_data_area[tabledef::DAILYFIND_ROLE_ID].vint = role_id;
				dailyfind_item.m_data_area[tabledef::DAILYFIND_FOR_INDEX].vint64 = dailyfind_for_index;
				dailyfind_item.m_data_area[tabledef::DAILYFIND_FIND_INDEX].vint = daily_find_param.daily_find_list[i].index;
				dailyfind_item.m_data_area[tabledef::DAILYFIND_FIND_TIME].vint = (int)daily_find_param.daily_find_list[i].find_time;
				dailyfind_item.m_data_area[tabledef::DAILYFIND_ROLE_LEVEL].vint = daily_find_param.daily_find_list[i].role_level;
				dailyfind_item.m_data_area[tabledef::DAILYFIND_PARAM].vint = daily_find_param.daily_find_list[i].param;
				dailyfind_item.m_data_area[tabledef::DAILYFIND_PARAM1].vint64 = daily_find_param.daily_find_list[i].param1;
				dailyfind_item.m_data_area[tabledef::DAILYFIND_FIND_COUNT].vint = daily_find_param.daily_find_list[i].find_count;

				if (DBCommand::RESULT_SUC != dailyfindcommand->Save(stmt, &dailyfind_item))
				{
					ret = -1;
				}

				dailyfind_item.Free();
			}
			break;

		default:
			break;
		}
	}

	return ret;
}

int RMIRoleObject::LoadRoleToCache(IStatement *stmt, int role_id)
{
	{
		// 角色
		const MataTableDef *roletable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE);
		DBCommand *rolecommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE);
		CacheBase *rolecache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE);

		QueryCondition condition_role;
		CmpNode node_role_loe(tabledef::ROLE_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role.cmp_list.push_back(node_role_loe);

		int ret = -1;
		DataAdapter role_attr = roletable->GetPrototype();
		role_attr.Malloc();

		if (DBCommand::RESULT_SUC == rolecommand->Find(stmt, condition_role, &role_attr, false, false))
		{
			ret = 0;
			rolecache->Insert(role_attr);
		}

		role_attr.Free();

		if (ret < 0) return ret;
	}

	{
		// 新的角色属性
		const MataTableDef *new_role_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL);
		DBCommand *new_role_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL);
		CacheBase *new_role_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		int ret = -1;
		DataAdapter new_data_role_attr = new_role_table->GetPrototype();
		new_data_role_attr.Malloc();

		if (DBCommand::RESULT_SUC == new_role_command->Find(stmt, condition_role_low, 
			&new_data_role_attr, false, false))
		{
			ret = 0;
			new_role_cache->Insert(new_data_role_attr);
		}

		new_data_role_attr.Free();

		if (ret < 0) return ret;
	}

	{
		// 新的角色属性2
		const MataTableDef *new_roleatt2_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL2);
		DBCommand *new_roleattr2_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL2);
		CacheBase *new_roleattr2_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL2);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL2_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		int ret = -1;
		DataAdapter new_data_role_attr2 = new_roleatt2_table->GetPrototype();
		new_data_role_attr2.Malloc();

		if (DBCommand::RESULT_SUC == new_roleattr2_command->Find(stmt, condition_role_low, 
			&new_data_role_attr2, false, false))
		{
			ret = 0;
			new_roleattr2_cache->Insert(new_data_role_attr2);
		}

		new_data_role_attr2.Free();

		if (ret < 0) return ret;
	}

	{
		// 新的角色属性3
		const MataTableDef *new_roleatt3_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL3);
		DBCommand *new_roleattr3_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL3);
		CacheBase *new_roleattr3_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL3);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL3_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		int ret = -1;
		DataAdapter new_data_role_attr3 = new_roleatt3_table->GetPrototype();
		new_data_role_attr3.Malloc();

		if (DBCommand::RESULT_SUC == new_roleattr3_command->Find(stmt, condition_role_low, 
			&new_data_role_attr3, false, false))
		{
			ret = 0;
			new_roleattr3_cache->Insert(new_data_role_attr3);
		}

		new_data_role_attr3.Free();

		if (ret < 0) return ret;
	}

	{
		// 新的角色属性4
		const MataTableDef *new_roleatt4_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL4);
		DBCommand *new_roleattr4_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL4);
		CacheBase *new_roleattr4_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL4);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL4_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		int ret = -1;
		DataAdapter new_data_role_attr4 = new_roleatt4_table->GetPrototype();
		new_data_role_attr4.Malloc();

		if (DBCommand::RESULT_SUC == new_roleattr4_command->Find(stmt, condition_role_low, 
			&new_data_role_attr4, false, false))
		{
			ret = 0;
			new_roleattr4_cache->Insert(new_data_role_attr4);
		}

		new_data_role_attr4.Free();

		if (ret < 0) return ret;
	}

	{
		// 新的角色属性5
		const MataTableDef *new_roleatt5_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL5);
		DBCommand *new_roleattr5_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL5);
		CacheBase *new_roleattr5_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL5);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL5_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		int ret = -1;
		DataAdapter new_data_role_attr5 = new_roleatt5_table->GetPrototype();
		new_data_role_attr5.Malloc();

		if (DBCommand::RESULT_SUC == new_roleattr5_command->Find(stmt, condition_role_low, 
			&new_data_role_attr5, false, false))
		{
			ret = 0;
			new_roleattr5_cache->Insert(new_data_role_attr5);
		}

		new_data_role_attr5.Free();

		if (ret < 0) return ret;
	}

	{
		// 新的角色属性6
		const MataTableDef *new_roleatt6_table = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL6);
		DBCommand *new_roleattr6_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL6);
		CacheBase *new_roleattr6_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL6);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL6_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		int ret = -1;
		DataAdapter new_data_role_attr6 = new_roleatt6_table->GetPrototype();
		new_data_role_attr6.Malloc();

		if (DBCommand::RESULT_SUC == new_roleattr6_command->Find(stmt, condition_role_low,
			&new_data_role_attr6, false, false))
		{
			ret = 0;
			new_roleattr6_cache->Insert(new_data_role_attr6);
		}

		new_data_role_attr6.Free();

		if (ret < 0) return ret;
	}

	{
		// 新的角色属性7
		const MataTableDef *mata_table_def = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL7);
		DBCommand *db_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL7);
		CacheBase *cache_base = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL7);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL7_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		int ret = -7;
		DataAdapter new_data_role_attr7 = mata_table_def->GetPrototype();
		new_data_role_attr7.Malloc();

		if (DBCommand::RESULT_SUC == db_command->Find(stmt, condition_role_low,
			&new_data_role_attr7, false, false))
		{
			ret = 0;
			cache_base->Insert(new_data_role_attr7);
		}

		new_data_role_attr7.Free();

		if (ret < 0) return ret;
	}

	{
		// 新的角色属性8
		const MataTableDef *mata_table_def = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL8);
		DBCommand *db_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL8);
		CacheBase *cache_base = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL8);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL8_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		int ret = -8;
		DataAdapter new_data_role_attr8 = mata_table_def->GetPrototype();
		new_data_role_attr8.Malloc();

		if (DBCommand::RESULT_SUC == db_command->Find(stmt, condition_role_low,
			&new_data_role_attr8, false, false))
		{
			ret = 0;
			cache_base->Insert(new_data_role_attr8);
		}

		new_data_role_attr8.Free();

		if (ret < 0) return ret;
	}

	{
		// 新的角色属性9
		const MataTableDef *mata_table_def = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL9);
		DBCommand *db_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL9);
		CacheBase *cache_base = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL9);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL9_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		int ret = -9;
		DataAdapter new_data_role_attr9 = mata_table_def->GetPrototype();
		new_data_role_attr9.Malloc();

		if (DBCommand::RESULT_SUC == db_command->Find(stmt, condition_role_low,
			&new_data_role_attr9, false, false))
		{
			ret = 0;
			cache_base->Insert(new_data_role_attr9);
		}

		new_data_role_attr9.Free();

		if (ret < 0) return ret;
	}

	{
		// 新的角色属性10
		const MataTableDef *mata_table_def = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL10);
		DBCommand *db_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL10);
		CacheBase *cache_base = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL10);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL10_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		int ret = -10;
		DataAdapter new_data_role_attr10 = mata_table_def->GetPrototype();
		new_data_role_attr10.Malloc();

		if (DBCommand::RESULT_SUC == db_command->Find(stmt, condition_role_low,
			&new_data_role_attr10, false, false))
		{
			ret = 0;
			cache_base->Insert(new_data_role_attr10);
		}

		new_data_role_attr10.Free();

		if (ret < 0) return ret;
	}

	{
		// 背包
		DBCommand *knapsackcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ITEMLIST);
		CacheBase *knapsackcache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ITEMLIST);

		QueryCondition knapsackcondition;
		CmpNode knapsacknode1(tabledef::ITEMLIST_ROLE_ID);
		knapsacknode1.SetInt32(role_id);
		knapsackcondition.cmp_list.push_back(knapsacknode1);

		DataAdapterList knapsack_list;
		int ret = knapsackcommand->Find(stmt, knapsackcondition, &knapsack_list, false, false);
		if (ret < 0) return ret;

		for (DataAdapterList::iterator i = knapsack_list.begin(); i != knapsack_list.end();++i)
		{
			knapsackcache->Insert(*i);
			i->Free();
		}
	}

	{
	}

	{	
	}

	{
		//快捷键
		DBCommand *hotkeycommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_HOTKEY);
		CacheBase *hotkeycache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_HOTKEY);

		QueryCondition hotkeycondition;

		CmpNode hotkeynode1(tabledef::HOTKEY_ROLE_ID);
		hotkeynode1.SetInt32(role_id);
		hotkeycondition.cmp_list.push_back(hotkeynode1);

		DataAdapterList hotkey_list;
		int ret = hotkeycommand->Find(stmt, hotkeycondition, &hotkey_list, false, false);
		if(ret < 0) return ret;

		for(DataAdapterList::iterator i = hotkey_list.begin(); i != hotkey_list.end(); ++i)
		{
			hotkeycache->Insert(*i);
			i->Free();
		}
	}

	{
		//好友
		DBCommand *friendcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_FRIENDS);
		CacheBase *friendcache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_FRIENDS);

		QueryCondition friendcontion;

		CmpNode friendnode1(tabledef::FRIENDS_ROLE_ID);
		friendnode1.SetInt32(role_id);
		friendcontion.cmp_list.push_back(friendnode1);

		DataAdapterList friend_list;
		int ret = friendcommand->Find(stmt,friendcontion,&friend_list, false, false);
		if(ret < 0) return ret;

		for(DataAdapterList::iterator i = friend_list.begin(); i != friend_list.end(); ++i)
		{
			friendcache->Insert(*i);
			i->Free();
		}
	}

	{
		// 黑名单
		DBCommand *blacklists_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_BLACK_LISTS);
		CacheBase *blacklists_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_BLACK_LISTS);

		QueryCondition blacklists_contion;

		CmpNode blacklists_node1(tabledef::BLACK_LISTS_ROLE_ID);
		blacklists_node1.SetInt32(role_id);
		blacklists_contion.cmp_list.push_back(blacklists_node1);

		DataAdapterList blacklists_list;
		int ret = blacklists_command->Find(stmt, blacklists_contion, &blacklists_list, false, false);
		if (ret < 0) return ret;

		for (DataAdapterList::iterator i = blacklists_list.begin(); i != blacklists_list.end(); ++i)
		{
			blacklists_cache->Insert(*i);
			i->Free();
		}
	}

	{
		//敌人
		DBCommand *enemycommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ENEMY);
		CacheBase *enemycache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ENEMY);

		QueryCondition	enemycontion;
		CmpNode enemynode1(tabledef::ENEMY_ROLE_ID);
		enemynode1.SetInt32(role_id);
		enemycontion.cmp_list.push_back(enemynode1);

		DataAdapterList enemylist;
		int ret = enemycommand->Find(stmt,enemycontion, &enemylist, false, false);
		if(ret < 0) return ret;

		for(DataAdapterList::iterator i = enemylist.begin();i != enemylist.end(); ++i)
		{
			enemycache->Insert(*i);
			i->Free();	
		}
	}

	{
		// 技能
		DBCommand *skillcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLESKILL);
		CacheBase *skillcache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLESKILL);

		QueryCondition skillcondition;
		CmpNode skillnode1(tabledef::ROLESKILL_ROLE_ID);
		skillnode1.SetInt32(role_id);
		skillcondition.cmp_list.push_back(skillnode1);

		DataAdapterList skill_list;
		int ret = skillcommand->Find(stmt, skillcondition, &skill_list, false, false);
		if (ret < 0) return ret;

		for (DataAdapterList::iterator i = skill_list.begin(); i != skill_list.end(); ++i)
		{
			skillcache->Insert(*i);
			i->Free();
		}
	}

	{
		// 日常找回
		DBCommand *dailyfindcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_DAILYFIND);
		CacheBase *dailyfindcache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_DAILYFIND);

		QueryCondition dailyfind_condition;
		CmpNode dailyfind_node1(tabledef::DAILYFIND_ROLE_ID);
		dailyfind_node1.SetInt32(role_id);
		dailyfind_condition.cmp_list.push_back(dailyfind_node1);

		DataAdapterList dailyfind_list;
		int ret = dailyfindcommand->Find(stmt, dailyfind_condition, &dailyfind_list, false, false);
		if (ret < 0) return ret;

		for (DataAdapterList::iterator i = dailyfind_list.begin(); i != dailyfind_list.end(); ++i)
		{
			dailyfindcache->Insert(*i);
			i->Free();
		}
	}

	return 0;
}

int RMIRoleObject::RemoveRoleFromCache(int role_id)
{
	{
		// 角色
		CacheBase *rolecache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE);
		
		QueryCondition condition_role;
		CmpNode node_role_loe(tabledef::ROLE_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role.cmp_list.push_back(node_role_loe);

		rolecache->RemoveCache(condition_role.cmp_list);
	}

	{
		CacheBase *new_role_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL);
		
		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		new_role_cache->RemoveCache(condition_role_low.cmp_list);
	}

	{
		CacheBase *new_role_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL2);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL2_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		new_role_cache->RemoveCache(condition_role_low.cmp_list);
	}

	{
		CacheBase *new_role_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL3);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL3_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		new_role_cache->RemoveCache(condition_role_low.cmp_list);
	}

	{
		CacheBase *new_role_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL4);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL4_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		new_role_cache->RemoveCache(condition_role_low.cmp_list);
	}

	{
		CacheBase *new_role_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL5);
		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL5_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		new_role_cache->RemoveCache(condition_role_low.cmp_list);
	}

	{
		CacheBase *new_role_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL6);
		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL6_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		new_role_cache->RemoveCache(condition_role_low.cmp_list);
	}

	{
		CacheBase *new_role_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL7);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL7_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		new_role_cache->RemoveCache(condition_role_low.cmp_list);
	}

	{
		CacheBase *new_role_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL8);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL8_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		new_role_cache->RemoveCache(condition_role_low.cmp_list);
	}

	{
		CacheBase *new_role_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL9);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL9_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		new_role_cache->RemoveCache(condition_role_low.cmp_list);
	}

	{
		CacheBase *new_role_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLE_ATTR_DETAIL10);

		QueryCondition condition_role_low;
		CmpNode node_role_loe(tabledef::ROLE_ATTR_DETAIL10_ROLE_ID);
		node_role_loe.SetInt32(role_id);
		condition_role_low.cmp_list.push_back(node_role_loe);

		new_role_cache->RemoveCache(condition_role_low.cmp_list);
	}

	{
		CacheBase *knapsackcache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ITEMLIST);

		QueryCondition knapsackcondition;
		CmpNode knapsacknode1(tabledef::ITEMLIST_ROLE_ID);
		knapsacknode1.SetInt32(role_id);
		knapsackcondition.cmp_list.push_back(knapsacknode1);

		knapsackcache->RemoveCache(knapsackcondition.cmp_list);
	}

	{
		CacheBase *hotkeycache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_HOTKEY);

		QueryCondition hotkeycondition;
		CmpNode hotkeynode1(tabledef::HOTKEY_ROLE_ID);
		hotkeynode1.SetInt32(role_id);
		hotkeycondition.cmp_list.push_back(hotkeynode1);

		hotkeycache->RemoveCache(hotkeycondition.cmp_list);
	}

	{
		CacheBase *friendcache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_FRIENDS);

		QueryCondition friendcontion;
		CmpNode friendnode1(tabledef::FRIENDS_ROLE_ID);
		friendnode1.SetInt32(role_id);
		friendcontion.cmp_list.push_back(friendnode1);

		friendcache->RemoveCache(friendcontion.cmp_list);
	}

	{
		CacheBase *blacklists_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_BLACK_LISTS);

		QueryCondition blacklists_contion;
		CmpNode blacklists_node1(tabledef::BLACK_LISTS_ROLE_ID);
		blacklists_node1.SetInt32(role_id);
		blacklists_contion.cmp_list.push_back(blacklists_node1);

		blacklists_cache->RemoveCache(blacklists_contion.cmp_list);
	}

	{
		CacheBase *enemycache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ENEMY);

		QueryCondition	enemycontion;
		CmpNode enemynode1(tabledef::ENEMY_ROLE_ID);
		enemynode1.SetInt32(role_id);
		enemycontion.cmp_list.push_back(enemynode1);

		enemycache->RemoveCache(enemycontion.cmp_list);
	}

	{
		CacheBase *skill_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_ROLESKILL);

		QueryCondition roleskill_condition;
		CmpNode roleskill_node(tabledef::ROLESKILL_ROLE_ID);
		roleskill_node.SetInt32(role_id);
		roleskill_condition.cmp_list.push_back(roleskill_node);

		skill_cache->RemoveCache(roleskill_condition.cmp_list);
	}

	{
		CacheBase *dailyfind_cache = DataBaseLayer::Instance().GetCache(tabledef::TABLE_DAILYFIND);

		QueryCondition dailyfind_condition;
		CmpNode dailyfind_node(tabledef::DAILYFIND_ROLE_ID);
		dailyfind_node.SetInt32(role_id);
		dailyfind_condition.cmp_list.push_back(dailyfind_node);

		dailyfind_cache->RemoveCache(dailyfind_condition.cmp_list);
	}

	return 0;
}

int RMIRoleObject::SaveTableRoleStatus(int role_id, bool is_logout, bool is_micro_pc)
{
	if (IsCrossUser(IntToUid(role_id)))
	{
		if (CrossRoleCacheManager::Instance().SaveTableRoleStatus(role_id, is_logout))
		{
			return 0;
		}

		return -1;
	}

	INIT_STMT(conn, stmt, -1);

	conn->begin();

	DBCommand *rolecommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE);

	DataNodeList role_update;
	role_update.reserve(tabledef::TABLE_ROLE_MAXINDEX - 1);

	DataNode last_save_time(tabledef::ROLE_LAST_SAVE_TIME);
	last_save_time.SetLong64(EngineAdapter::Instance().Time());
	role_update.push_back(last_save_time);

	DataNode is_online_node(tabledef::ROLE_IS_ONLINE);
	is_online_node.SetByte(is_logout ? 0 : 1);
	role_update.push_back(is_online_node);

	DataNode is_micro_pc_node(tabledef::ROLE_IS_MICRO_PC);
	is_micro_pc_node.SetByte(is_micro_pc ? 1 : 0);
	role_update.push_back(is_micro_pc_node);

	CmpNodeList condition;
	CmpNode id_node(tabledef::ROLE_ROLE_ID);
	id_node.SetInt32(role_id);
	condition.push_back(id_node);

	int count = rolecommand->Update(stmt, condition, role_update);
	if (count < 1)
	{
		conn->rollback();
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "SaveTableRoleStatus rolecommand->Update uid:%d", role_id);
		return -2;
	}

	conn->commit();

	return 0;
}
