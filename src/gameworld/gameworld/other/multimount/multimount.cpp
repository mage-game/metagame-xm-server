#include "multimount.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "other/multimount/multimountconfig.hpp"
#include "item/itemextern.hpp"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "world.h"
#include "scene/scene.h"
#include "other/event/eventhandler.hpp"
#include "protocal/msgmultimount.h"
#include "protocal/msgrole.h"
#include "servercommon/noticenum.h"
#include "obj/character/attribute.hpp"
#include "servercommon/activitydef.hpp"
#include "item/knapsack.h"
#include "other/capability/capability.hpp"
#include "other/rolestatus/rolestatusmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/dressingroom/dressingroomconfig.hpp"
#include "other/dressingroom/dressingroommgr.hpp"

std::map<int, MultiMountBindItem> MultiMount::m_uid_to_binditem_map;
std::map<int, int> MultiMount::m_follower_uid_to_owner_uid_map;

MultiMount::MultiMount() : m_role(NULL)
{
}

MultiMount::~MultiMount()
{

}

void MultiMount::Init(Role *role, const MultiMountParam &param)
{
	m_role = role;
	m_param = param;
}

void MultiMount::GetInitParam(MultiMountParam *param)
{
	*param = m_param;
}

void MultiMount::Init2(Role *role, const CoupleMountParam &param)
{
	m_role = role;
	m_couple_mount_param = param;
}

void MultiMount::GetInitParam2(CoupleMountParam *param)
{
	*param = m_couple_mount_param;
}

void MultiMount::ReCalcAttr(CharIntAttrs &base_add, bool is_recalc)
{
	if (is_recalc)
	{
		m_attrs_add.Reset();

		for (int mount_id = 1; mount_id <= COUPLEMOUNT_MAX_ID; ++ mount_id)
		{
			MultiMountParamInfo &mount_param = m_couple_mount_param.mount_list[mount_id - 1];
			const MultiMountInfoConfig::GradeCfg * attr_cfg = LOGIC_CONFIG->GetMultiMountCfg().GetMultiMountGradeCfg(mount_id, mount_param.grade);
			if (NULL != attr_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += attr_cfg->maxhp;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += attr_cfg->gongji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += attr_cfg->fangyu;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += attr_cfg->mingzhong;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += attr_cfg->shanbi;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += attr_cfg->baoji;
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += attr_cfg->jianren;
			}
		}

		MultiMountBindItem *bind_info = this->GetMountOnInfo();
		if (NULL != bind_info && this->IsOnMount())
		{
			const MultiMountInfoConfig::GradeCfg * attr_cfg = LOGIC_CONFIG->GetMultiMountCfg().GetMultiMountGradeCfg(bind_info->mount_id, bind_info->mount_star_level);
			//const MultiMountSpecialConfig * special_img_cfg = LOGIC_CONFIG->GetMultiMountCfg().GetMultiMountSpecialImgCfg(m_param.cur_use_img_id);
			
			if (NULL != attr_cfg)
			{
				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED] += attr_cfg->move_speed;
			}
// 			else if (NULL != special_img_cfg)
// 			{
// 				m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED] += special_img_cfg->move_speed;
// 			}
		}

		// 特殊形象
		for (int img_id = 1; img_id <= COUPLEMOUNT_SPECIAL_IMG_MAX_ID_LIMIT; ++ img_id)
		{
			if (0 != (m_couple_mount_param.special_img_active_flag & (1 << img_id)))
			{
				const MultiMountSpecialConfig *cfg_item = LOGIC_CONFIG->GetMultiMountCfg().GetMultiMountSpecialImgCfg(img_id);
				if (NULL != cfg_item)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg_item->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg_item->shanbi;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg_item->baoji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg_item->jianren;
				}
			}
		}

		for (int img_id = 1; img_id <= COUPLEMOUNT_SPECIAL_IMG_MAX_ID_LIMIT; ++img_id)
		{
			if (0 != (m_couple_mount_param.special_img_active_flag & (1 << img_id)))
			{
				const MultimountSpecialImgUplevelCfg *cfg_item = LOGIC_CONFIG->GetMultiMountCfg().GetSpecialImgUplevelCfg(img_id, m_couple_mount_param.mount_special_image_level_list[img_id]);
				if (NULL != cfg_item)
				{
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += cfg_item->mingzhong;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += cfg_item->shanbi;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += cfg_item->baoji;
					m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += cfg_item->jianren;
				}
				//this->HuanHuaSpecialCapAdd(img_id, m_couple_mount_param.mount_special_image_level_list[img_id], m_attrs_add);
			}
		}

		m_role->GetCapability()->ReCalcCap(CAPABILITY_TYPE_MULTIMOUNT, m_attrs_add);
	}

	if (this->IsOnMount())		// 如果骑乘就全加
	{
		base_add.Add(m_attrs_add.m_attrs, true);
	}
	else
	{
		for (int i = CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP; i <= CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN; ++ i)
		{
			base_add.m_attrs[i] += m_attrs_add.m_attrs[i];
		}
	}
}

void MultiMount::Update(unsigned int now_second)
{
	
}

