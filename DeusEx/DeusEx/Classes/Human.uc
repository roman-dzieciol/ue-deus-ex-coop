//=============================================================================
// Human.
//=============================================================================
class Human extends RpPlayerPawn
    abstract;

var float mpGroundSpeed;
var float mpWaterSpeed;
var float humanAnimRate;


// SW-- Variables

var name ANIM_CrouchWalk;
var name ANIM_Walk2H;
var name ANIM_Walk;
var name ANIM_Run;
var name ANIM_Strafe2H;
var name ANIM_Strafe;
var name ANIM_RunShoot2H;
var name ANIM_RunShoot;
var name ANIM_Panic;
var name ANIM_TreadShoot;
var name ANIM_Tread;
var name ANIM_BreatheLight2H;
var name ANIM_BreatheLight;
var name ANIM_Pickup;
var name ANIM_Jump;
var name ANIM_Land;
var name ANIM_CrouchShoot;
var name ANIM_Crouch;
var name ANIM_Stand;
var name ANIM_Attack;
var name ANIM_Shoot2H;
var name ANIM_Shoot;
var name ANIM_Reload;
var name ANIM_DeathBack;
var name ANIM_DeathFront;
var name ANIM_WaterDeath;

var Sound SND_Gasp;
var Sound SND_WaterDeath;
var Sound SND_Death;
var Sound SND_Drown;
var Sound SND_PainSmall;
var Sound SND_BodyHit;
var Sound SND_EyePain;
var Sound SND_Cough;
var Sound SND_PainMedium;
var Sound SND_PainLarge;

// --SW





replication
{
    reliable if (( Role == ROLE_Authority ) && bNetOwner )
        humanAnimRate;
}

function Bool IsFiring()
{
    if ((Weapon != None) && ( Weapon.IsInState('NormalFire') || Weapon.IsInState('ClientFiring') ) )
        return True;
    else
        return False;
}

function Bool HasTwoHandedWeapon()
{
    if ((Weapon != None) && (Weapon.Mass >= 30))
        return True;
    else
        return False;
}

//
// animation functions
//
function PlayTurning()
{
//  ClientMessage("PlayTurning()");
    if (bForceDuck || bCrouchOn || IsLeaning())
        TweenAnim(ANIM_CrouchWalk, 0.1);
    else
    {
        if (HasTwoHandedWeapon())
            TweenAnim(ANIM_Walk2H, 0.1);
        else
            TweenAnim(ANIM_Walk, 0.1);
    }
}

function TweenToWalking(float tweentime)
{
//  ClientMessage("TweenToWalking()");
    if (bForceDuck || bCrouchOn)
        TweenAnim(ANIM_CrouchWalk, tweentime);
    else
    {
        if (HasTwoHandedWeapon())
            TweenAnim(ANIM_Walk2H, tweentime);
        else
            TweenAnim(ANIM_Walk, tweentime);
    }
}

function PlayWalking()
{
    local float newhumanAnimRate;

    newhumanAnimRate = humanAnimRate;

    // UnPhysic.cpp walk speed changed by proportion 0.7/0.3 (2.33), but that looks too goofy (fast as hell), so we'll try something a little slower
    if ( Level.NetMode != NM_Standalone )
        newhumanAnimRate = humanAnimRate * 1.75;

    //  ClientMessage("PlayWalking()");
    if (bForceDuck || bCrouchOn)
        LoopAnim(ANIM_CrouchWalk, newhumanAnimRate);
    else
    {
        if (HasTwoHandedWeapon())
            LoopAnim(ANIM_Walk2H, newhumanAnimRate);
        else
            LoopAnim(ANIM_Walk, newhumanAnimRate);
    }
}

