// PX2VisibleSet.hpp

#ifndef PX2VISIBLESET_HPP
#define PX2VISIBLESET_HPP

#include "PX2GraphicsPre.hpp"
#include "PX2Assert.hpp"

namespace PX2
{

	class Renderable;

	typedef bool (*SortFun) (const Renderable *renderable0,
		const Renderable *renderable1);
	
	/// �ɼ�����
	class PX2_ENGINE_ITEM VisibleSet
	{
	public:
		VisibleSet ();
		~VisibleSet ();

		// ���ʿɼ�����
		inline int GetNumVisible () const;
		inline const std::vector<Renderable*>& GetAllVisible () const;
		inline Renderable* GetVisible (int i) const;

		// ����ɼ�����
		void Insert (Renderable* visible);

		void SetSortFun (SortFun fun);
		SortFun GetSortFun () const;

		// ����
		VisibleSet &Sort ();

		// ��տɼ������б�
		inline void Clear ();

	private:
		SortFun mSortFun;
		int mNumVisible;
		std::vector<Renderable*> mVisible;
 	};

#include "PX2VisibleSet.inl"

}

#endif