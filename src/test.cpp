#include <iostream>
#include "md5.h"
using namespace std;

int main(int argc, char const* argv[]) {
  string in;
  cin >> in;
  const unsigned char* data = (const unsigned char*)in.c_str();
  cout << getMD5(data, in.length());
  return 0;
}
