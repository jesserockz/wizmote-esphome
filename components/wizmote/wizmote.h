#pragma once

#include "esphome/components/espnow/espnow_component.h"
#include "esphome/core/automation.h"
#include "esphome/core/component.h"
#include "esphome/core/helpers.h"

#include <map>

namespace esphome::wizmote {

typedef uint64_t MACAddress;

typedef struct __attribute__((__packed__)) WizMotePacket {
  union {
    uint8_t bssid[6];
    MACAddress mac_address;
  };

  union {
    uint8_t program;  // 0x91 for ON button, 0x81 for all others
    struct __attribute__((__packed__)) {
      unsigned : 4;
      unsigned pairing : 1;
      unsigned : 3;
    };
  };
  uint32_t sequence;     // Incremental sequence number 32 bit unsigned integer LE
  uint8_t dType1 = 32;   // Data type: button (32)
  uint8_t button;        // Identifies which button is being pressed
  uint8_t dType2 = 1;    // Data type: batteryLevel (1)
  uint8_t batteryLevel;  // WiZMote batteryLevel out of 100
  uint8_t mac[4];        // CCM MAC (Message Authentication Code)

  static inline WizMotePacket build(const espnow::ESPNowRecvInfo &info, const uint8_t *data, uint8_t size);
} WizMotePacket;

class WizMoteListener : public Component, public espnow::ESPNowBroadcastedHandler {
 public:
  void setup() override;
  bool on_broadcasted(const espnow::ESPNowRecvInfo &info, const uint8_t *data, uint8_t size) override;

  Trigger<WizMotePacket> *get_on_button_trigger() { return this->on_button_; }

 protected:
  std::map<MACAddress, uint32_t> last_sequence_;
  Trigger<WizMotePacket> *on_button_ = new Trigger<WizMotePacket>();
};

}  // namespace esphome::wizmote
