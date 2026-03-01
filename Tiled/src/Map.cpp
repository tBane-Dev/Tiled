#include "../include/Map.hpp"
#include "../include/Shader.hpp"
#include "../include/ElementGUI/ElementGUI.hpp"
#include "../include/Dialogs/Palette.hpp"
#include "../include/Camera.hpp"
#include "../include/Dialogs/Palette.hpp"

Tile::Tile() {
	_type = 0;
}

Tile::~Tile() {

}

Chunk::Chunk() {
	_vertexArray = sf::VertexArray(sf::PrimitiveType::Triangles);
}

Chunk::~Chunk() {

}


bool Chunk::operator<(const Chunk& other) const {
	if (_coords.x == other._coords.x)
		return _coords.y < other._coords.y;
	return _coords.x < other._coords.x;
}

std::shared_ptr<Tile> Chunk::getTileByTileGlobalCoords(int x, int y) {

	int xx = x - _coords.x * tilesCols;
	int yy = y - _coords.y * tilesRows;

	if (xx < 0 || yy < 0 || xx >= tilesCols || yy >= tilesRows)
		return nullptr;

	return _tiles[yy * Chunk::tilesCols + xx];
}

std::shared_ptr<Tile> Chunk::getTileByGlobalPosition() {

	sf::IntRect rect;
	rect.position.x = _coords.x * tilesCols * Tile::side;
	rect.position.y = _coords.y * tilesRows * Tile::side;
	rect.size.x = tilesCols * Tile::side;
	rect.size.y = tilesRows * Tile::side;

	if (rect.contains(cursor->_worldPosition)) {
		int x = cursor->_worldPosition.x / Tile::side;
		int y = cursor->_worldPosition.y / Tile::side;
		return getTileByTileGlobalCoords(x, y);
	}

	return nullptr;
}

int Chunk::getNeighborTypeLocal(
	int nx, int ny,
	const std::shared_ptr<Chunk>& leftTopChunk,
	const std::shared_ptr<Chunk>& topChunk,
	const std::shared_ptr<Chunk>& rightTopChunk,
	const std::shared_ptr<Chunk>& leftChunk,
	const std::shared_ptr<Chunk>& rightChunk,
	const std::shared_ptr<Chunk>& leftBottomChunk,
	const std::shared_ptr<Chunk>& bottomChunk,
	const std::shared_ptr<Chunk>& rightBottomChunk
) const
{
	constexpr int EMPTY_TYPE = -1;

	if (nx >= 0 && nx < tilesCols && ny >= 0 && ny < tilesRows) {
		return _tiles[ny * tilesCols + nx]->_type;
	}

	const int dx = (nx < 0) ? -1 : (nx >= tilesCols ? 1 : 0);
	const int dy = (ny < 0) ? -1 : (ny >= tilesRows ? 1 : 0);

	std::shared_ptr<Chunk> c = nullptr;

	if (dx == -1 && dy == -1) c = leftTopChunk;
	else if (dx == 0 && dy == -1) c = topChunk;
	else if (dx == 1 && dy == -1) c = rightTopChunk;

	else if (dx == -1 && dy == 0) c = leftChunk;
	else if (dx == 1 && dy == 0) c = rightChunk;

	else if (dx == -1 && dy == 1) c = leftBottomChunk;
	else if (dx == 0 && dy == 1) c = bottomChunk;
	else if (dx == 1 && dy == 1) c = rightBottomChunk;

	if (!c) return EMPTY_TYPE;

	int lx = nx;
	int ly = ny;

	if (lx < 0) lx += tilesCols;
	else if (lx >= tilesCols) lx -= tilesCols;

	if (ly < 0) ly += tilesRows;
	else if (ly >= tilesRows) ly -= tilesRows;

	return c->_tiles[ly * tilesCols + lx]->_type;
}

