#include "FFPlayer.h"
#include "FFMediaPlayer.h"
#include <QFileDialog>
FFPlayer::FFPlayer(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::FFPlayerClass())
{
    ui->setupUi(this);
    m_handle = (HWND)ui->playerWeight->winId();
    //有了窗口句柄，可以创建一个用来上屏的D3D11Device
    auto ge = ui->playerWeight->geometry();

   // 连接播放按钮信号与槽
    connect(ui->pushButtonPlayer, &QPushButton::clicked, this, &FFPlayer::play);
    
    // 连接暂停按钮信号与槽
    connect(ui->pushButtonPause, &QPushButton::clicked, this, &FFPlayer::pause);

    // 连接打开文件菜单信号与槽
    connect(ui->actionOpen, &QAction::triggered, this, &FFPlayer::openFile);

}
void FFPlayer::play() {
    if (m_player) {
        m_player->play();
        ui->pushButtonPlayer->setEnabled(false);  
        ui->pushButtonPause->setEnabled(true); 
    }
}
void FFPlayer::pause() {
    if (m_player) {
        m_player->pasue();
        ui->pushButtonPause->setEnabled(false); 
        ui->pushButtonPlayer->setEnabled(true);   
    }
}
void FFPlayer::openFile()
{
    QString fileName = QFileDialog::getOpenFileName(
        this,
        tr("打开媒体文件"),
        QString(),
        tr("视频文件 (*.mp4 *.mkv *.avi *.mov);;所有文件 (*)"));
    if (fileName.isEmpty())
        return;

    // 停止并释放旧播放器
    if (m_player) {
        m_player->close();
        m_player = nullptr;
    }

    // 用新文件创建播放器
    m_player = std::make_shared<FFMediaPlayer>(fileName.toLocal8Bit().constData());
    m_player->setViewSize(1200, 800);
    m_player->initEngin(m_handle);

    // 重置按钮状态
    ui->pushButtonPlayer->setEnabled(true);
    ui->pushButtonPause->setEnabled(false);

}
FFPlayer::~FFPlayer()
{
    delete ui;
}


