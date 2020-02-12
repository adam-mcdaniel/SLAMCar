// PX2CSVParser.hpp

#ifndef PX2CSVPARSER_HPP
#define PX2CSVPARSER_HPP

#include "PX2UnityPre.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM CSVParser
	{
	public:
		class PX2_ENGINE_ITEM TableItem
		{
		public:
			TableItem (const char* szItemString);
			
			bool IsValid() const;

			operator const char*() const;
			const char *Str() const;
			std::string String() const;
			bool GetBool() const;
			char Char() const;
			unsigned char UChar() const;
			unsigned char Byte() const;
			short Short() const;
			unsigned short UShort() const;
			unsigned short Word() const;
			int Int() const;
			unsigned int UInt() const;
			long Long() const;
			long ULong() const;
			unsigned long DWord() const;
			float Float() const;
			double Double() const;

		private:
			const char* mItemString;
		};

		class TableLine
		{
		public:			
			friend class CSVParser;
			
			const TableItem operator[] (int index) const;
			const TableItem operator[] (const char* szIdx) const;

		protected:
			TableLine ();
			void SetLine (CSVParser* pTable, int iLineIdx);

		private:
			CSVParser *mTable;
			int mLineIdx;
		};

		CSVParser();
		virtual ~CSVParser();

		// ���ļ��м��أ�ʹ��PX2_RM���buffer����أ�
		bool Load (const std::string &filename); 
		bool IsLoaded ();
		void Clear();

		int GetNumLines ();
		int GetNumCols ();

		// ���ô���һ���ǡ������С���Phoenix3d����Ŀ�У���0��������ע�ͣ�
		// ��1���ǡ������С���index��ֵΪ1
		void SetTitleLine (int index);
		int GetTitleLine ();

		// ���ô���һ���ǡ������С�
		void SetTitleCol (int index);
		int GetTitleCol ();

		const TableLine& operator[] (int index) const;
		const TableLine& operator[] (const char* szIdx) const;

		// ȡ��ָ���к��е��ַ���
		const char* GetString(int line, int col) const;
		const char* GetString(int line, const char* szColIdx) const;
		const char* GetString(const char* szLineIdx, const char* szColIdx) const;
		bool Char  (size_t line, size_t col, char &val) const;
		bool Byte  (size_t line, size_t col, unsigned char &val) const;
		bool Short (size_t line, size_t col, short &val) const;
		bool Word  (size_t line, size_t col, unsigned short &val) const;
		bool Int   (size_t line, size_t col, int &val) const;
		bool UInt  (size_t line, size_t col, unsigned int &val) const;
		bool Long  (size_t line, size_t col, long &val) const;
		bool DWord (size_t line, size_t col, unsigned long &val) const;
		bool Float (size_t line, size_t col, float &val) const;
		bool Double(size_t line, size_t col, double &val) const;

		// ���ҹؼ���str��һ�γ��ֵ�λ�ã��ҵ�����true���ҷ������е�line, col���򷵻�false
		bool FindPosByString (const char* str, int& line, int& col);
		int FindLineByString (const char* str); // ����str��һ�γ��ֵ��У��Ҳ�������-1
		int FindColByString (const char* str);  // ����str��һ�γ��ֵ��У��Ҳ�������-1

	protected:
		bool ParseTextTable();

		char mSeparator;
		std::string mFilename;
		char *mContent;
		char **mItems;
		TableLine *mLines;
		TableLine mInvalidLine;
		int mNumLines;
		int mNumCols;
		int mTitleLine;
		int mTitleCol;
	};

#include "PX2CSVParser.inl"

}

#endif