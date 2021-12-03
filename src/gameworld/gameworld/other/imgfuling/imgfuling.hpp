#ifndef __IMG_FULING_HPP__
#define __IMG_FULING_HPP__

#include "servercommon/imgfulingdef.hpp"
#include "obj/character/attribute.hpp"

class Role;
class Character;

class ImgFuling
{
public:
	ImgFuling();
	~ImgFuling();

	void Init(Role *role, const ImgFulingParam &param);
	void GetInitParam(ImgFulingParam *param);

	void OnImgFulingOperate(short operate_type, short param_1, short param_2, short param_3);
	bool OnLevelUp(int jingjie_type, int stuff_index);
	void OnPutOnImg(int jingjie_type, int img_id);
	void CheckFirstLoadImg();

	void OnAttack(Attribute delta_hp);
	void OnHurtOther(Character *target, long long base_injure, long long &injure, int target_index);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);

private:
	template<class T>
	inline void AddJingJiePerAttr(CharIntAttrs &attr, int per_add, const T *cfg)
	{
		if (NULL != cfg)
		{
			attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MAXHP] += static_cast<Attribute>(cfg->maxhp * (per_add * ROLE_ATTR_PER));
			attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_GONGJI] += static_cast<Attribute>(cfg->gongji * (per_add * ROLE_ATTR_PER));
			attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_FANGYU] += static_cast<Attribute>(cfg->fangyu * (per_add * ROLE_ATTR_PER));
			attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_MINGZHONG] += static_cast<Attribute>(cfg->mingzhong * (per_add * ROLE_ATTR_PER));
			attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_SHANBI] += static_cast<Attribute>(cfg->shanbi * (per_add * ROLE_ATTR_PER));
			attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_BAOJI] += static_cast<Attribute>(cfg->baoji * (per_add * ROLE_ATTR_PER));
			attr.m_attrs[CharIntAttrs::BASE_CHARINTATTR_TYPE_JIANREN] += static_cast<Attribute>(cfg->jianren * (per_add * ROLE_ATTR_PER));
		}
	}

	void SendAllImgFulingInfo();

	bool CanPerformSkill(int jingjie_type);
	void FirstSetImgID(int jingjie_type, unsigned char * active_flag, int active_flag_byte);

	Role *m_role;
	ImgFulingParam m_param;

	CharIntAttrs m_attrs_add;															// 各属性加成数值

	unsigned int m_skill_next_perform_time[IMG_FULING_SKILL_MAX];							// 技能冷却时间
};

#endif //__IMG_FULING_HPP__
