#include "campdefendmanager.hpp"

#include "config/logicconfigmanager.hpp"
#include "other/camp/campconfig.hpp"

#include "obj/character/role.h"
#include "obj/character/monster.h"
#include "obj/character/roleshadow.hpp"

#include "servercommon/errornum.h"

#include "world.h"
#include "scene/scene.h"
#include "global/worldstatus//worldstatus.hpp"

#include "other/event/eventhandler.hpp"
#include "effect/skilleffect/effectbuff.hpp"
#include "servercommon/noticenum.h"
#include "servercommon/string/gameworldstr.h"
#include "other/route/mailroute.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "scene/worldshadow/worldshadow.hpp"
#include "other/route/mailroute.hpp"
#include "global/camp/campmanager.hpp"

#include "protocal/msgrole.h"

#include <algorithm>
#include <math.h>

CampDefendMgr::CampDefendMgr() : m_act_status(ACTIVITY_STATUS_CLOSE), m_activity_type(0), m_next_syncinfo_time(0)
{
	memset(m_diaoxiang_list, 0, sizeof(m_diaoxiang_list));
}

CampDefendMgr::~CampDefendMgr()
{

}

CampDefendMgr & CampDefendMgr::Instance()
{
	static CampDefendMgr cdm;
	return cdm;
}

void CampDefendMgr::OnWorldStatusFirstStart()
{
	this->UpdateDiaoXiang();
}

void CampDefendMgr::Update(unsigned long interval, time_t now_second)
{ 
	if (!this->IsCampDefending())
	{
		return;
	}

	if (static_cast<unsigned int>(now_second) >= m_next_syncinfo_time)
	{
		m_next_syncinfo_time = static_cast<unsigned int>(now_second) + 3;
		this->SyncCampDefendInfo();
	}
}

void CampDefendMgr::OnEnterScene(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (this->IsInCampDefendingScene(role) && NULL == this->GetJoiner(role->GetUID()))	// 进入过该场景就认为是参加者
	{
		this->AddJoiner(role);
	}
}

void CampDefendMgr::OnRoleChangeCamp(Role *role)
{
	if (NULL == role)
	{
		return;
	}

	if (!this->IsInCampDefendingScene(role))
	{
		return;
	}

	Joiner *joiner = this->GetJoiner(role->GetUID());
	if (NULL != joiner)
	{
		joiner->camp = role->GetCamp();
	}
	else
	{
		this->AddJoiner(role);
	}
}

void CampDefendMgr::OnActivityStatusChange(int status, int activity_type)
{
	if (!this->IsCampDefendActType(activity_type))
	{
		return;
	}

	m_act_status = status;

	switch (status)
	{
	case ACTIVITY_STATUS_STANDY:
		{
			m_activity_type = activity_type;
			CampManager::Instance().CalcCampRank3(this->GetActingShouFangCamp());
			WorldStatus::Instance().ResetCampDiaoXiangStatus(this->GetActingShouFangCamp());		// 每次活动开始重置下
			this->UpdateDiaoXiang();																// 每次活动开始重新创建雕象
		}
		break;

	case ACTIVITY_STATUS_OPEN:
		{
			m_activity_type = activity_type;

			{
				Scene *scene = this->GetActingScene();
				if (NULL != scene)
				{
					int role_num = static_cast<unsigned int>(scene->RoleNum());
					for (int i = 0; i < role_num; ++ i)
					{
						Role *role = scene->GetRoleByIndex(i);
						if (NULL != role && NULL == this->GetJoiner(role->GetUID()))
						{
							this->AddJoiner(role);
						}
					}
				}
			}
		}
		break;

	case ACTIVITY_STATUS_CLOSE:
		{
			this->CalcRewardOnActClose();
			m_activity_type = 0;
			m_next_syncinfo_time = 0;
			m_joiner_dic.clear();
			this->UpdateDiaoXiang();
		}
		break;
	}
}

void CampDefendMgr::UpdateDiaoXiang()
{
	this->DelAllDiaoXiang();
	this->CreateAllDiaoXiang();
}

void CampDefendMgr::DelAllDiaoXiang()
{
	for (int i = 0; i < MAX_CAMP_RECORD_RANK3_NUM; ++ i)
	{
		this->DelOneDiaoXiang(i);
	}
}

void CampDefendMgr::CreateAllDiaoXiang()
{
	for (int i = 0; i < MAX_CAMP_RECORD_RANK3_NUM; ++ i)
	{
		this->CreateOneDiaoXiang(i);
	}
}

