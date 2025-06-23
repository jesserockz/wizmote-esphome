#pragma once

#include "esphome/components/esp_now/esp_now_component.h"
#include "esphome/core/automation.h"
#include "esphome/core/helpers.h"

#include <map>

namespace esphome {
namespace wizmote {

typedef uint64_t MACAddress;

typedef struct __attribute__((__packed__)) WizMotePacket {
  union {
    uint8_t bssid[6];
    MACAddress mac_address;
  };

  union {
    uint8_t program;    // 0x91 for ON button, 0x81 for all others
    struct __attribute__((__packed__)) {
      unsigned :4;
      unsigned pairing:1;
      unsigned :3;
    };
  };
  uint32_t sequence;    // Incremental sequence number 32 bit unsigned integer LE
  uint8_t dType1 = 32;  // Data type: button (32)
  uint8_t button;       // Identifies which button is being pressed
  uint8_t dType2 = 1;   // Data type: batteryLevel (1)
  uint8_t batteryLevel; // WiZMote batteryLevel out of 100
  uint8_t mac[4];       // CCM MAC (Message Authentication Code)

  static inline WizMotePacket build(esp_now::ESPNowPacket espnow_packet) {
    WizMotePacket packet;
    memcpy(packet.bssid, espnow_packet.get_bssid().data(), 6);
    memcpy(&packet.program, espnow_packet.get_data().data(), espnow_packet.get_data().size());
    return packet;
  }
} WizMotePacket;

class WizMoteListener : public esp_now::ESPNowListener {
 public:
  void on_esp_now_message(esp_now::ESPNowPacket packet) override;

  Trigger<WizMotePacket> *get_on_button_trigger() { return this->on_button_; }

 protected:
  std::map<MACAddress, uint32_t> last_sequence_;
  Trigger<WizMotePacket> *on_button_ = new Trigger<WizMotePacket>();
};

}  // namespace wizmote
}  // namespace esphome
