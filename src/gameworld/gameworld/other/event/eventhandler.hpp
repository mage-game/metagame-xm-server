/******************************************************************************************
FileName: eventhander.hpp
Author: dreamwang
Description: 事件处理器
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2012/03/09/ 17:04:05     1.0           创建
*****************************************************************************************/

#ifndef __EVENT_HANDER_HPP__
#define __EVENT_HANDER_HPP__

class Monster;

#include "gamedef.h"
#include "servercommon/basedef.h"
#include "item/itemextern.hpp"
#include "servercommon/rankdef.hpp"
#include "global/globalrole/globalrolemanager.hpp"

class Character;
class Role;
class FallingItem;
class RoleShadow;

class EventHandler
{
public:
	static EventHandler & Instance();

	void OnRoleHurtMonster(Role *role, Monster *monster, Attribute &hurt);
	void OnMonsterDisapper(Monster *monster);																		// 怪物自己消失
	void OnKillerRoleKillMonster(Role *role, Monster *monster);														// 真正的击杀者，只有一个
	void OnKillerTeamKillMonster(Role *role, Monster *monster);														// 队伍击杀怪
	void OnKillMonster(Role *role, int scene_id, const Posi &pos, int monster_id, int monster_level);				// 杀怪
	void OnStartGather(Role *role, int gather_id, ObjID obj_id, const Posi &gather_pos);							// 开始采集 
	void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &gather_pos);									// 采集
	void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);					// 拾取掉落

	void OnCompleteTask(Role *role, TaskID task_id);																// 完成任务
	void OnChangeCapability(Role *role, int cap_type);																// 战斗力改变
	void OnRoleUpgradeLevel(Role *role, int old_level, int new_level);												// 角色升级

	void OnRoleRealiveSucc(Role *role, int realive_type);															// 角色复活成功

	void OnAddFriend(Role *role);																					// 添加好友
	void OnResetName(Role *role, GameName name);																	// 更改名字

	void OnJoinActivity(Role *role, int activity_type);																// 参加活动

	bool OnRMBBuy(Role* role, int chongzhi_num);
	void OnAddChongzhi(Role *role, long long chongzhi_gold, bool is_notice_mail = true);							// 充值增加
	void OnConsumeGold(Role *role, long long consume_gold);															// 消费元宝
	void OnCheckCrossConsumeGold(Role *role, long long total_consume_gold, long long daily_consume_gold);			// 检查跨服消费元宝
	void OnAddGoldBind(Role *role, long long gold_bind);															// 增加绑定钻石时

	void OnShopBuyItem(Role *role, int buy_count, const ItemExtern::BuyItemInfo buy_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM], long long consume_gold, long long consume_bind_gold);		// 商城购买物品
	void OnGiveFlower(Role *role, int flower_num);																	// 赠送鲜花
	void OnBeGiveFlower(Role *role);																				// 收到鲜花
	void OnVipLevelChange(Role *role);																				// Vip等级改变
	void OnMountUpgrade(Role *role, int old_grade, int to_grade);													// 坐骑进阶
	void OnFightMountUpgrade(Role *role, int old_grade, int to_grade);												// 战斗坐骑进阶
	void OnQibingUpgrade(Role *role, int old_grade, int to_grade);													// 麒麟臂进阶
	void OnCloakUpLevel(Role *role, int old_grade, int to_grade);													// 披风升级

	void OnMountUpStar(Role *role, int old_star, int to_star);														// 坐骑升星
	void OnWingUpStar(Role *role, int old_star, int to_star);														// 羽翼升星
	void OnHaloUpStar(Role *role, int old_star, int to_star);														// 光环升星
	void OnShengongUpStar(Role *role, int old_star, int to_star);													// 神弓升星
	void OnShenyiUpStar(Role *role, int old_star, int to_star);														// 神翼升星
	void OnFightMountUpStar(Role *role, int old_star, int to_star);													// 战斗坐骑升星
	void OnFootprintUpStar(Role *role, int old_star, int to_star);														// 光环升星

	void OnPutonEquip(Role *role, short equip_type, short color, short quality, short level, short xian_pin_count);	// 穿戴装备
	void OnTakeOffEquip(Role *role);																				// 脱下装备

	void OnPutOnZhuanZhiEquip(Role *role, short equip_type, short color, short level);								// 穿戴转职装备
	void OnForgeZhuanZhiSuit(Role *role);																			// 锻造转职装备套装

	void OnPutOnBaizhanEquip(Role *role, short equip_type, short color, short level);								// 穿戴百战装备
	
	void OnStrengthenConsumeCoin(Role *role, int consume_coin);														// 强化消耗铜钱
	void OnKillWorldBoss(Role *role);																				// 击杀世界BOSS
	void OnKillMiZangBoss(Role *role);																				// 击杀秘藏BOSS
	void OnKillCrossBoss(Role * role);																				// 击杀跨服boss
	void OnKillRoleOnCrossBossScene(Role *role);																	// 跨服boss击杀玩家

	void OnAddShengwang(Role *role, int add_shengwang);															    // 增加声望

	void OnChangeCamp(Role *role, int camp);																		// 改变阵营
	void OnItemCompose(Role *role, int product_id, int num);														// 合成物品
	void OnPetOpenEgg(Role *role);																					// 开宠物蛋

	void OnEquipShenzhu(Role *role);																				// 神铸
	void OnReCalcAttr(Role *role, bool is_init);																					// 星座精灵篇完成条件之一
	void OnStrengthenEquipOperate(Role *role, int equip_index, int old_level, int to_level);						// 强化装备操作
	//void OnUpLevelEquipOperate(Role *role, int equip_index, int old_level, int to_level);							// 升级装备操作
	void OnUpLevelEquipOperate(Role *role, bool is_puton, short type, short color, short quality, short level);		// 升级装备操作
	void OnUpStarEquipOperate(Role *role, int equip_index, int old_level, int to_level);							// 升星装备操作
	void OnEquipUpQuality(Role *role, int equip_index, int old_level, int to_level);								// 升品装备操作
	void OnInheritEquipOperate(Role *role, int equip_index1, int equip_index2);										// 传承装备操作
	void OnFuLingEquipOperate(Role *role, int equip_index1);														// 附灵操作
	void OnEquipBaptize(Role *role);																				// 装备洗炼

	void OnQunXianLuanDouKill(Role *role);																			// 在群仙乱斗中杀人
	void OnQunXianLuanDouAssist(Role *role);																		// 在群仙乱斗中助攻
	void OnChangeProf(Role *role, int prof);																		// 改变职业
	void OnZhuanzhi(Role *role);																					// 改变职业

	void OnAcceptHusongTask(Role *role);																			// 接受护送任务
	void OnAcceptDailyTask(Role *role);																				// 接受日常任务
	void OnAcceptGuildTask(Role *role);																				// 接受仙盟任务
	void OnAcceptPaohuanTask(Role *role);	

	void OnEnterFBStory(Role *role);																				// 进入副本 - 经验本
	void OnEnterFBExp(Role *role, int times);																		// 进入副本 - 经验本
	void OnEnterFBCoin(Role *role);																					// 进入铜币副本
	void OnEnterFBEquip(Role *role, bool is_first, bool is_trial);													// 进入副本 - 装备本
	void OnEnterFBTowerDefendPerson(Role *role, bool is_first);														// 进入副本 - 塔防本
	void OnEnterFBTowerDefendTeam(Role *role,bool is_first);														// 进入副本 - 塔防本组队
	void OnEnterYaoShouGuangChang(Role *role);																		// 进入妖兽广场
	void OnEnterSuoYaoTa(Role *role);																				// 进入锁妖塔
	void OnEnterGuildFb(Role *role);																				// 进入仙盟副本
	void OnEnterQingYuanFb(Role *role);																				// 进入情缘副本
	void OnEnterHotSpring(Role *role);																				// 进入副本 - 温泉
	void OnEnterArmorDefendFB(Role *role,bool is_first);															// 进入副本 
	void OnEnterBuildTowerFB(Role *role);																			// 进入副本 -建塔
	void OnEnterEquipTeamFb(Role * role);																			// 进入副本 -组队爬塔

	void OnPassYsjtTeamFb(Role *role);																				// 妖兽祭坛副本通关
	void OnPassExpFb(Role *role);																					// 经验本通关
	void OnPassTowerDfendTeamFb(Role *role);																		// 塔防坛副本通关
	void OnPassMgxfTeamFb(Role *role);																				// 迷宫仙府副本通关
	void OnShuiJingGather(Role *role);																				// 水晶幻境场景采集次数
	void OnAutoFbStory(Role *role);																					// 扫荡剧情副本
   
	void OnEnterQuanXianLuanDou(Role *role);	  																	// 进入战场 - 群仙乱斗
	void OnEnterZhuXieZhanChang(Role *role);																		// 进入战场 - 诛邪战场
	void OnEnterTianJiangCaiBao(Role *role);																		// 进入活动 － 天降财宝
	void OnEnterWangLingExplore(Role *role);																		// 进入活动 － 王陵探险
	void OnEnterGongchengzhan(Role *role);																			// 进入战场 - 攻城战
	void OnEnterXianmengzhan(Role *role);																			// 进入战场 - 仙盟战
	void OnEnterGuildBattle(Role *role);																			// 进入战场 - 公会争霸
	void OnEnterQuestion_2(Role *role);																				// 进入战场 - 答题
	void OnEnterTerritoryWar(Role *role);																			// 进入战场 - 领土战
	void OnEnterGuildYunBiao(Role *role);
	void OnEnterShuijing(Role *role);
	void OnEnterWorship(Role * role);
	void OnEnterCross3v3(Role * role);
	void OnEnterCrossShuijing(Role * role);
	void OnEnterCrossGuildBattle(Role * role);
	void OnEnterCrossDarkNight(Role * role);
	void OnEnterCrossLieKun(Role * role);
	void OnEnterCRossXiuluoTower(Role * role);

	void OnBanBenChouJiang(Role* role, int num);																	// 欢乐抽奖2

	void OnBuyFbCount(Role *role, int scene_type);																	// 购买副本次数

	void OnChestShopXunBao(Role *role, int shop_type, int count, bool is_free);													// 寻宝
	void OnRobHusong(Role *role);																					// 护送打劫
	void OnGuildGongXian(Role *role, int today_gongxian);															// 仙盟贡献

	void OnGuildDonate(Role *role, int donate_type, int donate_value);												// 仙盟捐献
	void OnEnterGuildMonsterFB(Role *role, bool is_first);															// 进入仙盟神兽副本
	void OnEnterChallengeFB(Role *role, int level, int layer);														// 进入挑战本
	void OnPassChallengeFB(Role *role, int level);																	// 通关挑战副本
	void OnPassStoryFB(Role *role, int fb_index);																	// 通关剧情副本
	void OnPassTowerDefendFB(Role *role, int level);																// 通关单人塔防副本
	void OnPassFBExp(Role *role);																					// 通关经验副本
	void OnEnterPhaseFB(Role *role);																				// 进入阶段本
	void OnPassPhaseFB(Role *role, int fb_type);																	// 通关阶段副本
	void OnPassPataFB(Role *role);																					// 通关爬塔副本
	void OnPassRuneTowerFB(Role *role, int layer, bool is_auto = false);											// 通关符文塔副本
	void OnPassArmordefendFB(Role *role);																			// 通关防具副本
	void OnEnterNationalBoss(Role *role);																			// 进入世界boss场景

	void OnCompleteHusongTask(Role *role, int task_color);															// 完成护送任务
	void OnCompleteDailyTask(Role *role, int complete_times);														// 完成日常任务
	void OnCompleteGuildTask(Role *role);																			// 完成仙盟任务
	void OnCompletePaohuanTask(Role *role, int complete_times);														// 完成跑环任务
	void OnAnswerQuestion(Role *role, int answer_times);															// 答题
	void OnXianjianSoulAddExp(Role *role);																			// 增加剑魂经验
	void OnXianjianSoulActive(Role *role);																			// 激活剑魂
	void OnXianjianActive(Role *role);																				// 激活仙剑
	void OnQingyuanPromoteLevel(Role *role);																		// 提升情缘装备等级
	void OnQingyuanUpQuality(Role *role);																			// 提升情缘装备品质
	void OnQingyuanEquip2UpLevel(Role *role);																		// 提升情比金坚装备等级
	void OnVipQifu(Role *role, int qifu_type, int qifu_times);														// 祈福

	void OnBuyMonthCard(Role *role);																				// 购买月卡事件

	void OnLeaveGuild(Role *role, GuildID guild_id);																// 离开仙盟
	void OnJoinGuild(Role *role, GuildID guild_id);																	// 加入仙盟

	void OnGatheXianJiu(Role *role);																				// 仙盟酒会采集	
	void OnCompleteWaBao(Role *role);																				// 挖宝
	void OnEnterTeamFB(Role *role);																					// 参与多人副本
	void OnEnterChallengeField(Role *role);																			// 参加排名竞技场
	void OnEnterZhanShenDian(Role *role);																			// 参加战神殿
	void OnPassZhanshendian(Role *role, int level);																	// 通关战神殿
	void OnCompleteJiLian(Role *role);																				// 完成一整次祭炼
	void OnCompleteCampTask(Role *role);																			// 完成阵营刺杀任务
	void OnCustomAvatarChange(Role *role, long long avatar_timestamp);												// 更新自定义头像
	void OnBuyTouzijihua(Role *role, int plan_type);																// 购买了投资计划
	void OnFriendBless(Role *role);																					// 好友祝福

	void OnSetSpecialAppearance(Role *role, int special_appearance_type);											// 设置特殊形象
	void OnSetOtherSpecialAppearance(Role *role, int special_appearance_type);										// 设置其他人特殊形象
	void OnActiveDegreeComplete(Role *role, int active_degree_type);												// 活跃度完成
	void OnMarry(Role *role);																						// 结婚
	void OnMarryRepeat(Role * role);																				// 重复结婚
	void OnDivorce(Role *role);																						// 离婚
	void OnRaProposeSucc(Role *role, Role *target_role);															// 求婚成功

	void OnEnterYiZhanDaoDi(Role *role);																			// 进入战场 - 一站到底
	void OnJoinTeam(Role *role);																					// 加入队伍
	void OnLeaveTeam(Role *role);																					// 离开队伍

	void OnChangeSex(Role *role, int from_sex);																		// 变性
	void OnPersonRankSnapshot(int old_top_uid_list[PERSON_RANK_TYPE_MAX], int new_top_uid_list[PERSON_RANK_TYPE_MAX]);// 个人排行榜做镜像
	//rank_index 从0开始(不要在本函数执行 太耗时的操作)
	void OnPersonRankChange(UserID uid, int rank_index, int rank_type, long long rank_value);
	void OnCoupleRankChange(UserID uid, int rank_index, int rank_type, long long rank_value);
	void OnKillRoleShadow(Role *role, RoleShadow *role_shadow);

	void OnChangeXianmengzhanWiner(int old_winer, int new_winer);													// 仙盟战盟主变化
	void OnChangeGongchengzhanWiner(int old_winer, int new_winer);													// 攻城战盟主变化

	void OnEffectHpStoreBroken(Character *cha, ObjID deliverer, UInt16 product_id, char product_method);			// 血盾buff被打破

	void OnAddChengJiu(Role *role, int old_chengjiu, int new_chengjiu);												// 增加成就时
	void OnAddAllCoin(Role *role, Int64 old_all_coin, Int64 new_allcoin);											// 增加铜钱时

	void OnUpgradeSysUpgrade(Role *role, int upgrade_type, int old_grade, int new_grade);							// 进阶系统进阶
	void OnUpgradeSysActive(Role *role, int upgrade_type, bool is_recal = true);									// 进阶系统激活
	void OnUpgradeSysActiveSpecialImage(Role *role, int upgrade_type, short special_img_id, bool is_notify = true);	// 进阶系统激活特殊形象

	void OnUpSkillLevel(Role *role, int skill_id, int old_level, int new_level, int total_skill_level);				// 技能升级
	void OnUpXianJieLevel(Role *role, int old_level, int new_level);												// 仙阶升级
	void OnUpChengJiuTitleLevel(Role *role, int old_level, int new_level);											// 成就称号等级
	void OnAccountTotalLoginDayCountChange(Role *role, int day_count);												// 登录天数改变时
	void OnShiZhuangUpgrade(Role *role, int old_jie, int new_jie);													// 时装阶数变化
	void OnShenBingUpgrade(Role *role, int old_jie, int new_jie);													// 神兵阶数变化
	void OnWingUpgrade(Role *role, int old_jie, int new_jie);														// 翅膀阶数变化
	void OnHaloUpgrade(Role *role, int old_jie, int new_jie);														// 光环阶数变化
	void OnShengongUpgrade(Role *role, int old_jie, int new_jie);													// 神弓阶数变化
	void OnShenyiUpgrade(Role *role, int old_jie, int new_jie);														// 神翼阶数变化
	void OnFootprintUpgrade(Role *role, int old_jie, int new_jie);													// 足迹阶数变化
	void OnFaBaoUpgrade(Role *role, int old_jie, int new_jie);														// 法宝阶数变化
	void OnPutOnJinglingCount(Role *role, int old_count, int new_count);											// 精灵穿戴数量变化
	void OnPataFBPassLevel(Role *role, int old_level, int new_level);												// 爬塔副本通关层数变化
	void OnExpFBPassWave(Role *role, int old_wave, int new_wave);													// 经验副本通关波数变化
	void OnTowerdefendteamFBPassWave(Role *role, int old_wave, int new_wave);										// 组队守护副本通关波数变化
	void OnEquipTeamFBPass(Role *role, int new_layer);																// 组队装备副本通关层数变化
	void OnRecycleItem(Role *role, ItemID item_id);																	// 回收物品
	void OnLieMingChouHun(Role *role);																				// 猎命抽魂
	void OnLieMingHunAddExp(Role *role);																			// 给魂加经验
	void OnInlayStone(Role *role, ItemID item_id);																	// 镶嵌宝石
	void OnUplevelstone(Role *role);																				// 升级宝石
	void OnJingLingUpLevel(Role *role, ItemID item_id, int to_level);												// 精灵升级
	void OnJingLingWuxingUpLevel(Role *role, int to_level);															// 精灵悟性升级
	void OnJingLingGather(Role *role, int count);																	// 精灵奇遇
	void OnDestroyTeam(int team_index);																				// 队伍解散时
	void OnGatherBonfire(Role *role);																				// 采集仙盟篝火
	void OnShenZhuangJinJie(Role *role);																			// 神装进阶		
	void OnShenZhuangShenZhu(Role *role);																			// 神装	神铸
	void OnJingLingFeiSheng(Role *role);																			// 精灵飞升
	void OnJingLingEquipmentStrength(Role *role);																	// 精灵强化
	void OnJingLingPuton(Role *role, int level);																	// 精灵装备
	void OnJingLingSkillChange(Role *role);																			// 精灵装备技能改变
	void OnActiveHeshenluoshu(Role *role);																			// 激活河神洛书
	void OnJingjieUplevel(Role *role);																				// 境界升级

	void OnEnterCrossTeamFb(Role *role);																			// 进入跨服组队本
	void OnEnterTeamFb(Role *role);																					// 进入组队本
	void OnPassTeamFb(Role *role);																					// 通关组队本
	void OnEnterCross1v1(Role *role);																				// 进入跨服1v1
	void OnCompleteCrossFB(Role *role);			    																// 完成跨服副本
	void OnOpenGuildBox(Role *role);																				// 挖掘公会宝箱
	void OnFetchGuildBox(Role *role);																				// 领取公会宝箱
	void OnUplevelZhiBao(Role *role, int level);																	// 至宝升级
	void OnXiuLuoTaKill(Role *role);																				// 跨服修罗塔杀人
	void OnGongChengZhanWin(Role *role);																			// 攻城战砍旗（砍旗公会的所有会员）
	void OnCoss1v1FightResult(Role *role, bool is_win);																// 跨服1v1比赛结束
	void OnPutEquipInGuild(Role *role);																				// 放装备到公会仓库
	void OnPutTerritorWarKillTa(Role *role);																		// 领土战摧毁塔
	void OnKillRole(Role *dead, Role *killer);																		// 任意场景杀人
	void OnActiveMoJie(Role *role, int mojie_itemid);																// 魔戒激活
	void OnActiveXiannv(Role *role, int xiannv_itemid);																// 女神激活
	void OnXiannvUplevelZizhi(Role *role, int level);																// 女神升资质等级
	void OnRuneUplevel(Role *role, int level);																		// 符文升级
	void OnXunzhangUpLevel(Role *role, int total_level);															// 勋章升级
	void OnKillGuildBoss(Role *role);																				// 杀公会boss
	void OnChestShop(Role *role, int shop_type, int item_id);														// 寻宝
	void OnUpdateTuodan(Role *role);																				// 更新脱单信息
	void OnUplevelShenBing(Role *role, int level);																	// 神兵升级
	void OnHotspringJoin(Role *role);																				// 参加跨服温泉
	void OnHotspringAnswerRight(Role *role);																		// 跨服温泉答题正确
	void OnMillionaireGatherBox(Role *role);																		// 大富豪采集宝箱
	void OnWanglingFinishTask(Role *role);																			// 王陵探险完成任务
	void OnTianjiangcaibaoFinishTask(Role *role);																	// 天降财宝完成任务
	void OnShuijingFinishTask(Role *role);																			// 水晶幻境完成任务
	void OnChatOnWorldChannel(Role *role);																			// 世界频道发言
	void OnPassFbTuitu(Role *role);																					// 通关推图副本
	void OnPassEquipFb(Role *role, int chapter, int level);															// 通关装备副本 武器材料
	void OnJoinFishingActivity(Role *role);																			// 进入跨服钓鱼
	void OnCross1v1DurWin(Role *role, int num);																		// 跨服1V1获得P1连胜

	void OnYaoShiUpgrade(Role *role, int old_grade, int to_grade);													// 腰饰进阶
	void OnTouShiUpgrade(Role *role, int old_grade, int to_grade);													// 头饰进阶
	void OnQiLinBiUpgrade(Role *role, int old_grade, int to_grade);													// 麒麟臂进阶
	void OnMaskUpgrade(Role *role, int old_grade, int to_grade);													// 面具进阶

	void OnShengqiStrengthLevel(Role *role);																		// 圣器强化
	void OnChineseZodiacUpLevel(Role *role);																		// 生肖升级
	void OnSealInlay(Role *role);																					// 圣器镶嵌

	void OnGetItem(Role *role, ItemID item_id, int num);                                                            // 获得物品
	void OnLightUpZhuanZhi(Role * role, int zhuanzhi_type, short level);										    // 点亮转职

	void OnHunyinInlay(Role *role, int hunqi_id, int hunyin_slot);													// 异火魂印镶嵌

	void OnPutonShenshouEquip(Role *role, int shenshou_id, int item_id);											// 穿戴龙器装备

	void OnPassXinMoFb(Role *role, int layer);

	void OnEnterFBHolidayGuardPerson(Role *role, bool is_first);													// 进入活动 - 塔防本 - 节日守护

	void OnDecShengwuEssence(Role * role, int consume_number);

	void OnAddShengwuEssence(Role * role, int add_number);

	void OnActiveGreateSoldier(Role *role, int seq);																// 激活名将
	void OnUpPotentialGreateSoldier(Role *role, int seq);															// 名将潜能升级
	void OnGreateSoldierTenDraw(Role *role);																		// 名将十连抽

	void OnForbidRole(int uid);																						// 封禁聊天

	void OnAddGuildRumorEvent(GuildID guild_id, const char *name_1, const char *name_2, int event_type, int param0 = 0, int param1 = 0, int param2 = 0, int param3 = 0);

	void OnGetSceneActivityReward(Role *role, int activity_id, int reward_id, int reward_type, int param = 0);

	void OnBroadCastCrossRankTop3ToGuild(int cross_rank_type, int param1, int param2, long long rank_value, int rank_pos);

private:
	EventHandler();
	~EventHandler();
};

#endif // __EVENT_HANDER_HPP__

