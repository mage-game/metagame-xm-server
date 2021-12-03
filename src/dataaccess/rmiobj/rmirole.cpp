#include "rmirole.h"
#include "rmidef.hpp"

#include "servercommon/servercommon.h"
#include "servercommon/darmi/darole.h"

#include "databaselayer.h"

#include "servercommon/struct/roleinitparam.h"
#include "servercommon/struct/roleotherinitparam.hpp"
#include "servercommon/struct/skilllistparam.h"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/struct/rolequeryparam.hpp"

#include "servercommon/struct/hotkeylistparam.h"
#include "servercommon/struct/friendparam.h"
#include "servercommon/struct/enemyparam.h"
#include "servercommon/struct/businessparam.h"
#include "servercommon/struct/blacklistsparam.h"

#include "servercommon/struct/mountparam.hpp"
#include "servercommon/struct/dailyfindlistparam.hpp"
#include "servercommon/jinglingdef.hpp"

#include "common/bintohex.h"

#include "servercommon/roleattrvermanager/roleattrvermanager.h"

#include "ilogmodule.h"
#include "dataaccesslog.hpp"

#include "rolecachemanager.hpp"
#include "servercommon/packetstream.hpp"
#include "servercommon/serverclmempool.h"
#include "crossrolecache/crossrolecachemanager.hpp"

USE_DATAACCESS_VAR();
OLD_USE_INTERNAL_NETWORK();

extern char g_struct_code_buffer[MAX_STRUCT_CODE_BUFFER_LEN];

RMIRoleObject::RMIRoleObject()
{	
	m_method_list[dataaccessrmi::role::GET_ROLE_INFO] = __GetRoleInfo;
	m_method_list[dataaccessrmi::role::ROLE_INIT] = __RoleInit;
	m_method_list[dataaccessrmi::role::ROLE_SAVE] = __RoleSave;
	m_method_list[dataaccessrmi::role::CREATE_ROLE] = __CreateRole;
	m_method_list[dataaccessrmi::role::DESTROY_ROLE] = __DestroyRole;
	m_method_list[dataaccessrmi::role::ROLE_SAVE_STATUS] = __RoleSaveStatus;
	m_method_list[dataaccessrmi::role::FRIEND_SAVE] = __FriendSave;
	m_method_list[dataaccessrmi::role::ROLE_QUERY] = __RoleQuery;
	m_method_list[dataaccessrmi::role::CREATE_CROSS_ROLE] = __CreateCrossRole;
}

RMIRoleObject::~RMIRoleObject()
{

}

void RMIRoleObject::__free()
{
	delete this;
}

const char * RMIRoleObject::__get_obj_id()
{
	return dataaccessrmi::ROLE;
}

int RMIRoleObject::__GetRoleInfo(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	int num;
	if (!in_param.Pop(&num))
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__GetRoleInfo_1 ");
		return rmi::DispatchParamError;
	}

	int role_id[16] = {0};
	for (int i = 0; i < 16 && i < num; ++i)
	{
		if (!in_param.Pop(&role_id[i])) 
		{
			dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__GetRoleInfo_2 ");
			return rmi::DispatchParamError;
		}
	}

	RoleInfoList info;
	int result = GetRoleInfo(role_id, num, &info);

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__GetRoleInfo_3 result:%d role_id:%d", 
			result, info.role_info_list[0].role_id);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (0 == result)
	{
		if (!info.Serialize(out_param))
		{
			dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__GetRoleInfo_4 result:%d role_id:%d", 
				result, info.role_info_list[0].role_id);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else 
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__GetRoleInfo_5 result:%d", result);
	}

	return rmi::DispatchOK;
}

int RMIRoleObject::GetRoleInfo(int *role_id, int num, RoleInfoList *info)
{
	if (num > 0 && IsCrossUser(IntToUid(role_id[0])))		// 不考虑role_id列表中既有普通角色ID也有跨服角色ID的情况
	{
		if (CrossRoleCacheManager::Instance().GetRoleInfo(role_id, num, info))
		{
			dataaccesslog::g_log_role.printf(LL_DEBUG, "GetCrossRoleInfo uid:%d num:%d count:%d", role_id[0], num, info->count);
			return 0;
		}

		return -1;
	}

	// const MataTableDef *roletable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE);
	DBCommand *command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE);

	INIT_STMT(conn, stmt, -1);

	QueryCondition condition;
	CmpNode node1(tabledef::ROLE_ROLE_ID);
	std::vector<int> idlist;
	for (int i = 0; i < num; ++i)
	{
		idlist.push_back(role_id[i]);
	}
	node1.SetIntSet(idlist);
	condition.cmp_list.push_back(node1);

	DataAdapterList role_list;
	int ret = command->Find(stmt, condition, &role_list);

	if (ret < 0) return ret;

	if (ret < num)
	{
		if (DataBaseLayer::Instance().IsDelayLoad())
		{
			// 启动了延迟加载策略, 则从实际数据库中load
			std::set<int> role_id_set;

			for (DataAdapterList::iterator i = role_list.begin(); i != role_list.end(); ++i)
			{
				role_id_set.insert(i->m_data_area[tabledef::ROLE_ROLE_ID].vint);
				i->Free();
			}
			role_list.clear();

			ret = command->Find(stmt, condition, &role_list, false, false);
			if (ret < 0) return ret;

			if(ret == 0) return -1;

			conn->begin();
			for (DataAdapterList::iterator i = role_list.begin(); i != role_list.end(); ++i)
			{
				if (role_id_set.find(i->m_data_area[tabledef::ROLE_ROLE_ID].vint) == role_id_set.end())
				{
					ret = LoadRoleToCache(stmt, i->m_data_area[tabledef::ROLE_ROLE_ID].vint);
				}
			}
			conn->commit();
		}
		else
		{
			// 不可能没有
			return -1;
		}
	}

	info->count = 0;
	DataAdapterList::iterator i = role_list.begin();

	for (; info->count < RoleInfoList::MAX_ROLE_NUM && i != role_list.end(); ++i, ++info->count)
	{
		RoleInfoList::__roleinfo &role_info = info->role_info_list[info->count];
		role_info.role_id = i->m_data_area[tabledef::ROLE_ROLE_ID].vint;
		STRNCPY(role_info.role_name, i->m_data_area[tabledef::ROLE_ROLE_NAME].vcharp,
			sizeof(info->role_info_list[0].role_name));
		role_info.scene_id = i->m_data_area[tabledef::ROLE_SCENE_ID].vint;
		role_info.last_scene_id = i->m_data_area[tabledef::ROLE_LAST_SCENE_ID].vint;
		role_info.level = i->m_data_area[tabledef::ROLE_LEVEL].vint;
		role_info.professional = i->m_data_area[tabledef::ROLE_PROFESSIONAL].vint;
		role_info.create_time = (unsigned int)i->m_data_area[tabledef::ROLE_CREATE_TIME].vint64;
		role_info.last_login_time = (unsigned int)i->m_data_area[tabledef::ROLE_LAST_SAVE_TIME].vint64;
		role_info.avatar = i->m_data_area[tabledef::ROLE_AVATAR].vchar;
		role_info.sex = i->m_data_area[tabledef::ROLE_SEX].vchar;
		role_info.country = i->m_data_area[tabledef::ROLE_COUNTRY].vchar;

		i->Free();

		{
			// 新角色属性表
			const MataTableDef *role_attr_table_detail = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL);
			DBCommand *role_attr_command_detail = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL);

			QueryCondition condition;
			CmpNode node1(tabledef::ROLE_ATTR_DETAIL_ROLE_ID);
			node1.SetInt32(role_info.role_id);
			condition.cmp_list.push_back(node1);

			DataAdapter data_attr = role_attr_table_detail->GetPrototype();
			data_attr.Malloc();

			if (DBCommand::RESULT_SUC == role_attr_command_detail->Find(stmt, condition, &data_attr))
			{
				// 羽翼
				static WingParam wing_param;
				GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL_WING_DATA, wing_param);
				role_info.wing_used_imageid = wing_param.used_imageid;
			}

			data_attr.Free();
		}

		{
			// 新角色属性表5
			const MataTableDef *role_attr_table_detail5 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL5);
			DBCommand *role_attr_command_detail5 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL5);

			QueryCondition condition;
			CmpNode node1(tabledef::ROLE_ATTR_DETAIL5_ROLE_ID);
			node1.SetInt32(role_info.role_id);
			condition.cmp_list.push_back(node1);

			DataAdapter data_attr = role_attr_table_detail5->GetPrototype();
			data_attr.Malloc();

			if (DBCommand::RESULT_SUC == role_attr_command_detail5->Find(stmt, condition, &data_attr))
			{
				
			}

			data_attr.Free();
		}

		{
			// 新角色属性表6
			const MataTableDef *role_attr_table_detail6 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL6);
			DBCommand *role_attr_command_detail3 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL6);

			QueryCondition condition;
			CmpNode node1(tabledef::ROLE_ATTR_DETAIL6_ROLE_ID);
			node1.SetInt32(role_info.role_id);
			condition.cmp_list.push_back(node1);

			DataAdapter data_attr = role_attr_table_detail6->GetPrototype();
			data_attr.Malloc();

			if (DBCommand::RESULT_SUC == role_attr_command_detail3->Find(stmt, condition, &data_attr))
			{
				// 时装
				ShizhuangParam shizhuang_param;
				GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL6_SHIZHUANG_DATA, shizhuang_param);
				role_info.shizhuang_wuqi_is_special = shizhuang_param.item_list[SHIZHUANG_TYPE_WUQI].is_use_special_img;
				if (role_info.shizhuang_wuqi_is_special)
				{
					role_info.shizhuang_wuqi = shizhuang_param.item_list[SHIZHUANG_TYPE_WUQI].use_special_img;
				}
				else
				{
					role_info.shizhuang_wuqi = shizhuang_param.item_list[SHIZHUANG_TYPE_WUQI].use_img;
				}
				role_info.shizhuang_body_is_special = shizhuang_param.item_list[SHIZHUANG_TYPE_BODY].is_use_special_img;
				if (role_info.shizhuang_body_is_special)
				{
					role_info.shizhuang_body = shizhuang_param.item_list[SHIZHUANG_TYPE_BODY].use_special_img;
				}
				else
				{
					role_info.shizhuang_body = shizhuang_param.item_list[SHIZHUANG_TYPE_BODY].use_img;
				}
			}

			data_attr.Free();
		}

		{
			// 新角色属性表7
			const MataTableDef *role_attr_table_detail7 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL7);
			DBCommand *role_attr_command_detail7 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL7);

			QueryCondition condition;
			CmpNode node1(tabledef::ROLE_ATTR_DETAIL7_ROLE_ID);
			node1.SetInt32(role_info.role_id);
			condition.cmp_list.push_back(node1);

			DataAdapter data_attr = role_attr_table_detail7->GetPrototype();
			data_attr.Malloc();

			if (DBCommand::RESULT_SUC == role_attr_command_detail7->Find(stmt, condition, &data_attr))
			{

			}

			data_attr.Free();
		}

		{
			// 新角色属性表8
			const MataTableDef *role_attr_table_detail8 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL8);
			DBCommand *role_attr_command_detail8 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL8);

			QueryCondition condition;
			CmpNode node1(tabledef::ROLE_ATTR_DETAIL8_ROLE_ID);
			node1.SetInt32(role_info.role_id);
			condition.cmp_list.push_back(node1);

			DataAdapter data_attr = role_attr_table_detail8->GetPrototype();
			data_attr.Malloc();

			if (DBCommand::RESULT_SUC == role_attr_command_detail8->Find(stmt, condition, &data_attr))
			{
				static UpgradeManagerParam upgrade_manager_param;
				GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_UPGRADE_MANAGER_DATA, upgrade_manager_param);
				role_info.upgrade_used_imageid_0 = upgrade_manager_param.data_list[UPGRADE_TYPE_0].used_imageid;
				role_info.upgrade_used_imageid_1 = upgrade_manager_param.data_list[UPGRADE_TYPE_1].used_imageid;
				role_info.upgrade_used_imageid_2 = upgrade_manager_param.data_list[UPGRADE_TYPE_2].used_imageid;
				role_info.upgrade_used_imageid_3 = upgrade_manager_param.data_list[UPGRADE_TYPE_3].used_imageid;
				role_info.upgrade_used_imageid_4 = upgrade_manager_param.data_list[UPGRADE_TYPE_4].used_imageid;
				role_info.upgrade_used_imageid_5 = upgrade_manager_param.data_list[UPGRADE_TYPE_5].used_imageid;
				role_info.upgrade_used_imageid_6 = upgrade_manager_param.data_list[UPGRADE_TYPE_6].used_imageid;
				role_info.upgrade_used_imageid_7 = upgrade_manager_param.data_list[UPGRADE_TYPE_7].used_imageid;
				role_info.upgrade_used_imageid_8 = upgrade_manager_param.data_list[UPGRADE_TYPE_8].used_imageid;

				// 神器
				static ShenqiParam shenqi_param;
				GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_SHENQI_DATA, shenqi_param);
				role_info.shenbing_used_img_id = shenqi_param.shenbing_cur_image_id;
				role_info.shenbing_texiao_id = shenqi_param.shenbing_cur_texiao_id;
				role_info.baojia_used_img_id = shenqi_param.baojia_cur_image_id;
				role_info.baojia_texiao_id = shenqi_param.baojia_cur_texiao_id;
			}

			data_attr.Free();
		}

		{
			// 新角色属性表9
			const MataTableDef *role_attr_table_detail9 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL9);
			DBCommand *role_attr_command_detail9 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL9);

			QueryCondition condition;
			CmpNode node1(tabledef::ROLE_ATTR_DETAIL9_ROLE_ID);
			node1.SetInt32(role_info.role_id);
			condition.cmp_list.push_back(node1);

			DataAdapter data_attr = role_attr_table_detail9->GetPrototype();
			data_attr.Malloc();

			if (DBCommand::RESULT_SUC == role_attr_command_detail9->Find(stmt, condition, &data_attr))
			{
				// 红装收集
				static RedEquipCollectParam red_equip_collect_param;
				GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_RED_EQUIP_COLLECT_DATA, red_equip_collect_param);

				// 光环
				HaloParam halo_param;
				GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_HALO, halo_param);
				role_info.halo_used_imageid = halo_param.used_imageid;
			}

			data_attr.Free();
		}

		{
			// 新角色属性表10
			const MataTableDef *role_attr_table_detail10 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL10);
			DBCommand *role_attr_command_detail10 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL10);

			QueryCondition condition;
			CmpNode node1(tabledef::ROLE_ATTR_DETAIL10_ROLE_ID);
			node1.SetInt32(role_info.role_id);
			condition.cmp_list.push_back(node1);

			DataAdapter data_attr = role_attr_table_detail10->GetPrototype();
			data_attr.Malloc();

			if (DBCommand::RESULT_SUC == role_attr_command_detail10->Find(stmt, condition, &data_attr))
			{
				// 红装收集
				static OrangeEquipCollectParam orange_equip_collect_param;
				GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_ORANGE_EQUIP_COLLECT_DATA, orange_equip_collect_param);

				// 腰饰
				static YaoShiParam yaoshi_param;
				GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_YAOSHI_DATA, yaoshi_param);
				role_info.yaoshi_used_imageid = yaoshi_param.used_imageid;

				// 头饰
				static TouShiParam toushi_param;
				GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_TOUSHI_DATA, toushi_param);
				role_info.toushi_used_imageid = toushi_param.used_imageid;

				// 麒麟臂
				static QiLinBiParam qilinbi_param;
				GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_QILINBI_DATA, qilinbi_param);
				role_info.qilinbi_used_imageid = qilinbi_param.used_imageid;

				// 面具
				static MaskParam mask_param;
				GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_MASK_DATA, mask_param);
				role_info.mask_used_imageid = mask_param.used_imageid;
			}

			data_attr.Free();
		}

		{
			// 物品列表
			DBCommand *itemlistcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ITEMLIST);

			QueryCondition itemlistcondition;
			CmpNode itemlistnode1(tabledef::ITEMLIST_ROLE_ID);
			itemlistnode1.SetInt32(role_info.role_id);
			itemlistcondition.cmp_list.push_back(itemlistnode1);

			DataAdapterList item_data_list;
			int count = itemlistcommand->Find(stmt, itemlistcondition, &item_data_list);
			if (count > 0)
			{
				for (DataAdapterList::iterator i_item = item_data_list.begin(); i_item != item_data_list.end(); ++i_item)
				{
					// 武器(注意与 E_INDEX_WUQI 对应)
					if (ItemNamespace::EQUIPMENT_INDEX_BEGIN + 6 == i_item->m_data_area[tabledef::ITEMLIST_ITEMLIST_INDEX].vint)
					{
						role_info.wuqi_id = (ItemID)i_item->m_data_area[tabledef::ITEMLIST_ITEM_ID].vint;
					}

					i_item->Free();
				}
			}
		}
	}

	for (; i != role_list.end(); ++i)
	{
		i->Free();
	}

	return 0;
}

