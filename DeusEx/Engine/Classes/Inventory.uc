//=============================================================================
// The inventory class, the parent class of all objects which can be
// picked up and carried by actors.
//=============================================================================
class Inventory extends Actor
    abstract
    native
    nativereplication;

#exec Texture Import File=Textures\Inventry.pcx Name=S_Inventory Mips=Off Flags=2

//-----------------------------------------------------------------------------
// Information relevant to Active/Inactive state.

var() travel byte AutoSwitchPriority; // Autoswitch value, 0=never autoswitch.
var() byte        InventoryGroup;     // The weapon/inventory set, 1-9 (0=none).
var() bool        bActivatable;       // Whether item can be activated.
var() bool        bDisplayableInv;    // Item displayed in HUD.
var travel bool   bActive;            // Whether item is currently activated.
var   bool        bSleepTouch;        // Set when item is touched when leaving sleep state.
var   bool        bHeldItem;          // Set once an item has left pickup state.
var   bool    bTossedOut;             // true if weapon was tossed out (so players can't cheat w/ weaponstay)

//-----------------------------------------------------------------------------
// Ambient glow related info.

var(Display) bool bAmbientGlow;       // Whether to glow or not.

//-----------------------------------------------------------------------------
// Information relevant to Pickup state.

var() bool      bInstantRespawn;      // Can be tagged so this item respawns instantly.
var() bool      bRotatingPickup;      // Rotates when in pickup state.
var() localized string PickupMessage; // Human readable description when picked up.
var() localized string ItemName;      // Human readable name of item
var() localized string ItemArticle;   // Human readable article (e.g. "a", "an")
var() float     RespawnTime;          // Respawn after this time, 0 for instant.
var name        PlayerLastTouched;    // Player who last touched this item.

//-----------------------------------------------------------------------------
// Rendering information.

// Player view rendering info.
var() vector      PlayerViewOffset;   // Offset from view center.
var() mesh        PlayerViewMesh;     // Mesh to render.
var() float       PlayerViewScale;    // Mesh scale.
var() float       BobDamping;         // how much to damp view bob

// Pickup view rendering info.
var() mesh        PickupViewMesh;     // Mesh to render.
var() float       PickupViewScale;    // Mesh scale.

// 3rd person mesh.
var() mesh        ThirdPersonMesh;    // Mesh to render.
var() float       ThirdPersonScale;   // Mesh scale.

//-----------------------------------------------------------------------------
// Status bar info.

var() texture     StatusIcon;         // Icon used with ammo/charge/power count.

//-----------------------------------------------------------------------------
// Armor related info.

var() name        ProtectionType1;    // Protects against DamageType (None if non-armor).
var() name        ProtectionType2;    // Secondary protection type (None if non-armor).
var() travel int  Charge;             // Amount of armor or charge if not an armor (charge in time*10).
var() int         ArmorAbsorption;    // Percent of damage item absorbs 0-100.
var() bool        bIsAnArmor;         // Item will protect player.
var() int         AbsorptionPriority; // Which items absorb damage first (higher=first).
var() inventory   NextArmor;          // Temporary list created by Armors to prioritize damage absorption.

//-----------------------------------------------------------------------------
// AI related info.

var() float       MaxDesireability;   // Maximum desireability this item will ever have.
var   InventorySpot MyMarker;

//-----------------------------------------------------------------------------
// 3rd person muzzleflash

var bool bSteadyFlash3rd;
var bool bFirstFrame;
var(MuzzleFlash) bool bMuzzleFlashParticles;
var(MuzzleFlash) bool bToggleSteadyFlash;
var bool    bSteadyToggle;
var byte FlashCount, OldFlashCount;
var(MuzzleFlash) ERenderStyle MuzzleFlashStyle;
var(MuzzleFlash) mesh MuzzleFlashMesh;
var(MuzzleFlash) float MuzzleFlashScale;
var(MuzzleFlash) texture MuzzleFlashTexture;

