// PX2Stream.hpp

#ifndef PX2STREAM_HPP
#define PX2STREAM_HPP

#include "PX2CorePre.hpp"
#include "PX2BufferIO.hpp"
#include "PX2InitTerm.hpp"
#include "PX2Memory.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Stream
	{
	public:
		/// �ַ����Ĵ洢�����ֽ�����
		/**
		* �ַ����ĳ����ȱ���Ϊһ������ֵ���ȴ洢�������ŵ����ַ���д����ַ���
		* ���浽4�ֽڵı�����С��
		*/
		static int GetStreamingSize (const std::string& input);

		enum StreamType
		{
			ST_IN,
			ST_OUT,
			ST_MAX_TYPE
		};

		StreamType GetStreamType () const;
		bool IsIn () const;

	protected:
		// �������
		Stream ();
		virtual ~Stream ();

		StreamType mStreamType;
	};

#include "PX2Stream.inl"

}

//----------------------------------------------------------------------------
#define PX2_NEW(classname) \
public: \
	static classname *New() \
	{ \
		classname *obj = new0 classname(); \
		return obj; \
	} \
	\
	static classname *New(const std::string &name) \
	{ \
		classname *obj = new0 classname(); \
		obj->SetName(name); \
		return obj; \
	}
//----------------------------------------------------------------------------
#define PX2_DECLARE_STREAM(classname) \
public: \
    static bool RegisterFactory (); \
    static void InitializeFactory (); \
    static Object* Factory (InStream&); \
    virtual void Load (InStream&); \
    virtual void Link (InStream&); \
    virtual void PostLink (); \
    virtual bool Register (OutStream&) const; \
    virtual void Save (OutStream&) const; \
    virtual int GetStreamingSize (Stream &) const; \
protected: \
    friend class Memory; \
    classname (LoadConstructor value); \
private: \
	unsigned short mReadedVersion; \
	unsigned short GetReadedVersion () const \
	{ \
		return mReadedVersion; \
	} \
private: \
    static bool msStreamRegistered
//----------------------------------------------------------------------------
#define PX2_IMPLEMENT_STREAM(classname) \
bool classname::msStreamRegistered = false; \
\
bool classname::RegisterFactory () \
{ \
    if (!msStreamRegistered) \
    { \
        InitTerm::AddInitializer(classname::InitializeFactory); \
        msStreamRegistered = true; \
    } \
    return msStreamRegistered; \
} \
\
void classname::InitializeFactory () \
{ \
    if (!msFactories) \
    { \
        msFactories = new0 Object::FactoryMap(); \
    } \
    /*msFactories->insert(std::make_pair(TYPE.GetName(), Factory));*/ \
	(*msFactories)[TYPE.GetName()] = Factory; \
}
//----------------------------------------------------------------------------
#define PX2_VERSION_LOAD(source) \
	source.Read(mReadedVersion)

#define PX2_VERSION_SAVE(target) \
	target.Write(TYPE.GetVersion())

