from osgeo import gdal
import os

gdal.UseExceptions()


def remove_srs(src_path, dst_path):
    print(f"\nProcessing: {src_path}")

    ds = gdal.Open(src_path, gdal.GA_ReadOnly)
    if ds is None:
        print("❌ 打开失败")
        return

    driver = gdal.GetDriverByName("GTiff")

    # 创建副本
    out_ds = driver.CreateCopy(dst_path, ds, strict=0)

    # ⭐ 清除 SRS
    out_ds.SetProjection("")
    out_ds.SetGeoTransform((0, 1, 0, 0, 0, -1))
    # ↑ 可选：如果你想连地理参考一起干掉

    out_ds.FlushCache()

    ds = None
    out_ds = None

    # 删除 aux.xml（ArcGIS最爱读这个）
    aux = src_path + ".aux.xml"
    if os.path.exists(aux):
        os.remove(aux)
        print("✔ 删除 aux.xml")

    print("✅ 完成 ->", dst_path)


def batch_remove(folder):
    for root, dirs, files in os.walk(folder):
        for f in files:
            if f.lower().endswith(".tif"):
                src = os.path.join(root, f)

                dst = os.path.join(
                    root,
                    f.replace(".tif", "_nosrs.tif")
                )

                try:
                    remove_srs(src, dst)
                except Exception as e:
                    print("❌ 出错:", e)


if __name__ == "__main__":
    folder = r"D:\data\temp"
    batch_remove(folder)