void CampDefendMgr::UpdateOneDiaoXiang(int index)
{
	this->DelOneDiaoXiang(index);
	this->CreateOneDiaoXiang(index);
}

void CampDefendMgr::DelOneDiaoXiang(int index)
{
	if (index < 0 || index >= MAX_CAMP_RECORD_RANK3_NUM)
	{
		return;
	}

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(m_diaoxiang_list[index].sceneid, 0);
	if (NULL != scene)
	{
		scene->DeleteObj(m_diaoxiang_list[index].objid);
	}

	m_diaoxiang_list[index].Reset();
}

void CampDefendMgr::CreateOneDiaoXiang(int index)
{
	if (index < 0 || index >= MAX_CAMP_RECORD_RANK3_NUM)
	{
		return;
	}

	WorldStatusCommonDataParam &world_common_p = WorldStatus::Instance().GetCommonData();
	
	const int uid = world_common_p.camp_toprank3_uid[index];
	if (uid <= 0)
	{
		return;
	}

	m_diaoxiang_list[index].roleid = uid;
	QueryHandler::Instance().OnQuery(uid, QUERY_FROM_CAMPDIAOXIANG, uid, 0);
}

void CampDefendMgr::QueryCallback(OfflineQueryCacheNode *cache_node)
{
	if (NULL == cache_node)
	{
		return;
	}

	int diaoxiang_index = -1;
	for (int i = 0; i < MAX_CAMP_RECORD_RANK3_NUM; ++ i)
	{
		if (cache_node->role_id == m_diaoxiang_list[i].roleid)
		{
			diaoxiang_index = i;
			break;
		}
	}

	if (diaoxiang_index < 0 || diaoxiang_index >= MAX_CAMP_RECORD_RANK3_NUM)
	{
		return;
	}

	const CampDiaoXiangAttrItemCfg *attr_cfg = LOGIC_CONFIG->GetCampCfg().GetDiaoXiangAttrItemCfg(WorldShadow::Instance().GetWorldLevel());
	if (NULL == attr_cfg)
	{
		return;
	}

	const CampDiaoXiangPosItemCfg *post_cfg = LOGIC_CONFIG->GetCampCfg().GetDiaoXiangPosItemCfg(diaoxiang_index);
	if (NULL == post_cfg)
	{
		return;
	}

	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(post_cfg->sceneid, 0);
	if (NULL == scene)
	{
		return;
	}

	ChallengeParam param;
	RoleAppearance appearance;

	{
		param.role_id = cache_node->role_id;
		F_STRNCPY(param.role_name, cache_node->role_name, sizeof(param.role_name));
		param.level = cache_node->level;
		param.camp = cache_node->camp;
		param.prof = cache_node->prof;
		param.sex = cache_node->sex;
		param.move_speed = 0;

		param.max_hp = attr_cfg->maxhp;
		param.fangyu = attr_cfg->fangyu;
		param.shanbi = attr_cfg->shanbi;
		param.dikangshanghai = attr_cfg->dikangshanghai;
		param.kangbao_per = attr_cfg->kangbao_per;
		param.mianshang_per = attr_cfg->mianshang_per;
		
		appearance.wuqi_id = cache_node->equipment_info[Equipment::E_INDEX_WUQI].equip_id;
		appearance.shizhuang_wuqi = cache_node->shizhuang_param.item_list[SHIZHUANG_TYPE_WUQI].use_img;
		appearance.shizhuang_body = cache_node->shizhuang_param.item_list[SHIZHUANG_TYPE_BODY].use_img;
		appearance.mount_used_imageid = cache_node->mount_param.temporary_imageid > 0 ? cache_node->mount_param.temporary_imageid : cache_node->mount_param.used_imageid;
		appearance.wing_used_imageid = cache_node->wing_param.temporary_imageid > 0 ? cache_node->wing_param.temporary_imageid : cache_node->wing_param.used_imageid;
		appearance.halo_used_imageid = cache_node->halo_param.used_imageid;
		appearance.shengong_used_imageid = cache_node->shengong_param.used_imageid;
		appearance.shenyi_used_imageid = cache_node->shenyi_param.used_imageid;
		appearance.footprint_used_imageid = cache_node->footprint_param.used_imageid;
		appearance.cloak_used_imageid = cache_node->cloak_param.used_imageid;

		appearance.xiannvshouhu_imageid = cache_node->xiannv_shouhu.used_imageid;
		appearance.jingling_fazhen_imageid = cache_node->jingling_fazhen.used_imageid;
		appearance.jingling_guanghuan_imageid = cache_node->jingling_guanghuan.used_imageid;
		appearance.fight_mount_used_imageid = cache_node->fight_mount_param.used_imageid;
		appearance.zhibao_used_imageid = cache_node->zhibao_param.use_image;
		appearance.fabao_used_imageid = cache_node->fabao_param.used_imageid;

		appearance.yaoshi_used_imageid = cache_node->yaoshi_param.used_imageid;
		appearance.toushi_used_imageid = cache_node->toushi_param.used_imageid;
		appearance.qilinbi_used_imageid = cache_node->qilinbi_param.used_imageid;

		appearance.mask_used_imageid = cache_node->mask_param.used_imageid;
	}

	RoleShadow *role_shadow = new RoleShadow(ROLE_SHADOW_TYPE_CAMPDEFEND);
 	role_shadow->Init(param, appearance, 0, 0);
 	role_shadow->SetPos(post_cfg->pos);
 	scene->AddObj(role_shadow);

 	m_diaoxiang_list[diaoxiang_index].objid = role_shadow->GetId();
 	m_diaoxiang_list[diaoxiang_index].sceneid = role_shadow->GetScene()->GetSceneID();
}

