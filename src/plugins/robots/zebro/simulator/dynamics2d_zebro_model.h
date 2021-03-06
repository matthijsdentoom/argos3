/**
 * @file <argos3/plugins/robots/e-puck/simulator/dynamics2d_epuck_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef DYNAMICS2D_ZEBRO_MODEL_H
#define DYNAMICS2D_ZEBRO_MODEL_H

namespace argos {
   class CDynamics2DDifferentialSteeringControl;
   class CDynamics2DGripper;
   class CDynamics2DGrippable;
   class CDynamics2DZebroModel;
}

#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_single_body_object_model.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_differentialsteering_control.h>
#include <argos3/plugins/robots/zebro/simulator/zebro_entity.h>

namespace argos {

   class CDynamics2DZebroModel : public CDynamics2DSingleBodyObjectModel {

   public:

      CDynamics2DZebroModel(CDynamics2DEngine& c_engine,
                              CZebroEntity& c_entity);
      virtual ~CDynamics2DZebroModel();

      virtual void Reset();

      virtual void UpdateFromEntityStatus();
      
   private:

      CZebroEntity& m_cZebroEntity;
      CWheeledEntity& m_cWheeledEntity;

      CDynamics2DDifferentialSteeringControl m_cDiffSteering;

      const Real* m_fCurrentWheelVelocity;

   };

}

#endif
