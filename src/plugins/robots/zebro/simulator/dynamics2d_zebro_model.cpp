/**
 * @file <argos3/plugins/robots/zebro/simulator/dynamics2d_zebro_model.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "dynamics2d_zebro_model.h"
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_gripping.h>
#include <argos3/plugins/simulator/physics_engines/dynamics2d/dynamics2d_engine.h>

namespace argos {

   /****************************************/
   /****************************************/

   static const Real ZEBRO_MASS                = 0.4f;

   static const Real ZEBRO_RADIUS              = 0.035f;
   static const Real ZEBRO_INTERWHEEL_DISTANCE = 0.053f;
   static const Real ZEBRO_HEIGHT              = 0.086f;

   static const Real ZEBRO_MAX_FORCE           = 1.5f;
   static const Real ZEBRO_MAX_TORQUE          = 1.5f;

   enum ZEBRO_WHEELS {
      ZEBRO_LEFT_WHEEL = 0,
      ZEBRO_RIGHT_WHEEL = 1
   };

   /****************************************/
   /****************************************/

   CDynamics2DZebroModel::CDynamics2DZebroModel(CDynamics2DEngine& c_engine,
                                                CZebroEntity& c_entity) :
      CDynamics2DSingleBodyObjectModel(c_engine, c_entity),
      m_cZebroEntity(c_entity),
      m_cWheeledEntity(m_cZebroEntity.GetWheeledEntity()),
      m_cDiffSteering(c_engine,
                      ZEBRO_MAX_FORCE,
                      ZEBRO_MAX_TORQUE,
                      ZEBRO_INTERWHEEL_DISTANCE),
      m_fCurrentWheelVelocity(m_cWheeledEntity.GetWheelVelocities()) {
      /* Create the body with initial position and orientation */
      cpBody* ptBody =
         cpSpaceAddBody(GetDynamics2DEngine().GetPhysicsSpace(),
                        cpBodyNew(ZEBRO_MASS,
                                  cpMomentForCircle(ZEBRO_MASS,
                                                    0.0f,
                                                    ZEBRO_RADIUS + ZEBRO_RADIUS,
                                                    cpvzero)));
      const CVector3& cPosition = GetEmbodiedEntity().GetOriginAnchor().Position;
      ptBody->p = cpv(cPosition.GetX(), cPosition.GetY());
      CRadians cXAngle, cYAngle, cZAngle;
      GetEmbodiedEntity().GetOriginAnchor().Orientation.ToEulerAngles(cZAngle, cYAngle, cXAngle);
      cpBodySetAngle(ptBody, cZAngle.GetValue());
      /* Create the body shape */
      cpShape* ptShape =
         cpSpaceAddShape(GetDynamics2DEngine().GetPhysicsSpace(),
                         cpCircleShapeNew(ptBody,
                                          ZEBRO_RADIUS,
                                          cpvzero));
      ptShape->e = 0.0; // No elasticity
      ptShape->u = 0.7; // Lots of friction
      /* Constrain the actual base body to follow the diff steering control */
      m_cDiffSteering.AttachTo(ptBody);
      /* Set the body so that the default methods work as expected */
      SetBody(ptBody, ZEBRO_HEIGHT);
   }

   /****************************************/
   /****************************************/

   CDynamics2DZebroModel::~CDynamics2DZebroModel() {
      m_cDiffSteering.Detach();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DZebroModel::Reset() {
      CDynamics2DSingleBodyObjectModel::Reset();
      m_cDiffSteering.Reset();
   }

   /****************************************/
   /****************************************/

   void CDynamics2DZebroModel::UpdateFromEntityStatus() {
      /* Do we want to move? */
      if((m_fCurrentWheelVelocity[ZEBRO_LEFT_WHEEL] != 0.0f) ||
         (m_fCurrentWheelVelocity[ZEBRO_RIGHT_WHEEL] != 0.0f)) {
         m_cDiffSteering.SetWheelVelocity(m_fCurrentWheelVelocity[ZEBRO_LEFT_WHEEL],
                                          m_fCurrentWheelVelocity[ZEBRO_RIGHT_WHEEL]);
      }
      else {
         /* No, we don't want to move - zero all speeds */
         m_cDiffSteering.Reset();
      }
   }

   /****************************************/
   /****************************************/

   REGISTER_STANDARD_DYNAMICS2D_OPERATIONS_ON_ENTITY(CZebroEntity, CDynamics2DZebroModel);

   /****************************************/
   /****************************************/

}
