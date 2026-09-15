idf_component_register(
    SRCS
        "main.c"
        "drv.c"
    INCLUDE_DIRS "."
    REQUIRES
        esp_driver_gpio
        esp_driver_ledc
)