#define PX2_VERSION_SIZE(value) 2
//----------------------------------------------------------------------------
#define PX2_IMPLEMENT_DEFAULT_STREAM(baseclassname, classname) \
classname::classname (LoadConstructor value) \
    : \
    baseclassname(value), \
	mReadedVersion(0) \
{ \
} \
\
void classname::Load (InStream& source) \
{ \
    PX2_BEGIN_DEBUG_STREAM_LOAD(source); \
    baseclassname::Load(source); \
	PX2_VERSION_LOAD(source); \
    PX2_END_DEBUG_STREAM_LOAD(classname, source); \
} \
\
void classname::Link (InStream& source) \
{ \
    baseclassname::Link(source); \
} \
void classname::PostLink () \
{ \
    baseclassname::PostLink(); \
} \
\
bool classname::Register (OutStream& target) const \
{ \
    return baseclassname::Register(target); \
} \
\
void classname::Save (OutStream& target) const \
{ \
    PX2_BEGIN_DEBUG_STREAM_SAVE(target); \
    baseclassname::Save(target); \
	PX2_VERSION_SAVE(target); \
    PX2_END_DEBUG_STREAM_SAVE(classname, target); \
} \
\
int classname::GetStreamingSize (Stream &stream) const \
{ \
	int size = baseclassname::GetStreamingSize(stream); \
	size += PX2_VERSION_SIZE(mVersion); \
    return size; \
}
//----------------------------------------------------------------------------
#define PX2_IMPLEMENT_ABSTRACT_FACTORY(classname) \
Object* classname::Factory (InStream&) \
{ \
    assertion(false, "Abstract classes have no factory.\n"); \
    return 0; \
}
//----------------------------------------------------------------------------
#define PX2_IMPLEMENT_FACTORY(classname) \
Object* classname::Factory (InStream& source) \
{ \
    classname* object = new0 classname(LC_LOADER); \
    object->Load(source); \
    return object; \
}
//----------------------------------------------------------------------------
#define PX2_REGISTER_STREAM(classname) \
static bool gsStreamRegistered_##classname = classname::RegisterFactory()
//----------------------------------------------------------------------------
#ifdef PX2_USE_ASSERT
#define PX2_BEGIN_DEBUG_STREAM_LOAD(source) \
    int beginLoad_ = source.GetBytesRead()
#else
#define PX2_BEGIN_DEBUG_STREAM_LOAD(source)
#endif
//----------------------------------------------------------------------------
#ifdef PX2_USE_ASSERT
#define PX2_END_DEBUG_STREAM_LOAD(classname,source) \
    int endLoad_ = source.GetBytesRead(); \
    int usedLoad_ = endLoad_ - beginLoad_ + \
        Stream::GetStreamingSize(GetRttiType().GetName()); \
    int usedReported_ = classname::GetStreamingSize(source); \
    assertion(usedLoad_ == usedReported_, \
        "Mismatched bytes during Load: loaded = %d, required = %d\n", \
        usedLoad_, usedReported_)
#else
#define PX2_END_DEBUG_STREAM_LOAD(classname,source)
#endif
//----------------------------------------------------------------------------
#ifdef PX2_USE_ASSERT
#define PX2_BEGIN_DEBUG_STREAM_SAVE(target) \
    int beginSave_ = target.GetBytesWritten()
#else
#define PX2_BEGIN_DEBUG_STREAM_SAVE(target)
#endif
//----------------------------------------------------------------------------
#ifdef PX2_USE_ASSERT
#define PX2_END_DEBUG_STREAM_SAVE(classname,target) \
    int endSave_ = target.GetBytesWritten(); \
    int usedSave_ = endSave_ - beginSave_; \
    int usedReported_ = classname::GetStreamingSize(target); \
    assertion(usedSave_ == usedReported_, \
        "Mismatched bytes during Save: saved = %d, required = %d\n", \
        usedSave_, usedReported_)
#else
#define PX2_END_DEBUG_STREAM_SAVE(classname,target)
#endif
//----------------------------------------------------------------------------
// PX2_POINTERSIZE���ڳ־û�ϵͳ��ʹ�õģ����ָ���С�ĺꡣָ�뱻����һ��4byte
// ��unsigned intд�뵽Ӳ���С�
#define PX2_POINTERSIZE(value) 4
//----------------------------------------------------------------------------
// PX2_BOOLSIZE���ڳ־û�ϵͳ��ʹ�õģ����bool���ʹ�С�ĺꡣһ��boolֵ������4
// byte��Сд�뵽Ӳ�̡�
#define PX2_BOOLSIZE(value) 4
//----------------------------------------------------------------------------
// PX2_ENUMSIZE���ڳ־û�ϵͳ��ʹ�ã�һ��ö�����ͱ�����4byte��Сд�뵽Ӳ�̡�
#define PX2_ENUMSIZE(value) 4
//----------------------------------------------------------------------------
#define PX2_STRINGSIZE(value) Stream::GetStreamingSize(value)
//----------------------------------------------------------------------------

#endif