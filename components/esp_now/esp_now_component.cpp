#include "esp_now_component.h"

#if defined(USE_ESP32)
#include "esp_now.h"
#elif defined(USE_ESP8266)
#include "espnow.h"
#endif

#include "esphome/core/helpers.h"
#include "esphome/core/log.h"

namespace esphome {
namespace esp_now {

static const char *const TAG = "esp_now";

ESPNowComponent::ESPNowComponent() { global_esp_now = this; }

void ESPNowComponent::setup() {
  ESP_LOGCONFIG(TAG, "Setting up ESP-NOW...");
  esp_err_t err = esp_now_init();
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "esp_now_init failed: %s", esp_err_to_name(err));
    this->mark_failed();
    return;
  }

#ifdef USE_ESP8266
  err = esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "esp_now_set_self_role failed: %s", esp_err_to_name(err));
    this->mark_failed();
    return;
  }
#endif

  err = esp_now_register_recv_cb(ESPNowComponent::on_data_received);
  if (err != ESP_OK) {
    ESP_LOGE(TAG, "esp_now_register_recv_cb failed: %s", esp_err_to_name(err));
    this->mark_failed();
    return;
  }

  ESP_LOGCONFIG(TAG, "ESP-NOW setup complete");
}
void ESPNowComponent::loop() {
  if (receive_queue_.empty())
    return;
  while (true) {
    std::unique_ptr<ESPNowPacket> packet = std::move(this->receive_queue_.front());
    this->receive_queue_.pop();
#if ESPHOME_VERSION >= VERSION_CODE(2022,1,0)
    ESP_LOGD(TAG, "mac: %s, data: %s", format_hex_pretty(packet->get_bssid().data(), 6).c_str(),
             format_hex_pretty(packet->get_data()).c_str());
#else
    ESP_LOGD(TAG, "mac: %s, data: %s", hexencode(packet->get_bssid().data(), 6).c_str(),
             hexencode(packet->get_data()).c_str());
#endif
    for (auto *listener : this->listeners_)
      listener->on_esp_now_message(*packet);

    this->on_packet_->trigger(*packet);
    if (this->receive_queue_.empty())
      break;
  }
}
void ESPNowComponent::dump_config() { ESP_LOGCONFIG(TAG, "esp_now:"); }

void ESPNowComponent::on_data_received(const uint8_t *bssid, const uint8_t *data, int len) {
  auto packet = make_unique<ESPNowPacket>(bssid, data, len);
  global_esp_now->receive_queue_.push(std::move(packet));
}

ESPNowComponent *global_esp_now = nullptr;

}  // namespace esp_now
}  // namespace esphome
