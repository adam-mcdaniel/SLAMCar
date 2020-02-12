// PX2ShaderFloat.hpp

#ifndef PX2SHADERFLOAT_HPP
#define PX2SHADERFLOAT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2ShaderStruct.hpp"

namespace PX2
{
	
	/// ��ɫ�������Ĵ�����
	/**
	* ������1���Ĵ�����ÿ����������4������ֵ��
	*/
	class PX2_ENGINE_ITEM ShaderFloat : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(ShaderFloat);

	public:
		ShaderFloat ();
		/// ��ȱʡ���캯��
		/**
		* ����ļĴ�����������Ǹ���ÿһ���Ĵ�������4��floating-pointֵ��
		*/
		ShaderFloat (int numRegisters);
		virtual ~ShaderFloat ();

		// �����������Ĵ�������
		void SetNumRegisters (int numRegisters);

		// ��Ա����
		inline int GetNumRegisters () const;
		inline const float* GetData () const;
		inline float* GetData ();

		//���üĴ���ֵ
		/*
		* SetRegister��data����4��float��register i��
		* SetRegisters��data����mNumElements��float��register��
		* Get*�����෴��
		*/
		void SetRegister (int i, const float* data);
		void SetRegisters (const float* data);
		void GetRegister (int i, float* data);
		float *GetRegister(int i);
		void GetRegisters (float* data);

		/// ���üĴ���ֵ
		/**
		* ��SetRegisters����˼��һ���ġ���һ���Ĵ����ṹ�����˵�'const float*'
		* ����ʽת���������ֵ�����ͷǳ������ˡ�
		*/
		ShaderFloat& operator= (const float* data);

		// ͨ��������������
		inline float operator[] (int i) const;
		inline float& operator[] (int i);

		// �������������ɫ�������Ķ�̬���¡������ڸ�������������ɫ������Ҳ
		// ��Ҫ�ı䡣
		inline void EnableUpdater ();
		inline void DisableUpdater ();
		inline bool AllowUpdater () const;
		virtual void Update (const ShaderStruct *struc);

	protected:
		int mNumElements;
		float* mData;
		bool mAllowUpdater;
	};

	PX2_REGISTER_STREAM(ShaderFloat);
	typedef Pointer0<ShaderFloat> ShaderFloatPtr;
#include "PX2ShaderFloat.inl"

}

#endif
