// PX2File_WIN32.hpp

#ifndef PX2FILE_WIN32_HPP
#define PX2FILE_WIN32_HPP

#if defined (WIN32) || defined(_WIN32)

#include "PX2CorePre.hpp"
#include "PX2Timestamp.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM FileImpl
	{
	protected:
		typedef uint64_t FileSizeImpl;

		FileImpl();
		FileImpl(const std::string& path);
		virtual ~FileImpl();
		void swapImpl(FileImpl& file);
		void setPathImpl(const std::string& path);
		const std::string& getPathImpl() const;
		bool existsImpl() const;
		bool canReadImpl() const;
		bool canWriteImpl() const;
		bool canExecuteImpl() const;
		bool isFileImpl() const;
		bool isDirectoryImpl() const;
		bool isLinkImpl() const;
		bool isDeviceImpl() const;
		bool isHiddenImpl() const;
		Timestamp createdImpl() const;
		Timestamp getLastModifiedImpl() const;
		void setLastModifiedImpl(const Timestamp& ts);
		FileSizeImpl getSizeImpl() const;
		void setSizeImpl(FileSizeImpl size);
		void setWriteableImpl(bool flag = true);
		void setExecutableImpl(bool flag = true);
		void copyToImpl(const std::string& path) const;
		void renameToImpl(const std::string& path);
		void removeImpl();
		bool createFileImpl();
		bool createDirectoryImpl();
		static void handleLastErrorImpl(const std::string& path);

	private:
		std::string  _path;
		std::wstring _upath;

		friend class FileHandle;
		friend class DirectoryIteratorImpl;
		friend class WindowsDirectoryWatcherStrategy;
	};

	inline const std::string& FileImpl::getPathImpl() const
	{
		return _path;
	}

}

#endif

#endif 
