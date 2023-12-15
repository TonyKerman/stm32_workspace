#pragma once
// MESSAGE POSE PACKING

#define MAVLINK_MSG_ID_POSE 3


typedef struct __mavlink_pose_t {
 double x; /*<  x 方向*/
 double y; /*<  y 方向*/
 double w; /*<  z 方向角*/
} mavlink_pose_t;

#define MAVLINK_MSG_ID_POSE_LEN 24
#define MAVLINK_MSG_ID_POSE_MIN_LEN 24
#define MAVLINK_MSG_ID_3_LEN 24
#define MAVLINK_MSG_ID_3_MIN_LEN 24

#define MAVLINK_MSG_ID_POSE_CRC 182
#define MAVLINK_MSG_ID_3_CRC 182



#if MAVLINK_COMMAND_24BIT
#define MAVLINK_MESSAGE_INFO_POSE { \
    3, \
    "POSE", \
    3, \
    {  { "x", NULL, MAVLINK_TYPE_DOUBLE, 0, 0, offsetof(mavlink_pose_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_DOUBLE, 0, 8, offsetof(mavlink_pose_t, y) }, \
         { "w", NULL, MAVLINK_TYPE_DOUBLE, 0, 16, offsetof(mavlink_pose_t, w) }, \
         } \
}
#else
#define MAVLINK_MESSAGE_INFO_POSE { \
    "POSE", \
    3, \
    {  { "x", NULL, MAVLINK_TYPE_DOUBLE, 0, 0, offsetof(mavlink_pose_t, x) }, \
         { "y", NULL, MAVLINK_TYPE_DOUBLE, 0, 8, offsetof(mavlink_pose_t, y) }, \
         { "w", NULL, MAVLINK_TYPE_DOUBLE, 0, 16, offsetof(mavlink_pose_t, w) }, \
         } \
}
#endif

/**
 * @brief Pack a pose message
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 *
 * @param x  x 方向
 * @param y  y 方向
 * @param w  z 方向角
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_pose_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,
                               double x, double y, double w)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_POSE_LEN];
    _mav_put_double(buf, 0, x);
    _mav_put_double(buf, 8, y);
    _mav_put_double(buf, 16, w);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_POSE_LEN);
#else
    mavlink_pose_t packet;
    packet.x = x;
    packet.y = y;
    packet.w = w;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_POSE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_POSE;
    return mavlink_finalize_message(msg, system_id, component_id, MAVLINK_MSG_ID_POSE_MIN_LEN, MAVLINK_MSG_ID_POSE_LEN, MAVLINK_MSG_ID_POSE_CRC);
}

/**
 * @brief Pack a pose message on a channel
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param x  x 方向
 * @param y  y 方向
 * @param w  z 方向角
 * @return length of the message in bytes (excluding serial stream start sign)
 */
static inline uint16_t mavlink_msg_pose_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,
                                   double x,double y,double w)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_POSE_LEN];
    _mav_put_double(buf, 0, x);
    _mav_put_double(buf, 8, y);
    _mav_put_double(buf, 16, w);

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), buf, MAVLINK_MSG_ID_POSE_LEN);
#else
    mavlink_pose_t packet;
    packet.x = x;
    packet.y = y;
    packet.w = w;

        memcpy(_MAV_PAYLOAD_NON_CONST(msg), &packet, MAVLINK_MSG_ID_POSE_LEN);
#endif

    msg->msgid = MAVLINK_MSG_ID_POSE;
    return mavlink_finalize_message_chan(msg, system_id, component_id, chan, MAVLINK_MSG_ID_POSE_MIN_LEN, MAVLINK_MSG_ID_POSE_LEN, MAVLINK_MSG_ID_POSE_CRC);
}

/**
 * @brief Encode a pose struct
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param msg The MAVLink message to compress the data into
 * @param pose C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_pose_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_pose_t* pose)
{
    return mavlink_msg_pose_pack(system_id, component_id, msg, pose->x, pose->y, pose->w);
}

/**
 * @brief Encode a pose struct on a channel
 *
 * @param system_id ID of this system
 * @param component_id ID of this component (e.g. 200 for IMU)
 * @param chan The MAVLink channel this message will be sent over
 * @param msg The MAVLink message to compress the data into
 * @param pose C-struct to read the message contents from
 */
static inline uint16_t mavlink_msg_pose_encode_chan(uint8_t system_id, uint8_t component_id, uint8_t chan, mavlink_message_t* msg, const mavlink_pose_t* pose)
{
    return mavlink_msg_pose_pack_chan(system_id, component_id, chan, msg, pose->x, pose->y, pose->w);
}

