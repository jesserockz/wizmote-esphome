# ESPHome WizMote Receiver

A pair of simple ESPHome components to receive button presses from the WizMote.

## What is the WizMote?

The WizMote is a Wi-Fi enabled remote controller from Wiz that uses ESP-NOW to control their own branded light bulbs and switches etc.

## What does this repo do?

This repo will allow you to use an ESP running ESPHome to "intercept" the WizMote button presses, and forward them to Home Assistant, or act on them directly within ESPHome.

This repo also contains some starting blueprints to get you started. See the below blueprint section.

### ESPHome configuration snippet
Insert this code after your Core Configuration:

```yaml
external_components:
  - source: github://jesserockz/wizmote-esphome
    components:
      - esp_now
      - wizmote

esp_now:

wizmote:
  on_button:
    if:
      condition:
        lambda: return data.button >= 16 && data.button <= 19;
      then:
        - homeassistant.event:
            event: esphome.wizmote_choose
            data:
              mac: !lambda 'return format_hex(data.bssid, 6);'
              button: !lambda 'return data.button - 15;'
              sequence: !lambda 'return data.sequence;'
      else:
        - homeassistant.event:
            event: esphome.wizmote_action
            data:
              mac: !lambda 'return format_hex(data.bssid, 6);'
              button: !lambda 'return data.button;'
              sequence: !lambda 'return data.sequence;'

```

If you are using an ESP32, wifi power save can interfere with reliable ESP-NOW reception. You can disable it with:

```yaml
wifi:
  power_save_mode: none

```

## Blueprints

This repo provides some blueprints to get you started with the remote. The blueprint is used to do actions based on the order of buttons pressed.

**wizmote-esphome-blueprint.yaml**: Basic blueprint which takes 4 lights entities

[![Open your Home Assistant instance and show the blueprint import dialog with a specific blueprint pre-filled.](https://my.home-assistant.io/badges/blueprint_import.svg)](https://my.home-assistant.io/redirect/blueprint_import/?blueprint_url=https%3A%2F%2Fgithub.com%2Fjesserockz%2Fwizmote-esphome%2Fblob%2Fmain%2Fwizmote-esphome-blueprint.yaml)



**wizmote-esphome-blueprint-flexible.yaml**
  A more flexible version of the ESPHome Wizmote blueprint that adds many features.
  Key improvements include:
- **Generic Device Slots:** Buttons 1-4 can control any combination of lights, switches, or fans.
- **Optional Devices:** Not all device buttons need to be assigned.
- **Fan High/Low Speed Control:** The up/down buttons can be configured to set fan speeds to a low and high percentage.
- **Configurable Timeouts:** All delays and transition times are customizable through the blueprint UI.

[![Open your Home Assistant instance and show the blueprint import dialog with a specific blueprint pre-filled.](https://my.home-assistant.io/badges/blueprint_import.svg)](https://my.home-assistant.io/redirect/blueprint_import/?blueprint_url=https%3A%2F%2Fgithub.com%2Fjesserockz%2Fwizmote-esphome%2Fblob%2Fmain%2Fwizmote-esphome-blueprint-flexible.yaml)

**wizmote-esphome-blueprint-default-behavior.yaml**
  This is a helper blueprint to go along with any of the other blueprints. The other blueprints all require that a user first press a "device" button (1-4) before it will do anything. This blueprint handles the case (if desired) that the user has not selected a device, or the device-specific automation has timed out. For example, if you have your remote set up so that it times out in 20 seconds, that means that once a user selects a device to turn on or off, once they stop pressing buttons for 20 seconds, the automation will time out and any subsequent on/off/up/down button presses will be ignored. This is most useful when the remote is put down and should be considered a "fresh start". If a user presses the on button, perhaps they expect some generic action to take place, such as turn on a default light. This blueprint provides that capability. When the On/Off/Moon button are pressed, it will check to see if the other wizmote blueprint automation is running or timed out. If it's timed out, then this automation will run based on the described actions. It is also possible always support the moon button behavior or only when timed out.
- **Default Actions:** Pressing ON or OFF without first selecting a device will control a default list of entities.
- **Moon Button:** The moon button turns off a configurable list of entities, with a separate, customizable fade-out time for lights. This has an additional toggle to support always being enabled or only when the other automation is timed out.

[![Open your Home Assistant instance and show the blueprint import dialog with a specific blueprint pre-filled.](https://my.home-assistant.io/badges/blueprint_import.svg)](https://my.home-assistant.io/redirect/blueprint_import/?blueprint_url=https%3A%2F%2Fgithub.com%2Fjesserockz%2Fwizmote-esphome%2Fblob%2Fmain%2Fwizmote-esphome-blueprint-default-behavior.yaml)
