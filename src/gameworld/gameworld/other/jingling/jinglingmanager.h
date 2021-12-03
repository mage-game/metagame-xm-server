#ifndef __JINGLING_MANAGER_H__
#define __JINGLING_MANAGER_H__

#include "servercommon/jinglingdef.hpp"
#include "obj/character/attribute.hpp"
#include "equipment/equipment.h"
#include "protocal/msgjingling.h"

#define Jingling_ATTR_PER 0.5f //未出战仙宠属性加成到出战仙宠的比例

class Character;
class Role;
struct JingLingSkillRefreshCfg;

class JingLingManager
{
public:
	JingLingManager();
	~JingLingManager();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const JingLingParam &jingling_param);
	void GetInitParam(JingLingParam *jingling_param);
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	void ReCalcFightOutJingLingAttr(int &other_capability);
	void ReCalcNoFightOutJingLingAttr(int &other_capability);
	void ReCalcGroupAttr();
	void ReCalcCardAttr();
	void ReCalcSpecialJinglingAttr();
	void ReCalcShuXinDanAttr();
	void ReCalcGradeAttr();
	void ReCalcEquipAttr();
	void ReCalcPhantomAttr();
	void ReCalcSoulAttr();
	void ReCalcSoulTaoZhuangAttr();
	void ReCalcJingLingHaloAttr();

	const ItemGridData *GetJingLingData(short jingling_index);
	static int CalcJingLingCapability(const ItemGridData *grid_data);

	void OnDayChange(int old_dayid, int now_dayid);
	void OnHurtOther(Character *target, int skill_id, long long base_injure, long long &injure, int target_index, int &passive_flag);
	void OnAttack(ObjID attacker, UInt16 skill_id, Attribute &delta_hp, bool from_skill);
	bool IsBaoJi();

	int GetJingLingMaxLevel();
	int GetJingLingGrade() { return m_grade; }
	void GetUsedJingLingInfo(ItemID *jingling_id, short *level, GameName jingling_name, char *use_imageid, short *phantom_imgageid);
	int GetJingLingLevelByIndex(short index);
	short GetJingLingIndexByItemId(ItemID item_id);
	short GetFirstEmptyGridIndex();
	bool HasSameJingLing(ItemID item_id);
	int GetJinglingId() {return m_use_jingling_id; }

	int PutOn(const ItemDataWrapper &item_wrapper, short index);
	int TakeOff(short equip_index, bool notice_client = true);
	void ReCalcGroup();

	void UpLevel(short index);

	void CallBackJingLing( bool notice_client = true);
	void FightOutJingLing(short index);

	void UpLevelCard(short card_type);
	void AddExp(int card_idx, JingLingCardParamItem *card_item, int add_exp);
	int GetCardMinLevel();
	int GetCardTotalLevel();

	void Rename(const GameName new_name);

	bool OnUseShuxingdan(int slot_idx, int use_count);

	bool OnUpGrade(int is_auto_buy);
	bool ActiveImage(int imageid);
	bool UseImage(int imageid);
	bool ActivePhantomImage(int type);
	bool ActivePhantomImageFlag(int phantom_img_id);
	bool UsePhantomImg(int type);
	bool IsActivedPhantomImg(int phantom_img_id);

	void OnStrength(short index);
	void OnUpLevelPhantom(const int type);
	int GetJingLingPhantomImg() { return m_phantom_imgageid; }

	void OnUpGradeSoul(const int type, int is_protected, int is_auto_buy);
	void UpdateSoulTaoZhuangLevel();
	int GetJingLingPhantomCount();

	short GetHaloUseImgID() { return m_use_halo_img; }
	bool ActiveHaloImage(int imageid, bool recalc = true);
	bool CheckActiveHalo(bool recalc);
	bool UseHaloImage(int imageid);
	bool HaloUpgrade(bool is_auto_buy);
	const int GetHaloLevel() {return m_halo_level; } 

	void SyncJingLingInfo();
	void BroadcastJingLingInfo();
	
	int GetPutOnJinglingCount(int color = 0);		// 获取穿戴个数
	int GetPutOnSkillCount(int level);				// 获取技能穿戴个数

	void OnUpLevelWuXing(short jingling_index, bool is_safe, bool is_auto_buy);
	void OnUpLevelXianZhen(bool is_auto_buy);
	void OnUpLevelHunYu(int hunyu_type);
	void OnSkillRemove(short jingling_index, short skill_index, bool is_auto_buy);
	void OnSkillChangeMove(short jingling_index, short skill_index, bool is_auto_buy);
	void OnSkillPutOn(short jingling_index, short skill_index, short storage_index);
	void OnSkillTakeOff(short jingling_index, short skill_index, short storage_index);
	void OnSkillLearn(short jingling_index, short skill_index, short item_index);
	void OnSkillRefreshActive(short refresh_index);
	void OnSkillRefresh(short refresh_index, bool is_ten, bool is_auto_buy = false);
	void OnSkillGet(short refresh_index, short skill_index);

	void SendExploreInfo(int reason = 0);
	void OnExploreSelectMode(short mode);
	void OnExploreChallenge();
	void OnExploreFetchReward(short level, bool is_buy);
	void OnExploreReset();
	void OnExploreBuyBuff();

	int GetHistoryMaxChengzhang() { return m_history_max_chengzhang; }
	int GetHistoryMaxWuxing() { return m_history_max_wuxing; }
	void OnLogin();
	void OnRoleLevelChange(int level);
	void HuanHuaSpecialCapAdd(int huanhua_id, int huanhua_level, CharIntAttrs &m_attrs_add);
	void OnCompleteTask(TaskID task_id);

	void OnOpenSkillSlot(int jingling_index, int slot_index);

