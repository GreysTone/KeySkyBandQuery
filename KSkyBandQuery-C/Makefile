# ------------------------------------------------
#
#   Makefile
#   KSkyBandQuery
#
#   Created by Armour on 11/09/2015
#   Copyright (c) 2015 Armour. All rights reserved.
#
# ------------------------------------------------

CC = gcc
CFALGS = -g -Wall
TARGET = sky_query
OBJECTS = sky_query.o sky_bucket_list.o sky_point_list.o sky_hashtable.o sky_domi_list.o

.PHONY: all clean

all: $(OBJECTS)
	$(CC) $(CFALGS) -o $(TARGET) $^

$%.o : %.c %.h
	$(CC) $(CFALGS) -c $< -o $@

clean:
	rm -rf *.o $(TARGET)

