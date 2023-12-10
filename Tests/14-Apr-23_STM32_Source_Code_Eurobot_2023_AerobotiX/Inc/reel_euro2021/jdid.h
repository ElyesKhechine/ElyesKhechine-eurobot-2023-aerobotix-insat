#ifndef _ROS_reel__euro2021_jdid_h
#define _ROS_reel__euro2021_jdid_h

#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "ros/msg.h"
#include "reel_euro2021/c.h"

namespace reel_euro2021
{

  class jdid : public ros::Msg
  {
    public:
      typedef const char* _command_type;
      _command_type command;
      typedef float _x_type;
      _x_type x;
      typedef float _y_type;
      _y_type y;
      typedef float _phi_type;
      _phi_type phi;
      typedef int32_t _vitesse_type;
      _vitesse_type vitesse;
      uint32_t curv_path_length;
      typedef reel_euro2021::c _curv_path_type;
      _curv_path_type st_curv_path;
      _curv_path_type * curv_path;
      typedef int32_t _nombre_type;
      _nombre_type nombre;
      typedef const char* _result_type;
      _result_type result;
      typedef bool _success_type;
      _success_type success;

    jdid():
      command(""),
      x(0),
      y(0),
      phi(0),
      vitesse(0),
      curv_path_length(0), st_curv_path(), curv_path(nullptr),
      nombre(0),
      result(""),
      success(0)
    {
    }

    virtual int serialize(unsigned char *outbuffer) const override
    {
      int offset = 0;
      uint32_t length_command = strlen(this->command);
      varToArr(outbuffer + offset, length_command);
      offset += 4;
      memcpy(outbuffer + offset, this->command, length_command);
      offset += length_command;
      union {
        float real;
        uint32_t base;
      } u_x;
      u_x.real = this->x;
      *(outbuffer + offset + 0) = (u_x.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_x.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_x.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_x.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->x);
      union {
        float real;
        uint32_t base;
      } u_y;
      u_y.real = this->y;
      *(outbuffer + offset + 0) = (u_y.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_y.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_y.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_y.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->y);
      union {
        float real;
        uint32_t base;
      } u_phi;
      u_phi.real = this->phi;
      *(outbuffer + offset + 0) = (u_phi.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_phi.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_phi.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_phi.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->phi);
      union {
        int32_t real;
        uint32_t base;
      } u_vitesse;
      u_vitesse.real = this->vitesse;
      *(outbuffer + offset + 0) = (u_vitesse.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_vitesse.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_vitesse.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_vitesse.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->vitesse);
      *(outbuffer + offset + 0) = (this->curv_path_length >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (this->curv_path_length >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (this->curv_path_length >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (this->curv_path_length >> (8 * 3)) & 0xFF;
      offset += sizeof(this->curv_path_length);
      for( uint32_t i = 0; i < curv_path_length; i++){
      offset += this->curv_path[i].serialize(outbuffer + offset);
      }
      union {
        int32_t real;
        uint32_t base;
      } u_nombre;
      u_nombre.real = this->nombre;
      *(outbuffer + offset + 0) = (u_nombre.base >> (8 * 0)) & 0xFF;
      *(outbuffer + offset + 1) = (u_nombre.base >> (8 * 1)) & 0xFF;
      *(outbuffer + offset + 2) = (u_nombre.base >> (8 * 2)) & 0xFF;
      *(outbuffer + offset + 3) = (u_nombre.base >> (8 * 3)) & 0xFF;
      offset += sizeof(this->nombre);
      uint32_t length_result = strlen(this->result);
      varToArr(outbuffer + offset, length_result);
      offset += 4;
      memcpy(outbuffer + offset, this->result, length_result);
      offset += length_result;
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.real = this->success;
      *(outbuffer + offset + 0) = (u_success.base >> (8 * 0)) & 0xFF;
      offset += sizeof(this->success);
      return offset;
    }

    virtual int deserialize(unsigned char *inbuffer) override
    {
      int offset = 0;
      uint32_t length_command;
      arrToVar(length_command, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_command; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_command-1]=0;
      this->command = (char *)(inbuffer + offset-1);
      offset += length_command;
      union {
        float real;
        uint32_t base;
      } u_x;
      u_x.base = 0;
      u_x.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_x.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_x.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_x.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->x = u_x.real;
      offset += sizeof(this->x);
      union {
        float real;
        uint32_t base;
      } u_y;
      u_y.base = 0;
      u_y.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_y.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_y.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_y.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->y = u_y.real;
      offset += sizeof(this->y);
      union {
        float real;
        uint32_t base;
      } u_phi;
      u_phi.base = 0;
      u_phi.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_phi.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_phi.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_phi.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->phi = u_phi.real;
      offset += sizeof(this->phi);
      union {
        int32_t real;
        uint32_t base;
      } u_vitesse;
      u_vitesse.base = 0;
      u_vitesse.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_vitesse.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_vitesse.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_vitesse.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->vitesse = u_vitesse.real;
      offset += sizeof(this->vitesse);
      uint32_t curv_path_lengthT = ((uint32_t) (*(inbuffer + offset))); 
      curv_path_lengthT |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1); 
      curv_path_lengthT |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2); 
      curv_path_lengthT |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3); 
      offset += sizeof(this->curv_path_length);
      if(curv_path_lengthT > curv_path_length)
        this->curv_path = (reel_euro2021::c*)realloc(this->curv_path, curv_path_lengthT * sizeof(reel_euro2021::c));
      curv_path_length = curv_path_lengthT;
      for( uint32_t i = 0; i < curv_path_length; i++){
      offset += this->st_curv_path.deserialize(inbuffer + offset);
        memcpy( &(this->curv_path[i]), &(this->st_curv_path), sizeof(reel_euro2021::c));
      }
      union {
        int32_t real;
        uint32_t base;
      } u_nombre;
      u_nombre.base = 0;
      u_nombre.base |= ((uint32_t) (*(inbuffer + offset + 0))) << (8 * 0);
      u_nombre.base |= ((uint32_t) (*(inbuffer + offset + 1))) << (8 * 1);
      u_nombre.base |= ((uint32_t) (*(inbuffer + offset + 2))) << (8 * 2);
      u_nombre.base |= ((uint32_t) (*(inbuffer + offset + 3))) << (8 * 3);
      this->nombre = u_nombre.real;
      offset += sizeof(this->nombre);
      uint32_t length_result;
      arrToVar(length_result, (inbuffer + offset));
      offset += 4;
      for(unsigned int k= offset; k< offset+length_result; ++k){
          inbuffer[k-1]=inbuffer[k];
      }
      inbuffer[offset+length_result-1]=0;
      this->result = (char *)(inbuffer + offset-1);
      offset += length_result;
      union {
        bool real;
        uint8_t base;
      } u_success;
      u_success.base = 0;
      u_success.base |= ((uint8_t) (*(inbuffer + offset + 0))) << (8 * 0);
      this->success = u_success.real;
      offset += sizeof(this->success);
     return offset;
    }

    virtual const char * getType() override { return "reel_euro2021/jdid"; };
    virtual const char * getMD5() override { return "315324b015440746ea5fe7b0ee76487a"; };

  };

}
#endif
