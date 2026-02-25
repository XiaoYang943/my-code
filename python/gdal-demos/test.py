from osgeo import ogr


def main():
    drivers = [ogr.GetDriver(i).GetName() for i in range(ogr.GetDriverCount())]
    print("PostgreSQL" in drivers)
    print(drivers)

if __name__ == "__main__":
    main()