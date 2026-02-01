fn main() {
    let v = Some(10);   // Option的Some状态，代表有一个值。v的类型是 Option<i32>

    // 模式匹配
    // 对于 enums 来说，match时需要穷举处理所有的情况，这也是类型系统安全的保证之一
    match v {
        Some(10) => {
            // 有值，且值必须是10
            println!("ten");
        }
        // Some(_) => {
        //     // 有值，但是我不关心值是多少
        //     println!("some other value");
        // }
        Some(x) => {
            // 有值，我要用这个值
            println!("value = {}", x)
        },
        None => {
            // 没有值
            println!("no value")
        },
        // 所有剩余没列举出来的情况
        // _ => {
        //     // 其他情况
        //     println!("others")
        // },
    }
}
