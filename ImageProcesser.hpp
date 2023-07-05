#include <opencv2/opencv.hpp>
#include "ItemFilter.h"

class ImageProcesser
{
private:
  bool detectOn;
  bool detectAll;

public:
  ImageProcesser()
  {
    init_category_map();
  }
  ~ImageProcesser() {}

  cv::Mat process(cv::Mat &frame)
  {
    cv::Mat newFrame;
    if (detectOn && detectAll)
    {
      cv::cvtColor(frame, newFrame, cv::COLOR_BGR2GRAY);
      cv::cvtColor(newFrame, newFrame, cv::COLOR_GRAY2BGR);
    }
    else
    {
      newFrame = frame;
    }
    return newFrame;
  }

  void setDetectOn(bool on)
  {
    detectOn = on;
  }

  bool getDetectOn()
  {
    return detectOn;
  }

  void setDetectAll(bool all)
  {
    detectAll = all;
  }

  void setDetectFilter(const std::vector<std::string> &categories)
  {
    category_filter_set(categories);
  }
};
