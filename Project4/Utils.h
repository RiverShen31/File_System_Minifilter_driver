#pragma once

#include "common.h"



VOID GenerateLog(
	_In_ MINIFILTER_DATA *MinifilterData,
	_In_ PFLT_FILE_NAME_INFORMATION fileNameInfo,
	_In_ ACTION action,
	_Out_ TRACKED_ITEM<TRACKED_ACTION>** item
);

VOID AddTrackedLog(
	PLIST_TRACKED_ITEM list,
	TRACKED_ITEM<TRACKED_ACTION>* action
);