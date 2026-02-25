# -*- coding: utf-8 -*-
import os
import sys
import traceback

# import os
# os.environ["GDAL_DRIVER_PATH"] = r"D:\soft\runtime\miniconda3\envs\my_gis_env\Library\lib\gdalplugins"

from osgeo import ogr, gdal

# 强制抛异常（非常关键）
gdal.UseExceptions()

# ========================
# 🔧 修改你的配置
# ========================

PG_HOST = "localhost"
PG_PORT = "5432"
PG_DB = "scene_map_server"
PG_USER = "postgres"
PG_PASSWORD = "a1352253543A@"

# ========================


def print_environment_info():
    print("\n========== 环境信息 ==========")
    print("Python 路径:", sys.executable)
    print("GDAL 版本:", gdal.VersionInfo())
    print("GDAL 数据路径:", gdal.GetConfigOption("GDAL_DATA"))
    print("PROJ 路径:", gdal.GetConfigOption("PROJ_LIB"))
    print("GDAL_DRIVER_PATH:", gdal.GetConfigOption("GDAL_DRIVER_PATH"))

    drivers = [ogr.GetDriver(i).GetName() for i in range(ogr.GetDriverCount())]
    print("\n驱动总数:", len(drivers))
    print("是否包含 PostgreSQL 驱动:", "PostgreSQL" in drivers)

    print("Post 相关驱动:",
          [d for d in drivers if "Post" in d])

    print("================================\n")


def build_pg_conn_str():
    # 用单引号包裹，避免 @ 解析问题
    return (
        "PG:"
        f"host='{PG_HOST}' "
        f"port='{PG_PORT}' "
        f"dbname='{PG_DB}' "
        f"user='{PG_USER}' "
        f"password='{PG_PASSWORD}'"
    )


def test_postgis_connection():
    print("========== 测试 PostGIS 连接 ==========")

    conn_str = build_pg_conn_str()
    safe_conn_str = conn_str.replace(PG_PASSWORD, "******")
    print("连接字符串:", safe_conn_str)

    try:
        ds = ogr.Open(conn_str, update=1)

        if ds is None:
            print("❌ ogr.Open 返回 None（无异常）")
            return False

        print("✅ 连接成功！")
        ds = None
        return True

    except Exception as e:
        print("\n❌ 捕获到 GDAL 异常:")
        print(str(e))
        print("\n详细堆栈:")
        traceback.print_exc()
        return False


def test_raw_libpq():
    print("\n========== 测试 libpq 是否可用 ==========")
    try:
        import psycopg2
        conn = psycopg2.connect(
            host=PG_HOST,
            port=PG_PORT,
            dbname=PG_DB,
            user=PG_USER,
            password=PG_PASSWORD,
        )
        conn.close()
        print("✅ psycopg2 连接成功")
    except Exception as e:
        print("❌ psycopg2 连接失败:")
        print(e)


def main():
    print_environment_info()

    if not test_postgis_connection():
        print("\n⚠ GDAL 连接失败，尝试 psycopg2 直接连接测试...\n")
        test_raw_libpq()

    print("\n========== 测试结束 ==========")


if __name__ == "__main__":
    main()