/**********************************************************************/
/*! \class RtMidi
    \brief An abstract base class for realtime MIDI input/output.

    This class implements some common functionality for the realtime
    MIDI input/output subclasses RtMidiIn and RtMidiOut.

    RtMidi WWW site: http://music.mcgill.ca/~gary/rtmidi/

    RtMidi: realtime MIDI i/o C++ classes
    Copyright (c) 2003-2010 Gary P. Scavone

    Permission is hereby granted, free of charge, to any person
    obtaining a copy of this software and associated documentation files
    (the "Software"), to deal in the Software without restriction,
    including without limitation the rights to use, copy, modify, merge,
    publish, distribute, sublicense, and/or sell copies of the Software,
    and to permit persons to whom the Software is furnished to do so,
    subject to the following conditions:

    The above copyright notice and this permission notice shall be
    included in all copies or substantial portions of the Software.

    Any person wishing to distribute modifications to the Software is
    requested to send the modifications to the original developer so that
    they can be incorporated into the canonical version.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
    EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
    MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
    IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
    ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
    CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
    WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/**********************************************************************/

#include "include/rtmidi.h"
#include <sstream>
#include <stdio.h>

#define  RT_SYSEX_BUFFER_SIZE 4096

//*********************************************************************//
//  Common RtMidi Definitions
//*********************************************************************//

RtMidi :: RtMidi() : apiData_(0), connected_(false)
{
#if defined(__AROS__)
    CamdBase = OpenLibrary("camd.library", 0L);
#endif
}

void RtMidi :: error(RtError::Type type)
{
    printf("%s\n",errorString_);
}

//*********************************************************************//
//  Common RtMidiIn Definitions
//*********************************************************************//

RtMidiIn :: RtMidiIn(char *clientName) : RtMidi()
{
    this->initialize(clientName);
}

void RtMidiIn :: setCallback(RtMidiCallback callback, void *userData)
{
    if(inputData_.usingCallback)
    {
        sprintf(errorString_, "RtMidiIn::setCallback: a callback function is already set!");
        error(RtError::WARNING);
        return;
    }

    if(!callback)
    {
        sprintf(errorString_, "RtMidiIn::setCallback: callback function value is invalid!");
        error(RtError::WARNING);
        return;
    }

    inputData_.userCallback = (void *) callback;
    inputData_.userData = userData;
    inputData_.usingCallback = true;
}

void RtMidiIn :: cancelCallback()
{
    if(!inputData_.usingCallback)
    {
        sprintf(errorString_, "RtMidiIn::cancelCallback: no callback function was set!");
        error(RtError::WARNING);
        return;
    }

    inputData_.userCallback = 0;
    inputData_.userData = 0;
    inputData_.usingCallback = false;
}

void RtMidiIn :: setQueueSizeLimit(unsigned int queueSize)
{
    inputData_.queueLimit = queueSize;
}

void RtMidiIn :: ignoreTypes(int midiSysex,
                             int midiTime,
                             int midiSense)
{
    inputData_.ignoreFlags = 0;
    if(midiSysex) inputData_.ignoreFlags = 0x01;
    if(midiTime) inputData_.ignoreFlags |= 0x02;
    if(midiSense) inputData_.ignoreFlags |= 0x04;
}

double RtMidiIn :: getMessage(std::vector<unsigned char> *message)
{
    message->clear();

    if(inputData_.usingCallback)
    {
        sprintf(errorString_, "RtMidiIn::getNextMessage: a user callback is currently set for this port.");
        error(RtError::WARNING);
        return 0.0;
    }

    if(inputData_.queue.size() == 0)
    {
        return 0.0;
    }

    // Copy queued message to the vector pointer argument and then "pop" it.
    std::vector<unsigned char> *bytes = &(inputData_.queue.front().bytes);
    message->assign( bytes->begin(), bytes->end() );
    double deltaTime = inputData_.queue.front().timeStamp;
    inputData_.queue.pop();

    return deltaTime;
}

//*********************************************************************//
//  Common RtMidiOut Definitions
//*********************************************************************//

RtMidiOut :: RtMidiOut(char *clientName) : RtMidi()
{
    this->initialize(clientName);
}

//*********************************************************************//
//  API: Macintosh OS-X
//*********************************************************************//

// API information found at:
//   - http://developer. apple .com/audio/pdf/coreaudio.pdf 

#if defined(__MACOSX_CORE__)

// The CoreMIDI API is based on the use of a callback function for
// MIDI input.  We convert the system specific time stamps to delta
// time values.

// OS-X CoreMIDI header files.
#include <CoreMIDI/CoreMIDI.h>
#include <CoreAudio/HostTime.h>
#include <CoreServices/CoreServices.h>

// A structure to hold variables related to the CoreMIDI API
// implementation.
struct CoreMidiData
{
    MIDIClientRef client;
    MIDIPortRef port;
    MIDIEndpointRef endpoint;
    MIDIEndpointRef destinationId;
    unsigned long long lastTime;
    MIDISysexSendRequest sysexreq;
};

static MIDIClientRef CoreMidiClientSingleton = 0;

void RtMidi_setCoreMidiClientSingleton(MIDIClientRef client)
{
    CoreMidiClientSingleton = client;
}

void RtMidi_disposeCoreMidiClientSingleton()
{
    if(CoreMidiClientSingleton == 0)
    {
        return;
    }
    MIDIClientDispose(CoreMidiClientSingleton);
    CoreMidiClientSingleton = 0;
}

//*********************************************************************//
//  API: OS-X
//  Class Definitions: RtMidiIn
//*********************************************************************//

void midiInputCallback(const MIDIPacketList *list, void *procRef, void *srcRef)
{
    RtMidiIn::RtMidiInData *data = static_cast<RtMidiIn::RtMidiInData *> (procRef);
    CoreMidiData *apiData = static_cast<CoreMidiData *> (data->apiData);

    unsigned char status;
    unsigned short nBytes, iByte, size;
    unsigned long long time;

    bool &continueSysex = data->continueSysex;
    RtMidiIn::MidiMessage &message = data->message;

    const MIDIPacket *packet = &list->packet[0];
    for(unsigned int i = 0; i < list->numPackets; ++i)
    {
        // My interpretation of the CoreMIDI documentation: all message
        // types, except sysex, are complete within a packet and there may
        // be several of them in a single packet.  Sysex messages can be
        // broken across multiple packets and PacketLists but are bundled
        // alone within each packet (these packets do not contain other
        // message types).  If sysex messages are split across multiple
        // MIDIPacketLists, they must be handled by multiple calls to this
        // function.

        nBytes = packet->length;
        if(nBytes == 0)
        {
            packet = MIDIPacketNext(packet);
            continue;
        }

        // Calculate time stamp.
        if(data->firstMessage)
        {
            message.timeStamp = 0.0;
            data->firstMessage = false;
        }
        else
        {
            time = packet->timeStamp;
            if(time == 0)
            {   // this happens when receiving asynchronous sysex messages
                time = AudioGetCurrentHostTime();
            }
            time -= apiData->lastTime;
            time = AudioConvertHostTimeToNanos(time);
            if(!continueSysex)
            {
                message.timeStamp = time * 0.000000001;
            }
        }

        // Track whether any non-filtered messages were found in this
        // packet for timestamp calculation
        bool foundNonFiltered = false;

        iByte = 0;
        if(continueSysex)
        {
            // We have a continuing, segmented sysex message.
            if(!(data->ignoreFlags & 0x01))
            {
                // If we're not ignoring sysex messages, copy the entire packet.
                for(unsigned int j = 0; j < nBytes; j++)
                {
                    message.bytes.push_back(packet->data[j]);
                }
            }
            continueSysex = packet->data[nBytes - 1] != 0xF7;

            if(!(data->ignoreFlags & 0x01) && !continueSysex)
            {
                // If not a continuing sysex message, invoke the user callback function or queue the message.
                if(data->usingCallback)
                {
                    RtMidiIn::RtMidiCallback callback = (RtMidiIn::RtMidiCallback) data->userCallback;
                    callback(message.timeStamp, &message.bytes, data->userData);
                }
                else
                {
                    // As long as we haven't reached our queue size limit, push the message.
                    if(data->queueLimit > data->queue.size())
                    {
                        data->queue.push(message);
                    }
                }
                message.bytes.clear();
            }
        }
        else
        {
            while(iByte < nBytes)
            {
                size = 0;
                // We are expecting that the next byte in the packet is a status byte.
                status = packet->data[iByte];
                if(!(status & 0x80)) break;
                // Determine the number of bytes in the MIDI message.
                if(status < 0xC0) size = 3;
                else if(status < 0xE0) size = 2;
                else if(status < 0xF0) size = 3;
                else if(status == 0xF0)
                {
                    // A MIDI sysex
                    if(data->ignoreFlags & 0x01)
                    {
                        size = 0;
                        iByte = nBytes;
                    }
                    else
                    {
                        size = nBytes - iByte;
                    }
                    continueSysex =  packet->data[nBytes - 1] != 0xF7;
                }
                else if(status == 0xF1)
                {
                    if(data->ignoreFlags & 0x02)
                    {
                        size = 0;
                        iByte += 2;
                    }
                    else
                    {
                        size = 2;
                    }
                }
                else if(status == 0xF2) size = 3;
                else if(status == 0xF3) size = 2;
                else if(status == 0xF8 && (data->ignoreFlags & 0x02))
                {
                    // A MIDI timing tick message and we're ignoring it.
                    size = 0;
                    iByte += 1;
                }
                else if(status == 0xFE && (data->ignoreFlags & 0x04 ))
                {
                    // A MIDI active sensing message and we're ignoring it.
                    size = 0;
                    iByte += 1;
                }
                else
                {
                    size = 1;
                }

                // Copy the MIDI data to our vector.
                if(size)
                {
                    foundNonFiltered = true;
                    message.bytes.assign(&packet->data[iByte], &packet->data[iByte + size]);
                    if(!continueSysex)
                    {
                        // If not a continuing sysex message, invoke the user callback function or queue the message.
                        if(data->usingCallback)
                        {
                            RtMidiIn::RtMidiCallback callback = (RtMidiIn::RtMidiCallback) data->userCallback;
                            callback(message.timeStamp, &message.bytes, data->userData);
                        }
                        else
                        {
                            // As long as we haven't reached our queue size limit, push the message.
                            if(data->queueLimit > data->queue.size())
                            {
                                data->queue.push( message );
                            }
                        }
                        message.bytes.clear();
                        // All subsequent messages within same MIDI packet will have time delta 0
                        message.timeStamp = 0.0;
                    }
                    iByte += size;
                }
            }
        }

        // Save the time of the last non-filtered message
        if(foundNonFiltered)
        {
            apiData->lastTime = packet->timeStamp;
            if(apiData->lastTime == 0)
            {   // this happens when receiving asynchronous sysex messages
                apiData->lastTime = AudioGetCurrentHostTime();
            }
        }

        packet = MIDIPacketNext(packet);
    }
}

void RtMidiIn :: initialize(char *clientName)
{
    // Set up our client.
    MIDIClientRef client;
    OSStatus result = MIDIClientCreate(CFStringCreateWithCString(NULL,
                                                                 clientName,
                                                                 kCFStringEncodingASCII),
                                                                 NULL,
                                                                 NULL,
                                                                 &client);
    if(result != noErr)
    {
        sprintf(errorString_, "RtMidiIn::initialize: error creating OS-X MIDI client object.");
        error(RtError::DRIVER_ERROR);
        return;
    }

    // Save our api-specific connection information.
    CoreMidiData *data = (CoreMidiData *) new CoreMidiData;
    data->client = client;
    data->endpoint = 0;
    apiData_ = (void *) data;
    inputData_.apiData = (void *) data;
}

