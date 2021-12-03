#include "queryhandler.hpp"
#include "rmibackobjdef.h"
#include "internalcomm.h"
#include "rmiclient/rmiroleclient.h"
#include "obj/character/role.h"
#include "world.h"
#include "global/rank/rankmanager.hpp"
#include "servercommon/struct/itemlistparam.h"
#include "servercommon/serverconfig/localconfig.hpp"
#include "item/itempool.h"
#include "global/usercache/usercache.hpp"
#include "globalconfig/globalconfig.h"
#include "scene/speciallogic/worldspecial/campdefendmanager.hpp"
#include "equipment/equipmentmanager.h"
#include "other/jingling/jinglingmanager.h"
#include "other/xiannv/xiannvmanager.hpp"
#include "other/vip/vip.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "equipment/stonemananger.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/halo/halomanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/footprint/footprint.hpp"
#include "other/cloak/cloakmanager.hpp"
#include "other/qingyuan/qingyuan.hpp"
#include "other/shizhuang/shizhuang.h"
#include "other/lieming/liemingmanager.h"
#include "other/xiannvshouhu/xiannvshouhumanager.hpp"
#include "other/jinglingguanghuan/jinglingguanghuanmanager.hpp"
#include "other/jinglingfazhen/jinglingfazhenmanager.hpp"
#include "other/shenzhuang/shenzhuang.h"
#include "other/fabao/fabaomanager.hpp"
#include "other/yaoshi/yaoshimanager.hpp"
#include "other/toushi/toushimanager.hpp"
#include "other/qilinbi/qilinbimanager.hpp"
#include "other/mask/maskmanager.hpp"
#include "other/zhuanzhiequip/zhuanzhiequip.h"
#include "other/upgrade/upgrademanager.hpp"

#include "protocal/msgrole.h"
#include "other/mojie/mojiemanager.hpp"
#include "other/shenbing/shenbingmanager.hpp"
#include "global/cross/crossmanager.h"
#include "other/equipbaptize/equipbaptize.hpp"
#include "other/greatesoldier/greatesoldier.hpp"
#include "other/baizhanequip/baizhanequip.h"
#include "other/cross/rolecross.h"
#include "other/tianshenhuti/tianshenhuti.hpp"

class EquipParamHelper
{
public:
	EquipParamHelper(EquipmentParam* equip_param) : m_equip_param(equip_param) {}

	~EquipParamHelper()
	{
		if (NULL != m_equip_param)
		{
			delete m_equip_param;
			m_equip_param = NULL;
		}
	}

protected:
	EquipmentParam	*m_equip_param;
};

void RMIRoleQueryBackObjectImpl::RoleQueryRet(int ret, const RoleQueryParam &p, const ItemListParam &item_list_param)
{
	QueryHandler::Instance().OnQueryBack(query_from, viewer_user_id, plat_type, target_user_id, ret, p, item_list_param);
}

void RMIRoleQueryBackObjectImpl::__timeout()
{
	printf("RMIRoleQueryBackObjectImpl::__timeout, viewer:%d, target:%d\n", UidToInt(viewer_user_id), target_user_id);
}

//////////////////////////////////////////////////////////////////////////
QueryHandler::QueryHandler()
{

}

QueryHandler::~QueryHandler()
{

}

QueryHandler & QueryHandler::Instance()
{
	static QueryHandler qh;
	return qh;
}

void QueryHandler::OnQueryBack(int query_from, UserID viewer_userid, int plat_type, int target_uid, int ret, const RoleQueryParam &p, const ItemListParam &item_list_param)
{
	if (0 != ret)
	{
		printf("OnQueryBack query_from %d, viewer:%d, target:%d ret:%d\n", query_from, UidToInt(viewer_userid), target_uid, ret);
		return;
	}

	if (target_uid != p.role_id)
	{
		return;
	}

	OfflineQueryCacheNode tmp_cache_node;
	this->GetInfoToCacheNode(p, item_list_param, &tmp_cache_node);

	m_cache_map.erase(target_uid);

	bool is_on_rank = this->NeedToCache(IntToUid(target_uid));
	if (is_on_rank)	
	{
		m_cache_map[target_uid] = tmp_cache_node;
	}

	this->SendInfoToViewer(query_from, UidToInt(viewer_userid), plat_type, &tmp_cache_node);
}

void QueryHandler::OnBaseInfoChange(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	// 存在于缓存
	int target_uid = role->GetUID();
	std::map<int, OfflineQueryCacheNode>::iterator iter = m_cache_map.find(target_uid);
	if (iter != m_cache_map.end())
	{
		OfflineQueryCacheNode tmp_cache_node;
		this->GetInfoToCacheNode(role, &tmp_cache_node);
		m_cache_map[target_uid] = tmp_cache_node;
	}
}

void QueryHandler::OnUserLogout(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	// 存在于缓存
	int target_uid = role->GetUID();
	std::map<int, OfflineQueryCacheNode>::iterator iter = m_cache_map.find(target_uid);
	if (iter != m_cache_map.end())
	{
		OfflineQueryCacheNode tmp_cache_node;
		this->GetInfoToCacheNode(role, &tmp_cache_node);
		m_cache_map[target_uid] = tmp_cache_node;
	}
}

void QueryHandler::OnQuery(int target_uid, int query_from, int viewer_uid, int plat_type)
{
	if (0 == viewer_uid || 0 == target_uid)
	{
		return;
	}

	// 先查在线
	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(target_uid));
	if (NULL != target_role)
	{
		this->OnQueryOnline(query_from, viewer_uid, plat_type, target_role);
		return;
	}

	// 再查缓存
	std::map<int, OfflineQueryCacheNode>::iterator iter = m_cache_map.find(target_uid);
	if (iter != m_cache_map.end())
	{
		OfflineQueryCacheNode *tmp_cache_node = &iter->second;
		this->SendInfoToViewer(query_from, viewer_uid, plat_type, tmp_cache_node);
		return;
	}

	// 后查DB
	RMIRoleQueryBackObjectImpl *ribo = new RMIRoleQueryBackObjectImpl();
	ribo->query_from = query_from;
	ribo->viewer_user_id = IntToUid(viewer_uid);
	ribo->target_user_id = target_uid;
	ribo->plat_type = plat_type;

	RMIRoleClient rc;
	rc.__bind_session(InternalComm::Instance().GetDBHandler().GetRoleDB());
	bool ret = rc.RoleQueryAsyn(target_uid, ribo);
	if (!ret)
	{
		printf("OnQuery query_from %d, viewer:%d, target:%d ret:%d\n", query_from, viewer_uid,target_uid, ret);
		return;
	}
}

void QueryHandler::OnQueryOnline(int query_from, int viewer_uid, int plat_type, Role *target_role)
{
	if (0 == viewer_uid || NULL == target_role)
	{
		return;
	}

	OfflineQueryCacheNode tmp_cache_node;
	this->GetInfoToCacheNode(target_role, &tmp_cache_node);

	int target_uid = target_role->GetUID();

	m_cache_map.erase(target_uid);

	bool is_on_rank = this->NeedToCache(target_role->GetUserId());
	if (is_on_rank)	
	{
		m_cache_map[target_uid] = tmp_cache_node;
	}

	this->SendInfoToViewer(query_from, viewer_uid, plat_type, &tmp_cache_node);
}

