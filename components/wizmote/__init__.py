import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import automation
from esphome.components import espnow
from esphome.const import CONF_ID

CODEOWNERS = ["@jesserockz"]
DEPENDENCIES = ["espnow"]

wizmote_ns = cg.esphome_ns.namespace("wizmote")
WizMoteListener = wizmote_ns.class_("WizMoteListener", cg.Component)
WizMotePacket = wizmote_ns.class_("WizMotePacket")

CONF_ON_BUTTON = "on_button"

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(WizMoteListener),
        cv.Optional(CONF_ON_BUTTON): automation.validate_automation(single=True),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)

    if CONF_ON_BUTTON in config:
        await automation.build_automation(
            var.get_on_button_trigger(),
            [(WizMotePacket, "data")],
            config[CONF_ON_BUTTON],
        )
