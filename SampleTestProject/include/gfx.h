#pragma once
#include <functional>
#include <memory>

namespace gfx
{
	class TextureImpl;

	/**
	 * Represents a 2D texture which can be used by a Sprite to render to screen
	 */
	class Texture
	{
	public:
		Texture();
		virtual ~Texture();
		/**
		 * Initializes a texture with the data in the image file "fileName".
		 *
		 * Important: Textures *cannot* be successfully loaded until after the Game Window has been created (callback
		 * onInit has been called)!
		 *
		 * Returns true if successful, false otherwise.
		 */
		bool LoadFromFile(const char* fileName, float scale);
		/**
		 * Returns Width of the texture, 0 if not yet initialized
		 */
		unsigned int Width() const;
		/**
		* Returns Height of the texture, 0 if not yet initialized
		*/
		unsigned int Height() const;
	private:
		friend class Window;
		const std::unique_ptr<TextureImpl> m_TextureImpl;
	};

	/**
	 * InputHandler encapsulates (very limited) keyboard input
	 */
	class InputHandler
	{
	public:
		//True if spacebar is pressed down
		bool IsSpaceDown() const;
		//True if left arrow is pressed down
		bool IsLeftDown() const;
		//True if right arrow is pressed down
		bool IsRightDown() const;
	private:
		friend class Window;
		void UpdateKeyPresses();
		bool m_SpaceDown = false;
		bool m_KeyRight = false;
		bool m_KeyLeft = false;
	};

	class IWindow
	{
	public:
		/**
		 * Creates a new Game window with title windowName
		 * Only one game window can be active at any given time
		 * Arguments:
		 *  windowName: The title of the window
		 *  width: Width of the created window
		 *  height: Height of the created window
		 *  onInit: Callback called when Window has been successfully initialized called with pointer to created window
		 *  onRender: Called when window is ready to draw. Only call Draw() methods from the onRender callback
		 *  onUpdate: Called once per frame with the delta time since last frame. Use this callback to perform game logic
		 */
		static IWindow* CreateGameWindow(
			const char* windowName,
			unsigned int width,
			unsigned int height,
			std::function<void(IWindow*)> onInit,
			std::function<void()> onRender,
			std::function<void(float)> onUpdate);
		/**
		 * Release resources associated with the game window.
		 */
		virtual void Release() = 0;
		/**
		 * Drive the update loop of the game. Must be called periodically to generate callbacks onInit, onRender, onUpdate
		 */
		virtual bool Update() = 0;
		/**
		 * Draw the given texture at coordinates x, y (0, 0 is top left corner of window).
		 */
		virtual void Draw(Texture* texture, unsigned short x, unsigned short y) = 0;
		/**
		 * Draw the given text at coordinates x, y (0, 0 is top left corner of window).
		 */
		virtual void Draw(const char* text, unsigned short size, unsigned short x, unsigned short y) = 0;
		/**
		 * Returns a pointer to the InputHandler associated with this game window.
		 */
		virtual InputHandler* GetInputHandler() = 0;
	};
}