// PX2FileIO.hpp

#ifndef PX2FILEIO_HPP
#define PX2FILEIO_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	/// �ļ���д��
	/**
	* ֧�ֶ������ļ���д��������ԡ�ֻ�����ķ�ʽ���ļ�������д�����ᵼ�´���
	* ͬ���ģ�������ԡ�ֻд����ʽ���ļ������ö���������ִ�������಻ͬʱ
	* ֧�֡���д�����ʡ�������װ��׼�ļ�������
	*/
	class PX2_ENGINE_ITEM FileIO
	{
	public:
		/// �ļ���ȡ��ʽ
		/**
		* FM_READ			�Ӵ��̶�ȡ��û���ֽڽ�����
		* FM_WRITE			д����̣�û���ֽڽ�����
		* FM_READ_AND_SWAP	�Ӵ��̶�ȡ���ҽ�����2-��4-��8-���룩
		* FM_WRITE_AND_SWAP	д����̲��ҽ�����2-��4-��8-���룩
		*/
		enum
		{
			FM_NONE,
			FM_READ,
			FM_WRITE,
			FM_READ_AND_SWAP,
			FM_WRITE_AND_SWAP,

#ifdef PX2_LITTLE_ENDIAN
			FM_DEFAULT_READ = FM_READ,
			FM_DEFAULT_WRITE = FM_WRITE
#else
			FM_DEFAULT_READ = FM_READ_AND_SWAP,
			FM_DEFAULT_WRITE = FM_WRITE_AND_SWAP
#endif
		};

		FileIO ();
		FileIO (const std::string& filename, int mode);
		~FileIO ();

		bool Open (const std::string& filename, int mode);
		bool Close ();

		// ��ʽת������������ļ��Ƿ�ɹ��򿪡�
		operator bool () const;

		inline int GetMode () const;

		enum SeekMode
		{
			SM_SET = 0,
			SM_CUR = 1,
			SM_END = 2
		};

		int Seek (long offset, SeekMode mode);

		// �����ɹ�����'true'
		bool Read (size_t itemSize, void* datum);
		bool Read (size_t itemSize, int numItems, void* data);
		bool Write (size_t itemSize, const void* datum);
		bool Write (size_t itemSize, int numItems, const void* data);

		// load to buffer
		static bool Load (const std::string& filename, bool binaryFile,
			int& bufferSize, char*& buffer);

		// save buffer to file
		static bool Save (const std::string& filename, bool binaryFile,
			int bufferSize, const char* buffer);

		// append buffer to file
		static bool Append (const std::string& filename, bool binaryFile,
			int bufferSize, const char* buffer);

	private:
		FILE* mFile;
		int mMode;
	};

#include "PX2FileIO.inl"

}

#endif