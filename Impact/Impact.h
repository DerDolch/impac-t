/*

    Copyright (c) 2015 Oliver Lau <ola@ct.de>

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.

    */

#ifndef __GAME_H_
#define __GAME_H_

#define IMPACT_VERSION "1.0.0-BETA14"

#include <SFML/Audio.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <SFML/Audio.hpp>
#include <SFML/OpenGL.hpp>

#include "BodyBall.h"
#include "BodyRacket.h"
#include "BodyGround.h"

namespace Impact {

  class Game;

  struct ContactPoint {
    b2Fixture *fixtureA;
    b2Fixture *fixtureB;
    b2Vec2 normal;
    b2Vec2 position;
    b2PointState state;
    float32 normalImpulse;
    float32 tangentImpulse;
    float32 separation;
  };


  struct OverlayDef {
    OverlayDef(void)
      : duration(sf::milliseconds(1000))
      , minScale(.2f)
      , maxScale(2.5f)
    { /* ... */ }
    OverlayDef(const OverlayDef &other)
      : duration(other.duration)
      , minScale(other.minScale)
      , maxScale(other.maxScale)
      , line1(other.line1)
      , line2(other.line2)
    { /* ... */
    }
    sf::Time duration;
    float minScale;
    float maxScale;
    std::string line1;
    std::string line2;
  };


  class Game : public b2ContactListener {

    typedef enum _Playmode {
      Campaign,
      SingleLevel,
      LastPlaymode
    } Playmode;

    typedef enum _Actions {
      NoAction,
      PauseAction,
      BackAction,
      NewBall,
      Restart,
      ContinueAction,
      LastAction
    } Action;

    typedef enum _State {
      Initialization,
      WelcomeScreen,
      CreditsScreen,
      OptionsScreen,
      AchievementsScreen,
      Playing,
      LevelCompleted,
      Pausing,
      PlayerWon,
      GameOver,
      LastState
    } State;


  public:
    static const int Scale = 16;
    static const float32 InvScale;
    static const unsigned int DefaultPlaygroundWidth = 640;
    static const unsigned int DefaultPlaygroundHeight = 400;
    static const unsigned int DefaultStatsWidth = DefaultPlaygroundWidth;
    static const unsigned int DefaultStatsHeight = 80;
    static const unsigned int DefaultWindowWidth = DefaultPlaygroundWidth;
    static const unsigned int DefaultWindowHeight = DefaultPlaygroundHeight + DefaultStatsHeight;
    static const unsigned int ColorDepth = 32;
    static const int DefaultLives;
    static const int DefaultPenalty;
    static const int NewLiveAfterSoManyPointsDefault;
    static const int NewLiveAfterSoManyPoints[];
    static const int32 MaxContactPoints = 512;
    static const sf::Time DefaultFadeEffectDuration;
    static const sf::Time DefaultAberrationEffectDuration;
    static const sf::Time DefaultEarthquakeDuration;
    static const sf::Time DefaultOverlayDuration;
    static const int DefaultKillingsPerKillingSpree = 5;
    static const int DefaultKillingSpreeBonus = 1000;
    static const sf::Time DefaultKillingSpreeInterval;

    Game(void);
    ~Game();
    void enterLoop(void);
    void addBody(Body *body);

    inline b2World *world(void)
    {
      return mWorld;
    }

    inline const Level *level(void) const
    {
      return &mLevel;
    }

    inline const Ground *ground(void) const
    {
      return mGround;
    }

  public: // slots
    void onBodyKilled(Body *body);

  private:
    int mGLVersionMajor;
    int mGLVersionMinor;
    const GLubyte *mGLShadingLanguageVersion;

