// PX2InitTerm.hpp

#ifndef PX2INITTERM_HPP
#define PX2INITTERM_HPP

#include "PX2CorePre.hpp"

namespace PX2
{
	
	/// ��ʼ��������
	/**	
	* ��Ӧ�ó����������ʱ�򴴽���ҪԤ�ȴ����Ķ�����Ӧ�ó��������ʱ���ͷ�Ԥ
	* �ȴ����Ķ�������������Ŀ����Ϊ�˱�����main��������֧�ֽ��ж�̬���ڴ�
	* ���䣬�������Ը������׵ع���͸����ڴ�й©��
	*/
	class PX2_ENGINE_ITEM InitTerm
	{
	public:
		typedef void (*Initializer)(void);
		static void AddInitializer (Initializer function);
		static void ExecuteInitializers ();

		typedef void (*Terminator)(void);
		static void AddTerminator (Terminator function);
		static void ExecuteTerminators ();

	private:
		/** 
		* Ϊ��֧�����Ӧ�ó��������ֵ�����㹻�󡣵����Ӧ�ó���ʹ�õĳ�ʼ��
		* ��Ϊ�������ֵ�ǣ�һ�����Իᴥ��������
		*/
		enum
		{
			MAX_ELEMENTS = 1024
		};

		static int msNumInitializers;
		static Initializer msInitializers[MAX_ELEMENTS];

		static int msNumTerminators;
		static Terminator msTerminators[MAX_ELEMENTS];
	};

}

//----------------------------------------------------------------------------
#define PX2_DECLARE_INITIALIZE \
public: \
	static bool RegisterInitialize (); \
	static void Initialize (); \
private: \
	static bool msInitializeRegistered
//----------------------------------------------------------------------------
#define PX2_IMPLEMENT_INITIALIZE(classname) \
	bool classname::msInitializeRegistered = false; \
	bool classname::RegisterInitialize () \
	{ \
		if (!msInitializeRegistered) \
		{ \
			InitTerm::AddInitializer(classname::Initialize); \
			msInitializeRegistered = true; \
		} \
		return msInitializeRegistered; \
	}
//----------------------------------------------------------------------------
#define PX2_REGISTER_INITIALIZE(classname) \
	static bool gsInitializeRegistered_##classname = \
	classname::RegisterInitialize ()
//----------------------------------------------------------------------------
#define PX2_DECLARE_TERMINATE \
public: \
	static bool RegisterTerminate (); \
	static void Terminate (); \
private: \
	static bool msTerminateRegistered
//----------------------------------------------------------------------------
#define PX2_IMPLEMENT_TERMINATE(classname) \
	bool classname::msTerminateRegistered = false; \
	bool classname::RegisterTerminate () \
	{ \
		if (!msTerminateRegistered) \
		{ \
			InitTerm::AddTerminator(classname::Terminate); \
			msTerminateRegistered = true; \
		} \
		return msTerminateRegistered; \
	}
//----------------------------------------------------------------------------
#define PX2_REGISTER_TERMINATE(classname) \
	static bool gsTerminateRegistered_##classname = \
	classname::RegisterTerminate ()
//----------------------------------------------------------------------------

#endif