function TweenToRunning(float tweentime)
{
//  ClientMessage("TweenToRunning()");
    if (bIsWalking)
    {
        TweenToWalking(0.1);
        return;
    }

    if (IsFiring())
    {
        if (aStrafe != 0)
        {
            if (HasTwoHandedWeapon())
                PlayAnim(ANIM_Strafe2H,humanAnimRate, tweentime);
            else
                PlayAnim(ANIM_Strafe,humanAnimRate, tweentime);
        }
        else
        {
            if (HasTwoHandedWeapon())
                PlayAnim(ANIM_RunShoot2H,humanAnimRate, tweentime);
            else
                PlayAnim(ANIM_RunShoot,humanAnimRate, tweentime);
        }
    }
    else if (bOnFire)
        PlayAnim(ANIM_Panic,humanAnimRate, tweentime);
    else
    {
        if (HasTwoHandedWeapon())
            PlayAnim(ANIM_RunShoot2H,humanAnimRate, tweentime);
        else
            PlayAnim(ANIM_Run,humanAnimRate, tweentime);
    }
}

function PlayRunning()
{
//  ClientMessage("PlayRunning()");
    if (IsFiring())
    {
        if (aStrafe != 0)
        {
            if (HasTwoHandedWeapon())
                LoopAnim(ANIM_Strafe2H, humanAnimRate);
            else
                LoopAnim(ANIM_Strafe, humanAnimRate);
        }
        else
        {
            if (HasTwoHandedWeapon())
                LoopAnim(ANIM_RunShoot2H, humanAnimRate);
            else
                LoopAnim(ANIM_RunShoot, humanAnimRate);
        }
    }
    else if (bOnFire)
        LoopAnim(ANIM_Panic, humanAnimRate);
    else
    {
        if (HasTwoHandedWeapon())
            LoopAnim(ANIM_RunShoot2H, humanAnimRate);
        else
            LoopAnim(ANIM_Run, humanAnimRate);
    }
}

function TweenToWaiting(float tweentime)
{
//  ClientMessage("TweenToWaiting()");
    if (IsInState('PlayerSwimming') || (Physics == PHYS_Swimming))
    {
        if (IsFiring())
            LoopAnim(ANIM_TreadShoot);
        else
            LoopAnim(ANIM_Tread);
    }
    else if (IsLeaning() || bForceDuck)
        TweenAnim(ANIM_CrouchWalk, tweentime);
    else if (((AnimSequence == 'Pickup') && bAnimFinished) || ((AnimSequence != 'Pickup') && !IsFiring()))
    {
        if (HasTwoHandedWeapon())
            TweenAnim(ANIM_BreatheLight2H, tweentime);
        else
            TweenAnim(ANIM_BreatheLight, tweentime);
    }
}

function PlayWaiting()
{
//  ClientMessage("PlayWaiting()");
    if (IsInState('PlayerSwimming') || (Physics == PHYS_Swimming))
    {
        if (IsFiring())
            LoopAnim(ANIM_TreadShoot);
        else
            LoopAnim(ANIM_Tread);
    }
    else if (IsLeaning() || bForceDuck)
        TweenAnim(ANIM_CrouchWalk, 0.1);
    else if (!IsFiring())
    {
        if (HasTwoHandedWeapon())
            LoopAnim(ANIM_BreatheLight2H);
        else
            LoopAnim(ANIM_BreatheLight);
    }

}

function PlaySwimming()
{
//  ClientMessage("PlaySwimming()");
    LoopAnim(ANIM_Tread);
}

function TweenToSwimming(float tweentime)
{
//  ClientMessage("TweenToSwimming()");
    TweenAnim(ANIM_Tread, tweentime);
}

function PlayInAir()
{
//  ClientMessage("PlayInAir()");
    if (!bIsCrouching && (AnimSequence != ANIM_Jump))
        PlayAnim(ANIM_Jump,3.0,0.1);
}

function PlayLanded(float impactVel)
{
//  ClientMessage("PlayLanded()");
    PlayFootStep();
    if (!bIsCrouching)
        PlayAnim(ANIM_Land,3.0,0.1);
}

function PlayDuck()
{
//  ClientMessage("PlayDuck()");
    if ((AnimSequence != ANIM_Crouch) && (AnimSequence != ANIM_CrouchWalk))
    {
        if (IsFiring())
            PlayAnim(ANIM_CrouchShoot,,0.1);
        else
            PlayAnim(ANIM_Crouch,,0.1);
    }
    else
        TweenAnim(ANIM_CrouchWalk, 0.1);
}

