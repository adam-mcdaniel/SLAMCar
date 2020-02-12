// PX2InStream.cpp

#include "PX2InStream.hpp"
#include "PX2FileIO.hpp"
#include "PX2Object.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
InStream::InStream ()
{
	mStreamType = ST_IN;
}
//----------------------------------------------------------------------------
InStream::~InStream ()
{
}
//----------------------------------------------------------------------------
int InStream::GetNumObjects () const
{
	return (int)mTopLevel.size();
}
//----------------------------------------------------------------------------
Object* InStream::GetObjectAt (int i) const
{
	if (0 <= i && i < (int)mTopLevel.size())
	{
		return mTopLevel[i];
	}
	return 0;
}
//----------------------------------------------------------------------------
void InStream::Load (int bufferSize, char* buffer, int mode)
{
	// Ϊ����buffer������ȡ��
	mSource.Open(bufferSize, buffer, mode);

	// ���ض��㼶�������б�
	std::string topLevel("Top Level");
	Object* object;
	while (mSource.GetNumBytesProcessed() < mSource.GetNumBytesTotal())
	{
		// ��ȡ"Top Level"����RTTI����
		std::string name;
		ReadString(name);
		bool isTopLevel = (name == topLevel);
		if (isTopLevel)
		{
			// ��ȡRTTI����
			ReadString(name);
		}

		// ��øö���Ĺ����������������Object::Find(name)����ʧ�ܣ�ȷ�����Ѿ�
		// ��main����WinMain��ʼʱ������InitTerm::ExecuteInitializers()��
		// InitTerm�ڵĳ�ʼ����������������������map��
		Object::FactoryFunction factory = Object::Find(name);
		if (!factory)
		{
			// Ѱ�Ҳ���������������.ȷ�������PX2_REGISTER_STREAM(someclass)��
			// ����'someclass'��ͷ�ļ��С������Ϊ�����ע���˹�������������
			assertion(false, "Cannot find factory for %s.\n", name.c_str());
			return;
		}

		// �������󣬼��س�Ա����
		object = (*factory)(*this);

		// ����Ƕ��㼶�����壬��������б�
		if (isTopLevel)
		{
			mTopLevel.push_back(object);
		}
	}

	// ���Ӷ���
	std::vector<Object* >::iterator iter = mOrdered.begin();
	std::vector<Object* >::iterator end = mOrdered.end();
	for (/**/; iter != end; ++iter)
	{
		(*iter)->Link(*this);
	}

	// �����󱻼���ʱ�����ö����ȱʡ���캯����������PostLink()��������һЩ
	// ��ȱʡ���캯��������ɵĹ�����
	iter = mOrdered.begin();
	end = mOrdered.end();
	for (/**/; iter != end; ++iter)
	{
		(*iter)->PostLink();
	}

	// ����б�Ϊ�����ĵ�����׼����
	mLinked.clear();
	mOrdered.clear();
	mSource.Close();
}
//----------------------------------------------------------------------------
bool InStream::Load1 (int bufferSize, char* buffer, std::string version,
	int mode)
{
	int length = (int)version.length();
	if (bufferSize < length)
	{
		return false;
	}

	if (strncmp(version.c_str(), buffer, length) != 0)
	{
		return false;
	}

	bufferSize -= length;
	Load(bufferSize, buffer + length, mode);

	return true;
}
//----------------------------------------------------------------------------
bool InStream::Load (const std::string& name, std::string version, int mode)
{
	// ��Ӳ�̼��ص��ڴ档
	int bufferSize;
	char* buffer;
	if (!FileIO::Load(name, true, bufferSize, buffer))
	{
		return false;
	}

	// ����ļ��汾
	int length = (int)version.length();
	if (bufferSize < length)
	{
		// �ļ����洢�ļ��汾�ַ����Ĵ�С������
		delete1(buffer);
		return false;
	}

	// ����Ҫ���صİ汾�ź��ļ��汾���Ƚ�
	if (strncmp(version.c_str(), buffer, length) != 0)
	{
		// �汾��ƥ��
		delete1(buffer);
		return false;
	}

	// ���ڴ�buffer�����¹�������ͼ
	bufferSize -= length;
	Load(bufferSize, buffer + length, mode);

	delete1(buffer);
	return true;
}
//----------------------------------------------------------------------------
bool InStream::ReadBool (bool& datum)
{
	unsigned int value;
	if (!mSource.Read(sizeof(unsigned int), &value))
	{
		return false;
	}
	datum = (value != 0);
	return true;
}
//----------------------------------------------------------------------------
bool InStream::ReadBoolVV (int numElements, bool* data)
{
	if (numElements > 0)
	{
		for (int i = 0; i < numElements; ++i)
		{
			if (!ReadBool(data[i]))
			{
				return false;
			}
		}
	}
	return true;
}
//----------------------------------------------------------------------------
bool InStream::ReadBoolVR (int numElements, bool*& data)
{
	if (numElements > 0)
	{
		data = new1<bool>(numElements);
		for (int i = 0; i < numElements; ++i)
		{
			if (!ReadBool(data[i]))
			{
				return false;
			}
		}
	}
	else
	{
		data = 0;
	}

	return true;
}
//----------------------------------------------------------------------------
bool InStream::ReadBoolRR (int& numElements, bool*& data)
{
	if (!mSource.Read(sizeof(int), &numElements))
	{
		return false;
	}

	if (numElements > 0)
	{
		data = new1<bool>(numElements);
		for (int i = 0; i < numElements; ++i)
		{
			if (!ReadBool(data[i]))
			{
				return false;
			}
		}
	}
	else
	{
		data = 0;
	}

	return true;
}
//----------------------------------------------------------------------------
bool InStream::ReadString (std::string& datum)
{
	int length;
	if(!mSource.Read(sizeof(int), &length))
	{
		datum.clear();
		return false;
	}

	if(length == 0)
	{
		datum.resize(0);
		return true;
	}

	// ���ַ������ȵ浽���ֽڵı���
	int padding = (length % 4);
	if (padding > 0)
	{
		padding = 4 - padding;
	}

	const char* text = mSource.GetBuffer() + mSource.GetNumBytesProcessed();
	datum.assign(text, length);
	mSource.IncrementNumBytesProcessed(length + padding);
	return true;
}
//----------------------------------------------------------------------------
bool InStream::ReadStringVV (int numElements, std::string* data)
{
	if (numElements > 0)
	{
		for (int i = 0; i < numElements; ++i)
		{
			if (!ReadString(data[i]))
			{
				return false;
			}
		}
	}
	return true;
}
//----------------------------------------------------------------------------
bool InStream::ReadStringVR (int numElements, std::string*& data)
{
	if (numElements > 0)
	{
		data = new1<std::string>(numElements);
		for (int i = 0; i < numElements; ++i)
		{
			if (!ReadString(data[i]))
			{
				return false;
			} 
		}
	}
	else
	{
		data = 0;
	}

	return true;
}
//----------------------------------------------------------------------------
bool InStream::ReadStringRR (int& numElements, std::string*& data)
{
	if (!mSource.Read(sizeof(int), &numElements))
	{
		return false;
	}

	if (numElements > 0)
	{
		data = new1<std::string>(numElements);
		for (int i = 0; i < numElements; ++i)
		{
			if (!ReadString(data[i]))
			{
				return false;
			}
		}
	}
	else
	{
		data = 0;
	}

	return true;
}
//----------------------------------------------------------------------------
int InStream::GetBytesRead () const
{
	return mSource.GetNumBytesProcessed();
}
//----------------------------------------------------------------------------
void InStream::ReadUniqueID (Object* object)
{
	unsigned int uniqueID;
	if (mSource.Read(sizeof(unsigned int), &uniqueID))
	{
		mLinked.insert(std::make_pair((uint64_t)uniqueID, object));
		mOrdered.push_back(object);
	}
}
//----------------------------------------------------------------------------