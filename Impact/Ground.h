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


#ifndef __BODYGROUND_H_
#define __BODYGROUND_H_

#include "Body.h"
#include "Impact.h"

namespace Impact {

  class Ground : public Body
  {
  public:
    Ground(Game *game, float32 width);

    // Body implementation
    virtual void onUpdate(float) { /* ... */ }
    virtual void onDraw(sf::RenderTarget &, sf::RenderStates) const  { /* ... */ }
    virtual BodyType type(void) const { return Body::BodyType::Ground; }

    static const std::string Name;

    virtual void setPosition(int x, int y);
    virtual void setPosition(float32 x, float32 y);
    virtual void setPosition(const b2Vec2 &pos);
  };

}

#endif // __BODYGROUND_H_

