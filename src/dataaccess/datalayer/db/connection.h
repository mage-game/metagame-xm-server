
#ifndef ICONNECTION_H
#define ICONNECTION_H

class IStatement;
class ITransaction;

class IConnection
{
public:
	IConnection(){};
	virtual ~IConnection(){};

	//��������
	virtual IConnection* clone() = 0;

	//�������ݿ�
	virtual bool connect() = 0;
	//�����������ݿ�
	virtual bool reconnect() = 0;

	//�����Ƿ�
	virtual bool isActive() = 0;
	//�����Ƿ�ʼ
	virtual bool isBegin() = 0;

	//�ر�����
	virtual void close() = 0;
	//�Ƿ�������
	virtual bool isClosed() = 0;

	//�����Ƿ��Զ��ύ
	virtual bool setAutoCommit(bool AutoCommit) = 0;
	virtual bool getAutoCommit() = 0;

	//�����Ƿ�ʹ�û���
	//virtual void setIsCache(bool iscache) = 0;
	//��ȡ�Ƿ�ʹ�û���
	virtual bool getIsCache() = 0;

	//��ȡ����ʵ�����ָ�룬����MYSQL*
	virtual void* getconn() = 0;
	virtual ITransaction* getTransaction() = 0;
	//��������ǰ��INSERT��UPDATE���ΪAUTO_INCREMENT�����ɵ�ֵ
	virtual long long getInsertID() = 0;

	//����ʼ,���������˴������Ƿ�ʹ�û�������
	virtual bool begin(bool iscache=true) = 0;
	//�����ύ
	virtual bool commit() = 0;
	//����ع�
	virtual bool rollback() = 0;

	virtual IStatement* createStatement() = 0;

	//��ȡ�������
	virtual int getErrNo() const = 0;
	//��ȡ�����ַ���
	virtual const char* getError() const = 0;

	//�����Ƿ�����
	virtual bool isDestroy() = 0;
};

#endif
