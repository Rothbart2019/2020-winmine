#include <stdio.h>
#include <windows.h>

int main() {
	DWORD Pid = 0;
	HANDLE hProcess = 0;

	// 控制时间自增的指令地址
	DWORD incAddress1 = 0x01002FF5;
	DWORD incAddress2 = 0x01003830;
	// 将时间自增NOP掉的6字节指令
	char inc[6] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	DWORD result1, result2;

	PBYTE pByte = NULL;
	DWORD dwHeight = 0, dwWidth = 0;

	// 获取扫雷游戏对应的窗口句柄
	HWND hWnd = FindWindow(NULL, L"扫雷");
	if (hWnd != 0) {
		// 获取扫雷进程ID
		GetWindowThreadProcessId(hWnd, &Pid);
		// 打开扫雷游戏获取其句柄
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid);
		if (hProcess == 0) {
			printf("Open winmine process failed.");
			return 0;
		}

		// NOP掉时间自增的语句
		result1 = WriteProcessMemory(hProcess, (LPVOID)incAddress1, &inc, 6, 0);
		result2 = WriteProcessMemory(hProcess, (LPVOID)incAddress2, &inc, 6, 0);
		if (result1 == 0 || result2 == 0) {
			printf("Inc modify failed.");
			return 0;
		}
		else {
			printf("(*^▽^*)");
		}

		// 存放雷区的起始地址
		DWORD dwBoomAddr = 0x01005340;
		// 雷区的最大值（包含边界）
		DWORD dwSize = 832;
		pByte = (PBYTE)malloc(dwSize);
		DWORD dwTmpAddr = 0;

		// 读取整个雷区的数据
		ReadProcessMemory(hProcess, (LPVOID)dwBoomAddr, pByte, dwSize, 0);
		BYTE bClear = 0x8E;
		int i = 0;
		int n = dwSize;

		while (i < dwSize) {
			if (pByte[i] == 0x8F) {
				dwTmpAddr = 0x01005340 + i;
				WriteProcessMemory(hProcess, (LPVOID)dwTmpAddr, &bClear, sizeof(BYTE), 0);
				n--;
			}
			i++;
		}

		// 刷新扫雷的客户区
		RECT rt;
		GetClientRect(hWnd, &rt);
		InvalidateRect(hWnd, &rt, TRUE);

		free(pByte);
		CloseHandle(hProcess);
	}
	else {
		printf("Get hWnd failed.");
	}
	return 0;
}



//201811123019 18信息安全 黄赢