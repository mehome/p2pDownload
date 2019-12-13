#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    this->ctrl.signalsConnect();
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_pushButton_name_clicked()
{
    this->ctrl.local->setHostName(ui->lineEdit_name->text());
}

void MainWindow::on_pushButton_pwd_clicked()
{
    this->ctrl.local->setPwd(ui->lineEdit_pwd->text());
}

void MainWindow::on_pushButton_path_clicked()
{
    this->ctrl.local->setMissionPath(ui->lineEdit_path->text());
}

void MainWindow::on_pushButton_url_clicked()
{
    this->ctrl.local->setMissionUrl(ui->lineEdit_URL->text());
}

void MainWindow::on_pushButton_mname_clicked()
{
    this->ctrl.local->setMissionName(ui->lineEdit_mname->text());
}


void MainWindow::on_pushButton_login_clicked()
{
    this->ctrl.local->regLocalClients();
}

void MainWindow::on_pushButton_create_clicked()
{
    this->ctrl.local->createMission(this->ctrl.local->myMission.url,
                                    this->ctrl.local->myMission.savePath,
                                    this->ctrl.local->myMission.name);
}


void MainWindow::on_pushButton_help_clicked()
{
    this->ctrl.local->initWaitingClients();
}

void MainWindow::on_pushButton_get_clients_clicked()
{
    this->ctrl.local->getExistClients();
}

void MainWindow::on_pushButton_punch_clicked()
{
    this->ctrl.local->sendPunchToPartners();
}

void MainWindow::on_pushButton_logout_clicked()
{
    this->ctrl.local->logoutLocalClients();
}

void MainWindow::on_pushButton_downloadstart_clicked()
{
    this->ctrl.local->createDownloadReq();
}

void MainWindow::on_pushButton_debug_pause_clicked()
{
    this->ctrl.local->mainctrlutil->sendDebugPauseTrue();
}

void MainWindow::on_pushButton_debug_pause_false_clicked()
{
    this->ctrl.local->mainctrlutil->sendDebugPauseFalse();
}
