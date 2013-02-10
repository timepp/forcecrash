#pragma once

// execute dll function remotely
#include <windows.h>

struct crash
{
	static HRESULT HRESULTWIN32()
	{
		DWORD err = ::GetLastError();
		return HRESULT_FROM_WIN32(err);
	}
	static HRESULT CrashProcess(DWORD pid)
	{
		HRESULT hr = S_OK;
		HANDLE process = NULL;
		HANDLE thrd = NULL;
		
		process = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);
		if (!process)
		{
			hr = HRESULTWIN32();
			goto END;
		}

		thrd = ::CreateRemoteThread(process, NULL, 0, 0, NULL, 0, NULL);
		if (!thrd)
		{
			hr = HRESULTWIN32();
			goto END;
		}
END:
		if (thrd) ::CloseHandle(thrd);
		if (process) ::CloseHandle(process);
		return hr;
	}
};

