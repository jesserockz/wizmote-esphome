#include "wizmote.h"
#include "esphome/core/log.h"

namespace esphome {
namespace wizmote {

void WizMoteListener::on_esp_now_message(esp_now::ESPNowPacket packet) {
  // Drop oversized packets (like WiZ SpaceSense/status payloads) 
  // to prevent heap memory corruption and CPU watchdog panics. 
  // Standard WiZmote packets are 13 bytes.
  if (packet.get_data().size() > 20) {
    return; 
  }

  WizMotePacket wizmote = WizMotePacket::build(packet);

  // --- OPTIMIZED LOGGING ---
  // This executes only for legitimate, small packets that pass the gate.
  // It prints the parsed data directly, bypassing the muted esp_now debug spray.
  ESP_LOGI("wizmote", "Legitimate Packet Intercepted -> Button: %d | Sequence: %d", wizmote.button, wizmote.sequence);

  if (this->last_sequence_[wizmote.mac_address] == wizmote.sequence) {
    return;
  }
  this->last_sequence_[wizmote.mac_address] = wizmote.sequence;

  this->on_button_->trigger(wizmote);
}

}  // namespace wizmote
}  // namespace esphome