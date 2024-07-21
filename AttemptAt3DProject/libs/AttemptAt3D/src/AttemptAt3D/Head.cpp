
#include "AttemptAt3D/Head.hpp"

#include <memory>
#include <cstring>
#include <chrono>
#include <cstdlib>
#include "AttemptAt3D/(Debug)/Debug.hpp"
#include "AttemptAt3D/(Form)/Form.hpp"
#include "AttemptAt3D/(Tran)/Tran.hpp"
#include "AttemptAt3D/(Mesh)/Mesh.hpp"
#include "AttemptAt3D/(Mesh)/MeshSamples/InvertedPyramid.hpp"
#include "AttemptAt3D/(Mesh)/MeshSamples/Cube.hpp"

namespace AttemptAt3D
{
	// TODO: delete
	static std::unique_ptr<Form> form1;
	static std::unique_ptr<Form> form2;
	static std::unique_ptr<Form> form3;

	/* Constructors */

	Head::Head() :
		windowForGlfw(nullptr),
		_fov(glm::radians(45.0f)),
		_aspectRatio(1.0f),
		_nearClippingPlane(0.1f),
		_farClippingPlane(100000.0f),
		shaderManager(),
		bodyManager()
	{}

	void Head::set_fov(float val)
	{
		this->_fov = val;
		this->_updateProjectionMatrix();
	}

	void Head::set_aspectRatio(float val)
	{
		this->_aspectRatio = val;
		this->_updateProjectionMatrix();
	}

	void Head::set_nearClippingPlane(float val)
	{
		this->_nearClippingPlane = val;
		this->_updateProjectionMatrix();
	}

	void Head::set_farClippingPlane(float val)
	{
		this->_farClippingPlane = val;
		this->_updateProjectionMatrix();
	}
	
	/* Methods */

	void Head::start(int windowWidth, int windowHeight, const std::string windowTitle)
	{
		/* Create the window, initializing GLFW and GLEW */

		// Initializes this->windowForGlfw
		{
			if (!glfwInit())
			{ Debug::LogFatalError("glfwInit failed."); }

			// Window hints
			glfwWindowHint(GLFW_SAMPLES, 7); // anti-aliasing

			this->windowForGlfw = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
			if (!this->windowForGlfw)
			{
				glfwTerminate();
				Debug::LogFatalError("glfwCreateWindow failed.");
			}

			glfwMakeContextCurrent(this->windowForGlfw);

			glewExperimental = GL_TRUE;
			glewInit();
		}

		/* OpenGL Settings */

		glEnable(GL_DEPTH_TEST);
		glEnable(GL_CULL_FACE);
		glCullFace(GL_BACK);
		glfwSetWindowSizeCallback(this->windowForGlfw, Head::onWindowResize);

		/* Activate Shaders */

		this->shaderManager.compileAndActivateShaders();

		/* Miscellaneous Pre-Main-Loop Tasks */

		glfwSetWindowUserPointer(this->windowForGlfw, this); // passes a pointer to this Head to OpenGL
		this->_updateProjectionMatrix();

		/* TEMP */

		{
			form1 = std::make_unique<Form>(this->shaderManager, this->bodyManager, Tran(), MeshSamples::Cube().make());
			form1->tran.acq_scale().x = 0.6f;
			form1->tran.acq_scale().y = 0.6f;
			form1->tran.acq_scale().z = 2.0f;

			form2 = std::make_unique<Form>(this->shaderManager, this->bodyManager, Tran(), MeshSamples::Cube().make());
			form2->tran.acq_position().z += 2.0f;
		}

		/* Main Loop */

		this->mainLoop();
	}

	void Head::mainLoop()
	{
		while (!glfwWindowShouldClose(this->windowForGlfw))
		{
			/* Calculate Delta Time */

			float deltaTime = Head::CalculateDeltaTime();

			/* Process inputs */

			/* Render everything */

			glClearColor(0.1f, 0.0f, 0.25f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			this->bodyManager.drawAllBodies(this->shaderManager);

			/* Stuff required by GLFW */

			glfwPollEvents();
			glfwSwapBuffers(this->windowForGlfw);

			/* TEMP */

			{
				// form1->trans.rot.set_z(0.4f);
				// form1->trans.scale.set_z(0.5f);
				// form1->trans.pos.set_x(1.5f);

				// form1->trans.rot.set_z(form1->trans.rot.get_z() + 0.0006f * deltaTime);
				// form2->trans.rot.set_z(form2->trans.rot.get_z() - 0.0015f * deltaTime);
				form1->tran.acq_rotation() *= glm::angleAxis(0.0006f * deltaTime, Vec3s::Up);
				form2->tran.acq_rotation() *= glm::angleAxis(0.0035f * deltaTime, Vec3s::Up);
			}
		}

		this->endGlfw();
	}

	void Head::endGlfw()
	{
		this->shaderManager.cleanupForGl();
		this->bodyManager.cleanupForGl();
		glfwTerminate();
	}

	void Head::_updateProjectionMatrix()
	{
		this->shaderManager.set_uni_projVal(
			glm::perspective(this->_fov, this->_aspectRatio, this->_nearClippingPlane, this->_farClippingPlane)
		);
	}

	/* Functions */

	float Head::CalculateDeltaTime()
	{
		using std::chrono::steady_clock;
		using std::chrono::duration;
		using durMillisecs = duration<float, std::ratio<1, 1000>>;

		static steady_clock::time_point timePoint = steady_clock::now();
		steady_clock::time_point oldTimePoint = timePoint;
		timePoint = steady_clock::now();

		durMillisecs timeElapsed = std::chrono::duration_cast<durMillisecs>(timePoint - oldTimePoint);
		return timeElapsed.count();
	}

	/* Methods for External Use */

	void Head::onWindowResize(GLFWwindow* windowForGlfw, int width, int height)
	{
		Head* self = (Head*)glfwGetWindowUserPointer(windowForGlfw);

		glViewport(0, 0, width, height);

		self->set_aspectRatio((float)width / (float)height);
	}
}
