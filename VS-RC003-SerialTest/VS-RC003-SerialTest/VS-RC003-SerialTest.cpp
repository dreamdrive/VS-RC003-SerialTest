////////////////////////////////////////////////////////////////
//	・VS-RC003/HVによるシリアル通信プログラムサンプル	'SerialSample.cpp'
//		(c)Vstone
//
//		本ソースはMicrosoft Visual C++（VC6.0以降）用です。
//		それ以外の開発環境をご利用の場合は、通信処理部分等をお使いの環境に応じて書き換えてください。
//
//		また、'TARGET_PORT'で定義されたシリアルポート番号を、お使いのPCの環境に応じて書き換えてください。
//
////////////////////////////////////////////////////////////////
#include "stdafx.h"
#include	<stdio.h>
#include	<stdlib.h>
#include	<windows.h>
#include	<tchar.h>
#include <time.h>

//通信ポート番号の設定。PCの環境に合わせて、最後の数値を書き換える
#define	TARGET_PORT	_T("\\\\.\\COM9")
//送受信バッファのサイズ
#define	BUFF_SZ	(256)

int sendmessage(HANDLE hCom, char *wbuf, char *rbuf);
short get_memmap(HANDLE hCom, unsigned char map_add);
HANDLE make_handle();


////////////////////////////////////////////////////////////////
//	main関数
////////////////////////////////////////////////////////////////

int main()
{
	HANDLE hCom = make_handle();
	char rbuf[BUFF_SZ], wbuf[BUFF_SZ];
	short out;
	int i;

	BOOL exit = FALSE;
	clock_t syori, syori_old;

	//syori = clock();

	//通信ハンドルの取得・設定に失敗していたらプログラムを終了する
	if (hCom == INVALID_HANDLE_VALUE) return 0;

	//VS-RC003の制御をコントローラからシリアル通信に切り替える
	printf("VS-RC003をシリアル通信より制御します。\n\n");
	sprintf_s(wbuf, "w 2009e0 00 00\r\n");

	printf("全メモリマップを取得するよー。\n");

	//メッセージの送受信。通信が成功しなかったらプログラムを終了
	if (sendmessage(hCom, wbuf, rbuf)) {
		CloseHandle(hCom);
		return 0;
	}

	for (i = 0; i < 256; i++) {
		if ((i % 8) == 0) {
			printf("\n");
		}
		out = get_memmap(hCom, i);
		printf("%03d : %04x | ", i, out & 0x0000FFFF);

	}

	//out = get_memmap(hCom, 192);
	//printf("%03d : %04d | ", 192,out);


	//VS-RC003の制御をコントローラに戻す
	printf("\nVS-RC003の制御をコントローラに戻します。\n\n");
	sprintf_s(wbuf, "w 2009e0 01 00\r\n");
	sendmessage(hCom, wbuf, rbuf);

	//取得したハンドルを閉じる
	CloseHandle(hCom);


	getchar();

	return 0;
}

////////////////////////////////////////////////////////////////
//	get_memmap関数
//	メモリマップを読みだす関数。
//	
//	・引数
//		HANDLE hCom	通信に使用するハンドル
//		get_memmap 読みだしたいメモリマップのアドレス0～255
//
//	・戻り値
//		メモリマップの値(2バイト)
////////////////////////////////////////////////////////////////

short get_memmap(HANDLE hCom, unsigned char map_add) {
	char rbuf[BUFF_SZ], wbuf[BUFF_SZ], kekka_rbuf[BUFF_SZ], outputc[4];
	short output;
	int pos_sharp = 0, i;

	//送受信メッセージバッファのクリア
	memset(wbuf, 0, sizeof(wbuf));
	memset(rbuf, 0, sizeof(rbuf));
	memset(kekka_rbuf, 0, sizeof(rbuf));

	//rコマンドのメッセージを作成
	sprintf_s(wbuf, "r 20%04X 02\r\n", (2048 + (map_add * 2)));

	//送受信！
	sendmessage(hCom, wbuf, rbuf);

	//受信結果を整形
	if (rbuf[0] == '\0') rbuf[0] = ' ';

	for (i = 0; rbuf[i] != '\0'; i++) {
		if (rbuf[i] == '#') pos_sharp = i;
	}

	for (i = 0; rbuf[i + pos_sharp] != '\0'; i++) {
		kekka_rbuf[i] = rbuf[pos_sharp + i];
	}

	outputc[0] = kekka_rbuf[11];
	outputc[1] = kekka_rbuf[12];
	outputc[2] = kekka_rbuf[8];
	outputc[3] = kekka_rbuf[9];

	//16進数を数値に変換 -> short(2byte)に格納
	output = strtol(outputc, NULL, 16);

	return output;
}


