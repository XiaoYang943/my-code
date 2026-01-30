//
// Created by Administrator on 2026/1/18.
//

/*
 * 将相关的元素放到结构体 struct 中
 */

// bad
void draw(double x,double y,double z);

// good
struct Point {
    double x;
    double y;
    double z;
};
void draw(Point point);