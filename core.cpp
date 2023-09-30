#include "core.h"

namespace GeoFrame {
	bool GLFW_INITIALIZED = false;
	bool GLAD_INITIALIZED = false;

	void Init() {
		int state = glfwInit();
		if (state != GLFW_TRUE) {
			throw FatalError("Dependence library [glfw] was failed to initialize.");
		}
		else { ; }
	}

	void InitGLAD() {
		if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
			throw FatalError("Dependence library [glad] was failed to initialize.");
		}
		else { ; }
	}

	std::string Clipboard() { return std::string(glfwGetClipboardString(NULL)); }

	void Clipboard(std::string text) { glfwSetClipboardString(NULL, text.c_str()); }


	Mods::Mods(int mods) {
		mShift = mods & GLFW_MOD_SHIFT;
		mControl = mods & GLFW_MOD_CONTROL;
		mAlt = mods & GLFW_MOD_ALT;
		mCapsLock = mods & GLFW_MOD_CAPS_LOCK;
		mNumLock = mods & GLFW_MOD_NUM_LOCK;
	}

	Key::Key(int key, int scancode, int action, int mods) : mKey(key), mScancode(scancode), mAction(action), mMods(mods) { ; }
}