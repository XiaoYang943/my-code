use crate::coord::CoordTrait;
use crate::iterator::impl_iterator;

pub trait LineStringTrait {
    type T: Copy;

    type CoordType<'a>: 'a + CoordTrait<T = Self::T>
    where
        Self: 'a;

    fn num_coords(&self) -> usize;

    fn coord(&self, i: usize) -> Option<Self::CoordType<'_>> {
        if i >= self.num_coords() { None } else { unsafe { Some(self.coord_unchecked(i)) } }
    }

    /// # Safety
    /// i 越界是 UB（实现者负责）
    unsafe fn coord_unchecked(&self, i: usize) -> Self::CoordType<'_>;

    fn coords(&self) -> LineStringIterator<'_, Self::T, Self::CoordType<'_>, Self>
    where
        Self: Sized,
    {
        LineStringIterator::new(self, 0, self.num_coords())
    }
}

impl_iterator!(LineStringIterator, LineStringTrait, CoordTrait, coord_unchecked, CoordType, CoordType);

/// 一个 Vec-backed 线：典型内存布局
#[derive(Debug)]
pub struct VecLineString<T: Copy> {
    pub coords: Vec<crate::coord::XY<T>>,
}

impl<T: Copy> LineStringTrait for VecLineString<T> {
    type T = T;
    type CoordType<'a> = crate::coord::XY<T> where Self: 'a;

    fn num_coords(&self) -> usize { self.coords.len() }

    unsafe fn coord_unchecked(&self, i: usize) -> Self::CoordType<'_> {
        *self.coords.get_unchecked(i)
    }
}


impl<'a, T, L> LineStringTrait for &'a L
where
    T: Copy,
    L: LineStringTrait<T = T>,
{
    type T = T;

    type CoordType<'b>
    = L::CoordType<'b>
    where
        Self: 'b;

    fn num_coords(&self) -> usize {
        // *self 是 &L
        // **self 是 L
        (**self).num_coords()
    }

    unsafe fn coord_unchecked(&self, i: usize) -> Self::CoordType<'_> {
        (**self).coord_unchecked(i)
    }
}
