#pragma once
#ifndef wlan
#define wlan
extern "C" {                //exetern外来的
	__declspec(dllexport) bool in();
	__declspec(dllexport) bool searchwlan();
	__declspec(dllexport) bool isconnected();
	__declspec(dllexport) bool connectwlan();
	__declspec(dllexport) void wlanfree();
	__declspec(dllexport) bool setwlanInterfaceson();
	__declspec(dllexport) bool setwlanInterfacesoff();
	__declspec(dllexport) void changetarget(char * tar, char * tarkey);
	__declspec(dllexport) bool getwlanInterfaces();
	__declspec(dllexport) bool openhandle();

	/*bool in();
	bool searchwlan();
	bool isconnected();
	bool connectwlan();
	void wlanfree();
	bool setwlanInterfaceson();
	bool setwlanInterfacesoff();
	void changetarget(char * tar, char * tarkey);
	bool getwlanInterfaces();
	bool openhandle();*/
}
#endif // wlan


