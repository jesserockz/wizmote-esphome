#include "wizmote.h"

namespace esphome {
namespace wizmote {
void WizMoteListener::on_esp_now_message(esp_now::ESPNowPacket packet) {
  WizMotePacket wizmote = WizMotePacket::build(packet);

  if (this->last_sequence_[wizmote.mac_address] == wizmote.sequence) {
    return;
  }
  this->last_sequence_[wizmote.mac_address] = wizmote.sequence;

  this->on_button_->trigger(wizmote);
}

}  // namespace wizmote
}  // namespace esphome
