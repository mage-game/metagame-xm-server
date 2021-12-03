#ifndef __MARRY_OBJ_HPP__
#define __MARRY_OBJ_HPP__

#include "character.h"

class Role;

class MarryObj : public Character
{
public:
	MarryObj();
	virtual ~MarryObj();

	void *	operator new(size_t c);
	void	operator delete(void *m);

	void Init(int marry_seq);

	virtual void Update(unsigned long interval, time_t now_second, unsigned int now_dayid);

	int GetMarrySeq() { return m_marry_seq; }
	bool IsFirstMarryobj() { return 0 == m_marry_seq; }

	void OnFirstMarryobjMove();
	void OnFirstMarryobjMoveStop();

protected:
	void SyncPosToGlobal();

	int m_marry_seq;
	time_t m_last_sync_pos_time;

	int m_cur_xunyou_point_index;
	Dir m_last_dir;
	Scalar m_last_dir_ditance;
};

#endif // __MARRY_OBJ_HPP__

