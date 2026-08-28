import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.const import CONF_ID

CODEOWNERS = ["@Marcobedendo78"]
DEPENDENCIES = ["esp32"]

waveshare_sd_ns = cg.esphome_ns.namespace("waveshare_sd")
WaveshareSD = waveshare_sd_ns.class_("WaveshareSD", cg.Component)

CONFIG_SCHEMA = cv.Schema(
    {
        cv.GenerateID(): cv.declare_id(WaveshareSD),
    }
).extend(cv.COMPONENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
