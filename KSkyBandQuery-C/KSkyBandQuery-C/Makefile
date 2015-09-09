cc=gcc
exe=main
obj=main.o BucketList.o PointList.o HashTable.o
$(exe):$(obj)
    $(cc) -g -Wall -o $(exe) $(obj)
main.o:main.c BucketList.h PointList.h
    $(cc) -c main.c
BucketList.o:BucketList.c BucketList.h
    $(cc) -c BucketList.c
PointList.o:PointList.c PointList.h
    $(cc) -c PointList.c
HashTable.o:HashTable.c HashTable.h
    $(cc) -c HashTable.c
clean:
    rm -rf *.o

