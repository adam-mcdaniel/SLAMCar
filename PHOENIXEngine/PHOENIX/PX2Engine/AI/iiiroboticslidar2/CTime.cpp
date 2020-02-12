/**********************************************************************************
File name:	  CTime.cpp
Author:       Kimbo
Version:      V1.5.0
Date:	 	  2016-4-25
Description:  Time class
Others:       None

History:
	1. Date:
	Author:
	Modification:
***********************************************************************************/

/********************************** File includes *********************************/
#include "CTime.h"
#include "CLogUtitls.h"
#include <assert.h>

/********************************** System libs includes **************************/
#if defined(_WIN32) || defined(WIN32)
#include <Windows.h>
#endif

#if defined __APPLE__
#include <sys/time.h>
#elif defined __LINUX__
#include <time.h>
#include <sys/time.h>
#include <sys/timeb.h>
#else
#include <time.h>
#include <sys/timeb.h>
#endif

/********************************** Other libs includes ***************************/

/*********************************** Name space ***********************************/
using namespace everest;
using namespace everest::hwdrivers;

/***********************************************************************************
Function:     CTime
Description:  The constructor of file path class
Input:        None
Output:       None
Return:       None
Others:       None
***********************************************************************************/
CTime::CTime()
{

}

/***********************************************************************************
Function:     ~CTime
Description:  The destructor of file path class
Input:        None
Output:       None
Return:       None
Others:       None
***********************************************************************************/
CTime::~CTime()
{

}

/***********************************************************************************
Function:     Add time
Description:
Input:        None
Output:       None
Return:       None
Others:       None
***********************************************************************************/
TTimeStamp2 CTime::addTime(TTimeStamp2 time, double time_ms)
{
    assert(time != INVALID_TIMESTAMP && time_ms > 0);

    return int64_t(time + (int64_t)(time_ms / 1000.0 * 10000000.0));
}


/***********************************************************************************
Function:     getRealTime
Description:  Get real time
Input:        None
Output:       None
Return:       None
Others:       None
***********************************************************************************/
TTimeStamp2 CTime::getRealTime()
{
 //   timespec  tim;
 //   clock_gettime(CLOCK_REALTIME, &tim);
	//return time_tToTimestamp( tim.tv_sec ) + tim.tv_nsec/100;

	TTimeStamp2 timeVal;

#if defined(_WIN32) || defined(WIN32)
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);

	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;

	ULARGE_INTEGER ts;
	ts.LowPart = ft.dwLowDateTime;
	ts.HighPart = ft.dwHighDateTime;
	ts.QuadPart -= epoch.QuadPart;
	timeVal = ts.QuadPart;
#else
	struct timeval tv;
	gettimeofday(&tv, 0);
	timeVal = (int64_t)(tv.tv_sec)*10000000 + tv.tv_usec;
#endif

	timeVal += ((uint64_t)116444736 * 1000000000);

	return timeVal;
}
/***********************************************************************************
Function:     getCpuTime
Description:  Get real time
Input:        None
Output:       None
Return:       None
Others:       None
***********************************************************************************/
TTimeStamp2 CTime::getCpuTime()
{
 //   timespec  tim;
 //   clock_gettime(CLOCK_MONOTONIC, &tim);
	//return time_tToTimestamp( tim.tv_sec ) + tim.tv_nsec/100;

	TTimeStamp2 timeVal;

#if defined(_WIN32) || defined(WIN32)
	FILETIME ft;
	GetSystemTimeAsFileTime(&ft);

	ULARGE_INTEGER epoch; // UNIX epoch (1970-01-01 00:00:00) expressed in Windows NT FILETIME
	epoch.LowPart = 0xD53E8000;
	epoch.HighPart = 0x019DB1DE;

	ULARGE_INTEGER ts;
	ts.LowPart = ft.dwLowDateTime;
	ts.HighPart = ft.dwHighDateTime;
	ts.QuadPart -= epoch.QuadPart;
	timeVal = ts.QuadPart;
#else
	struct timeval tv;
	gettimeofday(&tv, 0);
	timeVal = (int64_t)(tv.tv_sec)*10000000 + tv.tv_usec;
#endif

	return timeVal;
}

/***********************************************************************************
Function:     time_tToTimestamp
Description:  time_t to Timestamp
Input:        None
Output:       None
Return:       None
Others:       None
***********************************************************************************/
TTimeStamp2 CTime::time_tToTimestamp(const time_t &t)
{
    return (((uint64_t)t) * (uint64_t)10000000) + ((uint64_t)116444736*1000000000);
}

/***********************************************************************************
Function:     timeDifference
Description:  get time difference
Input:        None
Output:       None
Return:       None
Others:       None
***********************************************************************************/
double CTime::timeDifference( const TTimeStamp2 &t1, const TTimeStamp2 &t2 )
{
	assert(t1!=INVALID_TIMESTAMP);
	assert(t2!=INVALID_TIMESTAMP);

	return ((double)((int64_t)(t2-t1)))/10000000.0;
}

/***********************************************************************************
Function:     getTimeString
Description:  Get time string
Input:        None
Output:       None
Return:       None
Others:       None
***********************************************************************************/
std::string CTime::getTimeString()
{
    TTimeParts parts;
    timestampToParts(CTime::getRealTime(), parts, true);
    return format("%04u%02u%02u-%02u%02u%02u",
        (unsigned int)parts.year,
        (unsigned int)parts.month,
        (unsigned int)parts.day,
        (unsigned int)parts.hour,
        (unsigned int)parts.minute,
        (unsigned int)parts.second );
}

/*---------------------------------------------------------------
					timestampToParts
  ---------------------------------------------------------------*/
void CTime::timestampToParts( TTimeStamp2 t, TTimeParts &p , bool localTime)
{
	double T = timestampTotime_t(t);
	time_t tt = time_t(T);

	double sec_frac = T - tt;

	struct tm * parts =  localTime ? localtime(&tt) : gmtime(&tt);

	p.year		= parts->tm_year + 1900;
	p.month		= parts->tm_mon + 1;
	p.day		= parts->tm_mday;
	p.day_of_week = parts->tm_wday + 1;
	p.daylight_saving = parts->tm_isdst;
	p.hour		= parts->tm_hour;
	p.minute	= parts->tm_min;
	p.second	= parts->tm_sec + sec_frac;
}

/*---------------------------------------------------------------
					timestampTotime_t
  ---------------------------------------------------------------*/
double CTime::timestampTotime_t( const TTimeStamp2  &t )
{
    return double(t - ((uint64_t)116444736*1000000000)) / 10000000.0;
}
