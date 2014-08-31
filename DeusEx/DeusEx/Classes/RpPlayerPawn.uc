// ============================================================================
//  RpNetPlayer.uc ::
// ============================================================================
class RpPlayerPawn extends DeusExPlayer
    abstract;

var RpPendingAdjustment PendingAdjustment;
var float MaxResponseTime;
var vector LastAckedAccel;
var float CrouchHeight;
var byte OldFireInfo;
var bool bUpdating;

// TODO :: minimize client updates on jump
// TODO :: update check error sizes



// ============================================================================
//  replication
// ============================================================================
replication
{
    unreliable if( Role == ROLE_Authority )
        RpNet_ClientAckGoodMove;

    unreliable if( Role == ROLE_Authority )
        RpNet_ClientAdjustPosition, RpNet_ShortClientAdjustPosition, RpNet_VeryShortClientAdjustPosition;

    unreliable if( Role < ROLE_Authority )
        RpNet_ServerMove, RpNet_OldServerMove, RpNet_DualServerMove;

    reliable if( Role < ROLE_Authority )
        RpNet_ServerUpdateFiring;
}


// ============================================================================
//  Lifespan
// ============================================================================

simulated event PostBeginPlay()
{
    PendingAdjustment = Spawn(class'RpPendingAdjustment',self);
    MaxResponseTime = Default.MaxResponseTime * Level.TimeDilation;

    Super.PostBeginPlay();
}

simulated event Destroyed()
{
    Super.Destroyed();

    if( PendingAdjustment != None )
    {
        PendingAdjustment.Destroy();
        PendingAdjustment = None;
    }
}

// ============================================================================
//  Physics
// ============================================================================

function ForceCrouch()
{
    SetBasedPawnSize(Default.CollisionRadius, CrouchHeight);
    bIsCrouching = true;
    PlayDuck();
}

function bool CannotJumpNow()
{
    return false;
}



