// PX2Bound.hpp

#ifndef PX2BOUND_HPP
#define PX2BOUND_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2HPlane.hpp"
#include "PX2Transform.hpp"

namespace PX2
{
	
	/// ��Χ����
	/**
	* ÿ���������󣬽ڵ㶼ӵ��һ���������Χ��������Χ�壬�����Χ����ͨ����
	* ���Χ��任��������ֱ�����á���Χ��ֻҪ��������ײ��⣬����ü��ȡ�����
	* ʵ�ֵİ�Χ����һ����Χ����Ϊ��ͨ����˵�����ġ�
	*/
	class PX2_ENGINE_ITEM Bound
	{
	public:
		Bound ();
		~Bound ();

		// ��ֵ
		Bound& operator= (const Bound& bound);

		// �κα��붨�����ĺͰ뾶
		inline void SetCenter (const APoint& center);
		inline void SetRadius (float radius);
		inline const APoint& GetCenter () const;
		inline float GetRadius () const;

		// ��Χ�����
		int WhichSide (const HPlane& plane) const;
		void GrowToContain (const Bound& bound);
		void TransformBy (const Transform& transform, Bound& bound);
		void ComputeFromData (int numElements, int stride, const char* data);

		/// �ߺͰ�Χ����ײ���
		/**
		* �ȵı�����ʽΪP + t*D��P��ԭ�㣬D�ǵ�λ���ȵķ�������������[tmin,tmax]
		* ȡֵ���壺
		*	line:     tmin = -Mathf::MAX_REAL, tmax = Mathf::MAX_REAL
		*	ray:      tmin = 0.0f, tmax = Mathf::MAX_REAL
		*	segment:  tmin >= 0.0f, tmax > tmin
		*/
		bool TestIntersection (const APoint& origin, const AVector& direction,
			float tmin, float tmax) const;

		/// ������̬��Χ����ײ���
		bool TestIntersection (const Bound& bound) const;

		/// �����ƶ���Χ�е��ཻ���
		/**
		* Velocity0�ǵ��õİ�Χ�е��ٶȣ�velocity1������İ�Χ�е��ٶȡ�
		*/
		bool TestIntersection (const Bound& bound, float tmax,
			const AVector& velocity0, const AVector& velocity1) const;

		// �־û�֧��
		inline static int GetStreamingSize ();

	private:
		APoint mCenter;
		float mRadius;
	};

#include "PX2Bound.inl"

}

#endif
