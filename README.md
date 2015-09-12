# K-Skyband-Query
incomplete k-skyband, constrained skyline, and group-by skyline queries on incomplete data in postgresql

## Algorithm Discription
### kISB algorithm:
* Please see Mr. Gao's paper: <i><b>Processing k-skyband, constrained skyline, and group-by skyline queries on incomplete data</b></i>
  
### Initialization:


### Query


## Testing Environment

  * PostgreSQL version: PostgreSQL 9.4

## How to use?
### 0. Install postgresql-server-dev fist
~~~terminal
	sudo apt-get install postgreslq-server-dev-all
~~~

### 1. Clone and enter my repo (in terminal)
~~~terminal
    git clone git@github.com:Armour/KeySkyBandQuery.git
    cd KSkyBandQuery
~~~

### 2. Make and install iknnLP function (in terminal)
~~~terminal
	cd c
	make
	sudo make install
	cd ..
~~~

### 3. Import iknnLP function (in postgresql)
~~~sql
	\i c/iknnLP.sql
~~~

### 4. Performing iknn query with LP althrothm
~~~sql
	select a, b, c, d, distance from iknnLP('find 3 nearest neighbour of (a0,a1,a2,a3)(31,32,33,34) from test') AS (a int, b int, c int, d int, distance float);
~~~
* a0,a1,a2,a3 are columns in the table _test_.
* 31,32,33,34 are values of the columns respectively.
* the query object must have values for all columns of the query table
* The tuples returned are those considered nearest with the query object.

### 5. Here's the result
~~~sql
 a | b  | c | d  | distance 
---+----+---+----+----------
   |    |   | 46 |      576
   |    |   | 23 |      484
   | 17 |   | 35 |      452
(3 rows)
~~~

### 6. Inport LPwithdraw.sql

~~~
    \i LPwithdraw.sql
~~~

### 7. Withdraw iknn query support

~~~
    select lpwithdraw([table name]);
~~~

This function automatically does these things:
  1. drop the extra three columns of the original table;
  2. drop all tmp tables;
  3. drop the three triggers that maintains the three extra columns, buckets, and the tmp table;
  4. drop the lpinit function

## Contact us
1. You can get the paper from Mr. Gao: gaoyj@zju.edu.cn
2. The projet is coded by Armour Guo, feel free to ask any questions: gc497052684@gmail.com