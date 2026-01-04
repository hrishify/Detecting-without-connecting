#include "esp_wifi.h"
#include "esp_system.h"
#include "esp_event.h"
#include "nvs_flash.h"

uint32_t packetCount= 0;

void sniffer(void* buf, wifi_promiscuous_pkt_type_t type) {
  if (type != WIFI_PKT_MGMT) return;
  packetCount++;
}

void setup() {
  Serial.begin(115200);
  delay(2000);

  nvs_flash_init();
  esp_netif_init();
  esp_event_loop_create_default();

  wifi_init_config_t cfg = WIFI_INIT_CONFIG_DEFAULT();
  esp_wifi_init(&cfg);

  esp_wifi_set_storage(WIFI_STORAGE_RAM);
  esp_wifi_set_mode(WIFI_MODE_NULL);
  esp_wifi_start();

  esp_wifi_set_promiscuous(true);
  esp_wifi_set_promiscuous_rx_cb(&sniffer);

  esp_wifi_set_channel(1, WIFI_SECOND_CHAN_NONE);

  Serial.println("ESP32 Passive Wi-Fi Sniffer Started");
}

void loop() {
  packetCount = 0;
  delay(1000);

  Serial.print("Wi-Fi packets detected: ");
  Serial.println(packetCount);
}
