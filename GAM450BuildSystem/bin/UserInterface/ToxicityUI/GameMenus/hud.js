var intervals = [0,0,0,0];
var color = ["white","white","white","white"];
var scoreColor = ["#2680FF","#BF00B0","#00E34D","#FF1200"];
var hasBall = [false,false,false,false];

function updateTimer(time)
{
  var doc = document.getElementById("timer1");
  doc.innerHTML = time;
  var d = document.getElementById("winner");
  d.style.opacity = "0";
  if(time < 11)
  {
    doc.style.color = "red";
    doc.style.fontSize = 82 - (time * 5);
    doc.style.top = (48.5 - (10 - time) * 0.33) + "%";
    doc.style.left = (48 - (10 - time) * 0.0125) + "%";
    
    if(time < 5)
    {
      Skye.DispatchMsg("Audio PlayMenuSFX MenuClick");
    }
  }
  else
  {
    doc.style.top = "48.5%";
    doc.style.left = "48%";
    doc.style.fontSize = 32;
    doc.style.color = "white";
  }
}
var inter2 = 0;
function Reset()
{
  var d = document.getElementById("winner");
  d.src = "";
  d.style.opacity = "0";
  clearInterval(inter2);
  inter2 = 0;
}

function displayWinner(color)
{
  var d = document.getElementById("winner");
  d.style.opacity = "1";
  if(inter2 != 0)
  {
    clearInterval(inter2);
  }
  if(color == "Draw")
  {
    
    d.style.backgroundColor = "black";
    d.src = "images/GameEnd_draw.png";
    inter2 = setInterval(Reset, 1450);
  }
  else
  {
    d.style.backgroundColor = "black";
    d.src = "images/GameEnd_"+color+"_Wins.png";
    inter2 = setInterval(Reset, 1450);
  }
    
}

var leadInter = 0;

function showLeading(color)
{
  if(leadInter != 0)
    clearInterval(leadInter);
  
  var d1 = document.getElementById("leaderTop");
  var d2 = document.getElementById("leaderBot");
  d1.src = "images/Leading_"+color+".png";
  d2.src = "images/Leading_"+color+".png";
  d1.style.opacity = "1";
  d2.style.opacity = "1";
  leadInter = setInterval(resetLeader, 500);
}

function resetLeader()
{
  clearInterval(leadInter);
  leadInter = 0;
  var d1 = document.getElementById("leaderTop");
  var d2 = document.getElementById("leaderBot");
  d1.src = "";
  d2.src = "";
  d1.style.opacity = "0";
  d2.style.opacity = "0";
}

function togglePlayerHasBall(player_number)
{
  var d = document.getElementById("player" + player_number  + "_Score");
  if(!hasBall[player_number - 1]){
    d.style.color = scoreColor[player_number - 1];
    hasBall[player_number - 1] = true;
  }
  else{
    d.style.color = "white";
    hasBall[player_number - 1] = false;
  }
}

function toggleAllPlayerDead(dead)
{   
  for(var i = 1; i <= 4; ++i)
  {
    document.getElementById("P" + i  + "Glow").style.opacity = 0.0;
	  togglePlayerDead(i, dead);
  }
}

function togglePlayerDead(player_number, dead)
{   
  //alive
  if(dead == 0)
    document.getElementById("P" + player_number + "Center").src = "images/Hud_CrossHair_Outline.png";
  
  //dead
  else if(dead == 1)
    document.getElementById("P" + player_number + "Center").src = "images/Hud_YOUDEAD.png";
  
  //win condition
  else if(dead == 2)
    document.getElementById("P" + player_number + "Center").src  = "images/Hud_YOUWIN.png";
  
  //lose condition
  else if(dead == 3)
    document.getElementById("P" + player_number + "Center").src  = "images/Hud_YOULOSE.png";
  
  //Get ready...3
  else if(dead == 4)
    document.getElementById("P" + player_number + "Center").src  = "images/Hud_getReady3.png";
  
  //Get ready...2
  else if(dead == 5)
    document.getElementById("P" + player_number + "Center").src = "images/Hud_getReady2.png";
  
    //Get ready...1
  else if(dead == 6)
    document.getElementById("P" + player_number + "Center").src = "images/Hud_getReady1.png";
  
  else if(dead == 7)
    document.getElementById("P" + player_number + "Center").src = "images/Hud_GetTheRing.png";
  
  if(hasBall[player_number - 1])
  {
    togglePlayerHasBall(player_number);
  }
    
}

