//
// Created by matthijs on 11-5-18.
//

#include "../control_interface/zebro_dimensions.h"
#include "zebro_locomotion_actuator.h"


namespace argos {

    CZebroLocomotionActuator::CZebroLocomotionActuator()
    {
        m_cActualActuator = new CDifferentialSteeringDefaultActuator();

        m_fInterWheelDistance = ZEBRO_INTERWHEEL_DISTANCE;
    }

    CZebroLocomotionActuator::~CZebroLocomotionActuator()
    {
        delete m_cActualActuator;
    }

    void CZebroLocomotionActuator::SetRobot(CComposableEntity &c_entity)
    {
        m_cActualActuator->SetRobot(c_entity);
    }

    void CZebroLocomotionActuator::Init(TConfigurationNode &t_tree)
    {
        CCI_Actuator::Init(t_tree);
        m_cActualActuator->Init(t_tree);
        GetNodeAttributeOrDefault(t_tree, "max_angular_velocity", m_fMaxAngularVelocity, ZEBRO_MAX_ANGULAR_VELOCITY);
        GetNodeAttributeOrDefault(t_tree, "max_speed", m_fMaxSpeed, ZEBRO_MAX_SPEED);
    }

    void CZebroLocomotionActuator::Update()
    {
        //TODO: update these formula's to correclty indicate actuation per wheels
        Real speed = m_fCurrentSpeed * m_fMaxSpeed;
        Real leftVelocity, rightVelocity;       // Define the velocities of both wheels.

        Real turnSpeed = m_fCurrentDirection * m_fInterWheelDistance * m_fMaxAngularVelocity * 100;
        leftVelocity = (speed + turnSpeed);
        rightVelocity = (speed - turnSpeed);

        m_cActualActuator->SetLinearVelocity(leftVelocity, rightVelocity);

        m_cActualActuator->Update();
    }

    void CZebroLocomotionActuator::Reset()
    {
        CCI_Actuator::Reset();
        m_cActualActuator->Reset();
        m_fCurrentDirection = 0;
        m_fCurrentSpeed = 0;
    }

    void CZebroLocomotionActuator::UpdateMovement(Real f_speedFraction, Real f_direction)
    {
        f_direction = Clip(f_direction, -1, 1);
        f_speedFraction = Clip(f_speedFraction, 0, 1);

        m_fCurrentSpeed = f_speedFraction;
        m_fCurrentDirection = f_direction;
    }

    REGISTER_ACTUATOR(CZebroLocomotionActuator,
                      "zebro_locomotion", "default",
                      "Matthijs den Toom [thijsco4@gmail.com]",
                      "1.0",
                      "The locomotion actuator used by the zebro robot.",
                      "This actuator simulates the motions of a zebro robot by using a differential drive actuator."
                      "For a complete description of its usage, refer to the\n"
                      "ci_locomotion_actuator.h file.\n\n"
                      "REQUIRED XML CONFIGURATION\n\n"
                      "  <controllers>\n"
                      "    ...\n"
                      "    <my_controller ...>\n"
                      "      ...\n"
                      "      <actuators>\n"
                      "        ...\n"
                      "        <zebro_locomotion implementation=\"default\" />\n"
                      "        ...\n"
                      "      </actuators>\n"
                      "      ...\n"
                      "    </my_controller>\n"
                      "    ...\n"
                      "  </controllers>\n\n"
                      "OPTIONAL XML CONFIGURATION\n\n"
                      "All the options that can be given to a default differential drive actuator can be given to this"
                      "actuator as well. For now this includes 'noise_std_dev'."
                      "Furthermore, a maximum speed in cm/s can be given and a max angular (turn) velocity in rad/s:"
                      "  <controllers>\n"
                      "    ...\n"
                      "    <my_controller ...>\n"
                      "      ...\n"
                      "      <actuators>\n"
                      "        ...\n"
                      "        <zebro_locomotion implementation=\"default\"\n"
                      "                               max_speed=\"10\" max_angular_velocity=\"2\" />\n"
                      "        ...\n"
                      "      </actuators>\n"
                      "      ...\n"
                      "    </my_controller>\n"
                      "    ...\n"
                      "  </controllers>\n",
                      "Usable"
    );

}
