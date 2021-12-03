#ifndef __GUILD_XIANSHU_HPP__
#define __GUILD_XIANSHU_HPP__

class Role;

#include "servercommon/guilddef.hpp"
#include "obj/character/attribute.hpp"

class GuildXianshu
{
public:
	GuildXianshu();
	~GuildXianshu();

	void SetRole(Role *role) { m_role = role; }

	void Init(Role *role, GuildXianshuParam xianshu_param);
	void GetInitParam(GuildXianshuParam *xianshu_param);

	int	GetXianshuLevel() { return m_xianshu_param.xianshu_level; }
	void UplevelXianshu();
	void SendXianshuInfo();
	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);					// 重算龙徽加成

private:
	Role *m_role;
	GuildXianshuParam m_xianshu_param;

	CharIntAttrs m_attr_temp;													// 临时变量
};

#endif // __GUILD_XIANSHU_HPP__