protected:
	short RandJingLingSkill(const JingLingSkillRefreshCfg *refresh_cfg);
	int GetSkillNum(ItemID jingling_id, int jingling_level, int wuxing_level, int color);
	bool IsJingLingActived() { return m_active_image_flag != 0; }
	void CheckFuncOpen();

	Role *m_role;
	CharIntAttrs	m_attrs_add;										// 各属性加成数值

	int m_group_skill_id;
	int m_group_num;
	int m_groupid_list[JINGLING_MAX_GROUP_IN_ONECE];

	short m_use_jingling_index;
	ItemID m_use_jingling_id;
	GameName m_jingling_name;											// 精灵名字

	ItemGridData m_jingling_list[JINGLING_MAX_TAKEON_NUM];
	JingLingCardParamItem m_card_list[JINGLING_CARD_MAX_TYPE];
	
	int m_shuxingdan_list[JINGLING_SHUXINGDAN_MAX_TYPE];				//属性丹列表	

	int m_grade;
	int m_grade_bless;
	int m_active_image_flag;
	int m_used_imageid;
	short m_equip_strength_list[JINGLING_EQUIP_MAX_PART];
	short m_phantom_level_list[JINGLING_PTHANTOM_MAX_TYPE];				// 幻化等级
	unsigned char m_phantom_image_flag[JINGLING_PTHANTOM_MAX_BYTE];
	short m_phantom_imgageid;

	short m_soul_level_list[JINGLING_CARD_MAX_TYPE];					// 灵魄聚灵等级
	int m_soul_taozhuang_level;											// 套装等级

	int m_halo_active_image_flag;										// 光环激活的云形象列表
	short m_halo_level;													// 光环等级
	short m_use_halo_img;												// 光环当前使用的形象
	int m_halo_bless_val;												// 光环祝福值

	int m_wuxing_level;
	int m_xianzhen_level;
	int m_xianzhen_exp;
	int m_xianzhen_up_count;
	int m_hunyu_level_list[XIAN_ZHEN_HUN_YU_TYPE_MAX];
	JingLingSkillInfo m_skill_storage_list[JING_LING_SKILL_STORAGE_MAX];
	JingLingRefreshItem m_skill_refresh_item_list[JING_LING_SKILL_REFRESH_ITEM_MAX];

	int m_other_capability;
	static const int SKILL_LEI_TING_TYPE = 12;							// 雷霆一怒
	static const int SKILL_XI_XUE_TYPE = 13;							// 血之盛宴
	static const int SKILL_ZHI_MING_TYPE = 14;							// 致命一击（减伤）
	static const int SKILL_FAN_TAN_TYPE = 15;							// 荆棘之甲
	int m_skill_lei_ting_id;
	int m_skill_xi_xue_id;
	int m_skill_zhi_ming_id;
	int m_skill_fan_tan_id;

	int m_history_max_capability;
	int m_explore_maxhp;
	int m_explore_hp;
	short m_explore_mode;
	short m_explore_buy_buff_count;
	JingLingExploreInfo m_explore_info_list[JING_LING_EXPLORE_LEVEL_COUNT];

	int m_history_max_chengzhang;										// 历史最大成长
	int m_history_max_wuxing;											// 历史最大悟性

	unsigned int m_skill_last_perform_time_lei_ting;					// 技能上次释放时间
	unsigned int m_skill_last_perform_time_xixue;						// 技能上次释放时间
	unsigned int m_skill_last_perform_time_zhi_ming;					// 技能上次释放时间
	unsigned int m_skill_last_perform_time_fan_tan;						// 技能上次释放时间
};

#endif

