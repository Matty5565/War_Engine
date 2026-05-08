using UnrealBuildTool;

public class WarEngineTarget : TargetRules
{
    public WarEngineTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        IncludeOrderVersion = EngineIncludeOrderVersion.Unreal5_7;
        ExtraModuleNames.Add("WarEngine");
    }
}
