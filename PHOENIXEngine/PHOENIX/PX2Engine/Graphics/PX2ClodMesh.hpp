// PX2ClodMesh.hpp

#ifndef PX2CLODMESH_HPP
#define PX2CLODMESH_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2CollapseRecordArray.hpp"
#include "PX2TriMesh.hpp"


namespace PX2
{

	/**
	* ClodMesh�ᴴ��һ�������mesh�Ķ��������Ŀ�����
	*/
	class PX2_ENGINE_ITEM ClodMesh : public TriMesh
	{
		PX2_DECLARE_RTTI;
		PX2_DECLARE_NAMES;
		PX2_DECLARE_STREAM(ClodMesh);

	public:
		ClodMesh (TriMesh* mesh, CollapseRecordArray* recordArray);
		virtual ~ClodMesh ();

		inline int GetNumRecords () const;
		inline int& TargetRecord ();

		/// ���������ش˺����������������ȣ����Ŀ�����񼶱�
		inline virtual int GetAutomatedTargetRecord ();

		/// ����ͼ�θ���
		/**
		* ���øú��������¼���ͼ�β��ҵ�������������
		*/
		void SelectLevelOfDetail ();

	protected:
		virtual void OnGetVisibleSet (Culler& culler, bool noCull);

		int mCurrentRecord, mTargetRecord;
		CollapseRecordArrayPtr mRecordArray;
	};

	PX2_REGISTER_STREAM(ClodMesh);
	typedef Pointer0<ClodMesh> ClodMeshPtr;
#include "PX2ClodMesh.inl"

}

#endif