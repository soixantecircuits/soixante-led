
#pragma once
#include "ofMain.h"
#include "ofxOPC.h"
#include "Led.h"

class MyofxOPC : public ofxOPC {
  public:
  //--------------------------------------------------------------
  void writeChannel(uint8_t channel, vector<Led>pix, int offset = 0)
  {
      // Bail early if there's no pixel data or there is too much data
      if(pix.empty())
      {
          ofLogError() << "No Data";
          return;

      } else if(channel < 1 || channel > 8) {
          // TODO: Emit error
          return;
      }

      uint16_t channel_offset = (channel - 1) * 64;
      for (unsigned int i = offset; i < pix.size() && i < offset + 64 ; i++) {
          OPC_SPC_packet_data[channel_offset + i].r = pix[i].c.r;
          OPC_SPC_packet_data[channel_offset + i].g = pix[i].c.g;
          OPC_SPC_packet_data[channel_offset + i].b = pix[i].c.b;
      }
      
      // Send the data
      client.sendRawBytes((char *)(OPC_SPC_packet), OPC_SPC_packet_length);
  }
  //--------------------------------------------------------------
  void writeAllChannels(vector<Led>pix, int offset = 0)
  {
      // Bail early if there's no pixel data or there is too much data
      if(pix.empty())
      {
          ofLogError() << "No Data";
          return;

      } 

      uint16_t channel_offset = 0;
      for (unsigned int i = offset; i < pix.size() && i < offset + 512 ; i++) {
          OPC_SPC_packet_data[channel_offset + i].r = pix[i].c.r;
          OPC_SPC_packet_data[channel_offset + i].g = pix[i].c.g;
          OPC_SPC_packet_data[channel_offset + i].b = pix[i].c.b;
      }
      
      // Send the data
      client.sendRawBytes((char *)(OPC_SPC_packet), OPC_SPC_packet_length);
  }
  //--------------------------------------------------------------
  void writeAll(vector<Led>pix, int offset = 0, int opc_offset = 0)
  {
      // Bail early if there's no pixel data or there is too much data
      if(pix.empty())
      {
          ofLogError() << "No Data";
          return;

      } 

      uint16_t channel_offset = 0;
      for (unsigned int i = offset; i < pix.size() && i < 8*64* _fadecandyCount ; i++) {
          OPC_SPC_packet_data[channel_offset + opc_offset + i].r = pix[i].c.r;
          OPC_SPC_packet_data[channel_offset + opc_offset + i].g = pix[i].c.g;
          OPC_SPC_packet_data[channel_offset + opc_offset + i].b = pix[i].c.b;
      }
      
      // Send the data
      client.sendRawBytes((char *)(OPC_SPC_packet), OPC_SPC_packet_length);
  }
};
