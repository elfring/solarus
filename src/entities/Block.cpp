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
#include "entities/Block.h"
#include "entities/Hero.h"
#include "entities/Switch.h"
#include "movements/FollowMovement.h"
#include "Game.h"
#include "Map.h"
#include "KeysEffect.h"
#include "Sprite.h"
#include "lowlevel/FileTools.h"
#include "lowlevel/System.h"
#include "lowlevel/Debug.h"
#include "lowlevel/Sound.h"
#include "lua/LuaContext.h"

namespace solarus {

/**
 * \brief Creates a block.
 * \param name name identifying this block
 * \param layer layer of the entity to create
 * \param x x coordinate of the entity to create
 * \param y y coordinate of the entity to create
 * \param direction the only direction where the block can be moved
 * or -1 to allow it to be pushed in any direction
 * \param sprite_name animation set id of the sprite for this block
 * \param can_be_pushed true to allow the hero to push this block
 * \param can_be_pulled true to allow the hero to pull this block
 * \param maximum_moves indicates how many times the block can
 * be moved (0: none, 1: once, 2: infinite)
 */
Block::Block(
    const std::string& name,
    Layer layer,
    int x,
    int y,
    int direction,
    const std::string& sprite_name,
    bool can_be_pushed,
    bool can_be_pulled,
    int maximum_moves):
  Detector(COLLISION_FACING, name, layer, x, y, 16, 16),
  maximum_moves(maximum_moves),
  sound_played(false),
  when_can_move(System::now()),
  last_position(x, y),
  initial_position(x, y),
  initial_maximum_moves(maximum_moves),
  can_be_pushed(can_be_pushed),
  can_be_pulled(can_be_pulled) {

  Debug::check_assertion(maximum_moves >= 0 && maximum_moves <= 2,
      "maximum_moves must be between 0 and 2");
  set_origin(8, 13);
  set_direction(direction);
  create_sprite(sprite_name);
  set_drawn_in_y_order(get_sprite().get_size().get_height() > 16);
}

/**
 * \brief Destructor.
 */
Block::~Block() {

}

/**
 * \brief Returns the type of entity.
 * \return the type of entity
 */
EntityType Block::get_type() const {
  return ENTITY_BLOCK;
}

/**
 * \brief Returns whether this entity is sensible to the ground below it.
 * \return \c true if this entity is sensible to its ground.
 */
bool Block::is_ground_observer() const {
  return true;  // To make blocks fall into holes, water, etc.
}

/**
 * \brief Returns whether this entity is an obstacle for another one.
 * \param other another entity
 * \return true
 */
bool Block::is_obstacle_for(MapEntity& other) {

  return other.is_block_obstacle(*this);
}

/**
 * \brief Returns whether a hole is currently considered as an obstacle for this entity.
 * \return true if the holes are currently an obstacle for this entity
 */
bool Block::is_hole_obstacle() const {
  return false;
}

/**
 * \brief Returns whether a teletransporter is currently considered as an obstacle for this entity.
 * \param teletransporter a teletransporter
 * \return true if the teletransporter is currently an obstacle for this entity
 */
bool Block::is_teletransporter_obstacle(Teletransporter& teletransporter) {
  // necessary to push a block into a hole having a teletransporter
  return false;
}

/**
 * \brief Returns whether the hero is currently considered as an obstacle by this entity.
 * \param hero the hero
 * \return true if the hero is an obstacle for this entity.
 */
bool Block::is_hero_obstacle(Hero& hero) {
  return get_movement() == NULL;
}

/**
 * \brief Returns whether an enemy is currently considered as an obstacle by this entity.
 * \param enemy an enemy
 * \return true if this enemy is currently considered as an obstacle by this entity.
 */
bool Block::is_enemy_obstacle(Enemy& enemy) {
  return true;
}

/**
 * \brief Returns whether a destructible item is currently considered as an obstacle by this entity.
 * \param destructible a destructible item
 * \return true if the destructible item is currently an obstacle by this entity
 */
bool Block::is_destructible_obstacle(Destructible& destructible) {
  return true;
}

/**
 * \copydoc MapEntity::notify_created
 */
void Block::notify_created() {

  Detector::notify_created();

  check_collision_with_detectors(false);
  update_ground_below();
}

/**
 * \brief This function is called by the engine when there is a collision with another entity.
 * \param entity_overlapping the entity overlapping the detector
 * \param collision_mode the collision mode that detected the collision
 */
void Block::notify_collision(MapEntity& entity_overlapping, CollisionMode collision_mode) {

  entity_overlapping.notify_collision_with_block(*this);
}

/**
 * \brief This function is called when a switch detects a collision with this entity.
 * \param sw the switch
 * \param collision_mode the collision mode that detected the event
 */
void Block::notify_collision_with_switch(Switch& sw, CollisionMode collision_mode) {

  sw.try_activate(*this);
}

/**
 * \brief Notifies this detector that the player is interacting with it by
 * pressing the action command.
 *
 * This function is called when the player presses the action command
 * while the hero is facing this detector, and the action command effect lets
 * him do this.
 */
void Block::notify_action_command_pressed() {

  if (get_keys_effect().get_action_key_effect() == KeysEffect::ACTION_KEY_GRAB) {
    get_hero().start_grabbing();
  }
}

/**
 * \brief This function is called when the player tries to push or pull this block.
 * \return true if the player is allowed to move this block
 */
bool Block::start_movement_by_hero() {

  Hero& hero = get_hero();
  bool pulling = hero.is_grabbing_or_pulling();
  int allowed_direction = get_direction();
  int hero_direction = hero.get_animation_direction();
  if (pulling) {
    // the movement direction is backwards
    hero_direction = (hero_direction + 2) % 4;
  }

  if (get_movement() != NULL                // the block is already moving
      || maximum_moves == 0                 // the block cannot move anymore
      || System::now() < when_can_move      // the block cannot move for a while
      || (pulling && !can_be_pulled)        // the hero tries to pull a block that cannot be pulled
      || (!pulling && !can_be_pushed)       // the hero tries to push a block that cannot be pushed
      || (allowed_direction != -1 && hero_direction != allowed_direction)) { // incorrect direction
    return false;
  }

  int dx = get_x() - hero.get_x();
  int dy = get_y() - hero.get_y();

  set_movement(new FollowMovement(&hero, dx, dy, false));
  sound_played = false;

  return true;
}

/**
 * \brief Notifies the block that it has just moved.
 */
void Block::notify_position_changed() {

  // now we know that the block moves at least of 1 pixel:
  // we can play the sound
  if (get_movement() != NULL && !sound_played) {
    Sound::play("hero_pushes");
    sound_played = true;
  }

  check_collision_with_detectors(false);
  update_ground_below();

  if (are_movement_notifications_enabled()) {
    get_lua_context().entity_on_position_changed(*this, get_xy(), get_layer());
  }
}

/**
 * \brief Notifies this entity that it has just failed to change its position
 * because of obstacles.
 */
void Block::notify_obstacle_reached() {

  // the block is stopped by an obstacle while being pushed or pulled
  get_hero().notify_grabbed_entity_collision();

  Detector::notify_obstacle_reached();
}

/**
 * \copydoc MapEntity::notify_ground_below_changed
 */
void Block::notify_ground_below_changed() {

  Ground ground = get_ground_below();
  switch (ground) {

    case GROUND_HOLE:
      Sound::play("jump");
      remove_from_map();
      break;

    case GROUND_LAVA:
    case GROUND_DEEP_WATER:
      Sound::play("splash");
      remove_from_map();
      break;

    default:
      break;
  }
}

/**
 * \brief This function is called when this entity stops being moved by the
 * hero.
 */
void Block::stop_movement_by_hero() {

  clear_movement();
  when_can_move = System::now() + moving_delay;

  // see if the block has moved
  if (get_x() != last_position.get_x() || get_y() != last_position.get_y()) {

    // the block has moved
    last_position.set_xy(get_x(), get_y()); // save the new position for next time

    if (maximum_moves == 1) { // if the block could be moved only once,
      maximum_moves = 0;      // then it cannot move anymore
    }
  }
}

/**
 * \copydoc MapEntity::notify_moving_by
 */
void Block::notify_moving_by(MapEntity& entity) {

  get_lua_context().block_on_moving(*this);
}

/**
 * \copydoc MapEntity::notify_moved_by
 */
void Block::notify_moved_by(MapEntity& entity) {

  get_lua_context().block_on_moved(*this);
}

/**
 * \brief Resets the block at its initial position.
 */
void Block::reset() {

  if (get_movement() != NULL) {
    // the block was being pushed or pulled by the hero
    clear_movement();
    when_can_move = System::now() + moving_delay;
  }

  set_xy(initial_position);
  last_position.set_xy(initial_position);
  this->maximum_moves = initial_maximum_moves;
}

/**
 * \brief Returns whether this block can be pushed.
 * \return \c true if it can be pushed, independently of the maximum moves.
 */
bool Block::is_pushable() const {
  return can_be_pushed;
}

/**
 * \brief Returns whether this block can be pushed.
 * \return \c true if it can be pushed, independently of the maximum moves.
 */
void Block::set_pushable(bool pushable) {
  this->can_be_pushed = pushable;
}

/**
 * \brief Returns whether this block can be pulled.
 * \return \c true if it can be pulled, independently of the maximum moves.
 */
bool Block::is_pullable() const {
  return can_be_pulled;
}

/**
 * \brief Sets whether this block can be pulled.
 * \param pullable \c true to make the block pullable, independently of the
 * maximum moves.
 */
void Block::set_pullable(bool pullable) {
  this->can_be_pulled = pullable;
}

/**
 * \brief Returns the initial maximum moves of this block.
 *
 * This value is the one passed to the constructor or to set_maximum_moves,
 * no matter if the block was already moved or not.
 *
 * \return How many times the block can be moved
 * (0: none, 1: once, 2: infinite).
 */
int Block::get_maximum_moves() const {
  return initial_maximum_moves;
}

/**
 * \brief Sets the maximum moves of this block.
 *
 * This resets the remaining allowed moves.
 *
 * \return How many times the block can be moved
 * (0: none, 1: once, 2: infinite).
 */
void Block::set_maximum_moves(int maximum_moves) {

  Debug::check_assertion(maximum_moves >= 0 && maximum_moves <= 2,
        "maximum_moves must be between 0 and 2");

  this->initial_maximum_moves = maximum_moves;
  this->maximum_moves = maximum_moves;
}

}

