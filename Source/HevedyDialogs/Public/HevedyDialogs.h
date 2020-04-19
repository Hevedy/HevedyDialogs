// Copyright 2020 Hevedy

#pragma once

#include "CoreMinimal.h"

#include "Modules/ModuleManager.h"

class FHevedyDialogsModule : public IModuleInterface {
public:
    /** IModuleInterface implementation */
    virtual void StartupModule() override;
    virtual void ShutdownModule() override;

};