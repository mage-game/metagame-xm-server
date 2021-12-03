#include "magicalprecious.hpp"
#include "obj/character/role.h"
#include "config/logicconfigmanager.hpp"
#include "servercommon/errornum.h"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/struct/shizhuangparam.hpp"
#include "servercommon/zhuanzhiequipdef.hpp"
#include "magicalpreciouscfg.hpp"
#include "world.h"
#include "global/worldstatus/worldstatus.hpp"
#include "equipment/equipmentmanager.h"
#include "other/xiannv/xiannvmanager.hpp"
#include "other/runesystem/runesystem.hpp"
#include "item/knapsack.h"
#include "other/shop/roleshop.hpp"
#include "other/capability/capability.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "equipment/stonemananger.hpp"
#include "other/wing/wingmanager.hpp"
#include "other/halo/halomanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/equipsuit/equipsuit.hpp"
#include "other/fb/rolepatafb.hpp"
#include "other/fb/rolefbtuitu.h"
#include "protocal/msgactivity.hpp"
#include "other/jingling/jinglingmanager.h"
#include "other/shenzhuang/shenzhuang.h"
#include "global/globalrole/globalrolemanager.hpp"
#include "global/globalrole/qingyuanglobal/qingyuanglobal.hpp"
#include "global/challengefield/challengefield.hpp"
#include "other/shenzhouweapon/shenzhouweapon.hpp"
#include "other/lieming/liemingmanager.h"
#include "other/shengesystem/shengesystem.hpp"
#include "other/shenshou/shenshou.hpp"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include "other/zhuanzhiequip/zhuanzhiequip.h"
#include "other/fb/rolefbarmordefend.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "other/fb/rolefbequip.hpp"
#include "other/shizhuang/shizhuang.h"
#include "other/shengqi/shengqi.hpp"
#include "other/fabao/fabaomanager.hpp"
#include "other/footprint/footprint.hpp"
#include "other/zhuanzhiequip/zhuanzhiequip.h"
#include "other/xiannv/xiannvmanager.hpp"
#include "other/qingyuan/qingyuan.hpp"
#include "other/fb/fbequipconfig.hpp"
#include "other/heshenluoshu/heshenluoshu.hpp"
#include "other/equipbaptize/equipbaptize.hpp"
#include "other/upgrade/upgrademanager.hpp"
#include "other/greatesoldier/greatesoldier.hpp"
#include "other/mask/maskmanager.hpp"
#include "other/qilinbi/qilinbimanager.hpp"
#include "other/toushi/toushimanager.hpp"
#include "other/yaoshi/yaoshimanager.hpp"
#include "scene/scene.h"
#include "other/shenqi/shenqi.hpp"


MagicalPrecious::MagicalPrecious() : m_role(NULL)
{
}

MagicalPrecious::~MagicalPrecious()
{

}

void MagicalPrecious::Init(Role *role, const MagicalPreciousParam &param)
{
	m_role = role;
	m_param = param;
	m_is_send_all = false;
}

void MagicalPrecious::GetInitParam(MagicalPreciousParam *param)
{
	*param = m_param;
}

void MagicalPrecious::Update(unsigned int now_second)
{
	if (m_param.current_chapter >= 0 && m_param.current_chapter < MAGICAL_PRECIOUS_CHAPTER_COUNT && m_param.chapter_invalid_time >= 0)
	{
		if (now_second > m_param.chapter_invalid_time)
		{

			m_param.current_chapter += 1;
			if (m_param.current_chapter >= LOGIC_CONFIG->GetMagicalPreciousCfg().GetMaxChapter())
			{
				m_param.current_chapter = LOGIC_CONFIG->GetMagicalPreciousCfg().GetMaxChapter() - 1;
			}

			m_param.chapter_invalid_time += SECOND_PER_DAY;

			this->SendMagicalPreciousAllInfo();
			m_is_send_all = false;
		}
	}

	if (m_is_send_all)
	{
		m_is_send_all = false;
		SendMagicalPreciousAllInfo();
	}
}

void MagicalPrecious::OnDayChange(unsigned int old_dayid, unsigned int now_dayid)
{

}

void MagicalPrecious::OnRoleLevelChange(int level)
{
	if (m_param.current_chapter >= 0 || m_param.current_chapter == FINISH_CHAPTER_ID || 0 != m_param.chapter_invalid_time) return;

	const MagicalPreciousOtherCfg &other_cfg = LOGIC_CONFIG->GetMagicalPreciousCfg().GetOtherCfg();
	const MagicalPreciousFinishChapterRewardCfg *chapter_cfg = LOGIC_CONFIG->GetMagicalPreciousCfg().GetFinishChapterRewardCfg(0, 0);
	if (level >= other_cfg.open_level || NULL != chapter_cfg)
	{
		// 到达开启等级
		m_param.current_chapter = 0;
		m_param.chapter_invalid_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + static_cast<unsigned int>(EngineAdapter::Instance().NextDayInterval(0, 0, 0));
		this->SendMagicalPreciousAllInfo();
	}
}