void MultiMount::OnRoleLogin()
{
	MultiMountBindItem *bind_info = this->GetMountOnInfo();
	if (NULL == bind_info) // 玩家上线，更新坐骑信息
	{
		return;
	}

	if (bind_info->owner_uid <= 0)
	{
		bind_info->owner_uid = m_role->GetUID();
		bind_info->owner_obj_id = m_role->GetId();
	}

	if (m_couple_mount_param.cur_is_riding && m_couple_mount_param.cur_select_mount_id > 0)
	{
		bind_info->status = MULTI_MOUNT_RIDE_STATUS_OWNER_RIDE;
		bind_info->mount_id = m_couple_mount_param.cur_select_mount_id;
		bind_info->mount_star_level = m_couple_mount_param.mount_list[m_param.cur_select_mount_id - 1].grade;
		bind_info->res_id = this->GetCurUseResID();

		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MULTI_MOUNT);
	}
	else
	{
		bind_info->status = MULTI_MOUNT_RIDE_STATUS_NONE;
	}

	if (bind_info->owner_uid == m_role->GetUID())
	{
		this->AreaBroadcastOwnerRideInfo();
	}
}

void MultiMount::OnLogout()
{
	MultiMountBindItem *bind_info = GetMountOnInfo();
	if (NULL == bind_info) // 任何一个人下线，把跟随者脱离
	{
		return;
	}

	MultiMount::m_follower_uid_to_owner_uid_map.erase(bind_info->follower_uid);

	if (MULTI_MOUNT_RIDE_STATUS_TOW_RIDE == bind_info->status && bind_info->follower_uid > 0)
	{
		Role *owner_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(bind_info->owner_uid));

		int old_follower_uid = bind_info->follower_uid;
		ObjID old_follower_obj_id = bind_info->follower_obj_id;
		bind_info->follower_uid = 0;
		bind_info->follower_obj_id = INVALID_OBJ_ID;
		bind_info->invite_uid = 0;
		bind_info->status = MULTI_MOUNT_RIDE_STATUS_OWNER_RIDE;

		if (m_role->GetUID() == bind_info->owner_uid)
		{
			bind_info->res_id = 0;
			bind_info->status = MULTI_MOUNT_RIDE_STATUS_NONE;
		}

		if (NULL != owner_role)
		{
			owner_role ->GetMultiMount()->AreaBroadcastOwnerRideInfo(old_follower_uid, old_follower_obj_id);
		}
	}
	else
	{
		bind_info->res_id = 0;
		bind_info->status = MULTI_MOUNT_RIDE_STATUS_NONE;
	}
}

void MultiMount::OnEnterScene(int scene_id)
{
	// 进入场景时更新一下objid
	MultiMountBindItem *bind_info = this->GetMountOnInfo();

	if (NULL == bind_info) return;

	if (bind_info->owner_uid == m_role->GetUID())
	{
		bind_info->owner_obj_id = m_role->GetId();
		this->AreaBroadcastOwnerRideInfo();
	}
	else if (bind_info->follower_uid == m_role->GetUID())
	{
		bind_info->follower_obj_id = m_role->GetId();
	}
}

int MultiMount::GetCurUseResID()
{
	if (!this->IsOnMount())
	{
		return 0;
	}

	const MultiMountSpecialConfig * multi_mount_special_img_cfg = LOGIC_CONFIG->GetMultiMountCfg().GetMultiMountSpecialImgCfg(m_couple_mount_param.cur_use_img_id);
	if (NULL != multi_mount_special_img_cfg)
	{
		return multi_mount_special_img_cfg->res_id;
	}

	const MultiMountInfoConfig * multi_mount_info_cfg = LOGIC_CONFIG->GetMultiMountCfg().GetMultiMountCfg(m_couple_mount_param.cur_select_mount_id);
	if (NULL != multi_mount_info_cfg)
	{
		return multi_mount_info_cfg->res_id;
	}

	return 0;
}

void MultiMount::OnMountReq(int type, int param_1, int param_2, int param_3)
{
	switch(type)
	{
	case Protocol::MULTI_MOUNT_REQ_TYPE_SELECT_MOUNT:
		{
			this->SelectMount(param_1);
		}
		break;

	case Protocol::MULTI_MOUNT_REQ_TYPE_UPGRADE:
		{
			int mount_id = param_1;
			int repeat_times = param_2;
			bool is_auto_buy = (0 != param_3);
			if (mount_id <= 0 || mount_id > COUPLEMOUNT_MAX_ID)
			{
				break;
			}

			int old_star_level = m_couple_mount_param.mount_list[mount_id - 1].grade;
			bool can_continue = true;
			for (int i = 0; i < repeat_times; ++ i)
			{

				bool upgrade_opera_succ = this->Upgrade(mount_id, is_auto_buy);
				if (!upgrade_opera_succ || ((m_couple_mount_param.mount_list[mount_id - 1].grade / 10) - (old_star_level / 10) >= 1))
				{
					can_continue = false;
					break;
				}
			}
			m_role->SendOperateResult(Protocol::SCOperateResult::OP_MULTI_MOUNT_UPGRADE, can_continue ? 1 : 0);
		}
		break;

	case Protocol::MULTI_MOUNT_REQ_TYPE_RIDE:
		{
			this->RideMount();
		}
		break;

	case Protocol::MULTI_MOUNT_REQ_TYPE_UNRIDE:
		{
			this->UnRideMount();
		}
		break;

	case Protocol::MULTI_MOUNT_REQ_TYPE_INVITE_RIDE:
		{
			this->InviteRide(param_1);
		}
		break;

	case Protocol::MULTI_MOUNT_REQ_TYPE_INVITE_RIDE_ACK:
		{
			this->InviteRideAck(param_1, param_2);
		}
		break;

	case Protocol::MULTI_MOUNT_REQ_TYPE_USE_SPECIAL_IMG:
		{
			this->UseImage(param_1);
		}
		break;

	case Protocol::MULTI_MOUNT_REQ_TYPE_UPLEVEL_SPECIAL_IMG:
		{
			this->MultiMountSpecialImgUpLevel(param_1);
		}
		break;
	}
}

