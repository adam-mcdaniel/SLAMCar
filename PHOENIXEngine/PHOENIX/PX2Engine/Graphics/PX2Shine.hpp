// PX2Shine.hpp

#ifndef PX2SHINE_HPP
#define PX2SHINE_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2Float4.hpp"
#include "PX2Float3.hpp"

namespace PX2
{
	
	/// ����״̬��
	/**
	* ��¼���������Ĺ������ԡ�
	*/
	class PX2_ENGINE_ITEM Shine : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_PROPERTY;
		PX2_DECLARE_STREAM(Shine);

	public:
		Shine ();
		virtual ~Shine ();

		Float4 Emissive;  //< default: (0,0,0,1)
		Float4 Ambient;   //< default: (1,1,1,1)

		/// ͸���ȱ�����Diffuse��alphaͨ����
		Float4 Diffuse;   //< default: (0,0,0,1)

		/// ���ʵľ��淴�����������mSpecular��alphaͨ����
		Float4 Specular;  //< default: (0,0,0,0)

		void ReCalTemp ();

	protected:
		Float3 mEmissiveTemp;
		float mEmissiveAlphaTemp;
		Float3 mAmbientTemp;
		float mAmbientAlphaTemp;
		Float3 mDiffuseTemp;
		float mDiffuseAlphaTemp;
		Float3 mSpecularTemp;
		float mSpecularExponentTemp;
	};

	PX2_REGISTER_STREAM(Shine);
	typedef Pointer0<Shine> ShinePtr;

}

#endif