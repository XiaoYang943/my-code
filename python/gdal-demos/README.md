## 环境
### 清理 channel
`conda config --remove-key channels`
`conda config --add channels conda-forge`
`conda config --set channel_priority strict`
### 创建环境
`conda create -n my_gis_env python=3.11 gdal proj geos postgresql psycopg2`
`conda activate my_gis_env`
### 解决gdal-pg驱动找不到问题
#### 激活环境
`conda activate my_gis_env`
#### 设置 gdalplugins 环境变量
`conda env config vars set GDAL_DRIVER_PATH=%CONDA_PREFIX%\Library\lib\gdalplugins`
#### 重新激活环境
`conda deactivate`
`conda activate my_gis_env`
#### 验证gdal插件位置
`python -c "from osgeo import gdal; print(gdal.GetConfigOption('GDAL_DRIVER_PATH'))"`
- 输出如下：`D:\soft\runtime\miniconda3\envs\my_gis_env\Library\lib\gdalplugins`
#### 验证驱动注册
`python -c "from osgeo import ogr; print([ogr.GetDriver(i).GetName() for i in range(ogr.GetDriverCount()) if 'Post' in ogr.GetDriver(i).GetName()])"`
- 正确输出：`['PostgreSQL']`