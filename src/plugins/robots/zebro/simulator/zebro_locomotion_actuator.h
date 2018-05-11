//
// Created by matthijs on 11-5-18.
//

#ifndef ARGOS3_SIMULATOR_ZEBRO_LOCOMOTION_ACTUATOR_H
#define ARGOS3_SIMULATOR_ZEBRO_LOCOMOTION_ACTUATOR_H

namespace argos
{
    class CZebroLocomotionActuator;
}

#include <argos3/core/simulator/sensor.h>
#include <argos3/core/simulator/actuator.h>
#include <argos3/plugins/robots/generic/simulator/differential_steering_default_actuator.h>
#include <argos3/plugins/robots/zebro/control_interface/ci_zebro_locomotion_actuator.h>

namespace argos
{
    class CZebroLocomotionActuator : public CSimulatedActuator, public CCI_ZebroLocomotionActuator
    {
    public:

        CZebroLocomotionActuator();

        virtual ~CZebroLocomotionActuator();

        virtual void SetRobot(CComposableEntity& c_entity);

        virtual void Init(TConfigurationNode& t_tree);

        virtual void Update();

        virtual void Reset();

        void UpdateMovement(Real f_speed, Real f_direction) override;

    private:
        /* Variable to indicate the actual differentialDrive actuator that is used. */
        CDifferentialSteeringDefaultActuator *m_cActualActuator;

        /** Indicate the maximum forward speed of the robot. (m/s) */
        Real m_fMaxSpeed;

        /* Indicate the distance between the 'wheels' of the robot. (m) */
        Real m_fInterWheelDistance;

        /* Indicate the maximum turn speed of the robot. (rad/s)*/
        Real m_fMaxAngularVelocity;

        /** This function clips the given value between the given values. */
        Real clip(Real value, Real minValue, Real maxValue);
    };
}


#endif //ARGOS3_SIMULATOR_ZEBRO_LOCOMOTION_ACTUATOR_H