void CampDefendMgr::OnRoleDie(Role *role, Role *killer)
{
	if (NULL == role || NULL == killer || role == killer)
	{
		return;
	}

	if (CAMP_TYPE_INVALID == role->GetCamp() || CAMP_TYPE_INVALID == killer->GetCamp() ||
		role->GetCamp() == killer->GetCamp())
	{
		return;
	}

	if (!this->IsInCampDefendingScene(role) || !this->IsInCampDefendingScene(killer))
	{
		return;
	}

	Joiner *die_joiner = this->GetJoiner(role->GetUID());
	Joiner *kill_joiner = this->GetJoiner(killer->GetUID());
	
	if (NULL == die_joiner || NULL == kill_joiner)
	{
		return;
	}

	unsigned int const now_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	if (now_time - die_joiner->GetLastKillSelfTime(killer->GetUID()) >= LOGIC_CONFIG->GetCampCfg().GetOtherCfg().dx_killrole_timegap)
	{
		int reward_gongxun = LOGIC_CONFIG->GetCampCfg().GetOtherCfg().dx_killrole_addgongxun;
		if (reward_gongxun > 0)
		{
			killer->AddGongxun(reward_gongxun, "defendkillrole");
		}

		die_joiner->AddKillSelfRecord(killer->GetUID(), now_time);
	}
}

void CampDefendMgr::OnDiaoXiangDie(Role *role, RoleShadow *diaoxiang)
{
	if (NULL == role || NULL == diaoxiang)
	{
		return;
	}

	if (!this->IsInCampDefendingScene(role) || !this->IsInCampDefendingScene(diaoxiang))
	{
		return;
	}

	// 奖励
	{
		int reward_gongxun = LOGIC_CONFIG->GetCampCfg().GetOtherCfg().dx_killdiaoxiang_addgongxun;
		
		if (reward_gongxun > 0 && 
			CAMP_TYPE_INVALID != role->GetCamp() && role->GetCamp() != this->GetActingShouFangCamp())  //必须是攻方才可以奖励
		{
			role->AddGongxun(reward_gongxun, "defendkilldx");
		}
	
		this->CalcGongfangReward(diaoxiang);
	}

	// 传闻
	{
		int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_campdefend_diaoxiang_bekilled,
			diaoxiang->GetCamp(), UidToInt(diaoxiang->GetUserId()), diaoxiang->GetName(), diaoxiang->GetCamp(),
			role->GetCamp(), role->GetUID(), role->GetName(), role->GetCamp());
		
		if (length > 0)
		{
			World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_ROLL_2);
		}
	}
	
	WorldStatus::Instance().SetCampDiaoXiangStatus(this->GetDiaoXiangIndex(diaoxiang), DIAO_XIANG_STATUS_DESTORY);

	int diaoxiang_index = this->GetDiaoXiangIndex(diaoxiang);
	this->UpdateOneDiaoXiang(diaoxiang_index);

	this->SyncCampDefendInfo();
}

void CampDefendMgr::OnDiaoXiangAttack(Role *role, Attribute hurt)
{
	if (NULL == role)
	{
		return;
	}

	Joiner *joiner = this->GetJoiner(role->GetUID());
	if (NULL == joiner)
	{
		return;
	}

	joiner->total_diaoxiang_hurt += hurt;
}

