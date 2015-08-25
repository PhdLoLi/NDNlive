/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2015 Regents of the University of California.
 *
 * @author Lijing Wang wanglj11@mails.tsinghua.edu.cn
 */

#ifndef PRODUCER_CALLBACK_HPP
#define PRODUCER_CALLBACK_HPP
#include <ndn-cxx/contexts/producer-context.hpp>
#include <ndn-cxx/contexts/application-nack.hpp>
#include <chrono>

namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentions
  class ProducerCallback
  {
  public:
    ProducerCallback();

    void
    start_timer_audio() {
      start_a = std::chrono::high_resolution_clock::now();
    }

    void
    start_timer_video() {
      start_v = std::chrono::high_resolution_clock::now();
    }
    
    void
    setProducer(Producer* p);
    
    void
    setSampleNumber(size_t* n);

    void
    setStreaminfoVideo(std::string video);

    void 
    setStreaminfoAudio(std::string audio);

    void
    processConstData(Producer& pro, const Data& data);
    
    void
    processStreaminfoInterest(Producer& pro, const Interest& interest);

    void
    processInterest(Producer& pro, const Interest& interest);
    
    void
    processOutgoingData(Producer& pro, const Data& data);
   
    void
    processOutgoingStreaminfo(Producer& pro, const Data& data);


    void
    processIncomingInterest(Producer& pro, const Interest& interest);
    
    bool
    verifyInterest(Interest& interest);
    
    void
    processConstInterest(Producer& pro, const Interest& interest);
  
    unsigned long interest_incoming = 0;
    unsigned long interest_outgoing = 0;
    unsigned long interest_nohit = 0;
    size_t* m_curnum;
    std::chrono::high_resolution_clock::time_point start_v;
    std::chrono::high_resolution_clock::time_point start_a;

  private:
    Producer* m_producer;
    std::string m_streaminfovideo;
    std::string m_streaminfoaudio;
  };
  
} // namespace ndn
#endif
