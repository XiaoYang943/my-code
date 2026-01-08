const THREE_HOURS_IN_SECONDS: u32 = 60 * 60 * 3; // 常量

fn main() {
    let x = 5; // 不可变(immutable)的变量，不能二次赋值
    let mut y = 5; // 可变(mutable)的变量

    y = x;
    // x = y;

    println!("{}", x);
    println!("{}", y);
    println!("{}", THREE_HOURS_IN_SECONDS);
}
