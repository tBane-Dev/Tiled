#include "../include/Map/CursorOnMap.hpp"
#include "../include/Cursor.hpp"
#include "../include/Camera.hpp"
#include "../include/Dialogs/Palette.hpp"
#include "../include/Map/Map.hpp"


CursorOnMap::CursorOnMap() {
    window->setView(camera->_view);
    _position = sf::Vector2i(window->mapPixelToCoords(cursor->_position));
}

CursorOnMap::~CursorOnMap() {

}

void CursorOnMap::update() {
    window->setView(camera->_view);
    _position = sf::Vector2i(window->mapPixelToCoords(cursor->_position));
}

void CursorOnMap::handleEvent(const sf::Event& event) {

}

void CursorOnMap::draw()
{
	
    if (ElementGUI_hovered != mapa) return;

    window->setView(camera->_view);

    int r = palette->_brushSize;
    sf::Color color(255, 0, 0, 127);

	std::shared_ptr<Chunk> hoveredChunk = mapa->getChunkByGlobalPosition();
	if (!hoveredChunk) return;

	std::shared_ptr<Tile> hoveredTile = hoveredChunk->getTileByGlobalPosition();
	if (!hoveredTile) return;

    float bw = Tile::borderWidth;

    sf::VertexArray vertexArray(sf::PrimitiveType::Triangles);

    for (int yy = -r / 2; yy <= r / 2; yy++) {
        for (int xx = -r / 2; xx <= r / 2; xx++) {
            int tx = hoveredTile->_coords.x + xx;
            int ty = hoveredTile->_coords.y + yy;

			std::shared_ptr<Chunk> chunk = mapa->getChunkByTileGlobalCoords(tx, ty);
			if (!chunk) continue;

            std::shared_ptr<Tile> tile = chunk->getTileByTileGlobalCoords(tx, ty);
            if (!tile) continue;

            float x = (float)(tx * Tile::side);
            float y = (float)(ty * Tile::side);

            sf::Vertex v1({ x + bw, y + bw }, color);
            sf::Vertex v2({ x + bw, y + Tile::side - bw }, color);
            sf::Vertex v3({ x + Tile::side - bw, y + Tile::side - bw }, color);
            sf::Vertex v4({ x + Tile::side - bw, y + bw }, color);

            vertexArray.append(v1);
            vertexArray.append(v2);
            vertexArray.append(v3);

            vertexArray.append(v1);
            vertexArray.append(v3);
            vertexArray.append(v4);
        }
    }

	window->draw(vertexArray);
}

std::shared_ptr<CursorOnMap> cursorOnMap;