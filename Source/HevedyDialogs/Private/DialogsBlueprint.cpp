// Copyright 2019-2020 f00n.com
// Copyright 2020 Hevedy

#include "DialogsBlueprint.h"
#include "tinyfiledialogs.h"
#if WITH_EDITOR
#include "IDesktopPlatform.h"
#include "DesktopPlatformModule.h"
#endif
#include "Misc/MessageDialog.h"
#include "Engine/Engine.h"
#include <string>

/**
Show a message box with options for various responses.
*/
int32 UDialogsBlueprint::ShowMessageBoxTFD( FString _Title, FString _Message, ETinyFileDialogMessageType _DialogType, ETinyFileDialogIconType _IconType, int32 _DefaultButton, bool _UseWin32 ) {
	
	FString aDialogType;
	switch (_DialogType)
	{
		case ETinyFileDialogMessageType::OK:
			aDialogType = "ok";
			break;
		case ETinyFileDialogMessageType::OKCANCEL:
			aDialogType = "okcancel";
			break;
		case ETinyFileDialogMessageType::YESNO:
			aDialogType = "yesno";
			break;
		case ETinyFileDialogMessageType::YESNOCANCEL:
			aDialogType = "yesnocancel";
			break;
		default:
			aDialogType = "okcancel";
	}

	FString aIconType;
	switch (_IconType)
	{
		case ETinyFileDialogIconType::INFO:
			aIconType = "info";
			break;
		case ETinyFileDialogIconType::WARNING:
			aIconType = "warning";
			break;
		case ETinyFileDialogIconType::ERROR:
			aIconType = "error";
			break;
		case ETinyFileDialogIconType::QUESTION:
			aIconType = "question";
			break;
		default:
			aIconType = "info";
	}

	int32 outValue = 0;
	if (_UseWin32) {
		//windows dialog
		outValue = tinyfd_messageBoxW(*_Title, *_Message, *aDialogType, *aIconType, _DefaultButton );
	} else {
		//cross compatible
		outValue = tinyfd_messageBox(TCHAR_TO_ANSI(*_Title), TCHAR_TO_ANSI(*_Message), TCHAR_TO_ANSI(*aDialogType), TCHAR_TO_ANSI(*aIconType), _DefaultButton );
	}

	return outValue;
}

//https://docs.unrealengine.com/en-US/API/Runtime/Core/Misc/FMessageDialog/index.html
int32 UDialogsBlueprint::ShowMessageBox( const ETinyFileDialogMessageType _Type, const FText& _Title, const FString& _Message ) {
	
#if WITH_EDITOR
	EAppMsgType::Type type = EAppMsgType::Type::OkCancel;
	switch ( _Type ) {
		case ETinyFileDialogMessageType::OK:
			type = EAppMsgType::Type::Ok;
			break;
		case ETinyFileDialogMessageType::OKCANCEL:
			type = EAppMsgType::Type::OkCancel;
			break;
		case ETinyFileDialogMessageType::YESNO:
			type = EAppMsgType::Type::YesNo;
			break;
		case ETinyFileDialogMessageType::YESNOCANCEL:
			type = EAppMsgType::Type::YesNoCancel;
			break;
		default:
			type = EAppMsgType::Type::OkCancel;
	}
	int32 index = 0;
	const EAppReturnType::Type outType = FMessageDialog::Open( type, FText::FromString( *_Message ), &_Title );
	switch ( outType ) {
		case EAppReturnType::Type::Cancel:
			index = 0;
			break;
		case EAppReturnType::Type::Continue:
			index = 1;
			break;
		case EAppReturnType::Type::No:
			index = 0;
			break;
		case EAppReturnType::Type::NoAll:
			index = 0;
			break;
		case EAppReturnType::Type::Ok:
			index = 1;
			break;
		case EAppReturnType::Type::Retry:
			index = 2;
			break;
		case EAppReturnType::Type::Yes:
			index = 1;
			break;
		case EAppReturnType::Type::YesAll:
			index = 1;
			break;
		default:
			break;
	}
	return index;
#else
	return 0;
#endif
}

