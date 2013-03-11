/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#pragma once;

#include <hookah/Window.h>

#include <input/KeyboardDevice.h>
#include <input/MouseDevice.h>

#include <gui/InputEventAdapter.h>
#include <command/CommandDirectory.h>

#include <boost/shared_ptr.hpp>

#include <string>

class Renderer;
class Scene;

/**
 * Application controller
 */
class Controller :
	public v3D::MouseEventListener
{
	public:
		/**
		 * Initialize the controller
		 *
		 */
		Controller(const std::string & path);

		/**
		 * Run the application
		 */
		bool run();

		bool exec(const v3D::CommandInfo & command, const std::string & param);
		bool execUI(const v3D::CommandInfo & command, const std::string & param);

		// mouse event listener overrides
		void motion(unsigned int x, unsigned int y);
		void buttonPressed(unsigned int button);
		void buttonReleased(unsigned int button);

	private:
		boost::shared_ptr<Hookah::Window> window_;

		boost::shared_ptr<v3D::KeyboardDevice> keyboard_;
		boost::shared_ptr<v3D::MouseDevice> mouse_;

		boost::shared_ptr<v3D::CommandDirectory> directory_;
		boost::shared_ptr<v3D::InputEventAdapter> listenerAdapter_;

		boost::shared_ptr<Renderer> renderer_;
		boost::shared_ptr<Scene> scene_;

		std::string path_;

		bool debug_;
};
