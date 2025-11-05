#include "wizmote.h"

#include "esphome/core/log.h"

namespace esphome::wizmote {

static const char *const TAG = "wizmote";

WizMotePacket WizMotePacket::build(const espnow::ESPNowRecvInfo &info, const uint8_t *data, uint8_t size) {
  WizMotePacket packet;
  memcpy(packet.bssid, info.src_addr, ESP_NOW_ETH_ALEN);
  memcpy(&packet.program, data, size);
  return packet;
}

void WizMoteListener::setup() { espnow::global_esp_now->register_broadcasted_handler(this); }

bool WizMoteListener::on_broadcasted(const espnow::ESPNowRecvInfo &info, const uint8_t *data, uint8_t size) {
  WizMotePacket wizmote = WizMotePacket::build(info, data, size);

  ESP_LOGV(TAG, "Received from %s - %s", format_mac_address_pretty(wizmote.bssid).c_str(),
           format_hex_pretty(data, size).c_str());

  if (wizmote.program != 0x91 && wizmote.program != 0x81) {
    ESP_LOGD(TAG, "Ignoring packet with program %02X", wizmote.program);
    return false;
  }

  if (this->last_sequence_[wizmote.mac_address] == wizmote.sequence) {
    return true;
  }
  this->last_sequence_[wizmote.mac_address] = wizmote.sequence;

  this->on_button_->trigger(wizmote);
  return true;
}

}  // namespace esphome::wizmote
