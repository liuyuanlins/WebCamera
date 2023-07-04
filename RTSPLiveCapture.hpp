#include <opencv2/opencv.hpp>
#include <thread>
#include <atomic>
#include <mutex>

class RTSPLiveCapture
{
    cv::VideoCapture cap;
    cv::Mat frame;
    std::atomic<bool> ret;
    std::atomic<bool> running;
    std::thread worker;
    std::mutex mtx;

public:
    RTSPLiveCapture(std::string url) : cap(url), running(true), ret(false)
    {
        worker = std::thread([this]
                             { this->update(); });
    }

    ~RTSPLiveCapture()
    {
        stop();
    }

    void stop()
    {
        if (running)
        {
            running = false;
            worker.join();
            cap.release();
        }
    }

    // get rtsp frame size info
    cv::Size getFrameSize()
    {
        return cv::Size((int)cap.get(cv::CAP_PROP_FRAME_WIDTH),
                        (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT));
    }

    // get rtsp fps info
    double getFPS()
    {
        return cap.get(cv::CAP_PROP_FPS);
    }

    // get rtsp frame width info
    int getFrameWidth()
    {
        return (int)cap.get(cv::CAP_PROP_FRAME_WIDTH);
    }

    // get rtsp frame height info
    int getFrameHeight()
    {
        return (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT);
    }

    void update()
    {
        while (running)
        {
            cv::Mat newFrame;
            bool newRet = cap.read(newFrame);
            if (newRet)
            {
                std::lock_guard<std::mutex> lock(mtx);
                frame = newFrame;
                ret = newRet;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(1)); // 添加这一行
        }
    }

    bool read(cv::Mat &outFrame)
    {
        std::lock_guard<std::mutex> lock(mtx);
        bool ret = (bool)this->ret;
        if (ret)
        {
            outFrame = frame.clone();
            this->ret = false;
        }
        return ret;
    }
};
