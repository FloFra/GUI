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

#ifndef __TREADMILLDISPLAYEDITOR_H_4B4746D1__
#define __TREADMILLDISPLAYEDITOR_H_4B4746D1__

#include "../../../JuceLibraryCode/JuceHeader.h"
#include "VisualizerEditor.h"

#include "../Visualization/TreadmillDisplayCanvas.h"

class TreadmillDisplayNode;
class ParameterCheckbox;

/**
 
 User interface for the TreadmillDisplayEditor sink.
 
 @see TreadmillDisplayEditor, TreadmillDisplayCanvas
 
 */

class TreadmillDisplayEditor : public VisualizerEditor
{
public:
    TreadmillDisplayEditor(TreadmillDisplayNode*);
    ~TreadmillDisplayEditor();
    
    Visualizer* createNewCanvas();
    
    void buttonCallback(Button* button);
    void sliderEvent(Slider* slider);
    
    ScopedPointer<TreadmillDisplayCanvas> visualizer;
    
    // ToDo: These two should be part of VisualizerEditor, but are somehow not...
    
    /** Called just prior to the start of acquisition, to allow the editor to prepare.*/
    void startAcquisition() {
        enable();
    }

    /** Called after the end of acquisition.*/
    void stopAcquisition() {
        disable();
    }
    
private:
    
    ScopedPointer<ParameterCheckbox> smootherCheckbox;
    ScopedPointer<Slider> smootherSlider;
    ScopedPointer<Label> smootherLabel;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TreadmillDisplayEditor);
};


#endif  // __TREADMILLDISPLAYEDITOR_H_4B4746D1__
