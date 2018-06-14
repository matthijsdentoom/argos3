//
// Created by matthijs on 7-6-18.
//

#include "zebro_uwb_sensor.h"
#include <argos3/core/simulator/entity/composable_entity.h>
#include <argos3/plugins/simulator/entities/rab_equipped_entity.h>
#include <argos3/core/simulator/entity/controllable_entity.h>
#include <plugins/robots/generic/simulator/positioning_default_sensor.h>
#include <argos3/core/simulator/simulator.h>
#include <argos3/plugins/simulator/media/rab_medium.h>

namespace argos
{
    CRange<CRadians> INCLINATION_RANGE(CRadians(0), CRadians(ARGOS_PI));

    CZebroUWBSensor::CZebroUWBSensor() :
            m_pcRangeAndBearingEquippedEntity(nullptr),
            m_fDistanceNoiseStdDev(0.0f),
            m_fPacketDropProb(0.0f),
            m_pcRNG(nullptr),
            m_cSpace(CSimulator::GetInstance().GetSpace()),
            m_bShowRays(false) {}


    void CZebroUWBSensor::SetRobot(CComposableEntity &c_entity)
    {
        /* Assign RAB equipped entity to this sensor */
        m_pcRangeAndBearingEquippedEntity = &c_entity.GetComponent<CRABEquippedEntity>("rab");
        /* Get reference to controllable entity */
        m_pcControllableEntity = &c_entity.GetComponent<CControllableEntity>("controller");
    }


    void CZebroUWBSensor::Init(TConfigurationNode &t_tree)
    {
        try {
            /* Parent class init */
            CCI_ZebroLocalisationSensor::Init(t_tree);
            /* Show rays? */
            GetNodeAttributeOrDefault(t_tree, "show_rays", m_bShowRays, m_bShowRays);
            /* Parse noise */
            GetNodeAttributeOrDefault(t_tree, "noise_std_dev", m_fDistanceNoiseStdDev, m_fDistanceNoiseStdDev);
            GetNodeAttributeOrDefault(t_tree, "packet_drop_prob", m_fPacketDropProb, m_fPacketDropProb);
            GetNodeAttributeOrDefault(t_tree, "max_neighbours", m_iMaxReadings, m_iMaxReadings);
            if((m_fPacketDropProb > 0.0f) ||
               (m_fDistanceNoiseStdDev > 0.0f)) {
                m_pcRNG = CRandom::CreateRNG("argos");
            }
            /* Get RAB medium from id specified in the XML */
            std::string strMedium;
            GetNodeAttribute(t_tree, "medium", strMedium);
            m_pcRangeAndBearingMedium = &(CSimulator::GetInstance().GetMedium<CRABMedium>(strMedium));
            /* Assign RAB entity to the medium */
            m_pcRangeAndBearingEquippedEntity->SetMedium(*m_pcRangeAndBearingMedium);
            /* Enable the RAB equipped entity */
            m_pcRangeAndBearingEquippedEntity->Enable();
        }
        catch(CARGoSException& ex) {
            THROW_ARGOSEXCEPTION_NESTED("Error initializing the range and bearing medium sensor", ex);
        }
    }


    void CZebroUWBSensor::Update()
    {
        /* Delete old readings */
        m_tReadings.clear();
        /* Get list of communicating RABs */
        const CSet<CRABEquippedEntity*,SEntityComparator>& setRABs = m_pcRangeAndBearingMedium->GetRABsCommunicatingWith(*m_pcRangeAndBearingEquippedEntity);
        /* Buffer for calculating the message--robot distance */
        CVector3 cVectorRobotToMessage;
        /* Buffer for the received packet */
        CCI_ZebroLocalisationSensor::SPacket sPacket;
        /* Go through communicating RABs and create packets */
        for(CSet<CRABEquippedEntity*>::iterator it = setRABs.begin();
            it != setRABs.end(); ++it) {
            /* Should we drop this packet? */
            if(m_pcRNG == nullptr || /* No noise to apply */
               !(m_fPacketDropProb > 0.0f &&
                 m_pcRNG->Bernoulli(m_fPacketDropProb)) /* Packet is not dropped */
                    ) {
                /* Create a reference to the RAB entity to process */
                CRABEquippedEntity& cRABEntity = **it;
                /* Calculate vector to entity */
                cVectorRobotToMessage = cRABEntity.GetPosition();
                cVectorRobotToMessage -= m_pcRangeAndBearingEquippedEntity->GetPosition();
                /* If noise was setup, add it */
                if(m_pcRNG && m_fDistanceNoiseStdDev > 0.0f) {
                    cVectorRobotToMessage += CVector3(
                            m_pcRNG->Gaussian(m_fDistanceNoiseStdDev),
                            m_pcRNG->Uniform(INCLINATION_RANGE),
                            m_pcRNG->Uniform(CRadians::UNSIGNED_RANGE));
                }
                /*
                 * Set range and bearing from cVectorRobotToMessage
                 * First, we must rotate the cVectorRobotToMessage so that
                 * it is local to the robot coordinate system. To do this,
                 * it enough to rotate cVectorRobotToMessage by the inverse
                 * of the robot orientation.
                 */
                cVectorRobotToMessage.Rotate(m_pcRangeAndBearingEquippedEntity->GetOrientation().Inverse());
                cVectorRobotToMessage.ToSphericalCoords(sPacket.Range,
                                                        sPacket.VerticalBearing,
                                                        sPacket.HorizontalBearing);
                /* Convert range to cm */
                sPacket.Range *= 100.0f;
                /* Normalize horizontal bearing between [-pi,pi] */
                sPacket.HorizontalBearing.SignedNormalize();
                /*
                 * The vertical bearing is defined as the angle between the local
                 * robot XY plane and the message source position, i.e., the elevation
                 * in math jargon. However, cVectorRobotToMessage.ToSphericalCoords()
                 * sets sPacket.VerticalBearing to the inclination, which is the angle
                 * between the azimuth vector (robot local Z axis) and
                 * cVectorRobotToMessage. Elevation = 90 degrees - Inclination.
                 */
                sPacket.VerticalBearing.Negate();
                sPacket.VerticalBearing += CRadians::PI_OVER_TWO;
                sPacket.VerticalBearing.SignedNormalize();
                /* Set the address of the robot */
                sPacket.Address = cRABEntity.GetData()[0];
                sPacket.RelativeHeading = GetRelativeHeading(cRABEntity);

                // Insert the closest of the measurements.
                if (m_tReadings.size() < m_iMaxReadings) {
                    m_tReadings.push_back(sPacket);
                } else {
                    insertIfCloserThanFarthestNode(sPacket);
                }
            }
        }

        if (m_bShowRays)
        {
            showRays();
        }
    }