int RMIRoleObject::__RoleInit(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	int uid;
	if (!in_param.Pop(&uid))
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__RoleInit_1");
		return rmi::DispatchParamError;
	}

	static RoleInitParam p;
	static RoleOtherInitParam other_p;
	static ItemListParam item_list_param;
	static HotkeyListParam hotkey;
	static FriendListParam friends;
	static BlacklistsParam blacklists_param;
	static EnemyListParam enemy;
	static BusinessListParam business_good;
	static SkillListParam skill_param;
	static DailyFindListParam daily_find_param;

	item_list_param.count = 0;
	hotkey.count = 0;
	friends.count = 0;
	blacklists_param.count = 0;
	enemy.count = 0;
	business_good.count = 0;
	skill_param.count = 0;
	daily_find_param.count = 0;

	long long last_save_time = 0;

	int result = RoleInit(uid, &p, &other_p, &item_list_param, &hotkey, &friends, &blacklists_param, &enemy, &business_good, 
					&skill_param, &daily_find_param, &last_save_time);
	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__RoleInit_2 uid:%d result:%d", uid, result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (result == 0)
	{
		if (!p.Serialize(*out_param) 
			|| !other_p.Serialize(*out_param)
			|| !item_list_param.Serialize(*out_param) 
			|| !hotkey.Serialize(*out_param)
			|| !friends.Serialize(*out_param)
			|| !blacklists_param.Serialize(*out_param)
			|| !enemy.Serialize(*out_param)
			|| !business_good.Serialize(*out_param)
			|| !skill_param.Serialize(*out_param)
			|| !daily_find_param.Serialize(*out_param)
			|| !out_param->Push(last_save_time))
		{
			dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__RoleInit_3 uid:%d result:%d", uid, result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else 
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__RoleInit_4 uid:%d result:%d", uid, result);
	}

	RoleCacheManager::Instance().OnRoleLogin(uid); // 通知Cache 角色上线 
	
	return rmi::DispatchOK;
}