void RtMidiIn :: openPort(unsigned int portNumber, char *portName)
{
    if(connected_)
    {
        sprintf(errorString_, "RtMidiIn::openPort: a valid connection already exists!");
        error(RtError::WARNING);
        return;
    }

    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
    unsigned int nSrc = MIDIGetNumberOfSources();
    if(nSrc < 1)
    {
        sprintf(errorString_, "RtMidiIn::openPort: no MIDI input sources found!");
        error(RtError::NO_DEVICES_FOUND);
        return;
    }

    if(portNumber >= nSrc)
    {
        sprintf(errorString_, "RtMidiIn::openPort: the 'portNumber' argument (%d) is invalid.", portNumber);
        error(RtError::INVALID_PARAMETER);
        return;
    }

    MIDIPortRef port;
    CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);
    OSStatus result = MIDIInputPortCreate(data->client, 
                                          CFStringCreateWithCString(NULL,
                                                                    portName,
                                                                    kCFStringEncodingASCII),
                                          midiInputCallback,
                                          (void *) &inputData_,
                                          &port);
    if(result != noErr)
    {
        MIDIClientDispose(data->client);
        sprintf(errorString_, "RtMidiIn::openPort: error creating OS-X MIDI input port.");
        error(RtError::DRIVER_ERROR);
        return;
    }

    // Get the desired input source identifier.
    MIDIEndpointRef endpoint = MIDIGetSource(portNumber);
    if(endpoint == 0)
    {
        MIDIPortDispose(port);
        MIDIClientDispose(data->client);
        sprintf(errorString_, "RtMidiIn::openPort: error getting MIDI input source reference.");
        error(RtError::DRIVER_ERROR);
        return;
    }

    // Make the connection.
    result = MIDIPortConnectSource(port, endpoint, NULL);
    if(result != noErr)
    {
        MIDIPortDispose(port);
        MIDIClientDispose(data->client);
        sprintf(errorString_, "RtMidiIn::openPort: error connecting OS-X MIDI input port.");
        error(RtError::DRIVER_ERROR);
        return;
    }

    // Save our api-specific port information.
    data->port = port;
    connected_ = true;
}

void RtMidiIn :: openVirtualPort(char *portName)
{
    CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);

    // Create a virtual MIDI input destination.
    MIDIEndpointRef endpoint;
    
    OSStatus result = MIDIDestinationCreate(data->client,
                                            CFStringCreateWithCString(NULL,
                                                                      portName,
                                                                      kCFStringEncodingASCII),
                                            midiInputCallback,
                                            (void *) &inputData_,
                                            &endpoint);
    if(result != noErr)
    {
        sprintf(errorString_, "RtMidiIn::openVirtualPort: error creating virtual OS-X MIDI destination.");
        error(RtError::DRIVER_ERROR);
        return;
    }

    // Save our api-specific connection information.
    data->endpoint = endpoint;
}

void RtMidiIn :: closePort(void)
{
    if(connected_)
    {
        CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);
        MIDIPortDispose(data->port);
        data->port = 0;
    }
    connected_ = false;
}

RtMidiIn :: ~RtMidiIn()
{
    // Close a connection if it exists.
    closePort();

    // Cleanup.
    CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);
    if(data->client)
    {
        MIDIClientDispose(data->client);
    }
    if(data->endpoint)
    {
        MIDIEndpointDispose(data->endpoint);
        data->endpoint = 0;
    }
    delete data;
}

unsigned int RtMidiIn :: getPortCount()
{
    return MIDIGetNumberOfSources();
}

// This function was submitted by Douglas Casey Tucker and apparently
// derived largely from PortMidi.
CFStringRef EndpointName(MIDIEndpointRef endpoint, bool isExternal)
{
    CFMutableStringRef result = CFStringCreateMutable(NULL, 0);
    CFStringRef str;

    // Begin with the endpoint's name.
    str = NULL;
    MIDIObjectGetStringProperty(endpoint, kMIDIPropertyName, &str);
    if(str != NULL)
    {
        CFStringAppend(result, str);
        CFRelease(str);
    }
    
    // some MIDI devices have a leading space in endpoint name. trim
    CFStringTrim(result, CFSTR(" "));

    MIDIEntityRef entity = NULL;
    MIDIEndpointGetEntity(endpoint, &entity);
    if(entity == 0)
    {
        // probably virtual
        return result;
    }
    
    if(CFStringGetLength(result) == 0)
    {
        // endpoint name has zero length -- try the entity
        str = NULL;
        MIDIObjectGetStringProperty(entity, kMIDIPropertyName, &str);
        if(str != NULL)
        {
            CFStringAppend(result, str);
        }
    }

    // now consider the device's name
    MIDIDeviceRef device = 0;
    MIDIEntityGetDevice(entity, &device);
    if(device == 0)
    {
        return result;
    }

    str = NULL;
    MIDIObjectGetStringProperty(device, kMIDIPropertyName, &str);
    if(CFStringGetLength(result) == 0)
    {
        CFRelease(result);
        CFRetain(str);
        return str;
    }
    if(str != NULL)
    {
        // if an external device has only one entity, throw away
        // the endpoint name and just use the device name
        if(isExternal && MIDIDeviceGetNumberOfEntities(device) < 2)
        {
            CFRelease(result);
            CFRetain(str);
            return str;
        }
        else
        {
            if(CFStringGetLength(str) == 0)
            {
                return result;
            }
            // does the entity name already start with the device name?
            // (some drivers do this though they shouldn't)
            // if so, do not prepend
            if(CFStringCompareWithOptions(result, /* endpoint name */
                                          str /* device name */,
                                          CFRangeMake(0, CFStringGetLength(str)), 0) != kCFCompareEqualTo)
            {
                // prepend the device name to the entity name
                if(CFStringGetLength(result) > 0)
                {
                    CFStringInsert(result, 0, CFSTR(" "));
                }
                CFStringInsert(result, 0, str);
            }
            CFRelease(str);
        }
    }
    return result;
}

// This function was submitted by Douglas Casey Tucker and apparently
// derived largely from PortMidi.
static CFStringRef ConnectedEndpointName(MIDIEndpointRef endpoint)
{
    CFMutableStringRef result = CFStringCreateMutable(NULL, 0);
    CFStringRef str;
    OSStatus err;
    int i;

    // Does the endpoint have connections?
    CFDataRef connections = NULL;
    int nConnected = 0;
    bool anyStrings = false;

    err = MIDIObjectGetDataProperty(endpoint, kMIDIPropertyConnectionUniqueID, &connections);
    if(connections != NULL)
    {
        // It has connections, follow them
        // Concatenate the names of all connected devices
        nConnected = CFDataGetLength(connections) / sizeof(MIDIUniqueID);
        if(nConnected)
        {
            const SInt32 *pid = (const SInt32 *) (CFDataGetBytePtr(connections));

            for(i = 0; i < nConnected; ++i, ++pid)
            {
                MIDIUniqueID id = EndianS32_BtoN(*pid);
                MIDIObjectRef connObject;
                MIDIObjectType connObjectType;
                
                err = MIDIObjectFindByUniqueID(id, &connObject, &connObjectType);
                if(err == noErr)
                {
                    if(connObjectType == kMIDIObjectType_ExternalSource ||
                       connObjectType == kMIDIObjectType_ExternalDestination)
                    {
                        // Connected to an external device's endpoint (10.3 and later).
                        str = EndpointName((MIDIEndpointRef)(connObject), true);
                    }
                    else
                    {
                        // Connected to an external device (10.2) (or something else, catch-
                        str = NULL;
                        MIDIObjectGetStringProperty(connObject, kMIDIPropertyName, &str);
                        if(str)
                        {
                            CFRetain(str);
                        }
                    }
                    if(str != NULL)
                    {
                        if(anyStrings)
                        {
                            CFStringAppend(result, CFSTR(", "));
                        }
                        else
                        {
                            anyStrings = true;
                        }
                        CFStringAppend(result, str);
                        CFRelease(str);
                    }
                }
            }
        }
        CFRelease(connections);
    }
    if(anyStrings)
    {
        return result;
    }

    CFRelease(result);

    // Here, either the endpoint had no connections, or we failed to obtain names 
    return EndpointName(endpoint, false);
}

std::string RtMidiIn :: getPortName(unsigned int portNumber, char *Name)
{
    CFStringRef nameRef;
    MIDIEndpointRef portRef;
    char name[128];

    if(portNumber >= MIDIGetNumberOfSources())
    {
        sprintf(errorString_, "RtMidiIn::getPortName: the 'portNumber' argument (%d) is invalid.", portNumber);
        error(RtError::INVALID_PARAMETER);
        return 0;
    }
    portRef = MIDIGetSource(portNumber);
    nameRef = ConnectedEndpointName(portRef);

    CFStringGetCString(nameRef, name, sizeof(name), kCFStringEncodingUTF8);
    CFRelease(nameRef);
    std::string stringName = name;
    strcpy(Name, name);
    return stringName;
}

//*********************************************************************//
//  API: OS-X
//  Class Definitions: RtMidiOut
//*********************************************************************//

unsigned int RtMidiOut :: getPortCount()
{
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
    return MIDIGetNumberOfDestinations();
}

std::string RtMidiOut :: getPortName(unsigned int portNumber, char *Name)
{
    CFStringRef nameRef;
    MIDIEndpointRef portRef;
    char name[128];

    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
    if(portNumber >= MIDIGetNumberOfDestinations())
    {
        sprintf(errorString_, "RtMidiOut::getPortName: the 'portNumber' argument (%d) is invalid.", portNumber);
        error(RtError::INVALID_PARAMETER);
        return 0;
    }
    portRef = MIDIGetDestination(portNumber);
    nameRef = ConnectedEndpointName(portRef);

    CFStringGetCString(nameRef, name, sizeof(name), kCFStringEncodingUTF8);
    CFRelease(nameRef);
    std::string stringName = name;
    strcpy(Name, name);
    return stringName;
}

void RtMidiOut :: initialize(char *clientName)
{
    // Set up our client.
    MIDIClientRef client;
    OSStatus result = MIDIClientCreate(CFStringCreateWithCString(NULL,
                                                                 clientName,
                                                                 kCFStringEncodingASCII),
                                       NULL,
                                       NULL,
                                       &client);
    if(result != noErr)
    {
        sprintf(errorString_, "RtMidiOut::initialize: error creating OS-X MIDI client object.");
        error(RtError::DRIVER_ERROR);
        return;
    }

    // Save our api-specific connection information.
    CoreMidiData *data = (CoreMidiData *) new CoreMidiData;
    data->client = client;
    data->endpoint = 0;
    apiData_ = (void *) data;
}

void RtMidiOut :: openPort(unsigned int portNumber, char *portName)
{
    if(connected_)
    {
        sprintf(errorString_, "RtMidiOut::openPort: a valid connection already exists!");
        error(RtError::WARNING);
        return;
    }

    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
    unsigned int nDest = MIDIGetNumberOfDestinations();
    if(nDest < 1)
    {
        sprintf(errorString_, "RtMidiOut::openPort: no MIDI output destinations found!");
        error(RtError::NO_DEVICES_FOUND);
        return;
    }

    if(portNumber >= nDest)
    {
        sprintf(errorString_, "RtMidiOut::openPort: the 'portNumber' argument (%d) is invalid.", portNumber);
        error(RtError::INVALID_PARAMETER);
        return;
    }

    MIDIPortRef port;
    CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);
    OSStatus result = MIDIOutputPortCreate(data->client, 
                                           CFStringCreateWithCString(NULL,
                                                                     portName,
                                                                     kCFStringEncodingASCII),
                                           &port);
    if(result != noErr)
    {
        MIDIClientDispose(data->client);
        sprintf(errorString_, "RtMidiOut::openPort: error creating OS-X MIDI output port.");
        error(RtError::DRIVER_ERROR);
        return;
    }

    // Get the desired output port identifier.
    MIDIEndpointRef destination = MIDIGetDestination(portNumber);
    if(destination == NULL)
    {
        MIDIPortDispose(port);
        MIDIClientDispose(data->client);
        data->client = 0;
        sprintf(errorString_, "RtMidiOut::openPort: error getting MIDI output destination reference.");
        error(RtError::DRIVER_ERROR);
        return;
    }

    // Save our api-specific connection information.
    data->port = port;
    data->destinationId = destination;
    connected_ = true;
}

void RtMidiOut :: closePort(void)
{
    if(connected_)
    {
        CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);
        if(data->port)
        {
            MIDIPortDispose(data->port);
            data->port = 0;
        }
        connected_ = false;
    }
}

void RtMidiOut :: openVirtualPort(char *portName)
{
    CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);

    if(data->endpoint)
    {
        sprintf(errorString_, "RtMidiOut::openVirtualPort: a virtual output port already exists!");
        error(RtError::WARNING);
        return;
    }

    // Create a virtual MIDI output source.
    MIDIEndpointRef endpoint;
    OSStatus result = MIDISourceCreate(data->client,
                                       CFStringCreateWithCString(NULL,
                                                                 portName,
                                                                 kCFStringEncodingASCII),
                                       &endpoint);
    if(result != noErr)
    {
        sprintf(errorString_, "RtMidiOut::initialize: error creating OS-X virtual MIDI source.");
        error(RtError::DRIVER_ERROR);
        return;
    }

    // Save our api-specific connection information.
    data->endpoint = endpoint;
}

