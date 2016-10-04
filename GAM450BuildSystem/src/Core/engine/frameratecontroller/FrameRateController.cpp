/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: FrameRateController.cpp 
 * Author: Joel Shook
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#include "pch/precompiled.h"
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace Core
{
  FrameRateController * FRAMERATECONTROLLER = nullptr;

  FrameRateController::FrameRateController()
    : mTicksPerSecond(0), mDT(0.01667), mFramesPerSecond(60), mBaseTime(0),
    mPausedTime(0), mPrevTime(0), mCurrTime(0), mStopped(false)
  {
    __int64 countsPerSec;
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
    mTicksPerSecond = (double)countsPerSec;
    FRAMERATECONTROLLER = this;
  }

  void FrameRateController::Reset()
  {
    mTicksPerSecond = 0;
    mDT = 0.01667;
    mFramesPerSecond = 60;
    mBaseTime = 0;

    mPausedTime = 0;
    mPrevTime = 0;
    mCurrTime = 0;
    mStopped = false;


    __int64 countsPerSec;
    QueryPerformanceFrequency((LARGE_INTEGER*)&countsPerSec);
    mTicksPerSecond = (double)countsPerSec;
  }

  void FrameRateController::StartFrame()
  {
    QueryPerformanceCounter((LARGE_INTEGER*)&mFrameStart);
  }

  void FrameRateController::EndFrame()
  {
    QueryPerformanceCounter((LARGE_INTEGER*)&mCurrTime);
    mDT = (mCurrTime - mFrameStart) / mTicksPerSecond;
    while (mDT < 1.0 / mFramesPerSecond)
    {
      QueryPerformanceCounter((LARGE_INTEGER*)&mCurrTime);
      mDT = (mCurrTime - mFrameStart) / mTicksPerSecond;
    }

  }

  void FrameRateController::SetFramesPerSecond(unsigned framesPerSecond) { mFramesPerSecond = framesPerSecond; }
  unsigned FrameRateController::GetFramesPerSecond(){ return mFramesPerSecond; }
  float FrameRateController::GetDT()
  { 
    return (float)mDT; 
  }
  float FrameRateController::GetActualFramesPerSecond(){ return float(1.0 / mDT); }
}

/*
// Returns the total time elapsed since Reset() was called, NOT counting any
// time when the clock is stopped.
float GameTimer::TotalTime()const
{
  // If we are stopped, do not count the time that has passed since we stopped.
  // Moreover, if we previously already had a pause, the distance 
  // mStopTime - mBaseTime includes paused time, which we do not want to count.
  // To correct this, we can subtract the paused time from mStopTime:  
  //
  //                     |<--paused time-->|
  // ----*---------------*-----------------*------------*------------*------> time
  //  mBaseTime       mStopTime        startTime     mStopTime    mCurrTime

  if( mStopped )
  {
    return (float)(((mStopTime - mPausedTime)-mBaseTime)/mTicksPerSecond);
  }

  // The distance mCurrTime - mBaseTime includes paused time,
  // which we do not want to count.  To correct this, we can subtract 
  // the paused time from mCurrTime:  
  //
  //  (mCurrTime - mPausedTime) - mBaseTime 
  //
  //                     |<--paused time-->|
  // ----*---------------*-----------------*------------*------> time
  //  mBaseTime       mStopTime        startTime     mCurrTime
  
  else
  {
    return (float)(((mCurrTime-mPausedTime)-mBaseTime)*mSecondsPerCount);
  }
}

float GameTimer::DeltaTime()const
{
  return (float)mDeltaTime;
}

void GameTimer::Reset()
{
  __int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

  mBaseTime = currTime;
  mPrevTime = currTime;
  mStopTime = 0;
  mStopped  = false;
}

void GameTimer::Start()
{
  __int64 startTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&startTime);


  // Accumulate the time elapsed between stop and start pairs.
  //
  //                     |<-------d------->|
  // ----*---------------*-----------------*------------> time
  //  mBaseTime       mStopTime        startTime     

  if( mStopped )
  {
    mPausedTime += (startTime - mStopTime);  

    mPrevTime = startTime;
    mStopTime = 0;
    mStopped  = false;
  }
}

void GameTimer::Stop()
{
  if( !mStopped )
  {
    __int64 currTime;
    QueryPerformanceCounter((LARGE_INTEGER*)&currTime);

    mStopTime = currTime;
    mStopped  = true;
  }
}

void GameTimer::Tick()
{
  if( mStopped )
  {
    mDeltaTime = 0;
    return;
  }

  __int64 currTime;
  QueryPerformanceCounter((LARGE_INTEGER*)&currTime);
  mCurrTime = currTime;

  // Time difference between this frame and the previous.
  mDeltaTime = (mCurrTime - mPrevTime)*mSecondsPerCount;

  // Prepare for next frame.
  mPrevTime = mCurrTime;

  // Force nonnegative.  The DXSDK's CDXUTTimer mentions that if the 
  // processor goes into a power save mode or we get shuffled to another
  // processor, then mDeltaTime can be negative.
  if(mDeltaTime < 0)
  {
    mDeltaTime = 0;
  }
}

*/