use crate::{PointTrait, LineStringTrait, PolygonTrait};

#[derive(Debug)]
pub enum GeometryType<'a, P, LS, Poly>
where
    P: PointTrait,
    LS: LineStringTrait,
    Poly: PolygonTrait,
{
    Point(&'a P),
    LineString(&'a LS),
    Polygon(&'a Poly),
}

/// 丐版 GeometryTrait：只塞 3 个类型够你理解“模式”
pub trait GeometryTrait {
    type T: Copy;

    type PointType<'a>: 'a + PointTrait<T = Self::T> where Self: 'a;
    type LineStringType<'a>: 'a + LineStringTrait<T = Self::T> where Self: 'a;
    type PolygonType<'a>: 'a + PolygonTrait<T = Self::T> where Self: 'a;

    fn as_type(
        &self,
    ) -> GeometryType<'_, Self::PointType<'_>, Self::LineStringType<'_>, Self::PolygonType<'_>>;
}

/// 一个具体 Geometry 容器：用 enum 存
#[derive(Debug)]
pub enum SimpleGeometry<T: Copy> {
    P(crate::point::SimplePoint<T>),
    LS(crate::line_string::VecLineString<T>),
    Poly(crate::polygon::SimplePolygon<T>),
}

impl<T: Copy> GeometryTrait for SimpleGeometry<T> {
    type T = T;

    type PointType<'a> = crate::point::SimplePoint<T> where Self: 'a;
    type LineStringType<'a> = crate::line_string::VecLineString<T> where Self: 'a;
    type PolygonType<'a> = crate::polygon::SimplePolygon<T> where Self: 'a;

    fn as_type(
        &self,
    ) -> GeometryType<'_, Self::PointType<'_>, Self::LineStringType<'_>, Self::PolygonType<'_>> {
        match self {
            SimpleGeometry::P(p) => GeometryType::Point(p),
            SimpleGeometry::LS(ls) => GeometryType::LineString(ls),
            SimpleGeometry::Poly(poly) => GeometryType::Polygon(poly),
        }
    }
}
