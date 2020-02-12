// PX2Rtti.hpp

#ifndef PX2RTTI_HPP
#define PX2RTTI_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	/// ����ʶ�������
	/**
	* ��ϵͳ���е�Object�У�acName���Ʊ��벻ͬ����PX2�����ֿռ��У�һ������Poo��
	* �������ΪPX2.Poo�����Ӧ�ó��������������ֿռ䣬��SomeName����ô�������
	* Ϊ"SomeName.Poo"��
	*/
	class PX2_ENGINE_ITEM Rtti
	{
	public:
		Rtti (const char* name, unsigned short version, const Rtti* baseType);
		~Rtti ();

		inline const char* GetName () const;
		inline unsigned short GetVersion () const;
		inline bool IsExactly (const Rtti& type) const;
		bool IsDerived (const Rtti& type) const;

	private:
		const char* mName;
		unsigned short mVersion;
		const Rtti* mBaseType;
	};

#include "PX2Rtti.inl"

}

//----------------------------------------------------------------------------
#define PX2_DECLARE_RTTI \
public: \
	static const Rtti TYPE; \
	\
	virtual const Rtti& GetRttiType () const \
	{ \
		return TYPE; \
	} \
	virtual unsigned short GetRttiVersion () const \
	{ \
		return GetRttiType().GetVersion(); \
	}
//----------------------------------------------------------------------------
#define PX2_IMPLEMENT_RTTI(nsname, baseclassname, classname) \
	const Rtti classname::TYPE(#nsname"."#classname, 0, &baseclassname::TYPE)
//----------------------------------------------------------------------------
#define PX2_IMPLEMENT_RTTI_V(nsname, baseclassname, classname, version) \
	const Rtti classname::TYPE(#nsname"."#classname, version, &baseclassname::TYPE)
//----------------------------------------------------------------------------

#endif
