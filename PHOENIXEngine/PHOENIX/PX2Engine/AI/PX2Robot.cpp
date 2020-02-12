// PX2Robot.cpp

#include "PX2Robot.hpp"
#include "PX2Creater.hpp"
#include "PX2Project.hpp"
#include "PX2JSONData.hpp"
#include "PX2Timestamp.hpp"
#include "PX2Timespan.hpp"
#include "PX2VoxelPre.hpp"
#if defined PX2_USE_VOXEL
#include "PX2VoxelSection.hpp"
#endif
#include "PX2AIES.hpp"
#include "PX2Renderer.hpp"
#include "PX2Arduino.hpp"
#include "PX2ResourceManager.hpp"
#include "PX2Camera.hpp"
#include "PX2Canvas.hpp"
#include "PX2Scene.hpp"
#include "PX2UIVlc.hpp"
#include "PX2HMatrix.hpp"
#include "PX2AILiDar.hpp"
#include "PX2Polypoint.hpp"
#include "PX2Serial.hpp"
#include "PX2Timestamp.hpp"
#include "PX2AIPath.hpp"
#include "PX2Arduino.hpp"
#include "PX2Smoother.hpp"
#include "PX2SmartPointer.hpp"
#include "PX2Actor.hpp"

#if defined (__ANDROID__)

#define PX2_USE_SLAM2D 1

#if defined PX2_USE_SLAM2D
#include "Slam2DPlugin.hpp"
#endif

//#define PX2_USE_SLAMGMAPPING 1
#if defined PX2_USE_SLAMGMAPPING
#include "SlamGMappingPlugin.hpp"
#endif

#endif

#include "PX2PluginManager.hpp"
#include "PX2AIAgent.hpp"
#include "PX2EngineSceneCanvas.hpp"
#include "PX2Creater.hpp"
#include "PX2Actor.hpp"
using namespace PX2;

#if defined _WIN32 || defined WIN32
#include <windows.h> 
#endif

