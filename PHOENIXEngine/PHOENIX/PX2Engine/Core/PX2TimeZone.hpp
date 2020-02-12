// PX2TimeZone.hpp

#ifndef PX2TIMEZONE_HPP
#define PX2TIMEZONE_HPP

#include "PX2CorePre.hpp"
#include "PX2Timestamp.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM TimeZone
	{
	public:
		static int UTCOffset();

		/// ��ʱ�ƣ�Daylight Saving Time��DST��
		/**
		* �ֳơ��չ��Լʱ�ơ��͡�����ʱ�䡱����һ��Ϊ��Լ��Դ����Ϊ�涨�ط�ʱ
		* ����ƶȣ�����һ�ƶ�ʵ���ڼ������õ�ͳһʱ���Ϊ������ʱ�䡱��һ����
		* ��������ļ���Ϊ��ʱ����ǰһСʱ������ʹ��������˯���������������Գ�
		* �����ù�����Դ���Ӷ���Լ�����õ硣����������ʱ�ƵĹ��Ҿ���涨��ͬ��
		* Ŀǰȫ�����н�110������ÿ��Ҫʵ������ʱ����2011��3��27�տ�ʼ����˹��
		* ��ʹ������ʱ����ʱ�䲦��һСʱ�����ٵ��ء�
		* local time = UTC + UTCOffset() + Dst()
		*/		
		static int Dst();
		static bool IsDst(const Timestamp& timestamp);

		/// UTCOffset() + Dst().
		static int TZD();
	};

}

#endif