RtMidiOut :: ~RtMidiOut()
{
    // Close a connection if it exists.
    closePort();

    // Cleanup.
    CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);
    if(data->client)
    {
        MIDIClientDispose(data->client);
        data->client = 0;
    }
    if(data->endpoint)
    {
        MIDIEndpointDispose(data->endpoint);
    }
    delete data;
}

void RtMidiOut :: sendMessage(std::vector<unsigned char> *message)
{
    int i;
    // The CoreMidi documentation indicates a maximum PackList size of
    // 64K, so we may need to break long sysex messages into pieces and
    // send via separate lists.
    unsigned int nBytes = message->size();
    
    if(nBytes == 0)
    {
        sprintf(errorString_, "RtMidiOut::sendMessage: no data in message argument!");
        error(RtError::WARNING);
        return;
    }

    MIDITimeStamp timeStamp = 0;//AudioGetCurrentHostTime();
    CoreMidiData *data = static_cast<CoreMidiData *> (apiData_);
    OSStatus result;

    ByteCount bufsize = nBytes > 65535 ? 65535 : nBytes;
    Byte buffer[bufsize + 1024]; // pad for other struct members
    ByteCount listSize = sizeof(buffer);
    MIDIPacketList *packetList = (MIDIPacketList *) buffer;
  
    ByteCount remainingBytes = nBytes;

    while(remainingBytes)
    {
        MIDIPacket *packet = MIDIPacketListInit(packetList);
        
        // A MIDIPacketList can only contain a maximum of 64K of data, so if our message is longer,
        // break it up into chunks of 64K or less and send out as a MIDIPacketList with only one
        // MIDIPacket. Here, we reuse the memory allocated above on the stack for all.
        ByteCount bytesForPacket = remainingBytes > 65535 ? 65535 : remainingBytes;
        unsigned char *dataStartPtr = (unsigned char *) &message[nBytes - remainingBytes];
        for(i = 0; i < remainingBytes; i++)
        {
            printf("%x ", (int) dataStartPtr[i]);
        }
        printf("TEST %d %d %d\n", nBytes, remainingBytes, bytesForPacket);
        packet = MIDIPacketListAdd(packetList, listSize, packet, timeStamp, bytesForPacket, dataStartPtr);
        remainingBytes -= bytesForPacket;
        printf("REM %d\n", remainingBytes);

        if(!packet)
        {
            sprintf(errorString_, "RtMidiOut::sendMessage: could not allocate packet list");
            error(RtError::DRIVER_ERROR);
            return;
        }

        // Send to any destinations that may have connected to us.
        if(data->endpoint)
        {
            result = MIDIReceived(data->endpoint, packetList);
            if(result != noErr)
            {
                sprintf(errorString_, "RtMidiOut::sendMessage: error sending MIDI to virtual destinations.");
                error(RtError::WARNING);
            }
        }

        // And send to an explicit destination port if we're connected.
        if(connected_)
        {
            result = MIDISend(data->port, data->destinationId, packetList);
            if(result != noErr)
            {
                sprintf(errorString_, "RtMidiOut::sendMessage: error sending MIDI message to port.");
                error(RtError::WARNING);
            }
        }
    }
}

#endif  // __MACOSX_CORE__

//*********************************************************************//
//  API: LINUX ALSA SEQUENCER
//*********************************************************************//

// API information found at:
//   - http://www.alsa-project.org/documentation.php#Library

#if defined(__LINUX_ALSASEQ__)

// The ALSA Sequencer API is based on the use of a callback function for
// MIDI input.
//
// Thanks to Pedro Lopez-Cabanillas for help with the ALSA sequencer
// time stamps and other assorted fixes!!!

// If you don't need timestamping for incoming MIDI events, define the
// preprocessor definition AVOID_TIMESTAMPING to save resources
// associated with the ALSA sequencer queues.

#include <pthread.h>
#include <sys/time.h>

// ALSA header file.
#include <alsa/asoundlib.h>

// A structure to hold variables related to the ALSA API
// implementation.
struct AlsaMidiData
{
    snd_seq_t *seq;
    unsigned int client_id;
    int vport;
    snd_seq_port_subscribe_t *subscription;
    snd_midi_event_t *coder;
    unsigned int bufferSize;
    unsigned int requestedBufferSize;
    unsigned char *buffer;
    pthread_t thread;
    pthread_t dummy_thread_id;
    snd_seq_real_time_t lastTime;
    int queue_id; // an input queue is needed to get timestamped events
    int trigger_fds[2];
};

#define PORT_TYPE(pinfo, bits) ((snd_seq_port_info_get_capability(pinfo) & (bits)) == (bits))

//*********************************************************************//
//  API: LINUX ALSA
//  Class Definitions: RtMidiIn
//*********************************************************************//

extern "C" void *alsaMidiHandler(void *ptr)
{
    RtMidiIn::RtMidiInData *data = static_cast<RtMidiIn::RtMidiInData *> (ptr);
    AlsaMidiData *apiData = static_cast<AlsaMidiData *> (data->apiData);

    long nBytes;
    double time;
    bool continueSysex = false;
    bool doDecode = false;
    RtMidiIn::MidiMessage message;
    int poll_fd_count;
    struct pollfd *poll_fds;

    snd_seq_event_t *ev;
    int result;
    apiData->bufferSize = 32;
    result = snd_midi_event_new(0, &apiData->coder);

    if(result < 0)
    {
        data->doInput = false;
        return 0;
    }
    unsigned char *buffer = (unsigned char *) malloc(apiData->bufferSize);
    if(buffer == NULL)
    {
        data->doInput = false;
        snd_midi_event_free(apiData->coder);
        apiData->coder = 0;
        return 0;
    }

    snd_midi_event_init(apiData->coder);
    snd_midi_event_no_status(apiData->coder, 1); // suppress running status messages

    poll_fd_count = snd_seq_poll_descriptors_count( apiData->seq, POLLIN) + 1;
    poll_fds = (struct pollfd *) alloca(poll_fd_count * sizeof(struct pollfd));
    snd_seq_poll_descriptors(apiData->seq, poll_fds + 1, poll_fd_count - 1, POLLIN);
    poll_fds[0].fd = apiData->trigger_fds[0];
    poll_fds[0].events = POLLIN;

    while(data->doInput)
    {
        if(snd_seq_event_input_pending(apiData->seq, 1) == 0)
        {
            // No data pending
            if(poll(poll_fds, poll_fd_count, -1) >= 0)
            {
                if(poll_fds[0].revents & POLLIN)
                {
                    bool dummy;
                    int res = read(poll_fds[0].fd, &dummy, sizeof(dummy));
                    (void) res;
                }
            }
            continue;
        }

        // If here, there should be data.
        result = snd_seq_event_input(apiData->seq, &ev);
        if(result == -ENOSPC)
        {
            continue;
        }
        else if(result <= 0)
        {
            continue;
        }

        // This is a bit weird, but we now have to decode an ALSA MIDI
        // event (back) into MIDI bytes.  We'll ignore non-MIDI types.
        if(!continueSysex)
        {
            message.bytes.clear();
        }

        doDecode = false;
        switch(ev->type)
        {
		    case SND_SEQ_EVENT_PORT_SUBSCRIBED:
                break;

    		case SND_SEQ_EVENT_PORT_UNSUBSCRIBED:
                break;

            case SND_SEQ_EVENT_QFRAME: // MIDI time code
                if(data->ignoreFlags & 0x02)
                {
                    doDecode = true;
                    break;
                }

            case SND_SEQ_EVENT_TICK: // MIDI timing tick
                if(data->ignoreFlags & 0x02)
                {
                    doDecode = true;
                    break;
                }

            case SND_SEQ_EVENT_SENSING: // Active sensing
                if(data->ignoreFlags & 0x04)
                {
                    doDecode = true;
                    break;
                }

		    case SND_SEQ_EVENT_SYSEX:
                if((data->ignoreFlags & 0x01))
                {
                    break;
                }
                if(ev->data.ext.len > apiData->bufferSize)
                {
                    apiData->bufferSize = ev->data.ext.len;
                    free(buffer);
                    buffer = (unsigned char *) malloc(apiData->bufferSize);
                    if(buffer == NULL)
                    {
                        data->doInput = false;
                        break;
                    }
                }
                doDecode = true;
                break;

            default:
                doDecode = true;
        }

        if(doDecode)
        {
            nBytes = snd_midi_event_decode(apiData->coder, buffer, apiData->bufferSize, ev);
            if(nBytes > 0)
            {
                // The ALSA sequencer has a maximum buffer size for MIDI sysex
                // events of 256 bytes.  If a device sends sysex messages larger
                // than this, they are segmented into 256 byte chunks.  So,
                // we'll watch for this and concatenate sysex chunks into a
                // single sysex message if necessary.
                if(!continueSysex)
                {
                    message.bytes.assign(buffer, &buffer[nBytes]);
                }
                else
                {
                    message.bytes.insert(message.bytes.end(), buffer, &buffer[nBytes]);
                }

                continueSysex = ((ev->type == SND_SEQ_EVENT_SYSEX) && (message.bytes.back() != 0xF7));
                if(!continueSysex)
                {
                    // Calculate the time stamp:
                    message.timeStamp = 0.0;

                    // Method 1: Use the system time.
                    //(void)gettimeofday(&tv, (struct timezone *)NULL);
                    //time = (tv.tv_sec * 1000000) + tv.tv_usec;

                    // Method 2: Use the ALSA sequencer event time data.
                    // (thanks to Pedro Lopez-Cabanillas!).

                    // Using method from:
                    // https://www.gnu.org/software/libc/manual/html_node/Elapsed-Time.html

                    // Perform the carry for the later subtraction by updating y.
                    // Temp var y is timespec because computation requires signed types,
                    // while snd_seq_real_time_t has unsigned types.

                    snd_seq_real_time_t &x(ev->time.time);
                    struct timespec y;
                    
                    y.tv_nsec = apiData->lastTime.tv_nsec;
                    y.tv_sec = apiData->lastTime.tv_sec;
                    if(x.tv_nsec < y.tv_nsec)
                    {
                        int nsec = (y.tv_nsec - (int)x.tv_nsec) / 1000000000 + 1;
                        y.tv_nsec -= 1000000000 * nsec;
                        y.tv_sec += nsec;
                    }
                    if(x.tv_nsec - y.tv_nsec > 1000000000)
                    {
                        int nsec = ((int)x.tv_nsec - y.tv_nsec) / 1000000000;
                        y.tv_nsec += 1000000000 * nsec;
                        y.tv_sec -= nsec;
                    }
                }
            }
        }

        snd_seq_free_event(ev);
        if(message.bytes.size() == 0 || continueSysex)
        {
            continue;
        }

        if(data->usingCallback)
        {
            RtMidiIn::RtMidiCallback callback = (RtMidiIn::RtMidiCallback) data->userCallback;
            callback(message.timeStamp, &message.bytes, data->userData);
        }
        else
        {
            // As long as we haven't reached our queue size limit, push the message.
            if(data->queueLimit > data->queue.size())
            {
                data->queue.push(message);
            }
        }
    }

    if(buffer)
    {
        free(buffer);
    }
    snd_midi_event_free(apiData->coder);
    apiData->coder = 0;
    apiData->thread = apiData->dummy_thread_id;
    return 0;
}

void RtMidiIn :: initialize(char *clientName)
{
    // Set up the ALSA sequencer client.
    snd_seq_t *seq;
    int result = snd_seq_open(&seq,
                              "default",
                              SND_SEQ_OPEN_DUPLEX,
                              SND_SEQ_NONBLOCK);

    if(result < 0)
    {
        sprintf(errorString_, "RtMidiIn::initialize: error creating ALSA sequencer input client object.");
        error(RtError::DRIVER_ERROR);
        return;
    }

    // Set client name.
    snd_seq_set_client_name(seq, clientName);

    // Save our api-specific connection information.
    AlsaMidiData *data = (AlsaMidiData *) new AlsaMidiData;
    data->seq = seq;
    data->bufferSize = 32;
    data->client_id = -1;
    data->vport = -1;
    data->subscription = 0;
    data->dummy_thread_id = pthread_self();
    data->thread = data->dummy_thread_id;
    data->trigger_fds[0] = -1;
    data->trigger_fds[1] = -1;
  
    apiData_ = (void *) data;
    inputData_.apiData = (void *) data;

    if(pipe(data->trigger_fds) == -1)
    {
        sprintf(errorString_, "RtMidiIn::initialize: error creating pipe objects.");
        error(RtError::DRIVER_ERROR);
        return;
    }

    // Create the input queue
#ifndef AVOID_TIMESTAMPING
    data->queue_id = snd_seq_alloc_named_queue(seq, "RtMidi Queue");
    // Set arbitrary tempo (mm=100) and resolution (240)
    snd_seq_queue_tempo_t *qtempo;
    snd_seq_queue_tempo_alloca(&qtempo);
    snd_seq_queue_tempo_set_tempo(qtempo, 600000);
    snd_seq_queue_tempo_set_ppq(qtempo, 240);
    snd_seq_set_queue_tempo(data->seq, data->queue_id, qtempo);
    snd_seq_drain_output(data->seq);
#endif
}

