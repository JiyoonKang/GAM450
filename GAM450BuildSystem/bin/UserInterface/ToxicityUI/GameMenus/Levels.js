
var levels = new Array();

levels[0] = {over: "images/Level1_Button_Hover.png", out: "images/Level1_Button.png", clicked: "images/Level1_Button_Clicked.png"};
levels[1] = {over: "images/Level2_Button_Hover.png", out: "images/Level2_Button.png", clicked: "images/Level2_Button_Clicked.png"};
levels[2] = {over: "images/Level3_Button_Hover.png", out: "images/Level3_Button.png", clicked: "images/Level3_Button_Clicked.png"};
levels[3] = {over: "images/Level4_Button_Hover.png", out: "images/Level4_Button.png", clicked: "images/Level4_Button_Clicked.png"};
levels[4] = {over: "images/Level5_Button_Hover.png", out: "images/Level5_Button.png", clicked: "images/Level5_Button_Clicked.png"};
levels[5] = {over: "images/Level6_Button_Hover.png", out: "images/Level6_Button.png", clicked: "images/Level6_Button_Clicked.png"};

function SelectLevel(id)
{
	id+=1;
	Skye.DispatchMsg("ChangeUI HUD");
	Skye.DispatchMsg("ChangeLevel Level" + id);
	Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
	Skye.DispatchMsg("Action unPause");
}

function GenerateList()
{
	var page = document.getElementById("LevelList");
	var text = "";
	for(var i = 0; i < levels.length; ++i)
	{
		text +="<div class=\"LevelButton\">";
		text +="<img name=\"MenuItem" + i + "\" src=\"" + levels[i].out + "\" onmouseover=\"this.src=\'"+levels[i].over+"\'\" onmouseout=\"this.src= \'"+levels[i].out+"\'\" onclick=\"SelectLevel("+i+")\">";
		//console.log("<img name=\"MenuItem" + i + "\" src=\"" + levels[i].out + "\" onmouseover=\"this.src=\'"+levels[i].over+"\'\" onmouseout=\"this.src= \'"+levels[i].out+"\'\" onclick=\"SelectLevel("+i+")\"></img>");
		text += "</div>";
		
		//console.log("<img name=\"MenuItem" + i + "\" src=\"" + levels[i].out + "\"");
		//page.innerHTML +="onmouseover=\"this.src=\'"+levels[i].over+"\'\""
		//page.innerHTML +="onmouseout=\"this.src= \'"+levels[i].out+"\'\""
		//page.innerHTML +="onclick=\"SelectLevel("+i+")\"></img>"
		//page.innerHTML += "</div>";
	}
	text +="<div class=\"LevelButton\">";
	text +="<img name=\"MenuItem" + i + "\" src=\"images/Back_Button.png\" onmouseover=\"this.src=\'images/Back_Button_Hover.png\'\" onmouseout=\"this.src= \'images/Back_Button.png\'\" onclick=\"ClickBack()\">";
	//console.log("<img name=\"MenuItem" + i + "\" src=\"" + levels[i].out + "\" onmouseover=\"this.src=\'"+levels[i].over+"\'\" onmouseout=\"this.src= \'"+levels[i].out+"\'\" onclick=\"SelectLevel("+i+")\"></img>");
	text += "</div>";
	page.innerHTML = text;
	console.log(page.innerHTML);
}