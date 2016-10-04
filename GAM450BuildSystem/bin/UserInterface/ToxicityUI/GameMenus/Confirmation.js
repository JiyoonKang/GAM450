function ClickYes()
{
  Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
	Skye.DispatchMsg("Action Quit");
}

function ClickNo()
{
	Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
	Skye.DispatchMsg("ChangeUI PauseMenu");
	//Skye.DispatchMsg("Action Pause");
	
}


function ClickNoMM()
{
	Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
	Skye.DispatchMsg("ChangeUI MainMenu");
	//Skye.DispatchMsg("Action Pause");
	
}