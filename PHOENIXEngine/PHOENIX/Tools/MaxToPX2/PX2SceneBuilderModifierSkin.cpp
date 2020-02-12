// PX2SceneBuilderSkinModifier.cpp

#include "PX2SceneBuilder.hpp"
using namespace PX2;

//----------------------------------------------------------------------------
struct In 
{ 
	float data; 
	int index; 
};
//----------------------------------------------------------------------------
int cmp( const void *a ,const void *b)
{ 
	return (*(In *)b).data > (*(In *)a).data ? 1 : -1; 

} 
//----------------------------------------------------------------------------
void SceneBuilder::ProcessSkin(INode *node, Modifier *skinMod)
{
	// ����Ƥ�������������Max�����񱻰��ղ���ϸ�֣�ÿһ��������Ҫ�Լ�����Ƥ
	// ��Ϣ����������Ƥ��Ϣ�е�offset���ڶ�����ʼʱ�����㣬�ǹ���������任��
	//
	// node:
	//		ָ����Ƥ�޸���ָ���Max�еĽڵ㡣
	// skinMod:
	//		ָ����Ƥ�޸���

	// 1. ���max��Ƥ��Ϣ�еĹ�������Ӧ����Phoenix�Ĺ����ڵ��б�
	// 2. ���maxNodeӰ���Phoenix����
	// 3. ���max��ÿ��������Ӱ���Phoenix�����еĶ�������������Բ�����Ƥ��Ϣ
	//	  Ӱ�������
	// 4. ����Phoenix mesh����Ƥ��Ϣ������SkinControl��AttachController��
	//    Phoenix mesh�ϡ�
	
	// 1

	bool needDel;
	TriObject *triObj = GetTriObject(node, &needDel);
	Mesh *maxMesh = &triObj->GetMesh();

	// MaxƤ���������ӿ�
	ISkin *skin = (ISkin*)skinMod->GetInterface(I_SKIN);
	ISkinContextData *skinData = skin->GetContextInterface(node);

	// max Skin Bones -> Phoenix2 Skin Bones
	int b, numSkinBone = skin->GetNumBones();
	PX2::Node **bones = new1<PX2::Node*>(numSkinBone);
	for (b=0; b<numSkinBone; b++)
	{
		INode *boneNode = skin->GetBone(b);
		const std::string &boneName = boneNode->GetName();
		if (!boneName.empty())
		{
			std::vector<PX2::Object *> nodes;
			mScene->GetAllObjectsByName(boneName, nodes);
			if (1 == nodes.size())
			{
				bones[b] = DynamicCast<Node>(nodes[0]);
			}
			else
			{
				assertion(false, "");
			}

			//PX2::Node *node = PX2::StaticCast<PX2::Node>(mScene->GetObjectByName(boneName));
			//bones[b] = node;
		}
		else
		{
			assertion(false, "");
		}
	}

	// 1

	// ���maxNode�������Phoenix mesh
	std::vector<PX2::TriMesh*> meshes;
	PX2::Object *object = mScene->GetObjectByName(node->GetName());
	if (object->IsExactly(PX2::TriMesh::TYPE))
	{
		meshes.push_back(PX2::StaticCast<PX2::TriMesh>(object));
	}
	else
	{
		PX2::Node *node = PX2::StaticCast<PX2::Node>(object);
		const char *nName = node->GetName().c_str();
		for (int c=0; c<node->GetNumChildren(); c++)
		{
			PX2::Movable *child = node->GetChild(c);
			const char *cName = child->GetName().c_str();
			if (strncmp(cName, nName, strlen(nName)) == 0) // ���������strlen(nName),��Ϊ�ӽڵ���_1,_2
			{
				meshes.push_back(PX2::StaticCast<PX2::TriMesh>(child));
			}
		}
	}

	// ΪPhoenix2��ÿ����������ص�Ƥ��������

	int *boneInfuseNumVert = new1<int>(numSkinBone);
	for (int m=0; m<(int)meshes.size(); m++)
	{
		PX2::TriMesh *mesh = meshes[m];

		// Phoenix������max�����е�����
		PX2::VertexBuffer *vb = mesh->GetVertexBuffer();
		int px2MeshVertexNum = vb->GetNumElements();
		std::vector<int> MaxVertexIndex; // i->max����
		int v, i, j, k;

		PX2::VertexBufferAccessor vba(mesh->GetVertexFormat(), vb);

		// 3

		for (int v=0; v<px2MeshVertexNum; ++v)
		{
			Float3 &position = vba.Position<Float3>(v);
			for (i=0; i<maxMesh->getNumVerts(); i++)
			{
				if (position[0] == maxMesh->verts[i].x
					&& position[1] == maxMesh->verts[i].y 
					&& position[2] == maxMesh->verts[i].z)
				{
					MaxVertexIndex.push_back(i);
					break;
				}
			}
		}

		// ȷ��ÿ��������Ӱ��Ķ�������
		int maxVertexSize = (int)MaxVertexIndex.size();
		memset(boneInfuseNumVert, 0, sizeof(int)*numSkinBone);
		for (i=0; i<maxVertexSize; i++)
		{
			v = MaxVertexIndex[i];
			for (j=0; j<skinData->GetNumAssignedBones(v); j++)
			{ // ���max�еĶ����ܵ���������Ӱ�찡����
				b = skinData->GetAssignedBone(v, j); // ������Ӱ��Ĺ�������(���j�ǵڼ���)
				boneInfuseNumVert[b]++; // �������Ӱ��Ķ�������++
			}
		}

		// (ͨ��PX2�еĶ����ҵ�Max�еĶ��㣬�ҵ�Max��Ӱ��ö���Ĺ���)

		// ���Max�������Ǳ����ղ��ʷָ�ģ�����һЩ�����Ե�ǰPhoenix2����û��
		// Ӱ��
		int bQuantity = 0; // Ӱ�쵱ǰPhoenix2����Ĺ�������
		for (b=0; b<numSkinBone; b++)
		{
			if (boneInfuseNumVert[b] > 0)
				bQuantity++;
		}

		if (bQuantity == 0)
		{
			// Phoenix���񲻱��κι���Ӱ�죬������һ������
			continue;
		}

		// 4

		PX2::Node **theBones = new1<PX2::Node*>(bQuantity);
		float **weight = new2<float>(bQuantity, maxVertexSize);
		memset(weight[0],0,bQuantity*maxVertexSize*sizeof(float));
		PX2::APoint **offset = new2<PX2::APoint>(bQuantity, maxVertexSize);
		memset(offset[0],0,bQuantity*maxVertexSize*sizeof(PX2::APoint));
		PX2::HMatrix *mats = new1<PX2::HMatrix>(bQuantity);

		// ����max������Phoenix������Ӧ������(k)
		std::vector<int> bIArray(numSkinBone);
		for (b=0, k=0; b<numSkinBone; b++)
		{
			if (boneInfuseNumVert[b] > 0)
			{
				theBones[k] = bones[b]; // ��ȡ��Mesh��Ӱ��Ĺ���
				bIArray[b] = k; // max bone index -> px2 ����bone index
				
				HMatrix boneWorldMat = theBones[k]->WorldTransform.Matrix();
				HMatrix meshWorldMat =  mesh->WorldTransform.Matrix();

				mats[k] = boneWorldMat.Inverse() * meshWorldMat;
				
				k++;
			}
		}

		// �������㣬���㶥��Ȩ�غ�offset
		for (i=0; i<maxVertexSize; i++)
		{
			v = MaxVertexIndex[i];
			for (j=0; j<skinData->GetNumAssignedBones(v); j++)
			{ // ����Ӱ���Max����Ĺ���
				b = skinData->GetAssignedBone(v, j);
				k = bIArray[b];
				float wit = skinData->GetBoneWeight(v, j); // ��j��������Ӱ��Ȩ��
				weight[i][k] = wit;

				Float3 &position = vba.Position<Float3>(i);

				APoint point = theBones[k]->WorldTransform.Inverse() 
					* (mesh->WorldTransform * APoint(position));
				offset[i][k] = Float3(point.X(), point.Y(), point.Z()); // ������Ӱ������е�λ��
			}
		}

		PX2::SkinController *skinCtrl = new0 PX2::SkinController
			(maxVertexSize, bQuantity);
		skinCtrl->SetName("SkinController");
		for (int i=0; i<bQuantity; i++)
		{
			skinCtrl->GetBones()[i] = theBones[i];
			skinCtrl->GetTMMatrixs()[i] = mats[i];
		}

		// offset
		for (int i=0; i<maxVertexSize; i++)
		{
			for (int j=0; j<bQuantity; j++)
			{
				skinCtrl->GetWeights()[i][j] = weight[i][j];
				skinCtrl->GetOffsets()[i][j] = offset[i][j]; 
			}
		}

		// index weights
		for (int i=0; i<maxVertexSize; i++)
		{
			Float4 inds = Float4(0.0f, 0.0f, 0.0f, 0.0f);
			Float4 wights = Float4(0.0f, 0.0f, 0.0f, 0.0f);

			In *pSortBone = new In[bQuantity];
			for (int j=0; j<bQuantity; j++)
			{
				pSortBone[j].index = j;
				pSortBone[j].data = weight[i][j];
			}
			qsort(pSortBone, bQuantity, sizeof(pSortBone[0]), cmp);

			int useBoneNum = bQuantity;
			if (useBoneNum > 4)
				useBoneNum = 4;

			float allWeight = 0.0f;
			for (int useBoneIndex=0; useBoneIndex<useBoneNum; useBoneIndex++)
			{
				allWeight += pSortBone[useBoneIndex].data;
			}
			if (allWeight <= 0.0f)
				allWeight = 1.0f;

			for (int useBoneIndex=0; useBoneIndex<useBoneNum; useBoneIndex++)
			{
				inds[useBoneIndex] = (float)(pSortBone[useBoneIndex].index);
				wights[useBoneIndex] = pSortBone[useBoneIndex].data/allWeight;
			}

			vba.TCoord<Float4>(1, i) = inds;
			vba.TCoord<Float4>(2, i) = wights;

			delete [] pSortBone;
		}
		
		skinCtrl->Repeat = Controller::RT_NONE;
		skinCtrl->MinTime = 0.0f;
		skinCtrl->MaxTime = TicksToSec(mTimeEnd - mTimeStart);

		mesh->AttachController(skinCtrl);

		delete1(theBones);
		delete2(weight);
		delete2(offset);
		delete1(mats);
	}

	if (needDel)
	{
		delete0(triObj);
	}

	delete1(bones);
	delete1(boneInfuseNumVert);
}
//----------------------------------------------------------------------------