void QueryHandler::SendInfoToViewer(int query_from, int viewer_uid, int plat_type, OfflineQueryCacheNode *cache_node)
{
	if (0 == viewer_uid || NULL == cache_node)
	{
		return;
	}

	Protocol::SCGetRoleBaseInfoAck grbi;
	// 角色信息
	{
		grbi.role_id = cache_node->role_id;
		F_STRNCPY(grbi.role_name, cache_node->role_name, sizeof(grbi.role_name));

		grbi.level = cache_node->level;
		grbi.sex = cache_node->sex;

		grbi.camp_id = cache_node->camp;
		grbi.prof = cache_node->prof;
		grbi.vip_level = cache_node->vip_level;
		grbi.guild_post = cache_node->role_init_param.guild_post;
		grbi.role_uuid = cache_node->role_uuid;

		UniqueUserID role_uuid = LongLongToUniqueUserID(cache_node->role_uuid);
		grbi.plat_type = role_uuid.plat_type;
		LocalConfig::Instance().GetPlatName(grbi.plat_name);

		grbi.is_online = World::GetInstWorld()->GetSceneManager()->IsOnLine(cache_node->role_uuid) ? (char)1 : (char)0;

		grbi.guild_id = cache_node->guild_id;
		F_STRNCPY(grbi.guild_name, cache_node->role_init_param.guild_name, sizeof(grbi.guild_name));

		grbi.avatar_timestamp = cache_node->avatar_timestamp;

		grbi.lover_uid = cache_node->common_data_param.lover_uid;
		memset(grbi.lover_name, 0, sizeof(grbi.lover_name));
		grbi.qingyuan_value = cache_node->qingyuan_param.qingyuan_value;
		grbi.qingyuan_equip_id = cache_node->common_data_param.hunjie_item_id;
		grbi.qingyuan_equip_star_level = cache_node->common_data_param.hunjie_star_level;
		grbi.lover_prof = 0;
		grbi.lover_camp = 0;
		grbi.lover_sex = 0;
		grbi.lover_vip_level = 0;
		grbi.lover_avatar_timestamp = 0;
		grbi.lover_level = 0;
		UserCacheNode *user_node = UserCacheManager::Instance().GetUserNode(cache_node->common_data_param.lover_uid);
		if (NULL != user_node)
		{
			user_node->GetName(grbi.lover_name);
			grbi.lover_prof = user_node->prof;
			grbi.lover_sex = user_node->sex;
			grbi.lover_camp = user_node->camp;
			grbi.lover_vip_level = user_node->vip_level;
			grbi.lover_avatar_timestamp = user_node->avatar_timestamp;
			grbi.lover_level = user_node->level;
		}

		grbi.evil_val = cache_node->role_init_param.evil;
		grbi.name_color = static_cast<short>(GLOBALCONFIG->GetNameColorByEvil(grbi.evil_val));
		grbi.all_charm = cache_node->role_init_param.all_charm;
		grbi.capability = cache_node->role_init_param.capability;

		grbi.hp = cache_node->role_init_param.hp;
		grbi.max_hp = cache_node->role_init_param.max_hp;
		grbi.gongji = cache_node->role_init_param.gongji;
		grbi.fangyu = cache_node->role_init_param.fangyu;
		grbi.mingzhong = cache_node->role_init_param.mingzhong;
		grbi.shanbi = cache_node->role_init_param.shanbi;
		grbi.baoji = cache_node->role_init_param.baoji;
		grbi.jianren = cache_node->role_init_param.jianren;
		grbi.fujia_shanghai = cache_node->role_init_param.fujia_shanghai;
		grbi.dikang_shanghai = cache_node->role_init_param.dikang_shanghai;
		grbi.per_jingzhun = cache_node->role_init_param.per_jingzhun;
		grbi.per_baoji = cache_node->role_init_param.per_baoji;
		grbi.per_kangbao = cache_node->role_init_param.per_kangbao;
		grbi.per_pofang = cache_node->role_init_param.per_pofang;
		grbi.per_mianshang = cache_node->role_init_param.per_mianshang;
		grbi.constant_zengshang = cache_node->role_init_param.constant_zengshang;
		grbi.constant_mianshang = cache_node->role_init_param.constant_mianshang;
		grbi.pvp_zengshang = cache_node->role_init_param.pvp_zengshang;
		grbi.pvp_mianshang = cache_node->role_init_param.pvp_mianshang;
		grbi.pve_zengshang = cache_node->role_init_param.pve_zengshang;
		grbi.pve_mianshang = cache_node->role_init_param.pve_mianshang;
		grbi.huixinyiji = cache_node->role_init_param.huixinyiji;
		grbi.huixinyiji_hurt_per = cache_node->role_init_param.huixinyiji_hurt_per;
		grbi.per_baoji_hurt = cache_node->role_init_param.per_baoji_hurt;
		grbi.per_kangbao_hurt = cache_node->role_init_param.per_kangbao_hurt;
		grbi.zhufuyiji_per = cache_node->role_init_param.zhufuyiji_per;
		grbi.gedang_per = cache_node->role_init_param.gedang_per;
		grbi.gedang_dikang_per = cache_node->role_init_param.gedang_dikang_per;
		grbi.gedang_jianshang = cache_node->role_init_param.gedang_jianshang;
		grbi.skill_zengshang = cache_node->role_init_param.skill_zengshang;
		grbi.skill_jianshang = cache_node->role_init_param.skill_jianshang;
		grbi.mingzhong_per = cache_node->role_init_param.mingzhong_per;
		grbi.shanbi_per = cache_node->role_init_param.shanbi_per;
		grbi.skill_cap_per = cache_node->role_init_param.skill_cap_per;

	}

	// 坐骑信息
	{
		grbi.mount_info.level = cache_node->mount_param.level;
		grbi.mount_info.grade = cache_node->mount_param.grade;
		grbi.mount_info.used_imageid = cache_node->mount_param.temporary_imageid > 0 ? cache_node->mount_param.temporary_imageid : cache_node->mount_param.used_imageid;
		ARRAYCPY(grbi.mount_info.shuxingdan_list, cache_node->mount_param.shuxingdan_list);
		ARRAYCPY(grbi.mount_info.active_special_image_flag, cache_node->mount_param.active_special_image_flag);
		grbi.mount_info.star_level = cache_node->mount_param.star_level;
		grbi.mount_info.equip_skill_level = cache_node->mount_param.equip_skill_level;
		grbi.mount_info.capability = cache_node->mount_param.capability;
		memcpy(grbi.mount_info.equip_level_list, cache_node->mount_param.equip_level_list, sizeof(grbi.mount_info.equip_level_list));
		memcpy(grbi.mount_info.skill_level_list, cache_node->mount_param.skill_level_list, sizeof(grbi.mount_info.skill_level_list));
	}

	// 羽翼信息
	{
		grbi.wing_info.level = cache_node->wing_param.level;
		grbi.wing_info.grade = cache_node->wing_param.grade;
		grbi.wing_info.used_imageid = cache_node->wing_param.temporary_imageid > 0 ? cache_node->wing_param.temporary_imageid : cache_node->wing_param.used_imageid;
		ARRAYCPY(grbi.wing_info.shuxingdan_list, cache_node->wing_param.shuxingdan_list);
		ARRAYCPY(grbi.wing_info.active_special_image_flag, cache_node->wing_param.active_special_image_flag);
		grbi.wing_info.star_level = cache_node->wing_param.star_level;
		grbi.wing_info.equip_skill_level = cache_node->wing_param.equip_skill_level;
		grbi.wing_info.capability = cache_node->wing_param.capability;
		memcpy(grbi.wing_info.equip_level_list, cache_node->wing_param.equip_level_list, sizeof(grbi.wing_info.equip_level_list));
		memcpy(grbi.wing_info.skill_level_list, cache_node->wing_param.skill_level_list, sizeof(grbi.wing_info.skill_level_list));
	}

	// 光环信息
	{
		grbi.halo_info.level = cache_node->halo_param.level;
		grbi.halo_info.grade = cache_node->halo_param.grade;
		grbi.halo_info.used_imageid = cache_node->halo_param.used_imageid;
		grbi.halo_info.shuxingdan_count = cache_node->halo_param.shuxingdan_count;
		grbi.halo_info.chengzhangdan_count = cache_node->halo_param.chengzhangdan_count;
		grbi.halo_info.star_level = cache_node->halo_param.star_level;
		UNSTD_STATIC_CHECK(sizeof(grbi.halo_info.active_special_image_flag) == sizeof(cache_node->halo_param.active_special_image_flag));
		memcpy(grbi.halo_info.active_special_image_flag, cache_node->halo_param.active_special_image_flag, sizeof(grbi.halo_info.active_special_image_flag));
		grbi.halo_info.equip_skill_level = cache_node->halo_param.equip_skill_level;
		grbi.halo_info.capability = cache_node->halo_param.capability;
		memcpy(grbi.halo_info.equip_level_list, cache_node->halo_param.equip_level_list, sizeof(grbi.halo_info.equip_level_list));
		memcpy(grbi.halo_info.skill_level_list, cache_node->halo_param.skill_level_list, sizeof(grbi.halo_info.skill_level_list));
	}

	// 法宝信息
	{
		grbi.fabao_info.grade = cache_node->fabao_param.grade;
		grbi.fabao_info.used_imageid = cache_node->fabao_param.used_imageid;
		grbi.fabao_info.shuxingdan_count = cache_node->fabao_param.shuxingdan_count;
		grbi.fabao_info.used_special_imageid = cache_node->fabao_param.is_used_special_imaged > 0 ? cache_node->fabao_param.used_special_id : 0;                                                                    // 对齐字节,给它赋个0
		UNSTD_STATIC_CHECK(sizeof(grbi.fabao_info.active_special_image_flag) == sizeof(cache_node->fabao_param.active_special_image_flag));
		memcpy(grbi.fabao_info.active_special_image_flag, cache_node->fabao_param.active_special_image_flag, sizeof(grbi.fabao_info.active_special_image_flag));
		grbi.fabao_info.equip_skill_level = cache_node->fabao_param.equip_skill_level;
		grbi.fabao_info.capability = cache_node->fabao_param.capability;
		memcpy(grbi.fabao_info.equip_level_list, cache_node->fabao_param.equip_level_list, sizeof(grbi.fabao_info.equip_level_list));
		memcpy(grbi.fabao_info.skill_level_list, cache_node->fabao_param.skill_level_list, sizeof(grbi.fabao_info.skill_level_list));
	}

	// 神弓信息
	{
		grbi.shengong_info.level = cache_node->shengong_param.level;
		grbi.shengong_info.grade = cache_node->shengong_param.grade;
		grbi.shengong_info.used_imageid = cache_node->shengong_param.used_imageid;
		grbi.shengong_info.shuxingdan_count = cache_node->shengong_param.shuxingdan_count;
		grbi.shengong_info.chengzhangdan_count = cache_node->shengong_param.chengzhangdan_count;
		grbi.shengong_info.star_level = cache_node->shengong_param.star_level;
		UNSTD_STATIC_CHECK(sizeof(grbi.shengong_info.active_special_image_flag) == sizeof(cache_node->shengong_param.active_special_image_flag));
		memcpy(grbi.shengong_info.active_special_image_flag, cache_node->shengong_param.active_special_image_flag, sizeof(grbi.shengong_info.active_special_image_flag));
		grbi.shengong_info.equip_skill_level = cache_node->shengong_param.equip_skill_level;
		grbi.shengong_info.capability = cache_node->shengong_param.capability;
		for (int i = 0; i < ShengongParam::EQUIP_COUNT; i++)
		{
			grbi.shengong_info.equip_level_list[i] = cache_node->shengong_param.equip_level_list[i];
		}
		memcpy(grbi.shengong_info.skill_level_list, cache_node->shengong_param.skill_level_list, sizeof(grbi.shengong_info.skill_level_list));
	}

	// 神翼信息
	{
		grbi.shenyi_info.level = cache_node->shenyi_param.level;
		grbi.shenyi_info.grade = cache_node->shenyi_param.grade;
		grbi.shenyi_info.used_imageid = cache_node->shenyi_param.used_imageid;
		grbi.shenyi_info.shuxingdan_count = cache_node->shenyi_param.shuxingdan_count;
		grbi.shenyi_info.chengzhangdan_count = cache_node->shenyi_param.chengzhangdan_count;
		grbi.shenyi_info.star_level = cache_node->shenyi_param.star_level;
		UNSTD_STATIC_CHECK(sizeof(grbi.shenyi_info.active_special_image_flag) == sizeof(cache_node->shenyi_param.active_special_image_flag));
		memcpy(grbi.shenyi_info.active_special_image_flag, cache_node->shenyi_param.active_special_image_flag, sizeof(grbi.shenyi_info.active_special_image_flag));
		grbi.shenyi_info.equip_skill_level = cache_node->shenyi_param.equip_skill_level;
		grbi.shenyi_info.capability = cache_node->shenyi_param.capability;
		for (int i = 0; i < ShenyiParam::EQUIP_COUNT; i++)
		{
			grbi.shenyi_info.equip_level_list[i] = cache_node->shenyi_param.equip_level_list[i];
		}
		memcpy(grbi.shenyi_info.skill_level_list, cache_node->shenyi_param.skill_level_list, sizeof(grbi.shenyi_info.skill_level_list));
	}

	// 足迹信息
	{
		grbi.footprint_info.level = cache_node->footprint_param.level;
		grbi.footprint_info.grade = cache_node->footprint_param.grade;
		grbi.footprint_info.used_imageid = cache_node->footprint_param.used_imageid;
		grbi.footprint_info.shuxingdan_count = cache_node->footprint_param.shuxingdan_count;
		grbi.footprint_info.chengzhangdan_count = cache_node->footprint_param.chengzhangdan_count;
		grbi.footprint_info.star_level = cache_node->footprint_param.star_level;
		UNSTD_STATIC_CHECK(sizeof(grbi.footprint_info.active_special_image_flag) == sizeof(cache_node->footprint_param.active_special_image_flag));
		memcpy(grbi.footprint_info.active_special_image_flag, cache_node->footprint_param.active_special_image_flag, sizeof(grbi.footprint_info.active_special_image_flag));
		grbi.footprint_info.equip_skill_level = cache_node->footprint_param.equip_skill_level;
		grbi.footprint_info.capability = cache_node->footprint_param.capability;
		for (int i = 0; i < FootprintParam::EQUIP_COUNT; i++)
		{
			grbi.footprint_info.equip_level_list[i] = cache_node->footprint_param.equip_level_list[i];
		}
		memcpy(grbi.footprint_info.skill_level_list, cache_node->footprint_param.skill_level_list, sizeof(grbi.footprint_info.skill_level_list));
	}

	// 披风信息
	{
		grbi.cloak_info.level = cache_node->cloak_param.level;
		grbi.cloak_info.used_imageid = cache_node->cloak_param.used_imageid;
		grbi.cloak_info.active_special_image_flag = cache_node->cloak_param.active_special_image_flag;
		grbi.cloak_info.equip_skill_level = cache_node->cloak_param.equip_skill_level;
		grbi.cloak_info.capability = cache_node->cloak_param.capability;

		ARRAYCPY(grbi.cloak_info.shuxingdan_list, cache_node->cloak_param.shuxingdan_list);
		for (int i = 0; i < FootprintParam::EQUIP_COUNT; i++)
		{
			grbi.cloak_info.equip_level_list[i] = cache_node->cloak_param.equip_level_list[i];
		}
		memcpy(grbi.cloak_info.skill_level_list, cache_node->cloak_param.skill_level_list, sizeof(grbi.cloak_info.skill_level_list));
	}

	// 仙女守护
	{
		grbi.xiannv_shouhu_info.grade = cache_node->xiannv_shouhu.grade;
		grbi.xiannv_shouhu_info.used_imageid = cache_node->xiannv_shouhu.used_imageid;
	}

	// 精灵法阵
	{
		grbi.jingling_fazhen_info.grade = cache_node->jingling_fazhen.grade;
		grbi.jingling_fazhen_info.used_imageid = cache_node->jingling_fazhen.used_imageid;
	}

	// 精灵光环
	{
		grbi.jingling_guanghuan_info.grade = cache_node->jingling_guanghuan.grade;
		grbi.jingling_guanghuan_info.used_imageid = cache_node->jingling_guanghuan.used_imageid;
	}

	// 至宝信息
	{
		grbi.zhibao_info.level = cache_node->zhibao_param.level;
		grbi.zhibao_info.used_imageid = cache_node->zhibao_param.use_image;
	}

	// 战斗坐骑信息
	{
		grbi.fight_mount_info.level = cache_node->fight_mount_param.level;
		grbi.fight_mount_info.grade = cache_node->fight_mount_param.grade;
		grbi.fight_mount_info.used_imageid = cache_node->fight_mount_param.used_imageid;
		grbi.fight_mount_info.shuxingdan_count = cache_node->fight_mount_param.shuxingdan_count;
		grbi.fight_mount_info.chengzhangdan_count = cache_node->fight_mount_param.chengzhangdan_count;
		grbi.fight_mount_info.star_level = cache_node->fight_mount_param.star_level;
		UNSTD_STATIC_CHECK(sizeof(grbi.fight_mount_info.active_special_image_flag) == sizeof(cache_node->fight_mount_param.active_special_image_flag));
		memcpy(grbi.fight_mount_info.active_special_image_flag, cache_node->fight_mount_param.active_special_image_flag, sizeof(grbi.fight_mount_info.active_special_image_flag));
		grbi.fight_mount_info.equip_skill_level = cache_node->fight_mount_param.equip_skill_level;
		grbi.fight_mount_info.capability = cache_node->fight_mount_param.capability;
		for (int i = 0; i < FightMountParam::EQUIP_COUNT; i++)
		{
			grbi.fight_mount_info.equip_level_list[i] = cache_node->fight_mount_param.equip_level_list[i];
		}
		memcpy(grbi.fight_mount_info.skill_level_list, cache_node->fight_mount_param.skill_level_list, sizeof(grbi.fight_mount_info.skill_level_list));
	}

	// 装备信息
	for (int i = 0; i < ItemNamespace::MAX_EQUIPMENT_GRID_NUM; i ++)					
	{
		grbi.equipment_info[i].equip_id = cache_node->equipment_info[i].equip_id;
		grbi.equipment_info[i].star_level = cache_node->equipment_info[i].star_level;
		grbi.equipment_info[i].has_lucky = cache_node->equipment_info[i].has_lucky;
		grbi.equipment_info[i].quality = cache_node->equipment_info[i].quality;
		grbi.equipment_info[i].strengthen_level = cache_node->equipment_info[i].strengthen_level;
		grbi.equipment_info[i].shen_level = cache_node->equipment_info[i].shen_level;
		grbi.equipment_info[i].fuling_level = cache_node->equipment_info[i].fuling_level;
		
		//装备格子属性
		grbi.equipment_info[i].grid_strengthen_level = cache_node->equipment_info[i].grid_strengthen_level;
		grbi.equipment_info[i].grid_shen_level = cache_node->equipment_info[i].grid_shen_level;
		grbi.equipment_info[i].grid_star_level = cache_node->equipment_info[i].grid_star_level;
		grbi.equipment_info[i].eternity_level = cache_node->equipment_info[i].eternity_level;

		//传说属性
		UNSTD_STATIC_CHECK(sizeof(grbi.equipment_info[i].xianpin_type_list) == sizeof(cache_node->equipment_info[i].xianpin_type_list));
		memcpy(grbi.equipment_info[i].xianpin_type_list, cache_node->equipment_info[i].xianpin_type_list, sizeof(grbi.equipment_info[i].xianpin_type_list));
	}

	{
		//宝石
		grbi.stone_param = cache_node->stone_param;
	}

	// 时装
	{
		for (int i = 0; i < SHIZHUANG_TYPE_MAX; ++ i)
		{
			grbi.shizhuang_part_list[i].use_img = cache_node->shizhuang_param.item_list[i].use_img;
			grbi.shizhuang_part_list[i].use_special_img = cache_node->shizhuang_param.item_list[i].use_special_img;
			grbi.shizhuang_part_list[i].grade = cache_node->shizhuang_param.item_list[i].grade;
			grbi.shizhuang_part_list[i].reserve_ch = 0;                                                                 // 字节对齐，赋0
			grbi.shizhuang_part_list[i].shuxingdan_count = cache_node->shizhuang_param.item_list[i].shuxingdan_count;
			grbi.shizhuang_part_list[i].capability = cache_node->shizhuang_param.item_list[i].capability;                                                                  // 字节对齐，赋0
			grbi.shizhuang_part_list[i].equip_skill_level = cache_node->shizhuang_param.item_list[i].equip_skill_level;
			memcpy(grbi.shizhuang_part_list[i].equip_level_list, cache_node->shizhuang_param.item_list[i].equip_level_list, sizeof(grbi.shizhuang_part_list[i].equip_level_list));
			memcpy(grbi.shizhuang_part_list[i].skill_level_list, cache_node->shizhuang_param.item_list[i].skill_level_list, sizeof(grbi.shizhuang_part_list[i].skill_level_list));
		}
	}

	// 仙女
	{
		grbi.xiannv_info.active_xiannv_flag = cache_node->xiannv_param.active_xiannv_flag;
		UNSTD_STATIC_CHECK(sizeof(grbi.xiannv_info.active_huanhua_flag) == sizeof(cache_node->xiannv_param.active_huanhua_flag));
		memcpy(grbi.xiannv_info.active_huanhua_flag, cache_node->xiannv_param.active_huanhua_flag, sizeof(grbi.xiannv_info.active_huanhua_flag));
		grbi.xiannv_info.huanhua_id = cache_node->xiannv_param.huanhua_id;
		for (int i = 0; i < MAX_XIANNV_HUANHUA_COUNT; i ++)
		{
			grbi.xiannv_info.xiannv_huanhua_level[i] = cache_node->xiannv_param.xiannv_huanhua_level[i];
		}

		for (int i = 0; i < MAX_XIANNV_COUNT; i ++)
		{
			grbi.xiannv_info.xiannv_item_list[i].xn_level = cache_node->xiannv_param.xn_item_list[i].xn_level;
			grbi.xiannv_info.xiannv_item_list[i].xn_zizhi = cache_node->xiannv_param.xn_item_list[i].xn_zizhi;
		}

		memcpy(grbi.xiannv_info.pos_list, cache_node->xiannv_param.pos_list, sizeof(grbi.xiannv_info.pos_list));
		int xiannv_id = grbi.xiannv_info.pos_list[0];
		if (xiannv_id >= 0)
		{
			memcpy(grbi.xiannv_info.xiannv_name, cache_node->xiannv_param.xiannv_name[xiannv_id], sizeof(grbi.xiannv_info.xiannv_name));
		}

		//策划需求，神弓神翼也算进仙女中，所以加上
		grbi.xiannv_info.capability = cache_node->xiannv_param.capability + cache_node->shengong_param.capability + cache_node->shenyi_param.capability;
	}

	// 精灵
	{
		for (int i = 0; i < JINGLING_MAX_TAKEON_NUM; i ++)
		{
			grbi.jingling_info.jinling_item_list[i].jingling_id = cache_node->jingling_param.jingling_list[i].item_id;
			grbi.jingling_info.jinling_item_list[i].jingling_level = cache_node->jingling_param.jingling_list[i].level;
			grbi.jingling_info.jinling_item_list[i].is_bind = cache_node->jingling_param.jingling_list[i].is_bind;
			for (int j = 0; j < ItemNamespace::MAX_XIANPIN_NUM; j ++)
			{
				grbi.jingling_info.jinling_item_list[i].talent_list[j] = cache_node->jingling_param.jingling_list[i].talent_list[j];
			}
		}

		for (int i = 0; i < JINGLING_PTHANTOM_MAX_TYPE; i ++)
		{
			grbi.jingling_info.phantom_level_list[i] = cache_node->jingling_param.phantom_level_list[i];
		}

		grbi.jingling_info.use_jingling_id = cache_node->jingling_param.use_jingling_id;
		UNSTD_STATIC_CHECK(sizeof(grbi.jingling_info.phantom_image_flag) == sizeof(cache_node->jingling_param.phantom_image_flag));
		memcpy(grbi.jingling_info.phantom_image_flag, cache_node->jingling_param.phantom_image_flag, sizeof(grbi.jingling_info.phantom_image_flag));
		grbi.jingling_info.phantom_imgageid = cache_node->jingling_param.phantom_imageid;
	}

	// 命魂
	//for (int i = 0; i < LIEMING_FUHUN_SLOT_COUNT; i ++)
	//{
	//	grbi.slot_list[i].hunshou_id = cache_node->lieming_param.slot_list[i].hunshou_id;
	//	grbi.slot_list[i].level = cache_node->lieming_param.slot_list[i].level;
	//	grbi.slot_list[i].curr_exp = cache_node->lieming_param.slot_list[i].curr_exp;
	//}

	// 神装
	memcpy(grbi.szlevel_list, cache_node->shenzhuang_param.szlevel_list, sizeof(grbi.szlevel_list));

	// 腰饰信息
	{
		grbi.yaoshi_info.level = cache_node->yaoshi_param.level;
		grbi.yaoshi_info.grade = cache_node->yaoshi_param.grade;
		grbi.yaoshi_info.used_imageid = cache_node->yaoshi_param.temporary_imageid > 0 ? cache_node->yaoshi_param.temporary_imageid : cache_node->yaoshi_param.used_imageid;
		UNSTD_STATIC_CHECK(sizeof(grbi.yaoshi_info.active_special_image_flag) == sizeof(cache_node->yaoshi_param.active_special_image_flag));
		memcpy(grbi.yaoshi_info.active_special_image_flag, cache_node->yaoshi_param.active_special_image_flag, sizeof(grbi.yaoshi_info.active_special_image_flag));
		grbi.yaoshi_info.star_level = cache_node->yaoshi_param.star_level;
		grbi.yaoshi_info.equip_skill_level = cache_node->yaoshi_param.equip_skill_level;
		grbi.yaoshi_info.capability = cache_node->yaoshi_param.capability;
		ARRAYCPY(grbi.yaoshi_info.shuxingdan_list, cache_node->yaoshi_param.shuxingdan_list);
		memcpy(grbi.yaoshi_info.equip_level_list, cache_node->yaoshi_param.equip_level_list, sizeof(grbi.yaoshi_info.equip_level_list));
		memcpy(grbi.yaoshi_info.skill_level_list, cache_node->yaoshi_param.skill_level_list, sizeof(grbi.yaoshi_info.skill_level_list));
	}

	// 头饰信息
	{
		grbi.toushi_info.level = cache_node->toushi_param.level;
		grbi.toushi_info.grade = cache_node->toushi_param.grade;
		grbi.toushi_info.used_imageid = cache_node->toushi_param.temporary_imageid > 0 ? cache_node->toushi_param.temporary_imageid : cache_node->toushi_param.used_imageid;
		UNSTD_STATIC_CHECK(sizeof(grbi.toushi_info.active_special_image_flag) == sizeof(cache_node->toushi_param.active_special_image_flag));
		memcpy(grbi.toushi_info.active_special_image_flag, cache_node->toushi_param.active_special_image_flag, sizeof(grbi.toushi_info.active_special_image_flag));
		grbi.toushi_info.star_level = cache_node->toushi_param.star_level;
		grbi.toushi_info.equip_skill_level = cache_node->toushi_param.equip_skill_level;
		grbi.toushi_info.capability = cache_node->toushi_param.capability;
		ARRAYCPY(grbi.toushi_info.shuxingdan_list, cache_node->toushi_param.shuxingdan_list);
		memcpy(grbi.toushi_info.equip_level_list, cache_node->toushi_param.equip_level_list, sizeof(grbi.toushi_info.equip_level_list));
		memcpy(grbi.toushi_info.skill_level_list, cache_node->toushi_param.skill_level_list, sizeof(grbi.toushi_info.skill_level_list));
	}

	// 麒麟臂信息
	{
		grbi.qilinbi_info.level = cache_node->qilinbi_param.level;
		grbi.qilinbi_info.grade = cache_node->qilinbi_param.grade;
		grbi.qilinbi_info.used_imageid = cache_node->qilinbi_param.temporary_imageid > 0 ? cache_node->qilinbi_param.temporary_imageid : cache_node->qilinbi_param.used_imageid;
		UNSTD_STATIC_CHECK(sizeof(grbi.qilinbi_info.active_special_image_flag) == sizeof(cache_node->qilinbi_param.active_special_image_flag));
		memcpy(grbi.qilinbi_info.active_special_image_flag, cache_node->qilinbi_param.active_special_image_flag, sizeof(grbi.qilinbi_info.active_special_image_flag));
		grbi.qilinbi_info.star_level = cache_node->qilinbi_param.star_level;
		grbi.qilinbi_info.equip_skill_level = cache_node->qilinbi_param.equip_skill_level;
		grbi.qilinbi_info.capability = cache_node->qilinbi_param.capability;
		ARRAYCPY(grbi.qilinbi_info.shuxingdan_list, cache_node->qilinbi_param.shuxingdan_list);
		memcpy(grbi.qilinbi_info.equip_level_list, cache_node->qilinbi_param.equip_level_list, sizeof(grbi.qilinbi_info.equip_level_list));
		memcpy(grbi.qilinbi_info.skill_level_list, cache_node->qilinbi_param.skill_level_list, sizeof(grbi.qilinbi_info.skill_level_list));
	}

	// 面具信息
	{
		grbi.mask_info.level = cache_node->mask_param.level;
		grbi.mask_info.grade = cache_node->mask_param.grade;
		grbi.mask_info.used_imageid = cache_node->mask_param.temporary_imageid > 0 ? cache_node->mask_param.temporary_imageid : cache_node->mask_param.used_imageid;
		UNSTD_STATIC_CHECK(sizeof(grbi.mask_info.active_special_image_flag) == sizeof(cache_node->mask_param.active_special_image_flag));
		memcpy(grbi.mask_info.active_special_image_flag, cache_node->mask_param.active_special_image_flag, sizeof(grbi.mask_info.active_special_image_flag));
		grbi.mask_info.star_level = cache_node->mask_param.star_level;
		grbi.mask_info.equip_skill_level = cache_node->mask_param.equip_skill_level;
		grbi.mask_info.capability = cache_node->mask_param.capability;
		ARRAYCPY(grbi.mask_info.shuxingdan_list, cache_node->mask_param.shuxingdan_list);
		memcpy(grbi.mask_info.equip_level_list, cache_node->mask_param.equip_level_list, sizeof(grbi.mask_info.equip_level_list));
		memcpy(grbi.mask_info.skill_level_list, cache_node->mask_param.skill_level_list, sizeof(grbi.mask_info.skill_level_list));
	}
	
	// 飞仙装备信息
	for (int i = 0; i < FeiXianParam::E_INDEX_MAX; i++)
	{
		grbi.feixian_info[i].item_id = cache_node->feixian_param.equip_list[i].item_id;
		grbi.feixian_info[i].is_bind = cache_node->feixian_param.equip_list[i].is_bind;
		grbi.feixian_info[i].invalid_time = cache_node->feixian_param.equip_list[i].invalid_time;
		
		//传说属性
		UNSTD_STATIC_CHECK(sizeof(grbi.feixian_info[i].xianpin_type_list) == sizeof(cache_node->feixian_param.equip_list[i].param_data.xianpin_type_list));
		memcpy(grbi.feixian_info[i].xianpin_type_list, cache_node->feixian_param.equip_list[i].param_data.xianpin_type_list, sizeof(grbi.feixian_info[i].xianpin_type_list));
	}

	//守护小鬼
	{
		for (int i = 0; i < ImpGuard::IMP_GUARD_GRID_INDEX_MAX; ++i)
		{
			grbi.impguard_info.impguard_use_type[i] = cache_node->common_data_param.imp_guard_type[i];
			grbi.impguard_info.impguard_invaild_time[i] = cache_node->common_data_param.imp_guard_end_timestamp[i];
		}
	}

	//魔戒
	{
		for (int i = 0; i< MojieParam::MOJIE_MAX_TYPE; ++i)
		{
			grbi.mojie_info.mojie_level_list[i].mojie_type = i;
			grbi.mojie_info.mojie_level_list[i].mojie_level = static_cast<char>(cache_node->mojie_param.mojie_list[i].mojie_level);
		}
	}

	// 转职装备
	{
		grbi.zhuanzhi_equip_info.capability = cache_node->zhuanzhi_equip_param.capability;

		for (int i = 0; i < E_INDEX_ZHUANZHI::E_INDEX_MAX; i ++)
		{
			ZhuanzhiEquipItem &equip_item = cache_node->zhuanzhi_equip_param.zhuanzhi_equip_list[i];
			grbi.zhuanzhi_equip_info.zhuanzhi_equip_list[i].equip_id = equip_item.zhuanzhi_equip_data.item_id;
			grbi.zhuanzhi_equip_info.zhuanzhi_equip_list[i].star_level = equip_item.star_level;
			grbi.zhuanzhi_equip_info.zhuanzhi_equip_list[i].star_exp = equip_item.star_exp;

			UNSTD_STATIC_CHECK(sizeof(grbi.zhuanzhi_equip_info.zhuanzhi_equip_list[i].fuling_count_list) == sizeof(equip_item.fuling_count_list));
			memcpy(grbi.zhuanzhi_equip_info.zhuanzhi_equip_list[i].fuling_count_list, equip_item.fuling_count_list, sizeof(grbi.zhuanzhi_equip_info.zhuanzhi_equip_list[i].fuling_count_list));

			UNSTD_STATIC_CHECK(sizeof(grbi.zhuanzhi_equip_info.zhuanzhi_equip_list[i].xianpin_type_list) == sizeof(equip_item.zhuanzhi_equip_data.param_data.xianpin_type_list));
			memcpy(grbi.zhuanzhi_equip_info.zhuanzhi_equip_list[i].xianpin_type_list, equip_item.zhuanzhi_equip_data.param_data.xianpin_type_list, sizeof(grbi.zhuanzhi_equip_info.zhuanzhi_equip_list[i].xianpin_type_list));
		}

		UNSTD_STATIC_CHECK(sizeof(grbi.zhuanzhi_equip_info.stone_list) == sizeof(cache_node->zhuanzhi_equip_param.stone_list));
		memcpy(grbi.zhuanzhi_equip_info.stone_list, cache_node->zhuanzhi_equip_param.stone_list, sizeof(grbi.zhuanzhi_equip_info.stone_list));

		for (int i = 0; i < ZHUANZHI_SUIT_TYPE_MAX; ++i)
		{
			for (int part = 0; part < E_INDEX_ZHUANZHI::E_INDEX_MAX; ++part)
			{
				grbi.zhuanzhi_equip_info.part_suit_type_list[part] = 0;
				grbi.zhuanzhi_equip_info.part_order_list[part] = 0;

				if (cache_node->zhuanzhi_equip_param.zhuanzhi_suit_order_list[i].order_list[part] > 0)
				{
					grbi.zhuanzhi_equip_info.part_suit_type_list[part] = i;
					grbi.zhuanzhi_equip_info.part_order_list[part] = cache_node->zhuanzhi_equip_param.zhuanzhi_suit_order_list[i].order_list[part];
				}
			}
		}

		UNSTD_STATIC_CHECK(sizeof(grbi.zhuanzhi_equip_info.zhuanzhi_equip_awakening_list) == sizeof(cache_node->zhuanzhi_equip_param.zhuanzhi_equip_awakening_list));
		memcpy(grbi.zhuanzhi_equip_info.zhuanzhi_equip_awakening_list, cache_node->zhuanzhi_equip_param.zhuanzhi_equip_awakening_list, sizeof(grbi.zhuanzhi_equip_info.zhuanzhi_equip_awakening_list));
		UNSTD_STATIC_CHECK(sizeof(grbi.zhuanzhi_equip_info.zhuanzhi_equip_awakening_lock_flag) == sizeof(cache_node->zhuanzhi_equip_param.zhuanzhi_equip_awakening_lock_flag));
		memcpy(grbi.zhuanzhi_equip_info.zhuanzhi_equip_awakening_lock_flag, cache_node->zhuanzhi_equip_param.zhuanzhi_equip_awakening_lock_flag, sizeof(grbi.zhuanzhi_equip_info.zhuanzhi_equip_awakening_lock_flag));
	}

	// 进阶类系统
	{
		for (int upgrade_type = 0; upgrade_type < UPGRADE_TYPE_MAX; upgrade_type ++)
		{
			UpgradeBaseData &base_data = cache_node->upgrade_manager_param.data_list[upgrade_type];
			grbi.upgrade_sys_info[upgrade_type].level = base_data.level;
			grbi.upgrade_sys_info[upgrade_type].grade = base_data.grade;
			grbi.upgrade_sys_info[upgrade_type].used_imageid = base_data.temporary_imageid > 0 ? base_data.temporary_imageid : base_data.used_imageid;
			grbi.upgrade_sys_info[upgrade_type].star_level = base_data.star_level;
			grbi.upgrade_sys_info[upgrade_type].equip_skill_level = base_data.equip_skill_level;
			grbi.upgrade_sys_info[upgrade_type].capability = base_data.capability;

			UNSTD_STATIC_CHECK(sizeof(grbi.upgrade_sys_info[upgrade_type].shuxingdan_list) == sizeof(base_data.shuxingdan_list));
			memcpy(grbi.upgrade_sys_info[upgrade_type].shuxingdan_list, base_data.shuxingdan_list, sizeof(grbi.upgrade_sys_info[upgrade_type].shuxingdan_list));

			UNSTD_STATIC_CHECK(sizeof(grbi.upgrade_sys_info[upgrade_type].active_img_flag) == sizeof(base_data.active_img_flag));
			memcpy(grbi.upgrade_sys_info[upgrade_type].active_img_flag, base_data.active_img_flag, sizeof(grbi.upgrade_sys_info[upgrade_type].active_img_flag));

			UNSTD_STATIC_CHECK(sizeof(grbi.upgrade_sys_info[upgrade_type].equip_level_list) == sizeof(base_data.equip_level_list));
			memcpy(grbi.upgrade_sys_info[upgrade_type].equip_level_list, base_data.equip_level_list, sizeof(grbi.upgrade_sys_info[upgrade_type].equip_level_list));

			UNSTD_STATIC_CHECK(sizeof(grbi.upgrade_sys_info[upgrade_type].skill_level_list) == sizeof(base_data.skill_level_list));
			memcpy(grbi.upgrade_sys_info[upgrade_type].skill_level_list, base_data.skill_level_list, sizeof(grbi.upgrade_sys_info[upgrade_type].skill_level_list));
		}
	}

	{
		// 神器
		grbi.shenbing_use_image_id = cache_node->common_data_param.shenbing_use_image_id;
		grbi.baojia_use_image_id = cache_node->common_data_param.baojia_use_image_id;
	}

	{// 灵刃
		grbi.shenbing_info.level = cache_node->shenbing_param.level;
		grbi.shenbing_info.use_image = cache_node->shenbing_param.use_image;
		grbi.shenbing_info.capability = cache_node->shenbing_param.capability;
		UNSTD_STATIC_CHECK(sizeof(grbi.shenbing_info.shuxingdan_list) == sizeof(cache_node->shenbing_param.shuxingdan_list));
		memcpy(grbi.shenbing_info.shuxingdan_list, cache_node->shenbing_param.shuxingdan_list, sizeof(grbi.shenbing_info.shuxingdan_list));
	}

	{ // 装备洗炼 
		UNSTD_STATIC_CHECK(sizeof(grbi.equip_baptize_info) == sizeof(cache_node->equipbaptize_param.part_info_list));
		UNSTD_STATIC_CHECK((sizeof(grbi.equip_baptize_info) / sizeof(grbi.equip_baptize_info[0])) == EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM);
		UNSTD_STATIC_CHECK(sizeof(grbi.equip_baptize_info[0].attr_seq_list) == sizeof(cache_node->equipbaptize_param.part_info_list[0].attr_seq_list));
		UNSTD_STATIC_CHECK(sizeof(grbi.equip_baptize_info[0].baptize_list) == sizeof(cache_node->equipbaptize_param.part_info_list[0].baptize_list));
		for (int i = 0; i < EQUIP_BAPTIZE_EQUIP_GRID_MAX_NUM; ++i)
		{
			memcpy(grbi.equip_baptize_info[i].attr_seq_list, cache_node->equipbaptize_param.part_info_list[i].attr_seq_list, sizeof(grbi.equip_baptize_info[i].attr_seq_list));
			memcpy(grbi.equip_baptize_info[i].baptize_list, cache_node->equipbaptize_param.part_info_list[i].baptize_list, sizeof(grbi.equip_baptize_info[i].baptize_list));
		}
	}

	grbi.total_jingjie = cache_node->common_data_param.jingjie_level;

	// 名将
	{
		const auto &param = cache_node->greate_soldier_param;

		grbi.greate_soldier_info.img_id = param.slot_param.slot_item_list[0].item_seq;
		grbi.greate_soldier_info.level = param.slot_param.slot_item_list[0].level;
	}

	// 百战装备
	{
		grbi.baizhan_equip_info.capability = cache_node->baizhan_equip_param.capability;

		for (int i = 0; i < E_INDEX_BAIZHAN::E_INDEX_BAIZHAN_MAX; i++)
		{
			grbi.baizhan_equip_info.baizhan_equip_list[i] = cache_node->baizhan_equip_param.baizhan_equip_list[i].baizhan_equip_data.item_id;
			grbi.baizhan_equip_info.part_order_list[i] = cache_node->baizhan_equip_param.suit_order_list[BAIZHAN_SUIT_TYPE_1].order_list[i];
		}
	}

	{
		// 周末装备
		UNSTD_STATIC_CHECK(sizeof(grbi.tianshenhuti_inuse_equip_list) == sizeof(cache_node->tianshenhuti_param.inuse_equip_list));
		memcpy(grbi.tianshenhuti_inuse_equip_list, cache_node->tianshenhuti_param.inuse_equip_list, sizeof(grbi.tianshenhuti_inuse_equip_list));
	}

	if (QUERY_FROM_CLIENT == query_from)
	{
		Role *viwer_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(viewer_uid));
		if (NULL != viwer_role)
		{
			EngineAdapter::Instance().NetSend(viwer_role->GetNetId(), (const char *)&grbi, sizeof(grbi));
		}
	}
	else if (QUERY_FROM_CROSS == query_from)
	{
		UniqueUserID uuid = UniqueUserID(plat_type, IntToUid(viewer_uid));
		//InternalComm::Instance().UserProtocalTransferToGame(UniqueUserIDToLongLong(uuid), (const char *)&grbi, sizeof(grbi));
		CrossManager::Instance().UserProtocalTransferToGame(CROSS_FUNCTION_TYPE_CROSS_RANK, UniqueUserIDToLongLong(uuid), (const char *)&grbi, sizeof(grbi));
	}
	else if (QUERY_FROM_CAMPDIAOXIANG == query_from)
	{
		CampDefendMgr::Instance().QueryCallback(cache_node);
	}
}

