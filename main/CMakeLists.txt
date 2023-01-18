idf_component_register(
    SRCS
        "./src/espi2s.cpp"
        "./src/espdelay.cpp"
        "./src/main.cpp"
    INCLUDE_DIRS
        "."
        "./src"
        "./inc"
    REQUIRES
        freertos
        esp_common
        esp_system
        esp_timer
        driver
        soc
        nvs_flash
        log
)

target_compile_options(${COMPONENT_LIB}
    PRIVATE
        -Wall
        -Werror
        -fexceptions
)

target_compile_definitions(${COMPONENT_LIB}
    PRIVATE
        _DEBUG=1
)

target_include_directories(${COMPONENT_LIB}
    PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/inc
        ${CMAKE_CURRENT_SOURCE_DIR}/lib/DigitalFilters
)

target_link_libraries(${COMPONENT_LIB}
    PRIVATE
)