//-----------------------------------------------------------------------------
// Sound assignments.

var() sound PickupSound, ActivateSound, DeActivateSound, RespawnSound;

// DEUS_EX STM - added
var sound LandSound;

//-----------------------------------------------------------------------------
// HUD graphics.

var() texture Icon;
var() localized String M_Activated;
var() localized String M_Selected;
var() localized String M_Deactivated;

//-----------------------------------------------------------------------------
// Messaging

var() class<LocalMessage> PickupMessageClass;
var() class<LocalMessage> ItemMessageClass;

//
// DEUS_EX AJY - additions (from old DeusExPickup)
//
var bool                    bCanUseObjectBelt; // Can this object be placed on the object belt?
var texture                 largeIcon;         // Larger-than-usual icon for the inventory window
var int                     largeIconWidth;    // Width of graphic in texture
var int                     largeIconHeight;   // Height of graphic in texture
var int                     invSlotsX;         // Number of horizontal inv. slots this item takes
var int                     invSlotsY;         // Number of vertical inv. slots this item takes
var travel int              invPosX;           // X position on the inventory window
var travel int              invPosY;           // Y position on the inventory window
var travel bool             bInObjectBelt;     // Is this object actually in the object belt?
var travel int              beltPos;           // Position on the object belt
var() localized String      description;       // Description
var localized String        beltDescription;   // Description used on the object belt

// Network replication.
replication
{
    // Things the server should send to the client.
    reliable if( Role==ROLE_Authority && bNetOwner )
        bIsAnArmor, Charge, bActivatable, bActive, PlayerViewOffset, PlayerViewMesh, PlayerViewScale, invPosX, invPosY, bInObjectBelt, beltPos;
    unreliable if( Role==ROLE_Authority )
        FlashCount, bSteadyFlash3rd, ThirdPersonMesh, ThirdPersonScale;
}

function PostBeginPlay()
{
    //xLog( "PostBeginPlay" );

   PickupMessageClass = None;
    if ( ItemName == "" )
        ItemName = GetItemName(string(Class));

    // DEUS_EX CNN - scale since network passes vector components as ints
    PlayerViewOffset = Default.PlayerViewOffset * 100;

    Super.PostBeginPlay();
}

// Draw first person view of inventory
simulated event RenderOverlays( canvas Canvas )
{
    if ( Owner == None )
        return;
    if ( (Level.NetMode == NM_Client) && (!Owner.IsA('PlayerPawn') || (PlayerPawn(Owner).Player == None)) )
        return;
    SetLocation( Owner.Location + CalcDrawOffset() );
    SetRotation( Pawn(Owner).ViewRotation );
    Canvas.DrawActor(self, false);
}

function String GetHumanName()
{
    return ItemArticle@ItemName;
}

// overridable function to ask the inventory object to draw its StatusIcon
simulated function DrawStatusIconAt( canvas Canvas, int X, int Y, optional float Scale )
{
    if( Scale == 0.0 )
        Scale = 1.0;
    Canvas.SetPos( X, Y );
    Canvas.DrawIcon( StatusIcon, Scale );
}

//=============================================================================
// AI inventory functions.

event float BotDesireability( pawn Bot )
{
    local Inventory AlreadyHas;
    local float desire;
    local bool bChecked;

    desire = MaxDesireability;

    if ( RespawnTime < 10 )
    {
        bChecked = true;
        AlreadyHas = Bot.FindInventoryType(class);
        if ( (AlreadyHas != None)
            && (AlreadyHas.Charge >= Charge) )
                return -1;
    }

    if( bIsAnArmor )
    {
        if ( !bChecked )
            AlreadyHas = Bot.FindInventoryType(class);
        if ( AlreadyHas != None )
            desire *= (1 - AlreadyHas.Charge * AlreadyHas.ArmorAbsorption * 0.00003);

        desire *= (Charge * 0.005);
        desire *= (ArmorAbsorption * 0.01);
        return desire;
    }
    else return desire;
}

