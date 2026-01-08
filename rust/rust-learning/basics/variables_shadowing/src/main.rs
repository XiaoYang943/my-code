fn main() {
    let x = 5;
    // 可以定义一个与之前变量同名的新变量，这时，一个变量被第二个遮蔽（Shadowing） 了
    // 此时任何使用该变量名的行为中，都会视为是在使用第二个变量，直到第二个变量自己也被遮蔽或第二个变量的作用域结束
    let x = x + 1; // 第一次遮蔽
    {
        let x = x * 2; // 第二次遮蔽
        println!("The value of x in the inner scope is: {x}");
    }

    println!("The value of x is: {x}");
}
