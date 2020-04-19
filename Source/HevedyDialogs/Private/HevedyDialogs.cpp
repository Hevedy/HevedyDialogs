// Copyright 2020 Hevedy


#include "HevedyDialogs.h"
#include "CoreMinimal.h"
#include "HAL/PlatformProcess.h"
#include "Interfaces/IPluginManager.h"
#include "Misc/MessageDialog.h"
#include "Misc/Paths.h"
#include "Modules/ModuleManager.h"

#define LOCTEXT_NAMESPACE "FHevedyDialogsModule"

void FHevedyDialogsModule::StartupModule()
{

}

void FHevedyDialogsModule::ShutdownModule()
{

}

#undef LOCTEXT_NAMESPACE

IMPLEMENT_MODULE(FHevedyDialogsModule, HevedyDialogs)