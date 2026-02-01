/// 可变引用
/// 可变引用表明了：change函数将对它借用的值进行改动
fn main() {
    let mut s = String::from("hello");

    change(&mut s);
}

fn change(some_string: &mut String) {
    some_string.push_str(", world");
}


/// 如果有一个对该变量的可变引用
/// 就不能再创建对该变量的引用
/// 好处：编译时就避免数据竞争，数据竞争会导致UB，难以在运行时排查
fn main1() {
    let mut s = String::from("hello");

    let r1 = &mut s;
    // let r2 = &mut s;    // 编译报错：cannot borrow `s` as mutable more than once at a time

    println!("{}", r1);
}

fn main2() {
    let mut s = String::from("hello");

    {
        let r1 = &mut s;
    } // r1 在这里离开了作用域，所以我们完全可以创建一个新的引用

    let r2 = &mut s;
}

fn main3() {
    let mut s = String::from("hello");

    let r1 = &s; // 没问题
    let r2 = &s; // 没问题
    // let r3 = &mut s; // 编译报错：cannot borrow `s` as mutable because it is also borrowed as immutable

    println!("{}, {}", r1, r2);
}

fn main4() {
    let mut s = String::from("hello");

    let r1 = &s; // 没问题
    let r2 = &s; // 没问题
    println!("{r1} and {r2}");
    // 此位置之后 r1 和 r2 的作用域结束了

    let r3 = &mut s; // 没问题
    println!("{r3}");
}