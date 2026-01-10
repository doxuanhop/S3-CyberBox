#include <SPI.h>
#include <TFT_eSPI.h>

TFT_eSPI tft = TFT_eSPI();

// GÁN LẠI CHÂN THEO SƠ ĐỒ MẠCH THỰC TẾ
#define BTN_UP    45 // Joystick D
#define BTN_DOWN  40 // Joystick B
#define BTN_LEFT  4  // Joystick A
#define BTN_RIGHT 5  // Joystick C
#define BTN_A     37 // KEY2 (OK)
#define BTN_B     36 // KEY1 (Back)

void setup() {
  // 1. Khởi tạo đèn nền (LEDK)
  pinMode(39, OUTPUT);
  digitalWrite(39, HIGH);

  // 2. Khởi tạo các nút bấm (Pullup vì nối GND)
  pinMode(BTN_UP, INPUT_PULLUP);
  pinMode(BTN_DOWN, INPUT_PULLUP);
  pinMode(BTN_LEFT, INPUT_PULLUP);
  pinMode(BTN_RIGHT, INPUT_PULLUP);
  pinMode(BTN_A, INPUT_PULLUP);
  pinMode(BTN_B, INPUT_PULLUP);

  // 3. Khởi tạo màn hình theo đúng yêu cầu dự án
  tft.init();
  tft.setRotation(0);       // Xoay màn hình dự án là 3
  tft.fillScreen(TFT_BLACK); // Nền đen

  // Tiêu đề
  tft.setTextColor(TFT_YELLOW, TFT_BLACK); // Chữ vàng nền đen để không bị đè
  tft.setTextSize(2);
  tft.setCursor(10, 10);
  tft.println("S3 CYBER BOX TEST");
  tft.drawFastHLine(0, 35, 240, TFT_WHITE);
}

void loop() {
  // Kiểm tra và hiển thị trạng thái
  displayBtn(60,  "UP   ", !digitalRead(BTN_UP));
  displayBtn(90,  "DOWN ", !digitalRead(BTN_DOWN));
  displayBtn(120, "LEFT ", !digitalRead(BTN_LEFT));
  displayBtn(150, "RIGHT", !digitalRead(BTN_RIGHT));
  displayBtn(180, "OK (A)", !digitalRead(BTN_A));
  displayBtn(210, "BK (B)", !digitalRead(BTN_B));

  delay(50);
}

void displayBtn(int y, String label, bool isPressed) {
  tft.setCursor(20, y);
  if (isPressed) {
    // Quan trọng: Phải có tham số màu nền TFT_BLACK để xóa chữ cũ
    tft.setTextColor(TFT_RED, TFT_BLACK); 
    tft.print(label + ": [ ACTIVE ]");
  } else {
    tft.setTextColor(TFT_GREEN, TFT_BLACK); 
    tft.print(label + ": [ ------ ]");
  }
}