/**
 * @brief Send a pose message
 * @param chan MAVLink channel to send the message
 *
 * @param x  x 方向
 * @param y  y 方向
 * @param w  z 方向角
 */
#ifdef MAVLINK_USE_CONVENIENCE_FUNCTIONS

static inline void mavlink_msg_pose_send(mavlink_channel_t chan, double x, double y, double w)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char buf[MAVLINK_MSG_ID_POSE_LEN];
    _mav_put_double(buf, 0, x);
    _mav_put_double(buf, 8, y);
    _mav_put_double(buf, 16, w);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_POSE, buf, MAVLINK_MSG_ID_POSE_MIN_LEN, MAVLINK_MSG_ID_POSE_LEN, MAVLINK_MSG_ID_POSE_CRC);
#else
    mavlink_pose_t packet;
    packet.x = x;
    packet.y = y;
    packet.w = w;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_POSE, (const char *)&packet, MAVLINK_MSG_ID_POSE_MIN_LEN, MAVLINK_MSG_ID_POSE_LEN, MAVLINK_MSG_ID_POSE_CRC);
#endif
}

/**
 * @brief Send a pose message
 * @param chan MAVLink channel to send the message
 * @param struct The MAVLink struct to serialize
 */
static inline void mavlink_msg_pose_send_struct(mavlink_channel_t chan, const mavlink_pose_t* pose)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    mavlink_msg_pose_send(chan, pose->x, pose->y, pose->w);
#else
    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_POSE, (const char *)pose, MAVLINK_MSG_ID_POSE_MIN_LEN, MAVLINK_MSG_ID_POSE_LEN, MAVLINK_MSG_ID_POSE_CRC);
#endif
}

#if MAVLINK_MSG_ID_POSE_LEN <= MAVLINK_MAX_PAYLOAD_LEN
/*
  This variant of _send() can be used to save stack space by re-using
  memory from the receive buffer.  The caller provides a
  mavlink_message_t which is the size of a full mavlink message. This
  is usually the receive buffer for the channel, and allows a reply to an
  incoming message with minimum stack space usage.
 */
static inline void mavlink_msg_pose_send_buf(mavlink_message_t *msgbuf, mavlink_channel_t chan,  double x, double y, double w)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    char *buf = (char *)msgbuf;
    _mav_put_double(buf, 0, x);
    _mav_put_double(buf, 8, y);
    _mav_put_double(buf, 16, w);

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_POSE, buf, MAVLINK_MSG_ID_POSE_MIN_LEN, MAVLINK_MSG_ID_POSE_LEN, MAVLINK_MSG_ID_POSE_CRC);
#else
    mavlink_pose_t *packet = (mavlink_pose_t *)msgbuf;
    packet->x = x;
    packet->y = y;
    packet->w = w;

    _mav_finalize_message_chan_send(chan, MAVLINK_MSG_ID_POSE, (const char *)packet, MAVLINK_MSG_ID_POSE_MIN_LEN, MAVLINK_MSG_ID_POSE_LEN, MAVLINK_MSG_ID_POSE_CRC);
#endif
}
#endif

#endif

// MESSAGE POSE UNPACKING


/**
 * @brief Get field x from pose message
 *
 * @return  x 方向
 */
static inline double mavlink_msg_pose_get_x(const mavlink_message_t* msg)
{
    return _MAV_RETURN_double(msg,  0);
}

/**
 * @brief Get field y from pose message
 *
 * @return  y 方向
 */
static inline double mavlink_msg_pose_get_y(const mavlink_message_t* msg)
{
    return _MAV_RETURN_double(msg,  8);
}

/**
 * @brief Get field w from pose message
 *
 * @return  z 方向角
 */
static inline double mavlink_msg_pose_get_w(const mavlink_message_t* msg)
{
    return _MAV_RETURN_double(msg,  16);
}

/**
 * @brief Decode a pose message into a struct
 *
 * @param msg The message to decode
 * @param pose C-struct to decode the message contents into
 */
static inline void mavlink_msg_pose_decode(const mavlink_message_t* msg, mavlink_pose_t* pose)
{
#if MAVLINK_NEED_BYTE_SWAP || !MAVLINK_ALIGNED_FIELDS
    pose->x = mavlink_msg_pose_get_x(msg);
    pose->y = mavlink_msg_pose_get_y(msg);
    pose->w = mavlink_msg_pose_get_w(msg);
#else
        uint8_t len = msg->len < MAVLINK_MSG_ID_POSE_LEN? msg->len : MAVLINK_MSG_ID_POSE_LEN;
        memset(pose, 0, MAVLINK_MSG_ID_POSE_LEN);
    memcpy(pose, _MAV_PAYLOAD(msg), len);
#endif
}
