#include <Library/DebugLib.h>
#include <Library/UefiBootServicesTableLib.h>
#include <Library/DeviceBootManagerLib.h>
#include <Library/RfsProtectionLib.h>
#include <Library/UefiBootManagerLib.h>
#include <Library/DevicePathLib.h>
#include <Protocol/SimpleTextIn.h>
#include <Protocol/SimpleTextInEx.h>

/*
 * DIZI only: the generic BDS hook returns no ConIn paths and defers
 * input through an on-demand USB event. Map *existing* Qualcomm ButtonsDxe
 * console handles into ConIn before connecting the platform consoles.
 * This does not access PMIC registers or repair an incompatible binary.
 */
STATIC
VOID
DiziRegisterButtonConIn (
  IN EFI_GUID *ProtocolGuid
  )
{
  EFI_STATUS Status;
  EFI_HANDLE *Handles = NULL;
  UINTN Count = 0;
  UINTN Index;
  UINTN Added = 0;
  EFI_DEVICE_PATH_PROTOCOL *Path;

  Status = gBS->LocateHandleBuffer (
                  ByProtocol, ProtocolGuid, NULL, &Count, &Handles);
  if (EFI_ERROR (Status)) {
    DEBUG ((DEBUG_WARN, "DIZI ConIn: no input handles (%r)\n", Status));
    return;
  }

  for (Index = 0; Index < Count; ++Index) {
    if (Handles[Index] == gST->ConsoleInHandle) {
      continue;
    }
    Path = DevicePathFromHandle (Handles[Index]);
    if (Path == NULL) {
      DEBUG ((DEBUG_WARN, "DIZI ConIn: input handle has no DevicePath\n"));
      continue;
    }
    Status = EfiBootManagerUpdateConsoleVariable (ConIn, Path, NULL);
    if (EFI_ERROR (Status)) {
      DEBUG ((DEBUG_WARN, "DIZI ConIn: append failed: %r\n", Status));
    } else {
      ++Added;
    }
  }

  DEBUG ((DEBUG_INFO, "DIZI ConIn: %u handles, %u appended\n",
          (UINT32)Count, (UINT32)Added));
  gBS->FreePool (Handles);
}


STATIC
VOID
DummyNotify (
  IN EFI_EVENT Event,
  IN VOID     *Context) 
{
  return;
}

EFI_HANDLE
EFIAPI
DeviceBootManagerBeforeConsole (
  OUT EFI_DEVICE_PATH_PROTOCOL  **DevicePath,
  OUT BDS_CONSOLE_CONNECT_ENTRY **PlatformConsoles)
{
  DiziRegisterButtonConIn (&gEfiSimpleTextInProtocolGuid);
  DiziRegisterButtonConIn (&gEfiSimpleTextInputExProtocolGuid);
  return NULL;
}

EFI_DEVICE_PATH_PROTOCOL**
EFIAPI
DeviceBootManagerAfterConsole ()
{
  EFI_STATUS Status;
  EFI_EVENT  InitEvent;

  // Create SD Card Init Event
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, DummyNotify, NULL, &gSdCardInitGuid, &InitEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create SD Card Init Event! Status = %r\n", __FUNCTION__, Status));
    return NULL;
  }

  // Init SD Card
  gBS->SignalEvent (InitEvent);
  gBS->CloseEvent  (InitEvent);

  return NULL;
}

EFI_DEVICE_PATH_PROTOCOL**
EFIAPI
DeviceBootManagerOnDemandConInConnect ()
{
  EFI_STATUS Status;
  EFI_EVENT  InitEvent;

  // Create USB Controller Start Event
  Status = gBS->CreateEventEx (EVT_NOTIFY_SIGNAL, TPL_CALLBACK, DummyNotify, NULL, &gUsbControllerInitGuid, &InitEvent);
  if (EFI_ERROR (Status)) {
    DEBUG ((EFI_D_ERROR, "%a: Failed to Create USB Port Controller Start Event! Status = %r\n", __FUNCTION__, Status));
    return NULL;
  }

  // Start USB Controller
  gBS->SignalEvent (InitEvent);
  gBS->CloseEvent  (InitEvent);

  return NULL;
}

VOID
EFIAPI
DeviceBootManagerProcessBootCompletion (OUT EFI_BOOT_MANAGER_LOAD_OPTION *BootOption)
{
  return;
}

EFI_STATUS
EFIAPI
DeviceBootManagerPriorityBoot (IN OUT EFI_BOOT_MANAGER_LOAD_OPTION *BootOption)
{
  return EFI_SUCCESS;
}

VOID
EFIAPI
DeviceBootManagerUnableToBoot ()
{
  return;
}

VOID
EFIAPI
DeviceBootManagerBdsEntry ()
{
  // Protect Shared RFS Area
  ProtectRfsSharedArea ();
}
