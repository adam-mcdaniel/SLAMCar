// PX2ClodMesh.cpp

#include "PX2ClodMesh.hpp"
#include "PX2Renderer.hpp"
using namespace PX2;

PX2_IMPLEMENT_RTTI(PX2, TriMesh, ClodMesh);
PX2_IMPLEMENT_STREAM(ClodMesh);
PX2_IMPLEMENT_FACTORY(ClodMesh);

//----------------------------------------------------------------------------
ClodMesh::ClodMesh (TriMesh* mesh, CollapseRecordArray* recordArray)
:
TriMesh(mesh->GetVertexFormat(), mesh->GetVertexBuffer(), 0),
mCurrentRecord(0),
mTargetRecord(0),
mRecordArray(recordArray)
{
	assertion(recordArray != 0, "Record array is needed for construction.\n");

	// ����һ�ݶ��������Ŀ���
	IndexBuffer* ibuffer = mesh->GetIndexBuffer();
	int numIndices = ibuffer->GetNumElements();
	int elementSize = ibuffer->GetElementSize();
	assertion(elementSize == 4, "Invalid indices.\n");

	char* srcIndices = ibuffer->GetData();
	mIBuffer = new0 IndexBuffer(numIndices, elementSize);
	char* trgIndices = mIBuffer->GetData();
	memcpy(trgIndices, srcIndices, numIndices*elementSize);
}
//----------------------------------------------------------------------------
ClodMesh::~ClodMesh ()
{
}
//----------------------------------------------------------------------------
void ClodMesh::SelectLevelOfDetail ()
{
	const CollapseRecord* records = mRecordArray->GetRecords();

	// ���Ŀ��record�������������д�˺����������Ҫ��Ŀ�꼶��
	int targetRecord = GetAutomatedTargetRecord();

	// �������Ҫ����������
	int* indices = (int*)mIBuffer->GetData();
	bool ibufferChanged = (mCurrentRecord != targetRecord);

	int i, c;
	while (mCurrentRecord < targetRecord)
	{
		++mCurrentRecord;

		// �滻��������
		const CollapseRecord& record = records[mCurrentRecord];
		for (i = 0; i < record.NumIndices; ++i)
		{
			c = record.Indices[i];
			assertion(indices[c] == record.VThrow,
				"Inconsistent record in SelectLevelOfDetail.\n");

			indices[c] = record.VKeep;
		}

		// ���ٶ�������
		mVBuffer->SetNumElements(record.NumVertices);

		// ��������������
		mIBuffer->SetNumElements(3*record.NumTriangles);
	}

	// ��չ����
	while (mCurrentRecord > targetRecord)
	{
		// �ָ���������
		const CollapseRecord& record = records[mCurrentRecord];
		for (i = 0; i < record.NumIndices; ++i)
		{
			c = record.Indices[i];
			assertion(indices[c] == record.VKeep,
				"Inconsistent record in SelectLevelOfDetail.\n");

			indices[c] = record.VThrow;
		}

		--mCurrentRecord;
		const CollapseRecord& prevRecord = records[mCurrentRecord];

		// ���Ӷ�������
		mVBuffer->SetNumElements(prevRecord.NumVertices);

		// ��������������
		mIBuffer->SetNumElements(3*prevRecord.NumTriangles);
	}

	if (ibufferChanged)
	{
		Renderer::UpdateAll(mIBuffer);
	}
}
//----------------------------------------------------------------------------
void ClodMesh::OnGetVisibleSet (Culler& culler, bool noCull)
{
	SelectLevelOfDetail();
	TriMesh::OnGetVisibleSet(culler, noCull);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// ����
//----------------------------------------------------------------------------
Object* ClodMesh::GetObjectByName (const std::string& name)
{
	Object* found = TriMesh::GetObjectByName(name);
	if (found)
	{
		return found;
	}

	PX2_GET_OBJECT_BY_NAME(mRecordArray, name, found);
	return 0;
}
//----------------------------------------------------------------------------
void ClodMesh::GetAllObjectsByName (const std::string& name,
									std::vector<Object*>& objects)
{
	TriMesh::GetAllObjectsByName(name, objects);

	PX2_GET_ALL_OBJECTS_BY_NAME(mRecordArray, name, objects);
}
//----------------------------------------------------------------------------

//----------------------------------------------------------------------------
// �־û�
//----------------------------------------------------------------------------
ClodMesh::ClodMesh (LoadConstructor value)
:
TriMesh(value),
mCurrentRecord(0),
mTargetRecord(0)
{
}
//----------------------------------------------------------------------------
void ClodMesh::Load (InStream& source)
{
	PX2_BEGIN_DEBUG_STREAM_LOAD(source);

	TriMesh::Load(source);
	PX2_VERSION_LOAD(source);

	source.Read(mCurrentRecord);
	source.Read(mTargetRecord);
	source.ReadPointer(mRecordArray);

	PX2_END_DEBUG_STREAM_LOAD(ClodMesh, source);
}
//----------------------------------------------------------------------------
void ClodMesh::Link (InStream& source)
{
	TriMesh::Link(source);

	source.ResolveLink(mRecordArray);
}
//----------------------------------------------------------------------------
void ClodMesh::PostLink ()
{
	TriMesh::PostLink();
}
//----------------------------------------------------------------------------
bool ClodMesh::Register (OutStream& target) const
{
	if (TriMesh::Register(target))
	{
		target.Register(mRecordArray);
		return true;
	}
	return false;
}
//----------------------------------------------------------------------------
void ClodMesh::Save (OutStream& target) const
{
	PX2_BEGIN_DEBUG_STREAM_SAVE(target);

	TriMesh::Save(target);
	PX2_VERSION_SAVE(target);

	target.Write(mCurrentRecord);
	target.Write(mTargetRecord);
	target.WritePointer(mRecordArray);

	PX2_END_DEBUG_STREAM_SAVE(ClodMesh, target);
}
//----------------------------------------------------------------------------
int ClodMesh::GetStreamingSize (Stream &stream) const
{
	int size = TriMesh::GetStreamingSize(stream);
	size += PX2_VERSION_SIZE(mVersion);
	size += sizeof(mCurrentRecord);
	size += sizeof(mTargetRecord);
	size += PX2_POINTERSIZE(mRecordArray);
	return size;
}
//----------------------------------------------------------------------------
