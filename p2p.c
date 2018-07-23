#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <errno.h>
#include <ncurses.h>
#include <locale.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <curses.h>
#include <form.h>
#include <menu.h>
#include <panel.h>
#include <ctype.h>
#include <pthread.h>
#include "memory.h"
#include "time.h"
#define PLAIN_FILE_OPEN_ERROR -1
#define KEY_FILE_OPEN_ERROR -2
#define CIPHER_FILE_OPEN_ERROR -3
typedef char ElemType;
/*the key :581fbc94d3a452ea*/
FILE *ioutfileserver;
FILE *ioutfileclient;
FILE *open1;
FILE *open2;
FILE *open3;
FILE *open4;
int password = 0;/*µÇÈëÃÜÂëµÄ±êÖ¾*/
int wrong_time=1; /*ÃÜÂë´íÎóµÄ±êÖ¾*/
WINDOW *begin_win=NULL;   
WINDOW *menu_win1=NULL; 
WINDOW *pad=NULL;

int iport = 4600;
int rool=0;
int rool_page=0;

//³õÊ¼ÖÃ»»±íIP
int IP_Table[64] = {  57,49,41,33,25,17,9,1,
                                 59,51,43,35,27,19,11,3,
                                 61,53,45,37,29,21,13,5,
                                 63,55,47,39,31,23,15,7,
                                 56,48,40,32,24,16,8,0,
                                 58,50,42,34,26,18,10,2,
                                 60,52,44,36,28,20,12,4,
                                 62,54,46,38,30,22,14,6}; 
//Äæ³õÊ¼ÖÃ»»±íIP^-1
int IP_1_Table[64] = {39,7,47,15,55,23,63,31,
		   38,6,46,14,54,22,62,30,
		   37,5,45,13,53,21,61,29,
		   36,4,44,12,52,20,60,28,
		   35,3,43,11,51,19,59,27,
		   34,2,42,10,50,18,58,26,
		   33,1,41,9,49,17,57,25,
		   32,0,40,8,48,16,56,24};

//À©³äÖÃ»»±íE
int E_Table[48] = {31, 0, 1, 2, 3, 4,
	              3,  4, 5, 6, 7, 8,
	              7,  8,9,10,11,12,
	              11,12,13,14,15,16,
	              15,16,17,18,19,20,
	              19,20,21,22,23,24,
	              23,24,25,26,27,28,
	              27,28,29,30,31, 0};

//ÖÃ»»º¯ÊýP
int P_Table[32] = {15,6,19,20,28,11,27,16,
				  0,14,22,25,4,17,30,9,
				  1,7,23,13,31,26,2,8,
				  18,12,29,5,21,10,3,24};

//SºÐ
int S[8][4][16] =//S1
 	         {{{14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7},
   	         {0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8},
  	          {4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0},
  	          {15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13}},
  	          //S2
 	           {{15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10},
 	           {3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5},
 	           {0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15},
 	           {13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9}},
 	           //S3
 	           {{10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8},
 	           {13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1},
   	           {13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7},
 	           {1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12}},
 	           //S4
 	           {{7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15},
 	           {13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9},
 	           {10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4},
 	           {3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14}},
 	           //S5
 	           {{2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9},
 	           {14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6},
 	           {4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14},
 	           {11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3}},
 	           //S6
 	           {{12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11},
 	           {10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8},
 	           {9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6},
 	           {4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13}},
 	           //S7
 	           {{4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1},
 	           {13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6},
 	           {1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2},
 	           {6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12}},
 	           //S8
 	           {{13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7},
 	           {1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2},
 	           {7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8},
 	           {2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11}}};
//ÖÃ»»Ñ¡Ôñ1
int PC_1[56] = {56,48,40,32,24,16,8,
	          0,57,49,41,33,25,17,
	          9,1,58,50,42,34,26,
	          18,10,2,59,51,43,35,
	          62,54,46,38,30,22,14,
	          6,61,53,45,37,29,21,
	          13,5,60,52,44,36,28,
	          20,12,4,27,19,11,3};

//ÖÃ»»Ñ¡Ôñ2
int PC_2[48] = {13,16,10,23,0,4,2,27,
	          14,5,20,9,22,18,11,3,
	          25,7,15,6,26,19,12,1,
	          40,51,30,36,46,54,29,39,
	          50,44,32,46,43,48,38,55,
	          33,52,45,41,49,35,28,31};

//¶Ô×óÒÆ´ÎÊýµÄ¹æ¶¨
int MOVE_TIMES[16] = {1,1,2,2,2,2,2,2,1,2,2,2,2,2,2,1};
int ByteToBit(ElemType ch,ElemType bit[8]);
int BitToByte(ElemType bit[8],ElemType *ch);
int Char8ToBit64(ElemType ch[8],ElemType bit[64]);
int Bit64ToChar8(ElemType bit[64],ElemType ch[8]);
int DES_MakeSubKeys(ElemType key[64],ElemType subKeys[16][48]);
int DES_PC1_Transform(ElemType key[64], ElemType tempbts[56]);
int DES_PC2_Transform(ElemType key[56], ElemType tempbts[48]);
int DES_ROL(ElemType data[56], int time);
int DES_IP_Transform(ElemType data[64]);
int DES_IP_1_Transform(ElemType data[64]);
int DES_E_Transform(ElemType data[48]);
int DES_P_Transform(ElemType data[32]);
int DES_SBOX(ElemType data[48]);
int DES_XOR(ElemType R[48], ElemType L[48],int count);
int DES_Swap(ElemType left[32],ElemType right[32]);
int DES_EncryptBlock(ElemType plainBlock[8], ElemType subKeys[16][48], ElemType cipherBlock[8]);
int DES_DecryptBlock(ElemType cipherBlock[8], ElemType subKeys[16][48], ElemType plainBlock[8]);
int DES_Encrypt(char *plainFile, char *keyStr,char *cipherFile);
int DES_Decrypt(char *cipherFile, char *keyStr,char *plainFile);
//×Ö½Ú×ª»»³É¶þ½øÖÆÆ
int ByteToBit(ElemType ch, ElemType bit[8]){
	int cnt;
	for(cnt = 0;cnt < 8; cnt++){
		*(bit+cnt) = (ch>>cnt)&1;
	}
	return 0;
}

