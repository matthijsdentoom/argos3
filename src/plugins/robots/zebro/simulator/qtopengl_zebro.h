/**
 * @file <argos3/plugins/robots/e-puck/simulator/qtopengl_epuck.h>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#ifndef QTOPENGL_ZEBRO_H
#define QTOPENGL_ZEBRO_H

namespace argos {
   class CQTOpenGLZebro;
   class CZebroEntity;
}

#include <argos3/core/utility/math/vector2.h>

#ifdef __APPLE__
#include <gl.h>
#else
#include <GL/gl.h>
#endif

namespace argos {

   class CQTOpenGLZebro {

   public:

      CQTOpenGLZebro();

      virtual ~CQTOpenGLZebro();

      virtual void Draw(CZebroEntity& c_entity);

   protected:

      /** Sets a green plastic material */
      void SetGreenPlasticMaterial();
      /** Sets a red plastic material */
      void SetBlackPlasticMaterial();
      /** Sets a circuit board material */
      void SetCircuitBoardMaterial();
      /** Sets a colored LED material */
      void SetLEDMaterial(GLfloat f_red,
                          GLfloat f_green,
                          GLfloat f_blue);

      /** Renders a wheel as a representation of a leg. */
      void RenderWheel();
      /** Renders the body */
      void RenderBody();
      /** A single LED of the ring */
      void RenderLED();
      /** Draws a box with the given parameters. */
      void DrawBox(float width, float length, float height, float init_x, float init_y, float init_z);

   private:

      /** Start of the display list index */
      GLuint m_unLists;

      /** E-puck wheel */
      GLuint m_unWheelList;

      /** Chassis display list */
      GLuint m_unHeadList;

      /** Body display list */
      GLuint m_unBodyList;

      /** LED display list */
      GLuint m_unLEDList;

      /** Number of vertices to display the round parts
          (wheels, chassis, etc.) */
      GLuint m_unVertices;

      /* Angle gap between two leds */
      GLfloat m_fLEDAngleSlice;
      
      /** Draws a wheel at the point translated with respect to the center of the robot. */
      void DrawWheel(CVector2 translation);

      /** Renders the head of the robot. */
      void RenderHead();
   };

}

#endif
