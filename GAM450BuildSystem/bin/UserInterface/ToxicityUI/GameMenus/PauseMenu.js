 function ClickResume()
 {
	Skye.DispatchMsg("Action unPause");
	Skye.DispatchMsg("Audio ResumeMusicAll");
  	Skye.DispatchMsg("ChangeUI HUD");
 }
 
 
 function ChangeMenu(destination)
 {
	Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
    Skye.DispatchMsg("ChangeUI "+destination);
 }