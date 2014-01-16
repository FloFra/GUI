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

#include "TreadmillDisplayEditor.h"
#include "../TreadmillDisplayNode.h"
#include "ParameterEditor.h"


TreadmillDisplayEditor::TreadmillDisplayEditor(TreadmillDisplayNode* parentNode)
    : VisualizerEditor(parentNode, 200, false), visualizer(nullptr)
{
    tabText = "Treadmill";
    
    Font labelFont("Small Text", 14, Font::plain);

    // create checkbox
    smootherCheckbox = new ParameterCheckbox(false);
    smootherCheckbox->addListener(this);
    smootherCheckbox->setBounds(10, 30, 12, 12);
    addAndMakeVisible(smootherCheckbox);
    
    Label* label = new Label(String::empty, "enable smoother:");
    label->setColour(Label::textColourId, Colours::darkgrey);
    label->setFont(labelFont);
    label->setBounds(22, 30, 150, 12);
    addAndMakeVisible(label);
    
    smootherSlider = new Slider(Slider::IncDecButtons, Slider::TextBoxRight);
    smootherSlider->setTextBoxIsEditable(true);
    smootherSlider->setRange(2, 100, 1);
    smootherSlider->addListener(this);
    smootherSlider->setBounds(30, 52, 60, 15);
    smootherSlider->setEnabled(false);
    addAndMakeVisible(smootherSlider);
    
    smootherLabel = new Label(String::empty, "samples");
    smootherLabel->setBounds(90, 54, 100, 12);
    smootherLabel->setColour(Label::textColourId, Colours::darkgrey);
    smootherLabel->setEnabled(false);
    labelFont.setHeight(12);
    smootherLabel->setFont(labelFont);
    addAndMakeVisible(smootherLabel);
}

TreadmillDisplayEditor::~TreadmillDisplayEditor()
{
}

Visualizer* TreadmillDisplayEditor::createNewCanvas()
{
    if (visualizer == nullptr)
        visualizer = new TreadmillDisplayCanvas((TreadmillDisplayNode*) getProcessor());
        
    if (canvas == nullptr)
        canvas = visualizer;

    return canvas;
}

void TreadmillDisplayEditor::buttonCallback(Button* button)
{
    if(button == smootherCheckbox)
    {
        bool currentState = button->getToggleState();
        
        smootherSlider->setEnabled(currentState);
        smootherLabel->setEnabled(currentState);
        
        if(visualizer != nullptr)
            visualizer->setSmoother(currentState ? std::rint(smootherSlider->getValue()) : 0);
    }
}

void TreadmillDisplayEditor::sliderEvent(Slider* slider)
{
    std::cout << slider->getValue() << std::endl;
    
    if(slider == smootherSlider && smootherCheckbox->getToggleState())
    {
        if(visualizer != nullptr)
            visualizer->setSmoother(std::rint(slider->getValue()));
    }
}
