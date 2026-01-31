fn main() {
    let s = String::from("hello");  // s 进入作用域

    takes_ownership(s); // s 移动到函数里 ...
    // s到这里不再有效
    // println!("{}", s);   // 编译报错：Value used after being moved[E0382]

    let mut x = 5;                      // x 进入作用域
    makes_copy(x);  // x 应该移动函数里，
    // 但 i32 是 Copy 的(实现了 Copy trait)，所以在后面可继续使用 x
    println!("{}", x);  // 5
    x += 1;
    println!("{}", x);  // 6

} // x 先移出了作用域，然后是 s。但 s 的值已被移走，即这里不做处理


fn takes_ownership(some_string: String) { // some_string 进入作用域
    println!("{some_string}");  // hello
} // some_string 移出作用域并调用 drop 方法,占用的内存被释放


fn makes_copy(some_integer: i32) { // some_integer 进入作用域
    println!("{some_integer}"); // 5
} // some_integer 移出作用域
