Tilemap LoadMapFromImage(Image image)
{
	Tilemap tilemap;

	Color* pixels = GetImageData(image);

	tilemap.width = image.width;
	tilemap.height = image.height;
	tilemap.tiles = malloc(sizeof(uint32_t) * image.width * image.height);

	for (int i = 0; i < (image.width * image.height); i++)
	{
		tilemap.tiles[i] = pixels[i].r;
	}

	if (pixels) free(pixels);

	return tilemap;
}

void UnloadMap(Tilemap map)
{
	if (map.tiles) free(map.tiles);
}

inline uint32_t GetTile(Tilemap* map, uint32_t x, uint32_t y)
{
	if (x >= map->width || y >= map->height) return 0;

	return map->tiles[(y * map->width) + x];
	
}