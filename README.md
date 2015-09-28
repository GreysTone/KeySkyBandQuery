# K-Skyband-Query
incomplete k-skyband, constrained skyline, and group-by skyline queries on incomplete data in postgresql

## Algorithm Discription
### kISB algorithm:
* Please see Mr. Gao's paper: [Processing k-skyband, constrained skyline, and group-by skyline queries on incomplete data] (http://www.armourcy.com/skyband.pdf)

## Testing Environment
  * Operation System: Mac OSX / Ubuntu 14.04
  * PostgreSQL version: PostgreSQL 9.4.4

## How to use?
### 0. Install postgresql-server-dev fist

for ubuntu:

~~~terminal
	sudo apt-get install postgreslq-server-dev-all
~~~

for mac:

~~~terminal
	brew install postgresql
~~~

### 1. Clone and enter my repo (in terminal)
~~~terminal
    git clone git@github.com:Armour/KeySkyBandQuery.git
    cd KeySkyBandQuery
~~~

### 2. Make and install skyband function (in terminal)
~~~terminal
	cd skyband
	make
	sudo make install
~~~

you will see something like this:
.....

### 3. Import skyband function (in postgresql)

use postgresql command to enter below command

~~~sql
	\i ..../skyband.sql
~~~

### 4. Performing skyband query with warehouse algorithem
~~~sql
	select * from skyband('select price, distance, noisy from hotel', 2) as (price real, distance real, noisy real);
~~~

### 5. Here's the result
~~~sql
 a | b  | c | d  | distance 
---+----+---+----+----------
   |    |   | 46 |      576
   |    |   | 23 |      484
   | 17 |   | 35 |      452
(3 rows)
~~~

## Contact us
1. If you have any question about this paper, you can contact Mr. Gao: gaoyj@zju.edu.cn
2. The projet is coded by Armour Guo, feel free to ask any questions: gc497052684@gmail.com
