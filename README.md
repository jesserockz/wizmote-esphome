# ESPHome WizMote Receiver

A pair of simple ESPHome components to receive button presses from the WizMote.

## What is the WizMote?

The WizMote is a Wi-Fi enabled remote controller from Wiz that uses ESP-NOW to control their own branded light bulbs and switches etc.

## What does this repo do?

This repo will allow you to use an ESP running ESPHome to "intercept" the WizMote button presses, and forward them to Home Assistant, or act on them directly within ESPHome.

This repo also contains a blueprint for a Home Assistant automation which allows setting up any 4 lights in Home Assistant to be controlled by the WizMote.
There are 2 versions of the blueprint offered.
- **wizmote-esphome-blueprint.yaml**: Basic blueprint which takes 4 lights entities.
- **wizmote-esphome-blueprint-flexible.yaml**: Adds support for fans, and switches in addition to lights. Supports "default" entities to turn on or off if a channel/device has not been selected. Moon button acts like a "lights off" button, where you can select which entities (lights or switches) to turn off, and for light entities, you can even provide a transition time for the turn off. In addition to this, If a fan with percentage support is added to a channel, the - and + buttons act like low and high for setting fan speed to specified percentages.

[![Open your Home Assistant instance and show the blueprint import dialog with a specific blueprint pre-filled.](https://my.home-assistant.io/badges/blueprint_import.svg)](https://my.home-assistant.io/redirect/blueprint_import/?blueprint_url=https%3A%2F%2Fgithub.com%2Fjesserockz%2Fwizmote-esphome%2Fblob%2Fmain%2Fwizmote-esphome-blueprint.yaml) - wizmode-esphome-blueprint.yaml

[![Open your Home Assistant instance and show the blueprint import dialog with a specific blueprint pre-filled.](https://my.home-assistant.io/badges/blueprint_import.svg)](https://my.home-assistant.io/redirect/blueprint_import/?blueprint_url=https%3A%2F%2Fgithub.com%2Fjesserockz%2Fwizmote-esphome%2Fblob%2Fmain%2Fwizmote-esphome-blueprint-flexible.yaml) - wizmode-esphome-blueprint-flexible.yaml

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
