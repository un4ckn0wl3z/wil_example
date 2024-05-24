#include <Windows.h>
#include <stdio.h>
#include <wil\resource.h>

int main(int argc, const char* argv[])
{
	if (argc < 2)
	{
		printf("Usage: kill <pid>\n");
		return 0;
	}

  // ทำงานเหมือน unique_ptr ใน C++ ... เมื่อ หมด scope ของ handle hProcess ... WIL จะ CloseHandle ให้เอง
	wil::unique_handle hProcess(::OpenProcess(PROCESS_TERMINATE, FALSE, atoi(argv[1])));
	if (!hProcess)
	{
		printf("Error opening process (%u)\n", ::GetLastError());
		return 1;
	}

	if (!::TerminateProcess(hProcess.get(), 1337))
	{
		printf("Error terminating process (%u)\n", ::GetLastError());
		return 1;
	}


  // ใช้ .addressof() แทนการส่ง pointer
	wil::unique_handle hToken;
	if (!::OpenProcessToken(hProcess.get(), TOKEN_QUERY, hToken.addressof()))
	{
		return 1;
	}

	printf("PID: %u killed!\n", atoi(argv[1]));

	return 0;
}
