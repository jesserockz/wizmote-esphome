#pragma once

#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"

#include <array>
#include <memory>
#include <queue>
#include <vector>

namespace esphome {
namespace esp_now {

class ESPNowPacket {
 public:
  ESPNowPacket(const uint8_t *bssid, const uint8_t *data, int len) {
    std::copy_n(bssid, this->bssid_.size(), this->bssid_.begin());
    this->data_.resize(len);
    std::copy_n(data, len, this->data_.begin());
  }

  std::array<uint8_t, 6> get_bssid() { return bssid_; }
  std::vector<uint8_t> get_data() { return data_; }

 protected:
  std::array<uint8_t, 6> bssid_;
  std::vector<uint8_t> data_;
};

class ESPNowListener {
 public:
  virtual void on_esp_now_message(ESPNowPacket packet) = 0;
};

class ESPNowComponent : public Component {
 public:
  ESPNowComponent();
  void setup() override;
  void loop() override;
  void dump_config() override;
  float get_setup_priority() const override { return setup_priority::AFTER_WIFI; }

  static void on_data_received(const uint8_t *bssid, const uint8_t *data, int len);

  Trigger<ESPNowPacket> *get_on_packet_trigger() { return this->on_packet_; }

  void register_listener(ESPNowListener *listener) { this->listeners_.push_back(listener); }

 protected:
  std::queue<std::unique_ptr<ESPNowPacket>> receive_queue_;
  Trigger<ESPNowPacket> *on_packet_ = new Trigger<ESPNowPacket>();
  std::vector<ESPNowListener *> listeners_;
};

extern ESPNowComponent *global_esp_now;

}  // namespace esp_now
}  // namespace esphome
