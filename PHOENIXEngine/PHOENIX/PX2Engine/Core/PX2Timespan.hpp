// PX2Timespan.hpp

#ifndef PX2TIMESPAN_HPP
#define PX2TIMESPAN_HPP

#include "PX2CorePre.hpp"
#include "PX2Timestamp.hpp"

namespace PX2
{

	/**
	* ��ʱ��ת��Ϊ΢��(һ�����֮һ��)������ʱ��Ŀ��
	*/
	class PX2_ENGINE_ITEM Timespan
	{
	public:
		typedef Timestamp::TimeDiff TimeDiff;

		Timespan ();
		Timespan (TimeDiff microseconds);
		Timespan (long seconds, long microseconds);
		Timespan (int days, int hours, int minutes, int seconds,
			int microseconds);
		Timespan (const Timespan& timespan);
		~Timespan ();

		Timespan& operator = (const Timespan& timespan);
		Timespan& operator = (TimeDiff microseconds);
		Timespan& Assign(int days, int hours, int minutes, int seconds,
			int microseconds);
		Timespan& Assign(long seconds, long microseconds);
		void Swap(Timespan& timespan);

		bool operator == (const Timespan& ts) const;
		bool operator != (const Timespan& ts) const;
		bool operator >  (const Timespan& ts) const;
		bool operator >= (const Timespan& ts) const;
		bool operator <  (const Timespan& ts) const;
		bool operator <= (const Timespan& ts) const;

		bool operator == (TimeDiff microseconds) const;
		bool operator != (TimeDiff microseconds) const;
		bool operator >  (TimeDiff microseconds) const;
		bool operator >= (TimeDiff microseconds) const;
		bool operator <  (TimeDiff microseconds) const;
		bool operator <= (TimeDiff microseconds) const;

		Timespan operator + (const Timespan& d) const;
		Timespan operator - (const Timespan& d) const;
		Timespan& operator += (const Timespan& d);
		Timespan& operator -= (const Timespan& d);

		Timespan operator + (TimeDiff microseconds) const;
		Timespan operator - (TimeDiff microseconds) const;
		Timespan& operator += (TimeDiff microseconds);
		Timespan& operator -= (TimeDiff microseconds);

		int Days() const;

		int Hours() const; //< 0-23
		int TotalHours() const;

		int Minutes() const; //< 0-59
		int TotalMinutes() const;

		int Seconds() const; //< 0-59
		int TotalSeconds() const;

		int Milliseconds() const; //< 0-999
		TimeDiff TotalMilliseconds() const;

		int Microseconds() const; //< 0-999
		int Useconds() const; //< 0-999999
		TimeDiff TotalMicroseconds() const;

		static const TimeDiff MILLISECONDS; //< 1����Ϊ MILLISECONDS��΢����
		static const TimeDiff SECONDS;      //< 1��Ϊ	SECONDS		��΢����
		static const TimeDiff MINUTES;      //< 1����Ϊ MINUTES		��΢����
		static const TimeDiff HOURS;        //< 1СʱΪ HOURS		��΢����
		static const TimeDiff DAYS;         //< 1��Ϊ	DAYS		��΢����

	private:
		TimeDiff mSpan;
	};

	inline void Swap(Timespan& s1, Timespan& s2)
	{
		s1.Swap(s2);
	}

#include "PX2Timespan.inl"

}

#endif