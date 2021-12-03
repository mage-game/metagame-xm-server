#ifndef __JINJIE_SYS_FUNC_HELPER_HPP__
#define __JINJIE_SYS_FUNC_HELPER_HPP__

#include <functional>

class Role;

enum JingjieType
{
	JINJIE_TYPE_INVALID = -1,

	JINJIE_TYPE_CLOAK = 0,										// ����
	JINJIE_TYPE_FIGHT_MOUNT = 1,								// ս������
	JINJIE_TYPE_FOOTPRINT = 2,									// �㼣
	JINJIE_TYPE_HALO = 3,										// �⻷
	JINJIE_TYPE_LINGZHU = 4,									// ����
	JINJIE_TYPE_MASK = 5,										// ����
	JINJIE_TYPE_MOUNT = 6,										// ����
	JINJIE_TYPE_QILINBI = 7,									// �����
	JINJIE_TYPE_SHENGONG = 8,									// ��
	JINJIE_TYPE_SHENYI = 9,										// ����
	JINJIE_TYPE_TOUSHI = 10,									// ͷ��
	JINJIE_TYPE_WING = 11,										// ����
	JINJIE_TYPE_XIANBAO = 12,									// �ɱ�
	JINJIE_TYPE_YAOSHI = 13,									// ����
	JINJIE_TYPE_LINGGONG = 14,									// �鹭
	JINJIE_TYPE_LINGQI = 15,									// ����
	JINJIE_TYPE_LINGCHONG = 16,									// ���
	JINJIE_TYPE_WEIYAN = 17,									// β��
	JINJIE_TYPE_SHOUHUAN = 18,									// �ֻ�
	JINJIE_TYPE_TAIL = 19,										// β��
	JINJIE_TYPE_FLYPET = 20,									// �ɳ�
	JINJIE_TYPE_FABAO = 21,										// ����
	JINJIE_TYPE_SHIZHUANG = 22,									// ʱװ
	JINJIE_TYPE_SHENBING = 23,									// ���

	JINJIE_TYPE_MAX,
};

struct JinjiesysFuncStruct
{
	using GetGradeFunc = std::function<int(Role *)>;
	using CheckSpecialImgActiveFunc = std::function<bool(Role *, int)>;
	using ActiveSpecialImgFunc = std::function<void(Role *, int)>;

	GetGradeFunc get_grade_func = nullptr;
	CheckSpecialImgActiveFunc check_special_img_func = nullptr;
	ActiveSpecialImgFunc active_special_img_func = nullptr;
};

class JinjiesysFuncHelper
{
public:
	static JinjiesysFuncHelper & Instance()
	{
		static JinjiesysFuncHelper _instance;
		return _instance;
	}

	const JinjiesysFuncStruct * GetFuncStruct(int jinjie_type);

	void OnActiveJinjieSys(Role *role, int jinjie_type);
	void OnActiveSpecialImg(Role *role, int jinjie_type, int special_img_id);
	void OnUpgrade(Role *role, int jinjie_type, int old_grade, int new_grade);
	int UpgradeManagerTypeToJinJieSysType(int type);
	int JinJieSysTypeToUpgradeManagerType(int type);
private:
	JinjiesysFuncHelper();
	~JinjiesysFuncHelper() = default;

	void InitGetGradeFunc();
	void InitCheckSpecialImgActiveFunc();
	void InitActiveSpecialImgFunc();

	JinjiesysFuncStruct m_func_list[JINJIE_TYPE_MAX];
};

#endif // !__JINJIE_SYS_FUNC_HPP__
