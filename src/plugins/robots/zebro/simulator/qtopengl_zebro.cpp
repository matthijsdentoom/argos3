/**
 * @file <argos3/plugins/robots/zebro/simulator/qtopengl_zebro.cpp>
 *
 * @author Carlo Pinciroli - <ilpincy@gmail.com>
 */

#include "qtopengl_zebro.h"
#include "zebro_entity.h"
#include "zebro_dimensions.h"
#include <argos3/core/simulator/entity/embodied_entity.h>
#include <argos3/core/utility/math/vector2.h>
#include <argos3/core/utility/math/vector3.h>
#include <argos3/plugins/simulator/entities/led_equipped_entity.h>
#include <argos3/plugins/simulator/visualizations/qt-opengl/qtopengl_widget.h>

namespace argos {

    /****************************************/
    /****************************************/

    /* All measures are in meters */
   static const Real WHEEL_RADIUS                = CATERPILLAR_TRACK_HEIGHT * 0.5f;
   static const Real HALF_WHEEL_WIDTH            = CATERPILLAR_TRACK_WIDTH * 0.5f;
   static const Real INTERWHEEL_DISTANCE         = ZEBRO_WIDTH + 2 * SIDE_PANEL_WIDTH;
   static const Real HALF_INTERWHEEL_DISTANCE    = INTERWHEEL_DISTANCE * 0.5f;

   static const Real CHASSIS_ELEVATION           = 0.005f;                            // to be checked!
   static const Real HALF_CHASSIS_LENGTH         = 0.0275f;                             // to be checked!
   static const Real HALF_CHASSIS_WIDTH          = HALF_INTERWHEEL_DISTANCE - HALF_WHEEL_WIDTH;


   static const Real LED_SPACING                 = 0.02f;
   static const Real LED_OUTER_RADIUS            = 0.035f;
   static const Real LED_ELEVATION               = ZEBRO_ELEVATION + ZEBRO_HEIGHT;
   static const Real LED_HEIGHT                  = 0.01f;                               // to be checked!
   static const Real LED_UPPER_RING_INNER_RADIUS = 0.8 * LED_OUTER_RADIUS;

   static const Real HEAD_BOX_HEIGHT             = 0.02f;
   static const Real HEAD_BOX_WIDTH              = 0.03f;
   static const Real HEAD_BOX_LENGTH             = 0.06f;

   static const Real HEAD_HEIGHT             = 0.02f;
   static const Real HEAD_WIDTH              = 0.05f;
   static const Real HEAD_LENGTH             = 0.02f;

   /****************************************/
   /****************************************/

   CQTOpenGLZebro::CQTOpenGLZebro() :
      m_unVertices(40),
      m_fLEDAngleSlice(360.0f / 8.0f) {
      /* Reserve the needed display lists */
      m_unLists = glGenLists(4);

      /* Assign indices for better referencing (later) */
      m_unWheelList   = m_unLists;
      m_unHeadList    = m_unLists + 1;
      m_unBodyList    = m_unLists + 2;
      m_unLEDList     = m_unLists + 3;

      /* Create the wheel display list */
      glNewList(m_unWheelList, GL_COMPILE);
      RenderWheel();
      glEndList();

      /* Create the body display list */
      glNewList(m_unBodyList, GL_COMPILE);
      RenderBody();
      glEndList();

      /* Create the LED display list */
      glNewList(m_unLEDList, GL_COMPILE);
      RenderLED();
      glEndList();

      /* Create the Head display list */
      glNewList(m_unHeadList, GL_COMPILE);
      RenderHead();
      glEndList();
   }

   /****************************************/
   /****************************************/

