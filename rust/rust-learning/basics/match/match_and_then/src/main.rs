/// and_then的语义：
/// 如果当前步骤成功，就继续；否则立刻停下
///
/// map和and_then的区别：
/// 如果闭包返回的是普通值，那么用map
/// 如果闭包返回的是Option/Result，那么用and_the
///
/// 优点：
///
fn main() {
    let input = Some("42");

    let result = input
        .and_then(check_non_empty)
        .and_then(check_numeric)
        .and_then(check_positive);

    println!("{:?}", result); // Some(42)
}

fn check_non_empty(s: &str) -> Option<&str> {
    if s.is_empty() { None } else { Some(s) }
}

fn check_numeric(s: &str) -> Option<i32> {
    s.parse::<i32>().ok()
}

fn check_positive(n: i32) -> Option<i32> {
    if n > 0 { Some(n) } else { None }
}