function PlayRising()
{
//  ClientMessage("PlayRising()");
    PlayAnim(ANIM_Stand,,0.1);
}

function PlayCrawling()
{
//  ClientMessage("PlayCrawling()");
    if (IsFiring())
        LoopAnim(ANIM_CrouchShoot);
    else
        LoopAnim(ANIM_CrouchWalk);
}

function PlayFiring()
{
    local DeusExWeapon W;

//  ClientMessage("PlayFiring()");

    W = DeusExWeapon(Weapon);

    if (W != None)
    {
        if (IsInState('PlayerSwimming') || (Physics == PHYS_Swimming))
            LoopAnim(ANIM_TreadShoot,,0.1);
        else if (W.bHandToHand)
        {
            if (bAnimFinished || (AnimSequence != ANIM_Attack))
                PlayAnim(ANIM_Attack,,0.1);
        }
        else if (bIsCrouching || IsLeaning())
            LoopAnim(ANIM_CrouchShoot,,0.1);
        else
        {
            if (HasTwoHandedWeapon())
                LoopAnim(ANIM_Shoot2H,,0.1);
            else
                LoopAnim(ANIM_Shoot,,0.1);
        }
    }
}

function PlayWeaponSwitch(Weapon newWeapon)
{
//  ClientMessage("PlayWeaponSwitch()");
    if (!bIsCrouching && !bForceDuck && !bCrouchOn && !IsLeaning())
        PlayAnim(ANIM_Reload);
}

function PlayDying(name damageType, vector hitLoc)
{
    local Vector X, Y, Z;
    local float dotp;

//  ClientMessage("PlayDying()");
    GetAxes(Rotation, X, Y, Z);
    dotp = (Location - HitLoc) dot X;

    if (Region.Zone.bWaterZone)
    {
        PlayAnim(ANIM_WaterDeath,,0.1);
    }
    else
    {
        // die from the correct side
        if (dotp < 0.0)     // shot from the front, fall back
            PlayAnim(ANIM_DeathBack,,0.1);
        else                // shot from the back, fall front
            PlayAnim(ANIM_DeathFront,,0.1);
    }

    PlayDyingSound();
}

//
// sound functions
//

function float RandomPitch()
{
    return (1.1 - 0.2*FRand());
}


function Gasp()
{
    PlaySound(SND_Gasp, SLOT_Pain,,,, RandomPitch());
}

function PlayDyingSound()
{
    if (Region.Zone.bWaterZone)
        PlaySound(SND_WaterDeath, SLOT_Pain,,,, RandomPitch());
    else
        PlaySound(SND_Death, SLOT_Pain,,,, RandomPitch());
}

function PlayTakeHitSound(int Damage, name damageType, int Mult)
{
    local float rnd;

    if ( Level.TimeSeconds - LastPainSound < FRand() + 0.5)
        return;

    LastPainSound = Level.TimeSeconds;

    if (Region.Zone.bWaterZone)
    {
        if (damageType == 'Drowned')
        {
            if (FRand() < 0.8)
                PlaySound(SND_Drown, SLOT_Pain, FMax(Mult * TransientSoundVolume, Mult * 2.0),,, RandomPitch());
        }
        else
            PlaySound(SND_PainSmall, SLOT_Pain, FMax(Mult * TransientSoundVolume, Mult * 2.0),,, RandomPitch());
    }
    else
    {
        // Body hit sound for multiplayer only
        if (((damageType=='Shot') || (damageType=='AutoShot'))  && ( Level.NetMode != NM_Standalone ))
        {
            PlaySound(SND_BodyHit, SLOT_Pain, FMax(Mult * TransientSoundVolume, Mult * 2.0),,, RandomPitch());
        }

        if ((damageType == 'TearGas') || (damageType == 'HalonGas'))
            PlaySound(SND_EyePain, SLOT_Pain, FMax(Mult * TransientSoundVolume, Mult * 2.0),,, RandomPitch());
        else if (damageType == 'PoisonGas')
            PlaySound(SND_Cough, SLOT_Pain, FMax(Mult * TransientSoundVolume, Mult * 2.0),,, RandomPitch());
        else
        {
            rnd = FRand();
            if (rnd < 0.33)
                PlaySound(SND_PainSmall, SLOT_Pain, FMax(Mult * TransientSoundVolume, Mult * 2.0),,, RandomPitch());
            else if (rnd < 0.66)
                PlaySound(SND_PainMedium, SLOT_Pain, FMax(Mult * TransientSoundVolume, Mult * 2.0),,, RandomPitch());
            else
                PlaySound(SND_PainLarge, SLOT_Pain, FMax(Mult * TransientSoundVolume, Mult * 2.0),,, RandomPitch());
        }
        AISendEvent('LoudNoise', EAITYPE_Audio, FMax(Mult * TransientSoundVolume, Mult * 2.0));
    }
}