void MultiMount::GMActiveAllImg()
{
	for(int image_id = 1; image_id <= MULTIMOUNT_SPECIAL_IMG_MAX_ID_LIMIT; ++ image_id)
	{
		if (NULL == LOGIC_CONFIG->GetMultiMountCfg().GetMultiMountSpecialImgCfg(image_id))
		{
			break;
		}

		m_param.special_img_active_flag |= (1 << image_id);
	}

	this->SendAllInfoToRole();
}

MultiMountBindItem * MultiMount::GetMountOnInfo(int uid)
{
	int owner_uid = uid;
	if (0 == owner_uid)
	{
		owner_uid = m_role->GetUID();
	}

	// 先检查玩家是否跟随别人坐骑
	std::map<int, int>::iterator uid_it = MultiMount::m_follower_uid_to_owner_uid_map.find(owner_uid);
	if (uid_it != MultiMount::m_follower_uid_to_owner_uid_map.end())
	{
		owner_uid = uid_it->second;
	}

	std::map<int, MultiMountBindItem>::iterator binditem_it = MultiMount::m_uid_to_binditem_map.find(owner_uid);
	if (binditem_it != MultiMount::m_uid_to_binditem_map.end())
	{
		return &binditem_it->second;
	}

	return &MultiMount::m_uid_to_binditem_map[owner_uid];
}

void MultiMount::SendAllInfoToRole()
{
	static Protocol::SCMultiMountAllInfo mmai;
	mmai.cur_use_mount_id = m_couple_mount_param.cur_select_mount_id;

	mmai.special_img_active_flag = m_couple_mount_param.special_img_active_flag;
    mmai.cur_use_special_image_id = m_couple_mount_param.cur_use_img_id;

	for (int mount_id = 1; mount_id <= COUPLEMOUNT_MAX_ID; ++ mount_id)
	{
		mmai.mount_list[mount_id].is_mount_active = m_couple_mount_param.mount_list[mount_id - 1].is_mount_active;
		mmai.mount_list[mount_id].is_img_active = m_couple_mount_param.mount_list[mount_id - 1].is_img_active;
		mmai.mount_list[mount_id].star_level = m_couple_mount_param.mount_list[mount_id - 1].grade;
		mmai.mount_list[mount_id].grade_bless = m_couple_mount_param.mount_list[mount_id - 1].grade_bless;
	}

	for (int i = 0; i < COUPLEMOUNT_SPECIAL_IMG_MAX_ID_LIMIT; ++ i)
	{
		mmai.special_img_level_list[i] = m_couple_mount_param.mount_special_image_level_list[i];
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&mmai, sizeof(mmai));
}

void MultiMount::AreaBroadcastOwnerRideInfo(int follower_uid, ObjID follower_obj_id)
{
	// 广播骑乘信息改变，只给人玩家调用，协议带双方信息，方便客户端处理

	const MultiMountBindItem *bind_info = this->GetMountOnInfo();
	if (NULL != m_role->GetScene() && NULL != bind_info && bind_info->owner_uid == m_role->GetUID())
	{
		Protocol::SCMultiMountNotifyArea mac;
		mac.owner_uid = m_role->GetUID();
		mac.owner_multimount_res_id = bind_info->res_id;
		mac.owner_obj_id = bind_info->owner_obj_id;

		if (0 == follower_uid && bind_info->follower_uid > 0) // 双人骑乘
		{
			mac.follower_uid = bind_info->follower_uid;
			mac.folower_obj_id = bind_info->follower_obj_id;
			mac.folower_multimount_res_id = mac.owner_multimount_res_id;
		}
		else // 单人骑乘
		{
			mac.follower_uid = follower_uid;
			mac.folower_obj_id = follower_obj_id;
			mac.folower_multimount_res_id = 0;
		}
		m_role->GetScene()->GetZoneMatrix()->NotifyAreaMsg(m_role->GetObserHandle(), (void *)&mac, sizeof(mac));
	}
}

