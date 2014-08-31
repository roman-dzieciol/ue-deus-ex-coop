// ============================================================================
//  EntryGameInfo.uc ::
// ============================================================================
class EntryGameInfo extends GameInfo;

event PreLogin
(
	string Options,
	string Address,
	out string Error,
	out string FailCode
)
{
    if( Level.NetMode != NM_Standalone )
    {
        xLog( "PreLogin" #Options #Address #Error #FailCode );
    	Error = "ENTRY! PreLogin";
	}
	else
	   Super.PreLogin(Options,Address,Error,FailCode);
}

event playerpawn Login
(
	string Portal,
	string Options,
	out string Error,
	class<playerpawn> SpawnClass
)
{
    if( Level.NetMode != NM_Standalone )
    {
        xLog( "Login" #Portal #Options #Error #SpawnClass );
    	Error = "ENTRY! Login";
    	return None;
	}
	else
	   return Super.Login(Portal,Options,Error,SpawnClass);
}


DefaultProperties
{

}
