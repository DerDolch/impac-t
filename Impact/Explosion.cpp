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


#include "stdafx.h"


namespace Impact {

  std::vector<sf::Shader*>::size_type Explosion::sCurrentShaderIndex = 0;
  std::vector<sf::Shader*> Explosion::sShaders;
  Explosion::ShaderPool Explosion::sShaderPool;

  Explosion::Explosion(const ExplosionDef &def)
    : Body(Body::BodyType::Particle, def.game)
    , mParticles(def.count)
    , mShader(nullptr)
  {
    mName = std::string("Explosion");
    setLifetime(def.maxLifetime);
    mTexture = def.texture;

    if (gLocalSettings().useShaders() && gLocalSettings().useShadersForExplosions()) {
      mShader = ShaderPool::getNext();
      mShader->setParameter("uTexture", sf::Shader::CurrentTexture);
      mShader->setParameter("uMaxAge", def.maxLifetime.asSeconds());
    }

    static std::uniform_int_distribution<sf::Int32> randomLifetime(def.minLifetime.asMilliseconds(), def.maxLifetime.asMilliseconds());
    static std::uniform_real_distribution<float32> randomSpeed(def.minSpeed, def.maxSpeed);
    static std::uniform_real_distribution<float32> randomOffset(-1.f, +1.f);

    b2World *world = mGame->world();
    const int N = mParticles.size();
    for (int i = 0; i < N; ++i) {
      SimpleParticle &p = mParticles[i];
      p.dead = false;
      p.lifeTime = sf::milliseconds(randomLifetime(gRNG()));
      p.sprite.setTexture(mTexture);
      mTexture.setRepeated(false);
      mTexture.setSmooth(true);
      p.sprite.setOrigin(.5f * mTexture.getSize().x, .5f * mTexture.getSize().y);

      b2BodyDef bd;
      bd.type = b2_dynamicBody;
      bd.position = def.pos + Game::InvScale * b2Vec2(randomOffset(gRNG()), randomOffset(gRNG()));
      bd.fixedRotation = true;
      bd.bullet = false;
      bd.allowSleep = true;
      bd.userData = this;
      bd.gravityScale = def.gravityScale;
      bd.linearDamping = def.linearDamping;
      bd.linearVelocity = randomSpeed(gRNG()) * b2Vec2(randomOffset(gRNG()), randomOffset(gRNG()));
      p.body = world->CreateBody(&bd);

      b2CircleShape circleShape;
      circleShape.m_radius = def.radius * Game::InvScale;

      b2FixtureDef fd;
      fd.density = def.density;
      fd.restitution = def.restitution;
      fd.friction = def.friction;
      fd.filter.categoryBits = Body::ParticleMask;
      fd.filter.maskBits = 0xffffU ^ Body::ParticleMask ^ Body::RacketMask;
      if (!def.ballCollisionEnabled)
        fd.filter.maskBits ^= Body::BallMask;
      fd.shape = &circleShape;
      p.body->CreateFixture(&fd);
    }
  }


  Explosion::~Explosion()
  {
    b2World *world = mGame->world();
    for (std::vector<SimpleParticle>::const_iterator p = mParticles.cbegin(); p != mParticles.cend(); ++p) {
      if (!p->dead)
        world->DestroyBody(p->body);
    }
  }


  void Explosion::onUpdate(float)
  {
    bool allDead = true;
    const int N = mParticles.size();
    for (int i = 0; i < N; ++i) {
      SimpleParticle &p = mParticles[i];
      if (age() > p.lifeTime && !p.dead) {
        p.dead = true;
        mGame->world()->DestroyBody(p.body);
      }
      else {
        p.sprite.setPosition(float(Game::Scale) * sf::Vector2f(p.body->GetPosition().x, p.body->GetPosition().y));
        if (mShader == nullptr) {
          const float alpha = Easing<float>::quadEaseIn(age().asSeconds(), 0U, 255U, p.lifeTime.asSeconds());
          p.sprite.setColor(sf::Color(255U, 255U, 255U, 255U - sf::Uint8(alpha)));
        }
      }
      allDead &= p.dead;
    }
    if (allDead || overAge())
      this->kill();
  }


  void Explosion::onDraw(sf::RenderTarget &target, sf::RenderStates states) const
  {
    if (mShader != nullptr) {
      mShader->setParameter("uAge", age().asSeconds());
      states.shader = mShader;
    }
    for (std::vector<SimpleParticle>::const_iterator p = mParticles.cbegin(); p != mParticles.cend(); ++p)
      if (!p->dead)
        target.draw(p->sprite, states);
  }

}