void QueryHandler::GetInfoToCacheNode(Role *target_role, OfflineQueryCacheNode *cache_node)
{
	if (NULL == target_role || NULL == cache_node)
	{
		return;
	}

	static RoleQueryParam query_param; 
	ItemListParam item_list_param;
	{
		query_param.role_id = target_role->GetUID();
		query_param.common_data_param.uuid = UniqueUserIDToLongLong(target_role->GetRoleCross()->GetCrossOriginalUniqueUserID());
		target_role->GetName(query_param.role_name);
		query_param.level = target_role->GetLevel();
		query_param.sex = target_role->GetSex();
		query_param.prof = target_role->GetProf();
		query_param.vip_level = target_role->GetVip()->GetVipLevel();
		query_param.guild_id = target_role->GetGuildID();
		query_param.camp = target_role->GetCamp();
		query_param.avatar_timestamp = target_role->GetAvatarTimeStamp();

		target_role->GetTitleManager()->GetOtherInitParam(&query_param.title_param);
		target_role->GetRoleInitParam(&query_param.role_init_param);
		target_role->GetWing()->GetOtherInitParam(&query_param.wing_param);
		target_role->GetCommonDataParam(&query_param.common_data_param);

		target_role->GetQingyuan()->GetInitParam(&query_param.qingyuan_param);
		target_role->GetShizhuang()->GetInitParam(&query_param.shizhuang_param);

		target_role->GetMountManager()->GetOtherInitParam(&query_param.mount_param);
		target_role->GetStoneManager()->GetInitParam(&query_param.stone_param);

		target_role->GetHalo()->GetOtherInitParam(&query_param.halo_param);
		target_role->GetShengong()->GetOtherInitParam(&query_param.shengong_param);
		target_role->GetShenyi()->GetOtherInitParam(&query_param.shenyi_param);
		target_role->GetFootPrint()->GetOtherInitParam(&query_param.footprint_param);
		target_role->GetCloakManager()->GetOtherInitParam(&query_param.cloak_param);

		target_role->GetEquipmentManager()->GetInitParam(&query_param.common_data_param.cross_equip_itemid, &query_param.common_data_param.cross_equip_gain_timestamp, &query_param.equipment_param);
		target_role->GetXiannvShouhu()->GetOtherInitParam(&query_param.xiannv_shouhu_param);
		target_role->GetJinglingFazhen()->GetOtherInitParam(&query_param.jingling_fazhen_param);
		target_role->GetJinglingGuanghuan()->GetOtherInitParam(&query_param.jingling_guanghuan_param);
		target_role->GetFightMountManager()->GetOtherInitParam(&query_param.fight_mount_param);
		target_role->GetXiannvManager()->GetInitParam(&query_param.xiannv_param);
		target_role->GetJingLingMgr()->GetInitParam(&query_param.jingling_param);
		//target_role->GetLieMingMgr()->GeiInitLieMingParam(&query_param.lieming_param);
		target_role->GetLieMingMgr()->GeiInitLieMingParam(&query_param.lieming_param2);
		target_role->GetShenZhuang()->GetInitParam(&query_param.shenzhuang_param);
		target_role->GetFeixianManager()->GetInitParam(&query_param.feixian_param);
		target_role->GetFabaoManager()->GetInitParam(&query_param.fabao_param);

		target_role->GetYaoShiManager()->GetOtherInitParam(&query_param.yaoshi_param);
		target_role->GetTouShiManager()->GetOtherInitParam(&query_param.toushi_param);
		target_role->GetQiLinBiManager()->GetOtherInitParam(&query_param.qilinbi_param);
		target_role->GetMaskManager()->GetOtherInitParam(&query_param.mask_param);
		target_role->GetMojie()->GetInitParam(&query_param.mojie_param);
		target_role->GetZhuanzhiEquip()->GetInitParam(&query_param.zhuanzhi_equip_param);

		target_role->GetUpgradeManager()->GetInitParam(&query_param.upgrade_manager_param);
		target_role->GetShenBingManager()->GetInitParam(&query_param.shenbing_param);

		target_role->GetEquipBaptize()->GetInitParam(&query_param.equipbaptize_param);
		target_role->GetGreateSoldier()->GetInitParam(&query_param.greate_soldier_param);
		target_role->GetBaizhanEquip()->GetInitParam(&query_param.baizhan_equip_param);

		target_role->GetOtherMgr()->GetInitParamPage10(query_param.data_page_10);

		int size = ItemNamespace::MAX_GRID_NUM;
		target_role->GetEquipmentManager()->GetEquipmentList(&size, item_list_param.item_list);
		item_list_param.count = size;
	}
	
	this->GetInfoToCacheNode(query_param, item_list_param, cache_node);
}