   CQTOpenGLZebro::~CQTOpenGLZebro() {
      glDeleteLists(m_unLists, 4);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLZebro::DrawWheel(CVector2 translation)
   {
      glPushMatrix();
      glTranslatef(translation.GetX(), translation.GetY(), 0.0f);
      glCallList(m_unWheelList);
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLZebro::Draw(CZebroEntity& c_entity) {

      /* Place the body */
      glCallList(m_unBodyList);
      /* Place the wheels */
      DrawWheel(CVector2(0.0f, HALF_INTERWHEEL_DISTANCE));
      DrawWheel(CVector2(-0.1f, HALF_INTERWHEEL_DISTANCE - SIDE_PANEL_WIDTH));
      DrawWheel(CVector2(0.1f, HALF_INTERWHEEL_DISTANCE - SIDE_PANEL_WIDTH));
      DrawWheel(CVector2(0.0f, -HALF_INTERWHEEL_DISTANCE));
      DrawWheel(CVector2(-0.1f, -HALF_INTERWHEEL_DISTANCE + SIDE_PANEL_WIDTH));
      DrawWheel(CVector2(0.1f, -HALF_INTERWHEEL_DISTANCE + SIDE_PANEL_WIDTH));

      /* Place the LEDs */
      glPushMatrix();
      glTranslatef(LED_SPACING + LED_OUTER_RADIUS, 0.0f, 0.0f);
      CLEDEquippedEntity& cLEDEquippedEntity = c_entity.GetLEDEquippedEntity();
      for(UInt32 i = 0; i < 8; i++) {
         const CColor& cColor = cLEDEquippedEntity.GetLED(i).GetColor();
         glRotatef(-m_fLEDAngleSlice, 0.0f, 0.0f, 1.0f);
         SetLEDMaterial(cColor.GetRed(),
                        cColor.GetGreen(),
                        cColor.GetBlue());
         glCallList(m_unLEDList);
      }
      glPopMatrix();


      /* Place the LEDs */
      glPushMatrix();
      glTranslatef(-(LED_SPACING + LED_OUTER_RADIUS), 0.0f, 0.0f);
      for(UInt32 i = 0; i < 8; i++) {
         const CColor& cColor = cLEDEquippedEntity.GetLED(i).GetColor();
         glRotatef(-m_fLEDAngleSlice, 0.0f, 0.0f, 1.0f);
         SetLEDMaterial(cColor.GetRed(),
                        cColor.GetGreen(),
                        cColor.GetBlue());
         glCallList(m_unLEDList);
      }
      glPopMatrix();

      /** Place the head. */
      glPushMatrix();
      glTranslatef(LED_SPACING + LED_OUTER_RADIUS, 0.0f, ZEBRO_HEIGHT + ZEBRO_ELEVATION);
      glCallList(m_unHeadList);
      glPopMatrix();
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLZebro::SetGreenPlasticMaterial() {
      const GLfloat pfColor[]     = {   0.0f, 1.0f, 0.0f, 1.0f };
      const GLfloat pfSpecular[]  = {   0.9f, 0.9f, 0.9f, 1.0f };
      const GLfloat pfShininess[] = { 50.0f                   };
      const GLfloat pfEmission[]  = {   0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLZebro::SetBlackPlasticMaterial() {
      const GLfloat pfColor[]     = {   0.0f, 0.0f, 0.0f, 1.0f };
      const GLfloat pfSpecular[]  = {   0.9f, 0.9f, 0.9f, 1.0f };
      const GLfloat pfShininess[] = { 100.0f                   };
      const GLfloat pfEmission[]  = {   0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLZebro::SetCircuitBoardMaterial() {
      const GLfloat pfColor[]     = { 0.0f, 0.0f, 1.0f, 1.0f };
      const GLfloat pfSpecular[]  = { 0.5f, 0.5f, 1.0f, 1.0f };
      const GLfloat pfShininess[] = { 10.0f                  };
      const GLfloat pfEmission[]  = { 0.0f, 0.0f, 0.0f, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLZebro::SetLEDMaterial(GLfloat f_red,
                                       GLfloat f_green,
                                       GLfloat f_blue) {
      const GLfloat fEmissionFactor = 10.0f;
      const GLfloat pfColor[]     = {                    f_red,                   f_green,                   f_blue, 1.0f };
      const GLfloat pfSpecular[]  = {                     0.0f,                      0.0f,                     0.0f, 1.0f };
      const GLfloat pfShininess[] = {                     0.0f                                                            };
      const GLfloat pfEmission[]  = {  f_red * fEmissionFactor, f_green * fEmissionFactor, f_blue * fEmissionFactor, 1.0f };
      glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE, pfColor);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR,            pfSpecular);
      glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS,           pfShininess);
      glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION,            pfEmission);
   }

   /****************************************/
   /****************************************/

   void CQTOpenGLZebro::RenderWheel() {
      /* Set material */
      SetBlackPlasticMaterial();
      /* Right side */
      CVector2 cVertex(WHEEL_RADIUS, 0.0f);
      CRadians cAngle(CRadians::TWO_PI / m_unVertices);
      CVector3 cNormal(-1.0f, -1.0f, 0.0f);
      cNormal.Normalize();
      glBegin(GL_POLYGON);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
         glVertex3f(cVertex.GetX(), -HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
         cVertex.Rotate(cAngle);
         cNormal.RotateY(cAngle);
      }
      glEnd();

      /* Left side */
      cVertex.Set(WHEEL_RADIUS, 0.0f);
      cNormal.Set(-1.0f, 1.0f, 0.0f);
      cNormal.Normalize();
      cAngle = -cAngle;
      glBegin(GL_POLYGON);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
         glVertex3f(cVertex.GetX(), HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
         cVertex.Rotate(cAngle);
         cNormal.RotateY(cAngle);
      }
      glEnd();
      
      /* Tire */
      cNormal.Set(1.0f, 0.0f, 0.0f);
      cVertex.Set(WHEEL_RADIUS, 0.0f);
      cAngle = -cAngle;
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), cNormal.GetZ());
         glVertex3f(cVertex.GetX(), -HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
         glVertex3f(cVertex.GetX(),  HALF_WHEEL_WIDTH, WHEEL_RADIUS + cVertex.GetY());
         cVertex.Rotate(cAngle);
         cNormal.RotateY(cAngle);
      }
      glEnd();
   }

   void CQTOpenGLZebro::RenderHead()
   {
      SetBlackPlasticMaterial();
      
      DrawBox(HEAD_BOX_WIDTH, HEAD_BOX_LENGTH, HEAD_BOX_HEIGHT, -HEAD_BOX_WIDTH / 2, -HEAD_BOX_LENGTH / 2, 0);

      glRotatef(0, 0, -0.5f, 0);
      DrawBox(HEAD_WIDTH, HEAD_LENGTH, HEAD_HEIGHT, -HEAD_WIDTH / 2, HEAD_BOX_LENGTH, HEAD_BOX_HEIGHT + HEAD_HEIGHT);
   }


    /****************************************/
    /****************************************/

    void CQTOpenGLZebro::DrawBox(float length, float width, float height, float init_y, float init_x, float init_z) {

        glTranslatef(init_x, init_y, init_z);

        glBegin(GL_QUADS);

        // Bottom plane
        glNormal3f(0.0f, 0.0f, -1.0f);
        glVertex3f(0, 0, 0);
        glVertex3f(0, length, 0);
        glVertex3f(width, length, 0);
        glVertex3f(width, 0, 0);


        // Top plane
        glVertex3f(0, 0, height);
        glVertex3f(width, 0, height);
        glVertex3f(width, length, height);
        glVertex3f(0, length, height);

        // left-side plane
        glVertex3f(0, 0, 0);
        glVertex3f(0, 0, height);
        glVertex3f(0, length, height);
        glVertex3f(0, length, 0);

        // right-side plane
        glVertex3f(width, 0, 0);
        glVertex3f(width, length, 0);
        glVertex3f(width, length, height);
        glVertex3f(width, 0, height);

        // Front plane
        glVertex3f(0, 0, 0);
        glVertex3f(width, 0, 0);
        glVertex3f(width, 0, height);
        glVertex3f(0, 0, height);

        // Back plane
        glVertex3f(0, length, 0);
        glVertex3f(0, length, height);
        glVertex3f(width, length, height);
        glVertex3f(width, length, 0);
        glEnd();


        // Translate back.
        glTranslatef(-init_x, -init_y, -init_z);

    }

    /****************************************/
    /****************************************/

    void CQTOpenGLZebro::RenderBody() {
        /* Set material */
        SetGreenPlasticMaterial();

        // Draw main box.
        //DrawBox(ZEBRO_WIDTH, ZEBRO_LENGTH, ZEBRO_HEIGHT, SIDE_PANEL_WIDTH - 0.5 * ZEBRO_WIDTH, -0.5 * ZEBRO_LENGTH, ZEBRO_ELEVATION);
        DrawBox(ZEBRO_WIDTH, ZEBRO_LENGTH, ZEBRO_HEIGHT, - 0.5 * ZEBRO_WIDTH, -0.5 * ZEBRO_LENGTH, ZEBRO_ELEVATION);

        // Draw left side panel
        DrawBox(SIDE_PANEL_WIDTH, SIDE_PANEL_LENGTH, ZEBRO_HEIGHT, - 0.5 * ZEBRO_WIDTH - SIDE_PANEL_WIDTH, -SIDE_PANEL_LENGTH / 2, ZEBRO_ELEVATION);

        // Draw right side panel
        DrawBox(SIDE_PANEL_WIDTH, SIDE_PANEL_LENGTH, ZEBRO_HEIGHT, 0.5 * ZEBRO_WIDTH, -SIDE_PANEL_LENGTH / 2, ZEBRO_ELEVATION);
    }

   /****************************************/
   /****************************************/

   void CQTOpenGLZebro::RenderLED() {
      /* Side surface */
      CVector2 cVertex(LED_OUTER_RADIUS, 0.0f);
      CRadians cAngle(CRadians::TWO_PI / m_unVertices);
      CVector2 cNormal(1.0f, 0.0f);
      glBegin(GL_QUAD_STRIP);
      for(GLuint i = 0; i <= m_unVertices / 8; i++) {
         glNormal3f(cNormal.GetX(), cNormal.GetY(), 0.0f);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), LED_ELEVATION + LED_HEIGHT);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), LED_ELEVATION);
         cVertex.Rotate(cAngle);
         cNormal.Rotate(cAngle);
      }
      glEnd();

