// ============================================================================
//  RolePlayer.uc ::
// ============================================================================
class RolePlayer extends Human;



replication
{
    reliable if ( Role < ROLE_Authority )
        RolePlay;

    reliable if ( Role == ROLE_Authority )
        ImpersonateClient;

    reliable if ( Role < ROLE_Authority )
        ImpersonatePawn;
}

exec function RolePlay()
{
    xLog( "RolePlay" );
}



simulated event PostBeginPlay()
{
    // Use unique alliance name
    SetPropertyText("Alliance",GetPropertyText("name"));

    Super.PostBeginPlay();
}


function bool CanStartConversation()
{
    return False;
}

function Conversation GetActiveDataLink(String datalinkName)
{
    return None;
}

function bool PickupNanoKey(NanoKey newKey)
{
    return false;
}

function bool HandleItemPickup(Actor FrobTarget, optional bool bSearchOnly)
{
    if( NanoKey(FrobTarget) != None
    ||  NanoKeyRing(FrobTarget) != None )
        return false;

    return Super.HandleItemPickup(FrobTarget,bSearchOnly);
}




// ============================================================================
//  STATE InvalidState
// ============================================================================
auto state InvalidState
{
    event PlayerTick( float DeltaTime )
    {
        log(self$" invalid state");
        if ( bUpdatePosition )
            ClientUpdatePosition();

        PlayerMove(DeltaTime);
    }

    function PlayerMove( float DeltaTime )
    {
        if ( Role < ROLE_Authority ) // then save this move and replicate it
            ReplicateMove(DeltaTime, vect(0,0,0), Dodge_None, rot(0,0,0));
    }

    function ClientReStart()
    {
        //log("client restart");
        Velocity = vect(0,0,0);
        Acceleration = vect(0,0,0);
        BaseEyeHeight = Default.BaseEyeHeight;
        EyeHeight = BaseEyeHeight;

        GotoState('RpWaiting');
    }
}

simulated function ImpersonatePawn( ScriptedPawn P )
{
    local int i;
    local Inventory Inv;

    // - Replicated -----------------------------------------------------------

    HealthHead = P.HealthHead;
    HealthTorso = P.HealthTorso;
    HealthLegLeft = P.HealthLegLeft;
    HealthLegRight = P.HealthLegRight;
    HealthArmLeft = P.HealthArmLeft;
    HealthArmRight = P.HealthArmRight;

    bHidden = False;
    DrawType = P.DrawType;
    Mesh = P.Mesh;

    //RpSetCollisionSize(P.default.CollisionRadius, P.default.CollisionHeight);
    bCollideWorld = P.bCollideWorld;
    bProjTarget = P.bProjTarget;
    SetCollision(P.bCollideActors, P.bBlockActors, P.bBlockPlayers );

    for( i=0; i!=ArrayCount(MultiSkins); ++i )
    {
        MultiSkins[i] = P.MultiSkins[i];
    }

    bDetectable = P.bDetectable;
    Visibility = P.Visibility;
    //Alliance = P.Alliance;
    //AttitudeToPlayer = P.AttitudeToPlayer;

    // - Not Replicated -------------------------------------------------------


    for( Inv=P.Inventory; Inv!=None; Inv=Inv.Inventory )
    {
        Inv.SpawnClone(self);
    }

    ImpersonateClass(P.Class);
    ImpersonateClient(P.Class);
}

simulated function ImpersonateClient( Class<ScriptedPawn> P )
{
    ImpersonateClass(P);
}

simulated function ImpersonateClass( Class<ScriptedPawn> P )
{
    //RpSetCollisionSize(P.default.CollisionRadius, P.default.CollisionHeight);
    BaseEyeHeight = P.default.BaseEyeHeight;
    CarcassType = P.default.CarcassType;
    GotoState('PlayerWalking');
}