void QueryHandler::GetInfoToCacheNode(const RoleQueryParam &p, const ItemListParam &item_list_param, OfflineQueryCacheNode *cache_node)
{
	if (NULL == cache_node)
	{
		return;
	}

	// 角色信息
	{
		cache_node->role_id = p.role_id;
		cache_node->role_uuid = p.common_data_param.uuid;
		F_STRNCPY(cache_node->role_name, p.role_name, sizeof(cache_node->role_name));
		cache_node->level = p.level;
		cache_node->sex = p.sex;
		cache_node->prof = p.prof;
		cache_node->vip_level = p.vip_level;
		cache_node->guild_id = p.guild_id;
		cache_node->camp = p.camp;
		cache_node->avatar_timestamp = p.avatar_timestamp;
	}

	cache_node->title_param = p.title_param;
	cache_node->common_data_param = p.common_data_param;
	cache_node->wing_param = p.wing_param;
	cache_node->role_init_param = p.role_init_param;

	cache_node->qingyuan_param = p.qingyuan_param;
	cache_node->shizhuang_param = p.shizhuang_param;

	cache_node->mount_param = p.mount_param;
	cache_node->stone_param = p.stone_param;

	cache_node->pet_param = p.pet_param;

	cache_node->halo_param = p.halo_param;
	cache_node->shengong_param = p.shengong_param;
	cache_node->shenyi_param = p.shenyi_param;
	cache_node->footprint_param = p.footprint_param;
	cache_node->cloak_param = p.cloak_param;

	cache_node->xiannv_shouhu = p.xiannv_shouhu_param;
	cache_node->jingling_fazhen = p.jingling_fazhen_param;
	cache_node->jingling_guanghuan = p.jingling_guanghuan_param;

	cache_node->fight_mount_param = p.fight_mount_param;

	cache_node->xiannv_param = p.xiannv_param;
	cache_node->jingling_param = p.jingling_param;
	cache_node->lieming_param = p.lieming_param;
	cache_node->shenzhuang_param = p.shenzhuang_param;
	cache_node->fabao_param = p.fabao_param;

	cache_node->yaoshi_param = p.yaoshi_param;
	cache_node->toushi_param = p.toushi_param;
	cache_node->qilinbi_param = p.qilinbi_param;
	cache_node->mask_param = p.mask_param;
	cache_node->mojie_param = p.mojie_param;
	cache_node->zhuanzhi_equip_param = p.zhuanzhi_equip_param;

	cache_node->upgrade_manager_param = p.upgrade_manager_param;
	cache_node->shenbing_param = p.shenbing_param;

	cache_node->equipbaptize_param = p.equipbaptize_param;
	cache_node->greate_soldier_param = p.greate_soldier_param;

	cache_node->baizhan_equip_param = p.baizhan_equip_param;

	{
		static RoleOtherMgr tmp;
		tmp.InitParamPage10ByCahce(p.data_page_10);

		cache_node->tianshenhuti_param = *((TianshenHuti*)tmp.GetRoleModule(ROLE_MODULE_TYPE_TIANSHENHUTI))->GetTianshenhutiParam();
	}


	// 装备信息
	for (int i = 0; i < item_list_param.count; i++)					
	{
		const ItemListParam::ItemListData &item_data = item_list_param.item_list[i];

		if (item_data.index >= ItemNamespace::EQUIPMENT_INDEX_BEGIN)					// 将属于背包和装备的数据指针分别筛选出来
		{
			int equip_index = item_data.index - ItemNamespace::EQUIPMENT_INDEX_BEGIN;	// index经过转换则为装备部位
			if (equip_index < 0 || equip_index >= Equipment::E_INDEX_MAX) continue;

			const Equipment *equip = (const Equipment *)ITEMPOOL->GetItem(item_data.item_wrapper.item_id);
			if (NULL == equip || equip->GetItemType() != ItemBase::I_TYPE_EQUIPMENT)	// 装备部位不对
			{
				continue;
			}

			EquipmentParam *param = equip->CreateParamFromStruct(&(item_data.item_wrapper.param_data));
			EquipParamHelper equip_param_helper(param);

			if (NULL == param)															// struct_data有错
			{
				continue;
			}

			UNSTD_STATIC_CHECK(ItemNamespace::MAX_EQUIPMENT_GRID_NUM >= Equipment::E_INDEX_MAX);

			OfflineQueryCacheNode::EquipmentInfo &equip_inifo = cache_node->equipment_info[equip_index];
			equip_inifo.equip_id = item_data.item_wrapper.item_id;
			equip_inifo.star_level = param->star_level;
			equip_inifo.has_lucky = param->has_lucky;
			equip_inifo.quality = param->quality;
			equip_inifo.strengthen_level = param->strengthen_level;
			equip_inifo.shen_level = param->shen_level;
			equip_inifo.fuling_level = param->fuling_level;
			
			equip_inifo.grid_strengthen_level = p.equipment_param.equi_grid_item_list[equip_index].strengthen_level;
			equip_inifo.grid_shen_level = p.equipment_param.equi_grid_item_list[equip_index].shenzhu_level;
			equip_inifo.grid_star_level = p.equipment_param.equi_grid_item_list[equip_index].star_level;
			equip_inifo.eternity_level = p.equipment_param.equi_grid_item_list[equip_index].eternity_level;
			
			UNSTD_STATIC_CHECK(sizeof(equip_inifo.xianpin_type_list) == sizeof(param->xianpin_type_list));
			memcpy(equip_inifo.xianpin_type_list, param->xianpin_type_list, sizeof(equip_inifo.xianpin_type_list));
		}
	}

	cache_node->feixian_param = p.feixian_param;
}

