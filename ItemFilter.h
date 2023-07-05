
#ifndef _ITEM_FILTER_H_
#define _ITEM_FILTER_H_

#include <vector>
#include <map>
void init_category_map();

bool contains_in_filter(int class_id);
/**
 * @brief 获取类别,如果不在过滤列表中,则返回空字符串
 */
std::string get_category(int class_id);
void category_filter_set(const std::vector<std::string> &categories);


#endif // !_ITEM_FILTER_H_