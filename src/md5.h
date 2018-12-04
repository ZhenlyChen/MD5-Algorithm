#ifndef MD5_H
#define MD5_H
#include <iostream>
#include <sstream>
#include <string>
#include <vector>
typedef unsigned char byte;
typedef unsigned int bit32;

std::string getMD5(const byte* data, size_t len);

#endif
