#ifndef TCPSOCKETUTIL_H
#define TCPSOCKETUTIL_H

#include <QObject>
#include<QFile>

#include <QTcpServer>
#include <QVector>
#include <QMap>

#include <typeinfo>

#include "p2ptcpsocket.h"
#include "partner.h"
#include "commmsg.h"
#include "filemsg.h"

/**
 * @brief The TCPSocketUtil class
 * 客户端间P2P通信工具类
 * 带文件前缀意味着用于传送真正数据，否则用于传送控制信息
 */
class TCPSocketUtil : public QObject
{
    Q_OBJECT
private:
    /**
     * 作为朋友客户端的伙伴而建立的QTcpSocket对象，需要兼备收发数据的功能，每个连接对应一个C/S对
     */
    QMap<qint32, P2PTcpSocket *> guests;
    QMap<qint32, P2PTcpSocket *> fileGuests;

    /**
     * 作为伙伴客户端的朋友而建立的QTcpServer对象，需要兼备收发数据的功能，每个连接对应多个伙伴客户端
     */
    QTcpServer * host;
    QTcpServer * fileHost;

    /**
     * 监听到伙伴客户端发来的新请求后建立的QTcpSocket对象
     */
    QMap<qint32, P2PTcpSocket *> partnerConnections;
    QMap<qint32, P2PTcpSocket *> partnerFileConnections;

    /**
     * 伙伴客户端文件分块传送的当前块序，用于校验
     */
    QMap<qint32, qint32> partnerFileIndex;

    /**
     * QTcpServer侦听的端口
     */
    quint16 hostPort;
    quint16 fileHostPort;

    /**
     * QTcpSocket连接的端口
     */
    QMap<qint32, quint16> guestPort;
    QMap<qint32, quint16> fileGuestPort;

    /**
     * 所有处于登录状态的伙伴客户端
     */
    QMap<qint32, Partner *> parntersMap;

    /**
     * 各类连接的数量
     */
    qint32 connectedNum;
    qint32 fileConnectedNum;
    qint32 friendName;

    /**
     * 是否开启连接
     */
    bool openHost;
    bool openGuest;

    /**
     * 临时文件的存储信息
     */
    QString fileDir;
    QString fileType;

    /**
     * 文件分块传送时每一小块大小
     */
    qint32 groupSize;    

public:
    TCPSocketUtil();
    TCPSocketUtil(quint16 hostPort, quint16 fileHostPort, bool openHost, bool openGuest, QString fileDir, QString fileType, qint32 groupSize);

    ~TCPSocketUtil();

    /**
     * @brief 加载处于登录状态的伙伴客户端
     */
    bool bindPartners(QVector<Partner *> partners);
    bool addPartner(Partner * partner);

    /**
     * @brief 建立连接的一系列基础操作
     */
    bool stablishHost();
    bool stablishGuest(qint32 partnerId);

    bool stablishFileHost();
    bool stablishFileGuest(qint32 partnerId);

    bool createHost();
    bool createGuest(qint32 partnerId);

    bool closeHost();
    bool closeGuest(qint32 partnerId);

    bool createFileHost();
    bool createFileGuest(qint32 partnerId);

    bool closeFileHost();
    bool closeFileGuest(qint32 partnerId);

    bool listenPort();
    bool connectToPartner(qint32 partnerId);
    bool disConnectToPartner(qint32 partnerId);

    bool listenFilePort();    
    bool connectToFilePartner(qint32 partnerId);
    bool disConnectToFilePartner(qint32 partnerId);

    inline void setOpenHost(bool openHost);
    inline void setOpenGuest(bool openGuest);

    inline void setFileDir(QString fileDir);
    inline void setFileType(QString fileType);
    inline void setGroupSize(qint32 groupSize);

public slots:
    /**
     * @brief 监听到伙伴客户端的消息后执行相关操作
     */
    bool newConnectionWithPartner();
    bool recFromPartner(qint32 partnerId);
    bool sendToPartner(qint32 partnerId, CommMsg * msg);
    bool failToGetHelpFromPartner(QAbstractSocket::SocketError error, qint32 partnerId);

    /**
     * @brief 和朋友客户端互动
     */
    bool sendToFriend(qint32 partnerId, CommMsg * msg);
    bool recFromFriend(qint32 partnerId);
    bool failToHelpFriend(QAbstractSocket::SocketError error, qint32 partnerId);

    /**
     * @brief 监听到伙伴客户端发送的文件后执行相关操作
     */
    bool newConnectionWithFilePartner();
    bool recFromFilePartner(qint32 partnerId);
    bool failToGetHelpFromFilePartner(QAbstractSocket::SocketError error, qint32 partnerId);

    /**
     * @brief 真正的文件发送函数
     */
    bool sendToFileFriend(qint32 partnerId, CommMsg * msg);
    bool failToHelpFileFriend(QAbstractSocket::SocketError error, qint32 partnerId);

signals:
    // P2PPUNCH
    void timeToInitialTaskForPartner(qint32 partnerId);
    // ASKFORHELP
    void whetherToHelpFriend(qint32 partnerId, QString downloadAddress, qint32 lenMax);
    // AGREETOHELP
    void timeForFirstTaskForPartner(qint32 partnerId);
    // REFUSETOHELP
    void refuseToOfferHelpForPartner(qint32 partnerId);
    // DOWNLOADTASK
    void startToDownload(qint32 partnerId, qint32 token, qint64 pos, qint32 len);
    // TASKFINISH
    void readyToAcceptFileForPartner(qint32 partnerId, qint32 token);
    // TASKFAILURE
    void taskFailureForPartner(qint32 partnerId, qint32 token);
    // THANKYOURHELP
    void timeForNextTaskForPartner(qint32 partnerId, qint32 token, qint32 index);
    // ENDYOURHELP
    void taskHasFinishedForFriend(qint32 partnerId, qint32 token);

};

void TCPSocketUtil::setOpenHost(bool openHost)
{
    this->openHost = openHost;
}

void TCPSocketUtil::setOpenGuest(bool openGuest)
{
    this->openGuest = openGuest;
}

void TCPSocketUtil::setFileDir(QString fileDir)
{
    this->fileDir = fileDir;
}

void TCPSocketUtil::setFileType(QString fileType)
{
    this->fileType = fileType;
}

void TCPSocketUtil::setGroupSize(qint32 groupSize)
{
    this->groupSize = groupSize;
}

#endif // TCPSOCKETUTIL_H