// This function is used to count or get the pinfo structure for a given port number.
unsigned int portInfo(snd_seq_t *seq,
                      snd_seq_port_info_t *pinfo,
                      unsigned int type,
                      int portNumber)
{
    snd_seq_client_info_t *cinfo;
    int client;
    int count = 0;
    snd_seq_client_info_alloca(&cinfo);

    snd_seq_client_info_set_client(cinfo, -1);
    while(snd_seq_query_next_client(seq, cinfo) >= 0)
    {
        client = snd_seq_client_info_get_client(cinfo);
        if(client == 0)
        {
            continue;
        }
        // Reset query info
        snd_seq_port_info_set_client(pinfo, client);
        snd_seq_port_info_set_port(pinfo, -1);
        while(snd_seq_query_next_port(seq, pinfo) >= 0)
        {
            unsigned int atyp = snd_seq_port_info_get_type(pinfo);
            if(((atyp & SND_SEQ_PORT_TYPE_MIDI_GENERIC) == 0) &&
               ((atyp & SND_SEQ_PORT_TYPE_SYNTH) == 0) &&
               ((atyp & SND_SEQ_PORT_TYPE_APPLICATION) == 0))
            {  
               continue;
            }

            unsigned int caps = snd_seq_port_info_get_capability(pinfo);
            if(((caps & type) != type) ||
               ((caps & SND_SEQ_PORT_CAP_NO_EXPORT) != 0))
            {
                continue;
            }
            if(count == portNumber)
            {
                return 1;
            }
            ++count;
        }
    }

    // If a negative portNumber was used, return the port count.
    if(portNumber < 0)
    {
        return count;
    }
    return 0;
}

void RtMidiIn :: openPort(unsigned int portNumber, char *portName)
{
    if(connected_)
    {
        sprintf(errorString_, "RtMidiIn::openPort: a valid connection already exists!");
        error(RtError::WARNING);
        return;
    }

    unsigned int nSrc = this->getPortCount();
    if(nSrc < 1)
    {
        sprintf(errorString_, "RtMidiIn::openPort: no MIDI input sources found!");
        error(RtError::NO_DEVICES_FOUND);
        return;
    }

	snd_seq_port_info_t *pinfo;
	snd_seq_port_info_alloca(&pinfo);

    AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
    if(portInfo(data->seq,
                pinfo,
                SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
                (int) portNumber) == 0
               )
    {
        sprintf(errorString_, "RtMidiIn::openPort: the 'portNumber' argument (%d) is invalid.", portNumber);
        error(RtError::INVALID_PARAMETER);
        return;
    }

    snd_seq_addr_t sender, receiver;
    sender.client = snd_seq_port_info_get_client(pinfo);
    sender.port = snd_seq_port_info_get_port(pinfo);
    receiver.client = snd_seq_client_id(data->seq);
  
    if(data->vport < 0)
    {
        snd_seq_port_info_set_client(pinfo, 0);
        snd_seq_port_info_set_port(pinfo, 0);
        snd_seq_port_info_set_capability(pinfo,
                                         SND_SEQ_PORT_CAP_WRITE |
                                         SND_SEQ_PORT_CAP_SUBS_WRITE);
        snd_seq_port_info_set_type(pinfo,
                                   SND_SEQ_PORT_TYPE_MIDI_GENERIC |
                                   SND_SEQ_PORT_TYPE_APPLICATION);
        snd_seq_port_info_set_midi_channels(pinfo, 16);

#ifndef AVOID_TIMESTAMPING
        snd_seq_port_info_set_timestamping(pinfo, 1);
        snd_seq_port_info_set_timestamp_real(pinfo, 1);    
        snd_seq_port_info_set_timestamp_queue(pinfo, data->queue_id);
#endif

        snd_seq_port_info_set_name(pinfo, portName);
        data->vport = snd_seq_create_port(data->seq, pinfo);
  
        if(data->vport < 0)
        {
            sprintf(errorString_, "RtMidiIn::openPort: ALSA error creating input port.");
            error(RtError::DRIVER_ERROR);
            return;
        }
        data->vport = snd_seq_port_info_get_port(pinfo);
    }

    receiver.port = data->vport;

    if(!data->subscription)
    {
        // Make subscription
        if(snd_seq_port_subscribe_malloc(&data->subscription) < 0)
        {
            sprintf(errorString_, "RtMidiIn::openPort: ALSA error allocation port subscription.");
            error(RtError::DRIVER_ERROR);
            return;
        }
        snd_seq_port_subscribe_set_sender(data->subscription, &sender);
        snd_seq_port_subscribe_set_dest(data->subscription, &receiver);
        if(snd_seq_subscribe_port(data->seq, data->subscription))
        {
            snd_seq_port_subscribe_free(data->subscription);
            data->subscription = 0;
            sprintf(errorString_, "RtMidiIn::openPort: ALSA error making port connection.");
            error(RtError::DRIVER_ERROR);
            return;
        }
    }

    if(inputData_.doInput == false)
    {
        // Start the input queue
#ifndef AVOID_TIMESTAMPING
        snd_seq_start_queue(data->seq, data->queue_id, NULL);
        snd_seq_drain_output(data->seq);
#endif
        // Start our MIDI input thread.
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_attr_setschedpolicy(&attr, SCHED_OTHER);

        inputData_.doInput = true;
        int err = pthread_create(&data->thread, &attr, alsaMidiHandler, &inputData_);
        pthread_attr_destroy(&attr);
        if(err)
        {
            snd_seq_unsubscribe_port(data->seq, data->subscription);
            snd_seq_port_subscribe_free(data->subscription);
            data->subscription = 0;
            inputData_.doInput = false;
            sprintf(errorString_, "RtMidiIn::openPort: error starting MIDI input thread!");
            error(RtError::THREAD_ERROR);
            return;
        }
    }
    connected_ = true;
}

void RtMidiIn :: openVirtualPort(char *portName)
{
    AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
 
    if(data->vport < 0)
    {
        snd_seq_port_info_t *pinfo;
        snd_seq_port_info_alloca(&pinfo);
        snd_seq_port_info_set_capability(pinfo,
		                                 SND_SEQ_PORT_CAP_WRITE |
		                                 SND_SEQ_PORT_CAP_SUBS_WRITE);
        snd_seq_port_info_set_type(pinfo,
	                               SND_SEQ_PORT_TYPE_MIDI_GENERIC |
	                               SND_SEQ_PORT_TYPE_APPLICATION);
        snd_seq_port_info_set_midi_channels(pinfo, 16);
#ifndef AVOID_TIMESTAMPING
        snd_seq_port_info_set_timestamping(pinfo, 1);
        snd_seq_port_info_set_timestamp_real(pinfo, 1);    
        snd_seq_port_info_set_timestamp_queue(pinfo, data->queue_id);
#endif
        snd_seq_port_info_set_name(pinfo, portName);
        data->vport = snd_seq_create_port(data->seq, pinfo);

        if(data->vport < 0)
        {
            sprintf(errorString_, "RtMidiIn::openVirtualPort: ALSA error creating virtual port.");
            error(RtError::DRIVER_ERROR);
            return;
        }
        data->vport = snd_seq_port_info_get_port(pinfo);
    }

    if(inputData_.doInput == false)
    {
        // Wait for old thread to stop, if still running
        if(!pthread_equal( data->thread, data->dummy_thread_id))
        {
            pthread_join(data->thread, NULL);
        }
        
        // Start the input queue
#ifndef AVOID_TIMESTAMPING
        snd_seq_start_queue(data->seq, data->queue_id, NULL);
        snd_seq_drain_output(data->seq);
#endif
        // Start our MIDI input thread.
        pthread_attr_t attr;
        pthread_attr_init(&attr);
        pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
        pthread_attr_setschedpolicy(&attr, SCHED_OTHER);

        inputData_.doInput = true;
        int err = pthread_create(&data->thread, &attr, alsaMidiHandler, &inputData_);
        pthread_attr_destroy(&attr);
        if(err)
        {
            if(data->subscription)
            {
                snd_seq_unsubscribe_port(data->seq, data->subscription);
                snd_seq_port_subscribe_free(data->subscription);
                data->subscription = 0;
            }
            inputData_.doInput = false;
            sprintf(errorString_, "RtMidiIn::openPort: error starting MIDI input thread!");
            error(RtError::THREAD_ERROR);
            return;
        }
    }
}

void RtMidiIn :: closePort(void)
{
    AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
   
    if(connected_)
    {
        if(data->subscription)
        {
            snd_seq_unsubscribe_port(data->seq, data->subscription);
            snd_seq_port_subscribe_free(data->subscription);
            data->subscription = 0;
        }
        // Stop the input queue
#ifndef AVOID_TIMESTAMPING
        snd_seq_stop_queue(data->seq, data->queue_id, NULL);
        snd_seq_drain_output(data->seq);
#endif
        connected_ = false;
    }

    // Stop thread to avoid triggering the callback, while the port is intended to be closed
    if(inputData_.doInput)
    {
        inputData_.doInput = false;
        int res = write(data->trigger_fds[1], &inputData_.doInput, sizeof(inputData_.doInput));
        (void) res;
        if(!pthread_equal(data->thread, data->dummy_thread_id))
        {
            pthread_join(data->thread, NULL);
        }
    }
}

RtMidiIn :: ~RtMidiIn()
{
    // Close a connection if it exists.
    closePort();

    // Shutdown the input thread.
    AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
    if(inputData_.doInput)
    {
        inputData_.doInput = false;
        pthread_join(data->thread, NULL);
    }

    // Cleanup.
    if (data->vport >= 0)
    {
        snd_seq_delete_port(data->seq, data->vport);
    }
#ifndef AVOID_TIMESTAMPING
    snd_seq_free_queue(data->seq, data->queue_id);
#endif
    snd_seq_close(data->seq);
    delete data;
}

unsigned int RtMidiIn :: getPortCount()
{
    snd_seq_port_info_t *pinfo;
    snd_seq_port_info_alloca(&pinfo);

    AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
    return portInfo(data->seq,
                    pinfo,
                    SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
                    -1);
}

std::string RtMidiIn :: getPortName(unsigned int portNumber, char *Name)
{
    snd_seq_client_info_t *cinfo;
    snd_seq_port_info_t *pinfo;
    snd_seq_client_info_alloca(&cinfo);
    snd_seq_port_info_alloca(&pinfo);

    AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
    if(portInfo(data->seq,
                pinfo,
                SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_READ,
                (int) portNumber))
    {
        int cnum = snd_seq_port_info_get_client(pinfo);
        snd_seq_get_any_client_info(data->seq, cnum, cinfo);
        
        std::ostringstream os;
        os << snd_seq_client_info_get_name(cinfo);
        os << ":";
        os << snd_seq_port_info_get_port(pinfo);
        std::string stringName = os.str();
        strcpy(Name, stringName.c_str());
        return stringName;
    }

    // If we get here, we didn't find a match.
    sprintf(errorString_, "RtMidiIn::getPortName: error looking for port name!");
    error(RtError::INVALID_PARAMETER);
    return 0;
}

//*********************************************************************//
//  API: LINUX ALSA
//  Class Definitions: RtMidiOut
//*********************************************************************//

unsigned int RtMidiOut :: getPortCount()
{
	snd_seq_port_info_t *pinfo;
	snd_seq_port_info_alloca(&pinfo);

    AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
    return portInfo(data->seq,
                    pinfo,
                    SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
                    -1);
}

