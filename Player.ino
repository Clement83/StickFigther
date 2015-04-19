
void updPlayer(Figther * player);
void drwPlayer(Figther * player);
void movePlayer(Figther * player);
void movePlayerSlave();
void changeBoundPlayer(Figther * player);
boolean playerIsAttack(Figther player);
void gestionAttack(Figther * pAttack, Figther * pDef);
boolean addToCombo(Figther *player, byte moveTouch);

boolean stopGame=false;

void initPlayer()
{
  initPlayer(true);
}

void initPlayer(boolean isStartGame)
{
  Player1.currentState = 0;
  Player1.currentSprite = 0;
  Player1.posX = 8;
  Player1.posY = 42;
  Player1.vx = 0;
  Player1.vy = 0;
  Player1.height = 12;
  Player1.width = 8;
  Player1.life = 100;
  Player1.oldLife = 100;
  Player1.cadance = 2;
  Player1.damage = 5;
  Player1.dir = NOFLIP;
  Player1.timeAttack = TIME_ATTACK;
  Player1.isJump = false;
  
  Player2.currentState = 0;
  Player2.currentSprite = 0;
  Player2.posX = 68;
  Player2.posY = 42;
  Player2.vx = 0;
  Player2.vy = 0;
  Player2.height = 12;
  Player2.width = 8;
  Player2.life = 100;
  Player2.oldLife = 100;
  Player2.cadance = 2;
  Player2.damage = 5;
  Player2.dir = FLIPH;
  Player2.timeAttack = TIME_ATTACK;
  Player2.isJump = false;
  
  if(isStartGame)
  {  
    Player1.cptVictory = 0;
    Player2.cptVictory = 0;
  }
  stopGame = true;
  
  //for slave 
  bt_up = false;
  bt_down = false;
  bt_left = false;
  bt_right = false;
  bt_a = false;
  bt_b = false;
}
void updatePlayerSalve()
{
  switch(stateFight)
  {
    case 0 :
       //3, 2, 1 Fight! 
    break;
    case 1 :
      movePlayerSlave();
    break;
    case 2 :
       //Figther KO 
    case 3 :
       //Time UP 
      
    break;
  }
}
void updatePlayer()
{
   
   switch(stateFight)
  {
    case 0 :
       //3, 2, 1 Fight! 

    break;
    case 1 :
       //Fight
        movePlayer(&Player1);
    break;
    case 2 :
       //Figther KO 
    case 3 :
       //Time UP 
       if(stopGame)
       {
         if(Player1.life != Player2.life)
         {
           if(Player1.life>Player2.life)
           {
             Player1.cptVictory++;
           }
           else
           {
             Player2.cptVictory++;
           }
         }
         stopGame=false;
       } 
       
      
    break;
    case 4 : 
    //end Player KO screen
      if(Player1.cptVictory == 3 || Player2.cptVictory == 3 ) 
      {
        stateGame = 4;
      }
      else 
      {
         restartCombat();
      }
    break;
  }
  
  updPlayer(&Player1);
  updPlayer(&Player2);
  
  //update dir player 
  if(Player1.posX<Player2.posX)
  {
    Player1.dir = NOFLIP;
    Player2.dir = FLIPH;
  }
  else 
  {
    Player1.dir = FLIPH;
    Player2.dir = NOFLIP;
  }
  
  if(playerIsAttack(Player1))
  {
    gestionAttack(&Player1,&Player2);
    
  }
  if(playerIsAttack(Player2)) 
  {
      gestionAttack(&Player2,&Player1);
  }
}

