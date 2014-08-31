//=============================================================================
// AmmoShuriken.
//=============================================================================
class AmmoShuriken extends DeusExAmmo;

simulated function PreBeginPlay()
{
    Super.PreBeginPlay();

    // SW-- SP AmmoAmount
    /*
    // If this is a netgame, then override defaults
    if ( Level.NetMode != NM_StandAlone )
      AmmoAmount = 7;*/
      // --SW

}

defaultproperties
{
     AmmoAmount=5
     MaxAmmo=25
     PickupViewMesh=LodMesh'DeusExItems.TestBox'
     Mesh=LodMesh'DeusExItems.TestBox'
     CollisionRadius=22.500000
     CollisionHeight=16.000000
     bCollideActors=True
}
