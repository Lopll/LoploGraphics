#pragma once

#include <windows.h>
#include <unordered_set>
#include <SimpleMath.h>  // для Vector2
#include "Delegates.h"
#include "Keys.h"
#include "Exports.h"

class Game;

class GAMEFRAMEWORK_API InputDevice
{
    friend class Game;
    friend class DisplayWin32;
    
    Game* game;
    std::unordered_set<Keys>* keys;

public:
    struct MouseMoveEventArgs
    {
        DirectX::SimpleMath::Vector2 Position;
        DirectX::SimpleMath::Vector2 Offset;
        int WheelDelta;
    };

    DirectX::SimpleMath::Vector2 MousePosition;
    DirectX::SimpleMath::Vector2 MouseOffset;
    int MouseWheelDelta;

    MulticastDelegate<const MouseMoveEventArgs&> MouseMove;
    
public:
    InputDevice(Game* inGame);
    ~InputDevice();

    void AddPressedKey(Keys key);
    void RemovePressedKey(Keys key);
    bool IsKeyDown(Keys key);

protected:    

    struct KeyboardInputEventArgs {
        USHORT MakeCode;
        USHORT Flags;
        USHORT VKey;
        UINT   Message;
    };
    enum class MouseButtonFlags
    {
        LeftButtonDown = 1,
        LeftButtonUp = 2,
        RightButtonDown = 4,
        RightButtonUp = 8,
        MiddleButtonDown = 16,
        MiddleButtonUp = 32,
        Button1Down = LeftButtonDown,
        Button1Up = LeftButtonUp,
        Button2Down = RightButtonDown,
        Button2Up = RightButtonUp,
        Button3Down = MiddleButtonDown,
        Button3Up = MiddleButtonUp,
        Button4Down = 64,
        Button4Up = 128,
        Button5Down = 256,
        Button5Up = 512,
        MouseWheel = 1024,
        Hwheel = 2048,
        None = 0,
    };

    struct RawMouseEventArgs
    {
        int Mode;
        int ButtonFlags;
        int ExtraInformation;
        int Buttons;
        int WheelDelta;
        int X;
        int Y;
    };

    void OnKeyDown(KeyboardInputEventArgs args);
    void OnMouseMove(RawMouseEventArgs args);
};