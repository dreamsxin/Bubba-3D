/*
 * This file is part of Bubba-3D.
 *
 * Bubba-3D is free software: you can redistribute it and/or modify
 * it under the terms of the GNU Lesser General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bubba-3D is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with Bubba-3D. If not, see http://www.gnu.org/licenses/.
 */
#pragma once

#include <Layout.h>

class PositioningLayout : public Layout {
public:
    virtual void addChild(Layout* child);

    virtual void addChild(Layout* child, Dimension x, Dimension y);

    /**
     * \warning Never allowed to return a wrapping dimension
     */
    virtual Dimension getWidth();
    /**
     * \warning Never allowed to return a wrapping dimension
     */
    virtual Dimension getHeight();

    virtual void getGLSquares(float layoutXPos,float layoutYPos, float layoutWidth,
                              float layoutHeight, std::map<std::string, IHudDrawable*> *map);

    PositioningLayout(Dimension width, Dimension height);

    void checkChildCompatibility(Layout* child);

    virtual Layout* findById(std::string id);

    virtual void invokeListenersInternal(int x, int y, ListenerType listenerType, bool mayBeHit);

protected:
    struct PositionedLayout {
        Layout* child;
        Dimension x;
        Dimension y;
        PositionedLayout(Layout* child, Dimension x, Dimension y) : child(child), x(x), y(y){
        }
    };

    Dimension width,height;
    std::vector<PositionedLayout*> children;
};
