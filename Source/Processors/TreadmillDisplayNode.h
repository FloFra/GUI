/*
 ------------------------------------------------------------------
 
 This file is part of the Open Ephys GUI
 Copyright (C) 2013 Florian Franzen
 
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

#ifndef __TREADMILLDISPLAYNODE_H_A39331FD__
#define __TREADMILLDISPLAYNODE_H_A39331FD__

#include "../../JuceLibraryCode/JuceHeader.h"
#include "GenericProcessor.h"

class TreadmillDisplayNode :  public GenericProcessor
{
public:
    
    TreadmillDisplayNode();
    ~TreadmillDisplayNode();
    
    AudioProcessorEditor* createEditor();
    
    bool isSink()
    {
        return true;
    }
    
    void process(AudioSampleBuffer& buffer, MidiBuffer& midiMessages, int& nSamples);
        
    enum SyncState
    {
        UNSYNCED,
        SYNCING,
        SYNCED
    };

private:
    // Buffer that contains pitch, yaw1, roll, yaw2 and endl
    int8 buffer[5];
    
    SyncState state;
    
    int bytesInBuffer;
    
    JUCE_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(TreadmillDisplayNode);
};


#endif  // __TREADMILLDISPLAYNODE_H_A39331FD__
