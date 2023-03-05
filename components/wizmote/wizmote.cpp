#include "wizmote.h"

namespace esphome {
namespace wizmote {
void WizMoteListener::on_esp_now_message(esp_now::ESPNowPacket packet) {
  WizMotePacket wizmote = WizMotePacket::build(packet);
  if (wizmote.sequence <= this->last_sequence_ && wizmote.bssid == this->last_bssid_)
    return;
  
  this->last_sequence_ = wizmote.sequence;

  for(i=0;i<6;i++)
  {
    this->last_bssid_[i]=wizmote.bssid[i];
  }
  
  this->on_button_->trigger(wizmote);
}
}  // namespace wizmote
}  // namespace esphome