int RMIRoleObject::RoleInit(int uid, 
							RoleInitParam *p,
							RoleOtherInitParam *other_p,
							ItemListParam *item_list_param, 
							HotkeyListParam *hotkey,
							FriendListParam *friends,
							BlacklistsParam *blacklists_param,
							EnemyListParam *enemy,
							BusinessListParam *business_good,
							SkillListParam *skill_param,
							DailyFindListParam *daily_find_param,
							long long *last_save_time)
{

	if (IsCrossUser(IntToUid(uid)))
	{
		if (CrossRoleCacheManager::Instance().RoleInit(uid, p, other_p, item_list_param, hotkey, friends, blacklists_param, enemy, business_good, 
			skill_param, daily_find_param, last_save_time))
		{
			return 0;
		}

		return -100;
	}

	GameName u_name;
	memset(u_name, 0, sizeof(u_name));

	INIT_STMT(conn, stmt, -1);
	
	{
		// 角色
		const MataTableDef *roletable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE);
		DBCommand *rolecommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE);
		
		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -1;
		DataAdapter data = roletable->GetPrototype();
		data.Malloc();

		if (DBCommand::RESULT_SUC == rolecommand->Find(stmt, condition, &data))
		{
			ret = 0;
			p->role_id = data.m_data_area[tabledef::ROLE_ROLE_ID].vint;

			STRNCPY(p->role_name, data.m_data_area[tabledef::ROLE_ROLE_NAME].vcharp, sizeof(p->role_name));
			STRNCPY(u_name, p->role_name, sizeof(u_name)); // 保存下面直接拷贝

			p->avatar = data.m_data_area[tabledef::ROLE_AVATAR].vchar;
			p->sex = data.m_data_area[tabledef::ROLE_SEX].vchar;
			p->camp = data.m_data_area[tabledef::ROLE_COUNTRY].vchar;
			p->scene_id = data.m_data_area[tabledef::ROLE_SCENE_ID].vint;
			p->last_scene_id = data.m_data_area[tabledef::ROLE_LAST_SCENE_ID].vint;
			p->prof = data.m_data_area[tabledef::ROLE_PROFESSIONAL].vchar;
			p->level = data.m_data_area[tabledef::ROLE_LEVEL].vint;
			p->online_time = data.m_data_area[tabledef::ROLE_ONLINE_TIME].vint;
			p->create_time = data.m_data_area[tabledef::ROLE_CREATE_TIME].vint64;
			*last_save_time = data.m_data_area[tabledef::ROLE_LAST_SAVE_TIME].vint64;
		}

		data.Free();

		if (ret != 0)
		{
			return ret;
		}
	}
		
	{
		// 新角色属性表

		const MataTableDef *role_attr_table_detail = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL);
		DBCommand *role_attr_command_detail = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -2;
		DataAdapter data_attr = role_attr_table_detail->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail->Find(stmt, condition, &data_attr))
		{
			ret = 0;

			int max_capability = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_MAX_CAPABILITY].vint;
	
			unsigned int length = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_ATTR_DATA].length;
			if(length > (RMIRoleAttrVerManager::ROLE_ATTR_MAX_BUFFER_LENGTH * 2 ) || length < 0)
			{
				data_attr.Free(); // 可使用scope_spoint来解决该问题 TODO
				return -3;
			}

			static char buff[RMIRoleAttrVerManager::ROLE_ATTR_MAX_BUFFER_LENGTH];
			HexToBin(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_ATTR_DATA].vcharp, length, buff);

			TLVUnserializer attr_data;
			attr_data.Reset(buff, length / 2);
			if (!p->Unserialize(attr_data, true))
			{
				data_attr.Free();
				return -4;
			}
			
			// 修复合服之后role_id、role_name、camp被原服的role_attr_detail buf覆盖
			p->role_id = uid;
			F_STRNCPY(p->role_name, u_name, sizeof(p->role_name));
			p->max_capability = max_capability;
			p->camp = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_CAMP].vchar;

			{
				other_p->chest_buff_len = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_ROLECHEST_DATA_LEN].vint;
			}

			{
				other_p->mentality_param.Reset();

				length = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_MENTALITY_DATA].length;
				if (length > sizeof(other_p->mentality_param) * 2)
				{
					data_attr.Free();
					return -5;
				}

				HexToBin(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_MENTALITY_DATA].vcharp, length, (char*)&other_p->mentality_param);
			}
			
			{
				other_p->vip_param.Reset();

				length = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_VIP_DATA].length;
				if (length > sizeof(other_p->vip_param) * 2)
				{
					data_attr.Free();
					return -9;
				}

				HexToBin(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_VIP_DATA].vcharp, length, (char*)&other_p->vip_param);
			}
		
			{
				other_p->roleshop_param.Reset();

				length = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_ROLESHOP_DATA].length;
				if (length > sizeof(other_p->roleshop_param) * 2)
				{
					data_attr.Free();
					return -13;
				}

				HexToBin(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_ROLESHOP_DATA].vcharp, length, (char*)&other_p->roleshop_param);
			}

			{	
				other_p->active_degree_param.Reset();

				length = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_ACTIVE_DEGREE_DATA].length;
				if (length > sizeof(other_p->active_degree_param) * 2)
				{
					data_attr.Free();
					return -14;
				}

				HexToBin(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_ACTIVE_DEGREE_DATA].vcharp, length, (char*)&other_p->active_degree_param);
			}

			{
				other_p->welfare_param.Reset();

				length = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_WELFARE_DATA].length;
				if (length > sizeof(other_p->welfare_param) * 2)
				{
					data_attr.Free();
					return -17;
				}

				HexToBin(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_WELFARE_DATA].vcharp, length, (char*)&other_p->welfare_param);
			}

			{
				other_p->rolehangup_param.ResetHangupData();

				length = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_ROLEHANGUP_DATA].length;
				if (length > sizeof(other_p->rolehangup_param.hangup_set) * 2)
				{
					data_attr.Free();
					return -18;
				}

				HexToBin(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_ROLEHANGUP_DATA].vcharp, length, (char*)&other_p->rolehangup_param.hangup_set);
			}

			{
				other_p->itemcolddown_param.Reset();

				length = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_ITEMCOLDDOWN_DATA].length;
				if (length > sizeof(other_p->itemcolddown_param) * 2)
				{
					data_attr.Free();
					return -19;
				}

				HexToBin(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_ITEMCOLDDOWN_DATA].vcharp, length, (char*)&other_p->itemcolddown_param);
			}
		
			{
				other_p->evaluate_param.Reset();

				length = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_EVALUATE_DATA].length;
				if (length > sizeof(other_p->evaluate_param) * 2)
				{
					data_attr.Free();
					return -23;
				}

				HexToBin(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_EVALUATE_DATA].vcharp, length, (char*)&other_p->evaluate_param);
			}


			{
				other_p->rolehangup_param.ResetCustomData();

				length = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_ROLECUSTOM_DATA].length;
				if (length > sizeof(other_p->rolehangup_param.custom_set) * 2)
				{
					data_attr.Free();
					return -26;
				}

				HexToBin(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_ROLECUSTOM_DATA].vcharp, length, (char*)&other_p->rolehangup_param.custom_set);
			}

			{
				other_p->monitor_param.Reset();

				length = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_MONITOR_DATA].length;
				if (length > sizeof(other_p->monitor_param) * 2)
				{
					data_attr.Free();
					return -27;
				}

				HexToBin(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_MONITOR_DATA].vcharp, length, (char*)&other_p->monitor_param);
			}
			
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL_TITLE_DATA, other_p->title_param);

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL_HUSONGTASK_DATA, other_p->husong_param);

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL_COMMON_DATA, other_p->common_data_param);

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL_WING_DATA, other_p->wing_param);

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL_ROLEACTIVITY_DATA, other_p->roleactivity_param);

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL_FBTOWERDEFEND_DATA, other_p->towerdefend_param);

			{
				other_p->last_singlechat_monitor_time = static_cast<unsigned int>(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_LAST_SINGLE_CHAT_MONITOR_TIME].vint64);
				STRNCPY(other_p->last_singlechat_monitor_msg, data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_LAST_SINGLE_CHAT_MONITOR_MSG].vcharp, sizeof(other_p->last_singlechat_monitor_msg));
			}

			{
				other_p->last_worldchat_monitor_time = static_cast<unsigned int>(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_LAST_WORLD_CHAT_MONITOR_TIME].vint64);
				STRNCPY(other_p->last_worldchat_monitor_msg, data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_LAST_WORLD_CHAT_MONITOR_MSG].vcharp, sizeof(other_p->last_worldchat_monitor_msg));
			}

			{
				other_p->last_mail_monitor_time = static_cast<unsigned int>(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_LAST_MAIL_MONITOR_TIME].vint64);
				STRNCPY(other_p->last_mail_monitor_subject, data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_LAST_MAIL_MONITOR_SUBJECT].vcharp, sizeof(other_p->last_mail_monitor_subject));
			}
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表2

		const MataTableDef *role_attr_table_detail2 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL2);
		DBCommand *role_attr_command_detail2 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL2);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL2_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -2;
		DataAdapter data_attr = role_attr_table_detail2->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail2->Find(stmt, condition, &data_attr))
		{
			ret = 0;
			unsigned int length = 0;

			{
				length = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL2_ROLECHEST_DATA].length;
				if (length >= RoleOtherInitParam::ROLE_CHEST_SHOP_BUFF_LENGTH * 2)
				{
					data_attr.Free();
					return -10;
				}

				HexToBin(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL2_ROLECHEST_DATA].vcharp, length, other_p->chest_buff);
				length /= 2;
				other_p->chest_buff[length] = 0;
				other_p->chest_buff_len = length;
			}

			{
				length = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL2_EFFECT_DATA].length;
				if (length >= RoleOtherInitParam::MAX_EFFECT_NUM * RoleOtherInitParam::EFFECT_BUFFER_LENGTH * 2 + 1)
				{
					data_attr.Free();
					return -11;
				}

				HexToBin(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL2_EFFECT_DATA].vcharp, length, other_p->effect_buff);
				length /= 2;
				other_p->effect_buff[length] = 0;
				other_p->effect_buff_len = length;
			}

			{
				other_p->equipfb_param.Reset();

				length = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL2_EQUIPFB_DATA].length;
				if (length > sizeof(other_p->equipfb_param) * 2)
				{
					data_attr.Free();
					return -14;
				}

				HexToBin(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL2_EQUIPFB_DATA].vcharp, length, (char*)&other_p->equipfb_param);
			}

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL2_GUILD_TASK_DATA, other_p->guild_task_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL2_MAZE_DATA, other_p->maze_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL2_STORY_FB_DATA, other_p->storyfb_param);
			
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL2_CONVERT_RECORD_DATA, other_p->convert_record_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL2_NEWEQUIPFB_DATA, other_p->newequipfb_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL2_XIANNV_DATA, other_p->xiannv_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL2_STONE_DATA, other_p->stone_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL2_GUILD_XIANSHU_DATA, other_p->guild_xianshu_param);

			{
				length = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL2_TASKRECORD_DATA].length;
				if (length >= MAX_TASK_RECORD_BUFF_LEN * 2)
				{
					data_attr.Free();
					return -11;
				}

				HexToBin(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL2_TASKRECORD_DATA].vcharp, length, other_p->taskrecord_buff);
				length /= 2;
				other_p->taskrecord_buff[length] = 0;
				other_p->taskrecord_buff_len = length;
			}

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL2_TASKINFO_DATA, other_p->task_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL2_ROLE_SUPPLY_DATA, other_p->role_supply_param);
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表3

		const MataTableDef *role_attr_table_detail3 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL3);
		DBCommand *role_attr_command_detail3 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL3);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL3_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -2;
		DataAdapter data_attr = role_attr_table_detail3->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail3->Find(stmt, condition, &data_attr))
		{
			ret = 0;

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_OTHER_MODULE_DATA, other_p->other_module_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_QINGYUAN_DATA, other_p->qingyuan_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_ROLE_GOAL_DATA, other_p->role_goal_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_HISTORY_CAP_DATA, other_p->history_camp_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_SKILL_OTHER_DATA, other_p->skill_other_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_ROLE_GUILD_DATA, other_p->role_guild_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_SHENGWANG_DATA, other_p->shengwang_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_CHENGJIU_DATA, other_p->chengjiu_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_DAILYFB_DATA, other_p->dailyfb_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_LIFESKILL_DATA, other_p->lifeskill_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_BABY_DATA, other_p->baby_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_SHENGE_SYSTEM_DATA, other_p->shenge_system_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_ZERO_GIFT_DATA, other_p->zero_gift_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_MYSTERIOUS_SHOP_IN_MALL_DATA, other_p->mysterious_shop_in_mall_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_FRIEND_EXP_BOTTLE_DATA, other_p->friend_exp_bottle_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_SHENSHOU, other_p->shenshou_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_IMG_FULING_DATA, other_p->img_fuling_param);
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表4
		const MataTableDef *role_attr_table_detail4 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL4);
		DBCommand *role_attr_command_detail4 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL4);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL4_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -2;
		DataAdapter data_attr = role_attr_table_detail4->GetPrototype();
		data_attr.Malloc();
		
		if (DBCommand::RESULT_SUC == role_attr_command_detail4->Find(stmt, condition, &data_attr))
		{
			ret = 0;

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_TUHAOJIN_DATA, other_p->tuhaojin_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_BIGCHATFACE_DATA, other_p->bigchatface_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_PET_DATA, other_p->pet_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_PHASE_FB_DATA, other_p->phase_fb_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_VIP_FB_DATA, other_p->vipfb_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_HUASHEN_DATA, other_p->huashen_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_MAGIC_CARD_DATA, other_p->magic_card_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_MITAMA_DATA, other_p->mitama_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_EQUIPMENT_DATA, other_p->equipment_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_CHINESE_ZODIAC_DATA, other_p->chinese_zodiac_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_FRIEND_GIFT, other_p->friend_gift_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_FOLLOW_BOSS, other_p->follow_boss_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_GOLD_VIP, other_p->gold_vip_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_RUNE_SYSTEM, other_p->rune_system_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_MAGICAL_PRECIOUS_DATA, other_p->magical_precious_param);
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表5
		const MataTableDef *role_attr_table_detail5 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL5);
		DBCommand *role_attr_command_detail5 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL5);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL5_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -2;
		DataAdapter data_attr = role_attr_table_detail5->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail5->Find(stmt, condition, &data_attr))
		{
			ret = 0;

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_MULTI_MOUNT_DATA, other_p->multi_mount_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_PERSONALIZE_WINDOW_DATA, other_p->personalize_window_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_XUNZHANG, other_p->xunzhang_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_ZHIBAO, other_p->zhibao_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_JINGLING, other_p->jingling_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_XIANNV_SHOUHU, other_p->xiannv_shouhu_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_JINGLING_FAZHEN, other_p->jingling_fazhen_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_JINGLING_GUANGHUAN, other_p->jingling_guanghuan_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_SHENZHOU_WEAPON_DATA, other_p->shenzhou_weapon_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_LIEMING, other_p->lieming_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_LIEMING_BAG, other_p->lieming_bag_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_CARDZU, other_p->cardzu_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_ZHUANSHENG_EQUIP, other_p->zhuansheng_equip_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_SHENBING, other_p->shenbing_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_TUITUFB_DATA, other_p->tuitu_fb_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_CLOAK, other_p->cloak_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_ROLE_PRECIOUS_BOSS, other_p->precious_boss_param);

			UNSTD_STATIC_CHECK(19 == tabledef::TABLE_ROLE_ATTR_DETAIL5_MAXINDEX);

		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表6
		const MataTableDef *role_attr_table_detail6 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL6);
		DBCommand *role_attr_command_detail6 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL6);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL6_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -2;
		DataAdapter data_attr = role_attr_table_detail6->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail6->Find(stmt, condition, &data_attr))
		{
			ret = 0;

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL6_COUPLE_MOUNT_DATA, other_p->couple_mount_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL6_SHENYIN_DATA, other_p->shenyin_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL6_ELEMENT_HEART_DATA, other_p->element_heart_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL6_SHENGXINGZHULI, other_p->shengxing_zhuli_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL6_SHIZHUANG_DATA, other_p->shizhuang_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL6_FABAO_DATA, other_p->fabao_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL6_FEIXIAN_DATA, other_p->feixian_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL6_TALENT_DATA, other_p->talent_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL6_JINJIE_TALENT_DATA, other_p->jinjie_talent_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL6_ARMOR_FB_DATA, other_p->armor_fb_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL6_BOSSCARD_DATA, other_p->bosscard_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL6_TASK_ZHUANZHI_DATA, other_p->task_zhuanzhi_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL6_DISCOUNT_BUY_DATA, other_p->discount_buy_param);

			UNSTD_STATIC_CHECK( 15 == tabledef::TABLE_ROLE_ATTR_DETAIL6_MAXINDEX);
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表7
		const MataTableDef *role_attr_table_detail7 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL7);
		DBCommand *role_attr_command_detail7 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL7);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL7_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -2;
		DataAdapter data_attr = role_attr_table_detail7->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail7->Find(stmt, condition, &data_attr))
		{
			ret = 0;

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL7_CROSS_GLOBAL_DATA, other_p->cross_global_data_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL7_ZHUANZHI_EQUIP_DATA, other_p->zhuanzhi_equip_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL7_ROLE_CROSS_DATA, other_p->role_cross_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL7_HESHENLUOSHU_DATA, other_p->heshen_luoshu_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL7_SHENGQI_DATA, other_p->shengqi_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL7_SEAL_DATA, other_p->seal_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL7_DRESSING_ROOM_DATA, other_p->dressing_room_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL7_CHALLENGE_FB_DATA, other_p->challengefb_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL7_ROLE_CROSS_RAND_ACTIVITY_DATA, other_p->role_cross_rand_activity_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL7_CROSS_GOAL_DATA, other_p->cross_goal_param);

			UNSTD_STATIC_CHECK(12 == tabledef::TABLE_ROLE_ATTR_DETAIL7_MAXINDEX);
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表8
		const MataTableDef *role_attr_table_detail8 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL8);
		DBCommand *role_attr_command_detail8 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL8);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL8_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -2;
		DataAdapter data_attr = role_attr_table_detail8->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail8->Find(stmt, condition, &data_attr))
		{
			ret = 0;

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_UPGRADE_MANAGER_DATA, other_p->upgrade_manager_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_SHENQI_DATA, other_p->shenqi_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_MINGHUN2_DATA, other_p->lieming_param2);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_PERSON_PROFESS_DATA, other_p->profess_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_XIANZUNKA_DATA, other_p->xianzunka_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_EQUIP_BAPTIZE_DATA, other_p->equipbaptize_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_MOJIE, other_p->mojie_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_FB_PERSONBOSS_DATA, other_p->personboss_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_GREATE_SOLDIER_DATA, other_p->greate_soldier_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_LOTTERY_COUNT_DATA, other_p->role_lottery_count_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_UPGRADE_CARD_BUY_DATA, other_p->upgrade_card_buy_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_MYTH_DATA, other_p->myth_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_ZODIAC_DATA, other_p->zodiac_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_CROSS_EQUIP_DATA, other_p->cross_equip_param);
			UNSTD_STATIC_CHECK(16 == tabledef::TABLE_ROLE_ATTR_DETAIL8_MAXINDEX);
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表9
		const MataTableDef *role_attr_table_detail9 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL9);
		DBCommand *role_attr_command_detail9 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL9);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL9_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -2;
		DataAdapter data_attr = role_attr_table_detail9->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail9->Find(stmt, condition, &data_attr))
		{
			ret = 0;

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_RED_EQUIP_COLLECT_DATA, other_p->red_equip_collect_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_MOUNT_DATA, other_p->mount_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_FOOTPRINT, other_p->footprint_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_HALO, other_p->halo_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_HUGUOZHILI, other_p->huguozhili_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_BAIZHAN_EQUIP_DATA, other_p->baizhan_equip_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_FIGHT_MOUNT_DATA, other_p->fight_mount_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_MOJIEFB_DATA, other_p->mojiefb_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_EQUIPSUIT_DATA, other_p->equipsuit_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_SHENZHUANG, other_p->shenzhuang_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_CSA_EQUIP_DATA, other_p->csa_equip_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_SHEN_GONG, other_p->shengong_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_SHEN_YI, other_p->shenyi_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_DISCOUNTSHOP_DATA, other_p->discountbuy_param);

			UNSTD_STATIC_CHECK(16 == tabledef::TABLE_ROLE_ATTR_DETAIL9_MAXINDEX);
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表10
		const MataTableDef *role_attr_table_detail10 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL10);
		DBCommand *role_attr_command_detail10 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL10);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL10_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -2;
		DataAdapter data_attr = role_attr_table_detail10->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail10->Find(stmt, condition, &data_attr))
		{
			ret = 0;

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_ORANGE_EQUIP_COLLECT_DATA, other_p->orange_equip_collect_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_YAOSHI_DATA, other_p->yaoshi_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_TOUSHI_DATA, other_p->toushi_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_QILINBI_DATA, other_p->qilinbi_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_MASK_DATA, other_p->mask_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_JINJIESYS_REWARD_DATA, other_p->jinjiesys_reward_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_ROLE_BIG_SMALL_GOAL_DATA, other_p->role_big_small_goal_param);

			ret = HexToBinAutoDataPege(data_attr, other_p->data_page_10, tabledef::ROLE_ATTR_DETAIL10_DATA_PAGE, tabledef::ROLE_ATTR_DETAIL10_DATA_PAGE_LEN);
			if (ret != 0) ret = -1011;

			UNSTD_STATIC_CHECK(11 == tabledef::TABLE_ROLE_ATTR_DETAIL10_MAXINDEX);
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 物品列表
		DBCommand *itemlistcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ITEMLIST);

		QueryCondition itemlistcondition;
		CmpNode itemlistnode1(tabledef::ITEMLIST_ROLE_ID);
		itemlistnode1.SetInt32(uid);
		itemlistcondition.cmp_list.push_back(itemlistnode1);

		DataAdapterList item_data_list;
		int ret = itemlistcommand->Find(stmt, itemlistcondition, &item_data_list);
		if (ret < 0)
		{
			return ret;
		}

		item_list_param->count = 0;
		DataAdapterList::iterator i = item_data_list.begin(); 
		for (; item_list_param->count < ItemNamespace::MAX_GRID_NUM && i != item_data_list.end(); ++i, ++item_list_param->count)
		{
			int index = i->m_data_area[tabledef::ITEMLIST_ITEMLIST_INDEX].vint;
			if (index >= ItemNamespace::EQUIPMENT_INDEX_BEGIN + ItemNamespace::MAX_EQUIPMENT_GRID_NUM) // index 肯定出错了！！！
			{
				i->Free();
				continue; // 不能return 否则内存泄露
			}

			item_list_param->item_list[item_list_param->count].change_state_item = structcommon::CS_NONE;
			item_list_param->item_list[item_list_param->count].index = index;
			item_list_param->item_list[item_list_param->count].item_wrapper.item_id = (UInt16)i->m_data_area[tabledef::ITEMLIST_ITEM_ID].vint;
			item_list_param->item_list[item_list_param->count].item_wrapper.is_bind = (short)i->m_data_area[tabledef::ITEMLIST_IS_BIND].vchar;
			item_list_param->item_list[item_list_param->count].item_wrapper.num = i->m_data_area[tabledef::ITEMLIST_NUM].vint;
			item_list_param->item_list[item_list_param->count].item_wrapper.invalid_time = (unsigned int)i->m_data_area[tabledef::ITEMLIST_INVALID_TIME].vint64;
			item_list_param->item_list[item_list_param->count].item_wrapper.gold_price = i->m_data_area[tabledef::ITEMLIST_GOLD_PRICE].vint;
				
			item_list_param->item_list[item_list_param->count].item_wrapper.has_param = 0;

			unsigned int length = i->m_data_area[tabledef::ITEMLIST_PARAM].length;
			if (length > 0)
			{
				if (length > sizeof(item_list_param->item_list[item_list_param->count].item_wrapper.param_data) * 2)
				{
					i->Free();
					continue;
				}

				item_list_param->item_list[item_list_param->count].item_wrapper.param_data.Reset();
				item_list_param->item_list[item_list_param->count].item_wrapper.has_param = 1;
				HexToBin(i->m_data_area[tabledef::ITEMLIST_PARAM].vcharp, length, (char *)(&item_list_param->item_list[item_list_param->count].item_wrapper.param_data));
			}

			i->Free();
		}

		for (; i != item_data_list.end(); ++i)
		{
			i->Free();
		}
	}
	
	{
		// 快捷键
		// const MataTableDef *hotkeytable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_HOTKEY);
		DBCommand *hotkeycommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_HOTKEY);

		QueryCondition hotkeycondition;

		CmpNode hotkeynode1(tabledef::HOTKEY_ROLE_ID);
		hotkeynode1.SetInt32(uid);
		hotkeycondition.cmp_list.push_back(hotkeynode1);

		DataAdapterList hotkey_list;
		int ret = hotkeycommand->Find(stmt, hotkeycondition, &hotkey_list);
		if(ret < 0)
		{
			return ret;
		}

		int hk_count = 0;
		DataAdapterList::iterator i = hotkey_list.begin(); 

		for(; hk_count < HotkeyListParam::MAX_HOTKEY_NUM && i != hotkey_list.end(); ++i)
		{
			hotkey->hotkey_list[hk_count].change_state = structcommon::CS_NONE;

			hotkey->hotkey_list[hk_count].index = i->m_data_area[tabledef::HOTKEY_HOTKEY_INDEX].vint;
			hotkey->hotkey_list[hk_count].item_id = (unsigned short)i->m_data_area[tabledef::HOTKEY_HOTKEY_ITEM_ID].vint;
			hotkey->hotkey_list[hk_count].item_type = i->m_data_area[tabledef::HOTKEY_HOTKEY_ITEM_TYPE].vchar;
			++hk_count;
			i->Free();
		}
		
		for (; i != hotkey_list.end(); ++i)
		{
			i->Free();
		}

		hotkey->count = hk_count;
	}

	{
		// 好友
		DBCommand *friendcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_FRIENDS);

		QueryCondition friendcontion;

		CmpNode friendnode1(tabledef::FRIENDS_ROLE_ID);
		friendnode1.SetInt32(uid);
		friendcontion.cmp_list.push_back(friendnode1);

		DataAdapterList friend_list;
		int ret = friendcommand->Find(stmt,friendcontion,&friend_list);
		if(ret < 0)
		{
			return ret;
		}

		int friend_count = 0;
		DataAdapterList::iterator i = friend_list.begin(); 

		for (; friend_count < MAX_FRIEND_NUM && i != friend_list.end(); ++i)
		{
			friends->friend_list[friend_count].change_state = structcommon::CS_NONE;

			friends->friend_list[friend_count].friend_uid =  i->m_data_area[tabledef::FRIENDS_FRIEND_UID].vint;
			friends->friend_list[friend_count].intimacy = i->m_data_area[tabledef::FRIENDS_INTIMACY].vint;
			friends->friend_list[friend_count].team_today_intimacy = i->m_data_area[tabledef::FRIENDS_TEAM_TODAY_INTIMACY].vint;
			friends->friend_list[friend_count].gift_count = i->m_data_area[tabledef::FRIENDS_GIFT_COUNT].vint;
			friends->friend_list[friend_count].index  = i->m_data_area[tabledef::FRIENDS_FRIEND_INDEX].vint;
			friends->friend_list[friend_count].favorable_impression = i->m_data_area[tabledef::FRIENDS_FAVORABLE_IMPRESSION].vint;
			friends->friend_list[friend_count].day_chat_count = i->m_data_area[tabledef::FRIENDS_DAY_CHAT_COUNT].vint;
			friends->friend_list[friend_count].add_timestamp = i->m_data_area[tabledef::FRIENDS_ADD_TIMESTAMP].vint64;
			
			++friend_count;
			i->Free();
		}

		for (; i != friend_list.end(); ++i)
		{
			i->Free();
		}

		friends->count = friend_count;
	}

	{
		// 黑名单
		DBCommand *hatecommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_BLACK_LISTS);

		QueryCondition hatecontion;

		CmpNode hatenode1(tabledef::BLACK_LISTS_ROLE_ID);
		hatenode1.SetInt32(uid);
		hatecontion.cmp_list.push_back(hatenode1);

		DataAdapterList hate_list;
		int ret = hatecommand->Find(stmt, hatecontion, &hate_list);
		if (ret < 0)
		{
			return ret;
		}

		int hate_count = 0;
		DataAdapterList::iterator it = hate_list.begin();

		for (; it != hate_list.end() && hate_count < MAX_BLACK_NUM; ++it)
		{
			blacklists_param->black_list[hate_count].change_state = structcommon::CS_NONE;

			blacklists_param->black_list[hate_count].uid = it->m_data_area[tabledef::BLACK_LISTS_BLACK_UID].vint;
			STRNCPY(blacklists_param->black_list[hate_count].gamename, it->m_data_area[tabledef::BLACK_LISTS_BLACK_NAME].vcharp,
				sizeof(blacklists_param->black_list[0].gamename));
			blacklists_param->black_list[hate_count].sex = it->m_data_area[tabledef::BLACK_LISTS_SEX].vchar;
			blacklists_param->black_list[hate_count].prof = it->m_data_area[tabledef::BLACK_LISTS_PROF].vchar;
			blacklists_param->black_list[hate_count].index = it->m_data_area[tabledef::BLACK_LISTS_BLACK_INDEX].vint;

			++ hate_count;
			it->Free();
		}

		for (; it != hate_list.end(); ++it)
		{
			it->Free(); 
		}

		blacklists_param->count = hate_count;
	}


	{
		// 敌人
		DBCommand *enemycommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ENEMY);

		QueryCondition	enemycontion;
		CmpNode enemynode1(tabledef::ENEMY_ROLE_ID);
		enemynode1.SetInt32(uid);
		enemycontion.cmp_list.push_back(enemynode1);
		
		DataAdapterList enemylist;
		int ret = enemycommand->Find(stmt,enemycontion,&enemylist);
		if(ret < 0)
		{
			return ret;
		}

		int enemy_count = 0;
		DataAdapterList::iterator i = enemylist.begin(); 

		for(; enemy_count < MAX_ENEMY_NUM && i != enemylist.end(); ++i)
		{
			enemy->enemy_list[enemy_count].change_state = structcommon::CS_NONE;

			enemy->enemy_list[enemy_count].enemy_uid =	i->m_data_area[tabledef::ENEMY_ENEMY_UID].vint;
			enemy->enemy_list[enemy_count].enemy_plat_type = i->m_data_area[tabledef::ENEMY_ENEMY_PLAT_TYPE].vint;
			enemy->enemy_list[enemy_count].kill_count = i->m_data_area[tabledef::ENEMY_KILL_COUNT].vint;
			enemy->enemy_list[enemy_count].last_kill_time = i->m_data_area[tabledef::ENEMY_LAST_KILL_TIME].vint64;			
			enemy->enemy_list[enemy_count].index = i->m_data_area[tabledef::ENEMY_ENEMY_INDEX].vint;
			enemy->enemy_list[enemy_count].be_kill_count = i->m_data_area[tabledef::ENEMY_BE_KILL_COUNT].vint;

			++enemy_count;
			i->Free();	
		}
	
		for (; i != enemylist.end(); ++i)
		{
			i->Free();
		}

		enemy->count = enemy_count;
	}

	{
		// 技能
		DBCommand *skillcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLESKILL);

		QueryCondition skillcondition;
		CmpNode skillnode1(tabledef::ROLESKILL_ROLE_ID);
		skillnode1.SetInt32(uid);
		skillcondition.cmp_list.push_back(skillnode1);

		DataAdapterList skill_list;
		int ret = skillcommand->Find(stmt, skillcondition, &skill_list);
		if (ret < 0)
		{
			return ret;
		}

		DataAdapterList::iterator i = skill_list.begin();

		skill_param->count = 0;
		for (; skill_param->count < SkillListParam::MAX_SKILL_NUM && i != skill_list.end(); ++i, ++skill_param->count)
		{
			skill_param->skill_list[skill_param->count].index = (char)i->m_data_area[tabledef::ROLESKILL_SKILL_INDEX].vint;
			skill_param->skill_list[skill_param->count].skill_id = (unsigned short)i->m_data_area[tabledef::ROLESKILL_SKILL_ID].vint;
			skill_param->skill_list[skill_param->count].level = i->m_data_area[tabledef::ROLESKILL_SKILL_LEVEL].vint;
			skill_param->skill_list[skill_param->count].last_perform_time = i->m_data_area[tabledef::ROLESKILL_LAST_PERFORM_TIME].vint64;
			skill_param->skill_list[skill_param->count].exp = i->m_data_area[tabledef::ROLESKILL_SKILL_EXP].vint;
			i->Free();
		}

		for (; i != skill_list.end(); ++i)
		{
			i->Free();
		}
	}

	{
		// 日常找回
		DBCommand *dailyfindcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_DAILYFIND);

		QueryCondition dailyfind_condition;
		CmpNode dailyfind_node1(tabledef::DAILYFIND_ROLE_ID);
		dailyfind_node1.SetInt32(uid);
		dailyfind_condition.cmp_list.push_back(dailyfind_node1);

		DataAdapterList dailyfind_list;
		int ret = dailyfindcommand->Find(stmt, dailyfind_condition, &dailyfind_list);
		if (ret < 0)
		{
			return ret;
		}

		DataAdapterList::iterator i = dailyfind_list.begin();

		daily_find_param->count = 0;
		for (; daily_find_param->count < DAILYFIND_TYPE_MAX * WELFARE_DAILY_FIND_MAXDAYS && i != dailyfind_list.end(); ++i, ++daily_find_param->count)
		{
			daily_find_param->daily_find_list[daily_find_param->count].index = (short)i->m_data_area[tabledef::DAILYFIND_FIND_INDEX].vint;
			daily_find_param->daily_find_list[daily_find_param->count].find_time = (unsigned int)i->m_data_area[tabledef::DAILYFIND_FIND_TIME].vint;
			daily_find_param->daily_find_list[daily_find_param->count].role_level = i->m_data_area[tabledef::DAILYFIND_ROLE_LEVEL].vint;
			daily_find_param->daily_find_list[daily_find_param->count].param = i->m_data_area[tabledef::DAILYFIND_PARAM].vint;
			daily_find_param->daily_find_list[daily_find_param->count].param1 = i->m_data_area[tabledef::DAILYFIND_PARAM1].vint64;
			daily_find_param->daily_find_list[daily_find_param->count].find_count = i->m_data_area[tabledef::DAILYFIND_FIND_COUNT].vint;

			i->Free();
		}

		for (; i != dailyfind_list.end(); ++i)
		{
			i->Free();
		}
	}

	return 0;
}

