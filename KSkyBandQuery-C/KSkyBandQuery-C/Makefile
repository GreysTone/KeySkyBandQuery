cc=gcc
exe=sky_query
obj=sky_query.o sky_bucket_list.o sky_point_list.o sky_hashtable.o
$(exe):$(obj)
	$(cc) -g -Wall -o $(exe) $(obj)
sky_query.o:sky_query.c sky_bucket_list.h sky_point_list.h
	$(cc) -c sky_query.c
sky_bucket_list.o:sky_bucket_list.c sky_bucket_list.h
	$(cc) -c sky_bucket_list.c
sky_point_list.o:sky_point_list.c sky_point_list.h
	$(cc) -c sky_point_list.c
sky_hashtable.o:sky_hashtable.c sky_hashtable.h
	$(cc) -c sky_hashtable.c
clean:
	rm -rf *.o