function Weapon RecommendWeapon( out float rating, out int bUseAltMode )
{
    if ( inventory != None )
        return inventory.RecommendWeapon(rating, bUseAltMode);
    else
    {
        rating = -1;
        return None;
    }
}

//=============================================================================
// Inventory travelling across servers.

//
// Called after a travelling inventory item has been accepted into a level.
//
event TravelPreAccept()
{
    Super.TravelPreAccept();
    GiveTo( Pawn(Owner) );
    if( bActive )
        Activate();
}

//=============================================================================
// General inventory functions.

//
// Called by engine when destroyed.
//
function Destroyed()
{
    Super.Destroyed();

    if (MyMarker != None )
        MyMarker.markedItem = None;
    // Remove from owner's inventory.
    if( Pawn(Owner)!=None )
        Pawn(Owner).DeleteInventory( Self );
}

//
// Compute offset for drawing.
//
// DEUS_EX STM - took out "final"
//simulated final function vector CalcDrawOffset()
simulated function vector CalcDrawOffset()
{
    local vector DrawOffset, WeaponBob;
    local Pawn PawnOwner;

    PawnOwner = Pawn(Owner);
    DrawOffset = ((0.9/PawnOwner.FOVAngle * PlayerViewOffset) >> PawnOwner.ViewRotation);

    if ( (Level.NetMode == NM_DedicatedServer)
        || ((Level.NetMode == NM_ListenServer) && (Owner.RemoteRole == ROLE_AutonomousProxy)) )
        DrawOffset += (PawnOwner.BaseEyeHeight * vect(0,0,1));
    else
    {
        DrawOffset += (PawnOwner.EyeHeight * vect(0,0,1));
        WeaponBob = BobDamping * PawnOwner.WalkBob;
        WeaponBob.Z = (0.45 + 0.55 * BobDamping) * PawnOwner.WalkBob.Z;
        DrawOffset += WeaponBob;
    }
    return DrawOffset;
}

//
// Become a pickup.
//
function BecomePickup()
{
    //xLog( "BecomePickup" );

    if ( Physics != PHYS_Falling )
        RemoteRole    = ROLE_SimulatedProxy;
    Mesh          = PickupViewMesh;
    DrawScale     = PickupViewScale;
    bOnlyOwnerSee = false;
    bHidden       = false;
    bCarriedItem  = false;
    NetPriority   = 1.4;
    SetCollision( true, true, false );      // make things block actors as well - DEUS_EX CNN
}

//
// Become an inventory item.
//
function BecomeItem()
{
    //xLog( "BecomeItem" );

    RemoteRole    = ROLE_SimulatedProxy;
    Mesh          = PlayerViewMesh;
    DrawScale     = PlayerViewScale;
    bOnlyOwnerSee = true;
    bHidden       = true;
    bCarriedItem  = true;
    NetPriority   = 1.4;
    SetCollision( false, false, false );
    SetPhysics(PHYS_None);
//  SetTimer(0.0,False);    // DEUS_EX CNN - removed
    AmbientGlow = 0;
}

//
// Give this inventory item to a pawn.
//
function GiveTo( pawn Other )
{
    //xLog( "GiveTo" @Other );
    Instigator = Other;
    BecomeItem();
    Other.AddInventory( Self );
    GotoState('Idle2');
}

// Either give this inventory to player Other, or spawn a copy
// and give it to the player Other, setting up original to be respawned.
//
function inventory SpawnCopy( pawn Other )
{
    local inventory Copy;

    //xLog( "SpawnCopy" @Copy );

    // SW-- Spawn copy and die
        Copy = spawn(Class,Other,,,rot(0,0,0));
        Copy.Tag           = Tag;
        Copy.Event         = Event;
        Destroy();
    /*
    if( Level.Game.ShouldRespawn(self) )
    {
        Copy = spawn(Class,Other,,,rot(0,0,0));
        Copy.Tag           = Tag;
        Copy.Event         = Event;
        GotoState('Sleeping');
    }
    else
        Copy = self;*/
        // --SW

    Copy.RespawnTime = 0.0;
    Copy.bHeldItem = true;
    Copy.GiveTo( Other );
    return Copy;
}