void CampDefendMgr::CalcShoufangReward(RoleShadow *diaoxiang)
{
	if (NULL == diaoxiang || !diaoxiang->IsAlive())
	{
		return;
	}

	Attribute hp = diaoxiang->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	Attribute maxhp = diaoxiang->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	double rate = (double)maxhp / 10000.0;

	if (static_cast<int>(rate) <= 0)
	{
		return;
	}

	const int reward_gongxun = static_cast<int>(((double)hp / rate)  * LOGIC_CONFIG->GetCampCfg().GetOtherCfg().dx_sf_addgongxun_once);

	for (JoinerDic::const_iterator iter = m_joiner_dic.begin(); m_joiner_dic.end() != iter; ++ iter)
	{
		if (this->GetActingShouFangCamp() == iter->second.camp && reward_gongxun > 0)
		{
			static MailContentParam contentparam; contentparam.Reset();
			
			contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GONGXUN, reward_gongxun);

			int length_1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_campdefend_sf_reward_subject);
			int length_2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_campdefend_sf_reward_content, reward_gongxun);

			if (length_1 > 0 && length_2 > 0)
			{
				MailRoute::MailToUser(IntToUid(iter->second.roleid), SYSTEM_MAIL_REASON_INVALID, contentparam);
			}
		}
	}
}

void CampDefendMgr::CalcGongfangReward(RoleShadow *diaoxiang)
{
	if (NULL == diaoxiang)
	{
		return;
	}

	for (JoinerDic::const_iterator iter = m_joiner_dic.begin(); m_joiner_dic.end() != iter; ++ iter)
	{
		if (this->GetActingShouFangCamp() != iter->second.camp)
		{
			long long hurt = diaoxiang->GetHurtByUid(iter->second.roleid);
			Attribute maxhp = diaoxiang->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
			double rate = (double)maxhp / 10000.0;

			if (static_cast<int>(rate) <= 0)
			{
				continue;
			}

			int reward_gongxun = static_cast<int>(((double)hurt / rate) * LOGIC_CONFIG->GetCampCfg().GetOtherCfg().dx_gf_addgongxun_once);

			if (reward_gongxun > 0)
			{
				static MailContentParam contentparam; contentparam.Reset();
			
				contentparam.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_GONGXUN, reward_gongxun);

				int length_1 = SNPRINTF(contentparam.subject, sizeof(contentparam.subject), gamestring::g_campdefend_gf_reward_subject);
				int length_2 = SNPRINTF(contentparam.contenttxt, sizeof(contentparam.contenttxt), gamestring::g_campdefend_gf_reward_content, reward_gongxun);

				if (length_1 > 0 && length_2 > 0)
				{
					MailRoute::MailToUser(IntToUid(iter->second.roleid), SYSTEM_MAIL_REASON_INVALID, contentparam);
				}
			}
		}
	}
}

void CampDefendMgr::CalcRewardOnActClose()
{
	Scene *scene = this->GetActingScene();
	if (NULL == scene)
	{
		return;
	}

	for (int i = 0; i < MAX_CAMP_RECORD_RANK3_NUM; ++ i)
	{
		if (DIAO_XIANG_STATUS_DESTORY == WorldStatus::Instance().GetCampDiaoXiangStatus(i))
		{
			continue;
		}

		RoleShadow *diaoxiang = this->GetDiaoXiangByIndex(i);
		if (NULL == diaoxiang || scene != diaoxiang->GetScene())
		{
			continue;
		}

		this->CalcShoufangReward(diaoxiang);
		this->CalcGongfangReward(diaoxiang);
	}
}

Scene *CampDefendMgr::GetActingScene()
{
	int sceneid = LOGIC_CONFIG->GetCampCfg().GetCampDefendSceneIdByActType(m_activity_type);
	if (sceneid <= 0) return NULL;

	return World::GetInstWorld()->GetSceneManager()->GetSceneById(sceneid, 0);
}

int CampDefendMgr::GetActingShouFangCamp()
{
	if (ACTIVITY_TYPE_CAMP_DEFEND1 == m_activity_type)
	{
		return CAMP_TYPE_FEIXING;
	}

	else if (ACTIVITY_TYPE_CAMP_DEFEND2 == m_activity_type)
	{
		return CAMP_TYPE_ZHURI;
	}

	else if (ACTIVITY_TYPE_CAMP_DEFEND3 == m_activity_type)
	{
		return CAMP_TYPE_ZHUIYUE;
	}
	
	return CAMP_TYPE_INVALID;
}