void gestionAttack(Figther * pAttack, Figther * pDef)
{
    // IDL : 0 ,run : 1, kick : 2, punchLeft : 3, punchRight : 4,  duck1 : 5, duck1Kick : 6,jump1 : 7,jumpKick1 : 8 , dead1 : 9
    byte damage = 0;
    if(pAttack->currentState == 3 || pAttack->currentState == 4)
    {
      //2 px dist touch
      if(gb.collideRectRect(pAttack->posX - 2, pAttack->posY - 14, 10, 5, pDef->posX, (pDef->posY - pDef->height), pDef->width, pDef->height))
      {
         damage = 5;
      }
    }
    else if(pAttack->currentState == 2 || pAttack->currentState == 8)
    {
      //4 px dist touch
      if(gb.collideRectRect(pAttack->posX - 4, pAttack->posY - 12, 12, 5, pDef->posX, (pDef->posY - pDef->height), pDef->width, pDef->height))
      {
        damage = 8;
      }
    }
    else if(pAttack->currentState == 6)
    {
      if(gb.collideRectRect(pAttack->posX - 4, pAttack->posY - 5, 12, 5, pDef->posX, (pDef->posY - pDef->height), pDef->width, pDef->height))
      {
        damage = 3;
        if(random(0,2) == 0)
        {
          pDef->currentState = 9;
          changeBoundPlayer(pDef);
        }
      }
    }
    if(pDef->isDef>0)
    {
       pAttack->timeNextAttack += 2;
      damage = damage/2;
    }
    
    if(damage)
    {
      pDef->life -= damage;
      if(pDef->life <0)
      {
        pDef->life = 0;
      }
      //gestion des colisions
      if(pDef->dir == NOFLIP)
      {
        //Pdef a gauche 
        if(pDef->life > 0)
        {
          pDef->vx = -SPEED_RUN;
        }
        else 
        {
          pDef->vx = -SPEED_RUN*6;
          pDef->vy = -SPEED_RUN*10;
        }
      }
      else 
      {
        //Pdef a droite 
        if(pDef->life > 0)
        {
          pDef->vx = SPEED_RUN;
        }
        else 
        {
          pDef->vx = SPEED_RUN*6;
          pDef->vy = -SPEED_RUN*10;
        }
      }
    }
}

void drawPlayer()
{
  drwPlayer(&Player1);
  drwPlayer(&Player2);
  //  gb.display.print(Player1.currentState);
  
  //vie P1
  gb.display.fillRect(14, 2, lifeTopixel(Player1.life), 3);
  //vie P2
  byte offsetPx = lifeTopixel(Player2.life);
  gb.display.fillRect(47 + (23-offsetPx) , 2, offsetPx , 3);
  
  for(byte i=0;i<Player1.cptVictory;i++)
  {
    //2 - 7 - 12
    gb.display.fillRect((i*5 + 2) , 8, 2 , 2);
  }
  
  for(byte i=0;i<Player2.cptVictory;i++)
  {
    //2 - 7 - 12
    gb.display.fillRect(80 - (i*5) , 8, 2 , 2);
  }
}

byte lifeTopixel(byte life)
{
  return (life*0.23);
}

void drwPlayer(Figther * player)
{
  gb.display.drawBitmap(((player->dir == NOFLIP)? player->posX : (player->posX -(player->width-8) ) ) ,(player->posY - player->height) , ((player->currentSprite == 0) ? player->sprites[player->currentState].sprite1 : player->sprites[player->currentState].sprite2 ), 0,player->dir);
}

