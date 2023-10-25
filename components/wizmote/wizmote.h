#pragma once

#include "esphome/components/esp_now/esp_now_component.h"
#include "esphome/core/automation.h"
#include "esphome/core/helpers.h"

#include <vector>

namespace esphome {
namespace wizmote {

static const uint8_t WIZMOTEHISTORYSIZE = 20;

typedef struct __attribute__((__packed__)) WizMotePacket {
  uint8_t bssid[6];

  uint8_t program;      // 0x91 for ON button, 0x81 for all others
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
  
typedef struct WizMoteHistory {
  uint8_t bssid[6];
  uint32_t sequence;
} WizMoteHistory;

class WizMoteListener : public esp_now::ESPNowListener {
 public:
  void on_esp_now_message(esp_now::ESPNowPacket packet) override;

  Trigger<WizMotePacket> *get_on_button_trigger() { return this->on_button_; }

 protected:
  bool update_wizmote_history(WizMotePacket *packet);
  bool find_bssid_index(WizMotePacket *packet, uint8_t *index);
  bool is_bssid_equal(WizMotePacket *packet, WizMoteHistory *history);
  WizMoteHistory history[WIZMOTEHISTORYSIZE] = {0,};
  Trigger<WizMotePacket> *on_button_ = new Trigger<WizMotePacket>();
};
  

}  // namespace wizmote
}  // namespace esphome
