var counter = 0;
var inter = setInterval(NextSplash, 3000);

var splashes= new Array();

splashes[0] = "images/DigiSplash.png";
splashes[1] = "images/SplashScreen.png";
splashes[2] = "images/GamePadScreen.png";

function PressedKey(e)
{
  var keycode;
  if (window.event)
    keycode = window.event.keyCode;
  else if (e)
    keycode = e.keycode;
  if(keycode == 27)//esc
  {
    NextSplash();
  }
}

function SkipSplash()
{
  clearInterval(inter);
  Skye.DispatchMsg("ChangeUI MainMenu");
}

function NextSplash()
{
	++counter;
	clearInterval(inter);
	if(counter < splashes.length)
	{
		var elem = document.getElementById("SplashImage");
		elem.src = splashes[counter];
		inter = setInterval(NextSplash, 3000);
	}
	else
	{
		//load menu
		Skye.DispatchMsg("ChangeUI MainMenu");
	}
}