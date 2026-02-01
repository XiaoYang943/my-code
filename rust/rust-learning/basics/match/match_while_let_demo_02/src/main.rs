/// 重试任务
fn try_fetch() -> Result<i32, &'static str> {
    static mut ATTEMPT: i32 = 0;
    unsafe {
        ATTEMPT += 1;
        if ATTEMPT < 3 {
            Err("temporary error")
        } else {
            Ok(42)
        }
    }
}

fn main() {
    // 重试直到成功
    while let Err(e) = try_fetch() {
        println!("retrying... ({})", e);
    }

    println!("success!");
}
