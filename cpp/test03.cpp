#include <iostream>

using namespace std;

void HelloWorld(int n) {
  for (int i = 0; i < n; ++i) {
    // std::cout << "[" << i << "] " << "Hello World!" << std::endl;
    cout << "[" << i << "] " << "Hello World!" << endl;
  }
}

int add(int x, int y) {
  int s = x + y;
  return s;
}

int sum(int* arr, int arr_size) {
  int sum = 0;
  for (int i = 0; i < arr_size; i++) {
      sum += arr[i];
  }
  return sum;
}

void Hoge(char x) {
  cout << x << endl;
}

int main() {
  HelloWorld(10);

  int a = 10;
  int b = 20;
  int s = add(a, b);
  printf("%d", s);

  int arr[] = {1, 2, 3, 4, 5};
  int x;
  x = sum(arr, sizeof(arr) / sizeof(arr[0]));
  printf("%d\n", x);

  // Hoge("A");
  return 0;
}