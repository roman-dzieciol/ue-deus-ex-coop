//=============================================================================
// SavedMove is used during network play to buffer recent client moves,
// for use when the server modifies the clients actual position, etc.
// This is a built-in Unreal class and it shouldn't be modified.
//=============================================================================
class SavedMove extends Info;

// also stores info in Acceleration attribute
var SavedMove NextMove;     // Next move in linked list.
var float TimeStamp;        // Time of this move.
var float Delta;            // Distance moved.

var bool    bRun;
var bool    bDuck;
var bool    bPressedJump;

var EPhysics SavedPhysics;
var vector StartLocation, StartRelativeLocation, StartVelocity, StartFloor;
var vector SavedLocation, SavedVelocity, SavedRelativeLocation;
var Actor StartBase, EndBase;

var float AccelDotThreshold;

// unused
var bool    bFire;
var bool    bAltFire;
var bool    bForceFire;
var bool    bForceAltFire;
var EDodgeDir DodgeMove;    // Dodge info.

final function string PrintMove()
{
    local string S;

    S = name #TimeStamp #Delta #(byte(bRun) $byte(bDuck) $byte(bPressedJump))
        #SavedPhysics #(GON(StartBase) @GON(EndBase));

    return S;
}

final function string PrintChain()
{
    local string S;
    local SavedMove CurrentMove;

    CurrentMove = self;
    while( CurrentMove != None )
    {
        S = S $"<" $CurrentMove.PrintMove() $">";
        CurrentMove = CurrentMove.NextMove;
    }

    return S;
}


final function Clear()
{
    Acceleration = vect(0,0,0);

    TimeStamp = 0;
    Delta = 0;

    bRun = false;
    bDuck = false;
    bPressedJump = false;

    SavedPhysics = PHYS_None;
    StartBase = None;
    EndBase = None;

    StartLocation = vect(0,0,0);
    StartRelativeLocation = vect(0,0,0);
    StartVelocity = vect(0,0,0);
    StartFloor = vect(0,0,0);

    SavedLocation = vect(0,0,0);
    SavedVelocity = vect(0,0,0);
    SavedRelativeLocation = vect(0,0,0);
}

final function PostUpdate(PlayerPawn P)
{
    if( P != None )
    {
        SavedLocation = P.Location;
        SavedVelocity = P.Velocity;
        EndBase = P.Base;
        if( Mover(EndBase) != None )
            SavedRelativeLocation = P.Location - EndBase.Location;
    }
    SetRotation(P.Rotation);
}

final function bool IsImportantMove(vector CompareAccel)
{
    local vector AccelNorm;

    if( bPressedJump )
        return true;

    AccelNorm = Normal(Acceleration);
    return ( (CompareAccel != AccelNorm) && ((CompareAccel Dot AccelNorm) < AccelDotThreshold) );
}

final function vector GetStartLocation()
{
    if( Mover(StartBase) != None )
        return StartBase.Location + StartRelativeLocation;

    return StartLocation;
}

final function SetInitialPosition(Pawn P)
{
    SavedPhysics = P.Physics;
    StartLocation = P.Location;
    StartVelocity = P.Velocity;
    StartBase = P.Base;
    StartFloor = P.Floor;
    if( Mover(StartBase) != None )
        StartRelativeLocation = P.Location - StartBase.Location;
}

final function bool CanCombineWith(SavedMove NewMove, Pawn InPawn, float MaxDelta)
{
    if( NewMove.Acceleration == vect(0,0,0) )
    {
        return Acceleration == vect(0,0,0)
        && StartVelocity == vect(0,0,0)
        && NewMove.StartVelocity == vect(0,0,0)
        && SavedPhysics == inPawn.Physics
        && !bPressedJump && !NewMove.bPressedJump
        && bRun == NewMove.bRun
        && bDuck == NewMove.bDuck;
    }
    else
    {
        return inPawn != None
        && NewMove.Delta + Delta < MaxDelta
        && SavedPhysics == inPawn.Physics
        && !bPressedJump && !NewMove.bPressedJump
        && bRun == NewMove.bRun
        && bDuck == NewMove.bDuck
        && Normal( Acceleration ) Dot Normal( NewMove.Acceleration ) > 0.99;
    }
}


final function SetMoveFor(PlayerPawn P, float DeltaTime, vector NewAccel)
{
    Delta = DeltaTime;

    if( VSize(NewAccel) > 3072 )
        NewAccel = 3072 * Normal(NewAccel);

    SetInitialPosition(P);

    Acceleration = NewAccel;
    bRun = (P.bRun > 0);
    bDuck = (P.bDuck > 0);
    bPressedJump = P.bPressedJump;
    TimeStamp = Level.TimeSeconds;
}

final function byte GetCompressedFlags()
{
    local byte Result;

    if( bRun )             Result += 1;
    if( bDuck )            Result += 2;
    if( bPressedJump )     Result += 4;

    return Result;
}

final static function SetFlags(byte Flags, PlayerPawn P)
{
    P.bRun = int((Flags & 1) != 0);
    P.bDuck = int((Flags & 2) != 0);
    P.bPressedJump = (Flags & 4) != 0;
}


defaultproperties
{
     bHidden=True
     AccelDotThreshold=0.9
}