//
// Created by matthijs on 14-6-18.
//

#ifndef ARGOS3_SIMULATOR_CI_ZEBRO_LOCALISATION_SENSOR_H
#define ARGOS3_SIMULATOR_CI_ZEBRO_LOCALISATION_SENSOR_H

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/utility/math/angles.h>
#include <argos3/core/utility/datatypes/byte_array.h>
#include <argos3/core/control_interface/ci_sensor.h>

namespace argos {

    /**
     * This class gives an interface for the sensor that is used for localisation of other robots in the vicinity.
     * Created by matthijs on 14-6-18.
     */
    class CCI_ZebroLocalisationSensor : public CCI_Sensor {

    public:

        struct SPacket {
            UInt16 Address;
            Real Range;
            CRadians HorizontalBearing;
            /**
             * The vertical bearing is defined as the angle between the local
             * robot XY plane and the message source position, i.e., the elevation
             * in math jargon. This is different from the inclination, which is the
             * angle between the azimuth vector (robot local Z axis) and
             * the vector to the message source. Elevation = 90 degrees - Inclination.
             */
            CRadians VerticalBearing;
            /** The heading relative to this robot. */
            CRadians RelativeHeading;

            SPacket();
        };

        typedef std::vector<SPacket> TReadings;

    public:

        virtual ~CCI_ZebroLocalisationSensor() {}

        const TReadings& GetReadings() const;

    #ifdef ARGOS_WITH_LUA
        virtual void CreateLuaState(lua_State* pt_lua_state);

        virtual void ReadingsToLuaState(lua_State* pt_lua_state);
    #endif

    protected:

        TReadings m_tReadings;

    };
}


#endif //ARGOS3_SIMULATOR_CI_ZEBRO_LOCALISATION_SENSOR_H
