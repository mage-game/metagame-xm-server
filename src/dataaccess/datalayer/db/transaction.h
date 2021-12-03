

#ifndef ITRANSACTION_H
#define ITRANSACTION_H

class ITransaction
{
public:
	ITransaction(){};
	virtual  ~ITransaction(){};

	virtual bool begin() = 0;
	virtual bool commit() = 0;
	virtual bool rollback() = 0;
	virtual bool isBegin() = 0;
	virtual ITransaction* clone() = 0;
};


#endif

