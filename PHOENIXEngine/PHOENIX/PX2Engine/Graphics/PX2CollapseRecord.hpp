// PX2CollapseRecord.hpp

#ifndef PX2COLLAPSERECORD_HPP
#define PX2COLLAPSERECORD_HPP

#include "PX2GraphicsPre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM CollapseRecord
	{
	public:
		CollapseRecord (int vKeep = -1, int vThrow = -1, int numVertices = 0,
			int numTriangles = 0);

		~CollapseRecord ();

		// Edge <VKeep,VThrow>�����VThrow�ᱻVKeep�滻��
		int VKeep, VThrow;

		// �������ݵ��󣬶��������
		int NumVertices;

		// �������ݵ�������������
		int NumTriangles;

		// ����[0..NumTriangles-1]֮�ڱ��ӵ��Ķ��������
		int NumIndices;
		int* Indices;
	};

}

#endif