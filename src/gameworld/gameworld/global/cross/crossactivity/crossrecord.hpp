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

	UserID user_id;							// ��ɫID
	GameName role_name;						// ��ɫ����
	short level;							// �ȼ�
	char avatar;							// ͷ��
	char sex;								// �Ա�
	char prof;								// ְҵ
	char camp;								// ��Ӫ
	int order_view_match_index;				// ԤԼ��ս�ı�����
	UserCrossRecordData user_record_data;	// ���˿����¼����
	RoleAppearance user_appearance_data;	// �����������
};

#endif	// __CROSS_RECORD_HPP__