      /* Top surface  */
      cVertex.Set(LED_OUTER_RADIUS, 0.0f);
      CVector2 cVertex2(LED_UPPER_RING_INNER_RADIUS, 0.0f);
      glBegin(GL_QUAD_STRIP);
      glNormal3f(0.0f, 0.0f, 1.0f);      
      for(GLuint i = 0; i <= m_unVertices / 8; i++) {         
         glVertex3f(cVertex2.GetX(), cVertex2.GetY(), ZEBRO_ELEVATION + ZEBRO_HEIGHT + LED_HEIGHT);
         glVertex3f(cVertex.GetX(), cVertex.GetY(), ZEBRO_ELEVATION + ZEBRO_HEIGHT + LED_HEIGHT);
         cVertex.Rotate(cAngle);
         cVertex2.Rotate(cAngle);
      }
      glEnd();
   }

   /****************************************/
   /****************************************/

   class CQTOpenGLOperationDrawZebroNormal : public CQTOpenGLOperationDrawNormal {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CZebroEntity& c_entity) {
         static CQTOpenGLZebro m_cModel;
         c_visualization.DrawRays(c_entity.GetControllableEntity());
         c_visualization.DrawEntity(c_entity.GetEmbodiedEntity());
         m_cModel.Draw(c_entity);
      }
   };

   class CQTOpenGLOperationDrawZebroSelected : public CQTOpenGLOperationDrawSelected {
   public:
      void ApplyTo(CQTOpenGLWidget& c_visualization,
                   CZebroEntity& c_entity) {
         c_visualization.DrawBoundingBox(c_entity.GetEmbodiedEntity());
      }
   };

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawNormal, CQTOpenGLOperationDrawZebroNormal, CZebroEntity);

   REGISTER_QTOPENGL_ENTITY_OPERATION(CQTOpenGLOperationDrawSelected, CQTOpenGLOperationDrawZebroSelected, CZebroEntity);

   /****************************************/
   /****************************************/

}
