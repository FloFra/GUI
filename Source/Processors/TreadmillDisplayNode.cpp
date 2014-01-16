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

#include "TreadmillDisplayNode.h"

#include "Editors/TreadmillDisplayEditor.h"

TreadmillDisplayNode::TreadmillDisplayNode()
    : GenericProcessor("Treadmill Viewer"), bytesInBuffer(0)
{
}

TreadmillDisplayNode::~TreadmillDisplayNode()
{
}

AudioProcessorEditor* TreadmillDisplayNode::createEditor() {
    if (editor == nullptr)
        editor = new TreadmillDisplayEditor(this);

    return editor;
}


void TreadmillDisplayNode::process(AudioSampleBuffer&, MidiBuffer& events, int&)
{
    
    if (events.getNumEvents() > 0)
    {
        MidiBuffer::Iterator iterator(events);
        MidiMessage message(0xf4);
        
        int samplePosition = 0;
        iterator.setNextSamplePosition(samplePosition);
        
        // For each event message
        while (iterator.getNextEvent(message, samplePosition))
        {
            int messageLength = message.getRawDataSize() - 4;
            const uint8* rawMessage = message.getRawData();
            
            // We only handle serial data
            if(rawMessage[0] == SERIAL)
            {
                // Skip header
                rawMessage += 4;
                while(messageLength > 0)
                {
                    // Copy new data to buffer
                    int bytesToCopy = std::min(messageLength, 5 - bytesInBuffer);
                    memcpy(buffer + bytesInBuffer, rawMessage, bytesToCopy);
                    bytesInBuffer += bytesToCopy;
                    messageLength -= bytesToCopy;
                    rawMessage += bytesToCopy;
            
                    // If a whole data package was collected.
                    if(bytesInBuffer == 5)
                    {
                        // Check alignment
                        if (buffer[4] != '\n') // ToDo: Test for desynced and warn about them! States need to be reworked for this!
                        {
                            // Find endl character in buffer to align buffer.
                            int endlPosition;
                            for(endlPosition = 0; endlPosition < 5; endlPosition++)
                            {
                                if(buffer[endlPosition] == '\n')
                                    break;
                            }
                            
                            if(endlPosition == 5)
                            {
                                std::cout << "ERROR: Can not align buffer!" << std::endl;
                                bytesInBuffer = 0;
                                continue;
                            }
                            else
                            {
                                std::cout << "WARNING: Buffer needed to be (re-)aligned!" << std::endl;
                            }
                            bytesInBuffer = 4 - endlPosition;
                            memmove(buffer, buffer + endlPosition + 1, bytesInBuffer);
                        }
                        
                        // Send data to canvas if available
                        TreadmillDisplayEditor* ed = (TreadmillDisplayEditor*) getEditor();
                        if (ed->visualizer != nullptr){
                            ed->visualizer->addDataPoint(buffer[0], buffer[2], (buffer[1] + buffer[3]) / 2);
                        }
                        
                        // Clear buffer.
                        bytesInBuffer = 0;
                    }
                }
                    
            }
        }
        
    }
}