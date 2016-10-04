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
  var t = index;
	index -= 1;
	if(index < 0)
	{
		index = items.length - 1;
	}
  if(t != index)
  {
    Skye.DispatchMsg("Audio PlayMenuSFX MenuHover");
  }
	items[index].onmouseover();
}

function OnTickDown()
{
	items[index].onmouseout();
	var t = index;
	index += 1;
	if(index >= items.length)
	{
		index = 0;
	}
	
	if(t != index)
  {
    Skye.DispatchMsg("Audio PlayMenuSFX MenuHover");
  }
	items[index].onmouseover();
	
}


function OnHover()
{
	Skye.DispatchMsg("Audio PlayMenuSFX MenuHover");
}

function OnSelect()
{
	items[index].onclick();
}
