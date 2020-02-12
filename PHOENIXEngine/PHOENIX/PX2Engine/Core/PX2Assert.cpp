// PX2Assert.cpp

#include "PX2Assert.hpp"

#ifdef PX2_USE_ASSERT

#ifdef PX2_USE_ASSERT_WRITE_TO_OUTPUT_WINDOW
#include <windows.h>
#elif defined __ANDROID__
#include <android/log.h>
#endif

#ifdef PX2_USE_ASSERT_WRITE_TO_MESSAGE_BOX
#include <intrin.h>
#endif

using namespace PX2;

const char *Assert::msDebugPrompt = "Do you want to debug?";
const size_t Assert::msDebugPromptLength = strlen(Assert::msDebugPrompt);
const char *Assert::msMessagePrefix = "\nAssert failed at %s(%d):\n";

#ifdef PX2_USE_ASSERT_WRITE_TO_MESSAGE_BOX
const char *Assert::msMessageBoxTitle = "Assert Failed";
#endif

#ifdef PX2_USE_ASSERT_LOG_TO_FILE
const char *Assert::msLogFileName = "Assertions.txt";
#endif

//----------------------------------------------------------------------------
Assert::Assert (bool condition, const char* file, int line,
				const char* format, ...)
{
	if (condition)
	{
		// ����Ϊ��
		return;
	}

	// ����λ��
	char message[MAX_MESSAGE_BYTES];
	const size_t maxPrefixBytes = MAX_MESSAGE_BYTES - msDebugPromptLength - 1;
	_snprintf(message, maxPrefixBytes, msMessagePrefix, file, line);

	// ��Ӷ�����Ϣ
	va_list arguments;
	va_start(arguments, format);
	size_t length = strlen(message);
	_vsnprintf(message + length, maxPrefixBytes - length, format, arguments);
	va_end(arguments);

#ifdef PX2_USE_ASSERT_LOG_TO_FILE
	// ����Ϣд����־
	FILE* logFile = fopen(msLogFileName, "at");
	if (logFile)
	{
		fprintf(logFile, message);
		fflush(logFile);
	}
	fclose(logFile);
#endif

#ifdef PX2_USE_ASSERT_WRITE_TO_OUTPUT_WINDOW
	// ��������Ϣд����Դ���
	OutputDebugString(message);
#endif

#ifdef PX2_USE_ASSERT_WRITE_TO_MESSAGE_BOX
	// ��������Ϣд��MessageBox���ṩ�û�ѡ����������߽�������Ļ��ᡣ
	strcat(message, msDebugPrompt);
	int selection = ::MessageBox(0, message, msMessageBoxTitle,
		MB_ICONERROR | MB_YESNOCANCEL | MB_APPLMODAL | MB_TOPMOST);

	switch (selection)
	{
	case IDYES:
		__debugbreak();
		break;
	case IDNO:
		break;
	case IDCANCEL:
	default:
		exit(0);
		break;
	}
#endif
}
//----------------------------------------------------------------------------
Assert::~Assert ()
{
}
//----------------------------------------------------------------------------

#endif
