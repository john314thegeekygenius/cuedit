
namespace CU {


/*

// Special
INFO:Key pressed 27
INFO:Key pressed 91
// Left
INFO:Key pressed 68
// Special
INFO:Key pressed 27
INFO:Key pressed 91
// Special 2
INFO:Key pressed 49
INFO:Key pressed 59
// CTRL
INFO:Key pressed 53
// Left
INFO:Key pressed 68


// Special
INFO:Key pressed 27
INFO:Key pressed 91
// Special 2
INFO:Key pressed 49
INFO:Key pressed 59
// SHIFT
INFO:Key pressed 50
// Left
INFO:Key pressed 68

// Special
INFO:Key pressed 27
INFO:Key pressed 91
// Special 2
INFO:Key pressed 49
INFO:Key pressed 59
// ALT
INFO:Key pressed 51
// Left
INFO:Key pressed 68

// Function Special
INFO:Key pressed 27
INFO:Key pressed 79
// F2
INFO:Key pressed 81

// Function Special
INFO:Key pressed 27
INFO:Key pressed 79
// F4
INFO:Key pressed 83

//// Special Keys
// Up
INFO:Key pressed 65
// Down
INFO:Key pressed 66
// Left
INFO:Key pressed 68
// Right
INFO:Key pressed 67

// Home
INFO:Key pressed 72
// Page up
INFO:Key pressed 53
INFO:Key pressed 126
// End
INFO:Key pressed 70
// Page down
INFO:Key pressed 54
INFO:Key pressed 126
// Insert
INFO:Key pressed 50
INFO:Key pressed 126
*/

enum class keyCode {
	k_null = 0,
	k_bell = 7, // ????
	k_backspace = 8,
	k_tab = 9,
	k_return = 10, k_enter = 10, // Same keycode, different name
	k_vertical_tab = 11, // ????
	k_creturn = 13, // ????
	k_escape = 27,
	k_space = 32,
	k_exclamation = 33, bang = 33,
	k_str_quote = 34,
	k_hash = 35, pound = 35,
	k_dollar = 36,
	k_percent = 37,
	k_and = 38,
	k_sing_quote = 39,
	k_l_perenc = 40,
	k_r_perenc = 41,
	k_times = 42,
	k_plus = 43,
	k_comma = 44,
	k_minux = 45,
	k_period = 46,
	k_f_slash = 47,

	n_zero = 48,
	n_one = 49,
	n_two = 50,
	n_three = 51,
	n_four = 52,
	n_five = 53,
	n_six = 54,
	n_seven = 55,
	n_eight = 56,
	n_nine = 57,

	k_colon = 58,
	k_semi_colon = 59,
	k_l_a_bracket = 60,
	k_equals = 61,
	k_r_a_bracket = 62,
	k_question_mark = 63,
	k_at = 64,

    l_cap_a = 65,
    l_cap_b = 66,
    l_cap_c = 67,
    l_cap_d = 68,
    l_cap_e = 69,
    l_cap_f = 70,
    l_cap_g = 71,
    l_cap_h = 72,
    l_cap_i = 73,
    l_cap_j = 74,
    l_cap_k = 75,
    l_cap_l = 76,
    l_cap_m = 77,
    l_cap_n = 78,
    l_cap_o = 79,
    l_cap_p = 80,
    l_cap_q = 81,
    l_cap_r = 82,
    l_cap_s = 83,
    l_cap_t = 84,
    l_cap_u = 85,
    l_cap_v = 86,
    l_cap_w = 87,
    l_cap_x = 88,
    l_cap_y = 89,
    l_cap_z = 90,

    k_l_bracket = 91,
    k_b_slash = 92,
    k_r_bracket = 93,
    k_carrot = 94,
    k_underscore = 95,
    k_grave = 96, k_quote_mark = 96,

    l_a = 97,
    l_b = 98,
    l_c = 99,
    l_d = 100,
    l_e = 101,
    l_f = 102,
    l_g = 103,
    l_h = 104,
    l_i = 105,
    l_j = 106,
    l_k = 107,
    l_l = 108,
    l_m = 109,
    l_n = 110,
    l_o = 111,
    l_p = 112,
    l_q = 113,
    l_r = 114,
    l_s = 115,
    l_t = 116,
    l_u = 117,
    l_v = 118,
    l_w = 119,
    l_x = 120,
    l_y = 121,
    l_z = 122,

    k_l_c_bracket = 123,
    k_bar = 124,
    k_r_c_bracket = 125,
    k_tilde = 126,
    k_delete = 127,

	// special keys
	s_up = 0x100,
	s_down = 0x101,
	s_left = 0x102,
	s_right = 0x103,

	// control masks
	c_ctrl = 0x200,
	c_alt = 0x400,
	c_shift = 0x800,

    // Function keys
    f_1 = 80,
    f_2 = 81,
    f_3 = 82,
    f_4 = 83,
    f_5 = 84,
    f_6 = 95,

};

};
