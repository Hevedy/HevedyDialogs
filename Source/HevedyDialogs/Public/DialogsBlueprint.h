// Copyright 2019-2020 f00n.com
// Copyright 2020 Hevedy

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GenericPlatform/GenericPlatformMisc.h"
#include "DialogsBlueprint.generated.h"

UENUM(BlueprintType)
enum ETinyFileDialogMessageType
{
	OK			UMETA(DisplayName = "OK"),
	OKCANCEL	UMETA(DisplayName = "OK / CANCEL"),
	YESNO		UMETA(DisplayName = "YES / NO"),
	YESNOCANCEL UMETA(DisplayName = "YES / NO / CANCEL")
};

UENUM(BlueprintType)
enum ETinyFileDialogIconType
{
	INFO		UMETA(DisplayName = "Info"),
	WARNING		UMETA(DisplayName = "OK / Cancel"),
	ERROR		UMETA(DisplayName = "Yes / No"),
	QUESTION	UMETA(DisplayName = "Yes / No / Cancel")
};

UENUM(BlueprintType)
enum ETinyFileDialogInputType
{
	PLAINTEXT	UMETA(DisplayName = "Plain Text"),
	PASSWORD 	UMETA(DisplayName = "Password")
};

/**
 * 
 */
UCLASS(BlueprintType)
class HEVEDYDIALOGS_API UDialogsBlueprint : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:

	// Message may contain \n \t
	// DialogType "ok" "okcancel" "yesno" "yesnocancel"
	// IconType "info" "warning" "error" "question"
	// DefaultButton 0 for cancel/no , 1 for ok/yes , 2 for no in yesnocancel
	// Use a Win32 compatible dialog
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", CallableWithoutWorlContext, Keywords = "Prompts for a message box"), Category = "Hevedy|Dialog")
		static int32 ShowMessageBoxTFD( FString _Title, FString _Message, ETinyFileDialogMessageType _DialogType, ETinyFileDialogIconType _IconType, int32 _DefaultButton, bool _UseWin32 );

	UFUNCTION( BlueprintCallable, meta = ( WorldContext = "WorldContextObject", CallableWithoutWorlContext), Category = "Hevedy|Dialog" )
		static int32 ShowMessageBox( const ETinyFileDialogMessageType _Type, const FText& _Title, const FString& _Message );

	// Message may NOT contain \n \t on windows 
	// DefaultInput if NULL it's a passwordBox returns NULL on cancel
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", CallableWithoutWorlContext, Keywords = "Prompts for an input box"), Category = "Hevedy|Dialog")
		static FString ShowInputBoxTFD( FString _Title, FString _Message, ETinyFileDialogInputType _DefaultInput );

	// FilterPatterns NULL | {"*.jpg","*.png"}
	// SingleFilterDescription NULL | "text files" returns NULL on cancel
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", CallableWithoutWorlContext, Keywords = "Prompts for an save file dialog"), Category = "Hevedy|Dialog")
		static FString ShowSaveFileDialogTFD( FString _Title, FString _DefaultPathAndFile, TArray<FString> _FilterPatterns, FString _SingleFilterDescription, bool _UseWin32);


	// FilterPatterns, /* NULL {"*.jpg","*.png"} 
	// SingleFilterDescription, NULL | "image files" 
	// AllowMultipleSelects, 0 or 1 *//* in case of multiple files, the separator is |  returns NULL on cancel 
	// NumSelectedFiles); returns a string array of file paths 
	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", CallableWithoutWorlContext, Keywords = "Prompts for an open file dialog"), Category = "Hevedy|Dialog")
		static FString ShowOpenFileDialogTFD( FString _Title, FString _DefaultPathAndFile, TArray<FString> _FilterPatterns, FString _SingleFilterDescription, bool _AllowMultipleSelects,
		bool _UseWin32, TArray<FString>& _OutStringArr, int32& _NumSelectedFiles);

	// Split this extensions by |
	UFUNCTION( BlueprintCallable, meta = ( WorldContext = "WorldContextObject", CallableWithoutWorlContext ), Category = "Hevedy|Dialog" )
		static bool ShowSelectFileDialog( const FString& _Title, const FString& _DefaultPath, const FString& _DefaultFile, TArray<FString>& _SelectedFiles, int32& _SelectedIndex, 
		const FString& _Extensions = "", const bool _MultiSelection = false );

	UFUNCTION(BlueprintCallable, meta = (WorldContext = "WorldContextObject", CallableWithoutWorlContext, Keywords = "Prompts for an select folder dialog"), Category = "Hevedy|Dialog")
		static FString ShowSelectFolderDialogTFD( FString _Title, FString _DefaultPath, bool _UseWin32 );

	UFUNCTION( BlueprintCallable, meta = ( WorldContext = "WorldContextObject", CallableWithoutWorlContext ), Category = "Hevedy|Dialog" )
		static bool ShowSelectFolderDialog( const FString& _Title, const FString& _DefaultPath, FString& _SelectedFolder );

};