Plugin *Robot::mSlamPlugin = 0;
//----------------------------------------------------------------------------
Robot::Robot() :
mLiDar(0),
mOffsetDegree(0.0f),
mRobotMapX(0),
mRobotMapY(0),
m2DSlamAngle(0.0f),
mIsHasAxis(false),
mAgent(0),
mRotationRad(0.0f),
mIsSlamMapDoUpdate(true),
mArduino(0),
mIsNeedUpdateTexture(true),
mIsNeedSendSpeed(false)
{
	mMaxSpeed = 0.1f;
	mMinSpeed = 0.0f;
	mMaxAccelerate = 0.4f;
	mPredictTime = 6.0f;
	mRadius = 0.16f;
	mLidarIngoreRadius = 0.0f;
	mLidarOffset = 0.0f;
	mMotoRate = 35;
	mWheelRadius = 0.05f;

	mRoleType = RT_MASTER;

	mLeftSpeed = 0.0f;
	mRightSpeed = 0.0f;

	mPosition = APoint::ORIGIN;

	mDirection = AVector::UNIT_Y;
	mRotationRad = 0.0f;
	mRight = AVector::UNIT_X;
	mUp = AVector::UNIT_Z;

	// leidar
	mPicBoxMap = new0 UIFPicBox();
	mPicBoxMap->GetUIPicBox()->GetMaterialInstance()
		->GetMaterial()->GetPixelShader(0, 0)->SetFilter(0,
		Shader::SF_NEAREST);

	mMapDataPtr = new0 RobotMapData();
	_RecreateMapTex(mMapDataPtr->MapStruct.MapWidth,
		mMapDataPtr->MapStruct.MapHeight);

	mRobotTex = new0 UIFPicBox();
	mPicBoxMap->AttachChild(mRobotTex);
	mRobotTex->LocalTransform.SetTranslateY(-1.0f);
	mRobotTex->GetUIPicBox()->SetTexture("Data/engine/white.png");
	mRobotTex->GetUIPicBox()->SetColor(Float3::BLACK);
	mRobotTex->SetSize(2.0f, 6.0f);
	mRobotTex->SetPivot(0.5f, 0.0f);
	mRobotTex->SetAlphaSelfCtrled(true);
	UIFPicBox *tex = new0 UIFPicBox();
	mRobotTex->AttachChild(tex);
	tex->LocalTransform.SetTranslateY(-1.0f);
	tex->GetUIPicBox()->SetTexture("Data/engine/white.png");
	tex->SetSize(2.0f, 2.0f);
	tex->SetColor(Float3::WHITE);
	tex->SetAnchorHor(0.5f, 0.5f);
	tex->SetAnchorVer(0.0f, 0.0f);
	tex->SetPivot(0.5f, 0.5f);
	tex->SetAlphaSelfCtrled(true);

	mRobotTexSlam = new0 UIFPicBox();
	mPicBoxMap->AttachChild(mRobotTexSlam);
	mRobotTexSlam->LocalTransform.SetTranslateY(-1.0f);
	mRobotTexSlam->GetUIPicBox()->SetTexture("Data/engine/white.png");
	mRobotTexSlam->SetSize(2.0f, 6.0f);
	mRobotTexSlam->SetPivot(0.5f, 0.0f);
	mRobotTexSlam->GetUIPicBox()->SetColor(Float3::RED);
	mRobotTexSlam->SetAlphaSelfCtrled(true);
	UIFPicBox *texSlam = new0 UIFPicBox();
	mRobotTexSlam->AttachChild(texSlam);
	texSlam->LocalTransform.SetTranslateY(-1.0f);
	texSlam->GetUIPicBox()->SetTexture("Data/engine/white.png");
	texSlam->SetSize(2.0f, 2.0f);
	texSlam->SetColor(Float3::BLUE);
	texSlam->SetAnchorHor(0.5f, 0.5f);
	texSlam->SetAnchorVer(0.0f, 0.0f);
	texSlam->SetPivot(0.5f, 0.5f);
	texSlam->SetAlphaSelfCtrled(true);

	mPathGraph = new0 PathingGraph();

	mIsSlamMapInited = false;

	mIsSlamMapDataChanged = true;

	InitSlamMap(256, 256, 0.05f, true);

	mArduino = new0 Arduino();

	mIsAdjustToDirection = false;

	mVoxelSection = 0;
}
//----------------------------------------------------------------------------
Robot::~Robot()
{
	PX2_LOG_INFO("~Robot 0");
	if (mLiDar)
	{
		PX2_LOG_INFO("~Robot 1");
		delete0(mLiDar);
	}

	if (mSlamPlugin)
	{
		PX2_LOG_INFO("~Robot 2");
		PX2_PLUGINMAN.UninstallPlugin(mSlamPlugin);
	}
	PX2_LOG_INFO("~Robot 3");
}
//----------------------------------------------------------------------------
Robot *Robot::New()
{
	return new0 Robot();
}
//----------------------------------------------------------------------------
void Robot::Delete(Robot* robot)
{
	delete0(robot);
	Robot::Set(0);
}
//----------------------------------------------------------------------------
void Robot::InitlizeSlam2D()
{
#if defined PX2_USE_SLAM2D
	mSlamPlugin = new Slam2DPlugin();
#endif

#if defined PX2_USE_SLAMGMAPPING
	mSlamPlugin = new SlamGMappingPlugin();
#endif

	if (mSlamPlugin)
	{
		PX2_PLUGINMAN.InstallPlugin(mSlamPlugin);
	}
}
//----------------------------------------------------------------------------
void Robot::TernimateSlam2D()
{
	if (mSlamPlugin)
		PX2_PLUGINMAN.UninstallPlugin(mSlamPlugin);
}
//----------------------------------------------------------------------------
bool Robot::SaveMap(const std::string &filename)
{
	if (RT_MASTER == mRoleType || RT_CONNECTOR_CALCULATE == mRoleType)
	{
		_UpdateMapDataToPathFinder();

		std::string writeablePath = PX2_RM.GetWriteablePath();
		std::string dstPath = writeablePath + "Write_ZERONE/";
		if (!PX2_RM.IsFileFloderExist(dstPath))
		{
			PX2_RM.CreateFloder(writeablePath, "Write_ZERONE/");
		}

		if (mMapDataPtr)
		{
			mMapDataPtr->SetResourcePath(filename);

			OutStream outStream;
			outStream.Insert(mMapDataPtr);

			if (outStream.Save(dstPath + filename))
			{
				return true;
			}
		}

		return false;
	}
	else if (RT_CONNECTOR == mRoleType)
	{

	}

	return true;
}
//----------------------------------------------------------------------------
bool Robot::LoadMap(const std::string &filename, const APoint &initPos,
	float curAngle)
{
	if (RT_MASTER == mRoleType || RT_CONNECTOR_CALCULATE == mRoleType)
	{
		mMapDataInit = RobotMapData();

		std::string writeablePath = PX2_RM.GetWriteablePath();
		std::string dstPath = writeablePath + "Write_ZERONE/" + filename;

		InStream inStream;
		if (inStream.Load(dstPath))
		{
			RobotMapData *data = DynamicCast<RobotMapData>(inStream.GetObjectAt(0));
			mMapDataPtr = data;
			if (data)
			{
				mMapDataInit = *data;

				if (initPos != APoint::ORIGIN)
				{
					mMapDataInit.MapStruct.CurPos = initPos;
				}
				if (0.0f != curAngle)
				{
					mMapDataInit.MapStruct.CurAngle = curAngle;
				}
			}
		}

		if (mMapDataPtr)
		{
			InitSlamMap(mMapDataPtr->MapStruct.MapWidth,
				mMapDataPtr->MapStruct.MapHeight,
				mMapDataPtr->MapStruct.MapResolution, false);

			_UpdateMapDataToPathFinder();

			return true;
		}

		return false;
	}
	else
	{
	}

	return true;
}
//----------------------------------------------------------------------------
int _Roundle(float number)
{
	return (int)((number > 0.0f) ? floor(number + 0.5f) : ceil(number - 0.5f));
} 
//----------------------------------------------------------------------------
void Robot::_UpdateMapDataToPathFinder()
{
	int mapWidth = mMapDataPtr->MapStruct.MapWidth;
	int mapHeight = mMapDataPtr->MapStruct.MapHeight;
	int mapAllSize = mapWidth * mapHeight;
	float resolution = mMapDataPtr->MapStruct.MapResolution;

	RobotMapData rmd;
	std::vector<unsigned char> map = rmd._ExtendLarger(mMapDataPtr->Map2DOrigin,
		mMapDataPtr->MapStruct.MapWidth, 
		mMapDataPtr->MapStruct.MapHeight, RobotMapData::MOT_OBJST);
	std::vector<unsigned char> map1 = rmd._ExtendLarger(map,
		mMapDataPtr->MapStruct.MapWidth,
		mMapDataPtr->MapStruct.MapHeight, RobotMapData::MOT_OBJST);

	std::vector<unsigned char> selfDraw0 = rmd._ExtendLarger(mMapDataPtr->SelfDrawMapData2D,
		mMapDataPtr->MapStruct.MapWidth,
		mMapDataPtr->MapStruct.MapHeight, RobotMapData::MOT_OBJST);
	std::vector<unsigned char> selfDraw1 = rmd._ExtendLarger(selfDraw0,
		mMapDataPtr->MapStruct.MapWidth,
		mMapDataPtr->MapStruct.MapHeight, RobotMapData::MOT_OBJST);

	// mask
	std::vector<unsigned char> maskData;
	maskData.resize(mapAllSize);
	memset(&maskData[0], 0, maskData.size());

	for (int y = 0; y<mapHeight; y++)
	{
		for (int x = 0; x < mapWidth; x++)
		{
			int i = y*mapWidth + x;
			bool isHasObst = false;

			int mapObstValue = map1[i];
			if (RobotMapData::MOT_OBJST == mapObstValue)
			{
				isHasObst = true;
			}

			int selfDrawValue = selfDraw1[i];
			if (RobotMapData::MOT_OBJST == selfDrawValue)
			{
				isHasObst = true;
			}
			else if (RobotMapData::MOT_NONE == selfDrawValue)
			{
				// do nothing
			}
			else if (RobotMapData::MOT_CANGO == selfDrawValue)
			{
				isHasObst = false;
			}

			int gw = mPathGraph->GetMapWidth();
			int gh = mPathGraph->GetMapHeight();

			float xPercent = (float)x / (float)mapWidth;
			float yPercent = (float)y / (float)mapHeight;
			float fXIndex = xPercent * (float)gw;
			float fYIndex = yPercent * (float)gh;

			int xIndex = _Roundle(fXIndex);
			int yIndex = _Roundle(fYIndex);

			if (isHasObst)
			{
				_SetGraphValue(xIndex, yIndex, 1000.0f);
				_SetGraphValue(xIndex - 1, yIndex, 1000.0f);
				_SetGraphValue(xIndex + 1, yIndex, 1000.0f);
				_SetGraphValue(xIndex, yIndex - 1, 1000.0f);
				_SetGraphValue(xIndex, yIndex + 1, 1000.0f);

				_SetGraphValue(xIndex - 1, yIndex - 1, 1000.0f);
				_SetGraphValue(xIndex + 1, yIndex + 1, 1000.0f);
				_SetGraphValue(xIndex - 1, yIndex + 1, 1000.0f);
				_SetGraphValue(xIndex + 1, yIndex - 1, 1000.0f);

				maskData[i] = 1;
			}
			else
			{
				_SetGraphValue(xIndex, yIndex, PATHING_DEFAULT_ARC_WEIGHT);

				maskData[i] = 0;
			}
		}
	}

	mPathGraph->SetMapData(maskData, mapWidth, mapHeight, resolution);
}
//----------------------------------------------------------------------------
void Robot::_SetGraphValue(int x, int y, float val)
{
	int gw = mPathGraph->GetMapWidth();
	int gh = mPathGraph->GetMapHeight();
	if (x >= 0 && x < gw && y >= 0 && y < gh)
	{
		int i = x + y * gw;
		mPathGraph->SetArcValue(i, val);
	}
}
//----------------------------------------------------------------------------
const APoint &Robot::GetPosition() const
{
	return mPosition;
}
//----------------------------------------------------------------------------
RobotMapData &Robot::GetInitMapData()
{
	return mMapDataInit;
}
//----------------------------------------------------------------------------
void Robot::ClearInitMapData()
{
	mMapDataInit = RobotMapData();
}
//----------------------------------------------------------------------------
RobotMapData *Robot::GetCurMapData()
{
	return mMapDataPtr;
}
//----------------------------------------------------------------------------
void Robot::SetObstMapValueAtPos(const APoint &pos, float range, int val)
{
	if (!mMapDataPtr)
		return;

	if (0 == mMapDataPtr->SelfDrawMapData2D.size())
		return;

	_SetPosVal(mMapDataPtr->SelfDrawMapData2D, pos, range, val);

	mIsSlamMapDataChanged = true;
}
//----------------------------------------------------------------------------
void Robot::SetPathFinderLineValueAtPos(const APoint &pos, float range, 
	int val)
{
	if (!mIsSlamMapInited)
		return;

	if (0 == mMapDataPtr->PathFinderLineDraw.size())
		return;

	_SetPosVal(mMapDataPtr->PathFinderLineDraw, pos, range, val);

	mIsSlamMapDataChanged = true;
}
//----------------------------------------------------------------------------
void Robot::_SetPosVal(std::vector<unsigned char> &datas, const APoint &pos,
	float range, int value)
{
	int mapWidth = mMapDataPtr->MapStruct.MapWidth;
	int mapHeight = mMapDataPtr->MapStruct.MapHeight;
	float reso = mMapDataPtr->MapStruct.MapResolution;

	float mapAllWidth = (float)(mapWidth * reso);
	float mapAllHeight = (float)(mapHeight * reso);

	float texXStart = 0.5f + ((pos.X() - range) / mapAllWidth);
	float texXEnd = 0.5f + ((pos.X() + range) / mapAllWidth);

	float texYStart = 0.5f + ((pos.Y() - range) / mapAllHeight);
	float texYEnd = 0.5f + ((pos.Y() + range) / mapAllHeight);

	int iTexXStart = (int)(texXStart*mapWidth);
	int iTexXEnd = (int)(texXEnd*mapWidth);
	int iTexYStart = (int)(texYStart*mapHeight);
	int iTexYEnd = (int)(texYEnd*mapHeight);

	for (int x = iTexXStart; x < iTexXEnd; x++)
	{
		for (int y = iTexYStart; y < iTexYEnd; y++)
		{
			int index = y*mapWidth + x;
			datas[index] = (unsigned char)value;
		}
	}

}
//----------------------------------------------------------------------------
void Robot::SetMapValueAtPos(std::vector<unsigned char> &datas,
	const APoint &pos, int val)
{
	int mapWidth = mMapDataPtr->MapStruct.MapWidth;
	int mapHeight = mMapDataPtr->MapStruct.MapHeight;
	float mapReso = mMapDataPtr->MapStruct.MapResolution;

	float mapAllWidth = mapWidth * mapReso;
	float mapAllHeight = mapHeight * mapReso;

	float u = 0.5f + pos.X() / mapAllWidth;
	float v = 0.5f + pos.Y() / mapAllHeight;

	int x = (int)(u*mapWidth);
	int y = (int)(v*mapHeight);

	int index = y*mapWidth + x;
	if (0 <= index && index < (int)datas.size())
		datas[index] = (unsigned char)val;

	mIsSlamMapDataChanged = true;
}
//----------------------------------------------------------------------------
void Robot::_RecreateMapTex(int mapWidth, int mapHeight)
{
	Texture::Format formatLidar = Texture::TF_A8R8G8B8;
	mTextureMap = new0 Texture2D(formatLidar, mapWidth, mapHeight, 1);
	unsigned char *pDest = (unsigned char*)mTextureMap->GetData(0);
	int offsetDst = 0;
	for (int row = 0; row < mapHeight; ++row)
	{
		for (int col = 0; col < mapWidth; ++col)
		{
			pDest[offsetDst + 0] = 0; // b
			pDest[offsetDst + 1] = 0; // g
			pDest[offsetDst + 2] = 0; // r
			pDest[offsetDst + 3] = 255;

			offsetDst += 4;
		}
	}

	mPicBoxMap->GetUIPicBox()->SetTexture(mTextureMap);
}
//----------------------------------------------------------------------------
void Robot::SetNeedUpdateTexture(bool isNeed)
{
	mIsNeedUpdateTexture = isNeed;

	if (mLiDar)
	{
		mLiDar->SetNeedUpdateTexture(isNeed);
	}
}
//----------------------------------------------------------------------------
bool Robot::IsNeedUpdateTexture() const
{
	return mIsNeedUpdateTexture;
}
//----------------------------------------------------------------------------
void Robot::Update(float appseconds, float elpasedSeconds)
{
	PX2_UNUSED(appseconds);
	PX2_UNUSED(elpasedSeconds);

	if (mArduino)
	{
		mArduino->Update(elpasedSeconds);
	}

	if (mLiDar)
	{
		mLiDar->Update(appseconds, elpasedSeconds);
	}

	if (RT_MASTER == mRoleType || RT_CONNECTOR == mRoleType || 
		RT_CONNECTOR_CALCULATE == mRoleType)
	{
		if (mIsHasAxis)
		{
			Arduino::AxisObj curAxisObj = mArduino->GetCurAxisObj();

			float rad = curAxisObj.Y * Mathf::DEG_TO_RAD;
			float x = Mathf::Cos(rad);
			float y = Mathf::Sign(rad);

			mAxisDirection = AVector(x, y, 0.0f);
			mAxisUp = AVector::UNIT_Z;
			mAxisRight = mAxisDirection.Cross(mAxisUp);
			mAxisRotMatrix = HMatrix(mAxisRight, mAxisDirection, mAxisUp,
				APoint::ORIGIN, true);
		}

		_UpdateTransform(elpasedSeconds);
	}
}
//----------------------------------------------------------------------------
void Robot::AdjustToDirection(const AVector &dir)
{
	mIsAdjustToDirection = true;
	mAdjustToDirection = dir;
}
//----------------------------------------------------------------------------
bool Robot::IsSlamMapInited() const
{
	return mIsSlamMapInited;
}
//----------------------------------------------------------------------------
void Robot::InitSlamMap(int mapWidth, int mapHeight, float resolution,
	bool clearCurMapData)
{
	float perc = 1.0f;

#if defined _DEBUG
	perc = 10.0f;
#endif
	mPathGraph->InitSlamMap((int)(mapWidth / perc), (int)(mapHeight / perc),
		resolution*perc);

	int allSize = mapWidth * mapHeight;
	mMap2DLidarData.resize(allSize);
	memset(&mMap2DLidarData[0], RobotMapData::MOT_NONE, 
		allSize * sizeof(char));

	if (clearCurMapData)
		mMapDataPtr->Initlize(mapWidth, mapHeight, resolution);

	bool isNeedReCreateMapTex = false;
	if (!mTextureMap)
	{
		isNeedReCreateMapTex = true;
	}
	else
	{
		if (mapWidth != mTextureMap->GetWidth() ||
			mapHeight != mTextureMap->GetHeight())
			isNeedReCreateMapTex = true;
	}

	if (isNeedReCreateMapTex)
	{
		_RecreateMapTex(mapWidth, mapHeight);
	}

	mIsSlamMapInited = true;
	mIsSlamMapDataChanged = true;
}
//----------------------------------------------------------------------------
void Robot::SetSlam2DMap(std::vector<unsigned char> &map)
{
	if (!mIsSlamMapInited)
		return;

	if (!map.empty())
	{
		mMapDataPtr->Map2DOrigin = map;
		mIsSlamMapDataChanged = true;
	}
}
//----------------------------------------------------------------------------
void Robot::SetSelfDraw(const std::vector<unsigned char> &map)
{
	if (!mIsSlamMapInited)
		return;

	if (!map.empty())
	{
		mMapDataPtr->SelfDrawMapData2D = map;
		mIsSlamMapDataChanged = true;
	}
}
//----------------------------------------------------------------------------
void Robot::SetSlam2DPosition(const APoint &pos, float angle)
{
	if (m2DSlamPos == pos && m2DSlamAngle == angle)
		return;

	m2DSlamPos = pos;
	m2DSlamAngle = angle;
}
//----------------------------------------------------------------------------
void Robot::_UpdateRobotTex(UIFPicBox *picBox, const APoint &pos, float rot)
{
	HMatrix mat;
	mat.MakeRotation(AVector::UNIT_Y, -rot);
	picBox->LocalTransform.SetRotate(mat);
	float mapWidth = mMapDataPtr->MapStruct.MapWidth *
		mMapDataPtr->MapStruct.MapResolution;
	float mapHeight = mMapDataPtr->MapStruct.MapHeight *
		mMapDataPtr->MapStruct.MapResolution;

	float xPos = (pos.X() + mapWidth*0.5f) / mapWidth;
	float yPos = (pos.Y() + mapHeight*0.5f) / mapHeight;
	picBox->SetAnchorHor(xPos, xPos);
	picBox->SetAnchorVer(yPos, yPos);
}
//----------------------------------------------------------------------------
void Robot::_UpdateTransform(float elapsedSeconds)
{
	PX2_UNUSED(elapsedSeconds);

	mPosition = m2DSlamPos;
	float angle = m2DSlamAngle;	

	mMapDataPtr->MapStruct.CurPos = mPosition;
	mMapDataPtr->MapStruct.CurAngle = angle;

	float xDir = Mathf::Sin(-angle);
	float yDir = Mathf::Cos(angle);
	AVector dir = AVector(xDir, yDir, 0.0f);
	dir.Normalize();
	mDirection = dir;
	mDirection.Normalize();

	mRight = mDirection.Cross(AVector::UNIT_Z);
	mUp = AVector::UNIT_Z;
	mMatrix = HMatrix(mRight, mDirection, mUp, mPosition, true);

	if (mLiDar)
	{
		mLiDar->SetCurTransform(mMatrix);
	}

	_UpdateRobotTex(mRobotTex, mPosition, mRotationRad);
	_UpdateRobotTex(mRobotTexSlam, m2DSlamPos, m2DSlamAngle);
}
//----------------------------------------------------------------------------
void Robot::SetRoleType(RoleType rtType)
{
	mRoleType = rtType;
}
//----------------------------------------------------------------------------
void Robot::CreateLidar()
{
	if (mLiDar)
	{
		mLiDar->Close();
		mLiDar = 0;
	}

	mLiDar = new0 LiDar();
	LiDar::LiDarType lidarType = mLiDar->GetLiDarType();
	if (LiDar::LT_III == lidarType)
	{
		mOffsetDegree = -90.0f;
	}
	else if (LiDar::LT_RP == lidarType)
	{
		mOffsetDegree = 90.0f;
	}
}
//----------------------------------------------------------------------------
bool Robot::LidarOpen(const std::string &portIP, int baudratePort)
{
	if (mLiDar)
	{
		return mLiDar->Open(portIP, baudratePort);
	}

	return false;
}
//----------------------------------------------------------------------------
void Robot::SetArduino(Arduino *arduino)
{
	mArduino = arduino;
}
//----------------------------------------------------------------------------
bool Robot::IsArduinoConnected() const
{
	return mArduino->IsInitlized();
}
//----------------------------------------------------------------------------
void Robot::SetSpeedLeft(float speed)
{
	mLeftSpeed = speed;

	mIsNeedSendSpeed = true;
}
//----------------------------------------------------------------------------
void Robot::SetSpeedRight(float speed)
{
	mRightSpeed = speed;

	mIsNeedSendSpeed = true;
}
//----------------------------------------------------------------------------
void Robot::LeftRunSpeed(float speed)
{
	if (mArduino)
		mArduino->RunSpeed(0, speed, mWheelRadius, mMotoRate);
}
//----------------------------------------------------------------------------
void Robot::RightRunSpeed(float speed)
{
	if (mArduino)
		mArduino->RunSpeed(1, speed, mWheelRadius, mMotoRate);
}
//----------------------------------------------------------------------------
float Robot::GetYawCrossValue() const
{
	LiDar::LiDarType lidarType = mLiDar->GetLiDarType();
	if (LiDar::LT_III == lidarType)
	{
		return -1.0f;
	}
	else if (LiDar::LT_RP == lidarType)
	{
		return 1.0f;
	}

	return 1.0f;
}
//----------------------------------------------------------------------------
void Robot::SetVoxelSection(VoxelSection *vs)
{
	mVoxelSection = vs;
}
//----------------------------------------------------------------------------
VoxelSection *Robot::GetVoxelSection()
{
	return mVoxelSection;
}
//----------------------------------------------------------------------------
AVector &Robot::GetAxisDirection()
{
	return mAxisDirection;
}
//----------------------------------------------------------------------------
HMatrix &Robot::GetAxisRotMatrix()
{
	return mAxisRotMatrix;
}
//----------------------------------------------------------------------------
void Robot::GoTarget(const APoint &targetPos, PathType type)
{
	if (mMapDataPtr)
	{
		memset(&(mMapDataPtr->PathFinderLineDraw[0]), 0,
			(int)mMapDataPtr->PathFinderLineDraw.size());

		mIsSlamMapDataChanged = true;
	}

	mGoTargetPos = targetPos;

	std::vector<PathingNode*> vec;
	AISteeringPath aiAgentPath;
	std::vector<APoint> targetPoints;

	int posIndex = 0;
	APoint firstGoPos;

	if (type == PT_A)
	{
		mCurPathPlan = mPathGraph->FindPath(mPosition, mGoTargetPos);
		if (mCurPathPlan)
		{
			PathingNodeList list = mCurPathPlan->m_path;

			auto it = list.begin();
			for (; it != list.end(); it++)
			{
				PathingNode *node = *it;

				APoint pos = node->GetPos();
				targetPoints.push_back(pos);

				posIndex++;

				if (2 == posIndex)
				{
					firstGoPos = pos;
				}
			}

			mCurPathPlan->ResetPath();
		}
	}

	for (int i = 0; i < (int)targetPoints.size(); i++)
	{
		const APoint &pos = targetPoints[i];
		aiAgentPath.AddWayPoint(pos);
		SetPathFinderLineValueAtPos(pos, 0.05f, 200);
	}

	if (RT_MASTER == mRoleType || RT_CONNECTOR_CALCULATE == mRoleType)
	{
		if (mAgent)
		{
			mAgent->SetPath(aiAgentPath);
		}

		if (posIndex >= 2)
		{
			AVector direction = firstGoPos - mPosition;
			float dist = direction.Normalize();
			if (dist > 0.0f)
			{
				AdjustToDirection(direction);
			}
		}
	}
}
//----------------------------------------------------------------------------
void Robot::ClearPathFinder()
{
	if (mAgent)
	{
		AISteeringPath aiAgentPath;
		mAgent->SetPath(aiAgentPath);
	}

	if (mMapDataPtr)
	{
		memset(&(mMapDataPtr->PathFinderLineDraw[0]), 0,
			(int)mMapDataPtr->PathFinderLineDraw.size());

		mIsSlamMapDataChanged = true;
	}
}
//----------------------------------------------------------------------------
void Robot::_SetAIAgent(AIAgent *agent)
{
	mAgent = agent;
}
//----------------------------------------------------------------------------
AIAgent *Robot::GetAIAgent()
{
	return mAgent;
}
//----------------------------------------------------------------------------
bool Robot::_IsInRightDirection(const AVector &dir)
{
	float valDot = dir.Dot(mDirection);
	float degree = Mathf::ACos(valDot) * Mathf::RAD_TO_DEG;
	if (degree < 10.0f)
	{
		return true;
	}

	return false;
}
//----------------------------------------------------------------------------
void Robot::_UpdateAdjustDirection(const AVector &dir, float elapsedSeconds)
{
	PX2_UNUSED(elapsedSeconds);

	float valDot = dir.Dot(mDirection);
	float degree = Mathf::ACos(valDot) * Mathf::RAD_TO_DEG;
	if (degree < 12.0f)
	{
	}
	else
	{
		if (mDirection != AVector::ZERO)
		{
			AVector right = mDirection.Cross(dir);
			if (right.Z() > 0.0f)
			{
				// move left
				mArduino->Run(Arduino::SDT_LEFT, 25);
			}
			else
			{
				// move right
				mArduino->Run(Arduino::SDT_RIGHT, 25);
			}
		}
	}
}
//----------------------------------------------------------------------------