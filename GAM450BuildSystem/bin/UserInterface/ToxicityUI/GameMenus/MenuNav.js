var index = 1;

var items = new Array();

function Begin()
{
	var i = 0;
	var item = null;
	do
	{
		item = document.getElementsByName("MenuItem" + i)[0];
		console.log(i);
		console.log(item);
		if(item != null)
		{
			items.push(item);
		}
		++i;
	}while(item != null);
	if(items.length == 1)
		index = 0;
	OnTickUp();
	
	
}


function OnTickUp()
{
	
	items[index].onmouseout();
	index -= 1;
	if(index < 0)
	{
		index = items.length - 1;
	}
	items[index].onmouseover();
}

function OnTickDown()
{
	
	items[index].onmouseout();
	
	index += 1;
	if(index >= items.length)
	{
		index = 0;
	}
	
	
	items[index].onmouseover();
	
}



function ToggleFullScreen()
{
	var source = full.src;
	source = source.substring(source.indexOf("FullScreen"), source.length);
	
	if(source == "FullScreen_Checked.gif" || source == "FullScreen_Checked_Hovered.gif" )
	{
		document.getElementById("FullScreenCheckBox").onmouseover=function() 
		{
			OnHover();
			document.getElementById("FSIMG").src="images/FullScreen_UnChecked_Hovered.gif";
		};
		document.getElementById("FullScreenCheckBox").onmouseout=function() 
		{
			OnHover();
			document.getElementById("FSIMG").src="images/FullScreen_UnChecked.gif";
		};
		document.getElementById("FullScreenCheckBox").onmouseover();
		
		Skye.DispatchMsg("FullScreen false");
	}
	else
	{
		document.getElementById("FullScreenCheckBox").onmouseover=function() 
		{
			document.getElementById("FSIMG").src="images/FullScreen_Checked_Hovered.gif";
		};
		document.getElementById("FullScreenCheckBox").onmouseout=function() 
		{
			document.getElementById("FSIMG").src="images/FullScreen_Checked.gif";
		};
		document.getElementById("FullScreenCheckBox").onmouseover();
		
		Skye.DispatchMsg("FullScreen true");
	}
}

function ToggleMuteMusic()
{
	var source = mutemus.src;
	source = source.substring(source.indexOf("MuteMusic"), source.length);
	console.log(source);
	if(source == "MuteMusic_Checked.gif" || source == "MuteMusic_Checked_Hovered.gif" )
	{
		console.log("H!");
		document.getElementById("MuteMusicCheckBox").onmouseover=function() 
		{
			//OnHover();
			document.getElementById("MMIMG").src="images/MuteMusic_UnChecked_Hovered.gif";
		};
		document.getElementById("MuteMusicCheckBox").onmouseout=function() 
		{
			//OnHover();
			document.getElementById("MMIMG").src="images/MuteMusic_UnChecked.gif";
		};
		document.getElementById("MuteMusicCheckBox").onmouseover();
		
		//Skye.DispatchMsg("FullScreen false");
	}
	else
	{
		document.getElementById("MuteMusicCheckBox").onmouseover=function() 
		{
			document.getElementById("MMIMG").src="images/MuteMusic_Checked_Hovered.gif";
		};
		document.getElementById("MuteMusicCheckBox").onmouseout=function() 
		{
			document.getElementById("MMIMG").src="images/MuteMusic_Checked.gif";
		};
		document.getElementById("MuteMusicCheckBox").onmouseover();
		
		//Skye.DispatchMsg("FullScreen true");
	}
}

function ToggleMuteAll()
{
	var source = muteAll.src;
	source = source.substring(source.indexOf("MuteAll"), source.length);
	console.log(source);
	if(source == "MuteAll_Checked.gif" || source == "MuteAll_Checked_Hovered.gif" )
	{
		console.log("H!");
		document.getElementById("MuteAllCheckBox").onmouseover=function() 
		{
			//OnHover();
			document.getElementById("MAIMG").src="images/MuteAll_UnChecked_Hovered.gif";
		};
		document.getElementById("MuteAllCheckBox").onmouseout=function() 
		{
			//OnHover();
			document.getElementById("MAIMG").src="images/MuteAll_UnChecked.gif";
		};
		document.getElementById("MuteAllCheckBox").onmouseover();
		
		//Skye.DispatchMsg("FullScreen false");
	}
	else
	{
		document.getElementById("MuteAllCheckBox").onmouseover=function() 
		{
			document.getElementById("MAIMG").src="images/MuteAll_Checked_Hovered.gif";
		};
		document.getElementById("MuteAllCheckBox").onmouseout=function() 
		{
			document.getElementById("MAIMG").src="images/MuteAll_Checked.gif";
		};
		document.getElementById("MuteAllCheckBox").onmouseover();
		
		//Skye.DispatchMsg("FullScreen true");
	}
}


function OnHover()
{
	Skye.DispatchMsg("Audio PlayMenuSFX MenuHover");
}

function OnSelect()
{
	items[index].onclick();
}
