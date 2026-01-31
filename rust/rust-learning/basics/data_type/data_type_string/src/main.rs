/// 字符串类型 String
///
/// 数据结构(存在栈上)：
/// prt 指针: 指向存放字符串内容堆内存的指针
/// len 长度: String 的内容当前使用了多少字节的内存
/// capacity 容量: String 从分配器总共获取了多少字节的内存
///
/// 特点：
/// 1. 值可变
fn main() {
    let mut s = String::from("hello");

    s.push_str(", world!"); // 在字符串后追加字面值

    println!("{s}");
}