function Inventory SpawnClone( Pawn Other )
{
    local inventory Copy;

    Copy = spawn(Class,Other,,,rot(0,0,0));
    Copy.Tag           = Tag;
    Copy.Event         = Event;

    Copy.RespawnTime = 0.0;
    Copy.bHeldItem = true;
    Copy.GiveTo( Other );
    return Copy;
}

//
// Set up respawn waiting if desired.
//
function SetRespawn()
{
    //xLog( "SetRespawn" );
    // SW-- Spawn copy and die
        Destroy();
    /*
    if( Level.Game.ShouldRespawn(self) )
        GotoState('Sleeping');
    else
        Destroy();*/
        // --SW
}


//
// Toggle Activation of selected Item.
//
function Activate()
{
    if( bActivatable )
    {
        if (Level.Game.LocalLog != None)
            Level.Game.LocalLog.LogItemActivate(Self, Pawn(Owner));
        if (Level.Game.WorldLog != None)
            Level.Game.WorldLog.LogItemActivate(Self, Pawn(Owner));

        if ( M_Activated != "" )
            Pawn(Owner).ClientMessage(ItemName$M_Activated);
        GoToState('Activated');
    }
}

//
// Function which lets existing items in a pawn's inventory
// prevent the pawn from picking something up. Return true to abort pickup
// or if item handles pickup, otherwise keep going through inventory list.
//
function bool HandlePickupQuery( inventory Item )
{
    if ( Item.Class == Class )
        return true;
    if ( Inventory == None )
        return false;

    return Inventory.HandlePickupQuery(Item);
}

//
// Select first activatable item.
//
function Inventory SelectNext()
{
    if ( bActivatable )
    {
        if ( M_Selected != "" )
            Pawn(Owner).ClientMessage(ItemName$M_Selected);
        return self;
    }
    if ( Inventory != None )
        return Inventory.SelectNext();
    else
        return None;
}

//
// Toss this item out.
//
function DropFrom(vector StartLocation)
{
    if ( !SetLocation(StartLocation) )
        return;
    RespawnTime = 0.0; //don't respawn
    SetPhysics(PHYS_Falling);
    RemoteRole = ROLE_DumbProxy;
    BecomePickup();
    NetPriority = 2.5;
    bCollideWorld = true;
    if ( Pawn(Owner) != None )
        Pawn(Owner).DeleteInventory(self);
    Inventory = None;
    GotoState('PickUp', 'Dropped');
}

function DropInventory()
{
}

// DEUS_EX STM - added
function PlayLandingSound()
{
    if (LandSound != None)
        PlaySound(LandSound);
}

//=============================================================================
// Capabilities: For feeding general info to bots.

// For future use.
function float InventoryCapsFloat( name Property, pawn Other, actor Test );
function string InventoryCapsString( name Property, pawn Other, actor Test );

//=============================================================================
// Firing/using.

// Fire functions which must be implemented in child classes.
function Fire( float Value );
function AltFire( float Value );
function Use( pawn User );

//=============================================================================
// Weapon functions.

//
// Find a weapon in inventory that has an Inventory Group matching F.
//

function Weapon WeaponChange( byte F )
{
    if( Inventory == None)
        return None;
    else
        return Inventory.WeaponChange( F );
}

//=============================================================================
// Armor functions.

