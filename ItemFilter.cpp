#include "ItemFilter.h"

static std::string categories[] =
    {
        "person",
        "bicycle",
        "car",
        "motorcycle",
        "airplane",
        "bus",
        "train",
        "truck",
        "boat",
        "traffic light",
        "fire hydrant",
        "stop sign",
        "parking meter",
        "bench",
        "bird",
        "cat",
        "dog",
        "horse",
        "sheep",
        "cow",
        "elephant",
        "bear",
        "zebra",
        "giraffe",
        "backpack",
        "umbrella",
        "handbag",
        "tie",
        "suitcase",
        "frisbee",
        "skis",
        "snowboard",
        "sports ball",
        "kite",
        "baseball bat",
        "baseball glove",
        "skateboard",
        "surfboard",
        "tennis racket",
        "bottle",
        "wine glass",
        "cup",
        "fork",
        "knife",
        "spoon",
        "bowl",
        "banana",
        "apple",
        "sandwich",
        "orange",
        "broccoli",
        "carrot",
        "hot dog",
        "pizza",
        "donut",
        "cake",
        "chair",
        "couch",
        "potted plant",
        "bed",
        "dining table",
        "toilet",
        "tv",
        "laptop",
        "mouse",
        "remote",
        "keyboard",
        "cell phone",
        "microwave",
        "oven",
        "toaster",
        "sink",
        "refrigerator",
        "book",
        "clock",
        "vase",
        "scissors",
        "teddy bear",
        "hair drier",
        "toothbrush",
};

std::map<std::string, int> category_map;
std::vector<int> class_id_filter_list = {};

void init_category_map()
{
    for (int i = 0; i < sizeof(categories) / sizeof(categories[0]); i++)
    {
        category_map[categories[i]] = i;
    }
}

bool contains_in_filter(int class_id)
{
    bool ret = false;
    // 是否包含在过滤列表中
    for (auto it = class_id_filter_list.begin(); it != class_id_filter_list.end(); ++it)
    {
        if (*it == class_id)
        {
            ret = true;
            break;
        }
    }
    return ret;
}

/**
 * @brief 获取类别,如果不在过滤列表中,则返回空字符串
 */
std::string get_category(int class_id)
{
    std::string ret = "";
    if (class_id < sizeof(categories) / sizeof(categories[0]) && class_id >= 0)
    {
        ret = categories[class_id];
    }
    return ret;
}

static void category_filter_empty()
{
    class_id_filter_list.clear();
}

static void category_filter_add(int class_id)
{
    if (!contains_in_filter(class_id))
    {
        class_id_filter_list.push_back(class_id);
    }
}

void category_filter_set(const std::vector<std::string> &categories)
{
    category_filter_empty();
    for (auto it = categories.begin(); it != categories.end(); ++it)
    {
        if (category_map.find(*it) != category_map.end())
        {
            category_filter_add(category_map[*it]);
        }
    }
}
