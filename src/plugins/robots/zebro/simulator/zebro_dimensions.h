/**
 * @file <argos3/plugins/robots/zebro/simulator/zebro_dimensions.h>
 *
 * @author Matthijs den Toom - <thijsco4@gmail.com>
 *
 * This file contains all dimensions of the zebro's such that they can be used physics as well as graphics.
 */

namespace argos
{
    
    static const Real ZEBRO_WIDTH                = 0.176f;    // Units in meters.
    static const Real ZEBRO_LENGTH               = 0.268f;
    static const Real ZEBRO_HEIGHT               = 0.063f;
    static const Real ZEBRO_ELEVATION            = 0.05f;

    static const Real ZEBRO_MASS                = 2.1f;      // Mass in kg.

    static const Real ZEBRO_INTERWHEEL_DISTANCE = 0.176f;

    // This variable gives the width of the side tracks.
    static const Real SIDE_PANEL_WIDTH          = 0.01f;
    static const Real SIDE_PANEL_LENGTH         = 0.036f;

    // This variable gives the width of the caterpillar tracks.
    static const Real CATERPILLAR_TRACK_WIDTH   = 0.02f;
    static const Real CATERPILLAR_TRACK_HEIGHT  = 0.14f;
}
