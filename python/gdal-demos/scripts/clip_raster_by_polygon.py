import os
from osgeo import gdal, ogr

def clip_tif_by_polygons(tif_path, vector_path, output_dir, id_field="id"):
    # 打开矢量图层
    vector_ds = ogr.Open(vector_path)
    layer = vector_ds.GetLayer()

    # 遍历每个多边形
    for feature in layer:
        geom = feature.GetGeometryRef()
        poly_id = feature.GetField(id_field)

        # 创建一个临时矢量文件只包含当前多边形
        drv = ogr.GetDriverByName("GeoJSON")
        temp_vector_path = f"/tmp/temp_{poly_id}.geojson"
        if os.path.exists(temp_vector_path):
            os.remove(temp_vector_path)
        temp_ds = drv.CreateDataSource(temp_vector_path)
        temp_layer = temp_ds.CreateLayer("layer", geom_type=ogr.wkbPolygon)
        temp_layer.CreateFeature(feature.Clone())
        temp_ds = None  # 关闭写入

        # 裁剪输出路径
        output_path = os.path.join(output_dir, f"clip_{poly_id}.tif")

        # 裁剪
        gdal.Warp(
            output_path,
            tif_path,
            cutlineDSName=temp_vector_path,
            cropToCutline=True,
            dstNodata=0,
            format='GTiff'
        )

        print(f"裁剪完成：{output_path}")
        os.remove(temp_vector_path)  # 删除临时矢量文件

    print("所有裁剪完成。")

# 示例用法
clip_tif_by_polygons(
    tif_path=r"D:\data\tmp\shaanxi.tif",
    vector_path=r"D:\data\矢量数据\图幅接合表\网格\陕西_一比十万_缓冲800米.geojson",
    output_dir=r"D:\data\tmp",
    id_field="MSC"  # 图幅编号字段
)