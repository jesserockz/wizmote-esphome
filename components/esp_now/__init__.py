import esphome.config_validation as cv

CONFIG_SCHEMA = cv.invalid(
    "This external `esp_now` component has been replaced by `espnow` released in ESPHome 2025.8.0"
)
