#include "mainrecord.h"
#include"mainctrlmacro.h"

qint8 mainRecord::getRecordID() const
{
    return recordID;
}

void mainRecord::setRecordID(const qint8 &value)
{
    recordID = value;
}

qint32 mainRecord::getClientId() const
{
    return clientId;
}

void mainRecord::setClientId(const qint32 &value)
{
    clientId = value;
}

qint32 mainRecord::getToken() const
{
    return token;
}

void mainRecord::setToken(const qint32 &value)
{
    token = value;
}


void mainRecord::addBlockId(blockInfo blockId){
    this->blockIds.append(blockId);
}

QVector<blockInfo> mainRecord::getBlockIds() const
{
    return blockIds;
}

mainRecord::mainRecord()
{
    //伪记录
    this->recordID=FAKERECORD;this->token=FAKERECORD;this->clientId=FAKERECORD;
    //    this->blockIds=QVector<qint8>();//空向量
}

mainRecord::mainRecord(const mainRecord &obj){
    qDebug()<<"mainRecord::mainRecord 拷贝构造函数。 不拷贝timer"<<endl;
    this->recordID=obj.recordID;
    this->clientId=obj.clientId;
    this->token=obj.token;
    this->timer=nullptr;//NOTE 不拷贝timer！
    QVector<blockInfo> blockTemp(obj.blockIds);
    this->blockIds.swap(blockTemp);
}

mainRecord::mainRecord(qint8 recordid,qint32 clientId,qint32 token)
{
    this->recordID=recordid;
    this->clientId=clientId;
    this->token=token;
}

mainRecord::~mainRecord(){
    qDebug()<<"mainRecord::~mainRecord 析构"<<endl;
    if(this->timer!=nullptr){
        delete this->timer;
    }
}

void mainRecord::createTimer(qint64 gap, bool isSingle){
    this->timer=new QTimer();
    this->timer->setSingleShot(isSingle);
    this->timer->start(gap);
    QObject::connect(this->timer,SIGNAL(timeout()),this,SLOT(recAlert()));
}

void mainRecord::recAlert(){
    emit this->sendTimeOutToCtrl(this->token);
}
