#include "../../StringEncryption/Xor/xor.h"
#include <windows.h>
#include <tlhelp32.h>

uintptr_t virtualaddy;

#define MEDreadwyy CTL_CODE(FILE_DEVICE_UNKNOWN, 0x07D, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define MEDbab3 CTL_CODE(FILE_DEVICE_UNKNOWN, 0x09A, METHOD_BUFFERED, FILE_SPECIAL_ACCESS)
#define Scode_security 0x85b3e20

typedef struct _readwrite {
	INT32 security;
	INT32 process_id;
	ULONGLONG address;
	ULONGLONG buffer;
	ULONGLONG size;
	BOOLEAN write;
} rw, * prw;

typedef struct _ba {
	INT32 security;
	INT32 process_id;
	ULONGLONG* address;
} ba, * pba;

typedef struct _ga {
	INT32 security;
	ULONGLONG* address;
} ga, * pga;

namespace mem {
	HANDLE driver_handle;
	INT32 process_id;

	bool find_driver() {
		driver_handle = CreateFileW(_(L"\\\\.\\\MedusaNewCom"), GENERIC_READ | GENERIC_WRITE, FILE_SHARE_READ | FILE_SHARE_WRITE, NULL, OPEN_EXISTING, 0, NULL);

		if (!driver_handle || (driver_handle == INVALID_HANDLE_VALUE))
			return false;

		return true;
	}

	void read_physical(PVOID address, PVOID buffer, DWORD size) {
		_readwrite arguments = { 0 };

		arguments.security = Scode_security;
		arguments.address = (ULONGLONG)address;
		arguments.buffer = (ULONGLONG)buffer;
		arguments.size = size;
		arguments.process_id = process_id;
		arguments.write = FALSE;

		DeviceIoControl(driver_handle, MEDreadwyy, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}

	void write_physical(PVOID address, PVOID buffer, DWORD size) {
		_readwrite arguments = { 0 };

		arguments.security = Scode_security;
		arguments.address = (ULONGLONG)address;
		arguments.buffer = (ULONGLONG)buffer;
		arguments.size = size;
		arguments.process_id = process_id;
		arguments.write = TRUE;

		DeviceIoControl(driver_handle, MEDreadwyy, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);
	}

	uintptr_t find_image() {
		uintptr_t image_address = { NULL };
		_ba arguments = { NULL };

		arguments.security = Scode_security;
		arguments.process_id = process_id;
		arguments.address = (ULONGLONG*)&image_address;

		DeviceIoControl(driver_handle, MEDbab3, &arguments, sizeof(arguments), nullptr, NULL, NULL, NULL);

		return image_address;
	}

	INT32 find_process(LPCTSTR process_name) {
		PROCESSENTRY32 pt;
		HANDLE hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
		pt.dwSize = sizeof(PROCESSENTRY32);
		if (Process32First(hsnap, &pt)) {
			do {
				if (!lstrcmpi(pt.szExeFile, process_name)) {
					CloseHandle(hsnap);
					process_id = pt.th32ProcessID;
					return pt.th32ProcessID;
				}
			} while (Process32Next(hsnap, &pt));
		}
		CloseHandle(hsnap);
		return process_id;
	}

}

template <typename T>
T read(uint64_t address) {
	T buffer{ };
	mem::read_physical((PVOID)address, &buffer, sizeof(T));

	return buffer;
}

template <typename T>
T write(uint64_t address, T buffer) {

	mem::write_physical((PVOID)address, &buffer, sizeof(T));
	return buffer;
}