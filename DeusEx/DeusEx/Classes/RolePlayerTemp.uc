// ============================================================================
//  RolePlayerTemp.uc ::
// ============================================================================
class RolePlayerTemp extends RolePlayer;


function PostBeginPlay()
{

    Super.PostBeginPlay();

    // Eventually, these will all be unique sounds per NPC specified in
    // the defaultproperties

    // change the sounds for chicks
    if (bIsFemale)
    {
        HitSound1 = Sound'FemalePainMedium';
        HitSound2 = Sound'FemalePainLarge';
        Die = Sound'FemaleDeath';

        SND_PainSmall = HitSound1;
        SND_PainMedium = HitSound2;
        SND_PainLarge = HitSound2;
        SND_Death = Die;
        SND_WaterDeath = Die;
        SND_EyePain = HitSound1;
        SND_Cough = HitSound1;
    }

    // change the sounds for kids
    //if (IsA('ChildMale') || IsA('ChildMale2'))
    //{
        HitSound1 = Sound'ChildPainMedium';
        HitSound2 = Sound'ChildPainLarge';
        Die = Sound'ChildDeath';

        SND_PainSmall = HitSound1;
        SND_PainMedium = HitSound2;
        SND_PainLarge = HitSound2;
        SND_Death = Die;
        SND_WaterDeath = Die;
        SND_EyePain = HitSound1;
        SND_Cough = HitSound1;
    //}
}


/*

     WalkingSpeed=0.256000
     walkAnimMult=1.150000
     GroundSpeed=150.000000
     WaterSpeed=80.000000
     AirSpeed=160.000000

*/
DefaultProperties
{

     SkillPointsTotal=0
     SkillPointsAvail=0
     Credits=0
     Energy=0
     EnergyMax=0

     bCanStrafe=True
     AccelRate=500.000000
     UnderWaterTime=20.000000
     AttitudeToPlayer=ATTITUDE_Ignore
     VisibilityThreshold=0.010000
     DrawType=DT_Mesh


    ANIM_CrouchWalk=Crouch
    ANIM_Walk2H=Walk
    ANIM_Walk=Walk
    ANIM_Strafe2H=Run
    ANIM_Strafe=Run
    ANIM_RunShoot2H=Run
    ANIM_RunShoot=Run
    ANIM_Run=Run
    ANIM_Panic=Panic
    ANIM_TreadShoot=Tread
    ANIM_Tread=Tread
    ANIM_BreatheLight2H=BreatheLight
    ANIM_BreatheLight=BreatheLight
    ANIM_Pickup=Pickup
    ANIM_Jump=Jump
    ANIM_Land=Land
    ANIM_CrouchShoot=Crouch
    ANIM_Crouch=Crouch
    ANIM_Stand=Stand
    ANIM_Attack=Attack
    ANIM_Shoot2H=Attack
    ANIM_Shoot=Attack
    ANIM_Reload=GestureBoth
    ANIM_DeathBack=DeathBack
    ANIM_DeathFront=DeathFront
    ANIM_WaterDeath=WaterDeath

     AnimSequence=Idle

     CarcassType=Class'DeusEx.ChildMaleCarcass'
     BaseEyeHeight=26.000000
     HealthHead=50
     HealthTorso=50
     HealthLegLeft=50
     HealthLegRight=50
     HealthArmLeft=50
     HealthArmRight=50
     Mesh=LodMesh'DeusExCharacters.GMK_DressShirt'
     MultiSkins(0)=Texture'DeusExCharacters.Skins.ChildMaleTex0'
     MultiSkins(1)=Texture'DeusExCharacters.Skins.ChildMaleTex1'
     MultiSkins(2)=Texture'DeusExCharacters.Skins.PantsTex3'
     MultiSkins(3)=Texture'DeusExCharacters.Skins.ChildMaleTex0'
     MultiSkins(4)=Texture'DeusExItems.Skins.PinkMaskTex'
     MultiSkins(5)=Texture'DeusExCharacters.Skins.ChildMaleTex1'
     MultiSkins(6)=Texture'DeusExItems.Skins.GrayMaskTex'
     MultiSkins(7)=Texture'DeusExItems.Skins.BlackMaskTex'
     CollisionRadius=17.000000
     CollisionHeight=32.500000
     Mass=80.000000
     Buoyancy=85.000000
     BindName="Child"
     FamiliarName="Child"
     UnfamiliarName="Child"
}