int put_memmap(HANDLE hCom, unsigned char map_add, short value) {
	char rbuf[BUFF_SZ], wbuf[BUFF_SZ], kekka_rbuf[BUFF_SZ], outputc[4];
	short output;
	int pos_sharp = 0, i;

	//送受信メッセージバッファのクリア
	memset(wbuf, 0, sizeof(wbuf));
	memset(rbuf, 0, sizeof(rbuf));
	memset(kekka_rbuf, 0, sizeof(rbuf));

	//rコマンドのメッセージを作成
	sprintf_s(wbuf, "w 20%04X 02\r\n", (2048 + (map_add * 2)));

	//送受信！
	sendmessage(hCom, wbuf, rbuf);

	//受信結果を整形
	if (rbuf[0] == '\0') rbuf[0] = ' ';

	for (i = 0; rbuf[i] != '\0'; i++) {
		if (rbuf[i] == '#') pos_sharp = i;
	}

	for (i = 0; rbuf[i + pos_sharp] != '\0'; i++) {
		kekka_rbuf[i] = rbuf[pos_sharp + i];
	}

	outputc[0] = kekka_rbuf[11];
	outputc[1] = kekka_rbuf[12];
	outputc[2] = kekka_rbuf[8];
	outputc[3] = kekka_rbuf[9];

	//16進数を数値に変換 -> short(2byte)に格納
	output = strtol(outputc, NULL, 16);

	return output;
}



////////////////////////////////////////////////////////////////
//	sendmessage関数
//	メッセージを送受信する関数。
//	
//	・引数
//		HANDLE hCom	通信に使用するハンドル
//		char *wbuf	送信メッセージバッファへのポインタ
//		char *rbuf	受信メッセージバッファへのポインタ
//
//	・戻り値
//		0	メッセージの送受信に成功
//		1	メッセージの送信に失敗
//		2	メッセージの受信に失敗
//		3	メッセージの送受信に失敗
////////////////////////////////////////////////////////////////

int sendmessage(HANDLE hCom, char *wbuf, char *rbuf)
{

	BOOL werr = FALSE, rerr = FALSE;
	DWORD wn = 0, rn = 1;
	int i = 0, cr_num = 0;

	//メッセージの送信
	if (!WriteFile(hCom, wbuf, (int)strlen(wbuf), &wn, NULL)) werr = TRUE;

	//メッセージの受信
	//メッセージの最後まで1byteずつ読み込みを行なう
	//メッセージの最後の判断は、2回目に改行('\n')が登場する場所。1回目は送信メッセージ、2回目はCPUボードが付記するもの
	//また、受信がタイムアウトを迎えた場合もループを抜ける
	while (rn) {
		if (!ReadFile(hCom, &rbuf[i], 1, &rn, NULL)) { rerr = TRUE; break; }
		else if (rbuf[i] == '\n') cr_num++;

		if (cr_num >= 2) break;
		i++;
	}

	return (werr << 1) | rerr;

}


////////////////////////////////////////////////////////////////
//	make_handle関数
//	通信ハンドルを取得する関数。
//	
//	・戻り値
//		INVALID_HANDLE_VALUE	通信ハンドルの取得・設定に失敗
//		INVALID_HANDLE_VALUE以外	取得したハンドル
////////////////////////////////////////////////////////////////

HANDLE make_handle()
{
	HANDLE hCom = INVALID_HANDLE_VALUE;
	DCB    stDcb;
	COMMTIMEOUTS cto;

	//シリアルポートのハンドルを取得
	hCom = CreateFile(TARGET_PORT, GENERIC_READ | GENERIC_WRITE, 0, NULL,
		OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL | FILE_FLAG_WRITE_THROUGH, NULL);

	//シリアルポートのハンドルの取得に失敗したら戻る
	if (hCom == INVALID_HANDLE_VALUE) return INVALID_HANDLE_VALUE;


	//シリアルポートの通信設定の作成
	//VS-RC003/HVの場合、ボーレートは115200bps、バイトサイズは8、パリティビット無し、ストップビットは1となる
	memset(&stDcb, 0, sizeof(DCB));

	stDcb.BaudRate = CBR_115200;
	stDcb.fParity = 1;
	stDcb.ByteSize = 8;
	stDcb.Parity = NOPARITY;
	stDcb.StopBits = ONESTOPBIT;
	stDcb.EofChar = 26;

	//シリアルポートの通信設定の更新
	if (SetCommState(hCom, &stDcb) == FALSE) {
		CloseHandle(hCom);
		return INVALID_HANDLE_VALUE;
	}

	//タイムアウトの設定の作成
	memset(&cto, 0, sizeof(COMMTIMEOUTS));
	cto.ReadTotalTimeoutConstant = 100;
	cto.WriteTotalTimeoutConstant = 200;

	//タイムアウトの設定の更新
	if (SetCommTimeouts(hCom, &cto) == FALSE) {
		CloseHandle(hCom);
		return INVALID_HANDLE_VALUE;
	}

	return hCom;
}