#ifndef INPUT_STATE_H
#define INPUT_STATE_H

enum class InputState : std::uint32_t {
	left_mouse =		0b00000000000000000000000000000001,
	right_mouse =		0b00000000000000000000000000000010,

	w_key =				0b00000000000000000000000000000100,
	a_key =				0b00000000000000000000000000001000,
	s_key =				0b00000000000000000000000000010000,
	d_key =				0b00000000000000000000000000100000,

	q_key =				0b00000000000000010000000000000000,
	e_key =				0b00000000000000100000000000000000,

	space_key =			0b00000000000000000000000001000000,
	enter_key =			0b00000000000000000000000010000000,
	shift_key =			0b00000000000000000000000100000000,
	ctrl_key =			0b00000000000000000000001000000000,

	up_key =			0b00000000000000000000010000000000,
	right_key =			0b00000000000000000000100000000000,
	down_key =			0b00000000000000000001000000000000,
	left_key =			0b00000000000000000010000000000000,

	backspace_key =		0b00000000000000000100000000000000,
	esc_key =			0b00000000000000001000000000000000,

	undef_3 =			0b00000000000001000000000000000000,
	undef_4 =			0b00000000000010000000000000000000,
	undef_5 =			0b00000000000100000000000000000000,
	undef_6 =			0b00000000001000000000000000000000,
	undef_7 =			0b00000000010000000000000000000000,
	undef_8 =			0b00000000100000000000000000000000,
	undef_9 =			0b00000001000000000000000000000000,
	undef_10 =			0b00000010000000000000000000000000,
	undef_11 =			0b00000100000000000000000000000000,
	undef_12 =			0b00001000000000000000000000000000,
	undef_13 =			0b00010000000000000000000000000000,
	undef_14 =			0b00100000000000000000000000000000,
	undef_15 =			0b01000000000000000000000000000000,
	undef_16 =			0b10000000000000000000000000000000,

	null_state =		0b00000000000000000000000000000000
};

std::uint32_t operator+(InputState lhs, InputState rhs) {
	return static_cast<std::uint32_t>(lhs) | static_cast<std::uint32_t>(rhs); 
}
std::uint32_t operator-(InputState lhs, InputState rhs) {
	return static_cast<std::uint32_t>(lhs) & (~static_cast<std::uint32_t>(rhs));
}
bool operator>=(InputState lhs, InputState rhs) {
	return (static_cast<std::uint32_t>(rhs) & static_cast<std::uint32_t>(lhs)) == static_cast<std::uint32_t>(rhs);
}
std::uint32_t operator+(InputState lhs, std::uint32_t rhs) {
	return static_cast<std::uint32_t>(lhs) | rhs;
}
std::uint32_t operator-(InputState lhs, std::uint32_t rhs) {
	return static_cast<std::uint32_t>(lhs) & (~rhs);
}
bool operator>=(InputState lhs, std::uint32_t rhs) {
	return (rhs & static_cast<std::uint32_t>(lhs)) == rhs;
}
std::uint32_t operator+(std::uint32_t lhs, InputState rhs) {
	return lhs | static_cast<std::uint32_t>(rhs);
}
std::uint32_t operator-(std::uint32_t lhs, InputState rhs) {
	return lhs & (~static_cast<std::uint32_t>(rhs));
}
bool operator>=(std::uint32_t lhs, InputState rhs) {
	return (static_cast<std::uint32_t>(rhs) & lhs) == static_cast<std::uint32_t>(rhs);
}
void operator+=(InputState& lhs, InputState rhs) {
	lhs = static_cast<InputState>(static_cast<std::uint32_t>(lhs) | static_cast<std::uint32_t>(rhs));
}
void operator-=(InputState& lhs, InputState rhs) {
	lhs = static_cast<InputState>(static_cast<std::uint32_t>(lhs) & (~static_cast<std::uint32_t>(rhs)));
}
void operator+=(InputState& lhs, std::uint32_t rhs) {
	lhs = static_cast<InputState>(static_cast<std::uint32_t>(lhs) | rhs);
}
void operator-=(InputState& lhs, std::uint32_t rhs) {
	lhs = static_cast<InputState>(static_cast<std::uint32_t>(lhs) & ~(rhs));
}

constexpr uint32_t LEFT_MOUSE_BUTTON =		0b00000000000000000000000000000001;
constexpr uint32_t RIGHT_MOUSE_BUTTON =		0b00000000000000000000000000000010;

constexpr uint32_t W_KEY =					0b00000000000000000000000000000100;
constexpr uint32_t A_KEY =					0b00000000000000000000000000001000;
constexpr uint32_t S_KEY =					0b00000000000000000000000000010000;
constexpr uint32_t D_KEY =					0b00000000000000000000000000100000;

constexpr uint32_t Q_KEY =					0b00000000000000010000000000000000;
constexpr uint32_t E_KEY =					0b00000000000000100000000000000000;

constexpr uint32_t SPACE_KEY =				0b00000000000000000000000001000000;
constexpr uint32_t ENTER_KEY =				0b00000000000000000000000010000000;
constexpr uint32_t SHIFT_KEY =				0b00000000000000000000000100000000;
constexpr uint32_t CTRL_KEY =				0b00000000000000000000001000000000;

constexpr uint32_t UP_KEY =					0b00000000000000000000010000000000;
constexpr uint32_t RIGHT_KEY =				0b00000000000000000000100000000000;
constexpr uint32_t DOWN_KEY =				0b00000000000000000001000000000000;
constexpr uint32_t LEFT_KEY =				0b00000000000000000010000000000000;

constexpr uint32_t BACKSPACE_KEY =			0b00000000000000000100000000000000;
constexpr uint32_t ESC_KEY =				0b00000000000000001000000000000000;

constexpr uint32_t UNDEF_KEY_3 =			0b00000000000001000000000000000000; // UNDEFINED!
constexpr uint32_t UNDEF_KEY_4 =			0b00000000000010000000000000000000; // UNDEFINED!
constexpr uint32_t UNDEF_KEY_5 =			0b00000000000100000000000000000000; // UNDEFINED!
constexpr uint32_t UNDEF_KEY_6 =			0b00000000001000000000000000000000; // UNDEFINED!
constexpr uint32_t UNDEF_KEY_7 =			0b00000000010000000000000000000000; // UNDEFINED!
constexpr uint32_t UNDEF_KEY_8 =			0b00000000100000000000000000000000; // UNDEFINED! 
constexpr uint32_t UNDEF_KEY_9 =			0b00000001000000000000000000000000; // UNDEFINED!
constexpr uint32_t UNDEF_KEY_10 =			0b00000010000000000000000000000000; // UNDEFINED!
constexpr uint32_t UNDEF_KEY_11 =			0b00000100000000000000000000000000; // UNDEFINED!
constexpr uint32_t UNDEF_KEY_12 =			0b00001000000000000000000000000000; // UNDEFINED!
constexpr uint32_t UNDEF_KEY_13 =			0b00010000000000000000000000000000; // UNDEFINED!
constexpr uint32_t UNDEF_KEY_14 =			0b00100000000000000000000000000000; // UNDEFINED!
constexpr uint32_t UNDEF_KEY_15 =			0b01000000000000000000000000000000; // UNDEFINED!
constexpr uint32_t UNDEF_KEY_16 =			0b10000000000000000000000000000000;	// UNDEFINED!

constexpr uint32_t NULL_INPUT_STATE =		0b00000000000000000000000000000000;

#endif