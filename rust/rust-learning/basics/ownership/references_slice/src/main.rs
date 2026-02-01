/// slice(切片) 是对一段连续数据的“借用视图”，它不拥有数据，只是“指向 + 长度”。
///
/// 总结：
/// slice 是 Rust 中“零拷贝 + 安全抽象”的核心工具。用来表达：“我只想看一部分，不想拥有它”
///
/// 最常用的两种slice:
/// 字符串切片：&str
/// 数组切片：&[T]
fn main() {
    let s = String::from("hello world");

    let hello = &s[0..5];
    let world = &s[6..11];

    println!("hello = {}", hello);
    println!("world = {}", world);
}