/**
 * @file <argos3/plugins/ropucks/e-puck/simulator/physx_epuck_model.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef PHYSX_ZEBRO_MODEL_H
#define PHYSX_ZEBRO_MODEL_H

namespace argos {
   class CPhysXEngine;
   class CPhysXZebroModel;
   class CZebroEntity;
   class CWheeledEntity;
}

#include <argos3/plugins/simulator/physics_engines/physx/physx_differential_drive.h>
#include <argos3/plugins/simulator/physics_engines/physx/physx_multi_body_object_model.h>

namespace argos {

   class CPhysXZebroModel : public CPhysXMultiBodyObjectModel {

   public:

      CPhysXZebroModel(CPhysXEngine& c_engine,
                       CEPuckEntity& c_entity);

      virtual void Reset();

      virtual void UpdateFromEntityStatus();

      /**
       * Updates the origin anchor associated to the embodied entity.
       * @param s_anchor The origin anchor.
       */
      void UpdateOriginAnchor(SAnchor& s_anchor);

      inline CPhysXDifferentialDrive& GetDifferentialDrive() {
         return m_cDiffDrive;
      }

   private:

      const Real* m_fCurrentWheelVelocity;
      CPhysXDifferentialDrive m_cDiffDrive;
   };

}

#endif
