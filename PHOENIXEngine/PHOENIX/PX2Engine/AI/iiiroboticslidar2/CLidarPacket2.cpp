// CLidarPacket2.cpp

#include "CLidarPacket2.h"

#include "C3iroboticsLidarProtocol2.h"
#include <string.h>
#include <stdio.h>
#include <iostream>
using namespace std;
using namespace everest;
using namespace everest::hwdrivers;

CLidarPacket2::CLidarPacket2()
{
	reset();
}

void CLidarPacket2::reset()
{
	m_buf.resize(m_params.buf_size);
	m_read_length = 0;
	m_length = 0;
	m_valid = false;
}

bool CLidarPacket2::hasWriteCapacity(int bytes)
{
	if (bytes < 0)
	{
		printf("[CLidarPacket2] You should input bytes %d less than 0!\n", bytes);
		return false;
	}

	// Make sure there's enough room in the packet
	if ((m_length + bytes) <= m_params.buf_size)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void CLidarPacket2::pushBack(u8 ch)
{
	if (!hasWriteCapacity(1))
	{
		return;
	}
	m_buf[m_length] = ch;
	m_length++;
}

u16 CLidarPacket2::calcCheckSum(u8 *start_bytes, u16 num_bytes)
{
	uint8_t  uchCRCHi = 0xFF; // CRC���ֽڵĳ�ʼ��
	uint8_t  uchCRCLo = 0xFF; // CRC���ֽڵĳ�ʼ��
	uint16_t uInde = 0; // CRC���ұ��ָ��

	while (num_bytes--)
	{
		uInde = uchCRCLo ^ *start_bytes++;     // ����CRC
		uchCRCLo = uchCRCHi ^ auchCRCHi[uInde];
		uchCRCHi = auchCRCLo[uInde];
	}

	return (uchCRCHi << 8 | uchCRCLo);
}


bool CLidarPacket2::verifyCheckSum(u8 ProtoType)
{
	if (m_length < m_params.least_packet_len)
	{
		m_valid = false;
		return false;
	}

	u8 c2 = m_buf[m_length - 2];
	u8 c1 = m_buf[m_length - 1];
	u16 chksum = (c2 << 8) | c1;
	u16 caculate;

	if(ProtoType < 1)
		caculate = calcCheckSum(&m_buf[0], m_length - 2);
	else
		caculate = calcCheckSum_Xor(&m_buf[0], m_length - 2);

#if 0
	printf("[CLidarPacket2] m_length %d CRC 0x%X 0x%X, receive CRC %d, caculate CRC %d!\n",
		m_length, c2, c1, chksum, caculate);
#endif
	if (chksum == caculate)
	{
		m_valid = true;
		return true;
	}
	else
	{
		m_valid = false;
		return false;
	}
}

u16 CLidarPacket2::calcCheckSum_Xor(u8 *start_bytes, u16 num_bytes)
{
	u16 checksum = 0;                             // Checksum
	while (num_bytes--)
	{
		checksum += *start_bytes++;     // ����Checksum
	}
	return checksum;
}


u16 CLidarPacket2::getParamLength()
{
	if (isValid())
	{
		return bufToUByte2(&m_buf[6]);
	}
	else
	{
		return 0;
	}
}

u16 CLidarPacket2::bufToUByte2(u8 *src_ptr)
{
	u16 data = (src_ptr[0] << 8) | ((u8)src_ptr[1]);
	return data;
}

s16 CLidarPacket2::bufToByte2(u8 *src_ptr)
{
	s16 data = (src_ptr[0] << 8) | ((s8)src_ptr[1]);
	return data;
}

u8 CLidarPacket2::bufToUByte(u8 *src_ptr)
{
	u8 data = src_ptr[0];
	return data;
}

void CLidarPacket2::bufferToData(void *dest_ptr, void *src_ptr, size_t length)
{
	printf("[CLidarPacket2] It has not realize now!\n");
}

void CLidarPacket2::printHex()
{
	printf("[CRobotPacket] length %d, read_length %d!\n",
		m_length, m_read_length);

	for (size_t i = 0; i < m_length; i++)
	{
		printf("buf[%d] = 0x%x!\n", (int)i, (m_buf[i]));
	}
}