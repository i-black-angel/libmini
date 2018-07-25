#ifndef _UML-MODEL_H_
#define _UML-MODEL_H_

// version: 1.0.1
class MApplication
{
public:
    explicit MApplication();
    virtual ~MApplication();
};

class MProcess
{
public:
    explicit MProcess();
    virtual ~MProcess();
};


class MCondition
{
public:
    explicit MCondition();
    virtual ~MCondition();
};

class MMutex
{
public:
    explicit MMutex();
    virtual ~MMutex();
};

class MScopedLock
{
public:
    explicit MScopedLock();
    virtual ~MScopedLock();
};

class MEvent
{
public:
    explicit MEvent();
    virtual ~MEvent();
private:
	MMutex _mutex;
	MCondition _condition;
};

class MThread
{
public:
    explicit MThread();
    virtual ~MThread();

	void start();
protected:
	virtual void run();
};

class MQueue
{
public:
    explicit MQueue();
    virtual ~MQueue();
};

class MThreadPool
{
public:
    explicit MThreadPool();
    virtual ~MThreadPool();
};

class MFile
{
public:
    explicit MFile();
    virtual ~MFile();
};

class MFileInfo
{
public:
    explicit MFileInfo();
    virtual ~MFileInfo();
};

class MOptions
{
public:
    explicit MOptions();
    virtual ~MOptions();
};

class MDir
{
public:
    explicit MDir();
    virtual ~MDir();
};

class MSettings
{
public:
    explicit MSettings();
    virtual ~MSettings();
};

class MUrl
{
public:
    explicit MUrl();
    virtual ~MUrl();
};

class MUuid
{
public:
    explicit MUuid();
    virtual ~MUuid();
};

class MXml
{
public:
    explicit MXml();
    virtual ~MXml();
};

class MJson
{
public:
    explicit MJson();
    virtual ~MJson();
};

class MLog
{
public:
    explicit MLog();
    virtual ~MLog();
};

class MRand
{
public:
    explicit MRand();
    virtual ~MRand();
};

class MHttp
{
public:
    explicit MHttp();
    virtual ~MHttp();
};

class MHttpServer : public MHttp
{
public:
    explicit MHttpServer();
    virtual ~MHttpServer();
};

class MHttpClient : public MHttp
{
public:
    explicit MHttpClient();
    virtual ~MHttpClient();
};

class MHash
{
public:
    explicit MHash();
    virtual ~MHash();

	static string md5sum(const string file);
};

class MSocket
{
public:
    explicit MSocket();
    virtual ~MSocket();
};

class MUdpClient
{
public:
    explicit MUdpClient();
    virtual ~MUdpClient();
private:
	MSocket _socket;
};

class MUdpServer : public MThread
{
public:
    explicit MUdpServer();
    virtual ~MUdpServer();
protected:
	virtual void run();
};

class MTcpClient : public MThread
{
public:
    explicit MTcpClient();
    virtual ~MTcpClient();
protected:
	virtual void run();
};

class MTcpServer : public MThread
{
public:
    explicit MTcpServer();
    virtual ~MTcpServer();
protected:
	virtual void run();
};

class MDateTime
{
public:
    explicit MDateTime();
    virtual ~MDateTime();
};

class MDataHandler
{
public:
    explicit MDataHandler();
    virtual ~MDataHandler();
};

class MError
{
public:
    explicit MError();
    virtual ~MError();
};

class MHostAddress
{
public:
    explicit MHostAddress();
    virtual ~MHostAddress();
};

class MMongoDB
{
public:
    explicit MMongoDB();
    virtual ~MMongoDB();
};

class MString
{
public:
    explicit MString();
    virtual ~MString();
};

// termio
// class MTermIO
// {
// public:
//     explicit MTermIO();
//     virtual ~MTermIO();
// };

class MCrc16
{
public:
    explicit MCrc16();
    virtual ~MCrc16();
};

class MCrc32
{
public:
    explicit MCrc32();
    virtual ~MCrc32();
};

#endif /* _UML-MODEL_H_ */
