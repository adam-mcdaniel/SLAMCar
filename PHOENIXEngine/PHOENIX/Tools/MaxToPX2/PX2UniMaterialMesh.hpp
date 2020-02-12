// PX2UniMaterialMesh.hpp

#ifndef PX2UNIMATERIALMESH_HPP
#define PX2UNIMATERIALMESH_HPP

#include "PX2MaxToPX2PluginPre.hpp"

class UniMaterialMesh
{
public:
	UniMaterialMesh ();
	~UniMaterialMesh ();

	int &VQuantity ();
	PX2::Float3 *&VertexMap ();
	PX2::Float3 *&NormalMap ();
	int &CQuantity ();
	PX2::Float4 *&ColorMap ();
	int &AQuantity ();
	float *&AlphaMap ();
	int &TQuantity ();
	PX2::Float2 *&TextureCoordMap ();
	int &TQuantity1 ();
	PX2::Float2 *&TextureCoordMap1 ();
	int &FQuantity ();
	int *&Face ();
	int *&AFace ();
	int *&CFace ();
	int *&TFace ();
	int *&TFace1 ();

	void SetShineProperty (PX2::Shine *shine);
	void SetMaterialInstance (PX2::MaterialInstance *mi);

	// һ��Max�������ӵ�ж��UVW���ꡣPhoenix2ֻ֧��һ��UV���꣨W�����Ե�����
	// ����Max����ӵ�е�N��UVW������뱻���Ƶ�N��Phoenix2�Ķ��������У�ÿ����
	// ��ֻӵ��һ��UV���ꡣ
	void DuplicateGeometry ();

	void SetExportColor (bool color);
	void SetExportTargentBinormal (bool exp);
	void SetNumTexcoordToExport (int num);
	void SetExportSkin (bool skin);
	PX2::TriMesh *ToTriMesh ();

	class VertexAttr
	{
	public:
		VertexAttr ();
		bool operator== (const VertexAttr &vFormat) const;
		bool operator< (const VertexAttr &vFormat) const;
		int V, C, A, T, T1;
	};

private:

	// --
	int mVertexMapQuantity;
	PX2::Float3 *mVertexMap;
	PX2::Float3 *mNormalMap;

	int mColorMapQuantity;
	PX2::Float4 *mColorMap;

	int mAlphaMapQuantity;
	float *mAlphaMap;

	int mTMapQuantity;
	PX2::Float2 *mTextureCoordMap;

	int mTMapQuantity1;
	PX2::Float2 *mTextureCoordMap1;
	// --

	int mFQuantity;
	int *mVFace;	// ÿ��Ԫ�ش���һ�����ϣ�һ������Ķ�������
	int *mCFace;	// ÿ��Ԫ�ش���һ�����ϣ�һ���������ɫ����
	int *mAFace;	// ÿ��Ԫ�ش���һ�����ϣ�һ���������ɫ͸������
	int *mTFace;	// ÿ��Ԫ�ش���һ�����ϣ�һ���������ͼ��������
	int *mTFace1;

	bool mExportTargentBinormal;
	bool mExportColor;
	int mNumTexcoordToExport;
	bool mExportSkin;

	PX2::ShinePtr mShine;
	PX2::MaterialInstancePtr mMaterialInstance;
};

#endif