//
// Scan the player's inventory looking for items that reduce damage
// to the player.  If Armor's protection type matches DamageType, then no damage is taken.
// Returns the reduced damage.
//
function int ReduceDamage( int Damage, name DamageType, vector HitLocation )
{
    local Inventory FirstArmor;
    local int ReducedAmount,ArmorDamage;

    if( Damage<0 )
        return 0;

    ReducedAmount = Damage;
    FirstArmor = PrioritizeArmor(Damage, DamageType, HitLocation);
    while( (FirstArmor != None) && (ReducedAmount > 0) )
    {
        ReducedAmount = FirstArmor.ArmorAbsorbDamage(ReducedAmount, DamageType, HitLocation);
        FirstArmor = FirstArmor.nextArmor;
    }
    return ReducedAmount;
}

//
// Return the best armor to use.
//
function inventory PrioritizeArmor( int Damage, name DamageType, vector HitLocation )
{
    local Inventory FirstArmor, InsertAfter;

    if ( Inventory != None )
        FirstArmor = Inventory.PrioritizeArmor(Damage, DamageType, HitLocation);
    else
        FirstArmor = None;

    if ( bIsAnArmor)
    {
        if ( FirstArmor == None )
        {
            nextArmor = None;
            return self;
        }

        // insert this armor into the prioritized armor list
        if ( FirstArmor.ArmorPriority(DamageType) < ArmorPriority(DamageType) )
        {
            nextArmor = FirstArmor;
            return self;
        }
        InsertAfter = FirstArmor;
        while ( (InsertAfter.nextArmor != None)
            && (InsertAfter.nextArmor.ArmorPriority(DamageType) > ArmorPriority(DamageType)) )
            InsertAfter = InsertAfter.nextArmor;

        nextArmor = InsertAfter.nextArmor;
        InsertAfter.nextArmor = self;
    }
    return FirstArmor;
}

//
// Absorb damage.
//
function int ArmorAbsorbDamage(int Damage, name DamageType, vector HitLocation)
{
    local int ArmorDamage;

    if ( DamageType != 'Drowned' )
        ArmorImpactEffect(HitLocation);
    if( (DamageType!='None') && ((ProtectionType1==DamageType) || (ProtectionType2==DamageType)) )
        return 0;

    if (DamageType=='Drowned') Return Damage;

    ArmorDamage = (Damage * ArmorAbsorption) / 100;
    if( ArmorDamage >= Charge )
    {
        ArmorDamage = Charge;
        Destroy();
    }
    else
        Charge -= ArmorDamage;
    return (Damage - ArmorDamage);
}

//
// Return armor value.
//
function int ArmorPriority(name DamageType)
{
    if ( DamageType == 'Drowned' )
        return 0;
    if( (DamageType!='None')
        && ((ProtectionType1==DamageType) || (ProtectionType2==DamageType)) )
        return 1000000;

    return AbsorptionPriority;
}

//
// This function is called by ArmorAbsorbDamage and displays a visual effect
// for an impact on an armor.
//
function ArmorImpactEffect(vector HitLocation){ }

//
// Used to inform inventory when owner jumps.
//
function OwnerJumped()
{
    if( Inventory != None )
        Inventory.OwnerJumped();
}

//
// Used to inform inventory when owner weapon changes.
//
function ChangedWeapon()
{
    if( Inventory != None )
        Inventory.ChangedWeapon();
}

// used to ask inventory if it needs to affect its owners display properties
function SetOwnerDisplay()
{
    if( Inventory != None )
        Inventory.SetOwnerDisplay();
}

//=============================================================================
// Pickup state: this inventory item is sitting on the ground.

