#include <string.h>

#include "gm.hpp"

#include "engineadapter.h"
#include "world.h"
#include "gstr.h"

#include "globalconfig/globalconfig.h"
#include "npc/npc.h"
#include "npc/transmit/fbmanager.hpp"
#include "task/taskpool.h"
#include "scene/scene.h"
#include "item/itempool.h"
#include "gamelog.h"
#include "chat/chatmanager.h"
#include "friend/friendblessmanager.h"
#include "monster/monsterpool.h"

#include "obj/character/role.h"
#include "obj/otherobj/worldeventobj.h"
#include "obj/character/monster.h"
#include "obj/otherobj/gatherobj.h"

#include "skill/skillfunction.hpp"
#include "skill/skillpool.hpp"
#include "skill/skillbase.hpp"

#include "effect/skilleffect/effectbuff.hpp"
#include "effect/skilleffect/effecthpstore.hpp"
#include "effect/skilleffect/effecttalenthpstore.hpp"
#include "effect/skilleffect/effectinterval.hpp"
#include "effect/skilleffect/effectspecialstate.hpp"
#include "effect/skilleffect/effectwudiprotect.hpp"
#include "effect/skilleffect/effectmodifyglobalcd.hpp"
#include "effect/skilleffect/effectreboundhurt.hpp"
#include "effect/skilleffect/effectrecoveronatked.hpp"
#include "effect/skilleffect/effectgradualhurt.hpp"
#include "effect/skilleffect/effectjuhuapig.hpp"
#include "effect/skilleffect/effectinvisible.hpp"
#include "effect/skilleffect/effectmovespeed.hpp"
#include "effect/skilleffect/effectonce.hpp"
#include "effect/skilleffect/effectdoublehurt.hpp"
#include "effect/skilleffect/effectmpnocost.hpp"
#include "effect/skilleffect/effectpercentbuff.hpp"
#include "effect/skilleffect/effectforbidrecover.hpp"

#include "global/team/team.hpp"

#include "config/logicconfigmanager.hpp"
#include "config/reloadconfigmanager.hpp"
#include "other/shop/shop.hpp"
#include "other/shop/mysteriousshop.hpp"

#include "other/event/eventhandler.hpp"
#include "other/route/mailroute.hpp"
#include "other/route/publicsaleroute.hpp"
#include "other/queryhandler/queryhandler.hpp"
#include "other/zhibao/zhibaomanager.hpp"
#include "other/route/guildroute.hpp"
#include "other/cmdhandler/cmdhandler.hpp"
#include "other/mount/mountfunction.hpp"
#include "other/fightmount/fightmountfunction.hpp"
#include "other/wing/wingfunction.hpp"
#include "other/halo/halofunction.hpp"
#include "other/shengong/shengongfunction.hpp"
#include "other/shenyi/shenyifunction.hpp"
#include "other/mojie/mojiemanager.hpp"
#include "other/equipsuit/equipsuit.hpp"
#include "other/runesystem/runesystem.hpp"
#include "other/greatesoldier/greatesoldier.hpp"
#include "other/tianshenhuti/tianshenhuti.hpp"

#include "scene/speciallogic/speciallogic.hpp"
#include "scene/speciallogic/guildspecial/specialguildstation.hpp"
#include "scene/speciallogic/worldspecial/specialgongchengzhan.hpp"
#include "scene/speciallogic/worldspecial/specialhunyan.hpp"
#include "scene/speciallogic/worldspecial/specialcrossxiuluotower.hpp"
#include "scene/activityshadow/activityshadow.hpp"
#include "scene/worldshadow/worldshadow.hpp"

#include "internalcomm.h"
#include "servercommon/servercommon.h"
#include "servercommon/campdef.hpp"
#include "servercommon/serverconfig/localconfig.hpp"
#include "servercommon/noticenum.h"
#include "servercommon/performancecatch.hpp"
#include "servercommon/string/gameworldstr.h"
#include "servercommon/internalprotocal/gglobalprotocal.h"

#include "scene/speciallogic/worldspecial/specialcross1v1.hpp"
#include "scene/speciallogic/worldspecial/yaoshouguangchangmgr.hpp"
#include "scene/speciallogic/worldspecial/suoyaotamgr.hpp"
#include "scene/speciallogic/worldspecial/jinghuahusongmgr.hpp"
#include "scene/speciallogic/worldspecial/campdefendmanager.hpp"
#include "scene/speciallogic/personspecial/specialexpfb.hpp"

#include "globalconfig/equipconfig.h"
#include "global/worldevent/worldeventmanager.hpp"
#include "global/activity/activitymanager.hpp"
#include "global/challengefield/challengefield.hpp"
#include "global/challengefield/challengeuser.hpp"
#include "global/fishpool/fishpoolmanager.hpp"
#include "global/camp/campmanager.hpp"
#include "global/specialactivity/specialactivitymanager.hpp"
#include "global/combineserveractivity/combineserveractivitymanager.hpp"
#include "global/randactivity/randactivitymanager.hpp"
#include "global/guild/guildmanager.hpp"
#include "global/guild/guild.hpp"
#include "global/worldboss/worldbossmanager.hpp"
#include "global/bossfamily/bossfamilymanager.hpp"
#include "global/offlineregister/offlineregistermanager.hpp"
#include "global/globalrole/litttlepet/littlepet.hpp"
#include "global/globalrole/globalrolemanager.hpp"

#include "protocal/msgshop.h"
#include "protocal/msgactivity.hpp"
#include "protocal/msgfriend.h"
#include "protocal/msgcspublicsale.hpp"
#include "protocal/msgchatmsg.h"
#include "protocal/msgpet.h"
#include "protocal/msgtianshenhuti.h"

#include "other/convertshop/convertshop.hpp"
#include "equipment/equipmentmanager.h"
#include "other/shenzhouweapon/shenzhouweapon.hpp"
#include "other/xiannv/xiannvmanager.hpp"
#include "item/knapsack.h"
#include "task/newtaskmanager.h"
#include "task/taskrecorder.h"
#include "friend/friendmanager.h"
#include "friend/teaminfo.hpp"
#include "other/mentality/mentality.hpp"
#include "other/vip/vip.hpp"
#include "other/shop/roleshop.hpp"
#include "other/capability/capability.hpp"
#include "other/daycounter/daycounter.hpp"
#include "other/achieve/title/titlemanager.hpp"
#include "other/activedegree/activedegree.hpp"
#include "other/welfare/welfare.hpp"
#include "other/mount/mountmanager.hpp"
#include "other/roleactivity/roleactivity.hpp"
#include "task/other/tumotask.hpp"
#include "task/other/husongtask.hpp"
#include "other/fb/rolefbequip.hpp"
#include "other/footprint/footprint.hpp"
#include "task/other/guildtask.hpp"
#include "other/yaojiang/roleyaojiang.hpp"
#include "other/discountbuy/discountbuy.hpp"
#include "other/monthcard/monthcard.h"
#include "other/turntable/turntable.h"
#include "other/wabao/wabao.hpp"
#include "task/other/cishatask.hpp"
#include "other/jilian/jilian.hpp"
#include "other/fb/rolefbzhanshendian.hpp"
#include "other/touzijihua/touzijihua.hpp"
#include "other/rollmoney/rollmoney.h"
#include "global/worldevent/roleworldevent.hpp"
#include "other/rolegoal/rolegoal.hpp"
#include "other/cross/rolecross.h"
#include "other/fb/roledailyfb.hpp"
#include "task/other/paohuantask.hpp"
#include "other/pet/pet.hpp"
#include "other/multimount/multimount.hpp"
#include "other/lieming/liemingmanager.h"
#include "other/zhuanshengequip/zhuanshengequip.hpp"
#include "other/chinesezodiac/chinesezodiac.hpp"
#include "other/shengesystem/shengesystem.hpp"
#include "other/qingyuan/qingyuan.hpp"
#include "global/globalrole/fighting/rolefighting.hpp"

#include "other/wing/wingmanager.hpp"
#include "other/halo/halomanager.hpp"
#include "other/shengong/shengongmanager.hpp"
#include "other/shenyi/shenyimanager.hpp"
#include "other/fightmount/fightmountmanager.hpp"
#include "other/cloak/cloakmanager.hpp"
#include "other/shenshou/shenshou.hpp"
#include "other/rolezhuanzhi/rolezhuanzhi.hpp"

#include "global/fishpool/fishpool.hpp"
#include "scene/speciallogic/worldspecial/specialxingzuoyiji.hpp"
#include "other/fb/rolepreciousboss.hpp"
#include "other/jinglingadvantage/jinglingadvantage.hpp"
#include "other/mount/mountopconfig.hpp"
#include "other/fightmount/fightmountconfig.hpp"
#include "other/achieve/title/titleconfig.hpp"
#include "other/shenyin/shenyin.hpp"
#include "../elementheart/elementheart.hpp"
#include "gameworld/gameworld/netmsg/serialize_messagehandler.h"
#include "global/bossshanggu/bossshanggumanager.hpp"
#include "other/jinjiesysreward/jinjiesysrewardmgr.hpp"
#include "scene/speciallogic/teamspecial/specialtowerdefendteam.hpp"
#include "scene/speciallogic/teamspecial/specialequipteam.hpp"
#include "global/cross/crossmanager.h"
#include "servercommon/serverconfig/crossconfig.hpp"
#include "other/magicalprecious/magicalprecious.hpp"
#include "other/fb/rolepatafb.hpp"
#include "other/fb/rolefbtowerdefend.hpp"
#include "other/mojie/mojieskill.hpp"
#include "gameworld/config/agentadapterconfig.hpp"
#include "gameworld/gameworld/config/sharedconfig/sharedconfig.h"
#include "other/shizhuang/shizhuang.h"
#include "other/shizhuang/shizhuangconfig.h"
#include "other/zerogift/zerogift.hpp"
#include "other/littlehelper/littlehelper.h"
#include "protocal/msgzhuanzhiequip.hpp"
#include "other/zhuanzhiequip/zhuanzhiequip.h"
#include "other/crossequip/crossequip.hpp"
#include "other/myth/myth.hpp"
#include "other/zodiac/zodiac.hpp"
#include "scene/speciallogic/worldspecial/specialcrosshusongshuijing.hpp"


Gm & Gm::Instance()
{
	static Gm gm;
	return gm;
}

Gm::Gm()
{
	
}

Gm::~Gm()
{

}

bool Gm::MakeupResult(const GSNetID &netid, const char *commandType, bool ret, const char *addition_str)
{
	Protocol::SCGMCommand scgm;	

	STRNCPY(scgm.type, commandType, sizeof(scgm.type));	

	if (!ret)
	{	
		STRNCPY(scgm.result, "gm command FAIL!", sizeof(scgm.result));
	}
	else	
	{	
		STRNCPY(scgm.result, "gm command Succ", sizeof(scgm.result));
	}	

	if ('\0' != addition_str[0])
	{
		strcat(scgm.result, " : ");
		strcat(scgm.result, addition_str);
	}

	scgm.result[sizeof(scgm.result) - 1] = 0;

	EngineAdapter::Instance().NetSend(netid, (const char *)&scgm, sizeof(Protocol::SCGMCommand));

	return ret;
}

bool Gm::OnCommand(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *gmc)
{
	#ifndef _DEBUG

	if (Role::AUTHORITY_TYPE_GM != role->GetAuthority())  // 正式版本 必须设置了gm才允许开这个命令
	{
		return false;
	} 

	#endif 

	if (!GLOBALCONFIG->IsGmOpen()) return false;

	gmc->type[sizeof(gmc->type) - 1] = 0; gmc->command[sizeof(gmc->command) - 1] = 0;

	gamelog::g_log_gm.printf(LL_INFO, "user[%d %s] execute GM command [%s : %s]", UidToInt(role->GetUserId()), role->GetName(), gmc->type, gmc->command);

	CommandFuncItem *command_fun_list = this->GetCommandFuncItem();

	if (NULL != command_fun_list)
	{
		int i = 0; 

		while (NULL != command_fun_list[i].pfnCommandHandler)
		{
			if (strlen(gmc->type) == strlen(command_fun_list[i].type) && !strncmp(gmc->type, command_fun_list[i].type, strlen(gmc->type)))
			{
				bool ret = false;
				char *result[128] = {};
				int arg_num = this->ParseCommandParam(gmc->command, result, " ");

				if (arg_num == command_fun_list[i].param_count)
				{
					if (NULL != command_fun_list[i].pfnCommandHandler)
					{
						ret = (this->*command_fun_list[i].pfnCommandHandler)(netid, scene, role, obj_id, gmc);
					}
				}
			
				// 错误信息统一在这里发送 具体的命令仅仅处理成功消息 
				if (!ret)
				{
					Protocol::SCGMCommand scgm;
					STRNCPY(scgm.type, gmc->type, sizeof(scgm.type));
					STRNCPY(scgm.result, command_fun_list[i].help, sizeof(scgm.result));
					EngineAdapter::Instance().NetSend(netid, (const char *)&scgm, sizeof(Protocol::SCGMCommand));
				}

				break;
			}

			++ i;
		}
	}

	return false;
}

int Gm::ParseCommandParam(char *src, char **result, const char *spilt)
{
	int arg_num = 0;
	
	static char temp[Protocol::MAX_GM_COMMAND_LEN] = {0};  // 必须是static  

	STRNCPY(temp, src, sizeof(temp));

	char *token = strtok(temp, spilt);

	while (NULL != token)
	{
		result[arg_num++] = token;
		token = strtok(NULL, spilt);
	}

	return arg_num;
}

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::EquipUpLv(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int equip_index = atoi(result[0]);
	bool is_puton = (0 != atoi(result[1]));
	bool is_auto_buy = (0 != atoi(result[2]));
	//bool use_lucky_item = (0 != atoi(result[3]));
	//bool ret = EquipFunction::EquipUpLevel(role, is_puton, true, is_auto_buy, equip_index, use_lucky_item, true);
	bool ret = EquipFunction::EquipUpLevel(role, is_puton, is_auto_buy, equip_index);

	return MakeupResult(netid, command->type, ret, "");
}

bool Gm::MoveRole(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int tar_scene_id = atoi(result[0]);
	int tar_scene_key = atoi(result[1]);
	int x = atoi(result[2]);
	int y = atoi(result[3]);

	Posi tar_pos(x, y);

	bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(role ,tar_scene_id, tar_scene_key, tar_pos);

	return MakeupResult(netid, command->type, ret, "");
}

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::ResetPos(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	
	int x = atoi(result[0]);
	int y = atoi(result[1]);

	role->ResetPos(x, y);

	return MakeupResult(netid, command->type, true, "");
}

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::AddFriend(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	// 仅仅提供单向接口 客户端使用两次接口即可成双向的好友啦 

	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	//GameName name;

	int role_id = atoi(result[0]);
	/*STRNCPY(name, result[1], sizeof(name));

	int sex = atoi(result[2]);
	int prof = atoi(result[3]);*/

	UserID user_id = IntToUid(role_id);

	bool ret = role->GetFriendManager()->AddFriend(user_id, "GM");

	return MakeupResult(netid, command->type, ret, "");
}
// --------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::DeleteFriend(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int role_id = atoi(result[0]);
	int is_silence = atoi(result[1]);

	bool ret = role->GetFriendManager()->RemoveFriend(IntToUid(role_id), "GM");
	if (ret && !is_silence)
	{
		role->NoticeNum(noticenum::NT_REMOVE_FRIEND_SUCC);

		Role *target_role = World::GetInstWorld()->GetSceneManager()->GetRole(IntToUid(role_id));
		if (NULL != target_role)
		{
			target_role->GetFriendManager()->RemoveFriend(role->GetUserId(), "Client Be GM");
		}
	}

	return MakeupResult(netid, command->type, ret, "");
}

bool Gm::GiveFlower(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	
	ggprotocal::GiveFlower gf;
	gf.grid_index = -1;
	gf.item_id = atoi(result[0]);
	gf.is_anonymity = false;
	gf.target_uid = atoi(result[1]);
	gf.is_marry = 0;

	role->GetFriendManager()->OnGiveFlower(&gf);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddCharm(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int charm_value = atoi(result[0]);

	role->GetFriendManager()->AddAllCharm(charm_value);

	return MakeupResult(netid, command->type, true, "");
}

// --------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::AddItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	
	int item_id = atoi(result[0]);
	int item_num = atoi(result[1]);
	bool is_bind = (0 != atoi(result[2]));

	const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
	bool ret = false;
	if (NULL != itembase)
	{
		ItemConfigData item_config_data(item_id, is_bind, item_num);
		int grid_index = -1;
		ret = role->GetKnapsack()->Put(item_config_data, PUT_REASON_GM, &grid_index);
		

		// 随机获得属性
		if(ret && grid_index >= 0)
		{
			ItemGridData item_grid_data;
			role->GetKnapsack()->GetItemGridData(grid_index, &item_grid_data);
			if (EquipmentManager::GetRandomSpeicalAttr(&item_grid_data))
			{
				role->GetKnapsack()->SetItemDirty(grid_index);
			}
		}
	}

	return MakeupResult(netid, command->type, ret, "");
}
// --------------------------------------------------------------------------------------------------------------------------

bool Gm::AddItemList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	ItemID begin_item_id = atoi(result[0]);
	ItemID end_item_id = atoi(result[1]) + 1;
	ItemID increase = atoi(result[2]);
	int item_num = 0;
	if (increase < 1)
	{
		return false;
	}
	bool is_bind = false;

	int count = 0;
	bool ret = false;
	for (ItemID item_id = begin_item_id; item_id < end_item_id && count < ItemNamespace::MAX_KNAPSACK_GRID_NUM; item_id += increase)
	{
		const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
		if (NULL != itembase)
		{
			item_num = itembase->GetPileLimit();
			ItemConfigData item_config_data(item_id, is_bind, item_num);
			int grid_index = -1;
			ret = role->GetKnapsack()->Put(item_config_data, PUT_REASON_GM, &grid_index);


			// 随机获得属性
			if (ret && grid_index >= 0)
			{
				ItemGridData item_grid_data;
				role->GetKnapsack()->GetItemGridData(grid_index, &item_grid_data);
				if (EquipmentManager::GetRandomSpeicalAttr(&item_grid_data))
				{
					role->GetKnapsack()->SetItemDirty(grid_index);
				}
			}
		}
		count++;
	}
	return MakeupResult(netid, command->type, ret, "");
}

bool Gm::AddItemNumList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	ItemID begin_item_id = atoi(result[0]);
	ItemID end_item_id = atoi(result[1]) + 1;
	ItemID increase = atoi(result[2]);
	int item_num = atoi(result[3]);
	if (increase < 1)
	{
		return false;
	}
	bool is_bind = false;

	int count = 0;
	bool ret = false;
	for (ItemID item_id = begin_item_id; item_id < end_item_id && count < ItemNamespace::MAX_KNAPSACK_GRID_NUM; item_id += increase)
	{
		const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
		if (NULL != itembase)
		{
			if (item_num < 1)
			{
				item_num = itembase->GetPileLimit();
			}
			ItemConfigData item_config_data(item_id, is_bind, item_num);
			int grid_index = -1;
			ret = role->GetKnapsack()->Put(item_config_data, PUT_REASON_GM, &grid_index);


			// 随机获得属性
			if (ret && grid_index >= 0)
			{
				ItemGridData item_grid_data;
				role->GetKnapsack()->GetItemGridData(grid_index, &item_grid_data);
				if (EquipmentManager::GetRandomSpeicalAttr(&item_grid_data))
				{
					role->GetKnapsack()->SetItemDirty(grid_index);
				}
			}
		}
		count++;
	}
	return MakeupResult(netid, command->type, ret, "");
}

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::AddItemUse(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int item_id = atoi(result[0]);
	int item_num = atoi(result[1]);
	bool is_bind = (0 != atoi(result[2]));
	
	bool ret = false;

	const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
	if (NULL != itembase)
	{
		int index = -1;
		ItemConfigData item_config_data(item_id, is_bind, item_num);
		ret = role->GetKnapsack()->Put(item_config_data, PUT_REASON_GM, &index);
		if (ret)
		{
			ret = role->GetKnapsack()->Use(index, 1);
		}
	}

	return MakeupResult(netid, command->type, ret, "");
}
// --------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::KnapsackTest(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int value = atoi(result[0]);

	Knapsack *knapsack = role->GetKnapsack();

	switch (value)
	{
	case 1:
		// 扩展背包
		role->GetKnapsack()->ExtendValidNum(true, false, 35);

		ItemConfigData item_config_data;
		item_config_data.num = 1;
		item_config_data.is_bind = false;

		// 装备
		item_config_data.item_id = 10001;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 10002;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 10003;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 1001;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 1002;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 13000;
		knapsack->Put(item_config_data, PUT_REASON_GM);

		item_config_data.num = 50;

		// 强化石
		item_config_data.item_id = 24550;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24551;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24552;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24553;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24554;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24555;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24556;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24557;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24558;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24559;
		knapsack->Put(item_config_data, PUT_REASON_GM);

		// 洗炼石
		item_config_data.item_id = 24580;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24581;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24582;
		knapsack->Put(item_config_data, PUT_REASON_GM);

		// 洗炼锁
		item_config_data.item_id = 26050;
		knapsack->Put(item_config_data, PUT_REASON_GM);

		// 碎片材料
		item_config_data.item_id = 24750;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24751;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24760;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24761;
		knapsack->Put(item_config_data, PUT_REASON_GM);

		// 宝石
		item_config_data.item_id = 24000;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24001;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24100;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		item_config_data.item_id = 24101;
		knapsack->Put(item_config_data, PUT_REASON_GM);

		// 进阶保护符
		item_config_data.item_id = 24770;
		knapsack->Put(item_config_data, PUT_REASON_GM);
		
		break;
	}

	return true;
}

