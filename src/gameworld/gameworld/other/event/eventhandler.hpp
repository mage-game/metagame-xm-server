/******************************************************************************************
FileName: eventhander.hpp
Author: dreamwang
Description: �¼�������
Version: 1.0
History: 
<author>    <time>					<version >    <desc>
dreamwang   2012/03/09/ 17:04:05     1.0           ����
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
	void OnMonsterDisapper(Monster *monster);																		// �����Լ���ʧ
	void OnKillerRoleKillMonster(Role *role, Monster *monster);														// �����Ļ�ɱ�ߣ�ֻ��һ��
	void OnKillerTeamKillMonster(Role *role, Monster *monster);														// �����ɱ��
	void OnKillMonster(Role *role, int scene_id, const Posi &pos, int monster_id, int monster_level);				// ɱ��
	void OnStartGather(Role *role, int gather_id, ObjID obj_id, const Posi &gather_pos);							// ��ʼ�ɼ� 
	void OnGather(Role *role, int gather_id, ObjID obj_id, const Posi &gather_pos);									// �ɼ�
	void OnPickFallingItem(Role *role, FallingItem *falling_item, ItemDataWrapper *item_wrapper);					// ʰȡ����

	void OnCompleteTask(Role *role, TaskID task_id);																// �������
	void OnChangeCapability(Role *role, int cap_type);																// ս�����ı�
	void OnRoleUpgradeLevel(Role *role, int old_level, int new_level);												// ��ɫ����

	void OnRoleRealiveSucc(Role *role, int realive_type);															// ��ɫ����ɹ�

	void OnAddFriend(Role *role);																					// ��Ӻ���
	void OnResetName(Role *role, GameName name);																	// ��������

	void OnJoinActivity(Role *role, int activity_type);																// �μӻ

	bool OnRMBBuy(Role* role, int chongzhi_num);
	void OnAddChongzhi(Role *role, long long chongzhi_gold, bool is_notice_mail = true);							// ��ֵ����
	void OnConsumeGold(Role *role, long long consume_gold);															// ����Ԫ��
	void OnCheckCrossConsumeGold(Role *role, long long total_consume_gold, long long daily_consume_gold);			// ���������Ԫ��
	void OnAddGoldBind(Role *role, long long gold_bind);															// ���Ӱ���ʯʱ

	void OnShopBuyItem(Role *role, int buy_count, const ItemExtern::BuyItemInfo buy_list[ItemNamespace::MAX_BATCH_CONSUME_ITEM_NUM], long long consume_gold, long long consume_bind_gold);		// �̳ǹ�����Ʒ
	void OnGiveFlower(Role *role, int flower_num);																	// �����ʻ�
	void OnBeGiveFlower(Role *role);																				// �յ��ʻ�
	void OnVipLevelChange(Role *role);																				// Vip�ȼ��ı�
	void OnMountUpgrade(Role *role, int old_grade, int to_grade);													// �������
	void OnFightMountUpgrade(Role *role, int old_grade, int to_grade);												// ս���������
	void OnQibingUpgrade(Role *role, int old_grade, int to_grade);													// ����۽���
	void OnCloakUpLevel(Role *role, int old_grade, int to_grade);													// ��������

	void OnMountUpStar(Role *role, int old_star, int to_star);														// ��������
	void OnWingUpStar(Role *role, int old_star, int to_star);														// ��������
	void OnHaloUpStar(Role *role, int old_star, int to_star);														// �⻷����
	void OnShengongUpStar(Role *role, int old_star, int to_star);													// ������
	void OnShenyiUpStar(Role *role, int old_star, int to_star);														// ��������
	void OnFightMountUpStar(Role *role, int old_star, int to_star);													// ս����������
	void OnFootprintUpStar(Role *role, int old_star, int to_star);														// �⻷����

	void OnPutonEquip(Role *role, short equip_type, short color, short quality, short level, short xian_pin_count);	// ����װ��
	void OnTakeOffEquip(Role *role);																				// ����װ��

	void OnPutOnZhuanZhiEquip(Role *role, short equip_type, short color, short level);								// ����תְװ��
	void OnForgeZhuanZhiSuit(Role *role);																			// ����תְװ����װ

	void OnPutOnBaizhanEquip(Role *role, short equip_type, short color, short level);								// ������սװ��
	
	void OnStrengthenConsumeCoin(Role *role, int consume_coin);														// ǿ������ͭǮ
	void OnKillWorldBoss(Role *role);																				// ��ɱ����BOSS
	void OnKillMiZangBoss(Role *role);																				// ��ɱ�ز�BOSS
	void OnKillCrossBoss(Role * role);																				// ��ɱ���boss
	void OnKillRoleOnCrossBossScene(Role *role);																	// ���boss��ɱ���

	void OnAddShengwang(Role *role, int add_shengwang);															    // ��������

	void OnChangeCamp(Role *role, int camp);																		// �ı���Ӫ
	void OnItemCompose(Role *role, int product_id, int num);														// �ϳ���Ʒ
	void OnPetOpenEgg(Role *role);																					// �����ﵰ

	void OnEquipShenzhu(Role *role);																				// ����
	void OnReCalcAttr(Role *role, bool is_init);																					// ��������ƪ�������֮һ
	void OnStrengthenEquipOperate(Role *role, int equip_index, int old_level, int to_level);						// ǿ��װ������
	//void OnUpLevelEquipOperate(Role *role, int equip_index, int old_level, int to_level);							// ����װ������
	void OnUpLevelEquipOperate(Role *role, bool is_puton, short type, short color, short quality, short level);		// ����װ������
	void OnUpStarEquipOperate(Role *role, int equip_index, int old_level, int to_level);							// ����װ������
	void OnEquipUpQuality(Role *role, int equip_index, int old_level, int to_level);								// ��Ʒװ������
	void OnInheritEquipOperate(Role *role, int equip_index1, int equip_index2);										// ����װ������
	void OnFuLingEquipOperate(Role *role, int equip_index1);														// �������
	void OnEquipBaptize(Role *role);																				// װ��ϴ��

	void OnQunXianLuanDouKill(Role *role);																			// ��Ⱥ���Ҷ���ɱ��
	void OnQunXianLuanDouAssist(Role *role);																		// ��Ⱥ���Ҷ�������
	void OnChangeProf(Role *role, int prof);																		// �ı�ְҵ
	void OnZhuanzhi(Role *role);																					// �ı�ְҵ

	void OnAcceptHusongTask(Role *role);																			// ���ܻ�������
	void OnAcceptDailyTask(Role *role);																				// �����ճ�����
	void OnAcceptGuildTask(Role *role);																				// ������������
	void OnAcceptPaohuanTask(Role *role);	

	void OnEnterFBStory(Role *role);																				// ���븱�� - ���鱾
	void OnEnterFBExp(Role *role, int times);																		// ���븱�� - ���鱾
	void OnEnterFBCoin(Role *role);																					// ����ͭ�Ҹ���
	void OnEnterFBEquip(Role *role, bool is_first, bool is_trial);													// ���븱�� - װ����
	void OnEnterFBTowerDefendPerson(Role *role, bool is_first);														// ���븱�� - ������
	void OnEnterFBTowerDefendTeam(Role *role,bool is_first);														// ���븱�� - ���������
	void OnEnterYaoShouGuangChang(Role *role);																		// �������޹㳡
	void OnEnterSuoYaoTa(Role *role);																				// ����������
	void OnEnterGuildFb(Role *role);																				// �������˸���
	void OnEnterQingYuanFb(Role *role);																				// ������Ե����
	void OnEnterHotSpring(Role *role);																				// ���븱�� - ��Ȫ
	void OnEnterArmorDefendFB(Role *role,bool is_first);															// ���븱�� 
	void OnEnterBuildTowerFB(Role *role);																			// ���븱�� -����
	void OnEnterEquipTeamFb(Role * role);																			// ���븱�� -�������

	void OnPassYsjtTeamFb(Role *role);																				// ���޼�̳����ͨ��
	void OnPassExpFb(Role *role);																					// ���鱾ͨ��
	void OnPassTowerDfendTeamFb(Role *role);																		// ����̳����ͨ��
	void OnPassMgxfTeamFb(Role *role);																				// �Թ��ɸ�����ͨ��
	void OnShuiJingGather(Role *role);																				// ˮ���þ������ɼ�����
	void OnAutoFbStory(Role *role);																					// ɨ�����鸱��
   
	void OnEnterQuanXianLuanDou(Role *role);	  																	// ����ս�� - Ⱥ���Ҷ�
	void OnEnterZhuXieZhanChang(Role *role);																		// ����ս�� - ��аս��
	void OnEnterTianJiangCaiBao(Role *role);																		// ���� �� �콵�Ʊ�
	void OnEnterWangLingExplore(Role *role);																		// ���� �� ����̽��
	void OnEnterGongchengzhan(Role *role);																			// ����ս�� - ����ս
	void OnEnterXianmengzhan(Role *role);																			// ����ս�� - ����ս
	void OnEnterGuildBattle(Role *role);																			// ����ս�� - ��������
	void OnEnterQuestion_2(Role *role);																				// ����ս�� - ����
	void OnEnterTerritoryWar(Role *role);																			// ����ս�� - ����ս
	void OnEnterGuildYunBiao(Role *role);
	void OnEnterShuijing(Role *role);
	void OnEnterWorship(Role * role);
	void OnEnterCross3v3(Role * role);
	void OnEnterCrossShuijing(Role * role);
	void OnEnterCrossGuildBattle(Role * role);
	void OnEnterCrossDarkNight(Role * role);
	void OnEnterCrossLieKun(Role * role);
	void OnEnterCRossXiuluoTower(Role * role);

	void OnBanBenChouJiang(Role* role, int num);																	// ���ֳ齱2

	void OnBuyFbCount(Role *role, int scene_type);																	// ���򸱱�����

	void OnChestShopXunBao(Role *role, int shop_type, int count, bool is_free);													// Ѱ��
	void OnRobHusong(Role *role);																					// ���ʹ��
	void OnGuildGongXian(Role *role, int today_gongxian);															// ���˹���

	void OnGuildDonate(Role *role, int donate_type, int donate_value);												// ���˾���
	void OnEnterGuildMonsterFB(Role *role, bool is_first);															// �����������޸���
	void OnEnterChallengeFB(Role *role, int level, int layer);														// ������ս��
	void OnPassChallengeFB(Role *role, int level);																	// ͨ����ս����
	void OnPassStoryFB(Role *role, int fb_index);																	// ͨ�ؾ��鸱��
	void OnPassTowerDefendFB(Role *role, int level);																// ͨ�ص�����������
	void OnPassFBExp(Role *role);																					// ͨ�ؾ��鸱��
	void OnEnterPhaseFB(Role *role);																				// ����׶α�
	void OnPassPhaseFB(Role *role, int fb_type);																	// ͨ�ؽ׶θ���
	void OnPassPataFB(Role *role);																					// ͨ����������
	void OnPassRuneTowerFB(Role *role, int layer, bool is_auto = false);											// ͨ�ط���������
	void OnPassArmordefendFB(Role *role);																			// ͨ�ط��߸���
	void OnEnterNationalBoss(Role *role);																			// ��������boss����

	void OnCompleteHusongTask(Role *role, int task_color);															// ��ɻ�������
	void OnCompleteDailyTask(Role *role, int complete_times);														// ����ճ�����
	void OnCompleteGuildTask(Role *role);																			// �����������
	void OnCompletePaohuanTask(Role *role, int complete_times);														// ����ܻ�����
	void OnAnswerQuestion(Role *role, int answer_times);															// ����
	void OnXianjianSoulAddExp(Role *role);																			// ���ӽ��꾭��
	void OnXianjianSoulActive(Role *role);																			// �����
	void OnXianjianActive(Role *role);																				// �����ɽ�
	void OnQingyuanPromoteLevel(Role *role);																		// ������Եװ���ȼ�
	void OnQingyuanUpQuality(Role *role);																			// ������Եװ��Ʒ��
	void OnQingyuanEquip2UpLevel(Role *role);																		// ������Ƚ��װ���ȼ�
	void OnVipQifu(Role *role, int qifu_type, int qifu_times);														// ��

	void OnBuyMonthCard(Role *role);																				// �����¿��¼�

	void OnLeaveGuild(Role *role, GuildID guild_id);																// �뿪����
	void OnJoinGuild(Role *role, GuildID guild_id);																	// ��������

	void OnGatheXianJiu(Role *role);																				// ���˾ƻ�ɼ�	
	void OnCompleteWaBao(Role *role);																				// �ڱ�
	void OnEnterTeamFB(Role *role);																					// ������˸���
	void OnEnterChallengeField(Role *role);																			// �μ�����������
	void OnEnterZhanShenDian(Role *role);																			// �μ�ս���
	void OnPassZhanshendian(Role *role, int level);																	// ͨ��ս���
	void OnCompleteJiLian(Role *role);																				// ���һ���μ���
	void OnCompleteCampTask(Role *role);																			// �����Ӫ��ɱ����
	void OnCustomAvatarChange(Role *role, long long avatar_timestamp);												// �����Զ���ͷ��
	void OnBuyTouzijihua(Role *role, int plan_type);																// ������Ͷ�ʼƻ�
	void OnFriendBless(Role *role);																					// ����ף��

	void OnSetSpecialAppearance(Role *role, int special_appearance_type);											// ������������
	void OnSetOtherSpecialAppearance(Role *role, int special_appearance_type);										// ������������������
	void OnActiveDegreeComplete(Role *role, int active_degree_type);												// ��Ծ�����
	void OnMarry(Role *role);																						// ���
	void OnMarryRepeat(Role * role);																				// �ظ����
	void OnDivorce(Role *role);																						// ���
	void OnRaProposeSucc(Role *role, Role *target_role);															// ���ɹ�

	void OnEnterYiZhanDaoDi(Role *role);																			// ����ս�� - һվ����
	void OnJoinTeam(Role *role);																					// �������
	void OnLeaveTeam(Role *role);																					// �뿪����

	void OnChangeSex(Role *role, int from_sex);																		// ����
	void OnPersonRankSnapshot(int old_top_uid_list[PERSON_RANK_TYPE_MAX], int new_top_uid_list[PERSON_RANK_TYPE_MAX]);// �������а�������
	//rank_index ��0��ʼ(��Ҫ�ڱ�����ִ�� ̫��ʱ�Ĳ���)
	void OnPersonRankChange(UserID uid, int rank_index, int rank_type, long long rank_value);
	void OnCoupleRankChange(UserID uid, int rank_index, int rank_type, long long rank_value);
	void OnKillRoleShadow(Role *role, RoleShadow *role_shadow);

	void OnChangeXianmengzhanWiner(int old_winer, int new_winer);													// ����ս�����仯
	void OnChangeGongchengzhanWiner(int old_winer, int new_winer);													// ����ս�����仯

	void OnEffectHpStoreBroken(Character *cha, ObjID deliverer, UInt16 product_id, char product_method);			// Ѫ��buff������

	void OnAddChengJiu(Role *role, int old_chengjiu, int new_chengjiu);												// ���ӳɾ�ʱ
	void OnAddAllCoin(Role *role, Int64 old_all_coin, Int64 new_allcoin);											// ����ͭǮʱ

	void OnUpgradeSysUpgrade(Role *role, int upgrade_type, int old_grade, int new_grade);							// ����ϵͳ����
	void OnUpgradeSysActive(Role *role, int upgrade_type, bool is_recal = true);									// ����ϵͳ����
	void OnUpgradeSysActiveSpecialImage(Role *role, int upgrade_type, short special_img_id, bool is_notify = true);	// ����ϵͳ������������

	void OnUpSkillLevel(Role *role, int skill_id, int old_level, int new_level, int total_skill_level);				// ��������
	void OnUpXianJieLevel(Role *role, int old_level, int new_level);												// �ɽ�����
	void OnUpChengJiuTitleLevel(Role *role, int old_level, int new_level);											// �ɾͳƺŵȼ�
	void OnAccountTotalLoginDayCountChange(Role *role, int day_count);												// ��¼�����ı�ʱ
	void OnShiZhuangUpgrade(Role *role, int old_jie, int new_jie);													// ʱװ�����仯
	void OnShenBingUpgrade(Role *role, int old_jie, int new_jie);													// ��������仯
	void OnWingUpgrade(Role *role, int old_jie, int new_jie);														// �������仯
	void OnHaloUpgrade(Role *role, int old_jie, int new_jie);														// �⻷�����仯
	void OnShengongUpgrade(Role *role, int old_jie, int new_jie);													// �񹭽����仯
	void OnShenyiUpgrade(Role *role, int old_jie, int new_jie);														// ��������仯
	void OnFootprintUpgrade(Role *role, int old_jie, int new_jie);													// �㼣�����仯
	void OnFaBaoUpgrade(Role *role, int old_jie, int new_jie);														// ���������仯
	void OnPutOnJinglingCount(Role *role, int old_count, int new_count);											// ���鴩�������仯
	void OnPataFBPassLevel(Role *role, int old_level, int new_level);												// ��������ͨ�ز����仯
	void OnExpFBPassWave(Role *role, int old_wave, int new_wave);													// ���鸱��ͨ�ز����仯
	void OnTowerdefendteamFBPassWave(Role *role, int old_wave, int new_wave);										// ����ػ�����ͨ�ز����仯
	void OnEquipTeamFBPass(Role *role, int new_layer);																// ���װ������ͨ�ز����仯
	void OnRecycleItem(Role *role, ItemID item_id);																	// ������Ʒ
	void OnLieMingChouHun(Role *role);																				// �������
	void OnLieMingHunAddExp(Role *role);																			// ����Ӿ���
	void OnInlayStone(Role *role, ItemID item_id);																	// ��Ƕ��ʯ
	void OnUplevelstone(Role *role);																				// ������ʯ
	void OnJingLingUpLevel(Role *role, ItemID item_id, int to_level);												// ��������
	void OnJingLingWuxingUpLevel(Role *role, int to_level);															// ������������
	void OnJingLingGather(Role *role, int count);																	// ��������
	void OnDestroyTeam(int team_index);																				// �����ɢʱ
	void OnGatherBonfire(Role *role);																				// �ɼ���������
	void OnShenZhuangJinJie(Role *role);																			// ��װ����		
	void OnShenZhuangShenZhu(Role *role);																			// ��װ	����
	void OnJingLingFeiSheng(Role *role);																			// �������
	void OnJingLingEquipmentStrength(Role *role);																	// ����ǿ��
	void OnJingLingPuton(Role *role, int level);																	// ����װ��
	void OnJingLingSkillChange(Role *role);																			// ����װ�����ܸı�
	void OnActiveHeshenluoshu(Role *role);																			// �����������
	void OnJingjieUplevel(Role *role);																				// ��������

	void OnEnterCrossTeamFb(Role *role);																			// ��������ӱ�
	void OnEnterTeamFb(Role *role);																					// ������ӱ�
	void OnPassTeamFb(Role *role);																					// ͨ����ӱ�
	void OnEnterCross1v1(Role *role);																				// ������1v1
	void OnCompleteCrossFB(Role *role);			    																// ��ɿ������
	void OnOpenGuildBox(Role *role);																				// �ھ򹫻ᱦ��
	void OnFetchGuildBox(Role *role);																				// ��ȡ���ᱦ��
	void OnUplevelZhiBao(Role *role, int level);																	// ��������
	void OnXiuLuoTaKill(Role *role);																				// ���������ɱ��
	void OnGongChengZhanWin(Role *role);																			// ����ս���죨���칫������л�Ա��
	void OnCoss1v1FightResult(Role *role, bool is_win);																// ���1v1��������
	void OnPutEquipInGuild(Role *role);																				// ��װ��������ֿ�
	void OnPutTerritorWarKillTa(Role *role);																		// ����ս�ݻ���
	void OnKillRole(Role *dead, Role *killer);																		// ���ⳡ��ɱ��
	void OnActiveMoJie(Role *role, int mojie_itemid);																// ħ�伤��
	void OnActiveXiannv(Role *role, int xiannv_itemid);																// Ů�񼤻�
	void OnXiannvUplevelZizhi(Role *role, int level);																// Ů�������ʵȼ�
	void OnRuneUplevel(Role *role, int level);																		// ��������
	void OnXunzhangUpLevel(Role *role, int total_level);															// ѫ������
	void OnKillGuildBoss(Role *role);																				// ɱ����boss
	void OnChestShop(Role *role, int shop_type, int item_id);														// Ѱ��
	void OnUpdateTuodan(Role *role);																				// �����ѵ���Ϣ
	void OnUplevelShenBing(Role *role, int level);																	// �������
	void OnHotspringJoin(Role *role);																				// �μӿ����Ȫ
	void OnHotspringAnswerRight(Role *role);																		// �����Ȫ������ȷ
	void OnMillionaireGatherBox(Role *role);																		// �󸻺��ɼ�����
	void OnWanglingFinishTask(Role *role);																			// ����̽���������
	void OnTianjiangcaibaoFinishTask(Role *role);																	// �콵�Ʊ��������
	void OnShuijingFinishTask(Role *role);																			// ˮ���þ��������
	void OnChatOnWorldChannel(Role *role);																			// ����Ƶ������
	void OnPassFbTuitu(Role *role);																					// ͨ����ͼ����
	void OnPassEquipFb(Role *role, int chapter, int level);															// ͨ��װ������ ��������
	void OnJoinFishingActivity(Role *role);																			// ����������
	void OnCross1v1DurWin(Role *role, int num);																		// ���1V1���P1��ʤ

	void OnYaoShiUpgrade(Role *role, int old_grade, int to_grade);													// ���ν���
	void OnTouShiUpgrade(Role *role, int old_grade, int to_grade);													// ͷ�ν���
	void OnQiLinBiUpgrade(Role *role, int old_grade, int to_grade);													// ����۽���
	void OnMaskUpgrade(Role *role, int old_grade, int to_grade);													// ��߽���

	void OnShengqiStrengthLevel(Role *role);																		// ʥ��ǿ��
	void OnChineseZodiacUpLevel(Role *role);																		// ��Ф����
	void OnSealInlay(Role *role);																					// ʥ����Ƕ

	void OnGetItem(Role *role, ItemID item_id, int num);                                                            // �����Ʒ
	void OnLightUpZhuanZhi(Role * role, int zhuanzhi_type, short level);										    // ����תְ

	void OnHunyinInlay(Role *role, int hunqi_id, int hunyin_slot);													// ����ӡ��Ƕ

	void OnPutonShenshouEquip(Role *role, int shenshou_id, int item_id);											// ��������װ��

	void OnPassXinMoFb(Role *role, int layer);

	void OnEnterFBHolidayGuardPerson(Role *role, bool is_first);													// ���� - ������ - �����ػ�

	void OnDecShengwuEssence(Role * role, int consume_number);

	void OnAddShengwuEssence(Role * role, int add_number);

	void OnActiveGreateSoldier(Role *role, int seq);																// ��������
	void OnUpPotentialGreateSoldier(Role *role, int seq);															// ����Ǳ������
	void OnGreateSoldierTenDraw(Role *role);																		// ����ʮ����

	void OnForbidRole(int uid);																						// �������

	void OnAddGuildRumorEvent(GuildID guild_id, const char *name_1, const char *name_2, int event_type, int param0 = 0, int param1 = 0, int param2 = 0, int param3 = 0);

	void OnGetSceneActivityReward(Role *role, int activity_id, int reward_id, int reward_type, int param = 0);

	void OnBroadCastCrossRankTop3ToGuild(int cross_rank_type, int param1, int param2, long long rank_value, int rank_pos);

private:
	EventHandler();
	~EventHandler();
};

#endif // __EVENT_HANDER_HPP__

