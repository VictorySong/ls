#pragma once

extern "C" {
	_declspec(dllexport) bool in();
	_declspec(dllexport) bool searchwlan();
	_declspec(dllexport) bool isconnected();
	_declspec(dllexport) bool connectwlan();
	_declspec(dllexport) void wlanfree();
	_declspec(dllexport) bool setwlanInterfaceson();
	_declspec(dllexport) bool setwlanInterfacesoff();
	_declspec(dllexport) void changetarget(char * tar, char * tarkey);
	_declspec(dllexport) bool getwlanInterfaces();
	_declspec(dllexport) bool openhandle();
}
