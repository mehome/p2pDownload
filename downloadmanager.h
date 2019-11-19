#ifndef DOWNLOADMANAGER_H
#define DOWNLOADMANAGER_H

#include <QUrl>
#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui {
    class DownloadManager;
}
QT_END_NAMESPACE

const int MAX_THREAD_COUNT = 10;             // 最大线程数量
const int BLOCK_SIZE = 1024 * 1024 * 50;     // 文件分块的大小，默认为 50Mb

/*
    进行下载任务的分配调度、提供 GUI 界面
*/
class DownloadManager : public QMainWindow {

    Q_OBJECT

public:
    DownloadManager(QWidget *parent = nullptr);
    ~DownloadManager();

    static qint64 getFileSize(QUrl url);    // 根据 URL 获取文件大小，此函数可作为接口直接使用：theSize = DownloadManager::getFileSize(theURL);

private:
    QString getFileName(QUrl url);
    QString getFilePath();
    int     getThreadCount(qint64 size);    // 计算将文件分块数量，以确定子线程数量

signals:
    void supPauseDownload();        // 要求子进程暂停下载
    void supStartDownload();        // 要求子进程继续下载

public slots:
    void onThreadFinished();        // 子进程下载完毕后检测下载任务是否结束

private slots:
    void on_pushButton_Start_clicked();
    void on_pushButton_Path_clicked();
    void on_pushButton_Pause_clicked();

private:
    Ui::DownloadManager *ui;

    QUrl    url;
    QString path;
    QString name;
    qint64  size;

    qint64  startSecond;        // 记录开始下载的时间
    bool    isFromStart;        // 用于区分按下开始按钮时是初次开始下载还是暂停后继续下载

    int     threadCount;        // 划分的线程数
    int     finishedThreadCount;    // 已完成的线程数
};

#endif // DOWNLOADMANAGER_H