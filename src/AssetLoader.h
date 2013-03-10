/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include <string>

/**
 * Load file assets
 */
class AssetLoader
{
	public:
		/**
		 * Default constructor
		 */
		AssetLoader(const std::string & path);

		/**
		 * Load an asset from a file into a string
		 */
		std::string load(const std::string & filename);

		std::string path() const;

	private:
		std::string path_;
};
