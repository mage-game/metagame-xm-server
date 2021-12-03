

#ifndef ICONNFACTROY_H
#define ICONNFACTROY_H

class IConnection;

class IConnFactroy
{
public:
	IConnFactroy(){};
	virtual ~IConnFactroy(){};
	virtual IConnection* CreateConn() = 0;
};


#endif

