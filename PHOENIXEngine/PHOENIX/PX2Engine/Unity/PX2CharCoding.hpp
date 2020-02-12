// PX2CharCoding.hpp

#ifndef PX2CHARCODING_HPP
#define PX2CHARCODING_HPP

#include "PX2UnityPre.hpp"
#include "PX2FontDefine.hpp"

namespace PX2
{

	class CharCoding
	{
	public:
		virtual ~CharCoding () {}

		/// �����ַ���ͷ���Ŀհף����ؿհ׵��ֽڳ���
		virtual int JumpOverSpaces (const char *text) = 0;

		/// ���ַ���ͷ��ȡ��һ���ַ��������ַ����ֽڳ���
		virtual int GetAChar (const char *text, unsigned char *character) = 0;

		/// ȡ��ָ���ַ����ֽڳ���
		virtual int GetCharBytes (const unsigned char *character) = 0;

		/// ���ַ���ͷ��ȡ�ÿ����룬����ǿ����룬���ؿ�������ֽڳ��ȣ�
		/// ���򷵻�0
		virtual int GetControlCode (const char *text, CharCtrlCode &ctrlCode, 
			bool doTransfer) = 0;

		/// �Ƿ���Ӣ�ģ��������
		virtual bool IsEnglish (const unsigned char *character) = 0;

		/// �Ƿ���Ӣ�ı�����
		virtual bool IsPunctuation (const unsigned char *character) = 0;
		virtual bool IsPoint(const unsigned char *character) = 0;

		/// ���ַ�ת��ΪUnicode
		virtual unsigned short ToUnicode (const unsigned char *character) = 0;

		/// ���ַ�ת��Ϊ�ڱ��ַ����뷽����Χ�ڵ�ΨһID������hash key
		virtual unsigned int ToUniqueID (const unsigned char* character) = 0;
	};

}

#endif