//=============================================================================
//\author Nathan Kinnick, Felipe Robledo & Esteban Maldonado
//
//Copyright (C) 2014 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//=============================================================================
var g_InformDestroyEvent;
var g_ShouldCloseEvent;

// Esteban's Toxicity functions
function goChangeLevelbyID(level_id) {
  Skye.DispatchMsg("ChangeUI HUD");
	Skye.DispatchMsg("ChangeLevel Level" + level_id);
	Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
	Skye.DispatchMsg("Action unPause");
}

function goPauseMenu() {
	Skye.DispatchMsg("Action Pause");
	//Skye.DispatchMsg("Audio PauseAll");
	Skye.DispatchMsg("Audio PauseMusicAll");
	Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
	Skye.DispatchMsg("ChangeUI PauseMenu");
	//Skye.DispatchMsg("Action Pause");
}

function goResume() {
	Skye.DispatchMsg("Action unPause");
	//Skye.DispatchMsg("Audio ResumeAll");
	Skye.DispatchMsg("Audio ResumeMusicAll");

  	Skye.DispatchMsg("ChangeUI HUD");
	//Skye.DispatchMsg("Action unPause");

}

function goLevelSelectMM()
{
	Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
	Skye.DispatchMsg("ChangeUI LevelSelectMainMenu");
}

// End Esteban's Toxicity functions

function goMainMenu() {
  //var elem = document.getElementById("confirmRestart");
  //if (typeof(elem) != 'undefined' && elem != null)
  //{
  //  elem.dispatchEvent(g_ShouldCloseEvent);
  //}
  //Skye.DispatchMsg("Action MainMenu");
  //Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
  Skye.DispatchMsg("ChangeUI TitleScreen");
}

function goHowToPlay() {
  Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
  Skye.DispatchMsg("ChangeUI HowToPlay");
}

function goLevelSelect() {
  Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
  Skye.DispatchMsg("ChangeUI LevelSelect");
}

function goOptions() {
  Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
  Skye.DispatchMsg("ChangeUI OptionsMenu");
}

function goCredits() {
  Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
  Skye.DispatchMsg("ChangeUI Credits");
}

function goQuitGame() {
  Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
  Skye.DispatchMsg("ChangeUI QuitGame");
}

function goRestart() {
  var elem = document.getElementById("confirmRestart");
  if (typeof(elem) != 'undefined' && elem != null)
  {
	elem.dispatchEvent(g_ShouldCloseEvent);
  }
  Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
  Skye.DispatchMsg("Action Restart");
}

function goQuit() {
  var elem = document.getElementById("confirmQuit");
  if (typeof(elem) != 'undefined' && elem != null)
  {
    elem.dispatchEvent(g_ShouldCloseEvent);
  }
  Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
  Skye.DispatchMsg("Action Quit");
}

function goBack() {
	Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
    Skye.DispatchMsg("Action Back");
}

function goNewGame() {
	Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
    Skye.DispatchMsg("Action NewGame");
}

function goContinue() {
	Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
    Skye.DispatchMsg("Action Continue");
}

function playerIsIntercepting() {
	Skye.DispatchMsg("Action Intervene");
}

function goPlayerDeckSelection() {
  Skye.DispatchMsg("DeckSelection Back");
}

function playClick() {
    Skye.DispatchMsg("PlayClick");
}


//=============================================================================

function InformSaveFileExists() {
	var quitGameElem = document.getElementById("NewGame");
	var confirmationElem = document.getElementById("NewGameConfirmation");
	
	confirmationElem.onclick = function() {
								Skye.DispatchMsg("NewGame");
	                            quitGameElem.dispatchEvent(g_InformDestroyEvent);
	}
	
	quitGameElem.dispatchEvent(g_InformDestroyEvent);
}
