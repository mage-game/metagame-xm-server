
#ifndef BINTOHEX_H
#define BINTOHEX_H

// ���ܲ��ԣ���������������CPUΪInter Core 2 ˫��2.20G��PC�ϣ�����ת����Ϊdataÿ��130M����

// ������������ת����16�����ַ���
// ע�⣬hex_buff �Ŀռ����2��length����
void BinToHex(const char *data, int length, char *hex_buff);

// ��16�����ַ���ת���ض���������
// ע�⣬data���� 1/2��length����
void HexToBin(const char *hex_buff, int length, char *data);

#endif
