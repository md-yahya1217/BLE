#include <BLEDevice.h>
#include <BLEAdvertising.h>
#define analogue 1

#ifndef CONFIG_BT_BLE_50_FEATURES_SUPPORTED
#error "This SoC does not support BLE5. Try using ESP32-C3, or ESP32-S3"
#else

esp_ble_gap_ext_adv_params_t ext_adv_params_2M = {
  .type = ESP_BLE_GAP_SET_EXT_ADV_PROP_NONCONN_NONSCANNABLE_UNDIRECTED,
  .interval_min = 0x40,
  .interval_max = 0x40,
  .channel_map = ADV_CHNL_ALL,
  .own_addr_type = BLE_ADDR_TYPE_RANDOM,
  .peer_addr_type = BLE_ADDR_TYPE_RANDOM,
  .peer_addr = {0, 0, 0, 0, 0, 0},
  .filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
  .tx_power = EXT_ADV_TX_PWR_NO_PREFERENCE,
  .primary_phy = ESP_BLE_GAP_PHY_1M,
  .max_skip = 0,
  .secondary_phy = ESP_BLE_GAP_PHY_2M,
  .sid = 1,
  .scan_req_notif = false,
};

static uint8_t raw_scan_rsp_data_2m[] = {
  0x02, 0x01, 0x06, 0x02, 0x0a, 0xeb, 
  0x12, 0x09, 'E', 'S', 'P', '_', 'M', 
  'U', 'L', 'T', 'I', '_', 'A', 'D', 'V', '_', '2', 'M', 0x00
};

static esp_ble_gap_periodic_adv_params_t periodic_adv_params = {
  .interval_min = 0x320,  // 1000 ms interval
  .interval_max = 0x640,
  .properties = 0,  // Do not include TX power
};

uint8_t addr_2m[6] = {0xc0, 0xde, 0x52, 0x00, 0x00, 0x02};
BLEMultiAdvertising advert(1);  // max number of advertisement data

void setup() {
  Serial.begin(115200);
  Serial.println("Multi-Advertising...");

  BLEDevice::init("");

  advert.setAdvertisingParams(0, &ext_adv_params_2M);
  advert.setAdvertisingData(0, sizeof(raw_scan_rsp_data_2m), &raw_scan_rsp_data_2m[0]);
  advert.setInstanceAddress(0, addr_2m);
  advert.setDuration(0, 0, 0);

  // Read sensor value
  uint8_t sensor_value = analogRead(analogue);
  Serial.print("Sensor Value: ");
  Serial.println(sensor_value);

  // Convert integer to bytes
  uint8_t sensor_value_bytes[4];
  sensor_value_bytes[0] = (sensor_value >> 24) & 0xFF;
  sensor_value_bytes[1] = (sensor_value >> 16) & 0xFF;
  sensor_value_bytes[2] = (sensor_value >> 8) & 0xFF;
  sensor_value_bytes[3] = sensor_value & 0xFF;

  // Example of new advertising data with sensor value included
  static uint8_t periodic_adv_raw_data[] = {
    0x02, 0x01, 0x06,                   // Flags
    0x02, 0x0a, 0xeb,                   // TX Power Level
    0x03, 0x03, 0xab, 0xcd,             // Service UUID
    0x08, 0xff, sensor_value_bytes[0], sensor_value_bytes[1], sensor_value_bytes[2], sensor_value_bytes[3],  // Sensor Data
    0x11, 0x09, 'E', 'S', 'P', ' ', 'M', 'U', 'L', 'T', 'I', '_', 'A', 'D', 'V', '_', '2', 'M'  // Complete Local Name
  };

  advert.start();
  advert.setPeriodicAdvertisingParams(0, &periodic_adv_params);
  advert.setPeriodicAdvertisingData(0, sizeof(periodic_adv_raw_data), &periodic_adv_raw_data[0]);
  advert.startPeriodicAdvertising(0);
}

void loop() {
  delay(2000);
}
#endif
