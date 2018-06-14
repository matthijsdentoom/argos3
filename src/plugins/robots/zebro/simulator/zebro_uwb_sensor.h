//
// Created by matthijs on 7-6-18.
//

#ifndef ARGOS3_SIMULATOR_ZEBRO_UWB_SENSOR_H
#define ARGOS3_SIMULATOR_ZEBRO_UWB_SENSOR_H

namespace argos {
    class CZebroUWBSensor;
}

#include <argos3/core/simulator/sensor.h>
#include <plugins/robots/zebro/control_interface/ci_zebro_localisation_sensor.h>
#include <plugins/simulator/entities/rab_equipped_entity.h>
#include <plugins/robots/generic/simulator/range_and_bearing_medium_sensor.h>
#include "zebro_heading_default_sensor.h"

namespace argos {
    class CZebroUWBSensor : public CSimulatedSensor,
                            public CCI_ZebroLocalisationSensor
    {
    public:
        CZebroUWBSensor();

        virtual ~CZebroUWBSensor() = default;

        virtual void SetRobot(CComposableEntity& c_entity);

        virtual void Init(TConfigurationNode& t_tree);

        virtual void Update();

        virtual void Reset();

        virtual void Destroy();

    private:

        CRABEquippedEntity*  m_pcRangeAndBearingEquippedEntity;
        CControllableEntity* m_pcControllableEntity;
        CRABMedium*          m_pcRangeAndBearingMedium;
        Real                 m_fDistanceNoiseStdDev;
        Real                 m_fPacketDropProb;
        CRandom::CRNG*       m_pcRNG;
        CSpace&              m_cSpace;
        bool                 m_bShowRays;
        UInt32               m_iMaxReadings;

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

        void insertIfCloserThanFarthestNode(CCI_RangeAndBearingSensor::SPacket packet);

        CRadians GetRelativeHeading(CRABEquippedEntity cRABEntity);
    };
}




#endif //ARGOS3_SIMULATOR_ZEBRO_UWB_SENSOR_H
