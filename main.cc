#include <algorithm>
#include <functional>
#include <iostream>
#include <thread>
#include <vector>

const int kThreshold = 10;

auto func(double x) { return x * 3; }

template <typename T, typename R>
std::vector<R> SplitWork(const std::vector<T> &input_vector,
                         std::function<R(T)> convert_func) {
  std::vector<R> result(input_vector.size());

  // In this case, parallelism is not used
  if (input_vector.size() < kThreshold) {
    for (size_t i = 0; i < input_vector.size(); i++) {
      result[i] = convert_func(input_vector[i]);
    }
    return result;
  }

  int number_of_threads = std::thread::hardware_concurrency();
  int chunk_size = input_vector.size() / number_of_threads;
  std::vector<std::thread> threads;

  for (int i = 0; i < number_of_threads; i++) {
    int start = i * chunk_size;
    if (i == number_of_threads - 1) {
      chunk_size = input_vector.size() - start;
    }
    threads.emplace_back([=, &result]() {
      std::transform(input_vector.begin() + start,
                     input_vector.begin() + start + chunk_size,
                     result.begin() + start, func);
    });
  }

  for (auto &thread : threads) {
    thread.join();
  }

  return result;
}

int main() {
  auto start = std::chrono::high_resolution_clock::now();
  std::vector<double> vector;

  for (double i = 0; i < 1000; i++) {
    vector.push_back(i);
  }

  /*
  std::cout << "vector: { ";
  for (size_t i = 0; i < vector.size(); i++) {
    std::cout << vector[i] << " ";
  }
  std::cout << " }" << std::endl;
  */

  std::vector<double> string_vector = SplitWork<double, double>(vector, func);

  /*
  std::cout << "vector_after_function: { ";
  for (size_t i = 0; i < string_vector.size(); i++) {
    std::cout << string_vector[i] << " ";
  }
  std::cout << " }" << std::endl;
  */

  // Output of the program execution time
  auto end = std::chrono::high_resolution_clock::now();
  std::chrono::duration<float> duration = end - start;
  std::cout << "DURATION: " << duration.count() << " s" << std::endl;
  return 0;
}
