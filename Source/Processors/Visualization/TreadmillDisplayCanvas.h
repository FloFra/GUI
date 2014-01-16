/*
 ------------------------------------------------------------------
 
 This file is part of the Open Ephys GUI
 Copyright (C) 2014 Florian Franzen
 
 ------------------------------------------------------------------
 
 This program is free software: you can redistribute it and/or modify
 it under the terms of the GNU General Public License as published by
 the Free Software Foundation, either version 3 of the License, or
 (at your option) any later version.
 
 This program is distributed in the hope that it will be useful,
 but WITHOUT ANY WARRANTY; without even the implied warranty of
 MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 GNU General Public License for more details.
 
 You should have received a copy of the GNU General Public License
 along with this program.  If not, see <http://www.gnu.org/licenses/>.
 
 */

#ifndef __TREADMILLDISPLAYCANVAS_H_3E118D7E__
#define __TREADMILLDISPLAYCANVAS_H_3E118D7E__

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "Visualizer.h"

#include "../TreadmillDisplayNode.h"

class TreadmillDisplayCanvas : public Visualizer
{
public:
    TreadmillDisplayCanvas(TreadmillDisplayNode*);
    ~TreadmillDisplayCanvas();
    
    /** Called when the component's tab becomes visible again.*/
    void refreshState() {}
    
    /** Called when parameters of underlying data processor are changed.*/
    void update() {}
    
    /** Called by an editor to initiate a parameter change.*/
    void setParameter(int, float) {}
    
    /** Called by an editor to initiate a parameter change.*/
    void setParameter(int, int, int, float) {}
    
    /** Called instead of "repaint" to avoid redrawing underlying components if not necessary.*/
    void refresh()
    {
        repaint();
    }
    
    /** Called when data acquisition is active.*/
    void beginAnimation()
    {
        startCallbacks();
    }
    
    /** Called when data acquisition ends.*/
    void endAnimation()
    {
        stopCallbacks();
    }
    
    // Add datapoint so it can be drawn later.
    void addDataPoint(int8 pitch, int8 roll, int8 yaw);
    
    //
    void setSmoother(int n)
    {
        smootherSampleCount = n;
        if(smootherSampleCount > 0)
        {
            AlertWindow::showMessageBoxAsync(AlertWindow::WarningIcon, "Smoother not implemented yet!", "Sorry! ;-)");
        }
    }
    
    /* @internal */
    void paint(Graphics& g);

private:
    // translates sensor readings to cm/s
    float toCmPerS(int8 value);
    
    // rescales a point from the shown to the internal coordinate system
    Point<float> rescalePoint(Point<float> p);
    
    
    TreadmillDisplayNode* node;
    
    // data points
    Array<Point<float>> datapoints;
    
    float lastYaw;
    
    // scaling config
    int maxSpeed;
    int maxHistory;
    
    int smootherSampleCount;
    
    // calibration
    static const float SAMPLING_FREQUENCY; // in samples per minute
    static const float SENSOR_RESOLUTION; // in dpi
    
    // colour config
    static const Colour BACKGROUND_COLOUR;
    static const Colour GRID_COLOUR;
    static const Colour MARKER_COLOUR;
    static const Colour ARROW_COLOUR;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TreadmillDisplayCanvas);
};


#endif  // __TREADMILLDISPLAYCANVAS_H_3E118D7E__