bool Gm::SetEquipData(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int index = atoi(result[1]);
	int value = atoi(result[2]);

	EquipmentManager *equip_mgr = role->GetEquipmentManager();

	bool ret = false;
	ItemGridData equip_data;
	if (!equip_mgr->GetEquipmentGridData(index, &equip_data))
	{
		EquipmentParam *equip_param = (EquipmentParam *)equip_data.param;
		if (NULL != equip_param)
		{
			switch(type)
			{
			case 1:
				equip_param->strengthen_level = value;
				if (equip_param->strengthen_level < 0)
				{
					equip_param->strengthen_level = 0;
				}
				else if (equip_param->strengthen_level > EquipConfig::MAX_STRENGTH_LEVEL)
				{
					equip_param->strengthen_level = EquipConfig::MAX_STRENGTH_LEVEL;
				}
				break;
			}

			role->ReCalcAttr(CharIntAttrs::RECALC_REASON_EQUIPMENT);

			equip_mgr->SetEquipGridDirty(index);
		}
		
		ret = true;
	}

	return MakeupResult(netid, command->type, ret, "");
}
// --------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::TakeOutAll(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	bool ret = role->GetKnapsack()->TakeOutAllItemFromStorage();

	return MakeupResult(netid, command->type, ret, "");
}
// --------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::ClearBag(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetKnapsack()->ClearAll();

	return MakeupResult(netid, command->type, true, "");
}
// --------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::ExpandBag(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int extend_count = atoi(result[0]);
	role->GetKnapsack()->ExtendValidNum(true, false, extend_count);

	return MakeupResult(netid, command->type, true, "");
}
// --------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::MoveBagItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	
	int from_index = atoi(result[0]);
	int to_index = atoi(result[1]);

	bool ret = role->GetKnapsack()->Move(from_index, to_index);

	return MakeupResult(netid, command->type, ret, "");
}
// --------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::AddEquipUse(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	bool ret = false;

	int item_id = atoi(result[0]);
	int equip_index = atoi(result[1]);

	const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
	if (NULL != itembase)
	{
		int grid_index = -1;
		ret = role->GetKnapsack()->Put(ItemConfigData(item_id, 1, 1), PUT_REASON_GM, &grid_index);
		if (ret)
		{
			ret = role->GetKnapsack()->Use(grid_index, 1, equip_index);
		}
	}

	return MakeupResult(netid, command->type, ret, "");
}

bool Gm::NBRobot(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	bool ret = false;

	role->GMSetLevel(130);

	if (role->GetProf() <= PROF_TYPE_INVALID || role->GetProf() >= PROF_TYPE_PROF_NO_LIMIT)
	{
		role->SetProf(PROF_TYPE_PROF_2);
	}

	int prof = role->GetProf();

	int equip_id_list[5][2] = 
	{
		{0, 0}, {1522, 10522}, {1622, 10622}, {1722, 10722}, {1822, 10822}		// yifu, quqi
	};

	for (int i = 0; i < 2; ++i)
	{
		int item_id = equip_id_list[prof][i];

		const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
		if (NULL != itembase)
		{
			int grid_index = -1;
			ret = role->GetKnapsack()->Put(ItemConfigData(item_id, 1, 1), PUT_REASON_GM, &grid_index);
			if (ret)
			{
				ItemGridData item_data;
				ret = role->GetKnapsack()->GetItemGridData(grid_index, &item_data);
				if (ret)
				{
					ItemDataWrapper item_wrapper(item_id, 1, 1);
					item_wrapper.invalid_time = item_data.invalid_time;

					EquipmentParam* equip_param = (EquipmentParam*)item_data.param;
					equip_param->strengthen_level = 25;
					equip_param->SetInStructData(&item_wrapper.param_data);
					item_wrapper.has_param = 1;
				}

				int equip_index = 0;
				if (0 == i)
				{
					equip_index = Equipment::E_INDEX_YIFU;
				}
				else
				{
					equip_index = Equipment::E_INDEX_WUQI;
				}
				ret = role->GetKnapsack()->Use(grid_index, 1, equip_index);
			}
		}
	}

	return MakeupResult(netid, command->type, ret, "");
}
// --------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::DiscardBagItems(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	bool ret = false;

	int start_index = atoi(result[0]);
	int discard_count = atoi(result[1]);
	if (start_index < 0 || discard_count < -1) return MakeupResult(netid, command->type, ret, ""); 
	if (discard_count < 0 || discard_count > ItemNamespace::MAX_KNAPSACK_GRID_NUM) discard_count = ItemNamespace::MAX_KNAPSACK_GRID_NUM;

	int consume_count = 0;
	short comsume_index_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM] = {0};
	short comsume_num_list[ItemNamespace::MAX_KNAPSACK_GRID_NUM] = {0};
	
	for (int grid_index = start_index; grid_index < start_index + discard_count; ++grid_index)
	{
		ItemGridData item_grid_data;
		if (!role->GetKnapsack()->GetItemGridData(grid_index, &item_grid_data))	continue;

		if (consume_count < static_cast<int>(sizeof(comsume_index_list) / sizeof(comsume_index_list[0]))) 
		{
			comsume_num_list[consume_count] = item_grid_data.num;
			comsume_index_list[consume_count] = grid_index;
			++consume_count;
		}
	}

	ret = role->GetKnapsack()->ConsumeItemByIndexList(consume_count, comsume_index_list, comsume_num_list, "gm_discardbagitems");

	return MakeupResult(netid, command->type, ret, "");
}

bool Gm::RecycleBagItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	bool ret = false;

	int start_index = atoi(result[0]);
	int discard_count = atoi(result[1]);
	if (start_index < 0 || start_index >= ItemNamespace::MAX_KNAPSACK_GRID_NUM) return MakeupResult(netid, command->type, ret, "");
	if (discard_count <= 0) return MakeupResult(netid, command->type, ret, "");

	ret = role->GetKnapsack()->Discard(start_index, discard_count, DISCARD_METHOD_RECYCL);

	return MakeupResult(netid, command->type, ret, "");
}
// --------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::AddExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long exp = atoll(result[0]);
	role->AddExp(exp, "GM", Role::EXP_ADD_REASON_DEFAULT);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ZhuanSheng(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	role->ZhuanSheng();
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddMoney(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	Int64 add_money = atoll(result[0]);

	role->GetKnapsack()->GetMoney()->Add(add_money, add_money, add_money, add_money, "Gm");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddCoin(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	Int64 add_coin = atoll(result[0]);

	role->GetKnapsack()->GetMoney()->AddCoinBind(add_coin, "Gm");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::DecMoney(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	Int64 dec_money = atoll(result[0]);
	
	// 扣钱
	Int64 consume_gold = static_cast<Int64>(dec_money);
	if (dec_money > role->GetKnapsack()->GetMoney()->GetGold())
	{
		consume_gold = role->GetKnapsack()->GetMoney()->GetGold();
	}

	Int64 consume_gold_bind = static_cast<Int64>(dec_money);
	if (dec_money > role->GetKnapsack()->GetMoney()->GetGoldBind())
	{
		consume_gold_bind = role->GetKnapsack()->GetMoney()->GetGoldBind();
	}

	Int64 consume_coin = static_cast<Int64>(dec_money);
	if (dec_money > role->GetKnapsack()->GetMoney()->GetCoinBind())
	{
		consume_coin = role->GetKnapsack()->GetMoney()->GetCoinBind();
	}

	role->GetKnapsack()->GetMoney()->UseGold(consume_gold, "Gm");
	role->GetKnapsack()->GetMoney()->UseGoldBind(consume_gold_bind, "Gm");
	role->GetKnapsack()->GetMoney()->UseCoinBind(consume_coin, "Gm");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddYuanli(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int add_yuanli = atoi(result[0]);
	bool ret = role->AddYuanli(add_yuanli, "Gm");

	return MakeupResult(netid, command->type, ret, "");
}

bool Gm::AddNuqi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int add_nuqi = atoi(result[0]);
	role->AddNuqi(add_nuqi);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangeSpeed(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta_speed = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED, delta_speed);
	role->AddGmMoveSpeed(delta_speed);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangeGongji(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta_gongji = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, delta_gongji);
	role->AddGmChangeGongji(delta_gongji);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ResetGmChangeAttr(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->ResetGMChangeAttribute();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangeFangyu(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta_fangyu = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU, delta_fangyu);
	role->AddGmChangeFangYu(delta_fangyu);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangeMaxHP(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta_maxhp = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MAXHP, delta_maxhp);
	role->AddGmChangeMaxHp(delta_maxhp);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangeBaoji(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta_baoji = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI, delta_baoji);
	role->AddGmChangeBaoJi(delta_baoji);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangeMingzhong(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG, delta);
	role->AddGmChangeMingZhong(delta);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangeShanbi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_SHANBI, delta);
	role->AddGmChangeShanBi(delta);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangeJianren(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_JIANREN);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_JIANREN, delta);
	role->AddGmChangeJianRen(delta);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangeFujiaShanghai(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FUJIA_SHANGHAI, delta);
	role->AddGmChangeFujiaShanghai(delta);
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangeDikangShanghai(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_DIKANG_SHANGHAI);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_DIKANG_SHANGHAI, delta);
	role->AddGmChangeDikangShanghai(delta);
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangePerJingzhun(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_JINGZHUN);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_JINGZHUN, delta);
	role->AddGmChangePerJingzhun(delta);
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangePerBaoji(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI_HURT);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_BAOJI_HURT, delta);
	role->AddGmChangePerBaoji(delta);
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangePerKangbao(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_KANGBAO);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_KANGBAO, delta);
	role->AddGmChangePerKangbaoi(delta);
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangePerPofang(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_POFANG);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_POFANG, delta);
	role->AddGmChangePerPofang(delta);
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangePerMianshang(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_PER_MIANSHANG, delta);
	role->AddGmChangePerMianshang(delta);
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangeHuixinyiji(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HUIXINYIJI);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HUIXINYIJI, delta);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangeGedangPer(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long delta = atoll(result[0]) - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GEDANG_PER);
	role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GEDANG_PER, delta);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangeCapabilityAttr(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	long long cap = atoll(result[0]);
	role->SetGmChangeCapabilityAttr(cap);
	role->ReCalcAttr();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::Recover(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->Recover();

	return MakeupResult(netid, command->type, true, "");
}
// --------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::JoinCamp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	char camp_type = atoi(result[0]);
	role->SetCamp(camp_type, false);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetProf(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int prof = atoi(result[0]);
	role->GMSetProf(prof);

	return MakeupResult(netid, command->type, true, "");
}
// --------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::SetAttackMode(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int attack_mode = atoi(result[0]);
	role->SetAttackMode(attack_mode, true, false);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::StopAttack(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->StopAttack();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::PerformSkill(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int skill_id = atoi(result[0]);
	int pos_x = atoi(result[1]);
	int pos_y = atoi(result[2]);
	int target_id = atoi(result[3]);
	bool is_special = (1 == atoi(result[4]) ? true : false);

	if (!is_special)
	{
		int skill_index = 0;
		role->GetSkillManager()->GetSkillByID(skill_id, &skill_index);

		role->PerformSkill(skill_index, Posi(pos_x, pos_y), target_id, is_special);
	}
	else
	{
		role->PerformSkill(skill_id, Posi(pos_x, pos_y), target_id, is_special);
	}

	return MakeupResult(netid, command->type, true, "");
}

/*
	@effect_type：作用类型
	@buff_type: buff类型
	@skill_id: 技能id
	@skill_level:技能等级
	@param1/2/3/4:	修改属性类，effect_type = EFFECT_TYPE_ATTRBUFF，param1：属性类型，param2：属性值；param3：属性类型，param4：属性值
					定时改HP，effect_type = EFFECT_TYPE_INTERVAL_HP，param1：减少血量
					修改全局CD，effect_type = EFFECT_TYPE_MODIFY_GLOBAL_CD，param1：百分比
					伤害反弹，effect_type = EFFECT_TYPE_REBOUND_HURT，param1：百分比
					被击回血，effect_type = EFFECT_TYPE_RECOVER_ON_ATKED，param1：百分比
					渐增伤害，effect_type = EFFECT_TYPE_GRADUAL_HURT，param1：初始伤害值，param2：百分比；param3：修正伤害值
					菊花猪，effect_type = EFFECT_TYPE_JUHUAPIG，param1：爆炸条件，param2：反弹百分比；param3：影响范围
					速度类，effect_type = EFFECT_TYPE_MOVESPEED，param1：百分比
					修改属性百分比，effect_type = EFFECT_TYPE_ATTR_PER，param1：百分比
	@merge_rule:叠加规则
	@deliverer:施放者
	@product_method:产生原因
*/
bool Gm::AddEffect(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int effect_type = atoi(result[0]);
	int buff_type = atoi(result[1]);
	int skill_id = atoi(result[2]);
	int skill_level = atoi(result[3]);
	int param1 = atoi(result[4]);
	int param2 = atoi(result[5]);
	int param3 = atoi(result[6]);
	int param4 = atoi(result[7]);
	char merge_rule = static_cast<char>(atoi(result[8]));
	UInt16 deliverer = atoi(result[9]);
	char method = (char)(atoi(result[10]));

	if (0 == deliverer) deliverer = role->GetId();

	switch (effect_type)
	{
	case EffectBase::EFFECT_TYPE_MP_NO_COST:
		{
			EffectMPNoCost *mp_no_cost = new EffectMPNoCost(deliverer, 1, 10000, 1);
			mp_no_cost->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_1);
			role->AddEffect(mp_no_cost);
		}
		break;

	case EffectBase::EFFECT_TYPE_DOUBLE_HURT:
		{
			EffectDoubleHurt *effect_double_hurt = new EffectDoubleHurt(deliverer, 1, 10000, 1);
			effect_double_hurt->SetHurtParam(5, 1000);
			effect_double_hurt->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
			role->AddEffect(effect_double_hurt);
		}
		break;

	case EffectBase::EFFECT_TYPE_ATTRBUFF:
		{
			EffectBuff *effect_buff = new EffectBuff(deliverer, skill_id, 30000, skill_level, method);
			effect_buff->SetMaxMergeLayer(10);
			
			int percent = 10;
			if (param2 < 0)
			{
				percent *= -1;
			}
			effect_buff->AddAttrEffect(param1, param2, percent);

			percent = 10;
			if (param2 < 0)
			{
				percent *= -1;
			}
			effect_buff->AddAttrEffect(param3, param4, percent);

			role->AddEffect(effect_buff);
		}
		break;

	case EffectBase::EFFECT_TYPE_HPSTORE:
		{
			EffectHpStore *effect_hpstore = new EffectHpStore(deliverer, 1, 5000, 1000, skill_level);
			effect_hpstore->SetReplacePercent(100);
			role->AddEffect(effect_hpstore);
		}
		break;

	case EffectBase::EFFECT_TYPE_INTERVAL_HP:
		{
			EffectInterval *effect_interval = new EffectInterval(deliverer, skill_id, buff_type, skill_level);
			effect_interval->SetMergeRule(merge_rule);
			effect_interval->SetMaxMergeLayer(10);
			effect_interval->SetParam(3000, 50, param1);
			if (param1 < 0)
			{
				effect_interval->SetClientEffectType(EffectBase::ECT_SKILL_DROP_BLOOD);
			}
			role->AddEffect(effect_interval);
		}
		break;

	case EffectBase::EFFECT_TYPE_SPECIAL_STATE:
		{
			EffectSpecialState *effect_special = new EffectSpecialState(deliverer, 1, 20000, buff_type, skill_level);
			effect_special->SetParam(param1);
			effect_special->SetClientEffectType(param2);
			role->AddEffect(effect_special);
		}
		break;

	case EffectBase::EFFECT_TYPE_WUDI_PROTECT:
		{
			EffectWudiProtect *effect_wudi = new EffectWudiProtect(deliverer, skill_id, 30000, 1);
			role->AddEffect(effect_wudi);
		}
		break;

	case EffectBase::EFFECT_TYPE_MODIFY_GLOBAL_CD:
		{
			EffectModifyGlobalCD *effect_cd = new EffectModifyGlobalCD(deliverer, skill_id, 30000, param1, 1);
			role->AddEffect(effect_cd);
		}
		break;

	case EffectBase::EFFECT_TYPE_REBOUND_HURT:
		{
			EffectReboundHurt *effect_rebound = new EffectReboundHurt(deliverer, skill_id, 30000, param1, 1);
			role->AddEffect(effect_rebound);
		}
		break;

	case EffectBase::EFFECT_TYPE_RECOVER_ON_ATKED:
		{
			EffectRecoverOnAtked *effect_recover = new EffectRecoverOnAtked(deliverer, skill_id, 30000, param1, 1);
			role->AddEffect(effect_recover);
		}
		break;

	case EffectBase::EFFECT_TYPE_GRADUAL_HURT:
		{
			EffectGradualHurt *effect_gradual_hurt = new EffectGradualHurt(deliverer, skill_id, 1);
			effect_gradual_hurt->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_4);
			effect_gradual_hurt->SetParam(3000, 50, param1, param2, param3);
			role->AddEffect(effect_gradual_hurt);
		}
		break;

	case EffectBase::EFFECT_TYPE_JUHUAPIG:
		{
			EffectJuhuaPig *effect = new EffectJuhuaPig(deliverer, skill_id, 1);
			effect->SetParam(30000, param1, param2, param3, role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP));
			role->AddEffect(effect);
		}
		break;

	case EffectBase::EFFECT_TYPE_INVISIBLE:
		{
			EffectInvisible *effect_invisible = new EffectInvisible(deliverer, skill_id, 45000, 1, EffectBase::PRODUCT_METHOD_SCENE_SKILL);
			role->AddEffect(effect_invisible);
		}
		break;

	case EffectBase::EFFECT_TYPE_MOVESPEED:
		{
			unsigned char buff_type = EBT_CHIHUAN;
			if (param1 > 0)
			{
				buff_type = EBT_JIASU;
			}

			int chg_movespeed = static_cast<int>(role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED) * (param1 / 100));

			EffectMoveSpeed *effect_speed = new EffectMoveSpeed(deliverer, skill_id, 30000, buff_type, 1);
			effect_speed->SetMoveSpeed(chg_movespeed, param1);
			effect_speed->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_3);
			role->AddEffect(effect_speed);
		}
		break;

	case EffectBase::EFFECT_TYPE_ATTR_PER:
		{
			role->GetEffectList()->FindAndRemoveEffect(EffectBase::EFFECT_TYPE_ATTR_PER, EBT_INVALID, INVALID_OBJ_ID, EffectBase::PRODUCT_METHOD_NAME_COLOR, 0);
			EffectPercentBuff *effect = new EffectPercentBuff(INVALID_OBJ_ID, 0, 10000, 1, EffectBase::PRODUCT_METHOD_NAME_COLOR);
			effect->AddAttrEffect(CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP, (short)param1);
			effect->AddAttrEffect(CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI, (short)param2);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
			role->AddEffect(effect);
		}
		break;

	case EffectBase::EFEECT_TYPE_OTHER:
		{

		}
		break;

	case EffectBase::EFFECT_TYPE_TALENT_HPSTORE:
		{
			EffectTalentHpStore *effect = new EffectTalentHpStore(INVALID_OBJ_ID, 0, 10000, 1, EffectBase::PRODUCT_METHOD_TALENT_SKILL);
			effect->SetReplacePercent(50.0f);
			effect->SetMergeRule(EffectBase::MERGE_RULE_NOMERGE_2);
			role->AddEffect(effect);
		}
		break;

	case EffectBase::EFFECT_TYPE_FORBID_RECOVER:
		{
			EffectForbidRecover *effect_forbid = new EffectForbidRecover(INVALID_OBJ_ID, 0, 20000, 1, EffectBase::PRODUCT_METHOD_TALENT_SKILL);
			role->AddEffect(effect_forbid);
		}
		break;
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RemoveEffect(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int effect_type = atoi(result[0]);
	int buff_type = atoi(result[1]);
	ObjID deliverer = atoi(result[2]);
	int method = atoi(result[3]);
	int skill_id = atoi(result[4]);

	role->GetEffectList()->FindAndRemoveEffect(effect_type, buff_type, deliverer, method, skill_id);

	return MakeupResult(netid, command->type, true, "");
}
// --------------------------------------------------------------------------------------------------------------------------

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::AddSkill(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	
	int skill_id = atoi(result[0]);
	int skill_level = atoi(result[1]);

	bool ret = role->GetSkillManager()->AddSkill(skill_id, skill_level);

	return MakeupResult(netid, command->type, ret, "");
}

