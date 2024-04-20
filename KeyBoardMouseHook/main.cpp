#include <iostream>
#include <windows.h>
#include <nlohmann/json.hpp>
#include <fstream>
#include <codecvt>

using namespace std;

using json = nlohmann::json;
map<std::string, std::string> keyMap;

string findAndDisplayMapping(const std::string& searchKey) {
	auto it = keyMap.find(searchKey);
	if (it != keyMap.end()) {
		return it->second;
	}
	else {
		return "False";
	}
}

void loadKeyMappings(const std::string& filename) {
	std::ifstream file(filename);
	if (!file.is_open()) {
		std::cerr << "Failed to open the file: " << filename << std::endl;
		return;
	}

	json j;
	file >> j;

	// 将JSON对象的每个键值对添加到map中
	for (auto& element : j.items()) {
		std::string key = element.key();  // 完整的字符串键
		std::string value = element.value().get<std::string>();  // 完整的字符串值
		keyMap[key] = value;  // 添加到map中
	}

	// 输出加载的键映射
	std::cout << "Loaded Key Mappings:" << std::endl;
	for (const auto& pair : keyMap) {
		std::cout << pair.first << " -> " << pair.second << std::endl;
	}

	// 查找特定的key
	findAndDisplayMapping("Shift");
}

string TCHARToString(const TCHAR* tcharArray) {
	wstring_convert<codecvt_utf8_utf16<wchar_t>> converter;
	return converter.to_bytes(tcharArray);
}

void SendShift(bool isDown) {
	INPUT input = { 0 };  // 初始化并清零结构
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = VK_SHIFT;
	input.ki.wScan = 0;
	input.ki.dwFlags = isDown ? 0 : KEYEVENTF_KEYUP;
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;

	SendInput(1, &input, sizeof(INPUT));
}


UINT GetKeyCode(const std::string& keyName) {
	static const std::map<std::string, UINT> keyMap = {
		// Alphabet keys
		{"A", 0x41}, {"B", 0x42}, {"C", 0x43}, {"D", 0x44}, {"E", 0x45},
		{"F", 0x46}, {"G", 0x47}, {"H", 0x48}, {"I", 0x49}, {"J", 0x4A},
		{"K", 0x4B}, {"L", 0x4C}, {"M", 0x4D}, {"N", 0x4E}, {"O", 0x4F},
		{"P", 0x50}, {"Q", 0x51}, {"R", 0x52}, {"S", 0x53}, {"T", 0x54},
		{"U", 0x55}, {"V", 0x56}, {"W", 0x57}, {"X", 0x58}, {"Y", 0x59},
		{"Z", 0x5A},

		// Number keys
		{"0", 0x30}, {"1", 0x31}, {"2", 0x32}, {"3", 0x33}, {"4", 0x34},
		{"5", 0x35}, {"6", 0x36}, {"7", 0x37}, {"8", 0x38}, {"9", 0x39},

		// Control keys
		{"Shift", VK_SHIFT},
		{ "Ctrl", VK_CONTROL }, {"Right Ctrl", VK_RCONTROL}, 
		{"Alt", VK_MENU}, {"Right Alt", VK_RMENU},
		{"Space", VK_SPACE}, {"Esc", VK_ESCAPE},
		{"Enter", VK_RETURN},
		{"Backspace", VK_BACK}, {"Tab", VK_TAB}, {"Caps Lock", VK_CAPITAL},

		// Function keys
		{"F1", VK_F1}, {"F2", VK_F2}, {"F3", VK_F3}, {"F4", VK_F4},
		{"F5", VK_F5}, {"F6", VK_F6}, {"F7", VK_F7}, {"F8", VK_F8},
		{"F9", VK_F9}, {"F10", VK_F10}, {"F11", VK_F11}, {"F12", VK_F12},

		// Arrow and navigation keys
		{"Left", VK_LEFT}, {"Right", VK_RIGHT},
		{"Up", VK_UP}, {"Down", VK_DOWN},
		{"Page Up", VK_PRIOR}, {"Page Down", VK_NEXT},
		{"Home", VK_HOME}, {"End", VK_END},
		{"Insert", VK_INSERT}, {"Delete", VK_DELETE},

		// Numeric keypad
		{"Num Lock", VK_NUMLOCK},
		{"Num 0", VK_NUMPAD0}, {"Num 1", VK_NUMPAD1},
		{"Num 2", VK_NUMPAD2}, {"Num 3", VK_NUMPAD3}, {"Num 4", VK_NUMPAD4},
		{"Num 5", VK_NUMPAD5}, {"Num 6", VK_NUMPAD6}, {"Num 7", VK_NUMPAD7},
		{"Num 8", VK_NUMPAD8}, {"Num 9", VK_NUMPAD9},
		{"Num Enter", VK_RETURN},
		{"Num /", VK_DIVIDE},   // Division
		{"Num *", VK_MULTIPLY}, // Multiplication
		{"Num -", VK_SUBTRACT}, // Subtraction
		{"Num +", VK_ADD},      // Addition
		{"Num Del", VK_DECIMAL},   // Decimal point

		// Special keys 不能支持这些按键 不然会跟特殊功能冲突
		// {"ScrollLock", VK_SCROLL}, {"Pause", VK_PAUSE}, {"PrintScreen", VK_SNAPSHOT}
	};

	auto it = keyMap.find(keyName);
	if (it != keyMap.end()) {
		return it->second;
	}
	else {
		return 0;  // Key not found
	}
}

