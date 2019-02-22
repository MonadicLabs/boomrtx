#ifndef PROTOCOL_H
#define PROTOCOL_H

typedef enum
{
    OP_PING = 0,
    OP_PONG,
    OP_SET_DEVICE_ID,
    OP_GET_RADIO_CHANNEL,
    OP_SET_RADIO_CHANNEL,
    OP_RADIO_CHANNEL_ACK,
    OP_GET_RADIO_POWER,
    OP_SET_RADIO_POWER,
    OP_RADIO_POWER_ACK,
    OP_GET_RADIO_QUALITY,
    OP_RADIO_QUALITY_ACK,
    OP_GET_TARGET_ID,
    OP_SET_TARGET_ID,
    OP_TARGET_ID_ACK,
    OP_DEBUG_MESSAGE
} OpCode;

#endif // PROTOCOL_H
