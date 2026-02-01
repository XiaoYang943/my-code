// match 时是否发生所有权转移，取决于 match 的对象：
//
// 1. 如果 match 的是 Option<T> 本身，且 T 不是 Copy，
//    那么在模式中解构 Some(x) 会发生所有权转移。
//
// 2. 如果 match 的是 &Option<T>（引用），
//    那么解构得到的是 &T，不会发生所有权转移。

fn main() {
    let v = Some(String::from("hello"));

    match v {
        Some(s) => {
            println!("value = {}", s);
        }
        None => {
            println!("no value");
        }
    }

    // println!("{:?}", v); // ❌ 编译错误
}

fn main2() {
    let v = Some(String::from("hello"));

    match &v {
        Some(s) => {
            println!("value = {}", s);
        }
        None => {
            println!("no value");
        }
    }

    // 现在可以继续使用 v
    println!("v is still usable");
}

