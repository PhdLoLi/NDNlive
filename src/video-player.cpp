/* -*- Mode:C++; c-file-style:"gnu"; indent-tabs-mode:nil; -*- */
/*
 * Copyright (c) 2014-2015 Regents of the University of California.
 *
 * @author Lijing Wang wanglj11@mails.tsinghua.edu.cn
 */

#include "video-player.hpp"
#include <thread>

namespace ndn {

  VideoPlayer::VideoPlayer() : rate_ready(false)//, pipeline_pool(1)
  {
  }

  VideoPlayer::~VideoPlayer() 
  {
//    pipe_thread->join();
  }

  void
  VideoPlayer::get_streaminfo(std::string streaminfo)
  {
    VideoAudio *va =  &s_va;
    App *app = &(va->v_app);
    app->capstr = streaminfo; 
//    pthread_mutex_init(&(app->count_mutex), NULL);
//    pthread_cond_init (&(app->count_cond), NULL);
//    std::cout << "Video streaminfo " << streaminfo << std::endl;
//    pthread_t thread; 
//    int rc;
//    rc = pthread_create(&thread, NULL, h264_video_thread ,(void *)app);
//    std::cout << "h264_video_init OK! " << std::endl;
  }

  void
  VideoPlayer::get_streaminfo_audio(std::string streaminfo)
  {
    VideoAudio *va =  &s_va;
    App *app = &(va->a_app);
    app->capstr = streaminfo; 
//    pthread_mutex_init(&(app->count_mutex), NULL);
//    pthread_cond_init (&(app->count_cond), NULL);
//    std::cout << "Audio streaminfo " << streaminfo << std::endl;
//    pthread_t thread; 
//    int rc;
//    rc = pthread_create(&thread, NULL, h264_audio_thread ,(void *)app);
//    std::cout << "h264_audio_thread OK! " << std::endl;
    h264_appsrc_init();
  }

  void
  VideoPlayer::h264_appsrc_data(const uint8_t* buffer, size_t bufferSize )
  {
    VideoAudio *va = &s_va;
    App *app = &(va->v_app);
    /* get some vitals, this will be used to read data from the mmapped file and
     * feed it to appsrc. */
//    DataNode dataNode;
    uint8_t* bufferTmp = new uint8_t[bufferSize];
    memcpy(bufferTmp, buffer, bufferSize);
//    dataNode.length = bufferSize;
//    dataNode.data =(guint8 *)bufferTmp;

    app->queue_m.lock();
    (app->dataQue).push_back(DataNode(bufferSize, (guint8 *)bufferTmp));
    app->queue_m.unlock();

//    std::cout << "HERE@" << std::endl;
//    std::cout << "videoQueueSize " << (app->dataQue).size() <<std::endl;
//    t_video ++;
//    pthread_mutex_lock(&(app->count_mutex));
//    if((app->dataQue).size() > 0)
//       pthread_cond_signal(&(app->count_cond));
//    pthread_mutex_unlock(&(app->count_mutex));

//    std::cout << "CP Video Done! " << bufferSize <<std::endl;
  }

  void
  VideoPlayer::h264_appsrc_data_audio(const uint8_t* buffer, size_t bufferSize )
  {
    VideoAudio *va = &s_va;
    App *app = &(va->a_app);
    uint8_t* bufferTmp = new uint8_t[bufferSize];
    memcpy (bufferTmp, buffer, bufferSize);

    app->queue_m.lock();
    (app->dataQue).push_back(DataNode(bufferSize, (guint8 *)bufferTmp));
    app->queue_m.unlock();

//    std::cout << "audioQueueSize " << (app->dataQue).size() <<std::endl;
//    t_audio ++;
//    pthread_mutex_lock(&(app->count_mutex));
//    if((app->dataQue).size() > 0)
//       pthread_cond_signal(&(app->count_cond));
//    pthread_mutex_unlock(&(app->count_mutex));

//    std::cout << "CP Audio Done! " << bufferSize <<std::endl;
  }

/*
 * First INIT the h264_appsrc using thread
 * waiting in a loop
 */
  void
  VideoPlayer::h264_appsrc_init()
  {
    VideoAudio *va = &s_va;
//    pthread_t thread; 
//    int rc;
//    std::thread meme (std::bind(&VideoPlayer::h264_capture_thread, this, va));
    pipe_thread = new boost::thread(boost::bind(&VideoPlayer::h264_capture_thread ,this, va));
//    rc = pthread_create(&thread, NULL, h264_appsrc_thread ,(void *)va);
    std::cout << "h264_appsrc_init OK! " << std::endl;
  }
/* Call Consume Here From the start*/
  void
  VideoPlayer::consume_whole(Consumer *videoConsumer, Consumer *audioConsumer)
  {
    VideoAudio *va = &s_va;
    App *video = &(va->v_app);
    App *audio = &(va->a_app);
    std::cout << "Inside the consume_whole! " <<std::endl;
    int audio_time = audio->rate;
    int video_time = video->rate;
    for(int seconds = 0; seconds < 60*10 ; seconds++)
    {
      for(int j=0; j< audio_time; j++)
      {
        std::cout << "Consume Audio" << std::endl;
        Name audioSuffix(std::to_string(seconds*audio_time + j));
        audioConsumer->consume(audioSuffix);
//        usleep(100);
      }
      for(int i=0; i< video_time; i++)
      {
        std::cout << "Consume Video" << std::endl;
        Name videoSuffix(std::to_string(seconds*video_time + i));
        videoConsumer->consume(videoSuffix);
//        usleep(8000);
      }

    }
  }

  int VideoPlayer::get_video_rate() {
    VideoAudio *va = &s_va;
    App *video = &(va->v_app);
    return video->rate;
  }

  int VideoPlayer::get_audio_rate() {
    VideoAudio *va = &s_va;
    App *audio = &(va->a_app);
    return audio->rate;
  }
} // namespace ndn
