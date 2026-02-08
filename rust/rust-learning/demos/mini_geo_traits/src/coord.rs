#[derive(Clone, Copy, Debug, PartialEq, Eq)]
pub enum Dimensions {
    Xy, // 丐版：只做 2D
}
impl Dimensions {
    pub fn size(self) -> usize { 2 }
}

/// 丐版 CoordTrait：保留安全/不安全分层
pub trait CoordTrait {
    type T: Copy;

    fn dim(&self) -> Dimensions { Dimensions::Xy }

    fn x(&self) -> Self::T;
    fn y(&self) -> Self::T;

    fn x_y(&self) -> (Self::T, Self::T) { (self.x(), self.y()) }

    /// 安全访问：越界返回 None
    fn nth(&self, n: usize) -> Option<Self::T> {
        if n < self.dim().size() { Some(self.nth_or_panic(n)) } else { None }
    }

    /// 更底层：越界允许 panic
    fn nth_or_panic(&self, n: usize) -> Self::T;

    /// 允许实现者用 unsafe 做极致优化（默认实现是安全的）
    unsafe fn nth_unchecked(&self, n: usize) -> Self::T {
        self.nth_or_panic(n)
    }
}

/// 一个简单坐标类型（你也可以换成 geo-types::Coord）
#[derive(Clone, Copy, Debug)]
pub struct XY<T: Copy> {
    pub x: T,
    pub y: T,
}
impl<T: Copy> CoordTrait for XY<T> {
    type T = T;
    fn x(&self) -> T { self.x }
    fn y(&self) -> T { self.y }
    fn nth_or_panic(&self, n: usize) -> T {
        match n {
            0 => self.x,
            1 => self.y,
            _ => panic!("XY only supports 2 dimensions"),
        }
    }
}