RoleShadow *CampDefendMgr::GetDiaoXiangByIndex(int index)
{
	if (index < 0 || index >= MAX_CAMP_RECORD_RANK3_NUM)
	{
		return NULL;
	}
	
	Scene *scene = World::GetInstWorld()->GetSceneManager()->GetSceneById(m_diaoxiang_list[index].sceneid, 0);
	if (NULL == scene)
	{
		return NULL;
	}

	Obj *obj = scene->GetObj(m_diaoxiang_list[index].objid);
	if (NULL == obj || Obj::OBJ_TYPE_ROLE_SHADOW != obj->GetObjType())
	{
		return NULL;
	}

	RoleShadow *diaoxiang = (RoleShadow *)obj;
	if (ROLE_SHADOW_TYPE_CAMPDEFEND != diaoxiang->GetRoleShadowType())
	{
		return NULL;
	}

	return diaoxiang;
}

int CampDefendMgr::GetDiaoXiangIndex(RoleShadow *diaoxiang)
{
	if (NULL == diaoxiang || NULL == diaoxiang->GetScene())
	{
		return -1;
	}

	for (int i = 0; i < MAX_CAMP_RECORD_RANK3_NUM; ++ i)
	{
		if (UidToInt(diaoxiang->GetUserId()) == m_diaoxiang_list[i].roleid)
		{
			return i;
		}
	}

	return -1;
}

int CampDefendMgr::GetDiaoXiangStatus(RoleShadow *diaoxiang)
{
	return WorldStatus::Instance().GetCampDiaoXiangStatus(this->GetDiaoXiangIndex(diaoxiang));
}

void CampDefendMgr::AddJoiner(Role *role)
{
	if (NULL != role && NULL == this->GetJoiner(role->GetUID()) && 
		role->GetCamp() > CAMP_TYPE_INVALID && role->GetCamp() < CAMP_TYPE_MAX &&
		this->IsInCampDefendingScene(role))
	{
		CampDefendMgr::Joiner joiner;
		joiner.roleid = role->GetUID();
		joiner.camp = role->GetCamp();
		role->GetName(joiner.name);

		m_joiner_dic[joiner.roleid] = joiner;
	}
}

CampDefendMgr::Joiner *CampDefendMgr::GetJoiner(int roleid)
{
	JoinerDic::iterator iter = m_joiner_dic.find(roleid);
	if (m_joiner_dic.end() == iter)
	{
		return NULL;
	}

	return &iter->second;
}

bool CampDefendMgr::IsDiaoxiangCanBeAttacked(RoleShadow *diaoxiang, Role *role)
{
	if (NULL == diaoxiang || NULL == role)
	{
		return false;
	}

	if (!diaoxiang->IsAlive() || !role->IsAlive())
	{
		return false;
	}

	if (!this->IsInCampDefendingScene(role) || !this->IsInCampDefendingScene(diaoxiang))
	{
		return false;
	}

	if (role->GetCamp() <= CAMP_TYPE_INVALID || role->GetCamp() >= CAMP_TYPE_MAX)
	{
		return false;
	}

	if (role->GetCamp() == this->GetActingShouFangCamp()) // 守方师门不能打
	{
		return false;
	}

	if (DIAO_XIANG_STATUS_DESTORY == this->GetDiaoXiangStatus(diaoxiang))
	{
		return false;
	}

	return true;
}

bool CampDefendMgr::IsInCampDefendingScene(Character *cha)
{
	if (NULL == cha)
	{
		return false;
	}

	if (!this->IsCampDefending())
	{
		return false;
	}

	int sceneid = LOGIC_CONFIG->GetCampCfg().GetCampDefendSceneIdByActType(m_activity_type);

	if (NULL == cha->GetScene() || cha->GetScene()->GetSceneID() != sceneid)
	{
		return false;
	}

	return true;
}

bool CampDefendMgr::IsCampDefending()
{
	return ACTIVITY_STATUS_OPEN == m_act_status && m_activity_type > 0 && this->IsCampDefendActType(m_activity_type);
}

bool CampDefendMgr::IsCampDefendStandBy()
{
	return ACTIVITY_STATUS_STANDY == m_act_status && m_activity_type > 0 && this->IsCampDefendActType(m_activity_type);
}

bool CampDefendMgr::IsCampDefendActType(int activity_type)
{
	if (ACTIVITY_TYPE_CAMP_DEFEND1 == activity_type ||
		ACTIVITY_TYPE_CAMP_DEFEND2 == activity_type ||
		ACTIVITY_TYPE_CAMP_DEFEND3 == activity_type)
	{
		return true;
	}

	return false;
}