bool QueryHandler::NeedToCache(UserID user_id)
{
	// 排行榜的用户缓存离线查询	

	int rank_type_list[] = { // 陆续添加
		PERSON_RANK_TYPE_CAPABILITY_ALL,						// 综合战力榜
		PERSON_RANK_TYPE_LEVEL,									// 等级榜
		PERSON_RANK_TYPE_XIANNV_CAPABILITY,						// 仙女战力榜
		PERSON_RANK_TYPE_EQUIP,									// 装备战力榜
		PERSON_RANK_TYPE_ALL_CHARM,								// 魅力总榜	
		PERSON_RANK_TYPE_CAPABILITY_CAMP_1,						// 阵营1综合战力榜
		PERSON_RANK_TYPE_CAPABILITY_CAMP_2,						// 阵营2综合战力榜
		PERSON_RANK_TYPE_CAPABILITY_CAMP_3,						// 阵营3综合战力榜
		PERSON_RANK_TYPE_MOUNT,									// 坐骑战力榜
		PERSON_RANK_TYPE_XIANJIAN,								// 仙剑战力榜
		PERSON_RANK_TYPE_MENTALITY,								// 修炼战力榜
		PERSON_RANK_TYPE_WING,									// 羽翼战力榜
		PERSON_RANK_TYPE_CHARM,									// 魅力榜
		PERSON_RANK_TYPE_CHARM_MALE,							// 男性魅力榜
		PERSON_RANK_TYPE_CHARM_FEMALE,							// 女性魅力榜
		PERSON_RANK_TYPE_GONGCHENGZHAN,							// 攻城战排行榜
		PERSON_RANK_TYPE_QUNXIANLUANDOU,						// 群仙乱斗排行榜
		PERSON_RANK_TYPE_ZHANSHENDIAN,							// 战神殿排行榜
		PERSON_RANK_TYPE_KILL_WORLDBOSS,						// 击杀世界boss
		PERSON_RANK_TYPE_RA_CHONGZHI,							// 充值排行榜
		PERSON_RANK_TYPE_RA_CONSUME_GOLD,						// 消费排行榜
		PERSON_RANK_TYPE_RED_PAPER_CONSUME_GOLD,				// 红包土豪榜
		PERSON_RANK_TYPE_RED_PAPER_LUCKY_VALUE,					// 红包幸运榜
		PERSON_RANK_TYPE_EVIL,									// 罪恶榜
		PERSON_RANK_TYPE_CAPABILITY_SHENZHUANG,					// 神装战力榜
		PERSON_RANK_TYPE_CAPABILITY_JINGLING,					// 精灵战力榜
		PERSON_RANK_TYPE_CAPABILITY_WASH,						// 精炼战力榜
		PERSON_RANK_TYPE_FIGHT_MOUNT,							// 战骑战力榜
		PERSON_RANK_TYPE_HALO,									// 光环战力榜
		PERSON_RANK_TYPE_SHENGONG,								// 神弓战力榜
		PERSON_RANK_TYPE_SHENYI,								// 神翼战力榜
		PERSON_RANK_TYPE_EQUIP_STRENGTH_LEVEL,					// 全身装备强化总等级榜
		PERSON_RANK_TYPE_STONE_TOATAL_LEVEL,					// 全身宝石总等级榜
		PERSON_RANK_TYPE_WORLD_RIGHT_ANSWER,					// 世界题目对题榜
		PERSON_RANK_TYPE_DAY_CHARM,								// 每日魅力榜
		PERSON_RANK_TYPE_YAOSHI,								// 腰饰战力榜
		PERSON_RANK_TYPE_TOUSHI,								// 头饰战力榜
		PERSON_RANK_TYPE_QILINBI,								// 麒麟臂战力榜
		PERSON_RANK_TYPE_MASK,									// 面具战力榜
		PERSON_RANK_TYPE_LINGZHU,                              // 灵珠阶数榜
		PERSON_RANK_TYPE_XIANBAO,                              // 仙宝阶数榜
		PERSON_RANK_TYPE_LINGTONG,                             // 灵童阶数榜
		PERSON_RANK_TYPE_LINGGONG,                             // 灵弓阶数榜
		PERSON_RANK_TYPE_LINGQI,                               // 灵骑阶数榜
		PERSON_RANK_TYPE_WEIYAN,                               // 尾焰阶数榜
		PERSON_RANK_TYPE_SHOUHUAN,                             // 手环阶数榜
		PERSON_RANK_TYPE_WEIBA,                                // 尾巴阶数榜
		PERSON_RANK_TYPE_FEICHONG,                             // 飞宠阶数榜
		PERSON_RANK_TYPE_RA_PROFESS_MALE,						// 表白排行男榜
		PERSON_RANK_TYPE_RA_PROFESS_FEMALE,						// 表白排行女榜
		PERSON_RANK_TYPE_RA_CHONGZHI2,							// 充值排行榜2

	}; 

	for (int i = 0; i < (int)(sizeof(rank_type_list) / sizeof(rank_type_list[0])); ++i)
	{
		bool ret = RankManager::Instance().GetPersonRank()->IsUserOnRank(user_id, rank_type_list[i]);
		if (ret)
		{
			return true;
		}
	}

	return false;
}


