use crate::coord::CoordTrait;

pub trait PointTrait {
    type T: Copy;

    type CoordType<'a>: 'a + CoordTrait<T = Self::T>
    where
        Self: 'a;

    /// Simple Features 里点可以是 empty，所以用 Option
    fn coord(&self) -> Option<Self::CoordType<'_>>;
}

/// 一个简单 Point：用 Option 表示 empty
#[derive(Debug)]
pub struct SimplePoint<T: Copy> {
    pub coord: Option<crate::coord::XY<T>>,
}

impl<T: Copy> PointTrait for SimplePoint<T> {
    type T = T;
    type CoordType<'a> = crate::coord::XY<T> where Self: 'a;

    fn coord(&self) -> Option<Self::CoordType<'_>> {
        self.coord
    }
}
