/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2015 Regents of the University of California.
 *
 * @author Lijing Wang wanglj11@mails.tsinghua.edu.cn
 */

#include "producer-callback.hpp"

// Enclosing code in ndn simplifies coding (can also use `using namespace ndn`)
namespace ndn {
// Additional nested namespace could be used to prevent/limit name contentiojs
  
  ProducerCallback::ProducerCallback()
  {
  }
  
  void
  ProducerCallback::setProducer(Producer* p)
  {
    m_producer = p;
  }
  
  void
  ProducerCallback::setSampleNumber(size_t* n)
  {
    m_curnum = n;
  }
  
  void
  ProducerCallback::setStreaminfoVideo(std::string video)
  {
    m_streaminfovideo = video;  
  }

  void 
  ProducerCallback::setStreaminfoAudio(std::string audio)
  {
    m_streaminfoaudio = audio;
  }

  void
  ProducerCallback::processConstData(Producer& pro, const Data& data){}
  
   /* When the request can't be satisfied from the content store */
  void
  ProducerCallback::processStreaminfoInterest(Producer& pro, const Interest& interest)
  {
    interest_nohit ++; 
    std::cout << "NO HIT STREAMINFO Interest! Name: " << interest.getName().toUri() << std::endl;
    std::string type = interest.getName().get(-3).toUri();
    std::string streaminfo;
    if(type == "video")
    {
      streaminfo = m_streaminfovideo;
    }else
    {
      streaminfo = m_streaminfoaudio;
    }
    std::cout << "current_number: " << std::dec << *m_curnum << std::endl;
    Name streaminfoSuffix(Name(interest.getName().get(-1).toUri()).append(std::to_string(*m_curnum)));
//    Name streaminfoSuffix("");
    m_producer->produce(streaminfoSuffix, (uint8_t *)streaminfo.c_str(), streaminfo.size()+1); 
//    std::cout << "produce " << type << " streaminfo AGAIN " << streaminfo << std::endl;
//    std::cout << "streaminfo size "<< streaminfo.size() + 1 << std::endl;
  }

  /* When the request can't be satisfied from the content store */
  void
  ProducerCallback::processInterest(Producer& pro, const Interest& interest)
  {
    //if (interet.getName().get(-2).toSegment() < m_crrnFrameNumer)
    interest_nohit ++; 
    int sampleNumber;
    std::string name;
    if (interest.getName().toUri().find("audio") == std::string::npos) {
      sampleNumber =  std::stoi(interest.getName().get(-2).toUri());
      name = "video";
    }
    else {
      sampleNumber =  std::stoi(interest.getName().get(-2).toUri());
      name = "audio";
    }
//    std::cout << "Current Number" << std::dec << *m_curnum << std::endl;
//      std::cout << "Cache Miss" << std::dec <<  "Request Number: " << sampleNumber << " current_number: " << *m_curnum << std::endl;
    if (sampleNumber > *m_curnum)
    {
      printf("NACK!!!! Type: %s Request Number: %d Current Number: %zu\n", name.c_str(), sampleNumber, *m_curnum);
//      ApplicationNack appNack(interest, ApplicationNack::PRODUCER_DELAY);
//      appNack.setDelay(5000); // in ms
//      m_producer->nack(appNack);
    }
//    std::cout << "NO HIT Interest!" << interest.getName().toUri() << std::endl;
//    std::cout << "HAHA " <<std::dec<< sampleNumber << std::endl;
  }
  
  void
  ProducerCallback::processIncomingInterest(Producer& pro, const Interest& interest)
  {
    interest_incoming ++;
//    printf("processIncomingInterest %s\n", interest.getName().toUri().c_str());
//    std::string type = interest.getName().get(-2).toUri();
//    std::string streaminfo;
//    std::cout << "processIncomingInterest type " << type << " streaminfo" << streaminfo << std::endl;
//    std::cout << "streaminfo size "<< streaminfo.size() + 1 << std::endl;

  }
  void
  ProducerCallback::processOutgoingData(Producer& pro, const Data& data)
  {
    interest_outgoing ++;
//    printf("OutgoingData %s\n", data.getName().toUri().c_str());
//    std::cout << "OutgoingData " << data.getName() << std::endl;
  //    std::cout << data.getFinalBlockId() << std::endl;
  }

  void
  ProducerCallback::processOutgoingStreaminfo(Producer& pro, const Data& data)
  {
  //  printf("OutgoingStreaminfo Name: %s\n", data.getName().toUri().c_str());
  //  interest_outgoing ++;
  //    std::cout << data.getFinalBlockId() << std::endl;
  }
 
  bool
  ProducerCallback::verifyInterest(Interest& interest){return true;}
  
  void
  ProducerCallback::processConstInterest(Producer& pro, const Interest& interest){}

} // namespace ndn
