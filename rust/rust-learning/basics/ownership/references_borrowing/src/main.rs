// 将创建一个引用的行为称为 借用(borrowing)(借来，使用完后，必须还回去。因为我们并不拥有它的所有权)
fn main() {
    let s = String::from("hello");

    change(&s); // 这个行为叫做：借用
}

fn change(some_string: &String) {
    // 变量默认是不可变的，引用也一样。（默认）不允许修改引用的值
    // some_string.push_str(", world"); // 编译报错：Cannot borrow immutable local variable `some_string` as mutable
}