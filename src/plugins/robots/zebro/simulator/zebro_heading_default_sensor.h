//
// Created by matthijs on 26-4-18.
//

#ifndef ARGOS3_SIMULATOR_ZEBRO_HEADING_DEFAULT_SENSOR_H
#define ARGOS3_SIMULATOR_ZEBRO_HEADING_DEFAULT_SENSOR_H

namespace argos {
    class CZebroHeadingDefaultSensor;
}

#include <plugins/robots/generic/simulator/positioning_default_sensor.h>
#include <plugins/robots/zebro/control_interface/ci_zebro_heading_sensor.h>

namespace argos {


    class CZebroHeadingDefaultSensor : public CSimulatedSensor,
                                       public CCI_ZebroHeadingSensor
    {

    public:
        CZebroHeadingDefaultSensor();

        virtual ~CZebroHeadingDefaultSensor() {}

        virtual void SetRobot(CComposableEntity& c_entity);

        virtual void Init(TConfigurationNode& t_tree);

        virtual void Update();

        virtual void Reset();

    private:

        CPositioningDefaultSensor *m_cPositioningSensor;
    };
}



#endif //ARGOS3_SIMULATOR_ZEBRO_HEADING_DEFAULT_SENSOR_H
