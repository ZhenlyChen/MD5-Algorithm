#include "md5.h"

using std::cout;
using std::endl;
using std::string;
using std::vector;

namespace MD5 {
bit32 MD[4] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476};
bit32 IV[4] = {0x67452301, 0xEFCDAB89, 0x98BADCFE, 0x10325476};
bit32 T[64] = {
    0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee, 0xf57c0faf, 0x4787c62a,
    0xa8304613, 0xfd469501, 0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
    0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821, 0xf61e2562, 0xc040b340,
    0x265e5a51, 0xe9b6c7aa, 0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
    0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed, 0xa9e3e905, 0xfcefa3f8,
    0x676f02d9, 0x8d2a4c8a, 0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
    0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70, 0x289b7ec6, 0xeaa127fa,
    0xd4ef3085, 0x04881d05, 0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
    0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039, 0x655b59c3, 0x8f0ccc92,
    0xffeff47d, 0x85845dd1, 0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
    0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391};
byte s[64] = {7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22, 7, 12, 17, 22,
             5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20, 5, 9,  14, 20,
             4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23, 4, 11, 16, 23,
             6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21, 6, 10, 15, 21};
byte X[4][16] = {{0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
                {1, 6, 11, 0, 5, 10, 15, 4, 9, 14, 3, 8, 13, 2, 7, 12},
                {5, 8, 11, 14, 1, 4, 7, 10, 13, 0, 3, 6, 9, 12, 15, 2},
                {0, 7, 14, 5, 12, 3, 10, 1, 8, 15, 6, 13, 4, 11, 2, 9}};
vector<bit32> parts;
};  // namespace MD5

class BitSet32 {
 public:
  BitSet32() {
    current = 0;
    pos = 0;
  }
  void AddByte(byte b) {
    current |= (b << (pos * 8));
    pos++;
    if (pos == 4) {
      parts.push_back(current);
      current = 0;
      pos = 0;
    }
  }
  vector<bit32> GetBitSet() { return parts; }

 private:
  vector<bit32> parts;
  bit32 current;
  int pos;
};

// 小端编码
void encode(const bit32* input, byte* output, size_t length) {
  for (size_t i = 0, j = 0; j < length; ++i, j += 4) {
    output[j] = (byte)(input[i] & 0xff);
    output[j + 1] = (byte)((input[i] >> 8) & 0xff);
    output[j + 2] = (byte)((input[i] >> 16) & 0xff);
    output[j + 3] = (byte)((input[i] >> 24) & 0xff);
  }
}

// 填充
void padding(const byte* data, size_t len) {
  int paddingCount = len % 64;
  paddingCount = paddingCount > 55 ? 119 - paddingCount
                                   : 55 - paddingCount;  // 填充0x0的个数
  BitSet32 bitset = BitSet32();
  // 读取数据
  for (int i = 0; i < len; i++) {
    bitset.AddByte(data[i]);
  }
  // 填充首位1
  bitset.AddByte(0x80);
  // 填充0
  for (int i = 0; i < paddingCount; i++) {
    bitset.AddByte(0);
  }
  vector<bit32> parts = bitset.GetBitSet();
  // 填充(长度 mod 2^64)
  parts.push_back((len * 8) & 0xffff);
  parts.push_back(((size_t)(len * 8) >> 32) & 0xffff);
  MD5::parts = parts;
}

// 压缩
void H_MD5() {
  int count = MD5::parts.size() / 16;  // 循环次数
  for (int q = 0; q < count; q++) {
    bit32 ta = MD5::MD[0];
    bit32 tb = MD5::MD[1];
    bit32 tc = MD5::MD[2];
    bit32 td = MD5::MD[3];
    for (int j = 0; j < 4; j++) {
      for (int i = 0; i < 16; i++) {
        int rounds = j * 16 + i;
        bit32 a = ta;
        bit32 b = tb;
        bit32 c = tc;
        bit32 d = td;
        a += MD5::parts[q * 16 + MD5::X[j][i]] + MD5::T[rounds];
        switch (j) {
          case 0:
            a += ((b & c) | ((~b) & d));
            break;
          case 1:
            a += (((b) & (d)) | ((c) & (~d)));
            break;
          case 2:
            a += (((b) ^ (c)) ^ (d));
            break;
          case 3:
            a += (c ^ (b | (~d)));
            break;
        }
        a = (a << MD5::s[rounds] | a >> (32 - MD5::s[rounds])) + b;
        ta = d;
        tb = a;
        tc = b;
        td = c;
      }
      // cout << ta << " " << tb << " " << tc << " " << td << endl;
    }
    MD5::MD[0] += ta;
    MD5::MD[1] += tb;
    MD5::MD[2] += tc;
    MD5::MD[3] += td;
  }
}

string getMD5(const byte* data, size_t len) {
  // 填充
  padding(data, len);
  // for (int i = 0; i < MD5::parts.size(); i++){
  //   cout << "x"  << i << ":\t" << MD5::parts[i] << endl;
  // }
  // MD5压缩函数
  H_MD5();
  // 对结果进行小端编码
  byte digest[16];
  encode(MD5::MD, digest, 16);
  // 输出十六进制字符串
  std::ostringstream ss;
  ss << std::hex;
  for (int i = 0; i < 16; i++) {
    if (digest[i] < 16) ss << 0;
    ss << (int)digest[i];
  }
  ss << endl;
  return ss.str();
}
