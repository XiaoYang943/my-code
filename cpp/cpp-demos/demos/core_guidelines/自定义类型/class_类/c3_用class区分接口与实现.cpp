/*
================================================================================
C++ Core Guidelines — C.3

用类清晰地区分接口与实现

--------------------------------------------------------------------------------
原则说明：

    使用 class 将“对外可见的接口”与“内部实现细节”分离。

    public 部分 = 对外接口
    private 部分 = 内部实现细节

--------------------------------------------------------------------------------
原因：

    1. 提高可读性
       使用者只需要关心接口，不需要了解内部实现。

    2. 简化维护
       修改内部实现，不影响外部使用方式。

    3. 提高稳定性
       接口通常比实现更稳定。

--------------------------------------------------------------------------------
核心思想：

    明确区分“能用什么”和“怎么实现”。
================================================================================
*/

#include <iostream>
#include <tuple>

// Month 作为强类型枚举
enum class Month {
    Jan = 1, Feb, Mar, Apr, May, Jun,
    Jul, Aug, Sep, Oct, Nov, Dec
};

// 对外暴露的接口
class Date {
public:
    Date(int yy, Month mm, int dd)
        : year_(yy), month_(mm), day_(dd) {
        validate();
    }

    int day() const { return day_; }
    Month month() const { return month_; }
    int year() const { return year_; }

private:
    // 内部表示（实现细节）
    int year_;
    Month month_;
    int day_;

    void validate() const {
        if (day_ <= 0 || day_ > 31) {
            throw std::invalid_argument("Invalid day");
        }
    }
};

int main() {

    Date d(2026, Month::Feb, 14);

    std::cout << "Year: " << d.year() << "\n";
    std::cout << "Day: " << d.day() << "\n";

    // 用户只能通过接口访问
    // d.year_ = 1000;   ❌ 编译错误，无法访问 private

    return 0;
}
