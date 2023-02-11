#include <LiquidCrystal.h>
#include <IRremote.h>

const int bMove = 7;
const int bShoot = 8;
const int Contrast = 120;

LiquidCrystal lcd(12, 11, 5, 4, 3, 2);

int randRow, randCol, killCount = 0, level=200;

bool up = true, lose = true;

String gameOver = "Game Over", title = "Zombie Rush";

// Sprites
byte guy[] = {
  B01110,
  B01110,
  B01110,
  B00100,
  B00111,
  B00100,
  B01010,
  B10010
};

byte muzzleFlash [] = {
  B00000,
  B00000,
  B00000,
  B00100,
  B01000,
  B00100,
  B00000,
  B00000
};

byte zombie[] = {
  B00000,
  B00111,
  B00111,
  B00010,
  B01110,
  B00110,
  B00011,
  B00101
};

byte hitMarker[] = {
  B10001,
  B01010,
  B01010,
  B00100,
  B00100,
  B01010,
  B01010,
  B10001
};

void setup() {

  // Buttons
  pinMode(bMove, INPUT_PULLUP);
  pinMode(bShoot, INPUT_PULLUP);
  
  analogWrite(6, Contrast);
  lcd.begin(16, 2);

  // slot characters
  lcd.createChar(1, guy);
  lcd.createChar(2, zombie);
  lcd.createChar(3, muzzleFlash);
  lcd.createChar(4, hitMarker);

  randomSeed(analogRead(A1));
  Serial.begin(9600);
}

void loop() {
  
  startMenu();

  while (lose == false) {

  	generation();   
    for (randCol; randCol > 0; randCol--) {
      
      gameLost();
      lcd.setCursor(randCol, randRow);
      lcd.write(2);
      delay(level);
      lcd.clear();
      redraw();
      control();
    }
  }
}

void startMenu() {
  
  lcd.setCursor(2, 0);
    for (int t = 0; t < 11; t++) {
      lcd.print(title[t]);
      delay(100);
  }
  
  lcd.setCursor(0, 1);
  lcd.write(1);
  lcd.setCursor(15, 0);
  lcd.write(2);
  lcd.setCursor(3, 1);
  lcd.print("Press Start");
  
  while(digitalRead(bMove) != LOW) {}
  lcd.clear();
  lose = false;
  lcd.setCursor(0, 0);
  lcd.write(1);
}

void generation() {
  
   randRow = random(2);
   randCol = random(16, 20);
}

void moveUpandDown() {

  if (up == true) {
    lcd.clear();
    lcd.setCursor(0, 1);
    lcd.write(1);
    up = !up;
  } else if (!up) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.write(1);
    up = !up;
  }
}

void control() {

  if (digitalRead(bMove) == LOW) {
    moveUpandDown();
  }
  if (digitalRead(bShoot) == LOW){
    shoot();
  }
}

void redraw() {

  if (up == true) {
    lcd.setCursor(0, 0);
    lcd.write(1);
  } else {
    lcd.setCursor(0, 1);
    lcd.write(1);
  }
}

void shoot() {

  if (up == true) {
    lcd.setCursor(1, 0);
  } else {
    lcd.setCursor(1, 1);
  }
  lcd.write(3);
  delay(70);
  lcd.clear();
  redraw();
  kill();
}

void kill() {

  if ((killCount > 5) && (killCount <= 10)) {
    level = 150;
  } else if ((killCount > 10) && (killCount <= 15)) {
    level = 100;
  } else if ((killCount > 15) && (killCount <= 200)) {
    level = 70;
  } else level = 200;

  if ((up == true) && (randRow == 0)) {
    lcd.setCursor(randCol, randRow);
    lcd.write(4);
    delay(300);
    killCount++;
    generation();
  } else if ((up == false) && (randRow == 1)) {
    lcd.setCursor(randCol, randRow);
    lcd.write(4);
    delay(300);
    killCount++;
    generation();
  }
}

void gameLost() {
   
  if (randCol <= 1) {
    if (!up) {
      lcd.setCursor(0, 1);
    } else lcd.setCursor(0, 0);
    lcd.write(4);
    delay(1000);
    lcd.clear();
    lcd.setCursor(3, 0);
    for (int g = 0; g < 9; g++) {
      lcd.print(gameOver[g]);
      delay(100);
    }
    lcd.setCursor(3, 1);
    lcd.print("Kills:");
    lcd.print(killCount);
    killCount = 0;
    lose = true;
    delay(3000);
    startMenu();
  }
}