function updatePlayerHealth(player_number, health)
{
  //get the element by Id in format playerN_score
  //console.log("P"+ player_number +"Square");
  //document.getElementById("player" + player_number  + "_Health").innerHTML = "" + health;
  
  var healthBar = document.getElementById("P" + player_number  + "_HealthBar");
  healthBar.style.width = "1%";
  healthBar.style.height = (health * 4 * .1).toString() + "%";
  if(health < 25)
  {
    document.getElementById("P"+ player_number +"Square").style.backgroundColor = "rgba(255,0,0,0.8)";
    clearInterval(player_number);
    intervals[player_number] = setInterval(clearFlash, 75, player_number);
  }
  if(health <= 0)
  {
	togglePlayerDead(player_number, 1);
  document.getElementById("P" + player_number  + "Glow").style.opacity = 0.0;
  }
}

function goMainMenu() 
{
  Skye.DispatchMsg("ChangeUI MainMenu");
}

function clearFlash(num)
{
	clearInterval(intervals[num]);
	document.getElementById("P"+ num +"Square").style.backgroundColor = "rgba(0,0,0,0.0)";
	intervals[num] = 0;
}
var scoreIntervals = [0,0,0,0];

function resetScoreSize(player_number)
{
  clearInterval(scoreIntervals[player_number - 1]);
  scoreIntervals[player_number - 1] = 0;
  var doc = document.getElementById("player" + player_number  + "_Score");
  doc.style.fontSize = "24px";
}

function updatePlayerScore(player_number, score, isRing)
{
  //get the element by Id in format playerN_score
  var doc = document.getElementById("player" + player_number  + "_Score");
  doc.innerHTML = "" + score;
  if(score > 0)
  {
    doc.style.fontSize = "32px";
    if(scoreIntervals[player_number - 1] != 0)
    {
      clearInterval(scoreIntervals[player_number - 1]);
    }
    scoreIntervals[player_number - 1] = setInterval(resetScoreSize, 100, player_number);
  }
  if(isRing == 1)
  {
    document.getElementById("P" + player_number  + "Glow").style.opacity = 1.0;
  }
}

function updatePlayerAmmo(player_number, ammo)
{
  //get the element by Id in format playerN_score
  //playerAmmo[player_number - 1].innerHTML = "Ammo: " + ammo;
  var ammoBar = document.getElementById("P" + player_number  + "_AmmoBar");
  ammoBar.style.width = "1%";
  ammoBar.style.height = (ammo * .1).toString() + "%";
  //ammoBar.style.top = (55 - (ammo * .05)).toString() + "%";
  ammoBar.style.background = color[player_number - 1];
  if(ammo >=98)
  {
    var overheat = document.getElementById("P" + player_number  + "OverheatOuterdiv");
    //overheat.src = "images/Hud_OverHeat.png";
    //overheat.style.width = "30%";
    //console.log("AEDGEG");
    //overheat.style.filter = "alpha(opacity=100)";
    //overheat.style.opacity = 1.0;
    overheat.innerHTML="OVERHEAT";
	  color[player_number - 1] = "red";
  }
  if(ammo == 0)
  {
    var overheat = document.getElementById("P" + player_number  + "OverheatOuterdiv");
    //overheat.src = "";
    //overheat.style.width = "1.0%";
    //console.log("!AEDGEG");
    //overheat.style.filter = "alpha(opacity=0)";
    //overheat.style.opacity = 0.0;
    overheat.innerHTML="";
	  color[player_number - 1] = "white";
  }
}

function updatePlayerStats(player_number, health, score, ammo)
{
  updatePlayerAmmo(player_number, ammo);
  updatePlayerScore(player_number, score);
  updatePlayerHealth(player_number, health);
}