//¶þ½øÖÆ×ª»»³É×Ö½Ú
int BitToByte(ElemType bit[8],ElemType *ch){
	int cnt;
	for(cnt = 0;cnt < 8; cnt++){
		*ch |= *(bit + cnt)<<cnt;
	}
	return 0;
}

//½«³¤¶ÈÎª8µÄ×Ö·û´®×ªÎª¶þ½øÖÆÎ»´®
int Char8ToBit64(ElemType ch[8],ElemType bit[64]){
	int cnt;
	for(cnt = 0; cnt < 8; cnt++){		
		ByteToBit(*(ch+cnt),bit+(cnt<<3));
	}
	return 0;
}

//½«¶þ½øÖÆÎ»´®×ªÎª³¤¶ÈÎª8µÄ×Ö·û´®
int Bit64ToChar8(ElemType bit[64],ElemType ch[8]){
	int cnt;
	memset(ch,0,8);
	for(cnt = 0; cnt < 8; cnt++){
		BitToByte(bit+(cnt<<3),ch+cnt);
	}
	return 0;
}



//Éú³É×ÓÃÜÔ¿
int DES_MakeSubKeys(ElemType key[64],ElemType subKeys[16][48]){
	ElemType temp[56];
	int cnt;
	DES_PC1_Transform(key,temp);//PC1ÖÃ»»
	for(cnt = 0; cnt < 16; cnt++){//16ÂÖµø´ú£¬²úÉú16¸ö×ÓÃÜÔ¿
		DES_ROL(temp,MOVE_TIMES[cnt]);//Ñ­»·×óÒÆ
		DES_PC2_Transform(temp,subKeys[cnt]);//PC2ÖÃ»»£¬²úÉú×ÓÃÜÔ¿
	}
	return 0;
}

//ÃÜÔ¿ÖÃ»»1
int DES_PC1_Transform(ElemType key[64], ElemType tempbts[56]){
	int cnt;	
	for(cnt = 0; cnt < 56; cnt++){
		tempbts[cnt] = key[PC_1[cnt]];
	}
	return 0;
}

//ÃÜÔ¿ÖÃ»»2
int DES_PC2_Transform(ElemType key[56], ElemType tempbts[48]){
	int cnt;
	for(cnt = 0; cnt < 48; cnt++){
		tempbts[cnt] = key[PC_2[cnt]];
	}
	return 0;
}

//Ñ­»·×óÒÆ
int DES_ROL(ElemType data[56], int time){	
	ElemType temp[56];

	//±£´æ½«ÒªÑ­»·ÒÆ¶¯µ½ÓÒ±ßµÄÎ»
	memcpy(temp,data,time);
	memcpy(temp+time,data+28,time);
	
	//Ç°28Î»ÒÆ¶¯
	memcpy(data,data+time,28-time);
	memcpy(data+28-time,temp,time);

	//ºó28Î»ÒÆ¶¯
	memcpy(data+28,data+28+time,28-time);
	memcpy(data+56-time,temp+time,time);	

	return 0;
}

//IPÖÃ»»
int DES_IP_Transform(ElemType data[64]){
	int cnt;
	ElemType temp[64];
	for(cnt = 0; cnt < 64; cnt++){
		temp[cnt] = data[IP_Table[cnt]];
	}
	memcpy(data,temp,64);
	return 0;
}

//IPÄæÖÃ»»
int DES_IP_1_Transform(ElemType data[64]){
	int cnt;
	ElemType temp[64];
	for(cnt = 0; cnt < 64; cnt++){
		temp[cnt] = data[IP_1_Table[cnt]];
	}
	memcpy(data,temp,64);
	return 0;
}

//À©Õ¹ÖÃ»»
int DES_E_Transform(ElemType data[48]){
	int cnt;
	ElemType temp[48];
	for(cnt = 0; cnt < 48; cnt++){
		temp[cnt] = data[E_Table[cnt]];
	}	
	memcpy(data,temp,48);
	return 0;
}

//PÖÃ»»
int DES_P_Transform(ElemType data[32]){
	int cnt;
	ElemType temp[32];
	for(cnt = 0; cnt < 32; cnt++){
		temp[cnt] = data[P_Table[cnt]];
	}	
	memcpy(data,temp,32);
	return 0;
}

//Òì»ò
int DES_XOR(ElemType R[48], ElemType L[48] ,int count){
	int cnt;
	for(cnt = 0; cnt < count; cnt++){
		R[cnt] ^= L[cnt];
	}
	return 0;
}


//SºÐÖÃ»»
int DES_SBOX(ElemType data[48]){
	int cnt;
	int line,row,output;
	int cur1,cur2;
	for(cnt = 0; cnt < 8; cnt++){
		cur1 = cnt*6;
		cur2 = cnt<<2;
		
		//¼ÆËãÔÚSºÐÖÐµÄÐÐÓëÁÐ
		line = (data[cur1]<<1) + data[cur1+5];
		row = (data[cur1+1]<<3) + (data[cur1+2]<<2)
			+ (data[cur1+3]<<1) + data[cur1+4];
		output = S[cnt][line][row];

		//»¯Îª2½øÖÆ
		data[cur2] = (output&0X08)>>3;
		data[cur2+1] = (output&0X04)>>2;
		data[cur2+2] = (output&0X02)>>1;
		data[cur2+3] = output&0x01;
	}	
	return 0;
}

