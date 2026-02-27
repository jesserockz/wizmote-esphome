#include "wizmote.h"

namespace esphome {
namespace wizmote {

void WizMoteListener::on_esp_now_message(esp_now::ESPNowPacket packet) {
  // Drop oversized packets (like WiZ SpaceSense/status payloads) 
  // to prevent heap memory corruption. Standard WiZmote packets are 13 bytes.
  if (packet.get_data().size() > 20) {
    return; 
  }

  WizMotePacket wizmote = WizMotePacket::build(packet);

  if (this->last_sequence_[wizmote.mac_address] == wizmote.sequence) {
    return;
  }
  this->last_sequence_[wizmote.mac_address] = wizmote.sequence;

  this->on_button_->trigger(wizmote);
}

}  // namespace wizmote
}  // namespace esphome