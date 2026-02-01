/// 消息处理循环
#[derive(Debug)]
enum Message {
    Data(String),
    Ping,
    Quit,
}

fn recv_message() -> Option<Message> {
    static mut COUNT: i32 = 0;

    unsafe {
        COUNT += 1;
        match COUNT {
            1 => Some(Message::Ping),
            2 => Some(Message::Data("hello".into())),
            3 => Some(Message::Quit),
            _ => None,
        }
    }
}

fn main() {
    // 消费消息，直到没有为止
    while let Some(msg) = recv_message() {
        match msg {
            Message::Ping => println!("ping"),
            Message::Data(s) => println!("data = {}", s),
            Message::Quit => {
                println!("quit");
                break;
            }
        }
    }
}
