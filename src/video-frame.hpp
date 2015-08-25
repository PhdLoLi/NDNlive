/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/**
 * Copyright (C) 2014 Named Data Networking Project
 * See COPYING for copyright and distribution information.
 *
 * Author: Ilya Moiseenko <iliamo@ucla.edu>
 */

#ifndef VIDEO_FRAME_HPP
#define VIDEO_FRAME_HPP

#include <map>
#include <vector>
#include <ndn-cxx/data.hpp>
#include <ndn-cxx/common.hpp>

class VideoFrame
{
public:
  /**
   * Default constructor.
   */
  VideoFrame();
  
  /**
   * The destructor.
   */
  ~VideoFrame();
  
  void
  addSegment(ndn::Data& data);

  // user must allocate const uint8_t* data which is long enough to accomodate whole video frame
  // dataLength is a return value. Either the length of video frame data, 
  // or -1 if it has not been reassembled
  void 
  getData(std::vector<uint8_t>& dataVec, int& dataLength);

private:
  void
  reassemble();
  
  void
  copyContent(ndn::shared_ptr<ndn::Data> data);

private:
  uint64_t m_lastReassembledSegment;
  uint64_t m_lastSegment;
  std::map<uint64_t, ndn::shared_ptr<ndn::Data> > m_receivedSegments;
  std::vector<uint8_t> m_reassembledData;
};

#endif // VIDEO_FRAME_HPP

