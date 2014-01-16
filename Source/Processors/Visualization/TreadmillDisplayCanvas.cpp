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

#include "TreadmillDisplayCanvas.h"

const Colour TreadmillDisplayCanvas::BACKGROUND_COLOUR = Colour(0, 18, 43); // dark bluer
const Colour TreadmillDisplayCanvas::GRID_COLOUR = Colour(25, 25, 60); // light blue

// ToDo: Find out why 'Colours::yellow' does not work here!
const Colour TreadmillDisplayCanvas::MARKER_COLOUR = Colour(255, 255, 0); // yellow
const Colour TreadmillDisplayCanvas::ARROW_COLOUR = Colour(255, 165, 0); // orange

const float TreadmillDisplayCanvas::SAMPLING_FREQUENCY = 90.9f; // in samples per minute
const float TreadmillDisplayCanvas::SENSOR_RESOLUTION = 395.0f; // in dpi


TreadmillDisplayCanvas::TreadmillDisplayCanvas(TreadmillDisplayNode* parentNode)
    : node(parentNode), maxSpeed(65), maxHistory(60)
{
    
}

TreadmillDisplayCanvas::~TreadmillDisplayCanvas()
{
    
}

float TreadmillDisplayCanvas::toCmPerS(int8 value)
{
    return 100 * value * 2.54 * SAMPLING_FREQUENCY / (100 * SENSOR_RESOLUTION);
}

Point<float> TreadmillDisplayCanvas::rescalePoint(Point<float> p)
{
    return Point<float>(getWidth() * (1 + p.getX() / maxSpeed) / 2.0f,
                        getHeight() * (1 - p.getY() / maxSpeed) / 2.0f);
}

void TreadmillDisplayCanvas::addDataPoint(int8 pitch, int8 roll, int8 yaw)
{
    datapoints.add(Point<float>(toCmPerS(pitch), toCmPerS(roll)));
    
    lastYaw = toCmPerS(yaw);
    
    if(datapoints.size() > maxHistory) {
        datapoints.remove(0);
    }
}

void TreadmillDisplayCanvas::paint(Graphics& g)
{
    
    // Compute scaling
    float xScaling = getWidth() / (2.0f * maxSpeed);
    float yScaling = getHeight() / (2.0f * maxSpeed);
    
    // Draw background
    g.fillAll(BACKGROUND_COLOUR);
    
    g.setColour(GRID_COLOUR);
    
    g.drawHorizontalLine(getHeight() * 0.5f, 0, getWidth());
    g.drawVerticalLine(getWidth() * 0.5f, 0, getHeight());
    
    
    for(int radius = 10; radius <= maxSpeed; radius += 10)
    {
        // Draw ring
        g.drawEllipse(xScaling * (maxSpeed - radius), yScaling * (maxSpeed - radius), 2.0f * radius * xScaling, 2.0f * radius * yScaling, 1.0f);

        // X-label
        g.drawText(String(radius), xScaling * (maxSpeed + radius) - 20, getHeight() / 2.0, 50, 20, Justification::left, false);
        // Y-label
        g.drawText(String(radius), getWidth() / 2.0 + 2, yScaling * (maxSpeed - radius), 50, 20, Justification::left, false);
    }
    
    if(datapoints.size() > 0)
    {
        Point<float> centerPoint(getWidth() / 2.0f, getHeight() / 2.0f);
        Point<float> currentPoint;
        
        // The last few measurements
        float markerSize = 5;
        g.setColour(MARKER_COLOUR);
        for(int i = 0; i < datapoints.size() - 1; i++)
        {
            currentPoint = rescalePoint(datapoints[i]);
            g.drawEllipse(currentPoint.getX() - markerSize / 2.0f, currentPoint.getY() - markerSize / 2.0f, markerSize, markerSize, 2);
        }
        
        // The current measurement
        g.setColour(ARROW_COLOUR);
        
        currentPoint = rescalePoint(datapoints.getLast());
        g.drawArrow (Line<float>(centerPoint, currentPoint), 2, 10, 10);
        
        // Add the arc for the last yaw value
        float radius = datapoints.getLast().getDistanceFromOrigin();
        float fromRadians = -Point<float>(0, 0).getAngleToPoint(datapoints.getLast()) + float_Pi;
        float toRadians = fromRadians + float_Pi * 2.0f * lastYaw / maxSpeed;
        
        Path p;
        p.addCentredArc(centerPoint.getX(), centerPoint.getY(), xScaling * radius, yScaling * radius, 0.0f, fromRadians, toRadians, true);
        g.strokePath(p, PathStrokeType(2));
    }
}