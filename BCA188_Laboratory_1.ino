#define CUSTOM_SETTINGS
#define INCLUDE_TERMINAL_MODULE

#include "BluetoothSerial.h"  // Bluetooth library for ESP32
#include <DabbleESP32.h>       // ESP32-compatible Dabble library

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to enable it.
#endif
#if !defined(CONFIG_BT_SPP_ENABLED)
#error Serial Bluetooth not available or not enabled. It is only available for the ESP32 chip.
#endif
#if !defined(CONFIG_BT_BLE_ENABLED)
#error Bluetooth Low Energy (BLE) not available or not enabled. Please run `make menuconfig` to enable BLE support.
#endif

BluetoothSerial ESP_BT;  // Create Bluetooth object

void setup() {
  Serial.begin(9600);  // Initialize serial communication with the computer
  ESP_BT.begin("ESP32_Dabble");  // Start Bluetooth with the name "ESP32_Dabble"

  // Set Bluetooth PIN for pairing security
  ESP_BT.setPin("1234", 4);  // Set a 4-digit PIN for pairing

  Dabble.begin("ESP32_Dabble");  // Initialize Dabble library with Bluetooth device name
  Serial.println("Bluetooth is ready. Connect using Dabble app.");
}

void loop() {
  Dabble.processInput();  // Process incoming data from Dabble app

  // Check if a message is received from the Terminal module
  if (Terminal.available()) {
    String message = Terminal.readString();  // Read the message
    Serial.print("Received from phone: ");
    Serial.println(message);

    // Send a reply back to the mobile phone via Terminal
    Terminal.println("ESP32: Message received - " + message);
  }

  // Check if Bluetooth is still connected
  if (!ESP_BT.hasClient()) {
    Serial.println("Waiting for Bluetooth connection...");
    delay(1000);  // Wait a bit before rechecking
  }

  delay(500);  // Small delay to avoid loop overload
}