std::string RtMidiOut :: getPortName(unsigned int portNumber, char *Name)
{
    snd_seq_client_info_t *cinfo;
    snd_seq_port_info_t *pinfo;
    snd_seq_client_info_alloca(&cinfo);
    snd_seq_port_info_alloca(&pinfo);

    AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
    if(portInfo(data->seq,
                pinfo,
                SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
                (int) portNumber))
    {
        int cnum = snd_seq_port_info_get_client(pinfo);
        snd_seq_get_any_client_info(data->seq, cnum, cinfo);
        std::ostringstream os;
        os << snd_seq_client_info_get_name(cinfo);
        os << ":";
        os << snd_seq_port_info_get_port(pinfo);
        std::string stringName = os.str();
        strcpy(Name, stringName.c_str());
        return stringName;
    }

    // If we get here, we didn't find a match.
    sprintf(errorString_, "RtMidiOut::getPortName: error looking for port name!");
    error(RtError::INVALID_PARAMETER);
    return 0;
}

void RtMidiOut :: initialize(char *clientName)
{
    // Set up the ALSA sequencer client.
    snd_seq_t *seq;
    int result = snd_seq_open(&seq,
                              "default",
                              SND_SEQ_OPEN_DUPLEX,
                              SND_SEQ_NONBLOCK);

    if(result < 0)
    {
        sprintf(errorString_, "RtMidiOut::initialize: error creating ALSA sequencer client object.");
        error(RtError::DRIVER_ERROR);
        return;
    }

    // Set client name.
    snd_seq_set_client_name(seq, clientName);

    // Save our api-specific connection information.
    AlsaMidiData *data = (AlsaMidiData *) new AlsaMidiData;
    data->seq = seq;
    data->client_id = -1;
    data->vport = -1;
    data->bufferSize = 32;
    data->coder = 0;
    data->buffer = 0;
    result = snd_midi_event_new(data->bufferSize, &data->coder);
    if(result < 0)
    {
        delete data;
        sprintf(errorString_, "RtMidiOut::initialize: error initializing MIDI event parser!\n\n");
        error(RtError::DRIVER_ERROR);
        return;
    }
    data->buffer = (unsigned char *) malloc(data->bufferSize);
    if(data->buffer == NULL)
    {
        delete data;
        sprintf(errorString_, "RtMidiOut::initialize: error allocating buffer memory!\n\n");
        error(RtError::MEMORY_ERROR);
        return;
    }
    snd_midi_event_init(data->coder);
    snd_midi_event_no_status(data->coder, 1);
    apiData_ = (void *) data;
}

void RtMidiOut :: openPort(unsigned int portNumber, char *portName)
{
    if(connected_)
    {
        sprintf(errorString_, "RtMidiOut::openPort: a valid connection already exists!");
        error(RtError::WARNING);
        return;
    }

    unsigned int nSrc = this->getPortCount();
    if(nSrc < 1)
    {
        sprintf(errorString_, "RtMidiOut::openPort: no MIDI output sources found!");
        error(RtError::NO_DEVICES_FOUND);
        return;
    }

    snd_seq_port_info_t *pinfo;
    snd_seq_port_info_alloca(&pinfo);

    AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
    if(portInfo(data->seq,
                pinfo,
                SND_SEQ_PORT_CAP_WRITE | SND_SEQ_PORT_CAP_SUBS_WRITE,
                (int) portNumber) == 0)
    {
        sprintf(errorString_, "RtMidiOut::openPort: the 'portNumber' argument (%d) is invalid.", portNumber);
        error(RtError::INVALID_PARAMETER);
        return;
    }

    snd_seq_addr_t sender, receiver;
    receiver.client = snd_seq_port_info_get_client(pinfo);
    receiver.port = snd_seq_port_info_get_port(pinfo);
    sender.client = snd_seq_client_id(data->seq);
    data->client_id = receiver.client;

    if(data->vport < 0)
    {
        data->vport = snd_seq_create_simple_port(data->seq, portName,
                                                 SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_WRITE,
                                                 SND_SEQ_PORT_TYPE_APPLICATION );
        if(data->vport < 0)
        {
            sprintf(errorString_, "MidiOutAlsa::openPort: ALSA error creating output port.");
            error(RtError::DRIVER_ERROR);
            return;
        }
    }

    sender.port = data->vport;

    if(!data->subscription)
    {
        // Make subscription
        if(snd_seq_port_subscribe_malloc(&data->subscription) < 0)
        {
            sprintf(errorString_, "RtMidiIn::openPort: ALSA error allocation port subscription.");
            error(RtError::DRIVER_ERROR);
            return;
        }
        snd_seq_port_subscribe_set_sender(data->subscription, &sender);
        snd_seq_port_subscribe_set_dest(data->subscription, &receiver);
        snd_seq_port_subscribe_set_time_update(data->subscription, 1);
        snd_seq_port_subscribe_set_time_real(data->subscription, 1);
        if(snd_seq_subscribe_port(data->seq, data->subscription))
        {
            snd_seq_port_subscribe_free(data->subscription);
            data->subscription = 0;
            sprintf(errorString_, "RtMidiOut::openPort: ALSA error making port connection.");
            error(RtError::DRIVER_ERROR);
            return;
        }
    }

    connected_ = true;
}

void RtMidiOut :: closePort(void)
{
    if(connected_)
    {
        AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
        snd_seq_unsubscribe_port(data->seq, data->subscription);
        snd_seq_port_subscribe_free(data->subscription);
        data->subscription = 0;
        connected_ = false;
    }
}

void RtMidiOut :: openVirtualPort(char *portName)
{
    AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
    if(data->vport < 0)
    {
        data->vport = snd_seq_create_simple_port(data->seq, portName,
                                                 SND_SEQ_PORT_CAP_READ | SND_SEQ_PORT_CAP_SUBS_WRITE,
                                                 SND_SEQ_PORT_TYPE_APPLICATION);

        if(data->vport < 0)
        {
            sprintf(errorString_, "RtMidiOut::openVirtualPort: ALSA error creating virtual port.");
            error(RtError::DRIVER_ERROR);
            return;
        }
    }
}

RtMidiOut :: ~RtMidiOut()
{
    // Close a connection if it exists.
    closePort();

    // Cleanup.
    AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
    if(data->vport >= 0) snd_seq_delete_port(data->seq, data->vport);
    if(data->coder) snd_midi_event_free(data->coder);
    if(data->buffer) free(data->buffer);
    snd_seq_close(data->seq);
    delete data;
}

void RtMidiOut :: sendMessage(std::vector<unsigned char> *message)
{
    int result;
    AlsaMidiData *data = static_cast<AlsaMidiData *> (apiData_);
    unsigned int nBytes = message->size();
    
    if(nBytes > data->bufferSize)
    {
        data->bufferSize = nBytes;
        result = snd_midi_event_resize_buffer(data->coder, nBytes);
        if(result != 0)
        {
            sprintf(errorString_, "RtMidiOut::sendMessage: ALSA error resizing MIDI event buffer.");
            error(RtError::DRIVER_ERROR);
            return;
        }
        free(data->buffer);
        data->buffer = (unsigned char *) malloc(data->bufferSize);
        if(data->buffer == NULL)
        {
            sprintf(errorString_, "RtMidiOut::initialize: error allocating buffer memory!\n\n");
            error(RtError::MEMORY_ERROR);
            return;
        }
    }

    for(unsigned int i = 0; i < nBytes; i++)
    {
        data->buffer[i] = message->at(i);
    }

    unsigned int offset = 0;
    
    while(offset < nBytes)
    {
        snd_seq_event_t ev;
        snd_seq_ev_clear(&ev);
        snd_seq_ev_set_direct(&ev);
        snd_seq_ev_set_dest(&ev, data->client_id, data->vport);
        snd_seq_ev_set_subs(&ev);
        
        result = snd_midi_event_encode(data->coder, data->buffer + offset, (long) (nBytes - offset), &ev);
        if(result < 0)
        {
            sprintf(errorString_, "RtMidiOut::sendMessage: event parsing error!");
            error(RtError::WARNING);
            return;
        }
        if(ev.type == SND_SEQ_EVENT_NONE)
        {
            sprintf(errorString_, "RtMidiOut::sendMessage: incomplete message!");
            error(RtError::WARNING);
            return;
        }

        offset += result;

        // Send the event.
        result = snd_seq_event_output(data->seq, &ev);
        if(result < 0)
        {
            sprintf(errorString_, "RtMidiOut::sendMessage: error sending MIDI message to port (%d).", result);
            error(RtError::WARNING);
            return;
        }
    }
    snd_seq_drain_output(data->seq);
}

#endif // __LINUX_ALSA__

//*********************************************************************//
//  API: Windows Multimedia Library (MM)
//*********************************************************************//

// API information deciphered from:
//  - http://msdn.microsoft.com/library/default.asp?url=/library/en-us/multimed/htm/_win32_midi_reference.asp

// Thanks to Jean-Baptiste Berruchon for the sysex code.

#if defined(__WINDOWS_MM__)

// The Windows MM API is based on the use of a callback function for
// MIDI input.  We convert the system specific time stamps to delta
// time values.

// Windows MM MIDI header files.
#include <windows.h>
#include <mmsystem.h>

#define  RT_SYSEX_BUFFER_COUNT 4

// A structure to hold variables related to the CoreMIDI API
// implementation.
struct WinMidiData
{
    HMIDIIN inHandle;    // Handle to Midi Input Device
    HMIDIOUT outHandle;  // Handle to Midi Output Device
    DWORD lastTime;
    RtMidiIn::MidiMessage message;
    LPMIDIHDR sysexBuffer[RT_SYSEX_BUFFER_COUNT];
    CRITICAL_SECTION _mutex; // [Patrice] see https://groups.google.com/forum/#!topic/mididev/6OUjHutMpEo
};

#if !defined(__GCC__)
extern "C"
{
    int __lc_codepage = 0;
    int __lc_handle = 0;
    int __mb_cur_max = 0;
    int errno;
    unsigned short *_pctype = 0;
}
#endif

//*********************************************************************//
//  API: Windows MM
//  Class Definitions: RtMidiIn
//*********************************************************************//

static void CALLBACK midiInputCallback(HMIDIOUT hmin,
                                       UINT inputStatus, 
                                       DWORD_PTR instancePtr,
                                       DWORD_PTR midiMessage,
                                       DWORD timestamp)
{
    if(inputStatus != MIM_DATA &&
       inputStatus != MIM_LONGDATA &&
       inputStatus != MIM_LONGERROR)
    {
        return;
    }

    //RtMidiIn::RtMidiInData *data = static_cast<RtMidiIn::RtMidiInData *> (instancePtr);
    RtMidiIn::RtMidiInData *data = (RtMidiIn::RtMidiInData *)instancePtr;
    WinMidiData *apiData = static_cast<WinMidiData *> (data->apiData);

    // Calculate time stamp.
    if(data->firstMessage == true)
    {
        apiData->message.timeStamp = 0.0f;
        data->firstMessage = false;
    }
    else
    {
        apiData->message.timeStamp = (double) (timestamp - apiData->lastTime) * 0.001f;
    }

    if(inputStatus == MIM_DATA)
    {   // Channel or system message

        // Make sure the first byte is a status byte.
        unsigned char status = (unsigned char) (midiMessage & 0x000000FF);
        if(!(status & 0x80))
        {
            return;
        }

        // Determine the number of bytes in the MIDI message.
        unsigned short nBytes = 1;
        if(status < 0xC0) nBytes = 3;
        else if(status < 0xE0) nBytes = 2;
        else if(status < 0xF0) nBytes = 3;
        else if(status == 0xF1)
        {
            if(data->ignoreFlags & 0x02)
            {
                return;
            }
            else
            {
                nBytes = 2;
            }
        }
        else if(status == 0xF2) nBytes = 3;
        else if(status == 0xF3) nBytes = 2;
        else if(status == 0xF8 && (data->ignoreFlags & 0x02))
        {
            // A MIDI timing tick message and we're ignoring it.
            return;
        }
        else if(status == 0xFE && (data->ignoreFlags & 0x04))
        {
            // A MIDI active sensing message and we're ignoring it.
            return;
        }

        // Copy bytes to our MIDI message.
        unsigned char *ptr = (unsigned char *) &midiMessage;
        for(int i = 0; i < nBytes; i++)
        {
            apiData->message.bytes.push_back(*ptr++);
        }
    }
    else
    {   // Sysex message (MIM_LONGDATA or MIM_LONGERROR)
        MIDIHDR *sysex = (MIDIHDR *) midiMessage;
        if(!(data->ignoreFlags & 0x01) && inputStatus != MIM_LONGERROR)
        {
            // Sysex message and we're not ignoring it
            for(int i = 0; i < (int) sysex->dwBytesRecorded; i++)
            {
                apiData->message.bytes.push_back(sysex->lpData[i]);
            }
        }

        // The WinMM API requires that the sysex buffer be requeued after
        // input of each sysex message.  Even if we are ignoring sysex
        // messages, we still need to requeue the buffer in case the user
        // decides to not ignore sysex messages in the future.  However,
        // it seems that WinMM calls this function with an empty sysex
        // buffer when an application closes and in this case, we should
        // avoid requeueing it, else the computer suddenly reboots after
        // one or two minutes.
	    if(apiData->sysexBuffer[sysex->dwUser]->dwBytesRecorded > 0)
        {
            EnterCriticalSection(&(apiData->_mutex));
            MMRESULT result = midiInAddBuffer(apiData->inHandle, apiData->sysexBuffer[sysex->dwUser], sizeof(MIDIHDR));
            LeaveCriticalSection(&(apiData->_mutex));
            if(result != MMSYSERR_NOERROR)
            {
                std::cerr << "\nRtMidiIn::midiInputCallback: error sending sysex to Midi device!!\n\n";
                return;
            }
            if(data->ignoreFlags & 0x01)
            {
                return;
            }
        }
        else
        {
            return;
        }
    }

    // Save the time of the last non-filtered message
    apiData->lastTime = timestamp;

    if(data->usingCallback)
    {
        RtMidiIn::RtMidiCallback callback = (RtMidiIn::RtMidiCallback) data->userCallback;
        callback(apiData->message.timeStamp, &apiData->message.bytes, data->userData);
    }
    else
    {
        // As long as we haven't reached our queue size limit, push the message.
        if(data->queueLimit > data->queue.size())
        {
            data->queue.push(apiData->message);
        }
    }

    // Clear the vector for the next input message.
    apiData->message.bytes.clear();
}