auto state Pickup
{
    singular function ZoneChange( ZoneInfo NewZone )
    {
        local float splashsize;
        local actor splash;

        if( NewZone.bWaterZone && !Region.Zone.bWaterZone )
        {
            splashSize = 0.000025 * Mass * (250 - 0.5 * Velocity.Z);
            if ( NewZone.EntrySound != None )
                PlaySound(NewZone.EntrySound, SLOT_Interact, splashSize);
            if ( NewZone.EntryActor != None )
            {
                splash = Spawn(NewZone.EntryActor);
                if ( splash != None )
                    splash.DrawScale = 2 * splashSize;
            }
        }
    }

    // Validate touch, and if valid trigger event.
    function bool ValidTouch( actor Other )
    {
        local Actor A;

        if( Other.bIsPawn && Pawn(Other).bIsPlayer && (Pawn(Other).Health > 0) && Level.Game.PickupQuery(Pawn(Other), self) )
        {
            if( Event != '' )
                foreach AllActors( class 'Actor', A, Event )
                    A.Trigger( Other, Other.Instigator );
            return true;
        }
        return false;
    }

    // When touched by an actor.
    // Now, when frobbed by an actor - DEUS_EX CNN
    function Frob(Actor Other, Inventory frobWith)
//  function Touch( actor Other )
    {
        // If touched by a player pawn, let him pick this up.
        if( ValidTouch(Other) )
        {
            //xLog( "Frob ValidTouch" );

            if (Level.Game.LocalLog != None)
                Level.Game.LocalLog.LogPickup(Self, Pawn(Other));
            if (Level.Game.WorldLog != None)
                Level.Game.WorldLog.LogPickup(Self, Pawn(Other));
            SpawnCopy(Pawn(Other));
            if ( PickupMessageClass == None )
                // DEUS_EX CNN - use the itemArticle and itemName
//              Pawn(Other).ClientMessage(PickupMessage, 'Pickup');
                Pawn(Other).ClientMessage(PickupMessage @ itemArticle @ itemName, 'Pickup');
            else
                Pawn(Other).ReceiveLocalizedMessage( PickupMessageClass, 0, None, None, Self.Class );
            PlaySound (PickupSound);
            if ( Level.Game.Difficulty > 1 )
                Other.MakeNoise(0.1 * Level.Game.Difficulty);
            if ( Pawn(Other).MoveTarget == self )
                Pawn(Other).MoveTimer = -1.0;
        }
        else if ( bTossedOut && (Other.Class == Class) && Inventory(Other).bTossedOut )
        {
            //xLog( "Frob Destroy" );
            Destroy();
        }

    }

    // Landed on ground.
    function Landed(Vector HitNormal)
    {
        local rotator newRot;
        newRot = Rotation;
        newRot.pitch = 0;
        SetRotation(newRot);
        PlayLandingSound();  // DEUS_EX STM - added
//      SetTimer(2.0, false);   // DEUS_EX CNN - removed
    }

    // Make sure no pawn already touching (while touch was disabled in sleep).
    function CheckTouching()
    {
        local int i;

        bSleepTouch = false;
        for ( i=0; i<4; i++ )
            if ( (Touching[i] != None) && Touching[i].IsA('Pawn') )
                Touch(Touching[i]);
    }
    // DEUS_EX CNN - removed this crap - we want items to stick around forever
/*
    function Timer()
    {
        if ( RemoteRole != ROLE_SimulatedProxy )
        {
            NetPriority = 1.4;
            RemoteRole = ROLE_SimulatedProxy;

            // DEUS_EX STM - ick
//          if ( bHeldItem )
//              SetTimer(40.0, false);

            if ( bHeldItem )
            {
                if ( bTossedOut )
                    SetTimer(15.0, false);
                else
                    SetTimer(40.0, false);
            }
            return;
        }

        // DEUS_EX CNN
//      if ( bHeldItem )
//          Destroy();

        if ( bHeldItem )
        {
            if (  (FRand() < 0.1) || !PlayerCanSeeMe() )
                Destroy();
            else
                SetTimer(3.0, true);
        }
    }
*/
    function BeginState()
    {
        //xLog( "BeginState" );
        BecomePickup();
        bCollideWorld = true;
        // DEUS_EX CNN - removed
//      if ( bHeldItem )
//          SetTimer(30, false);
//      else if ( Level.bStartup )
        if ( Level.bStartup )
            bAlwaysRelevant = true;

        // Don't destroy it!  What were they thinking? - DEUS_EX CNN
//      if ( bHeldItem )
//          SetTimer(45, false);
    }

    function EndState()
    {
        //xLog( "EndState" );
        bCollideWorld = false;
        bSleepTouch = false;
    }

Begin:
    //xLog( "Begin" );
    BecomePickup();
    if ( bRotatingPickup && (Physics != PHYS_Falling) )
        SetPhysics(PHYS_Rotating);

Dropped:
    if( bAmbientGlow )
        AmbientGlow=255;
    if( bSleepTouch )
        CheckTouching();
}

