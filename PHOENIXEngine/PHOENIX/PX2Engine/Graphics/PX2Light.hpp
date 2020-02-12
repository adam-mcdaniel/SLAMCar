// PX2Light.hpp

#ifndef PX2LIGHT_HPP
#define PX2LIGHT_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Object.hpp"
#include "PX2Float4.hpp"
#include "PX2APoint.hpp"

namespace PX2
{
	
	/// �ƹ���
	class PX2_ENGINE_ITEM Light : public Object
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(Light);

	public:
		enum Type
		{
			LT_AMBIENT,
			LT_DIRECTIONAL,
			LT_POINT,
			LT_SPOT,
			LT_QUANTITY
		};

		Light (Type eType = LT_AMBIENT);
		virtual ~Light ();

		// ���õƹ����ͣ�ȱʡ������LT_AMBIENT
		inline void SetType (Type type);
		inline Type GetType () const;

		// �ƹ���ɫ
		Float4 Ambient;   //< default: (0,0,0,1)
		Float4 Diffuse;   //< default: (0,0,0,1)
		Float4 Specular;  //< default: (0,0,0,1)'

		float Range;

		// ˥��ͨ���ĵ���ģʽΪ
		// m = 1/(C + L*d + Q*d*d)
		// C�ǳ���ϵ����L������һ��ϵ����Q�Ƕ���ϵ����d�ǴӴ˶��㵽��Դ�ľ��롣
		// Ϊ�˶Եƹ��ǿ�ȿɵ����������ģʽ���
		// m = I/(C + L*d + Q*d*d)
		// I�������ǿ��ϵ����
		float Constant;   //< default: 1
		float Linear;     //< default: 0
		float Quadratic;  //< default: 0
		float Intensity;  //< default: 1

		// �۹�Ʋ�����׶�νǱ������Ի����Ƽ���ġ��Ƕȱ�������0 < Angle <= pi��
		float Angle;     //< default: pi
		float CosAngle;  //< default: -1
		float SinAngle;  //< default:  0
		float Exponent;  //< default:  1

		/**
		* һ���������������������Angle���ڲ���Ϊ������CosAngle��SinAngle��
		* ׶�νǱ������Ի����Ƽ���ġ�
		*/
		void SetAngle (float angle);

		// ���ܱ�׼�ķ����͵��֮��Ҫdirection����������������ʹ��ȫ����ϵ
		// �ƹ������ϵ��������������ϵ�еġ�
		// {D,U,R}���������������ϵ��ÿ�������ǵ�λ���ȡ��������뻥��˴˴�ֱ��
		// ���� R = Cross(D,U)��
		APoint Position; //< default: (0,0,0)
		AVector DVector; //< default: (0,0,-1)
		AVector UVector; //< default: (0,1,0)
		AVector RVector; //< default: (1,0,0)

		/**
		* һ���������������������direction�������ڲ���Ϊ������up��right������
		*/
		void SetDirection (const AVector& direction);

	protected:
		Type mType;
	};

	PX2_REGISTER_STREAM(Light);
	typedef Pointer0<Light> LightPtr;
#include "PX2Light.inl"

}

#endif