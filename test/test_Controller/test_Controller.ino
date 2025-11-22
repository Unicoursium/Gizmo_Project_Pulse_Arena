#include <Bluepad32.h>

ControllerPtr myController = nullptr;

void onConnected(ControllerPtr ctl) {
  Serial.println(">>> Controller connected!");
  myController = ctl;
}

void onDisconnected(ControllerPtr ctl) {
  Serial.println(">>> Controller disconnected!");
  if (myController == ctl) {
    myController = nullptr;
  }
}

void setup() {
  Serial.begin(115200);
  delay(2000);
  Serial.println("Starting Bluepad32 on classic ESP32...");

  // initialize Bluepad32
  BP32.setup(&onConnected, &onDisconnected);

  // on first use, uncomment it
  //BP32.forgetBluetoothKeys();

  // Print esp32 info
  const uint8_t* addr = BP32.localBdAddress();
  Serial.printf("BD Addr: %02X:%02X:%02X:%02X:%02X:%02X\n",
                addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);

  Serial.println("Now put your gamepad into pairing mode.");
}

void loop() {
  BP32.update();

  if (myController && myController->isConnected()) {
    int lx = myController->axisX();   // L X
    int ly = myController->axisY();   // L Y
    int rx = myController->axisRX();  // R X
    int ry = myController->axisRY();  // R Y

    bool aPressed = myController->a();  // A button

    Serial.print("LX: "); Serial.print(lx);
    Serial.print("  LY: "); Serial.print(ly);
    Serial.print("  RX: "); Serial.print(rx);
    Serial.print("  RY: "); Serial.print(ry);
    Serial.print("  A: "); Serial.println(aPressed ? "Pressed" : "Released");

    delay(50);
  } else {
    // if the controller is not connected
    static uint32_t lastPrint = 0;
    if (millis() - lastPrint > 2000) {
      Serial.println("Waiting for controller... Put your gamepad in pairing mode.");
      lastPrint = millis();
    }
  }
}
