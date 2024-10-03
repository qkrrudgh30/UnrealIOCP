// Copyright Epic Games, Inc. All Rights Reserved.

using System.IO;
using UnrealBuildTool;

public class Client : ModuleRules
{
	public Client(ReadOnlyTargetRules Target) : base(Target)
	{
		PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

		PublicDependencyModuleNames.AddRange(new string[] 
		{ 
			"Core", "CoreUObject", "Engine", "InputCore", "HeadMountedDisplay", "EnhancedInput",

			"Sockets", "Networking",
			
		});

		PrivateDependencyModuleNames.AddRange(new string[] 
		{ 
			"ProtobufCore", // 언리얼 엔진 내부적으로는 프로토버프 3.18 버전을 고정적으로 사용중. 우리는 1.17버전을 사용중이라, 우리만의 모듈을 추가한것.
		});
		
		PublicIncludePaths.AddRange(new string[] 
		{ 
			Path.Combine(ModuleDirectory, "../../../Server/ServerCore"),
		});
		
		PublicAdditionalLibraries.AddRange(new string[] 
		{ 
			Path.Combine(ModuleDirectory, "../../../Server/Libraries/ServerCore/Debug", "ServerCore.lib"),
			Path.Combine(ModuleDirectory, "../../../Server/Libraries/ServerCore/Release", "ServerCore.lib"),
		});
		
	}
}
