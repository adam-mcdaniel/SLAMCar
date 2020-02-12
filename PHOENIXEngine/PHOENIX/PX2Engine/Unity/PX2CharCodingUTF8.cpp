// PX2CharCodingUTF8.cpp

#include "PX2CharCodingUTF8.hpp"
#include "PX2StringHelp.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
CharCodingUTF8::CharCodingUTF8 ()
{
}
//----------------------------------------------------------------------------
CharCodingUTF8::~CharCodingUTF8 ()
{
}
//----------------------------------------------------------------------------
int CharCodingUTF8::JumpOverSpaces (const char *text)
{
	int ret = 0;
	const unsigned char *p = (const unsigned char*)text;

	bool cont = true;
	while (cont)
	{
		if (' ' == *p) // ��ǿո�
		{
			p++;
			ret++;
		}
		else
		{
			break;
		}
	}

	return ret;
}
//----------------------------------------------------------------------------
int CharCodingUTF8::GetAChar (const char *text, unsigned char *character)
{
	if (!text)
		return 0;

	unsigned char head = (unsigned char)(*text);

	if ((head & 0x80) == 0)
	{
		// Ӣ��
		character[0] = head;
		character[1] = 0;
		return 1;
	}
	else
	{
		unsigned char mask = 0x80;
		int numBytes = 0;

		for( int i=0; i<8; i++)
		{
			if( (head & mask) == 0 )
				break;
			numBytes++;
			mask >>= 1;
		}

		if (numBytes > 3 || numBytes < 2)
		{
			// �Ƿ���Utf8�ַ���ת����Ĭ���ַ�'?'����������ֽڳԵ�
			character[0] = 63;
			character[1] = 0;
			return 1;
		}
		else
		{
			for( int i=0; i<numBytes; i++ )
			{
				// ��ֹ��������ı����ִ�ʱ���ִ���β��0�ֽڳԵ�
				if (text[i] == 0)
				{
					character[i] = 0;
					return i;
				}
				character[i] = text[i];
			}
			character[numBytes] = 0;
			return numBytes;
		}
	}
}
//----------------------------------------------------------------------------
int CharCodingUTF8::GetCharBytes (const unsigned char *character)
{
	if (!character)
		return 0;

	unsigned char head = (unsigned char)(*character);

	if ((head & 0x80) == 0)
	{
		return 1;
	}
	else
	{
		unsigned char nMask = 0x80;
		int numBytes = 0;

		for (int i=0; i<8; i++)
		{
			if ((head & nMask) == 0)
				break;
			numBytes++;
			nMask >>= 1;
		}

		if (numBytes > 4 || numBytes < 2)
			return 1;
		else
			return numBytes;
	}
}
//----------------------------------------------------------------------------
int CharCodingUTF8::GetControlCode (const char *text, CharCtrlCode &ctrlCode, 
	bool doTransfer)
{
	if (!text)
		return 0;

	const unsigned char *p = (unsigned char *)text;
	unsigned char c = *p;

	if (c == 0)
	{
		ctrlCode = CCC_ENDOFSTRING;
		return 1;
	}

	unsigned char c1 = *(p + 1);
	if (c == '\n')
	{
		ctrlCode = CCC_NEWLINE;
		return 1;
	}
	else if (c == '\\' && c1 == 'n')
	{
		ctrlCode = CCC_NEWLINE;
		return 2;
	}
	else if (c == '\r')
	{
		ctrlCode = CCC_RETURN;
		return 1;
	}
	else if (doTransfer && c == '#' && c1 != '#')
	{
		ctrlCode = CCC_TRANSFER;
		return 1;
	}
	else
	{
		ctrlCode = CCC_CHARACTER;
		return 0;
	}
}
//----------------------------------------------------------------------------
bool CharCodingUTF8::IsEnglish (const unsigned char *character)
{
	if (character[0] >= 33 && character[0] <= 127)
		return true;
	
	return false;
}
//----------------------------------------------------------------------------
bool CharCodingUTF8::IsPunctuation(const unsigned char *character)
{
	if (character[0] == ',' || character[0] == '.' || character[0] == '!'|| 
		character[0] == '?' || character[0] == ':' || character[0] == ';')
		return true;

	return false;
}
//----------------------------------------------------------------------------
bool CharCodingUTF8::IsPoint(const unsigned char *character)
{
	return character[0] == '.';
}
//----------------------------------------------------------------------------
unsigned short CharCodingUTF8::ToUnicode(const unsigned char *character)
{
	return *(StringHelp::UTF8ToUnicode((const char*)character, 
		GetCharBytes(character)));
}
//----------------------------------------------------------------------------
unsigned int CharCodingUTF8::ToUniqueID(const unsigned char* character)
{
	unsigned int id = 0;

	for (int i=0; i<(int)strlen((const char*)character); i++)
	{
		id *= 256;
		id += character[i];
	}

	return id;
}
//----------------------------------------------------------------------------