//½»»»
int DES_Swap(ElemType left[32], ElemType right[32]){
	ElemType temp[32];
	memcpy(temp,left,32);	
	memcpy(left,right,32);	
	memcpy(right,temp,32);
	return 0;
}

//¼ÓÃÜµ¥¸ö·Ö×é
int DES_EncryptBlock(ElemType plainBlock[8], ElemType subKeys[16][48], ElemType cipherBlock[8]){
	ElemType plainBits[64];
	ElemType copyRight[48];
	int cnt;

	Char8ToBit64(plainBlock,plainBits);		
	//³õÊ¼ÖÃ»»£¨IPÖÃ»»£©
	DES_IP_Transform(plainBits);

	//16ÂÖµü´ú
	for(cnt = 0; cnt < 16; cnt++){		
		memcpy(copyRight,plainBits+32,32);
		//½«ÓÒ°ë²¿·Ö½øÐÐÀ©Õ¹ÖÃ»»£¬´Ó32Î»À©Õ¹µ½48Î»
		DES_E_Transform(copyRight);
		//½«ÓÒ°ë²¿·ÖÓë×ÓÃÜÔ¿½øÐÐÒì»ò²Ù×÷
		DES_XOR(copyRight,subKeys[cnt],48);	
		//Òì»ò½á¹û½øÈëSºÐ£¬Êä³ö32Î»½á¹û
		DES_SBOX(copyRight);
		//PÖÃ»»
		DES_P_Transform(copyRight);
		//½«Ã÷ÎÄ×ó°ë²¿·ÖÓëÓÒ°ë²¿·Ö½øÐÐÒì»ò
		DES_XOR(plainBits,copyRight,32);
		if(cnt != 15){
			//×îÖÕÍê³É×óÓÒ²¿µÄ½»»»
			DES_Swap(plainBits,plainBits+32);
		}
	}
	//Äæ³õÊ¼ÖÃ»»£¨IP^1ÖÃ»»£©
	DES_IP_1_Transform(plainBits);
	Bit64ToChar8(plainBits,cipherBlock);
	return 0;
}

//½âÃÜµ¥¸ö·Ö×é
int DES_DecryptBlock(ElemType cipherBlock[8], ElemType subKeys[16][48],ElemType plainBlock[8]){
	ElemType cipherBits[64];
	ElemType copyRight[48];
	int cnt;

	Char8ToBit64(cipherBlock,cipherBits);		
	//³õÊ¼ÖÃ»»£¨IPÖÃ»»£©
	DES_IP_Transform(cipherBits);
	
	//16ÂÖµü´ú
	for(cnt = 15; cnt >= 0; cnt--){		
		memcpy(copyRight,cipherBits+32,32);
		//½«ÓÒ°ë²¿·Ö½øÐÐÀ©Õ¹ÖÃ»»£¬´Ó32Î»À©Õ¹µ½48Î»
		DES_E_Transform(copyRight);
		//½«ÓÒ°ë²¿·ÖÓë×ÓÃÜÔ¿½øÐÐÒì»ò²Ù×÷
		DES_XOR(copyRight,subKeys[cnt],48);		
		//Òì»ò½á¹û½øÈëSºÐ£¬Êä³ö32Î»½á¹û
		DES_SBOX(copyRight);
		//PÖÃ»»
		DES_P_Transform(copyRight);		
		//½«Ã÷ÎÄ×ó°ë²¿·ÖÓëÓÒ°ë²¿·Ö½øÐÐÒì»ò
		DES_XOR(cipherBits,copyRight,32);
		if(cnt != 0){
			//×îÖÕÍê³É×óÓÒ²¿µÄ½»»»
			DES_Swap(cipherBits,cipherBits+32);
		}
	}
	//Äæ³õÊ¼ÖÃ»»£¨IP^1ÖÃ»»£©
	DES_IP_1_Transform(cipherBits);
	Bit64ToChar8(cipherBits,plainBlock);
	return 0;
}

//¼ÓÃÜÎÄ¼þ
int DES_Encrypt(char *plainFile, char *keyStr,char *cipherFile){
	FILE *plain,*cipher;
	int count;
	ElemType plainBlock[8],cipherBlock[8],keyBlock[8];
	ElemType bKey[64];
	ElemType subKeys[16][48];
	if((plain = fopen(plainFile,"rb")) == NULL){
		return PLAIN_FILE_OPEN_ERROR;
	}	
	if((cipher = fopen(cipherFile,"wb")) == NULL){
		return CIPHER_FILE_OPEN_ERROR;
	}
	//ÉèÖÃÃÜÔ¿
	memcpy(keyBlock,keyStr,8);
	//½«ÃÜÔ¿×ª»»Îª¶þ½øÖÆÁ÷
	Char8ToBit64(keyBlock,bKey);
	//Éú³É×ÓÃÜÔ¿
	DES_MakeSubKeys(bKey,subKeys);
	
	while(!feof(plain)){
		//Ã¿´Î¶Á8¸ö×Ö½Ú£¬²¢·µ»Ø³É¹¦¶ÁÈ¡µÄ×Ö½ÚÊý
		if((count = fread(plainBlock,sizeof(char),8,plain)) == 8){
			DES_EncryptBlock(plainBlock,subKeys,cipherBlock);
			fwrite(cipherBlock,sizeof(char),8,cipher);	
		}
	}
	if(count){
		//Ìî³ä
		memset(plainBlock + count,'\0',7 - count);
		//×îºóÒ»¸ö×Ö·û±£´æ°üÀ¨×îºóÒ»¸ö×Ö·ûÔÚÄÚµÄËùÌî³äµÄ×Ö·ûÊýÁ¿
		plainBlock[7] = 8 - count;
		DES_EncryptBlock(plainBlock,subKeys,cipherBlock);
		fwrite(cipherBlock,sizeof(char),8,cipher);
	}
	fclose(plain);
	fclose(cipher);
	return 1;
}

