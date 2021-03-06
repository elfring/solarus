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

/**
 * \file Types.h
 * \brief Some declarations of types.
 *
 * This header is included by Common.h and every class header file should include Common.h.
 */

#ifndef SOLARUS_TYPES_H
#define SOLARUS_TYPES_H

#include <stdint.h>

namespace solarus {

// declaration of all classes to avoid dependencies between the header files

// main classes
class MainLoop;
class CommandLine;
class QuestProperties;
class QuestResourceList;
class Settings;
class KeysEffect;
class StringResource;
class Timer;
class RefCountable;

// low level
class System;
class FileTools;
class Video;
class VideoMode;
class Surface;
class TextSurface;
class Color;
class Shader;
class PixelFilter;
class Scale2xFilter;
class Hq2xFilter;
class Hq3xFilter;
class Hq4xFilter;
class Sound;
class Music;
class SpcDecoder;
class ItDecoder;
class Random;
class Geometry;
class Rectangle;
class PixelBits;
class InputEvent;
class Debug;

// game
class Game;
class GameCommands;
class Savegame;
class Equipment;
class EquipmentItem;
class EquipmentItemUsage;
class Treasure;
class Map;
class MapLoader;
class Camera;
class Dialog;
class DialogResource;

// Lua
class ExportableToLua;
class LuaContext;
class LuaTools;

// drawable objects
class Sprite;
class SpriteAnimationSet;
class SpriteAnimation;
class SpriteAnimationDirection;
class Drawable;

// transition effects
class Transition;
class TransitionFade;
class TransitionImmediate;
class TransitionScrolling;

// map entities
class MapEntities;
class MapEntity;
class Hero;
class HeroSprites;
class NonAnimatedRegions;
class Tile;
class DynamicTile;
class Detector;
class Teletransporter;
class Destination;
class Pickable;
class CarriedItem;
class Destructible;
class Chest;
class Jumper;
class Enemy;
class EnemyReaction;
class Npc;
class Block;
class Switch;
class Wall;
class Sensor;
class Crystal;
class CrystalBlock;
class ShopTreasure;
class Separator;
class CustomEntity;
class Boomerang;
class Arrow;
class Stream;
class StreamAction;
class Stairs;
class Door;
class Explosion;
class Bomb;
class Fire;
class Hookshot;

// tile patterns
class Tileset;
class TilePattern;
class SimpleTilePattern;
class AnimatedTilePattern;
class SelfScrollingTilePattern;
class TimeScrollingTilePattern;
class ParallaxScrollingTilePattern;

// movements
class Movement;
class StraightMovement;
class PlayerMovement;
class PixelMovement;
class FallingOnFloorMovement;
class JumpMovement;
class PathMovement;
class RandomPathMovement;
class PathFindingMovement;
class PathFinding;
class RandomMovement;
class FollowMovement;
class TargetMovement;
class CircleMovement;

}

#endif

