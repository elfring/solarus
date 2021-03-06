/*
 * Copyright (C) 2006-2014 Christopho, Solarus - http://www.solarus-games.org
 *
 * Solarus Quest Editor is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Solarus Quest Editor is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License along
 * with this program. If not, see <http://www.gnu.org/licenses/>.
 */
package org.solarus.editor.entities;

import org.solarus.editor.*;

import java.awt.*;

/**
 * A switch that the hero can activate to change the configuration of the
 * blue and orange blocks on the map.
 */
public class Crystal extends MapEntity {

    /**
     * Description of the default image representing this kind of entity.
     */
    public static final EntityImageDescription[] generalImageDescriptions = {
        new EntityImageDescription("entity_crystal.png", 0, 0, 32, 32)
    };

    /**
     * Origin point of a destination point.
     */
    private static final Point origin = new Point(8, 13);

    /**
     * Creates a new crystal.
     * @param map the map
     */
    public Crystal(Map map) throws MapException {
        super(map, 16, 16);

        try {
            setSprite(new Sprite("entities/crystal", map));
        }
        catch (MapException ex) {
            // The sprite could not be loaded.
            // The default icon for this entity will be used instead.
        }
    }

    /**
     * Returns the coordinates of the origin point of the entity.
     * @return (8,13)
     */
    protected Point getOrigin() {
        return origin;
    }
}

