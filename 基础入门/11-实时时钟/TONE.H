/*  
// BELOW FOR DEBUG
  
  MAIN_POWER_ON                                      ;
  P7DIR   |= BUZZER;
  Start_Sound();
  for(;;)
  {
  for(j=0;j<1000;j++)
  {
    P7OUT ^= BUZZER             ;
    Delay_nus(302)   ; //1
  }
  
  for(j=0;j<1000;j++)
  {
    P7OUT ^= BUZZER             ;
    Delay_nus(268)   ; //2
  }
    
  for(j=0;j<1200;j++)
  {
    P7OUT ^= BUZZER             ;
    Delay_nus(237)   ; //3
  }
  for(j=0;j<1200;j++)
  {
    P7OUT ^= BUZZER             ;
    Delay_nus(227)   ; //4
  }
  for(j=0;j<1300;j++)
  {
    P7OUT ^= BUZZER             ;
    Delay_nus(202)   ;//5
  }

  for(j=0;j<1300;j++)
  {
    P7OUT ^= BUZZER             ;
    Delay_nus(180)   ;//6
  }

  for(j=0;j<1800;j++)
  {
    P7OUT ^= BUZZER             ;
    Delay_nus(160)   ;//7
  }
  for(j=0;j<3000;j++)
  {
    P7OUT ^= BUZZER             ;
    Delay_nus(151)   ; //1+
  }

  }
  
  

// ABOVE FOR DEBUG
    
// BELOW FOR DEBUG
  MAIN_POWER_ON                                      ;
  P10DIR  |= TXD;
  for(;;)
  {
//    P10OUT |= TXD             ;
//    __delay_cycles(22*1000)   ;
//    P10OUT &=~TXD             ;
//    __delay_cycles(22*1000)   ;
  
    P7OUT |= BUZZER             ;
    __delay_cycles(22*300)   ;
    P7OUT &=~BUZZER             ;
    __delay_cycles(22*300)   ;
  }
//ABOVE FOR DEBUG
*/  


void Delay_nus(unsigned int time)
{
  unsigned int i              ;
  for(i=time;i>0;i--)
//    __delay_cycles(20*2)   ;    
    __delay_cycles(17*2)   ;    // FLL_FACTOR = 549
  
}

void Start_Sound(void)
{
  unsigned int j;
//  for(;;)
  {  
    for(j=0;j<200*0.9;j++)
    {
      P7OUT ^= BUZZER             ;
      Delay_nus(400)   ; // -5
    }
    for(j=0;j<300*0.9;j++)
    {
      P7OUT ^= BUZZER             ;
      Delay_nus(300)   ; //1
    }
  
    for(j=0;j<300*0.9;j++)
    {
      P7OUT ^= BUZZER             ;
      Delay_nus(240)   ; //3
    }
    for(j=0;j<500*0.9;j++)
    {
      P7OUT ^= BUZZER             ;
      Delay_nus(200)   ;//5
    }
    for(j=0;j<350*0.9;j++)
    {
      Delay_nus(200)   ; // PAUSE
    }
  
    for(j=0;j<300*0.9;j++)
    {
      P7OUT ^= BUZZER             ;
      Delay_nus(240)   ; // 3
    }
    for(j=0;j<1000*0.9;j++)
    {
      P7OUT ^= BUZZER             ;
      Delay_nus(200)   ; // 5
    }
    for(j=0;j<2800;j++)
      Delay_nus(300)      ;
  }  
}