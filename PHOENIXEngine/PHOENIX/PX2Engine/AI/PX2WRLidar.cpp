// PX2WRLidar.hpp

#include "PX2WRLidar.hpp"
#include "PX2WRConstant.hpp"
#include "PX2AILidarData.hpp"
#include "PX2Log.hpp"
#include "PX2System.hpp"
#include "PX2ScopedCS.hpp"
#include <iostream>
using namespace PX2;

struct DataSaveSt
{
	unsigned int   iTotalFrameIndex;
	unsigned char  subPkgNum;
	unsigned char  subPkgIndex;
	unsigned int   iRawDataLens;
	unsigned char  sens_data[CMD_FRAME_MAX_LEN];
};
unsigned char mRecvBuf[RECV_BUFFER_SIZE];
int first_step_index = 0;
int last_step_index = 800;
unsigned int iFrameTotalIndex = 0;
unsigned int rawDatalen = 0;
unsigned char subPkgNum = 0;
unsigned char subPkgIndex = 0;
unsigned char checkSum = 0;
DataSaveSt mDataSaveSt[CMD_FRAME_MAX_SUB_PKG_NUM];
int iCurFrameNumber = 0;
std::vector<int> iDistance;
std::vector<int> iIntensity;
#define angleResolution 1.0/3.0
//----------------------------------------------------------------------------
WRLidar::WRLidar()
{
	memset(&mRecvBuf[0], 0, RECV_BUFFER_SIZE);

	mThread = new0 Thread();
	mIsStop = true;
}
//----------------------------------------------------------------------------
WRLidar::~WRLidar()
{
	PX2_LOG_INFO("~WRLidar0");
	mIsStop = true;
	PX2_LOG_INFO("~WRLidar1");
}
//----------------------------------------------------------------------------
void WRLidar::Run()
{
	while (!mIsStop)
	{
		iDistance.clear();
		iIntensity.clear();

		int recvNum = mSocket.ReceiveFrom(&mRecvBuf[0], RECV_BUFFER_SIZE, mSocketAddress);
		if (recvNum <= 0)
		{
			continue;
		}

		std::cout << std::string("recvNum:") << recvNum << std::endl;

		rawDatalen = (unsigned int)((*(mRecvBuf + CMD_FRAME_HEADER_LENGTH_H) << 8) | (*(mRecvBuf + CMD_FRAME_HEADER_LENGTH_L))) - (CMD_FRAME_DATA_START - CMD_FRAME_HEADER_CHECK_SUM);
		
		std::cout << std::string("rawDatalen") << rawDatalen << std::endl;

		iFrameTotalIndex = (*(mRecvBuf + CMD_FRAME_HEADER_TOTAL_INDEX_H) << 8) | (*(mRecvBuf + CMD_FRAME_HEADER_TOTAL_INDEX_L));
		subPkgNum = *(mRecvBuf + CMD_FRAME_HEADER_SUB_PKG_NUM);
		subPkgIndex = *(mRecvBuf + CMD_FRAME_HEADER_SUB_INDEX);
		checkSum = 0;
		for (int i = 0; i < rawDatalen + (CMD_FRAME_DATA_START - CMD_FRAME_HEADER_TYPE); i++)
		{
			checkSum += mRecvBuf[CMD_FRAME_HEADER_TYPE + i];
		}
		if (checkSum != mRecvBuf[CMD_FRAME_HEADER_CHECK_SUM])
		{
			continue;
		}

		mDataSaveSt[subPkgIndex].iTotalFrameIndex = iFrameTotalIndex;
		mDataSaveSt[subPkgIndex].iRawDataLens = rawDatalen;
		mDataSaveSt[subPkgIndex].subPkgNum = subPkgNum;
		mDataSaveSt[subPkgIndex].subPkgIndex = subPkgIndex;

		std::cout << rawDatalen << std::endl;

		memcpy(mDataSaveSt[subPkgIndex].sens_data, mRecvBuf + CMD_FRAME_DATA_START, rawDatalen);

		bool checkResult = true;
		for (int i = 0; i < subPkgNum - 1; i++)
		{
			if (mDataSaveSt[i].iTotalFrameIndex != mDataSaveSt[i + 1].iTotalFrameIndex || mDataSaveSt[i].subPkgIndex != mDataSaveSt[i + 1].subPkgIndex - 1)
			{
				checkResult = false;
				break;
			}
		}
		if (!checkResult)
		{
			continue;
		}

		std::cout << std::string("333.555") << std::endl;

		///////У��ͨ�����,��ȡ������Ϣ
		std::vector<int> vcDistance;
		std::vector<int> vcIntensity;

		int iCurDataNumber = mDataSaveSt[0].iRawDataLens / 2;

		std::cout << iCurDataNumber << std::endl;
		for (int i = 0; i < iCurDataNumber; i++)
		{
			//std::cout << i << std::endl;

			vcDistance.push_back(mDataSaveSt[0].sens_data[2 * i] * 256 + mDataSaveSt[0].sens_data[2 * i + 1]);
			vcIntensity.push_back(mDataSaveSt[2].sens_data[2 * i] * 256 + mDataSaveSt[2].sens_data[2 * i + 1]);
		}

		iCurDataNumber = mDataSaveSt[1].iRawDataLens / 2;
		for (int i = 0; i < iCurDataNumber; i++)
		{
			vcDistance.push_back(mDataSaveSt[1].sens_data[2 * i] * 256 + mDataSaveSt[1].sens_data[2 * i + 1]);
			vcIntensity.push_back(mDataSaveSt[3].sens_data[2 * i] * 256 + mDataSaveSt[3].sens_data[2 * i + 1]);
		}

		for (int i = first_step_index; i <= last_step_index; i++)
		{
			iDistance.push_back(vcDistance[i]);
			iIntensity.push_back(vcIntensity[i] > 5000 ? (vcIntensity[i] > 55000 ? 600 : 200 + (vcIntensity[i] - 5000) / 1200) : vcIntensity[i] / 25);
		}
		iCurFrameNumber = mDataSaveSt[0].iTotalFrameIndex;

		std::vector<RslidarDataComplete> lidarDataThread;
		lidarDataThread.resize(last_step_index - first_step_index);

		RslidarDataComplete rdc;
		for (int i = 0; i < (int)last_step_index- first_step_index; i++)
		{
			rdc.signal = iIntensity[i];
			rdc.distance = iDistance[i] * 0.01f;	


			rdc.angle = (i*angleResolution - 45);

			lidarDataThread[i] = rdc;
		}

		{
			ScopedCS cs(&mMutex);
			RDCS = lidarDataThread;
		}
	}
}
//----------------------------------------------------------------------------
void WRLidar::Start(const std::string &ip, int port)
{
	mIP = ip;
	mPort = port;

	std::cout << ip << std::endl;
	std::cout << port << std::endl;

	mSocketAddress = SocketAddress(mIP, (int16_t)mPort);
	mSocket.Bind(mSocketAddress);

	PX2_LOG_INFO("PX2Engine %s", "0---");

	Start_measurement();

	PX2_LOG_INFO("PX2Engine %s", "1---");

	mIsStop = false;
	mThread->Start(*this);

	PX2_LOG_INFO("PX2Engine %s", "2---");
}
//----------------------------------------------------------------------------
void WRLidar::Stop()
{
	mIsStop = true;
	System::SleepSeconds(1.0f);
	//mThread->Join();
}
//----------------------------------------------------------------------------
void WRLidar::Update(float appSeconds, float elapsedSeconds)
{
}
//----------------------------------------------------------------------------
bool WRLidar::Set_scanning_parameters(int iFirstStepIndex, int iLastStepIndex)
{
	if (iFirstStepIndex < 0 || iLastStepIndex>810 || iFirstStepIndex > iLastStepIndex)
		return false;
	else
	{
		first_step_index = iFirstStepIndex;
		last_step_index = iLastStepIndex;
		return true;
	}
}
//----------------------------------------------------------------------------
bool WRLidar::Start_measurement()
{
	int iSendNumber;
	int sendVal = mSocket.SendTo(CMD_START_SCAN, SIZE_OF_CMD,
		mSocketAddress);
	
	if (sendVal != SIZE_OF_CMD)
	{
		is_actived = false;
		return false;
	}

	is_actived = true;
	
	return true;
}
//----------------------------------------------------------------------------
std::vector<RslidarDataComplete> &WRLidar::GetRDCS()
{
	{
		ScopedCS cs(&mMutex);
		ReturnRDCS = RDCS;
	}

	return ReturnRDCS;
}
//----------------------------------------------------------------------------