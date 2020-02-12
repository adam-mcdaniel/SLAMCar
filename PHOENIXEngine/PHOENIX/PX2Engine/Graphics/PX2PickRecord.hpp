// PX2PickRecord.hpp

#ifndef PX2PICKRECORD_HPP
#define PX2PICKRECORD_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Movable.hpp"

namespace PX2
{

	/// �ü������¼��
	class PX2_ENGINE_ITEM PickRecord
	{
	public:
		PickRecord ();
		~PickRecord ();

		// ����ѡ��֧��
		inline bool operator== (const PickRecord& record) const;
		inline bool operator!= (const PickRecord& record) const;
		inline bool operator<  (const PickRecord& record) const;
		inline bool operator<= (const PickRecord& record) const;
		inline bool operator>  (const PickRecord& record) const;
		inline bool operator>= (const PickRecord& record) const;

		// �ཻ������
		MovablePtr Intersected;

		// ����P + t*D�е�����Ԫ������T������t�ı�ʾ��
		float T;

		// �������ཻ��������
		int Triangle;

		// ��������ϵ��ѡ���������궼������[0,1]�ڣ�
		// ����Bary[0] + Bary[1] + Bary[2] = 1��
		float Bary[3];

		// ��������ϵ�µ�λ��
		APoint WorldPos;

		// ��������ϵ�µķ���
		AVector LocalNormal;

		// �Ƿ�����Ч��
		bool IsValied;
	};

#include "PX2PickRecord.inl"

}

#endif
