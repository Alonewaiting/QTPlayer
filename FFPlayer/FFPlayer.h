#pragma once

#include <QtWidgets/QMainWindow>

#include "ui_FFPlayer.h"

QT_BEGIN_NAMESPACE
namespace Ui { class FFPlayerClass; };
QT_END_NAMESPACE
class FFMediaPlayer;
class FFPlayer : public QMainWindow
{
    Q_OBJECT

public:
    FFPlayer(QWidget *parent = nullptr);
    ~FFPlayer();

public slots:
    void play();
    void pause();
    void openFile();

private:
   
private:
    std::shared_ptr<FFMediaPlayer> m_player;
    Ui::FFPlayerClass *ui;
    HWND m_handle;
};

