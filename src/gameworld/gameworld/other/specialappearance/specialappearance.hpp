#ifndef __SPECIAL_APPEARANCE_HPP__
#define __SPECIAL_APPEARANCE_HPP__

class Role;

enum SPECIAL_APPEARANCE_TYPE
{
	SPECIAL_APPEARANCE_TYPE_NORMAL = 0,					// �������

	SPECIAL_APPERANCE_TYPE_WORD_EVENT_YURENCARD,		// �����¼����˿����
	SPECIAL_APPERANCE_TYPE_GREATE_SOLDIER,				// ����									
	SPECIAL_APPERANCE_TYPE_CHANGE_HUASHEN_IMAGE,		// �������
	SPECIAL_APPERANCE_TYPE_TERRITORYWAR,				// ����ս
	SPECIAL_APPERANCE_TYPE_CROSS_HOTSPRING,				// �����Ȫ

	SPECIAL_APPEARANCE_TYPE_CROSS_FISHING,				// �������

	SPECIAL_APPEARANCE_TYPE_SHNEQI = 9,					// ����

	SPECIAL_APPEARANCE_TYPE_MAX
};

// �����������
enum BIANSHEN_EFEECT_APPEARANCE
{
	APPEARANCE_NORMAL = 0,								// ������ۣ����ı���������

	APPEARANCE_DATI_RABBIT,								// �������-С��
	APPEARANCE_DATI_PIG,								// �������-С��
	APPEARANCE_MOJIE_GUAIWU,							// ħ�似��-��������
	APPEARANCE_YIZHANDAODI,								// һվ����-����
	APPEARANCE_ROLE,									// ��ɱ��˵�����
	APPEARANCE_INITAL_STATUS,							// ��ɫ��ʼ״̬
	APPEARANCE_MODEL_SIZE_BIG,							// �ı�ģ�ʹ�С
	APPEARANCE_MODEL_SIZE_SMALL,						// �ı�ģ�ʹ�С

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
	bool InAppearnceCountdown();	// �Ƿ����������󵹼�ʱ��

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
