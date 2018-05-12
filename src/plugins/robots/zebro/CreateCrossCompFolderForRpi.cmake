# This file Creates a seperate folder with all the header and class files that are required for cross-compilation
# by the raspberry pi, to run the zebro controll algorithms.

# Set the folder where the source files need to be put.
set(RPI_SOURCE_FOLDER ${CMAKE_SOURCE_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rpi-sources/argos3)

# Create the folder if it does not exist yet.
file(MAKE_DIRECTORY ${RPI_SOURCE_FOLDER})

# Copy the datatype files.
file(COPY ${CMAKE_SOURCE_DIR}/core/utility/datatypes/datatypes.h DESTINATION ${RPI_SOURCE_FOLDER}/core/utility/datatypes)

# Copy the required control interface files for the zebro
set(ZEBRO_INTERFACES_PATH /plugins/robots/zebro/control_interface)
file(GLOB ZEBRO_INTERFACE_SOURCES ${CMAKE_SOURCE_DIR}/${ZEBRO_INTERFACES_PATH}/*.cpp)
file(GLOB ZEBRO_INTERFACE_HEADERS ${CMAKE_SOURCE_DIR}/${ZEBRO_INTERFACES_PATH}/*.h)
file(COPY ${ZEBRO_INTERFACE_SOURCES} ${ZEBRO_INTERFACE_HEADERS} DESTINATION ${RPI_SOURCE_FOLDER}/${ZEBRO_INTERFACES_PATH})

message("${ZEBRO_INTERFACE_SOURCES}")

# Write dummy top-level interfaces.
file(WRITE ${RPI_SOURCE_FOLDER}/core/control_interface/ci_actuator.h
        "class CCI_Actuator {};")
file(WRITE ${RPI_SOURCE_FOLDER}/core/control_interface/ci_sensor.h
        "class CCI_Sensor {};")

# Copy angle ability
set(MATH_DIRECTORY core/utility/math)
file(COPY
        ${CMAKE_SOURCE_DIR}/${MATH_DIRECTORY}/angles.h
        ${CMAKE_SOURCE_DIR}/${MATH_DIRECTORY}/angles.cpp
        ${CMAKE_SOURCE_DIR}/${MATH_DIRECTORY}/range.h
        ${CMAKE_SOURCE_DIR}/${MATH_DIRECTORY}/general.h
        DESTINATION
        ${RPI_SOURCE_FOLDER}/${MATH_DIRECTORY}/
        )

# Copy the string utilities
file(COPY
        ${CMAKE_SOURCE_DIR}/core/utility/string_utilities.h
        ${CMAKE_SOURCE_DIR}/core/utility/string_utilities.cpp

        DESTINATION
        ${RPI_SOURCE_FOLDER}/core/utility/
        )

# Copy the argos exception
file(COPY
        ${CMAKE_SOURCE_DIR}/core/utility/configuration/argos_exception.h
        DESTINATION
        ${RPI_SOURCE_FOLDER}/core/utility/configuration/
        )

# Create an empty config file, since it has to be included.
file(WRITE ${RPI_SOURCE_FOLDER}/core/config.h "")