int RMIRoleObject::__RoleQuery(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	int uid;
	if (!in_param.Pop(&uid))
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__RoleQuery_1");
		return rmi::DispatchParamError;
	}
	
	RoleQueryParam *rolequery_p = (RoleQueryParam *)new ServerMsgMem[sizeof(RoleQueryParam)];

	ItemListParam *itemlist_p = (ItemListParam *)new ServerMsgMem[sizeof(ItemListParam)];
	itemlist_p->count = 0;

	int result = RoleQuery(uid, rolequery_p, itemlist_p);
	if (!out_param->Push(result))
	{
		ServerMsgMem *tmp_rolequery_p = (ServerMsgMem*)rolequery_p;
		delete []tmp_rolequery_p;

		ServerMsgMem *tmp_itemlist_p = (ServerMsgMem*)itemlist_p;
		delete []tmp_itemlist_p;

		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__RoleQuery_2 uid:%d result:%d", uid, result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (result == 0)
	{
		if (!rolequery_p->Serialize(*out_param)
			|| !itemlist_p->Serialize(*out_param))
		{
			ServerMsgMem *tmp_rolequery_p = (ServerMsgMem*)rolequery_p;
			delete []tmp_rolequery_p;

			ServerMsgMem *tmp_itemlist_p = (ServerMsgMem*)itemlist_p;
			delete []tmp_itemlist_p;

			dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__RoleQuery_3 uid:%d result:%d", uid, result);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else 
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__RoleQuery_4 uid:%d result:%d", uid, result);
	}

	ServerMsgMem *tmp_rolequery_p = (ServerMsgMem*)rolequery_p;
	delete []tmp_rolequery_p;

	ServerMsgMem *tmp_itemlist_p = (ServerMsgMem*)itemlist_p;
	delete []tmp_itemlist_p;

	return rmi::DispatchOK;
}

int RMIRoleObject::RoleQuery(int uid, RoleQueryParam *other_p, ItemListParam *item_list_param)
{
	GameName u_name;
	memset(u_name, 0, sizeof(u_name));

	INIT_STMT(conn, stmt, -1);

	{
		// 新角色属性表

		const MataTableDef *role_attr_table_detail = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL);
		DBCommand *role_attr_command_detail = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -2;
		DataAdapter data_attr = role_attr_table_detail->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail->Find(stmt, condition, &data_attr, false, false))
		{
			ret = 0;

			other_p->role_id = uid;
			STRNCPY(other_p->role_name, data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_ROLE_NAME].vcharp, sizeof(other_p->role_name));
			other_p->camp = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_CAMP].vchar;
			other_p->prof = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_PROF].vchar;
			other_p->sex = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_SEX].vchar;
			other_p->level = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_LEVEL].vint;
			other_p->guild_id = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_GUILD_ID].vint;
			other_p->vip_level = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_VIP_TYPE].vchar;
			other_p->avatar_timestamp = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_AVATAR_TIMESTAMP].vint64;

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL_TITLE_DATA, other_p->title_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL_COMMON_DATA, other_p->common_data_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL_WING_DATA, other_p->wing_param);

			{
				unsigned int length = data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_ATTR_DATA].length;
				if(length > (RMIRoleAttrVerManager::ROLE_ATTR_MAX_BUFFER_LENGTH * 2 ) || length < 0)
				{
					data_attr.Free(); // 可使用scope_spoint来解决该问题 TODO
					return -3;
				}

				static char buff[RMIRoleAttrVerManager::ROLE_ATTR_MAX_BUFFER_LENGTH];
				HexToBin(data_attr.m_data_area[tabledef::ROLE_ATTR_DETAIL_ATTR_DATA].vcharp, length, buff);

				TLVUnserializer attr_data;
				attr_data.Reset(buff, length / 2);
				if (!other_p->role_init_param.Unserialize(attr_data, true))
				{
					data_attr.Free();
					return -4;
				}
			}
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表2

		const MataTableDef *role_attr_table_detail2 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL2);
		DBCommand *role_attr_command_detail2 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL2);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL2_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -2;
		DataAdapter data_attr = role_attr_table_detail2->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail2->Find(stmt, condition, &data_attr, false, false))
		{
			ret = 0;
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL2_STONE_DATA, other_p->stone_param);
			
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL2_XIANNV_DATA, other_p->xiannv_param);
			
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表3

		const MataTableDef *role_attr_table_detail3 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL3);
		DBCommand *role_attr_command_detail3 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL3);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL3_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -2;
		DataAdapter data_attr = role_attr_table_detail3->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail3->Find(stmt, condition, &data_attr, false, false))
		{
			ret = 0;

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL3_QINGYUAN_DATA, other_p->qingyuan_param);
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表4
		const MataTableDef *role_attr_table_detail4 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL4);
		DBCommand *role_attr_command_detail4 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL4);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL4_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -2;
		DataAdapter data_attr = role_attr_table_detail4->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail4->Find(stmt, condition, &data_attr, false, false))
		{
			ret = 0;
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_PET_DATA, other_p->pet_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL4_EQUIPMENT_DATA, other_p->equipment_param);
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表5
		const MataTableDef *role_attr_table_detail5 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL5);
		DBCommand *role_attr_command_detail5 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL5);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL5_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -2;
		DataAdapter data_attr = role_attr_table_detail5->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail5->Find(stmt, condition, &data_attr, false, false))
		{
			ret = 0;
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_XIANNV_SHOUHU, other_p->xiannv_shouhu_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_JINGLING_FAZHEN, other_p->jingling_fazhen_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_JINGLING_GUANGHUAN, other_p->jingling_guanghuan_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_JINGLING, other_p->jingling_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_LIEMING, other_p->lieming_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_CLOAK, other_p->cloak_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL5_SHENBING, other_p->shenbing_param);
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表6
		const MataTableDef *role_attr_table_detail6 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL6);
		DBCommand *role_attr_command_detail6 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL6);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL6_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -2;
		DataAdapter data_attr = role_attr_table_detail6->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail6->Find(stmt, condition, &data_attr, false, false))
		{
			ret = 0;
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL6_SHIZHUANG_DATA, other_p->shizhuang_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL6_FABAO_DATA, other_p->fabao_param);
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表7
		const MataTableDef *role_attr_table_detail7 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL7);
		DBCommand *role_attr_command_detail7 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL7);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL7_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -7;
		DataAdapter data_attr = role_attr_table_detail7->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail7->Find(stmt, condition, &data_attr, false, false))
		{
			ret = 0;
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL7_ZHUANZHI_EQUIP_DATA, other_p->zhuanzhi_equip_param);
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表8
		const MataTableDef *role_attr_table_detail8 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL8);
		DBCommand *role_attr_command_detail8 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL8);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL8_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -8;
		DataAdapter data_attr = role_attr_table_detail8->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail8->Find(stmt, condition, &data_attr, false, false))
		{
			ret = 0;

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_UPGRADE_MANAGER_DATA, other_p->upgrade_manager_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_EQUIP_BAPTIZE_DATA, other_p->equipbaptize_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_MOJIE, other_p->mojie_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL8_GREATE_SOLDIER_DATA, other_p->greate_soldier_param);
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表9
		const MataTableDef *role_attr_table_detail9 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL9);
		DBCommand *role_attr_command_detail9 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL9);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL9_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -9;
		DataAdapter data_attr = role_attr_table_detail9->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail9->Find(stmt, condition, &data_attr, false, false))
		{
			ret = 0;
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_MOUNT_DATA, other_p->mount_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_FOOTPRINT, other_p->footprint_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_HALO, other_p->halo_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_BAIZHAN_EQUIP_DATA, other_p->baizhan_equip_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_FIGHT_MOUNT_DATA, other_p->fight_mount_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_SHENZHUANG, other_p->shenzhuang_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_SHEN_GONG, other_p->shengong_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL9_SHEN_YI, other_p->shenyi_param);
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 新角色属性表10
		const MataTableDef *role_attr_table_detail10 = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL10);
		DBCommand *role_attr_command_detail10 = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL10);

		QueryCondition condition;
		CmpNode node1(tabledef::ROLE_ATTR_DETAIL10_ROLE_ID);
		node1.SetInt32(uid);
		condition.cmp_list.push_back(node1);

		int ret = -10;
		DataAdapter data_attr = role_attr_table_detail10->GetPrototype();
		data_attr.Malloc();

		if (DBCommand::RESULT_SUC == role_attr_command_detail10->Find(stmt, condition, &data_attr, false, false))
		{
			ret = 0;

			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_YAOSHI_DATA, other_p->yaoshi_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_TOUSHI_DATA, other_p->toushi_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_QILINBI_DATA, other_p->qilinbi_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_MASK_DATA, other_p->mask_param);
			GET_BINARY_DATA(data_attr, tabledef::ROLE_ATTR_DETAIL10_JINJIESYS_REWARD_DATA, other_p->jinjiesys_reward_param);
			
			ret = HexToBinAutoDataPege(data_attr, other_p->data_page_10, tabledef::ROLE_ATTR_DETAIL10_DATA_PAGE, tabledef::ROLE_ATTR_DETAIL10_DATA_PAGE_LEN);
			if (ret != 0) ret = -1011;
			
		}

		data_attr.Free();

		if (ret != 0)
		{
			return ret;
		}
	}

	{
		// 物品列表
		DBCommand *itemlistcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ITEMLIST);

		QueryCondition itemlistcondition;
		CmpNode itemlistnode1(tabledef::ITEMLIST_ROLE_ID);
		itemlistnode1.SetInt32(uid);
		itemlistcondition.cmp_list.push_back(itemlistnode1);

		DataAdapterList item_data_list;
		int ret = itemlistcommand->Find(stmt, itemlistcondition, &item_data_list, false, false);
		if (ret < 0)
		{
			return ret;
		}

		item_list_param->count = 0;
		DataAdapterList::iterator i = item_data_list.begin(); 
		for (; item_list_param->count < ItemNamespace::MAX_GRID_NUM && i != item_data_list.end(); ++i, ++item_list_param->count)
		{
			int index = i->m_data_area[tabledef::ITEMLIST_ITEMLIST_INDEX].vint;
			if (index >= ItemNamespace::EQUIPMENT_INDEX_BEGIN + ItemNamespace::MAX_EQUIPMENT_GRID_NUM) // index 肯定出错了！！！
			{
				i->Free();
				continue; // 不能return 否则内存泄露
			}

			item_list_param->item_list[item_list_param->count].change_state_item = structcommon::CS_NONE;
			item_list_param->item_list[item_list_param->count].index = index;
			item_list_param->item_list[item_list_param->count].item_wrapper.item_id = (UInt16)i->m_data_area[tabledef::ITEMLIST_ITEM_ID].vint;
			item_list_param->item_list[item_list_param->count].item_wrapper.is_bind = (short)i->m_data_area[tabledef::ITEMLIST_IS_BIND].vchar;
			item_list_param->item_list[item_list_param->count].item_wrapper.num = i->m_data_area[tabledef::ITEMLIST_NUM].vint;
			item_list_param->item_list[item_list_param->count].item_wrapper.invalid_time = (unsigned int)i->m_data_area[tabledef::ITEMLIST_INVALID_TIME].vint64;
			item_list_param->item_list[item_list_param->count].item_wrapper.gold_price = i->m_data_area[tabledef::ITEMLIST_GOLD_PRICE].vint;

			item_list_param->item_list[item_list_param->count].item_wrapper.has_param = 0;

			unsigned int length = i->m_data_area[tabledef::ITEMLIST_PARAM].length;
			if (length > 0)
			{
				if (length > sizeof(item_list_param->item_list[item_list_param->count].item_wrapper.param_data) * 2)
				{
					i->Free();
					continue;
				}

				item_list_param->item_list[item_list_param->count].item_wrapper.param_data.Reset();
				item_list_param->item_list[item_list_param->count].item_wrapper.has_param = 1;
				HexToBin(i->m_data_area[tabledef::ITEMLIST_PARAM].vcharp, length, (char *)(&item_list_param->item_list[item_list_param->count].item_wrapper.param_data));
			}

			i->Free();
		}

		for (; i != item_data_list.end(); ++i)
		{
			i->Free();
		}
	}

	return 0;
}

