#ifndef __CROSS_RECORD_HPP__
#define __CROSS_RECORD_HPP__

#include "servercommon/crossdef.hpp"

#include "servercommon/struct/global/crossrecordlistparam.hpp"

class CrossRecord
{
public:
	CrossRecord();

	void Init(const CrossRecordListParam::CrossRecordAttr &attr);
	void GetAttr(CrossRecordListParam::CrossRecordAttr *attr);
	void AddCrossMultiuserChallengeScore(int add_score);

	void *	operator new(size_t c);
	void	operator delete(void *m);

	UserID user_id;							// 角色ID
	GameName role_name;						// 角色名称
	short level;							// 等级
	char avatar;							// 头像
	char sex;								// 性别
	char prof;								// 职业
	char camp;								// 阵营
	int order_view_match_index;				// 预约观战的比赛组
	UserCrossRecordData user_record_data;	// 个人跨服记录数据
	RoleAppearance user_appearance_data;	// 个人外观数据
};

#endif	// __CROSS_RECORD_HPP__

