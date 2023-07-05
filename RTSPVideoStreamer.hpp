#include <opencv2/opencv.hpp>
#include <iostream>
#include <stdexcept>

class RTSPVideoStreamer
{
private:
    FILE *ffmpeg;
    int frame_width;
    int frame_height;
    int fps;
    std::string rtsp_url;

public:
    RTSPVideoStreamer(int frame_width, int frame_height, int fps, const std::string &rtsp_url)
    {
        this->frame_width = frame_width;
        this->frame_height = frame_height;
        this->fps = fps;
        this->rtsp_url = rtsp_url;
        ffmpeg = nullptr;
    }

    ~RTSPVideoStreamer()
    {
    }

    void start()
    {
        if (ffmpeg == nullptr)
        {
            std::string ffmpeg_cmd = "ffmpeg -y -f rawvideo -vcodec rawvideo -pix_fmt bgr24 -s " + std::to_string(frame_width) + "x" + std::to_string(frame_height) + " -r " + std::to_string(fps) + " -i - -c:v libx264 -pix_fmt yuv420p -preset ultrafast -f rtsp " + rtsp_url;

            ffmpeg = popen(ffmpeg_cmd.c_str(), "w");
            if (!ffmpeg)
            {
                throw std::runtime_error("Could not open pipe for output.");
            }
        }
    }

    void stop()
    {
        if (ffmpeg)
        {
            pclose(ffmpeg);
            ffmpeg = nullptr;
        }
    }

    bool pushFrame(const cv::Mat &frame)
    {
        if (frame.empty() || !ffmpeg)
        {
            return false;
        }

        // check frame size
        if (frame.cols != frame_width || frame.rows != frame_height)
        {
            return false;
        }

        // check frame type
        if (frame.type() != CV_8UC3)
        {
            return false;
        }

        // Write raw pixel data to the pipe
        size_t count = fwrite(frame.data, 1, frame.total() * frame.elemSize(), ffmpeg);
        return count == (frame.total() * frame.elemSize());
    }
};
