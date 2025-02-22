idf_component_register(
    SRCS "cb_wifi.c" 
    INCLUDE_DIRS "."
    PRIV_REQUIRES esp_event esp_wifi
)
