#pragma once
#include <QWidget>
#include "FFAngleContext.h"
#include <memory>
class FFMediaPlayer;
class QDXWidget : public QWidget {
    Q_OBJECT
public :
    explicit QDXWidget(QWidget*parent = nullptr);
public:
    QPaintEngine* paintEngine() const override { return nullptr;};

public:
    //void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;
private:    
    std::shared_ptr<FFAngleContext> m_angleCtx;
    
};