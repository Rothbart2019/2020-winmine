#include <stdio.h>
#include <windows.h>

int main() {
	DWORD Pid = 0;
	HANDLE hProcess = 0;

	// ����ʱ��������ָ���ַ
	DWORD incAddress1 = 0x01002FF5;
	DWORD incAddress2 = 0x01003830;
	// ��ʱ������NOP����6�ֽ�ָ��
	char inc[6] = { 0x90, 0x90, 0x90, 0x90, 0x90, 0x90 };
	DWORD result1, result2;

	PBYTE pByte = NULL;
	DWORD dwHeight = 0, dwWidth = 0;

	// ��ȡɨ����Ϸ��Ӧ�Ĵ��ھ��
	HWND hWnd = FindWindow(NULL, L"ɨ��");
	if (hWnd != 0) {
		// ��ȡɨ�׽���ID
		GetWindowThreadProcessId(hWnd, &Pid);
		// ��ɨ����Ϸ��ȡ����
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, Pid);
		if (hProcess == 0) {
			printf("Open winmine process failed.");
			return 0;
		}

		// NOP��ʱ�����������
		result1 = WriteProcessMemory(hProcess, (LPVOID)incAddress1, &inc, 6, 0);
		result2 = WriteProcessMemory(hProcess, (LPVOID)incAddress2, &inc, 6, 0);
		if (result1 == 0 || result2 == 0) {
			printf("Inc modify failed.");
			return 0;
		}
		else {
			printf("(*^��^*)");
		}

		// �����������ʼ��ַ
		DWORD dwBoomAddr = 0x01005340;
		// ���������ֵ�������߽磩
		DWORD dwSize = 832;
		pByte = (PBYTE)malloc(dwSize);
		DWORD dwTmpAddr = 0;

		// ��ȡ��������������
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

		// ˢ��ɨ�׵Ŀͻ���
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



//201811123019 18��Ϣ��ȫ ��Ӯ