//½âÃÜÎÄ¼þ
int DES_Decrypt(char *cipherFile, char *keyStr,char *plainFile){
	FILE *plain, *cipher;
	int count,times = 0;
	long fileLen;
	ElemType plainBlock[8],cipherBlock[8],keyBlock[8];
	ElemType bKey[64];
	ElemType subKeys[16][48];
	if((cipher = fopen(cipherFile,"rb")) == NULL){
		return CIPHER_FILE_OPEN_ERROR;
	}
	if((plain = fopen(plainFile,"wb")) == NULL){
		return PLAIN_FILE_OPEN_ERROR;
	}

	//ÉèÖÃÃÜÔ¿
	memcpy(keyBlock,keyStr,8);
	//½«ÃÜÔ¿×ª»»Îª¶þ½øÖÆÁ÷
	Char8ToBit64(keyBlock,bKey);
	//Éú³É×ÓÃÜÔ¿
	DES_MakeSubKeys(bKey,subKeys);

	//È¡ÎÄ¼þ³¤¶È	
	fseek(cipher,0,SEEK_END);	//½«ÎÄ¼þÖ¸ÕëÖÃÎ²
	fileLen = ftell(cipher);	//È¡ÎÄ¼þÖ¸Õëµ±Ç°Î»ÖÃ
	rewind(cipher);				//½«ÎÄ¼þÖ¸ÕëÖØÖ¸ÏòÎÄ¼þÍ·
	while(1){
		//ÃÜÎÄµÄ×Ö½ÚÊýÒ»¶¨ÊÇ8µÄÕûÊý±¶
		fread(cipherBlock,sizeof(char),8,cipher);
		DES_DecryptBlock(cipherBlock,subKeys,plainBlock);						
		times += 8;
		if(times < fileLen){
			fwrite(plainBlock,sizeof(char),8,plain);
		}
		else{
			break;
		}
	}
	//ÅÐ¶ÏÄ©Î²ÊÇ·ñ±»Ìî³ä
	if(plainBlock[7] < 8){
		for(count = 8 - plainBlock[7]; count < 7; count++){
			if(plainBlock[count] != '\0'){
				break;
			}
		}
	}	
	if(count == 7){//ÓÐÌî³ä
		fwrite(plainBlock,sizeof(char),8 - plainBlock[7],plain);
	}
	else{//ÎÞÌî³ä
		fwrite(plainBlock,sizeof(char),8,plain);
	}

	fclose(plain);
	fclose(cipher);
	return 1;
}
void init_win(void)
{
    /* ³õÊ¼»¯ÐéÄâ´°¿Ú½øÈëncursesÄ£Ê½ */
    initscr();

    /*µ±cbreakÄ£Ê½±»¿ªÆôáá£¬³ýÁËDELETE»òCTRL
µÈÈÔ±»ÊÓÎªÌØÊâ¿ØÖÆ×ÖÔªÍâÒ»ÇÐÊäÈëµÄ×ÖÔª½«Á¢¿Ì±»Ò»Ò»¶ÁÈ¡¡£*/
    cbreak();
    //noecho(); /*¹Ø±Õ»ØÏÔ*/
    start_color();
       init_pair(1,COLOR_WHITE,COLOR_BLACK);
	init_pair(2,COLOR_BLUE,COLOR_BLACK);
	init_pair(3,COLOR_GREEN,COLOR_BLACK);
	init_pair(4,COLOR_YELLOW,COLOR_BLACK);
	init_pair(5,COLOR_MAGENTA,COLOR_BLACK);
	init_pair(6,COLOR_CYAN,COLOR_BLACK);
	init_pair(7,COLOR_RED,COLOR_BLACK);
}

