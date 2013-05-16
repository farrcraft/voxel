/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include "font/TextBuffer.h"

#include <vertical3d/gl/Program.h>

#include <boost/shared_ptr.hpp>

class AssetLoader;
class Scene;

class DebugOverlay
{
	public:
		DebugOverlay(boost::shared_ptr<Scene> scene, boost::shared_ptr<v3D::Program> shaderProgram, boost::shared_ptr<AssetLoader> loader);

		void enable(bool status);
		void render();
		void update(unsigned int delta);

		bool enabled() const;

	private:
		boost::shared_ptr<Scene> scene_;
		boost::shared_ptr<TextBuffer> text_;
		bool enabled_;
		TextBuffer::Markup markup_;
		size_t frames_;
		size_t elapsed_;
};