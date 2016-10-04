/*******************************************************************************
 * All content © 2015 DigiPen (USA) Corporation, all rights reserved.
 * Reproduction or disclosure of this file or its contents without the prior
 * written consent of DigiPen Institute of Technology is prohibited.
 *
 * File: FrameRateController.h 
 * Author: Joel Shook
 * Class: GAM300/GAM302/GAM350/GAM352/GAM375/GAM400
 ******************************************************************************/
#pragma  once
namespace Core
{
  class FrameRateController;
  extern FrameRateController * FRAMERATECONTROLLER;

  class FrameRateController
  {
  public:
    FrameRateController();

    //float TotalTime()const;  // in seconds
    //float DeltaTime()const; // in seconds
    //
    //void Reset(); // Call before message loop.
    //void Start(); // Call when unpaused.
    //void Stop();  // Call when paused.
    //void Tick();  // Call every frame.
    void Reset();
    void StartFrame();
    void EndFrame();
    void SetFramesPerSecond(unsigned framesPerSecond);
    unsigned GetFramesPerSecond();
    float GetDT();
    float GetActualFramesPerSecond();

  private:
    double mTicksPerSecond;
    double mDT;
    unsigned mFramesPerSecond;
    __int64 mBaseTime;
    __int64 mPausedTime;
    __int64 mStopTime;
    __int64 mPrevTime;
    __int64 mCurrTime;
    __int64 mFrameStart;
    double mTotalTime;

    bool mStopped;
  };
}