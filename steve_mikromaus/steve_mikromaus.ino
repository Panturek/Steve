#define LP 2
#define LT 3
#define PT 4
#define PP 5
#define trigLeft  11
#define trigRight 6
#define trigFront 8
#define echoLeft  10
#define echoRight 7
#define echoFront 9
#define macro_right PORTD=0b101000;
#define macro_left  PORTD=0b010100;
#define macro_go    PORTD=0b100100;
#define macro_back  PORTD=0b011000;
#define macro_stop  PORTD=0b000000;
#define tTurnL 550
#define tTurnR 500
#define tCorr 100
#define tGo 1000


void halt(){
  macro_stop
}

void go(){
  macro_go
  delay(tGo/2);
  macro_left
  delay(100);
  macro_go
  delay(tGo/2);
  macro_stop
  delay(200);
}

void goBack(){
  macro_back
  delay(tGo);
  macro_stop
  delay(200);
}

void turnLeft(){
  macro_left
  delay(tTurnL);
  macro_stop
  delay(200);
}

void turnRight(){
  macro_right
  delay(tTurnR);
  macro_stop
  delay(200);
}

void turnBack(){
  macro_right
  delay(2*tTurnR);
  macro_stop
  delay(200);
}

void corrRight(){
  macro_left
  delay(tCorr);
  macro_stop
  delay(200);
}

void corrLeft(){
  macro_right
  delay(tCorr);
  macro_stop
}


void skalibruj(){
  
  long czas = 0, p1 = 0, p2 = 0, t1 = 0, t2 = 0;
  delay(100);
      digitalWrite(trigFront, LOW);
      delayMicroseconds(2);
      digitalWrite(trigFront, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigFront, LOW);
      czas = pulseIn(echoFront, HIGH);
      p1 = czas/58;
        
      p2 = p1;
      t1 = millis();
      macro_go
    while(p1-p2 < 17){
      digitalWrite(trigFront, LOW);
      delayMicroseconds(2);
      digitalWrite(trigFront, HIGH);
      delayMicroseconds(10);
      digitalWrite(trigFront, LOW);
      czas = pulseIn(echoFront, HIGH);
      p2 = czas/58;
    }
    
    macro_stop
    t2 = millis();
    delay(100);
//    Serial.println(t2-t1);
    macro_back
    delay(t2-t1);
    macro_stop
    delay(200);
    macro_left
    delay(50);
    macro_stop;
    delay(500);

 
}

bool detectFront(){
  unsigned long czas, dystans;
  
  digitalWrite(trigFront, LOW);
  delayMicroseconds(2);
  digitalWrite(trigFront, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigFront, LOW);
  
  czas = pulseIn(echoFront, HIGH);
  dystans = czas/58;
  
  if(dystans > 10) return 1;
  return 0;
}

bool detectLeft(){
  unsigned long czas, dystans;
  
  digitalWrite(trigLeft, LOW);
  delayMicroseconds(2);
  digitalWrite(trigLeft, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigLeft, LOW);
  
  czas = pulseIn(echoLeft, HIGH);
  dystans = czas/58;
  
  if(dystans <= 4) corrLeft();
  else if(dystans > 10) return 1;
  return 0;
}

bool detectRight(){
  unsigned long czas, dystans;
  
  digitalWrite(trigRight, LOW);
  delayMicroseconds(2);
  digitalWrite(trigRight, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigRight, LOW);
  
  czas = pulseIn(echoRight, HIGH);
  dystans = czas/58;

  if(dystans <= 4) corrRight();
  else if(dystans > 10) return 1;
  return 0;
}

void setup() {
  pinMode(OUTPUT, LP);
  pinMode(OUTPUT, LT);
  pinMode(OUTPUT, PP);
  pinMode(OUTPUT, PT);
  pinMode(OUTPUT, trigLeft);
  pinMode(OUTPUT, trigRight);
  pinMode(OUTPUT, trigFront);
  pinMode(INPUT, echoLeft);
  pinMode(INPUT, echoRight);
  pinMode(INPUT, echoFront);

}

void loop() {
  go();delay(1400);
}

void nic(){
  bool ruch;
  bool prawa;
  bool lewa;

  ruch = detectFront();
  delay(50);
  lewa = detectLeft();
  delay(50);
  prawa = detectRight();
  delay(50);

// jak nie może dalej jechać to się cofa
//póki nie bedzie gdzie skręcić
  
  if(!ruch && prawa && lewa){
   
    while(prawa && lewa){
      goBack();
      lewa = detectLeft();
      delay(50);
      prawa = detectRight();
      delay(50);
    }
    if(lewa) turnLeft();
    else if(prawa) turnRight();
    else turnBack();
  }
  
// jak ma gdzie jechać to sprawdza czy
// ma po prawej gdzie jechać
  else if(prawa){
    turnRight();
  }
  
// jak nie ma gdzie skręcić w prawo
// to niech jedzie prosto
  else if(ruch){
    go();
  }
  
// a to tajakby był zakręt w lewo
  else if(lewa){
    turnLeft();  
  }
  
// jak to nie zadziała, to jak Pazura
// pierdolę nie jadę
  else{
    halt();
  }
   
}