bool MultiMount::OnUseMultiMountCard(int mount_id, int bless)
{
	if (mount_id <= 0 || mount_id > COUPLEMOUNT_MAX_ID)
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_ID_INVALID);
		return false;
	}
	
	MultiMountParamInfo &param = m_couple_mount_param.mount_list[mount_id - 1];
	
	int old_star_level = param.grade;
	int old_bless = param.grade_bless;

	const MultiMountInfoConfig *mount_cfg = LOGIC_CONFIG->GetMultiMountCfg().GetMultiMountCfg(mount_id);
	const MultiMountInfoConfig::GradeCfg *mount_grade_cfg = LOGIC_CONFIG->GetMultiMountCfg().GetMultiMountActiveImgCfg(mount_id);
	if (NULL == mount_cfg || NULL == mount_grade_cfg)
	{
		return false;
	}

	bool aleady_got_all = true; // 本身已经达到所有要求，使用物品改为增加祝福值
	if (param.is_mount_active <= 0)
	{
		aleady_got_all = false;
		param.is_mount_active = 1;
	}
	if (param.is_img_active <= 0)
	{
		aleady_got_all = false;
		param.is_img_active = 1;
	}
	if (param.grade < mount_grade_cfg->star_level)
	{
		aleady_got_all = false;
		param.grade = mount_grade_cfg->star_level;

		if (mount_grade_cfg->active_mount_id > 0)
		{
			m_param.mount_list[mount_grade_cfg->active_mount_id - 1].is_mount_active = 1;
		}
	}

	if (aleady_got_all)
	{
		if (param.grade >= mount_cfg->max_grade)
		{
			m_role->NoticeNum(errornum::EN_MULTI_MOUNT_GRADE_IS_FULL);
			return false;
		}
		++param.grade;// 取消祝福值，使用道具卡直接升一阶

		//int target_star_level = param.star_level;
		//int target_bless = param.grade_bless;
		//int limit_bless = bless;

		//for (; target_star_level <= mount_cfg->max_grade && target_star_level <= COUPLEMOUNT_MAX_GRADE; ++ target_star_level)
		//{
		//	const MultiMountInfoConfig::GradeCfg *cur_mount_cfg = LOGIC_CONFIG->GetMultiMountCfg().GetMultiMountGradeCfg(mount_id, target_star_level);
		//	if (NULL == cur_mount_cfg) // 配置重新获取，预防本身已经超过了激活形象对应的阶数
		//	{
		//		return false;
		//	}

		//	target_bless += limit_bless;
		//	if (target_bless > cur_mount_cfg->max_bless)
		//	{
		//		limit_bless = target_bless - cur_mount_cfg->max_bless;
		//		target_bless = 0;
		//	}
		//	else
		//	{
		//		break;
		//	}
		//}

		//param.star_level = target_star_level;
		//param.grade_bless = target_bless;
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MULTI_MOUNT);

	this->SendMountChangeNotify(Protocol::MULTI_MOUNT_CHANGE_NOTIFY_TYPE_UPGRADE, mount_id, param.grade, param.grade_bless);

	gamelog::g_log_multi_mount.printf(LL_INFO, "MultiMount::OnUseMultiMountCard role[%d,%s] grade[%d->%d] bless[%d->%d]",
		m_role->GetId(), m_role->GetName(), old_star_level, param.grade, old_bless, param.grade_bless);

	return true;
}

bool MultiMount::IsOnMount()
{
	MultiMountBindItem *bind_info = this->GetMountOnInfo();
	if (NULL == bind_info)
	{
		return false;
	}

	return MULTI_MOUNT_RIDE_STATUS_NONE != bind_info->status;
}

void MultiMount::MountOn(bool is_ride)
{
	if (is_ride)
	{
		this->RideMount();
	}
	else
	{
		this->UnRideMount();
	}
}

void MultiMount::SelectMount(int mount_id)
{
	if (mount_id <= 0 || mount_id > COUPLEMOUNT_MAX_ID)
	{
		return;
	}

	if (Role::MOVE_MODE_FLY == m_role->GetMoveMode())
	{
		return;
	}

	if (m_couple_mount_param.mount_list[mount_id - 1].is_mount_active <= 0)
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_NOT_ACTIVE);
		return;
	}

	if (m_couple_mount_param.mount_list[mount_id - 1].is_img_active <= 0)
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_IMAGE_NOT_ACTIVE);
		return;
	}

	if (m_couple_mount_param.cur_select_mount_id == mount_id) // 若当前已使用这个坐骑，则表示取消使用
	{
		m_couple_mount_param.cur_select_mount_id = 0;
	}
	else
	{
		m_couple_mount_param.cur_select_mount_id = mount_id;
	}
	
	MultiMountBindItem *bind_info = this->GetMountOnInfo();
	if (NULL != bind_info && bind_info->owner_uid == m_role->GetUID()) // 自己是坐骑的主人，改变坐骑外观
	{
		if (0 != m_couple_mount_param.cur_is_riding)
		{
			if (0 == m_couple_mount_param.cur_select_mount_id) this->UnRideMount();
			else
			{
				bind_info->mount_id = m_couple_mount_param.cur_select_mount_id;
				bind_info->res_id = this->GetCurUseResID();
			}
		}
		else
		{
			if (0 != m_couple_mount_param.cur_select_mount_id) this->RideMount();
		}
	}

	this->SendMountChangeNotify(Protocol::MULTI_MOUNT_CHANGE_NOTIFY_TYPE_SELECT_MOUNT, m_couple_mount_param.cur_select_mount_id);
	this->AreaBroadcastOwnerRideInfo();
}