void RtMidiIn :: initialize(char *clientName)
{
    // We'll issue a warning here if no devices are available but not
    // throw an error since the user can plugin something later.
    unsigned int nDevices = getPortCount();

    if(nDevices == 0)
    {
        sprintf(errorString_, "RtMidiIn::initialize: no MIDI input devices currently available.");
        error(RtError::WARNING);
        return;
    }

    // Save our api-specific connection information.
    WinMidiData *data = (WinMidiData *) new WinMidiData;
    apiData_ = (void *) data;
    inputData_.apiData = (void *) data;
    data->message.bytes.clear();  // needs to be empty for first input message

    InitializeCriticalSection(&data->_mutex);
}

void RtMidiIn :: openPort(unsigned int portNumber, char *portName)
{
    if(connected_)
    {
        sprintf(errorString_, "RtMidiIn::openPort: a valid connection already exists!");
        error(RtError::WARNING);
        return;
    }

    unsigned int nDevices = getPortCount();
    if(nDevices == 0)
    {
        sprintf(errorString_, "RtMidiIn::openPort: no MIDI input sources found!");
        error(RtError::NO_DEVICES_FOUND);
        return;
    }

    if(portNumber >= nDevices)
    {
        sprintf(errorString_, "RtMidiIn::openPort: the 'portNumber' argument (%d) is invalid.", portNumber);
        error(RtError::INVALID_PARAMETER);
        return;
    }

    WinMidiData *data = static_cast<WinMidiData *> (apiData_);
    MMRESULT result = midiInOpen(&data->inHandle,
                                 portNumber,
                                 (DWORD_PTR) &midiInputCallback,
                                 (DWORD_PTR ) &inputData_,
                                 CALLBACK_FUNCTION);
    if(result != MMSYSERR_NOERROR)
    {
        sprintf(errorString_, "RtMidiIn::openPort: error creating Windows MM MIDI input port.");
        error(RtError::DRIVER_ERROR);
        return;
    }

    // Allocate and init the sysex buffer.
    for(unsigned int i = 0; i < RT_SYSEX_BUFFER_COUNT; ++i)
    {
        data->sysexBuffer[i] = (MIDIHDR *) new char[sizeof(MIDIHDR)];
        data->sysexBuffer[i]->lpData = new char[RT_SYSEX_BUFFER_SIZE];
        data->sysexBuffer[i]->dwBufferLength = RT_SYSEX_BUFFER_SIZE;
        data->sysexBuffer[i]->dwUser = i;   // We use the dwUser parameter as buffer indicator
        data->sysexBuffer[i]->dwFlags = 0;

        result = midiInPrepareHeader(data->inHandle, data->sysexBuffer[i], sizeof(MIDIHDR));
        if(result != MMSYSERR_NOERROR)
        {
            midiInClose(data->inHandle);
            data->inHandle = 0;
            sprintf(errorString_, "RtMidiIn::openPort: error starting Windows MM MIDI input port (PrepareHeader).");
            error(RtError::DRIVER_ERROR);
            return;
        }

        // Register the buffer.
        result = midiInAddBuffer(data->inHandle, data->sysexBuffer[i], sizeof(MIDIHDR));
        if(result != MMSYSERR_NOERROR)
        {
            midiInClose(data->inHandle);
            data->inHandle = 0;
            sprintf(errorString_, "RtMidiIn::openPort: error starting Windows MM MIDI input port (AddBuffer).");
            error(RtError::DRIVER_ERROR);
            return;
        }
    }

    result = midiInStart(data->inHandle);
    if(result != MMSYSERR_NOERROR)
    {
        midiInClose(data->inHandle);
        data->inHandle = 0;
        sprintf(errorString_, "RtMidiIn::openPort: error starting Windows MM MIDI input port.");
        error(RtError::DRIVER_ERROR);
        return;
    }

    connected_ = true;
}

void RtMidiIn :: openVirtualPort(char *portName)
{
    // This function cannot be implemented for the Windows MM MIDI API.
    sprintf(errorString_, "RtMidiIn::openVirtualPort: cannot be implemented in Windows MM MIDI API!");
    error(RtError::WARNING);
}

void RtMidiIn :: closePort(void)
{
    if(connected_)
    {
        WinMidiData *data = static_cast<WinMidiData *> (apiData_);
        EnterCriticalSection(&(data->_mutex));
        midiInReset(data->inHandle);
        midiInStop(data->inHandle);

        for(int i = 0; i < RT_SYSEX_BUFFER_COUNT; ++i)
        {
            int result = midiInUnprepareHeader(data->inHandle,
                                               data->sysexBuffer[i],
                                               sizeof(MIDIHDR));
            delete []data->sysexBuffer[i]->lpData;
            delete []data->sysexBuffer[i];
            if(result != MMSYSERR_NOERROR)
            {
                midiInClose(data->inHandle);
                data->inHandle = 0;
                sprintf(errorString_, "RtMidiIn::openPort: error closing Windows MM MIDI input port (midiInUnprepareHeader).");
                error(RtError::DRIVER_ERROR);
                return;
            }
        }

        midiInClose(data->inHandle);
        data->inHandle = 0;
        connected_ = false;
        LeaveCriticalSection(&(data->_mutex));
    }
}

RtMidiIn :: ~RtMidiIn()
{
    // Cleanup.
    WinMidiData *data = static_cast<WinMidiData *> (apiData_);
    DeleteCriticalSection(&data->_mutex);
    delete data;
    data = 0;
}

unsigned int RtMidiIn :: getPortCount()
{
    return midiInGetNumDevs();
}
/*
// Convert a null-terminated wide string or ANSI-encoded string to UTF-8.
static std::string ConvertToUTF8(const TCHAR *str)
{
    std::string u8str;
    const WCHAR *wstr = L"";

#if defined(UNICODE) || defined(_UNICODE)
    wstr = str;
#else
    // Convert from ANSI encoding to wide string
    int wlength = MultiByteToWideChar(CP_ACP, 0, str, -1, NULL, 0);
    std::wstring wstrtemp;
    if(wlength)
    {
        wstrtemp.assign(wlength - 1, 0);
        MultiByteToWideChar(CP_ACP, 0, str, -1, &wstrtemp[0], wlength);
        wstr = &wstrtemp[0];
    }
#endif
    
    // Convert from wide string to UTF-8
    int length = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, NULL, 0, NULL, NULL);
    if(length)
    {
        u8str.assign(length - 1, 0);
        length = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &u8str[0], length, NULL, NULL);
    }
    return u8str;
}
*/
std::string RtMidiIn :: getPortName(unsigned int portNumber, char *Name)
{
    std::string stringName;
    unsigned int nDevices = getPortCount();

    if(portNumber >= nDevices)
    {
        sprintf(errorString_, "RtMidiIn::getPortName: the 'portNumber' argument (%d) is invalid.", portNumber);
        error(RtError::INVALID_PARAMETER);
        return("");
    }

    MIDIINCAPS deviceCaps;
    midiInGetDevCaps(portNumber, &deviceCaps, sizeof(MIDIINCAPS));
/*
    stringName = ConvertToUTF8(deviceCaps.szPname);

    // Next lines added to add the portNumber to the name so that
    // the device's names are sure to be listed with individual names
    // even when they have the same brand name
#ifndef RTMIDI_DO_NOT_ENSURE_UNIQUE_PORTNAMES
    std::ostringstream os;
    os << " ";
    os << portNumber;
    stringName += os.str();
#endif
*/
    // For some reason, we need to copy character by character with
    // UNICODE (thanks to Eduardo Coutinho!).
    char nameString[MAXPNAMELEN];
    
    memset(nameString, 0, MAXPNAMELEN);
    for(int i = 0; i < MAXPNAMELEN; i++)
    {
        nameString[i] = (char) (deviceCaps.szPname[i]);
    }
    strcpy(Name, nameString);

    return stringName;
}

//*********************************************************************//
//  API: Windows MM
//  Class Definitions: RtMidiOut
//*********************************************************************//

unsigned int RtMidiOut :: getPortCount()
{
    return midiOutGetNumDevs();
}

std::string RtMidiOut :: getPortName(unsigned int portNumber, char *Name)
{
    unsigned int nDevices = getPortCount();
    
    if(portNumber >= nDevices)
    {
        sprintf(errorString_, "RtMidiOut::getPortName: the 'portNumber' argument (%d) is invalid.", portNumber);
        error(RtError::INVALID_PARAMETER);
        return("");
    }

    MIDIOUTCAPS deviceCaps;
    midiOutGetDevCaps(portNumber, &deviceCaps, sizeof(MIDIOUTCAPS));

/*    stringName = ConvertToUTF8(deviceCaps.szPname);

    // Next lines added to add the portNumber to the name so that
    // the device's names are sure to be listed with individual names
    // even when they have the same brand name
#ifndef RTMIDI_DO_NOT_ENSURE_UNIQUE_PORTNAMES
    std::ostringstream os;
    os << " ";
    os << portNumber;
    stringName += os.str();
#endif
*/
    // For some reason, we need to copy character by character with
    // UNICODE (thanks to Eduardo Coutinho!).
    char nameString[MAXPNAMELEN];
    
    memset(nameString, 0, MAXPNAMELEN);
    for(int i = 0; i < MAXPNAMELEN; i++)
    {
        nameString[i] = (char)(deviceCaps.szPname[i]);
    }

    std::string stringName(nameString);
    strcpy(Name, nameString);
    return stringName;
}

void RtMidiOut :: initialize(char *clientName)
{
    // We'll issue a warning here if no devices are available but not
    // throw an error since the user can plug something in later.
    unsigned int nDevices = getPortCount();
    if(nDevices == 0)
    {
        sprintf(errorString_, "RtMidiOut::initialize: no MIDI output devices currently available.");
        error(RtError::WARNING);
        return;
    }

    // Save our api-specific connection information.
    WinMidiData *data = (WinMidiData *) new WinMidiData;
    memset(data, 0, sizeof(WinMidiData));
    apiData_ = (void *) data;
}

void RtMidiOut :: openPort(unsigned int portNumber, char *portName)
{
    if(connected_)
    {
        sprintf(errorString_, "RtMidiOut::openPort: a valid connection already exists!");
        error(RtError::WARNING);
        return;
    }

    unsigned int nDevices = getPortCount();
    if(nDevices < 1)
    {
        sprintf(errorString_, "RtMidiOut::openPort: no MIDI output destinations found!");
        error(RtError::NO_DEVICES_FOUND);
        return;
    }

    if(portNumber >= nDevices)
    {
        sprintf(errorString_, "RtMidiOut::openPort: the 'portNumber' argument (%d) is invalid.", portNumber);
        error(RtError::INVALID_PARAMETER);
        return;
    }

    WinMidiData *data = static_cast<WinMidiData *> (apiData_);
    MMRESULT result = midiOutOpen(&data->outHandle,
                                  portNumber,
                                  (DWORD) NULL,
                                  (DWORD) NULL,
                                  CALLBACK_NULL);
    if(result != MMSYSERR_NOERROR)
    {
        sprintf(errorString_, "RtMidiOut::openPort: error creating Windows MM MIDI output port.");
        error(RtError::DRIVER_ERROR);
        return;
    }

    connected_ = true;
}

