/// v.map(|x| x * 2)
/// map是Option的方法，做Some 变换，这里的传参是一个闭包
/// |x| x * 2是闭包的语法，等价于 f(x) = x * 2
/// |参数| 表达式
fn main() {
    let v = Some(10);

    // 如果 v 是 Some(x)，就把 x 变成 x * 2，再包回 Some(...)
    // 如果 v 是 None，就还是 None
    let r = v.map(|x| x * 2);
    // let r = v.map(|x| {x * 2});  // 闭包的表达式可以用{}

    println!("{:?}", r); // Some(20)
}

fn main_bad() {
    let v = Some(10);

    let r = match v {
        Some(x) => Some(x * 2),
        None => None,
    };

    println!("{:?}", r); // Some(20)
}

/// 对于一个值的链式处理，用map比match更好，解决match嵌套问题
fn map_good_1() {
    let r = Some(10)
        .map(|x| x + 1)
        .map(|x| x * 2);
}

/// 满足下面 全部条件 时，map 非常合适：
/// 只关心 Some / Ok 的值
/// 不打印、不写日志、不修改外部状态
/// 返回一个新的 Option / Result
/// None / Err 原样传递
fn map_good_2() {
    let v = Some(10);

    let r = v.map(|x| x * 2);
}