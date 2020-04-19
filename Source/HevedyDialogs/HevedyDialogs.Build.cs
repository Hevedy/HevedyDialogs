// Copyright 1998-2020 Epic Games, Inc. All Rights Reserved.
// Copyright 2020 Hevedy

using UnrealBuildTool;
using System.IO;
	
public class HevedyDialogs : ModuleRules
{
	public HevedyDialogs(ReadOnlyTargetRules Target) : base(Target)
	{
        PCHUsage = ModuleRules.PCHUsageMode.UseExplicitOrSharedPCHs;

		PrivateIncludePaths.Add(System.IO.Path.Combine(ModuleDirectory, "Private"));
		PublicIncludePaths.Add(System.IO.Path.Combine(ModuleDirectory, "Public"));

        PublicDependencyModuleNames.AddRange(
		new string[]
		{
            "Core"
			// ... add other public dependencies that you statically link with here ...
		}
		);
			
		if( Target.bBuildEditor == true ) {
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"CoreUObject",
					"Engine",
					"Slate",
					"SlateCore",
					"Projects",
					"DesktopPlatform"
					// ... add other public dependencies that you statically link with here ...
				}
			);
		} else {
			PrivateDependencyModuleNames.AddRange(
				new string[]
				{
					"CoreUObject",
					"Engine",
					"Slate",
					"SlateCore",
					"Projects"
					// ... add other public dependencies that you statically link with here ...
				}
			);
		}
		
		
		DynamicallyLoadedModuleNames.AddRange(
			new string[]
			{
				// ... add any modules that your module loads dynamically here ...
			}
		);
		
		string basePath = Path.GetDirectoryName(RulesCompiler.GetFileNameFromType(GetType()));
		string thirdPartyPath = Path.Combine(basePath, "..", "Thirdparty");
		PublicIncludePaths.Add(Path.Combine(thirdPartyPath, "TinyFileDialogs", "Includes"));
		
		
        if (Target.Platform == UnrealTargetPlatform.Win64)
		{

                string tinyFileDialogsBinaryPath = Path.Combine(thirdPartyPath, "TinyFileDialogs", "Libraries", "Win64");
				PublicAdditionalLibraries.Add(Path.Combine( tinyFileDialogsBinaryPath, "TinyFileDialogs.lib"));

		}
    }
}