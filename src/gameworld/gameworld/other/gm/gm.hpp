/******************************************************************************************
FileName: gm.hpp
Author: dreamwang
Description: gm系统 
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2011/06/29/ 09:04:05     1.0           创建
*****************************************************************************************/

#ifndef __GM_HPP__
#define __GM_HPP__

#include <stdio.h>

#include "gmdef.hpp"
#include "servercommon/serverdef.h"
#include "protocal/msggm.hpp"

class Scene;
class Role;

class Gm
{
public:
	static Gm & Instance();
	
	// 处理GM命令 
	bool OnCommand(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

private:
	Gm();
	~Gm();
	
	bool EquipUpLv(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool MoveRole(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ResetPos(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddFriend(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool DeleteFriend(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool GiveFlower(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddCharm(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddItemList(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool AddItemNumList(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool AddItemUse(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool KnapsackTest(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetEquipData(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool TakeOutAll(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ClearBag(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ExpandBag(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool MoveBagItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddEquipUse(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool DiscardBagItems(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool RecycleBagItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool NBRobot(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddOneTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool MoveRoleToNPC(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);	
	bool JumpToTrunk(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CommitAllTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool JumpToTrunkNoCommitBranch(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	
	bool ForceCommitTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool TestTaskRecord(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool GiveUpTaskByID(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ZhuanSheng(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddMoney(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddCoin(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool DecMoney(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddYuanli(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangeSpeed(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangeGongji(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangeFangyu(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangeMaxHP(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangeBaoji(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangeMingzhong(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangeShanbi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangeJianren(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangeFujiaShanghai(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangeDikangShanghai(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangePerJingzhun(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangePerBaoji(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangePerKangbao(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangePerPofang(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangePerMianshang(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangeHuixinyiji(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangeGedangPer(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangeCapabilityAttr(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ResetGmChangeAttr(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool Recover(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddNuqi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool JoinCamp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetProf(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool SetAttackMode(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool StopAttack(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool PerformSkill(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddEffect(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool RemoveEffect(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool LearnSkill(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddSkill(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool RemoveSkill(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangeSkillLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool TestSkill(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ShowTargets(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool Wudi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool Boss(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SpecialState(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	
	bool ViewInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ViewPos(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool SetVip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool MoveToGuildStation(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool OnGetGuildDoubleReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	
	bool SetHp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetMp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool SetLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool TurnTable(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ShopBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddTitle(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool UseTitle(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool GetTitleOwnerInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddGuildExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddGuildTotemExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ActivityToNextState(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ForceToOpenActivity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ForceToCloseActivity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddRMB(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddChongzhi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool FetchChongzhiReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddActiveDegree(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	
	bool AddMail(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CleanMail(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddMonster(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddMyMonster(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool DelMonster(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool DelAllMonster(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CallAndDrop(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddDay(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddGuildDay(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddWorldDay(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ResetData(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool WorldDayChange(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddEvil(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	// ----------------------------------------------------------------------------------------------------------------------------------

	bool RefreshWorldBossNow(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool RefreshFamilyBossNow(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool RefreshMikuBossNow(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ResetMikuInterval(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ReduceFBDayCount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ResetDayCount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetDayCount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool QuerySceneInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetMoveMode(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool TestCore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ReloadConfig(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ReloadOneItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool FetchDailyFind(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetAuthority(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetCount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetFbCount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool YiJianTiSheng(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool TrainMentality(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SpeedUp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool UpgradeGengu(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool UpgradeGenguTest(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ResetName(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool GuildResetName(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool SetSex(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddXianhun(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddShengwang(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddGongXun(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddNvWaShi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddLingJing(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddChengJiu(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddHunLi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddHuoLi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddChestShopScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool UpgradeMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ActiveMountImg(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool GoonMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool UpgradeMountTest(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddFightMount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool SetMountGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetWingGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetHaloGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetShengongGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetShenyiGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetFightMountGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetFootprintGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetShizhuangGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetShenbingGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetFabaoGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetQilinbiGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetYaoshiGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetMaskGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetToushiGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetUpgradeGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetCloakLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetUpgradeSystemGrade(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool Strengthen(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetEquipStrengthLvl(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool StrengthenTest(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool UpQuality(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool UpLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool UpEternity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ForceCompleteTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AcceptCishaTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	
	bool GetTuMoTaskInfo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CommitTuMoTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool FetchTumoReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ImediatelyCompleteTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ReflushHuSongTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool HusongBuyTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool FBEquipBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool NPCFunc(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ActivityEnterReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool EnterFB(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool GetUid(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ConverShopBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool SendGuildSos(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool SendSaleItemInfoToWorld(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddHonor(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool BuyChestShopItem(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool TestChestShop(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	
	bool ClearTrunkTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	
	bool SetCapability(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool GetCapability(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool XianMengZhanGuildCall(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool XianMengZhanGuildFollow(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool XianMengZhanAddXianli(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool Zhuanzhi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool QueryOther(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool QueryCrossOther(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool CombineServerToNext(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CombineServerReset(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CombineServerResetSelf(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddSysMailEvent(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddGuildBossExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool TestLostHP(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool TestFight(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool SetWorldLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool Standard(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool TouZiJiHuaBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool TouZiJiHuaReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool TouZiJiHuaReset(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool BaiBeiFanLiBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	
	bool RollMoneyOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddWeek(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CrossAddWeek(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool EvaluateRole(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool YaoJiangReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool MarryReserveReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool MarryXunyouOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool MarryHunyanBless(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool MarryHunyanOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool MarryHunyanInvite(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool DatingForceInvite(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool SetIsReserveMarry(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	
	bool DiscountBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddNeqScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool Divorce(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ResetDivorceTime(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ResetMarry(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool FindActivity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool BeMonthCardUser(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool BeTouZiJiHuaUser(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool SatisfyTaskCondition(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	// ----------------------------------------------------------------------------------------------------------------------------------

	bool GetSignInReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SignIn(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SignInFindBack(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool GetActivityFind(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool XiannvCall(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool XiannvRename(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool XiannvActive(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddEnergy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool TestErrnum(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool NotifySystemMsg(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool FetchVipLevelReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool FetchOpenGameActivityReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool CFResetOpponent(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CFFightReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool Wabao(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool JilianOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool TestSingleChat(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool TestChanelChat(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddFootPrint(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool GMQingyuanResetHunyan(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool GMSetZhanShenDianPassLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool FriendBless(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool GMFishPoolRaiseFish(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool GMFishPoolExtendCapacity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool GMFishPoolHarvestFish(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool GMFishSendWorldPoolList(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool GMFishPoolBuyBulletReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool GMFishPoolUpquality(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool TouchWorldEventObj(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool WorldEventNextLogicState(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool WorldEventAddScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool WorldEventRoleAddScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool WorldEventNextEventType(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool RADayChongzhiFanLiOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool RADayConsumeGoldOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool RATotalConsumeGoldOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool RADayActiveDegreeOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool RAServerPanicBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool RAPersonalPanicBuy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool RoleGoalFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool RAKillBossOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool RALoginGiftOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool RAShouYouYuXiangOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool RedPaperCreate(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool RedPaperFetch(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool WuxingUplevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ChangeCamp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChangeProf(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ChargeWantMoneyTest1(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChargeWantMoneyTest2(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ZaiChongzhiReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool MonthCardFetchDayReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool CornucopiaFetchReward(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CornucopiaTaskValue(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool CSAOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool CSFuling(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool StartCross(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool Cross1V1Match(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool Cross1V1Ready(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddCross1V1Score(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);

	bool CrossMultiuserChallengeMatchingReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CrossMultiuserChallengeCancelMatching(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CrossMultiuserChallengeScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CrossMultiuserChallengeAddGongxun(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CrossMultiuserChallengeMatchTime(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddCrossHonor(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool YaoShouGuangChangNextStatus(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SuoYaoTaNextstatus(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool PaoHuanRewardRollReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CommitPaohuanTask(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool JingHuaHusongRefresh(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CleraGlobalData(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddOnlineTime(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ChongzhiCmd(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool MysteriousshopNextStatus(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ReclacCampRank(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool GetRandBipinRewardSeq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ResetBipinFetchFlag(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CombineReset(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CmdToGuild(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CmdToGuildBox(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddKillerCount(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool PetQinmiOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ActiveAllMultiMountImg(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool MultiMountOperaReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CrossBossBuyReliveTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CloseAllRandActivity(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool RATreasuresMallTest(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddZhiBaoExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool XnActiveHuanhua(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool XiannvShengwuGm(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool SetMojieLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddWing(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddHalo(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddShengong(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddShenyi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddIntimacy(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetQingyuanBlessDays(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddWelfareHappytreeVal(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddWelfareHappytreeValByType(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool XLTKillRole(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetZhuanshenLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool LittlePetOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddHunshouExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddIdentifyExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddGather(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool EquipmentSuitReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddXLTScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool XLTBoss(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool FetchTimeLimitVip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool EnterBossFamily(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetRuneTower(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool RuneSystemOpera(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool ExpFbBuyBuff(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ExpFbBuyTimes(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ShenzhouAddIdentifyExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ShenzhouAddExp(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ShengeAddJinghua(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool GetLinuxTimeMS(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool PutXingzuoMiji(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CompoundXingzuoMiji(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool UpgradeXingling(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool UplevelZhangkong(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool GuildPaoSaizi(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool OnRoleFighting(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ActivityOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool Shenshou(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool TestSkip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddPastureScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool ElementOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool TestAddShenYinScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool TakeFeixian(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool GStrTest(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	
	bool SetPrintMsgType(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ForceOpenGuildTianCiTongBI(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool AddTianCiTongBiMaturity(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);

	bool GMShangGuBossReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetRADayActivityDegree(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool JinjiesysRewardOpera(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CrossReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	
	bool TeamTowerdefend(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool TeamFbJump(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddCarnivalScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool DecCarnivalScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CarnivalNextChapter(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool CarnivalFinish(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);

	bool RALuckyWishLottery(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool ShengwuUpgrade(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool AddShengwuEssence(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);

	bool SetShengwuLevel(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);

	bool AddShenzhouWeaponAddHunqiLevel(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);

	bool PassPataFbLayer(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);

	bool AddProfessScore(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);

	bool DisconnectCross(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool GmSetChannelChatLevel(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetChatLimit(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool SetAudioChatForbidFlag(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddFakeFirstChongzhiNum(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool AddFirstChongzhiNum(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool ShowOpengameDay(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool GreateSoldierBianshenTrial(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool ClearGreateSoldierCD(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool ClearGreateSoldierInfo(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool ClearGreateSoldierBianshen(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool SetWorldQuestionTime(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool GmAdjustServerRoleNum(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool GmShowAdjustServerRoleNum(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);

	bool FetchTodayThemeReward(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool BuyGodCostume(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool FetchGodCostume(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool ZeroGiftReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool ForbidTalkReq(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);

	bool AddChuanshiFrag(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool AddCrossGuildKillBossRankNum(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool SetFavorableImpression(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool OnMythGMOpera(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool AddZodiacMarrow(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);
	bool ClearZodiac(const GSNetID & netid, Scene * scene, Role * role, ObjID obj_id, Protocol::CSGMCommand * command);

	bool TianshenhutiAddEquip(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool TianshenhutiAddScore(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	bool TianshenhutiReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	bool HusongShuijingReq(const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);
	// ----------------------------------------------------------------------------------------------------------------------------------

	typedef bool (Gm::*CommandFunc) (const GSNetID &netid, Scene *scene, Role *role, ObjID obj_id, Protocol::CSGMCommand *command);

	struct CommandFuncItem 
	{
		char type[Protocol::MAX_GM_COMMAND_TYPE_LEN];
		CommandFunc pfnCommandHandler;
		int param_count;	
		char help[MAX_HELP_INFO_LEN];
	};

	CommandFuncItem* GetCommandFuncItem()
	{
		static CommandFuncItem stCommandFuncItem[] = 
		{
			{"equipuplv", &Gm::EquipUpLv, 5, "format: equipuplv index is_puton is_auto_buy use_lucky_item use_lucky_num" },
			{"move", &Gm::MoveRole, 4, "format: move tar_scene_id tar_scene_key x y"},
			{"resetpos", &Gm::ResetPos, 2, "format: resetpos x y"},

			{"addfriend", &Gm::AddFriend, 4, "format: addfriend roleid name sex prof"},
			{"deletefriend", &Gm::DeleteFriend, 2, "format: deletefriend roleid is_silence"},
			{"giveflower", &Gm::GiveFlower, 2, "format: item_id friend_uid"},	
			{"addcharm", &Gm::AddCharm, 1, "format: charm_value"},

			{"additem", &Gm::AddItem, 3, "format: additem item_id item_num is_bind"},
			{"additemlist", &Gm::AddItemList, 3, "format: additem begin_item_id end_item_id increase"},
			{"additemnumlist", &Gm::AddItemNumList, 4, "format: additem begin_item_id end_item_id increase item_num"},
			{"additemuse", &Gm::AddItemUse, 3, "format: additemuse item_id item_num is_bind"},
			{"knapsacktest", &Gm::KnapsackTest, 1, "format: knapsacktest value"},
			{"setequipdata", &Gm::SetEquipData, 3, "format: type equip_type value"},
			{"takeoutall", &Gm::TakeOutAll, 0, "format: takeoutall"},
			{"clearbag", &Gm::ClearBag, 0, "format: clearbag"},
			{"expandbag", &Gm::ExpandBag, 1, "format: expandbag  value"},
			{"movebagitem", &Gm::MoveBagItem, 2, "format: movebagitem  from_index to_index"},
			{"addequipuse", &Gm::AddEquipUse, 2, "format: addequipuse item_id  equip_index"},
			{"discardbagitems", &Gm::DiscardBagItems, 2, "format: discardbagitems start_index  discard_count"},
			{ "recyclebagitem", &Gm::RecycleBagItem, 2, "format: recyclebagitem start_index  discard_count" },

			{"nbrobot", &Gm::NBRobot, 0, "format:"},

			{"addexp", &Gm::AddExp, 1, "format: addexp exp"},
			{"zhuansheng", &Gm::ZhuanSheng, 0, "format:"},
			{"addmoney", &Gm::AddMoney, 1, "format: addmoney money"},
			{"addcoin", &Gm::AddCoin, 1, "format: addcoin coin"},
			{"decmoney", &Gm::DecMoney, 1, "format: decmoney money"},
			{"addyuanli", &Gm::AddYuanli, 1, "format: addyuanli yuanli"},
			{"addnuqi", &Gm::AddNuqi, 1, "format: nuqi"},
			{"changespeed", &Gm::ChangeSpeed, 1, "format: changespeed: value"},
			{"changegongji", &Gm::ChangeGongji, 1, "format: changegongji: value"},
			{"changefangyu", &Gm::ChangeFangyu, 1, "format: changefangyu: value"},
			{"changemaxhp", &Gm::ChangeMaxHP, 1, "format: changemaxhp: value"},
			{"changebaoji", &Gm::ChangeBaoji, 1, "format: value"},
			{"changemingzhong", &Gm::ChangeMingzhong, 1, "format: value"},
			{"changeshanbi", &Gm::ChangeShanbi, 1, "format: value"},
			{"changejianren", &Gm::ChangeJianren, 1, "format: value"},
			{"changefujiashanghai", &Gm::ChangeFujiaShanghai, 1, "format: value"},
			{"changedikangshanghai", &Gm::ChangeDikangShanghai, 1, "format: value"},
			{"changeperjingzhun", &Gm::ChangePerJingzhun, 1, "format: value"},
			{"changeperbaoji", &Gm::ChangePerBaoji, 1, "format: value"},
			{"changeperkangbaoi", &Gm::ChangePerKangbao, 1, "format: value"},
			{"changeperpofang", &Gm::ChangePerPofang, 1, "format: value"},
			{"changepermianshang", &Gm::ChangePerMianshang, 1, "format: value"},
			{ "changehuixinyiji", &Gm::ChangeHuixinyiji, 1, "format: value" },
			{ "changegedangper", &Gm::ChangeGedangPer, 1, "format: value" },
			{ "changezhanli", &Gm::ChangeCapabilityAttr, 1, "format: value" },
			{"recover", &Gm::Recover, 0, "format: recover"},
			{"resetgmchangeattr", &Gm::ResetGmChangeAttr, 0, "format: recover"},

			{"joincamp", &Gm::JoinCamp, 1, "format: joincamp camp"},
			{"setprof", &Gm::SetProf, 1, "format: setprof prof"},

			{"setattackmode", &Gm::SetAttackMode, 1, "format: setattackmode attack_mode"},
			{"stopattack", &Gm::StopAttack, 0, "format: stopattack"},
			{"performskill", &Gm::PerformSkill, 5, "format: performskill skill_id pos_x pos_y obj_id is_special_skill"},
			{"addeffect", &Gm::AddEffect, 11, "format: addeffect effect_type buff_type skill_id skill_level param1 param2 param3 param4 merge_rule deliverer product_method"},
			{"removeeffect", &Gm::RemoveEffect, 5, "format: removeeffect effect_type buff_type deliverer method skill_id"},

			{"learnskill", &Gm::LearnSkill, 2, "format: learnskill skill_id one_key_learn"},
			{"addskill", &Gm::AddSkill, 2, "format: addskill skill_id skill_leve"},
			{"removeskill", &Gm::RemoveSkill, 1, "format: removeskill skill_id"},
			{"changeskilllevel", &Gm::ChangeSkillLevel, 2, "format: changeskilllevel skill_id skill_leve"},

			{"testskill", &Gm::TestSkill, 5, "format: testskill skill_id pos_x pos_y obj_id special_param"},
			{"showtargets", &Gm::ShowTargets, 1, "format: showtargets range"},

			{"addonetask", &Gm::AddOneTask, 1, "format: addonetask task_id"},
			{"movetonpc", &Gm::MoveRoleToNPC, 1, "format: movetonpc npc_id"},
			{"jumptotrunk", &Gm::JumpToTrunk, 1, "format: task_id"},
			{"commitalltask", &Gm::CommitAllTask, 0, "format: "},
			{"jumptotrunknocommitbranch", &Gm::JumpToTrunkNoCommitBranch, 1, "format: task_id"},

			{"forcecommittask", &Gm::ForceCommitTask, 1, "format: forcecommittask task_id"},
			{"testtaskrecord", &Gm::TestTaskRecord, 1, "format: record_count"},
			{"giveuptask", &Gm::GiveUpTaskByID, 1, "format: task_id"},

			{"wudi", &Gm::Wudi, 0, "format: wudi"},
			{"boss", &Gm::Boss, 0, "format: boss"},
			{"specialstate", &Gm::SpecialState, 3, "format: specialstate buff_type dur_time product_level"},
				
			{"viewinfo", &Gm::ViewInfo, 1, "format: viewinfo which_info"},
			{"viewpos", &Gm::ViewPos, 1, "format: viewpos obj_id"},
			{"setvip", &Gm::SetVip, 1, "format: setvip vip_type"},
			
			{"movetoguild", &Gm::MoveToGuildStation, 3, "format: movetoguild tar_scene_id x y"},

			{"sethp", &Gm::SetHp, 1, "format: sethp value"},
			{"setmp", &Gm::SetMp, 1, "format: setmp value"},

			{"setrolelevel", &Gm::SetLevel, 1, "format: setlevel value"},

			{"turntable", &Gm::TurnTable, 1, "format: turntable type"},

			{"shopbuy", &Gm::ShopBuy, 4, "format: item_id num consume_type if_use"},
			
			{"addtitle", &Gm::AddTitle, 1, "format: title_id"},
			{"usetitle", &Gm::UseTitle, 3, "format: title_id_1 title_id_2 title_id_3"},
			{"gettitleowner", &Gm::GetTitleOwnerInfo, 0, "format: "},

			{"addguildexp", &Gm::AddGuildExp, 2, "format: type value"},	
			{"addtotemexp", &Gm::AddGuildTotemExp, 1, "format:value"},

			{"activitynextstate", &Gm::ActivityToNextState, 1, "format: type"},

			{"openactivity",  &Gm::ForceToOpenActivity, 1 ,"format: type"},

			{"closeactivity",  &Gm::ForceToCloseActivity, 1 ,"format: type"},

			{"addrmb", &Gm::AddRMB, 1, "format: addrmb"},
			{"addchongzhi", &Gm::AddChongzhi, 1, "format: addchongzhi"},
			{"fetchczr", &Gm::FetchChongzhiReward, 3, "format: type param"},

			{"adddegree", &Gm::AddActiveDegree, 1, "format: activedegree_type"},
			
			{"addmail", &Gm::AddMail, 1, "format: addmail virtual_item_value"},
			{"cleanmail", &Gm::CleanMail, 0, "format: cleanmail"},

			{"addmonster", &Gm::AddMonster, 2, "format: addmonster id count"},
			{"addmymonster", &Gm::AddMyMonster, 1, "format: addmymonster id"},
			{"delmonster", &Gm::DelMonster, 1, "format: delmonster id"},
			{"delallmonster", &Gm::DelAllMonster, 0, "format: delallmonster"},
			{"callanddrop", &Gm::CallAndDrop, 3, "format: monster_id, count clean"},

			{"addday", &Gm::AddDay, 1, "format: addday pass_days"}, 
			{"addguildday", &Gm::AddGuildDay, 1, "format: addday pass_days" },
			{"addworldday", &Gm::AddWorldDay, 1, "format: addday pass_days" },
			{"resetdata", &Gm::ResetData, 1, "format: resetdata times"}, 
			{ "worldaddday", &Gm::WorldDayChange, 0, "format:" },

			{"refreshworldboss", &Gm::RefreshWorldBossNow, 0, "format:"},
			{"refreshfamilyboss", &Gm::RefreshFamilyBossNow, 0, "format:"},
			{"refreshmikuboss", &Gm::RefreshMikuBossNow, 0, "format:"},
			{"resetmiku", &Gm::ResetMikuInterval, 0, "format:"},

			{"reducefb", &Gm::ReduceFBDayCount, 1, "format: reducefb fb_id"},
			{"resetdaycount", &Gm::ResetDayCount, 0, "format: resetdaycount"},
			{"setdaycount", &Gm::SetDayCount, 2, "format: setdaycount daycount_id count"},
			
			{"addevil", &Gm::AddEvil, 1, "format: addevil value"},

			{"querysceneinfo", &Gm::QuerySceneInfo, 0, "format: querysceneinfo"},

			{"setmovemode", &Gm::SetMoveMode, 2, "format: setmovemode move_mode move_mode_param"},

			{"testcore", &Gm::TestCore, 0, "format:testcore"},
			{"reloadconfig", &Gm::ReloadConfig, 1, "format:reloadconfig module"},
			{"reloadoneitem", &Gm::ReloadOneItem, 2, "format:reloadoneitem reload_item_type, item_id"},

			{"fetchdailyfind", &Gm::FetchDailyFind, 2, "format:fetchdailyfind dailyfind_type get_type"},

			{"setauthrity", &Gm::SetAuthority, 1, "format: type"},

			{"setcount", &Gm::SetCount, 2, "format: type count"},
			{"setfbcount", &Gm::SetFbCount, 2, "format: day_count_id count"},

			{"yijiantisheng", &Gm::YiJianTiSheng, 0, "format:"},
			{"trainmentality", &Gm::TrainMentality, 0, "format:"},
			{"speedup", &Gm::SpeedUp, 0, "format:"},
			{"upgradegengu", &Gm::UpgradeGengu, 3, "format: type use_protect_item is_auto_buy"},
			{"upgradegengutest", &Gm::UpgradeGenguTest, 3, "format: start_level end_level test_count"},

			{"rename", &Gm::ResetName, 1, "format: rename name"},
			{"guildrename", &Gm::GuildResetName, 1, "format: guildrename name"},

			{"setsex", &Gm::SetSex, 1, "format: sex"},

			{"addxianhun", &Gm::AddXianhun, 1, "format: change"},
			{"addshengwang", &Gm::AddShengwang, 1, "format: change"},
			{"addgongxun", &Gm::AddGongXun, 1, "format: change"},
			{"addnvwashi", &Gm::AddNvWaShi, 1, "format: change"},
			{"addlingjing", &Gm::AddLingJing, 1, "format: change"},
			{"addchengjiu", &Gm::AddChengJiu, 1, "format: change"},
			{"addhunli", &Gm::AddHunLi, 1, "format: change"},
			{"addhuoli", &Gm::AddHuoLi, 1, "format: change"},
			{ "addchestshopscore", &Gm::AddChestShopScore, 1, "format: change" },

			{"addmount", &Gm::AddMount, 0, "format: addmount  "},
			{"addfightmount", &Gm::AddFightMount, 0, "format: addfightmount  "},
			{"mountupgradetest", &Gm::UpgradeMountTest, 2, "format: index, count"},
			{"mountupgrade", &Gm::UpgradeMount, 2, "format: mountindex, auto_buy"},
			{"activemountimg", &Gm::ActiveMountImg, 1, "format: imgid"},
			{"goonmount", &Gm::GoonMount, 1, "format: is_on"},

			{ "setmountgrade", &Gm::SetMountGrade, 1, "format: grade  " },
			{ "setwinggrade", &Gm::SetWingGrade, 1, "format: grade  " },
			{ "sethalograde", &Gm::SetHaloGrade, 1, "format: grade  " },
			{ "setshengonggrade", &Gm::SetShengongGrade, 1, "format: grade  " },
			{ "setshenyigrade", &Gm::SetShenyiGrade, 1, "format: grade  " },
			{ "setfightmountgrade", &Gm::SetFightMountGrade, 1, "format: grade  " },
			{ "setfootprintgrade", &Gm::SetFootprintGrade, 1, "format: grade  " },
			{ "setshizhuanggrade", &Gm::SetShizhuangGrade, 1, "format: grade  " },
			{ "setshenbinggrade", &Gm::SetShenbingGrade, 1, "format: grade  " },
			{ "setcloaklevel", &Gm::SetCloakLevel, 1, "format: level  " },

			{"strengthen", &Gm::Strengthen, 5, "format:is_on_body,equip_idx,auto_buy,bind_first,use_lucky_stuff"},
			{"strengthentest", &Gm::StrengthenTest, 2, "format: index round"},
			{"setequipstrengthlevel", &Gm::SetEquipStrengthLvl, 2, "format: index level"},
			{"upquality", &Gm::UpQuality, 3, "format:is_puton, equip_idx, select_bind_first"},
			{ "uplevel", &Gm::UpLevel, 4, "format:is_puton, equip_idx, select_bind_first, use_protect_stuff" },
			{ "upeternity", &Gm::UpEternity, 2, "format:equip_idx, is_auto_buy" },

			{"forcecompletetask", &Gm::ForceCompleteTask, 1, "format: task_id"},
			{"acceptcishatask", &Gm::AcceptCishaTask, 0, "format: "},

			{"gettumotaskinfo", &Gm::GetTuMoTaskInfo, 0, "format:"},
			{"committumotask", &Gm::CommitTuMoTask, 2, "format: commit_all, task_id"},
			{"fetchtumoreward", &Gm::FetchTumoReward, 0, "format: "},
			{"immediatelycompletetask", &Gm::ImediatelyCompleteTask, 1, "format: task_id"},

			{"refreshhusongtask", &Gm::ReflushHuSongTask, 2, "format: is_autoflush, to_color"},
			{"husongbuytimes", &Gm::HusongBuyTimes, 0, "format: "},
		
			{"fbequipbuy", &Gm::FBEquipBuy, 2, "format:shop_item_seq, is_personal"},

			{"npcfunc", &Gm::NPCFunc, 4, "format:npc_id, func_idx, param1, param2"},

			{"activityenterreq", &Gm::ActivityEnterReq, 2, "format:activity_type, room_index"},
			
			{"enterfb", &Gm::EnterFB, 4, "format: enterfb fb_type param1 param2 param3"},

			{"getuid", &Gm::GetUid, 0, "format:getuid"},

			{"convershopbuy", &Gm::ConverShopBuy, 3, "format: conver_type, conver_index, conver_num"},

			{"sendguildsos", &Gm::SendGuildSos, 1, "format: sos_type"},

			{"sendsaleiteminfotoworld", &Gm::SendSaleItemInfoToWorld, 1, "format: sale_index"},

			{"addhonor", &Gm::AddHonor, 1, "format: honor_value"},

			{"buychestshopitem", &Gm::BuyChestShopItem, 2, "format: type[0-1] mode[1-3]"},
			{"testchestshop", &Gm::TestChestShop, 3, "format: type[0-1] mode[1-3] times"},

			{"cleartrunktask", &Gm::ClearTrunkTask, 3, "format: start_task_id is_clear_zhi_xian add_start_task"},
			
			{"setcapability", &Gm::SetCapability, 1, "format: capability"},
			{"getcapability", &Gm::GetCapability, 1, "format: cap_type"},

			{"xmzguildcall", &Gm::XianMengZhanGuildCall, 1, "format: use_gold"},
			{"xmzguildfollow", &Gm::XianMengZhanGuildFollow, 0, "format:"},
			{"zhuanzhi", &Gm::Zhuanzhi, 0, "format: zhuanzhi"},
			{"getguilddoublereward", &Gm::OnGetGuildDoubleReward, 0, "format: double_reward"},

			{"queryother", &Gm::QueryOther, 1, "format: other_uid"},
			{"querycrossother", &Gm::QueryCrossOther, 2, "format: other_play_type other_uid"},

			{"sysmail", &Gm::AddSysMailEvent, 7, "format:user_id item_id virtual_item_type virtual_item_num bind_gold bind_coin gold"},
			{"addguildbossexp", &Gm::AddGuildBossExp, 1, "format:add_exp"},

			{"testlosthp", &Gm::TestLostHP, 1, "format:target_obj_id"},
			{"testfight", &Gm::TestFight, 2, "format:monsterid skill_level"},

			{"setworldlevel", &Gm::SetWorldLevel, 1, "format:"},

			{"standard", &Gm::Standard, 0, "format:"},

			{"tzjhbuy", &Gm::TouZiJiHuaBuy, 0, "format:"},
			{"tzjhreward", &Gm::TouZiJiHuaReward, 2, "format: plan_type seq"},
			{"tzjhreset", &Gm::TouZiJiHuaReset, 0, "format:"},
			{"bbflbuy", &Gm::BaiBeiFanLiBuy, 0, "format:"},

			{"rollmoney", &Gm::RollMoneyOpera, 1, "format: opera_type"},

			{"addweek", &Gm::AddWeek, 0, "format:"},
			{"crossaddweek", &Gm::CrossAddWeek, 0, "format:"},

			{"evaluaterole", &Gm::EvaluateRole, 3, "format:rank_type is_admire times"},

			{"combineserverreset", &Gm::CombineServerReset, 0, "format:"},
			{"combineserverresetself", &Gm::CombineServerResetSelf, 0, "format:"},
			{"combineservernext", &Gm::CombineServerToNext, 0, "format:"}, 

			{"yaojiang", &Gm::YaoJiangReq, 1, "format:yaojiang yaojiang_type"},

			{"marryreserve", &Gm::MarryReserveReq, 5, "format: reserved_index marry_level is_clear is_force_start is_force_next_status"},
			{"xunyouopera", &Gm::MarryXunyouOpera, 1, "format: opera_type"},
			{"hunyanbless", &Gm::MarryHunyanBless, 2, "format: bless_uid gift_index"},
			{"hunyanopera", &Gm::MarryHunyanOpera, 3, "format: opera_type opera_param invited_uid"},
			{"hunyaninvite", &Gm::MarryHunyanInvite, 3, "format: be_invite_uid rune_index is_free_invite"},

			{"datinginvite", &Gm::DatingForceInvite, 1, "format: uid"},

			{"setisreservemarry", &Gm::SetIsReserveMarry, 1, "format: is_reserve_marry"},

			{"discountbuy", &Gm::DiscountBuy, 1, "format: seq"},

			{"addneqscore", &Gm::AddNeqScore, 1, "format: score"},

			{"divorce", &Gm::Divorce, 1, "format: divorce clear_intimacy"},

			{"addintimacy", &Gm::AddIntimacy, 1, "format: intimacy value"},

			{"resetdivorcetime", &Gm::ResetDivorceTime, 0, "format: resetdivorcetime"},
			{"resetmarry", &Gm::ResetMarry, 0, "format: resetmaryy"},

			{"findactivity", &Gm::FindActivity, 2, "format:  is_find, activity_find_type"},

			{"bemonthcarduser", &Gm::BeMonthCardUser, 0, "format:"},

			{"betouzijihuauser", &Gm::BeTouZiJiHuaUser, 1, "format: plan_type"},

			{"satisfytaskcond", &Gm::SatisfyTaskCondition, 3, "format: task_condition_type param1 param2"},

			{"getsigninreward", &Gm::GetSignInReward, 1, "format: day"},
			{"signin",&Gm::SignIn, 1, "format: day"},
			{"signinfindback", &Gm::SignInFindBack, 0, "format: "},
			{"getactivityfind", &Gm::GetActivityFind, 2, "format: find_type is_free_find"},

			{"xvcall", &Gm::XiannvCall, 4, "format: xiannv_id"},
			{"xvrename", &Gm::XiannvRename, 1, "format: xiannv_id"},
			{"xvactive", &Gm::XiannvActive, 1, "format: xiannv_id"},

			{"addenergy", &Gm::AddEnergy, 1, "format: energy"},

			{"testerrnum", &Gm::TestErrnum, 1, "format: err_num"},

			{"systemmsg", &Gm::NotifySystemMsg, 2, "format: msg_type content"},

			{"fetchviplevelreward", &Gm::FetchVipLevelReward, 1, "format: seq"},

			{"fetchogareward", &Gm::FetchOpenGameActivityReward, 2, "format: type seq"},

			{"cfresetopponent", &Gm::CFResetOpponent, 0, "format: "},
			{"cffightreq", &Gm::CFFightReq, 4, "format: opponent_index"},

			{"wabao", &Gm::Wabao, 1, "format: opera_type"},
			{"jilian", &Gm::JilianOpera, 2, "format: opera_type param"},

			{"testsinglechat", &Gm::TestSingleChat, 1, "format: content"},
			{"testchanelchat", &Gm::TestChanelChat, 1, "format: content"},

			{"addfootprint", &Gm::AddFootPrint, 0, "format:addfootprint"},

			{"resethunyan", &Gm::GMQingyuanResetHunyan, 0, "format: "},

			{"setzhanshendianlevel", &Gm::GMSetZhanShenDianPassLevel, 1, "format:set_level "},	

			{"friendbless", &Gm::FriendBless, 2, "format:opera_type param "},	

			{"fishraisefish", &Gm::GMFishPoolRaiseFish, 1, "format: fish_type"},
			{"fishextendcapacity", &Gm::GMFishPoolExtendCapacity, 1, "format: extend_type"},
			{"fishharvestfish", &Gm::GMFishPoolHarvestFish, 0, "format:"},
			{"fishsendworldpoollist", &Gm::GMFishSendWorldPoolList, 0, "format:"},
			{"fishbuybulletreq", &Gm::GMFishPoolBuyBulletReq, 0, "format:"},
			{"fishpoolupquality", &Gm::GMFishPoolUpquality, 0, "format:"},

			{"wetouch", &Gm::TouchWorldEventObj, 1, "format: objid"},
			{"wenextstate", &Gm::WorldEventNextLogicState, 0, "format:"},
			{"wenextevent", &Gm::WorldEventNextEventType, 0, "format:"},
			{"weaddscore", &Gm::WorldEventAddScore, 1, "format: add_score"},
			{"weaddrolescore", &Gm::WorldEventRoleAddScore, 2, "format: event_type, add_score"},
			{"radaychongzhifanli", &Gm::RADayChongzhiFanLiOpera, 2, "format: opera, reward_seq"},
			{"radayconsumegold", &Gm::RADayConsumeGoldOpera, 2, "format: opera, reward_seq"},
			{"raserverpanicbuy", &Gm::RAServerPanicBuy, 2, "format: opera_type, param"},
			{"rapersonalpanicbuy", &Gm::RAPersonalPanicBuy, 2, "format: opera_type, param"},
			{"ratotalconsumegold", &Gm::RATotalConsumeGoldOpera, 2, "format: opera, param"},
			{"radayactivedegree", &Gm::RADayActiveDegreeOpera, 2, "format: opera, param"},

			{"rolegoalfetchreward", &Gm::RoleGoalFetchReward, 0, "format:"},
			{ "rakillboss", &Gm::RAKillBossOpera, 1, "format: opera" },
			{ "ralogingiftopera", &Gm::RALoginGiftOpera, 2, "format: opera, param" },
			{ "rashouyouyuxiang", &Gm::RAShouYouYuXiangOpera, 2, "format: opera, param" },

			{ "redpapercreate", &Gm::RedPaperCreate, 4, "format: type, gold_num, can_fetch_times, is_bind_gold" },
			{ "redpaperfetch", &Gm::RedPaperFetch, 1, "format: id" },

			{ "wuxinguplevel", &Gm::WuxingUplevel, 2, "format: wx_index is_auto_buy" },

			{ "changecamp", &Gm::ChangeCamp, 1, "format: changecamp camp" },
			{ "changeprof", &Gm::ChangeProf, 2, "format: changecamp prof sex" },

			{ "chargeWantMoneyTest1", &Gm::ChargeWantMoneyTest1, 0, "format:" },
			{ "chargeWantMoneyTest2", &Gm::ChargeWantMoneyTest2, 0, "format:" },

			{ "zaichongzhireward", &Gm::ZaiChongzhiReward, 0, "format:" },

			{ "monthcardfatchdayreward", &Gm::MonthCardFetchDayReward, 0, "format:" },

			{ "cornucopiafetchreward", &Gm::CornucopiaFetchReward, 0, "format:" },
			{ "cornucopiavalue", &Gm::CornucopiaTaskValue, 2, "format:task_type value" },

			{ "csaoperareq", &Gm::CSAOperaReq, 3, "format: sbu_typ param_1 param_2" },

			{ "csfuling", &Gm::CSFuling, 1, "format: equip_index" },

			{ "startcross", &Gm::StartCross, 7, "format: activity_type, is_force, param1, param2, param3, move_to_x, move_to_y" },
			{ "addcrosshonor", &Gm::AddCrossHonor, 1, "format: cross honor" },
			{ "cross1v1match", &Gm::Cross1V1Match, 0, "format:" },
			{ "cross1v1ready", &Gm::Cross1V1Ready, 0, "format:" },
			{ "cross1v1score", &Gm::AddCross1V1Score, 2, "format: type score" },
			{ "crossmcmatching", &Gm::CrossMultiuserChallengeMatchingReq, 0, "format:" },
			{ "crossmccancelmatching", &Gm::CrossMultiuserChallengeCancelMatching, 0, "format:" },
			{ "cross3v3score", &Gm::CrossMultiuserChallengeScore, 1, "format: score" },
			{ "cross3v3gongxun", &Gm::CrossMultiuserChallengeAddGongxun, 1, "format:add_gongxun" },
			{ "cross3v3matchtime", &Gm::CrossMultiuserChallengeMatchTime, 1, "format:add_matchtime" },

			{ "ysgcnextstatus", &Gm::YaoShouGuangChangNextStatus, 0, "format:" },
			{ "sytnextstatus", &Gm::SuoYaoTaNextstatus, 0, "format:" },

			{ "skippaohuan", &Gm::CommitPaohuanTask, 0, "format:" },
			{ "rollpaohuan", &Gm::PaoHuanRewardRollReq, 0, "format:" },
			{ "jinghuahusongrefresh", &Gm::JingHuaHusongRefresh, 0, "format:" },
			{ "clearactglobaldata", &Gm::CleraGlobalData, 0, "format:" },
			{ "addonlinetime", &Gm::AddOnlineTime, 1, "format: minute" },
			{ "chongzhicmd", &Gm::ChongzhiCmd, 1, "format: minute" },
			{ "mysteriousshopnextstatus", &Gm::MysteriousshopNextStatus, 0, "format: minute" },
			{ "camprank", &Gm::ReclacCampRank, 1, "format: minute" },

			{ "bipintest", &Gm::GetRandBipinRewardSeq, 2, "format: activity_type seq" },
			{ "resetbipindata", &Gm::ResetBipinFetchFlag, 0, "format: " },
			{ "combineresetdata", &Gm::CombineReset, 0 ,"foramat: " },
			{ "cmdtoguild", &Gm::CmdToGuild, 2, "format:guild_id, oper_type" },
			{ "guildbox", &Gm::CmdToGuildBox, 3, "format:oper_type param1 param2" },
			{ "addkillcount", &Gm::AddKillerCount, 2, "format:type, count" },
			{ "petqinmi", &Gm::PetQinmiOpera, 3, "format:type, param1, param2" },
			{ "activeallmultimountimg", &Gm::ActiveAllMultiMountImg, 0, "format:" },
			{ "multimountoperareq", &Gm::MultiMountOperaReq, 4, "format:type, param1, param2, param3" },
			{ "crossbossbuyrelivetimes", &Gm::CrossBossBuyReliveTimes, 1, "format:times" },
			{ "closeallrandact", &Gm::CloseAllRandActivity, 0, "format:" },
			{ "treasuremall", &Gm::RATreasuresMallTest, 3, "format:opera, param1, param2" },
			{ "addzhibaoexp", &Gm::AddZhiBaoExp, 1, "format:value" },							// 仙令
			{ "xvactivehuanhua", &Gm::XnActiveHuanhua, 2, "format:huanhua_id, item_index" },
			{ "xnshengwu", &Gm::XiannvShengwuGm, 4, "format:type param1 param2 param3" },

			{ "setmojielevel", &Gm::SetMojieLevel, 2, "format:mojie_type,is_auto_buy" },

			{ "addwing", &Gm::AddWing, 0, "format: addwing" },
			{ "addhalo", &Gm::AddHalo, 0, "format: addhalo" },
			{ "addshengong", &Gm::AddShengong, 0, "format: addshengong" },
			{ "addshenyi", &Gm::AddShenyi, 0, "format: addshenyi" },
			{ "setqingyuanblessdays", &Gm::SetQingyuanBlessDays, 1, "format: days" },
			{ "addhappytreevalue", &Gm::AddWelfareHappytreeVal, 1, "format: value" },
			{ "addhappytreevaluebytype", &Gm::AddWelfareHappytreeValByType, 2, "format:type ,value" },
			{ "xltkillrole", &Gm::XLTKillRole, 0, "format:" },
			{ "setzhuanshenglevel", &Gm::SetZhuanshenLevel, 1, "format:level" },
			{ "littlepet", &Gm::LittlePetOpera, 4, "format:type, param1, param2, param3" },
			{ "addhunshouexp", &Gm::AddHunshouExp, 1, "format: add_exp" },
			{ "addidentifyexp", &Gm::AddIdentifyExp, 1, "format:exp" },
			{ "addgather", &Gm::AddGather, 1, "format: gather_id" },
			{ "equipsuitreq", &Gm::EquipmentSuitReq, 2, "format:operate_type, equip_index" },
			{ "addxltscore", &Gm::AddXLTScore, 1, "format: add_score" },
			{ "xltboss", &Gm::XLTBoss, 0, "format:" },
			{ "timelimitvip", &Gm::FetchTimeLimitVip, 0, "format:" },
			{ "enterbossramily", &Gm::EnterBossFamily, 1, "format:boss_type scene_id" },
			{ "setrunetower", &Gm::SetRuneTower, 2, "format: layer" },
			{ "runesystem", &Gm::RuneSystemOpera, 5, "format:type p1 p2 p3 p4" },
			{ "expfbbuybuff", &Gm::ExpFbBuyBuff, 0, "format:" },
			{ "expfbbuytimes", &Gm::ExpFbBuyTimes, 0, "format:" },
			{ "addidentifyexp", &Gm::ShenzhouAddIdentifyExp, 1, "format: exp" },
			{ "addshenzhouexp", &Gm::ShenzhouAddExp, 1, "format: exp" },
			{ "addshengejinghua", &Gm::ShengeAddJinghua, 1, "format: jinghua" },
			{ "getlinuxtimems", &Gm::GetLinuxTimeMS, 0, "format:" },
			{ "putxingzuomiji", &Gm::PutXingzuoMiji, 2, "format: zodiac_type, miji_index" },
			{ "compoundxingzuomiji", &Gm::CompoundXingzuoMiji, 3, "format: index1, index2, index3" },
			{ "upgradexingling", &Gm::UpgradeXingling, 2, "format: id, is_auto_buy" },
			{ "uplevelzhangkong", &Gm::UplevelZhangkong, 1, "format: type" },
			{ "guildpaosaizi", &Gm::GuildPaoSaizi, 0, "format" },
			{ "rolefighting", &Gm::OnRoleFighting, 1, "format: type" },
			{ "activityopera", &Gm::ActivityOpera, 4, "format: activity_type, type, param_1, param_2" },
			{ "shenshou", &Gm::Shenshou, 4, "format:type p1 p2 p3" },
			{ "testskip", &Gm::TestSkip, 2, "format:type p1" },
			{ "addpasturescore", &Gm::AddPastureScore, 1, "format:type p1" },
			{ "element", &Gm::ElementOpera, 6, "format:type p1 ..." },
			{ "shenyin", &Gm::TestAddShenYinScore, 2, "format: type, num" },
			
			{ "feixian", &Gm::TakeFeixian, 2, "format: type" },

			{ "shanggu", &Gm::GMShangGuBossReq, 2, "format: type p1 p2 p3" },

			{ "gamestringtest", &Gm::GStrTest, 3, "format:type param1 param2" },

			{ "setdegree", &Gm::SetRADayActivityDegree, 1, "format:degree" },

			{ "forceopentiancitongbi", &Gm::ForceOpenGuildTianCiTongBI, 0, "format:" },
			{ "addtiancitongbimaturity", &Gm::AddTianCiTongBiMaturity, 1, "format:param1" },
			{ "jinjiesysopera", &Gm::JinjiesysRewardOpera, 3, "format: opera_type, sys_type, reward_type" },
			{ "crossreq", &Gm::CrossReq, 3, "format: opera_type, param_1, param_2" },

			{ "teamfb", &Gm::TeamFbJump, 1, "format: to_layer" },
			{ "addcarnivalscore", &Gm::AddCarnivalScore, 2, "format: chapter, score" },
			{ "deccarnivalscore", &Gm::DecCarnivalScore, 2, "format: chapter, score" },
			{ "carnivalnextchapter", &Gm::CarnivalNextChapter, 1, "format: chapter" },
			{ "carnivalfinish", &Gm::CarnivalFinish, 3, "format: chapter reward_index flag" },
			{ "raluckywishlottery", &Gm::RALuckyWishLottery, 2, "format: type, repeat_times" },
			{ "shengwuupgrade", &Gm::ShengwuUpgrade, 1, "format: shengwu_id" },
			{ "addshengwuessence", &Gm::AddShengwuEssence, 1, "format: number" },
			{ "setshengwulevel", &Gm::SetShengwuLevel, 2, "format: shengwu_id level" },
			{ "addshenzhouweaponaddhunqilevel", &Gm::AddShenzhouWeaponAddHunqiLevel, 1, "format: hunqi_id" },
			{ "passpatafblayer", &Gm::PassPataFbLayer, 1, "format: layer" },
			{ "addprofessscore", &Gm::AddProfessScore, 1, "format: score" },

			{ "gmsetchatlevel", &Gm::GmSetChannelChatLevel, 2, "format: channel_type limit_level" },
			{ "setchatlimit", &Gm::SetChatLimit, 4, "format : channel_type role_level vip_level limit_type" },
			{ "forbidaudiochat", &Gm::SetAudioChatForbidFlag, 1, "format: audio_forbid_flag" },
			{ "addfakefirstchongzhinum", &Gm::AddFakeFirstChongzhiNum, 1, "format: num" },
			{ "addfirstchongzhinum", &Gm::AddFirstChongzhiNum, 1, "format: num" },

			{ "printmsgtype", &Gm::SetPrintMsgType, 1, "format:is_print" },							// 设置是否打印从客户端接收到的协议号
			{ "teamshouhu",&Gm::TeamTowerdefend,1, "format:wave"},									// 组队守护到多少波
				
			{ "disconnectcross", &Gm::DisconnectCross, 1, "format:cross_type" },

			{ "showopengameday", &Gm::ShowOpengameDay, 0, "format:" },

			{ "gsbianshentrial", &Gm::GreateSoldierBianshenTrial, 1, "format:seq" },				// 名将变身体验 /gm gsbianshentrial:0
			{ "cleargscd", &Gm::ClearGreateSoldierCD, 0, "format:" },
			{ "cleargsinfo", &Gm::ClearGreateSoldierInfo, 0, "format:" },
			{ "cleargsbianshen", &Gm::ClearGreateSoldierBianshen, 0, "format:" },
			{ "worldquestion",&Gm::SetWorldQuestionTime, 0, "format:"},								// 设下次世界答题时间为当前
			{ "adjustrolenum",&Gm::GmAdjustServerRoleNum, 1, "format:server_role_num" },			// 设置服务器导入人数
			{ "showadjustrolenum",&Gm::GmShowAdjustServerRoleNum, 0, "format:" },					// 显示当前服务器导入人数
			{ "fetchtodaythemereward",&Gm::FetchTodayThemeReward, 1, "format:seq" },				//领取今日主题奖励
			{ "buygodcostume",&Gm::BuyGodCostume, 1, "format:seq" },								//购买四阶红装
			{ "fetchgodcostume",&Gm::FetchGodCostume, 2, "format:seq level_phase" },				//领取四阶红装奖励
			{ "zerogiftreq",&Gm::ZeroGiftReq, 3, "format:type seq level_phase" },					//零元礼包请求
			{ "forbidtalk",&Gm::ForbidTalkReq, 1, "format:forbid_time" },							//禁言
			{ "addcrossguildbossnum",&Gm::AddCrossGuildKillBossRankNum, 1, "format:num" },			//跨服仙盟bos击杀榜
			{ "setfavorableimpression",&Gm::SetFavorableImpression, 2, "format:friend_uid number" },//设置好感度
			{ "addchuanshifrag", &Gm::AddChuanshiFrag, 1, "format:num" },							// 增加传世碎片
			{ "myth", &Gm::OnMythGMOpera, 5, "format:" },
			{ "addzodiacmarrow", &Gm::AddZodiacMarrow, 1, "format:num" },
			{ "clearzodiac", &Gm::ClearZodiac, 0, "format:" },

			{ "tianshenhutiaddequip", &Gm::TianshenhutiAddEquip, 1, "format:equip_id" },
			{ "tianshenhutiaddscore", &Gm::TianshenhutiAddScore, 1, "format:score" },
			{ "tianshenhutireq", &Gm::TianshenhutiReq, 5, "format:req_type param1 param2 param3" },
			{ "husongshuijingreq",&Gm::HusongShuijingReq, 2 ,"format:param1 param2" },
				
			{"null", NULL, 0, "format:"},
		};

		return &stCommandFuncItem[0];
	}

	bool MakeupResult(const GSNetID &netid, const char *commandType, bool ret, const char *addition_str);
	
	// ------------------------------------------------------------------------------

	// @brief 按空格分析字符串 并分解到一个数组中
	// @src 源字符串 
	// @result 目的缓冲区
	// @spilt 分解字符串 
	// @return 返回多少个参数 
	int ParseCommandParam(char *src, char **result, const char *spilt);
};

#endif  // __GM_HPP__

