/**
 * Voxel Engine
 *
 * (c) Joshua Farr <j.wgasa@gmail.com>
 *
 */

#include "Controller.h"
#include "Renderer.h"
#include "AssetLoader.h"
#include "Scene.h"

#include <hookah/Hookah.h>
#include <command/BindLoader.h>

#include <boost/bind.hpp>
#include <boost/property_tree/xml_parser.hpp>


Controller::Controller(const std::string & path) : 
	path_(path)
{
	AssetLoader loader(path);

	// create a new command directory object
	directory_.reset(new v3D::CommandDirectory());

	// create new app window and set caption
	window_ = Hookah::Create3DWindow(512, 512);

	// create input devices
	// NB - dynamic_pointer_cast appears to be a replacement for shared_dynamic_cast
	keyboard_ = boost::dynamic_pointer_cast<v3D::KeyboardDevice, v3D::InputDevice>(Hookah::CreateInputDevice("keyboard"));
	mouse_ = boost::dynamic_pointer_cast<v3D::MouseDevice, v3D::InputDevice>(Hookah::CreateInputDevice("mouse"));

	// register directory as an observer of input device events
	listenerAdapter_.reset(new v3D::InputEventAdapter(keyboard_, mouse_));
	listenerAdapter_->connect(directory_.get());

	// add device to window
	window_->addInputDevice("keyboard", keyboard_);
	window_->addInputDevice("mouse", mouse_);

	window_->caption("Voxel");

	// load config file into a property tree
	boost::property_tree::ptree ptree;
	std::string configPath = path + std::string("config.xml");
	boost::property_tree::read_xml(configPath, ptree);

	// register game commands
	// player commands
	directory_->add("moveUp", "voxel", boost::bind(&Controller::exec, boost::ref(*this), _1, _2));
	directory_->add("moveDown", "voxel", boost::bind(&Controller::exec, boost::ref(*this), _1, _2));
	directory_->add("moveLeft", "voxel", boost::bind(&Controller::exec, boost::ref(*this), _1, _2));
	directory_->add("moveRight", "voxel", boost::bind(&Controller::exec, boost::ref(*this), _1, _2));
	directory_->add("moveForward", "voxel", boost::bind(&Controller::exec, boost::ref(*this), _1, _2));
	directory_->add("moveBackward", "voxel", boost::bind(&Controller::exec, boost::ref(*this), _1, _2));
	directory_->add("look", "voxel", boost::bind(&Controller::exec, boost::ref(*this), _1, _2));
	// ui commands
	directory_->add("showGameMenu", "ui", boost::bind(&Controller::execUI, boost::ref(*this), _1, _2));

	// load key binds from the property tree
	v3D::utility::load_binds(ptree, directory_.get());

	scene_.reset(new Scene());

	// this is actually the game controller
	// maybe we need a separate player controller class to intercept mouse events?
	mouse_->addEventListener(this, "player_controller");

	renderer_.reset(new Renderer(scene_, loader));

	// set the scene size according to the window canvas
	renderer_->resize(window_->width(), window_->height());

	// register event listeners
	window_->addDrawListener(boost::bind(&Renderer::draw, boost::ref(renderer_), _1));
	window_->addResizeListener(boost::bind(&Renderer::resize, boost::ref(renderer_), _1, _2));
}

bool Controller::run()
{
	return window_->run(Hookah::Window::EVENT_HANDLING_NONBLOCKING);
}

bool Controller::exec(const v3D::CommandInfo & command, const std::string & param)
{
	if (command.scope() != "voxel")
	{
		return false;
	}

	if (command.name() == "moveForward")
	{
		scene_->player().move(Player::MOVE_FORWARD);
	}
	else if (command.name() == "moveBackward")
	{
		scene_->player().move(Player::MOVE_BACKWARD);
	}
	else if (command.name() == "moveLeft")
	{
		scene_->player().move(Player::MOVE_LEFT);
	}
	else if (command.name() == "moveRight")
	{
		scene_->player().move(Player::MOVE_RIGHT);
	}
	else if (command.name() == "moveUp")
	{
		scene_->player().move(Player::MOVE_UP);
	}
	else if (command.name() == "moveDown")
	{
		scene_->player().move(Player::MOVE_DOWN);
	}

	return false;
}

bool Controller::execUI(const v3D::CommandInfo & command, const std::string & param)
{
	if (command.scope() != "ui")
	{
		return false;
	}

	if (command.name() == "showGameMenu")
	{
		window_->shutdown();
	}

	return false;
}

void Controller::motion(unsigned int x, unsigned int y)
{
	v3D::Vector2ui position(x, y);

	if (x > last_[0])
	{
		scene_->player().look(Player::LOOK_RIGHT);
	}
	else if (x < last_[0])
	{
		scene_->player().look(Player::LOOK_LEFT);
	}

	if (y > last_[1])
	{
		scene_->player().look(Player::LOOK_UP);
	}
	else if (y < last_[1])
	{
		scene_->player().look(Player::LOOK_DOWN);
	}

	last_ = position;
}

void Controller::buttonPressed(unsigned int button)
{
}

void Controller::buttonReleased(unsigned int button)
{
}