void exit_win(void)
{
    endwin();
}
/*clear win*/
 void ko_win(WINDOW *koo_win)
{
	wclear(koo_win);
        wrefresh(koo_win);
        delwin(koo_win); 
}
void begin_dos(void)
{
	begin_win=newwin(24, 80, 0, 0);
	box(begin_win,0,0);  
	keypad(begin_win, true);
	wattron(begin_win,COLOR_PAIR(1));
	wattron(begin_win,A_REVERSE);
	//mvwprintw(begin_win,1,78,"X");
       char str1[10];
       char str2[8];
       wrefresh(begin_win);
	mvwprintw(begin_win,5,6,"            ");
	mvwprintw(begin_win,6,6,"            ");
	mvwprintw(begin_win,7,6,"    ");
	mvwprintw(begin_win,8,6,"    ");
	mvwprintw(begin_win,9,6,"    ");
	mvwprintw(begin_win,10,6,"    ");
	mvwprintw(begin_win,11,6,"            ");
	mvwprintw(begin_win,12,6,"            ");
	
	mvwprintw(begin_win,5,22,"    ");
	mvwprintw(begin_win,6,22,"    ");
	mvwprintw(begin_win,7,22,"    ");
	mvwprintw(begin_win,8,22,"           ");
	mvwprintw(begin_win,9,22,"           ");
	mvwprintw(begin_win,10,22,"    ");
	mvwprintw(begin_win,11,22,"    ");
	mvwprintw(begin_win,12,22,"    ");
	mvwprintw(begin_win,5,33,"    ");
	mvwprintw(begin_win,6,33,"    ");
	mvwprintw(begin_win,7,33,"    ");
	mvwprintw(begin_win,8,33,"    ");
	mvwprintw(begin_win,9,33,"    ");
	mvwprintw(begin_win,10,33,"    ");
	mvwprintw(begin_win,11,33,"    ");
	mvwprintw(begin_win,12,33,"    ");

	mvwprintw(begin_win,5,41,"               ");
	mvwprintw(begin_win,6,41,"               ");
	mvwprintw(begin_win,7,41,"    ");
	mvwprintw(begin_win,8,41,"    ");
	mvwprintw(begin_win,9,41,"               ");
	mvwprintw(begin_win,10,41,"               ");
	mvwprintw(begin_win,11,41,"    ");
	mvwprintw(begin_win,12,41,"    ");

	mvwprintw(begin_win,6,52,"    ");
	mvwprintw(begin_win,7,52,"    ");
	mvwprintw(begin_win,8,52,"    ");
	mvwprintw(begin_win,9,52,"    ");
	mvwprintw(begin_win,10,52,"    ");
	mvwprintw(begin_win,11,52,"    ");
	mvwprintw(begin_win,12,52,"    ");

	mvwprintw(begin_win,5,59,"               ");
	mvwprintw(begin_win,6,59,"               ");
	mvwprintw(begin_win,7,64,"     ");
	mvwprintw(begin_win,8,64,"     ");
	mvwprintw(begin_win,9,64,"     ");
	mvwprintw(begin_win,10,64,"     ");
	mvwprintw(begin_win,11,64,"     ");
	mvwprintw(begin_win,12,64,"     ");

	
       mvwprintw(begin_win,17,13,"UserName:");
       mvwprintw(begin_win,17,40,"[The max length of username is 10]");
       mvwprintw(begin_win,19,13,"Password:");
       mvwprintw(begin_win,19,40,"[The  length of password is 8]");
	mvwprintw(begin_win,21,50,"[you have 3 times]");
	wmove(begin_win,17,22);
       wrefresh(begin_win); 
	wattroff(begin_win,A_REVERSE);
 	noecho();
	fflush(stdin); 
	 int k=0;
  	 int e=0;/*ÅÐ¶ÏÕËºÅÊäÈë ¿ÉÍË¸ñ*/
   while((str1[k++]=wgetch(begin_win))!='\n')
		{	
			if(str1[k-1]==(char)KEY_BACKSPACE)
				{
				str1[--k]='\0';
				str1[--k]='\0';
				if(k<0)
					k=0;
				for(e=0;e<k;e++)
				mvwaddch(begin_win,17,22+e,str1[e]);
				mvwprintw(begin_win,17,22+e,"        ");
				wmove(begin_win,17,22+e);
				}
			else
			mvwaddch(begin_win,17,21+k,str1[k-1]);
			wrefresh(begin_win);
			fflush(stdin);
			
				
		}
	str1[k]='\0';
	str1[k+1]='\0';
	str1[k-1]='\0';
    wmove(begin_win,19,22);
	 k=0;
    fflush(stdin);
	noecho();
	/*ÅÐ¶ÏÃÜÂëÊäÈë ¿ÉÍË¸ñ*/
	while((str2[k++]=wgetch(begin_win))!='\n')
		{	
				if(str2[k-1]==(char)KEY_BACKSPACE)
				{
				str2[--k]='\0';
				str2[--k]='\0';
				if(k<0)
					k=0;
				for(e=0;e<k;e++)
				mvwaddch(begin_win,19,22+e,'*');
				mvwprintw(begin_win,19,22+e,"        ");
				wmove(begin_win,19,22+e);
				}
			else
			mvwprintw(begin_win,19,21+k,"*");
			wrefresh(begin_win);
			fflush(stdin);
				
		}
	str2[k]='\0';
	str2[k+1]='\0';
	str2[k-1]='\0';
	 if(strcmp(str1,"admin")==0&&strcmp(str2,"123456")==0)
	 	{
	 	wrong_time=0;
	 	password=1;
	 	}
	 else
	 	wrong_time++;
	 wattroff(begin_win,COLOR_PAIR(1));
}
int checkip(char *cip)
{
	char ipaddress[128];
	char ipnumber[4];
	int isubip=0;
	int idot = 0;
	int iresult = 0;
	int iipresult = 1;
	int i;
	memset(ipnumber,0,4);
	strncpy(ipaddress,cip,128);
	for(i=0;i<128;i++)
	{
		if(ipaddress[i]=='.')
		{
			idot++;
			isubip=0;
			if(atoi(ipnumber)>255)
				iipresult = 0;
			memset(ipnumber,0,4);
		}
		else
			ipnumber[isubip++]=ipaddress[i];
		if(idot==3&&iipresult!=0)
			iresult=1;
	}
	return iresult;
}
void exitsystem()
{
		if(ioutfileserver!=NULL)
			fclose(ioutfileserver);
		if(ioutfileclient!=NULL)
			fclose(ioutfileclient);
		//WSACleanup();
		exit(0);
	
}

