# -*- coding: utf-8 -*-
# import os
# os.environ["GDAL_DRIVER_PATH"] = r"D:\soft\runtime\miniconda3\envs\my_gis_env\Library\lib\gdalplugins"

from osgeo import ogr

# ========================
# 🔧 这里修改你的配置
# ========================

GDB_FOLDER = r"D:\data\矢量数据\香港地形图\iB1000_Fullset_Tile-based_FGDB"

PG_HOST = "localhost"
PG_PORT = "5432"
PG_DB = "scene_map_server"
PG_SCHEMA = "test"
PG_USER = "postgres"
PG_PASSWORD = "a1352253543A@"

# ========================


def normalize_name(name: str) -> str:
    """
    规范化表名
    """
    name = name.lower()
    result = []
    for c in name:
        if c.isalnum() or c == "_":
            result.append(c)
        else:
            result.append("_")
    return "".join(result)


def build_pg_conn_str():
    return (
        f"PG:host={PG_HOST} "
        f"port={PG_PORT} "
        f"dbname={PG_DB} "
        f"user={PG_USER} "
        f"password={PG_PASSWORD}"
    )


def create_spatial_index(pg_ds, schema, table, geom_column="geom"):
    """
    创建 GIST 空间索引 + ANALYZE
    """
    print(f"  创建空间索引 {schema}.{table}")

    sql_index = f"""
        CREATE INDEX IF NOT EXISTS {table}_geom_gist
        ON {schema}.{table}
        USING GIST ({geom_column})
    """
    pg_ds.ExecuteSQL(sql_index)

    pg_ds.ExecuteSQL(f"ANALYZE {schema}.{table}")


def import_gdb(gdb_path):
    print(f"\n========== 处理 GDB: {gdb_path} ==========")

    driver = ogr.GetDriverByName("OpenFileGDB")
    datasource = driver.Open(gdb_path, 0)

    if datasource is None:
        print(f"无法打开 GDB: {gdb_path}")
        return

    pg_conn_str = build_pg_conn_str()
    pg_ds = ogr.Open(pg_conn_str, update=1)

    if pg_ds is None:
        print("无法连接 PostGIS")
        return

    layer_count = datasource.GetLayerCount()
    print(f"GDB 图层数量: {layer_count}")

    for i in range(layer_count):
        layer = datasource.GetLayerByIndex(i)
        layer_name = layer.GetName()
        table_name = normalize_name(layer_name)

        print(f"\n导入图层: {layer_name} -> {PG_SCHEMA}.{table_name}")

        # 删除旧表
        drop_sql = f"DROP TABLE IF EXISTS {PG_SCHEMA}.{table_name} CASCADE"
        pg_ds.ExecuteSQL(drop_sql)

        # 导入
        options = [
            f"SCHEMA={PG_SCHEMA}",
            "OVERWRITE=YES",
            "GEOMETRY_NAME=geom"
        ]

        new_layer = pg_ds.CopyLayer(layer, table_name, options)

        if new_layer is None:
            print("  ❌ 导入失败")
            continue

        print("  ✅ 导入成功")

        # 创建空间索引
        create_spatial_index(pg_ds, PG_SCHEMA, table_name)

    pg_ds = None
    datasource = None


def main():
    print("开始扫描 GDB 文件夹...")

    for root, dirs, files in os.walk(GDB_FOLDER):
        for d in dirs:
            if d.lower().endswith(".gdb"):
                gdb_path = os.path.join(root, d)
                import_gdb(gdb_path)

    print("\n🎉 全部完成")


if __name__ == "__main__":
    main()