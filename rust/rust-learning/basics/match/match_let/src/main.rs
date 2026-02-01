// ===============================
// if let / while let 教学示例合集
// ===============================
//
// 核心思想（先记住这一句）：
// if let / while let = 用“模式匹配”作为条件
// → 匹配成功：解构并执行
// → 匹配失败：跳过（if）或结束循环（while）

fn main() {
    main_01();
    main_02();
    main_03();
    main_04();
    main_05();
}

// ------------------------------------------------
// DEMO 1：if let —— 只关心 Some 的情况
// ------------------------------------------------
fn main_01() {
    let v = Some(10);
    // v 的类型是 Option<i32>
    // v 可能是：
    //   - Some(10)
    //   - None

    // if let PATTERN = EXPR 的含义是：
    // “如果 EXPR 的结构能匹配 PATTERN，就执行代码块”
    //
    // 这里的 PATTERN 是 Some(x)
    // 如果 v == Some(10)，那么：
    //   x = 10
    //   执行 println!
    //
    // 如果 v == None：
    //   条件不成立，整个 if let 直接跳过
    if let Some(x) = v {
        println!("main_01: value = {}", x);
    }

    // 等价于下面这个 match（但 if let 更简洁）：
    //
    // match v {
    //     Some(x) => println!("value = {}", x),
    //     None => {}
    // }
}

// ------------------------------------------------
// DEMO 2：if let + Result —— 只关心成功分支
// ------------------------------------------------

// 这个函数返回 Result：
// 成功：Ok(i32)
// 失败：Err(ParseIntError)
fn parse_num(s: &str) -> Result<i32, std::num::ParseIntError> {
    s.parse()
}

fn main_02() {
    // parse_num("42") 的返回值是 Ok(42)

    // if let Ok(n) = ... 的含义是：
    // “如果结果是 Ok(某个值)，就把这个值取出来用”
    //
    // 如果是 Err(_)：
    //   直接忽略，不 panic，也不传播错误
    if let Ok(n) = parse_num("42") {
        println!("main_02: parsed = {}", n);
    }

    // 这种写法常用于：
    // - main 函数
    // - demo / 示例
    // - CLI 层
    //
    // 核心逻辑中更推荐用 Result + ?
}

// ------------------------------------------------
// DEMO 3：if let + as_ref() —— 避免所有权转移
// ------------------------------------------------
fn main_03() {
    let v = Some(String::from("hello"));
    // v 的类型是 Option<String>
    // String 不是 Copy 类型

    // 如果直接写：
    // if let Some(s) = v { ... }
    // 会发生 move（String 被拿走）

    // v.as_ref() 的作用是：
    // Option<String>  →  Option<&String>
    //
    // 也就是说：
    // - 不拿走 String
    // - 只借用里面的值
    if let Some(s) = v.as_ref() {
        // s 的类型是 &String（引用）
        println!("main_03: len = {}", s.len());
    }

    // 因为我们只是“借用”，没有 move
    // 所以 v 在这里仍然是可用的
    println!("main_03: v is still usable");
}

// ------------------------------------------------
// DEMO 4：if let + take() —— 一次性消费
// ------------------------------------------------
fn main_04() {
    let mut token = Some(String::from("secret"));
    // token 是 Option<String>
    // 必须是 mut，因为 take() 会修改它

    // token.take() 的行为：
    // - 如果 token == Some("secret")
    //     → 返回 Some("secret")
    //     → 同时把 token 置为 None
    // - 如果 token == None
    //     → 返回 None
    //
    // take 的本质是：
    // “安全地把值 move 出来，同时留下一个 None”
    if let Some(t) = token.take() {
        println!("main_04: use token = {}", t);
    }

    // 这里 token 一定是 None
    // 因为值已经被“拿走”
    println!("main_04: token after take = {:?}", token);
}

// ------------------------------------------------
// DEMO 5：while let —— 消费直到失败（黄金场景）
// ------------------------------------------------
fn main_05() {
    let mut stack = vec![1, 2, 3];
    // stack.pop() 的返回类型是 Option<i32>：
    // - 有元素：Some(value)
    // - 没元素：None

    // while let 的含义是：
    // “只要表达式的结果还能匹配模式，就继续循环”
    //
    // 每一轮都会执行 stack.pop()
    while let Some(v) = stack.pop() {
        // 如果 pop() 返回 Some(v)：
        //   v 是弹出的元素
        //   执行循环体
        println!("main_05: pop = {}", v);
    }

    // 当 stack.pop() 返回 None：
    // - 模式匹配失败
    // - while let 条件不成立
    // - 循环结束

    // 等价于下面这种写法（更底层，但更啰嗦）：
    //
    // loop {
    //     match stack.pop() {
    //         Some(v) => println!("{}", v),
    //         None => break,
    //     }
    // }
}
