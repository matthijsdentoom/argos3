#
# Zebro headers
#
# argos3/plugins/robots/zebro/control_interface
set(ARGOS3_HEADERS_PLUGINS_ROBOTS_ZEBRO_CONTROLINTERFACE
  # Add control interface headers here.
  control_interface/ci_zebro_proximity_sensor.h
  control_interface/ci_zebro_heading_sensor.h
  control_interface/ci_zebro_locomotion_actuator.h
  control_interface/ci_zebro_localisation_sensor.h
  control_interface/zebro_dimensions.h
)

# argos3/plugins/robots/zebro/simulator
if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_HEADERS_PLUGINS_ROBOTS_ZEBRO_SIMULATOR
    # Add simulation headers here.
    simulator/dynamics2d_zebro_model.h
    # simulator/physx_zebro_model.h
    simulator/zebro_entity.h
    simulator/zebro_heading_default_sensor.h
    simulator/zebro_proximity_default_sensor.h
    simulator/zebro_locomotion_actuator.h
    simulator/zebro_uwb_sensor.h)
endif(ARGOS_BUILD_FOR_SIMULATOR)

#
# Zebro sources
#
set(ARGOS3_SOURCES_PLUGINS_ROBOTS_ZEBRO
  ${ARGOS3_HEADERS_PLUGINS_ROBOTS_ZEBRO_CONTROLINTERFACE}
  # Add control interface cpp files here.
  control_interface/ci_zebro_heading_sensor.cpp
  control_interface/ci_zebro_proximity_sensor.cpp
  control_interface/ci_zebro_locomotion_actuator.cpp
  control_interface/ci_zebro_localisation_sensor.cpp
        )

if(ARGOS_BUILD_FOR_SIMULATOR)
  set(ARGOS3_SOURCES_PLUGINS_ROBOTS_ZEBRO
    ${ARGOS3_SOURCES_PLUGINS_ROBOTS_ZEBRO}
    ${ARGOS3_HEADERS_PLUGINS_ROBOTS_ZEBRO_SIMULATOR}
    # Add simulation cpp files here.
    simulator/dynamics2d_zebro_model.cpp
    # simulator/physx_zebro_model.cpp
    simulator/zebro_entity.cpp
    simulator/zebro_heading_default_sensor.cpp
    simulator/zebro_proximity_default_sensor.cpp
    simulator/zebro_locomotion_actuator.cpp
    simulator/zebro_uwb_sensor.cpp)
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

#
# Create a folder with the files required for cross-compilation of the ZebroRobot with the raspberry pi.
#
include(CreateCrossCompFolderForRpi.cmake)
