// PX2Buffer.hpp

#ifndef PX2BUFFER_HPP
#define PX2BUFFER_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"

namespace PX2
{

	class Renderer;

	/// ��Դ������
	/**
	* vertex buffers, index buffers, ��textures��Щ��Դ�Ӵ���������
	*/
	class PX2_ENGINE_ITEM Buffer : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Buffer);

	public:
		/// Buffer�������
		/**
		* ���vertex buffers, index buffers, ��textures��
		*/
		enum Usage
		{
			BU_STATIC,
			BU_DYNAMIC,
			BU_RENDERTARGET,
			BU_DEPTHSTENCIL,
			BU_TEXTURE,
			BU_QUANTITY
		};

		// Buffer����ģʽ
		enum Locking
		{
			BL_READ_ONLY,
			BL_WRITE_ONLY,
			BL_READ_WRITE,
			BL_QUANTITY
		};

	protected:
		Buffer ();
		Buffer (int numElements, int elementSize, Usage usage);
	public:
		virtual ~Buffer ();

		// ��Ա����
		inline int GetNumElements () const;
		inline int GetNumMaxElements() const;
		inline int GetElementSize () const;
		inline Usage GetUsage () const;

		/// ���ü����buffer����
		/**
		* ���ͨ�������ڶ������������С���������ı�ԭ�����ݵĴ洢��ֻ�ı�
		* Buffer::mNumElements����������ְ�𱣴�ԭ�еĶ����������������������
		* �����������ú��û�ԭ�еĶ����������������ߴ��ݵ�numElements�����ԭ��
		* �Ķ������������١�
		*/
		inline void SetNumElements (int numElements);

		// ��VertexBuffer��IndexBuffer����Ҫ���ʻ������ݡ�GetData()���ص�ֻ����
		// ֻ�����ݡ���Ҫ��"char*"����ǿ��ת����Ȼ��д���µ����ݣ�������Ⱦ������
		// ֪�����ݱ��ı��ˡ�
		inline int GetNumBytes () const;
		inline const char* GetData () const;

		void ClearData ();

		// pdr
	public:
		void *PdrPointer;

	protected:
		int mNumElements;
		int mNumMaxElements;
		int mElementSize;
		Usage mUsage;
		int mNumBytes;
		char* mData;
	};

	PX2_REGISTER_STREAM(Buffer);
	typedef Pointer0<Buffer> BufferPtr;
#include "PX2Buffer.inl"

}

#endif