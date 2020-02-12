// PX2GeneralAlloctor.hpp

#ifndef PX2GENERALALLOCTOR_HPP
#define PX2GENERALALLOCTOR_HPP

#include "PX2CorePre.hpp"
#include "PX2Mutex.hpp"

namespace PX2
{

	// ģ��C++ SGISTL�Ķ����ڴ���䷨���̰߳�ȫ���ڴ������
	/*
	* ͨ����������ƽ�������ͷŵ�С���ڴ档
	* ��512�ֽ����µ��ڴ�ֳ�128�ݣ���4���ֽڵ��������ǣ���������һ����128����
	* �㣬ÿ���ڵ�ָ��һ�����ڷ���ýڵ��������ڴ��С�Ŀ�������
	*
	*	a) ���numBytes�ӽڶ�Ӧ�Ŀ�����������û��
	*	b) ���û�У������������棬�ܲ��ܷ���
	*	c) ���������Ҳû���ˣ�����ϵͳҪ
	*
	*/
	class PX2_ENGINE_ITEM GeneralAlloctor
	{
	public:
		static void *Allocate (size_t numBytes);
		static void Deallocate (void *buffer, size_t numBytes);
		static void *Reallocate (void *buffer, size_t oldSize, size_t newSize);

	private:
		static const int Align = 4;
		static const int MaxAllowAlloctedBytes = 512;
		static const int NumFreelists = 128;

		union AllocObj
		{
			union AllocObj *ObjLink;
			char ChientData[1];
		};
		static AllocObj* volatile msFreeList[NumFreelists]; 

		// ���numBytes=11, (11 + 4-1)/4 -1 = 2, 2�������λ��
		static  size_t FreeListIndex(size_t numBytes) ;

		// ����һ���������龰: ������Ҫһ���СΪ11�ֽڵ��ڴ棬����11��������
		// ��Ԥ���кõ�4�ı�������������һ��RoundUp,ȥ��ͼ����12�ֽڴ�С���ڴ�
		// (11 + 3) & (~11) = 12
		static size_t RoundUp (size_t numBytes);

		// ����һ����СΪnumBytes���ڴ�
		/* 
		* �ڲ����Խ������numBytes�����������������������δʹ�õģ�ֱ�Ӵ�
		* ����ѡһ����
		*/
		static void* ReFill (size_t numBytes);
		static char* ChunkAlloc (size_t numBytesPerObj, int &numObjs);

		static char* msStartFree;
		static char* msEndFree;
		static size_t msHeapSize;
		static Mutex msMutex;
	};

#include "PX2GeneralAlloctor.inl"

}

#endif