int Chunk::getNeighborGroupLocal(
	int nx, int ny,
	const std::shared_ptr<Chunk>& leftTopChunk,
	const std::shared_ptr<Chunk>& topChunk,
	const std::shared_ptr<Chunk>& rightTopChunk,
	const std::shared_ptr<Chunk>& leftChunk,
	const std::shared_ptr<Chunk>& rightChunk,
	const std::shared_ptr<Chunk>& leftBottomChunk,
	const std::shared_ptr<Chunk>& bottomChunk,
	const std::shared_ptr<Chunk>& rightBottomChunk
) const
{
	int t = getNeighborTypeLocal(
		nx, ny,
		leftTopChunk, topChunk, rightTopChunk,
		leftChunk, rightChunk,
		leftBottomChunk, bottomChunk, rightBottomChunk
	);

	return t;
}

void Chunk::generateVertexArray(
	std::shared_ptr<Chunk> leftTopChunk,
	std::shared_ptr<Chunk> topChunk,
	std::shared_ptr<Chunk> rightTopChunk,
	std::shared_ptr<Chunk> leftChunk,
	std::shared_ptr<Chunk> rightChunk,
	std::shared_ptr<Chunk> leftBottomChunk,
	std::shared_ptr<Chunk> bottomChunk,
	std::shared_ptr<Chunk> rightBottomChunk,
	const std::shared_ptr<Tile>& hoveredTile
) {

	_vertexArray.clear();
	_vertexArray.setPrimitiveType(sf::PrimitiveType::Triangles);

	float borderWidth = float(Tile::borderWidth);

	for (int y = 0; y < tilesRows; y++) {
		for (int x = 0; x < tilesCols; x++) {

			int tx = _coords.x * tilesCols + x;
			int ty = _coords.y * tilesRows + y;

			float xx = float(tx * Tile::side);
			float yy = float(ty * Tile::side);

			sf::Color color = sf::Color::White;

			if (hoveredTile && 
				tx > hoveredTile->_coords.x - palette->_brushSize && 
				tx < hoveredTile->_coords.x + palette->_brushSize && 
				ty > hoveredTile->_coords.y - palette->_brushSize && 
				ty < hoveredTile->_coords.y + palette->_brushSize) {
					color = sf::Color(255, 0, 0, 127);
			}


			int tileType = 5;
			int type = getTileByTileGlobalCoords(tx, ty)->_type;
			// Sprawdzanie sąsiadów z uwzględnieniem krawędzi
			bool hasLeft = areFriends(type, getNeighborGroupLocal(
				x - 1, y,
				leftTopChunk, topChunk, rightTopChunk,
				leftChunk, rightChunk,
				leftBottomChunk, bottomChunk, rightBottomChunk
			));

			bool hasRight = areFriends(type, getNeighborGroupLocal(
				x + 1, y,
				leftTopChunk, topChunk, rightTopChunk,
				leftChunk, rightChunk,
				leftBottomChunk, bottomChunk, rightBottomChunk
			));

			bool hasTop = areFriends(type, getNeighborGroupLocal(
				x, y - 1,
				leftTopChunk, topChunk, rightTopChunk,
				leftChunk, rightChunk,
				leftBottomChunk, bottomChunk, rightBottomChunk
			));

			bool hasBottom = areFriends(type, getNeighborGroupLocal(
				x, y + 1,
				leftTopChunk, topChunk, rightTopChunk,
				leftChunk, rightChunk,
				leftBottomChunk, bottomChunk, rightBottomChunk
			));

			// Narożniki:
			bool hasTopLeft = areFriends(type, getNeighborGroupLocal(
				x - 1, y - 1,
				leftTopChunk, topChunk, rightTopChunk,
				leftChunk, rightChunk,
				leftBottomChunk, bottomChunk, rightBottomChunk
			));

			bool hasTopRight = areFriends(type, getNeighborGroupLocal(
				x + 1, y - 1,
				leftTopChunk, topChunk, rightTopChunk,
				leftChunk, rightChunk,
				leftBottomChunk, bottomChunk, rightBottomChunk
			));

			bool hasBottomLeft = areFriends(type, getNeighborGroupLocal(
				x - 1, y + 1,
				leftTopChunk, topChunk, rightTopChunk,
				leftChunk, rightChunk,
				leftBottomChunk, bottomChunk, rightBottomChunk
			));

			bool hasBottomRight = areFriends(type, getNeighborGroupLocal(
				x + 1, y + 1,
				leftTopChunk, topChunk, rightTopChunk,
				leftChunk, rightChunk,
				leftBottomChunk, bottomChunk, rightBottomChunk
			));
			// Logika dobierania odpowiedniego fragmentu
			if (!hasTop && !hasLeft) {
				if (hasRight && hasBottom && !hasBottomRight) {
					tileType = 13; // wewnętrzny lewo-góra
				}
				else {
					tileType = 1; // standardowy lewo-góra
				}
			}
			else if (!hasTop && !hasRight) {
				tileType = 3; // prawo-góra
			}
			else if (!hasBottom && !hasLeft) {
				tileType = 7; // lewo-dół
			}
			else if (!hasBottom && !hasRight) {
				tileType = 9; // prawo-dół
			}
			else if (!hasTop) {
				tileType = 2; // góra
			}
			else if (!hasLeft) {
				tileType = 4; // lewo
			}
			else if (!hasRight) {
				tileType = 6; // prawo
			}
			else if (!hasBottom) {
				tileType = 8; // dół
			}
			else {
				// Dla pełnego otoczenia - sprawdź wewnętrzne narożniki
				if (!hasTopLeft && hasTop && hasLeft) {
					tileType = 11; // wewnętrzny lewo-góra dla środka
				}
				else if (!hasTopRight && hasTop && hasRight) {
					tileType = 12; // wewnętrzny prawo-góra dla środka
				}
				else if (!hasBottomLeft && hasBottom && hasLeft) {
					tileType = 13; // wewnętrzny lewo-dół dla środka
				}
				else if (!hasBottomRight && hasBottom && hasRight) {
					tileType = 14; // wewnętrzny prawo-dół dla środka
				}
				else {
					tileType = 5; // środek
				}
			}

			std::shared_ptr<Tile> t = getTileByTileGlobalCoords(tx, ty);
			Tileset::Tile tile = tileset->groups[t->_type]->tiles[tileType];

			int u = (tile.x) * Tile::side + tx * Tile::side % 64;
			int v = (tile.y) * Tile::side + ty * Tile::side % 64;

			sf::Vertex v1 = sf::Vertex({ xx + borderWidth, yy + borderWidth }, color, sf::Vector2f(u + 1, v + 1));
			sf::Vertex v2 = sf::Vertex({ xx + borderWidth, yy + Tile::side - borderWidth }, color, sf::Vector2f(u + 1, v + Tile::side - 1));
			sf::Vertex v3 = sf::Vertex({ xx + Tile::side - borderWidth, yy + Tile::side - borderWidth }, color, sf::Vector2f(u + Tile::side - 1, v + Tile::side - 1));
			sf::Vertex v4 = sf::Vertex({ xx + Tile::side - borderWidth, yy + borderWidth }, color, sf::Vector2f(u + Tile::side - 1, v + 1));


			_vertexArray.append(v1);
			_vertexArray.append(v2);
			_vertexArray.append(v3);

			_vertexArray.append(v1);
			_vertexArray.append(v3);
			_vertexArray.append(v4);
		}
	}
}

