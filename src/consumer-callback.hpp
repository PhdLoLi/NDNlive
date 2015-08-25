/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2015 Regents of the University of California.
 *
 * @author Lijing Wang wanglj11@mails.tsinghua.edu.cn
 */


#ifndef CONSUMER_CALLBACK_HPP
#define CONSUMER_CALLBACK_HPP
#include <ndn-cxx/contexts/consumer-context.hpp>
#include <fstream>
#include <iostream>
#include "video-player.hpp"
#include "video-frame.hpp"
//#define VIDEO_SIZE 32 
//#define AUDIO_SIZE 31 

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentions

  class ConsumerCallback
  {
      
  struct DataNode_G
  {
    uint64_t length;
    uint8_t *data;
    DataNode_G(uint64_t len, uint8_t *d) : length(len), data(d)
    {
    }
  };

  public:
    ConsumerCallback();

    void
    wait_rate()
    {
      boost::unique_lock<boost::mutex> lock(player.rate_mut);
      while(!player.rate_ready)
      {
//        std::cout << "waiting for rate" << std::endl;
        player.rate_con.wait(lock);
//        std::cout << "waiting for rate over" << std::endl;
      }
      frame_rate_v = player.get_video_rate() / 1;
      frame_rate_a = player.get_audio_rate() / 1;
//      buffers_v.resize(frame_rate_v, DataNode_G(0, NULL));
//      buffers_a.resize(frame_rate_a, DataNode_G(0, NULL));
    }

    void
    start_timer_audio() {
      start_a = std::chrono::high_resolution_clock::now();
    }

    void
    start_timer_video() {
      start_v = std::chrono::high_resolution_clock::now();
    }
    
    void
    processPayload(Consumer& con, const uint8_t* buffer, size_t bufferSize);
    
    void
    processStreaminfo(Consumer& con, const uint8_t* buffer, size_t bufferSize);

    void
    processPayloadAudio(Consumer& con, const uint8_t* buffer, size_t bufferSize);
    
    void
    processStreaminfoAudio(Consumer& con, const uint8_t* buffer, size_t bufferSize);
    
    void
    initFrameVideo();

    void
    initFrameAudio();

    void
    pushQueueVideo();

    void
    pushQueueAudio();

    void
    processDataVideo(Consumer& con, const Data& data);
    
    void
    processDataAudio(Consumer& con, const Data& data);
    
    void
    processData(Consumer& con, const Data& data);
    
    void
    streaminfoEnter(Consumer& con, const Data& data);

    void
    streaminfoEnterAudio(Consumer& con, const Data& data);

    bool
    verifyData(Consumer& con, Data& data);
    
    void
    processConstData(Consumer& con, const Data& data);
    
    void
    processLeavingInterest(Consumer& con, Interest& interest);
    
    void
    processLeavingStreaminfo(Consumer& con, Interest& interest);

    void
    onRetx(Consumer& con, Interest& interest);

    void
    onExpr(Consumer& con, Interest& interest);

    void
    onRetx_info(Consumer& con, Interest& interest);

    void
    onExpr_info(Consumer& con, Interest& interest);

    void
    processFile(Consumer& con, const uint8_t* buffer, size_t bufferSize)
    {
      std::ofstream filestr;
    	// binary open
      
    	filestr.open ("/Users/lijing/next-ndnvideo/build/haha.mp3",  std::ios::out | std::ios::app | std::ios::binary);
      filestr.write((char *)buffer, bufferSize);
      filestr.close();
 
    }
    
    VideoFrame *frame_v;
    VideoFrame *frame_a;
    VideoPlayer player;
    gsize payload_v;
    gsize payload_a;
    gsize interest_s;
    gsize interest_r;
    gsize interest_r_v;
    gsize interest_r_a;
    gsize interest_retx;
    gsize interest_expr;
    gsize data_v;
    gsize data_a;
    gsize start_frame_v;
    gsize start_frame_a;
    int frame_cnt_v;
    int frame_cnt_a;
    int frame_crr_v;
    int frame_crr_a;
    std::chrono::high_resolution_clock::time_point start_v;
    std::chrono::high_resolution_clock::time_point start_a;
    int frame_rate_v;
    int frame_rate_a;
    bool ready_v;
    bool ready_a;
    int playback_v;
    int playback_a;
    int framenumber_v;
    int framenumber_a;

    boost::mutex frame_cnt_v_m;
    boost::mutex frame_cnt_a_m;
    boost::mutex interest_s_m;
    boost::mutex interest_r_m;
    boost::mutex interest_r_v_m;
    boost::mutex interest_r_a_m;
    boost::mutex interest_retx_m;
    boost::mutex interest_expr_m;
    boost::condition_variable con_v;
    boost::mutex mut_v;
    boost::condition_variable con_a;
    boost::mutex mut_a;

    std::vector<DataNode_G> buffers_v;
    std::vector<DataNode_G> buffers_a;

    int m_v_size;
    int m_a_size;
  };

} // namespace ndn
#endif
