var UNFull = "";
var UNAll = "Un";
var UNMus = "Un";

function ClickFullScreen()
{
	if(UNFull==="")
	{
		UNFull="Un";
		Skye.DispatchMsg("FullScreen false");
	}
	else
	{
		UNFull ="";
		Skye.DispatchMsg("FullScreen true");
	}
  Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
	var full = document.getElementById("FullScreen");
	full.src = "images/FullScreen_" + UNFull + "Checked_Hovered.png";
	full.dataset.mouseOver = "images/FullScreen_" + UNFull + "Checked_Hovered.png";
	full.dataset.mouseOut = "images/FullScreen_" + UNFull + "Checked.png";
	//full.onmouseover = "this.src=\'images/FullScreen_" + UNFull + "Checked_Hovered.gif\'";
	//full.onmouseout = "this.src=\'images/FullScreen_" + UNFull + "Checked.gif\'";
	//console.log(full.onmouseout);
}

function ClickMuteAll()
{
	if(UNAll==="")
	{
		UNAll="Un";
		Skye.DispatchMsg("MuteAll false");
	}
	else
	{
		UNAll ="";
		Skye.DispatchMsg("MuteAll true");
	}
  Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
	var full = document.getElementById("MuteAll");
	full.src = "images/MuteAll_" + UNAll + "Checked_Hovered.png";
	full.dataset.mouseOver = "images/MuteAll_" + UNAll + "Checked_Hovered.png";
	full.dataset.mouseOut = "images/MuteAll_" + UNAll + "Checked.png";
	//full.onmouseover = "this.src=\'images/FullScreen_" + UNFull + "Checked_Hovered.gif\'";
	//full.onmouseout = "this.src=\'images/FullScreen_" + UNFull + "Checked.gif\'";
	//console.log(full.onmouseout);
}

function ClickMuteMusic()
{
	if(UNMus==="")
	{
		UNMus="Un";
		Skye.DispatchMsg("MuteMusic false");
	}
	else
	{
		UNMus ="";
		Skye.DispatchMsg("MuteMusic true");
	}
  Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
	var full = document.getElementById("MuteMusic");
	full.src = "images/MuteMusic_" + UNMus + "Checked_Hovered.png";
	full.dataset.mouseOver = "images/MuteMusic_" + UNMus + "Checked_Hovered.png";
	full.dataset.mouseOut = "images/MuteMusic_" + UNMus + "Checked.png";
	//full.onmouseover = "this.src=\'images/FullScreen_" + UNFull + "Checked_Hovered.gif\'";
	//full.onmouseout = "this.src=\'images/FullScreen_" + UNFull + "Checked.gif\'";
	//console.log(full.onmouseout);
}

function MouseOver(obj)
{
	var img = document.getElementById(obj);
	img.onload="";
	//console.log(img.dataset.mouseOver);
	img.src = img.dataset.mouseOver;
}

function MouseOut(obj)
{
	var img = document.getElementById(obj);
	img.onload="";
	//console.log(img.dataset.mouseOut);
	img.src = img.dataset.mouseOut;
}

function ClickBack()
{
	Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
	Skye.DispatchMsg("ChangeUI PauseMenu");
	//Skye.DispatchMsg("Action Pause");
}