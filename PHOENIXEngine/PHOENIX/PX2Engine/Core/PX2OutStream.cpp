// PX2OutStream.cpp

#include "PX2OutStream.hpp"
#include "PX2FileIO.hpp"
#include "PX2Object.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
OutStream::OutStream ()
{
	mStreamType = ST_OUT;
}
//----------------------------------------------------------------------------
OutStream::~OutStream ()
{
}
//----------------------------------------------------------------------------
bool OutStream::Insert (Object* object)
{
	if (object)
	{
		// ͬһ������ֻ�ܱ����뵽�����б�һ��
		std::vector<Object*>::iterator iter = mTopLevel.begin();
		std::vector<Object*>::iterator end = mTopLevel.end();
		for (/**/; iter != end; ++iter)
		{
			if (object == *iter)
			{
				return false;
			}
		}

		if (object)
		{
			mTopLevel.push_back(object);
			return true;
		}
	}

	return false;
}
//----------------------------------------------------------------------------
bool OutStream::IsTopLevel (const Object* object) const
{
	std::vector<Object*>::const_iterator iter = mTopLevel.begin();
	std::vector<Object*>::const_iterator end = mTopLevel.end();
	for (/**/; iter != end; ++iter)
	{
		if (object == *iter)
		{
			return true;
		}
	}
	return false;
}
//----------------------------------------------------------------------------
void OutStream::Save (int& bufferSize, char*& buffer, int mode)
{
	// ����ָ���0�����ԣ���֤ÿ�������ID��0��ʼ, ��Ϊ0ָ����Ҫ��
	const Object* object = 0;
	unsigned int uniqueID = 0;
	mRegistered.insert(std::make_pair(object, uniqueID));
	mOrdered.push_back(object);

	// ��Ҫ����Ķ������ע��ct->Register(*this);
	std::vector<Object*>::iterator iterT = mTopLevel.begin();
	std::vector<Object*>::iterator endT = mTopLevel.end();
	for (/**/; iterT != endT; ++iterT)
	{
		object = *iterT;
		object->Register(*this);
	}

	// ��Ҫ�Ļ�������С
	RegisterArray::iterator iterO = mOrdered.begin();
	RegisterArray::iterator endO = mOrdered.end();
	bufferSize = 0;
	for (++iterO; iterO != endO; ++iterO)
	{
		object = *iterO;
		bufferSize += object->GetStreamingSize(*this);
	}

	// Ϊ"Top Level"�ַ���������������С
	std::string topLevel("Top Level");
	int numTopLevelBytes = GetStreamingSize(topLevel);
	bufferSize += numTopLevelBytes*(int)mTopLevel.size();

	// ������Ҫ�Ļ�����
	buffer = new1<char>(bufferSize);
	mTarget.Open(bufferSize, buffer, mode);

	// ������д�뻺����
	iterO = mOrdered.begin();
	endO = mOrdered.end();
	for (++iterO, uniqueID = 1; iterO != endO; ++iterO, ++uniqueID)
	{
		object = *iterO;
		if (IsTopLevel(object))
		{
			WriteString(topLevel);
		}
		object->Save(*this);
	}

	// ���������Ϊ����������׼����
	mRegistered.clear();
	mOrdered.clear();
	mTarget.Close();
}
//----------------------------------------------------------------------------
bool OutStream::Save (const std::string& name, std::string version, int mode)
{
	// �����ļ��İ汾��
	if (!FileIO::Save(name, true, (int)version.length(), version.c_str()))
	{
		// �ļ����ܱ���
		return false;
	}

	// ���������浽�ڴ�buffer
	int bufferSize;
	char* buffer;
	Save(bufferSize, buffer, mode);

	// ������ͼ���ڴ汣�浽Ӳ��
	if (!FileIO::Append(name, true, bufferSize, buffer))
	{
		// �ļ����ܱ�appended
		delete1(buffer);
		return false;
	}

	delete1(buffer);
	return true;
}
//----------------------------------------------------------------------------
bool OutStream::WriteBool (const bool datum)
{
	unsigned int value = (datum ? 0xFFFFFFFFu : 0u);
	return mTarget.Write(sizeof(unsigned int), &value);
}
//----------------------------------------------------------------------------
bool OutStream::WriteBoolW (int numElements, const bool* data)
{
	if (!mTarget.Write(sizeof(int), &numElements))
	{
		return false;
	}

	if (numElements > 0)
	{
		for (int i = 0; i < numElements; ++i)
		{
			if (!WriteBool(data[i]))
			{
				return false;
			}
		}
	}
	return true;
}
//----------------------------------------------------------------------------
bool OutStream::WriteBoolN (int numElements, const bool* data)
{
	if (numElements > 0)
	{
		for (int i = 0; i < numElements; ++i)
		{
			if (!WriteBool(data[i]))
			{
				return false;
			}
		}
	}
	return true;
}
//----------------------------------------------------------------------------
bool OutStream::WriteString (const std::string& datum)
{
	int length = (int)datum.length();
	if (!mTarget.Write(sizeof(int), &length))
	{
		return false;
	}

	if (length > 0)
	{
		if (!mTarget.Write(sizeof(char), length, datum.c_str()))
		{
			return false;
		}

		// �ַ���������4�ֽڱ�����Сд��
		char zero[4] = { 0, 0, 0, 0 };
		int padding = (length % 4);
		if (padding > 0)
		{
			padding = 4 - padding;
			if (!mTarget.Write(sizeof(char), padding, zero))
			{
				return false;
			}
		}
	}
	return true;
}
//----------------------------------------------------------------------------
bool OutStream::WriteStringW (int numElements, const std::string* data)
{
	if (!mTarget.Write(sizeof(int), &numElements))
	{
		return false;
	}

	if (numElements > 0)
	{
		for (int i = 0; i < numElements; ++i)
		{
			if (!WriteString(data[i]))
			{
				return false;
			}
		}
	}
	return true;
}
//----------------------------------------------------------------------------
bool OutStream::WriteStringN (int numElements, const std::string* data)
{
	if (numElements > 0)
	{
		for (int i = 0; i < numElements; ++i)
		{
			if (!WriteString(data[i]))
			{
				return false;
			}
		}
	}
	return true;
}
//----------------------------------------------------------------------------
int OutStream::GetBytesWritten () const
{
	return mTarget.GetNumBytesProcessed();
}
//----------------------------------------------------------------------------
bool OutStream::RegisterRoot (const Object* object)
{
	if (mRegistered.find(object) == mRegistered.end())
	{
		// ���ǵ�һ���ڳ����������ö���������Ҫ���˶���Ķ���һ�޶��ı�ʶ��
		// unique identifier����д�뵽Ӳ�̣���֧�ֶ���ġ�����ʱ�����ӻ��ƣ�
		// loader-linker������Ϊ֧������������򵥵ķ�ʽ����д�����ĵ�ַ��
		// ���Ҫд���ڴ��ַ�Ļ�����Ҫ��Ҫ�������㣺
		//
		// 1. ����Ӧ�ó�����������У�����ķ���ĵ�ַ�ǲ�һ���ģ��⽫���¶���
		//	  �б�д���˳��һ�����������ǲ���map����array��Ϊ�洢������
		//
		// 2. ��ʹ�����˳����ȷ���ģ�����ͬ�������浽�ļ������α��棬�ֽں���
		//	  ���ǲ�ƥ��ġ���������ǣ����Ǹ�ע�����ʱ��Ϊ�������һ��������
		//    ��ַ��virtual address����������ʹ��������index+1��Ϊ������ַ��

		unsigned int uniqueID = (unsigned int)mOrdered.size();
		mRegistered.insert(std::make_pair(object, uniqueID));
		mOrdered.push_back(object);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void OutStream::WriteUniqueID (const Object* object)
{
	unsigned int uniqueID = mRegistered.find(object)->second;
	mTarget.Write(sizeof(unsigned int), &uniqueID);
}
//----------------------------------------------------------------------------