use mini_geo_traits::{CoordTrait, LineStringTrait};
use mini_geo_traits::coord::{Dimensions, XY};

/// 一个“扁平内存布局”的线：coords 存成 [x0,y0,x1,y1,...]
#[derive(Debug)]
struct FlatLineString {
    coords_xy: Vec<f64>,
}

#[derive(Clone, Copy)]
struct FlatCoord<'a> {
    dim: Dimensions,
    buf: &'a [f64],
    i: usize, // 第 i 个点
}

impl<'a> CoordTrait for FlatCoord<'a> {
    type T = f64;

    fn dim(&self) -> Dimensions { self.dim }

    fn x(&self) -> f64 { self.buf[self.i * 2] }
    fn y(&self) -> f64 { self.buf[self.i * 2 + 1] }

    fn nth_or_panic(&self, n: usize) -> f64 {
        match n {
            0 => self.x(),
            1 => self.y(),
            _ => panic!("FlatCoord only supports XY"),
        }
    }
}

impl LineStringTrait for FlatLineString {
    type T = f64;
    type CoordType<'a> = FlatCoord<'a> where Self: 'a;

    fn num_coords(&self) -> usize { self.coords_xy.len() / 2 }

    unsafe fn coord_unchecked(&self, i: usize) -> Self::CoordType<'_> {
        FlatCoord { dim: Dimensions::Xy, buf: &self.coords_xy, i }
    }
}

fn main() {
    let ls = FlatLineString { coords_xy: vec![0.0, 0.0, 10.0, 10.0] };
    for c in ls.coords() {
        println!("{:?}", c.x_y());
    }

    // 你也可以把 FlatCoord 转成“自有”坐标
    let owned: Vec<XY<f64>> = ls.coords().map(|c| XY { x: c.x(), y: c.y() }).collect();
    println!("{owned:?}");
}
