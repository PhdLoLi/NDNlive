/**
 * Copyright (C) 2014 Named Data Networking Project
 * See COPYING for copyright and distribution information.
 *
 * Author: Ilya Moiseenko <iliamo@ucla.edu>
 */

#include "video-frame.hpp"

VideoFrame::VideoFrame()
  : m_lastReassembledSegment(0)
  , m_lastSegment(0)
{
}

VideoFrame::~VideoFrame()
{

}

void
VideoFrame::getData(std::vector<uint8_t> &dataVec, int& dataLength)
{
//  std::cout << "getData " << m_lastReassembledSegment << " last " << m_lastSegment << std::endl;
  if (m_lastReassembledSegment == m_lastSegment + 1)
  {
    dataVec = m_reassembledData;
    dataLength = m_reassembledData.size();
//    const uint8_t* data = new uint8_t[dataLength];
//    const uint8_t* data =  m_reassembledData.data();
//    data = m_reassembledData.data();
//    delete m_reassembledData.data();
//    return data;
  }
  else
  {
    dataLength = -1;
//    return NULL;
  }
}

void
VideoFrame::addSegment(ndn::Data& data)
{
//  std::cout << "addSegment " << data.getName() << std::endl;
  uint64_t segment = data.getName().get(-1).toSegment();
  m_receivedSegments[segment] = data.shared_from_this();
  
  //infer final segment number
  if (!data.getFinalBlockId().empty())
  {
    uint64_t finalBlock = data.getFinalBlockId().toSegment();
    
    if (finalBlock > m_lastSegment)
    {
//      std::cout << "finalBlock " << finalBlock << std::endl;
      m_lastSegment = finalBlock;
    }
  }
  
  reassemble();
}

void
VideoFrame::reassemble()
{  
  std::map<uint64_t, ndn::shared_ptr<ndn::Data> >::iterator head = m_receivedSegments.find(m_lastReassembledSegment);
  while (head != m_receivedSegments.end())
  {
    copyContent(head->second);
    
    m_receivedSegments.erase(head);
    m_lastReassembledSegment++;
    head = m_receivedSegments.find(m_lastReassembledSegment);
  }
}

void
VideoFrame::copyContent(ndn::shared_ptr<ndn::Data> data)
{
//  std::cout << "copy " << data->getName() << std::endl;
  const ndn::Block content = data->getContent();
  m_reassembledData.insert(m_reassembledData.end(), &content.value()[0], &content.value()[content.value_size()]);
}
