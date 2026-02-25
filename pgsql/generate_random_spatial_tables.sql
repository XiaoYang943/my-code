CREATE EXTENSION IF NOT EXISTS postgis;

DROP FUNCTION IF EXISTS generate_random_spatial_tables(text, integer, integer, integer);

-- 创建空间表(随机)
CREATE OR REPLACE FUNCTION generate_random_spatial_tables(
    p_schema TEXT,
    p_table_count INT,
    p_srid INT,
    p_row_count INT
)
RETURNS VOID AS
$$
DECLARE
    i INT;
    v_table_name TEXT;
    v_geom_type TEXT;
    v_sql TEXT;
BEGIN
    -- 创建 schema
    EXECUTE format('CREATE SCHEMA IF NOT EXISTS %I', p_schema);

    FOR i IN 1..p_table_count LOOP

        -- 随机英文表名
        v_table_name := 'tbl_' || substr(md5(random()::text), 1, 8);

        -- 随机几何类型
        v_geom_type := (
            ARRAY['POINT', 'LINESTRING', 'POLYGON']
        )[floor(random()*3 + 1)];

        -------------------------------------------------------------
        -- 创建表
        -------------------------------------------------------------
        EXECUTE format(
            'CREATE TABLE %I.%I (
                id SERIAL PRIMARY KEY,
                name TEXT,
                value_int INTEGER,
                value_double DOUBLE PRECISION,
                geom geometry(%s, %s)
            )',
            p_schema,
            v_table_name,
            v_geom_type,
            p_srid
        );

        -------------------------------------------------------------
        -- 插入数据
        -------------------------------------------------------------
        IF v_geom_type = 'POINT' THEN

            v_sql := format(
                'INSERT INTO %I.%I (name, value_int, value_double, geom)
                 SELECT
                     substr(md5(random()::text),1,6),
                     (random()*1000)::int,
                     random()*1000,
                     ST_SetSRID(
                        ST_MakePoint(
                            random()*360 - 180,
                            random()*180 - 90
                        ),
                        %s
                     )
                 FROM generate_series(1,%s)',
                p_schema,
                v_table_name,
                p_srid,
                p_row_count
            );

        ELSIF v_geom_type = 'LINESTRING' THEN

            v_sql := format(
                'INSERT INTO %I.%I (name, value_int, value_double, geom)
                 SELECT
                     substr(md5(random()::text),1,6),
                     (random()*1000)::int,
                     random()*1000,
                     ST_SetSRID(
                        ST_MakeLine(
                            ST_MakePoint(random()*360 - 180, random()*180 - 90),
                            ST_MakePoint(random()*360 - 180, random()*180 - 90)
                        ),
                        %s
                     )
                 FROM generate_series(1,%s)',
                p_schema,
                v_table_name,
                p_srid,
                p_row_count
            );

        ELSE  -- POLYGON (使用 ST_MakeEnvelope)

            v_sql := format(
                'INSERT INTO %I.%I (name, value_int, value_double, geom)
                 SELECT
                     substr(md5(random()::text),1,6),
                     (random()*1000)::int,
                     random()*1000,
                     ST_SetSRID(
                        ST_MakeEnvelope(
                            x,
                            y,
                            x + random()*5,
                            y + random()*5
                        ),
                        %s
                     )
                 FROM (
                     SELECT
                        random()*360 - 180 AS x,
                        random()*180 - 90 AS y
                     FROM generate_series(1,%s)
                 ) t',
                p_schema,
                v_table_name,
                p_srid,
                p_row_count
            );

        END IF;

        EXECUTE v_sql;

        -------------------------------------------------------------
        -- 创建 GIST 索引
        -------------------------------------------------------------
        EXECUTE format(
            'CREATE INDEX %I_geom_idx ON %I.%I USING GIST (geom)',
            v_table_name,
            p_schema,
            v_table_name
        );

    END LOOP;

END;
$$ LANGUAGE plpgsql;

-- 批量创建表
SELECT generate_random_spatial_tables(
    'test',
    1000,
    4326,
    1000
);

-- 查询表的数量
SELECT count(*)
FROM pg_tables
WHERE schemaname = 'test';

-- 删除批量创建的表
DO $$
DECLARE
    r RECORD;
BEGIN
    FOR r IN
        SELECT tablename
        FROM pg_tables
        WHERE schemaname = 'test'
          AND tablename LIKE 'tbl_%'
    LOOP
        EXECUTE format(
            'DROP TABLE %I.%I',
            'test',
            r.tablename
        );
        COMMIT;
    END LOOP;
END $$;