// ============================================================================
//  STATE RpWaiting
// ============================================================================
state RpWaiting
{
ignores SeePlayer, HearNoise, Bump, Died, ZoneChange, FootZoneChange;

    exec function RolePlay()
    {
        local ScriptedPawn P;
        local vector HL;

        Global.RolePlay();


        if( DeusExRootWindow(rootWindow).hud.augDisplay != None )
        {
            P = ScriptedPawn(DeusExRootWindow(rootWindow).hud.augDisplay.TraceLOS(8000,HL));
        }

        if( P == None )
        {
            ClientMessage("No target!",,True);
            return;
        }

        if( Animal(P) != None
        ||  Robot(P) != None
//        ||  P.bImportant
        || !P.bIsHuman
        ||  P.AttitudeToPlayer < ATTITUDE_Ignore
        ){
            ClientMessage("Impossible!",,True);
            return;
        }

        ImpersonatePawn(P);



//        foreach VisibleCollidingActors( class'ScriptedPawn', P, 256, Location, true )
//        {
//            if( P != None )
//            {
//                if( Animal(P) != None
//                ||  Robot(P) != None
//                ||  P.bImportant
//                ||  P.AttitudeToPlayer < ATTITUDE_Ignore
//                ){
//                    ClientMessage("You can't possess this pawn!",,True);
//                    return;
//                }
//
//                ImpersonatePawn(P);
//
//                return;
//            }
//        }

    }

    // ------------------------------------------------------------------------
    //  Lifespan
    // ------------------------------------------------------------------------

    event BeginState()
    {
        xLog("BeginState");
        PlayerReplicationInfo.bIsSpectator = true;
        PlayerReplicationInfo.bWaitingPlayer = true;
        //bShowScores = true;
        Mesh = None;
        SetCollision(false,false,false);
        EyeHeight = BaseEyeHeight;
        SetPhysics(PHYS_Flying);
    }

    event EndState()
    {
        xLog("EndState");
        PlayerReplicationInfo.bIsSpectator = false;
        PlayerReplicationInfo.bWaitingPlayer = false;
        //Mesh = Default.Mesh;
        SetCollision(true,true,true);
    }

    event PlayerTick( float DeltaTime )
    {
        if( bUpdatePosition )
            ClientUpdatePosition();

        PlayerMove(DeltaTime);
    }

    // ------------------------------------------------------------------------
    //  Movement
    // ------------------------------------------------------------------------

//    function ProcessMove(float DeltaTime, vector NewAccel, eDodgeDir DodgeMove, rotator DeltaRot)
//    {
//        Acceleration = Normal(NewAccel);
//        Velocity = Normal(NewAccel) * 300;
//        AutonomousPhysics(DeltaTime);
//    }

    function StartWalk()
    {
        ClientReStart();
    }

    function ProcessMove(float DeltaTime, vector NewAccel, eDodgeDir DodgeMove, rotator DeltaRot)
    {
        Acceleration = NewAccel;
        MoveSmooth(Acceleration * DeltaTime);
    }

    function PlayerMove(float DeltaTime)
    {
        local rotator newRotation;
        local vector X,Y,Z;

        GetAxes(ViewRotation,X,Y,Z);

        aForward *= 0.1;
        aStrafe  *= 0.1;
        aLookup  *= 0.24;
        aTurn    *= 0.24;
        aUp      *= 0.1;

        Acceleration = aForward*X + aStrafe*Y + aUp*vect(0,0,1);

        UpdateRotation(DeltaTime, 1);

        if ( Role < ROLE_Authority ) // then save this move and replicate it
            ReplicateMove(DeltaTime, Acceleration, DODGE_None, rot(0,0,0));
        else
            ProcessMove(DeltaTime, Acceleration, DODGE_None, rot(0,0,0));
    }

    event FootZoneChange(ZoneInfo newFootZone)
    {
    }

    event HeadZoneChange(ZoneInfo newHeadZone)
    {
    }

    // ------------------------------------------------------------------------
    //  Inventory
    // ------------------------------------------------------------------------

    exec function ThrowWeapon()
    {
    }

    exec function SwitchWeapon (byte F )
    {
    }

    exec function NextItem()
    {
    }

    exec function PrevItem()
    {
    }

    exec function Fire( optional float F )
    {
        if ( Role == ROLE_Authority )
        {
            RolePlay();
//            bBehindView = true;
//            ViewPlayerNum(-1);
//            if ( ViewTarget == None )
//                bBehindView = false;
        }
    }

    // The player wants to alternate-fire.
    exec function AltFire( optional float F )
    {
//        bBehindView = false;
//        Viewtarget = None;
//        ClientMessage(ViewingFrom@OwnCamera, 'Event', true);
    }


    // ------------------------------------------------------------------------
    //  Controller
    // ------------------------------------------------------------------------
    function ClientReStart()
    {
        //log("client restart");
        Velocity = vect(0,0,0);
        Acceleration = vect(0,0,0);
        BaseEyeHeight = Default.BaseEyeHeight;
        EyeHeight = BaseEyeHeight;
    }

    function PlayerTimeOut()
    {
        if (Health > 0)
            Died(None, 'dropped', Location);
    }


    // ------------------------------------------------------------------------
    //  Messages
    // ------------------------------------------------------------------------

    function SendVoiceMessage(PlayerReplicationInfo Sender, PlayerReplicationInfo Recipient, name messagetype, byte messageID, name broadcasttype)
    {
    }

    exec function Taunt( name Sequence )
    {
    }

    exec function CallForHelp()
    {
    }

    // Send a message to all players.
    exec function Say( string S )
    {
        if ( Len(S) > 63 )
            S = Left(S,63);
        if ( !Level.Game.bMuteSpectators )
            BroadcastMessage( PlayerReplicationInfo.PlayerName$":"$S, true );
    }

    // ------------------------------------------------------------------------
    //  Teams
    // ------------------------------------------------------------------------
    function ChangeTeam( int N )
    {
    }


    // ------------------------------------------------------------------------
    //  Execs
    // ------------------------------------------------------------------------

    exec function Walk()
    {
    }

    exec function Fly()
    {
    }

    exec function Suicide()
    {
    }

    exec function Grab()
    {
    }

    exec function BehindView( Bool B )
    {
        bBehindView = B;
        if ( ViewTarget == None )
            bBehindView = false;
    }

    exec function RestartLevel()
    {
    }

    // ------------------------------------------------------------------------
    //  Damage
    // ------------------------------------------------------------------------

    function TakeDamage( int Damage, Pawn instigatedBy, Vector hitlocation, Vector momentum, name damageType)
    {
    }
}




// ============================================================================
//  Debug
// ============================================================================


// ============================================================================
//  DefaultProperties
// ============================================================================
DefaultProperties
{

     PlayerReStartState=RpWaiting

    bBeltIsMPInventory=True
    Alliance=RolePlayer

    bDetectable=False

     CollisionRadius=20.0
     CollisionHeight=47.0

    AirSpeed=400.000000
    Visibility=0
    AttitudeToPlayer=ATTITUDE_Friendly
    MenuName="RolePlayer"
    DrawType=DT_None
    bHidden=True
    bCollideActors=False
    bCollideWorld=False
    bBlockActors=False
    bBlockPlayers=False
    bProjTarget=False
}
