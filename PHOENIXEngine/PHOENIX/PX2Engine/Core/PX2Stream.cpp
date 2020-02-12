// PX2Stream.cpp

#include "PX2Stream.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Stream::Stream () :
mStreamType(ST_MAX_TYPE)
{
}
//----------------------------------------------------------------------------
Stream::~Stream ()
{
}
//----------------------------------------------------------------------------
int Stream::GetStreamingSize (const std::string& input)
{
	// string��д���Ӳ�̣�����д���ַ������ȣ����д���ַ���������Ҫд��
	// "\0"�ս��.д����ַ������浽4�ֽڵı�����С.
    int length = (int)strlen(input.c_str());
    int padding = 0;
    if (length > 0)
    {
        padding = (length % 4);
        if (padding > 0)
        {
            padding = 4 - padding;
        }
    }
    return sizeof(int) + length*sizeof(char) + padding;
}
//----------------------------------------------------------------------------