void Chunk::cursorHover(std::shared_ptr<Tile>& hoveredTile) {
	auto tile = getTileByGlobalPosition();

	if (!tile || tile == hoveredTile) return;

	hoveredTile = tile;
}

void Chunk::draw() {

	sf::RenderStates states;
	std::shared_ptr<Texture> texture = getTexture(L"tex\\tileset.png");
	shader->setUniform("resolution", sf::Vector2f(window->getSize()));
	shader->setUniform("time", currentTime.asSeconds());
	shader->setUniform("startPos", sf::Vector2f(0, 0));
	states.texture = &*texture->_texture;
	states.shader = &*shader;
	window->draw(_vertexArray, states);
}

Map::Map() {
	create(4, 2);
	_prevHoveredTile = nullptr;
	_hoveredChunk = nullptr;
	_hoveredTile = nullptr;
}

Map::~Map() {

}

void Map::create(int width, int height) {
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			std::shared_ptr<Chunk> chunk = std::make_shared<Chunk>();

			chunk->_coords.x = x;
			chunk->_coords.y = y;

			for (int ty = 0; ty < Chunk::tilesRows; ty++) {
				for (int tx = 0; tx < Chunk::tilesCols; tx++) {
					std::shared_ptr<Tile> tile = std::make_shared<Tile>();


					tile->_coords.x = x * Chunk::tilesCols + tx;
					tile->_coords.y = y * Chunk::tilesRows + ty;

					tile->_position.x = (float)(tile->_coords.x * Tile::side);
					tile->_position.y = (float)(tile->_coords.y * Tile::side);

					chunk->_tiles.push_back(tile);

				}
			}

			chunk->generateVertexArray(
				getChunkByCoords(x - 1, y - 1), getChunkByCoords(x, y - 1), getChunkByCoords(x + 1, y - 1),
				getChunkByCoords(x - 1, y), getChunkByCoords(x + 1, y),
				getChunkByCoords(x - 1, y + 1), getChunkByCoords(x, y + 1), getChunkByCoords(x + 1, y + 1)
			);

			_chunks.push_back(chunk);

		}
	}
}

