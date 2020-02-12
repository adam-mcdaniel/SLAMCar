// PX2Table.hpp

#ifndef PX2TABLE_H
#define PX2TABLE_H

#include "PX2Tuple.hpp"

namespace PX2
{

	/// ����
	/**
	* TYPE��ԭʼ�������ݻ������������ݣ���Ҫ������ĳ�Ա
	* TYPE::TYPE ()
	* TYPE::TYPE (const TYPE&);
	* TYPE& TYPE::operator= (const TYPE&)
	*/
	template <int NUMROWS, int NUMCOLS, typename TYPE>
	class Table
	{
	public:
		Table ();
		Table (const Table& table);
		~Table ();

		inline operator const TYPE* () const;
		inline operator TYPE* ();
		inline const TYPE* operator[] (int row) const;
		inline TYPE* operator[] (int row);
		inline TYPE operator() (int row, int col) const;
		inline TYPE& operator() (int row, int col);
		void SetRow (int row, const Tuple<NUMROWS,TYPE>& tuple);
		Tuple<NUMROWS,TYPE> GetRow (int row) const;
		void SetColumn (int col, const Tuple<NUMCOLS,TYPE>& tuple);
		Tuple<NUMCOLS,TYPE> GetColumn (int col) const;

		Table& operator= (const Table& table);

		bool operator== (const Table& table) const;
		bool operator!= (const Table& table) const;
		bool operator<  (const Table& table) const;
		bool operator<= (const Table& table) const;
		bool operator>  (const Table& table) const;
		bool operator>= (const Table& table) const;

	protected:
		enum { NUMENTRIES = NUMROWS*NUMCOLS };
		TYPE mEntry[NUMENTRIES];
	};

#include "PX2Table.inl"

}

#endif
