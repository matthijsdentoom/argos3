//
// Created by matthijs on 7-6-18.
//

#include "zebro_uwb_sensor.h"
#include <argos3/core/simulator/entity/composable_entity.h>

namespace argos
{

    CZebroUWBSensor::CZebroUWBSensor() :
            cRabSensor(new CRangeAndBearingMediumSensor()),
            m_maxReadings(INT32_MAX)
    {}


    CZebroUWBSensor::~CZebroUWBSensor()
    {
        delete cRabSensor;
    }


    void CZebroUWBSensor::SetRobot(CComposableEntity &c_entity)
    {
        cRabSensor->SetRobot(c_entity);
    }


    void CZebroUWBSensor::Init(TConfigurationNode &t_tree)
    {
        CCI_RangeAndBearingSensor::Init(t_tree);
        cRabSensor->Init(t_tree);
        GetNodeAttributeOrDefault(t_tree, "max_neighbours", m_maxReadings, m_maxReadings);
    }


    void CZebroUWBSensor::Update()
    {
        /** Delete old packets. */
        m_tReadings.clear();
        // Update the sensor.
        cRabSensor->Update();
        TReadings tempReadings = cRabSensor->GetReadings();

        for (const SPacket &reading : tempReadings) {
            if (m_tReadings.size() < m_maxReadings) {
                m_tReadings.push_back(reading);
            } else {
                insertIfCloserThanFarthestNode(reading);
            }
        }

        // Note that seeing behind other robots is possible here, since that would also be possible
        // with the UWB sensor itself.

        //TODO: filter out robots that are behind each other.
        //TODO: give distance color to rays.

    }

    void CZebroUWBSensor::Reset()
    {
        cRabSensor->Reset();
        CCI_RangeAndBearingSensor::Reset();
    }


    void CZebroUWBSensor::Destroy()
    {
        cRabSensor->Destroy();
        CCI_RangeAndBearingSensor::Destroy();
    }


    void CZebroUWBSensor::insertIfCloserThanFarthestNode(CCI_RangeAndBearingSensor::SPacket packet)
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
                    "  </controllers>\n",
                    "Usable");

}
