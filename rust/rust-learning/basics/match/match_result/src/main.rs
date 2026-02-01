fn divide(a: i32, b: i32) -> Result<i32, String> {
    if b == 0 {
        Err("division by zero".to_string())
    } else {
        Ok(a / b)
    }
}

/// ?语法糖的硬性规则
/// 1. 所在函数必须返回 Result
/// 2. 错误类型要匹配
fn half(v: i32) -> Result<i32, String> {
    // let x = match divide(v, 2) {
    //     Ok(v) => v,
    //     Err(e) => return Err(e),
    // };
    let x = divide(v, 2)?;      // 这里的问号?是match + early return的语法糖，可以简化、平替上面的代码
    Ok(x)
}

fn main() {
    match half(10) {
        Ok(v) => println!("half = {}", v),
        Err(e) => println!("error = {}", e),
    }
}