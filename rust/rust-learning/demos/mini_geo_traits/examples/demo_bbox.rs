use mini_geo_traits::algorithm::bbox::bbox;
use mini_geo_traits::coord::XY;
use mini_geo_traits::geometry::SimpleGeometry;
use mini_geo_traits::line_string::VecLineString;
use mini_geo_traits::polygon::SimplePolygon;

fn main() {
    let ls = VecLineString { coords: vec![XY { x: 0.0, y: 1.0 }, XY { x: 2.0, y: -1.0 }] };
    let g = SimpleGeometry::LS(ls);
    println!("{:?}", bbox(&g)); // Some((0.0, -1.0, 2.0, 1.0))

    let poly = SimplePolygon {
        exterior: Some(VecLineString { coords: vec![XY { x: 0.0, y: 0.0 }, XY { x: 1.0, y: 0.0 }] }),
        holes: vec![],
    };
    let g2 = SimpleGeometry::Poly(poly);
    println!("{:?}", bbox(&g2));
}