    CRadians CZebroUWBSensor::GetRelativeHeading(CRABEquippedEntity cRABEntity)
    {
        CRadians tmp;
        CRadians otherHeading;
        CRadians ownHeading;
        cRABEntity.GetOrientation().ToEulerAngles(otherHeading, tmp, tmp);
        m_pcRangeAndBearingEquippedEntity->GetOrientation().ToEulerAngles(ownHeading, tmp, tmp);

        return ownHeading - otherHeading;
    }

    void CZebroUWBSensor::Reset()
    {
        m_tReadings.clear();
    }


    void CZebroUWBSensor::Destroy()
    {
        m_pcRangeAndBearingMedium->RemoveEntity(*m_pcRangeAndBearingEquippedEntity);
    }


    void CZebroUWBSensor::insertIfCloserThanFarthestNode(CCI_ZebroLocalisationSensor::SPacket packet)
    {
        // Get the farrest element
        UInt32 iIndexOfFarrest = 0;
        for (UInt32 i = 0; i < m_tReadings.size(); i++) {
            if (m_tReadings.at(i).Range > m_tReadings.at(iIndexOfFarrest).Range) {
                iIndexOfFarrest = i;
            }
        }
        // If the farrest packet is farther than this one, insert this packet.
        if (m_tReadings.at(iIndexOfFarrest).Range > packet.Range) {
            m_tReadings.erase(m_tReadings.begin() + iIndexOfFarrest);
            m_tReadings.push_back(packet);
        }
    }


    void CZebroUWBSensor::showRays()
    {
        for (const SPacket &reading : m_tReadings) {

            CVector3 directionVector = CVector3(reading.Range / 100, 0, 0).RotateZ(reading.HorizontalBearing);

            // Rotate with the orientation of this robot to point to the direct point.
            directionVector.Rotate(m_pcRangeAndBearingEquippedEntity->GetOrientation());

            m_pcControllableEntity->AddCheckedRay(false,
                                                  CRay3(m_pcRangeAndBearingEquippedEntity->GetPosition(),
                                                        m_pcRangeAndBearingEquippedEntity->GetPosition()
                                                        + directionVector));
        }
    }


    REGISTER_SENSOR(CZebroUWBSensor,
                    "range_and_bearing", "zebro-uwb",
                    "Matthijs den Toom [m.denToom@student.tudelft.nl]",
                    "1.0",
                    "The range-and-bearing sensor.",
                    "This sensor allows robots to perform situated communication, i.e., a form of\n"
                    "wireless communication whereby the receiver also knows the location of the\n"
                    "sender with respect to its own frame of reference.\n"
                    "It is a slighlty adapted implementation with respect to medium, that limits the number of sensors"
                    "that are Percieved. Additional usage therefore becomes:"
                    "  <controllers>\n"
                    "    ...\n"
                    "    <my_controller ...>\n"
                    "      ...\n"
                    "      <sensors>\n"
                    "        ...\n"
                    "        <range_and_bearing implementation=\"zebro-uwb\"\n"
                    "                           medium=\"rab\"\n"
                    "                           max_neighbours=\"10\" />\n"
                    "        ...\n"
                    "      </sensors>\n"
                    "      ...\n"
                    "    </my_controller>\n"
                    "    ...\n"
                    "  </controllers>\n"
                    "Additionally, instead of all rays only the rays that are actually being used by the sensor can be"
                    "shown by defining show_used_rays:"
                    "  <controllers>\n"
                    "    ...\n"
                    "    <my_controller ...>\n"
                    "      ...\n"
                    "      <sensors>\n"
                    "        ...\n"
                    "        <range_and_bearing implementation=\"zebro-uwb\"\n"
                    "                           medium=\"rab\"\n"
                    "                           show_used_rays=\"true\" />\n"
                    "        ...\n"
                    "      </sensors>\n"
                    "      ...\n"
                    "    </my_controller>\n"
                    "    ...\n"
                    "  </controllers>\n",
                    "Usable");

}
