// PX2OutStream.hpp

#ifndef PX2OUTSTREAM_HPP
#define PX2OUTSTREAM_HPP

#include "PX2CorePre.hpp"
#include "PX2Stream.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class Object;

	class PX2_ENGINE_ITEM OutStream : public Stream
	{
	public:
		OutStream ();
		virtual ~OutStream ();

		// ���صĶ��㼶��top-level������
		bool Insert (Object* object);
		bool IsTopLevel (const Object* object) const;

		// ������д���ڴ棬����buffer����new1���з���ģ����������ʹ��delete1
		// �����ͷš�������һ�㲻֪��Ҫ���ٶ��Ļ��������������Ϊ����ļ��غ�
		// ��������������ɹ�����������'true'��
		void Save (int& bufferSize, char*& buffer,
			int mode = BufferIO::BM_DEFAULT_WRITE);

		/**
		* ����Ҫ����Ķ���д�뵽����.�汾�ַ������ȱ�д�룬���д������Ҫ����
		* �Ķ���.��������ɹ�����������'true'��
		*/
		bool Save (const std::string& name,
			std::string version = "PX2_VERSION_1_0",
			int mode = BufferIO::BM_DEFAULT_WRITE);

public_internal:
		// ��������д�����飬����������������д�룺W��N��
		// W����numElementsд��buffer��
		// N: do not��numElementsд��buffer��

		// ԭʼ��������д����
		template <typename T> bool Write (T datum);
		template <typename T> bool WriteW (int numElements, const T* data);
		template <typename T> bool WriteN (int numElements, const T* data);

		// ��4-byte��Сд��boolֵ
		bool WriteBool (const bool datum);
		bool WriteBoolW (int numElements, const bool* data);
		bool WriteBoolN (int numElements, const bool* data);

		// д���ַ�������4-byte��ʾ�ַ������ȣ������ŵ����ַ���
		bool WriteString (const std::string& datum);
		bool WriteStringW (int numElements, const std::string* data);
		bool WriteStringN (int numElements, const std::string* data);

		// ����һ��ö�ٱ���4-byteд��ö��ֵ
		template <typename T> bool WriteEnum (const T datum);
		template <typename T> bool WriteEnumW (int numElements, const T* data);
		template <typename T> bool WriteEnumN (int numElements, const T* data);

		// ����ָ��д��
		template <typename T> bool WritePointer (const T* object);
		template <typename T> bool WritePointerW (int numElements,
			T* const* objects);
		template <typename T> bool WritePointerN (int numElements,
			T* const* objects);

		template <typename T> bool WritePointer (const Pointer0<T>& object);

		template <typename T> bool WritePointerW (int numElements,
			Pointer0<T> const* objects);

		template <typename T> bool WritePointerN (int numElements,
			Pointer0<T> const* objects);

		// ֧����չ���͵�д�롣��Щ��չ���ͱ�������չ���͵������ר��д����
		// TDCore�в��ᱻʵ�֡�
		template <typename T> bool WriteAggregate (const T& datum);
		template <typename T> bool WriteAggregateW (int numElements,
			const T* data);
		template <typename T> bool WriteAggregateN (int numElements,
			const T* data);

		// ��������
		int GetBytesWritten () const;
		bool RegisterRoot (const Object* object);
		void WriteUniqueID (const Object* object);

		template <typename T> void Register(const T* object);
		template <typename T> void Register(int numElements, T* const* objects);
		template <typename T> void Register(const Pointer0<T>& object);
		template <typename T> void Register(int numElements,
			Pointer0<T> const* objects);

	private:
		typedef std::map<const Object*, unsigned int> RegisterMap;
		typedef std::vector<const Object*> RegisterArray;

		// �������г־û��Ķ����б�
		std::vector<Object*> mTopLevel;

		// �ڱ��泡��ʱ������������ȱ���ʱ�������洢������б�
		RegisterMap mRegistered;
		RegisterArray mOrdered;
		BufferIO mTarget;
	};

#include "PX2OutStream.inl"

}

#endif
