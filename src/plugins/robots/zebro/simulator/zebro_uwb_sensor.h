//
// Created by matthijs on 7-6-18.
//

#ifndef ARGOS3_SIMULATOR_ZEBRO_UWB_SENSOR_H
#define ARGOS3_SIMULATOR_ZEBRO_UWB_SENSOR_H

namespace argos {
    class CZebroUWBSensor;
}

#include <argos3/plugins/robots/generic/control_interface/ci_range_and_bearing_sensor.h>
#include <argos3/core/simulator/sensor.h>
#include <argos3/plugins/robots/generic/simulator/range_and_bearing_medium_sensor.h>

namespace argos {
    class CZebroUWBSensor : public CSimulatedSensor,
                            public CCI_RangeAndBearingSensor
    {
    public:
        CZebroUWBSensor();

        virtual ~CZebroUWBSensor();

        virtual void SetRobot(CComposableEntity& c_entity);

        virtual void Init(TConfigurationNode& t_tree);

        virtual void Update();

        virtual void Reset();

        virtual void Destroy();

    private:

        CRABEquippedEntity*  m_pcRangeAndBearingEquippedEntity;

        CControllableEntity* m_pcControllableEntity;

        CRangeAndBearingMediumSensor *cRabSensor;

        UInt32 m_maxReadings;

        bool m_bShowRays;

        /**
         * Insert the given packet in the list of readings, if it is closer than the farrest.
         * In case it is closes the farrest element is removed.
         * @param packet        - The packet that might be inserted.
         */
        void insertIfCloserThanFarthestNode(SPacket packet);


        /**
         * This function shows the rays to the uwb objects used in this round.
         */
        void showRays();
    };
}




#endif //ARGOS3_SIMULATOR_ZEBRO_UWB_SENSOR_H
