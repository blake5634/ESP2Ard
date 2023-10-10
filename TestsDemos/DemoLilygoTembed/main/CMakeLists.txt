idf_component_register(SRCS "tembed_main.c" "tembed_lvgl.c" "ESP2Ard.c"
                    INCLUDE_DIRS "")

target_compile_options(${COMPONENT_LIB} PRIVATE "-Wno-format")
