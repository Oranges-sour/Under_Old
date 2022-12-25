#ifndef __QUAD_TREE_H__
#define __QUAD_TREE_H__

#include <functional>
#include <memory>
#include <set>
#include <string>

#include "Random.h"

struct QuadCoor {
    QuadCoor(int x = 0, int y = 0) : x(x), y(y) {}
    int x, y;
};

// struct QuadContainer_node {
//     int where_idx;
//     int where_arr;
// };
//
// template <class T>
// class QuadContainer {
//     using Type = std::pair<bool, T>;
//
// public:
//     /* n代表数组长度，生成的容器能存储的数据量保证 大于等于n
//     default 默认值
//     */
//     QuadContainer(int n) {
//         range_size = sqrt(n);
//
//         idx_size = n / range_size;
//         if (n % range_size != 0) {
//             idx_size += 1;
//         }
//
//         idx = new int[idx_size];
//         arr = new Type[idx_size * range_size];
//
//         for (int i = 0; i < idx_size * range_size; ++i) {
//             arr[i].first = false;  // 默认没存
//         }
//
//         for (int i = 0; i < idx_size; ++i) {
//             idx[i] = range_size;
//         }
//     }
//
//     ~QuadContainer() {
//         delete[] idx;
//         delete[] arr;
//     }
//
//     std::pair<bool, QuadContainer_node> store(const T &val) {
//         QuadContainer_node result;
//
//         int p = -1;
//         for (int i = 0; i < idx_size; ++i) {
//             if (idx[i] > 0) {
//                 p = i;
//                 break;
//             }
//         }
//
//         if (p == -1) {
//             return {false, result};
//         }
//
//         int p1 = -1;
//         for (int i = 0; i < range_size; ++i) {
//             int k = p * range_size + i;
//             if (arr[k].first == false) {
//                 // 存进去
//                 arr[k].first = true;
//                 arr[k].second = val;
//
//                 p1 = k;
//                 break;
//             }
//         }
//         // 存了一个，计数减一
//         idx[p] -= 1;
//
//         result.where_idx = p;
//         result.where_arr = p1;
//
//         return {true, result};
//     }
//
//     void remove(const QuadContainer_node &node) {
//         idx[node.where_idx] += 1;
//         arr[node.where_arr].first = false;
//     }
//
// public:
//     int idx_size;
//     int range_size;
//
//     int *idx;
//     Type *arr;
// };

struct QuadContainer_node {
    std::string uid;
};

template <class T>
class QuadContainer {
    using Type = T;

public:
    /* n代表数组长度，生成的容器能存储的数据量保证 大于等于n
    default 默认值
    */
    QuadContainer(int) {}

    ~QuadContainer() {}

    std::pair<bool, QuadContainer_node> store(const T &val) {
        QuadContainer_node result;

        static const char vvv[] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                                   'g', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                                   's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

        std::string key;

        static rand_int r(0, 25);
        for (int i = 1; i <= 6; ++i) {
            key += vvv[r()];
        }

        contain.insert({key, val});

        result.uid = key;

        return {true, result};
    }

    void remove(const QuadContainer_node &node) { contain.erase(node.uid); }

public:
    std::map<std::string, Type> contain;
};

template <class T>
struct Quad_node {
    std::pair<bool, QuadContainer_node> containerResult;
    std::shared_ptr<QuadContainer<T>> container;
    QuadCoor coor;
};

template <class T>
class Quad {
public:
    Quad(const QuadCoor &left_top, const QuadCoor &right_bottom)
        : _child(false) {
        this->left_top = left_top;
        this->right_bottom = right_bottom;

        // 最后一层,要存节点
        if (abs(left_top.x - right_bottom.x) < 1 &&
            abs(left_top.y - right_bottom.y) < 1) {
            container = std::make_shared<QuadContainer<T>>(30);

            _child = true;
        }
    }

    Quad_node<T> insert(const QuadCoor &_where, const T &val) {
        if (!inside(_where)) {
            return Quad_node<T>{};
        }

        if (_child) {
            auto p = container->store(val);
            return Quad_node<T>{p, container, _where};
        }

        int pp = get_and_new_quads(_where);

        return quads[pp]->insert(_where, val);
    }

    void visit_in_rect(
        const QuadCoor &left_top, const QuadCoor &right_bottom,
        const std::function<void(const QuadCoor &cor, T &val)> &func) const {
        // 判断界外
        if (this->right_bottom.x < left_top.x ||
            this->left_top.x > right_bottom.x ||
            this->right_bottom.y > left_top.y ||
            this->left_top.y < right_bottom.y) {
            return;
        }

        if (_child) {
            for (auto &it : container->contain) {
                func(left_top, it.second);
            }
        } else {
            for (int i = 1; i <= 4; ++i) {
                if (quads[i].get()) {
                    quads[i]->visit_in_rect(left_top, right_bottom, func);
                }
            }
        }
    }

    bool inside(const QuadCoor &cor) {
        if (cor.x >= left_top.x && cor.x <= right_bottom.x &&
            cor.y >= right_bottom.y && cor.y <= left_top.y) {
            return true;
        }
        return false;
    }

private:
    int get_and_new_quads(const QuadCoor &_where) {
        int x_mid = (left_top.x + right_bottom.x) >> 1;
        int y_mid = (left_top.y + right_bottom.y) >> 1;

        int pp = 0;
        // 判断象限
        if (_where.x <= x_mid) {
            if (_where.y <= y_mid) {
                pp = 3;
            } else {
                pp = 2;
            }
        } else {
            if (_where.y <= y_mid) {
                pp = 4;
            } else {
                pp = 1;
            }
        }

        // 还没创建过要创建
        if (quads[pp].get() == nullptr) {
            if (pp == 1) {
                quads[1] = std::make_shared<Quad<T>>(
                    QuadCoor{x_mid + 1, left_top.y},
                    QuadCoor{right_bottom.x, y_mid + 1});
            } else if (pp == 2) {
                quads[2] =
                    std::make_shared<Quad<T>>(QuadCoor{left_top.x, left_top.y},
                                              QuadCoor{x_mid, y_mid + 1});
            } else if (pp == 3) {
                quads[3] =
                    std::make_shared<Quad<T>>(QuadCoor{left_top.x, y_mid},
                                              QuadCoor{x_mid, right_bottom.y});
            } else if (pp == 4) {
                quads[4] = std::make_shared<Quad<T>>(
                    QuadCoor{x_mid + 1, y_mid},
                    QuadCoor{right_bottom.x, right_bottom.y});
            }
        }

        return pp;
    }
    QuadCoor left_top;
    QuadCoor right_bottom;

    bool _child;

    // 最后一层,存元素的容器
    std::shared_ptr<QuadContainer<T>> container;

    // 四个子象限 1,2,3,4. 0不用
    std::shared_ptr<Quad<T>> quads[5];
};

#endif