bool MultiMount::Upgrade(int mount_id, bool is_auto_buy)
{
	if (mount_id <= 0 || mount_id > COUPLEMOUNT_MAX_ID)
	{
		return false;
	}

	/*if (m_couple_mount_param.mount_list[mount_id - 1].is_mount_active <= 0)
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_NOT_ACTIVE);
		return false;
	}*/

	MultiMountParamInfo &param = m_couple_mount_param.mount_list[mount_id - 1];
	
	const MultiMountInfoConfig::GradeCfg *cur_grade_cfg = LOGIC_CONFIG->GetMultiMountCfg().GetMultiMountGradeCfg(mount_id, param.grade);
	const MultiMountInfoConfig::GradeCfg *next_grade_cfg = LOGIC_CONFIG->GetMultiMountCfg().GetMultiMountGradeCfg(mount_id, param.grade + 1);
	if (NULL == cur_grade_cfg)
	{
		return false;
	}

	if (NULL == next_grade_cfg)
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_ALEADY_MAX_GRADE);
		return false;
	}

	//Knapsack *knapsack = m_role->GetKnapsack();

	static ItemExtern::ItemConsumeList consume_list; 
	consume_list.Reset();
	{
		short stuff_count = 0;

		// 进阶符
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[stuff_count].item_id = cur_grade_cfg->upgrade_stuff_id;
		stuff_list[stuff_count].num = cur_grade_cfg->upgrade_stuff_num;
		stuff_list[stuff_count].buyable = is_auto_buy;

		++ stuff_count;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, stuff_count, stuff_list, &consume_list, true))
		{
			return false;
		}
	}

	int old_grade = param.grade;
	int old_bless = param.grade_bless;

	int target_grade = param.grade;
	int target_bless = param.grade_bless;

	int add_bless = cur_grade_cfg->GetRandFailBless();
	if (add_bless <= 0)
	{
		return false;
	}

	// 取消祝福值，直接升星
	/*target_bless += add_bless;
	if (target_bless >= cur_grade_cfg->max_bless)
	{
		++target_grade;
		target_bless -= cur_grade_cfg->max_bless;
	}*/
	++target_grade;

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "MultiMount::Upgrade"))
	{
		return false;
	}

	// 给予
	param.grade = target_grade;
	param.grade_bless = target_bless;

	if (param.grade > old_grade) // 升阶成功，检查是否激活形象或者激活其他坐骑
	{
		if (next_grade_cfg->active_mount_id > 0)
		{
			m_couple_mount_param.mount_list[next_grade_cfg->active_mount_id - 1].is_mount_active = 1;
		}

		if (next_grade_cfg->is_active_image)
		{
			param.is_img_active = 1;
		}

		if (next_grade_cfg->is_broadcast)
		{
			int length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_multi_mount_upgrade_content, 
				m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp(), mount_id, param.grade);
			if (length > 0) 
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE_3);
			}
		}
	}

	this->SendMountChangeNotify(Protocol::MULTI_MOUNT_CHANGE_NOTIFY_TYPE_UPGRADE, mount_id, param.grade, param.grade_bless);

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MULTI_MOUNT);

	gamelog::g_log_multi_mount.printf(LL_INFO, "MultiMount::Upgrade role[%d,%s] star_level[%d->%d] bless[%d->%d] consume_item[%d,%d] consume_gold[%d]",
		m_role->GetUID(), m_role->GetName(), old_grade, param.grade, old_bless, param.grade_bless, cur_grade_cfg->upgrade_stuff_id, cur_grade_cfg->upgrade_stuff_num, consume_list.need_gold_num);
	
	return true;
}

void MultiMount::RideMount()
{
	if (this->IsOnMount())
	{
		return;
	}

	if (!m_role->GetRoleStatusManager()->CanMountOn())
	{
		return;
	}

	if (Role::MOVE_MODE_FLY == m_role->GetMoveMode())
	{
		return;
	}

	if (m_couple_mount_param.cur_select_mount_id <= 0) // 当前未选择坐骑
	{
		return;
	}

	if (m_role->GetFightMountManager()->IsOnFightMount())
	{
		m_role->GetFightMountManager()->GoonFightMount(false, false);
	}

	if (m_role->GetMountManager()->IsOnMount())
	{
		m_role->GetMountManager()->GoonMount(false);
	}

	MultiMountBindItem *bind_info = this->GetMountOnInfo();
	if (NULL == bind_info)
	{
		return;
	}

	m_param.cur_is_riding = 1;

	bind_info->owner_obj_id = m_role->GetId();
	bind_info->status = MULTI_MOUNT_RIDE_STATUS_OWNER_RIDE;
	bind_info->mount_star_level = m_couple_mount_param.mount_list[m_param.cur_select_mount_id - 1].grade;
	bind_info->mount_id = m_couple_mount_param.cur_select_mount_id;
	bind_info->res_id = this->GetCurUseResID();

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MULTI_MOUNT);

	this->AreaBroadcastOwnerRideInfo();
}

void MultiMount::UnRideMount()
{
	if (!this->IsOnMount())
	{
		return;
	}

	MultiMountBindItem *bind_info = this->GetMountOnInfo();
	if (NULL == bind_info)
	{
		return;
	}
	Role *owner_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(bind_info->owner_uid));
	Role *follower_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(bind_info->follower_uid));

	if (NULL == owner_role) // 拥有者不能为空
	{
		return;
	}

	m_couple_mount_param.cur_is_riding = 0;
	int old_follower_uid = 0;
	ObjID old_follower_obj_id = 0;

	// 处理跟随者
	if (MULTI_MOUNT_RIDE_STATUS_TOW_RIDE == bind_info->status && NULL != follower_role)
	{
		MultiMount::m_follower_uid_to_owner_uid_map.erase(bind_info->follower_uid);

		follower_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MULTI_MOUNT);

		old_follower_uid = bind_info->follower_uid;
		old_follower_obj_id = bind_info->follower_obj_id;
		bind_info->follower_uid = 0;
		bind_info->follower_obj_id = INVALID_OBJ_ID;

		// 更新跟随者自己的那份数据
		MultiMountBindItem *follower_bind_info = this->GetMountOnInfo(follower_role->GetUID());
		if (NULL != bind_info && follower_role->GetUID() == follower_bind_info->owner_uid)
		{
			follower_bind_info->owner_obj_id = follower_role->GetId();
			follower_bind_info->res_id = follower_role->GetMultiMount()->GetCurUseResID();
		}
	}

	if (bind_info->owner_uid == m_role->GetUID()) // 拥有者下坐骑，变成下坐骑状态
	{
		bind_info->status = MULTI_MOUNT_RIDE_STATUS_NONE;
		bind_info->mount_id = 0;
		bind_info->res_id = 0;
	}
	else // 跟随者下坐骑，变成单人骑乘状态
	{
		bind_info->status = MULTI_MOUNT_RIDE_STATUS_OWNER_RIDE;
	}

	bind_info->invite_uid = 0;

	owner_role->GetMultiMount()->AreaBroadcastOwnerRideInfo(old_follower_uid, old_follower_obj_id);

	if (follower_role)
	{
		follower_role->ResetPos(owner_role->GetPos().x, owner_role->GetPos().y);
	}

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MULTI_MOUNT);
}

