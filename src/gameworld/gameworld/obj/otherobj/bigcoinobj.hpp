#ifndef __BIGCOINOBJ_H__
#define __BIGCOINOBJ_H__

#include "obj/obj.h"

class Role;

class BigCoinObj : public Obj
{
public:
	enum BIGCOIN_TYPE
	{
		BIGCOIN_TYPE_BINDCOIN = 0,
		BIGCOIN_TYPE_EXP,
		BIGCOIN_TYPE_BUFF,
		BIGCOIN_TYPE_EFFECT_BALL,
		BIGCOIN_TYPE_HONGBAO,
	};

	BigCoinObj();
	virtual ~BigCoinObj();

	void Init(char type, long long value, int disappear_s = 0);

	virtual void Update(unsigned long interval, time_t now_second, unsigned int now_dayid);

	void Pick(Role *role);
	
	short GetType() { return m_type; }
	long long GetValue() { return m_value; }
	ObjID GetPickerObjId() { return m_picker_objid; }

	void *			operator new(size_t c);
	void			operator delete(void *m);

private:
	long long m_value;
	ObjID m_picker_objid;

	bool m_has_pick;
	short m_type;
	unsigned int m_disappear_time;
};

#endif
