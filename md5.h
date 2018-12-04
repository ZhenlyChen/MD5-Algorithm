#ifndef MD5_H
#define MD5_H
#include <string>
typedef unsigned char byte;
typedef unsigned int bit32;


std::string getMD5(const byte* data, size_t len);

#endif
