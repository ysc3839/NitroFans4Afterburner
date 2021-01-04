#pragma once

inline void ReadFileCheckSize(HANDLE hFile, void* buffer, DWORD size)
{
	DWORD read;
	THROW_IF_WIN32_BOOL_FALSE(ReadFile(hFile, buffer, size, &read, nullptr));
	THROW_HR_IF(HRESULT_FROM_WIN32(ERROR_HANDLE_EOF), size != read);
}

inline void WriteFileCheckSize(HANDLE hFile, void* data, DWORD size)
{
	DWORD written;
	THROW_IF_WIN32_BOOL_FALSE(WriteFile(hFile, data, size, &written, nullptr));
	THROW_HR_IF(HRESULT_FROM_WIN32(ERROR_HANDLE_DISK_FULL), size != written);
}