void MultiMount::InviteRide(int uid)
{
	if (!m_role->GetScene()->InStaticScene())
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_NORMAL_SCENE_CAN_INVITE);
		return;
	}

	Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (NULL == target_role)
	{
		return;
	}

	if (!target_role->GetRoleStatusManager()->CanMountOn())
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_OTHER_CANNOT_MOUNT_ON);
		return;
	}

	MultiMountBindItem *target_bind_info = this->GetMountOnInfo(uid);
	if (NULL == target_bind_info)
	{
		return;
	}

	if (MULTI_MOUNT_RIDE_STATUS_TOW_RIDE == target_bind_info->status)
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_OWNER_MOUNT_IS_FULL);
		return;
	}

	if (target_role->GetScene() != m_role->GetScene())
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_PARTNER_TOO_FARAWAY);
		return;
	}

	const Posi &p1 = target_role->GetPos();
	const Posi &p2 = m_role->GetPos();
	int distance1 = abs(p1.x - p2.x);
	int distance2 = abs(p1.y - p2.y);

	if (distance1 > MULTI_MOUNT_INVITE_MAX_DISTANCE || distance1 < -MULTI_MOUNT_INVITE_MAX_DISTANCE ||
		distance2 > MULTI_MOUNT_INVITE_MAX_DISTANCE || distance2 < -MULTI_MOUNT_INVITE_MAX_DISTANCE)
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_PARTNER_TOO_FARAWAY);
		return;
	}

	MultiMountBindItem *bind_info = this->GetMountOnInfo();
	if (NULL == bind_info)
	{
		return;
	}

	if (MULTI_MOUNT_RIDE_STATUS_NONE == bind_info->status)
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_IS_NOT_RIDING);
		return;
	}

	if (MULTI_MOUNT_RIDE_STATUS_TOW_RIDE == bind_info->status)
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_ALEADY_TOW_RIDE);
		return;
	}

	bind_info->invite_uid = uid;
	bind_info->follower_uid = 0;
	bind_info->follower_obj_id = INVALID_OBJ_ID;
	bind_info->status = MULTI_MOUNT_RIDE_STATUS_SEND_REQUEST;

	target_role->GetMultiMount()->SendMountChangeNotify(Protocol::MULTI_MOUNT_CHANGE_NOTIFY_TYPE_INVITE_RIDE, m_role->GetUID());
}

void MultiMount::InviteRideAck(int uid, int agree_ride)
{
	if (!m_role->GetScene()->InStaticScene())
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_NORMAL_SCENE_CAN_INVITE);
		return;
	}
	
	MultiMountBindItem *my_bind_info = this->GetMountOnInfo();
	if (NULL == my_bind_info)
	{
		return;
	}

	if (MULTI_MOUNT_RIDE_STATUS_TOW_RIDE == my_bind_info->status)
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_MY_MOUNT_IS_FULL);
		return;
	}

	MultiMountBindItem *bind_info = this->GetMountOnInfo(uid);
	if (NULL == bind_info)
	{
		return;
	}

	if (uid != bind_info->owner_uid)
	{
		return;
	}

	if (MULTI_MOUNT_RIDE_STATUS_NONE == bind_info->status) // 对方已下坐骑
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_OWNER_NOT_RIDING);
		return;
	}
	else if (MULTI_MOUNT_RIDE_STATUS_OWNER_RIDE == bind_info->status) // 对方没有发出邀请
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_OWNER_NOT_INVITE);
		return;
	}
	else if (MULTI_MOUNT_RIDE_STATUS_TOW_RIDE == bind_info->status) // 对方坐骑已坐满
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_OWNER_MOUNT_IS_FULL);
		return;
	}

	if (bind_info->invite_uid != m_role->GetUID()) // 对方已经改邀请了其他人
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_OWNER_CHANGE_INVITE);
		return;
	}

	// 先清零，防止下面直接返回而没有重置
	bind_info->invite_uid = 0;
	bind_info->follower_uid = 0;
	bind_info->follower_obj_id = INVALID_OBJ_ID;

	if (!m_role->GetRoleStatusManager()->CanMountOn())
	{
		return;
	}
	
	Role *owner_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(uid));
	if (NULL == owner_role)
	{
		return;
	}
	
	if (owner_role->GetScene() != m_role->GetScene())
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_PARTNER_TOO_FARAWAY);
		return;
	}

	const Posi &p1 = owner_role->GetPos();
	const Posi &p2 = m_role->GetPos();
	int distance1 = abs(p1.x - p2.x);
	int distance2 = abs(p1.y - p2.y);

	if (distance1 > MULTI_MOUNT_INVITE_MAX_DISTANCE || distance1 < -MULTI_MOUNT_INVITE_MAX_DISTANCE ||
		distance2 > MULTI_MOUNT_INVITE_MAX_DISTANCE || distance2 < -MULTI_MOUNT_INVITE_MAX_DISTANCE)
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_PARTNER_TOO_FARAWAY);
		return;
	}

	if (agree_ride > 0)
	{
		MultiMountBindItem *own_bind_info = this->GetMountOnInfo();
		if (NULL != own_bind_info) // 关联之前，先清理自己本身的状态
		{
			own_bind_info->invite_uid = 0;
			own_bind_info->follower_uid = 0;
			own_bind_info->follower_obj_id = INVALID_OBJ_ID;
			own_bind_info->status = MULTI_MOUNT_RIDE_STATUS_NONE;
		}

		bind_info->status = MULTI_MOUNT_RIDE_STATUS_TOW_RIDE;
		bind_info->owner_obj_id = owner_role->GetId();
		bind_info->follower_uid = m_role->GetUID();
		bind_info->follower_obj_id = m_role->GetId();
		MultiMount::m_follower_uid_to_owner_uid_map[m_role->GetUID()] = uid;

		m_role->ResetPos(owner_role->GetPos().x, owner_role->GetPos().y);
		m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MULTI_MOUNT);

		owner_role->GetMultiMount()->AreaBroadcastOwnerRideInfo();
	}
	else
	{
		bind_info->status = MULTI_MOUNT_RIDE_STATUS_OWNER_RIDE;
	}
}

