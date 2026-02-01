use std::fs::File;
use std::io::{self, Read};

fn open_file(path: &str) -> Result<File, io::Error> {
    File::open(path)
}

fn read_file(mut f: File) -> Result<String, io::Error> {
    let mut s = String::new();
    f.read_to_string(&mut s)?;
    Ok(s)
}

/// 只有“成功打开文件”，才“继续读取内容”
fn main() {
    let result = open_file("hello.txt")
        .and_then(read_file);

    println!("{:?}", result);
}
