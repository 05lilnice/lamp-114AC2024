// ==================== 1. 接腳與模式設定 ====================
const int RED_PIN   = 9;   // RGB 模組 R 腳位
const int GREEN_PIN = 10;  // RGB 模組 G 腳位
const int BLUE_PIN  = 11;  // RGB 模組 B 腳位
const int BUTTON_PIN = 2;  // 按鈕模組 S/OUT 腳位

// 共陽極設為 true；共陰極設為 false（根據你的 -, R, G, B 模組請設為 false）
const bool COMMON_ANODE = false;

// ==================== 2. 顏色定義結構 ====================
struct ColorItem {
  const char* name; // 顏色名稱
  int r;            // 紅色數值 (0~255)
  int g;            // 綠色數值 (0~255)
  int b;            // 藍色數值 (0~255)
};

// 定義可切換的顏色清單
const ColorItem colorList[] = {
  {"OFF (關閉)",   0,   0,   0},
  {"Red (紅色)",    255,   0,   0},
  {"Orange (橘色)", 255, 100,   0},
  {"Yellow (黃色)", 255, 255,   0},
  {"Green (綠色)",    0, 255,   0},
  {"Cyan (青色)",     0, 255, 255},
  {"Blue (藍色)",     0,   0, 255},
  {"Purple (紫色)", 148,   0, 211},
  {"White (白色)",  255, 255, 255}
};

const int TOTAL_COLORS = sizeof(colorList) / sizeof(colorList[0]);
int currentColorIndex = 0; // 紀錄當前的顏色索引

// ==================== 3. 按鈕防抖動變數 ====================
int lastButtonReading = HIGH;
int buttonState = HIGH;
unsigned long lastDebounceTime = 0;
const unsigned long DEBOUNCE_DELAY = 40;

void setup() {
  // 初始化串口通訊，波特率設定為 9600（用於 Display 顯示訊息）
  Serial.begin(9600);

  pinMode(RED_PIN, OUTPUT);
  pinMode(GREEN_PIN, OUTPUT);
  pinMode(BLUE_PIN, OUTPUT);
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // 開機預設為關閉狀態
  applyColor(currentColorIndex);
}

void loop() {
  // 檢查按鈕按壓
  checkButton();
}

// ==================== 4. 功能函式 ====================

// 按鈕邊緣檢測與切換邏輯
void checkButton() {
  int reading = digitalRead(BUTTON_PIN);

  if (reading != lastButtonReading) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > DEBOUNCE_DELAY) {
    if (reading != buttonState) {
      buttonState = reading;

      // 當按鈕被按下（HIGH 變 LOW）時觸發
      if (buttonState == LOW) {
        // 切換到下一個顏色索引
        currentColorIndex = (currentColorIndex + 1) % TOTAL_COLORS;
        
        // 套用新顏色並顯示在電腦螢幕上
        applyColor(currentColorIndex);
      }
    }
  }

  lastButtonReading = reading;
}

// 套用顏色並輸出文字到 Serial Monitor
void applyColor(int index) {
  int r = colorList[index].r;
  int g = colorList[index].g;
  int b = colorList[index].b;

  // 設定 RGB 腳位輸出
  setColor(r, g, b);

  // 在 Display (Serial Monitor) 顯示當前顏色資訊
  Serial.print("Current Color [");
  Serial.print(index);
  Serial.print("/");
  Serial.print(TOTAL_COLORS - 1);
  Serial.print("]: ");
  Serial.print(colorList[index].name);
  Serial.print(" -> RGB(");
  Serial.print(r); Serial.print(", ");
  Serial.print(g); Serial.print(", ");
  Serial.print(b); Serial.println(")");
}

// PWM 控制函式
void setColor(int red, int green, int blue) {
  if (COMMON_ANODE) {
    red   = 255 - red;
    green = 255 - green;
    blue  = 255 - blue;
  }

  analogWrite(RED_PIN, red);
  analogWrite(GREEN_PIN, green);
  analogWrite(BLUE_PIN, blue);
}