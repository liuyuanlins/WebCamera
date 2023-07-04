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
        running = false;
        worker.join();
    }

    //get rtsp frame size info
    cv::Size getFrameSize()
    {
        return cv::Size((int)cap.get(cv::CAP_PROP_FRAME_WIDTH),
                        (int)cap.get(cv::CAP_PROP_FRAME_HEIGHT));
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

int main()
{
    RTSPLiveCapture cap("rtsp://127.0.0.1/live/test");
    cv::Mat frame;
    while (true)
    {
        if (cap.read(frame))
        {
            // 使用你的帧...
            cv::imshow("Frame", frame);
        }
        if (cv::waitKey(1000) == 'q')
        {
            break;
        }
    }
    cv::destroyAllWindows();
    return 0;
}