void MultiMount::SendMountChangeNotify(short notify_type, int param_1, int param_2, int param_3)
{
	static Protocol::SCMultiMountChangeNotify mmcn;
	mmcn.notify_type = notify_type;
	mmcn.param_1 = param_1;
	mmcn.param_2 = param_2;
	mmcn.param_3 = param_3;
	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (char*)&mmcn, sizeof(mmcn));
}

bool MultiMount::ActiveSpecialImage(int imageid)
{
	if (imageid <= 0 || imageid > MULTIMOUNT_SPECIAL_IMG_MAX_ID_LIMIT)
	{
		return false;
	}

	if (0 != (m_couple_mount_param.special_img_active_flag & (1 << imageid)))
	{
		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_SPECIAL_IMG_ALREADY_ACTIVE);
		return false;
	}

	const MultiMountSpecialConfig *cfg_item = LOGIC_CONFIG->GetMultiMountCfg().GetMultiMountSpecialImgCfg(imageid);
	if (NULL == cfg_item)
	{
		return false;
	}
	
	int old_special_img_active_flag = m_couple_mount_param.special_img_active_flag;
	m_couple_mount_param.special_img_active_flag |= (1 << imageid);
	if (0 == m_couple_mount_param.mount_special_image_level_list[imageid])
	{
		m_couple_mount_param.mount_special_image_level_list[imageid] = 1;
	}

	this->SendMountChangeNotify(Protocol::MULTI_MOUNT_CHANGE_NOTIFY_TYPE_ACTIVE_SPECIAL_IMG, m_couple_mount_param.special_img_active_flag, imageid, m_couple_mount_param.mount_special_image_level_list[imageid]);
	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MULTI_MOUNT);
	m_role->GetDressingRoomMgr()->CheckSuitChange(DressingRoomConfig::SPECIAL_IMG_TYPE_MULIT_MOUNT, imageid, true);

	gamelog::g_log_multi_mount.printf(LL_INFO, "MultiMount::ActiveSpecialImage role[%d,%s] imageid[%d] special_img_active_flag[%d->%d], level[%d]",
				m_role->GetId(), m_role->GetName(), imageid, old_special_img_active_flag, m_couple_mount_param.special_img_active_flag, m_couple_mount_param.mount_special_image_level_list[imageid]);

	return true;
}

bool MultiMount::UseImage(int imageid)
{
	if (imageid < 0 || imageid > COUPLEMOUNT_SPECIAL_IMG_MAX_ID_LIMIT)
	{
		return false;
	}

	if (0 != imageid)
	{
		if (0 == (m_couple_mount_param.special_img_active_flag & (1 << imageid)))
		{
			m_role->NoticeNum(errornum::EN_MULTI_MOUNT_SPECIAL_IMG_NOT_ACTIVE);
			return false;
		}
	}

	int old_use_img_id = m_couple_mount_param.cur_use_img_id;
	m_couple_mount_param.cur_use_img_id = imageid;

	MultiMountBindItem *bind_info = this->GetMountOnInfo();
	if (NULL != bind_info && bind_info->owner_uid == m_role->GetUID())
	{
		bind_info->res_id = this->GetCurUseResID();
		this->AreaBroadcastOwnerRideInfo();
	}

	this->SendMountChangeNotify(Protocol::MULTI_MOUNT_CHANGE_NOTIFY_TYPE_USE_SPECIAL_IMG, m_couple_mount_param.cur_use_img_id);

	gamelog::g_log_multi_mount.printf(LL_INFO, "MultiMount::UseImage role[%d,%s] use_img_id[%d->%d]",
				m_role->GetId(), m_role->GetName(), old_use_img_id, m_couple_mount_param.cur_use_img_id);

	return true;
}

