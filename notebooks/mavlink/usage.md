# mavlink 文件结构

使用mavlink生成器生成C lib后,文件结构

    -mavlink
    |
    --common
    -mavlink_msg_xxx.h
    -....
    --minimal
    -xxx.h
    |
    xxx.h

每一个 malink_msg_xxx.h 都定义了：
1. 一个msg结构体`mavlink_xxx_t`
2. `mavlink_msg_xxx_pack()`函数
3. `mavlink_msg_xxx_pack_chan()`函数
4. `mavlik_msg_xxx_encode()`函数
5. `mavlink_msg_xxx_encode_chan()`函数
6. `mavlink_msg_xxx_decode()`函数
7. `mavlink_msg_xxx_get_xxxx()`函数

# 函数作用

### `mavlink_msg_xxx_pack()`函数
```c
static inline uint16_t mavlink_msg_attitude_quaternion_pack(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg,....)
```
生成一个msg结构体


 * @brief Pack a attitude_quaternion message
 * @param system_id ID of this system 系统ID
 * @param component_id ID of this component (e.g. 200 for IMU) 部件ID
 * @param msg The MAVLink message to compress the data into 输出的msg结构体指针
 * @param 其他参数
 * @return length of the message in bytes (excluding serial stream start sign) msg字节数

## `mavlink_msg_xxx_pack_chan()`函数
```c
static inline uint16_t mavlink_msg_attitude_quaternion_pack_chan(uint8_t system_id, uint8_t component_id, uint8_t chan,
                               mavlink_message_t* msg,...)
```
在指定的Chan中生成特定msg

## `mavlik_msg_xxx_encode()`函数
```c
static inline uint16_t mavlink_msg_attitude_quaternion_encode(uint8_t system_id, uint8_t component_id, mavlink_message_t* msg, const mavlink_xxx_t* xxx)

```
根据一个mavlink_xxx_t结构体生成一个msg


## `mavlink_msg_xxx_get_xxxx()`函数
```c
static inline uint32_t mavlink_msg_xxx_get_xxxx(const mavlink_message_t* msg)

```
从msg获取一个内容