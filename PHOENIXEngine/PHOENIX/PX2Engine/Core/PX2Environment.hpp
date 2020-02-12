// PX2Environment.hpp

#ifndef PX2ENVIRONMENT_HPP
#define PX2ENVIRONMENT_HPP

#include "PX2CorePre.hpp"
#include "PX2InitTerm.hpp"

namespace PX2
{

	class PX2_ENGINE_ITEM Environment
	{
		PX2_DECLARE_INITIALIZE;
		PX2_DECLARE_TERMINATE;

	public:
		// �����ַ��������Ӧ�Ļ�������
		static std::string GetVariable (const std::string& varName);

		/*
		* ����һ��Ŀ¼�б�.Ϊ��ƽ̨�����ԣ�ʹ��"/"��Ϊ·���ָ��.��Ŀ¼��ĩβ��
		* ��"/"����������ɹ���Insert/Remove���������档
		*/
		static int GetNumDirectories ();
		static std::string GetDirectory (int i);
		static bool InsertDirectory (const std::string& directory);
		static bool RemoveDirectory (const std::string& directory);
		static void RemoveAllDirectories ();

		/* 
		* GetPath*��������Ŀ¼�б����������ļ���������ļ����ڣ������棻���
		* �����ڷ���""��GetPathR�������ļ�ֻ�����ʼ�⣬��������·����GetPathW
		* ���ļ�����д���ʼ�⣬��������·����GetPathRW���ļ����ж�д���ʼ�⣬
		* ��������·����
		*/
		static std::string GetPath (const std::string& fileName, int mode);
		static std::string GetPathR (const std::string& fileName);
		static std::string GetPathW (const std::string& fileName);
		static std::string GetPathRW (const std::string& fileName);

	private:
		static std::string GetPath (const std::string& filename,
			const char* attributes);

		// ���������б�.�б���InitTerm::ExecuteInitializers������ʱ��������
		static std::vector<std::string>* msDirectories;
	};

	PX2_REGISTER_INITIALIZE(Environment);
	PX2_REGISTER_TERMINATE(Environment);

}

#endif