/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2015 Regents of the University of California.
 *
 * @author Lijing Wang wanglj11@mails.tsinghua.edu.cn
 */

#ifndef CONSUMER_HPP
#define CONSUMER_HPP
#include "consumer-callback.hpp"
#include <pthread.h>
#include <ndn-cxx/security/validator.hpp>
#include "threadpool.hpp"

using namespace boost::threadpool;

namespace ndn{

#define IDENTITY_NAME "/Lijing/Wang"
  
  class Verificator
  {
  public:
    Verificator()
    {
      Name identity(IDENTITY_NAME);
      Name keyName = m_keyChain.getDefaultKeyNameForIdentity(identity);
      m_publicKey = m_keyChain.getPublicKey(keyName); 
    };
    
    bool
    onPacket(Data& data)
    {
    //  return true;
      if (Validator::verifySignature(data, *m_publicKey))
      {
        std::cout << "Verified Name:" << data.getName() <<  std::endl;
        return true;
      }
      else
      {
        std::cout << "NOT Verified" << data.getName() << std::endl;
        return false;
      }
    }
    
  private:
    KeyChain m_keyChain;
    shared_ptr<PublicKey> m_publicKey;
 };

  struct Consumer_Need
  {
    ConsumerCallback *cb;
  };

  class FrameConsumer
  {
   public:
    shared_ptr<Face> m_face;
    boost::asio::io_service& m_ioService;
    Scheduler m_scheduler;
    std::vector<Consumer *> m_consumers;
    boost::mutex m_mut;
    boost::mutex m_size_mut;
    uint64_t m_interval;
    uint64_t m_size;
    uint64_t m_accum;
    uint64_t m_counter;
    uint64_t m_avg;
    
    int framenumber;
    EventId eventid;
    time::nanoseconds interval;
    std::chrono::high_resolution_clock::time_point start;


//    int v_framenumber;

    FrameConsumer();

    void 
    consume_audio_frame(Consumer* sampleConsumer, Name sampleSuffix);
    void 
    consume_video_frame(Consumer* sampleConsumer, Name sampleSuffix);
    void
    audioConsumeFrames();
    void
    videoConsumeFrames();
    void
    setup_audio(std::string, std::string);
    void
    setup_video(std::string, std::string);
    void
    start_timer() {
      start = std::chrono::high_resolution_clock::now();
    }
  };
}
#endif
