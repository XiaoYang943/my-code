fn main1() { // s还未声明，s无效
    let s = "hello";    // 字符串字面量s大小已知，进入作用域时，值进入程序的只读静态区，s引用入栈，s有效
}   // 作用域结束，栈帧被销毁，值继续存在

fn main2() {
    // 请求所需的内存
    // 栈上保存 String 结构体
    // 堆上保存实际的字符串数据，s 拥有这块堆内存
    let mut s = String::from("hello");

    s.push_str(", world!"); // 修改堆上的字符串内容，必要时重新分配堆内存

    println!("{s}");
}   // 作用域结束，调用 drop(s)，释放占有的堆内存，栈帧被销毁，s不再有效


fn main3() {
    let x = 5;  // 将 5 绑定到 x，入栈
    let y = x;  // 生成一个值 x 的拷贝并绑定到 y，入栈
    println!("x: {}, y: {}", x, y);

    let s1 = String::from("hello");
    // 将 s1 赋值给 s2：
    // String 没有实现 Copy，因此发生的是所有权的移动（move）
    // 栈上的 ptr指针 / len长度 / cap容量 被按位拷贝到 s2，而指针指向的堆上数据没有动，
    // s2 成为堆内存的唯一所有者，s1 进入 moved 状态
    let s2 = s1;

    // 从实现角度看，赋值时拷贝了指针值，因此曾经指向同一块堆内存
    // 但在 Rust 的语义模型中，s1 已经失效（moved-from），不再被认为拥有该内存
    // 如果没有所有权系统，当 s2 和 s1 离开作用域时，这种浅拷贝 + 析构会导致 double free 问题(尝试释放相同的内存)

    // println!("s1: {}", s1); // 编译不通过：Value used after being moved
    println!("s2: {}", s2);
}

fn main4() {
    let mut s = String::from("rust");
    s = String::from("hello");  // 给一个已有的变量赋一个全新的值时,Rust 立即调用 drop 并释放原始值的内存

    println!("{s}, world!");    // hello, world!
}

fn main() {
    let s1 = String::from("hello");
    let s2 = s1.clone();    // 复制一份进行使用

    println!("s1 = {s1}, s2 = {s2}");   // s1 = hello, s2 = hello
}