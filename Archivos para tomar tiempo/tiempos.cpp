#include <iostream>
#include <chrono>

using namespace std;

int main() {
  // Get the start time.
  auto start = chrono::high_resolution_clock::now();

  // Execute the other C++ program.
  system("./cache.out");

  // Get the end time.
  auto end = chrono::high_resolution_clock::now();

  // Calculate the execution time.
  auto duration = chrono::duration_cast<chrono::duration<double>>(end - start);

  // Print the execution time.
  cout << "The execution time was " << duration.count() << " seconds." << endl;

  return 0;
}