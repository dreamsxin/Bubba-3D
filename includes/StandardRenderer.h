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

#include "IRenderComponent.h"
#include "SFML/Window.hpp"
#include <memory>

class Mesh;
class ShaderProgram;
class GameObject;
class Chunk;

/**
 * \brief Renders meshes with the default shader
 *
 * Class is responsible for rendering a Mesh/GameObject using
 * the default shader simple.vert and simple.frag. 
 *
 */
class StandardRenderer : public IRenderComponent {
public:
    StandardRenderer();
    StandardRenderer(std::shared_ptr<Mesh> mesh, std::shared_ptr<ShaderProgram> shader);

    void update(float dt);

    void render();
    void renderShadow(std::shared_ptr<ShaderProgram> &shaderProgram);
    void renderEmissive(std::shared_ptr<ShaderProgram> &shaderProgram);
private:
    std::shared_ptr<Mesh> mesh;
    sf::Clock clock;

    void setBones(std::shared_ptr<ShaderProgram> &shaderProgram) const;
};


