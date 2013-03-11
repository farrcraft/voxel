/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once


#include <boost/shared_ptr.hpp>

class TextBuffer;
class AssetLoader;
class Program;

class DebugOverlay
{
	public:
		DebugOverlay(boost::shared_ptr<Program> shaderProgram, boost::shared_ptr<AssetLoader> loader);

		void enable(bool status);
		void render();
		void update();

		bool enabled() const;

	private:
		boost::shared_ptr<TextBuffer> text_;
		bool enabled_;
};