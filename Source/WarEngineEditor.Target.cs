using UnrealBuildTool;

public class WarEngineEditorTarget : TargetRules
{
    public WarEngineEditorTarget(TargetInfo Target) : base(Target)
    {
        Type = TargetType.Editor;
        DefaultBuildSettings = BuildSettingsVersion.Latest;
        ExtraModuleNames.Add("WarEngine");
    }
}
