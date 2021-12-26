from esphome import automation
from esphome.components import esp_now
import esphome.config_validation as cv
import esphome.codegen as cg
from esphome.const import CONF_ID

CODEOWNERS = ["@jesserockz"]
DEPENDENCIES = ["esp_now"]

wizmote_ns = cg.esphome_ns.namespace("wizmote")
WizMoteListener = wizmote_ns.class_("WizMoteListener")
WizMotePacket = wizmote_ns.class_("WizMotePacket")

CONF_ON_BUTTON = "on_button"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(WizMoteListener),
        cv.GenerateID(esp_now.CONF_ESP_NOW): cv.use_id(esp_now.ESPNowComponent),
        cv.Optional(CONF_ON_BUTTON): automation.validate_automation(single=True),
    }
)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])

    esp_now_ = await cg.get_variable(config[esp_now.CONF_ESP_NOW])
    cg.add(esp_now_.register_listener(var))

    if CONF_ON_BUTTON in config:
        await automation.build_automation(
            var.get_on_button_trigger(),
            [(WizMotePacket, "data")],
            config[CONF_ON_BUTTON],
        )
