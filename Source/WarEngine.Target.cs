using UnrealBuildTool;

public class WarEngineTarget : TargetRules
{
    public WarEngineTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Game;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        ExtraModuleNames.Add("WarEngine");
    }
}