std::shared_ptr<Chunk> Map::getChunkByCoords(int x, int y) {
	for (auto& chunk : _chunks) {
		if (chunk->_coords.x == x && chunk->_coords.y == y) {
			return chunk;
		}
	}

	return nullptr;
}

std::shared_ptr<Chunk> Map::getChunkByTileGlobalCoords(int x, int y) {
	x /= Chunk::tilesCols;
	y /= Chunk::tilesRows;

	for (auto& chunk : _chunks) {
		if (chunk->_coords.x == x && chunk->_coords.y == y) {
			return chunk;
		}
	}

	return nullptr;
}

std::shared_ptr<Chunk> Map::getChunkByGlobalPosition() {

	int x = cursor->_worldPosition.x / Tile::side;
	int y = cursor->_worldPosition.y / Tile::side;

	return getChunkByTileGlobalCoords(x, y);
}

std::shared_ptr<Tile> Map::getTileByTileGlobalCoords(int x, int y) {

	std::shared_ptr<Chunk> chunk = getChunkByTileGlobalCoords(x, y);

	if (chunk == nullptr) {
		return nullptr;
	}

	return chunk->getTileByTileGlobalCoords(x, y);
}

sf::IntRect Map::getRect() {
	sf::Vector2i minCoords = sf::Vector2i(INT_MAX, INT_MAX);
	sf::Vector2i maxCoords = sf::Vector2i(0, 0);

	for (auto& chunk : _chunks) {
		if (chunk->_coords.x < minCoords.x) minCoords.x = chunk->_coords.x;
		if (chunk->_coords.y < minCoords.y) minCoords.y = chunk->_coords.y;
		if (chunk->_coords.x > maxCoords.x) maxCoords.x = chunk->_coords.x;
		if (chunk->_coords.y > maxCoords.y) maxCoords.y = chunk->_coords.y;
	}


	sf::IntRect rect;
	rect.position = sf::Vector2i(
		minCoords.x * Chunk::tilesCols * Tile::side,
		minCoords.y * Chunk::tilesRows * Tile::side
		);

	rect.size = sf::Vector2i(
		(maxCoords.x - minCoords.x + 1) * Chunk::tilesCols * Tile::side,
		(maxCoords.y - minCoords.y + 1) * Chunk::tilesRows * Tile::side
	);

	return rect;
}

