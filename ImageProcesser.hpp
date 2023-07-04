#include <opencv2/opencv.hpp>


class ImageProcesser
{
private:
    
public:
    ImageProcesser(){}
    ~ImageProcesser(){}

    cv::Mat process(cv::Mat &frame)
    {
        cv::Mat newFrame;
                      cv::cvtColor(frame, newFrame, cv::COLOR_BGR2GRAY);
                cv::cvtColor(newFrame, newFrame, cv::COLOR_GRAY2BGR);
        return newFrame;
    }

};