void RtMidiOut :: closePort(void)
{
    if(connected_)
    {
        WinMidiData *data = static_cast<WinMidiData *> (apiData_);
        // Disabled because midiOutReset triggers 0x7b (if any note was ON) and 0x79 "Reset All
        // Controllers" (to all 16 channels) CC messages which is undesirable (see issue #222)
        // midiOutReset(data->outHandle);
        midiOutClose(data->outHandle);
        data->outHandle = 0;
        connected_ = false;
    }
}

void RtMidiOut :: openVirtualPort(char *portName)
{
    // This function cannot be implemented for the Windows MM MIDI API.
    sprintf(errorString_, "RtMidiOut::openVirtualPort: cannot be implemented in Windows MM MIDI API!");
    error(RtError::WARNING);
}

RtMidiOut :: ~RtMidiOut()
{
    // Cleanup.
    WinMidiData *data = static_cast<WinMidiData *> (apiData_);
    delete data;
    apiData_ = NULL;
    data = NULL;
}

void RtMidiOut :: sendMessage(std::vector<unsigned char> *message)
{
    unsigned int nBytes = message->size();
    if(nBytes == 0)
    {
        sprintf(errorString_, "RtMidiOut::sendMessage: message argument is empty!");
        error(RtError::WARNING);
        return;
    }

    MMRESULT result;
    WinMidiData *data = static_cast<WinMidiData *> (apiData_);
    if(data && data->outHandle)
    {
        if(nBytes > 3)
        {   // Sysex message

            // Allocate buffer for sysex data.
            char *buffer = (char *) malloc(nBytes);
            if(buffer == NULL)
            {
                sprintf(errorString_, "RtMidiOut::sendMessage: error allocating sysex message memory!");
                error(RtError::MEMORY_ERROR);
                return;
            }

            // Copy data to buffer.
            for(unsigned int i = 0; i < nBytes; i++ )
            {
                buffer[i] = message->at(i);
            }

            // Create and prepare MIDIHDR structure.
            MIDIHDR sysex;
            sysex.lpData = (LPSTR) buffer;
            sysex.dwBufferLength = nBytes;
            sysex.dwFlags = 0;
            result = midiOutPrepareHeader(data->outHandle,  &sysex, sizeof(MIDIHDR));
            if( result != MMSYSERR_NOERROR)
            {
                free(buffer);
                sprintf(errorString_, "RtMidiOut::sendMessage: error preparing sysex header.");
                error(RtError::DRIVER_ERROR);
                return;
            }

            // Send the message.
            result = midiOutLongMsg(data->outHandle, &sysex, sizeof(MIDIHDR));
            if(result != MMSYSERR_NOERROR)
            {
                free(buffer);
                sprintf(errorString_, "RtMidiOut::sendMessage: error sending sysex message.");
                error(RtError::DRIVER_ERROR);
                return;
            }

            // Unprepare the buffer and MIDIHDR.
            while(MIDIERR_STILLPLAYING == midiOutUnprepareHeader(data->outHandle, &sysex, sizeof (MIDIHDR)))
            {
                Sleep(1);
            }
            free(buffer);

        }
        else
        {   // Channel or system message.

            // Make sure the message size isn't too big.
            if(nBytes > 3)
            {
                sprintf(errorString_, "RtMidiOut::sendMessage: message size is greater than 3 bytes (and not sysex)!");
                error(RtError::WARNING);
                return;
            }

            // Pack MIDI bytes into double word.
            DWORD packet;
            unsigned char *ptr = (unsigned char *) &packet;

            for(unsigned int i = 0; i < nBytes; i++)
            {
                *ptr = message->at(i);
                ++ptr;
            }

            // Send the message immediately.
            result = midiOutShortMsg(data->outHandle, packet);
            if(result != MMSYSERR_NOERROR)
            {
                sprintf(errorString_, "RtMidiOut::sendMessage: error sending MIDI message.");
                error(RtError::WARNING);
            }
        }
    }
}

#endif  // __WINDOWS_MM__

//*********************************************************************//
//  API: AROS camd.library
//*********************************************************************//

#if defined(__AROS__)

#include <pthread.h>
#include <proto/camd.h>
#include <midi/mididefs.h>

struct Library *CamdBase = NULL;

#define  RT_SYSEX_BUFFER_COUNT 4

struct CAMDMidiData
{
    struct MidiNode *ourNode;
    struct MidiLink *inHandle;      // Handle to Midi Input Device
    struct MidiLink *outHandle;     // Handle to Midi Output Device
    int MidiSig;
    DWORD lastTime;
    struct EClockVal stampeclock;
    RtMidiIn::MidiMessage message;
    LPMIDIHDR sysexBuffer[RT_SYSEX_BUFFER_COUNT];
};

//*********************************************************************//
//  API: camd.library
//  Class Definitions: RtMidiIn
//*********************************************************************//
/*
static void CALLBACK midiInputCallback(HMIDIOUT hmin,
                                       UINT inputStatus, 
                                       DWORD instancePtr,
                                       DWORD midiMessage,
                                       DWORD timestamp)
{
    if(inputStatus != MIM_DATA &&
       inputStatus != MIM_LONGDATA &&
       inputStatus != MIM_LONGERROR)
    {
        return;
    }

    RtMidiIn::RtMidiInData *data = (RtMidiIn::RtMidiInData *)instancePtr;
    WinMidiData *apiData = static_cast<WinMidiData *> (data->apiData);

    // Calculate time stamp.
    apiData->message.timeStamp = 0.0f;
    if(data->firstMessage == true)
    {
        data->firstMessage = false;
    }
    else
    {
        apiData->message.timeStamp = (double) (timestamp - apiData->lastTime) * 0.001f;
    }
    apiData->lastTime = timestamp;

    if(inputStatus == MIM_DATA)
    {   // Channel or system message

        // Make sure the first byte is a status byte.
        unsigned char status = (unsigned char) (midiMessage & 0x000000FF);
        if(!(status & 0x80))
        {
            return;
        }

        // Determine the number of bytes in the MIDI message.
        unsigned short nBytes = 1;
        if(status < 0xC0) nBytes = 3;
        else if(status < 0xE0) nBytes = 2;
        else if(status < 0xF0) nBytes = 3;
        else if(status < 0xF3)
        {
            // A MIDI time code message and we're ignoring it.
            if(status == 0xF1 && (data->ignoreFlags & 0x02) ) return;
            nBytes = 3;
        }
        else if(status == 0xF3) nBytes = 2;
        else if(status == 0xF8 && (data->ignoreFlags & 0x02))
        {
            // A MIDI timing tick message and we're ignoring it.
            return;
        }
        else if ( status == 0xFE && (data->ignoreFlags & 0x04) )
        {
            // A MIDI active sensing message and we're ignoring it.
            return;
        }

        // Copy bytes to our MIDI message.
        unsigned char *ptr = (unsigned char *) &midiMessage;
        for(int i = 0; i < nBytes; i++ )
        {
            apiData->message.bytes.push_back(*ptr++);
        }
    }
    else
    {   // Sysex message (MIM_LONGDATA or MIM_LONGERROR)
        MIDIHDR *sysex = (MIDIHDR *) midiMessage; 
        if(!(data->ignoreFlags & 0x01) && inputStatus != MIM_LONGERROR)
        {
            // Sysex message and we're not ignoring it
            for(int i = 0; i < (int) sysex->dwBytesRecorded; i++)
            {
                apiData->message.bytes.push_back(sysex->lpData[i]);
            }
        }

        // The WinMM API requires that the sysex buffer be requeued after
        // input of each sysex message.  Even if we are ignoring sysex
        // messages, we still need to requeue the buffer in case the user
        // decides to not ignore sysex messages in the future.  However,
        // it seems that WinMM calls this function with an empty sysex
        // buffer when an application closes and in this case, we should
        // avoid requeueing it, else the computer suddenly reboots after
        // one or two minutes.
	    if(apiData->sysexBuffer[sysex->dwUser]->dwBytesRecorded > 0)
        {
            MMRESULT result = midiInAddBuffer(apiData->inHandle, apiData->sysexBuffer[sysex->dwUser], sizeof(MIDIHDR));
            if (result != MMSYSERR_NOERROR)
            {
                std::cerr << "\nRtMidiIn::midiInputCallback: error sending sysex to Midi device!!\n\n";
                return;
            }
            if(data->ignoreFlags & 0x01)
            {
                return;
            }
        }
        else
        {
            return;
        }
    }

    if(data->usingCallback)
    {
        RtMidiIn::RtMidiCallback callback = (RtMidiIn::RtMidiCallback) data->userCallback;
        callback(apiData->message.timeStamp, &apiData->message.bytes, data->userData);
    }
    else
    {
        // As long as we haven't reached our queue size limit, push the message.
        if(data->queueLimit > data->queue.size())
        {
            data->queue.push(apiData->message);
        }
    }

    // Clear the vector for the next input message.
    apiData->message.bytes.clear();
}
*/

struct MidiNode *RtMidiIn :: getMidiNode(void)
{
    if(apiData_->ourNode != NULL)
    {
        return apiData_->ourNode;
    }
    
    apiData_->MidiSig = IExec->AllocSignal(-1);
    if(apiData_->MidiSig == -1)
    {
        return NULL;
    }
    
    ReadEClock(&apiData_->stampeclock);

	// Let's create MIDI node for this piece of software
    apiData_->CamdNode = (struct MidiNode *) CreateMidi(MIDI_Name,  "ProTrekkr",
								                        MIDI_TimeStamp, &apiData_->stampeclock.ev_lo,
                                                        MIDI_RecvSignal, apiData_->MidiSig,
                                                        MIDI_MsgQueue, 2048,
                                                        MIDI_SysExSize, 1024,
                                                        MIDI_ClientType, CCType_Sequencer,
								                        MIDI_ErrFilter, CMEF_All,
                                                        TAG_END
                                                       );
    if(apiData_->CamdNode == NULL)
    {
        FreeSignal(apiData_->MidiSig);
        apiData_->MidiSig = -1;
        return NULL;
    }
    return CamdNode;
}


void RtMidiIn :: initialize(char *clientName)
{
    // Save our api-specific connection information.
    CAMDMidiData *data = (CAMDMidiData *) new CAMDMidiData;
	data->id_in = 0; //MLINK_PortID
    apiData_ = (void *) data;
    inputData_.apiData = (void *) data;
    data->message.bytes.clear();  // needs to be empty for first input message
    data->MidiSig = -1;

    // We'll issue a warning here if no devices are available but not
    // throw an error since the user can plugin something later.
    apiData_->node = getMidiNode();

    if(apiData_->node == NULL)
    {
        sprintf(errorString_, "RtMidiIn::initialize: no MIDI input devices currently available.");
        error(RtError::WARNING);
        return;
    }
	
	apiData_->inHandle = AddMidiLink(apiData_->node, MLTYPE_Receiver,
                                                     MLINK_Name, clientName,
                                                     MLINK_Location, clientName,
                                                     MLINK_EventMask, CMF_Note | CMF_Mode | CMF_RealTime,
                                                     MLINK_Comment, clientName,
                                                     MLINK_PortID, 0,
                                                     TAG_DONE
                                     );
    if(apiData_->inHandle)
    {
        sprintf(errorString_, "RtMidiIn::initialize: error creating CAMD MidiLink object.");
        error(RtError::DRIVER_ERROR);
        return;
    }

}

