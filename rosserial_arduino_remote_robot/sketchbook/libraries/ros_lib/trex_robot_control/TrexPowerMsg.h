#ifndef _ROS_trex_robot_control_TrexPowerMsg_h
#define _ROS_trex_robot_control_TrexPowerMsg_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"

namespace trex_robot_control
{

  class TrexPowerMsg : public ros::Msg
  {
    public:
      char * power_msg;
      float voltage_level;
      float timing_msg;

    virtual int serialize(unsigned char *outbuffer) const
    {
      int offset = 0;
      uint32_t length_power_msg = strlen( (const char*) this->power_msg);
      memcpy(outbuffer + offset, &length_power_msg, sizeof(uint32_t));
      offset += 4;
      memcpy(outbuffer + offset, this->power_msg, length_power_msg);
      offset += length_power_msg;
      union {
        float real;
        uint32_t base;
      } u_voltage_level;
      u_voltage_level.real = this->voltage_level;
      *(outbuffer + offset + 0) = (u_voltage_level.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_voltage_level.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_voltage_level.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_voltage_level.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->voltage_level);
      int32_t * val_timing_msg = (int32_t *) &(this->timing_msg);
      int32_t exp_timing_msg = (((*val_timing_msg)>>23)&255);
      if(exp_timing_msg != 0)
        exp_timing_msg += 1023-127;
      int32_t sig_timing_msg = *val_timing_msg;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = 0;
      *(outbuffer + offset++) = (sig_timing_msg<<5) & 0xff;
      *(outbuffer + offset++) = (sig_timing_msg>>3) & 0xff;
      *(outbuffer + offset++) = (sig_timing_msg>>11) & 0xff;
      *(outbuffer + offset++) = ((exp_timing_msg<<4) & 0xF0) | ((sig_timing_msg>>19)&0x0F);
      *(outbuffer + offset++) = (exp_timing_msg>>4) & 0x7F;
      if(this->timing_msg < 0) *(outbuffer + offset -1) |= 0x80;
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer)
    {
      int offset = 0;
      uint32_t length_power_msg;
      memcpy(&length_power_msg, (inbuffer + offset), sizeof(uint32_t));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_power_msg; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_power_msg-1]=0;
      this->power_msg = (char *)(inbuffer + offset-1);
      offset += length_power_msg;
      union {
        float real;
        uint32_t base;
      } u_voltage_level;
      u_voltage_level.base = 0;
      u_voltage_level.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_voltage_level.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_voltage_level.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_voltage_level.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->voltage_level = u_voltage_level.real;
      offset += sizeof(this->voltage_level);
      uint32_t * val_timing_msg = (uint32_t*) &(this->timing_msg);
      offset += 3;
      *val_timing_msg = ((uint32_t)(*(inbuffer + offset++))>>5 & 0x07);
      *val_timing_msg |= ((uint32_t)(*(inbuffer + offset++)) & 0xff)<<3;
      *val_timing_msg |= ((uint32_t)(*(inbuffer + offset++)) & 0xff)<<11;
      *val_timing_msg |= ((uint32_t)(*(inbuffer + offset)) & 0x0f)<<19;
      uint32_t exp_timing_msg = ((uint32_t)(*(inbuffer + offset++))&0xf0)>>4;
      exp_timing_msg |= ((uint32_t)(*(inbuffer + offset)) & 0x7f)<<4;
      if(exp_timing_msg !=0)
        *val_timing_msg |= ((exp_timing_msg)-1023+127)<<23;
      if( ((*(inbuffer+offset++)) & 0x80) > 0) this->timing_msg = -this->timing_msg;
     return offset;
    }

    const char * getType(){ return "trex_robot_control/TrexPowerMsg"; };
    const char * getMD5(){ return "4fc89064f1eaee525f4c21624b9de2f0"; };

  };

}
#endif