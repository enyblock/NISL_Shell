#pragma once

#pragma comment(lib,"XorEncrypt.lib")

extern "C" ULONG WINAPI GetXorBinarySize();
extern "C" BOOL  WINAPI GetXorBinary(
									PVOID pFileData,
									ULONG StartDataRva,
									ULONG DataSize,
									PUCHAR &pInBinary,
									ULONG &OffsetCode,
									PULONG &NextCode);