/**
Show an input box with a message and return the user input as an FString
*/
FString UDialogsBlueprint::ShowInputBoxTFD( FString _Title, FString _Message, ETinyFileDialogInputType _DefaultInput ) {
	
	FString outValue;
	const char* aDefaultInput;
	switch (DefaultInput)
	{
		case ETinyFileDialogInputType::PLAINTEXT:
			aDefaultInput = "";
			break;
		case ETinyFileDialogInputType::PASSWORD:
			aDefaultInput = NULL;
			break;
		default:
			aDefaultInput = "";
	}

	outValue = tinyfd_inputBox( TCHAR_TO_ANSI(*_Title), TCHAR_TO_ANSI(*_Message), aDefaultInput );

	return outValue;
}


/**
Open a save file dialog using a default file and path and filename.
Filter patterns is an array of acceptable filetypes. ie. { *.jpg, *.png }
*/
FString UDialogsBlueprint::ShowSaveFileDialogTFD( FString _Title, FString _DefaultPathAndFile, TArray<FString> _FilterPatterns, FString _SingleFilterDescription, bool _UseWin32) {

	FString result;
	if (_UseWin32) {
		int32 arrLength = _FilterPatterns.Num();
		TArray<wchar_t const*> tmpWCharArr;
		tmpWCharArr.SetNum(arrLength);
		//Loop the FilterPatterns array and copy the pointer values to std allocated storage.
		for (int32 i = 0; i < arrLength; i++)
		{
			tmpWCharArr[i] = *_FilterPatterns[i];
		}
		result = tinyfd_saveFileDialogW( *_Title, *_DefaultPathAndFile, arrLength, tmpWCharArr.GetData(), *_SingleFilterDescription );
	} else {
		//use the incoming FilterPatterns array length to create a const char array
		int32 arrLength = _FilterPatterns.Num();
		TArray<char const*> tmpCharArr;
		tmpCharArr.SetNum(arrLength);

		//Let std do the storage from char** to char
		TArray<std::string> tempStrings;
		tempStrings.SetNum(arrLength);

		//Loop the FilterPatterns array and copy the pointer values to std allocated storage.
		for (int32 i = 0; i < arrLength; i++)
		{

			//store a copy of the value from the FString array
			std::string fp = TCHAR_TO_ANSI(*_FilterPatterns[i]);
			tempStrings[i] = fp;

			//Get the char const* value from the array
			tmpCharArr[i] = tempStrings[i].c_str();

		}

		//Execute the file dialog
		result = tinyfd_saveFileDialog( TCHAR_TO_ANSI(*_Title), TCHAR_TO_ANSI(*_DefaultPathAndFile), arrLength, tmpCharArr.GetData(), TCHAR_TO_ANSI(*_SingleFilterDescription) );

	}

	return result;
}

