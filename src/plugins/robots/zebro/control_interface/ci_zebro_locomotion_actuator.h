/**
 * @file <argos3/plugins/robots/zebro/control_interface/ci_zebro_locomotion_actuator.h>
 *
 * @author Matthijs den Toom <thijsco4@gmail.com>
 */

#ifndef ARGOS3_SIMULATOR_CI_ZEBRO_LOCOMOTION_ACTUATOR_H
#define ARGOS3_SIMULATOR_CI_ZEBRO_LOCOMOTION_ACTUATOR_H

namespace argos
{
    class CCI_ZebroLocomotionActuator;
}

#include <argos3/core/utility/datatypes/datatypes.h>
#include <argos3/core/control_interface/ci_actuator.h>

namespace argos {
    class CCI_ZebroLocomotionActuator : public CCI_Actuator {

    public:

        virtual ~CCI_ZebroLocomotionActuator() = default;

        /**
         * This function sets the new desired movement of the robot.
         * @param f_speed       - Speed as a value in [0, 1].
         * @param f_direction   - Direction as a value [-1, 1].
         */
        virtual void UpdateMovement(Real f_speed, Real f_direction) = 0;

#ifdef ARGOS_WITH_LUA

        virtual void CreateLuaState(lua_State *pt_lua_state);

#endif

        /** This function clips the given value between the given values. */
        Real Clip(Real value, Real minValue, Real maxValue);

    protected:

        /** Variable stores the current speed as value in range [0, 1]. */
        Real m_fCurrentSpeed;
        /** Variable stores the current direction in range [-1 (left), 1 (right)]. */
        Real m_fCurrentDirection;

    };
}


#endif //ARGOS3_SIMULATOR_CI_ZEBRO_LOCOMOTION_ACTUATOR_H
