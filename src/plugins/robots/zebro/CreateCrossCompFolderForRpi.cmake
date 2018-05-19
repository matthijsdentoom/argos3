# This file Creates a seperate folder with all the header and class files that are required for cross-compilation
# by the raspberry pi, to run the zebro controll algorithms.

# Set the folder where the source files need to be put.
set(RPI_SOURCE_FOLDER ${CMAKE_SOURCE_DIR}/${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/rpi-sources/argos3)

# Create the folder if it does not exist yet.
file(MAKE_DIRECTORY ${RPI_SOURCE_FOLDER})

# Copy the datatype files.
file(COPY ${CMAKE_SOURCE_DIR}/core/utility/datatypes/datatypes.h
        ${CMAKE_SOURCE_DIR}/core/utility/datatypes/byte_array.h
        ${CMAKE_SOURCE_DIR}/core/utility/datatypes/byte_array.cpp
        DESTINATION
        ${RPI_SOURCE_FOLDER}/core/utility/datatypes)

# Copy the required control interface files for the zebro
set(ZEBRO_INTERFACES_PATH /plugins/robots/zebro/control_interface)
file(GLOB ZEBRO_INTERFACE_SOURCES ${CMAKE_SOURCE_DIR}/${ZEBRO_INTERFACES_PATH}/*.cpp)
file(GLOB ZEBRO_INTERFACE_HEADERS ${CMAKE_SOURCE_DIR}/${ZEBRO_INTERFACES_PATH}/*.h)
file(COPY ${ZEBRO_INTERFACE_SOURCES} ${ZEBRO_INTERFACE_HEADERS} DESTINATION ${RPI_SOURCE_FOLDER}/${ZEBRO_INTERFACES_PATH})

message("${ZEBRO_INTERFACE_SOURCES}")

# Write dummy top-level interfaces.
file(WRITE ${RPI_SOURCE_FOLDER}/core/control_interface/ci_actuator.h
        "#pragma once\nclass CCI_Actuator { public: virtual ~CCI_Actuator() {} };")
file(WRITE ${RPI_SOURCE_FOLDER}/core/control_interface/ci_sensor.h
        "#pragma once\nclass CCI_Sensor { public: virtual ~CCI_Sensor() = default; \n virtual void Update() = 0; }; ")

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

# Copy the argos logging functionalities.
set(LOGGING_DIRECTORY core/utility/logging)
file(GLOB LOGGING_FILES ${CMAKE_SOURCE_DIR}/${LOGGING_DIRECTORY}/*)
file(COPY
        ${LOGGING_FILES}
        DESTINATION
        ${RPI_SOURCE_FOLDER}/${LOGGING_DIRECTORY})

# Copy the range an bearing sensor interface.
set(GENERAL_SENSOR_DIRECTORY plugins/robots/generic/control_interface)
file(COPY
        ${CMAKE_SOURCE_DIR}/${GENERAL_SENSOR_DIRECTORY}/ci_range_and_bearing_actuator.cpp
        ${CMAKE_SOURCE_DIR}/${GENERAL_SENSOR_DIRECTORY}/ci_range_and_bearing_actuator.h
        ${CMAKE_SOURCE_DIR}/${GENERAL_SENSOR_DIRECTORY}/ci_range_and_bearing_sensor.cpp
        ${CMAKE_SOURCE_DIR}/${GENERAL_SENSOR_DIRECTORY}/ci_range_and_bearing_sensor.h
        DESTINATION
        ${RPI_SOURCE_FOLDER}/${GENERAL_SENSOR_DIRECTORY})

# Create an empty config file, since it has to be included.
file(WRITE ${RPI_SOURCE_FOLDER}/core/config.h "")
