// 返回值也会发生所有权转移（move）
fn main() {
    // ── 情况 1：从函数“拿到”一个 String ──
    let s1 = gives_ownership();
    // gives_ownership 创建了一个 String，
    // 并把它的所有权通过返回值 move 给 s1
    // 此时：
    // - s1 有效，拥有该 String
    // - gives_ownership 内部的变量已经失效

    // ── 情况 2：把 String 传给函数，再拿回来 ──
    let s2 = String::from("hello");
    // s2 有效，拥有 String("hello")

    let s3 = takes_and_gives_back(s2);
    // s2 的所有权 move 进 takes_and_gives_back
    // 因此：s2 在这里已经失效（不能再使用）
    // 函数又把同一个 String 作为返回值 move 给 s3
    // 此时：
    // - s3 有效
    // - s2 已失效

    // println!("{}", s2); // 编译错误：Value used after being moved
    println!("{}", s3);
} // 作用域结束：s3 和 s1 依次被 drop（释放堆内存）

// 创建一个 String，并把所有权返回给调用者
fn gives_ownership() -> String {
    let some_string = String::from("yours");
    // some_string 在这里有效

    some_string
    // 返回值发生 move：
    // some_string 的所有权被转移给调用方
    // 本函数不再负责释放它
}

// 接收一个 String，并原样返回
fn takes_and_gives_back(a_string: String) -> String {
    // a_string 拥有 String

    a_string
    // 返回时发生 move：
    // 所有权转移回调用方
}