// ============================================================================
//  STATE PlayerWalking
// ============================================================================
state PlayerWalking
{
    event BeginState()
    {
        xLog("BeginState");
        Super.BeginState();
    }

    event EndState()
    {
        xLog("EndState");
        Super.EndState();
    }

    function PlayerMove( float DeltaTime )
    {
        local vector X,Y,Z, NewAccel;
        local rotator OldRotation;
        local bool  bSaveJump;
//        local EDodgeDir OldDodge;
//        local eDodgeDir DodgeMove;
//        local float Speed2D;
//        local name AnimGroupName;

        GetAxes(Rotation,X,Y,Z);

        aForward *= 0.4;
        aStrafe  *= 0.4;
        aLookup  *= 0.24;
        aTurn    *= 0.24;

        // Update acceleration.
        NewAccel = aForward*X + aStrafe*Y;
        NewAccel.Z = 0;
        if( VSize(NewAccel) < 1.0 )
            NewAccel = vect(0,0,0);

        //ViewRotation = Rotation;
        if( Physics == PHYS_Walking )
        {
//            //if walking, look up/down stairs - unless player is rotating view
//            if( !bKeyboardLook && (bLook == 0) )
//            {
//                if( bLookUpStairs )
//                    ViewRotation.Pitch = FindStairRotation(deltaTime);
//                else if( bCenterView )
//                {
//                    ViewRotation.Pitch = ViewRotation.Pitch & 65535;
//                    if (ViewRotation.Pitch > 32768)
//                        ViewRotation.Pitch -= 65536;
//                    ViewRotation.Pitch = ViewRotation.Pitch * (1 - 12 * FMin(0.0833, deltaTime));
//                    if ( Abs(ViewRotation.Pitch) < 1000 )
//                        ViewRotation.Pitch = 0;
//                }
//            }

//            Speed2D = Sqrt(Velocity.X * Velocity.X + Velocity.Y * Velocity.Y);
//
//            //add bobbing when walking
//            if( !bShowMenu )
//                CheckBob(DeltaTime, Speed2D, Y);

        }
//        else if ( !bShowMenu )
//        {
//            BobTime = 0;
//            WalkBob = WalkBob * (1 - FMin(1, 8 * deltatime));
//        }

        //CheckBob(DeltaTime, Y);

        // Update rotation.
        //SetRotation(ViewRotation);
        OldRotation = Rotation;
        UpdateRotation(DeltaTime, 1);

        if( bPressedJump && CannotJumpNow() )
        {
            bSaveJump = true;
            bPressedJump = false;
        }
        else
            bSaveJump = false;

        if( Role < ROLE_Authority ) // then save this move and replicate it
            ReplicateMove(DeltaTime, NewAccel, DODGE_None, OldRotation - Rotation);
        else
            ProcessMove(DeltaTime, NewAccel, DODGE_None, OldRotation - Rotation);
        bPressedJump = bSaveJump;
    }

    function ProcessMove( float DeltaTime, vector newAccel, eDodgeDir DodgeMove, rotator DeltaRot)
    {
        local vector OldAccel;

        OldAccel = Acceleration;
        Acceleration = NewAccel;
        bIsTurning = ( Abs(DeltaRot.Yaw/DeltaTime) > 5000 );

        if( bPressedJump )
            DoJump();

        if( Physics == PHYS_Walking )
        {
            if (!bIsCrouching)
            {
                if (bDuck != 0)
                {
                    bIsCrouching = true;
                    PlayDuck();
                }
            }
            else if (bDuck == 0)
            {
                OldAccel = vect(0,0,0);
                bIsCrouching = false;
                TweenToRunning(0.1);
            }

            if ( !bIsCrouching )
            {
                if ( (!bAnimTransition || (AnimFrame > 0)) && (GetAnimGroup(AnimSequence) != 'Landing') )
                {
                    if ( Acceleration != vect(0,0,0) )
                    {
                        if ( (GetAnimGroup(AnimSequence) == 'Waiting') || (GetAnimGroup(AnimSequence) == 'Gesture') || (GetAnimGroup(AnimSequence) == 'TakeHit') )
                        {
                            bAnimTransition = true;
                            TweenToRunning(0.1);
                        }
                    }
                    else if ( (Velocity.X * Velocity.X + Velocity.Y * Velocity.Y < 1000)
                        && (GetAnimGroup(AnimSequence) != 'Gesture') )
                    {
                        if ( GetAnimGroup(AnimSequence) == 'Waiting' )
                        {
                            if ( bIsTurning && (AnimFrame >= 0) )
                            {
                                bAnimTransition = true;
                                PlayTurning();
                            }
                        }
                        else if ( !bIsTurning )
                        {
                            bAnimTransition = true;
                            TweenToWaiting(0.2);
                        }
                    }
                }
            }
            else
            {
                if ( (OldAccel == vect(0,0,0)) && (Acceleration != vect(0,0,0)) )
                    PlayCrawling();
                else if ( !bIsTurning && (Acceleration == vect(0,0,0)) && (AnimFrame > 0.1) )
                    PlayDuck();
            }
        }
    }
}

// ============================================================================
//  Movement Netcode
// ============================================================================