int RMIRoleObject::__RoleSave(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	int uid = 0;
	bool ret = in_param.Pop(&uid);
	if (!ret) 
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__RoleSave_1");
		return rmi::DispatchParamError;
	}

	static RoleInitParam role_p;
	static RoleOtherInitParam other_p;
	static ItemListParam knapsack;
	static HotkeyListParam hotkey;
	static FriendListParam friends;
	static BlacklistsParam blacklists_param;
	static EnemyListParam enemy;
	static BusinessListParam business;
	static MountParam mount_param;
	static SkillListParam skill_param;
	static DailyFindListParam daily_find_param;

	bool is_logout = false;
	int allow_to_cross_flag = 0;

	ret = role_p.Unserialize(in_param);
	if (!ret) return rmi::DispatchParamError;
	ret = other_p.Unserialize(in_param);
	if (!ret) return rmi::DispatchParamError;
	ret = knapsack.Unserialize(in_param);
	if (!ret) return rmi::DispatchParamError;

	ret = hotkey.Unserialize(in_param);
	if (!ret) return rmi::DispatchParamError;
	ret = friends.Unserialize(in_param);
	if (!ret) return rmi::DispatchParamError;
	ret = blacklists_param.Unserialize(in_param);
	if (!ret) return rmi::DispatchParamError;
	ret = enemy.Unserialize(in_param);
	if (!ret) return rmi::DispatchParamError;
	ret = business.Unserialize(in_param);
	if (!ret) return rmi::DispatchParamError;

	ret = skill_param.Unserialize(in_param);
	if (!ret) return rmi::DispatchParamError;
	ret = daily_find_param.Unserialize(in_param);
	if (!ret) return rmi::DispatchParamError;
	ret = in_param.Pop(&is_logout);
	if (!ret) return rmi::DispatchParamError;
	ret = in_param.Pop(&allow_to_cross_flag);
	if (!ret) return rmi::DispatchParamError;

	int result = RoleSave(uid, is_logout, role_p, other_p, knapsack, hotkey, friends, blacklists_param, enemy, business, 
					skill_param, daily_find_param);

	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__RoleSave_2 uid:%d result:%d", uid, result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	if (is_logout)
	{
		RoleCacheManager::Instance().OnRoleLogout(uid, allow_to_cross_flag);
	}

	return rmi::DispatchOK;
}

