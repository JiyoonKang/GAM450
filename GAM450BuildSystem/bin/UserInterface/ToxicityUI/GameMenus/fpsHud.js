/*
\author Esteban Maldonado 

Copyright (C) 2015 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the prior 
written consent of DigiPen Institute of Technology is prohibited.
*/
var playerAmmo = new Array();


function updateTimer(time)
{
  document.getElementById("timer1").innerHTML = time;
  // document.getElementById("timer2").innerHTML = time;
  // document.getElementById("timer3").innerHTML = time;
  // document.getElementById("timer4").innerHTML = time;
}

function toggleAllPlayerDead(dead)
{   
  //alive
  if(dead == 0)
  {
    document.getElementById("player1" ).style.background = "url('images/Hud_CrossHair.png')";
    document.getElementById("player2" ).style.background = "url('images/Hud_CrossHair.png')";
    document.getElementById("player3" ).style.background = "url('images/Hud_CrossHair.png')";
    document.getElementById("player4" ).style.background = "url('images/Hud_CrossHair.png')";
  }
    //dead
  else if(dead == 1)
  {
    document.getElementById("player1").style.background = "url('images/Hud_YOUDEAD.png')";
    document.getElementById("player2").style.background = "url('images/Hud_YOUDEAD.png')";
    document.getElementById("player3").style.background = "url('images/Hud_YOUDEAD.png')";
    document.getElementById("player4").style.background = "url('images/Hud_YOUDEAD.png')";
  }
  
  //win condition
  else if(dead == 2)
  {
    document.getElementById("player1").style.background = "url('images/Hud_YOUWIN.png')";
    document.getElementById("player2").style.background = "url('images/Hud_YOUWIN.png')";
    document.getElementById("player3").style.background = "url('images/Hud_YOUWIN.png')";
    document.getElementById("player4").style.background = "url('images/Hud_YOUWIN.png')";
  }
  
  //lose condition
  else if(dead == 3)
  {  
    document.getElementById("player1").style.background = "url('images/Hud_YOULOSE.png')";
    document.getElementById("player2").style.background = "url('images/Hud_YOULOSE.png')";
    document.getElementById("player3").style.background = "url('images/Hud_YOULOSE.png')";
    document.getElementById("player4").style.background = "url('images/Hud_YOULOSE.png')";
  }
  //Get ready...3
  else if(dead == 4)
  {
    document.getElementById("player1").style.background = "url('images/Hud_getReady3.png')";
    document.getElementById("player2").style.background = "url('images/Hud_getReady3.png')";
    document.getElementById("player3").style.background = "url('images/Hud_getReady3.png')";
    document.getElementById("player4").style.background = "url('images/Hud_getReady3.png')";
  }
  
  //Get ready...2
  else if(dead == 5)
  {
    document.getElementById("player1").style.background = "url('images/Hud_getReady2.png')";
    document.getElementById("player2").style.background = "url('images/Hud_getReady2.png')";
    document.getElementById("player3").style.background = "url('images/Hud_getReady2.png')";
    document.getElementById("player4").style.background = "url('images/Hud_getReady2.png')";
  }
  
    //Get ready...1
  else if(dead == 6)
  {
    document.getElementById("player1").style.background = "url('images/Hud_getReady1.png')";
    document.getElementById("player2").style.background = "url('images/Hud_getReady1.png')";
    document.getElementById("player3").style.background = "url('images/Hud_getReady1.png')";
    document.getElementById("player4").style.background = "url('images/Hud_getReady1.png')";
  }
    
}

function togglePlayerDead(player_number, dead)
{   
  //alive
  if(dead == 0)
    document.getElementById("player" + player_number).style.background = "url('images/Hud_CrossHair.png')";
  
    //dead
  else if(dead == 1)
    document.getElementById("player" + player_number).style.background = "url('images/Hud_YOUDEAD.png')";
  
  //win condition
  else if(dead == 2)
    document.getElementById("player" + player_number).style.background = "url('images/Hud_YOUWIN.png')";
  
  //lose condition
  else if(dead == 3)
    document.getElementById("player" + player_number).style.background = "url('images/Hud_YOULOSE.png')";
  
  //Get ready...3
  else if(dead == 4)
    document.getElementById("player" + player_number).style.background = "url('images/Hud_getReady3.png')";
  
  //Get ready...2
  else if(dead == 5)
    document.getElementById("player" + player_number).style.background = "url('images/Hud_getReady2.png')";
  
    //Get ready...1
  else if(dead == 6)
    document.getElementById("player" + player_number).style.background = "url('images/Hud_getReady1.png')";
    
}

function updatePlayerStats(player_number, health, score, ammo)
{
    //get the element by Id in format playerN_score
  document.getElementById("player" + player_number  + "_health").innerHTML = "Health: " + health;
  
    //get the element by Id in format playerN_score
  document.getElementById("player" + player_number  + "_score").innerHTML = "Score: " + score;
  
    //get the element by Id in format playerN_score
  document.getElementById("player" + player_number  + "_ammo").innerHTML = "Ammo: " + ammo;
}

function updatePlayerHealth(player_number, health)
{
  //get the element by Id in format playerN_score
  document.getElementById("player" + player_number  + "_health").innerHTML = "Health: " + health;
}

function updatePlayerScore(player_number, score)
{
  //get the element by Id in format playerN_score
  document.getElementById("player" + player_number  + "_score").innerHTML = "Score: " + score;
}

function updatePlayerAmmo(player_number, ammo)
{
  //get the element by Id in format playerN_score
  //playerAmmo[player_number - 1].innerHTML = "Ammo: " + ammo;
  document.getElementById("player" + player_number  + "_ammo").innerHTML = "Ammo: " + ammo;
}