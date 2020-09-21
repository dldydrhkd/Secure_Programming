#include <stdio.h>
#include <cstdio>
#include <iostream>
#include <string>
#include <stdlib.h>
using namespace std;
#define BYTE unsigned char  // unsigned integer 1byte
#define WORD unsigned short // unsigned integer 2byte
#define DWORD unsigned int  // unsigned integer 4byte
#define LONG int            // signed integer 4byte

#pragma pack(push,1)        // 구조체를 1바이트 크기로 정렬

typedef struct _BITMAPFILEHEADER{  //Bitmap 파일 헤더 구조체
    WORD bfType;            // BMP 파일 매직 넘버
    DWORD bfSize;           // 파일 크기
    WORD bfReserved1;       //예약변수(0으로 설정)
    WORD bfReserved2;       //예약변수 (0으로 설정)
    DWORD bfOffBits;        //비트맵 데이터의 시작 위치
} BITMAPFILEHEADER;

typedef struct _BITMAPINFOHEADER{ //BMP 비트맵 정보 헤더 구조체(DIB 헤더)
    DWORD biSize;           //현재 구조체의 크기
    LONG biWidth;           //비트맵 이미지의 가로 크기
    LONG biHeight;          //비트맵 이미지의 세로 크기
    WORD biPlanes;          //사용하는 색상판의 수
    WORD biBitCount;        //픽셀 하나를 표현하는 비트 수
    DWORD biCompression;    //압축 방식
    DWORD biSizeImage;      //비트맵 이미지의 픽셀 데이터 크기
    LONG biXpelsPerMeter;   //그림의 가로 해상도(미터당 픽셀)
    LONG biYPelsPerMeter;   //그림의 세로 해상도(미터당 픽셀)
    DWORD biClrUsed;        //색상 테이블에서 실제 사용되는 색상 수
    DWORD biClrImportant;   //비트맵을 표현하기 위해 필요한 색상 인덱스 수
} BITMAPINFOHEADER;

typedef struct _RGBTRIPLE            // 24비트 비트맵 이미지의 픽셀 구조체
{
    unsigned char rgbtBlue;          // 파랑
    unsigned char rgbtGreen;         // 초록
    unsigned char rgbtRed;           // 빨강
} RGBTRIPLE;

#pragma pack(pop)

int main(int argc, char **argv){
    if(argc!=2){
        cout<<"Wrong command!";
        return 0;
    }
    else{
        if(argv[1][0]=='e'){                //e 옵션일 경우
            char str[600]; 
            fgets(str,sizeof(str),stdin);         //문자열 입력
            str[strlen(str)-1]='\0';
            FILE *infile;
            infile = fopen("origin.bmp", "rb");
            if(infile==NULL){
                cout<<"No Image File";
                return 0;
            }
            BITMAPFILEHEADER hf;                //헤더 정보
            BITMAPINFOHEADER hInfo;             //bitmap info
            fread(&hf, sizeof(BITMAPFILEHEADER),1,infile);
            if(hf.bfType!=0x4D42) exit(1);
            fread(&hInfo, sizeof(BITMAPINFOHEADER),1, infile);  
            if(hInfo.biBitCount!=24){
                cout<<"Bad File Format!";
                return 0;
            }
            int size;                       //비트맵 크기
            int width, height;              //가로 세로 길이
            int padding;                    //패딩
            size = hInfo.biSizeImage;
            width = hInfo.biWidth;
            height = hInfo.biHeight;
            padding = (4-((width*3)%4))%4;         //패딩 계산
            if(size==0){
                size = (width * 3 + padding)*height;
            }
            unsigned char *image = new unsigned char(size);             //size만큼 동적 할당
            fread(image,size,1,infile);                     //원본으로 부터 size만큼 읽음
            int j=0;                                        //입력받은 str index
            for(int y=height-1; y>=0; y--){                            //뒤에서부터 거꾸로 추가
                bool check=false;
                for(int x=0; x<width; x++){                             //가로 
                    int index=(x*3)+(y*(width*3))+(padding*y);          //index 계산
                    if(index%17!=0) continue;                        //랜덤으로 넣기 위해 17의 배수가 아니면 skip
                    if(str[j]){                             //null까지 입력
                        image[index]=str[j];                        //image의 데이터에 str값 입력
                        j++;                                        //str index 증가
                    }
                    else{
                        check=true;
                        image[index]='\0';
                        break;
                    }
                }
                if(check) break;
            }

            fclose(infile);                                 //file 닫기
            //Image Output
            FILE *outfile = fopen("stego.bmp","wb");                        //새 bmp 파일 열기
            fwrite(&hf, sizeof(char), sizeof(BITMAPFILEHEADER), outfile);           //헤더 쓰기
            fwrite(&hInfo, sizeof(char), sizeof(BITMAPINFOHEADER), outfile);        //비트맵 info
            fwrite(image,size,1,outfile);                                    // outfile에 변화된 image 쓰기
            delete image;                                   //동적 메모리 해제
            fclose(outfile);                                //outfile 닫기
        }
        else if(argv[1][0]=='d'){
            FILE *infile;
            infile = fopen("stego.bmp", "rb");                  //stego 파일 열기
            if(infile==NULL){
                cout<<"No Image File";
                return 0;
            }
            BITMAPFILEHEADER hf;                                //헤더
            BITMAPINFOHEADER hInfo;                                 //비트맵 정보
            fread(&hf, sizeof(BITMAPFILEHEADER),1,infile);          //헤더 읽기
            if(hf.bfType!=0x4D42) exit(1);
            fread(&hInfo, sizeof(BITMAPINFOHEADER),1, infile);      //비트맵 정보 읽기
            if(hInfo.biBitCount!=24){
                cout<<"Bad File Format!";
                return 0;
            }
            int size;                                       //비트맵 크기
            int width, height;                              //너비 높이
            int padding;                                    //패딩
            size = hInfo.biSizeImage;
            width = hInfo.biWidth;
            height = hInfo.biHeight;
            padding = (4-((width*3)%4))%4;                  //패딩 계산
            if(size==0){
                size = (width * 3 + padding)*height;
            }
            unsigned char *image = new unsigned char(size);         //size만큼 동적 할당
            fread(image,size,1,infile);                             //size만큼 읽기
            for(int y=height-1; y>=0; y--){                         //맨 뒤부터 
                for(int x=0; x<width; x++){                         //가로
                    int index=(x*3)+(y*(width*3))+(padding*y);      //index 계산
                    if(index%17!=0) continue;                        //17의 배수가 아니면 skip
                    if(image[index]=='\0'){                         //null이 나올때까지 읽음
                        cout<<"\n";
                        fclose(infile);
                        return 0;
                    }
                    else{
                        cout<<image[index];
                    }
                }
            }
        }
        else{
            cout<<"Wrong option!";
            return 0;
        }
    }
}