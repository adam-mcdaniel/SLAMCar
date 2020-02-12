// PX2BitHacks.hpp

#ifndef PX2BITHACKS_HPP
#define PX2BITHACKS_HPP

#include "PX2MathematicsPre.hpp"

namespace PX2
{

	PX2_ENGINE_ITEM bool IsPowerOfTwo(unsigned int value);
	PX2_ENGINE_ITEM bool IsPowerOfTwo(int value);

	PX2_ENGINE_ITEM unsigned int Log2OfPowerOfTwo(unsigned int powerOfTwo);
	PX2_ENGINE_ITEM int Log2OfPowerOfTwo(int powerOfTwo);

	/// ���ٽ�������[0,1]��Χ�ڵ�32bit�ĸ�����F��ת����������[0,2^P-1]��32bit��
	// ����I��value��ʾF��power��ʾP������ֵ��ʾI��
	inline int ScaledFloatToInt (float value, int power);

#include "PX2BitHacks.inl"

}

#endif