#define SEUIL_MIN_MOVE 0.1
void updPlayer(Figther * player)
{
  if(gb.frameCount%player->cadance == 0)
  {
    player->currentSprite++;
    player->currentSprite = player->currentSprite%NB_SPRITE_STATE;
  }
  
  if(player->timeNextAttack>0)
  {
    player->timeNextAttack--;
  }
  
  if(player->timeAttack>0)
  {
    player->timeAttack--;
  }
  
  if(player->isDef>0)
  {
    player->isDef--;
  }
  
  if(player->life>0)
  {
    if(player->timeAttack==0 && player->currentState != 0 && player->currentState != 5  && player->currentState != 9)
    {
      // IDL : 0 ,run : 1, kick : 2, punchLeft : 3, punchRight : 4,  duck1 : 5, duck1Kick : 6,jump1 : 7,jumpKick1 : 8 , dead1 : 9
      if(player->currentState == 6)
        player->currentState = 5;
      else if(player->isJump)
        player->currentState = 7;
      else if(player->vx<SEUIL_MIN_MOVE && player->vx>-SEUIL_MIN_MOVE)
        player->currentState = 0;
      else 
        player->currentState = 1;
      
      changeBoundPlayer(player);
    }
  }
  else 
  {
    player->currentState = 9;
    changeBoundPlayer(player);
  }
  
  if(!player->isJump)
  {
    if(player->vx!=0)
    {
      player->posX += player->vx;
      player->vx = player->vx*0.4;//on ralentie rapidement
    }
    else if(player->vx<SEUIL_MIN_MOVE && player->vx>-SEUIL_MIN_MOVE)
    {
      player->vx = 0;
    }
  }
  else 
  {
    if(player->vy<-1.2)
    {
      player->vy *= 0.9;
      player->posY += player->vy;
    }
    else
    {
      if(player->vy<0) player->vy = -player->vy;
      player->vy *= 1.3;
      player->posY += player->vy;
    }
    player->posX += player->vx;
    player->vx = player->vx*0.9;//on ralentie rapidement
    
    if(player->posY>= GROUND_Y)
    {      
      player->posY = GROUND_Y;
      player->isJump = false;
      player->vy = 0;
      player->vx = 0;
      player->timeAttack = 0;
    }
  }
  
  if(player->posX<0)
  {
    player->posX = 0;
  }
  else if((player->posX + player->width) >84)
  {
    player->posX = (84 - player->width);
  }
}



void movePlayerSlave()
{
   if(gb.buttons.repeat(BTN_RIGHT, 1))
  {
    bt_right = true;
  }
  else if(gb.buttons.repeat(BTN_LEFT, 1))
  {
    bt_left = true;
  }
  if(gb.buttons.pressed(BTN_UP))
  {
    bt_up = true;
  }
  else if(gb.buttons.repeat(BTN_DOWN, 1))
  {
    bt_down = true;
  }
  
  if(gb.buttons.pressed(BTN_A))
  {
     bt_a = true;
  }
  else if(gb.buttons.pressed(BTN_B))
  {
    bt_b = true;
  }
}

void movePlayer(Figther * player)
{
  
  if(gb.buttons.repeat(BTN_RIGHT, 1))
  {
    rightFigther(player);
  }
  else if(gb.buttons.repeat(BTN_LEFT, 1))
  {
    leftFigther(player);
  }
  if(gb.buttons.pressed(BTN_UP))
  {
    highFigther(player);
  }
  else if(gb.buttons.repeat(BTN_DOWN, 1))
  {
    bottomFigther(player);
  }
  
  if(gb.buttons.pressed(BTN_A))
  {
    punchFigther(player);
  }
  else if(gb.buttons.pressed(BTN_B))
  {
    kickFigther(player);
  }
}

void moveIAPlayer(Figther * player,Figther * human)
{
  if(stateFight == 1)
  {
    byte rdm = random(0,100);
    if(rdm <=20)
    {
      punchFigther(player);
    }
    else if(rdm >65 && rdm <95)
    {
      kickFigther(player);
    }
    if(rdm <=30)
    {
      if(player->dir == NOFLIP) rightFigther(player);
      else leftFigther(player);
    }
    if(rdm >19 && rdm <25)
    {
      highFigther(player);
    }
    else if(rdm >39 && rdm <45)
    {
      bottomFigther(player);
    }
  }
}

