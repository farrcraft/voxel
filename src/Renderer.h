/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include "voxel/Voxel.h"

#include <hookah/Window.h>

#include <boost/shared_ptr.hpp>

class Program;
class Scene;
class AssetLoader;
class VertexBuffer;
class TextBuffer;

/**
 * Main engine renderer
 */
class Renderer
{
	public:
		/**
		 * Default Constructor
		 */
		Renderer(boost::shared_ptr<Scene> scene, boost::shared_ptr<AssetLoader> loader);

		/**
		 * Draw the frame
		 */
		void draw(Hookah::Window * window);
		/**
		 * Resize the frame
		 */
		void resize(int width, int height);

	protected:
		void drawMesh(boost::shared_ptr<Mesh> mesh);

	private:
		std::map<std::string, boost::shared_ptr<Program> > programs_;
		boost::shared_ptr<VertexBuffer> buffer_;
		unsigned int vao_;
		boost::shared_ptr<TextBuffer> text_;
		boost::shared_ptr<Scene> scene_;
};