function ReplicateMove
(
    float DeltaTime,
    vector NewAccel,
    eDodgeDir DodgeMove,
    rotator DeltaRot
)
{
    local SavedMove NewMove, OldMove, AlmostLastMove, LastMove;
    local byte ClientRoll;
    local float NetMoveDelta;

    MaxResponseTime = Default.MaxResponseTime * Level.TimeDilation;
    DeltaTime = FMin(DeltaTime, MaxResponseTime);

    // Replicate firing info
    RpNet_ReplicateFiring();

    // find the most recent move (LastMove), and the oldest (unacknowledged) important move (OldMove)
    // a SavedMove is interesting if it differs significantly from the last acknowledged move
    if( SavedMoves != None )
    {
        LastMove = SavedMoves;
        AlmostLastMove = LastMove;
        while( LastMove.NextMove != None )
        {
            // find first important unacknowledged move
            if( OldMove == None && LastMove.IsImportantMove(LastAckedAccel) )
            {
                OldMove = LastMove;
            }
            AlmostLastMove = LastMove;
            LastMove = LastMove.NextMove;
        }
    }

    // Get a SavedMove actor to store the movement in.
    NewMove = GetFreeMove();
    if( NewMove == None )
        return;
    NewMove.SetMoveFor(self, DeltaTime, NewAccel);

    // Simulate the movement locally.
    ProcessMove(NewMove.Delta, NewMove.Acceleration, DODGE_None, DeltaRot);

    // see if the two moves could be combined
    if( PendingMove != None && PendingMove.CanCombineWith(NewMove, self, MaxResponseTime) )
    {
        xMsg("ReplicateMove MERGE");

        SetLocation(PendingMove.GetStartLocation());
        Velocity = PendingMove.StartVelocity;
        if( PendingMove.StartBase != Base);
            SetBase(PendingMove.StartBase);
        NewMove.Delta += PendingMove.Delta;
        NewMove.SetInitialPosition(self);

        // remove pending move from move list
        if( LastMove == PendingMove )
        {
            if( SavedMoves == PendingMove )
            {
                SavedMoves.NextMove = FreeMoves;
                FreeMoves = SavedMoves;
                SavedMoves = None;
            }
            else
            {
                PendingMove.NextMove = FreeMoves;
                FreeMoves = PendingMove;
                if( AlmostLastMove != None )
                {
                    AlmostLastMove.NextMove = None;
                    LastMove = AlmostLastMove;
                }
            }
            FreeMoves.Clear();
        }
        PendingMove = None;
    }

    AutonomousPhysics(DeltaTime);
    NewMove.PostUpdate(self);

    if( SavedMoves == None )
        SavedMoves = NewMove;
    else
        LastMove.NextMove = NewMove;

    if( PendingMove == None )
    {
        // Decide whether to hold off on move
        NetMoveDelta = 0.011;
        if( (Level.TimeSeconds - ClientUpdateTime) * Level.TimeDilation * 0.91 < NetMoveDelta )
        {
            xMsg("ReplicateMove HOLD" #ClientUpdateTime);
            PendingMove = NewMove;
            return;
        }
    }

    ClientUpdateTime = Level.TimeSeconds;

    // Send to the server
    ClientRoll = (Rotation.Roll >> 8) & 255;
    RpNet_CallServerMove( NewMove,
                        Location,
                        ClientRoll,
                        ((Rotation.Yaw & 65535) << 16) + (Rotation.Pitch & 65535),
                        OldMove );

    PendingMove = None;
}

function RpNet_CallServerMove
(
    SavedMove NewMove,
    vector ClientLoc,
    byte ClientRoll,
    int View,
    SavedMove OldMove
)
{
    local vector BuildAccel;
    local byte OldAccelX, OldAccelY, OldAccelZ;

    // compress old move if it exists
    if( OldMove != None )
    {
        //xMsg("CallServerMove OLD");
        // old move important to replicate redundantly
        BuildAccel = 0.05 * OldMove.Acceleration + vect(0.5, 0.5, 0.5);
        OldAccelX = CompressAccel(BuildAccel.X);
        OldAccelY = CompressAccel(BuildAccel.Y);
        OldAccelZ = CompressAccel(BuildAccel.Z);
        RpNet_OldServerMove(OldMove.TimeStamp, OldAccelX, OldAccelY, OldAccelZ, OldMove.GetCompressedFlags());
    }

    if( PendingMove != None )
    {
        //xMsg("CallServerMove DUAL");
        RpNet_DualServerMove
        (
            PendingMove.TimeStamp,
            PendingMove.Acceleration * 10,
            PendingMove.GetCompressedFlags(),
            ((PendingMove.Rotation.Yaw & 65535) << 16) + (PendingMove.Rotation.Pitch & 65535),
            NewMove.TimeStamp,
            NewMove.Acceleration * 10,
            ClientLoc,
            NewMove.GetCompressedFlags(),
            ClientRoll,
            View
        );
    }
    else
    {
        //xMsg("CallServerMove");
        RpNet_ServerMove
        (
            NewMove.TimeStamp,
            NewMove.Acceleration * 10,
            ClientLoc,
            NewMove.GetCompressedFlags(),
            ClientRoll,
            View
        );
    }
}

function RpNet_OldServerMove
(
    float OldTimeStamp,
    byte OldAccelX,
    byte OldAccelY,
    byte OldAccelZ,
    byte OldMoveFlags
)
{
    local vector Accel;

    if( CurrentTimeStamp < OldTimeStamp - 0.001 )
    {
        // split out components of lost move (approx)
        Accel.X = OldAccelX;
        if( Accel.X > 127 )
            Accel.X = -1 * (Accel.X - 128);
        Accel.Y = OldAccelY;
        if( Accel.Y > 127 )
            Accel.Y = -1 * (Accel.Y - 128);
        Accel.Z = OldAccelZ;
        if( Accel.Z > 127 )
            Accel.Z = -1 * (Accel.Z - 128);
        Accel *= 20;

        xMsg("OldMove" #(OldTimeStamp @CurrentTimeStamp) #(Accel @Acceleration), True );
        OldTimeStamp = FMin(OldTimeStamp, CurrentTimeStamp + MaxResponseTime);
        RpNet_MoveAutonomous(OldTimeStamp - CurrentTimeStamp, OldMoveFlags, Accel, rot(0,0,0));
        CurrentTimeStamp = OldTimeStamp;
    }
    else
    {
        xMsg("OldMove FAIL" #(OldTimeStamp @CurrentTimeStamp) #(Accel @Acceleration), True );
    }
}

function RpNet_DualServerMove
(
    float TimeStamp0,
    vector InAccel0,
    byte PendingFlags,
    int View0,
    float TimeStamp,
    vector InAccel,
    vector ClientLoc,
    byte NewFlags,
    byte ClientRoll,
    int View
)
{
    RpNet_ServerMove(TimeStamp0,InAccel0,vect(1,2,3),PendingFlags,ClientRoll,View0);
    if( ClientLoc == vect(1,2,3) )
        ClientLoc = vect(1.1,2,3);
    RpNet_ServerMove(TimeStamp,InAccel,ClientLoc,NewFlags,ClientRoll,View);
}

function RpNet_ServerMove
(
    float TimeStamp,
    vector InAccel,
    vector ClientLoc,
    byte MoveFlags,
    byte ClientRoll,
    int View
)
{
    local float DeltaTime, clientErr;
    local rotator DeltaRot, Rot, ViewRot;
    local vector Accel, LocDiff;
    local int maxPitch, ViewPitch, ViewYaw;

    // If this move is outdated, discard it.
    if( CurrentTimeStamp >= TimeStamp )
        return;

    // View components
    ViewPitch   = (View & 65535);
    ViewYaw     = (View >> 16);

    // Make acceleration.
    Accel = InAccel * 0.1;

    // Save move parameters.
    DeltaTime = FMin(MaxResponseTime, TimeStamp - CurrentTimeStamp);

//    // speedhack detection
//    if( ServerTimeStamp > 0 )
//    {
//        // allow 1% error
//        TimeMargin += DeltaTime - 1.01 * (Level.TimeSeconds - ServerTimeStamp);
//        if( TimeMargin > MaxTimeMargin )
//        {
//            // player is too far ahead
//            xLog( "player is too far ahead" );
//            TimeMargin -= DeltaTime;
//            if( TimeMargin < 0.5 )
//                MaxTimeMargin = Default.MaxTimeMargin;
//            else
//                MaxTimeMargin = 0.5;
//            DeltaTime = 0;
//        }
//    }

    CurrentTimeStamp = TimeStamp;
    ServerTimeStamp = Level.TimeSeconds;

    // Update rotation
    ViewRotation.Pitch = ViewPitch;
    ViewRotation.Yaw = ViewYaw;
    ViewRotation.Roll = 0;
    //SetRotation(ViewRot);

    Rot.Roll = 256 * ClientRoll;
    Rot.Yaw = ViewYaw;
    if( Physics == PHYS_Swimming || Physics == PHYS_Flying )
        maxPitch = 2;
    else
        maxPitch = 1;
    if( (ViewPitch > maxPitch * RotationRate.Pitch) && (ViewPitch < 65536 - maxPitch * RotationRate.Pitch) )
    {
        if( ViewPitch < 32768 )
            Rot.Pitch = maxPitch * RotationRate.Pitch;
        else
            Rot.Pitch = 65536 - maxPitch * RotationRate.Pitch;
    }
    else
        Rot.Pitch = ViewPitch;

    // From what I gather, this should have been the case in single player as well since
    // pitch is zeroed out in UpdateRotation.  Oh well, now it doesn't look goofy in mp
    // MBCODE
    if( Level.NetMode != NM_Standalone )
        Rot.Pitch = 0;

    DeltaRot = (Rotation - Rot);
    SetRotation(Rot);

    // Perform actual movement
    if( Level.Pauser == "" && DeltaTime > 0 )
        RpNet_MoveAutonomous(DeltaTime, MoveFlags, Accel, DeltaRot);

    // DEUS_EX AMSD Do any ticking which needs to be done.
    MultiplayerTick(DeltaTime);

    // Accumulate movement error.
    if( ClientLoc == vect(1,2,3) )
        return; // first part of dual servermove
    else if( Level.TimeSeconds - LastUpdateTime < 180.0/Player.CurrentNetSpeed )
        return;

    LocDiff = Location - ClientLoc;
    ClientErr = VSize(LocDiff);

    // If client has accumulated a noticeable positional error, correct him.
    if( ClientErr > 3 )
    {
        PendingAdjustment.newPhysics = Physics;
        PendingAdjustment.NewLoc = Location;
        PendingAdjustment.NewVel = Velocity;
        PendingAdjustment.NewFloor = Floor;
        PendingAdjustment.TimeStamp = TimeStamp;
        PendingAdjustment.newState = GetStateName();
        PendingAdjustment.bAckGoodMove = 0;

        if( Mover(Base) != None )
            PendingAdjustment.NewLoc = Location - Base.Location;
        else
            PendingAdjustment.NewLoc = Location;

        LastUpdateTime = Level.TimeSeconds;

        xMsg("Client Error at "$TimeStamp$" is "$ClientErr$" with acceleration "$Accel$" LocDiff "$LocDiff$" Physics "$Physics);
    }
    else
    {
        // acknowledge receipt of this successful servermove()
        PendingAdjustment.TimeStamp = TimeStamp;
        PendingAdjustment.bAckGoodMove = 1;
    }

    //xlog("Server moved stamp "$TimeStamp$" location "$Pawn.Location$" Acceleration "$Pawn.Acceleration$" Velocity "$Pawn.Velocity);
}



function RpNet_SendClientAdjustment()
{
    //xLog("SendClientAdjustment");

    if( PendingAdjustment.bAckGoodMove == 1 )
    {
        RpNet_ClientAckGoodMove(PendingAdjustment.TimeStamp);
    }
    else
    {
        if( PendingAdjustment.NewVel == vect(0,0,0) )
        {
            if( GetStateName() == 'PlayerWalking' && Physics == PHYS_Walking )
            {
                RpNet_VeryShortClientAdjustPosition
                (
                    PendingAdjustment.TimeStamp,
                    PendingAdjustment.NewLoc.X,
                    PendingAdjustment.NewLoc.Y,
                    PendingAdjustment.NewLoc.Z,
                    PendingAdjustment.NewBase
                );
            }
            else
            {
                RpNet_ShortClientAdjustPosition
                (
                    PendingAdjustment.TimeStamp,
                    PendingAdjustment.newState,
                    PendingAdjustment.newPhysics,
                    PendingAdjustment.NewLoc.X,
                    PendingAdjustment.NewLoc.Y,
                    PendingAdjustment.NewLoc.Z,
                    PendingAdjustment.NewBase
                );
            }
        }
        else
        {
            RpNet_ClientAdjustPosition
            (
                PendingAdjustment.TimeStamp,
                PendingAdjustment.newState,
                PendingAdjustment.newPhysics,
                PendingAdjustment.NewLoc.X,
                PendingAdjustment.NewLoc.Y,
                PendingAdjustment.NewLoc.Z,
                PendingAdjustment.NewVel.X,
                PendingAdjustment.NewVel.Y,
                PendingAdjustment.NewVel.Z,
                PendingAdjustment.NewBase
            );
        }
    }

    PendingAdjustment.TimeStamp = 0;
    PendingAdjustment.bAckGoodMove = 0;
}

function RpNet_ClientAckGoodMove(float TimeStamp)
{
    //ClientMessage( "RpNet_ClientAckGoodMove" $TimeStamp );
    //UpdatePing(TimeStamp);
    CurrentTimeStamp = TimeStamp;
    RpNet_ClearAckedMoves();
}

function RpNet_ClearAckedMoves()
{
    local SavedMove CurrentMove;

    CurrentMove = SavedMoves;
    while( CurrentMove != None )
    {
        if( CurrentMove.TimeStamp <= CurrentTimeStamp )
        {
            if( CurrentMove.TimeStamp == CurrentTimeStamp )
                LastAckedAccel = CurrentMove.Acceleration;
            SavedMoves = CurrentMove.NextMove;
            CurrentMove.NextMove = FreeMoves;
            FreeMoves = CurrentMove;
            FreeMoves.Clear();
            CurrentMove = SavedMoves;
        }
        else
            break;
    }
}

function RpNet_VeryShortClientAdjustPosition
(
    float TimeStamp,
    float NewLocX,
    float NewLocY,
    float NewLocZ,
    Actor NewBase
)
{
    RpNet_ClientAdjustPosition(TimeStamp,'PlayerWalking',PHYS_Walking,NewLocX,NewLocY,NewLocZ,0,0,0,NewBase);
}

function RpNet_ShortClientAdjustPosition
(
    float TimeStamp,
    name newState,
    EPhysics newPhysics,
    float NewLocX,
    float NewLocY,
    float NewLocZ,
    Actor NewBase
)
{
    RpNet_ClientAdjustPosition(TimeStamp,newState,newPhysics,NewLocX,NewLocY,NewLocZ,0,0,0,NewBase);
}

function RpNet_ClientAdjustPosition
(
    float TimeStamp,
    name newState,
    EPhysics newPhysics,
    float NewLocX,
    float NewLocY,
    float NewLocZ,
    float NewVelX,
    float NewVelY,
    float NewVelZ,
    Actor NewBase
)
{
    local Decoration Carried;
    local vector OldLoc, NewLocation, NewVelocity;
    local SavedMove CurrentMove;

    if( CurrentTimeStamp > TimeStamp )
        return;
    CurrentTimeStamp = TimeStamp;

    if( SavedMoves != None )
    {
        //xLog( "Adjust" #(CurrentTimeStamp @TimeStamp) #SavedMoves.PrintChain() );
    }

    NewLocation.X = NewLocX;
    NewLocation.Y = NewLocY;
    NewLocation.Z = NewLocZ;
    NewVelocity.X = NewVelX;
    NewVelocity.Y = NewVelY;
    NewVelocity.Z = NewVelZ;


    // skip update if no error
    CurrentMove = SavedMoves;
    while( CurrentMove != None )
    {
        if( CurrentMove.TimeStamp <= CurrentTimeStamp )
        {
            SavedMoves = CurrentMove.NextMove;
            CurrentMove.NextMove = FreeMoves;
            FreeMoves = CurrentMove;
            if( CurrentMove.TimeStamp == CurrentTimeStamp )
            {
                LastAckedAccel = CurrentMove.Acceleration;
                FreeMoves.Clear();
                if( Mover(NewBase) != None && NewBase == CurrentMove.EndBase )
                {
                    if( GetStateName() == NewState
                    &&  IsInState('PlayerWalking')
                    &&  (Physics == PHYS_Walking || Physics == PHYS_Falling) )
                    {
                        if( VSize(CurrentMove.SavedRelativeLocation - NewLocation) < 3 )
                        {
                            xMsg( "Adjust SKIP MOVER" #(CurrentTimeStamp @TimeStamp) );
                            CurrentMove = None;
                            return;
                        }
                    }
                }
                else if( VSize(CurrentMove.SavedLocation - NewLocation) < 3
                &&  VSize(CurrentMove.SavedVelocity - NewVelocity) < 3
                &&  GetStateName() == NewState
                &&  IsInState('PlayerWalking')
                &&  (Physics == PHYS_Walking || Physics == PHYS_Falling) )
                {
                    xMsg( "Adjust SKIP WALK" #(CurrentTimeStamp @TimeStamp) );
                    CurrentMove = None;
                    return;
                }
                CurrentMove = None;
            }
            else
            {
                FreeMoves.Clear();
                CurrentMove = SavedMoves;
            }
        }
        else
            CurrentMove = None;
    }


    //ClientMessage("Adjust "$Role$" adjust "$self$" stamp "$TimeStamp$" location "$Location @"Physics"$Physics );

    // update location
    if( Mover(NewBase) != None )
        NewLocation += NewBase.Location;

    OldLoc = Location;
    bCanTeleport = false;
    SetLocation(NewLocation);
    if( !SetLocation(NewLocation)
    &&  CollisionHeight > CrouchHeight
    &&  !bIsCrouching
    &&  newPhysics == PHYS_Walking )
    {
        SetPhysics(newPhysics);

        if( !SetLocation(NewLocation + vect(0,0,1)*MaxStepHeight) )
        {
            xMsg("Adjust FORCE CROUCH" #(CurrentTimeStamp @TimeStamp));
            ForceCrouch();
            SetLocation(NewLocation);
        }
        else
        {
            xMsg("Adjust FORCE STEP" #(CurrentTimeStamp @TimeStamp));
            MoveSmooth(vect(0,0,-1)*MaxStepHeight);
        }
    }
    bCanTeleport = true;

    // update carried item
    Carried = CarriedDecoration;
    if( Carried != None )
    {
        CarriedDecoration = Carried;
        CarriedDecoration.SetLocation(NewLocation + CarriedDecoration.Location - OldLoc); // TODO: absolute location?
        CarriedDecoration.SetPhysics(PHYS_None);
        CarriedDecoration.SetBase(self);
    }

    // update physics
    SetPhysics(newPhysics);
    SetBase(NewBase);
    Velocity = NewVelocity;

    // update state
    if( !IsInState(newState) )
        GotoState(newState);

    bUpdatePosition = true;
}

function ClientUpdatePosition()
{
    local SavedMove CurrentMove;
    local int realbRun, realbDuck;
    local bool bRealJump;

    if( SavedMoves != None )
    {
        //xLog( "ClientUpdate" #(CurrentTimeStamp) #SavedMoves.PrintChain() );
    }

    bUpdatePosition = false;

    realbRun = bRun;
    realbDuck = bDuck;
    bRealJump = bPressedJump;
    bUpdating = true;

    RpNet_ClearAckedMoves();
    CurrentMove = SavedMoves;
    while( CurrentMove != None )
    {
        xMsg("Update SAVED" #CurrentTimeStamp #CurrentMove.TimeStamp);

        if( PendingMove == CurrentMove )
            PendingMove.SetInitialPosition(self);

        RpNet_MoveAutonomous(CurrentMove.Delta, CurrentMove.GetCompressedFlags(), CurrentMove.Acceleration, rot(0,0,0));

        CurrentMove.SavedLocation = Location;
        CurrentMove.SavedVelocity = Velocity;
        CurrentMove.EndBase = Base;
        if( Mover(CurrentMove.EndBase) != None )
            CurrentMove.SavedRelativeLocation = Location - CurrentMove.EndBase.Location;

        CurrentMove = CurrentMove.NextMove;
    }

    bUpdating = false;
    bDuck = realbDuck;
    bRun = realbRun;
    bPressedJump = bRealJump;
    //xlog("Client adjusted "$self$" stamp "$CurrentTimeStamp$" location "$Location$" dodge "$DodgeDir);
}

function RpNet_MoveAutonomous
(
    float DeltaTime,
    byte CompressedFlags,
    vector newAccel,
    rotator DeltaRot
)
{
    class'SavedMove'.static.SetFlags(CompressedFlags, self);

    HandleWalking();
    ProcessMove(DeltaTime, newAccel, DODGE_None, DeltaRot);
    AutonomousPhysics(DeltaTime);
    //log("Role "$Role$" moveauto time "$100 * DeltaTime$" ("$WorldInfo.TimeDilation$")");
}


// ============================================================================
//  Firing
// ============================================================================

function RpNet_ReplicateFiring()
{
    local byte FireInfo;

    if( bJustFired )        FireInfo += 3;
    else if( bFire != 0 )   FireInfo += 1;

    if( bJustAltFired )        FireInfo += 12;
    else if( bAltFire != 0 )   FireInfo += 4;

    if( FireInfo != OldFireInfo )
    {
        RpNet_ServerUpdateFiring( FireInfo );
        OldFireInfo = FireInfo;
    }

    if( Weapon != None )
        Weapon.bPointing = (bFire != 0 || bAltFire != 0);

    bJustFired = false;
    bJustAltFired = false;
}

function RpNet_ServerUpdateFiring( byte FireInfo )
{
    // handle firing
    if( (FireInfo & 1) != 0 )
    {
        if( (FireInfo & 2) != 0 && Weapon != None )
            Weapon.ForceFire();
        else if( bFire == 0 )
            Fire(0);
        bFire = 1;
    }
    else
        bFire = 0;

    // handle alt-firing
    if( (FireInfo & 4) != 0 )
    {
        if( (FireInfo & 8) != 0 && Weapon != None )
            Weapon.ForceAltFire();
        else if( bAltFire == 0 )
            AltFire(0);
        bAltFire = 1;
    }
    else
        bAltFire = 0;
}

// ============================================================================
//  Debug
// ============================================================================

simulated function string GetDebugString()
{
    return string(Physics);
}

simulated function xMsg( coerce string S, optional bool bQuiet )
{
    xLog(S);
    if( !bQuiet )
        ClientMessage(S, 'Event', True);
}

// ============================================================================
//  DefaultProperties
// ============================================================================
DefaultProperties
{
    MaxResponseTime=0.125
    CrouchHeight=16

}