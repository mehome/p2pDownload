#ifndef MAINFRIEND_H
#define MAINFRIEND_H

#include<QQueue>
#include <QEventLoop>
#include"mainrole.h"
#include"mainrecord.h"
#include"client.h"

/**
 * @brief The MainFriend class
 * 作为friedn朋友机，即请求发起机，执行下载
 */
class MainFriend:public QObject, public MainRole
{
    Q_OBJECT

private:
    //TODO:使用配置文件
    QString hostName;//登记在服务器的名字与密码
    QString pwd;
    Client local;//本地主机
    //本地下载任务
    QVector<mainRecord> localRecordLists;
    //下载控制
    qint32 clientNum;//参与下载的主机数量
    QVector<blockInfo> blockQueue;
    QVector<Client> existClients;//服务器中注册的主机
    QQueue<Client> waitingClients;//下载任务中待分配的主机
    QVector<Client> workingClients;//任务进行中的主机
    QVector<mainRecord> taskTable;//进行中的任务分配表
    QVector<historyRecord> historyTable;//历史记录表

public:
    MainFriend();
    MainFriend(UDPSocketUtil *udpSocketUtil,TCPSocketUtil * tcpSocketUtil,
               mainCtrlUtil * mainctrlutil,MsgUtil * msgUtil);

    /**
     * @brief regLocalClients 将本机信息注册到服务器
     * @return
     */
    bool regLocalClients();

    /**
     * @brief getExistClient 询问服务器当前注册主机信息
     */
    void getExistClients();
    void initExistClients();

    /**
     * @brief initWaitingClients 初始化空闲主机队列
     * @return
     */
    bool initWaitingClients();
    bool partnerAccept(qint32 partnerId);//加入waitingClients
    bool partnerReject(qint32 partnerId);

    /**
     * @brief createMission 创建下载任务，检测url可达性，获取文件大小，设置存储路径
     * @param url
     * @param savePath 下载文件的存储路径，默认在软件中创建tmp文件夹
     * @return
     */
    bool createMission(QString url,QString savePath="./tmp",QString missionName="temp");

    /**
     * @brief creatDownloadReq 发起下载请求
     * 请求分如下步骤：
     * 1. 询问伙伴机
     * 2. 初始化 hostNum, blockSize
     * 3. 创建任务块队列
     * @return
     */
    bool creatDownloadReq();

    /**
     * @brief downLoadSchedule
     * 下载管理，最核心的任务调度
     */
    void downLoadSchedule();

    //注册任务
    void addToTaskTable(QVector<mainRecord> recordLists);
    //删除任务,调用addToHistoryTable，将任务登记为历史记录
    //响应伙伴机信号或自身下载完成信号
    void deleteFromTaskTableLocal(qint32 clientID);
    void deleteFromTaskTablePartner(qint32 clientID);
    //增加历史记录
    void addToHistoryTable(historyRecord &hRecord);
    //查询历史记录
    void searchHistoryTable();
    //根据client的能力（taskNum），从blockQueue中取出对应数量的block
    QVector<blockInfo> getTaskBlocks(quint8 taskNum);
    //检查blcok是否连续，创建任务记录
    QVector<mainRecord> createTaskRecord(QVector<blockInfo> blockLists,qint32 clientId);
    //分配任务，发消息给伙伴机，token从reacordLists中取
    void assignTaskToPartner(qint32 partnerID,QVector<mainRecord> recordLists);
    //从工作队列挪到空闲队列
    void work2wait(qint32 clientId);



public slots:
    //TASKEXECUING 接收到伙伴机文件分片,发送THANKYOURHELP
    void recParnterSlice(qint32 partnerId, qint32 token, qint32 index);
    //从任务表中删除记录，确认任务完成，将Partner转移至空闲队列
    void taskEndConfig(qint32 clientId,qint32 token);
    //分配任务给本机，执行下载
    void assignTaskToLocal();
    //（上层封装，调用taskEndConfig）本地主机完成当前任务
    void taskEndAsLocal();

signals:
    //调用assignTaskToLocal，执行本地下载
    void callAssignTaskToLocal();
    //本地下载完成，调用taskEndAsLocal处理相关任务表、状态的变更
    void callTaskEndAsLocal();

};

#endif // MAINFRIEND_H