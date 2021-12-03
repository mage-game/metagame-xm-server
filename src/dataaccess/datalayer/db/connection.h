
#ifndef ICONNECTION_H
#define ICONNECTION_H

class IStatement;
class ITransaction;

class IConnection
{
public:
	IConnection(){};
	virtual ~IConnection(){};

	//复制连接
	virtual IConnection* clone() = 0;

	//连接数据库
	virtual bool connect() = 0;
	//重新连接数据库
	virtual bool reconnect() = 0;

	//连接是否活动
	virtual bool isActive() = 0;
	//事务是否开始
	virtual bool isBegin() = 0;

	//关闭连接
	virtual void close() = 0;
	//是否连接中
	virtual bool isClosed() = 0;

	//设置是否自动提交
	virtual bool setAutoCommit(bool AutoCommit) = 0;
	virtual bool getAutoCommit() = 0;

	//设置是否使用缓存
	//virtual void setIsCache(bool iscache) = 0;
	//获取是否使用缓存
	virtual bool getIsCache() = 0;

	//获取连接实体对象指针，例如MYSQL*
	virtual void* getconn() = 0;
	virtual ITransaction* getTransaction() = 0;
	//返回由以前的INSERT或UPDATE语句为AUTO_INCREMENT列生成的值
	virtual long long getInsertID() = 0;

	//事务开始,参数表明此次事务是否使用缓存事务
	virtual bool begin(bool iscache=true) = 0;
	//事务提交
	virtual bool commit() = 0;
	//事务回滚
	virtual bool rollback() = 0;

	virtual IStatement* createStatement() = 0;

	//获取错误代码
	virtual int getErrNo() const = 0;
	//获取错误字符串
	virtual const char* getError() const = 0;

	//连接是否销毁
	virtual bool isDestroy() = 0;
};

#endif
