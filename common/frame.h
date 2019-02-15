#ifndef FRAME_H__
#define FRAME_H__

#include "crc16.h"
#include "protocol.h"

#define FRAME_START_SYMBOL 0xBE
#define FRAME_END_SYMBOL 0xEF
#define FRAME_MAX_PAYLOAD_SIZE 255

#define FRAME_ENCRYPTION_FLAG 0x10

typedef struct
{
    uint8_t		flags;
    uint8_t 	opcode;
    uint8_t 	payload_size;
    uint8_t 	payload[ FRAME_MAX_PAYLOAD_SIZE ];
    uint16_t  	crc;
} Frame;

Frame createFrame( uint8_t opcode, uint8_t flags, uint8_t* payload, size_t payload_size, bool encryption = false )
{
    Frame ret;
    ret.opcode = opcode;
    ret.flags = flags;
    if( payload_size <= FRAME_MAX_PAYLOAD_SIZE )
    {
        ret.payload_size = payload_size;
        memcpy( ret.payload, payload, ret.payload_size );
    }
    else
    {
        ret.payload_size = 0;
    }

    if( encryption )
    {
        ret.flags |= FRAME_ENCRYPTION_FLAG;
    }
    return ret;
}

Frame createPingFrame( uint8_t flags = 0x00 )
{
    Frame ret;
    ret.flags = flags;
    ret.payload_size = 0;
    ret.opcode = OP_PING;
    return ret;
}

Frame createPongFrame( uint8_t device_id, uint8_t device_type, uint8_t flags = 0x00 )
{
    Frame ret;
    ret.flags = flags;
    ret.payload_size = 2;
    ret.payload[ 0 ] = device_id;
    ret.payload[ 1 ] = device_type;
    ret.opcode = OP_PONG;
    return ret;
}

Frame createSetDeviceIdFrame( uint8_t device_id, uint8_t flags = 0x00 )
{
    Frame ret;
    ret.flags = flags;
    ret.payload_size = 1;
    ret.payload[ 0 ] = device_id;
    ret.opcode = OP_SET_DEVICE_ID;
    return ret;
}

Frame createGetRadioChannelFrame( uint8_t device_id, uint8_t flags = 0x00 )
{
    Frame ret;
    ret.flags = flags;
    ret.payload_size = 1;
    ret.payload[ 0 ] = device_id;
    ret.opcode = OP_GET_RADIO_CHANNEL;
    return ret;
}

Frame createSetDeviceChannelFrame( uint8_t device_id, uint8_t radio_channel, uint8_t flags = 0x00 )
{
    Frame ret;
    ret.flags = flags;
    ret.payload_size = 2;
    ret.payload[ 0 ] = device_id;
    ret.payload[ 1 ] = radio_channel;
    ret.opcode = OP_SET_RADIO_CHANNEL;
    return ret;
}

Frame createRadioChannelAckFrame( uint8_t device_id, uint8_t radio_channel, uint8_t flags = 0x00 )
{
    Frame ret;
    ret.flags = flags;
    ret.payload_size = 2;
    ret.payload[ 0 ] = device_id;
    ret.payload[ 1 ] = radio_channel;
    ret.opcode = OP_RADIO_CHANNEL_ACK;
    return ret;
}

Frame createGetRadioPowerFrame( uint8_t device_id, uint8_t flags = 0x00 )
{
    Frame ret;
    ret.flags = flags;
    ret.payload_size = 1;
    ret.payload[ 0 ] = device_id;
    ret.opcode = OP_GET_RADIO_POWER;
    return ret;
}

Frame createSetDevicePowerFrame( uint8_t device_id, uint8_t radio_power, uint8_t flags = 0x00 )
{
    Frame ret;
    ret.flags = flags;
    ret.payload_size = 2;
    ret.payload[ 0 ] = device_id;
    ret.payload[ 1 ] = radio_power;
    ret.opcode = OP_SET_RADIO_POWER;
    return ret;
}

Frame createRadioPowerAckFrame( uint8_t device_id, uint8_t radio_power, uint8_t flags = 0x00 )
{
    Frame ret;
    ret.flags = flags;
    ret.payload_size = 2;
    ret.payload[ 0 ] = device_id;
    ret.payload[ 1 ] = radio_power;
    ret.opcode = OP_RADIO_POWER_ACK;
    return ret;
}

Frame createGetRadioQualityFrame( uint8_t device_id, uint8_t flags = 0x00 )
{
    Frame ret;
    ret.flags = flags;
    ret.payload_size = 1;
    ret.payload[ 0 ] = device_id;
    ret.opcode = OP_GET_RADIO_QUALITY;
    return ret;
}

Frame createRadioQualityAckFrame( uint8_t device_id, uint8_t quality, uint8_t flags = 0x00 )
{
    Frame ret;
    ret.flags = flags;
    ret.payload_size = 2;
    ret.payload[ 0 ] = device_id;
    ret.payload[ 1 ] = quality;
    ret.opcode = OP_RADIO_QUALITY_ACK;
    return ret;
}

size_t frameToBuffer( Frame& f, uint8_t* buffer, size_t bufferSize )
{
    int offset = 0;
    int sizeRequirement = 7 + f.payload_size;
    if( bufferSize < sizeRequirement )
    {
        // Fuck
    }
    else
    {
        buffer[ offset++ ] = FRAME_START_SYMBOL;
        buffer[ offset++ ] = f.flags;
        buffer[ offset++ ] = f.opcode;
        buffer[ offset++ ] = f.payload_size;
        for( int k = 0; k < f.payload_size; ++k )
        {
            buffer[ offset++ ] = f.payload[k];
        }

        // Compute CRC
        CRC16 crc;
        uint16_t compCRC = crc.processBuffer( (const char*)(buffer + 2), f.payload_size + 2 );
        memcpy( buffer + offset, &compCRC, sizeof(uint16_t) );
        offset += sizeof(uint16_t);

        buffer[ offset++ ] = FRAME_END_SYMBOL;

        return offset;

    }
    return 0;
}

#endif
