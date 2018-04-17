/**
 * @file <argos3/plugins/robots/zebro/control_interface/ci_zebro_proximity_sensor.h>
 *
 * @brief This file provides the definition of the zebro proximity sensor.
 *
 * This file provides the definition of the zebro proximity sensor.
 * The sensors are spaced in an area in front of the robot.
 *
 * @author Matthijs den Toom <thijsco4@gmail.com>
 */

#ifndef CCI_ZEBRO_PROXIMITY_SENSOR_H
#define CCI_ZEBRO_PROXIMITY_SENSOR_H

namespace argos {
   class CCI_ZebroProximitySensor;
}

#include <argos3/core/control_interface/ci_sensor.h>
#include <argos3/core/utility/math/angles.h>

namespace argos {

   class CCI_ZebroProximitySensor : public CCI_Sensor {

   public:

      struct SReading {
         Real Value;
         CRadians Angle;

         SReading() :
            Value(0.0f) {}

         SReading(Real f_value,
                  const CRadians& c_angle) :
            Value(f_value),
            Angle(c_angle) {}
      };

      typedef std::vector<SReading> TReadings;

   public:

      /**
       * Class constructor
       */
      CCI_ZebroProximitySensor();

      /**
       * Class destructor
       */
      virtual ~CCI_ZebroProximitySensor() {}

      /**
       * Returns the readings of this sensor
       */
      const TReadings& GetReadings() const;

#ifdef ARGOS_WITH_LUA
      virtual void CreateLuaState(lua_State* pt_lua_state);

      virtual void ReadingsToLuaState(lua_State* pt_lua_state);
#endif

   protected:

      TReadings m_tReadings;

   };

   std::ostream& operator<<(std::ostream& c_os, const CCI_ZebroProximitySensor::SReading& s_reading);
   std::ostream& operator<<(std::ostream& c_os, const CCI_ZebroProximitySensor::TReadings& t_readings);

}

#endif
