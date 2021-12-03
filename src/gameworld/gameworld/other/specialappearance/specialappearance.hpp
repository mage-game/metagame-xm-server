#ifndef __SPECIAL_APPEARANCE_HPP__
#define __SPECIAL_APPEARANCE_HPP__

class Role;

enum SPECIAL_APPEARANCE_TYPE
{
	SPECIAL_APPEARANCE_TYPE_NORMAL = 0,					// 正常外观

	SPECIAL_APPERANCE_TYPE_WORD_EVENT_YURENCARD,		// 世界事件愚人卡外观
	SPECIAL_APPERANCE_TYPE_GREATE_SOLDIER,				// 名将									
	SPECIAL_APPERANCE_TYPE_CHANGE_HUASHEN_IMAGE,		// 化神外观
	SPECIAL_APPERANCE_TYPE_TERRITORYWAR,				// 领土战
	SPECIAL_APPERANCE_TYPE_CROSS_HOTSPRING,				// 跨服温泉

	SPECIAL_APPEARANCE_TYPE_CROSS_FISHING,				// 跨服钓鱼

	SPECIAL_APPEARANCE_TYPE_SHNEQI = 9,					// 神器

	SPECIAL_APPEARANCE_TYPE_MAX
};

// 变身外观形象
enum BIANSHEN_EFEECT_APPEARANCE
{
	APPEARANCE_NORMAL = 0,								// 正常外观，不改变人物形象

	APPEARANCE_DATI_RABBIT,								// 答题变身卡-小兔
	APPEARANCE_DATI_PIG,								// 答题变身卡-小猪
	APPEARANCE_MOJIE_GUAIWU,							// 魔戒技能-怪物形象
	APPEARANCE_YIZHANDAODI,								// 一站到底-树人
	APPEARANCE_ROLE,									// 变成别人的形象
	APPEARANCE_INITAL_STATUS,							// 角色初始状态
	APPEARANCE_MODEL_SIZE_BIG,							// 改变模型大小
	APPEARANCE_MODEL_SIZE_SMALL,						// 改变模型大小

	APPEARANCE_MAX
};

class SpecialAppearance
{
public:
	SpecialAppearance();
	~SpecialAppearance();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, int skill_appearance, unsigned int skill_appearance_end_time, int item_appearance, int item_aaearance_param, unsigned int item_appearance_end_time, 
				int huashen_appearance, int huashen_appearance_param, unsigned int huashen_appearance_end_time,
				int greate_soldier_appearance, int greate_soldier_appearance_param, unsigned int greate_soldier_appearance_end_time);
	void GetInitParam(int *skill_appearance, unsigned int *skill_appearance_end_time, int *item_appearance, int *item_aaearance_param, unsigned int *item_appearance_end_time, 
				int *huashen_appearance, int *huashen_appearance_param, unsigned int *huashen_appearance_end_time,
				int *greate_soldier_appearance, int *greate_soldier_appearance_param, unsigned int *greate_soldier_appearance_end_time);

	void Update(unsigned long interval, unsigned int now_second);

	short GetShowAppearance() { return m_show_appearance; }
	int GetShowAppearanceParam() { return m_show_appearance_param; }

	void SetSkillAppearance(short appearance, unsigned int end_time, bool is_broadcast = true);
	void SetFbAppearance(short appearance, int param, unsigned int end_time, bool is_broadcast = true);
	void SetItemAppearance(short appearance, int param, unsigned int end_time, bool is_broadcast = true);

	void SendAppearance(bool is_broadcast);

	bool CanUserSpecialAppearanceCard();
	bool OnUseSpecialAppearanceCard(short appearance, int last_time);
	void SetHuaShenAppearance(short appearance, int param, unsigned int end_time, bool is_broadcast = true);
	void SetShenqiAppearance(int param, unsigned int end_time, bool is_broadcast = true);

	void SetGreateSoldierAppearance(int param, unsigned int end_time, bool is_broadcast = true);

private:
	void CalcAppearance();
	bool InAppearnceCountdown();	// 是否在特殊形象倒计时中

	Role *m_role;

	short m_skill_appearance;
	unsigned int m_skill_appearance_end_time;

	short m_fb_appearance;
	int m_fb_appearance_param;
	unsigned int m_fb_appearance_end_time;

	short m_item_appearance;
	int m_item_appearance_param;
	unsigned int m_item_appearance_end_time;

	short m_huashen_appearance;
	int m_huashen_appearance_param;
	unsigned int m_huashen_appearance_end_time;

	short m_show_appearance;
	int m_show_appearance_param;

	short m_shenqi_appearance;
	int m_shenqi_appearance_param;
	unsigned int m_shenqi_appearance_end_time;

	short m_greate_soldier_appearance;
	int m_greate_soldier_appearance_param;
	unsigned int m_greate_soldier_appearance_end_time;
};

#endif
