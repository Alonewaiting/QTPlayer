#pragma once
#include <string>
#include<windows.h>
#include<thread>
#include<mutex>
#include <deque>
enum class RunningStaus
{
    UNINIT = 0,
    Pause,
    Running,
    Exit

};
class FFFFmpegDecode;
class FFAngleContext;
struct AVFrame;


class FFMediaPlayer {

public:
    FFMediaPlayer(const std::string & fileName = nullptr);
    ~FFMediaPlayer();
    bool initEngin(HWND handle);
    bool setViewSize(int width, int height);
    bool play();
    bool pasue();
    bool close();

private:
    void renderThreadRunLoop();
    void decodeThreadRunLoop();
private:
    HWND m_handle = 0;
    std::thread m_decodeThread;
    std::thread m_renderAndDisplayThread;
    std::mutex m_decodeLock;
    std::mutex m_renderLock;
    std::atomic_bool m_angleInit = false;
    RunningStaus m_staus = RunningStaus::UNINIT;
    std::shared_ptr<FFFFmpegDecode> m_decode;
    std::shared_ptr<FFAngleContext> m_angleCtx;
    std::deque<AVFrame*> m_frameDeque;
    int MAX_CACHE_SIZE = 3;
    int m_width = 0;
    int m_height = 0;
};