void Map::cursorHover() {

	if (!getRect().contains(cursor->_worldPosition)) {
		if (_hoveredTile || _hoveredChunk) {
			if (_hoveredChunk) {
				_hoveredChunk->generateVertexArray(
					getChunkByCoords(_hoveredChunk->_coords.x - 1, _hoveredChunk->_coords.y - 1), getChunkByCoords(_hoveredChunk->_coords.x, _hoveredChunk->_coords.y - 1), getChunkByCoords(_hoveredChunk->_coords.x + 1, _hoveredChunk->_coords.y - 1),
					getChunkByCoords(_hoveredChunk->_coords.x - 1, _hoveredChunk->_coords.y), getChunkByCoords(_hoveredChunk->_coords.x + 1, _hoveredChunk->_coords.y),
					getChunkByCoords(_hoveredChunk->_coords.x - 1, _hoveredChunk->_coords.y + 1), getChunkByCoords(_hoveredChunk->_coords.x, _hoveredChunk->_coords.y + 1), getChunkByCoords(_hoveredChunk->_coords.x + 1, _hoveredChunk->_coords.y + 1),
					nullptr
				);
			}
			_hoveredTile = nullptr;
			_hoveredChunk = nullptr;
		}

		ElementGUI_hovered = nullptr;
		return;
	}

	auto newChunk = getChunkByGlobalPosition();
	if (!newChunk) return;

	auto prevChunk = _hoveredChunk;
	auto prevTile = _hoveredTile;

	// zaktualizuj hovered tile
	newChunk->cursorHover(_hoveredTile);

	// jeśli hover się zmienił albo chunk się zmienił:
	if (newChunk != prevChunk || _hoveredTile != prevTile) {

		// nałóż highlight na nowy chunk
		newChunk->generateVertexArray(
			getChunkByCoords(newChunk->_coords.x - 1, newChunk->_coords.y - 1), getChunkByCoords(newChunk->_coords.x, newChunk->_coords.y - 1), getChunkByCoords(newChunk->_coords.x + 1, newChunk->_coords.y - 1),
			getChunkByCoords(newChunk->_coords.x - 1, newChunk->_coords.y), getChunkByCoords(newChunk->_coords.x + 1, newChunk->_coords.y),
			getChunkByCoords(newChunk->_coords.x - 1, newChunk->_coords.y + 1), getChunkByCoords(newChunk->_coords.x, newChunk->_coords.y + 1), getChunkByCoords(newChunk->_coords.x + 1, newChunk->_coords.y + 1),
			_hoveredTile
		);

		std::set<std::shared_ptr<Chunk>> chunksToEdit;

		if (getChunkByCoords(newChunk->_coords.x - 1, newChunk->_coords.y - 1)) chunksToEdit.insert(getChunkByCoords(newChunk->_coords.x - 1, newChunk->_coords.y - 1));
		if (getChunkByCoords(newChunk->_coords.x, newChunk->_coords.y - 1)) chunksToEdit.insert(getChunkByCoords(newChunk->_coords.x, newChunk->_coords.y - 1));
		if (getChunkByCoords(newChunk->_coords.x + 1, newChunk->_coords.y - 1)) chunksToEdit.insert(getChunkByCoords(newChunk->_coords.x + 1, newChunk->_coords.y - 1));

		if (getChunkByCoords(newChunk->_coords.x - 1, newChunk->_coords.y)) chunksToEdit.insert(getChunkByCoords(newChunk->_coords.x - 1, newChunk->_coords.y));
		if (getChunkByCoords(newChunk->_coords.x + 1, newChunk->_coords.y)) chunksToEdit.insert(getChunkByCoords(newChunk->_coords.x + 1, newChunk->_coords.y));

		if (getChunkByCoords(newChunk->_coords.x - 1, newChunk->_coords.y + 1)) chunksToEdit.insert(getChunkByCoords(newChunk->_coords.x - 1, newChunk->_coords.y + 1));
		if (getChunkByCoords(newChunk->_coords.x, newChunk->_coords.y + 1)) chunksToEdit.insert(getChunkByCoords(newChunk->_coords.x, newChunk->_coords.y + 1));
		if (getChunkByCoords(newChunk->_coords.x + 1, newChunk->_coords.y + 1)) chunksToEdit.insert(getChunkByCoords(newChunk->_coords.x + 1, newChunk->_coords.y + 1));

		for (auto& chunk : chunksToEdit) {
			chunk->generateVertexArray(
				getChunkByCoords(chunk->_coords.x - 1, chunk->_coords.y - 1), getChunkByCoords(chunk->_coords.x, chunk->_coords.y - 1), getChunkByCoords(chunk->_coords.x + 1, chunk->_coords.y - 1),
				getChunkByCoords(chunk->_coords.x - 1, chunk->_coords.y), getChunkByCoords(chunk->_coords.x + 1, chunk->_coords.y),
				getChunkByCoords(chunk->_coords.x - 1, chunk->_coords.y + 1), getChunkByCoords(chunk->_coords.x, chunk->_coords.y + 1), getChunkByCoords(chunk->_coords.x + 1, chunk->_coords.y + 1),
				_hoveredTile
			);
		}
	}

	_hoveredChunk = newChunk;

	if (getRect().contains(cursor->_worldPosition)) {
		ElementGUI_hovered = shared_from_this();
	}
}