/**
Open a dialog and let the user select one or multiple files
Filter patterns is an array of acceptable filetypes. ie. { *.jpg, *.png }
*/
FString UDialogsBlueprint::ShowOpenFileDialogTFD( FString _Title, FString _DefaultPathAndFile, TArray<FString> _FilterPatterns, FString _SingleFilterDescription, 
													bool _AllowMultipleSelects, bool _UseWin32, TArray<FString>& _OutStringArr, int32& _NumSelectedFiles ) {

	if (_UseWin32)
	{
		//Filter settings
		int32 arrLength = _FilterPatterns.Num();
		TArray<wchar_t const*> tmpWCharArr;
		tmpWCharArr.SetNum(arrLength);

		//Loop the FilterPatterns array and copy the pointer values to std allocated storage.
		for (int32 i = 0; i < arrLength; i++) {
			tmpWCharArr[i] = *_FilterPatterns[i];
		}

		//Call the tinyfd function
		FString result = tinyfd_openFileDialogW( *_Title, *_DefaultPathAndFile, arrLength, tmpWCharArr.GetData(), *_SingleFilterDescription, _AllowMultipleSelects );

		if (_AllowMultipleSelects && result != "") {
			_NumSelectedFiles = result.ParseIntoArray(_OutStringArr, TEXT("|"));
		}

		return result;
	} else {
		//Filter settings
		int32 arrLength = _FilterPatterns.Num();
		TArray<char const*> tmpCharArr;
		tmpCharArr.SetNum(arrLength);

		//Let std do the storage from char** to char
		TArray<std::string> tempStrings;
		tempStrings.SetNum(arrLength);

		//Loop the FilterPatterns array and copy the pointer values to std allocated storage.
		for (int i = 0; i < arrLength; i++) {
			//store a copy of the value from the FString array
			std::string fp = TCHAR_TO_ANSI(*_FilterPatterns[i]);
			tempStrings[i] = fp;

			//Get the char const* value from the array
			tmpCharArr[i] = tempStrings[i].c_str();
		}
		
		//Call the tinyfd function
		FString result = tinyfd_openFileDialog( TCHAR_TO_ANSI(*_Title), TCHAR_TO_ANSI(*_DefaultPathAndFile), arrLength, tmpCharArr.GetData(), TCHAR_TO_ANSI(*_SingleFilterDescription), 
												_AllowMultipleSelects );

		if (_AllowMultipleSelects && result != "") {
			NumSelectedFiles = result.ParseIntoArray(_OutStringArr, TEXT("|"));
		}
		return result;

	}
}


// FBX file (*.fbx)|*.fbx|OBJ file (*.obj)|*.obj"  Example for one kind of file
//EFileDialogFlags::None
// "All Font Files (*.ttf, *.ttc, *.otf, *.otc)|*.ttf;*.ttc;*.otf;*.otc|TrueType fonts (*.ttf, *.ttc)|*.ttf;*.ttc|OpenType fonts (*.otf, *.otc)|*.otf;*.otc"
bool UDialogsBlueprint::ShowSelectFileDialog( const FString& _Title, const FString& _DefaultPath, const FString& _DefaultFile, TArray<FString>& _SelectedFiles, int32& _SelectedIndex, 
												const FString& _Extensions, const bool _MultiSelection) {
#if WITH_EDITOR
	const int32 var = _MultiSelection ? 1 : 0;
	return FDesktopPlatformModule::Get()->OpenFileDialog( GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle(), *_Title, *_DefaultPath, *_DefaultFile, *_Extensions, 
															var, _SelectedFiles, _SelectedIndex );
#else
	return false;
#endif
}

/**
Open a dialog to let the user select a folder
*/
FString UDialogsBlueprint::ShowSelectFolderDialogTFD( FString _Title, FString _DefaultPath, bool _UseWin32) {
	if (_UseWin32) {
		FString result = tinyfd_selectFolderDialogW(*_Title, *_DefaultPath);
		if (result == "") {
			result = _DefaultPath;
		}
	} else {
		FString result = tinyfd_selectFolderDialog(TCHAR_TO_ANSI(*_Title), TCHAR_TO_ANSI(*_DefaultPath));
		if (result == "") {
			result = _DefaultPath;
		}
	}
	return result;
}

// https://docs.unrealengine.com/en-US/API/Developer/DesktopPlatform/IDesktopPlatform/index.html
bool UDialogsBlueprint::ShowSelectFolderDialog( const FString& _Title, const FString& _DefaultPath, FString& _SelectedFolder ) {
#if WITH_EDITOR
	return FDesktopPlatformModule::Get()->OpenDirectoryDialog( GEngine->GameViewport->GetWindow()->GetNativeWindow()->GetOSWindowHandle(), *_Title, *_DefaultPath, _SelectedFolder );
#else
	return false;
#endif
}