void threadproclient(int pparam)
{
	int hsock =pparam;
	char crecvbuffer[1024];
	char cshowbuffer[1024];
	char crecvbufferdes[1024];
	int num=0;
	if(hsock!=-1)
		{
				mvwprintw(pad,rool++,1,"start:");
			prefresh(pad, rool_page ,0 , 10 ,2 , 20, 78) ;
		}
	while(1)
	{
		num=recv(hsock,crecvbuffer,1024,0);
		
		open3=fopen("7.txt","wb");
		if(open3!=NULL)
			{
				fprintf(open3,"%s",crecvbuffer);
			}
		else
			{
				ko_win(pad);
			ko_win(menu_win1);
			exit_win();	
				exitsystem();
			}
		fclose(open3);
		DES_Decrypt("7.txt","581fbc94d3a452ea","8.txt");
		
		open4=fopen("8.txt","rb");
		if(open4!=NULL)
			{
				fscanf(open4,"%s",crecvbufferdes);
			}
		else
			{
				ko_win(pad);
			ko_win(menu_win1);
			exit_win();	
				exitsystem();
			}
		fclose(open4);

		
		if(num>=0)
		{
			crecvbufferdes[num]='\0';
			sprintf(cshowbuffer,"to me:%s\n",crecvbufferdes);
			
			mvwprintw(pad,rool++,1,"%s",cshowbuffer);
				if(rool<11)
					rool_page=0;
				else
					rool_page=rool-10;
				prefresh(pad, rool_page ,0 , 10 ,2 , 20, 78) ;
			fwrite(cshowbuffer,sizeof(char),strlen(cshowbuffer),ioutfileclient);
			fflush(ioutfileclient);
			if(strcmp("exit",crecvbufferdes)==0)
				{ko_win(pad);
			ko_win(menu_win1);
			exit_win();	
				exitsystem();
				}
		}
	}
	//return 0;
}
void threadproserver(int pparam)
{
	int hsock =pparam;
	char crecvbuffer[8192];
	char cshowbuffer[1024];
	char crecvbufferdes[1024];
	int num=0;
	if(hsock!=-1)
		{
				mvwprintw(pad,rool++,1,"start:");
			prefresh(pad, rool_page ,0 , 10 ,2 , 20, 78) ;
		}
	while(1)
	{
		
		num=recv(hsock,crecvbuffer,1024,0);
		

		open3=fopen("5.txt","wb");
		if(open3!=NULL)
			{
				fprintf(open3,"%s",crecvbuffer);
			}
		else
			{
				ko_win(pad);
			ko_win(menu_win1);
			exit_win();	
				exitsystem();
			}
		fclose(open3);
		DES_Decrypt("5.txt","581fbc94d3a452ea","6.txt");
		
		open4=fopen("6.txt","rb");
		if(open4!=NULL)
			{
				fscanf(open4,"%s",crecvbufferdes);
			}
		else
			{
				ko_win(pad);
			ko_win(menu_win1);
			exit_win();	
				exitsystem();
			}
		fclose(open4);

		if(num>=0)
		{
			crecvbufferdes[num]='\0';
			sprintf(cshowbuffer,"to me:%s\n",crecvbufferdes);
			
			mvwprintw(pad,rool++,1,"%s",cshowbuffer);
				if(rool<11)
					rool_page=0;
				else
					rool_page=rool-10;
				prefresh(pad, rool_page ,0 , 10 ,2 , 20, 78) ;
			fwrite(cshowbuffer,sizeof(char),strlen(cshowbuffer),ioutfileserver);
			fflush(ioutfileserver);
			if(strcmp("exit",crecvbufferdes)==0)
					{
					ko_win(pad);
			ko_win(menu_win1);
			exit_win();	
				exitsystem();
				}
		}
	}
	//return 0;
}