void MultiMount::UpgradeEquip(int equip_type)
{
// 	if (equip_type < 0 || equip_type > MULTIMOUNT_EQUIP_TYPE_NUM)
// 	{
// 		return;
// 	}
// 
// 	const MultimountEquipConfig *equip_cfg = LOGIC_CONFIG->GetMultiMountCfg().GetEquipCfg(equip_type);
// 	if (NULL == equip_cfg)
// 	{
// 		return;
// 	}
// 
// 	if (m_param.equip_level_list[equip_type] >= equip_cfg->max_level)
// 	{
// 		m_role->NoticeNum(errornum::EN_MULTI_MOUNT_EQUIP_ALEADY_MAX_LEVEL);
// 		return;
// 	}
// 
// 	if (m_role->GetKnapsack()->Count(equip_cfg->upgrade_need_stuff) < equip_cfg->upgrade_stuff_count)
// 	{
// 		m_role->NoticeNum(errornum::EN_ITEM_NUM_NOT_ENOUGH);
// 		return;
// 	}
// 
// 	if (!m_role->GetKnapsack()->ConsumeItem(equip_cfg->upgrade_need_stuff, equip_cfg->upgrade_stuff_count, "MultiMount::UpgradeEquip"))
// 	{
// 		return;
// 	}
// 
// 	int old_level = m_param.equip_level_list[equip_type];
// 	++ m_param.equip_level_list[equip_type];
// 
// 	this->SendMountChangeNotify(Protocol::MULTI_MOUNT_CHANGE_NOTIFY_TYPE_UPGRADE_EQUIP, equip_type, m_param.equip_level_list[equip_type]);
// 	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MULTI_MOUNT);
// 
// 	gamelog::g_log_multi_mount.printf(LL_INFO, "MultiMount::UpgradeEquip role[%d,%s] equip_type[%d] level[%d->%d]",
// 		m_role->GetId(), m_role->GetName(), equip_type, old_level, m_param.equip_level_list[equip_type]);
}

void MultiMount::MultiMountSpecialImgUpLevel(int special_img_id)
{
	if (special_img_id <= 0 || special_img_id >= COUPLEMOUNT_SPECIAL_IMG_MAX_ID_LIMIT)
	{
		return;
	}

	Knapsack *knapsack = m_role->GetKnapsack();
	if (knapsack->IsLock())
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_IS_LOCK);
		return;
	}

	if (!this->IsSpecialImageActive(special_img_id))
	{
		m_role->NoticeNum(errornum::EN_MOUNT_IMG_NOT_ACTIVE);
		return;
	}

	const int old_grade = m_couple_mount_param.mount_special_image_level_list[special_img_id];

	const MultimountSpecialImgUplevelCfg *level_cfg = LOGIC_CONFIG->GetMultiMountCfg().GetSpecialImgUplevelCfg(special_img_id, old_grade);
	if (NULL == level_cfg)
	{
		return;
	}

	const MultimountSpecialImgUplevelCfg *next_level_cfg = LOGIC_CONFIG->GetMultiMountCfg().GetSpecialImgUplevelCfg(special_img_id, old_grade + 1);
	if (NULL == next_level_cfg)
	{
		return;
	}

	static ItemExtern::ItemConsumeList consume_list; consume_list.Reset();

	{
		// 进阶符
		ItemExtern::ItemConsumeConfig stuff_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM];
		stuff_list[0].item_id = level_cfg->stuff_id;
		stuff_list[0].num = level_cfg->stuff_num;
		stuff_list[0].buyable = false;

		if (!ItemExtern::GetConsumeListByOrder(m_role, false, 1, stuff_list, &consume_list, true))
		{
			return;
		}
	}

	// 消耗
	if (!ItemExtern::ConsumeItem(m_role, consume_list, "MultiMountSpecialImgUpLevel"))
	{
		return;
	}

	// 生效
	++ m_couple_mount_param.mount_special_image_level_list[special_img_id];

	m_role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MULTI_MOUNT);

	this->SendMountChangeNotify(Protocol::MULTI_MOUNT_CHANGE_NOTIFY_TYPE_UPLEVEL_SPECIAL_IMG, special_img_id, m_couple_mount_param.mount_special_image_level_list[special_img_id]);

	gamelog::g_log_multi_mount.printf(LL_INFO, "MultiMount::MultiMountSpecialImgUpLevel role[%d,%s] special_img_id[%d], grade[%d -> %d]",
		m_role->GetId(), m_role->GetName(), special_img_id, old_grade, m_couple_mount_param.mount_special_image_level_list[special_img_id]);
}

//// 幻化特殊战力加成
//void MultiMount::HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add)
//{
//	if (!LOGIC_CONFIG->GetMultiMountCfg().IsAddSpecialCap())
//	{
//		return;
//	}
//
//	auto cfg_item = LOGIC_CONFIG->GetMultiMountCfg().GetHuanHuaSpecialCapAddCfg(huanhua_id);
//	if (nullptr == cfg_item)
//	{
//		return;
//	}
//
//	if (huanhua_level >= cfg_item->huanhua_level)
//	{
//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += cfg_item->maxhp;
//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += cfg_item->gongji;
//		m_attrs_add.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += cfg_item->fangyu;
//	}
//}
