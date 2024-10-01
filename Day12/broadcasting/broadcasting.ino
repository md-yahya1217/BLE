#include <BLEDevice.h>
#include <BLEAdvertising.h>
#include <DHT.h>

#define DHTPIN 19       // Pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

// BLE advertising parameters
esp_ble_gap_ext_adv_params_t ext_adv_params = {
    .type = ESP_BLE_GAP_SET_EXT_ADV_PROP_NONCONN_NONSCANNABLE_UNDIRECTED,
    .interval_min = 0x40,  // 64 * 0.625 ms = 40 ms
    .interval_max = 0x40,  // 64 * 0.625 ms = 40 ms
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

// BLE periodic advertising parameters
esp_ble_gap_periodic_adv_params_t periodic_adv_params = {
    .interval_min = 0x320,  // 1000 ms interval
    .interval_max = 0x640,  // 2000 ms interval
    .properties = 0,  // No additional properties
};

uint8_t addr_2m[6] = {0xc0, 0xde, 0x52, 0x00, 0x00, 0x02};

// BLE Multi-advertising instance
BLEMultiAdvertising advert(1);  // max number of advertisement data

void setup() {
    Serial.begin(115200);
    Serial.println("Multi-Advertising with Sensor Data...");

    // Initialize DHT sensor
    dht.begin();

    // Initialize BLE
    BLEDevice::init("");

    // Set up the advertising parameters
    advert.setAdvertisingParams(0, &ext_adv_params);

    // Start the advertising
    advert.start();

    // Set up periodic advertising parameters
    advert.setPeriodicAdvertisingParams(0, &periodic_adv_params);
    advert.startPeriodicAdvertising(0);

    // Wait for BLE stack to settle
    delay(1000);
}

void loop() {
    // Read data from DHT11 sensor
    float temperature = dht.readTemperature();
    float humidity = dht.readHumidity();

    // Check if any reads failed
    if (isnan(temperature) || isnan(humidity)) {
        Serial.println("Failed to read from DHT sensor!");
        delay(5000);  // Retry after 5 seconds
        return;
    }

    // Create advertisement data payload with sensor values
    char advData[30];
    snprintf(advData, sizeof(advData), "T:%.1fC H:%.1f%%", temperature, humidity);

    // Create raw advertisement data
    uint8_t periodic_adv_raw_data[30];
    size_t data_len = strlen(advData);
    periodic_adv_raw_data[0] = data_len + 1;  // Length of the payload
    periodic_adv_raw_data[1] = 0x09;  // Type: Complete Local Name
    memcpy(&periodic_adv_raw_data[2], advData, data_len);

    // Update the advertising data
    advert.setPeriodicAdvertisingData(0, sizeof(periodic_adv_raw_data), periodic_adv_raw_data);

    // Log the advertised data
    Serial.print("Advertising data: ");
    Serial.println(advData);

    // Sleep for a while to optimize power consumption
    delay(10000);  // Adjust delay as needed
}