int  createserver(void)
{
	curs_set(0);
	pad=newpad(80,128);
	int  m_sockserver;
	int m_server;
	struct sockaddr_in serveraddr;
	struct sockaddr_in serveraddrfrom;
	
	int ibindresult =1;
	int iwhilecount =200;
	char cwelcomebuffer[]="welcome to you\0";
	int len =sizeof(struct sockaddr);
	int iwhilelistencount =10;
	char *localip;
	//DWORD nthreadid=0;
	pthread_t tid2;
	int ires;
	char csendbuffer[1024];
	char cshowbuffer[1024];
	char csendbufferdes[1024];
		
	m_sockserver =socket(AF_INET,SOCK_STREAM,0);
	
				ioutfileserver =fopen("messageserver.txt","a");
				mvwprintw(menu_win1,6,4,"Please enter the port number:");
				 wmove(menu_win1,6,33);
				wrefresh(menu_win1);
				wscanw(menu_win1,"%d",&iport);


				serveraddr.sin_family =AF_INET;
				serveraddr.sin_port =htons(iport);/*¶Ë¿Ú*/
				serveraddr.sin_addr.s_addr =htonl(INADDR_ANY);/*µØÖ·*/

				ibindresult =bind(m_sockserver,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr));
	/*Èç¹û¶Ë¿Ú²»ÄÜ±»°ó¶¨£¬ÖØÐÂÉèÖÃ¶Ë¿Ú*/

	while(ibindresult!=0&&iwhilecount>0)
	{
		mvwprintw(menu_win1,6,4,"Wrong! enter the port number again:");
		wmove(menu_win1,6,39);
		wrefresh(menu_win1);
		wscanw(menu_win1,"%d",&iport);

		/*ÉèÖÃÍøÂçµØÖ·ÐÅÏ¢*/
		serveraddr.sin_family =AF_INET;
		serveraddr.sin_port =htons(iport);/*¶Ë¿Ú*/
		serveraddr.sin_addr.s_addr =inet_addr(localip);/*µØÖ·*/
		/*°ó¶¨µØÖ·ÐÅÏ¢*/

		ibindresult =bind(m_sockserver,(struct sockaddr *)&serveraddr,sizeof(struct sockaddr));
		iwhilecount--;
		if(iwhilecount<=0)
		{
			
		mvwprintw(menu_win1,7,4,"Port binding failure");
		wrefresh(menu_win1);
		ko_win(menu_win1);
		 exit_win();
			exit(0);
		}
	
	}
	int kkk=0;
		while(iwhilelistencount>0)
	{
		mvwprintw(menu_win1,7,4,"start listen");
		wrefresh(menu_win1);
		listen(m_sockserver,0);/*·µ»ØÖµÅÐ¶Ïµ¥¸ö¼àÌýÊÇ·ñ³¬Ê±*/
		m_server=accept(m_sockserver,(struct sockaddr*)&serveraddrfrom,&len);
		if(m_server!=-1)
		{
			/*ÓÐÁ¬½Ó³É¹¦£¬·¢ËÍ»¶Ó­ÐÅÏ¢*/
			send(m_server,cwelcomebuffer,sizeof(cwelcomebuffer),MSG_NOSIGNAL);
			/*Æô¶¯½ÓÊÕÐÅÏ¢µÄÏß³Ì*/
			//CreateThread(NULL,0,threadproserver,(LPVOID)m_server,0,&nthreadid);
			pthread_create(&tid2,NULL,(void *)threadproserver,(void *)m_server); 
			break;
		}
		mvwprintw(menu_win1,7,4+kkk,".");
		wrefresh(menu_win1);
		iwhilelistencount--;
		kkk++;
		if(iwhilelistencount<=0)
		{
			
			mvwprintw(menu_win1,7,4,"To establish the connection fails");
		wrefresh(menu_win1);
		pthread_cancel(tid2);
		ko_win(pad);
		ko_win(menu_win1);
		 exit_win();
			exit(0);
		}			
}
	
		while(1)
	{
		memset(csendbuffer,0,1024);
		mvwprintw(menu_win1,8,4,"send:                               ");
		wrefresh(menu_win1);
		mvwscanw(menu_win1,8,9,"%s",csendbuffer);
		open1=fopen("3.txt","wb");
		if(open1!=NULL)
			{
				fprintf(open1,"%s",csendbuffer);
			}
		else
			{
				pthread_cancel(tid2);
				ko_win(pad);
				ko_win(menu_win1);
		 		exit_win();
				exit(0);
			}
		fclose(open1);
		DES_Encrypt("3.txt","581fbc94d3a452ea","4.txt");
		
		open2=fopen("4.txt","rb");
		if(open2!=NULL)
			{
				fscanf(open2,"%s",csendbufferdes);
			}
		else
			{
				pthread_cancel(tid2);
				ko_win(pad);
				ko_win(menu_win1);
		 		exit_win();
				exit(0);
			}
		fclose(open2);
		if(strlen(csendbufferdes)>0)
		{
			ires=send(m_server,csendbufferdes,strlen(csendbufferdes),MSG_NOSIGNAL);
			if(ires<0)
				{
			mvwprintw(pad,rool++,1,"send fail");
			prefresh(pad, rool_page ,0 , 10 ,2 , 20, 78) ;
				}
			else
			{
				
				sprintf(cshowbuffer,"send to :%s\n",csendbuffer);				
				mvwprintw(pad,rool++,1,"%s",cshowbuffer);
				if(rool<11)
					rool_page=0;
				else
					rool_page=rool-10;
				prefresh(pad, rool_page ,0 , 10 ,2 , 20, 78) ;
				fwrite(cshowbuffer,sizeof(char),strlen(cshowbuffer),ioutfileserver);
			}
			if(strcmp("exit",csendbuffer)==0)
				{
				pthread_cancel(tid2);
				ko_win(pad);
			ko_win(menu_win1);
			exit_win();	
				exitsystem();
				return 0;
				}
		}
	}

}


