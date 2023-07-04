#include "WebServer.hpp"
#include "RTSPLiveCapture.hpp"
#include "RTSPVideoStreamer.hpp"
#include "ImageProcesser.hpp"

WFFacilities::WaitGroup wait_group(1);
bool webServerRun = false;
static void sig_handler(int signo)
{
    wait_group.done(); // 使用静态成员函数时，需要指定具体的 wait_group 对象
    cout << "stop web server" << endl;
    webServerRun = false;
}

int WebServerTest()
{
    WebServer server;
    server.start();
    webServerRun = true;
    signal(SIGINT, sig_handler);
    while (webServerRun)
    {
    }
    server.stop();
    return 0;
}

int RTSPLiveCaptureTest()
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

int RTSPVideoStreamerTest()
{

    // 读取视频帧或从摄像头捕获帧
    cv::VideoCapture cap("../test.mp4"); // 替换为你的视频文件路径，或者使用摄像头，例如：0
    if (!cap.isOpened())
    {
        std::cout << "无法打开视频文件或摄像头" << std::endl;
        return 1;
    }

    std::string rtspUrl = "rtsp://127.0.0.1/live/test2"; // 替换为你的RTSP服务器地址
    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);       // 替换为帧的宽度
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT);     // 替换为帧的高度
    int fps = cap.get(cv::CAP_PROP_FPS);                 // 替换为帧率

    try
    {
        RTSPVideoStreamer streamer(width, height, fps, rtspUrl);
        while (true)
        {
            cv::Mat frame;
            cap >> frame;
            if (frame.empty())
                break;

            // 将帧推送到RTSP流
            if (!streamer.pushFrame(frame))
            {
                std::cout << "推送帧失败" << std::endl;
                break;
            }

            // 显示帧（可选）
            cv::imshow("Frame", frame);
            if (cv::waitKey(1000 / fps) == 'q')
            {
                break;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    cap.release();
    return 0;
}

int main()
{

    // WebServerTest();
    // RTSPLiveCaptureTest();
    // RTSPVideoStreamerTest();

    // return 0;
    WebServer server;
    server.start();
    webServerRun = true;
    signal(SIGINT, sig_handler);

    RTSPLiveCapture cap("rtsp://127.0.0.1/live/test");
    cv::Mat frame;
    cv::Mat outFrame;
    std::string rtspUrl = "rtsp://127.0.0.1/live/test2"; // 替换为你的RTSP服务器地址
    int width = cap.getFrameWidth();                     // 替换为帧的宽度
    int height = cap.getFrameHeight();                   // 替换为帧的高度
    int fps = cap.getFPS();                              // 替换为帧率

    ImageProcesser imageProcesser;
    try
    {
        RTSPVideoStreamer streamer(width, height, fps, rtspUrl);
        while (webServerRun)
        {
            if (cap.read(frame))
            {
                // 使用你的帧...
                cv::imshow("Frame", frame);

                outFrame = imageProcesser.process(frame);
  
                cv::imshow("outFrame", outFrame);
                
                // 将帧推送到RTSP流
                if (!streamer.pushFrame(outFrame))
                {
                    std::cout << "推送帧失败" << std::endl;
                    break;
                }
            }

            // 显示帧（可选）
            if (cv::waitKey(1000 / fps) == 'q')
            {
                break;
            }
        }
    }
    catch (const std::exception &e)
    {
        std::cerr << e.what() << '\n';
    }

    cv::destroyAllWindows();
    server.stop();
    return 0;
}