#pragma once
#include "ntdef.h"
#include "fltKernel.h"
#include "ntddstor.h"
#include "storduid.h"
#include "wdmsec.h"
#include "ntddvol.h"
#include "usbioctl.h"

#include "autolock.h"
#include "fastmutex.h"

#define EXTERNAL_DEVICE_MONITOR_DEVICE_NAME		L"\\Device\\ExternalDeviceMonitor"
#define EXTERNAL_DEVICE_MONITOR_DEVICE_SYMLINK	L"\\??\\ExternalDeviceMonitor"
#define BOOT_VOLUME_NT_PATH L"\\Device\\HardDiskVolume3"
#define MAX_ITEMS_COUNT 1024

template<typename T>
struct TRACKED_ITEM {
	LIST_ENTRY Entry;
	T Data;
};

//typedef struct {
//	PDEVICE_OBJECT DeviceObject;
//	PDEVICE_OBJECT VolumeDeviceObject;
//	PFILE_OBJECT FileObject;
//	PSTORAGE_DEVICE_UNIQUE_IDENTIFIER StorageDeviceUniqueId;
//	PSTORAGE_DEVICE_DESCRIPTOR StorageDeviceDescriptor;
//	PSTORAGE_HOTPLUG_INFO StorageHotplugInfo;
//	UNICODE_STRING DeviceId;
//	ULONG DeviceNumber;
//	BOOLEAN IsMountBlocked;
//	BOOLEAN IsReadBlocked;
//	BOOLEAN IsWriteBlocked;
//	BOOLEAN IsLogged;
//} TRACKED_STORAGE, * PTRACKED_STORAGE;
//
//typedef struct {
//	UNICODE_STRING DriveLetterDosPath;
//	UNICODE_STRING VolumeGuid;
//	UNICODE_STRING VolumeNtPath;
//	ULONG DeviceNumber;
//	ULONG DeviceType;
//	ULONG PartitionNumber;
//	PTRACKED_STORAGE AssociatedTrackedStorage;
//} TRACKED_VOLUME, PTRACKED_VOLUME;
//

enum ACTION {
	Connect,
	Disconnect,
	Read,
	Write,
	Create,
	Open,
	Delete,
};

struct _ACTION_HEADER {
	ACTION Action;
	ULONG size;
	PLARGE_INTEGER Time;
};

typedef struct _TRACKED_ACTION_CRUD : _ACTION_HEADER {
	UINT32 CanonicalFilePathOffset;
	UINT32 CanonicalFilePathLength;
} TRACKED_ACTION, PTRACKED_ACTION;

typedef struct {
	LIST_ENTRY ItemsHead;
	UINT32 ItemsCount;
	FastMutex mtx;
} LIST_TRACKED_ITEM, *PLIST_TRACKED_ITEM;

typedef struct {
	PFLT_FILTER Filter;
	PDRIVER_OBJECT DriverObject;
	//PLIST_TRACKED_ITEM TrackedStorageItems;
	PLIST_TRACKED_ITEM LoggingItems;
	//PLIST_TRACKED_ITEM TrackedVolumeItems;
} MINIFILTER_DATA, * PMINIFILTER_DATA;