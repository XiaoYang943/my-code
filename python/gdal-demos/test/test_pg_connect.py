import psycopg2

def main():
    try:
        conn = psycopg2.connect(
            host="127.0.0.1",
            port="5432",
            dbname="scene_map_server",
            user="postgres",
            password="a1352253543A@",
        )
        print("连接成功")
        conn.close()

    except Exception as e:
        print("连接失败 repr:")
        print(repr(e))


if __name__ == "__main__":
    main()