if(PICO_BOARD STREQUAL "pico")
  add_executable(ampel ampel.c)
  # pull in common dependencies
  target_link_libraries(ampel pico_stdlib)
elseif(PICO_BOARD STREQUAL "pico_w")
  add_executable(blink picow_blink.c)
  # pull in common dependencies
  target_link_libraries(blink pico_stdlib pico_cyw43_arch_none)
endif()

# create map/bin/hex file etc.
pico_add_extra_outputs(ampel)

# add url via pico_set_program_url
example_auto_set_url(ampel)
