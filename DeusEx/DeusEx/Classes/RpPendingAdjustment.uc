// ============================================================================
//  RpPendingAdjustment.uc ::
// ============================================================================
class RpPendingAdjustment extends Info;

var float TimeStamp;
var name newState;
var EPhysics newPhysics;
var vector NewLoc;
var vector NewVel;
var actor NewBase;
var vector NewFloor;
var byte bAckGoodMove;

event Tick( float DeltaSeconds )
{
    if( TimeStamp > 0
    &&  RpPlayerPawn(Owner).PendingAdjustment == self
    &&  RpPlayerPawn(Owner).RemoteRole == ROLE_AutonomousProxy
    &&  NetConnection(RpPlayerPawn(Owner).Player) != None )
    {
        RpPlayerPawn(Owner).RpNet_SendClientAdjustment();
    }
}

DefaultProperties
{
    bAlwaysTick=True
    RemoteRole=ROLE_None
}