static bool SortCampDefendFunc(const Protocol::SCCampDefendInfo::RankItem &rankitem_1, 
						const Protocol::SCCampDefendInfo::RankItem &rankitem_2)
{
	return rankitem_1.hurt > rankitem_2.hurt;
}

void CampDefendMgr::SyncCampDefendInfo()
{
	Scene *scene = this->GetActingScene();
	if (NULL == scene)
	{
		return;
	}

	static Protocol::SCCampDefendInfo cmd;
	memset(cmd.rank_list, 0, sizeof(cmd.rank_list));
	cmd.self_hurt = 0;
	cmd.diaoxiang1_hp = 0;
	cmd.diaoxiang1_maxhp = 0;
	cmd.diaoxiang2_hp = 0;
	cmd.diaoxiang2_maxhp = 0;
	cmd.diaoxiang3_hp = 0;
	cmd.diaoxiang3_maxhp = 0;

	int camp = this->GetActingShouFangCamp();
	RoleShadow *diaoxiang1 = this->GetDiaoXiangByIndex((camp - 1) * MAX_CAMP_TOPRANK3 + 0);
	RoleShadow *diaoxiang2 = this->GetDiaoXiangByIndex((camp - 1) * MAX_CAMP_TOPRANK3 + 1);
	RoleShadow *diaoxiang3 = this->GetDiaoXiangByIndex((camp - 1) * MAX_CAMP_TOPRANK3 + 2);

	typedef std::vector<Protocol::SCCampDefendInfo::RankItem> RankVec;
	RankVec rank_vec;

	// 计算各个参与者对三个雕象的总伤害
	{
		for (JoinerDic::const_iterator iter = m_joiner_dic.begin(); m_joiner_dic.end() != iter; ++ iter)
		{
			if (iter->second.camp == camp)
			{
				continue;
			}

			Protocol::SCCampDefendInfo::RankItem rank_item;

			rank_item.roleid = iter->second.roleid;
			rank_item.hurt = iter->second.total_diaoxiang_hurt;
			memcpy(rank_item.name, iter->second.name, sizeof(rank_item.name));
			
			rank_vec.push_back(rank_item);
		}
	}

	// 计算排名
	{
		std::sort(rank_vec.begin(), rank_vec.end(), SortCampDefendFunc);

		int count = 0;
		for(RankVec::const_iterator iter = rank_vec.begin(); rank_vec.end() != iter; ++ iter)
		{
			if (count >= Protocol::SCCampDefendInfo::MAX_RANK_NUM)
			{
				break;
			}

			Protocol::SCCampDefendInfo::RankItem &rank_item = cmd.rank_list[count];
			rank_item.roleid = iter->roleid;
			rank_item.hurt = iter->hurt;
			memcpy(rank_item.name, iter->name, sizeof(rank_item.name));

			++ count;
		}
	}

	// 计算雕象剩余血量
	{
		if (NULL != diaoxiang1)
		{
			if (DIAO_XIANG_STATUS_EXIST == this->GetDiaoXiangStatus(diaoxiang1))
			{
				cmd.diaoxiang1_hp = diaoxiang1->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			}
			cmd.diaoxiang1_maxhp = diaoxiang1->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		}
		
		if (NULL != diaoxiang2)
		{
			if (DIAO_XIANG_STATUS_EXIST == this->GetDiaoXiangStatus(diaoxiang2))
			{
				cmd.diaoxiang2_hp = diaoxiang2->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			}
			cmd.diaoxiang2_maxhp = diaoxiang2->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		}

		if (NULL != diaoxiang3)
		{
			if (DIAO_XIANG_STATUS_EXIST == this->GetDiaoXiangStatus(diaoxiang3))
			{
				cmd.diaoxiang3_hp = diaoxiang3->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
			}
			cmd.diaoxiang3_maxhp = diaoxiang3->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
		}
	}

	for (int i = 0; i < (int)scene->RoleNum(); ++ i)
	{
		Role *role = scene->GetRoleByIndex(i);
		if (NULL != role)
		{
			Joiner *joiner = this->GetJoiner(role->GetUID());
			if (NULL != joiner)
			{
				cmd.self_hurt = joiner->total_diaoxiang_hurt;
			}

			EngineAdapter::Instance().NetSend(role->GetNetId(), (const char *)&cmd, sizeof(cmd));
		}
	}

	rank_vec.clear();
}