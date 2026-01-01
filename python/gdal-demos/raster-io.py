import numpy as np
from matplotlib import pyplot as plt
from osgeo import gdal
import morecantile

from utils.plt_utils import plot_4_tiles

gdal.UseExceptions()

tif_path = ""

tms = morecantile.tms.get("WGS1984Quad")

def sample_dem(
    tif_path,
    xmin, ymin, xmax, ymax,
    tile_size=65,
    method="warp",
    resample_alg=gdal.GRA_Bilinear,
):

    src_ds = gdal.Open(tif_path, gdal.GA_ReadOnly)
    if src_ds is None:
        raise RuntimeError("Cannot open DEM")

    if method == "warp":
        return _sample_with_warp(
            src_ds,
            xmin, ymin, xmax, ymax,
            tile_size,
            resample_alg
        )

    elif method == "rasterio":
        return _sample_with_rasterio(
            src_ds, xmin, ymin, xmax, ymax,
            tile_size, resample_alg
        )

    elif method == "rasterio":
        return []

    else:
        raise ValueError(f"Unknown sampling method: {method}")

def _sample_with_rasterio(
    src_ds,
    xmin, ymin, xmax, ymax,
    tile_size,
    resample_alg
):
    band = src_ds.GetRasterBand(1)

    """
    仿射变换六参数(这六个参数构建了图像行列号与投影坐标之间的转换关系)
    geoTransform[0]：左上角x坐标
    geoTransform[1]：东西方向空间分辨率
    geoTransform[2]：x方向旋转角
    geoTransform[3]：左上角y坐标
    geoTransform[4]：y方向旋转角
    geoTransform[5]：南北方向空间分辨率
    <p>
    GeoTransform默认值
    0 1 0 0 0 1
    TFW默认值
    1 0 0 1 0 0
    """
    gt = src_ds.GetGeoTransform()

    # 地理坐标->像素坐标
    def geo_to_pixel(x, y):
        px = (x - gt[0]) / gt[1]
        py = (y - gt[3]) / gt[5]
        return px, py

    # 计算像素窗口坐标
    px_min, py_max = geo_to_pixel(xmin, ymax)
    px_max, py_min = geo_to_pixel(xmax, ymin)

    xoff = int(np.floor(px_min))
    yoff = int(np.floor(py_max))

    win_xsize = int(np.ceil(px_max - px_min))
    win_ysize = int(np.ceil(py_min - py_max))

    # RasterIO 重采样
    data = band.ReadAsArray(
        xoff=xoff,
        yoff=yoff,
        win_xsize=win_xsize,
        win_ysize=win_ysize,
        buf_xsize=tile_size,
        buf_ysize=tile_size,
        buf_type=gdal.GDT_Float32
    )

    """ 
    以 12-6603-2832 这张瓦片为例：
    
    在源数据中的位置：左上角(16890,18344)
    TMS(WGS84)网格集下的像素窗口大小：159*159
    从源数据中，使用159*159的像素窗口作为源采样窗口，通过插值计算出65*65个顶点高度
    """
    # data = band.ReadAsArray(
    #     xoff=16890,
    #     yoff=18344,
    #     win_xsize=159,
    #     win_ysize=159,
    #     buf_xsize=65,
    #     buf_ysize=65,
    #     buf_type=gdal.GDT_Float32
    # )

    # data = band.ReadAsArray(
    #     xoff=16890,
    #     yoff=18344,
    #     win_xsize=159,
    #     win_ysize=159,
    #     buf_xsize=159,
    #     buf_ysize=159,
    #     buf_type=gdal.GDT_Float32
    # )

    return data



def _sample_with_warp(
    src_ds,
    xmin, ymin, xmax, ymax,
    tile_size,
    resample_alg
):

    warp_options = gdal.WarpOptions(
        format="VRT",
        outputBounds=(xmin, ymin, xmax, ymax),
        width=tile_size,
        height=tile_size,
        resampleAlg=resample_alg,
    )

    vrt = gdal.Warp("", src_ds, options=warp_options)
    if vrt is None:
        raise RuntimeError("Warp failed")

    band = vrt.GetRasterBand(1)
    data = band.ReadAsArray().astype(np.float32)

    return data


def flip_y(z: int, y: int) -> int:
    return (1 << z) - 1 - y

def main():
    bounds = tms.xy_bounds(morecantile.Tile(6603, flip_y(12, 2832), 12))

    print("=== Warp sampling ===")
    h_warp = sample_dem(
        tif_path,
        bounds.left, bounds.bottom,
        bounds.right, bounds.top,
        tile_size=65,
        method="warp"
    )
    print("min/max:", h_warp.min(), h_warp.max())

    print("=== rasterio sampling ===")
    h_rasterio = sample_dem(
        tif_path,
        bounds.left, bounds.bottom,
        bounds.right, bounds.top,
        tile_size=65,
        method="rasterio"
    )
    print("min/max:", h_rasterio.min(), h_rasterio.max())

    plt.figure(figsize=(6, 6))
    plt.imshow(h_warp, cmap="terrain")
    plt.colorbar(label="Elevation")
    plt.title("Warp Sampling Height")
    plt.show()

    plt.figure(figsize=(6, 6))
    plt.imshow(h_rasterio, cmap="terrain")
    plt.colorbar(label="Elevation")
    plt.title("RasterIO Sampling Height")
    plt.show()


def main_mosaic():
    bounds_left_top = tms.xy_bounds(morecantile.Tile(6603, flip_y(12, 2832), 12))
    bounds_right_top = tms.xy_bounds(morecantile.Tile(6604, flip_y(12, 2832), 12))
    bounds_left_bottom = tms.xy_bounds(morecantile.Tile(6603, flip_y(12, 2831), 12))
    bounds_right_bottom = tms.xy_bounds(morecantile.Tile(6604, flip_y(12, 2831), 12))

    left_top_rasterio = sample_dem(
        tif_path,
        bounds_left_top.left, bounds_left_top.bottom,
        bounds_left_top.right, bounds_left_top.top,
        tile_size=65,
        method="rasterio"
    )

    right_top_rasterio = sample_dem(
        tif_path,
        bounds_right_top.left, bounds_right_top.bottom,
        bounds_right_top.right, bounds_right_top.top,
        tile_size=65,
        method="rasterio"
    )

    left_bottom_rasterio = sample_dem(
        tif_path,
        bounds_left_bottom.left, bounds_left_bottom.bottom,
        bounds_left_bottom.right, bounds_left_bottom.top,
        tile_size=65,
        method="rasterio"
    )

    right_bottom_rasterio = sample_dem(
        tif_path,
        bounds_right_bottom.left, bounds_right_bottom.bottom,
        bounds_right_bottom.right, bounds_right_bottom.top,
        tile_size=65,
        method="rasterio"
    )

    plot_4_tiles(left_top_rasterio,right_top_rasterio,left_bottom_rasterio,right_bottom_rasterio)

if __name__ == "__main__":
    tif_path = r"D:\data\栅格数据\数字高程模型\地形融合\陕西_DEM_12米.tif"

    main()

    main_mosaic()