int RMIRoleObject::RoleSave(int role_id, 
							bool is_logout,
							const RoleInitParam &role_p, 
							const RoleOtherInitParam &other_p,
							const ItemListParam &item_list_param,
							const HotkeyListParam &hotkey,
							const FriendListParam &friends,
							const BlacklistsParam &blacklists_param,
							const EnemyListParam &enemy,
							const BusinessListParam &business, 
							const SkillListParam &skill_param,
							const DailyFindListParam &daily_find_param)
{
	if (IsCrossUser(IntToUid(role_id)))
	{
		if (CrossRoleCacheManager::Instance().RoleSave(role_id, is_logout, role_p, other_p, item_list_param, hotkey, 
			friends, blacklists_param, enemy, business, skill_param, daily_find_param))
		{
			return 0;
		}

		return -100;
	}

	INIT_STMT(conn, stmt, dataaccessrmi::role::CONNET_ERROR);

	conn->begin();

	int ret = dataaccessrmi::role::SUC;

	int result = 0; 

	int error = 0;
	if (0 != (result = SaveTableRole(stmt, role_id, is_logout, role_p, other_p, &error))) // 更新role及role_atte other_p
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::RoleSave SaveTableRole uid:%d result:%d", role_id, result);
		ret |= error;
	}
	
	if (0 != (result = SaveTableItemList(stmt, role_id, item_list_param))) // 更新背包
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::RoleSave SaveTableKnapsack uid:%d result:%d", role_id, result);
		ret |= dataaccessrmi::role::KNAPSACK_ERROR;
	}

	if (0 != (result = SaveTableHotkey(stmt, role_id, hotkey))) // 更新快捷键
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::RoleSave SaveTableHotkey uid:%d result:%d", role_id, result);
		ret |= dataaccessrmi::role::HOTKEY_ERROR;
	}
	
	if (0 != (result = SaveTableFrinend(stmt, role_id, friends))) // 更新好友
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::RoleSave SaveTableFrinend uid:%d result:%d", role_id, result);
		ret |= dataaccessrmi::role::FRINEND_ERROR;
	}

	if (0 != (result = SaveBlacklists(stmt, role_id, blacklists_param))) // 更新黑名单
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::RoleSave SaveBlacklists uid:%d result:%d", role_id, result);
		ret |= dataaccessrmi::role::BLACKLIST_ERROR;
	}

	if(0 != (result = SaveTableEnemy(stmt, role_id, enemy))) // 更新敌人
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::RoleSave SaveTableEnemy uid:%d result:%d", role_id, result);
		ret |= dataaccessrmi::role::ENEMY_ERROR;
	}

	if (0 != (result = SaveRoleSkill(stmt, role_id, skill_param))) // 更新技能
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::RoleSave SaveRoleSkill uid:%d result:%d", role_id, result);
		ret |= dataaccessrmi::role::SKILL_ERROR;
	}

	if (0 != (result = SaveDailyFind(stmt, role_id, daily_find_param))) // 更新日常找回
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::RoleSave SaveDailyFind uid:%d result:%d", role_id, result);
		ret |= dataaccessrmi::role::DAILYFIND_ERROR;
	}

	conn->commit();

	return ret;
}

