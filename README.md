# K-Skyband-Query
incomplete k-skyband, constrained skyline, and group-by skyline queries on incomplete data in c and c++

## Algorithm Discription
### kISB algorithm:
* Please see Mr. Gao's paper: [Processing k-skyband, constrained skyline, and group-by skyline queries on incomplete data] (http://www.armourcy.com/skyband.pdf)

## Testing Environment
  * Operation System: Mac OSX / Ubuntu 14.04

## How to use?
cd into c code directory
just run 'make' and then you can execute './skyband' with three arguments m, n and k.

## Test
cd into test directory
just run './test.sh' with three arguments m, n and k, it will automaticly run both brute force and k-skyband algorithem, after both finished, it will check whether two results are the same and return the time that two algorithem taked.

## Contact us
1. If you have any question about this paper, you can contact Mr. Gao: gaoyj@zju.edu.cn
2. The projet is coded by Armour Guo, feel free to ask any questions: gc497052684@gmail.com
