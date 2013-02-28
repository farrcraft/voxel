/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once

#include "Voxel.h"
#include "AssetLoader.h"

#include <hookah/Window.h>

#include <boost/shared_ptr.hpp>

class Program;
class Scene;

/**
 * Main engine renderer
 */
class Renderer
{
	public:
		/**
		 * Default Constructor
		 */
		Renderer(boost::shared_ptr<Scene> scene, AssetLoader loader);

		/**
		 * Draw the frame
		 */
		void draw(Hookah::Window * window);
		/**
		 * Resize the frame
		 */
		void resize(int width, int height);

	protected:

		unsigned int createIndexBuffer(const std::vector<unsigned int> & data);
		unsigned int createVertexBuffer(boost::shared_ptr<Mesh> mesh);

		void drawMesh(boost::shared_ptr<Mesh> mesh);

	private:
		boost::shared_ptr<Program> program_;

		unsigned int vbo_;
		unsigned int vao_;

		boost::shared_ptr<Scene> scene_;
};
