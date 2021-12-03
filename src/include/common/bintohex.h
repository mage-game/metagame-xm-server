
#ifndef BINTOHEX_H
#define BINTOHEX_H

// 性能测试：以下两个函数在CPU为Inter Core 2 双核2.20G的PC上，单向转换均为data每秒130M以上

// 将二进制数据转换成16进制字符串
// 注意，hex_buff 的空间必须2倍length以上
void BinToHex(const char *data, int length, char *hex_buff);

// 将16进制字符串转换回二进制数据
// 注意，data必须 1/2的length以上
void HexToBin(const char *hex_buff, int length, char *data);

#endif
