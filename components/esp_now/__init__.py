from esphome import automation
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID

CODEOWNERS = ["@jesserockz"]
DEPENDENCIES = ["wifi"]

esp_now_ns = cg.esphome_ns.namespace("esp_now")
ESPNowComponent = esp_now_ns.class_("ESPNowComponent", cg.Component)

ESPNowPacket = esp_now_ns.class_("ESPNowPacket")

CONF_ESP_NOW = "esp_now"
CONF_ON_PACKET = "on_packet"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(ESPNowComponent),
        cv.Optional(CONF_ON_PACKET): automation.validate_automation(single=True),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    cg.add_define("USE_ESP_NOW")

    if CONF_ON_PACKET in config:
        await automation.build_automation(
            var.get_on_packet_trigger(),
            [(ESPNowPacket, "data")],
            config[CONF_ON_PACKET],
        )
