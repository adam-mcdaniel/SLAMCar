// PX2SkinController.cpp

#include "PX2SkinController.hpp"
#include "PX2Renderer.hpp"
#include "PX2VertexBufferAccessor.hpp"
#include "PX2Renderable.hpp"
#include "PX2GraphicsRoot.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, Controller, SkinController);
PX2_IMPLEMENT_STREAM(SkinController);
PX2_IMPLEMENT_FACTORY(SkinController);
PX2_IMPLEMENT_DEFAULT_NAMES(Controller, SkinController);

//----------------------------------------------------------------------------
SkinController::SkinController (int numVertices, int numBones)
:
mNumVertices(numVertices),
mNumBones(numBones),
mOriginPoses(0),
mGPUMatrixs(0),
mIsUseCPU(false)
{
	mBones = new1<Node*>(mNumBones);
	mWeights = new2<float>(mNumBones, mNumVertices);
	mOffsets = new2<APoint>(mNumBones, mNumVertices);
	mTMMatrixs = new1<HMatrix>(mNumBones);
	mGPUMatrixs = new1<HMatrix>(mNumBones);

	SetName("SkinController");
}
//----------------------------------------------------------------------------
SkinController::~SkinController ()
{
	delete1(mBones);
	delete2(mWeights);
	delete2(mOffsets);
	delete1(mTMMatrixs);

	delete1(mOriginPoses);
	delete1(mGPUMatrixs);
}
//----------------------------------------------------------------------------
bool SkinController::Update(double applicationTime, double elapsedTime)
{
	if (!Controller::Update(applicationTime, elapsedTime))
	{
		return false;
	}

	// ���Ҫ���ʵ�VertexBuffer
	Renderable* renderable = StaticCast<Renderable>(mObject);
	renderable->WorldTransform = Transform::IDENTITY;
	renderable->WorldTransformIsCurrent = true;
	renderable->IsSkinCtrlSetWroldTrans = true;

	if (mIsUseCPU)
	{
		assertion(mNumVertices == renderable->GetVertexBuffer()->GetNumElements(),
			"Controller must have the same number of vertices as the buffer\n");
		VertexBufferAccessor vba(renderable);

		if (!mOriginPoses)
		{
			mOriginPoses = new1<APoint>(mNumVertices);
			for (int vertex=0; vertex<mNumVertices; ++vertex)
			{
				mOriginPoses[vertex] = vba.Position<Float3>(vertex);
			}
		}

		// skin�е�����Ķ��������������ϵ����������ϵ���С�mOffsets��¼������Ը�
		// ��������λ�ã���˹����任����õ��ù����µ���������ϵλ�á�

		// ����skin����λ��
		for (int vertex = 0; vertex < mNumVertices; ++vertex)
		{
			APoint position = APoint::ORIGIN;
			for (int bone = 0; bone < mNumBones; ++bone)
			{
				float weight = mWeights[vertex][bone];

				if (weight != 0.0f)
				{
					APoint offset = mOffsets[vertex][bone];
					APoint worldOffset;

					worldOffset = mBones[bone]->WorldTransform*offset;

					position += weight*worldOffset;
				}
			}
			vba.Position<Float3>(vertex) = position;
		}

		//renderable->  (Renderable::GU_NORMALS);
		Renderer::UpdateAll(renderable->GetVertexBuffer());
	}
	else
	{
		for (int bone = 0; bone < mNumBones; ++bone)
		{
			if (mBones[bone])
			{
				const HMatrix &boneMat = mBones[bone]->WorldTransform.Matrix();
				mGPUMatrixs[bone] = boneMat * mTMMatrixs[bone];
			}
		}
	}

	for (int i = 0; i < mNumBones; i++)
	{
		if (mBones[i])
		{
			const APoint &parentPos = mBones[i]->GetParent()->WorldTransform.GetTranslate();
			const APoint &bonePos = mBones[i]->WorldTransform.GetTranslate();

			Movable *mov = DynamicCast<Movable>(GetControlledable());
			if (mov)
			{
				Canvas *canvas = mov->GetFirstParentDerivedFromType<Canvas>();
				if (canvas)
				{
					//canvas->AddDebugLine(parentPos, bonePos, Float4::WHITE);
				}
			}
		}
	}

	return true;
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// Property
//----------------------------------------------------------------------------
void SkinController::RegistProperties ()
{
	Controller::RegistProperties();

	AddPropertyClass("SkinController");
	
	AddProperty("NumVertices", Object::PT_INT, GetNumVertices(), false);
	AddProperty("NumBones", Object::PT_INT, GetNumBones(), false);
}
//----------------------------------------------------------------------------
void SkinController::OnPropertyChanged (const PropertyObject &obj)
{
	Controller::OnPropertyChanged(obj);
}
//----------------------------------------------------------------------------
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// �־û�
//----------------------------------------------------------------------------
SkinController::SkinController (LoadConstructor value)
:
Controller(value),
mNumVertices(0),
mNumBones(0),
mBones(0),
mWeights(0),
mOffsets(0),
mOriginPoses(0),
mGPUMatrixs(0),
mIsUseCPU(false)
{
}
//----------------------------------------------------------------------------
void SkinController::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	Controller::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mNumVertices);
	source.Read(mNumBones);

	int numWeightsOffsets = mNumVertices*mNumBones;
	mWeights = new2<float>(mNumBones, mNumVertices);
	mOffsets = new2<APoint>(mNumBones, mNumVertices);
	mTMMatrixs = new1<HMatrix>(mNumBones);
	source.ReadVV(numWeightsOffsets, mWeights[0]);
	source.ReadAggregateVV(numWeightsOffsets, mOffsets[0]);
	source.ReadPointerVR(mNumBones, mBones);

	source.ReadAggregateVV(mNumBones, mTMMatrixs);

	PX2_END_DEBUG_STREAM_LOAD(SkinController, source);
}
//----------------------------------------------------------------------------
void SkinController::Link (InStream& source)
{
	Controller::Link(source);

	source.ResolveLink(mNumBones, mBones);
}
//----------------------------------------------------------------------------
void SkinController::PostLink ()
{
	Controller::PostLink();

	mGPUMatrixs = new1<HMatrix>(mNumBones);
}
//----------------------------------------------------------------------------
bool SkinController::Register (OutStream& target) const
{
	if (Controller::Register(target))
	{
		target.Register(mNumBones, mBones);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void SkinController::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	Controller::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mNumVertices);
	target.Write(mNumBones);

	int numWeightsOffsets = mNumVertices*mNumBones;
	target.WriteN(numWeightsOffsets, mWeights[0]);
	target.WriteAggregateN(numWeightsOffsets, mOffsets[0]);

	target.WritePointerN(mNumBones, mBones);

	target.WriteAggregateN(mNumBones, mTMMatrixs);

	PX2_END_DEBUG_STREAM_SAVE(SkinController, target);
}
//----------------------------------------------------------------------------
int SkinController::GetStreamingSize (Stream &stream) const
{
	int size = Controller::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += sizeof(mNumVertices);
	size += sizeof(mNumBones);

	int numWeightsOffsets = mNumVertices*mNumBones;
	size += numWeightsOffsets*sizeof(mWeights[0][0]);
	size += numWeightsOffsets*sizeof(mOffsets[0][0]);

	size += mNumBones*PX2_POINTERSIZE(mBones[0]);
	size += mNumBones*sizeof(HMatrix);

	return size;
}
//----------------------------------------------------------------------------