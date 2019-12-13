#ifndef MAINCTRLUTIL_H
#define MAINCTRLUTIL_H
#include<QObject>
#include<qstring.h>
#include<QtAlgorithms>
#include"client.h"
#include"mainctrlmacro.h"
#include"mainrecord.h"

/**
 * @brief The mainCtrlUtil class
 * @author Vincent Xue
 * 主控模块辅助工具类
 */
class mainCtrlUtil:public QObject
{
    Q_OBJECT

private:
    qint32 startId;
    qint32 nowId;
    qint32 startTokenId;
    qint32 nowTokenId;
    qint32 startRecordId;
    qint32 nowRecordId;
    quint16 startPort;
    quint16 nowPort;
    quint16 startFilePort;
    quint16 nowFilePort;

public:
    mainCtrlUtil();
    //创建clientID
    qint32 createId();

    //创建recordID
    qint32 createRecordId();

    //创建token
    qint32 createTokenId();

    //创建port
    quint16 createPort();

    //创建fileport
    quint16 createFilePort();

    //根据历史记录表合并文件,filePath:临时文件所在路径
    void mergeMissionFiles(QVector<historyRecord> historyTable,
                           const QString missionName,const QString filePath);

    //从existClients中根据name找到对应id,未找到则返回-1
    qint32 findIdFromExistClientsByName(QString name, QVector<Client*> existClients);

    //partner机 从scheduler中找对应token的task的slices结构体
    partnerSlices* findParnterTaskSlices(qint32 token,QVector<partnerSlices> &sliceSchedule);

    //friend机 从taskRecord中找对应token的task
    mainRecord* findTaskRecord(qint32 token,QVector<mainRecord*> &taskTable);

    //指定路径下创建文件夹，指定路径不存在导致创建失败return false
    static bool createDirectory(QString dirName,QString savePath);

    //指定路径下创建空文件
    static void createEmptyFile(QString fileName,QString savePath);

    //清空mission结构体
    static void clearMissionStruct(mission &m);

    //删除路径下文件，要求文件名完整（包括后缀）,删除失败返回false
    static bool deleteFile(QString path,QString name);

    //检查相对路径或绝对路径发给出的文件是否存在
    static bool isFileExist(QString fileNameWithPath);

    //检查任务合法性，包括url可访问性，存储路径
    //TODO:实现
    static bool isValidMission(mission m);

    //debug用，发送debugPause信号
    void sendDebugPauseTrue(){emit(debugPause(true));}
    void sendDebugPauseFalse(){emit(debugPause(false));}

public slots:
    //根据历史纪录表检查下载文件完整性
    bool missionIntegrityCheck( QVector<historyRecord> historyTable,
                                QString missionName, QString filePath, qint32 fileSize);

signals:
    //下载是否成功信号
    void missionValidation(bool success);

    //debug用，供调试时任意方法连接
    void debugPause(bool value);


};

#endif // MAINCTRLUTIL_H
