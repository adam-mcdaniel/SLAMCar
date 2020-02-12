// PX2BufferIO.hpp

#ifndef PX2BUFFERIO_HPP
#define PX2BUFFERIO_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	/// �ڴ滺������д��
	/**
	* ֧�ֶ������ڴ滺������д������������ԡ�ֻ�����ķ�ʽ���ڴ滺����������д
	* ����ᵼ�´���ͬ���ģ����ԡ�ֻд���ķ�ʽ���ڴ滺���������ö����������
	* ����಻֧�֡���д����ʽ���ʡ�
	* ͨ������ʹ��FileIO::Load��ȡ�ļ����ڴ滺������Ȼ��ʹ��BufferIO��ȡ������ֵ��
	* �������������෴��
	*/
	class PX2_ENGINE_ITEM BufferIO
	{
	public:
		/// ��������д��ʽ
		/**
		* BM_NONE
		* BM_READ			�ӻ�������ȡ��û���ֽڽ�����
		* BM_WRITE			д�뻺������û���ֽڽ�����
		* BM_READ_AND_SWAP	�ӻ�������ȡ���ҽ�����2-��4-��8-���룩
		* BM_WRITE_AND_SWAP	д�뻺�������ҽ�����2-��4-��8-���룩
		*/
		enum
		{
			BM_NONE,
			BM_READ,
			BM_WRITE,
			BM_READ_AND_SWAP,
			BM_WRITE_AND_SWAP,

#ifdef PX2_LITTLE_ENDIAN
			BM_DEFAULT_READ = BM_READ,
			BM_DEFAULT_WRITE = BM_WRITE
#else
			BM_DEFAULT_READ = BM_READ_AND_SWAP,
			BM_DEFAULT_WRITE = BM_WRITE_AND_SWAP
#endif
		};

		BufferIO ();
		/// ���캯��
		/**
		* �������ṩ��Ҫ����ȡ����д����ڴ滺�������������Լ���������ڴ档��
		* �滺������һ�������Ƕ�̬����ġ�
		* ע�⣺�ڽ����ݶ�ȡ���ⲿ��ʱ����Ҫ����GetNumBytes()�����������
		*/
		BufferIO (int numBytesTotal, char* buffer, int mode);
		~BufferIO ();

		bool Open (int numBytesTotal, char* buffer, int mode);
		bool Close ();

		// ��ʽת����������⻺�������Ƿ�ɹ���
		inline operator bool () const;

		// ��Ա����
		inline const char* GetBuffer () const;
		inline int GetNumBytesTotal () const;
		inline int GetNumBytesProcessed () const;
		inline int GetMode () const;
		bool IncrementNumBytesProcessed (int numBytes);

		// ��������ɹ�������'true'
		bool Read (size_t itemSize, void* datum);
		bool Read (size_t itemSize, int numItems, void* data);
		bool Write (size_t itemSize, const void* datum);
		bool Write (size_t itemSize, int numItems, const void* data);

	private:
		char* mBuffer;
		int mNumBytesTotal;
		int mNumBytesProcessed;
		int mMode;
	};

#include "PX2BufferIO.inl"

}

#endif