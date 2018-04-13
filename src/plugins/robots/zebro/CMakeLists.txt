#
# E-puck headers
#
# argos3/plugins/robots/zebro/simulator
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_ZEBRO_SIMULATOR
    simulator/dynamics2d_zebro_model.h
    # simulator/physx_zebro_model.h
    simulator/zebro_entity.h)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# E-puck sources
#
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_ZEBRO
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_ZEBRO}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_ZEBRO_SIMULATOR}
    simulator/dynamics2d_zebro_model.cpp
    # simulator/physx_zebro_model.cpp
    simulator/zebro_entity.cpp)
  # Compile the graphical visualization only if the necessary libraries have been found
  if(ARGOS_COMPILE_QTOPENGL)
    set(ARGOS3_HEADERS_PLUGINS_ROBOTS_ZEBRO_SIMULATOR
      ${ARGOS3_HEADERS_PLUGINS_ROBOTS_ZEBRO_SIMULATOR}
      simulator/qtopengl_zebro.h)
    set(ARGOS3_SOURCES_PLUGINS_ROBOTS_ZEBRO
      ${ARGOS3_SOURCES_PLUGINS_ROBOTS_ZEBRO}
      simulator/qtopengl_zebro.h
      simulator/qtopengl_zebro.cpp)
  endif(ARGOS_COMPILE_QTOPENGL)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Create zebro plugin
#
add_library(argos3plugin_${ARGOS_BUILD_FOR}_zebro SHARED ${ARGOS3_SOURCES_PLUGINS_ROBOTS_ZEBRO})
target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_zebro
  argos3plugin_${ARGOS_BUILD_FOR}_genericrobot
  argos3plugin_${ARGOS_BUILD_FOR}_dynamics2d)
  # argos3plugin_${ARGOS_BUILD_FOR}_physx)
if(ARGOS_BUILD_FOR_SIMULATOR AND ARGOS_COMPILE_QTOPENGL)
  target_link_libraries(argos3plugin_${ARGOS_BUILD_FOR}_zebro argos3plugin_${ARGOS_BUILD_FOR}_qtopengl)
endif(ARGOS_BUILD_FOR_SIMULATOR AND ARGOS_COMPILE_QTOPENGL)

install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_ZEBRO_CONTROLINTERFACE} DESTINATION include/argos3/plugins/robots/zebro/control_interface)

if(ARGOS_BUILD_FOR_SIMULATOR)
  install(FILES ${ARGOS3_HEADERS_PLUGINS_ROBOTS_ZEBRO_SIMULATOR}      DESTINATION include/argos3/plugins/robots/zebro/simulator)
endif(ARGOS_BUILD_FOR_SIMULATOR)

install(TARGETS argos3plugin_${ARGOS_BUILD_FOR}_zebro
  RUNTIME DESTINATION bin
  LIBRARY DESTINATION lib/argos3
  ARCHIVE DESTINATION lib/argos3)