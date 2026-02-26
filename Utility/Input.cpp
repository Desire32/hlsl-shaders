module input;

import <array>;

static std::array<KeyState, static_cast<int>(KeyCode::NumKeyCodes)> gKeyStates;

// mouse pos
static int gMouseX = 0;
static int gMouseY = 0;

void InitInput()
{
    gKeyStates.fill(KeyState::NotPressed);
    gMouseX = gMouseY = 0;
}

void KeyDownEvent(KeyCode key)
{
    auto& state = gKeyStates[static_cast<int>(key)];
    state = (state == KeyState::NotPressed) ? KeyState::Pressed : KeyState::Held;
}

void KeyUpEvent(KeyCode key)
{
    gKeyStates[static_cast<int>(key)] = KeyState::NotPressed;
}

void MouseMoveEvent(int x, int y)
{
    gMouseX = x;
    gMouseY = y;
}

bool KeyHit(KeyCode keyCode)
{
    auto& state = gKeyStates[static_cast<int>(keyCode)];
    if (state == KeyState::Pressed)
    {
        state = KeyState::Held;
        return true;
    }
    return false;
}

bool KeyHeld(KeyCode keyCode)
{
    auto& state = gKeyStates[static_cast<int>(keyCode)];
    if (state == KeyState::NotPressed)
        return false;

    state = KeyState::Held;
    return true;
}

int GetMouseX() { return gMouseX; }
int GetMouseY() { return gMouseY; }
