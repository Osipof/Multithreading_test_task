# Multithreading_test_task
 
Implementation of basic function to split some generic computational work between threads. Split should occur only on some threshold - if computational work (input length) is shorter than this threshold, no splitting should occur and no threads should be created.

Input:
1. `std::vector<T>`.
2. Function that converts `T` into `R`, i.e. `R func(T t)`.

Threshold is just a constant.

Return:
1. `std::vector<R>` of the same length as input(1). Data returned should contain results of applying function (2) to every element of the source vector (1).