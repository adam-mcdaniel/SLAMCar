// PX2HPoint.hpp

#ifndef PX2HPOINT_HPP
#define PX2HPOINT_HPP

#include "PX2MathematicsPre.hpp"

namespace PX2
{

	/// ��/������
	/**
	* ��/��������һ����������µĵ�(x,y,z,w)���������һ���㣬��w = 1����APoint��
	* ���w = 0����һ����������AVector��
	*/
	class PX2_ENGINE_ITEM HPoint
	{
	public:
		HPoint ();  //< δ��ʼ��
		HPoint (const HPoint& pnt);
		HPoint (float x, float y, float z, float w);
		~HPoint (); 

		// ��Ա����
		inline operator const float* () const;
		inline operator float* ();
		inline float operator[] (int i) const;
		inline float& operator[] (int i);
		inline float X () const;
		inline float& X ();
		inline float Y () const;
		inline float& Y ();
		inline float Z () const;
		inline float& Z ();
		inline float W () const;
		inline float& W ();

		// ��ֵ
		HPoint& operator= (const HPoint& pnt);

		// �Ƚ�
		bool operator== (const HPoint& pnt) const;
		bool operator!= (const HPoint& pnt) const;
		bool operator<  (const HPoint& pnt) const;
		bool operator<= (const HPoint& pnt) const;
		bool operator>  (const HPoint& pnt) const;
		bool operator>= (const HPoint& pnt) const;

	protected:
		float mTuple[4];
	};

#include "PX2HPoint.inl"

}

#endif