int createclient(void)
{
	curs_set(0);
	pad=newpad(80,128);
	int m_sockclient;
	struct sockaddr_in clientaddr;
	char cserverip[128];
	int iwhileip=10;
	int icnnres;
	//DWORD nthreadid = 0;
	pthread_t tid1;
	char csendbuffer[1024];
	char cshowbuffer[1024];
	char crecvbuffer[1024];
	char csendbufferdes[1024];

	int num;
	int ires;
	int iipres;
	m_sockclient=socket(AF_INET,SOCK_STREAM,0);
	
	mvwprintw(menu_win1,6,4,"Please input the id address:");
				 wmove(menu_win1,6,32);
				wrefresh(menu_win1);
				wscanw(menu_win1,"%s",cserverip);
	

	/*ipµØÖ·ÅÐ¶Ï*/
	if(strlen(cserverip)==0)
		strcpy(cserverip,"127.0.0.1");/*Ã»ÓÐÊäÈëµØÖ·£¬Ê¹ÓÃ»·»ØµØÖ·*/
	else
	{
		iipres=checkip(cserverip);
		while(!iipres&&iwhileip>0)
		{
			mvwprintw(menu_win1,6,4,"Wrong! enter the id address again:");
		wmove(menu_win1,6,38);
		wrefresh(menu_win1);
		wscanw(menu_win1,"%s",cserverip);
			
			iipres=checkip(cserverip);
			iwhileip--;
			if(iwhileip<=0)
			{
				mvwprintw(menu_win1,6,4,"enter the wrong id address!");
				ko_win(pad);
			ko_win(menu_win1);
				exit(0);
			}
		}
	}
	ioutfileclient=fopen("messageserverclient.txt","a");
	clientaddr.sin_family=AF_INET;

	/*¿Í»§¶ËÏò·þÎñÆ÷ÇëÇóµÄ¶Ë¿ÚºÅ£¬Ó¦¸ÃÓë·þÎñÆ÷°ó¶¨µÄ¶Ë¿ÚºÅÒ»ÖÂ*/
	clientaddr.sin_port = htons(4600);
	clientaddr.sin_addr.s_addr = inet_addr(cserverip);
	icnnres = connect(m_sockclient,(struct sockaddr*)&clientaddr,sizeof(struct sockaddr));
	if(icnnres==0)
	{
		num=recv(m_sockclient,crecvbuffer,1024,0);
		if(num>0)
		{
			mvwprintw(pad,rool++,1,"receive from server :%s",crecvbuffer);
			if(rool<11)
					rool_page=0;
				else
					rool_page=rool-10;
			prefresh(pad, rool_page ,0 , 10 ,2 , 20, 78) ;
			
			/*¿ªÆôÏß³Ì*/
			//CreateThread(NULL,0,threadproclient,(LPVOID)m_sockclient,0,&nthreadid);
			pthread_create(&tid1,NULL,(void *) threadproclient,(void *)m_sockclient); 
		}
		while(1)
		{
			memset(csendbuffer,0,1024);
			mvwprintw(menu_win1,8,4,"send:                               ");
		wrefresh(menu_win1);
		mvwscanw(menu_win1,8,9,"%s",csendbuffer);

		open1=fopen("1.txt","wb");
		if(open1!=NULL)
			{
				fprintf(open1,"%s",csendbuffer);
			}
	
		fclose(open1);
		
		DES_Encrypt("1.txt","581fbc94d3a452ea","2.txt");
		
		open2=fopen("2.txt","rb");
		if(open2!=NULL)
			{
				fscanf(open2,"%s",csendbufferdes);
			}

		fclose(open2);
			
			if(strlen(csendbufferdes)>0)
			{
				ires=send(m_sockclient,csendbufferdes,strlen(csendbufferdes),MSG_NOSIGNAL);
				if(ires<0)
					{
			mvwprintw(pad,rool++,1,"send fail");
			if(rool<11)
					rool_page=0;
				else
					rool_page=rool-10;
			prefresh(pad, rool_page ,0 , 10 ,2 , 20, 78) ;
				}
				else
				{
					sprintf(cshowbuffer,"send to :%s\n",csendbuffer);
					mvwprintw(pad,rool++,1,"%s",cshowbuffer);
				if(rool<11)
					rool_page=0;
				else
					rool_page=rool-10;
				prefresh(pad, rool_page ,0 , 10 ,2 , 20, 78) ;
					
					fwrite(cshowbuffer,sizeof(char),strlen(cshowbuffer),ioutfileclient);
					fflush(ioutfileclient);
				}
				if(strcmp("exit",csendbuffer)==0)
					{pthread_cancel(tid1);
					ko_win(pad);
			ko_win(menu_win1);
			exit_win();	
						exitsystem();
						return 0;
					}
		}
	}
		}
		else
			{
			mvwprintw(menu_win1,5,4,"error!");
		wrefresh(menu_win1);
			}
		
	}

void menu_dos(void)
{
	MEVENT mouse1;
	int key1;
	raw();
	
	
	 menu_win1=newwin(24,80, 0, 0);   
	 box(menu_win1,0,0);  
	  wattron(menu_win1,COLOR_PAIR(4));
   
    keypad(menu_win1, true);
    mousemask(BUTTON1_CLICKED | BUTTON2_CLICKED, 0);	
    wattron(menu_win1,A_REVERSE);
    mvwprintw(menu_win1,2,14,"                   ");
    mvwprintw(menu_win1,3,14,"PEER TO PEER SERVER");
    mvwprintw(menu_win1,4,14,"                   ");
    wattroff(menu_win1,COLOR_PAIR(4));
    wattron(menu_win1,COLOR_PAIR(3));
    mvwprintw(menu_win1,2,36,"                   ");
    mvwprintw(menu_win1,3,36,"PEER TO PEER CLIENT");
    mvwprintw(menu_win1,4,36,"                   ");
    wattroff(menu_win1,COLOR_PAIR(3));
		wattron(menu_win1,COLOR_PAIR(7));
	mvwprintw(menu_win1,2,71,"    ");
    mvwprintw(menu_win1,3,71,"EXIT");
	mvwprintw(menu_win1,4,71,"    ");
	wattroff(menu_win1,COLOR_PAIR(7));
	wrefresh(menu_win1);
	  int choices=8;
    while(choices>7){
   curs_set(0);
    wrefresh(menu_win1);
    echo();
    //wscanw(menu_win1,"%d",&choices);
    key1 = wgetch(menu_win1);
		switch(key1)
			{
				case KEY_MOUSE : 
					getmouse(&mouse1); /*get mouse action*/

					if (!wenclose(menu_win1, mouse1.y, mouse1.x)) 
					break; /*do nothing if not in window*/

					wmouse_trafo(menu_win1, &mouse1.y, &mouse1.x, FALSE);
					if ((14 <= mouse1.x && mouse1.x <= 33) && (2<= mouse1.y && mouse1.y <= 4))
					choices=1;
				if ((36 <= mouse1.x && mouse1.x <= 56) && (2<= mouse1.y && mouse1.y <= 4))
					choices=2;
				if ((71 <= mouse1.x && mouse1.x <= 75) && (2<= mouse1.y && mouse1.y <= 4))
					choices=7;
				break;
				case 'q' :
						choices=7;
						break;
						default :
						break;
			}
		curs_set(1);
}
			if(choices==1)
				{
				
				createserver();
		
				}
			if(choices==2)
				{
				createclient();
				}
	
}

int main(void)
{
	 init_win();
	 while(!password)
       	 {
           		 begin_dos();		/*µÇÈë½çÃæ*/
        		  if(wrong_time>3)
             {
                 exit_win();
                 return 0;
             }
	 	}
	 ko_win(begin_win);
	menu_dos();
	 
	 ko_win(menu_win1);
	 exit_win();	
        return 0;/* ÍË³ö´°¿Ú */   
}