// IDL : 0 ,run : 1, kick : 2, punchLeft : 3, punchRight : 4,  duck1 : 5, duck1Kick : 6,jump1 : 7,jumpKick1 : 8 , dead1 : 9
void leftFigther(Figther * player)
{
  if(player->life==0)
    return;
  addToCombo(player,3);
  if(!player->isJump)
  {
    
    if(player->dir == NOFLIP)
    {
      //recule
      player->isDef = TIME_DEF;
      player->vx =  -SPEED_RUN/2;
    }
    else 
    {
      //avance
      player->vx =  -SPEED_RUN;
    }
  
    if(!playerIsAttack(*player)) player->currentState = 1;
  }
  else 
  {
    if(player->vx>-SPEED_RUN) player->vx -= 0.1;
  }
  
    changeBoundPlayer(player);
}

void rightFigther(Figther * player)
{
  if(player->life==0)
    return;
  addToCombo(player,4);
  if(!player->isJump)
  {
    if(player->dir == FLIPH)
    {
      //recule
      player->isDef = TIME_DEF;
      player->vx =  SPEED_RUN/2;
    }
    else 
    {
      //avance
      player->vx =  SPEED_RUN;
    }
    if(!playerIsAttack(*player))player->currentState = 1;
  }
  else 
  {
    if(player->vx<SPEED_RUN) player->vx += 0.1;
  }
    changeBoundPlayer(player);
}

void highFigther(Figther * player)
{
  if(player->life==0)
    return;
  addToCombo(player,1);
 if(!player->isJump)
 {
    player->currentState = 7;
    changeBoundPlayer(player);
    player->isJump = true;
    player->vy = -SPEED_RUN;
 }
}

void bottomFigther(Figther * player)
{
  if(player->life==0)
    return;
  addToCombo(player,2);
  player->currentState = 5;
  changeBoundPlayer(player);
}

void punchFigther(Figther * player)
{
  if(player->life==0)
    return;
  if(player->timeNextAttack>0)
    return;
 
  addToCombo(player,5);   
  if(player->currentState == 5)
  {
     player->currentState = 6;
     player->timeAttack = TIME_ATTACK;
  }
  else if(!player->isJump)
  {
    player->currentState = random(3,5);
    player->timeAttack = TIME_ATTACK;
  }
  else 
  {
    player->currentState = 8;
    player->timeAttack = TIME_ATTACK*4;
  }
  player->timeNextAttack = TIME_ATTACK+1; // puch are fast 
  changeBoundPlayer(player);
}

void kickFigther(Figther * player)
{
  if(player->life==0)
    return;
  if(player->timeNextAttack>0)
    return;
  addToCombo(player,6);
  if(player->currentState == 5)
 {
   player->currentState = 6;
   player->timeAttack = TIME_ATTACK;
 }
 else  if(!player->isJump)
 {
  player->timeAttack = TIME_ATTACK;
  player->currentState = 2;
 }
 else 
 {
    player->timeAttack = TIME_ATTACK*4;
  player->currentState = 2;
 }
  player->timeNextAttack = TIME_ATTACK+3; // Kick are slow 
  changeBoundPlayer(player);
}

void changeBoundPlayer(Figther * player)
{
  player->width = pgm_read_byte(player->sprites[player->currentState].sprite1);
  player->height = pgm_read_byte(player->sprites[player->currentState].sprite1 + 1);
}

boolean playerIsAttack(Figther player)
{
  return player.timeAttack>0;
}

boolean addToCombo(Figther *player, byte moveTouch) // moveTouch : 1=>up, 2=>down, 3=>forward, 4=>backward, 5=>A, 6=>B
{
  for(byte i=1;i<NB_MOVE_SAVE;i++)
  {
    player->combo[i] = player->combo[i-1];
  }
  player->combo[0] = moveTouch;
  
  if(player->combo[0] == 3 && player->combo[1] == 3 )
  {
    gb.popup(F("AYOUKEN"),5);
    //test combo et fire ball
    //player->currentState = 10;
    //TIME_LIVE_AYOUKEN;
    return true;
  }
  
  return false;
}


