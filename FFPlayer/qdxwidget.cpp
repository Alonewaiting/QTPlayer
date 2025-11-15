#include "qdxwidget.h"
#include <thread>
#include "FFMediaPlayer.h"
QDXWidget::QDXWidget(QWidget* parent) :QWidget(parent) {
    //获取窗口句柄
    
    //std::thread myThread([=]() {

    //    auto hdc = GetDC(handle);
    //    m_angleCtx = std::make_shared<FFAngleContext>(hdc);
    //    while (true)
    //    {
    //        m_angleCtx->test();
    //        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    //    }

    //    });
    //myThread.detach();
}


//void QDXWidget::paintEvent(QPaintEvent* event) {
//    m_angleCtx->test();
//}
void QDXWidget::resizeEvent(QResizeEvent* event) {

}



