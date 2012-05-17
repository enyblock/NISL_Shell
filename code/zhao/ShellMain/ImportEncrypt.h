#pragma once
#pragma comment(lib,"ImportEncrypt.lib")
#include "PEplugin.h"

extern "C" bool WINAPI GetBinary(IN PVOID pFileData,IN ULONG StartRva,OUT PUCHAR &Binary,OUT ULONG &Size);