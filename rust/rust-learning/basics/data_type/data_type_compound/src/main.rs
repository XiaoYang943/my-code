// 复合类型（Compound types）可以将多个值组合成一个类型。Rust 有两个原生的复合类型：元组（tuple）和数组（array）
fn main() {
    // 元组：
    // 元组中的每个元素的类型可以不相同
    // 元组的长度是固定的
    let tuple: (i32, f64, u8) = (500, 6.4, 1); // 创建了一个元组并绑定到 tuple 变量上

    let (x, y, z) = tuple; // 用了 let 和模式匹配（pattern matching）将 tuple 解构（destructuring）成了三个不同的变量，x、y 和 z

    let first = tuple.0; // 可以使用点号（.）后跟值的索引来直接访问所需的元组元素

    // 数组：
    // 数组中的每个元素的类型必须相同
    // 数组的长度是固定的
    let array: [i32; 5] = [-1, 0, 1, 2, 3];
    let array: [u32; 5] = [0, 1, 2, 3, 4];
    let array: [u32; 5] = [0; 5]; // 初始化5个0

    let first = array[0];
    let second = array[1];
}
