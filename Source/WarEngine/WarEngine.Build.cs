using UnrealBuildTool;

public class WarEngine : ModuleRules
{
    public WarEngine(ReadOnlyTargetRules Target) : base(Target)
    {
        PCHUsage = PCHUsageMode.UseExplicitOrSharedPCHs;

        PublicDependencyModuleNames.AddRange(new string[]
        {
            "Core", "CoreUObject", "Engine", "InputCore", "CesiumRuntime"
        });
    }
}
