//=============================================================================
//\author Nathan Kinnick, Felipe Robledo
//
//Copyright (C) 2014 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//=============================================================================
var g_InformDestroyEvent;
var g_ShouldCloseEvent;

function goMainMenu() {
  var elem = document.getElementById("confirmRestart");
  if (typeof(elem) != 'undefined' && elem != null)
  {
	elem.dispatchEvent(g_ShouldCloseEvent);
  }
  Skye.DispatchMsg("Action MainMenu");
}

function goHowToPlay() {
  Skye.DispatchMsg("Action HowToPlay");
}

function goOptions() {
  Skye.DispatchMsg("Action Options");
}

function goCredits() {
  Skye.DispatchMsg("Action Credits");
}

function goResume() {
  Skye.DispatchMsg("Action Resume");
}

function goRestart() {
  var elem = document.getElementById("confirmRestart");
  if (typeof(elem) != 'undefined' && elem != null)
  {
	elem.dispatchEvent(g_ShouldCloseEvent);
  }
  Skye.DispatchMsg("Action Restart");
}

function goQuit() {
  var elem = document.getElementById("confirmQuit");
  if (typeof(elem) != 'undefined' && elem != null)
  {
	elem.dispatchEvent(g_ShouldCloseEvent);
  }
  Skye.DispatchMsg("Action Quit");
}

function goBack() {
    Skye.DispatchMsg("Action Back");
}

function goNewGame() {
    Skye.DispatchMsg("Action NewGame");
}

function goContinue() {
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