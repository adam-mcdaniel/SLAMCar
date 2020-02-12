// PX2SmartPointer.hpp

#ifndef PX2SMARTPOINTER_HPP
#define PX2SMARTPOINTER_HPP

#include "PX2CorePre.hpp"
#include "PX2Memory.hpp"

namespace PX2
{

	//----------------------------------------------------------------------------
	class PointerBase
	{
	protected:
		// map��key(void*)�ǹ������ĵ�ַ��map��value(int)�ǹ�������������
		typedef std::map<void*,int> ReferenceMap;
		typedef ReferenceMap::iterator RMIterator;
		typedef ReferenceMap::const_iterator RMCIterator;

		PX2_ENGINE_ITEM static ReferenceMap msMap;
		PX2_ENGINE_ITEM static Mutex msMutex;
	};

	//----------------------------------------------------------------------------
	template <typename T>
	class Pointer0 : public PointerBase
	{
	public:
		Pointer0 (T* data = 0);
		Pointer0 (const Pointer0& pointer);
		~Pointer0 ();

		int GetCount () const;

		// ��ʽת��
		inline operator T* () const;
		inline T& operator* () const;
		inline T* operator-> () const;

		// ��ֵ
		Pointer0& operator= (T* data);
		Pointer0& operator= (const Pointer0& pointer);

		// �Ƚ�
		inline bool operator== (T* data) const;
		inline bool operator!= (T* data) const;
		inline bool operator== (const Pointer0& pointer) const;
		inline bool operator!= (const Pointer0& pointer) const;

	protected:
		T* mData;
	};

	//----------------------------------------------------------------------------
	template <typename T>
	class Pointer1 : public PointerBase
	{
	public:
		Pointer1 (T* data = 0);
		Pointer1 (const Pointer1& pointer);
		~Pointer1 ();

		int GetCount () const;

		// ��ʽת��
		inline operator T* () const;
		inline T& operator* () const;
		inline T* operator-> () const;

		// ��ֵ
		Pointer1& operator= (T* data);
		Pointer1& operator= (const Pointer1& pointer);

		// �Ƚ�
		inline bool operator== (T* data) const;
		inline bool operator!= (T* data) const;
		inline bool operator== (const Pointer1& pointer) const;
		inline bool operator!= (const Pointer1& pointer) const;

	protected:
		T* mData;
	};

	//----------------------------------------------------------------------------
	template <typename T>
	class Pointer2 : public PointerBase
	{
	public:
		Pointer2 (T** data = 0);
		Pointer2 (const Pointer2& pointer);
		~Pointer2 ();

		int GetCount () const;

		// ��ʽת��
		inline operator T** () const;
		inline T*& operator* () const;

		// ��ֵ
		Pointer2& operator= (T** data);
		Pointer2& operator= (const Pointer2& pointer);

		// �Ƚ�
		inline bool operator== (T** data) const;
		inline bool operator!= (T** data) const;
		inline bool operator== (const Pointer2& pointer) const;
		inline bool operator!= (const Pointer2& pointer) const;

	protected:
		T** mData;
	};

	//----------------------------------------------------------------------------
	template <typename T>
	class Pointer3 : public PointerBase
	{
	public:
		Pointer3 (T*** data = 0);
		Pointer3 (const Pointer3& pointer);
		~Pointer3 ();

		int GetCount () const;

		// ��ʽת��
		inline operator T*** () const;
		inline T**& operator* () const;

		// ��ֵ
		Pointer3& operator= (T*** data);
		Pointer3& operator= (const Pointer3& pointer);

		// ��ֵ
		inline bool operator== (T*** data) const;
		inline bool operator!= (T*** data) const;
		inline bool operator== (const Pointer3& pointer) const;
		inline bool operator!= (const Pointer3& pointer) const;

	protected:
		T*** mData;
	};

	//----------------------------------------------------------------------------
	template <typename T>
	class Pointer4 : public PointerBase
	{
	public:
		Pointer4 (T**** data = 0);
		Pointer4 (const Pointer4& pointer);
		~Pointer4 ();

		int GetCount () const;

		// ��ʽת��
		inline operator T**** () const;
		inline T***& operator* () const;

		// ��ֵ
		Pointer4& operator= (T**** data);
		Pointer4& operator= (const Pointer4& pointer);

		// �Ƚ�
		inline bool operator== (T**** data) const;
		inline bool operator!= (T**** data) const;
		inline bool operator== (const Pointer4& pointer) const;
		inline bool operator!= (const Pointer4& pointer) const;

	protected:
		T**** mData;
	};

	//----------------------------------------------------------------------------

#include "PX2SmartPointer.inl"

}

#endif