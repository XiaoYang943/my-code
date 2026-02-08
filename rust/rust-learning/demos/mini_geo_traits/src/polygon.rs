use crate::iterator::impl_iterator;
use crate::line_string::{LineStringTrait, VecLineString};

pub trait PolygonTrait {
    type T: Copy;

    type RingType<'a>: 'a + LineStringTrait<T = Self::T>
    where
        Self: 'a;

    fn exterior(&self) -> Option<Self::RingType<'_>>;

    fn num_interiors(&self) -> usize;

    fn interior(&self, i: usize) -> Option<Self::RingType<'_>> {
        if i >= self.num_interiors() { None } else { unsafe { Some(self.interior_unchecked(i)) } }
    }

    unsafe fn interior_unchecked(&self, i: usize) -> Self::RingType<'_>;

    fn interiors(&self) -> PolygonInteriorIterator<'_, Self::T, Self::RingType<'_>, Self>
    where
        Self: Sized,
    {
        PolygonInteriorIterator::new(self, 0, self.num_interiors())
    }
}

impl_iterator!(PolygonInteriorIterator, PolygonTrait, LineStringTrait, interior_unchecked, RingType, RingType);

#[derive(Debug)]
pub struct SimplePolygon<T: Copy> {
    pub exterior: Option<VecLineString<T>>,
    pub holes: Vec<VecLineString<T>>,
}

impl<T: Copy> PolygonTrait for SimplePolygon<T> {
    type T = T;
    type RingType<'a> = &'a VecLineString<T> where Self: 'a;

    fn exterior(&self) -> Option<Self::RingType<'_>> {
        self.exterior.as_ref()
    }

    fn num_interiors(&self) -> usize { self.holes.len() }

    unsafe fn interior_unchecked(&self, i: usize) -> Self::RingType<'_> {
        self.holes.get_unchecked(i)
    }
}
