/*
 * Copyright (C) 2006-2014 Christopho, Solarus - http://www.solarus-games.org
 * 
 * Solarus is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * Solarus is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
#ifndef SOLARUS_STREAM_H
#define SOLARUS_STREAM_H

#include "Common.h"
#include "entities/Detector.h"

namespace solarus {

/**
 * \brief A special terrain where the hero is moved towards a specific
 * direction.
 *
 * The hero may or may not resist to the movement of the stream, depending
 * on its properties.
 */
class Stream: public Detector {

  public:

    Stream(
        const std::string& name,
        Layer layer,
        int x,
        int y,
        int direction,
        const std::string& sprite_name
    );
    ~Stream();

    EntityType get_type() const;

    int get_speed() const;
    void set_speed(int speed);
    bool get_allow_movement() const;
    void set_allow_movement(bool allow_movement);
    bool get_allow_attack() const;
    void set_allow_attack(bool allow_attack);
    bool get_allow_item() const;
    void set_allow_item(bool allow_item);
    void notify_direction_changed();

    bool is_obstacle_for(MapEntity& other);
    void notify_collision(MapEntity& entity_overlapping, CollisionMode collision_mode);
    void activate(MapEntity& target);

  private:

    int speed;                    /**< Speed to apply in pixels per second. */
    bool allow_movement;          /**< Whether the player can move the hero in this stream. */
    bool allow_attack;            /**< Whether the player can use the sword in this stream. */
    bool allow_item;              /**< Whether the player can use equipment items in this stream. */

};

}

#endif

