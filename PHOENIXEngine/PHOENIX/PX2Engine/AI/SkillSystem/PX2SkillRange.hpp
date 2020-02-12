// PX2SkillRange.hpp

#ifndef PX2SKILLRANGE_HPP
#define PX2SKILLRANGE_HPP

#include "PX2SkillSysPre.hpp"
#include "PX2SmartPointer.hpp"

namespace PX2
{

	class SKILLSYS_DLL_ITEM SkillRange
	{
	public:
		SkillRange();
		virtual ~SkillRange();

		enum D23Type
		{
			DT_2D,
			DT_3D,
			DT_MAX_TYPE
		};
		D23Type TheD23Type;

		// ��Χ����
		// SkillDirection RT_CIRCLE ��RadiusLength��Degree �������һ��Բ��������
		// SkillDirection RT_RECT ��RadiusLength, Width �������һ������
		enum RangeType
		{
			RT_CIRCLE,
			RT_RECT,
			RT_MAX_TYPE
		};
		RangeType TheRangeType;

		float RadiusLength;
		float Degree;
		float Width;
	};
	typedef Pointer0<SkillRange> SkillRangePtr;

}

#endif