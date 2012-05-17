#pragma once

#pragma comment(lib,"ShiftEncrypt.lib")

extern "C" ULONG WINAPI GetShiftBinarySize();
extern "C" BOOL  WINAPI GetShiftBinary(
									 PVOID pFileData,
									 ULONG StartDataRva,
									 ULONG DataSize,
									 PUCHAR &pInBinary,
									 ULONG &OffsetCode,
									PULONG &NextCode);