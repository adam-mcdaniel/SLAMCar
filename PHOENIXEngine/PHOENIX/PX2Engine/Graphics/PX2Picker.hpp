// PX2Picker.hpp

#ifndef PX2PICKER_HPP
#define PX2PICKER_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2PickRecord.hpp"
#include "PX2APoint.hpp"

namespace PX2
{

	/// ѡ������
	/** 
	* �ߵı�����ʽΪP + t*D��P����ԭ�㣬D�ǵ�λ���ȵķ���������t�Ƿ����ϵ�
	* ���Ų�����������[tmin,tmax]�ڣ�tmin < tmax��P��D��������������ϵ���ֵ��
	* ��ͬ���Ͳ���ѡ�񣬾����ߵ����͡�
	* ֱ��line:		tmin = -Mathf::MAX_REAL, tmax = Mathf::MAX_REAL
	* ����ray��		tmin = 0, tmax = Mathf::MAX_REAL
	* �߶�segment��	tmin = 0, tmax > 0
	*/
	class PX2_ENGINE_ITEM Picker
	{
	public:
		Picker (bool isDoMovPickCall=false, int pickInfo=-1);
		~Picker ();

		/// ִ�м�⣬����¼���浽Records�С�
		/**
		* ���ô˺����Զ���ռ�¼���顣�����ǰ��¼���������Ϣ��������ȱ��档
		*/
		void Execute (Movable* scene, const APoint& origin,
			const AVector& direction, float tmin, float tmax);

		// �����3��������������ѡ�ĳ����ڵ㣬���ҽ���Records.size() > 0ʱ����
		// �ɱ����á�

		/// ���ؾ���ֵT��ӽ�0�������μ�¼
		/**
		* ���RecordsΪ�գ�������Чֵ��
		*/
		const PickRecord& GetClosestToZero () const;

		/// ���طǸ���T��ӽ�0�������μ�¼
		/** 
		* ���RecordsΪ�գ�������Чֵ��
		*/
		const PickRecord& GetClosestNonnegative () const;

		/// ���ط�����T��ӽ�0�������μ�¼
		/** 
		* ���RecordsΪ�գ�������Чֵ��
		*/
		const PickRecord& GetClosestNonpositive () const;

		/// ��ѡ�ļ�¼��
		std::vector<PickRecord> Records;

	private:
		void ExecuteRecursive (Movable *object, bool &hasMeshPicked);
		int mPickInfo;
		bool mIsDoMovPickCall;
		APoint mOrigin;
		AVector mDirection;
		float mTMin, mTMax;

		PickRecord sNotValiedRecord;
	};

}

#endif
