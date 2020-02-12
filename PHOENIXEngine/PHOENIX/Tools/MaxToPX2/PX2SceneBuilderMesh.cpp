// PX2SceneBuilderMesh.cpp

#include "PX2SceneBuilder.hpp"
#include "PX2UniMaterialMesh.hpp"
#include <set>

//----------------------------------------------------------------------------
TriObject *SceneBuilder::GetTriObject(INode* maxNode, bool *needDel)
{
	// ȷ��Max�Ľڵ��Ƿ���Mesh���塣
	//
	// pkNode:
	//		Ҫ���Ľڵ㡣
	// pbNeedDel:
	//		true����ʹ����֮�󣬵�������Ҫɾ�����ص�TriObject*��false������Ҫ��
	// ����mesh����ָ�룬���û����ڵ���ص�mesh������0��

	Object *obj = maxNode->EvalWorldState(mTimeStart).obj;
	if (!obj)
		return 0;

	if (!obj->CanConvertToType(Class_ID(TRIOBJ_CLASS_ID, 0)))
		return 0;

	TriObject *triObject = (TriObject*)obj->ConvertToType(mTimeStart,
		Class_ID(TRIOBJ_CLASS_ID,0));

	if (triObject == 0)
		return 0;
	
	*needDel = (triObject!=obj ? true : false);

	return triObject;
}
//----------------------------------------------------------------------------
PX2::Movable *SceneBuilder::BuildMesh(INode *maxNode,
									 PX2::Node *relatParentOrEqualNode, bool hasSkin)
{
	// ��Max����������������ת����һ�����߸���ĵȼ۵�Phoenix2����������
	//
	// maxNode��
	//		Max����ͼ�е�Mesh�ڵ㡣
	// relatParentOrEqualNode:
	//		��Phoenix2����ͼϵͳ�����´����ĸ��׽ڵ㡣
	// ������Phoenix2������ָ���µĺ��ӽڵ��ָ�룬���ָ��ֱ��ָ��TriMesh���壻
	// ������һ����link���ڵ㣬��link���Ķ������TriMesh����Max�еĶ������mesh��

	bool needDel = false;
	TriObject *triObject = GetTriObject(maxNode, &needDel);
	if (!triObject)
	{
		return 0;
	}

	Mesh *maxMesh = &triObject->GetMesh();

	Mtl *mtl = maxNode->GetMtl();
	int mtlIndex = mMtls.GetIndex(mtl);

	// �ж����Max�ļ���ͼ�νڵ��Ƿ��С��Ӽ���ͼ�νڵ㡱��������Ӽ���ͼ�νڵ�
	// isEqualNodeΪ�棬��֮Ϊ�١�
	// ���������ȣ��Ͳ���relatParentOrEqualNode�ˣ�����equalNode
	PX2::Movable *link = 0;
	bool isEqualNode = (relatParentOrEqualNode->GetName().length()>0 &&
		strcmp(maxNode->GetName(), relatParentOrEqualNode->GetName().c_str()) == 0);

	// maxName
	const char *maxName = maxNode->GetName();

	// ���ֻ��Ҫһ��Phoenix��Mesh��ʾMax��Mesh��ֱ�ӽ�Phoenix��Mesh���ӵ�
	// Phoenix�ĳ���ͼ�У����򣬴���һ��"link"�ڵ㣬�����ղ��ʷָ����Mesh
	// ����"link"�¡�
	int i;
	std::vector<UniMaterialMesh*> uMeshs;
	SplitGeometry(maxMesh, mtlIndex, uMeshs);
	if ((int)uMeshs.size() > 1)
	{
		if (!isEqualNode)
		{
			link = BuildNode(maxNode, relatParentOrEqualNode);
		}
		else
		{
			link = relatParentOrEqualNode;
		}

		assertion(link->IsDerived(PX2::Node::TYPE), "link must be a Node.");

		for (i=0; i<(int)uMeshs.size(); i++)
		{
			uMeshs[i]->SetExportSkin(hasSkin);

			PX2::TriMesh *triMesh = uMeshs[i]->ToTriMesh();

			if (triMesh)
			{
				char meshNumber[6];
				sprintf_s(meshNumber, 6, "_%d", i+1);
				size_t size = strlen(maxName) + strlen(meshNumber) + 1;
				char *tdName = new1<char>((int)size);
				strcpy_s(tdName, size, maxName);
				strcat_s(tdName, size, meshNumber);
				triMesh->SetName(tdName);
				delete1(tdName);
				((PX2::Node*)link)->AttachChild(triMesh);
			}
		}
	}
	else if ((int)uMeshs.size() == 1)
	{
		uMeshs[0]->SetExportSkin(hasSkin);

		PX2::TriMesh *triMesh = uMeshs[0]->ToTriMesh();
		if (triMesh)
		{
			if (!isEqualNode)
			{
				triMesh->SetName(maxName);
				triMesh->LocalTransform = GetLocalTransform(maxNode, mTimeStart);
			}
			else
			{
				size_t size = strlen(maxName) + 3;
				char *tdName = new1<char>((int)size);
				strcpy_s(tdName, size, maxName);
				strcat_s(tdName, size, "_1");
				triMesh->SetName(tdName);
				delete1(tdName);
			}

			assertion(relatParentOrEqualNode->IsDerived(PX2::Node::TYPE), 
				"relatParentOrEqualNode must be a Node.");

			relatParentOrEqualNode->AttachChild(triMesh);
			link = triMesh;
		}
	}

	for (i=0; i<(int)uMeshs.size(); i++)
	{
		delete0(uMeshs[i]);
	}

	if (needDel)
	{
		delete0(triObject);
	}

	return link;
}
//----------------------------------------------------------------------------
void SceneBuilder::SplitGeometry(Mesh *maxMesh, int mtlIndex, 
								 std::vector<UniMaterialMesh*> &uMeshes)
{
	// ������Mesh�ж��material����ʹ���������߶��material�����������Ҫ
	// ����֣���ΪPhoenix2��һ������ֻ��Ӧһ��material��
	//
	// maxMesh:
	//		Ҫ�ָ��Max����
	// mtlIndex:
	//		����ID
	// uMeshes:
	//		UniMaterialMesh���ϣ�Max��mesh����ֺ�����档

	int i, j;
	PX2::Float3 *normalsAll = 0;
	if (mSettings->IncludeNormals)
	{
		maxMesh->buildNormals();
		normalsAll = new1<PX2::Float3>(maxMesh->numVerts);
		for (i=0; i<maxMesh->numFaces; i++)
		{
			Face &face = maxMesh->faces[i];
			for (j=0; j<3; j++)
			{
				int vertexIndex = face.getVert(j);
				normalsAll[vertexIndex] = GetVertexNormal(maxMesh, i, vertexIndex);
			}
		}
	}

	// û�в���
	if (mtlIndex < 0)
	{
		UniMaterialMesh *triMesh = new0 UniMaterialMesh;
		triMesh->SetMaterialInstance(PX2::VertexColor4Material::CreateUniqueInstance());

		std::vector<int> faceIndexs;
		int faceIndex = -1;
		for (faceIndex=0; faceIndex<maxMesh->numFaces; faceIndex++)
		{
			faceIndexs.push_back(faceIndex);
		}

		PackVertices(triMesh, maxMesh, faceIndexs, normalsAll);

		if (mSettings->IncludeVertexColors && maxMesh->numCVerts>0)
		{
			triMesh->SetExportColor(true);
			PackColors(triMesh, maxMesh, faceIndexs);
		}

		if (mSettings->IncludeTargentBiNormal && maxMesh->numTVerts>0)
		{
			triMesh->SetExportTargentBinormal(true);
		}
		else
		{
			triMesh->SetExportTargentBinormal(false);
		}

		if (mSettings->IncludeTexCoords && maxMesh->numTVerts>0)
		{
			triMesh->SetNumTexcoordToExport(mSettings->NumTexCoords);
			PackTextureCoords(triMesh, maxMesh, faceIndexs);
		}

		uMeshes.push_back(triMesh);

		triMesh->DuplicateGeometry();
	}
	else
	{
		// ���Mtl���Ӳ�������
		MtlTree &tree = mMtlTreeList[mtlIndex];
		int subQuantity = 0; // �Ӳ�������
		if (mtlIndex >= 0)
		{
			subQuantity = tree.GetMChildQuantity();
		}

		// ���㼸��ͼ����ʹ�õ���󸽼Ӳ�������
		int faceIndex, subID, maxSubID = -1;
		for (faceIndex=0; faceIndex<maxMesh->numFaces; faceIndex++)
		{
			subID = maxMesh->faces[faceIndex].getMatID();
			if (subID >= subQuantity)
			{
				if (subQuantity > 0)
				{
					subID = subID % subQuantity;
				}
				else
				{
					subID = 0;
				}
			}

			if (subID > maxSubID)
			{
				maxSubID = subID;
			}
		}

		if (-1 != maxSubID)
		{
			// ����material ID����ÿ�������������
			std::vector<int> *faceIndexPartByMtl = new1<std::vector<int> >(maxSubID+1);
			for (faceIndex=0; faceIndex<maxMesh->numFaces; faceIndex++)
			{
				subID = maxMesh->faces[faceIndex].getMatID();
				if (subID >= subQuantity)
				{
					if (subQuantity > 0)
					{
						subID = subID % subQuantity;
					}
					else
					{
						subID = 0;
					}
				}
				//  ��ÿ���水�ղ��ʷ���
				faceIndexPartByMtl[subID].push_back(faceIndex);
			}

			// ��ÿ�ֲ������ͣ���������
			for (subID=0; subID<=maxSubID; subID++)
			{
				if (faceIndexPartByMtl[subID].size() == 0)
				{
					// ���ֲ���û����������
					continue;
				}

				// Ϊÿ�ֲ����½�һ��mesh
				UniMaterialMesh *triMesh = new0 UniMaterialMesh;
				if (mtlIndex >= 0)
				{
					PX2::MaterialInstance *mi = 0;

					if (subQuantity > 0)
					{
						MtlTree &subtree = tree.GetMChild(subID);
						triMesh->SetShineProperty(subtree.GetShine());

						mi = subtree.GetMaterialInstance();
					}
					else
					{
						triMesh->SetShineProperty(tree.GetShine());

						mi = tree.GetMaterialInstance();
					}

					triMesh->SetMaterialInstance(mi);
				}

				PackVertices(triMesh, maxMesh, faceIndexPartByMtl[subID], normalsAll);

				if (mSettings->IncludeVertexColors && maxMesh->numCVerts>0)
				{
					PackColors(triMesh, maxMesh, faceIndexPartByMtl[subID]);
					PackAlphas(triMesh, maxMesh, faceIndexPartByMtl[subID]);
				}

				if (mSettings->IncludeTargentBiNormal && maxMesh->numTVerts>0)
				{
					triMesh->SetExportTargentBinormal(true);
				}
				else
				{
					triMesh->SetExportTargentBinormal(false);
				}

				if (mSettings->IncludeTexCoords && maxMesh->numTVerts>0)
				{
					triMesh->SetNumTexcoordToExport(mSettings->NumTexCoords);
					PackTextureCoords(triMesh, maxMesh, faceIndexPartByMtl[subID]);
				}

				uMeshes.push_back(triMesh);
			}

			delete1(faceIndexPartByMtl);
		}

		for (i=0; i<(int)uMeshes.size(); i++)
		{
			uMeshes[i]->DuplicateGeometry();
		}
	}

	delete1(normalsAll);
}
//----------------------------------------------------------------------------
void SceneBuilder::PackVertices (UniMaterialMesh* uniMesh, Mesh* maxMesh,
									std::vector<int>& faceIndexParts,
									PX2::Float3 *normalsAll)
{
	// ��Max��������зָ(���������ζ����������)�����������ݴ����uniMesh��
	// 
	// uniMesh
	//		���������UniMaterialMesh
	// maxMesh
	//		����������Դ
	// faceIndexParts
	//		�Ӵ˻���棬�ٸ������ö�������
	// normalsAll
	//		����������Դ

	// ͨ��Set��ȡmax�еĶ�������
	// Stl��set���յ���˳�����Ԫ��
	std::set<int> vertexIndexs; // Set
	int i, j;
	for (i=0; i<(int)faceIndexParts.size(); i++)
	{
		Face &face = maxMesh->faces[faceIndexParts[i]];
		for (j = 0; j < 3; j++)
		{
			vertexIndexs.insert(face.v[j]);
		}
	}

	if (vertexIndexs.size() == 0)
		return;

	// vMapҪ�㹻��ȥ���ɿ��ܵ�j����������Stl::set��������Ԫ��
	// vMap[k] == -1 ��ʾmax�е�k���㲻�����Phoenix������
	int indexMax = *vertexIndexs.rbegin();
	int *vMap = new1<int>(indexMax+1);// max vertex index -> Phoenix vertex index
	memset(vMap, 0xFF, (indexMax+1)*sizeof(int));

	uniMesh->VQuantity() = (int)vertexIndexs.size();
	uniMesh->VertexMap() = new1<PX2::Float3>(uniMesh->VQuantity());
	uniMesh->NormalMap() = new1<PX2::Float3>(uniMesh->VQuantity());

	std::set<int>::iterator iter = vertexIndexs.begin();
	for (i=0; i<(int)vertexIndexs.size(); i++, iter++)
	{
		j = *iter; // max vertex index
		vMap[j] = i; // max vertex index -> Phoenix vertex index

		(uniMesh->VertexMap()[i])[0] = maxMesh->verts[j].x;
		(uniMesh->VertexMap()[i])[1] = maxMesh->verts[j].y;
		(uniMesh->VertexMap()[i])[2] = maxMesh->verts[j].z;

		if (normalsAll)
			uniMesh->NormalMap()[i] = normalsAll[j];
	}

	// ����Phoenix2������Ķ�������
	uniMesh->FQuantity() = (int)faceIndexParts.size();
	uniMesh->Face() = new1<int>(3*uniMesh->FQuantity());
	for (i = 0; i < (int)faceIndexParts.size(); i++)
	{
		Face &face = maxMesh->faces[faceIndexParts[i]];
		for (j = 0; j < 3; j++)
		{
			uniMesh->Face()[3*i+j] = vMap[face.v[j]];
		}
	}
	delete1(vMap);
}
//----------------------------------------------------------------------------
void SceneBuilder::PackColors (UniMaterialMesh* uniMesh, Mesh* maxMesh, 
								  std::vector<int>& faceIndexParts)
{
	// ��Max��������зָ(������������ɫ�������)������ɫ���ݴ����uniMesh��
	// 
	// uniMesh
	//		���������UniMaterialMesh
	// maxMesh
	//		����������Դ
	// faceIndexParts
	//		�Ӵ˻����ɫ�棬�ٸ�����ɫ������ɫ����

	// ��ô����񶥵���ɫ���ݵ�����
	std::set<int> colorIndexs;
	int i, j;
	for (i = 0; i < (int)faceIndexParts.size(); i++)
	{
		TVFace &face = maxMesh->vcFace[faceIndexParts[i]];
		for (j = 0; j < 3; j++)
		{
			colorIndexs.insert(face.t[j]);
		}
	}

	int colorIndexMax = *colorIndexs.rbegin();
	int *colorMap = new1<int>(colorIndexMax+1);
	memset(colorMap, 0xFF, (colorIndexMax+1)*sizeof(int));

	uniMesh->CQuantity() = (int)colorIndexs.size();
	uniMesh->ColorMap() = new1<PX2::Float4>(uniMesh->CQuantity());

	std::set<int>::iterator iter = colorIndexs.begin();
	for (i = 0; i < (int)colorIndexs.size(); i++, iter++)
	{
		j = *iter;
		colorMap[j] = i;

		(uniMesh->ColorMap()[i])[0] = maxMesh->vertCol[j].x;
		(uniMesh->ColorMap()[i])[1] = maxMesh->vertCol[j].y;
		(uniMesh->ColorMap()[i])[2] = maxMesh->vertCol[j].z;
		(uniMesh->ColorMap()[i])[3] = 1.0f;
	}

	uniMesh->CFace() = new1<int>(3*uniMesh->FQuantity());
	for (i = 0; i < (int)faceIndexParts.size(); i++)
	{
		TVFace &colorFace = maxMesh->vcFace[faceIndexParts[i]];
		for (j = 0; j < 3; j++)
		{
			uniMesh->CFace()[3*i+j] = colorMap[colorFace.t[j]];
		}
	}	
	delete1(colorMap);
}
//----------------------------------------------------------------------------
void SceneBuilder::PackAlphas(UniMaterialMesh* uniMesh, Mesh* maxMesh, 
	std::vector<int>& faceIndexParts)
{
	// �Ƿ�֧��Alpha
	bool isVertexAlpha = (TRUE == maxMesh->mapSupport(-VDATA_ALPHA));
	if (!isVertexAlpha) isVertexAlpha = (TRUE == maxMesh->mapSupport(MAP_ALPHA));

	if (!isVertexAlpha)
		return;

	TVFace* _TvFaceArray = maxMesh->mapFaces(MAP_ALPHA);
	UVVert* _TabTVertArray = maxMesh->mapVerts(MAP_ALPHA);

	std::set<int> alphaIndexs;
	for (int i = 0; i < (int)faceIndexParts.size(); i++)
	{
		int faceIndex = faceIndexParts[i];

		const TVFace& tvface = _TvFaceArray[faceIndex];

		for (int j=0; j<3; j++)
		{
			alphaIndexs.insert(tvface.t[j]);
		}
	}

	int alpahIndexMax = *alphaIndexs.rbegin();
	int *alphaMap = new1<int>(alpahIndexMax+1);
	memset(alphaMap, 0xFF, (alpahIndexMax+1)*sizeof(int));

	uniMesh->AQuantity() = (int)alphaIndexs.size();
	uniMesh->AlphaMap() = new1<float>(uniMesh->AQuantity());

	if (_TvFaceArray && _TabTVertArray)
	{
		std::set<int>::iterator kIter = alphaIndexs.begin();
		for (int i = 0; i < (int)alphaIndexs.size(); i++, kIter++)
		{
			int j = *kIter; // max�е�����
			alphaMap[j] = i; // max��������(j)->�����Լ����ֵ���Ӱ(i)

			Color pickCol(0, 0, 0);
			pickCol = _TabTVertArray[j];
			uniMesh->AlphaMap()[i] = pickCol.r;
		}
	}

	uniMesh->AFace() = new1<int>(3*uniMesh->FQuantity());

	for (int i = 0; i<(int)faceIndexParts.size(); i++)
	{
		TVFace& rkTFace = _TvFaceArray[faceIndexParts[i]];

		for (int j=0; j<3; j++)
		{
			uniMesh->AFace()[3*i+j] = alphaMap[rkTFace.t[j]];
		}
	}

	delete1(alphaMap);
}
//----------------------------------------------------------------------------
void SceneBuilder::PackTextureCoords (UniMaterialMesh* uniMesh,	 
									  Mesh* maxMesh,
									  std::vector<int>& faceIndexParts)
{
	// ��Max��������зָ(������������ɫ�������)������ͼ�����������ݴ����
	// UniMaterialMesh��
	//
	// uniMesh
	//		���������UniMaterialMesh
	// maxMesh
	//		����������Դ
	// faceIndexParts
	//		�Ӵ˻����ͼ�����棬��ͨ����ͼ����������ͼ��������
	
	// ���������õ�����������ŵ�һ���γ�һ����һ�޶����ֵ䣩
	std::set<int> texIndexs;
	std::set<int> texIndexs1;

	bool isHasTwoUVs = false;
	int iNumberOfMaxChannels = maxMesh->getNumMaps();
	for(int iMaxChannelID=1; iMaxChannelID<iNumberOfMaxChannels; iMaxChannelID++)
	{
		if (maxMesh->mapSupport(iMaxChannelID) && maxMesh->getNumMapVerts(iMaxChannelID)>0)
		{
			if (iMaxChannelID >= 2) isHasTwoUVs = true;

			TVFace* _TvFaceArray = maxMesh->mapFaces(iMaxChannelID);
			UVVert* _TabTVertArray = maxMesh->mapVerts(iMaxChannelID);

			if (_TvFaceArray && _TabTVertArray)
			{
				for (int i=0; i<(int)faceIndexParts.size(); i++)
				{
					int faceIndex = faceIndexParts[i];

					const TVFace& tvface = _TvFaceArray[faceIndex];

					for (int j=0; j<3; j++)
					{
						if (1 == iMaxChannelID)
							texIndexs.insert(tvface.t[j]);
						else if (2 == iMaxChannelID)
							texIndexs1.insert(tvface.t[j]);
					}
				}
			}
		}
	}

	int texIndexMax = *texIndexs.rbegin();
	int *texMap = new1<int>(texIndexMax+1);
	memset(texMap, 0xFF, (texIndexMax + 1)*sizeof(int));

	int texIndexMax1 = 0;
	int *texMap1 = 0;
	if (isHasTwoUVs)
	{
		texIndexMax1 = *texIndexs1.rbegin();
		texMap1 = new1<int>(texIndexMax1+1);
		memset(texMap1, 0xFF, (texIndexMax1 + 1)*sizeof(int));
	}

	uniMesh->TQuantity() = (int)texIndexs.size();
	uniMesh->TextureCoordMap() = new1<PX2::Float2>(uniMesh->TQuantity());

	if (isHasTwoUVs)
	{
		uniMesh->TQuantity1() = (int)texIndexs1.size();
		uniMesh->TextureCoordMap1() = new1<PX2::Float2>(uniMesh->TQuantity1());
	}

	for(int iMaxChannelID=1; iMaxChannelID<iNumberOfMaxChannels; iMaxChannelID++)
	{
		if (maxMesh->mapSupport(iMaxChannelID) && maxMesh->getNumMapVerts(iMaxChannelID)>0)
		{		
			TVFace* _TvFaceArray = maxMesh->mapFaces(iMaxChannelID);
			UVVert* _TabTVertArray = maxMesh->mapVerts(iMaxChannelID);

			if (_TvFaceArray && _TabTVertArray)
			{
				if (1 == iMaxChannelID)
				{
					std::set<int>::iterator kIter = texIndexs.begin();
					for (int i = 0; i < (int)texIndexs.size(); i++, kIter++)
					{
						int j = *kIter; // max�е�����
						texMap[j] = i; // max��������(j)->�����Լ����ֵ���Ӱ(i)

						uniMesh->TextureCoordMap()[i][0] = _TabTVertArray[j].x;
						uniMesh->TextureCoordMap()[i][1] = 1.0f - _TabTVertArray[j].y;
					}
				}
				else if (2 == iMaxChannelID)
				{
					std::set<int>::iterator kIter = texIndexs1.begin();
					for (int i = 0; i < (int)texIndexs1.size(); i++, kIter++)
					{
						int j = *kIter; // max�е�����
						texMap1[j] = i;

						uniMesh->TextureCoordMap1()[i][0] = _TabTVertArray[j].x;
						uniMesh->TextureCoordMap1()[i][1] = 1.0f - _TabTVertArray[j].y;
					}
				}
			}
		}
	}

	// face
	for(int iMaxChannelID=1; iMaxChannelID<iNumberOfMaxChannels; iMaxChannelID++)
	{
		if (maxMesh->mapSupport(iMaxChannelID) && maxMesh->getNumMapVerts(iMaxChannelID)>0)
		{		
			TVFace* _TvFaceArray = maxMesh->mapFaces(iMaxChannelID);
			UVVert* _TabTVertArray = maxMesh->mapVerts(iMaxChannelID);

			if (_TvFaceArray && _TabTVertArray)
			{
				if (1 == iMaxChannelID)
				{
					uniMesh->TFace() = new1<int>(3*uniMesh->FQuantity());

					for (int i = 0; i<(int)faceIndexParts.size(); i++)
					{
						TVFace& rkTFace = _TvFaceArray[faceIndexParts[i]];

						for (int j=0; j<3; j++)
						{
							uniMesh->TFace()[3*i+j] = texMap[rkTFace.t[j]];
						}
					}
				}
				else if (2 == iMaxChannelID)
				{
					uniMesh->TFace1() = new1<int>(3*uniMesh->FQuantity());

					for (int i = 0; i<(int)faceIndexParts.size(); i++)
					{
						TVFace& rkTFace = _TvFaceArray[faceIndexParts[i]];

						for (int j=0; j<3; j++)
						{
							uniMesh->TFace1()[3*i+j] = texMap1[rkTFace.t[j]];
						}
					}
				}
			}
		}
	}

	delete1(texMap);
	delete1(texMap1);
}
//----------------------------------------------------------------------------
PX2::Float3 SceneBuilder::GetVertexNormal (Mesh* maxMesh, int faceIndex,
										  int vertexIndex)
{
	// ���������һ���ض�����ķ���
	//
	// maxMesh��
	//		����������������
	// faceIndex:
	//		�������ڵ�������
	// vertexIndex:
	//		����Ķ�������
	// ���ط�������

	Point3 normal;

	RVertex* pkRV = maxMesh->getRVertPtr(vertexIndex);

	if (pkRV->rFlags & SPECIFIED_NORMAL)
	{
		normal = pkRV->rn.getNormal();
	}
	else
	{
		// ���RVertex�������ض��ķ��������������smoothing group����
		// smoothing group���㣻���������smoothing group��ʹ����ķ��ߡ�
		Face& rkFace = maxMesh->faces[faceIndex];
		DWORD dwSmoothingGroup = rkFace.smGroup;
		int numNormals = (pkRV->rFlags & NORCT_MASK);
		if (numNormals && dwSmoothingGroup)
		{
			if (numNormals == 1)
			{
				// smoothing group��ֻ��һ�����ߣ�rkRV�Ѿ��洢���ˡ�
				normal = pkRV->rn.getNormal();
			}
			else
			{
				// �Ӱ���������smoothing group����ҷ���
				for (int i = 0; i < numNormals; i++)
				{
					if (pkRV->ern[i].getSmGroup() & dwSmoothingGroup)
					{
						normal = pkRV->ern[i].getNormal();
						break;
					}
				}
			}
		}
		else
		{
			// ��smoothing group������ʱ��ʹ����ķ��ߣ���Ϊ����ķ��ߡ�
			normal = maxMesh->FaceNormal(faceIndex, true);
		}
	}

	PX2::AVector nor = PX2::AVector(normal.x, normal.y, normal.z);
	nor.Normalize();

	return PX2::Float3(nor.X(), nor.Y(), nor.Z());
}
//----------------------------------------------------------------------------
