use crate::{
    CoordTrait,
    GeometryTrait,
    GeometryType,
    LineStringTrait,
    PolygonTrait,
    PointTrait,
};

pub fn bbox<G>(g: &G) -> Option<(f64, f64, f64, f64)>
where
    G: GeometryTrait<T = f64>,
{
    match g.as_type() {
        GeometryType::Point(p) => {
            let c = p.coord()?;
            let (x, y) = c.x_y();
            Some((x, y, x, y))
        }

        GeometryType::LineString(ls) => bbox_of_linestring(ls),

        GeometryType::Polygon(poly) => {
            let ext = poly.exterior()?;
            bbox_of_linestring(&ext)
        }
    }
}

/// implementation detail — 不要 pub!
fn bbox_of_linestring<L>(ls: &L) -> Option<(f64, f64, f64, f64)>
where
    L: LineStringTrait<T = f64>,
{
    if ls.num_coords() == 0 {
        return None;
    }

    let mut iter = ls.coords();
    let first = iter.next()?;

    let (mut minx, mut miny) = first.x_y();
    let (mut maxx, mut maxy) = (minx, miny);

    for c in iter {
        let (x, y) = c.x_y();
        minx = minx.min(x);
        miny = miny.min(y);
        maxx = maxx.max(x);
        maxy = maxy.max(y);
    }

    Some((minx, miny, maxx, maxy))
}