    // SFML
    sf::RenderWindow mWindow;
    sf::View mDefaultView;
    sf::View mPlaygroundView;
    sf::View mStatsView;
    sf::RenderTexture mRenderTexture0;
    sf::RenderTexture mRenderTexture1;
    sf::Shader mMixShader;
    int mFadeEffectsActive;
    bool mFadeEffectsDarken;
    sf::Time mFadeEffectDuration;
    sf::Shader mHBlurShader;
    sf::Shader mVBlurShader;
    bool mBlurPlayground;
    sf::Font mFixedFont;
    sf::Font mTitleFont;
    sf::Texture mBackgroundTexture;
    sf::Sprite mBackgroundSprite;
    sf::Shader mTitleShader;
    sf::Texture mTitleTexture;
    sf::Sprite mTitleSprite;
    sf::Texture mLogoTexture;
    sf::Sprite mLogoSprite;
    sf::Texture mOverlayTexture;
    sf::Sprite mOverlaySprite;
    sf::Shader mOverlayShader;
    sf::Time mOverlayDuration;
    sf::Clock mOverlayClock;
    std::vector<OverlayDef> mOverlayQueue;
    sf::Texture mParticleTexture;
    sf::Texture mSoftParticleTexture;
    std::string mFadeShaderCode;
    sf::Shader mEarthquakeShader;
    float32 mEarthquakeIntensity;
    sf::Clock mEarthquakeClock;
    sf::Time mEarthquakeDuration;
    sf::Shader mAberrationShader;
    sf::Clock mAberrationClock;
    sf::Time mAberrationDuration;
    float32 mAberrationIntensity;
    sf::Clock mClock;
    sf::Clock mWallClock;
    sf::Clock mScoreClock;
    sf::Clock mBlurClock;
    sf::Clock mFadeEffectTimer;
    sf::Clock mScaleGravityClock;
    sf::Time mScaleGravityDuration;
    bool mScaleGravityEnabled;
    sf::Clock mScaleBallDensityClock;
    sf::Time mScaleBallDensityDuration;
    bool mScaleBallDensityEnabled;
    sf::Text mLevelCompletedMsg;
    sf::Text mGameOverMsg;
    sf::Text mPlayerWonMsg;
    sf::Text mScoreMsg;
    sf::Text mYourScoreMsg;
    sf::Text mTotalScoreMsg;
    sf::Text mTotalScorePointsMsg;
    sf::Text mStatMsg;
    sf::Text mStartMsg;
    sf::Text mProgramInfoMsg;
    sf::Text mLevelMsg;
    sf::SoundBuffer mStartupBuffer;
    sf::Sound mStartupSound;
    sf::SoundBuffer mNewBallBuffer;
    sf::Sound mNewBallSound;
    sf::SoundBuffer mBallOutBuffer;
    sf::Sound mBallOutSound;
    sf::SoundBuffer mBlockHitBuffer;
    sf::Sound mBlockHitSound;
    sf::SoundBuffer mPenaltyBuffer;
    sf::Sound mPenaltySound;
    sf::SoundBuffer mRacketHitBuffer;
    sf::Sound mRacketHitSound;
    sf::SoundBuffer mRacketHitBlockBuffer;
    sf::Sound mRacketHitBlockSound;
    sf::SoundBuffer mExplosionBuffer;
    sf::Sound mExplosionSound;
    sf::SoundBuffer mNewLifeBuffer;
    sf::Sound mNewLifeSound;
    sf::SoundBuffer mLevelCompleteBuffer;
    sf::Sound mLevelCompleteSound;
    sf::SoundBuffer mKillingSpreeSoundBuffer;
    sf::Sound mKillingSpreeSound;

    std::vector<sf::Music*> mMusic;
    std::vector<int> mFPSArray;
    std::vector<int>::size_type mFPSIndex;
    int mFPS;

    // Box2D
    static const int32 VelocityIterations = 100;
    static const int32 PositionIterations = 50;
    b2World *mWorld;
    Ground *mGround;
    ContactPoint mPoints[MaxContactPoints];
    int32 mContactPointCount;

    // b2ContactListener interface
    virtual void PreSolve(b2Contact *contact, const b2Manifold *oldManifold)
    {
      B2_NOT_USED(contact);
      B2_NOT_USED(oldManifold);
    }
    virtual void BeginContact(b2Contact *contact)
    {
      B2_NOT_USED(contact);
    }
    virtual void EndContact(b2Contact *contact)
    {
      B2_NOT_USED(contact);
    }
    virtual void PostSolve(b2Contact *contact, const b2ContactImpulse *impulse);

    // game logic
    std::vector<sf::Keyboard::Key> mKeyMapping;
    State mState;
    Playmode mPlaymode;
    int mScore;
    int mTotalScore;
    int mLives;
    bool mPaused;
    BodyList mBodies;
    int mBlockCount;
    int mWelcomeLevel;
    int mExtraLifeIndex;
    bool mBallHasBeenLost;
    Ball *mBall;
    Racket *mRacket;
    Level mLevel;
    LevelTimer mLevelTimer;
    std::vector<sf::Time> mLastKillings;
    int mLastKillingsIndex;

    void showScore(int score, const b2Vec2 &atPos, int factor = 1);
    void addToScore(int);
    void newBall(const b2Vec2 &pos = b2Vec2_zero);
    void extraBall(void);
    void setState(State state);
    void clearWorld(void);
    void clearWindow(void);
    void drawWorld(const sf::View &view);
    void drawStartMessage(void);
    void drawPlayground(const sf::Time &elapsed);
    void stopAllMusic(void);
    void restart(void);
    void resize(void);
    void pause(void);
    void resume(void);
    void buildLevel(void);
    void handlePlayerInteraction(const sf::Time &elapsed);
    void update(const sf::Time &elapsed);
    void evaluateCollisions(void);
    void handleEvents(void);
    void startOverlay(const OverlayDef &);
    void startEarthquake(float32 intensity, const sf::Time &duration);
    void startFadeEffect(bool darken = false, const sf::Time &duration = DefaultFadeEffectDuration);
    void startAberrationEffect(float32 gravityScale, const sf::Time &duration = DefaultAberrationEffectDuration);
    void setKillingsPerKillingSpree(int);
    void executeAberration(sf::RenderTexture &out, sf::RenderTexture &in);
    void executeCopy(sf::RenderTexture &out, sf::RenderTexture &in);
    void executeBlur(sf::RenderTexture &out, sf::RenderTexture &in);
    void executeEarthquake(sf::RenderTexture &out, sf::RenderTexture &in);
    void resetKillingSpree(void);

    void gotoWelcomeScreen(void);
    void onWelcomeScreen(void);

    void gotoNextLevel(void);
    void onPlaying(void);

    void gotoLevelCompleted(void);
    void onLevelCompleted(void);

    void gotoGameOver(void);
    void onGameOver(void);

    void gotoPlayerWon(void);
    void onPlayerWon(void);

    void gotoAchievementsScreen(void);
    void onAchievementsScreen(void);

    void gotoCreditsScreen(void);
    void onCreditsScreen(void);
    
    void gotoOptionsScreen(void);
    void onOptionsScreen(void);

    void onPausing(void);
  };

}

#endif // __GAME_H_
