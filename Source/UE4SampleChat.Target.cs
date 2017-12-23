// Copyright ZX.Diablo 2017

using UnrealBuildTool;
using System.Collections.Generic;

public class UE4SampleChatTarget : TargetRules
{
	public UE4SampleChatTarget(TargetInfo Target) : base(Target)
	{
		Type = TargetType.Game;

		ExtraModuleNames.AddRange( new string[] { "UE4SampleChat" } );
	}
}
