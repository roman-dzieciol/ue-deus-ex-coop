//-----------------------------------------------------------
//
//-----------------------------------------------------------
class MenuMainRPC extends MenuUIMenuWindow;


var string RPCBuild;


// ----------------------------------------------------------------------
// InitWindow()
//
// Initialize the Window
// ----------------------------------------------------------------------

event InitWindow()
{
    Super.InitWindow();

    UpdateButtonStatus();
    ShowVersionInfo();
}

// ----------------------------------------------------------------------
// UpdateButtonStatus()
// ----------------------------------------------------------------------

function UpdateButtonStatus()
{
}

// ----------------------------------------------------------------------
// ShowVersionInfo()
// ----------------------------------------------------------------------

function ShowVersionInfo()
{
    local TextWindow version;
    local string S;
    local int i;

    S = player.GetDeusExVersion();
    i = instr(S," v");
    if( i != -1 )
        S = Mid(S,i+1);
    S = S @RPCBuild;

    version = TextWindow(NewChild(Class'TextWindow'));
    version.SetTextMargins(0, 0);
    version.SetWindowAlignments(HALIGN_Right, VALIGN_Bottom);
    version.SetTextColorRGB(255, 255, 255);
    version.SetTextAlignments(HALIGN_Right, VALIGN_Bottom);
    version.SetText(S);
}

function ProcessCustomMenuButton(string key)
{
    if( key == "Join" )
    {
        messageBoxMode = MB_JoinGameWarning;
        root.MessageBox("Help", "To join game, type 'open ip' in console, where ip is the server ip address.", 1, False, Self);
    }
    else if( key == "Help" )
    {
        messageBoxMode = MB_JoinGameWarning;
        root.MessageBox("Help", "To possess NPC while spectating, target him with crosshair and press fire. Visit our forums at www.dxcoop.info for further help.", 1, False, Self);
    }
}


event bool BoxOptionSelected(Window button, int buttonNumber)
{
    // Destroy the msgbox!
    root.PopWindow();

    switch(messageBoxMode)
    {
        case MB_Exit:
            if ( buttonNumber == 0 )
            {
                /* TODO: This is what Unreal Does,
                player.SaveConfig();
                if ( Level.Game != None )
                    Level.Game.SaveConfig();
                */

                root.ExitGame();
            }
            break;

        case MB_AskToTrain:
            if (buttonNumber == 0)
                player.StartTrainingMission();
            else
                root.InvokeMenuScreen(Class'MenuSelectDifficulty');
            break;

        case MB_Training:
            if (buttonNumber == 0)
                player.StartTrainingMission();
            break;

        case MB_Intro:
            if (buttonNumber == 0)
                player.ShowIntro();
            break;

        case MB_JoinGameWarning:
            break;
    }

    return true;
}

// ----------------------------------------------------------------------
// ----------------------------------------------------------------------

defaultproperties
{
    RPCBuild="RPC v0"
    ButtonNames(0)="Create Game"
    ButtonNames(1)="Join Game"
    ButtonNames(2)="Settings"
    ButtonNames(3)="Help"
    ButtonNames(4)="Exit"
    buttonDefaults(0)=(Y=13,Action=MA_NewGame)
    buttonDefaults(1)=(Y=49,Action=MA_Custom,Key="Join")
    buttonDefaults(2)=(Y=85,Invoke=Class'DeusEx.MenuSettings')
    buttonDefaults(3)=(Y=121,Action=MA_Custom,Key="Help")
    buttonDefaults(4)=(Y=179,Action=MA_Quit)
    buttonXPos=7
    buttonWidth=245
    Title="DEUS EX: RolePlay Coop"
    ClientWidth=258
    ClientHeight=221
    clientTextures(0)=Texture'DeusExUI.UserInterface.MenuDifficultyBackground_1'
    clientTextures(1)=Texture'DeusExUI.UserInterface.MenuDifficultyBackground_2'
    textureRows=1
    textureCols=2
}
