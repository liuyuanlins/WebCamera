#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdexcept>

class RTSPVideoStreamer
{
private:
    FILE *ffmpeg;

public:
    RTSPVideoStreamer(int frame_width, int frame_height, int fps, const std::string &rtsp_url) throw(std::runtime_error)
    {
        std::string ffmpeg_cmd = "ffmpeg -y -f rawvideo -vcodec rawvideo -pix_fmt bgr24 -s " + std::to_string(frame_width) + "x" + std::to_string(frame_height) + " -r " + std::to_string(fps) + " -i - -c:v libx264 -pix_fmt yuv420p -preset ultrafast -f rtsp " + rtsp_url;

        ffmpeg = popen(ffmpeg_cmd.c_str(), "w");
        if (!ffmpeg)
        {
            throw std::runtime_error("Could not open pipe for output.");
        }
    }

    ~RTSPVideoStreamer()
    {
        if (ffmpeg)
        {
            pclose(ffmpeg);
        }
    }

    bool pushFrame(const cv::Mat &frame)
    {
        if (frame.empty() || !ffmpeg)
        {
            return false;
        }

        // Write raw pixel data to the pipe
        size_t count = fwrite(frame.data, 1, frame.total() * frame.elemSize(), ffmpeg);
        return count == (frame.total() * frame.elemSize());
    }
};

int main()
{

    // 读取视频帧或从摄像头捕获帧
    cv::VideoCapture cap("../test.mp4"); // 替换为你的视频文件路径，或者使用摄像头，例如：0
    if (!cap.isOpened())
    {
        std::cout << "无法打开视频文件或摄像头" << std::endl;
        return 1;
    }

    std::string rtspUrl = "rtsp://127.0.0.1/live/test"; // 替换为你的RTSP服务器地址
    int width = cap.get(cv::CAP_PROP_FRAME_WIDTH);   // 替换为帧的宽度
    int height = cap.get(cv::CAP_PROP_FRAME_HEIGHT); // 替换为帧的高度
    int fps = cap.get(cv::CAP_PROP_FPS);             // 替换为帧率

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
            if (cv::waitKey(1000/fps) == 'q')
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