void SendKey(UINT keyCode, bool keyDown) {
	INPUT input = { 0 };
	input.type = INPUT_KEYBOARD;
	input.ki.wVk = keyCode;
	input.ki.dwFlags = (keyDown ? 0 : KEYEVENTF_KEYUP);
	input.ki.time = 0;
	input.ki.dwExtraInfo = 0;
	SendInput(1, &input, sizeof(INPUT));
}


LRESULT CALLBACK KeyboardProc(int nCode, WPARAM wParam, LPARAM lParam) {
	if (nCode >= 0) {
		PKBDLLHOOKSTRUCT p = (PKBDLLHOOKSTRUCT)lParam;
		TCHAR keyName[128];
		DWORD msg = 1;
		msg += p->scanCode << 16;
		msg += p->flags << 24;
		int keyNameLength = GetKeyNameText(msg, keyName, 128);
		string StrKeyName = TCHARToString(keyName);
		if (keyNameLength > 0) {
			if (wParam == WM_KEYDOWN || wParam == WM_SYSKEYDOWN) {
				if (GetKeyCode(StrKeyName) != 0) {
					cout << "support>>True" << " KeyCode>>" << GetKeyCode(StrKeyName) << " KeyNameStr>>" << StrKeyName << endl;
				}
				else {
					cout << "不受支持" << StrKeyName << endl;
				}
				UINT keyCode = GetKeyCode(findAndDisplayMapping(StrKeyName));
				if (keyCode) {
					SendKey(keyCode, true);
					return 1;
				}
			}
			else if (wParam == WM_KEYUP || wParam == WM_SYSKEYUP) {
				UINT keyCode = GetKeyCode(findAndDisplayMapping(StrKeyName));
				if (keyCode) {
					SendKey(keyCode, false);
					return 1;
				}

			}
		}



	}
	return CallNextHookEx(NULL, nCode, wParam, lParam);
}

int main() {
	loadKeyMappings("map.json");
	HHOOK hKeyboardHook = SetWindowsHookEx(WH_KEYBOARD_LL, KeyboardProc, NULL, 0);
	if (hKeyboardHook == NULL) {
		cerr << "Failed to install hook!" << endl;
		return 1;
	}

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0) > 0) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	UnhookWindowsHookEx(hKeyboardHook);
	return 0;
}