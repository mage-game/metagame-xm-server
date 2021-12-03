#ifndef __BIG_CHATFACE_HPP__
#define __BIG_CHATFACE_HPP__

#include "servercommon/bigchatfacedef.hpp"
#include "obj/character/attribute.hpp"

class Role;

class BigChatFace
{
public:
	BigChatFace();
	~BigChatFace();

	void SetRole(Role *role) { m_role = role; }
	void Init(Role *role, const BigChatFaceParam &param);
	void GetInitParam(BigChatFaceParam *param);

	void ReCalcAttr(CharIntAttrs &base_add, bool is_recalc);
	bool OnBigFaceUpLevel();

	void SendBigChatFaceAllInfo();

	short GetBigChatFaceStatus();				// 获取聊天大表情激活状态，0代表未激活

protected:

private:
	Role *m_role;
	BigChatFaceParam m_param;

	CharIntAttrs m_attrs_add;				// 属性加成数值
};

#endif