void MagicalPrecious::FetchMagicalPreciousReward(int reward_index, int chapter)
{
	if (chapter < 0 || chapter >= LOGIC_CONFIG->GetMagicalPreciousCfg().GetMaxChapter() || chapter >= MAGICAL_PRECIOUS_MAX_CHAPTER || chapter > m_param.current_chapter)
	{
		return;
	}

	//现在是不会失效的
	//if (m_param.chapter_invalid_time <= 0 || static_cast<unsigned int>(EngineAdapter::Instance().Time()) > m_param.chapter_invalid_time) return;

	const MagicalPreciousChapterCfg *chapter_cfg = LOGIC_CONFIG->GetMagicalPreciousCfg().GetChapterTaskCfg(chapter, reward_index);
	if (NULL == chapter_cfg)
	{
		return;
	}

	if (this->IsFetchReward(reward_index, chapter))
	{
		return;
	}

	bool can_fetch_reward = false;
	switch (chapter_cfg->condition_type)
	{
	case CONDITION_TYPE_TOTAL_GONGJI:
	{
		if (m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_TOTAL_FANGYU:
	{
		if (m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_TOTAL_MAXHP:
	{
		if (m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_TOTAL_SHANBI:
	{
		if (m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_TOTAL_MINGZHONG:
	{
		if (m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_TOTAL_BAOJI:
	{
		if (m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_TOTAL_JIANREN:
	{
		if (m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_MOUNT_GRADE:
	{
		if (m_role->GetMountManager()->GetMountParam()->grade >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_WING_GRADE:
	{
		if (m_role->GetWing()->GetWingParam()->grade >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_HALO_GRADE:
	{
		if (m_role->GetHalo()->GetHaloParam()->grade >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHENGONG_GRADE:
	{
		if (m_role->GetShengong()->GetShengongParam()->grade >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHENYI_GRADE:
	{
		if (m_role->GetShenyi()->GetShenyiParam()->grade >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_FIGHT_MOUNT_GRADE:
	{
		if (m_role->GetFightMountManager()->GetFightMountParam()->grade >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHIZHUANG_GRADE:
	{
		if (m_role->GetShizhuang()->GetShiZhuangGrade(SHIZHUANG_TYPE_BODY) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHENBING_GRADE:
	{
		if (m_role->GetShizhuang()->GetShiZhuangGrade(SHIZHUANG_TYPE_WUQI) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_FABAO_GRADE:
	{
		if (m_role->GetFabaoManager()->GetGrade() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_FOOTPRINT_GRADE:
	{
		if (m_role->GetFootPrint()->GetGrade() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_JINEJIE_LEVEL:
	{
		if (m_role->GetJingJieLevel() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_MOUNT_STAR:
	{
		if (static_cast<int>(m_role->GetMountManager()->GetMountParam()->star_level) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_WING_STAR:
	{
		if (static_cast<int>(m_role->GetWing()->GetWingParam()->star_level) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_HALO_STAR:
	{
		if (static_cast<int>(m_role->GetHalo()->GetHaloParam()->star_level) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHENGONG_STAR:
	{
		if (static_cast<int>(m_role->GetShengong()->GetShengongParam()->star_level) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHENYI_STAR:
	{
		if (static_cast<int>(m_role->GetShenyi()->GetShenyiParam()->star_level) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_FIGHT_MOUNT_STAR:
	{
		if (static_cast<int>(m_role->GetFightMountManager()->GetFightMountParam()->star_level) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_ALL_CAPACITY:
	{
		if (m_role->GetCapability()->GetTotalCapability() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_RUNESYSTER_CAPACITY:
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_RUNE_SYSTEM) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHENGE_CAPACITY:
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENGE_SYSTEM) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHENZHOU_WEAPON_CAPACITY:
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENZHOU_WEAPON) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_CHINESE_ZODIAC_CAPACITY:
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_CHINESE_ZODIAC) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_JINGLING_CAPACITY:
	{
		if (m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_JINGLING_CHENGZHANG:
	{
		if (m_role->GetJingLingMgr()->GetHistoryMaxChengzhang() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_JINGLING_WUXING:
	{
		if (m_role->GetJingLingMgr()->GetHistoryMaxWuxing() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_XIANNV_SHENWU_CAP:
	{
		if (m_role->GetXiannvManager()->GetShengwuBaseCap() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_EQUIP_STRENGTHEN:
	{
		if (m_role->GetEquipmentManager()->GetTotalStrengthen() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_EQUIP_STAR:
	{
		if (m_role->GetZhuanzhiEquip()->GetEquipTotalStarLevel() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_EQUIP_STONE:
	{
		if (m_role->GetStoneManager()->GetTotalStoneLevel() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_ROLE_LEVEL:
	{
		if (m_role->GetLevel() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_XIANNV_ZIZHI_LEVEL:
	{
		if (m_role->GetXiannvManager()->GetMaxXiannvZizhi() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_RUNETOWER_LEVEL:
	{
		if (m_role->GetRuneSystem()->GetRuneTowerPassLayer() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_PATA_FB_LEVEL:
	{
		if (m_role->GetRolePataFB()->GetPassLevel() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_TUITU_FB_NORMAL:
	{
		if (m_role->GetTuituFb()->IsPassFb(TUITU_FB_TYPE_NORMAL, chapter_cfg->param_a, chapter_cfg->param_b))
		{
			can_fetch_reward = true;
		}
	}
	break;

	//case CONDITION_TYPE_TUITU_FB_HARD:
	//{
	//	if (m_role->GetTuituFb()->IsPassFb(TUITU_FB_TYPE_HARD, chapter_cfg->param_a, chapter_cfg->param_b))
	//	{
	//		can_fetch_reward = true;
	//	}
	//}
	//break;

	case CONDITION_TYPE_TEAMEQUIP_FB_LEVEL:
	{
		const FBEquipLayerCfg *cur_layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgBySceneID(m_role->GetScene()->GetSceneID());
		if (nullptr != cur_layer_cfg)
		{
			if (cur_layer_cfg->layer >= chapter_cfg->param_a)
			{
				can_fetch_reward = true;
			}
		}
	}
	break;

	case CONDITION_TYPE_ARMORDEFEND_FB_LEVEL:
	{
		int pass_level = m_role->GetRoleFbArmorDefend()->GetArmoedefendPassLayer();
		if (pass_level >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_TOWERDEFEND_FB_LEVEL:
	{
		int pass_level = m_role->GetFBTowerDefend()->GetPersonalFBPassLevel();
		if (pass_level >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_TOWERDEFEND_TEAM_FB_WAVE:
	{
		int pass_wave = m_role->GetFBTowerDefend()->GetTeamPassdeWave();
		if (pass_wave >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_CHALLENGE_RANK:
	{
		int rank_pos = ChallengeField::Instance().GetChallengeUserRankPos(m_role);
		if (rank_pos > 0 && rank_pos <= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_NEWEQUIP_FB_LEVEL:
	{
		if (m_role->GetFBEquip()->GetNeqFBPassMaxLevel() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_GUILD_TASK:
	{
		if (m_param.finish_guild_task_times >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_DAILY_TASK:
	{
		if (m_param.finish_daily_task_times >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_WABAO:
	{
		if (m_param.wabao_times >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_OPEN_GUILD_BOX:
	{
		if (m_param.open_guild_box_times >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_HOTSPRING_JOIN:
	{
		if (m_param.husong_times >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_HOTSPRING_ANSWER_RIGHT:
	{
		if (m_param.hotspring_answer_right_times >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_MILLIONAIRE_BOX:
	{
		if (m_param.millionaire_box_times >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_HUSONG:
	{
		if (m_param.husong_times >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_ENTER_EXP_FB:
	{
		if (m_param.enter_exp_fb_times >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_PASS_PHASE_FB:
	{
		if (m_param.pass_phase_fb_times >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_WANGLING_TASK:
	{
		if (m_param.wangling_task_times >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_TIANJIANGCAIBAO_TASK:
	{
		if (m_param.tianjiangcaibao_task_times >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHUIJING_TASK:
	{
		if (m_param.shuijing_task_times >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHUIJING_GATHER:
	{
		if (m_param.shuijing_gather_times >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_GONGCHENG_ZHAN:
	{
		if (m_param.join_gongcheng_zhan_times >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case  CONDITION_TYPE_KILL_MIKU_BOSS:
	{
		if (m_param.kill_miku_boss >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_KILL_WORLD_BOSS:
	{
		if (m_param.kill_world_boss >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_KILL_ACTIVE_BOSS:
	{
		if (m_param.kill_active_boss >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case  CONDITION_TYPE_KILL_DABAO_BOSS:
	{
		if (m_param.kill_dabao_boss >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case  CONDITION_TYPE_KILL_VIP_BOSS:
	{
		if (m_param.kill_vip_boss >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case  CONDITION_TYPE_KILL_VIP_MIKU_BOSS1:
	{
		if (m_param.kill_boss_count_1 >= chapter_cfg->param_b)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case  CONDITION_TYPE_KILL_VIP_MIKU_BOSS2:
	{
		if (m_param.kill_boss_count_2 >= chapter_cfg->param_b)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case  CONDITION_TYPE_KILL_VIP_MIKU_BOSS3:
	{
		if (m_param.kill_boss_count_3 >= chapter_cfg->param_b)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case  CONDITION_TYPE_KILL_ROLE_IN_FIGHTING:
	{
		if (m_param.kill_other_role_in_fighting >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case  CONDITION_TYPE_MINE_TIMES:
	{
		if (m_param.mine_times >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case  CONDITION_TYPE_FISH_NUM:
	{
		if (m_param.fish_num >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SAILING_NUM:
	{
		if (m_param.sailing_num >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_ONLINE_TIME:
	{
		if (m_role->GetOnlineTime() / 3600 >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_KILL_THREE_TYPE_BOSS:
	{
		if (m_param.kill_miku_boss + m_param.kill_world_boss + m_param.kill_active_boss >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_KILL_MONSTER_NUM:
	{
		if (m_param.kill_monster_num >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_ACTIVE_DEGREE:
	{
		if (m_param.active_degree >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_MARRY_SUCC:
	{
		if (m_role->IsMarry())
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_MARRY_SUCC_TYPE:
	{
		if (m_role->GetQingyuan()->IsHadPropose(chapter_cfg->param_a))
		{
			can_fetch_reward = true;
		}
	}

	case CONDITION_TYPE_PROF_LEVEL:
	{
		if (m_role->GetProfLevel() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_GONGCHENG_ZHAN_WINER:
	{
		int winer_guild_id = WorldStatus::Instance().GetGCZWinGuild();
		if (m_role->GetGuildID() == winer_guild_id)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case ZHUANGBEI_CONDITION_TYPE:
	{
		if (m_role->GetEquipmentManager()->IsSatisfyMagicalPrecious(chapter_cfg->param_a, chapter_cfg->param_b, chapter_cfg->param_c))
		{
			can_fetch_reward = true;
		}
	}
	break;

	case STONE_CONDITION_TYPE:
	{
		if (m_role->GetStoneManager()->IsSatisfyMagicalPrecious(chapter_cfg->param_a, chapter_cfg->param_b))
		{
			can_fetch_reward = true;
		}
	}
	break;

	case DUANZAO_CONDITION_TYPE:
	{
		if (m_role->GetEquipSuit()->IsSatisfyMagicalPrecious(chapter_cfg->param_a, chapter_cfg->param_b))
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_WEAR_EQUIPMENT:
	{
		if (m_role->GetEquipmentManager()->GetSatisfyMagicalPrecious2Count(chapter_cfg->param_b, chapter_cfg->param_c) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_WEAR_ZHUANZHI_EQUIPMENT:
	{
		int count = m_role->GetZhuanzhiEquip()->GetSatisfyPutonCount(chapter_cfg->param_b, chapter_cfg->param_c);
		if (count >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_WEAR_MARRY_EQUIPMENT:
	{
		GlobalRole *g_role = GlobalRoleManager::Instance().GetGlobalRole(m_role->GetUID());
		if (nullptr != g_role && g_role->GetQingyuanGlobal()->GetSatisfyMagicalPreciousEquipCount(chapter_cfg->param_b, chapter_cfg->param_c) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_COLLECT_JINGLING:
	{
		if (m_role->GetJingLingMgr()->GetPutOnJinglingCount(chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_ACTIVE_ZHUANZHI_SUIT_FANG:
	{
		int count = m_role->GetZhuanzhiEquip()->ReCalcSameOrderCount(ZHUANZHI_SUIT_TYPE_ZHUSHEN, ZHUANZHI_SUIT_GROUP_TYPE_FANG, chapter_cfg->param_b);
		if (count >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_ACTIVE_ZHUANZHI_SUIT:
	{
		int count = m_role->GetZhuanzhiEquip()->GetSuitTypeCount(chapter_cfg->param_b);
		if (count >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_ACTIVE_SHENZHOU_WEAPON:
	{
		if (m_role->GetShenzhouWeapon()->GetActiveCount() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_XIANNV_GONGMING_LEVEL:
	{
		if (m_role->GetXiannvManager()->GetGongMingTotalLevel() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_XIANNV_SHENGWU_LEVEL:
	{
		if (m_role->GetXiannvManager()->GetShengwuTotalLevel() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_LIEMING_COUNT:
	{
		if (m_role->GetLieMingMgr()->GetSatisfyMagicalPreciousCount(chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHENGE_COUNT:
	{
		if (m_role->GetShengeSystem()->GetSatisfyMagicalPreciousCount(chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHENSHOU_COUNT:
	{
		if (m_role->GetShenshou()->GetActiveCount() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_CHINESE_ZODIAC_LEVEL:
	{
		if (m_role->GetChineseZodiac()->GetTotalLevel() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHENGQI_STRENGTH_LEVEL:
	{
		if (m_role->GetShengqi()->GetTotalStrengthLevel() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_ACTIVE_XIANNV:
	{
		if (m_role->GetXiannvManager()->GetActiveXiannvZizhiNum(chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_TOTAL_SHENGWANG:
	{
		if (m_role->GetShengwang() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_ACTIVE_QINGYUAN_EQUIP2:
	{
		GlobalRole *g_role = GlobalRoleManager::Instance().GetGlobalRole(m_role->GetUID());
		if (nullptr != g_role && g_role->GetQingyuanGlobal()->GetActiveQingyuanEquip2Count() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_ZHUANZHI_STONE_TOTAL_LEVEL:
	{
		if (m_role->GetZhuanzhiEquip()->GetTotalStoneLevel() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_ACTIVE_LUOSHU:
	{
		if (m_role->GetHeShengLuoShu()->GetActiveLuoShuNum(0, chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_EQUIP_BAPTIZE:
	{
		if (m_role->GetEquipBaptize()->GetColorAttrCount(chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_GREATESOLDIER_ACTIVE:
	{
		if (m_role->GetGreateSoldier()->GetActiveCount(chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_UPGRADE_SYS_GRADE:
	{
		if (m_role->GetUpgradeManager()->GetGrade(chapter_cfg->param_a) >= chapter_cfg->param_b)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHENZHOU_WEAPON_INLAY_COUNT:
	{
		if (m_role->GetShenzhouWeapon()->GetHunyinInlayCount(chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHENSHOU_EQUIP_COUNT:
	{
		if (m_role->GetShenshou()->GetEquipCount(chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHENQI_INLAY_COUNT:
	{
		if (m_role->GetShenqi()->GetActiveQualityAllSuitCount(chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_UPLEVEL_QINGYUAN_HALO:
	{
		if (m_role->GetQingyuan()->GetCoupleHaloLevel(chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_ACTIVE_QINGYUAN_HALO:
	{
		if (m_role->GetQingyuan()->GetActiveCoupleHaloCount(chapter_cfg->param_a) >= chapter_cfg->param_b)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_GREATESOLDIER_UPLEVEL:
	{
		if (m_role->GetGreateSoldier()->GetLevelCount(chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SHENGQI_LEVEL_COUNT:
	{
		if (m_role->GetShengqi()->GetStrengthLevelCount(chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_CHINESE_ZODIAC_LEVEL_COUNT:
	{
		if (m_role->GetChineseZodiac()->GetUpLevelCount(chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_CHINESE_ZODIAC_ACTIVE_COUNT:
	{
		if (m_role->GetChineseZodiac()->GetActiveCount(chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_SEAL_INLAY_COUNT:
	{
		if (m_role->GetSeal()->GetInlaySealCount(chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_COLLECT_JINGLING_SKILL:
	{
		if (m_role->GetJingLingMgr()->GetPutOnSkillCount(chapter_cfg->param_b) >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_QILINGBI_GRADE:
	{
		if (m_role->GetQiLinBiManager()->GetGrade() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_MASK_GRADE:
	{
		if (m_role->GetMaskManager()->GetGrade() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_TOUSHI_GRADE:
	{
		if (m_role->GetTouShiManager()->GetGrade() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	case CONDITION_TYPE_YAOSHI_GRADE:
	{
		if (m_role->GetYaoShiManager()->GetGrade() >= chapter_cfg->param_a)
		{
			can_fetch_reward = true;
		}
	}
	break;

	default:
		break;
	}

	if (can_fetch_reward)
	{
		if (this->SetFetchRewardFlag(reward_index, chapter))
		{
			//m_role->GetRoleShop()->AddChestShopShengwang(chapter_cfg->shenwang_reward, "MagicalPreciousReward");
			//m_role->GetRoleShop()->AddChestShopMojing(chapter_cfg->mojing_reward, "MagicalPreciousReward");
			//m_role->GetKnapsack()->GetMoney()->AddGoldBind(chapter_cfg->bind_gold_reward, "MagicalPreciousReward");
			m_role->GetKnapsack()->PutListOrMail(chapter_cfg->reward_item, PUT_REASON_MAGICAL_PRECIOUS);

			this->SendMagicalPreciousChapterInfo(chapter);
		}
	}
	else
	{
		m_role->NoticeNum(errornum::EN_MAGICAL_PRECIOUS_CAN_NOT_FETCHT_REWARD);
	}
}

void MagicalPrecious::ConvertPreciousChapterReward(int chapter, int seq)
{
	if (chapter < 0 || chapter >= MAGICAL_PRECIOUS_MAX_CHAPTER || chapter > m_param.current_chapter || seq < 0) return;

	int max_reward_index = LOGIC_CONFIG->GetMagicalPreciousCfg().GetChapterMaxRewardIndex(chapter);
	if (max_reward_index <= 0 || max_reward_index > MAGICAL_PRECIOUS_MAX_REWARD_INDEX)
	{
		return;
	}

	if (0 != (m_param.chapter_fetch_finish_reward_flag & (1 << chapter)))
	{
		m_role->NoticeNum(errornum::EN_MAGICAL_PRECIOUS_FETCHT_REWARD_LIMIT);
		return;
	}

	const MagicalPreciousFinishChapterRewardCfg *finish_chapter_cfg = LOGIC_CONFIG->GetMagicalPreciousCfg().GetFinishChapterRewardCfg(chapter, seq);
	if (NULL == finish_chapter_cfg) return;

	if (m_param.chapter_score_list[chapter] < finish_chapter_cfg->need_score)
	{
		m_role->NoticeNum(errornum::EN_SCORE_NOT_ENOUGH);
		return;
	}

	if (!m_role->GetKnapsack()->CheckEmptyGridNoLessThan(finish_chapter_cfg->reward_item_count))
	{
		m_role->NoticeNum(errornum::EN_KNAPSACK_NO_SAPCE);
		return;
	}

	bool is_finish_chapter = true;
	int finish_count = 0;
	for (int i = 0; i < max_reward_index; i++)
	{
		char chaper_flag = m_param.chapter_fetch_reward_flag[chapter][i];
		if (MAGICAL_PRECIOUS_REWARD_TYPE_FINISH == static_cast<int>(chaper_flag))
		{
			++finish_count;
		}
	}

	if (finish_count >= max_reward_index - 1)
	{
		is_finish_chapter = true;
	}


	if (!is_finish_chapter)
	{
		m_role->NoticeNum(errornum::EN_MAGICAL_PRECIOUS_CAN_NOT_FETCHT_REWARD);
		return;
	}

	//m_param.chapter_score_list[chapter] -= finish_chapter_cfg->need_score;
	m_param.chapter_fetch_finish_reward_flag |= (1 << chapter);
	m_role->GetKnapsack()->PutList(finish_chapter_cfg->reward_item_count, finish_chapter_cfg->reward_item, PUT_REASON_MAGICAL_PRECIOUS);

	int	length = SNPRINTF(gamestring::GAMESTRING_BUF, sizeof(gamestring::GAMESTRING_BUF), gamestring::g_finish_chapter,
		m_role->GetUID(), m_role->GetName(), (int)m_role->GetCamp());
	if (length > 0)
	{
		World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gamestring::GAMESTRING_BUF, length, SYS_MSG_CENTER_NOTICE);
	}

	//const MagicalPreciousFinishChapterRewardCfg *next_finish_chapter_cfg = LOGIC_CONFIG->GetMagicalPreciousCfg().GetFinishChapterRewardCfg(chapter + 1);
	//if (NULL != next_finish_chapter_cfg)
	//{
	//	m_param.current_chapter++;
	//	m_param.chapter_invalid_time = static_cast<unsigned int>(EngineAdapter::Instance().Time()) + next_finish_chapter_cfg->valid_time;
	//}

	this->SendMagicalPreciousAllInfo();

	if (IsFinish())
	{
		//表示全部完成了
		m_param.current_chapter = FINISH_CHAPTER_ID;
	}
}

void MagicalPrecious::FetchScore(int reward_index, int chapter)
{
	if (chapter < 0 || chapter >= LOGIC_CONFIG->GetMagicalPreciousCfg().GetMaxChapter() || chapter >= MAGICAL_PRECIOUS_MAX_CHAPTER || chapter > m_param.current_chapter)
	{
		return;
	}
	int max_reward_index = LOGIC_CONFIG->GetMagicalPreciousCfg().GetChapterMaxRewardIndex(chapter);
	if (max_reward_index <= 0 || reward_index < 0 || reward_index >= max_reward_index || reward_index >= MAGICAL_PRECIOUS_MAX_REWARD_INDEX)
	{
		return;
	}
	char &chapter_flag = m_param.chapter_fetch_reward_flag[chapter][reward_index];
	if (MAGICAL_PRECIOUS_REWARD_TYPE_FINISH == static_cast<int>(chapter_flag))
	{
		m_role->NoticeNum(errornum::EN_MAGICAL_PRECIOUS_FETCHT_REWARD_LIMIT);
		return;
	}
	
	if (MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH != static_cast<int>(chapter_flag))
	{
		return;
	}
	const MagicalPreciousChapterCfg* cfg = LOGIC_CONFIG->GetMagicalPreciousCfg().GetChapterTaskCfg(chapter, reward_index);
	if (nullptr != cfg)
	{
		chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_FINISH;
		m_param.chapter_score_list[chapter] += cfg->get_score;
		this->SendMagicalPreciousAllInfo();
	}
}

bool MagicalPrecious::IsFetchReward(int reward_index, int chapter)
{
	if (chapter < 0 || chapter >= LOGIC_CONFIG->GetMagicalPreciousCfg().GetMaxChapter() || chapter >= MAGICAL_PRECIOUS_MAX_CHAPTER || chapter > m_param.current_chapter)
	{
		return true;
	}

	int max_reward_index = LOGIC_CONFIG->GetMagicalPreciousCfg().GetChapterMaxRewardIndex(chapter);
	if (max_reward_index <= 0 || reward_index < 0 || reward_index >= max_reward_index || reward_index >= MAGICAL_PRECIOUS_MAX_REWARD_INDEX)
	{
		return true;
	}

	char chapter_flag = m_param.chapter_fetch_reward_flag[chapter][reward_index];

	if (MAGICAL_PRECIOUS_REWARD_TYPE_FINISH == static_cast<int>(chapter_flag))
	{
		m_role->NoticeNum(errornum::EN_MAGICAL_PRECIOUS_FETCHT_REWARD_LIMIT);
		return true;
	}

	return false;
}

bool MagicalPrecious::IsFinish()
{
	int max_chapter = LOGIC_CONFIG->GetMagicalPreciousCfg().GetMaxChapter();
	for (int i = 0; i < max_chapter && i < MAGICAL_PRECIOUS_MAX_CHAPTER; ++i)
	{
		if (0 == (m_param.chapter_fetch_finish_reward_flag & (1 << i)))
		{
			return false;
		}
	}

	return true;
}

bool MagicalPrecious::SetFetchRewardFlag(int reward_index, int chapter)
{
	if (chapter < 0 || chapter >= LOGIC_CONFIG->GetMagicalPreciousCfg().GetMaxChapter() || chapter >= MAGICAL_PRECIOUS_MAX_CHAPTER || chapter > m_param.current_chapter)
	{
		return false;
	}

	int max_reward_index = LOGIC_CONFIG->GetMagicalPreciousCfg().GetChapterMaxRewardIndex(chapter);
	if (max_reward_index <= 0 || reward_index < 0 || reward_index >= max_reward_index || reward_index >= MAGICAL_PRECIOUS_MAX_CHAPTER)
	{
		return false;
	}
	m_param.chapter_fetch_reward_flag[chapter][reward_index] = MAGICAL_PRECIOUS_REWARD_TYPE_FINISH;
	return true;
}

void MagicalPrecious::SendMagicalPreciousAllInfo()
{
	if (m_param.current_chapter < 0 || m_param.current_chapter >= LOGIC_CONFIG->GetMagicalPreciousCfg().GetMaxChapter() || m_param.current_chapter >= MAGICAL_PRECIOUS_MAX_CHAPTER)
	{
		return;
	}

	static Protocol::SCSendMagicalPreciousAllInfo cmd;
	cmd.current_chaper = m_param.current_chapter;
	cmd.chapter_invalid_time = m_param.chapter_invalid_time;
	cmd.chapter_fetch_finish_reward_flag = m_param.chapter_fetch_finish_reward_flag;

	UNSTD_STATIC_CHECK(sizeof(cmd.chapter_score_list) == sizeof(m_param.chapter_score_list));
	memcpy(cmd.chapter_score_list, m_param.chapter_score_list, sizeof(cmd.chapter_score_list));

	memset(cmd.param_list, 0, sizeof(cmd.param_list));

	for (int i = 0; i < MAGICAL_PRECIOUS_CHAPTER_COUNT; i++)
	{
		for (int j = 0; j < MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT; j++)
		{
			cmd.chapter_fetch_reward_flag[i][j] = m_param.chapter_fetch_reward_flag[i][j];
		}
	}

	cmd.param_list_len = 0;

	for (int i = 0; i < LOGIC_CONFIG->GetMagicalPreciousCfg().GetMaxChapter() && i < MAGICAL_PRECIOUS_CHAPTER_COUNT; ++i)
	{
		for (int j = 0; j < LOGIC_CONFIG->GetMagicalPreciousCfg().GetChapterMaxRewardIndex(i) && j < MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT; j++)
		{
			const MagicalPreciousChapterCfg *chapter_cfg = LOGIC_CONFIG->GetMagicalPreciousCfg().GetChapterTaskCfg(i, j);
			if (NULL == chapter_cfg) continue;
			
			Protocol::SCSendMagicalPreciousAllInfo::ChapterParam &cha_param = cmd.param_list[cmd.param_list_len++];
			cha_param.charper = i;
			cha_param.charper_index = j;

			long long &chapter_param = cha_param.param;

			switch (chapter_cfg->condition_type)
			{
			case CONDITION_TYPE_TOTAL_GONGJI:
			{
				chapter_param = m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI);
			}
			break;

			case CONDITION_TYPE_TOTAL_FANGYU:
			{
				chapter_param = m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU);
			}
			break;

			case CONDITION_TYPE_TOTAL_MAXHP:
			{
				chapter_param = m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP);
			}
			break;

			case CONDITION_TYPE_TOTAL_SHANBI:
			{
				chapter_param = m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI);
			}
			break;

			case CONDITION_TYPE_TOTAL_MINGZHONG:
			{
				chapter_param = m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG);
			}
			break;

			case CONDITION_TYPE_TOTAL_BAOJI:
			{
				chapter_param = m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI);
			}
			break;

			case CONDITION_TYPE_TOTAL_JIANREN:
			{
				chapter_param = m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN);
			}
			break;

			case CONDITION_TYPE_MOUNT_GRADE:
			{
				chapter_param = m_role->GetMountManager()->GetMountParam()->grade;
			}
			break;

			case CONDITION_TYPE_WING_GRADE:
			{
				chapter_param = m_role->GetWing()->GetWingParam()->grade;
			}
			break;

			case CONDITION_TYPE_HALO_GRADE:
			{
				chapter_param = m_role->GetHalo()->GetHaloParam()->grade;
			}
			break;

			case CONDITION_TYPE_SHENGONG_GRADE:
			{
				chapter_param = m_role->GetShengong()->GetShengongParam()->grade;
			}
			break;

			case CONDITION_TYPE_SHENYI_GRADE:
			{
				chapter_param = m_role->GetShenyi()->GetShenyiParam()->grade;
			}
			break;

			case CONDITION_TYPE_FIGHT_MOUNT_GRADE:
			{
				chapter_param = m_role->GetFightMountManager()->GetFightMountParam()->grade;
			}
			break;

			case CONDITION_TYPE_SHIZHUANG_GRADE:
			{
				chapter_param = m_role->GetShizhuang()->GetShiZhuangGrade(SHIZHUANG_TYPE_BODY);
			}
			break;

			case CONDITION_TYPE_SHENBING_GRADE:
			{
				chapter_param = m_role->GetShizhuang()->GetShiZhuangGrade(SHIZHUANG_TYPE_WUQI);
			}
			break;

			case CONDITION_TYPE_FABAO_GRADE:
			{
				chapter_param = m_role->GetFabaoManager()->GetGrade();
			}
			break;

			case CONDITION_TYPE_FOOTPRINT_GRADE:
			{
				chapter_param = m_role->GetFootPrint()->GetGrade();
			}
			break;

			case CONDITION_TYPE_JINEJIE_LEVEL:
			{
				chapter_param = m_role->GetJingJieLevel();
			}
			break;

			case CONDITION_TYPE_MOUNT_STAR:
			{
				chapter_param = m_role->GetMountManager()->GetMountParam()->star_level;
			}
			break;

			case CONDITION_TYPE_WING_STAR:
			{
				chapter_param = m_role->GetWing()->GetWingParam()->star_level;
			}
			break;

			case CONDITION_TYPE_HALO_STAR:
			{
				chapter_param = m_role->GetHalo()->GetHaloParam()->star_level;
			}
			break;

			case CONDITION_TYPE_SHENGONG_STAR:
			{
				chapter_param = m_role->GetShengong()->GetShengongParam()->star_level;
			}
			break;

			case CONDITION_TYPE_SHENYI_STAR:
			{
				chapter_param = m_role->GetShenyi()->GetShenyiParam()->star_level;
			}
			break;

			case CONDITION_TYPE_FIGHT_MOUNT_STAR:
			{
				chapter_param = m_role->GetFightMountManager()->GetFightMountParam()->star_level;
			}
			break;

			case CONDITION_TYPE_ALL_CAPACITY:
			{
				chapter_param = m_role->GetCapability()->GetTotalCapability();
			}
			break;

			case CONDITION_TYPE_RUNESYSTER_CAPACITY:
			{
				chapter_param = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_RUNE_SYSTEM);
			}
			break;

			case CONDITION_TYPE_SHENGE_CAPACITY:
			{
				chapter_param = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENGE_SYSTEM);
			}
			break;

			case CONDITION_TYPE_SHENZHOU_WEAPON_CAPACITY:
			{
				chapter_param = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENZHOU_WEAPON);
			}
			break;

			case CONDITION_TYPE_CHINESE_ZODIAC_CAPACITY:
			{
				chapter_param = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_CHINESE_ZODIAC);
			}
			break;

			case CONDITION_TYPE_JINGLING_CAPACITY:
			{
				chapter_param = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING);
			}
			break;

			case CONDITION_TYPE_JINGLING_CHENGZHANG:
			{
				chapter_param = m_role->GetJingLingMgr()->GetHistoryMaxChengzhang();
			}
			break;

			case CONDITION_TYPE_JINGLING_WUXING:
			{
				chapter_param = m_role->GetJingLingMgr()->GetHistoryMaxWuxing();
			}
			break;

			case CONDITION_TYPE_XIANNV_SHENWU_CAP:
			{
				chapter_param = m_role->GetXiannvManager()->GetShengwuBaseCap();
			}
			break;

			case CONDITION_TYPE_EQUIP_STRENGTHEN:
			{
				chapter_param = m_role->GetEquipmentManager()->GetTotalStrengthen();
			}
			break;

			case CONDITION_TYPE_EQUIP_STAR:
			{
				chapter_param = m_role->GetZhuanzhiEquip()->GetEquipTotalStarLevel();
			}
			break;

			case CONDITION_TYPE_EQUIP_STONE:
			{
				chapter_param = m_role->GetStoneManager()->GetTotalStoneLevel();
			}
			break;

			case CONDITION_TYPE_ROLE_LEVEL:
			{
				chapter_param = m_role->GetLevel();
			}
			break;

			case CONDITION_TYPE_XIANNV_ZIZHI_LEVEL:
			{
				chapter_param = m_role->GetXiannvManager()->GetMaxXiannvZizhi();
			}
			break;

			case CONDITION_TYPE_RUNETOWER_LEVEL:
			{
				chapter_param = m_role->GetRuneSystem()->GetRuneTowerPassLayer();
			}
			break;

			case CONDITION_TYPE_PATA_FB_LEVEL:
			{
				chapter_param = m_role->GetRolePataFB()->GetPassLevel();
			}
			break;

			case CONDITION_TYPE_TUITU_FB_NORMAL:
			{
				chapter_param = m_role->GetTuituFb()->IsPassFb(TUITU_FB_TYPE_NORMAL, chapter_cfg->param_a, chapter_cfg->param_b) ? 1 : 0;
			}
			break;

			//case CONDITION_TYPE_TUITU_FB_HARD:
			//{
			//	chapter_param = m_role->GetTuituFb()->IsPassFb(TUITU_FB_TYPE_HARD, chapter_cfg->param_a, chapter_cfg->param_b) ? 1 : 0;
			//}
			//break;

			case CONDITION_TYPE_ARMORDEFEND_FB_LEVEL:
			{
				int pass_level = m_role->GetRoleFbArmorDefend()->GetArmoedefendPassLayer();

				chapter_param = pass_level;
			}
			break;

			case CONDITION_TYPE_TOWERDEFEND_FB_LEVEL:
			{
				int pass_level = m_role->GetFBTowerDefend()->GetPersonalFBPassLevel();

				chapter_param = pass_level;
			}
			break;

			case CONDITION_TYPE_TEAMEQUIP_FB_LEVEL:
			{
				const FBEquipLayerCfg *cur_layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgBySceneID(m_role->GetScene()->GetSceneID());

				chapter_param = nullptr == cur_layer_cfg ? 0 : cur_layer_cfg->layer;
			}
			break;

			case CONDITION_TYPE_TOWERDEFEND_TEAM_FB_WAVE:
			{
				chapter_param = m_role->GetFBTowerDefend()->GetTeamPassdeWave();
			}
			break;

			case CONDITION_TYPE_CHALLENGE_RANK:
			{
				int rank_pos = ChallengeField::Instance().GetChallengeUserRankPos(m_role);

				chapter_param = rank_pos;
			}
			break;

			case CONDITION_TYPE_NEWEQUIP_FB_LEVEL:
			{
				chapter_param = m_role->GetFBEquip()->GetNeqFBPassMaxLevel();
			}
			break;

			case CONDITION_TYPE_GUILD_TASK:
			{
				chapter_param = m_param.finish_guild_task_times;
			}
			break;

			case CONDITION_TYPE_DAILY_TASK:
			{
				chapter_param = m_param.finish_daily_task_times;
			}
			break;

			case CONDITION_TYPE_WABAO:
			{
				chapter_param = m_param.wabao_times;
			}
			break;

			case CONDITION_TYPE_OPEN_GUILD_BOX:
			{
				chapter_param = m_param.open_guild_box_times;
			}
			break;

			case CONDITION_TYPE_HOTSPRING_JOIN:
			{
				chapter_param = m_param.hotspring_join_times;
			}
			break;

			case CONDITION_TYPE_HOTSPRING_ANSWER_RIGHT:
			{
				chapter_param = m_param.hotspring_answer_right_times;
			}
			break;

			case CONDITION_TYPE_MILLIONAIRE_BOX:
			{
				chapter_param = m_param.millionaire_box_times;
			}
			break;

			case CONDITION_TYPE_HUSONG:
			{
				chapter_param = m_param.husong_times;
			}
			break;

			case CONDITION_TYPE_ENTER_EXP_FB:
			{
				chapter_param = m_param.enter_exp_fb_times;
			}
			break;

			case CONDITION_TYPE_PASS_PHASE_FB:
			{
				chapter_param = m_param.pass_phase_fb_times;
			}
			break;

			case CONDITION_TYPE_WANGLING_TASK:
			{
				chapter_param = m_param.wangling_task_times;
			}
			break;

			case CONDITION_TYPE_TIANJIANGCAIBAO_TASK:
			{
				chapter_param = m_param.tianjiangcaibao_task_times;
			}
			break;

			case CONDITION_TYPE_SHUIJING_TASK:
			{
				chapter_param = m_param.shuijing_task_times;
			}
			break;

			case CONDITION_TYPE_SHUIJING_GATHER:
			{
				chapter_param = m_param.shuijing_gather_times;
			}
			break;

			case CONDITION_TYPE_GONGCHENG_ZHAN:
			{
				chapter_param = m_param.join_gongcheng_zhan_times;
			}
			break;

			case CONDITION_TYPE_KILL_MIKU_BOSS:
			{
				chapter_param = m_param.kill_miku_boss;
			}
			break;

			case CONDITION_TYPE_KILL_WORLD_BOSS:
			{
				chapter_param = m_param.kill_world_boss;
			}
			break;

			case CONDITION_TYPE_KILL_ACTIVE_BOSS:
			{
				chapter_param = m_param.kill_active_boss;
			}
			break;

			case CONDITION_TYPE_KILL_DABAO_BOSS:
			{
				chapter_param = m_param.kill_dabao_boss;
			}
			break;

			case  CONDITION_TYPE_KILL_VIP_BOSS:
			{
				chapter_param = m_param.kill_vip_boss;
			}
			break;

			case  CONDITION_TYPE_KILL_VIP_MIKU_BOSS1:
			{
				chapter_param = m_param.kill_boss_count_1;
			}
			break;

			case  CONDITION_TYPE_KILL_VIP_MIKU_BOSS2:
			{
				chapter_param = m_param.kill_boss_count_2;
			}
			break;

			case  CONDITION_TYPE_KILL_VIP_MIKU_BOSS3:
			{
				chapter_param = m_param.kill_boss_count_3;
			}
			break;

			case CONDITION_TYPE_KILL_ROLE_IN_FIGHTING:
			{
				chapter_param = m_param.kill_other_role_in_fighting;
			}
			break;

			case CONDITION_TYPE_MINE_TIMES:
			{
				chapter_param = m_param.mine_times;
			}
			break;

			case CONDITION_TYPE_FISH_NUM:
			{
				chapter_param = m_param.fish_num;
			}
			break;

			case CONDITION_TYPE_SAILING_NUM:
			{
				chapter_param = m_param.sailing_num;
			}
			break;

			case CONDITION_TYPE_ONLINE_TIME:
			{
				chapter_param = m_role->GetOnlineTime() / 3600;
			}
			break;

			case CONDITION_TYPE_KILL_THREE_TYPE_BOSS:
			{
				chapter_param = m_param.kill_miku_boss + m_param.kill_world_boss + m_param.kill_active_boss;
			}
			break;

			case CONDITION_TYPE_KILL_MONSTER_NUM:
			{
				chapter_param = m_param.kill_monster_num;
			}
			break;

			case CONDITION_TYPE_ACTIVE_DEGREE:
			{
				chapter_param = m_param.active_degree;
			}
			break;

			case CONDITION_TYPE_MARRY_SUCC:
			{
				chapter_param = m_role->IsMarry() ? 1 : 0;
			}
			break;

			case CONDITION_TYPE_MARRY_SUCC_TYPE:
			{
				chapter_param = -1;
				if (m_role->GetQingyuan()->IsHadPropose(chapter_cfg->param_a))
				{
					chapter_param = chapter_cfg->param_a;
				}
			}

			case CONDITION_TYPE_PROF_LEVEL:
			{
				chapter_param = m_role->GetProfLevel();
			}
			break;

			case CONDITION_TYPE_GONGCHENG_ZHAN_WINER:
			{
				int winer_guild_id = WorldStatus::Instance().GetGCZWinGuild();

				chapter_param = (m_role->GetGuildID() == winer_guild_id) ? 1 : 0;
			}
			break;

			case ZHUANGBEI_CONDITION_TYPE:
			{
				chapter_param = m_role->GetEquipmentManager()->IsSatisfyMagicalPrecious(chapter_cfg->param_a, chapter_cfg->param_b, chapter_cfg->param_c) ? 1 : 0;
			}
			break;

			case STONE_CONDITION_TYPE:
			{
				chapter_param = m_role->GetStoneManager()->IsSatisfyMagicalPrecious(chapter_cfg->param_a, chapter_cfg->param_b) ? 1 : 0;
			}
			break;

			case DUANZAO_CONDITION_TYPE:
			{
				chapter_param = m_role->GetEquipSuit()->IsSatisfyMagicalPrecious(chapter_cfg->param_a, chapter_cfg->param_b) ? 1 : 0;
			}
			break;

			case CONDITION_TYPE_WEAR_EQUIPMENT:
			{
				chapter_param = m_role->GetEquipmentManager()->GetSatisfyMagicalPrecious2Count(chapter_cfg->param_b, chapter_cfg->param_c);
			}
			break;

			case CONDITION_TYPE_WEAR_ZHUANZHI_EQUIPMENT:
			{
				chapter_param = m_role->GetZhuanzhiEquip()->GetSatisfyPutonCount(chapter_cfg->param_b, chapter_cfg->param_c);
			}
			break;

			case CONDITION_TYPE_WEAR_MARRY_EQUIPMENT:
			{
				GlobalRole *g_role = GlobalRoleManager::Instance().GetGlobalRole(m_role->GetUID());
				if (nullptr != g_role)
				{
					chapter_param = g_role->GetQingyuanGlobal()->GetSatisfyMagicalPreciousEquipCount(chapter_cfg->param_b, chapter_cfg->param_c);
				}
				else
				{
					chapter_param = 0;
				}
			}
			break;

			case CONDITION_TYPE_COLLECT_JINGLING:
			{
				chapter_param = m_role->GetJingLingMgr()->GetPutOnJinglingCount(chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_ACTIVE_ZHUANZHI_SUIT_FANG:
			{
				int count = m_role->GetZhuanzhiEquip()->ReCalcSameOrderCount(ZHUANZHI_SUIT_TYPE_ZHUSHEN, ZHUANZHI_SUIT_GROUP_TYPE_FANG, chapter_cfg->param_b);

				chapter_param = count;
			}
			break;

			case CONDITION_TYPE_ACTIVE_ZHUANZHI_SUIT:
			{
				int count = m_role->GetZhuanzhiEquip()->GetSuitTypeCount(chapter_cfg->param_b);
				chapter_param = count;
			}
			break;

			case CONDITION_TYPE_ACTIVE_SHENZHOU_WEAPON:
			{
				chapter_param = m_role->GetShenzhouWeapon()->GetActiveCount();
			}
			break;

			case CONDITION_TYPE_XIANNV_GONGMING_LEVEL:
			{
				chapter_param = m_role->GetXiannvManager()->GetGongMingTotalLevel();
			}
			break;

			case CONDITION_TYPE_XIANNV_SHENGWU_LEVEL:
			{
				chapter_param = m_role->GetXiannvManager()->GetShengwuTotalLevel();
			}
			break;

			case CONDITION_TYPE_LIEMING_COUNT:
			{
				chapter_param = m_role->GetLieMingMgr()->GetSatisfyMagicalPreciousCount(chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_SHENGE_COUNT:
			{
				chapter_param = m_role->GetShengeSystem()->GetSatisfyMagicalPreciousCount(chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_SHENSHOU_COUNT:
			{
				chapter_param = m_role->GetShenshou()->GetActiveCount();
			}
			break;

			case CONDITION_TYPE_CHINESE_ZODIAC_LEVEL:
			{
				chapter_param = m_role->GetChineseZodiac()->GetTotalLevel();
			}
			break;

			case CONDITION_TYPE_SHENGQI_STRENGTH_LEVEL:
			{
				chapter_param = m_role->GetShengqi()->GetTotalStrengthLevel();
			}
			break;

			case CONDITION_TYPE_ACTIVE_XIANNV:
			{
				chapter_param = m_role->GetXiannvManager()->GetActiveXiannvZizhiNum(chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_TOTAL_SHENGWANG:
			{
				chapter_param = m_role->GetShengwang();
			}
			break;

			case CONDITION_TYPE_ACTIVE_QINGYUAN_EQUIP2:
			{
				GlobalRole *g_role = GlobalRoleManager::Instance().GetGlobalRole(m_role->GetUID());
				if (nullptr != g_role)
				{
					chapter_param = g_role->GetQingyuanGlobal()->GetActiveQingyuanEquip2Count();
				}
			}
			break;

			case CONDITION_TYPE_ZHUANZHI_STONE_TOTAL_LEVEL:
			{
				chapter_param = m_role->GetZhuanzhiEquip()->GetTotalStoneLevel();
			}
			break;

			case CONDITION_TYPE_ACTIVE_LUOSHU:
			{
				chapter_param = m_role->GetHeShengLuoShu()->GetActiveLuoShuNum(0, chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_EQUIP_BAPTIZE:
			{
				chapter_param = m_role->GetEquipBaptize()->GetColorAttrCount(chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_GREATESOLDIER_ACTIVE:
			{
				chapter_param = m_role->GetGreateSoldier()->GetActiveCount(chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_UPGRADE_SYS_GRADE:
			{
				chapter_param = m_role->GetUpgradeManager()->GetGrade(chapter_cfg->param_a);
			}
			break;

			case CONDITION_TYPE_SHENZHOU_WEAPON_INLAY_COUNT:
			{
				chapter_param = m_role->GetShenzhouWeapon()->GetHunyinInlayCount(chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_SHENSHOU_EQUIP_COUNT:
			{
				chapter_param = m_role->GetShenshou()->GetEquipCount(chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_SHENQI_INLAY_COUNT:
			{
				chapter_param = m_role->GetShenqi()->GetActiveQualityAllSuitCount(chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_UPLEVEL_QINGYUAN_HALO:
			{
				chapter_param = m_role->GetQingyuan()->GetCoupleHaloLevel(chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_ACTIVE_QINGYUAN_HALO:
			{
				chapter_param = m_role->GetQingyuan()->GetActiveCoupleHaloCount(chapter_cfg->param_a);
			}
			break;

			case CONDITION_TYPE_GREATESOLDIER_UPLEVEL:
			{
				chapter_param = m_role->GetGreateSoldier()->GetLevelCount(chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_SHENGQI_LEVEL_COUNT:
			{
				chapter_param = m_role->GetShengqi()->GetStrengthLevelCount(chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_CHINESE_ZODIAC_LEVEL_COUNT:
			{
				chapter_param = m_role->GetChineseZodiac()->GetUpLevelCount(chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_CHINESE_ZODIAC_ACTIVE_COUNT:
			{
				chapter_param = m_role->GetChineseZodiac()->GetActiveCount(chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_SEAL_INLAY_COUNT:
			{
				chapter_param = m_role->GetSeal()->GetInlaySealCount(chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_COLLECT_JINGLING_SKILL:
			{
				chapter_param = m_role->GetJingLingMgr()->GetPutOnSkillCount(chapter_cfg->param_b);
			}
			break;

			case CONDITION_TYPE_QILINGBI_GRADE:
			{
				chapter_param = m_role->GetQiLinBiManager()->GetGrade();
			}
			break;

			case CONDITION_TYPE_MASK_GRADE:
			{
				chapter_param = m_role->GetMaskManager()->GetGrade();
			}
			break;

			case CONDITION_TYPE_TOUSHI_GRADE:
			{
				chapter_param = m_role->GetTouShiManager()->GetGrade();
			}
			break;

			case CONDITION_TYPE_YAOSHI_GRADE:
			{
				chapter_param = m_role->GetYaoShiManager()->GetGrade();
			}
			break;

			default:
				break;
			}
		}
	}

	int send_len = sizeof(Protocol::SCSendMagicalPreciousAllInfo) -
		sizeof(Protocol::SCSendMagicalPreciousAllInfo::ChapterParam) * 
		(MAGICAL_PRECIOUS_CHAPTER_COUNT * MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT - cmd.param_list_len);

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, send_len);
}

void MagicalPrecious::SendMagicalPreciousChapterInfo(int chapter)
{
	Protocol::SCSendMagicalPreciousCurChapterInfo cmd;
	cmd.fetch_chapter = chapter;
	for (int j = 0; j < MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT; j++)
	{
		cmd.chapter_fetch_reward_flag[j] = m_param.chapter_fetch_reward_flag[chapter][j];
	}

	EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, sizeof(cmd));
}

void MagicalPrecious::SendMagicalPreciousConditionChange(int condition_type)
{
	// 完成了就不发信息了
	if (m_param.current_chapter == FINISH_CHAPTER_ID)
	{
		return;
	}
	//if (false == m_is_send_all)
	//{
	//	this->SendMagicalPreciousAllInfo();
	//	m_is_send_all = true;
	//}
	
	m_is_send_all = true;

	//static Protocol::SCMagicalPreciousConditionParamChange cmd;
	//memset(cmd.chapter_score_list, 0, sizeof(cmd.chapter_score_list));
	//memset(cmd.chapter_fetch_reward_flag, 0, sizeof(cmd.chapter_fetch_reward_flag));
	//memset(cmd.param_list, 0, sizeof(cmd.param_list));

	const std::vector<const MagicalPreciousChapterCfg *> &cfg_vec = LOGIC_CONFIG->GetMagicalPreciousCfg().GetChapterCfgByCondition(condition_type);
	
	//cmd.param_list_len = 0;
	for (auto chapter_cfg : cfg_vec)
	{
		if (nullptr == chapter_cfg)
		{
			continue;
		}

		long long chapter_param = 0;
		char &chapter_flag = m_param.chapter_fetch_reward_flag[chapter_cfg->chapter_id][chapter_cfg->reward_index];
//		char old_chapter_flag = chapter_flag;
		if (MAGICAL_PRECIOUS_REWARD_TYPE_UNDONE != static_cast<int>(chapter_flag))
		{
			continue;			//已经加过该章节，该reward_index的积分
		}
		switch (condition_type)
		{
		case CONDITION_TYPE_TOTAL_GONGJI:
		{
			chapter_param = m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_TOTAL_FANGYU:
		{
			chapter_param = m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_TOTAL_MAXHP:
		{
			chapter_param = m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_TOTAL_SHANBI:
		{
			chapter_param = m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_TOTAL_MINGZHONG:
		{
			chapter_param = m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_TOTAL_BAOJI:
		{
			chapter_param = m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_TOTAL_JIANREN:
		{
			chapter_param = m_role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_MOUNT_GRADE:
		{
			chapter_param = m_role->GetMountManager()->GetMountParam()->grade;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_WING_GRADE:
		{
			chapter_param = m_role->GetWing()->GetWingParam()->grade;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_HALO_GRADE:
		{
			chapter_param = m_role->GetHalo()->GetHaloParam()->grade;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHENGONG_GRADE:
		{
			chapter_param = m_role->GetShengong()->GetShengongParam()->grade;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHENYI_GRADE:
		{
			chapter_param = m_role->GetShenyi()->GetShenyiParam()->grade;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_FIGHT_MOUNT_GRADE:
		{
			chapter_param = m_role->GetFightMountManager()->GetFightMountParam()->grade;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHIZHUANG_GRADE:
		{
			chapter_param = m_role->GetShizhuang()->GetShiZhuangGrade(SHIZHUANG_TYPE_BODY);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHENBING_GRADE:
		{
			chapter_param = m_role->GetShizhuang()->GetShiZhuangGrade(SHIZHUANG_TYPE_WUQI);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_FABAO_GRADE:
		{
			chapter_param = m_role->GetFabaoManager()->GetGrade();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_FOOTPRINT_GRADE:
		{
			chapter_param = m_role->GetFootPrint()->GetGrade();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_JINEJIE_LEVEL:
		{
			chapter_param = m_role->GetJingJieLevel();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_MOUNT_STAR:
		{
			chapter_param = m_role->GetMountManager()->GetMountParam()->star_level;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_WING_STAR:
		{
			chapter_param = m_role->GetWing()->GetWingParam()->star_level;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_HALO_STAR:
		{
			chapter_param = m_role->GetHalo()->GetHaloParam()->star_level;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHENGONG_STAR:
		{
			chapter_param = m_role->GetShengong()->GetShengongParam()->star_level;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHENYI_STAR:
		{
			chapter_param = m_role->GetShenyi()->GetShenyiParam()->star_level;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_FIGHT_MOUNT_STAR:
		{
			chapter_param = m_role->GetFightMountManager()->GetFightMountParam()->star_level;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_ALL_CAPACITY:
		{
			chapter_param = m_role->GetCapability()->GetTotalCapability();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_RUNESYSTER_CAPACITY:
		{
			chapter_param = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_RUNE_SYSTEM);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHENGE_CAPACITY:
		{
			chapter_param = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENGE_SYSTEM);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHENZHOU_WEAPON_CAPACITY:
		{
			chapter_param = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_SHENZHOU_WEAPON);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_CHINESE_ZODIAC_CAPACITY:
		{
			chapter_param = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_CHINESE_ZODIAC);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_JINGLING_CAPACITY:
		{
			chapter_param = m_role->GetCapability()->GetRoleCapability(CAPABILITY_TYPE_JINGLING);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_JINGLING_CHENGZHANG:
		{
			chapter_param = m_role->GetJingLingMgr()->GetHistoryMaxChengzhang();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_JINGLING_WUXING:
		{
			chapter_param = m_role->GetJingLingMgr()->GetHistoryMaxWuxing();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_XIANNV_SHENWU_CAP:
		{
			chapter_param = m_role->GetXiannvManager()->GetShengwuBaseCap();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_EQUIP_STRENGTHEN:
		{
			chapter_param = m_role->GetEquipmentManager()->GetTotalStrengthen();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_EQUIP_STAR:
		{
			chapter_param = m_role->GetZhuanzhiEquip()->GetEquipTotalStarLevel();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_EQUIP_STONE:
		{
			chapter_param = m_role->GetStoneManager()->GetTotalStoneLevel();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_ROLE_LEVEL:
		{
			chapter_param = m_role->GetLevel();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_XIANNV_ZIZHI_LEVEL:
		{
			chapter_param = m_role->GetXiannvManager()->GetMaxXiannvZizhi();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_RUNETOWER_LEVEL:
		{
			chapter_param = m_role->GetRuneSystem()->GetRuneTowerPassLayer();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_PATA_FB_LEVEL:
		{
			chapter_param = m_role->GetRolePataFB()->GetPassLevel();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_TUITU_FB_NORMAL:
		{
			chapter_param = m_role->GetTuituFb()->IsPassFb(TUITU_FB_TYPE_NORMAL, chapter_cfg->param_a, chapter_cfg->param_b) ? 1 : 0;
			if (1 == chapter_param)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		//case CONDITION_TYPE_TUITU_FB_HARD:
		//{
		//	chapter_param = m_role->GetTuituFb()->IsPassFb(TUITU_FB_TYPE_HARD, chapter_cfg->param_a, chapter_cfg->param_b) ? 1 : 0;
		//}
		//break;

		case CONDITION_TYPE_ARMORDEFEND_FB_LEVEL:
		{
			chapter_param = m_role->GetRoleFbArmorDefend()->GetArmoedefendPassLayer();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_TOWERDEFEND_FB_LEVEL:
		{
			int pass_level = m_role->GetFBTowerDefend()->GetPersonalFBPassLevel();

			chapter_param = pass_level;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_TEAMEQUIP_FB_LEVEL:
		{
			const FBEquipLayerCfg *cur_layer_cfg = LOGIC_CONFIG->GetEquipFBConfig().GetTeamLayerCfgBySceneID(m_role->GetScene()->GetSceneID());
			if (nullptr != cur_layer_cfg)
			{
				chapter_param = cur_layer_cfg->layer;
				if (chapter_param >= chapter_cfg->param_a)
				{
					chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
				}
			}
		}
		break;

		case CONDITION_TYPE_TOWERDEFEND_TEAM_FB_WAVE:
		{
			chapter_param = m_role->GetFBTowerDefend()->GetTeamPassdeWave();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_CHALLENGE_RANK:
		{
			int rank_pos = ChallengeField::Instance().GetChallengeUserRankPos(m_role);
			
			chapter_param = rank_pos;
			if (chapter_param <= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_NEWEQUIP_FB_LEVEL:
		{
			chapter_param = m_role->GetFBEquip()->GetNeqFBPassMaxLevel();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_GUILD_TASK:
		{
			chapter_param = m_param.finish_guild_task_times;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_DAILY_TASK:
		{
			chapter_param = m_param.finish_daily_task_times;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_WABAO:
		{
			chapter_param = m_param.wabao_times;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_OPEN_GUILD_BOX:
		{
			chapter_param = m_param.open_guild_box_times;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_HOTSPRING_JOIN:
		{
			chapter_param = m_param.hotspring_join_times;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_HOTSPRING_ANSWER_RIGHT:
		{
			chapter_param = m_param.hotspring_answer_right_times;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_MILLIONAIRE_BOX:
		{
			chapter_param = m_param.millionaire_box_times;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_HUSONG:
		{
			chapter_param = m_param.husong_times;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_ENTER_EXP_FB:
		{
			chapter_param = m_param.enter_exp_fb_times;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_PASS_PHASE_FB:
		{
			chapter_param = m_param.pass_phase_fb_times;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_WANGLING_TASK:
		{
			chapter_param = m_param.wangling_task_times;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_TIANJIANGCAIBAO_TASK:
		{
			chapter_param = m_param.tianjiangcaibao_task_times;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHUIJING_TASK:
		{
			chapter_param = m_param.shuijing_task_times;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHUIJING_GATHER:
		{
			chapter_param = m_param.shuijing_gather_times;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_GONGCHENG_ZHAN:
		{
			chapter_param = m_param.join_gongcheng_zhan_times;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_KILL_MIKU_BOSS:
		{
			chapter_param = m_param.kill_miku_boss;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_KILL_WORLD_BOSS:
		{
			chapter_param = m_param.kill_world_boss;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_KILL_ACTIVE_BOSS:
		{
			chapter_param = m_param.kill_active_boss;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_KILL_DABAO_BOSS:
		{
			chapter_param = m_param.kill_dabao_boss;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case  CONDITION_TYPE_KILL_VIP_BOSS:
		{
			chapter_param = m_param.kill_vip_boss;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case  CONDITION_TYPE_KILL_VIP_MIKU_BOSS1:
		{
			chapter_param = m_param.kill_boss_count_1;
			if (chapter_param >= chapter_cfg->param_b)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case  CONDITION_TYPE_KILL_VIP_MIKU_BOSS2:
		{
			chapter_param = m_param.kill_boss_count_2;
			if (chapter_param >= chapter_cfg->param_b)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case  CONDITION_TYPE_KILL_VIP_MIKU_BOSS3:
		{
			chapter_param = m_param.kill_boss_count_3;
			if (chapter_param >= chapter_cfg->param_b)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_KILL_ROLE_IN_FIGHTING:
		{
			chapter_param = m_param.kill_other_role_in_fighting;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_MINE_TIMES:
		{
			chapter_param = m_param.mine_times;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_FISH_NUM:
		{
			chapter_param = m_param.fish_num;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SAILING_NUM:
		{
			chapter_param = m_param.sailing_num;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_ONLINE_TIME:
		{
			chapter_param = m_role->GetOnlineTime() / 3600;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_KILL_THREE_TYPE_BOSS:
		{
			chapter_param = m_param.kill_miku_boss + m_param.kill_world_boss + m_param.kill_active_boss;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_KILL_MONSTER_NUM:
		{
			chapter_param = m_param.kill_monster_num;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_ACTIVE_DEGREE:
		{
			chapter_param = m_param.active_degree;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_MARRY_SUCC:
		{
			chapter_param = m_role->IsMarry() ? 1 : 0;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_MARRY_SUCC_TYPE:
		{
			chapter_param = -1;
			if (m_role->GetQingyuan()->IsHadPropose(chapter_cfg->param_a))
			{
				chapter_param = chapter_cfg->param_a;
			}

			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_PROF_LEVEL:
		{
			chapter_param = m_role->GetProfLevel();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_GONGCHENG_ZHAN_WINER:
		{
			int winer_guild_id = WorldStatus::Instance().GetGCZWinGuild();

			chapter_param = (m_role->GetGuildID() == winer_guild_id) ? 1 : 0;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case ZHUANGBEI_CONDITION_TYPE:
		{
			chapter_param = m_role->GetEquipmentManager()->IsSatisfyMagicalPrecious(chapter_cfg->param_a, chapter_cfg->param_b, chapter_cfg->param_c) ? 1 : 0;
			if (1 == chapter_param)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case STONE_CONDITION_TYPE:
		{
			chapter_param = m_role->GetStoneManager()->IsSatisfyMagicalPrecious(chapter_cfg->param_a, chapter_cfg->param_b) ? 1 : 0;
			if (1 == chapter_param)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case DUANZAO_CONDITION_TYPE:
		{
			chapter_param = m_role->GetEquipSuit()->IsSatisfyMagicalPrecious(chapter_cfg->param_a, chapter_cfg->param_b) ? 1 : 0;
			if (1 == chapter_param)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_WEAR_EQUIPMENT:
		{
			chapter_param = m_role->GetEquipmentManager()->GetSatisfyMagicalPrecious2Count(chapter_cfg->param_b, chapter_cfg->param_c);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_WEAR_ZHUANZHI_EQUIPMENT:
		{
			chapter_param = m_role->GetZhuanzhiEquip()->GetSatisfyPutonCount(chapter_cfg->param_b, chapter_cfg->param_c);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_WEAR_MARRY_EQUIPMENT:
		{
			GlobalRole *g_role = GlobalRoleManager::Instance().GetGlobalRole(m_role->GetUID());
			if (nullptr != g_role)
			{
				chapter_param = g_role->GetQingyuanGlobal()->GetSatisfyMagicalPreciousEquipCount(chapter_cfg->param_b, chapter_cfg->param_c);
			}
			else
			{
				chapter_param = 0;
			}

			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_COLLECT_JINGLING:
		{
			chapter_param = m_role->GetJingLingMgr()->GetPutOnJinglingCount(chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_ACTIVE_ZHUANZHI_SUIT_FANG:
		{
			int count = m_role->GetZhuanzhiEquip()->ReCalcSameOrderCount(ZHUANZHI_SUIT_TYPE_ZHUSHEN, ZHUANZHI_SUIT_GROUP_TYPE_FANG, chapter_cfg->param_b);

			chapter_param = count;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_ACTIVE_ZHUANZHI_SUIT:
		{
			int count = m_role->GetZhuanzhiEquip()->GetSuitTypeCount(chapter_cfg->param_b);
			chapter_param = count;
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_ACTIVE_SHENZHOU_WEAPON:
		{
			chapter_param = m_role->GetShenzhouWeapon()->GetActiveCount();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_XIANNV_GONGMING_LEVEL:
		{
			chapter_param = m_role->GetXiannvManager()->GetGongMingTotalLevel();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_XIANNV_SHENGWU_LEVEL:
		{
			chapter_param = m_role->GetXiannvManager()->GetShengwuTotalLevel();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_LIEMING_COUNT:
		{
			chapter_param = m_role->GetLieMingMgr()->GetSatisfyMagicalPreciousCount(chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHENGE_COUNT:
		{
			chapter_param = m_role->GetShengeSystem()->GetSatisfyMagicalPreciousCount(chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHENSHOU_COUNT:
		{
			chapter_param = m_role->GetShenshou()->GetActiveCount();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_CHINESE_ZODIAC_LEVEL:
		{
			chapter_param = m_role->GetChineseZodiac()->GetTotalLevel();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_ACTIVE_XIANNV:
		{
			chapter_param = m_role->GetXiannvManager()->GetActiveXiannvZizhiNum(chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_TOTAL_SHENGWANG:
		{
			chapter_param = m_role->GetShengwang();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHENGQI_STRENGTH_LEVEL:
		{
			chapter_param = m_role->GetShengqi()->GetTotalStrengthLevel();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_ACTIVE_QINGYUAN_EQUIP2:
		{
			GlobalRole *g_role = GlobalRoleManager::Instance().GetGlobalRole(m_role->GetUID());
			if (nullptr != g_role)
			{
				chapter_param = g_role->GetQingyuanGlobal()->GetActiveQingyuanEquip2Count();
				if (chapter_param >= chapter_cfg->param_a)
				{
					chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
				}
			}
		}
		break;

		case CONDITION_TYPE_ZHUANZHI_STONE_TOTAL_LEVEL:
		{
			chapter_param = m_role->GetZhuanzhiEquip()->GetTotalStoneLevel();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_ACTIVE_LUOSHU:
		{
			chapter_param = m_role->GetHeShengLuoShu()->GetActiveLuoShuNum(0, chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_EQUIP_BAPTIZE:
		{
			chapter_param = m_role->GetEquipBaptize()->GetColorAttrCount(chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_GREATESOLDIER_ACTIVE:
		{
			chapter_param = m_role->GetGreateSoldier()->GetActiveCount(chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_UPGRADE_SYS_GRADE:
		{
			chapter_param = m_role->GetUpgradeManager()->GetGrade(chapter_cfg->param_a);
			if (chapter_param >= chapter_cfg->param_b)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHENZHOU_WEAPON_INLAY_COUNT:
		{
			chapter_param = m_role->GetShenzhouWeapon()->GetHunyinInlayCount(chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHENSHOU_EQUIP_COUNT:
		{
			chapter_param = m_role->GetShenshou()->GetEquipCount(chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHENQI_INLAY_COUNT:
		{
			chapter_param = m_role->GetShenqi()->GetActiveQualityAllSuitCount(chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_UPLEVEL_QINGYUAN_HALO:
		{
			chapter_param = m_role->GetQingyuan()->GetCoupleHaloLevel(chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_ACTIVE_QINGYUAN_HALO:
		{
			chapter_param = m_role->GetQingyuan()->GetActiveCoupleHaloCount(chapter_cfg->param_a);
			if (chapter_param >= chapter_cfg->param_b)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_GREATESOLDIER_UPLEVEL:
		{
			chapter_param = m_role->GetGreateSoldier()->GetLevelCount(chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SHENGQI_LEVEL_COUNT:
		{
			chapter_param = m_role->GetShengqi()->GetStrengthLevelCount(chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_CHINESE_ZODIAC_LEVEL_COUNT:
		{
			chapter_param = m_role->GetChineseZodiac()->GetUpLevelCount(chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_CHINESE_ZODIAC_ACTIVE_COUNT:
		{
			chapter_param = m_role->GetChineseZodiac()->GetActiveCount(chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_SEAL_INLAY_COUNT:
		{
			chapter_param = m_role->GetSeal()->GetInlaySealCount(chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_COLLECT_JINGLING_SKILL:
		{
			chapter_param = m_role->GetJingLingMgr()->GetPutOnSkillCount(chapter_cfg->param_b);
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_QILINGBI_GRADE:
		{
			chapter_param = m_role->GetQiLinBiManager()->GetGrade();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_MASK_GRADE:
		{
			chapter_param = m_role->GetMaskManager()->GetGrade();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_TOUSHI_GRADE:
		{
			chapter_param = m_role->GetTouShiManager()->GetGrade();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		case CONDITION_TYPE_YAOSHI_GRADE:
		{
			chapter_param = m_role->GetYaoShiManager()->GetGrade();
			if (chapter_param >= chapter_cfg->param_a)
			{
				chapter_flag = MAGICAL_PRECIOUS_REWARD_TYPE_CAN_FETCH;
			}
		}
		break;

		default:
			break;
		}

		//if (cmd.param_list_len >= 0 && static_cast<size_t>(cmd.param_list_len) < static_array_size(cmd.param_list))
		//{
		//	cmd.param_list[cmd.param_list_len] = Protocol::SCSendMagicalPreciousAllInfo::ChapterParam(chapter_cfg->chapter_id, chapter_cfg->reward_index, chapter_param);
		//	cmd.param_list_len++;
		//}	
	}
	
	//UNSTD_STATIC_CHECK(sizeof(cmd.chapter_score_list) == sizeof(m_param.chapter_score_list));
	//memcpy(cmd.chapter_score_list, m_param.chapter_score_list, sizeof(cmd.chapter_score_list));

	//UNSTD_STATIC_CHECK(sizeof(cmd.chapter_fetch_reward_flag) == sizeof(m_param.chapter_fetch_reward_flag));
	//memcpy(cmd.chapter_fetch_reward_flag, m_param.chapter_fetch_reward_flag, sizeof(cmd.chapter_fetch_reward_flag));

	//int send_len = sizeof(cmd) - sizeof(Protocol::SCSendMagicalPreciousAllInfo::ChapterParam) * (MAGICAL_PRECIOUS_CHAPTER_COUNT - cmd.param_list_len);
	//EngineAdapter::Instance().NetSend(m_role->GetNetId(), (const char *)&cmd, send_len);
}

void MagicalPrecious::AddConditionTimes(int condition_type, int add_times)
{
	// 已完成则不计算进度
	if (m_param.current_chapter == FINISH_CHAPTER_ID)
	{
		return;
	}

	bool is_change = false;
	switch (condition_type)
	{
	case CONDITION_TYPE_GUILD_TASK:
	{
		m_param.finish_guild_task_times += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_DAILY_TASK:
	{
		m_param.finish_daily_task_times += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_WABAO:
	{
		m_param.wabao_times += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_OPEN_GUILD_BOX:
	{
		m_param.open_guild_box_times += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_HOTSPRING_JOIN:
	{
		m_param.hotspring_join_times += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_HOTSPRING_ANSWER_RIGHT:
	{
		m_param.hotspring_answer_right_times += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_MILLIONAIRE_BOX:
	{
		m_param.millionaire_box_times += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_HUSONG:
	{
		m_param.husong_times += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_ENTER_EXP_FB:
	{
		m_param.enter_exp_fb_times += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_PASS_PHASE_FB:
	{
		m_param.pass_phase_fb_times += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_WANGLING_TASK:
	{
		m_param.wangling_task_times += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_TIANJIANGCAIBAO_TASK:
	{
		m_param.tianjiangcaibao_task_times += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_SHUIJING_TASK:
	{
		m_param.shuijing_task_times += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_SHUIJING_GATHER:
	{
		m_param.shuijing_gather_times += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_GONGCHENG_ZHAN:
	{
		m_param.join_gongcheng_zhan_times += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_KILL_MIKU_BOSS:
	{
		m_param.kill_miku_boss += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_KILL_WORLD_BOSS:
	{
		m_param.kill_world_boss += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_KILL_ACTIVE_BOSS:
	{
		m_param.kill_active_boss += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_KILL_DABAO_BOSS:
	{
		m_param.kill_dabao_boss += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_KILL_VIP_BOSS:
	{
		m_param.kill_vip_boss += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_KILL_ROLE_IN_FIGHTING:
	{
		m_param.kill_other_role_in_fighting += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_MINE_TIMES:
	{
		m_param.mine_times += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_FISH_NUM:
	{
		m_param.fish_num += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_SAILING_NUM:
	{
		m_param.sailing_num += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_KILL_MONSTER_NUM:
	{
		m_param.kill_monster_num += add_times;
		is_change = true;
	}
	break;

	case CONDITION_TYPE_ACTIVE_DEGREE:
	{
		m_param.active_degree += add_times;
		is_change = true;
	}
	break;

	default:
		break;
	}

	if (is_change)
	{
		this->SendMagicalPreciousConditionChange(condition_type);
	}
}

void MagicalPrecious::CheckAddKillBossTimes(int condition_type, int add_times, int level)
{
	// 已完成则不计算进度
	if (m_param.current_chapter == FINISH_CHAPTER_ID)
	{
		return;
	}

	int cfg_chapter_max = LOGIC_CONFIG->GetMagicalPreciousCfg().GetMaxChapter();
	for (int chapter_index = 0; chapter_index < cfg_chapter_max && chapter_index < MAGICAL_PRECIOUS_CHAPTER_COUNT; ++chapter_index)
	{
		int cfg_index_max = LOGIC_CONFIG->GetMagicalPreciousCfg().GetChapterMaxRewardIndex(chapter_index);
		for (int reward_index = 0; reward_index < cfg_index_max && reward_index < MAGICAL_PRECIOUS_CHAPTER_REWARD_INDEX_COUNT; reward_index++)
		{
			const MagicalPreciousChapterCfg *chapter_cfg = LOGIC_CONFIG->GetMagicalPreciousCfg().GetChapterTaskCfg(chapter_index, reward_index);
			if (NULL == chapter_cfg)
			{
				continue;
			}

			if (chapter_cfg->condition_type != condition_type)
			{
				continue;
			}

			switch (condition_type)
			{
			case  CONDITION_TYPE_KILL_VIP_MIKU_BOSS1:
			{
				if (level >= chapter_cfg->param_a)
				{
					m_param.kill_boss_count_1 += add_times;
				}
			}
			break;

			case  CONDITION_TYPE_KILL_VIP_MIKU_BOSS2:
			{
				if (level >= chapter_cfg->param_a)
				{
					m_param.kill_boss_count_2 += add_times;
				}
			}
			break;

			case  CONDITION_TYPE_KILL_VIP_MIKU_BOSS3:
			{
				if (level >= chapter_cfg->param_a)
				{
					m_param.kill_boss_count_3 += add_times;
				}
			}
			break;
			}
		}
	}

	this->SendMagicalPreciousConditionChange(condition_type);
}

void MagicalPrecious::GMIncScore(int chapter, int score)
{
	if (chapter < 0 || chapter >= LOGIC_CONFIG->GetMagicalPreciousCfg().GetMaxChapter() || chapter >= MAGICAL_PRECIOUS_MAX_CHAPTER)
	{
		return;
	}
	if (score <= 0)
	{
		return;
	}
	int new_score = m_param.chapter_score_list[chapter] + score;
	m_param.chapter_score_list[chapter] = new_score < m_param.chapter_score_list[chapter] ? 65535 : new_score;	//处理溢出
	this->SendMagicalPreciousAllInfo();
}

void MagicalPrecious::GMDecScore(int chapter, int score)
{
	if (chapter < 0 || chapter >= LOGIC_CONFIG->GetMagicalPreciousCfg().GetMaxChapter() || chapter >= MAGICAL_PRECIOUS_MAX_CHAPTER)
	{
		return;
	}
	if (score <= 0)
	{
		return;
	}
	int new_score = m_param.chapter_score_list[chapter] - score;
	m_param.chapter_score_list[chapter] = new_score > m_param.chapter_score_list[chapter] ? 0 : new_score;	//处理溢出
	this->SendMagicalPreciousAllInfo();
}

void MagicalPrecious::GMIncChapter(int chapter)
{
	if (chapter <= 0)
	{
		return;
	}
	int new_chapter = m_param.current_chapter + chapter;
	if (new_chapter < 0 || new_chapter >= LOGIC_CONFIG->GetMagicalPreciousCfg().GetMaxChapter() || new_chapter >= MAGICAL_PRECIOUS_MAX_CHAPTER)
	{
		return;
	}
	m_param.current_chapter = new_chapter;
	m_param.chapter_invalid_time += SECOND_PER_DAY * chapter;
	this->SendMagicalPreciousAllInfo();
}

void MagicalPrecious::GMFinish(int chapter, int reward_index, char flag)
{
	if (chapter < 0 || chapter >= LOGIC_CONFIG->GetMagicalPreciousCfg().GetMaxChapter() || chapter >= MAGICAL_PRECIOUS_MAX_CHAPTER)
	{
		return;
	}
	int max_reward_index = LOGIC_CONFIG->GetMagicalPreciousCfg().GetChapterMaxRewardIndex(chapter);
	if (max_reward_index <= 0 || reward_index < 0 || reward_index >= max_reward_index || reward_index >= MAGICAL_PRECIOUS_MAX_REWARD_INDEX)
	{
		return;
	}
	if (flag <= MAGICAL_PRECIOUS_REWARD_TYPE_INVALID || flag >= MAGICAL_PRECIOUS_REWARD_TYPE_MAX)
	{
		return;
	}
	m_param.chapter_fetch_reward_flag[chapter][reward_index] = flag;
	this->SendMagicalPreciousAllInfo();
}

