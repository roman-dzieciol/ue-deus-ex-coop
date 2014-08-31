//-----------------------------------------------------------
//
//-----------------------------------------------------------
class MenuPlayer extends DeusExPlayer;


function InitializeSubSystems()
{
    // Spawn the BarkManager
    if (BarkManager == None)
        BarkManager = Spawn(class'BarkManager', Self);

    // Spawn the Color Manager
    CreateColorThemeManager();
    ThemeManager.SetOwner(self);

    // install the augmentation system if not found
    if (AugmentationSystem == None)
    {
        AugmentationSystem = Spawn(class'AugmentationManager', Self);
        AugmentationSystem.CreateAugmentations(Self);
        AugmentationSystem.AddDefaultAugmentations();
        AugmentationSystem.SetOwner(Self);
    }
    else
    {
        AugmentationSystem.SetPlayer(Self);
        AugmentationSystem.SetOwner(Self);
    }

    // install the skill system if not found
    if (SkillSystem == None)
    {
        SkillSystem = Spawn(class'SkillManager', Self);
        SkillSystem.CreateSkills(Self);
    }
    else
    {
        SkillSystem.SetPlayer(Self);
    }
}

exec function Fire(optional float F)
{
    ShowMainMenu();
}

DefaultProperties
{
     BaseEyeHeight=40.000000
     CollisionRadius=20.000000
     CollisionHeight=47.500000

     AirSpeed=400.000000
     Visibility=0
     AttitudeToPlayer=ATTITUDE_Friendly
     MenuName="Spectator"
     bHidden=True
     bCollideActors=False
     bCollideWorld=False
     bBlockActors=False
     bBlockPlayers=False
     bProjTarget=False

}
