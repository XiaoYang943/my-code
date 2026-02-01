/// 悬垂引用（dangling reference）：
/// 一个引用指向了已经离开作用域、其内存已被释放的值。
///
/// 在 Rust 中，编译器通过生命周期检查保证：
/// 任何引用都不会比它所引用的数据活得更久，
/// 因此悬垂引用会在编译期被直接禁止。
fn main() {
    // let reference_to_nothing = dangle(); // 编译错误：返回对局部变量的引用
    let reference_to_nothing = no_dangle();
}

// fn dangle() -> &String {
//     let s = String::from("hello");
//     &s
//     // 错误原因：
//     // s 在函数结束时被 drop，而返回的引用会逃逸出当前作用域
// }

/// 正确做法：返回所有权，而不是返回引用
fn no_dangle() -> String {
    let s = String::from("hello");
    s
}
