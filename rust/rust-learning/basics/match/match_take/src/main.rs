// fn main1() {
//     let v = Some(String::from("hello"));
//
//     let s = match v {
//         Some(s) => s,
//         None => String::from("default"),
//     };
//
//     // v 不能再用了（v 被 move）
// }

/// take() 是 Option<T> 的一个方法：
/// 把 Option 里包着的值“拿出来”（如果有）
/// 同时把原来的 Some 变成 None(所以v必须是 mut 的)
fn main() {
    let mut v = Some(String::from("hello"));

    let s = match v.take() {
        Some(s) => s,
        None => String::from("default"),
    };

    println!("taken = {}", s);  // taken = hello
    println!("v after take = {:?}", v); // v after take = None
}
