// PX2Endian.hpp

#ifndef PX2ENDIAN_H
#define PX2ENDIAN_H

#include "PX2CorePre.hpp"

namespace PX2
{

	/// �ֽ�����
	/**
	* �ֽ�˳����ָռ�ڴ����һ���ֽ����͵��������ڴ��еĴ��˳��ͨ����С�ˡ�
	* ��������ֽ�˳��С���ֽ���ָ���ֽ����ݴ�����ڴ�͵�ַ�������ֽ����ݴ�
	* �����ڴ�ߵ�ַ��������ֽ����Ǹ��ֽ����ݴ���ڵ͵�ַ�������ֽ����ݴ����
	* �ߵ�ַ����
	* ���� int a = 0x05060708 ����
	* ��BIG-ENDIAN������´��Ϊ�� 
	*	�ֽں� 0  1  2  3 ����
	*	����   05 06 07 08 ��
	* ��LITTLE-ENDIAN������´��Ϊ�� 
	*	�ֽں� 0  1  2  3 
	*	����   08 07 06 05 
	*/
	class PX2_ENGINE_ITEM Endian
	{
	public:
		/// �ֽ������Ƿ��Ǵ��ֽ���
		static bool IsBig ();

		/// �ֽ������Ƿ���С�ֽ���
		static bool IsLittle ();

		// �����ֽ�˳��
		static void Swap2 (void* datum);
		static void Swap2 (int numItems, void* data);
		static void Swap4 (void* datum);
		static void Swap4 (int numItems, void* data);
		static void Swap8 (void* datum);
		static void Swap8 (int numItems, void* data);
		static void Swap (size_t itemSize, void* datum);
		static void Swap (size_t itemSize, int numItems, void* data);

	private:
		static bool msIsLittle;
	};

}

#endif
