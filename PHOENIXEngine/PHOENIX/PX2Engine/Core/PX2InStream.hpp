// PX2InStream.hpp

#ifndef PX2INSTREAM_H
#define PX2INSTREAM_H

#include "PX2CorePre.hpp"
#include "PX2Stream.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class Object;

	class PX2_ENGINE_ITEM InStream : public Stream
	{
	public:
		InStream ();
		virtual ~InStream ();

		// ���صĶ��㼶��top-level������
		int GetNumObjects () const;
		Object* GetObjectAt (int i) const;

		// ��Ӳ�̼��ض����б������߱����Լ���������Ļ�����buffer��������һ��
		// ��֪��Ҫ���ٶ��Ļ��������������Ϊ����ļ��غ�������
		void Load (int bufferSize, char* buffer,
			int mode = BufferIO::BM_DEFAULT_READ);

		bool Load1 (int bufferSize, char* buffer,
			std::string version = "PX2_VERSION_1_0",
			int mode = BufferIO::BM_DEFAULT_READ);

		// ���ļ���ȡ�����ļ��ڲ��汾�ַ�����Ͳ���version���бȽϣ�����ַ���һ��
		// ���󱻼��ص��б���������ɹ������������档
		bool Load (const std::string& name,
			std::string version = "PX2_VERSION_1_0",
			int mode = BufferIO::BM_DEFAULT_READ);

public_internal:
		// ������������3����ʽ��ȡ���飺VV��VR��RR��V��ʾ��ֵ��ȡ��R��ʾ������
		// �ö�ȡ��
		//	numElements:
		//		V = �����ߴ��ݴ�С
		//		R = �ӻ������ж�ȡ
		//	data:
		//		V = �ɵ��������ȷ����ڴ�
		//		R = �к�����buffer�ж�ȡ�ڴ��С��Ȼ����з���

		// ԭʼ���Ͷ�����
		template <typename T> bool Read (T& datum);
		template <typename T> bool ReadVV (int numElements, T* data);
		template <typename T> bool ReadVR (int numElements, T*& data);
		template <typename T> bool ReadRR (int& numElements, T*& data);

		// ����4-byte��С��boolֵ
		bool ReadBool (bool& datum);
		bool ReadBoolVV (int numElements, bool* data);
		bool ReadBoolVR (int numElements, bool*& data);
		bool ReadBoolRR (int& numElements, bool*& data);

		// ��ȡ�ַ�������4-byte��ʾ�ַ������ȣ������ŵ����ַ���
		bool ReadString (std::string& datum);
		bool ReadStringVV (int numElements, std::string* data);
		bool ReadStringVR (int numElements, std::string*& data);
		bool ReadStringRR (int& numElements, std::string*& data);

		// ����һ��ö�ٱ���4-byte��ȡö��ֵ
		template <typename T> bool ReadEnum (T& datum);
		template <typename T> bool ReadEnumVV (int numElements, T* data);
		template <typename T> bool ReadEnumVR (int numElements, T*& data);
		template <typename T> bool ReadEnumRR (int& numElements, T*& data);

		// ��ȡ�����ָ�루ָ�뱻��Ϊ����Ķ�һ�޶���ID��
		template <typename T> bool ReadPointer (T*& object);
		template <typename T> bool ReadPointerVV (int numElements, T** objects);
		template <typename T> bool ReadPointerVR (int numElements, T**& objects);
		template <typename T> bool ReadPointerRR (int& numElements, T**& objects);

		template <typename T> bool ReadPointer (Pointer0<T>& object);
		template <typename T> bool ReadPointerVV (int numElements,
			Pointer0<T>* objects);
		template <typename T> bool ReadPointerVR (int numElements,
			Pointer0<T>*& objects);
		template <typename T> bool ReadPointerRR (int& numElements,
			Pointer0<T>*& objects);

		// ֧����չ���͵Ķ�ȡ����Щ��չ���ͱ�������չ���͵������ר��д����
		// TDCore�в��ᱻʵ�֡�
		template <typename T> bool ReadAggregate (T& datum);
		template <typename T> bool ReadAggregateVV (int numElements, T* data);
		template <typename T> bool ReadAggregateVR (int numElements, T*& data);
		template <typename T> bool ReadAggregateRR (int& numElements, T*& data);

		// ��������
		int GetBytesRead () const;
		void ReadUniqueID (Object* object);
		template <typename T> void ResolveLink (T*& object);
		template <typename T> void ResolveLink (int numElements, T** object);
		template <typename T> void ResolveLink (Pointer0<T>& object);
		template <typename T> void ResolveLink (int numElements,
			Pointer0<T>* objects);

	private:
		typedef std::map<uint64_t, Object*> LinkMap; //< uint64_t��¼��mOrdered�е�˳��ţ�Object*��¼��ǰ����ָ��
		typedef std::vector<Object*> LinkArray;

		// ��Ҫ���־û��Ķ����б�
		std::vector<Object*> mTopLevel;

		// ���س��������ж�����б�
		LinkMap mLinked;
		LinkArray mOrdered;
		BufferIO mSource;
	};

#include "PX2InStream.inl"

}

#endif