int RMIRoleObject::__CreateRole(TLVUnserializer &in_param, TLVSerializer *out_param)
{	
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) * 2)
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__CreateRole_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	int db_index = 0;
	if (!in_param.Pop(&db_index))
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__CreateRole__11");
		return rmi::DispatchParamError;
	}

	static RoleInitParam p;

	if (!p.Unserialize(in_param))
	{
		return rmi::DispatchParamError;
	}

	if (out_param->MaxSize() - out_param->Size() < 32)
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__CreateRole_2");
		return rmi::DispatchOutParamBuffTooShort;
	}

	int role_id;
	int result = CreateRole(p, db_index, &role_id);
	
	if (0 != result)
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__CreateRole_3 result: %d", result);
	}

	if (!out_param->Push(result) || !out_param->Push(role_id))
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__CreateRole_4 result: %d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIRoleObject::CreateRole(const RoleInitParam &role_p, int db_index, int *role_id)
{
	int temp_role_id = 0;
	int ret = RMIRoleObject::CreateRoleId(&temp_role_id);
	if (ret < 0) return ret;

	INIT_STMT(conn, stmt, -1);

	conn->begin();
	
	const MataTableDef *roletable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE);
	DBCommand *rolecommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE);

	DataAdapter roledata = roletable->GetPrototype();
	roledata.Malloc();

	STRNCPY(roledata.m_data_area[tabledef::ROLE_ROLE_NAME].vcharp, role_p.role_name, roledata.m_data_area[tabledef::ROLE_ROLE_NAME].length);

	roledata.m_data_area[tabledef::ROLE_AVATAR].vchar = role_p.avatar;
	roledata.m_data_area[tabledef::ROLE_SEX].vchar = role_p.sex;
	roledata.m_data_area[tabledef::ROLE_COUNTRY].vchar = role_p.camp;
	roledata.m_data_area[tabledef::ROLE_SCENE_ID].vint = role_p.scene_id;
	roledata.m_data_area[tabledef::ROLE_LAST_SCENE_ID].vint = role_p.last_scene_id;
	roledata.m_data_area[tabledef::ROLE_LEVEL].vint = role_p.level;
	roledata.m_data_area[tabledef::ROLE_PROFESSIONAL].vint = role_p.prof;
	roledata.m_data_area[tabledef::ROLE_CREATE_TIME].vint64 = role_p.create_time;
	roledata.m_data_area[tabledef::ROLE_ONLINE_TIME].vint = 0;
	roledata.m_data_area[tabledef::ROLE_LAST_SAVE_TIME].vint64 = role_p.create_time;

	int real_role_id = UidToInt(UserID(db_index, temp_role_id));
	roledata.m_data_area[tabledef::ROLE_ROLE_ID].vint = real_role_id;

	if (DBCommand::RESULT_SUC != rolecommand->Save(stmt, &roledata))
	{
		roledata.Free();
		conn->rollback();
		return -1;
	}

	roledata.Free();
 
	{
		const MataTableDef *attrtable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL);
		DBCommand *attrcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL);

		DataAdapter attrdata = attrtable->GetPrototype();
		attrdata.Malloc();
		
		attrdata.m_data_area[tabledef::ROLE_ATTR_DETAIL_ROLE_ID].vint = real_role_id;
		attrdata.m_data_area[tabledef::ROLE_ATTR_DETAIL_LEVEL].vint = role_p.level;
		attrdata.m_data_area[tabledef::ROLE_ATTR_DETAIL_SEX].vchar = role_p.sex;
		attrdata.m_data_area[tabledef::ROLE_ATTR_DETAIL_PROF].vchar = role_p.prof;

		static char buff[RMIRoleAttrVerManager::ROLE_ATTR_MAX_BUFFER_LENGTH];
		TLVSerializer attr_data;
		attr_data.Reset(buff, RMIRoleAttrVerManager::ROLE_ATTR_MAX_BUFFER_LENGTH);

		if (!role_p.Serialize(attr_data, true))
		{
			attrdata.Free();
			conn->rollback();
			return -2;
		}
	
		static char hex_buff[RMIRoleAttrVerManager::ROLE_ATTR_MAX_BUFFER_LENGTH * 2 + 1] = {0};

		BinToHex(buff, attr_data.Size(), hex_buff);
		STRNCPY(attrdata.m_data_area[tabledef::ROLE_ATTR_DETAIL_ATTR_DATA].vcharp, hex_buff,
			attrdata.m_data_area[tabledef::ROLE_ATTR_DETAIL_ATTR_DATA].length);
		hex_buff[attr_data.Size() * 2] = 0;
	
		if (DBCommand::RESULT_SUC != attrcommand->Save(stmt, &attrdata))
		{
			attrdata.Free();
			conn->rollback();
			return -3;
		}

		attrdata.Free();
	}

	{
		const MataTableDef *attrtable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL2);
		DBCommand *attrcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL2);

		DataAdapter attrdata = attrtable->GetPrototype();
		attrdata.Malloc();

		attrdata.m_data_area[tabledef::ROLE_ATTR_DETAIL2_ROLE_ID].vint = real_role_id;

		if (DBCommand::RESULT_SUC != attrcommand->Save(stmt, &attrdata))
		{
			attrdata.Free();
			conn->rollback();
			return -4;
		}

		attrdata.Free();
	}

	{
		const MataTableDef *attrtable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL3);
		DBCommand *attrcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL3);

		DataAdapter attrdata = attrtable->GetPrototype();
		attrdata.Malloc();

		attrdata.m_data_area[tabledef::ROLE_ATTR_DETAIL3_ROLE_ID].vint = real_role_id;

		if (DBCommand::RESULT_SUC != attrcommand->Save(stmt, &attrdata))
		{
			attrdata.Free();
			conn->rollback();
			return -5;
		}

		attrdata.Free();
	}

	{
		const MataTableDef *attrtable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL4);
		DBCommand *attrcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL4);

		DataAdapter attrdata = attrtable->GetPrototype();
		attrdata.Malloc();

		attrdata.m_data_area[tabledef::ROLE_ATTR_DETAIL4_ROLE_ID].vint = real_role_id;

		if (DBCommand::RESULT_SUC != attrcommand->Save(stmt, &attrdata))
		{
			attrdata.Free();
			conn->rollback();
			return -6;
		}

		attrdata.Free();
	}

	{
		const MataTableDef *attrtable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL5);
		DBCommand *attrcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL5);

		DataAdapter attrdata = attrtable->GetPrototype();
		attrdata.Malloc();

		attrdata.m_data_area[tabledef::ROLE_ATTR_DETAIL5_ROLE_ID].vint = real_role_id;

		if (DBCommand::RESULT_SUC != attrcommand->Save(stmt, &attrdata))
		{
			attrdata.Free();
			conn->rollback();
			return -7;
		}

		attrdata.Free();
	}

	{
		const MataTableDef *attrtable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL6);
		DBCommand *attrcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL6);

		DataAdapter attrdata = attrtable->GetPrototype();
		attrdata.Malloc();

		attrdata.m_data_area[tabledef::ROLE_ATTR_DETAIL6_ROLE_ID].vint = real_role_id;

		if (DBCommand::RESULT_SUC != attrcommand->Save(stmt, &attrdata))
		{
			attrdata.Free();
			conn->rollback();
			return -8;
		}

		attrdata.Free();
	}

	// 角色属性表7
	{
		const MataTableDef *attrtable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL7);
		DBCommand *attrcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL7);

		DataAdapter attrdata = attrtable->GetPrototype();
		attrdata.Malloc();

		attrdata.m_data_area[tabledef::ROLE_ATTR_DETAIL7_ROLE_ID].vint = real_role_id;

		if (DBCommand::RESULT_SUC != attrcommand->Save(stmt, &attrdata))
		{
			attrdata.Free();
			conn->rollback();
			return -9;
		}

		attrdata.Free();
	}

	// 角色属性表8
	{
		const MataTableDef *attrtable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL8);
		DBCommand *attrcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL8);

		DataAdapter attrdata = attrtable->GetPrototype();
		attrdata.Malloc();

		attrdata.m_data_area[tabledef::ROLE_ATTR_DETAIL8_ROLE_ID].vint = real_role_id;

		if (DBCommand::RESULT_SUC != attrcommand->Save(stmt, &attrdata))
		{
			attrdata.Free();
			conn->rollback();
			return -10;
		}

		attrdata.Free();
	}

	// 角色属性表9
	{
		const MataTableDef *attrtable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL9);
		DBCommand *attrcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL9);

		DataAdapter attrdata = attrtable->GetPrototype();
		attrdata.Malloc();

		attrdata.m_data_area[tabledef::ROLE_ATTR_DETAIL9_ROLE_ID].vint = real_role_id;

		if (DBCommand::RESULT_SUC != attrcommand->Save(stmt, &attrdata))
		{
			attrdata.Free();
			conn->rollback();
			return -11;
		}

		attrdata.Free();
	}

	// 角色属性表10
	{
		const MataTableDef *attrtable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ATTR_DETAIL10);
		DBCommand *attrcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL10);

		DataAdapter attrdata = attrtable->GetPrototype();
		attrdata.Malloc();

		attrdata.m_data_area[tabledef::ROLE_ATTR_DETAIL10_ROLE_ID].vint = real_role_id;

		if (DBCommand::RESULT_SUC != attrcommand->Save(stmt, &attrdata))
		{
			attrdata.Free();
			conn->rollback();
			return -12;
		}

		attrdata.Free();
	}

	conn->commit();

	if (role_id) *role_id = real_role_id;

	return 0;
}

