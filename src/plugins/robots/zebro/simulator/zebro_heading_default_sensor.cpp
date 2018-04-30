//
// Created by matthijs on 26-4-18.
//

#include "zebro_heading_default_sensor.h"


argos::CZebroHeadingDefaultSensor::CZebroHeadingDefaultSensor()
{
    m_cPositioningSensor = new CPositioningDefaultSensor();
};


void argos::CZebroHeadingDefaultSensor::SetRobot(argos::CComposableEntity &c_entity)
{
    m_cPositioningSensor->SetRobot(c_entity);
}

void argos::CZebroHeadingDefaultSensor::Init(argos::TConfigurationNode &t_tree)
{
    CCI_Sensor::Init(t_tree);
    m_cPositioningSensor->Init(t_tree);
}

void argos::CZebroHeadingDefaultSensor::Update()
{
    m_cPositioningSensor->Update();

    /** Transform the data from the positioning sensor to the heading. */
    CRadians cTmp1, cTmp2;
    CCI_PositioningSensor::SReading directionReading = m_cPositioningSensor->GetReading();
    directionReading.Orientation.ToEulerAngles(m_cHeading, cTmp1, cTmp2);

}

void argos::CZebroHeadingDefaultSensor::Reset()
{
    CCI_Sensor::Reset();
    m_cPositioningSensor->Reset();
}

REGISTER_SENSOR(CZebroHeadingDefaultSensor,
                "zebro_heading", "default",
                "Matthijs den Toom [thijsco4@gmail.com]",
                "1.0",
                "A simulated version of the heading sensor of the zebro.",
                "This sensor returns the heading of the robot. Control is the same as\n"
                "the positioning default sensor, since it uses this sensor internally.\n\n"
                "None.\n",
                "Usable"
)