// PX2DateTime.hpp

#ifndef PX2DATETIME_HPP
#define PX2DATETIME_HPP

#include "PX2CorePre.hpp"
#include "PX2Timestamp.hpp"
#include "PX2Timespan.hpp"

namespace PX2
{

	// Coordinated Universal Time��UTC���������׼ʱ�� 
	// ���Ǵ������֪�ĸ������α�׼ʱ�䣨Greenwich Mean Time��GMT�������磬�й�
	// �ڵص�ʱ����UTC��ʱ��Ϊ+8��Ҳ����UTC+8��������UTC-5��
	
	// Calendar Time������ʱ�� 
	// �ǡ���һ����׼ʱ��㵽��ʱ��ʱ�侭���� (����)������ʾ��ʱ�䡣
	// �����磺1970��1��1��0ʱ0��0��)�������׼ʱ���Բ�ͬ�ı�������˵��������
	// ͬ������һ������ϵͳ��˵�������׼ʱ����ǲ���ģ��ñ���ϵͳ�е�ʱ���Ӧ
	// ������ʱ�䶼ͨ���ñ�׼ʱ��������������Կ���˵����ʱ���ǡ����ʱ�䡱����
	// ������������һ��ʱ������ͬһʱ�̶�ͬһ����׼ʱ�����˵������ʱ�䶼��һ��
	// �ġ� 
	
	// Epoch��ʱ��� 
	// ʱ����ڱ�׼C/C++����һ�����������ô�ʱ��ʱ��ͱ�׼ʱ���������������
	// ����ʱ�䣩����ʾ��

	class PX2_ENGINE_ITEM DateTime
	{
	public:
		enum Months
		{
			JANUARY = 1,
			FEBRUARY,
			MARCH,
			APRIL,
			MAY,
			JUNE,
			JULY,
			AUGUST,
			SEPTEMBER,
			OCTOBER,
			NOVEMBER,
			DECEMBER
		};

		enum DaysOfWeek
		{
			SUNDAY = 0,
			MONDAY,
			TUESDAY,
			WEDNESDAY,
			THURSDAY,
			FRIDAY,
			SATURDAY
		};

		DateTime();
		DateTime(const Timestamp& timestamp);
		DateTime(int year, int month, int day, int hour = 0, int minute = 0,
			int second = 0, int millisecond = 0, int microsecond = 0);
		DateTime(double julianDay);
		DateTime(Timestamp::UtcTimeVal UTCTime, Timestamp::TimeDiff diff);
		DateTime(const DateTime& dateTime);
		~DateTime();

		DateTime& operator = (const DateTime& dateTime);
		DateTime& operator = (const Timestamp& timestamp);
		DateTime& operator = (double julianDay);
		DateTime& Assign(int year, int month, int day, int hour = 0, int minute = 0, 
			int second = 0, int millisecond = 0, int microseconds = 0);

		void Swap(DateTime& dateTime);

		int Year() const;
		int Month() const; //< 1-12
		int Week(int firstDayOfWeek = MONDAY) const;
		int Day() const; //< 1-31
		int DayOfWeek() const; //< 0-6 (0 = Sunday, 1 = Monday, ..., 6 = Saturday)

		/// ������һ�굱�еĵڼ���
		/**
		* ���磬1��1���ǵ�һ�죬2��1���ǵ�32��
		*/
		int DayOfYear() const;

		int Hour() const; //< 0-23

		int HourAMPM() const; //< 0-12

		bool IsAM() const; //< hour < 12;

		bool IsPM() const; //< hour >= 12

		int Minute() const; //< 0-59

		int Second() const; // 0-59

		int Millisecond() const; // 0-999

		int Microsecond() const; // 0-999

		double JulianDay() const;
		/// Returns the julian day for the date and time.

		Timestamp GetTimestamp() const;
		/// Returns the date and time expressed as a Timestamp.

		Timestamp::UtcTimeVal UTCTime() const;
		/// Returns the date and time expressed in UTC-based
		/// time. UTC base time is midnight, October 15, 1582.
		/// Resolution is 100 nanoseconds.

		bool operator == (const DateTime& dateTime) const;	
		bool operator != (const DateTime& dateTime) const;	
		bool operator <  (const DateTime& dateTime) const;	
		bool operator <= (const DateTime& dateTime) const;	
		bool operator >  (const DateTime& dateTime) const;	
		bool operator >= (const DateTime& dateTime) const;	

		DateTime  operator +  (const Timespan& span) const;
		DateTime  operator -  (const Timespan& span) const;
		Timespan  operator -  (const DateTime& dateTime) const;
		DateTime& operator += (const Timespan& span);
		DateTime& operator -= (const Timespan& span);

		void MakeUTC (int tzd);
		/// Converts a local time into UTC, by applying the given time zone differential.

		void MakeLocal (int tzd);
		/// Converts a UTC time into a local time, by applying the given time zone differential.

		static bool IsLeapYear (int year);
		/// Returns true if the given year is a leap year;
		/// false otherwise.

		static int DaysOfMonth(int year, int month);
		/// Returns the number of days in the given month
		/// and year. Month is from 1 to 12.

		static bool IsValid(int year, int month, int day, int hour = 0, 
			int minute = 0, int second = 0, int millisecond = 0, int microsecond = 0);
		/// Checks if the given date and time is valid
		/// (all arguments are within a proper range).
		///
		/// Returns true if all arguments are valid, false otherwise.

	protected:	
		static double ToJulianDay(Timestamp::UtcTimeVal UTCTime);
		static double ToJulianDay(int year, int month, int day, int hour = 0,
			int minute = 0, int second = 0, int millisecond = 0,
			int microsecond = 0);
		static Timestamp::UtcTimeVal ToUtcTime(double julianDay);

		void ComputeGregorian(double julianDay);

		void ComputeDaytime();
		/// Extracts the daytime (hours, minutes, seconds, etc.) from the stored UTCTime.

	private:
		void CheckLimit(int& lower, int& higher, int limit);
		void Normalize();
		///utility functions used to correct the overflow in computeGregorian

		Timestamp::UtcTimeVal mUtcTime;
		int  mYear;
		int  mMonth;
		int  mDay;
		int  mHour;
		int  mMinute;
		int  mSecond;
		int  mMillisecond;
		int  mMicrosecond;
	};
	
	inline void Swap(DateTime& d1, DateTime& d2)
	{
		d1.Swap(d2);
	}

#include "PX2DateTime.inl"

}

#endif