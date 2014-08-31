//=============================================================================
// SmellNode.
//=============================================================================

// DEUS_EX STM - new class for Engine

class SmellNode extends Actor
	abstract;

// ----------------------------------------------------------------------
// Variables

// BOOGER MAR! For now...
var(Smell) byte MaxSmellRadius;
var(Smell) byte Strength;
var(Smell) bool Communicable;

var SmellNode   nextNode;
var SmellNode   prevNode;
var Actor       Owner;

defaultproperties
{
}