bool Gm::LearnSkill(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int skill_id = atoi(result[0]);
	//int one_key_learn = atoi(result[1]);

	bool ret = SkillFunction::LearnSkill(role, skill_id);

	return MakeupResult(netid, command->type, ret, "");
}

bool Gm::RemoveSkill(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int skill_id = atoi(result[0]);

	bool ret = role->GetSkillManager()->RemoveSkill(skill_id);

	return MakeupResult(netid, command->type, ret, "");
}

bool Gm::ChangeSkillLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int skill_id = atoi(result[0]);
	int skill_level = atoi(result[1]);

	bool ret = role->GetSkillManager()->GmSetSkillLevel(skill_id, skill_level);

	return MakeupResult(netid, command->type, ret, "");
}

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::TestSkill(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	UInt16 skill_id = static_cast<UInt16>(atoi(result[0]));
	int pos_x = atoi(result[1]);
	int pos_y = atoi(result[2]);
	int target_id = atoi(result[3]);
	int special_param = atoi(result[4]);

	if (pos_x < 0 || pos_y < 0)
	{
		pos_x = role->GetPos().x;
		pos_y = role->GetPos().y;
	}

	const SkillBase *skill = SKILLPOOL->GetSkill(skill_id);
	if (NULL != skill)
	{
		if (skill->IsAttackCharacter())
		{
			Obj *target_obj = scene->GetObj(target_id);
			if (NULL != target_obj && Obj::IsCharactor(target_obj->GetObjType()))
			{
				Character *target = (Character *)target_obj;
				skill->Perform(role, role->GetLevel(), target, Posi(pos_x, pos_y), special_param);
			}
		}
		else
		{
			skill->Perform(role, role->GetLevel(), NULL, Posi(pos_x, pos_y), special_param);
		}
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ShowTargets(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int range = atoi(result[0]);

	char target_list_str[256] = {0}; 
	int target_list_str_len = 0;

	ObjID target_list[16] = {0};
	int count = SkillBase::GetTargetByArea(role, role->GetPos(), range, target_list, 16);
	for (int i = 0; i < count; ++ i)
	{
		char temp_buf[16] = {0};
		int temp_len = SNPRINTF(temp_buf, sizeof(temp_buf), "%d, ", target_list[i]);
		if (temp_len > 0)
		{
			STRNCPY(target_list_str + target_list_str_len, temp_buf, sizeof(target_list_str) - target_list_str_len);
			target_list_str_len += temp_len;
		}
	}

	target_list_str[sizeof(target_list_str) - 1] = 0;

	return MakeupResult(netid, command->type, true, target_list_str);
}

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::AddOneTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {0};
	this->ParseCommandParam(command->command, result, " ");

	TaskID task_id = atoi(result[0]);
	role->GetTaskManager()->GMAddTask(task_id);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::JumpToTrunk(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {0};
	this->ParseCommandParam(command->command, result, " ");
	TaskID task_id = atoi(result[0]);

	int count = 0; 
	int task_list[1000]; memset(task_list, 0, sizeof(task_list));
	const TaskCfg *task = TASKPOOL->GetCfgByTaskID(task_id);

	while (NULL != task)
	{
		if (count < 1000)
		{
			task_list[count++] = task->pretaskid;
		}
		else
		{
			break;
		}

		task = TASKPOOL->GetCfgByTaskID(task->pretaskid);
	}

	for (int i = count - 1; i >= 0; --i)
	{
		role->GetTaskManager()->GMAddTask(task_list[i]);

		for (int try_count = 0; try_count < 256; ++ try_count)
		{
			int  task_id = 0;
			bool ret = role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_BRANCH, &task_id);

			if (!ret && !role->GetTaskManager()->GetTaskIdByType(TASK_TYPE_TRUNK, &task_id))
			{
				break;
			}
			
			role->GetTaskManager()->ForceCompleteTask(task_id);
			role->GetTaskManager()->GMCommitTask(task_id);
		}
	}

	role->GetTaskManager()->GMAddTask(task_id);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CommitAllTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {0};
	this->ParseCommandParam(command->command, result, " ");

	role->GetTaskManager()->GMCommitAllTask();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::JumpToTrunkNoCommitBranch(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {0};
	this->ParseCommandParam(command->command, result, " ");
	TaskID task_id = atoi(result[0]);

	int count = 0; 
	int task_list[1000]; memset(task_list, 0, sizeof(task_list));
	const TaskCfg *task = TASKPOOL->GetCfgByTaskID(task_id);

	while (NULL != task)
	{
		if (count < 1000)
		{
			task_list[count++] = task->pretaskid;
		}
		else
		{
			break;
		}

		task = TASKPOOL->GetCfgByTaskID(task->pretaskid);
	}

	for (int i = count - 1; i >= 0; --i)
	{
		role->GetTaskManager()->GMAddTask(task_list[i]);
		role->GetTaskManager()->ForceCompleteTask(task_list[i]);
		role->GetTaskManager()->GMCommitTask(task_list[i]);
	}

	return MakeupResult(netid, command->type, true, "");
}

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::MoveRoleToNPC(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {0};
	this->ParseCommandParam(command->command, result, " ");

	UInt16 npc_id = atoi(result[0]);
	if (npc_id == 0) return MakeupResult(netid, command->type, false, "");

	Scene *npc_scenes[1] = {NULL};
	if (World::GetInstWorld()->GetSceneManager()->GetNpcScenes(npc_id, npc_scenes, 1) > 0)
	{
		Scene *scene = npc_scenes[0];
		NPC *npc = scene->GetNPCList()->GetNpc(npc_id);
		if (NULL != npc)
		{
			bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(role ,scene->GetSceneID(), scene->GetSceneKey(), npc->GetPos());
			return MakeupResult(netid, command->type, ret, "");	
		}
	}

	return MakeupResult(netid, command->type, false, "");
}

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::ForceCommitTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	TaskID task_id = static_cast<TaskID>(atoi(result[0]));

	role->GetTaskManager()->CommitTask(task_id);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TestTaskRecord(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int record_count = (int)(atoi(result[0]));

	for (int i = 0; i < record_count; ++i)
	{
		role->GetTaskRecorder()->Complete(i + 1);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GiveUpTaskByID(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	TaskID task_id = static_cast<TaskID>(atoi(result[0]));

	role->GetTaskManager()->GiveupTask(task_id);

	return MakeupResult(netid, command->type, true, "");
}

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::Wudi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {0};
	ParseCommandParam(command->command, result, " ");

	CharIntAttrs::AttrPair attr_list[2];
	attr_list[0].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI; attr_list[0].attr_value = 1;
	attr_list[1].attr_type = CharIntAttrs::FIGHT_CHARINTATTR_TYPE_FANGYU; attr_list[1].attr_value = 99999999;
	role->ChangeCharAttrHelper(attr_list, sizeof(attr_list)/sizeof(attr_list[0]));

	EffectWudiProtect *effect_wudi = new EffectWudiProtect(role->GetId(), 2, 30000, 1);
	effect_wudi->SetClientEffectType(1108);
	role->AddEffect(effect_wudi);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::Boss(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {0};
	ParseCommandParam(command->command, result, " ");
	
	role->GetEffectList()->AddSpecialBuff(EBT_WUDI);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SpecialState(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {0};
	ParseCommandParam(command->command, result, " ");

	int buff_type = atoi(result[0]);
	int dur_time = atoi(result[1]);
	int product_level = atoi(result[2]);

	if ((EBT_SUPER_MIANYI == buff_type || buff_type >= EBT_XUANYUN) && dur_time > 0)
	{
		EffectSpecialState *effect_special = new EffectSpecialState(role->GetId(), 0, dur_time, buff_type, product_level);
		role->AddEffect(effect_special);
	}

	return MakeupResult(netid, command->type, true, "");
}

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::ViewInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {0};
	ParseCommandParam(command->command, result, " ");

	bool info_support = false;
	static char info_buf[128] = {0};
	if (strncmp(result[0], "id", strlen("id")) == 0)
	{
		info_support = true;
		SNPRINTF(info_buf, sizeof(info_buf), "role_id=%d (%d, %d) obj_id=%u camp=%d prof=%d", UidToInt(role->GetUserId()), role->GetUserId().db_index, role->GetUserId().role_id, obj_id, role->GetCamp(), role->GetProf());
	}
	else if (strncmp(result[0], "pos", strlen("pos")) == 0)
	{
		info_support = true;
		SNPRINTF(info_buf, sizeof(info_buf), "scene:%d pos:(%d, %d)", role->GetScene()->GetSceneID(), role->GetPos().x, role->GetPos().y); 
	}
	else if (strncmp(result[0], "attr", strlen("attr")) == 0)
	{
		info_support = true;
		SNPRINTF(info_buf, sizeof(info_buf), "speed:%d, speed_base:%d", role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MOVE_SPEED), role->GetCharaIntAttr(CharIntAttrs::BASE_CHARINTATTR_TYPE_MOVE_SPEED)); 
	}
	else if (strncmp(result[0], "guild", strlen("guild")) == 0)
	{
		info_support = true;
		SNPRINTF(info_buf, sizeof(info_buf), "guild_id:%d", role->GetGuildID()); 
	}

	return MakeupResult(netid, command->type, info_support, info_buf);
}

// --------------------------------------------------------------------------------------------------------------------------
bool Gm::ViewPos(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {0};
	ParseCommandParam(command->command, result, " ");

	static char info_buf[128] = {0};
	ObjID target_obj_id = static_cast<ObjID>(atoi(result[0]));

	Obj *obj = scene->GetObj(target_obj_id);
	if (NULL != obj)
	{
		SNPRINTF(info_buf, sizeof(info_buf), "obj_id = %d, pos = (%d, %d), dir= %f, distance = %f", target_obj_id, obj->GetPos().x, obj->GetPos().y, obj->GetDir(), obj->GetDirDistance());
	}
	else
	{
		SNPRINTF(info_buf, sizeof(info_buf), "invalid object");
	}

	return MakeupResult(netid, command->type, true, info_buf);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Gm::SetVip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int vip_type = atoi(result[0]);
	
	role->GetVip()->SetVipLevel(vip_type);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::MoveToGuildStation(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	if (INVALID_GUILD_ID == role->GetGuildID()) 
	{
		return MakeupResult(netid, command->type, false, "");
	}

	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	
	int tar_scene_id = atoi(result[0]);
	int x = atoi(result[1]);
	int y = atoi(result[2]);

	Posi tar_pos(x, y);	
	bool ret = World::GetInstWorld()->GetSceneManager()->GoTo(role ,tar_scene_id, role->GetGuildID(), tar_pos);

	return MakeupResult(netid, command->type, ret, "");
}

bool Gm::OnGetGuildDoubleReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	
	role->GetGuildTask()->GetGuildDoubleReward();

	return MakeupResult(netid, command->type, true, ""); 
}

bool Gm::SetHp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	Attribute hp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);
	Attribute change_hp = atoll(result[0]) - hp;

	role->ChangeHp(&change_hp, true, CharIntAttrs::ATTR_NOTIFY_REASON_NORMAL, true);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetMp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	
	Attribute mp = role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MP);
	Attribute change_mp = atoll(result[0]) - mp;

	role->ChangeMp(&change_mp, true);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	
	int level = atoi(result[0]);
	role->GMSetLevel(level);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TurnTable(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);

	role->GetTurnTable()->OnRoll(type);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ShopBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	
	Protocol::CSShopBuy bs;
	bs.item_id = static_cast<unsigned short>(atoi(result[0]));
	bs.item_num = atoi(result[1]);
	bs.if_use = atoi(result[3]) > 0 ? true : false;
	bs.is_bind = 0;
	
	LOGIC_CONFIG->GetShop().OnBuyItemReq(role, &bs);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddTitle(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int title_id = atoi(result[0]);
	role->GetTitleManager()->AddTitle(title_id, true, true, true, true);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::UseTitle(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	UNSTD_STATIC_CHECK(MAX_USE_TITLE_COUNT >= 3);

	unsigned short title_id_list[MAX_USE_TITLE_COUNT]; memset(title_id_list, 0, sizeof(title_id_list));

	title_id_list[0] = static_cast<unsigned short>(atoi(result[0]));
	title_id_list[1] = static_cast<unsigned short>(atoi(result[1]));;
	title_id_list[2] = static_cast<unsigned short>(atoi(result[2]));;

	role->GetTitleManager()->UseTitleList(title_id_list);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GetTitleOwnerInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetRoleActivity()->OnGetTitleOwnerInfo();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddGuildExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int value = atoi(result[1]);

	Protocol::CSAddGuildExp age;
	age.type = type;
	age.value = value;
	GuildRoute::Instance().GuildAddGuildExp(role, &age);

	return MakeupResult(netid, command->type, true, ""); 
}

bool Gm::AddGuildTotemExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int value = atoi(result[0]);

	if (role->GetGuildID() > 0)
	{
		Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
		if (NULL != guild)
		{
			guild->AddTotemExp(value);
		}
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ActivityToNextState(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " "); 

	int activity_type = atoi(result[0]);

	ActivityManager::Instance().OnForceToNextState(activity_type);	
	RandActivityManager::Instance().OnForceToNextState(activity_type);
	ActivityShadow::Instance().CrossActivityForceNextState(activity_type);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ForceToOpenActivity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " "); 

	int activity_type = atoi(result[0]);

	if (!ActivityManager::Instance().IsActivityOpen(activity_type))
	{
		ActivityManager::Instance().OnForceToNextState(activity_type);
	}

	if (!ActivityShadow::Instance().IsRandActivityOpen(activity_type))
	{
		RandActivityManager::Instance().OnForceToNextState(activity_type);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ForceToCloseActivity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " "); 

	int activity_type = atoi(result[0]);

	if (!ActivityManager::Instance().IsActivityClose(activity_type))
	{
		ActivityManager::Instance().OnForceToNextState(activity_type);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddActiveDegree(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int activedegree_type = atoi(result[0]);

	role->GetActiveDegree()->AddActiveDegreeHelper(activedegree_type, 1);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddMail(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int shengwang = atoi(result[0]);

	static MailContentParam cont_param; cont_param.Reset();
	
	cont_param.AddVirtualItem(0, MAIL_VIRTUAL_ITEM_BATTLEFIELDHONOR, shengwang);

	MailRoute::Instance().MailToUser(role->GetUserId(), 0, cont_param);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CleanMail(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	MailRoute::Instance().CleanMail(role);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddRMB(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int rmb = atoi(result[0]);
	if (!EventHandler::Instance().OnRMBBuy(role, rmb))
	{
		EventHandler::Instance().OnAddChongzhi(role, rmb, false);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddChongzhi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	
	int chongzhi = atoi(result[0]);

	role->GetKnapsack()->GetMoney()->AddGold(chongzhi, "GMaddchongzhi");

	EventHandler::Instance().OnAddChongzhi(role, chongzhi);
	role->GetRoleActivity()->CheckChongzhiExtraRewardByNum(static_cast<int>(chongzhi));

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::FetchChongzhiReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int param = atoi(result[1]);
	int param2 = atoi(result[2]);

	if (Protocol::CHONGZHI_REWARD_TYPE_SPECIAL_FIRST == type)
	{
		role->GetRoleActivity()->FetchSpecialFirstChongzhiReward(param);
	}
	else if (Protocol::CHONGZHI_REWARD_TYPE_DAILY_FIRST == type)
	{
		role->GetRoleActivity()->FetchDailyFirstChongzhiReward();
	}
	else if (Protocol::CHONGZHI_REWARD_TYPE_DAILY_TOTAL == type)
	{
		role->GetRoleActivity()->FetchDailyTotalChongzhiReward(param);
	}
	else if (Protocol::CHONGZHI_REWARD_TYPE_FIRST == type)
	{
		role->GetRoleActivity()->FetchFirstChongzhiReward(role->GetLevel(), param);
	}
	else if (Protocol::CHONGZHI_REWARD_TYPE_DAILY == type)
	{
		role->GetRoleActivity()->FetchDailyChongzhiReward(param, param2);
	}
	else if (Protocol::CHONGZHI_REWARD_TYPE_DAILY_TIMES == type)
	{
		role->GetRoleActivity()->FetchDailyChongzhiTimesReward(param);
	}
	else if (Protocol::CHONGZHI_REWARD_TYPE_DAILY2 == type)
	{
		role->GetRoleActivity()->FetchDailyChongzhiReward2(param);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddMonster(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	UInt16 monster_id = atoi(result[0]);
	int count = atoi(result[1]);

	for (int i = 0; i < count && i < 100; i++)
	{
		MONSTERPOOL->CreateMonster(monster_id, scene, role->GetPos());
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddMyMonster(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	UInt16 monster_id = atoi(result[0]);

	Monster *monster = MONSTERPOOL->CreateMonster(monster_id, scene, role->GetPos());
	if (NULL != monster)
	{
		monster->SetOwnerUserId(role->GetUserId());

		monster->ForceSetEnemy(role->GetId());
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::DelMonster(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");	

	UInt16 monster_id = atoi(result[0]);
	UInt32 monster_num = scene->MonsterNum();
	for (UInt32 i = 0; i < monster_num; i++)
	{
		Monster *monster = scene->GetMonsterByIndex(i);
		if (NULL != monster && Obj::OBJ_TYPE_MONSTER == monster->GetObjType() && monster_id == monster->GetMonsterId())
		{
			EffectBase* effect = new EffectOnce(role->GetId(), 0, -1, EffectBase::FIGHT_TYPE_NORMAL);
			monster->AddEffect(effect);
			if (monster->IsAlive())
			{
				monster->ForceSetDead(role);
			}
		}
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::DelAllMonster(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");	
	UInt32 monster_num = scene->MonsterNum();
	for (UInt32 i = 0; i < monster_num; i++)
	{
		Monster *monster = scene->GetMonsterByIndex(i);
		if (NULL != monster && Obj::OBJ_TYPE_MONSTER == monster->GetObjType())
		{
			EffectBase* effect = new EffectOnce(role->GetId(), 0, -1, EffectBase::FIGHT_TYPE_NORMAL);
			monster->AddEffect(effect);
			if (monster->IsAlive())
			{
				monster->ForceSetDead(role);
			}
		}
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CallAndDrop(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	UInt16 monster_id = atoi(result[0]);
	UInt16 count = atoi(result[1]);
	int is_clean = atoi(result[2]);

	if (is_clean != 0)
	{
		role->GetKnapsack()->ClearAll();
	}

	if (count > 1000)
	{
		count = 1000;
	}

	for (int i = 0; i < count; i++)
	{
		Monster *monster = MONSTERPOOL->CreateMonster(monster_id, scene, role->GetPos());
		if (NULL != monster)
		{
			EffectBase* effect = new EffectOnce(role->GetId(), 0, -1, EffectBase::FIGHT_TYPE_NORMAL);
			monster->AddEffect(effect);
			monster->ForceSetDead(role);
		}
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddDay(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int pass_days = atoi(result[0]);

	if (pass_days > 10)
	{
		pass_days = 10;
	}

	for (int i = 0; i < pass_days; i++)
	{
		role->OnDayChange(0, 1);
	}

	role->GetTouzijihua()->GmAddDay(pass_days);

	if (role->GetRoleActivity()->GetParam()->chongzhi_7day_reward_timestamp > 0)
	{
		role->GetRoleActivity()->GetParam()->chongzhi_7day_reward_timestamp -= 24 * 3600;
		role->GetRoleActivity()->SendChongZhiInfo();
	}

	ChallengeUser *challenge_user = ChallengeField::Instance().GetChallengeUser(role->GetRoleCross()->GetCrossOriginalUniqueUserID());
	if (NULL != challenge_user)
	{
		challenge_user->OnDayChange(0, 1);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddGuildDay(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int pass_days = atoi(result[0]);

	if (pass_days > 10)
	{
		pass_days = 10;
	}

	for (int i = 0; i < pass_days; i++)
	{
		const std::map<GuildID, Guild*> &guild_map = GuildManager::Instance().GetGuildMap();
		std::map<GuildID, Guild*>::const_iterator iter = guild_map.begin();
		for (; iter != guild_map.end(); iter++)
		{
			if (NULL != iter->second)
			{
				iter->second->OnDayChange();
			}
		}
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddWorldDay(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int pass_days = atoi(result[0]);

	if (pass_days > 10)
	{
		pass_days = 10;
	}

	for (int i = 0; i < pass_days; i++)
	{
		World::GetInstWorld()->OnDayChange(0, 1);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ResetData(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int reset_times = atoi(result[0]);

	if (reset_times > 10)
	{
		reset_times = 10;
	}

	for (int i = 0; i < reset_times; i++)
	{
		role->OnResetModuleData(0, 1);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::WorldDayChange(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	World::GetInstWorld()->OnDayChange(1, 2);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddEvil(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int evil = atoi(result[0]);

	role->AddEvil(evil);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RefreshWorldBossNow(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	WorldBossManager::Instance().GMRefreshNow();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RefreshFamilyBossNow(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	BossFamilyManager::Instance().GMRefreshFamilyBossNow();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RefreshMikuBossNow(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	BossFamilyManager::Instance().GMRefreshMikuBossNow();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ResetMikuInterval(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	BossFamilyManager::Instance().RecalcMikuFlushTime();
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ReduceFBDayCount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int fb_id = atoi(result[0]);
	role->GetDayCounter()->DayCountReduce(fb_id);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ResetDayCount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	
	role->GetDayCounter()->GMResetDayCount();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetDayCount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int daycount_id = atoi(result[0]);
	int count = atoi(result[1]);
	role->GetDayCounter()->DayCountSet(daycount_id, count);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::QuerySceneInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int obj_num = 0, role_num = 0, monster_num = 0, pet_num = 0, dorp_item_num = 0;
	scene->GetSceneStatus(&obj_num, &role_num, &monster_num, &pet_num, &dorp_item_num);

	static char info_buf[256] = {0};
	SNPRINTF(info_buf, sizeof(info_buf), "obj_num = %d, role_num = %d, monster_num = %d, dorp_item_num = %d", obj_num, role_num, monster_num, dorp_item_num);

	return MakeupResult(netid, command->type, true, info_buf);
}

bool Gm::SetMoveMode(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int move_mode = atoi(result[0]);
	char move_mode_param = (char)atoi(result[1]);
	role->SetMoveMode(move_mode, Posi(0, 0), move_mode_param);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TestCore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	role = NULL;
	role->ReCalcAttr();
	return true;
}

bool Gm::ReloadConfig(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	
	int reload_type = atoi(result[0]);

	ReloadConfigManager::Instance().ReloadConfig(reload_type, 0, 0);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ReloadOneItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int param1 = atoi(result[0]);
	int param2 = atoi(result[1]);

	ReloadConfigManager::Instance().ReloadConfig(RELOAD_CONFIG_TYPE_ONE_ITEM, param1, param2);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::FetchDailyFind(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
 	char *result[128] = {};
 	this->ParseCommandParam(command->command, result, " ");
 
 	int dailyfind_type = atoi(result[0]);
 	int get_type = atoi(result[1]);
 
 	role->GetWelfare()->GetDailyFind(dailyfind_type, get_type);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetAuthority(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);

	bool ret = role->SetAuthority((char)type);

	return MakeupResult(netid, command->type, ret, "");
}

bool Gm::SetCount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int count = atoi(result[1]);

	bool ret = CmdHandler::Instance().OnCmdToRoleCount(role, type, count, false);

	return MakeupResult(netid, command->type, ret, "");
}

bool Gm::SetFbCount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int day_count_id = atoi(result[0]);
	int count = atoi(result[1]);

	bool ret = CmdHandler::Instance().OnCmdToRoleFbCount(role, day_count_id, count, false);

	return MakeupResult(netid, command->type, ret, "");
}

bool Gm::YiJianTiSheng(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetMentality()->YijianTishengMentality();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TrainMentality(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetMentality()->TrainMentality();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SpeedUp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetMentality()->ClearMentalityCD();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::UpgradeGengu(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");	int type = atoi(result[0]);
	bool use_protect_item =  atoi(result[1]) == 0 ? false : true;
	bool is_auto_buy = atoi(result[2]) == 0 ? false : true;

	role->GetMentality()->UpgradeMentalityGengu(type, is_auto_buy, use_protect_item);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::UpgradeGenguTest(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int start_level = atoi(result[0]);
	int end_level = atoi(result[1]);
	int count = atoi(result[2]);

	if (start_level < 0) start_level = 0;
	if (end_level > MAX_MENTALITY_GENGU_LEVEL) end_level = MAX_MENTALITY_GENGU_LEVEL;

	if (start_level >= end_level)
	{
		return true;
	}

	int run_count = 0;
	double total_coin = 0;
	double total_gold = 0;
	int round = 0;

	for (int k = 0; k < count && k < 20000; ++k)
	{
		bool ok = false;

		role->GetMentality()->GMResetGengu(MENTALITY_TYPE_GONGJI, start_level);

		for (int i = 0; i < MAX_MENTALITY_GENGU_LEVEL * 10000; ++i)
		{
			int coin_cost = 0, gold_cost = 0;

			int ret = role->GetMentality()->GMUpgradeGenguTest(MENTALITY_TYPE_GONGJI, end_level, &coin_cost, &gold_cost);
			if (ret < 0)
			{
				return true;
			}

			total_coin += coin_cost; 
			total_gold += gold_cost; 
			++ run_count;		

			if (ret > 0)
			{
				ok = true;
				break;
			}	
		}

		if (ok)
		{
			++ round;
		}
	}

	gamelog::g_log_strengthen_test.buff_printf("\n // UPGRADE GENGU ROUND = %d FROM LV %d TO %d", (int)round, start_level, end_level);

	if (round > 0)
	{
		float count = (float)run_count / (float)round;
		int coin = static_cast<int>((double)total_coin / (double)round);
		int gold = static_cast<int>((double)total_gold / (double)round);

		gamelog::g_log_strengthen_test.buff_printf("\n    %.2f %8d %4d", (float)count, (int)coin, (int)gold);
	}

	gamelog::g_log_strengthen_test.commit_buff(LL_INFO);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ResetName(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	GameName new_name;
	STRNCPY(new_name, result[0], sizeof(GameName));						// 这里不允许使用F_STRNCPY

	role->OnResetNameReq(new_name);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GuildResetName(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	static Protocol::CSGuildResetName grn;
	grn.guild_id = role->GetGuildID();
	STRNCPY(grn.new_name, result[0], sizeof(grn.new_name));

	GuildRoute::Instance().GuildResetName(role, &grn);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetSex(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	char sex = static_cast<char>(atoi(result[0]));
	role->SetSex(sex);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddXianhun(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int chg = atoi(result[0]);
	role->AddXianhun(chg, "gm");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddShengwang(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int chg = atoi(result[0]);
	role->AddShengwang(chg, "gm");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddGongXun(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int chg = atoi(result[0]);
	role->AddGongxun(chg, "gm");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddNvWaShi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int chg = atoi(result[0]);
	role->AddNvWaShi(chg, "gm");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddLingJing(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int chg = atoi(result[0]);
	role->AddLingJing(chg, "gm");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddChengJiu(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int chg = atoi(result[0]);
	role->AddChengJiu(chg, "gm");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddHunLi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int chg = atoi(result[0]);
	role->AddHunli(chg, "gm");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddHuoLi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int chg = atoi(result[0]);
	role->AddHuoli(chg, "gm");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddChestShopScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int chg = atoi(result[0]);
	role->GetRoleShop()->AddChestShopMojing(chg, "GM");
	role->GetRoleShop()->AddChestShopShengwang(chg, "GM");
	role->GetRoleShop()->AddChestShopGongXun(chg, "GM");
	role->GetRoleShop()->AddChestShopWeiWang(chg, "GM");
	role->GetRoleShop()->AddChestShopTreasureCredit(chg, "GM");
	role->GetRoleShop()->AddChestShopTreasureCredit1(chg, "GM");
	role->GetRoleShop()->AddChestShopTreasureCredit2(chg, "GM");
	role->GetRoleShop()->AddChestShopTreasureCredit3(chg, "GM");
	role->GetRoleShop()->AddChestShopJingLingCredit(chg, "GM");
	role->GetRoleShop()->AddChestShopHappyTreeGrow(chg, "GM");
	role->GetRoleShop()->AddChestShopMysteriousShopCredit(chg, "GM");
	role->GetRoleShop()->AddChestShopHunyinLingzhi(chg, chg, chg, "GM");
	role->GetRoleShop()->AddChestShopGuanghui(chg, "GM");
	role->GetRoleShop()->AddChestShopHunjing(chg, "GM");
	role->GetRoleShop()->AddCrossEquipChuanshiScore(chg, "GM");

	return MakeupResult(netid, command->type, true, "");
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Gm::AddMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	MountFunction::ActiveMount(role);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddFightMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	FightMountFunction::ActiveFightMount(role);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetMountGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int grade = atoi(result[0]);

	MountParam *param = role->GetMountManager()->GetMountParam();
	if (NULL != param && param->IsMountActived())
	{
		const MountGradeConfig *grade_cfg = LOGIC_CONFIG->GetMountOpConfig().GetGradeConfig(grade);
		if (NULL == grade_cfg)
		{
			return MakeupResult(netid, command->type, false, "");
		}

		memset(param->active_image_flag, 0, sizeof(param->active_image_flag));

		for (int i = 0; i <= grade_cfg->active_image_id; i++)
		{
			role->GetMountManager()->ActiveImage(i, false);
		}

		param->grade = grade;
		param->grade_bless_val = 0;
		param->star_level = (param->grade - 1) * 10;
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_MOUNT);
		role->GetMountManager()->SendMountInfo();
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetWingGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int grade = atoi(result[0]);

	WingParam *param = role->GetWing()->GetWingParam();
	if (NULL != param && param->IsWingActived())
	{
		const WingGradeConfig *grade_cfg = LOGIC_CONFIG->GetWingConfig().GetGradeConfig(grade);
		if (NULL == grade_cfg)
		{
			return MakeupResult(netid, command->type, false, "");
		}

		memset(param->active_image_flag, 0, sizeof(param->active_image_flag));

		for (int i = 0; i <= grade_cfg->active_image_id; i++)
		{
			role->GetWing()->ActiveImage(i, false);
		}

		param->grade = grade;
		param->grade_bless_val = 0;
		param->star_level = (param->grade - 1) * 10;
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_WING);
		role->GetWing()->SendWingInfo();
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetHaloGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int grade = atoi(result[0]);

	HaloParam *param = role->GetHalo()->GetHaloParam();
	if (NULL != param && param->IsHaloActived())
	{
		const HaloGradeConfig *grade_cfg = LOGIC_CONFIG->GetHaloConfig().GetGradeConfig(grade);
		if (NULL == grade_cfg)
		{
			return MakeupResult(netid, command->type, false, "");
		}

		memset(param->active_image_flag, 0, sizeof(param->active_image_flag));

		for (int i = 0; i <= grade_cfg->active_image_id; i++)
		{
			role->GetHalo()->ActiveImage(i, false);
		}

		param->grade = grade;
		param->grade_bless_val = 0;
		param->star_level = (param->grade - 1) * 10;
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_HALO);
		role->GetHalo()->SendHaloInfo();
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetShengongGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int grade = atoi(result[0]);

	ShengongParam *param = role->GetShengong()->GetShengongParam();
	if (NULL != param && param->IsShengongActived())
	{
		const ShengongGradeConfig *grade_cfg = LOGIC_CONFIG->GetShengongConfig().GetGradeConfig(grade);
		if (NULL == grade_cfg)
		{
			return MakeupResult(netid, command->type, false, "");
		}

		memset(param->active_image_flag, 0, sizeof(param->active_image_flag));

		for (int i = 0; i <= grade_cfg->active_image_id; i++)
		{
			role->GetShengong()->ActiveImage(i, false);
		}

		param->grade = grade;
		param->grade_bless_val = 0;
		param->star_level = (param->grade - 1) * 10;
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENGONG);
		role->GetShengong()->SendShengongInfo();
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetShenyiGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int grade = atoi(result[0]);

	ShenyiParam *param = role->GetShenyi()->GetShenyiParam();
	if (NULL != param && param->IsShenyiActived())
	{
		const ShenyiGradeConfig *grade_cfg = LOGIC_CONFIG->GetShenyiConfig().GetGradeConfig(grade);
		if (NULL == grade_cfg)
		{
			return MakeupResult(netid, command->type, false, "");
		}

		memset(param->active_image_flag, 0, sizeof(param->active_image_flag));

		for (int i = 0; i <= grade_cfg->active_image_id; i++)
		{
			role->GetShenyi()->ActiveImage(i, false);
		}

		param->grade = grade;
		param->grade_bless_val = 0;
		param->star_level = (param->grade - 1) * 10;
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHENYI);
		role->GetShenyi()->SendShenyiInfo();
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetFightMountGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int grade = atoi(result[0]);

	FightMountParam *param = role->GetFightMountManager()->GetFightMountParam();
	if (NULL != param)
	{
		const FightMountGradeConfig *grade_cfg = LOGIC_CONFIG->GetFightMountOpConfig().GetGradeConfig(grade);
		if (NULL == grade_cfg)
		{
			return MakeupResult(netid, command->type, false, "");
		}

		memset(param->active_image_flag, 0, sizeof(param->active_image_flag));

		for (int i = 0; i <= grade_cfg->active_image_id; i++)
		{
			role->GetFightMountManager()->ActiveImage(i, false);
		}

		param->grade = grade;
		param->grade_bless_val = 0;
		param->star_level = (param->grade - 1) * 10;
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FIGHT_MOUNT);
		role->GetFightMountManager()->SendFightMountInfo();
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetFootprintGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int grade = atoi(result[0]);

	FootprintParam *param = role->GetFootPrint()->GetFootprintParam();
	if (NULL != param && param->IsFootprintActived())
	{
		const FootprintGradeConfig *grade_cfg = LOGIC_CONFIG->GetFootPrintConfig().GetGradeConfig(grade);
		if (NULL == grade_cfg)
		{
			return MakeupResult(netid, command->type, false, "");
		}

		memset(param->active_image_flag, 0, sizeof(param->active_image_flag));

		for (int i = 0; i <= grade_cfg->active_image_id; i++)
		{
			role->GetFootPrint()->ActiveImage(i, false);
		}
		param->grade = grade;
		param->grade_bless_val = 0;
		param->star_level = (param->grade - 1) * 10;
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_FOOTPRINT);
		role->GetFootPrint()->SendFootprintInfo();
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetShizhuangGrade(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int grade = atoi(result[0]);
	int shizhuang_type = SHIZHUANG_TYPE_BODY;
	ShizhuangItem *shizhuang_item = role->GetShizhuang()->GetParam()->GetShizhuangItem(shizhuang_type);
	if (NULL != shizhuang_item && shizhuang_item->grade > 0)
	{
		const ShizhuangGradeConfig *grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetGradeConfigByType(grade, shizhuang_type);
		if (NULL == grade_cfg)
		{
			return MakeupResult(netid, command->type, false, "");
		}
		int old_grade = shizhuang_item->grade;
		shizhuang_item->grade = 0;
		memset(shizhuang_item->img_active_flag, 0, sizeof(shizhuang_item->img_active_flag));
		role->GetShizhuang()->ActiveShiZhuang(shizhuang_type);
		for (int i = 0; i <= grade_cfg->active_image_id; i++)
		{
			BitmapFunction::SetBit(shizhuang_item->img_active_flag, sizeof(shizhuang_item->img_active_flag), i);
		}
		shizhuang_item->grade = grade;
		shizhuang_item->grade_bless = 0;
		role->GetShizhuang()->UseMaxGradeImg(shizhuang_type);
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHIZHUANG);
		EventHandler::Instance().OnShiZhuangUpgrade(role, old_grade, shizhuang_item->grade);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetShenbingGrade(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int grade = atoi(result[0]);
	int shizhuang_type = SHIZHUANG_TYPE_WUQI;
	ShizhuangItem *shizhuang_item = role->GetShizhuang()->GetParam()->GetShizhuangItem(shizhuang_type);
	if (NULL != shizhuang_item && shizhuang_item->grade > 0)
	{
		const ShizhuangGradeConfig *grade_cfg = LOGIC_CONFIG->GetShizhuangConfig().GetGradeConfigByType(grade, shizhuang_type);
		if (NULL == grade_cfg)
		{
			return MakeupResult(netid, command->type, false, "");
		}
		int old_grade = shizhuang_item->grade;
		shizhuang_item->grade = 0;
		memset(shizhuang_item->img_active_flag, 0, sizeof(shizhuang_item->img_active_flag));
		role->GetShizhuang()->ActiveShiZhuang(shizhuang_type);
		for (int i = 0; i <= grade_cfg->active_image_id; i++)
		{
			BitmapFunction::SetBit(shizhuang_item->img_active_flag, sizeof(shizhuang_item->img_active_flag), i);
		}
		shizhuang_item->grade = grade;
		shizhuang_item->grade_bless = 0;
		role->GetShizhuang()->UseMaxGradeImg(shizhuang_type);
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_SHIZHUANG);
		EventHandler::Instance().OnShenBingUpgrade(role, old_grade, shizhuang_item->grade);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetFabaoGrade(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	return false;
}

bool Gm::SetQilinbiGrade(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	return false;
}

bool Gm::SetYaoshiGrade(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	return false;
}

bool Gm::SetMaskGrade(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	return false;
}

bool Gm::SetToushiGrade(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	return false;
}

bool Gm::SetUpgradeGrade(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	return false;
}

bool Gm::SetCloakLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int level = atoi(result[0]);

	CloakParam *param = role->GetCloakManager()->GetCloakParam();
	if (NULL != param)
	{
		param->level = level;
		role->ReCalcAttr(CharIntAttrs::RECALC_REASON_CLOAK);
		role->GetCloakManager()->SendCloakInfo();
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetUpgradeSystemGrade(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	return false;
}

bool Gm::UpgradeMountTest(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int count = atoi(result[1]);

	struct UpgradeTestLevelInfo
	{
		int count;
		double coin;
		double gold;
		int round;
	};

	const int MAX_GRADE = 30;

	static UpgradeTestLevelInfo grade_info_list[MAX_GRADE];
	memset(grade_info_list, 0, sizeof(grade_info_list));

	int grade_info_count = 0;

	for (int k = 0; k < count && k < 20000; ++k)
	{
		int max_grade = 0; 

		for (int i = 0; i < MAX_GRADE * 1000; ++i)
		{
			int old_grade = 0, coin_cost = 0, gold_cost = 0;

			if (!MountFunction::Upgrade(role, true, 1, &old_grade, &coin_cost, &gold_cost))
			{
				break;
			}

			if (old_grade >= 1 && old_grade < MAX_GRADE)
			{
				grade_info_list[old_grade].coin += coin_cost;
				grade_info_list[old_grade].gold += gold_cost;
				++grade_info_list[old_grade].count;
				max_grade = old_grade;
			}
		}

		if (max_grade > 0)
		{
			++ grade_info_count;
			for (int i = 1; i <= max_grade && i < MAX_GRADE; ++i)
			{
				++ grade_info_list[i].round;
			}

			if (!MountFunction::GMResetGrade(role))
			{
				break;
			}

			role->GetKnapsack()->GetMoney()->AddGold(9999999, "testmountupgrade");
		}
	}

	if (grade_info_count > 0)
	{
		gamelog::g_log_strengthen_test.buff_printf("\n // MOUNT UPGRADE ROUND = %d", (int)grade_info_count);

		for (int i = 1; i < MAX_GRADE; ++i)
		{
			if (grade_info_list[i].round > 0)
			{
				float count = (float)grade_info_list[i].count / (float)grade_info_list[i].round;
				int coin = static_cast<int>((double)grade_info_list[i].coin / (double)grade_info_list[i].round);
				int gold = static_cast<int>((double)grade_info_list[i].gold / (double)grade_info_list[i].round);

				gamelog::g_log_strengthen_test.buff_printf("\n    %2d %.2f %8d %4d", 
					(int)i, (float)count, (int)coin, (int)gold);
			}
		}

		gamelog::g_log_strengthen_test.commit_buff(LL_INFO);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::UpgradeMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int auto_buy = atoi(result[1]);

	MountFunction::Upgrade(role, 1 == auto_buy ? true: false);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ActiveMountImg(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int imgid = (atoi(result[0]));

	role->GetMountManager()->ActiveImage(imgid);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GoonMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int is_on = (atoi(result[0]));

	role->GetMountManager()->GoonMount(0 != is_on);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::Strengthen(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	bool is_puton = atoi(result[0]) == 1 ? true: false;
	short equip_idx = atoi(result[1]);
	bool is_auto_buy = atoi(result[2]) == 1 ? true: false;
	bool select_bind_first = atoi(result[3]) == 1? true: false;
	bool use_lucky_stuff = atoi(result[4]) == 1? true: false;

	EquipFunction::EquipStrengthen(role, is_puton, select_bind_first, is_auto_buy, equip_idx, use_lucky_stuff, 0);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetEquipStrengthLvl(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	short equip_idx = atoi(result[0]);
	int level = atoi(result[1]);

	EquipFunction::GMResetStrengthenLevel(role, true, equip_idx, level, true);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::StrengthenTest(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	short equip_idx = atoi(result[0]);
	int count = atoi(result[1]);

	struct StrengthTestLevelInfo
	{
		int count;
		double coin;
		double gold;
		int round;
	};

	static StrengthTestLevelInfo level_info_list[EquipConfig::MAX_STRENGTH_LEVEL];
	memset(level_info_list, 0, sizeof(level_info_list));

	int level_info_count = 0;

	for (int k = 0; k < count && k < 20000; ++k)
	{
		int max_level = 0;

		for (int i = 0; i < EquipConfig::MAX_STRENGTH_LEVEL * 100; ++i)
		{
			int old_level = 0, coin_cost = 0, gold_cost = 0;
			if (!EquipFunction::EquipStrengthen(role, false, false, true, equip_idx, false, 0, &old_level, &coin_cost, &gold_cost))
			{
				break;
			}

			if (old_level >= 0 && old_level < EquipConfig::MAX_STRENGTH_LEVEL)
			{
				level_info_list[old_level].coin += coin_cost;
				level_info_list[old_level].gold += gold_cost;
				++level_info_list[old_level].count;
				max_level = old_level;
			}
		}

		if (max_level > 0)
		{
			++ level_info_count;
			for (int i = 0; i <= max_level && i < EquipConfig::MAX_STRENGTH_LEVEL; ++i)
			{
				++ level_info_list[i].round;
			}

			if (!EquipFunction::GMResetStrengthenLevel(role, false, equip_idx))
			{
				break;
			}

			role->GetKnapsack()->GetMoney()->AddGold(9999999, "StrengthenTest");
		}
	}

	if (level_info_count > 0)
	{
		gamelog::g_log_strengthen_test.buff_printf("\n // EQUIPMENT STRENGTHEN ROUND = %d", (int)level_info_count);

		for (int i = 0; i < EquipConfig::MAX_STRENGTH_LEVEL; ++i)
		{
			if (level_info_list[i].round > 0)
			{
				float count = (float)level_info_list[i].count / (float)level_info_list[i].round;
				int coin = static_cast<int>((double)level_info_list[i].coin / (double)level_info_list[i].round);
				int gold = static_cast<int>((double)level_info_list[i].gold / (double)level_info_list[i].round);

				gamelog::g_log_strengthen_test.buff_printf("\n    %2d %.2f %8d %4d", (int)i, (float)count, (int)coin, (int)gold);
			}
		}

		gamelog::g_log_strengthen_test.commit_buff(LL_INFO);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::UpQuality(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	bool is_puton = atoi(result[0]) == 1 ? true: false;
	short equip_idx = atoi(result[1]);
	bool select_bind_first = atoi(result[2]) == 1? true: false;

	EquipFunction::EquipUpQuality(role, is_puton, select_bind_first, equip_idx);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::UpLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
// 	char *result[128] = {};
// 	this->ParseCommandParam(command->command, result, " ");
// 
// 	bool is_puton = atoi(result[0]) == 1 ? true: false;
// 	short equip_idx = atoi(result[1]);
// 	bool select_bind_first = atoi(result[2]) == 1? true: false;
// 	bool use_protect_stuff = atoi(result[3]) == 1 ? true: false;
// 
// 	EquipFunction::EquipUpLevel(role, is_puton, select_bind_first, equip_idx, use_protect_stuff);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::UpEternity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	short equip_idx = atoi(result[0]);
	bool is_auto_buy = atoi(result[1]) == 1 ? true : false;

	EquipFunction::EquipUpEternity(role, equip_idx, is_auto_buy);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ForceCompleteTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int task_id = atoi(result[0]);
	role->GetTaskManager()->ForceCompleteTask(task_id);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AcceptCishaTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetCishaTask()->AcceptTask();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GetTuMoTaskInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetDailyTask()->SendTuMoTaskInfo(0);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CommitTuMoTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int commit_all_task = atoi(result[0]);
	TaskID task_id = static_cast<TaskID>(atoi(result[1]));

	role->GetDailyTask()->OnCommitTask((0 != commit_all_task), task_id);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::FetchTumoReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetDailyTask()->FetchCompleteAllTaskReward();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ImediatelyCompleteTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ReflushHuSongTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	Protocol::CSRefreshHusongTask rhst;
	rhst.is_autoflush = atoi(result[0]);
	rhst.to_color = atoi(result[1]);

	role->GetHusongTask()->OnRefreshTaskColor(&rhst);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::HusongBuyTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetHusongTask()->OnBuyTimes();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::FBEquipBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int shop_item_seq = atoi(result[0]);
	bool is_personal = atoi(result[1]) == 1 ? true : false;

	if (is_personal)
	{
		role->GetFBEquip()->OnPersonalBuyMysteryItem(shop_item_seq);
	}
	else
	{
		role->GetFBEquip()->OnTeamBuyMysteryItem(shop_item_seq);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::NPCFunc(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ActivityEnterReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	Protocol::CSActivityEnterReq aer;
	aer.activity_type = atoi(result[0]);
	aer.room_index = atoi(result[1]);

	role->GetRoleActivity()->OnActivityEnterReq(&aer);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::EnterFB(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	
	int fb_type = atoi(result[0]);
	int param1 = atoi(result[1]);
	int param2 = atoi(result[2]);
	int param3 = atoi(result[3]);

	FBManager::Instance().EnterFB(role, fb_type, param1, param2, param3);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GetUid(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	char uid_str[32];
	SNPRINTF(uid_str, sizeof(uid_str), "%d", UidToInt(role->GetUserId()));

	return MakeupResult(netid, command->type, true, uid_str);
}

bool Gm::ConverShopBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	Protocol::CSScoreToItemConvert stic;

	stic.scoretoitem_type = atoi(result[0]);
	stic.index = atoi(result[1]);
	stic.num = atoi(result[2]);

	LOGIC_CONFIG->GetConvertShop().ConvertScoreToItem(role, stic);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SendGuildSos(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int sos_type = atoi(result[0]);

	role->GetRoleActivity()->OnSendGuildSosReq(sos_type);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SendSaleItemInfoToWorld(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	Protocol::CSPublicSaleSendItemInfoToWorld psbii;
	psbii.sale_index = atoi(result[0]);

	PublicSaleRoute::Instance().OnSendPublicSaleItemInfoToWorld(role, &psbii);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetCapability(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int capability = atoi(result[0]);

	role->GetCapability()->GmSetCapability(capability);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GetCapability(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int cap_type = atoi(result[0]);

	int capability = 0;
	if (cap_type < 0)
	{
		capability = role->GetCapability()->GetTotalCapability();
	}
	else
	{
		capability = role->GetCapability()->GetRoleCapability(cap_type);
	}

	char personal_msg[256]; memset(personal_msg, 0, sizeof(personal_msg));

	sprintf(personal_msg, "{showpos;2} cap_type[%d] = %d", cap_type, capability);
	personal_msg[sizeof(personal_msg) - 1] = '\0';
	
	int msg_len = (int)strlen(personal_msg);

	role->SendPersonalMsg(msg_len, personal_msg);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddHonor(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int honor_value = atoi(result[0]);

	role->GetRoleActivity()->AddBattleFieldHonor(honor_value, "GmAddHonor", true);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::BuyChestShopItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	
	//int mode = atoi(result[1]);

	//LOGIC_CONFIG->GetChestShop().Buy(role, mode);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TestChestShop(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int mode = atoi(result[1]);
	int times = atoi(result[2]);

	std::map<unsigned short, int> itemmap;

	for (int i = 0; i < times; ++i)
	{
		LOGIC_CONFIG->GetChestShop().GMBuyTest(role, type, mode, itemmap);
	}

	char chesttype[64] = {0};
	char chestmode[64] = {0};

	if (CHEST_SHOP_TYPE_EQUIP == type)
	{
		sprintf(chesttype, "%s", "Chuji");
	}
	else
	{
		sprintf(chesttype, "%s", "Gaoji");
	}

	if (CHEST_SHOP_MODE_1 == mode)
	{
		sprintf(chestmode, "%s", "MODE_1");
	}
	else if (CHEST_SHOP_MODE_10 == mode)
	{
		sprintf(chestmode, "%s", "MODE_10");
	}
	else
	{
		sprintf(chestmode, "%s", "MODE_50");
	}

	gamelog::g_log_strengthen_test.buff_printf("\n // CHEST TYPE = %s MODE = %s ROUND = %d", 
		chesttype, chestmode, times);

	for (std::map<unsigned short, int>::iterator iter = itemmap.begin(); iter != itemmap.end(); ++iter)
	{
		int item_id = iter->first;
		int count = iter->second;

		char itemname[128] = {0};

		const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
		if (NULL != itembase)
		{
			sprintf(itemname, "%s", itembase->GetItemName());
		}

		gamelog::g_log_strengthen_test.buff_printf("\n    %4d(%s)  %4d", item_id, itemname, count);
	}

	gamelog::g_log_strengthen_test.commit_buff(LL_INFO);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ClearTrunkTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int start_task_id = atoi(result[0]);
	bool is_clear_zhi_xian = (0 != atoi(result[1]));
	bool add_start_id = (0 != atoi(result[2]));

	role->GetTaskManager()->GMClearTrunkTask(start_task_id, is_clear_zhi_xian, add_start_id);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::XianMengZhanGuildCall(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int use_gold = atoi(result[0]);

	role->GetRoleActivity()->XianMengZhanGuildCall(0 != use_gold);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::XianMengZhanGuildFollow(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetRoleActivity()->XianMengZhanGuildFollow();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::Zhuanzhi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetRoleZhuanZhi()->GMFinishZhuanzhiTask();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::QueryOther(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int other_uid = atoi(result[0]);

	QueryHandler::Instance().OnQuery(other_uid, false, role->GetUID(), 0);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::QueryCrossOther(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int other_plat_type = atoi(result[0]);
	int other_uid = atoi(result[1]);

	static crossgameprotocal::GameCrossQueryRoleInfoRequest gcqrir;
	gcqrir.target_plat_type = other_plat_type;
	gcqrir.target_uid = other_uid;
	gcqrir.request_plat_type = LocalConfig::Instance().GetPlatType();
	gcqrir.request_uid = role->GetUID();
	gcqrir.request_type = crossgameprotocal::GameCrossQueryRoleInfoRequest::QUERY_ROLE_INFO_TYPE_ALL;
	//InternalComm::Instance().SendToCross((const char *)&gcqrir, sizeof(gcqrir));
	CrossManager::Instance().SendToCrossWithFuncType(CROSS_FUNCTION_TYPE_CROSS_RANK, (const char *)&gcqrir, sizeof(gcqrir));

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CombineServerToNext(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	unsigned begin_time = static_cast<unsigned int>(EngineAdapter::Instance().Time());
	unsigned end_time = begin_time + 10 * 24 * 60 * 60;
	
	SpecialActivityManager::Instance().OnForceSetSpecialActivityTime(SPECIAL_ACTIVITY_TYPE_COMBINE_SERVER, begin_time, end_time);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CombineServerReset(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CombineServerResetSelf(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddSysMailEvent(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int gold = atoi(result[6]);
	int bind_coin = atoi(result[5]);
	int bind_gold = atoi(result[4]);
	int virtual_item_num = atoi(result[3]);
	int virtual_item_type = atoi(result[2]);
	ItemID item_id = (ItemID)atoi(result[1]);
	int user_id = atoi(result[0]);

	if (0 == user_id)
	{
		user_id = role->GetUID();
	}

	{
		// 发邮件
		static MailContentParam reward_mail; reward_mail.Reset();

		if (virtual_item_type >= 0 && virtual_item_type < MAIL_VIRTUAL_ITEM_MAX && virtual_item_num > 0)
		{
			reward_mail.AddVirtualItem(0, virtual_item_type, virtual_item_num);
		}

		if (bind_gold > 0)
		{
			reward_mail.AddVirtualItem(1, MAIL_VIRTUAL_ITEM_BIND_GOLD, bind_gold);
		}

		if (gold > 0)
		{
			reward_mail.gold = gold;
		}

		if (bind_coin > 0)
		{
			reward_mail.coin_bind = bind_coin;
		}

		const ItemBase *itembase = ITEMPOOL->GetItem(item_id);
		if (NULL != itembase) 
		{
			reward_mail.item_list[0].item_id = item_id;
			reward_mail.item_list[0].num = 1;
			reward_mail.item_list[0].is_bind = false;
			reward_mail.item_list[0].invalid_time = itembase->CalInvalidTime();
		}

		int l1 = SNPRINTF(reward_mail.subject, sizeof(reward_mail.subject), "this is test system message");
		int l2 = SNPRINTF(reward_mail.contenttxt, sizeof(reward_mail.contenttxt), "test content ");
		if (l1 > 0 && l2 > 0)
		{
			MailRoute::MailToUser(IntToUid(user_id), SYSTEM_MAIL_REASON_INVALID, reward_mail);
		}
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddGuildBossExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	gglobalprotocal::GmAddGuildBossExp gagbe;
	gagbe.guild_id = role->GetGuildID();
	gagbe.uid = UidToInt(role->GetUserId());
	gagbe.add_exp = atoi(result[0]);

	InternalComm::Instance().NetSend(InternalComm::Instance().m_golbal_server_id, (const char*)&gagbe, sizeof(gagbe));

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TestLostHP(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int target_id = atoi(result[0]);
	Obj *chara = scene->GetObj(target_id);
	if (NULL == chara || Obj::OBJ_TYPE_MONSTER != chara->GetObjType()) return false;
	
	Monster *target_monster = (Monster *)chara;

	Attribute now_hp = target_monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);

	long long injure = (long long)(now_hp * 0.4);
	int fight_type = EffectBase::FIGHT_TYPE_NORMAL;

	EffectOnce *effect_first = new EffectOnce(role->GetId(), 207, -injure, fight_type);
	target_monster->AddEffect(effect_first);

	injure = (long long)(now_hp * 0.5);
	EffectOnce *effect_sec = new EffectOnce(role->GetId(), 207, -injure, fight_type);
	target_monster->AddEffect(effect_sec);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TestFight(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int monster_id = atoi(result[0]);
	int SKILL_LEVEL = atoi(result[1]);

	int prof = role->GetProf();

	{
		Attribute delta_gongji = 77792 - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI);
		role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI, delta_gongji);

		Attribute delta_mingzhong = 76290 - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG);
		role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG, delta_mingzhong);

		Attribute delta_baoji = 16828 - role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI);
		role->ChangeCharAttrHelper(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI, delta_baoji);
	}

	//const int SKILL_LEVEL = 1;

	struct  
	{
		int skill_id; int skill_level; long long injure; int perform_count;
	} skill_list[] =
	{
		{0, 0, 0, 0},
		{prof * 100 + 1, SKILL_LEVEL, 0, 0}, {prof * 100 + 2, SKILL_LEVEL, 0, 0}, {prof * 100 + 3, SKILL_LEVEL, 0, 0},
		{prof * 100 + 4, SKILL_LEVEL, 0, 0}, {prof * 100 + 5, SKILL_LEVEL, 0, 0}, {prof * 100 + 6, SKILL_LEVEL, 0, 0}
	};

	int index_list[] = {4,5,6, 1,2,3, 4,5,6, 1,2,3, 1,2,3, 4,5,6, 1,2,3, 1,2,3, 4,5,6, 1,2,3, 1,2,3};
	int index_count = sizeof(index_list) / sizeof(index_list[0]);

	int skill_count = sizeof(skill_list) / sizeof(skill_list[0]);

	Monster *monster = MONSTERPOOL->CreateMonster(monster_id, scene, role->GetPos());
	if (NULL == monster) return false;

	Attribute init_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);

	int skill_index = 0;

	while (NULL != monster && monster->IsAlive())
	{
		skill_index = skill_index % index_count;
		
		Attribute old_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);

		int seq = index_list[skill_index];
		const SkillBase *skill = SKILLPOOL->GetSkill(skill_list[seq].skill_id);
		if (NULL != skill)
		{
			skill->Perform(role, skill_list[seq].skill_level, monster, monster->GetPos());

			Attribute curr_hp = monster->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_HP);

			long long hp_lost = old_hp - curr_hp;
			if (hp_lost < 0)
			{
				printf("oh no hp_lost %lld < 0\n", hp_lost);
			}

			skill_list[seq].injure += hp_lost;
			skill_list[seq].perform_count += 1;
		}

		++ skill_index;
	}

	gamelog::g_log_strengthen_test.buff_printf("\n // Target Monster id = %d, hp = %lld, My Prof = %d, gongji = %lld, mingzhong = %lld, baoji = %lld", 
		monster_id, init_hp, prof, 
		role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_GONGJI),
		role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_MINGZHONG),
		role->GetCharaIntAttr(CharIntAttrs::FIGHT_CHARINTATTR_TYPE_BAOJI));

	long long hurt_sum = 0;
	int perform_sum = 0;

	for (int i = 0; i < skill_count; ++i)
	{
		if (skill_list[i].skill_id <= 0) continue;

		gamelog::g_log_strengthen_test.buff_printf("\n  skill(%d, %d) hurt = %lld, perform_times = %d", 
			skill_list[i].skill_id, skill_list[i].skill_level, skill_list[i].injure, skill_list[i].perform_count);

		hurt_sum += skill_list[i].injure;
		perform_sum += skill_list[i].perform_count;
	}

	gamelog::g_log_strengthen_test.buff_printf("\n  total hurt = %lld, perform:%d", hurt_sum, perform_sum); 
	
	gamelog::g_log_strengthen_test.commit_buff(LL_INFO);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetWorldLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int world_level = atoi(result[0]);

	WorldShadow::Instance().SetGMWorldLevel(world_level);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::Standard(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int my_prof = role->GetProf();
	bool auto_prof = false;

	if (my_prof < PROF_TYPE_PROF_1 || my_prof > PROF_TYPE_PROF_4)
	{
		my_prof = 1 + RandomNum(4);

		if (my_prof < PROF_TYPE_PROF_1 || my_prof > PROF_TYPE_PROF_4)
		{
			my_prof = PROF_TYPE_PROF_1;
		}

		role->SetProf((char)my_prof);

		auto_prof = true;
	}

	if (role->GetCamp() <= CAMP_TYPE_INVALID || role->GetCamp() >= PROF_TYPE_PROF_NO_LIMIT)
	{
		int camp = 1 + RandomNum(3);
		role->SetCamp(camp);
	}

	if (auto_prof)
	{
		int add_money = 9999999;
		role->GetKnapsack()->GetMoney()->Add(add_money, add_money, add_money, add_money, "Gm");

		role->GetKnapsack()->GMExtendAll();

		//int count = 0;
		int task_list[1000];
		memset(task_list, 0, sizeof(task_list));
		//count = 0;

		/*TaskID task_id = 1900;

		const Task *task = TASKPOOL->GetTask(task_id);
		while (NULL != task && task_id > 0)
		{
			if (count < 1000)
			{
				task_list[count++] = task_id;
			}
			else
			{
				break;
			}

			task_id = task->GetTaskLimit().m_pre_task;
			task = TASKPOOL->GetTask(task_id);
		}

		for (int i = count - 1; i >= 0; --i)
		{
			role->GetTaskManager()->GMAddTask(task_list[i]);
			role->GetTaskManager()->ForceCompleteTask(task_list[i]);
			role->GetTaskManager()->CommitTask(task_list[i]);
		}*/
	}

	// 等级
	int target_level = 55;

	role->GMSetLevel(target_level);

	//// 装备
	//int equip_id[PROF_TYPE_PROF_NO_LIMIT][Equipment::E_INDEX_WEDRING] =
	//{
	//	{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
	//	{105, 1105, 2105, 3105, 4105, 5105, 6105, 7105, 8105, 9105, 10105, 11105, 11105},
	//	{205, 1205, 2205, 3205, 4205, 5205, 6205, 7205, 8105, 9105, 10205, 11105, 11105},
	//	{305, 1305, 2305, 3305, 4305, 5305, 6305, 7305, 8105, 9105, 10305, 11105, 11105},
	//	{405, 1405, 2405, 3405, 4405, 5405, 6405, 7405, 8105, 9105, 10405, 11105, 11105}
	//};
	//
	//for (int i = 0; i < Equipment::E_INDEX_WEDRING; ++i)
	//{
	//	bool ret = role->GetEquipmentManager()->GMPuton(i, equip_id[my_prof][i]);
	//	if (!ret)
	//	{
	//		printf("GMPuton equip idx:%d id:%d fail\n", i, equip_id[my_prof][i]);
	//	}
	//}
	//role->GetEquipmentManager()->SendSuitInfo();

	// 技能
	for (int i = 0; i <= 10; ++i)
	{
		int skill_id = my_prof * 100 + i;
		int skill_level = 5;

		role->GetSkillManager()->AddSkill(skill_id, skill_level);
	}

	// 仙女
	role->GetMentality()->GMSetMentalityLevel(20);

	// 宠物
	

	// 坐骑
	role->GetMountManager()->ActiveImage(1);
	role->GetMountManager()->UseImage(1);

	role->GetMountManager()->SendMountInfo();

	role->ReCalcAttr();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TouZiJiHuaBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	role->GetTouzijihua()->OnNewBuy();
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TouZiJiHuaReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int plan_type = atoi(result[0]);
	int seq = atoi(result[1]);

	if (10 == plan_type)
	{
		role->GetTouzijihua()->OnNewFetchReward(false, seq);
	}
	else if (11 == plan_type)
	{
		role->GetTouzijihua()->OnNewFetchReward(true, seq);
	}
	else
	{
		role->GetTouzijihua()->OnFetchReward(plan_type, seq);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TouZiJiHuaReset(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetTouzijihua()->GmReset();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::BaiBeiFanLiBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	role->GetRoleActivity()->OnBaiBeiFanLiBuy();
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RollMoneyOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera_type = atoi(result[0]);

	role->GetRollMoney()->OnOperaReq(opera_type);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddWeek(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->OnWeekChange();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CrossAddWeek(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	static crossgameprotocal::GameCrossGmAddWeek pro;
	InternalComm::Instance().SendToCross((const char *)&pro, sizeof(pro));

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::EvaluateRole(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int rank_type = atoi(result[0]);
	int is_admire = atoi(result[1]);
	int times = atoi(result[2]);

	UserID user_id = role->GetUserId();

	static gglobalprotocal::GMEvaluateRole er;
	er.db_index = user_id.db_index;
	er.role_id = user_id.role_id;
	er.target_uid = UidToInt(user_id);
	er.rank_type = rank_type;
	er.is_admire = is_admire;
	er.times = times;
	InternalComm::Instance().NetSend(InternalComm::Instance().m_golbal_server_id, (const char*)&er, sizeof(er));

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::YaoJiangReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int yaojiang_type = atoi(result[0]);

	role->GetRoleYaojiang()->OnYaojiangReq(yaojiang_type);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::MarryReserveReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	static gglobalprotocal::GmMarryReserveReq gmrr;
	gmrr.uid = UidToInt(role->GetUserId());
	gmrr.reserve_index = static_cast<short>(atoi(result[0]));
	gmrr.marry_level = static_cast<short>(atoi(result[1]));
	gmrr.is_clear = static_cast<short>(atoi(result[2]));
	gmrr.is_force_start = static_cast<short>(atoi(result[3]));
	gmrr.is_force_next_status = atoi(result[4]);
	InternalComm::Instance().NetSend(InternalComm::Instance().m_golbal_server_id, (const char *)&gmrr, sizeof(gmrr));

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::DatingForceInvite(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int uid = atoi(result[0]);

	gglobalprotocal::GMDatingForceInvite dfi;
	dfi.self_uid = UidToInt(role->GetUserId());
	dfi.target_uid = uid;
	InternalComm::Instance().NetSend(InternalComm::Instance().m_golbal_server_id, (const char *)&dfi, sizeof(dfi));

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::MarryXunyouOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera_type = atoi(result[0]);
	role->GetRoleActivity()->OnMarryXunyouOpera(opera_type);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::MarryHunyanBless(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	static Protocol::CSMarryHunyanBless mhb;
	mhb.merry_uid = atoi(result[0]);
	mhb.zhufu_type = atoi(result[1]);
	mhb.contenttxt_len = 0;
	mhb.contenttxt[sizeof(mhb.contenttxt) - 1] = '\0';

	SpecialHunyan *special_logic = dynamic_cast<SpecialHunyan *>(role->GetScene()->GetSpecialLogic());
	if (NULL != special_logic && Scene::SCENE_TYPE_HUN_YAN_FB == special_logic->GetSceneType())
	{
		special_logic->ZhuFuHelper(role, &mhb);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::MarryHunyanOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera_type = atoi(result[0]);
	int opera_param = atoi(result[1]);
	int invited_uid = atoi(result[2]);

	SpecialHunyan *special_logic = dynamic_cast<SpecialHunyan *>(role->GetScene()->GetSpecialLogic());
	if (NULL != special_logic && Scene::SCENE_TYPE_HUN_YAN_FB == special_logic->GetSceneType())
	{
		special_logic->OnHunyanOpera(role, opera_type, opera_param, invited_uid);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::MarryHunyanInvite(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	//int be_invite_uid = atoi(result[0]);
	//int rune_index = atoi(result[1]);
	//int is_free_invite = atoi(result[2]);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetIsReserveMarry(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int is_reserve_marry = atoi(result[0]);

	role->SetIsReserveMarry(static_cast<char>(is_reserve_marry));
	
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::DiscountBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int seq = atoi(result[0]);

	role->GetDiscountBuy()->OnBuy(seq);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddNeqScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int score = atoi(result[0]);

	role->GetFBEquip()->GMAddScore(score);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::Divorce(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetQingyuan()->DivorceReq(role->GetLoverUid());

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddIntimacy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int intimacy_value = atoi(result[0]);

	Team *team = role->GetTeamInfo()->GetMyTeam();
	if (NULL != team)
	{
		for (int i = 0; i < MAX_TEAM_MEMBER_NUM; ++ i)
		{
			Team::TeamMember *member = team->GetTeamMember(i);
			if (NULL != member && role->GetUID() != (UidToInt(member->user_id) && role->GetFriendManager()->IsFriend(member->user_id)))
			{
				Role *other_role = World::GetInstWorld()->GetSceneManager()->GetRole(member->user_id);
				if (NULL != other_role)
				{
					FriendManager::IntimacyIncrease(role, other_role, intimacy_value, "GM");
				}
				continue;
			}
		}
	}

	return true;
}

bool Gm::SetQingyuanBlessDays(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int days = atoi(result[0]);
	role->GetQingyuan()->SetBlessDays(days);

	return true;
}

bool Gm::AddWelfareHappytreeVal(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int value = atoi(result[0]);
	role->GetWelfare()->GMAddTotalGrouthVal(value);

	return true;
}

bool Gm::AddWelfareHappytreeValByType(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	role->GetWelfare()->AddHappytreeGrowthvalue(type);

	return true;
}

bool Gm::XLTKillRole(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	if (Scene::SCENE_TYPE_CROSS_XIULUO_TOWER == scene->GetSceneType())
	{
		scene->GetSpecialLogic()->OnCharacterDie(role, obj_id);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetZhuanshenLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int value = atoi(result[0]);

	role->GetZhuanShengEquip()->GMSetZhuanShengLevel(value);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ResetDivorceTime(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->SetDivorce(0);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ResetMarry(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	GameName temp_name = {0};
	role->GetLoverName(temp_name);

	role->SetMarryInfo(role->GetLoverUid(), temp_name, 1);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::FindActivity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int is_find_reward = atoi(result[0]);
	int activity_find_type = atoi(result[1]);

	gglobalprotocal::GmActivityFindReq gafr;
	gafr.self_uid = UidToInt(role->GetUserId());
	gafr.is_find_reward = static_cast<short>(is_find_reward);
	gafr.activity_find_type = static_cast<short>(activity_find_type);
	InternalComm::Instance().NetSend(InternalComm::Instance().m_golbal_server_id, (const char*)&gafr, sizeof(gafr));

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::BeMonthCardUser(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	EventHandler::Instance().OnBuyMonthCard(role);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::BeTouZiJiHuaUser(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int plan_type = atoi(result[0]);

	role->GetTouzijihua()->OnBuyPlan(plan_type);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SatisfyTaskCondition(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int condition_type = atoi(result[0]);
	int param_1 = atoi(result[1]);
	int param_2 = atoi(result[2]);

	switch(condition_type)
	{
	case TASK_CON_TALK:
		{
			role->GetTaskManager()->OnTalkToNPC(param_1);
		}
		break;

	case TASK_CON_KILL_MONSTER:
		{
			role->GetTaskManager()->OnKillMonster(0, Posi(0, 0), param_1, param_2);
		}
		break;

	case TASK_CON_KILL_BOSS_TYPE:
		{
			role->GetTaskManager()->OnKillMonster(0, Posi(0, 0), param_1, param_2);
		}
		break;

	case TASK_CON_GATHER:
		{
			role->GetTaskManager()->OnGather(param_1);
		}
		break;

	case TASK_CON_PASS_FB:
		{
			role->GetTaskManager()->OnPassFB(param_1);
		}
		break;

	case TASK_CON_COMMIT_TASK:
		{
			role->GetTaskManager()->OnCommitOneTask(param_1);
		}
		break;

	case TASK_CON_NOTHING:
		{
		}
		break;
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GetSignInReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	//int day = atoi(result[0]);

	role->GetWelfare()->FetchSignInReward();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SignIn(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int day = atoi(result[0]);

	role->GetWelfare()->SignInByDay(day);

	return MakeupResult(netid, command->type, true, "");
}


bool Gm::SignInFindBack(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetWelfare()->SignInFindBack();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GetActivityFind(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int find_type = atoi(result[0]);
	int is_free_find = atoi(result[1]);

	role->GetWelfare()->ActivityFind(find_type, (is_free_find != 0));

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::XiannvCall(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	UNSTD_STATIC_CHECK(4 == MAX_XIANNV_POS);

	char pos_list[MAX_XIANNV_POS] = {-1};
	pos_list[0] = (char)atoi(result[0]);
	pos_list[1] = (char)atoi(result[1]);
	pos_list[2] = (char)atoi(result[2]);
	pos_list[3] = (char)atoi(result[3]);

	role->GetXiannvManager()->FightOut(pos_list);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::XiannvRename(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::XiannvActive(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int xiannv_id = atoi(result[0]);

	role->GetXiannvManager()->ActiveXiannv(xiannv_id);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddEnergy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int energy = atoi(result[0]);

	role->AddEnergy(energy, "GM");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TestErrnum(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int err_num = atoi(result[0]);

	role->NoticeNum(err_num);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::NotifySystemMsg(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	short msg_type = static_cast<short>(atoi(result[0]));
	char *msg_content = result[1];
	short content_len = static_cast<short>(strlen(msg_content));

	role->SystemMsg(msg_content, content_len, msg_type);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::FetchVipLevelReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int seq = atoi(result[0]);
	
	role->GetVip()->FetchLevelReward(seq);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::FetchOpenGameActivityReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int reward_type = atoi(result[0]);
	int reward_seq = atoi(result[1]);

	role->GetRoleActivity()->OnFetchOpenGameActivityReward(reward_type, reward_seq);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CFResetOpponent(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	ChallengeField::Instance().OnResetOpponentReq(role);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CFFightReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opponent_index = atoi(result[0]);
	bool ignore_pos = (0 != atoi(result[1]));
	int rank_pos = atoi(result[2]);
	int buy_jointimes = atoi(result[3]);

	ChallengeField::Instance().OnFightReq(role, opponent_index, ignore_pos, rank_pos, (0 != buy_jointimes));

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::Wabao(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera_type = atoi(result[0]);
	if (10 == opera_type)
	{
		role->GetWabao()->SetBaotuCount(30);
		role->GetWabao()->SendWabaoInfo();
	}
	else
	{
		role->GetWabao()->OnOperaReq(opera_type);
	}
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::JilianOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera_type = atoi(result[0]);
	int param = atoi(result[1]);

	if (Protocol::CSJilianOpera::OPERA_TYPE_JILIAN == opera_type)
	{
		role->GetJilian()->Start(param);
	}
	else
	{
		role->GetJilian()->Continue();
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TestSingleChat(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	char *msg_content = result[0];
	static char buff[1024]; memset(buff, 0, sizeof(buff));

	Protocol::CSSingleChat * sc = (Protocol::CSSingleChat *)buff;
	//sc->to_uid = role->GetUID();
	sc->to_uuid = UniqueUserIDToLongLong(role->GetUniqueUserID());
	sc->msg_length = static_cast<int>(strlen(msg_content) + 1);

	int total_length = static_cast<int>(sizeof(Protocol::CSSingleChat)) + sc->msg_length;
	if (total_length <= static_cast<int>(sizeof(buff)))
	{
		memcpy(buff + sizeof(Protocol::CSSingleChat), msg_content, sc->msg_length);

		PerformanceCatch::Instance().SimplePerformanceCatchBegin(PERFORMANCE_TEST_SINGLE_CHAT);

		int try_count = 0;
		while(try_count ++ < 10000)
		{
			ChatManager::Instance().OnSingleChat(role, sc, total_length, false);
		}

		PerformanceCatch::Instance().SimplePerformanceCatchEnd(PERFORMANCE_TEST_SINGLE_CHAT);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TestChanelChat(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	char *msg_content = result[0];
	static char buff[1024]; memset(buff, 0, sizeof(buff));

	Protocol::CSChannelChat * cc = (Protocol::CSChannelChat *)buff;
	cc->channel_type = RandomNum(5);
	cc->msg_length = static_cast<int>(strlen(msg_content) + 1);

	int total_length = static_cast<int>(sizeof(Protocol::CSChannelChat)) + cc->msg_length;
	if (total_length <= static_cast<int>(sizeof(buff)))
	{
		memcpy(buff + sizeof(Protocol::CSChannelChat), msg_content, cc->msg_length);

		PerformanceCatch::Instance().SimplePerformanceCatchBegin(PERFORMANCE_TEST_CHANEL_CHAT);

		int try_count = 0;
		while(try_count ++ < 10000)
		{
			ChatManager::Instance().OnChannelChat(role, cc, total_length, chatdef::ORIGIN_TYPE_NORMAL_CHAT);
		}

		PerformanceCatch::Instance().SimplePerformanceCatchBegin(PERFORMANCE_TEST_CHANEL_CHAT);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddFootPrint(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetFootPrint()->ActiveFootprint();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GMQingyuanResetHunyan(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetQingyuan()->GMResetHunyan();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GMSetZhanShenDianPassLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int pass_level = atoi(result[0]);

	role->GetZhanShenDian()->SetPassLevel(pass_level);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::FriendBless(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	int opera_type = atoi(result[0]);
	int param = atoi(result[1]);
	
	if (Protocol::FRIEND_BLESS_OPERA_TYPE_QUERY == opera_type)
	{
		FriendBlessManager::Instance().SendAllInfo(role);
	}
	else if (Protocol::FRIEND_BLESS_OPERA_TYPE_BLESS == opera_type)
	{
		FriendBlessManager::Instance().OnBlessReq(role, param);
	}
	else if (Protocol::FRIEND_BLESS_OPERA_TYPE_FETCH_REWARD == opera_type)
	{
		FriendBlessManager::Instance().OnFetchReward(role, param);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GMFishPoolRaiseFish(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	FishPoolManager::Instance().RaiseFish(role);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GMFishPoolExtendCapacity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GMFishPoolHarvestFish(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	FishPoolManager::Instance().HarvestFish(role);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GMFishSendWorldPoolList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	FishPoolManager::Instance().SendWorldFishPoolGeneralInfo(role);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GMFishPoolBuyBulletReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	FishPoolManager::Instance().BuyBulletReq(role);
	

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GMFishPoolUpquality(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	FishPoolManager::Instance().UpFishQuality(role);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TouchWorldEventObj(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	ObjID world_event_obj_id = static_cast<ObjID>(atoi(result[0]));

	Obj *obj = scene->GetObj(world_event_obj_id);
	if (NULL != obj && Obj::OBJ_TYPE_WORLDEVENT_OBJ == obj->GetObjType())
	{
		WorldEventObj *world_event_obj = (WorldEventObj*)obj;
		world_event_obj->Touch(role);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::WorldEventNextLogicState(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	WorldEventManager::Instance().GmNextLogicState(role);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::WorldEventAddScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int add_score = atoi(result[0]);

	WorldEventManager::Instance().GmAddScore(role, add_score);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::WorldEventRoleAddScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int event_type = atoi(result[0]);
	int add_score = atoi(result[1]);

	role->GetRoleWorldEvent()->AddScore(event_type, add_score);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::WorldEventNextEventType(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	WorldEventManager::Instance().GmNextEventType(role);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RADayChongzhiFanLiOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera = atoi(result[0]);
	int param = atoi(result[1]);

	role->GetRoleActivity()->OnRADayChongZhiFanLiOperaReq(opera, param);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RADayConsumeGoldOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera = atoi(result[0]);
	int param = atoi(result[1]);

	role->GetRoleActivity()->OnRADayConsumeGoldOperaReq(opera, param);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RATotalConsumeGoldOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera = atoi(result[0]);
	int param = atoi(result[1]);

	role->GetRoleActivity()->OnRATotalConsumeGoldOperaReq(opera, param);

	return MakeupResult(netid, command->type, true, "");

}

bool Gm::RADayActiveDegreeOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera = atoi(result[0]);
	int param = atoi(result[1]);

	role->GetRoleActivity()->OnRADayActiveDegreeOperaReq(opera, param);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RAServerPanicBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera_type = atoi(result[0]);
	int param = atoi(result[1]);

	role->GetRoleActivity()->OnRAServerPanicBuyOperaReq(opera_type, param);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RAPersonalPanicBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera_type = atoi(result[0]);
	int param = atoi(result[1]);

	role->GetRoleActivity()->OnRAPersonalPanicBuyOperaReq(opera_type, param);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RoleGoalFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	role->GetRoleGoal()->FetchPersonalGoalReward();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RAKillBossOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera_type = atoi(result[0]);

	role->GetRoleActivity()->OnRAKillBossOperaReq(opera_type);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RALoginGiftOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera_type = atoi(result[0]);
	int param = atoi(result[1]);

	role->GetRoleActivity()->OnRALoginGiftOperaReq(opera_type, param);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RAShouYouYuXiangOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera_type = atoi(result[0]);
	int param = atoi(result[1]);

	role->GetRoleActivity()->OnRAShouYouYuXiangOperaReq(opera_type, param);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RedPaperCreate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int gold_num = atoi(result[1]);
	int can_fetch_times = atoi(result[2]);
	int is_bind_gold = atoi(result[3]);
	
	RedPaperManager::Instance().OnCreateRedPaper(role, type, gold_num, can_fetch_times, false, is_bind_gold);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RedPaperFetch(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int id = atoi(result[0]);

	RedPaperManager::Instance().OnFetchRedPaper(role, id);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::WuxingUplevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int wx_index = atoi(result[0]);
	bool is_auto_buy = (0 != atoi(result[1]));

	role->GetMentality()->OnMentalityUplevelWuxing(wx_index, is_auto_buy);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangeCamp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, "");

	char camp_type = atoi(result[0]);
	role->ChangeCamp(camp_type);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChangeProf(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int prof = atoi(result[0]);
	int sex = atoi(result[1]);

	role->ChangeProf(static_cast<char>(prof), static_cast<char>(sex));

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChargeWantMoneyTest1(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetRoleActivity()->GetTotalChongzhiWantMoneyInfo();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChargeWantMoneyTest2(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetRoleActivity()->FetchTotalChongzhiWantMoneyReward();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ZaiChongzhiReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetRoleActivity()->FetchZaiChongzhiReward();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::MonthCardFetchDayReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetMonthCard()->FetchDayReward();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CornucopiaFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetRoleActivity()->OnRAFetchCornucopiaReward();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CornucopiaTaskValue(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int task_type = atoi(result[0]);
	int value = atoi(result[1]);

	role->GetRoleActivity()->GmSetNewCornucopiaTaskValue(task_type, value);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CSAOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int sub_type = atoi(result[0]);
	int param_1 = atoi(result[1]);
	int param_2 = atoi(result[2]);

	role->GetRoleActivity()->OnCSAOperaReq(sub_type, param_1, param_2);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CSFuling(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int equip_index = atoi(result[0]);
	EquipFunction::EquipFuling(role, true, true, equip_index);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::StartCross(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int activity_type = atoi(result[0]);
	int is_force = atoi(result[1]);
	int param1 = atoi(result[2]);
	int param2 = atoi(result[3]);
	int param3 = atoi(result[4]);
	int x = atoi(result[5]);
	int y = atoi(result[6]);

	role->GetRoleCross()->OnStartCrossReq(activity_type, 0 != is_force, param1, param2, param3, x, y);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::Cross1V1Match(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetRoleCross()->OnCrossMatch1v1Req();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::Cross1V1Ready(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	if (NULL != role->GetScene())
	{
		SpecialLogic *special_logic = role->GetScene()->GetSpecialLogic();
		if (NULL != special_logic && Scene::SCENE_TYPE_CROSS_1V1 == special_logic->GetSceneType())
		{
			SpecialCross1v1 *special_cross_1v1 = dynamic_cast<SpecialCross1v1*>(special_logic);
			if (nullptr != special_cross_1v1)
				special_cross_1v1->OnReady(role);
		}
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddCross1V1Score(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int score = atoi(result[1]);

	role->GetRoleCross()->OnCross1v1GM(type, score);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CrossMultiuserChallengeMatchingReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetRoleCross()->OnCrossMultiuserChallengeOpera(true);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CrossMultiuserChallengeCancelMatching(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetRoleCross()->CancelCrossMultiuserChallengeMatching();
	role->GetRoleCross()->ResetCrossMultiuserChallengeMatchingState();
	crossgameprotocal::CrossGameMultiuserChallengeMatchingStateNotify scmcms;
	scmcms.matching_state = CROSS_MULTIUSER_CHALLENGE_MATCHING_STATE_CANCEL;
	scmcms.auto_start_cross = 0;
	scmcms.notice_match = 0;
	role->GetRoleCross()->OnSyncCrossMultiuserChallengeMatchingState(&scmcms);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CrossMultiuserChallengeScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int score = atoi(result[0]);
	role->GetRoleCross()->AddCrossMultiuserChallengeScore(score, "GmAdd3v3Score");
	role->GetRoleCross()->SendCrossMultiuserChallengeSelfActivityInfo();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CrossMultiuserChallengeAddGongxun(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int gongxun = atoi(result[0]);
	role->GetRoleCross()->Add3v3Gongxun(gongxun, "GM::CrossMultiuserChallengeAddGongxun");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CrossMultiuserChallengeMatchTime(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int add_times = atoi(result[0]);
	role->GetRoleCross()->Cross3v3AddMatchTimes(add_times, "GM::CrossMultiuserChallengeMatchTime");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddCrossHonor(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int cross_honor= atoi(result[0]);

	role->AddCrossHonor(cross_honor, "GM");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::YaoShouGuangChangNextStatus(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	YaoShouGuangChangMgr::Instance().GmForceNextStatus();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SuoYaoTaNextstatus(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	SuoYaoTaMgr::Instance().GmForceNextStatus();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CommitPaohuanTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	
	role->GetPaoHuanTask()->OnSkipTask(true, 0);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::PaoHuanRewardRollReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetPaoHuanTask()->OnGetPhaseRewardRollReq(1);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::JingHuaHusongRefresh(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	JingHuaHuSongMgr::Instance().GmRefresh();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CleraGlobalData(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	WorldStatusCommonDataParam & data = WorldStatus::Instance().GetCommonData();
	data.buy_month_card_times = 0;

	memset(data.oga_putonequipment_fetch_times, 0, sizeof(data.oga_putonequipment_fetch_times));
	memset(data.oga_capability_fetch_times, 0, sizeof(data.oga_capability_fetch_times));
	memset(data.oga_rolelevel_fetch_times, 0, sizeof(data.oga_rolelevel_fetch_times));
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddOnlineTime(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int add_onlinetime= atoi(result[0]);

	role->GMAddRoleOnLineTime(add_onlinetime * 60);
	role->GetActiveDegree()->AddSetOnlineTime(add_onlinetime);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ChongzhiCmd(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int chongzhi_num= atoi(result[0]);

	role->GetRoleActivity()->CheckChongzhiExtraRewardByNum(chongzhi_num);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::MysteriousshopNextStatus(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	LOGIC_CONFIG->GetMysteriousShop().GmNextStatus();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ReclacCampRank(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int camp = atoi(result[0]);

	CampManager::Instance().CalcCampRank3(camp);
	CampDefendMgr::Instance().UpdateDiaoXiang();
	
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GetRandBipinRewardSeq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int act_type = atoi(result[0]);

	role->GetRoleActivity()->SendRABipinCapabilityInfo(act_type);
	return true;
}

bool Gm::ResetBipinFetchFlag(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetRoleActivity()->GmResetBipinData();
	role->GetRoleActivity()->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_TOTAL);
	role->GetRoleActivity()->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_EQUIP);
	role->GetRoleActivity()->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_MOUNT);
	role->GetRoleActivity()->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WING);
	role->GetRoleActivity()->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_SHENGZHUANG);
	role->GetRoleActivity()->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_JINGLING);
	role->GetRoleActivity()->SendRABipinCapabilityInfo(RAND_ACTIVITY_TYPE_BP_CAPABILITY_WASH);
	return true;
}

bool Gm::CombineReset(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	CombineServerActivityManager::Instance().ResetData();

	return true;
}

bool Gm::CmdToGuild(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	GuildID guild_id = atoi(result[0]);
	int oper_type = atoi(result[1]);

	Guild *guild = GuildManager::Instance().GetGuild(guild_id);
	if (NULL != guild)
	{
		GuildManager::Instance().OnGmCommand(guild_id, oper_type);
	}
	
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CmdToGuildBox(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int oper_type = atoi(result[0]);
	int param1 = atoi(result[1]);
	int param2 = atoi(result[1]);

	switch (oper_type)
	{
	case 100:
		GuildManager::Instance().GuildBoxGmOpenNow(role, param1);
		break;

	case 101:
		GuildManager::Instance().GuildBoxGmCleanCD(role);
		break;

	case 102:
		GuildManager::Instance().GuildBoxGmStart(0 != param1);
		break;

	case 103:
		OfflineRegisterManager::Instance().GuildBoxResetAll();
		break;

	default:
		GuildManager::Instance().GuildBoxOperate(role, oper_type, param1, param2);
		break;
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddKillerCount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int count = atoi(result[1]);

	if (0 == type)
	{
		role->AddHCHZKillCount(count);
		role->GetTitleManager()->CheckAllTitleCondition(TITLE_TYPE_HCHZ_KILLER);
	}
	else
	{
		role->AddGCZKillCount(count);
		role->GetTitleManager()->CheckAllTitleCondition(TITLE_TYPE_GCZ_100_KILLER);
		role->GetTitleManager()->CheckAllTitleCondition(TITLE_TYPE_GCZ_1000_KILLER);
		role->GetTitleManager()->CheckAllTitleCondition(TITLE_TYPE_GCZ_10000_KILLER);
	}

	return true;
}

bool Gm::PetQinmiOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]) + Protocol::PET_REQ_TYPE_QINMI_PROMOTE;
	int param1 = atoi(result[1]);
	int param2 = atoi(result[2]);

	switch(type)
	{
	case Protocol::PET_REQ_TYPE_QINMI_PROMOTE:
		{
			role->GetPet()->PetQinmiPromote(param1, param2);
		}
		break;

	case Protocol::PET_REQ_TYPE_QINMI_AUTO_PROMOTE:
		{
			role->GetPet()->PetQinmiAutoPromote(param1);
		}
		break;

	case Protocol::PET_REQ_TYPE_FOOD_MARKET_CHOU_ONCE:
		{
			role->GetPet()->FoodMarketChouOnce(0 != param1);
		}
		break;

	case Protocol::PET_REQ_TYPE_FOOD_MARKET_CHOU_TIMES:
		{
			role->GetPet()->FoodMarketChouTimes(param1, 0 != param2);
		}
		break;
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ActiveAllMultiMountImg(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetMultiMount()->GMActiveAllImg();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::MultiMountOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera_type = atoi(result[0]);
	int param_1 = atoi(result[1]);
	int param_2 = atoi(result[2]);
	int param_3 = atoi(result[3]);

	role->GetMultiMount()->OnMountReq(opera_type, param_1, param_2, param_3);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CrossBossBuyReliveTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int times = atoi(result[0]);
	role->GetRoleCross()->OnCrossBossBuyReliveTimes(times);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CloseAllRandActivity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	unsigned int now = static_cast<unsigned int>(EngineAdapter::Instance().Time());

	for (int i = RAND_ACTIVITY_TYPE_BEGIN; i < RAND_ACTIVITY_TYPE_END; ++ i)
	{
		RandActivityManager::Instance().OnForceSetActivityTime(i, now, now, RAND_ACTIVITY_OPEN_TYPE_NORMAL);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RATreasuresMallTest(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera = atoi(result[0]);
	int param1 = atoi(result[1]);
	int param2 = atoi(result[2]); 

	role->GetRoleActivity()->OnRaTreasuresMallOperaReq(opera, param1, param2);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddZhiBaoExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int value = atoi(result[0]);

	role->GetZhiBaoManager()->AddZhiBaoExp(value);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::XnActiveHuanhua(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int xianvn_id = atoi(result[0]);
	int item_index = atoi(result[1]);

	role->GetXiannvManager()->OnActiveHuanHuaReq(xianvn_id,item_index);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::XiannvShengwuGm(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int param1 = atoi(result[1]);
	int param2 = atoi(result[2]);
	int param3 = atoi(result[3]);

	role->GetXiannvManager()->OnShengwuGm(type, param1, param2, param3);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetMojieLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int mojie_type = atoi(result[0]);
	int is_auto_buy = atoi(result[1]);

	role->GetMojie()->OnUpMojielevel(role, mojie_type,0 != is_auto_buy);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddWing(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	WingFunction::ActiveWing(role);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddHalo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	HaloFunction::ActiveHalo(role);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddShengong(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	ShengongFunction::ActiveShengong(role);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddShenyi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	ShenyiFunction::ActiveShenyi(role);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::LittlePetOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int param1 = atoi(result[1]);
	int param2 = atoi(result[2]);
	int param3 = atoi(result[3]);

	enum LITTLE_PET_OPERA_TYPE
	{
		ADD_SCORE,
		ACTIVE,
		REPLACE,
		STRENGTHEN,
	};

	LittlePet *m_little_pet = GLOBAL_ROLE_MGR.GetLittlePet(role->GetUID());
	if (NULL == m_little_pet)
	{
		return false;
	}

	switch (type)
	{
	case ADD_SCORE:
		{
			m_little_pet->AddScore(param1, "gm");
		}
		break;

	case ACTIVE:
		{
			m_little_pet->ActiveLittlePet(param1);
		}
		break;

	case REPLACE:
		{
			m_little_pet->Replace(param1, param2);
		}
		break;

	case STRENGTHEN:
		{
			m_little_pet->Strengthen(role, param1, param2, param3);
		}
		break;

	default:
		break;
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddHunshouExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int add_exp = atoi(result[0]);
	role->GetLieMingMgr()->GmAddHunshouExp(add_exp);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddIdentifyExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int exp = atoi(result[0]);

	role->GetShenzhouWeapon()->GMAddIdentifyExp(exp);
	return true;
}

bool Gm::AddGather(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int gather_id = atoi(result[0]);

	GatherObj *gather_obj = new GatherObj();
	gather_obj->SetPos(role->GetPos());
	gather_obj->Init(NULL, gather_id, 1000, 0, true);
	scene->AddObj(gather_obj);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::EquipmentSuitReq( const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command )
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int operate_type = atoi(result[0]);
	int equip_index = atoi(result[1]);

	role->GetEquipSuit()->OnSuitEquipOperate(operate_type, equip_index);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddXLTScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	SpecialCrossXiuluoTower *logic = dynamic_cast<SpecialCrossXiuluoTower*>(scene->GetSpecialLogic());
	if (NULL == logic || Scene::SCENE_TYPE_CROSS_XIULUO_TOWER != scene->GetSceneType())
	{
		return false;
	}

	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int add_score = atoi(result[0]);

	logic->GMAddScore(role, add_score);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::XLTBoss(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	SpecialLogic *logic = scene->GetSpecialLogic();
	if (NULL == logic || Scene::SCENE_TYPE_CROSS_XIULUO_TOWER != scene->GetSceneType())
	{
		return false;
	}

	SpecialCrossXiuluoTower *xlt_logic = (SpecialCrossXiuluoTower *)logic;
	xlt_logic->GMFlushBoss(role);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::FetchTimeLimitVip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	role->GetVip()->FetchTimeLimitVip();
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::EnterBossFamily(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int boss_type = atoi(result[0]);
	int scene_id = atoi(result[1]);

	Protocol::CSEnterBossFamily msg;
	msg.enter_type = boss_type;
	msg.scene_id = scene_id;

	BossFamilyManager::Instance().OnEnterBossFamily(role, &msg);
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetRuneTower(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int layer = atoi(result[0]);
	int kill_monster_num = atoi(result[1]);

	role->GetRuneSystem()->GMSetRuneTower(layer, kill_monster_num);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::RuneSystemOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int p1 = atoi(result[1]);
	int p2 = atoi(result[2]);
	int p3 = atoi(result[3]);
	int p4 = atoi(result[4]);

	role->GetRuneSystem()->GmOpera(type, p1, p2, p3, p4);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ExpFbBuyBuff(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	if (Scene::SCENE_TYPE_EXPFB == role->GetScene()->GetSpecialLogic()->GetSceneType())
	{
		SpecialExpFB *special_logic = dynamic_cast<SpecialExpFB*>(role->GetScene()->GetSpecialLogic());
		if (nullptr != special_logic)
			special_logic->OnPayBuff(role);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ExpFbBuyTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	role->GetRoleDailyFb()->OnPayFbTimes();
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ShenzhouAddIdentifyExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int exp = atoi(result[0]);

	role->GetShenzhouWeapon()->GMAddIdentifyExp(exp);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ShenzhouAddExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int exp = atoi(result[0]);

	role->GetShenzhouWeapon()->GMAddShenzhouExp(exp);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ShengeAddJinghua(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int jinghua = atoi(result[0]);

	role->GetShengeSystem()->AddMarrowScore(jinghua, "GmAddMarrowScore");

	return MakeupResult(netid, command->type, true, "");
}
bool Gm::GetLinuxTimeMS(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	struct timeval tv;
	gettimeofday(&tv, NULL);
	unsigned long long tv_sec = static_cast<unsigned long long>(tv.tv_sec);
	unsigned long long tv_usec = static_cast<unsigned long long>(tv.tv_usec);
	unsigned long long local_time_ms = tv_sec * 1000 + tv_usec / 1000;

	gamelog::g_log_gm.printf(LL_INFO, "tv_sec: %ld, tv_usec: %ld, cur_ms: %lld", tv.tv_sec, tv.tv_usec, local_time_ms);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::PutXingzuoMiji(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

// 	int zodiac_type = atoi(result[0]);
// 	int miji_index = atoi(result[1]);

	//role->GetChineseZodiac()->PutMiji(zodiac_type, miji_index);
	//role->GetChineseZodiac()->PutMijiRet();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::CompoundXingzuoMiji(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

//	int index1 = atoi(result[0]);
 	//int index2 = atoi(result[1]);
 	//int index3 = atoi(result[2]);

	//role->GetChineseZodiac()->MijiCompound(index1);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::UpgradeXingling(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int id = atoi(result[0]);
	int is_auto_buy = atoi(result[1]);

	role->GetChineseZodiac()->UpgradeXingling(id, is_auto_buy);
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::UplevelZhangkong(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);

	role->GetShengeSystem()->ReqUpLevelZhangkong(type);
	role->GetShengeSystem()->UpZhangkongLevelRet();
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GuildPaoSaizi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	Guild *guild = GuildManager::Instance().GetGuild(role->GetGuildID());
	if (NULL != guild)
	{
		guild->GetGuildMemberManager().ReqGuildPaoSaizi(role);
	}
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::OnRoleFighting(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);

	RoleFighting *fighting = GlobalRoleManager::Instance().GetRoleFighting(role->GetUID());
	if (NULL != fighting)
	{
		fighting->OnGmReq(type, role);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ActivityOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int activity_type = atoi(result[0]);
	int type = atoi(result[1]);
	int param_1 = atoi(result[2]);
	int param_2 = atoi(result[3]);
	switch (activity_type)
	{
		case RAND_ACTIVITY_TYPE_KING_DRAW:
		{
			role->GetRoleActivity()->OnRAKingDrawOperaReq(type, param_1, param_2);
		}
		break;

		case RAND_ACTIVITY_TYPE_MINE:
		{
			role->GetRoleActivity()->OnRAMineOperaReq(type, param_1);
		}
		break;

		case RAND_ACTIVITY_TYPE_TIANMING_DIVINATION:
		{
			role->GetRoleActivity()->OnRATianMingDivinationOperaReq(type, param_1);
		}
		break;

		case RAND_ACTIVITY_TYPE_EXTREME_LUCKY:
		{
			role->GetRoleActivity()->OnRAExtremeLuckyOperaReq(type, param_1);
		}
		break;

		case RAND_ACTIVITY_TYPE_PROMOTING_POSITION:
		{
			role->GetRoleActivity()->OnRAPromotingPositionOperaReq(type, param_1);
		}
		break;

		case RAND_ACTIVITY_TYPE_MAP_HUNT:
		{
			role->GetRoleActivity()->OnRaMapHuntOperaReq(type, param_1, param_2);
		}
		break;
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::Shenshou(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int param1 = atoi(result[1]);
	int param2 = atoi(result[2]);
	int param3 = atoi(result[3]);

	role->GetShenshou()->OnGMReq(type, param1, param2, param3);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TestSkip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int param1 = atoi(result[1]);

	switch (type)
	{
	case Protocol::CSSkipReq::SKIP_TYPE_CHALLENGE:
		{
			RoleFighting *fighting = GlobalRoleManager::Instance().GetRoleFighting(role->GetUID());
			if (NULL != fighting)
			{
				fighting->OnSkipChallenge(role);
			}
		}
		break;

	case Protocol::CSSkipReq::SKIP_TYPE_SAILING:
		{
			RoleFighting *fighting = GlobalRoleManager::Instance().GetRoleFighting(role->GetUID());
			if (NULL != fighting)
			{
				fighting->OnSkipSailing(role, param1);
			}
		}
		break;

	case Protocol::CSSkipReq::SKIP_TYPE_MINE:
		{
			RoleFighting *fighting = GlobalRoleManager::Instance().GetRoleFighting(role->GetUID());
			if (NULL != fighting)
			{
				fighting->OnSkipMining(role, param1);
			}
		}
		break;

	case Protocol::CSSkipReq::SKIP_TYPE_FISH:
		{
			FishPool *fish_pool = FishPoolManager::Instance().GetFishPool(role->GetUID());
			if (NULL != fish_pool)
			{
				fish_pool->OnSkipFishPool(role, param1);
			}
		}
		break;

	case Protocol::CSSkipReq::SKIP_TYPE_JINGLING_ADVANTAGE:
		{
			JinglingAdvantage::Instance().OnSkipGather(role);
		}
		break;

	case Protocol::CSSkipReq::SKIP_TYPE_SHENZHOU_WEAPON:
		{
			role->GetShenzhouWeapon()->SkipGather();
		}
		break;

	case Protocol::CSSkipReq::SKIP_TYPE_XINGZUOYIJI:
		{
			int fb_key = ActivityShadow::Instance().GetFBKeyByRoomIndex(0);
			SpecialXingzuoYiji *xingzuoyiji_logic = World::GetInstWorld()->GetSceneManager()->GetSpecialXingzuoYiji(fb_key, true);
			if (NULL != xingzuoyiji_logic)
			{
				xingzuoyiji_logic->SkipGather(role);
			}
		}
		break;

	case Protocol::CSSkipReq::SKIP_TYPE_QYSD:
		{
			role->GetQingyuan()->OnQingYuanShengDiSkipTask();
		}
		break;

	case Protocol::CSSkipReq::SKIP_TYPE_PRECIOUS_BOSS:
		{
			role->GetRolePreciousBoss()->SkipTask();
		}
		break;

	case Protocol::CSSkipReq::SKIP_TYPE_PAOHUAN_TASK:
		{
			role->GetPaoHuanTask()->OnSkipTask(false, 0);
		}
		break;

	case Protocol::CSSkipReq::SKIP_TYPE_CROSS_GUIDE_BATTLE:
		{
			role->GetRoleCross()->SkipCrossGuildBattleTask();
		}
		break;

	default:
		break;
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TestAddShenYinScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int num = atoi(result[1]);

	switch (type)
	{
	case 0:
		{
			role->GetShenYin()->GmAddImpritScore(num);
		}
		break;

	case 1:
		{
			role->GetShenYin()->AddShenYinChouHunScore(num);
		}
		break;

	default:
		break;
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddPastureScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int score = atoi(result[0]);

	role->GetRoleCross()->AddPastureScore(score, "GM");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ElementOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int p1 = atoi(result[1]);
	int p2 = atoi(result[2]);
	int p3 = atoi(result[3]);
	int p4 = atoi(result[4]);
	int p5 = atoi(result[5]);

	role->GetElementHeart()->GmOpera(type, p1, p2, p3, p4, p5);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TakeFeixian(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{

	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");


	int type = atoi(result[0]);
	short index = atoi(result[1]);
	if (type == 1)
	{ // 脱下
		role->GetFeixianManager()->TakeOff(6, false);
	}
	else
	{ // 穿上
		role->GetKnapsack()->Use(index, 1, 6);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GStrTest(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);

	switch (type)
	{
	case ContainerType_Error:
	{
		gstr::SendError(role, result[1], result[2]);
	}
	break;

	case ContainerType_Notice:
	{
		gstr::SendNotice(role, result[1], result[2]);
	}
	break;

	case ContainerType_MailContent:
	{
		static MailContentParam contentparam;
		contentparam.Reset();
		int length = gstr::MailContent(contentparam.contenttxt, sizeof(contentparam.contenttxt), result[1], atoi(result[2]));
		if (length > 0)
		{
			MailRoute::MailToUser(role->GetUserId(), SYSTEM_MAIL_REASON_INVALID, contentparam);
		}
	}
	break;

	case ContainerType_SysMsg:
	{
		if (CrossConfig::Instance().IsHiddenServer())
		{
			int cross_type = CrossConfig::Instance().GetHiddenServerCrossType();
			int sendlen = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), result[1], atoi(result[2]), cross_type);
			if (sendlen > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, sendlen, SYS_MSG_CENTER_ROLL);
			}
		}
		else
		{
			int sendlen = gstr::SysMsgContent(gstr::GSTR_BUF, sizeof(gstr::GSTR_BUF), result[1], atoi(result[2]));
			if (sendlen > 0)
			{
				World::GetInstWorld()->GetSceneManager()->SystemMsgThread(gstr::GSTR_BUF, sendlen, SYS_MSG_CENTER_ROLL);
			}
		}
	}
	break;
	}

	return true;
}

bool Gm::GMShangGuBossReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int p1 = atoi(result[1]);
	int p2 = atoi(result[2]);
	int p3 = atoi(result[3]);

	BossShangGuManager::Instance().OnShangGuBossReq(role, type, p1, p2, p3);
	return true;
}

bool Gm::SetRADayActivityDegree(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int degree = atoi(result[0]);
	
	role->GetRoleActivity()->SetRADayActivityDegree(degree);
	role->GetActiveDegree()->GMAddActiveDegree();
	return true;
}

bool Gm::ForceOpenGuildTianCiTongBI(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	SpecialGuildStation *special_logic = World::GetInstWorld()->GetSceneManager()->GetGuildStationSpecialLogic(role->GetGuildID(), true);
	if (NULL != special_logic)
	{
		special_logic->ForceTianCiTongBiNextState();
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddTianCiTongBiMaturity(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int param_1 = atoi(result[0]);

	SpecialGuildStation *special_logic = World::GetInstWorld()->GetSceneManager()->GetGuildStationSpecialLogic(role->GetGuildID(), true);
	if (NULL != special_logic)
	{
		special_logic->AddTreeMaturity(param_1);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::JinjiesysRewardOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera_type = atoi(result[0]);
	int sys_type = atoi(result[1]);
	int reward_type = atoi(result[2]);

	JinjieSysRewardMgr::Instance().OnJinjiesysRewardOpera(role, opera_type, sys_type, reward_type);

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::CrossReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int opera_type = atoi(result[0]);
	int param_1 = atoi(result[1]);
	int param_2 = atoi(result[2]);

	static crossgameprotocal::GameCrossGmOpera pro;
	pro.opera_type = opera_type;
	pro.param_1 = param_1;
	pro.param_2 = param_2;

	InternalComm::Instance().SendToCross((const char *)&pro, sizeof(pro));
	if (opera_type == 0)
	{
		if (param_1 == CROSS_PERSON_RANK_TYPE_ADD_CAPABILITY && param_2 == 1)    //清跨服排行榜顺便清原服数据，方便进行重算
		{
			role->GetCommonDataParam()->add_cap_for_rank = 0;
		}
		else if (param_1 == CROSS_PERSON_RANK_TYPE_ADD_CHARM && param_2 == 1)
		{
			role->GetCommonDataParam()->add_charm_for_rank = 0;
		}
	}

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::TeamTowerdefend(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	int wave = atoi(result[0]);
	if (nullptr != scene)
	{
		SpecialTeamTowerDefend * teamsp = dynamic_cast<SpecialTeamTowerDefend*>(scene->GetSpecialLogic());
		if(nullptr != teamsp)
		{
			teamsp->GmTowave(wave);
		}
	}
	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::TeamFbJump(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{

	SpecialEquipTeam *logic = dynamic_cast<SpecialEquipTeam*>(scene->GetSpecialLogic());
	if (NULL == logic || Scene::SCENE_TYPE_TEAM_EQUIPFB != scene->GetSceneType())
	{
		return false;
	}

	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	int to_layer = atoi(result[0]);

	logic->GMJumpLayerReq(role, to_layer);

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::AddCarnivalScore(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	if (nullptr == role)
	{
		return false;
	}
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	int chapter = atoi(result[0]) - 1;
	int score = atoi(result[1]);
	role->GetMagicalPreciousManager()->GMIncScore(chapter, score);

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::DecCarnivalScore(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	if (nullptr == role)
	{
		return false;
	}
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	int chapter = atoi(result[0]) - 1;
	int score = atoi(result[1]);
	role->GetMagicalPreciousManager()->GMDecScore(chapter, score);

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::CarnivalNextChapter(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	if (nullptr == role)
	{
		return false;
	}
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	int chapter = atoi(result[0]);
	role->GetMagicalPreciousManager()->GMIncChapter(chapter);

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::CarnivalFinish(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	if (nullptr == role)
	{
		return false;
	}
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	int chapter = atoi(result[0]) - 1;
	int reward_index = atoi(result[1]) - 1;
	char flag = (char)atoi(result[2]);
	role->GetMagicalPreciousManager()->GMFinish(chapter, reward_index, flag);

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::RALuckyWishLottery(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	if (nullptr == role)
	{
		return false;
	}
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	int param_0 = atoi(result[0]);
	int param_1 = atoi(result[1]);
	role->GetRoleActivity()->OnRALuckyWishOpera(param_0, param_1);

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::ShengwuUpgrade(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int param_1 = atoi(result[0]);
	role->GetXiannvManager()->ShengwuUpgrade(param_1);

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::AddShengwuEssence(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int param_1 = atoi(result[0]);
	role->GetXiannvManager()->AddShengwuEssence(param_1, "Gm::AddShengwuEssence");

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::SetShengwuLevel(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int shengwu_id = atoi(result[0]);
	int level = atoi(result[1]);
	role->GetXiannvManager()->GMSetShengwuLevel(shengwu_id, level);

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::AddShenzhouWeaponAddHunqiLevel(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int hunqi_id = atoi(result[0]);
	role->GetShenzhouWeapon()->GMAddHunqiLevel(hunqi_id);

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::PassPataFbLayer(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int layer = atoi(result[0]);
	role->GetRolePataFB()->OnFinish(layer);

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::AddProfessScore(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int score = atoi(result[0]);
	role->GetRoleActivity()->OnRAProfessAddScore(score, true);

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::GmSetChannelChatLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int channel_type = atoi(result[0]);
	int limit_level = atoi(result[1]);

	LOGIC_CONFIG->GetAgentAdapterConfig().CmdSetChannelChatLevel(channel_type, limit_level);

	ChatManager::Instance().SyncChatOpenLevelToClient();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetChatLimit(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = { 0 };
	this->ParseCommandParam(command->command, result, " ");

	int channel_type = atoi(result[0]);
	int role_level = atoi(result[1]);
	int vip_level = atoi(result[2]);
	int chat_limit_condition_type = atoi(result[3]);

	LOGIC_CONFIG->GetAgentAdapterConfig().CmdSetChannelChatLevel(channel_type, role_level);
	WorldStatus::Instance().CmdSetChannelVipLevelLimit(channel_type, vip_level);
	WorldStatus::Instance().CmdSetChatLimitConditionType(channel_type, chat_limit_condition_type);

	ChatManager::Instance().SyncChatOpenLevelToClient();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetAudioChatForbidFlag(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = { 0 };
	this->ParseCommandParam(command->command, result, " ");

	int audio_chat_forbid_flag = atoi(result[0]);

	WorldStatus::Instance().CmdSetAudioChatForbidFlag(audio_chat_forbid_flag);
	ChatManager::Instance().SyncChatOpenLevelToClient();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddFakeFirstChongzhiNum(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int num = atoi(result[0]);
	
	for (int i = 0; i < num; i ++)
	{
		WorldStatus::Instance().OnFakeFirstChongzhiSystemMsg();
	}

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::AddFirstChongzhiNum(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int num = atoi(result[0]);

	WorldStatus::Instance().AddTodayFirstChongzhiCount(num);

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::ShowOpengameDay(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	const int opengame_day = GetDayIndex(SHAREDCONFIG->GetOpenServerConfig().GetOpenServerZeroTime(), EngineAdapter::Instance().Time()) + 1;

	gstr::SendError(role, "cur_opengame_day", opengame_day);

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::DisconnectCross(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int cross_type = atoi(result[0]);
	CrossManager::Instance().DisconnectFromCrossServer(cross_type);

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::GreateSoldierBianshenTrial(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int seq = atoi(result[0]);

	GREATE_SOLDIER(role)->OnBianshenTrial(seq);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ClearGreateSoldierCD(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	GREATE_SOLDIER(role)->GmClearBianshenCD();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ClearGreateSoldierInfo(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	GREATE_SOLDIER(role)->GmClearAllInfo();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ClearGreateSoldierBianshen(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	GREATE_SOLDIER(role)->ForceClearBianshenStatus();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetWorldQuestionTime(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	WorldStatus::Instance().GmSetNextQuestionTime();
	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::GmAdjustServerRoleNum(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int role_num = atoi(result[0]);

	WorldStatus::Instance().CmdSetServerRoleNum(role_num);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::GmShowAdjustServerRoleNum(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int server_role_num = WorldStatus::Instance().GetCurAdjustServerRoleNum();
	gstr::SendError(role, "cur_adjust_server_role_num", server_role_num);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::FetchTodayThemeReward(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int seq = atoi(result[0]);

	role->GetRoleActivity()->FetchTodayThemeReward(seq);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::BuyGodCostume(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int seq = atoi(result[0]);

	role->GetZeroGift()->ZeroGiftOperate(Protocol::CSZeroGiftOperate::ZERO_GIFT_BUY_GOD_COSTUME, seq, 0);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::FetchGodCostume(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int seq = atoi(result[0]);
	int level_phase = atoi(result[1]);

	role->GetZeroGift()->ZeroGiftOperate(Protocol::CSZeroGiftOperate::ZERO_GIFT_FETCH_GOD_COSTUME_REWARD_ITEM, seq, level_phase);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ZeroGiftReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int type = atoi(result[0]);
	int param_1 = atoi(result[1]);
	int param_2 = atoi(result[2]);

	role->GetZeroGift()->ZeroGiftOperate(type, param_1, param_2);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ForbidTalkReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int forbid_time = atoi(result[0]);

	role->ForbidTalk((time_t)forbid_time, "gm", "gm");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddCrossGuildKillBossRankNum(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int num = atoi(result[0]);
	GuildManager::Instance().OnAddGuildKillBossNum(role->GetGuildID(), num);

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::SetFavorableImpression(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int uid = atoi(result[0]);
	int num = atoi(result[1]);
	role->GetFriendManager()->SetFavorableImpression(IntToUid(uid), num);
	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddChuanshiFrag(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int num = atoi(result[0]);

	role->GetCrossEquip()->AddChuanshiFragment(num);

	return this->MakeupResult(netid, command->command, true, " ");
}

bool Gm::OnMythGMOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	enum MYTH_OPERA_TYPE
	{
		MYTH_OPERA_TYPE_ADDITEM,
		MYTH_OPERA_TYPE_ADD_SOUL_ESSENCE,
	};

	int opera_type = atoi(result[0]);
	int p1 = atoi(result[1]);

	if (MYTH_OPERA_TYPE_ADDITEM == opera_type)
	{
		//role->GetMyth()->TransformFromKnapsackItem(p1, p2, __FUNCTION__);
	}
	else
	{
		role->GetMyth()->GmAddEssence(p1);
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::AddZodiacMarrow(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");
	int num = atoi(result[0]);

	role->GetZodiac()->AddJinghua(num, "gm");

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::ClearZodiac(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	role->GetZodiac()->GMClearAll();

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TianshenhutiAddEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int equip_id = atoi(result[0]);

	TianshenHuti* tianshenhuti = role->GetOtherModule<TianshenHuti>(ROLE_MODULE_TYPE_TIANSHENHUTI);
	if (nullptr != tianshenhuti)
	{
		tianshenhuti->PutEuipToBackpack(equip_id);
		tianshenhuti->SendAllInfoToRole();
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TianshenhutiAddScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int score = atoi(result[0]);

	TianshenHuti* tianshenhuti = role->GetOtherModule<TianshenHuti>(ROLE_MODULE_TYPE_TIANSHENHUTI);
	if (nullptr != tianshenhuti)
	{
		tianshenhuti->AddRollScore(score, "GM");
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::TianshenhutiReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int req_type = atoi(result[0]);
	int param_1 = atoi(result[1]);
	int param_2 = atoi(result[2]);
	int param_3 = atoi(result[3]);
	int param_4 = atoi(result[4]);

	TianshenHuti* tianshenhuti = role->GetOtherModule<TianshenHuti>(ROLE_MODULE_TYPE_TIANSHENHUTI);
	if (nullptr == tianshenhuti)
	{
		return false;
	}

	switch (req_type)
	{
	case Protocol::CS_TIANSHENHUTI_REQ_TYPE_PUT_ON:
	{
		tianshenhuti->PutOnEquip(param_1);
	}
	break;

	case Protocol::CS_TIANSHENHUTI_REQ_TYPE_TAKE_OFF:
	{
		tianshenhuti->TakeOffEquip(param_1);
	}
	break;

	case Protocol::CS_TIANSHENHUTI_REQ_TYPE_TRANSFORM:
	{
		tianshenhuti->TransformEquip(param_1, param_2, param_3);
	}
	break;

	case Protocol::CS_TIANSHENHUTI_REQ_TYPE_COMBINE:
	{
		tianshenhuti->CombineEquip(param_1, param_2, param_3, param_4);
	}
	break;

	case Protocol::CS_TIANSHENHUTI_REQ_TYPE_ROLL:
	{
		tianshenhuti->RollReq(param_1);
	}
	break;

	case Protocol::CS_TIANSHENHUTI_REQ_TYPE_COMBINE_ONE_KEY:
	{
		tianshenhuti->CombineEquipOneKey(param_1);
	}
	break;
	}

	return MakeupResult(netid, command->type, true, "");
}

bool Gm::HusongShuijingReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	int param_1 = atoi(result[0]);
	int param_2 = atoi(result[1]);

	if (NULL != scene)
	{
		SpecialCrossHusongShuijing * special = dynamic_cast<SpecialCrossHusongShuijing *>(scene->GetSpecialLogic());
		if (NULL != special)
		{
			special->GmReq(param_1, param_2);
		}
	}

	return MakeupResult(netid, command->type, true, "");
}
//////////////////////////////////////////////////////////////////////////

bool Gm::SetPrintMsgType(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command)
{
	char *result[128] = {};
	this->ParseCommandParam(command->command, result, " ");

	bool is_print = 0 != atoi(result[0]);

#ifndef _WIN32
	is_print = false;	// 只有win32平台才开启这种调试功能
#endif

	MessageHandler::is_print_msg_type_from_client = is_print;
	SerializeMessageHandler::is_print_msg_type_from_client = is_print;

	return MakeupResult(netid, command->type, true, "");
}