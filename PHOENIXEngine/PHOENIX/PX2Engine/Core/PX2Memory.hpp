// PX2Memory.hpp

#ifndef PX2MEMORY_HPP
#define PX2MEMORY_HPP

#include "PX2CorePre.hpp"
#include "PX2Assert.hpp"
#include "PX2Mutex.hpp"

#if defined (PX2_USE_MEMORY)

namespace PX2
{

	class PX2_ENGINE_ITEM Memory
	{
	public:
		// �����û��Լ��ṩ�ڴ�ķ�����ͷź���
		typedef void* (*Allocator)(size_t numBytes, const char* file, int line);
		typedef void (*Deallocator)(void* memblock, const char* file, int line);

		// ��PX2Application.cpp�е��õĳ�ʼ�����սắ����
		static void Initialize (
			Allocator allocator = &DefaultAllocator,
			Deallocator deallocator = &DefaultDeallocator
			);
		static Allocator msAllocator;
		static Deallocator msDeallocator;

		static void Terminate (const std::string& filename, bool alwaysReportFile=true);

		static int64_t GetCurTotalBytes ();
		static int64_t GetMaxTotalBytes ();

		static void DumpCurMemory (const std::string &filename, int biggerSize=-1);
		
		static void BeginDump_Add (const std::string &filename, int biggerSize=-1);
		static void EndDump_Add ();

public_internal:
		inline Memory (const char* file, int line) : mFile(file), mLine(line) {}
		inline ~Memory () {}

		inline static bool HasMap () { return msMap != 0; }
		void* CreateBlock (size_t numBytes, int numDimensions) const;

		// 1D �������:  data[bound0]
		template <typename T>
		T* New1 (const int bound0);

		// 2D �������:  data[bound1][bound0]
		template <typename T>
		T** New2 (const int bound0, const int bound1);

		// 3D �������:  data[bound2][bound1][bound0]
		template <typename T>
		T*** New3 (const int bound0, const int bound1, const int bound2);

		// 4D �������:  data[bound3][bound2][bound1][bound0]
		template <typename T>
		T**** New4 (const int bound0, const int bound1, const int bound2,
			const int bound3);

		// ��һά���ͷ�
		template <typename T>
		void Delete0 (T*& data);

		// 1D �����ͷ�:  data[bound0]
		template <typename T>
		void Delete1 (T*& data);

		// 2D �����ͷ�:  data[bound1][bound0]
		template <typename T>
		void Delete2 (T**& data);

		// 3D �����ͷ�:  data[bound2][bound1][bound0]
		template <typename T>
		void Delete3 (T***& data);

		// 4D �����ͷ�:  data[bound3][bound2][bound1][bound0]
		template <typename T>
		void Delete4 (T****& data);

private_internal:
		static void* DefaultAllocator (size_t numBytes, const char* file,
			int line);

		static void DefaultDeallocator (void* memBlock,	const char* file,
			int line);

		class Information
		{
		public:
			inline Information () {}

			inline Information (int numBytes, int numDimensions, const char* file,
				int line)
				:
			mNumBytes(numBytes),
				mNumDimensions(numDimensions),
				mFile(file),
				mLine(line),
				mUniqueID(++msUniqueID)
			{
			}
			~Information ()
			{
			}

			int mNumBytes;			//< ������ֽ���
			int mNumDimensions;		//< �����ά��
			const char* mFile;		//< ����ʱ�Ĵ����ļ���
			int mLine;				//< ����ʱ���ļ��еĴ�������
			unsigned int mUniqueID; //< ������ϵͳ�з�����Ⱥ�ID��
			static unsigned int msUniqueID;
		};

		typedef std::map<void*,Information> MemoryMap;
		typedef std::map<unsigned int, std::pair<void*,Information> > SortedMap;

		const char* mFile;
		int mLine;

		// msMutex��������ֹͬʱ����msMap
		static MemoryMap* msMap;
		static Mutex *msMutex;

public_internal:
		static void BeforeEraseInformation(void *data);
		static int64_t msCurTotalBytes;
		static int64_t msMaxTotalBytes;

		static FILE* msDumpFile;
		static MemoryMap *msDumpMap;
		static Mutex *msDumpMutex;
		static bool msIsBeginDumpMemory;
		static int64_t msBiggerSize;
	};

#include "PX2Memory.inl"

}

//----------------------------------------------------------------------------
inline void* operator new (size_t numBytes, const PX2::Memory& memory)
{
	if (memory.HasMap())
	{
		return memory.CreateBlock(numBytes, 0);
	}
	else
	{
#ifdef PX2_USE_MEMORY_ASSERT_ON_PREMAIN_POSTMAIN_OPERATIONS
		assertion(false, "Pre-main allocations are not tracked.\n");
#endif
		return malloc(numBytes);
	}
}
//----------------------------------------------------------------------------
inline void operator delete (void*, const PX2::Memory&)
{
	// Only called during exception handling.
}
//----------------------------------------------------------------------------

#define new0 new(PX2::Memory(__FILE__,__LINE__))
#define new1 PX2::Memory(__FILE__,__LINE__).New1
#define new2 PX2::Memory(__FILE__,__LINE__).New2
#define new3 PX2::Memory(__FILE__,__LINE__).New3
#define new4 PX2::Memory(__FILE__,__LINE__).New4
#define delete0 PX2::Memory(__FILE__,__LINE__).Delete0
#define delete1 PX2::Memory(__FILE__,__LINE__).Delete1
#define delete2 PX2::Memory(__FILE__,__LINE__).Delete2
#define delete3 PX2::Memory(__FILE__,__LINE__).Delete3
#define delete4 PX2::Memory(__FILE__,__LINE__).Delete4

#else

// ��׼�ڴ����
#define new0 new

// 1D �������:  data[bound0]
template <typename T>
T* new1 (const int bound0);

// 2D �������:  data[bound1][bound0]
template <typename T>
T** new2 (const int bound0, const int bound1);

// 3D �������:  data[bound2][bound1][bound0]
template <typename T>
T*** new3 (const int bound0, const int bound1, const int bound2);

// 4D �������:  data[bound3][bound2][bound1][bound0]
template <typename T>
T**** new4 (const int bound0, const int bound1, const int bound2,
			const int bound3);

// ��һά���ͷ�
template <typename T>
void delete0 (T*& data);

// 1D �����ͷ�:  data[bound0]
template <typename T>
void delete1 (T*& data);

// 2D �����ͷ�:  data[bound1][bound0]
template <typename T>
void delete2 (T**& data);

// 3D �����ͷ�:  data[bound2][bound1][bound0]
template <typename T>
void delete3 (T***& data);

// 4D �����ͷ�:  data[bound3][bound2][bound1][bound0]
template <typename T>
void delete4 (T****& data);

#include "PX2Memory.inl"

#endif

#endif