function UpdateAnimRate( float augValue )
{
    // SW-- humanAnimRate
    if ( Level.NetMode != NM_Standalone )
    {
        if ( augValue == -1.0 )
            humanAnimRate = (Default.GroundSpeed/320.0);
        else
            humanAnimRate = (Default.GroundSpeed/320.0) * augValue * 0.85;    // Scale back about 15% so were not too fast
    }
    /*
    if ( Level.NetMode != NM_Standalone )
    {
        if ( augValue == -1.0 )
            humanAnimRate = (Default.mpGroundSpeed/320.0);
        else
            humanAnimRate = (Default.mpGroundSpeed/320.0) * augValue * 0.85;    // Scale back about 15% so were not too fast
    }*/
    // --SW

}

simulated function PreBeginPlay()
{
    Super.PreBeginPlay();

    // SW-- SP Speeds
    /*
    // If this is a netgame, then override defaults
    if ( Level.NetMode != NM_StandAlone )
    {
        GroundSpeed = mpGroundSpeed;
        WaterSpeed = mpWaterSpeed;
        humanAnimRate = (GroundSpeed/320.0);
    }*/
    // --SW

}

defaultproperties
{
     mpGroundSpeed=230.000000
     mpWaterSpeed=110.000000
     humanAnimRate=1.000000
     bIsHuman=True
     WaterSpeed=300.000000
     AirSpeed=4000.000000
     AccelRate=1000.000000
     JumpZ=300.000000
     BaseEyeHeight=40.000000
     UnderWaterTime=20.000000
     CollisionRadius=20.000000
     CollisionHeight=47.500000
     Mass=150.000000
     Buoyancy=155.000000
     RotationRate=(Pitch=4096,Yaw=50000,Roll=3072)

    ANIM_CrouchWalk=CrouchWalk
    ANIM_Walk2H=Walk2H
    ANIM_Walk=Walk
    ANIM_Strafe2H=Strafe2H
    ANIM_Strafe=Strafe
    ANIM_RunShoot2H=RunShoot2H
    ANIM_RunShoot=RunShoot
    ANIM_Run=Run
    ANIM_Panic=Panic
    ANIM_TreadShoot=TreadShoot
    ANIM_Tread=Tread
    ANIM_BreatheLight2H=BreatheLight2H
    ANIM_BreatheLight=BreatheLight
    ANIM_Pickup=Pickup
    ANIM_Jump=Jump
    ANIM_Land=Land
    ANIM_CrouchShoot=CrouchShoot
    ANIM_Crouch=Crouch
    ANIM_Stand=Stand
    ANIM_Attack=Attack
    ANIM_Shoot2H=Shoot2H
    ANIM_Shoot=Shoot
    ANIM_Reload=Reload
    ANIM_DeathBack=DeathBack
    ANIM_DeathFront=DeathFront
    ANIM_WaterDeath=WaterDeath

    AnimSequence=Fighter


    SND_Gasp=sound'MaleGasp'
    SND_WaterDeath=sound'MaleWaterDeath'
    SND_Death=sound'MaleDeath'
    SND_Drown=sound'MaleDrown'
    SND_PainSmall=sound'MalePainSmall'
    SND_BodyHit=sound'BodyHit'
    SND_EyePain=sound'MaleEyePain'
    SND_Cough=sound'MaleCough'
    SND_PainMedium=sound'MalePainMedium'
    SND_PainLarge=sound'MalePainLarge'
}
