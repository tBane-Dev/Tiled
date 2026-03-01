#pragma once
#include <SFML/Graphics.hpp>
#include <set>
#include "../include/Cursor.hpp"
#include "../include/Tileset.hpp"
#include "../include/Textures.hpp"
#include "../include/Shader.hpp"
#include "../include/Time.hpp"
#include "../include/ElementGUI/ElementGUI.hpp"


class Tile {
public:
	const static int side = 16;
	const static int borderWidth = 1;
	sf::Vector2i _coords;
	sf::Vector2f _position;
	int _type;

	Tile();
	~Tile();
};

class Chunk {
public:
	const static int tilesCols = 16;
	const static int tilesRows = 16;
	sf::Vector2i _coords;
	std::vector<std::shared_ptr<Tile>> _tiles;
	sf::VertexArray _vertexArray;

	Chunk();
	~Chunk();

	bool operator<(const Chunk& other) const;
	std::shared_ptr<Tile> getTileByTileGlobalCoords(int x, int y);
	std::shared_ptr<Tile> getTileByGlobalPosition();

	int getNeighborTypeLocal(
		int nx, int ny,
		const std::shared_ptr<Chunk>& leftTopChunk,
		const std::shared_ptr<Chunk>& topChunk,
		const std::shared_ptr<Chunk>& rightTopChunk,
		const std::shared_ptr<Chunk>& leftChunk,
		const std::shared_ptr<Chunk>& rightChunk,
		const std::shared_ptr<Chunk>& leftBottomChunk,
		const std::shared_ptr<Chunk>& bottomChunk,
		const std::shared_ptr<Chunk>& rightBottomChunk
	) const;

	int getNeighborGroupLocal(
		int nx, int ny,
		const std::shared_ptr<Chunk>& leftTopChunk,
		const std::shared_ptr<Chunk>& topChunk,
		const std::shared_ptr<Chunk>& rightTopChunk,
		const std::shared_ptr<Chunk>& leftChunk,
		const std::shared_ptr<Chunk>& rightChunk,
		const std::shared_ptr<Chunk>& leftBottomChunk,
		const std::shared_ptr<Chunk>& bottomChunk,
		const std::shared_ptr<Chunk>& rightBottomChunk
	) const;

	void generateVertexArray(
		std::shared_ptr<Chunk> leftTopChunk,
		std::shared_ptr<Chunk> topChunk,
		std::shared_ptr<Chunk> rightTopChunk,
		std::shared_ptr<Chunk> leftChunk,
		std::shared_ptr<Chunk> rightChunk,
		std::shared_ptr<Chunk> leftBottomChunk,
		std::shared_ptr<Chunk> bottomChunk,
		std::shared_ptr<Chunk> rightBottomChunk,
		const std::shared_ptr<Tile>& hoveredTile = nullptr
	);

	void cursorHover(std::shared_ptr<Tile>& hoveredTile);
	void draw();
};

class Map : public ElementGUI {
public:
	std::vector<std::shared_ptr<Chunk>> _chunks;

	std::shared_ptr<Tile> _prevHoveredTile;
	std::shared_ptr<Chunk> _hoveredChunk;
	std::shared_ptr<Tile> _hoveredTile;

	Map();
	~Map();

	void create(int width, int height);

	std::shared_ptr<Chunk> getChunkByCoords(int x, int y);
	std::shared_ptr<Chunk> getChunkByTileGlobalCoords(int x, int y);
	std::shared_ptr<Chunk> getChunkByGlobalPosition();
	std::shared_ptr<Tile> getTileByTileGlobalCoords(int x, int y);
	sf::IntRect getRect();

	void cursorHover();
	void handleEvent(const sf::Event& event);
	void draw();
};

extern std::shared_ptr<Map> mapa;
