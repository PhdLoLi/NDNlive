/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2015 Regents of the University of California.
 *
 * @author Lijing Wang wanglj11@mails.tsinghua.edu.cn
 */

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
#ifndef VIDEO_GENERATOR_HPP
#define VIDEO_GENERATOR_HPP

#include <fstream>
#include <iostream>
#include <ctime>
#include <pthread.h>
#include <string>
#include <vector>
#include <gst/gst.h>
#include "producer-callback.hpp"
#include <ndn-cxx/contexts/producer-context.hpp>


namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentions
#define IDENTITY_NAME "/Lijing/Wang"  
  class Signer
{
public:
  Signer()
  : m_identityName(IDENTITY_NAME)
  {
    m_keyChain.createIdentity(m_identityName);
  };
  
  void
  onPacket(Data& data)
  {
    m_keyChain.signByIdentity(data, m_identityName);
  }
  
private:
  KeyChain m_keyChain;
  Name m_identityName;
};


  class VideoGenerator
  {
    public:
      struct Producer_Need
      {
        GstElement *sink;
//        Producer *streaminfoProducer;
//        Producer *sampleProducer;
        std::string name;
        std::string prefix;
        std::string stream_id;
        std::string streaminfo;
        gsize throughput;
        ProducerCallback streaminfoCB;
        ProducerCallback sampleCB;

//        ProducerCallback cbProducer;
      };

      VideoGenerator();
      char * 
      generateVideoOnce(std::string filename, long &size);
      char *
      playbin_file_info (std::string filename);
      void
      playbin_generate_frames (std::string filename, Producer * producer);
      void 
      h264_generate_frames (std::string filename, Producer * producer);
      void 
      h264_generate_capture (std::string, std::string, Producer_Need *pro_video, Producer_Need *pro_audio);
      void
      h264_file_info (std::string filename);

    private:

      struct GstElement_Duo
      {
        GstElement *video;
        GstElement *audio;
      };

      struct GstCaps_Duo
      {
        GstCaps *video;
        GstCaps *audio;
      };

      struct GstSample_Duo
      {
        GstSample *video;
        GstSample *audio;
      };


/* 
 * Lijing Wang
 * Now use thread produce streaminfo & frames & samples seprately. 
 *
 */
      static void
      *produce_thread (void * threadData)
      {
        Producer_Need *pro;
        pro = (Producer_Need *) threadData;
        GstCaps *caps;
        GstSample *sample;
        std::string streaminfo;
        GstBuffer *buffer;
        GstMapInfo map;

        Producer *streaminfoProducer;
        Producer *sampleProducer;

        time_t time_start = std::time(0);
        size_t samplenumber = 0;

//        ProducerCallback cb_producer;
//        std::cout << pro->prefix + "/" + pro->stream_id + "/" + pro->name +  "/streaminfo" << std::endl;
        Name videoName_streaminfo = Name(pro->prefix).append(pro->stream_id).append(pro->name +  "/streaminfo");
      /* streaminfoFrameProducer */
        streaminfoProducer = new Producer(videoName_streaminfo);
        pro->streaminfoCB.setProducer(streaminfoProducer); // needed for some callback functionality
        pro->streaminfoCB.setSampleNumber(&samplenumber);
        streaminfoProducer->setContextOption(INTEREST_ENTER_CNTX,
                      (ProducerInterestCallback)bind(&ProducerCallback::processIncomingInterest, &(pro->streaminfoCB), _1, _2));
        streaminfoProducer->setContextOption(DATA_LEAVE_CNTX,
                      (ProducerDataCallback)bind(&ProducerCallback::processOutgoingStreaminfo, &(pro->streaminfoCB), _1, _2));
        streaminfoProducer->setContextOption(CACHE_MISS,
                      (ProducerInterestCallback)bind(&ProducerCallback::processStreaminfoInterest, &(pro->streaminfoCB), _1, _2));
        streaminfoProducer->attach();

//        Signer signer;
        Name videoName_content = Name(pro->prefix).append(pro->stream_id).append(pro->name + "/content");
        sampleProducer = new Producer(videoName_content);
        pro->sampleCB.setProducer(sampleProducer); // needed for some callback functionality
        pro->sampleCB.setSampleNumber(&samplenumber);
        if(pro->name == "video")
        {
          std::cout << "I'm video~ "<<std::endl;
          sampleProducer->setContextOption(SND_BUF_SIZE,100000);
//          sampleProducer->setContextOption(EMBEDDED_MANIFESTS, true);
//          sampleProducer->setContextOption(DATA_TO_SECURE,
//               (DataCallback)bind(&Signer::onPacket, &signer, _1));

        }else
        {
          sampleProducer->setContextOption(SND_BUF_SIZE,100000);
//          sampleProducer->setContextOption(DATA_TO_SECURE,
//               (DataCallback)bind(&Signer::onPacket, &signer, _1));
        }
        sampleProducer->setContextOption(INTEREST_ENTER_CNTX,
                        (ProducerInterestCallback)bind(&ProducerCallback::processIncomingInterest, &(pro->sampleCB), _1, _2));
        sampleProducer->setContextOption(DATA_LEAVE_CNTX,
            (ProducerDataCallback)bind(&ProducerCallback::processOutgoingData, &(pro->sampleCB), _1, _2));
        sampleProducer->setContextOption(CACHE_MISS,
                          (ProducerInterestCallback)bind(&ProducerCallback::processInterest, &(pro->sampleCB), _1, _2));
        sampleProducer->attach();          
        
        auto start = std::chrono::high_resolution_clock::now();

        do {
          g_signal_emit_by_name (pro->sink, "pull-sample", &sample);
          if (sample == NULL){
            g_print("Meet the EOS!\n");
            break;
            }
          if (samplenumber == 0)
          {
            caps = gst_sample_get_caps(sample);
            streaminfo = gst_caps_to_string(caps);
            pro->streaminfo = streaminfo; //Remember the streaminfoSuffix

            if(pro->name == "video")
            {
              pro->streaminfoCB.setStreaminfoVideo(streaminfo); 
            }else
            {
              pro->streaminfoCB.setStreaminfoAudio(streaminfo); 
            }

//            Name streaminfoSuffix("");
            Name streaminfoSuffix(std::to_string(samplenumber));
            streaminfoProducer->produce(streaminfoSuffix, (uint8_t *)streaminfo.c_str(), streaminfo.size()+1);
//            std::cout << "produce " << pro->name << " streaminfo OK" << streaminfo << std::endl;
//            std::cout << "streaminfo size "<< streaminfo.size() + 1 << std::endl;
          }
          buffer = gst_sample_get_buffer (sample);
          gst_buffer_map (buffer, &map, GST_MAP_READ);
          Name sampleSuffix(std::to_string(samplenumber));
//          std::cout << pro->name << " sample number: "<< std::dec << samplenumber <<std::endl;
//          std::cout << pro->name <<" sample Size: "<< std::dec << map.size * sizeof(uint8_t) <<std::endl;
          pro->throughput += map.size * sizeof(uint8_t);

//          if( samplenumber % 100 != 0)
          sampleProducer->produce(sampleSuffix, (uint8_t *)map.data, map.size * sizeof(uint8_t));

          auto finish = std::chrono::high_resolution_clock::now();
//          printf("%s -- %llu ms\n", pro->name.c_str(), std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count());
          start = finish;

          samplenumber ++;
//          if ( samplenumber > 250)
//            break;
          if (sample)
            gst_sample_unref (sample);
          }while (sample != NULL);

        time_t time_end = std::time(0);
        double seconds = difftime(time_end, time_start);
        std::cout << pro->name <<" "<< seconds << " seconds have passed" << std::endl;

        sleep(50000);
        pthread_exit(NULL);
      }

      static void
      read_video_props (GstCaps *caps)
      {
        gint width, height, num, denom;
        const GstStructure *str;
        const char *format;
        const char *type;
      
        g_return_if_fail (gst_caps_is_fixed (caps));
      
        str = gst_caps_get_structure (caps, 0);
        type = gst_structure_get_name (str);
        format = gst_structure_get_string (str, "stream-format");
        if (!gst_structure_get_int (str, "width", &width) ||
            !gst_structure_get_int (str, "height", &height) ||
            !gst_structure_get_fraction (str, "framerate", &num, &denom)) {
          g_print ("No width/height available\n");
          return;
         }
      //  g_print ("Capabilities:\n the size is %d x %d\n the framerate is %d/%d\n the format is %s\n the type is %s\n", width, height,num,denom,format,type);
        g_print("caps: %s\n", gst_caps_to_string(caps));
      }

     static void 
     on_pad_added (GstElement *element, 
     GstPad *pad, 
     gpointer data) 
     { 
        GstPad *sinkpad; 
        GstCaps *caps;
        GstElement_Duo *parser = (GstElement_Duo *) data;
      //  GstElement *parser = (GstElement *) data; 
        GstStructure *str;
        std::string type;
        /* We can now link this pad with the h264parse sink pad */ 
        caps =  gst_pad_get_current_caps (pad);
        str = gst_caps_get_structure (caps, 0);
        type = gst_structure_get_name (str);
      
        g_print("%s\n", gst_caps_to_string(caps));
      
        if(type.find("video") != std::string::npos)
        {
          sinkpad = gst_element_get_static_pad (parser->video, "sink"); 
          gst_pad_link (pad, sinkpad); 
          gst_object_unref (sinkpad); 
          g_print ("linking demuxer/h264parse\n"); 
        }
        else
        {
          sinkpad = gst_element_get_static_pad (parser->audio, "sink"); 
          gst_pad_link (pad, sinkpad); 
          gst_object_unref (sinkpad); 
          g_print ("linking demuxer/accparse\n"); 
        }
     }
  };
} // namespace ndn
#endif
