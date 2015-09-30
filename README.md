# K-Skyband-Query
incomplete k-skyband, constrained skyline, and group-by skyline queries on incomplete data in c and c++

## Algorithm Discription
### kISB algorithm:
* Please see Mr. Gao's paper: [Processing k-skyband, constrained skyline, and group-by skyline queries on incomplete data] (http://www.armourcy.com/skyband.pdf)

## Testing Environment
  * Operation System: Mac OSX / Ubuntu 14.04

## How to use?

1. cd into c code directory
2. run 'make'
3. execute './skyband' with three arguments n, m and k. (n: data amonut, m: data dimension, k: k-skyband)

(Remember to change the input and output path in sky_query.c!! I used a terrible absolute path lol).

## Test

1. cd into test directory
2. run './test.sh' with three arguments n, m and k (it will automaticly run both brute force and k-skyband algorithem, after both finished, it will check whether two results are the same and return the time that two algorithem taked.)

## Others

You can change the test data generator to fit your need, for example if you want to change the percentage of NULL data. 

## Contact us
1. If you have any question about this paper, you can contact Mr. Gao: gaoyj@zju.edu.cn
2. The projet is coded by Armour Guo, feel free to ask any questions: gc497052684@gmail.com
