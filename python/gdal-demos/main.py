import sys
import os
from osgeo import gdal, osr

def print_header(title):
    print("\n" + "=" * 60)
    print(title)
    print("=" * 60)


def main():
    # -----------------------------
    # Python 信息
    # -----------------------------
    print_header("Python Environment")

    print("Python executable:", sys.executable)
    print("Python version   :", sys.version)
    print("Platform         :", sys.platform)

    # -----------------------------
    # GDAL 基本信息
    # -----------------------------
    print_header("GDAL Information")

    print("GDAL VersionInfo():", gdal.VersionInfo())
    print("GDAL Release Name :", gdal.__version__)

    try:
        print("GDAL_DATA         :", gdal.GetConfigOption("GDAL_DATA"))
    except Exception as e:
        print("GDAL_DATA         : <error>", e)

    try:
        print("PROJ_LIB          :", gdal.GetConfigOption("PROJ_LIB"))
    except Exception as e:
        print("PROJ_LIB          : <error>", e)

    # -----------------------------
    # 驱动信息
    # -----------------------------
    print_header("GDAL Drivers")

    driver_count = gdal.GetDriverCount()
    print("Driver count:", driver_count)

    common_drivers = [
        "GTiff", "MEM", "VRT"
    ]

    for name in common_drivers:
        drv = gdal.GetDriverByName(name)
        print(f"{name:10s}:", "OK" if drv else "NOT FOUND")

    # -----------------------------
    # PROJ 信息
    # -----------------------------
    print_header("PROJ / Spatial Reference")

    srs = osr.SpatialReference()
    srs.ImportFromEPSG(4326)

    print("EPSG:4326 WKT:")
    print(srs.ExportToPrettyWkt())

    # -----------------------------
    # 环境变量
    # -----------------------------
    print_header("Important Environment Variables")

    keys = [
        "GDAL_DATA",
        "PROJ_LIB",
        "PATH",
        "CONDA_PREFIX",
        "CONDA_DEFAULT_ENV"
    ]

    for k in keys:
        print(f"{k}: {os.environ.get(k)}")


if __name__ == "__main__":
    main()