int RMIRoleObject::__DestroyRole(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	int role_id;
	if (!in_param.Pop(&role_id))
	{
		return rmi::DispatchParamError;
	}

	if (out_param->MaxSize() - out_param->Size() < 16)
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	int result = DestroyRole(role_id);

	if (!out_param->Push(result))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIRoleObject::DestroyRole(int role_id)
{
	INIT_STMT(conn, stmt, -1);
	conn->begin();

	{
		CmpNodeList role_cmp_list;
		CmpNode node_detail(tabledef::ROLE_ROLE_ID);
		node_detail.SetInt32(role_id);
		role_cmp_list.push_back(node_detail);

		DBCommand *rolecommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE);
		int count = rolecommand->Remove(stmt, role_cmp_list);
		if (count < 1)
		{
			conn->rollback();
			return -1;
		}
	}

	{
		CmpNodeList roleattrdetail_cmp_list;
		CmpNode node_detail(tabledef::ROLE_ATTR_DETAIL_ROLE_ID);
		node_detail.SetInt32(role_id);
		roleattrdetail_cmp_list.push_back(node_detail);

		DBCommand *roleattrdetailcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL);
		int count = roleattrdetailcommand->Remove(stmt, roleattrdetail_cmp_list);
		if (count < 1)
		{
			conn->rollback();
			return -1;
		}
	}

	{
		CmpNodeList roleattrdetail2_cmp_list;
		CmpNode node_detail(tabledef::ROLE_ATTR_DETAIL2_ROLE_ID);
		node_detail.SetInt32(role_id);
		roleattrdetail2_cmp_list.push_back(node_detail);

		DBCommand *roleattrdetail2command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL2);
		int count = roleattrdetail2command->Remove(stmt, roleattrdetail2_cmp_list);
		if (count < 1)
		{
			conn->rollback();
			return -2;
		}
	}

	{
		CmpNodeList roleattrdetail3_cmp_list;
		CmpNode node_detail(tabledef::ROLE_ATTR_DETAIL3_ROLE_ID);
		node_detail.SetInt32(role_id);
		roleattrdetail3_cmp_list.push_back(node_detail);

		DBCommand *roleattrdetail3command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL3);
		int count = roleattrdetail3command->Remove(stmt, roleattrdetail3_cmp_list);
		if (count < 1)
		{
			conn->rollback();
			return -3;
		}
	}

	{
		CmpNodeList roleattrdetail4_cmp_list;
		CmpNode node_detail(tabledef::ROLE_ATTR_DETAIL4_ROLE_ID);
		node_detail.SetInt32(role_id);
		roleattrdetail4_cmp_list.push_back(node_detail);

		DBCommand *roleattrdetail4command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL4);
		int count = roleattrdetail4command->Remove(stmt, roleattrdetail4_cmp_list);
		if (count < 1)
		{
			conn->rollback();
			return -4;
		}
	}

	{
		CmpNodeList roleattrdetail5_cmp_list;
		CmpNode node_detail(tabledef::ROLE_ATTR_DETAIL5_ROLE_ID);
		node_detail.SetInt32(role_id);
		roleattrdetail5_cmp_list.push_back(node_detail);

		DBCommand *roleattrdetail5command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL5);
		int count = roleattrdetail5command->Remove(stmt, roleattrdetail5_cmp_list);
		if (count < 1)
		{
			conn->rollback();
			return -5;
		}
	}

	{
		CmpNodeList roleattrdetail6_cmp_list;
		CmpNode node_detail(tabledef::ROLE_ATTR_DETAIL6_ROLE_ID);
		node_detail.SetInt32(role_id);
		roleattrdetail6_cmp_list.push_back(node_detail);

		DBCommand *roleattrdetail6command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL6);
		int count = roleattrdetail6command->Remove(stmt, roleattrdetail6_cmp_list);
		if (count < 1)
		{
			conn->rollback();
			return -6;
		}
	}

	// 角色属性表7
	{
		CmpNodeList roleattrdetail7_cmp_list;
		CmpNode node_detail(tabledef::ROLE_ATTR_DETAIL7_ROLE_ID);
		node_detail.SetInt32(role_id);
		roleattrdetail7_cmp_list.push_back(node_detail);

		DBCommand *roleattrdetail7command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL7);
		int count = roleattrdetail7command->Remove(stmt, roleattrdetail7_cmp_list);
		if (count < 1)
		{
			conn->rollback();
			return -7;
		}
	}

	// 角色属性表8
	{
		CmpNodeList roleattrdetail8_cmp_list;
		CmpNode node_detail(tabledef::ROLE_ATTR_DETAIL8_ROLE_ID);
		node_detail.SetInt32(role_id);
		roleattrdetail8_cmp_list.push_back(node_detail);

		DBCommand *roleattrdetail8command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL8);
		int count = roleattrdetail8command->Remove(stmt, roleattrdetail8_cmp_list);
		if (count < 1)
		{
			conn->rollback();
			return -8;
		}
	}

	// 角色属性表9
	{
		CmpNodeList roleattrdetail9_cmp_list;
		CmpNode node_detail(tabledef::ROLE_ATTR_DETAIL9_ROLE_ID);
		node_detail.SetInt32(role_id);
		roleattrdetail9_cmp_list.push_back(node_detail);

		DBCommand *roleattrdetail9command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL9);
		int count = roleattrdetail9command->Remove(stmt, roleattrdetail9_cmp_list);
		if (count < 1)
		{
			conn->rollback();
			return -9;
		}
	}

	// 角色属性表10
	{
		CmpNodeList roleattrdetail10_cmp_list;
		CmpNode node_detail(tabledef::ROLE_ATTR_DETAIL10_ROLE_ID);
		node_detail.SetInt32(role_id);
		roleattrdetail10_cmp_list.push_back(node_detail);

		DBCommand *roleattrdetail10command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ATTR_DETAIL10);
		int count = roleattrdetail10command->Remove(stmt, roleattrdetail10_cmp_list);
		if (count < 1)
		{
			conn->rollback();
			return -10;
		}
	}

	{
		CmpNodeList knapsack_cmp_list;
		CmpNode node_knapsack(tabledef::ITEMLIST_ROLE_ID);
		node_knapsack.SetInt32(role_id);
		knapsack_cmp_list.push_back(node_knapsack);

		DBCommand *knapsackcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ITEMLIST);
		int count = knapsackcommand->Remove(stmt, knapsack_cmp_list);
		if (count < 0)
		{
			conn->rollback();
			return -1;
		}
	}

	{
		CmpNodeList hotkey_cmp_list;
		CmpNode hotkey_node(tabledef::HOTKEY_ROLE_ID);
		hotkey_node.SetInt32(role_id);
		hotkey_cmp_list.push_back(hotkey_node);

		DBCommand *hotkeycommond = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_HOTKEY);
		int count = hotkeycommond->Remove(stmt, hotkey_cmp_list);
		if (count < 0)
		{
			conn->rollback();
			return -1;
		}
	}

	{
		CmpNodeList friend_cmp_list;
		CmpNode friend_node(tabledef::FRIENDS_ROLE_ID);
		friend_node.SetInt32(role_id);
		friend_cmp_list.push_back(friend_node);

		DBCommand *friendcommond = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_FRIENDS);
		int count = friendcommond->Remove(stmt, friend_cmp_list);
		if (count < 0)
		{
			conn->rollback();
			return -1;
		}
	}

	{
		CmpNodeList backlist_cmp_list;
		CmpNode node_back(tabledef::BLACK_LISTS_ROLE_ID);
		node_back.SetInt32(role_id);
		backlist_cmp_list.push_back(node_back);

		DBCommand *backlist_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_BLACK_LISTS);
		int count = backlist_command->Remove(stmt, backlist_cmp_list);
		if (count < 0)
		{
			conn->rollback();
			return -1;
		}
	}

	{
		CmpNodeList enemy_cmp_list;
		CmpNode enemy_node(tabledef::ENEMY_ROLE_ID);
		enemy_node.SetInt32(role_id);
		enemy_cmp_list.push_back(enemy_node);

		DBCommand *enemycommond = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ENEMY);
		int count = enemycommond->Remove(stmt, enemy_cmp_list);
		if (count < 0)
		{
			conn->rollback();
			return -1;
		}
	}

	{
		CmpNodeList skill_cmp_list;
		CmpNode node_skill(tabledef::ROLESKILL_ROLE_ID);
		node_skill.SetInt32(role_id);
		skill_cmp_list.push_back(node_skill);

		DBCommand *skillcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLESKILL);
		int count = skillcommand->Remove(stmt, skill_cmp_list);
		if (count < 0)
		{
			conn->rollback();
			return -1;
		}
	}

	{
		CmpNodeList dailyfind_cmp_list;
		CmpNode node_dailyfind(tabledef::DAILYFIND_ROLE_ID);
		node_dailyfind.SetInt32(role_id);
		dailyfind_cmp_list.push_back(node_dailyfind);

		DBCommand *dailyfind_command = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_DAILYFIND);
		int count = dailyfind_command->Remove(stmt, dailyfind_cmp_list);
		if (count < 0)
		{
			conn->rollback();
			return -1;
		}
	}

	conn->commit();

	return 0;
}

int RMIRoleObject::__RoleSaveStatus(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	int role_id = 0;
	if (!in_param.Pop(&role_id))
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__RoleSaveStatus_1 ");
		return rmi::DispatchParamError;
	}

	bool is_logout = false;
	if (!in_param.Pop(&is_logout))
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__RoleSaveStatus_2 ");
		return rmi::DispatchParamError;
	}

	bool is_micro_pc = false;
	if (!in_param.Pop(&is_micro_pc))
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__RoleSaveStatus_3 ");
		return rmi::DispatchParamError;
	}

	int result = SaveTableRoleStatus(role_id, is_logout, is_micro_pc);
	if (0 == result)
	{
		if (!out_param->Push(result))
		{
			dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__RoleSaveStatus_3 result:%d role_id:%d", result, role_id);
			return rmi::DispatchOutParamBuffTooShort;
		}
	}
	else 
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__RoleSaveStatus_4 result:%d", result);
	}

	return rmi::DispatchOK;
}

int RMIRoleObject::__FriendSave(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int))
	{
		return rmi::DispatchOutParamBuffTooShort;
	}

	int role_id = 0;
	bool ret = in_param.Pop(&role_id);
	if (!ret) 
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__FriendSave");
		return rmi::DispatchParamError;
	}

	static FriendListParam friends;
	static BlacklistsParam blacks;

	ret = friends.Unserialize(in_param);
	if (!ret) return rmi::DispatchParamError;

	ret = blacks.Unserialize(in_param);
	if (!ret) return rmi::DispatchParamError;

	int result = FriendSave(role_id, friends, blacks);
	if (!out_param->Push(result))
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__RoleSave_2 uid:%d result:%d", role_id, result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIRoleObject::FriendSave(int role_id, const FriendListParam &friends, const BlacklistsParam &blacklists)
{
	INIT_STMT(conn, stmt, dataaccessrmi::role::CONNET_ERROR);

	conn->begin();

	int ret = dataaccessrmi::role::SUC;
	int result = 0; 

	if (0 != (result = SaveTableFrinend(stmt, role_id, friends)))
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::FriendSave SaveTableFrinend role_id:%d result:%d", role_id, result);
		ret |= dataaccessrmi::role::FRINEND_ERROR;
	}

	if (0 != (result = SaveBlacklists(stmt, role_id, blacklists)))
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::FriendSave SaveBlacklists role_id:%d result:%d", role_id, result);
		ret |= dataaccessrmi::role::BLACKLIST_ERROR;
	}

	conn->commit();

	return ret;
}

int RMIRoleObject::CreateRoleId(int *role_id)
{
	INIT_STMT(conn, stmt, -101);

	conn->begin();

	const MataTableDef *roleidtable = DataBaseLayer::Instance().GetTable(tabledef::TABLE_ROLE_ID_MAP);
	DBCommand *roleidcommand = DataBaseLayer::Instance().GetCommand(tabledef::TABLE_ROLE_ID_MAP);

	DataAdapter roleiddata = roleidtable->GetPrototype();
	roleiddata.Malloc();

	roleiddata.m_data_area[tabledef::ROLE_ID_MAP_IS_USE].vint = 1;

	if (DBCommand::RESULT_SUC != roleidcommand->Save(stmt, &roleiddata))
	{
		roleiddata.Free();
		conn->rollback();
		return -102;
	}

	*role_id = (int)roleiddata.m_data_area[tabledef::ROLE_ID_MAP_IDROLE_ID_MAP].vint64;

	roleiddata.Free();
	conn->commit();

	if (*role_id >= MAX_ROLE_ID) return -103;

	return 0;
}


int RMIRoleObject::__CreateCrossRole(TLVUnserializer &in_param, TLVSerializer *out_param)
{
	if (out_param->MaxSize() - out_param->Size() < sizeof(int) * 2)
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__CreateCrossRole_1");
		return rmi::DispatchOutParamBuffTooShort;
	}

	static PlatName pname = {0};
	int cross_activity_type = 0;
	long long original_uuid = 0;

	const char *temp_platname = NULL;
	if (!in_param.Popf("sik", &temp_platname, &cross_activity_type, &original_uuid))
	{
		return rmi::DispatchParamError;
	}

	STRNCPY(pname, temp_platname, sizeof(pname));

	int role_id = 0;
	int result = CreateCrossRole(pname, cross_activity_type, original_uuid, &role_id);

	if (0 != result)
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__CreateCrossRole_2 result: %d", result);
	}

	if (!out_param->Push(result) || !out_param->Push(role_id))
	{
		dataaccesslog::g_log_role.printf(LL_CRITICAL, "RMIRoleObject::__CreateCrossRole_3 result: %d", result);
		return rmi::DispatchOutParamBuffTooShort;
	}

	return rmi::DispatchOK;
}

int RMIRoleObject::CreateCrossRole(PlatName pname, int cross_activity_type, long long original_uuid, int *role_id)
{	
	if (!CrossRoleCacheManager::Instance().CreateCrossRole(pname, cross_activity_type, original_uuid, role_id))
	{
		return -1;
	}

	return 0;
}

