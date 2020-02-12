// PX2Tuple.hpp

#ifndef PX2TUPLE_HPP
#define PX2TUPLE_HPP

#include "PX2CorePre.hpp"

namespace PX2
{

	/// Ԫ����
	/**
	* TYPE��ԭʼ�������ݻ������������ݣ���Ҫ������ĳ�Ա
	* TYPE::TYPE ()
	* TYPE::TYPE (const TYPE&);
	* TYPE& TYPE::operator= (const TYPE&)
	*/
	template <int DIMENSION, typename TYPE>
	class Tuple
	{
	public:
		Tuple ();
		Tuple (const Tuple& tuple);
		~Tuple ();

		inline operator const TYPE* () const;
		inline operator TYPE* ();
		inline const TYPE& operator[] (int i) const;
		inline TYPE& operator[] (int i);

		Tuple& operator= (const Tuple& tuple);

		bool operator== (const Tuple& tuple) const;
		bool operator!= (const Tuple& tuple) const;
		bool operator<  (const Tuple& tuple) const;
		bool operator<= (const Tuple& tuple) const;
		bool operator>  (const Tuple& tuple) const;
		bool operator>= (const Tuple& tuple) const;

	protected:
		TYPE mTuple[DIMENSION];
	};

#include "PX2Tuple.inl"

}

#endif