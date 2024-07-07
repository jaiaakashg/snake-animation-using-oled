#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define OLED_RESET -1

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

#define VRx_pin A0
#define VRy_pin A1
#define SW_pin 2

const int snake_max_length = 128;
int snakeX[snake_max_length], snakeY[snake_max_length];
int snakeLength = 3;
int snakeDir = 1;
int foodX, foodY;

void setup() {
  Serial.begin(9600);

  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    for (;;);
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(0, 0);
  display.println("Snake Game");
  display.display();

  snakeX[0] = SCREEN_WIDTH / 2;
  snakeY[0] = SCREEN_HEIGHT / 2;

  placeFood();


  drawSnake();
  display.fillRect(foodX, foodY, 4, 4, SSD1306_WHITE);
  display.display();
}

void loop() {
  int xValue = analogRead(VRx_pin);
  int yValue = analogRead(VRy_pin);

  if (xValue < 100) {
    snakeDir = 3; 
  } else if (xValue > 900) {
    snakeDir = 1; 
  } else if (yValue < 100) {
    snakeDir = 0; 
  } else if (yValue > 900) {
    snakeDir = 2; 
  }

  moveSnake();

  if (checkCollision()) {
    gameOver();
    return;
  }

  if (snakeX[0] == foodX && snakeY[0] == foodY) {
    snakeLength++;
    placeFood();
  }

  display.clearDisplay();
  drawSnake();
  display.fillRect(foodX, foodY, 4, 4, SSD1306_WHITE);
  display.display();

  delay(100);
}

void moveSnake() {
  for (int i = snakeLength - 1; i > 0; i--) {
    snakeX[i] = snakeX[i - 1];
    snakeY[i] = snakeY[i - 1];
  }

  switch (snakeDir) {
    case 0:
      snakeY[0]--;
      break;
    case 1: 
      snakeX[0]++;
      break;
    case 2: 
      snakeY[0]++;
      break;
    case 3: 
      snakeX[0]--;
      break;
  }

  // Wrap snake around screen edges
  if (snakeX[0] >= SCREEN_WIDTH)
    snakeX[0] = 0;
  if (snakeX[0] < 0)
    snakeX[0] = SCREEN_WIDTH - 1;
  if (snakeY[0] >= SCREEN_HEIGHT)
    snakeY[0] = 0;
  if (snakeY[0] < 0)
    snakeY[0] = SCREEN_HEIGHT - 1;
}

void drawSnake() {
  // Draw each segment of the snake
  for (int i = 0; i < snakeLength; i++) {
    display.fillRect(snakeX[i], snakeY[i], 4, 4, SSD1306_WHITE);
  }
}

void placeFood() {
  // Place food at random position within screen bounds
  foodX = random(0, SCREEN_WIDTH - 5);
  foodY = random(0, SCREEN_HEIGHT - 5);
}

bool checkCollision() {
  // Check if snake collides with itself
  for (int i = 1; i < snakeLength; i++) {
    if (snakeX[0] == snakeX[i] && snakeY[0] == snakeY[i]) {
      return true;
    }
  }
  return false;
}

void gameOver() {
  // Display game over message
  display.clearDisplay();
  display.setTextSize(2);
  display.setTextColor(SSD1306_WHITE);
  display.setCursor(20, SCREEN_HEIGHT / 2 - 10);
  display.println("Game Over");
  display.display();
  
  // Reset game variables if needed
  snakeLength = 3;
  snakeDir = 1;
  snakeX[0] = SCREEN_WIDTH / 2;
  snakeY[0] = SCREEN_HEIGHT / 2;
  delay(2000); // Delay before restarting game
}

