//=============================================================================
//\author Felipe Robledo
//
//Copyright (C) 2014 DigiPen Institute of Technology.
//Reproduction or disclosure of this file or its contents without the prior 
//written consent of DigiPen Institute of Technology is prohibited.
//=============================================================================

//=============================================================================
//  EXTERNAL FILES:
//  classie.js
//=============================================================================

//=============================================================================
//  GLOBAL DATA
//=============================================================================
var g_SelectedID = 0;
var g_MainMenuElements = new Array;
var g_ShouldUseMouse = true;
var g_CSSEffectClassName = "default";

// Constants
var g_Right = 39;
var g_Left = 37;
var g_Up = 38;
var g_Down = 40;

//=============================================================================
//  INITIALIZATION FUNCTIONALITY
//=============================================================================
function LoadMainMenu(css_effect_class_name) {
	g_CSSEffectClassName = css_effect_class_name;
	console.log("CSS EFFET NAME: " + css_effect_class_name);
	var domNodeList = document.getElementsByClassName("IcarusButton");
	for(var i = 0; i < domNodeList.length; ++i)
	{
		// Work-around the fact that onload is not working on button elements
		domNodeList[i].onload();
	}
	
	// Get the options from the engine to know if
	//    whether or not we should be using mouse
	
	// Get from engine: info about if there is a save
	//	  file or not.
	
	// Pop either New Game or Continue, depending on what
	//    the engine tells us
}

function RegisterButton(menuElement) {
	//Icarus.DispatchMsg("Logging: " + menuElement.id);
	g_MainMenuElements.push(menuElement.id);
	
}

function SelectMenuItem(menuButtonId) {
	var menuElement = document.getElementById(menuButtonId);
	classie.add(menuElement,g_CSSEffectClassName);
	//console.log("Pop-up button: " + menuButtonId);
}	

function DeselectMenuItem(menuButtonId) {
	var menuElement = document.getElementById(menuButtonId);
	classie.remove(menuElement,g_CSSEffectClassName); // !! I think this will be a bug
	//console.log("Fade button: " + menuButtonId);
}

//=============================================================================
//  KEYBOARD FUNCTIONALITY
//=============================================================================

// Handles keyboard input to move through the array of indices. After resolving
//    input, we either perform our effects with CSS or not. Depends on if we 
//    have changed indices or not.
function NextMenuItem(event) {
	//console.log("Doing something");
	//Icarus.DispatchMsg("Doing something");
	var keyString = String.fromCharCode(event.keyCode);
	var previousIndex = g_SelectedID;
	if(keyString == 'S' || keyString == 'D' || event.keyCode == g_Right || event.keyCode == g_Up) {
    g_SelectedID = (g_SelectedID + 1) % g_MainMenuElements.length;
	} else if(keyString == 'W' || keyString == 'A' || event.keyCode == g_Left || event.keyCode == g_Down) {
		  --g_SelectedID;
		if(g_SelectedID < 0) { // Wrap
			g_SelectedID = g_MainMenuElements.length - 1;
		}
	} else if(keyString == '\n' || keyString == ' ') {
		// basically, if either the spacebar or enter are pressed
		DispatchMsg(g_MainMenuElements[g_SelectedID]);
	}

	ResolveChange(previousIndex,g_SelectedID);
}

function NextMenuItemByDirection(direction) {
	if(direction < 0) {
	    --g_SelectedID;
		if(g_SelectedID < 0) { // Wrap
			g_SelectedID = g_MainMenuElements.length - 1;
		}
	} else {
		g_SelectedID = (g_SelectedID + 1) % g_MainMenuElements.length;
	}
}

//=============================================================================
//  MOUSE FUNCTIONALITY
//=============================================================================

// Figures out which button we are on. We need to keep track of this
//    for the keyboard to work, too.
function MouseEnters(menuButton) {
	// Get the index where the id of the menuButton resides
	var previousIndex = g_SelectedID;
	for(var i = 0; i < g_MainMenuElements.length; ++i) {
		if(menuButton.id == g_MainMenuElements[i]) {
			g_SelectedID = i;
			break;
		}
	}
	
	ResolveChange(previousIndex,g_SelectedID);
}

//=============================================================================
//  MUNDANE HELPERS
//=============================================================================

function ResolveChange(previousIndex,newIndex) {
	if(previousIndex != newIndex) {
		var menuButtonId = g_MainMenuElements[newIndex];
		var prevMenuButtonId = g_MainMenuElements[previousIndex];
		SelectMenuItem(menuButtonId);
		DeselectMenuItem(prevMenuButtonId);
	}	
}