// PX2Culler.cpp

#include "PX2Culler.hpp"
#include "PX2Renderable.hpp"
#include "PX2Light.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
Culler::Culler() :
mPlaneQuantity(6),
mFlag_CastShadow(0),
mCurCullingScene(0),
mCamera(0)
{
}
//----------------------------------------------------------------------------
Culler::Culler (const Camera* camera) :
mCamera(camera),
mPlaneQuantity(6),
mFlag_CastShadow(0),
mCurCullingScene(0)
{
}
//----------------------------------------------------------------------------
Culler::~Culler ()
{
}
//----------------------------------------------------------------------------
void Culler::SetFrustum (const float* frustum)
{
	if (!mCamera)
	{
		assertion(false, "SetFrustum requires the existence of a camera\n");
		return;
	}

	// ������ͷ���ֵ
	mFrustum[Camera::VF_DMIN] = frustum[Camera::VF_DMIN];
	mFrustum[Camera::VF_DMAX] = frustum[Camera::VF_DMAX];
	mFrustum[Camera::VF_UMIN] = frustum[Camera::VF_UMIN];
	mFrustum[Camera::VF_UMAX] = frustum[Camera::VF_UMAX];
	mFrustum[Camera::VF_RMIN] = frustum[Camera::VF_RMIN];
	mFrustum[Camera::VF_RMAX] = frustum[Camera::VF_RMAX];

	float dMin2 = mFrustum[Camera::VF_DMIN]*mFrustum[Camera::VF_DMIN];
	float uMin2 = mFrustum[Camera::VF_UMIN]*mFrustum[Camera::VF_UMIN];
	float uMax2 = mFrustum[Camera::VF_UMAX]*mFrustum[Camera::VF_UMAX];
	float rMin2 = mFrustum[Camera::VF_RMIN]*mFrustum[Camera::VF_RMIN];
	float rMax2 = mFrustum[Camera::VF_RMAX]*mFrustum[Camera::VF_RMAX];

	// ��������coordinate frame.
	APoint position = mCamera->GetPosition();
	AVector dVector = mCamera->GetDVector();
	AVector uVector = mCamera->GetUVector();
	AVector rVector = mCamera->GetRVector();
	float dirDotEye = position.Dot(dVector);

	// ���½��ü���
	mPlane[Camera::VF_DMIN].SetNormal(dVector);
	mPlane[Camera::VF_DMIN].SetConstant(
		dirDotEye + mFrustum[Camera::VF_DMIN]);

	// ����Զ�ü���
	mPlane[Camera::VF_DMAX].SetNormal(-dVector);
	mPlane[Camera::VF_DMAX].SetConstant(
		-(dirDotEye + mFrustum[Camera::VF_DMAX]));

	// ���µײü���
	float invLength = Mathf::InvSqrt(dMin2 + uMin2);
	float c0 = -mFrustum[Camera::VF_UMIN]*invLength;  // D component
	float c1 = +mFrustum[Camera::VF_DMIN]*invLength;  // U component
	AVector normal = c0*dVector + c1*uVector;
	float constant = position.Dot(normal);
	mPlane[Camera::VF_UMIN].SetNormal(normal);
	mPlane[Camera::VF_UMIN].SetConstant(constant);

	// ���¶��ü���
	invLength = Mathf::InvSqrt(dMin2 + uMax2);
	c0 = +mFrustum[Camera::VF_UMAX]*invLength;  // D component
	c1 = -mFrustum[Camera::VF_DMIN]*invLength;  // U component
	normal = c0*dVector + c1*uVector;
	constant = position.Dot(normal);
	mPlane[Camera::VF_UMAX].SetNormal(normal);
	mPlane[Camera::VF_UMAX].SetConstant(constant);

	// ������ü���
	invLength = Mathf::InvSqrt(dMin2 + rMin2);
	c0 = -mFrustum[Camera::VF_RMIN]*invLength;  // D component
	c1 = +mFrustum[Camera::VF_DMIN]*invLength;  // R component
	normal = c0*dVector + c1*rVector;
	constant = position.Dot(normal);
	mPlane[Camera::VF_RMIN].SetNormal(normal);
	mPlane[Camera::VF_RMIN].SetConstant(constant);

	// �����Ҳü���
	invLength = Mathf::InvSqrt(dMin2 + rMax2);
	c0 = +mFrustum[Camera::VF_RMAX]*invLength;  // D component
	c1 = -mFrustum[Camera::VF_DMIN]*invLength;  // R component
	normal = c0*dVector + c1*rVector;
	constant = position.Dot(normal);
	mPlane[Camera::VF_RMAX].SetNormal(normal);
	mPlane[Camera::VF_RMAX].SetConstant(constant);

	// ���вü��涼������
	mPlaneState = 0xFFFFFFFF;
}
//----------------------------------------------------------------------------
void Culler::Insert (Renderable* visible)
{
	mVisibleSet.Insert(visible);
}
//----------------------------------------------------------------------------
bool Culler::IsVisible (const Bound& bound)
{
	if (bound.GetRadius() == 0.0f)
	{
		return false;
	}

	// �����һ��ƽ�濪ʼ�Ƚ�
	int index = mPlaneQuantity - 1;
	unsigned int mask = (1 << index);

	for (int i = 0; i < mPlaneQuantity; ++i, --index, mask >>= 1)
	{
		if (mPlaneState & mask)
		{
			int side = bound.WhichSide(mPlane[index]);

			if (side < 0)
			{
				// �����ڲü��ķ��棬����ü���
				return false;
			}

			if (side > 0)
			{
				// �����ڲü������棬û�б�Ҫ�����Ƚ��Ӷ���
				mPlaneState &= ~mask;
			}
		}
	}

	return true;
}
//----------------------------------------------------------------------------
void Culler::Clear()
{
	mVisibleSet.Clear();
}
//----------------------------------------------------------------------------
void Culler::ComputeVisibleSet (Movable* scene)
{
	if (mCamera && scene)
	{
		mCurCullingScene = scene;

		SetFrustum(mCamera->GetFrustum());
		mVisibleSet.Clear();
		scene->GetVisibleSet(*this, false);
	}
	else
	{
		assertion(false, "A camera and a scene are required for culling\n");
	}
}
//----------------------------------------------------------------------------