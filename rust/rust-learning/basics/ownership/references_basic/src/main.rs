fn main() {
    println!("=== Bad practice: transfer ownership ===");

    let s1 = String::from("hello");

    // ❌ Bad：只是想读长度，却被迫转移所有权
    let (s1_back, len1) = calculate_length_bad(s1);

    // s1 已经失效，只能使用“还回来”的变量
    println!("The length of '{s1_back}' is {len1}.");

    println!();
    println!("=== Best practice: borrow with reference ===");

    let s2 = String::from("hello");

    // ✅ Good：只借用，不拿走所有权
    let len2 = calculate_length_good(&s2);

    // s2 仍然有效
    println!("The length of '{s2}' is {len2}.");
}

// --------------------------------------------------
// ❌ Bad practice
// --------------------------------------------------
// 获取 String 的所有权，仅仅为了读取长度
// 为了不让调用方失去 String，只能再把它返回
fn calculate_length_bad(s: String) -> (String, usize) {
    let length = s.len();
    (s, length)
}

// --------------------------------------------------
// ✅ Best practice
// --------------------------------------------------
// 使用&引用（reference）：只读访问，不转移所有权
fn calculate_length_good(s: &String) -> usize {
    s.len()
}
