#ifndef _ROS_trex_robot_control_TrexMotorCmds_h
#define _ROS_trex_robot_control_TrexMotorCmds_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace trex_robot_control
{

  class TrexMotorCmds : public ros::Msg
  {
    public:
      uint8_t cmd_type;
      char * cmd_msg;
      uint8_t left_mode;
      uint8_t right_mode;
      int32_t left_speed;
      int32_t right_speed;

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      *(outbuffer + offset + 0) = (this->cmd_type >> (8 * 0)) & 0xFF;
      offset += sizeof(this->cmd_type);
      uint32_t length_cmd_msg = strlen( (const char*) this->cmd_msg);
      memcpy(outbuffer + offset, &length_cmd_msg, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->cmd_msg, length_cmd_msg);
      offset += length_cmd_msg;
      *(outbuffer + offset + 0) = (this->left_mode >> (8 * 0)) & 0xFF;
      offset += sizeof(this->left_mode);
      *(outbuffer + offset + 0) = (this->right_mode >> (8 * 0)) & 0xFF;
      offset += sizeof(this->right_mode);
      union {
        int32_t real;
        uint32_t base;
      } u_left_speed;
      u_left_speed.real = this->left_speed;
      *(outbuffer + offset + 0) = (u_left_speed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_left_speed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_left_speed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_left_speed.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->left_speed);
      union {
        int32_t real;
        uint32_t base;
      } u_right_speed;
      u_right_speed.real = this->right_speed;
      *(outbuffer + offset + 0) = (u_right_speed.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_right_speed.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_right_speed.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_right_speed.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->right_speed);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      this->cmd_type =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->cmd_type);
      uint32_t length_cmd_msg;
      memcpy(&length_cmd_msg, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_cmd_msg; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_cmd_msg-1]=0;
      this->cmd_msg = (char *)(inbuffer + offset-1);
      offset += length_cmd_msg;
      this->left_mode =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->left_mode);
      this->right_mode =  ((uint8_t) (*(inbuffer + offset)));
      offset += sizeof(this->right_mode);
      union {
        int32_t real;
        uint32_t base;
      } u_left_speed;
      u_left_speed.base = 0;
      u_left_speed.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_left_speed.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_left_speed.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_left_speed.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->left_speed = u_left_speed.real;
      offset += sizeof(this->left_speed);
      union {
        int32_t real;
        uint32_t base;
      } u_right_speed;
      u_right_speed.base = 0;
      u_right_speed.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_right_speed.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_right_speed.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_right_speed.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->right_speed = u_right_speed.real;
      offset += sizeof(this->right_speed);
     return offset;
    }

    const char * getType(){ return "trex_robot_control/TrexMotorCmds"; };
    const char * getMD5(){ return "86a12dd4d290bdb21df900bf9b21d980"; };

  };

}
#endif