void Map::handleEvent(const sf::Event& event) {

	if (!(ElementGUI_hovered.get() == this && (ElementGUI_pressed.get() == this || ElementGUI_pressed == nullptr))) return;


	bool conditionToDraw = false;

	const auto* mv = event.getIf<sf::Event::MouseMoved>();
	const auto* mbp = event.getIf<sf::Event::MouseButtonPressed>();
	
	if((mv || mbp) && sf::Mouse::isButtonPressed(sf::Mouse::Button::Left))
		conditionToDraw = true;

	if (conditionToDraw) {

		if (_prevHoveredTile != nullptr && _prevHoveredTile == _hoveredTile)
			return;

		int r = palette->_brushSize;
		int type = palette->_selectedType;

		std::set<std::shared_ptr<Chunk>> chunksToEdit;
		for (int yy = -r/2; yy <= r/2; yy++) {
			for (int xx = -r/2; xx <= r/2; xx++) {

				std::shared_ptr<Chunk> c = getChunkByTileGlobalCoords(_hoveredTile->_coords.x + xx, _hoveredTile->_coords.y + yy);
				if (!c) continue;

				std::shared_ptr<Tile> t = getTileByTileGlobalCoords(_hoveredTile->_coords.x + xx, _hoveredTile->_coords.y + yy);
				if (!t) continue;

				t->_type = type;
				chunksToEdit.insert(c);

				if (getChunkByCoords(c->_coords.x - 1, c->_coords.y - 1)) chunksToEdit.insert(getChunkByCoords(c->_coords.x - 1, c->_coords.y - 1));
				if (getChunkByCoords(c->_coords.x, c->_coords.y - 1)) chunksToEdit.insert(getChunkByCoords(c->_coords.x, c->_coords.y - 1));
				if (getChunkByCoords(c->_coords.x + 1, c->_coords.y - 1)) chunksToEdit.insert(getChunkByCoords(c->_coords.x + 1, c->_coords.y - 1));

				if (getChunkByCoords(c->_coords.x - 1, c->_coords.y)) chunksToEdit.insert(getChunkByCoords(c->_coords.x - 1, c->_coords.y));
				if (getChunkByCoords(c->_coords.x + 1, c->_coords.y)) chunksToEdit.insert(getChunkByCoords(c->_coords.x + 1, c->_coords.y));

				if (getChunkByCoords(c->_coords.x - 1, c->_coords.y + 1)) chunksToEdit.insert(getChunkByCoords(c->_coords.x - 1, c->_coords.y + 1));
				if (getChunkByCoords(c->_coords.x, c->_coords.y + 1)) chunksToEdit.insert(getChunkByCoords(c->_coords.x, c->_coords.y + 1));
				if (getChunkByCoords(c->_coords.x + 1, c->_coords.y + 1)) chunksToEdit.insert(getChunkByCoords(c->_coords.x + 1, c->_coords.y + 1));
			}
		}

		for (auto& c : chunksToEdit)
			c->generateVertexArray(
				getChunkByCoords(c->_coords.x - 1, c->_coords.y - 1), getChunkByCoords(c->_coords.x, c->_coords.y - 1), getChunkByCoords(c->_coords.x + 1, c->_coords.y - 1),
				getChunkByCoords(c->_coords.x - 1, c->_coords.y), getChunkByCoords(c->_coords.x + 1, c->_coords.y),
				getChunkByCoords(c->_coords.x - 1, c->_coords.y + 1), getChunkByCoords(c->_coords.x, c->_coords.y + 1), getChunkByCoords(c->_coords.x + 1, c->_coords.y + 1),
				_hoveredTile

			);
	}
}

void Map::draw() {

	window->setView(camera->_view);


	for (auto& chunk : _chunks) {
		chunk->draw();
	}
}

std::shared_ptr<Map> mapa;