void RtMidiIn :: openPort(unsigned int portNumber, char *portName)
{
    if(connected_)
    {
        sprintf(errorString_, "RtMidiIn::openPort: a valid connection already exists!");
        error(RtError::WARNING);
        return;
    }

    unsigned int nDevices = GetNumDevs();
    if(nDevices == 0)
    {
        sprintf(errorString_, "RtMidiIn::openPort: no MIDI input sources found!");
        error(RtError::NO_DEVICES_FOUND);
        return;
    }

    if(portNumber >= nDevices)
    {
        sprintf(errorString_, "RtMidiIn::openPort: the 'portNumber' argument (%d) is invalid.", portNumber);
        error(RtError::INVALID_PARAMETER);
        return;
    }

    CAMDMidiData *data = static_cast<CAMDMidiData *> (apiData_);
/*    MMRESULT result = midiInOpen(&data->inHandle,
                                 portNumber,
                                 (DWORD) &midiInputCallback,
                                 (DWORD) &inputData_,
                                 CALLBACK_FUNCTION);
    if(result != MMSYSERR_NOERROR)
    {
        sprintf(errorString_, "RtMidiIn::openPort: error creating Windows MM MIDI input port.");
        error(RtError::DRIVER_ERROR);
        return;
    }
*/
    // Allocate and init the sysex buffer.
    for(int i = 0; i < RT_SYSEX_BUFFER_COUNT; ++i)
    {
        data->sysexBuffer[i] = (MIDIHDR *) new char[sizeof(MIDIHDR)];
        data->sysexBuffer[i]->lpData = new char[RT_SYSEX_BUFFER_SIZE];
        data->sysexBuffer[i]->dwBufferLength = RT_SYSEX_BUFFER_SIZE;
        data->sysexBuffer[i]->dwUser = i;   // We use the dwUser parameter as buffer indicator
        data->sysexBuffer[i]->dwFlags = 0;

/*        result = midiInPrepareHeader(data->inHandle, data->sysexBuffer[i], sizeof(MIDIHDR));
        if(result != MMSYSERR_NOERROR)
        {
            midiInClose(data->inHandle);
            sprintf(errorString_, "RtMidiIn::openPort: error starting Windows MM MIDI input port (PrepareHeader).");
            error(RtError::DRIVER_ERROR);
            return;
        }

        // Register the buffer.
        result = midiInAddBuffer(data->inHandle, data->sysexBuffer[i], sizeof(MIDIHDR));
        if(result != MMSYSERR_NOERROR)
        {
            midiInClose(data->inHandle);
            sprintf(errorString_, "RtMidiIn::openPort: error starting Windows MM MIDI input port (AddBuffer).");
            error(RtError::DRIVER_ERROR);
            return;
        }*/
    }

/*    result = midiInStart(data->inHandle);
    if(result != MMSYSERR_NOERROR)
    {
        midiInClose(data->inHandle);
        sprintf(errorString_, "RtMidiIn::openPort: error starting Windows MM MIDI input port.");
        error(RtError::DRIVER_ERROR);
        return;
    }
*/
    connected_ = true;
}

void RtMidiIn :: openVirtualPort(char *portName)
{
    sprintf(errorString_, "RtMidiIn::openVirtualPort: cannot be implemented in CAMD API!");
    error(RtError::WARNING);
}

void RtMidiIn :: closePort(void)
{
    if(connected_)
    {
        CAMDMidiData *data = static_cast<CAMDMidiData *> (apiData_);
//        midiInReset(data->inHandle);
  //      midiInStop(data->inHandle);

        for(int i = 0; i < RT_SYSEX_BUFFER_COUNT; ++i)
        {
    /*        int result = midiInUnprepareHeader(data->inHandle,
                                               data->sysexBuffer[i],
                                               sizeof(MIDIHDR));
            delete []data->sysexBuffer[i]->lpData;
            delete []data->sysexBuffer[i];
            if(result != MMSYSERR_NOERROR)
            {
                midiInClose(data->inHandle);
                sprintf(errorString_, "RtMidiIn::openPort: error closing Windows MM MIDI input port (midiInUnprepareHeader).");
                error(RtError::DRIVER_ERROR);
                return;
            }
        }

        //midiInClose(data->inHandle);*/
        connected_ = false;
    }
}

RtMidiIn :: ~RtMidiIn()
{
    // Cleanup.
    CAMDMidiData *data = static_cast<CAMDMidiData *> (apiData_);

    if(data->inHandle)
    {
        ICamd->RemoveMidiLink(data->link_in);
    }  
    if(data->MidiSig)
    {
        FreeSignal(data->MidiSig);
    }
    data->MidiSig = NULL;
    if(CamdBase)
    {
        CloseLibrary(CamdBase);
    }
    delete data;
    apiData_ = NULL;
    data = NULL;
    CamdBase = NULL;
}

unsigned int RtMidiIn :: getPortCount()
{
	APTR key;
	struct MidiCluster *clus;
    unsigned int devices = 0;

    if(key = LockCAMD(CD_Linkages))
	{
        for(clus = NextCluster(NULL); clus; clus = NextCluster(clus))
		{
            devices++;
		}
		UnlockCAMD(key);
	}
    return devices;
}

std::string RtMidiIn :: getPortName(unsigned int portNumber, char *Name)
{
	APTR key;
	struct MidiCluster *clus;
    int i;

    unsigned int nDevices = getPortCount();

    if(portNumber >= nDevices)
    {
        sprintf(errorString_, "RtMidiIn::getPortName: the 'portNumber' argument (%d) is invalid.", portNumber);
        error(RtError::INVALID_PARAMETER);
        return("");
    }

    if(key = LockCAMD(CD_Linkages))
	{
        clus = NextCluster(NULL);
        if(clus)
        {
            for(i = 0 ; i < portNumber; i++)
		    {
                clus = NextCluster(clus);
		    }
        }
		UnlockCAMD(key);
	}

//    MIDIINCAPS deviceCaps;
  //  midiInGetDevCaps(portNumber, &deviceCaps, sizeof(MIDIINCAPS));

    // For some reason, we need to copy character by character with
    // UNICODE (thanks to Eduardo Coutinho!).
    char nameString[MAXPNAMELEN];
    
    memset(nameString, 0, MAXPNAMELEN);
    for(int i = 0; i < MAXPNAMELEN; i++)
    {
        nameString[i] = (char) (clus.szPname[i]);
    }

    std::string stringName(nameString);
    strcpy(Name, nameString);
    return stringName;
}

//*********************************************************************//
//  API: CAMD
//  Class Definitions: RtMidiOut
//*********************************************************************//

unsigned int RtMidiOut :: getPortCount()
{
	APTR key;
	struct MidiCluster *clus;
    unsigned int devices = 0;

    if(key = LockCAMD(CD_Linkages))
	{
        for(clus = NextCluster(NULL); clus; clus = NextCluster(clus))
		{
            devices++;
		}
		UnlockCAMD(key);
	}
    return devices;
}

std::string RtMidiOut :: getPortName(unsigned int portNumber, char *Name)
{
    unsigned int nDevices = getPortCount();

    if(portNumber >= nDevices)
    {
        sprintf(errorString_, "RtMidiOut::getPortName: the 'portNumber' argument (%d) is invalid.", portNumber);
        error(RtError::INVALID_PARAMETER);
        return("");
    }

//    MIDIOUTCAPS deviceCaps;
  //  midiOutGetDevCaps(portNumber, &deviceCaps, sizeof(MIDIOUTCAPS));

    // For some reason, we need to copy character by character with
    // UNICODE (thanks to Eduardo Coutinho!).
    char nameString[MAXPNAMELEN];
    
    memset(nameString, 0, MAXPNAMELEN);
    for(int i = 0; i < MAXPNAMELEN; i++)
    {
        nameString[i] = (char)(deviceCaps.szPname[i]);
    }

    std::string stringName(nameString);
    strcpy(Name, nameString);
    return stringName;
}

void RtMidiOut :: initialize(char *clientName)
{
    // We'll issue a warning here if no devices are available but not
    // throw an error since the user can plug something in later.
    unsigned int nDevices = getPortCount();
    if(nDevices == 0)
    {
        sprintf(errorString_, "RtMidiOut::initialize: no MIDI output devices currently available.");
        error(RtError::WARNING);
        return;
    }

    // Save our api-specific connection information.
    CAMDMidiData *data = (CAMDMidiData *) new CAMDMidiData;
    memset(data, 0, sizeof(CAMDMidiData));
    apiData_ = (void *) data;
}

void RtMidiOut :: openPort(unsigned int portNumber, char *portName)
{
    if(connected_)
    {
        sprintf(errorString_, "RtMidiOut::openPort: a valid connection already exists!");
        error(RtError::WARNING);
        return;
    }

    unsigned int nDevices = getPortCount();
    if(nDevices < 1)
    {
        sprintf(errorString_, "RtMidiOut::openPort: no MIDI output destinations found!");
        error(RtError::NO_DEVICES_FOUND);
        return;
    }

    if(portNumber >= nDevices)
    {
        sprintf(errorString_, "RtMidiOut::openPort: the 'portNumber' argument (%d) is invalid.", portNumber);
        error(RtError::INVALID_PARAMETER);
        return;
    }

    CAMDMidiData *data = static_cast<CAMDMidiData *> (apiData_);
/*    MMRESULT result = midiOutOpen(&data->outHandle,
                                  portNumber,
                                  (DWORD) NULL,
                                  (DWORD) NULL,
                                  CALLBACK_NULL);
    if(result != MMSYSERR_NOERROR)
    {
        sprintf(errorString_, "RtMidiOut::openPort: error creating CAMD MIDI output port.");
        error(RtError::DRIVER_ERROR);
        return;
    }*/

    connected_ = true;
}

void RtMidiOut :: closePort(void)
{
    if(connected_)
    {
        CAMDMidiData *data = static_cast<CAMDMidiData *> (apiData_);
//        midiOutReset(data->outHandle);
  //      midiOutClose(data->outHandle);
        connected_ = false;
    }
}

void RtMidiOut :: openVirtualPort(char *portName)
{
    // This function cannot be implemented for the AROS MIDI API.
    sprintf(errorString_, "RtMidiOut::openVirtualPort: cannot be implemented in CAMD API!");
    error(RtError::WARNING);
}

RtMidiOut :: ~RtMidiOut()
{
    // Cleanup.
    CAMDMidiData *data = static_cast<CAMDMidiData *> (apiData_);
    delete data;
    apiData_ = NULL;
    data = NULL;
}

void RtMidiOut :: sendMessage(std::vector<unsigned char> *message)
{
    unsigned int nBytes = message->size();
    if(nBytes == 0)
    {
        sprintf(errorString_, "RtMidiOut::sendMessage: message argument is empty!");
        error(RtError::WARNING);
        return;
    }

//    MMRESULT result;
    CAMDMidiData *data = static_cast<CAMDMidiData *> (apiData_);
    if(data && data->outHandle)
    {
        if(message->at(0) == 0xF0)
        { // Sysex message

            // Allocate buffer for sysex data.
            char *buffer = (char *) malloc(nBytes);
            if(buffer == NULL)
            {
                sprintf(errorString_, "RtMidiOut::sendMessage: error allocating sysex message memory!");
                error(RtError::MEMORY_ERROR);
                return;
            }

            // Copy data to buffer.
            for(unsigned int i = 0; i < nBytes; i++)
            {
                buffer[i] = message->at(i);
            }

            // Create and prepare MIDIHDR structure.
            /*MIDIHDR sysex;
            sysex.lpData = (LPSTR) buffer;
            sysex.dwBufferLength = nBytes;
            sysex.dwFlags = 0;
            result = midiOutPrepareHeader(data->outHandle,  &sysex, sizeof(MIDIHDR));
            if( result != MMSYSERR_NOERROR)
            {
                free(buffer);
                sprintf(errorString_, "RtMidiOut::sendMessage: error preparing sysex header.");
                error(RtError::DRIVER_ERROR);
                return;
            }

            // Send the message.
            result = midiOutLongMsg(data->outHandle, &sysex, sizeof(MIDIHDR));
            if(result != MMSYSERR_NOERROR)
            {
                free(buffer);
                sprintf(errorString_, "RtMidiOut::sendMessage: error sending sysex message.");
                error(RtError::DRIVER_ERROR);
                return;
            }

            // Unprepare the buffer and MIDIHDR.
            while(MIDIERR_STILLPLAYING == midiOutUnprepareHeader(data->outHandle, &sysex, sizeof (MIDIHDR)))
            {
                Sleep(1);
            }*/
            free(buffer);

        }
        else
        {   // Channel or system message.

            // Make sure the message size isn't too big.
            if(nBytes > 3)
            {
                sprintf(errorString_, "RtMidiOut::sendMessage: message size is greater than 3 bytes (and not sysex)!");
                error(RtError::WARNING);
                return;
            }

            // Pack MIDI bytes into double word.
            DWORD packet;
            unsigned char *ptr = (unsigned char *) &packet;

            for(unsigned int i = 0; i < nBytes; i++)
            {
                *ptr = message->at(i);
                ++ptr;
            }

            // Send the message immediately.
            /*result = midiOutShortMsg(data->outHandle, packet);
            if(result != MMSYSERR_NOERROR)
            {
                sprintf(errorString_, "RtMidiOut::sendMessage: error sending MIDI message.");
                error(RtError::WARNING);
            }*/
        }
    }
}

#endif  // __AROS__
