// PX2SceneBuilderAnimation.cpp

#include "PX2SceneBuilder.hpp"

//----------------------------------------------------------------------------
bool SceneBuilder::AreEqual (const Point3 &point1, const Point3 &point2)
{
	return fabsf(point1.x - point2.x) <= MIN_DIFFERENCE
		&& fabsf(point1.y - point2.y) <= MIN_DIFFERENCE
		&& fabsf(point1.z - point2.z) <= MIN_DIFFERENCE;
}
//----------------------------------------------------------------------------
bool SceneBuilder::AreEqual (const Quat &quat1, const Quat &quat2)
{
	return fabsf(quat1.x - quat2.x) <= MIN_DIFFERENCE
		&& fabsf(quat1.y - quat2.y) <= MIN_DIFFERENCE
		&& fabsf(quat1.z - quat2.z) <= MIN_DIFFERENCE
		&& fabsf(quat1.w - quat2.w) <= MIN_DIFFERENCE;
}
//----------------------------------------------------------------------------
bool SceneBuilder::CompareKeyTimes (KeyInfo* pkFirst, KeyInfo* pkSecond)
{
	return pkFirst->Time < pkSecond->Time;
}
//----------------------------------------------------------------------------
void SceneBuilder::BuildKeyFrameController(INode *node, PX2::Movable *movable)
{
	assertion(node!=0, "node must exist.");
	assertion(movable!=0, "movable must exist.");

	// ��⵱ǰ�ڵ��Ƿ��Ƕ���
	Control *tmCtl = node->GetTMController();
	assertion(tmCtl!=0, "must contains time.");

	if (!tmCtl->IsKeyable())
	{
		// �ڵ㲻��keyframe����������Ȼ�Ƕ����ġ�������˵����������һ��IK��
		// �㡣������������£������ڵ������frames��
		BuildFrameController(node, movable);
		return;
	}

	// �����Ż������ڵ��ƽ�ƣ���ת�������Ƿ��������ʱ��ı仯���仯��
	AnimationTiming trnTiming, rotTiming, scaTiming;
	if (!GetAnimationTiming(node, trnTiming, rotTiming, scaTiming)) 
	{
		return;
	}

	// �ؼ�֡��Ϣ��
	std::vector<KeyInfo*> keyInfos;

	// �ռ�ƽ�Ʊ任����Ϣ
	int numTrnKeys = 0;
	if (trnTiming.Active)
	{
		Control* ctl = tmCtl->GetPositionController();
		if (ctl)
		{
			IKeyControl* keyCtl = GetKeyControlInterface(ctl);
			if (keyCtl)
			{
				numTrnKeys = keyCtl->GetNumKeys();
				GetTrnKeyInfo(numTrnKeys, ctl->ClassID(), keyCtl, trnTiming,
					keyInfos);
			}
		}
	}

	// �ռ���ת�任��Ϣ
	int numRotKeys = 0;
	if (rotTiming.Active)
	{
		Control *ctl = tmCtl->GetRotationController();
		if (ctl)
		{
			IKeyControl *keyCtl = GetKeyControlInterface(ctl);
			if (keyCtl)
			{
				numRotKeys = keyCtl->GetNumKeys();
				GetRotKeyInfo(numRotKeys, ctl->ClassID(), keyCtl, rotTiming,
					keyInfos);
			}
		}
	}

	// �ռ����ű任��Ϣ
	int numScaKeys = 0;
	if (scaTiming.Active)
	{
		Control* ctl = tmCtl->GetScaleController();
		if (ctl)
		{
			IKeyControl* keyCtl = GetKeyControlInterface(ctl);
			if (keyCtl)
			{
				numScaKeys = keyCtl->GetNumKeys();
				GetScaleKeyInfo(numScaKeys, ctl->ClassID(), keyCtl, scaTiming,
					keyInfos);
			}
		}
	}

	// �������ʹ�ó��������������ʹ��һ��Ŀ���������������
	// GetKeyControlInterface(...)�����ĵ��û᷵��null����Ϊ�ڳ����������û��
	// "keys"��
	if ( 0==numTrnKeys && 0==numRotKeys && 0==numScaKeys)
	{
		BuildFrameController(node, movable);
		return;
	}

	// ��KeyInfo���尴��ʱ������
	std::sort(keyInfos.begin(), keyInfos.end(), CompareKeyTimes);

	// ��ʼ���ؼ��п���������
	PX2::KeyframeController *kfc = new0 PX2::KeyframeController
		(0, numTrnKeys, numRotKeys, numScaKeys, movable->LocalTransform);
	kfc->Repeat = PX2::Controller::RT_NONE;
	PX2::APoint *tKey = kfc->GetTranslations();
	float *tTime = kfc->GetTranslationTimes();

	PX2::HQuaternion *rKey = kfc->GetRotations();
	float *rTime = kfc->GetRotationTimes();

	PX2::Float3 *sKey = kfc->GetScales();
	float *sTime = kfc->GetScaleTimes();

	TimeValue timeNow = -1;
	PX2::Transform trans;
	for (int i=0; i<(int)keyInfos.size(); i++)
	{
		KeyInfo *info = keyInfos[i];

		if (timeNow != info->Time)
		{
			timeNow = info->Time;
			trans = GetLocalTransform(node, timeNow);
		}

		switch (info->Type)
		{
		case KT_TRANSLATION:
			*tKey++ = trans.GetTranslate();
			*tTime++ = TicksToSec(timeNow - mTimeOffset);
			break;
		case KT_ROTATION:
			if (trans.IsRSMatrix())
			{
				rKey++->FromRotationMatrix(trans.GetRotate());
				*rTime++ = TicksToSec(timeNow - mTimeOffset);
			}
			break;
		case KT_SCALE:
			// ������ͬһ���Ż���Ϊ�������ǻᷢ������
			PX2::Float3 scale3;
			if (trans.IsUniformScale())
			{
				float scale = trans.GetUniformScale();
				scale3 = PX2::Float3(scale, scale, scale);
			}
			else
			{
				scale3 = PX2::Float3(
					PX2::Mathf::FAbs(trans.GetScale().X()),
					PX2::Mathf::FAbs(trans.GetScale().Y()),
					PX2::Mathf::FAbs(trans.GetScale().Z()));
			}

			*sKey++ = scale3;
			*sTime++ = TicksToSec(timeNow - mTimeOffset);
			break;
		}

		delete0(info);
	}

	kfc->MinTime = TicksToSec(mTimeStart - mTimeOffset);
	kfc->MaxTime = TicksToSec(mTimeEnd - mTimeOffset);
	
	movable->AttachController(kfc);
}
//----------------------------------------------------------------------------
void SceneBuilder::BuildFrameController (INode *node, PX2::Movable *movable)
{
	// ����һ��"frame controller"��һ�������ڵ㡣���������������֡������
	// �ǹؼ�֡���ڳ����У�Phoenix2���ڸ���������֮֡����в�ֵ��Ȼ����ĳЩ
	// ����¶���ʹ��IK���������������ǰ��֧�֡�
	// node:
	//		��Max�����ڵ���Ķ����ڵ㡣
	// movable:
	//		��Phoenix2������Ķ����ڵ㡣

	std::vector<int> ttTime, rrTime, ssTime;
	std::vector<Point3> ttData, ssData;
	std::vector<Quat> rrData;

	int time = mTimeStart;
	Matrix3 local = node->GetNodeTM(time) * 
		Inverse(node->GetParentTM(time));
	AffineParts affParts;
	decomp_affine(local, &affParts);
	ttTime.push_back(time);
	rrTime.push_back(time);
	ssTime.push_back(time);
	ttData.push_back(affParts.t);
	rrData.push_back(affParts.q);
	ssData.push_back(affParts.k);

	// ��ÿһ֡����һ������
	for (time+=mTicksPerFrame; time<=mTimeEnd; 
		time+=mTicksPerFrame)
	{
		local = node->GetNodeTM(time) * 
			Inverse(node->GetParentTM(time));
		decomp_affine(local, &affParts);

		if (!AreEqual(affParts.t, ttData.back()))
		{
			ttTime.push_back(time);
			ttData.push_back(affParts.t);
		}

		if (!AreEqual(affParts.q, rrData.back()))
		{
			// q��-q������ͬ����ת
			if (!AreEqual(-affParts.q, rrData.back()))
			{
				rrTime.push_back(time);

				// Ϊ��ת����Ԫ���洢����ĽǶ�?
				float dot = affParts.q % rrData.back();
				if (dot > 0.0f)
				{
					rrData.push_back(affParts.q);
				}
				else
				{
					rrData.push_back(-affParts.q);
				}
			}
		}

		if (!AreEqual(affParts.k, ssData.back()))
		{
			ssTime.push_back(time);
			ssData.push_back(affParts.f * affParts.k);
		}

		// ȷ�������Ƿ���ͬһ����
		bool scaleIsUniform =
			fabsf(affParts.k.x - affParts.k.y) < MIN_DIFFERENCE &&
			fabsf(affParts.k.y - affParts.k.z) < MIN_DIFFERENCE;

		// ȷ���Ƿ��ǵ�λ����
		bool scaleIsUnity = scaleIsUniform &&
			fabsf(affParts.k.x - 1.0f) < MIN_DIFFERENCE;

		// ȷ�����������Ƿ��ǵ�λ
		if (affParts.u.w < 0.0f)
		{
			affParts.u = -affParts.u;
		}

		bool orientIsIdentity = scaleIsUniform || (
			fabsf(affParts.u.w - 1.0f) < MIN_DIFFERENCE &&
			fabsf(affParts.u.x) < MIN_DIFFERENCE &&
			fabsf(affParts.u.y) < MIN_DIFFERENCE &&
			fabsf(affParts.u.z) < MIN_DIFFERENCE);

		PX2_UNUSED(scaleIsUnity);
		PX2_UNUSED(orientIsIdentity);
	}

	if (ttData.size()==1 && rrData.size()==1 && ssData.size()==1)
	{
		return;
	}

	int numTKeys = (int)ttData.size();
	int numRKeys = (int)rrData.size();
	int numSKeys = (int)ssData.size();

	PX2::KeyframeController *kfc = new0 PX2::KeyframeController(0, numTKeys,
		numRKeys, numSKeys, movable->LocalTransform);
	kfc->MinTime = TicksToSec(mTimeStart - mTimeOffset);
	kfc->MaxTime = TicksToSec(mTimeEnd - mTimeOffset);
	kfc->Repeat = PX2::Controller::RT_NONE;
	PX2::APoint *tKey = kfc->GetTranslations();
	float *tTime = kfc->GetTranslationTimes();
	PX2::HQuaternion *rKey = kfc->GetRotations();
	float *rTime = kfc->GetRotationTimes();
	PX2::Float3 *sKey = kfc->GetScales();
	float *sTime = kfc->GetScaleTimes();

	// t
	for (int i=0; i<numTKeys; i++)
	{
		tTime[i] = TicksToSec(ttTime[i] - mTimeOffset);
		tKey[i].X() = ttData[i][0];
		tKey[i].Y() = ttData[i][1];
		tKey[i].Z() = ttData[i][2];
	}

	// r
	for (int i=0; i<numRKeys; i++)
	{
		rTime[i] = TicksToSec(rrTime[i] - mTimeOffset);
		rKey[i].W() = rrData[i].w;
		rKey[i].X() = -rrData[i].x;
		rKey[i].Y() = -rrData[i].y;
		rKey[i].Z() = -rrData[i].z;
	}

	// s
	for (int i=0; i<numSKeys; i++)
	{
		sTime[i] = TicksToSec(ssTime[i] - mTimeOffset);
		sKey[i] = PX2::Float3(ssData[i].x, ssData[i].y, ssData[i].z);
	}

	movable->AttachController(kfc);
}
//----------------------------------------------------------------------------
bool SceneBuilder::GetAnimationTiming(INode *node,
									  SceneBuilder::AnimationTiming &tTiming, 
									  SceneBuilder::AnimationTiming &rTiming, 
									  SceneBuilder::AnimationTiming &sTiming)
{
	// ���ڵ�ķ�λ�Ƿ��������ʱ��ı仯���仯��

	int ticksPerFrame = GetTicksPerFrame();
	Matrix3 nodeTM;
	AffineParts affParts;
	Point3 prevTrans, rotAxis, prevRotAxis, prevScaleFactor;
	float rotAngle, prevRotAngle;
	tTiming.Active = FALSE;
	rTiming.Active = FALSE;
	sTiming.Active = FALSE;

	// ��ýڵ�����任�����任���зֽ�
	nodeTM = node->GetNodeTM(mTimeStart) *
		Inverse(node->GetParentTM(mTimeStart));
	decomp_affine(nodeTM, &affParts);
	AngAxisFromQ(affParts.q, &prevRotAngle, prevRotAxis);
	prevTrans = affParts.t;
	prevScaleFactor = affParts.k;

	// ��������֡�����任
	TimeValue timeNow = mTimeStart + ticksPerFrame;
	for (; timeNow<=mTimeEnd; timeNow+=ticksPerFrame)
	{
		nodeTM = node->GetNodeTM(timeNow)*
			Inverse(node->GetParentTM(timeNow));
		decomp_affine(nodeTM, &affParts);
		AngAxisFromQ(affParts.q, &rotAngle, rotAxis);

		// t
		if (!tTiming.Active)
		{
			if (!AreEqual(affParts.t, prevTrans))
			{
				tTiming.Active = TRUE;
				tTiming.Start = timeNow - ticksPerFrame;
				tTiming.End = timeNow;
			}
		}
		else
		{
			if (!AreEqual(affParts.t, prevTrans))
			{
				tTiming.End = timeNow;
			}
		}

		// r
		if (!rTiming.Active)
		{
			if (fabsf(rotAngle - prevRotAngle) > MIN_DIFFERENCE
				|| !AreEqual(rotAxis, prevRotAxis))
			{
				rTiming.Active = TRUE;
				rTiming.Start = timeNow - ticksPerFrame;
				rTiming.End = timeNow;
			}
		}
		else
		{
			if (fabsf(rotAngle - prevRotAngle) > MIN_DIFFERENCE
				|| !AreEqual(rotAxis, prevRotAxis))
			{
				rTiming.End = timeNow;
			}
		}

		// s
		if (!sTiming.Active)
		{
			if (!AreEqual(affParts.k, prevScaleFactor))
			{
				sTiming.Active = TRUE;
				sTiming.Start = timeNow - ticksPerFrame;
				sTiming.End = timeNow;
			}
		}
		else
		{
			if (!AreEqual(affParts.k, prevScaleFactor))
			{
				sTiming.End = timeNow;
			}
		}

		prevRotAngle = rotAngle;
		prevRotAxis = rotAxis;
		prevTrans = affParts.t;
		prevScaleFactor = affParts.k;
	}

	return tTiming.Active || rTiming.Active || sTiming.Active;
}
//----------------------------------------------------------------------------
void SceneBuilder::GetTrnKeyInfo(int &numKeys, Class_ID classID,
								 IKeyControl *keyCtl, AnimationTiming &tTiming, 
								 std::vector<KeyInfo*> &keyInfos)
{
	// ��ȡMax�ڵ�λ�ñ任��������λ�Ʊ任�ؼ�֡
	//
	// numKeys:
	//		��Ϊ�����������ʾҪ����Ĺؼ�֡��������Ϊ�����������ʾ��֮��
	// ƥ���Trudant�еĹؼ�֡��������
	// classID:
	//		Max��λ�ƶ����任���ؼ�֡��������Class_ID��
	// keyOn:
	//		Max�ڵ�ġ�λ�ƶ����任���ؼ�֡�������ӿڡ�
	// tTiming:
	//		�����Žڵ㡰λ�ƶ����任��ʱ��ϸ�ڡ�
	// keyInfos:
	//		����KeyInfo������������洢�˹ؼ�֡�����ͺ�ʱ����Ϣ��

	int i, tempNumKeys = numKeys;
	numKeys = 0;

	if (classID == Class_ID(TCBINTERP_POSITION_CLASS_ID, 0))
	{
		// ����Tension/Continuity/Bias������
		ITCBPoint3Key key;
		for (i=0; i<tempNumKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time < tTiming.Start)
			{
				continue;
			}
			else if (key.time > tTiming.End)
			{
				break;
			}

			keyInfos.push_back(new0 KeyInfo(key.time, KT_ROTATION));
			numKeys++;
		}
	}
	else if (classID == Class_ID(HYBRIDINTERP_POSITION_CLASS_ID, 0))
	{
		// ����Bezier������
		IBezPoint3Key key;
		for (i=0; i<tempNumKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time < tTiming.Start)
				continue;
			else if (key.time > tTiming.End)
				break;

			keyInfos.push_back(new0 KeyInfo(key.time, KT_TRANSLATION));
			numKeys++;
		}
	}
	else if (classID == Class_ID(LININTERP_POSITION_CLASS_ID, 0))
	{
		// ���� Linear ������

		ILinPoint3Key key;
		for (int i=0; i<tempNumKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time < tTiming.Start)
				continue;
			else if (key.time > tTiming.End)
				break;

			keyInfos.push_back(new0 KeyInfo(key.time, KT_TRANSLATION));
			numKeys++;
		}
	}
}
//----------------------------------------------------------------------------
void SceneBuilder::GetRotKeyInfo (int &numKeys, Class_ID classID,
								  IKeyControl *keyCtl,
								  AnimationTiming &rTiming,
								  std::vector<KeyInfo*> &keyInfos)
{
	// ��ȡMax�ڵ�λ����ת��������λ�Ʊ任�ؼ�֡
	//
	// numKeys:
	//		��Ϊ�����������ʾҪ����Ĺؼ�֡��������Ϊ�����������ʾ��֮��
	// ƥ���Trudant�йؼ�֡��������
	// classID:
	//		Max����ת�����任���ؼ�֡��������class ID��
	// keyCtl:
	//		Max�ڵ�ġ���ת�����任���ؼ�֡�������ӿڡ�
	// rTiming:
	//		�����Žڵ㡰��ת�����任��ʱ��ϸ�ڡ�
	// keyInfos:
	//		����KeyInfo������������洢�˹ؼ�֡�����ͺ�ʱ����Ϣ��

	int i, tempNumKeys = numKeys;
	numKeys = 0;

	if (classID == Class_ID(TCBINTERP_ROTATION_CLASS_ID, 0))
	{
		// ���� Tension/Continuity/Bias ������

		ITCBRotKey key;
		for (i=0; i<tempNumKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time < rTiming.Start)
				continue;
			else if (key.time > rTiming.End)
				break;

			keyInfos.push_back(new0 KeyInfo(key.time, KT_ROTATION));
			numKeys++;
		}
	}
	else if (classID == Class_ID(HYBRIDINTERP_ROTATION_CLASS_ID, 0))
	{
		// ���� Bezier ������

		IBezQuatKey key;
		for (i=0; i<tempNumKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time < rTiming.Start)
				continue;
			else if (key.time > rTiming.End)
				break;

			keyInfos.push_back(new0 KeyInfo(key.time, KT_ROTATION));
			numKeys++;
		}
	}
	else if (classID == Class_ID(LININTERP_ROTATION_CLASS_ID, 0))
	{
		// ���� Linear ������

		ILinRotKey key;
		for (i=0; i<numKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time <rTiming.Start)
				continue;
			else if (key.time > rTiming.End)
				break;

			keyInfos.push_back(new0 KeyInfo(key.time, KT_ROTATION));
			numKeys++;
		}
	}
}
//----------------------------------------------------------------------------
void SceneBuilder::GetScaleKeyInfo(int &numKeys, Class_ID classID, 
								   IKeyControl *keyCtl, 
								   AnimationTiming &sTiming,
								   std::vector<KeyInfo*> &keyInfos)
{
	// ��ȡMax�ڵ����ű任�����������ű任�ؼ�֡
	//
	// numKeys:
	//		��Ϊ�����������ʾҪ����Ĺؼ�֡��������Ϊ�����������ʾ��֮��
	// ƥ���Trudant�йؼ�֡��������
	// classID:
	//		Max�����Ŷ����任���ؼ�֡��������class ID��
	// keyCtl:
	//		Max�ڵ�ġ����Ŷ����任���ؼ�֡�������ӿڡ�
	// tTiming:
	//		�����Žڵ㡰���Ŷ����任��ʱ��ϸ�ڡ�
	// keyInfos:
	//		����KeyInfo������������洢�˹ؼ�֡�����ͺ�ʱ����Ϣ��

	int i, tempNumKeys = numKeys;
	numKeys = 0;

	if (classID == Class_ID(TCBINTERP_SCALE_CLASS_ID, 0))
	{
		// ���� Tension/Continuity/Bias ������
		ITCBRotKey key;
		for (i=0; i<tempNumKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time < sTiming.Start)
				continue;
			else if (key.time > sTiming.End)
				break;

			keyInfos.push_back(new0 KeyInfo(key.time, KT_SCALE));
			numKeys++;
		}
	}
	else if (classID == Class_ID(HYBRIDINTERP_SCALE_CLASS_ID, 0))
	{
		// ���� Bezier ������
		IBezScaleKey key;
		for (i=0; i<tempNumKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time < sTiming.Start)
				continue;
			else if (key.time > sTiming.End)
				break;

			keyInfos.push_back(new0 KeyInfo(key.time, KT_SCALE));
			numKeys++;
		}
	}
	else if (classID == Class_ID(LININTERP_SCALE_CLASS_ID, 0))
	{
		// ���� Linear ������
		ILinScaleKey key;
		for (i=0; i<tempNumKeys; i++)
		{
			keyCtl->GetKey(i, &key);
			if (key.time < sTiming.Start)
				continue;
			else if (key.time > sTiming.End)
				break;

			keyInfos.push_back(new0 KeyInfo(key.time, KT_SCALE));
			numKeys++;
		}
	}
}
//----------------------------------------------------------------------------