//=============================================================================
// Active state: this inventory item is armed and ready to rock!

state Activated
{
    function BeginState()
    {
        bActive = true;
        if ( Pawn(Owner).bIsPlayer && (ProtectionType1 != '') )
            Pawn(Owner).ReducedDamageType = ProtectionType1;
    }

    function EndState()
    {
        bActive = false;
        if ( (Pawn(Owner) != None)
            && Pawn(Owner).bIsPlayer && (ProtectionType1 != '') )
            Pawn(Owner).ReducedDamageType = '';
    }

    function Activate()
    {
        if ( (Pawn(Owner) != None) && (M_Deactivated != "") )
            Pawn(Owner).ClientMessage(ItemName$M_Deactivated);
        GoToState('DeActivated');
    }
}

//=============================================================================
// Sleeping state: Sitting hidden waiting to respawn.

State Sleeping
{
    ignores Touch;

    function BeginState()
    {
        BecomePickup();
        bHidden = true;
    }
    function EndState()
    {
        local int i;

        bSleepTouch = false;
        for ( i=0; i<4; i++ )
            if ( (Touching[i] != None) && Touching[i].IsA('Pawn') )
                bSleepTouch = true;
    }
Begin:
    Sleep( ReSpawnTime );
    PlaySound( RespawnSound );
    Sleep( Level.Game.PlaySpawnEffect(self) );
    GoToState( 'Pickup' );
}

function ActivateTranslator(bool bHint)
{
    if( Inventory!=None )
        Inventory.ActivateTranslator( bHint );
}

//
// Null state.
//
State Idle2
{
}

// ----------------------------------------------------------------------
// UpdateInfo(()
//
// Updates the InformationWindow when an item is selected in the
// inventory screen
//
// DEUS_EX AJY
// ----------------------------------------------------------------------

function bool UpdateInfo(Object winObject)
{
    return False;
}

// ----------------------------------------------------------------------
// TestMPBeltSpot()
// Returns true if the suggested belt location is ok for the object in mp.
// ----------------------------------------------------------------------

simulated function bool TestMPBeltSpot(int BeltSpot)
{
   return ((BeltSpot <= 3) && (BeltSpot > 0));
}



defaultproperties
{
     bDisplayableInv=True
     bRotatingPickup=True
     PickupMessage="Snagged an item"
     ItemArticle="a"
     PlayerViewScale=1.000000
     BobDamping=0.960000
     PickupViewScale=1.000000
     ThirdPersonScale=1.000000
     MaxDesireability=0.005000
     bFirstFrame=True
     bToggleSteadyFlash=True
     M_Activated=" activated"
     M_Selected=" selected"
     M_Deactivated=" deactivated"
     bCanUseObjectBelt=True
     invSlotsX=1
     invSlotsY=1
     invPosX=-1
     invPosY=-1
     beltPos=-1
     Description="Made by the ABC Widget ACME Corporation"
     bIsItemGoal=True
     bTravel=True
     Physics=PHYS_Falling
     RemoteRole=ROLE_SimulatedProxy
     DrawType=DT_Mesh
     Texture=Texture'Engine.S_Inventory'
     CollisionRadius=30.000000
     CollisionHeight=30.000000
     bCollideActors=True
     bBlockActors=True
     bFixedRotationDir=True
     NetPriority=1.400000
}
