// Copyright (c) 2021 by Rockchip Electronics Co., Ltd. All Rights Reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include "Yolov5Adapter.h"


int Yolov5AdapterTest(int argc, char **argv)
{
  char *model_name = NULL;
  if (argc != 3)
  {
    printf("Usage: %s <rknn model> <jpg> \n", argv[0]);
    return -1;
  }

  model_name = (char *)argv[1];
  char *image_name = argv[2];

  Yolov5Adapter yolov5(model_name);
  printf("Read %s ...\n", image_name);
  cv::Mat orig_img = cv::imread(image_name, 1);
  if (!orig_img.data)
  {
    printf("cv::imread %s fail!\n", image_name);
    return -1;
  }

  cv::Mat result = yolov5.inference(orig_img);
  cv::imwrite("result.jpg", result);
  printf("save result.jpg done!\n");
}

/*-------------------------------------------
                  Main Functions
-------------------------------------------*/
int main(int argc, char **argv)
{
  Yolov5AdapterTest(argc, argv);
  return 0;
}
