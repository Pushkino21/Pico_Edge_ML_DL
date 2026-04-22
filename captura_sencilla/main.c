#include <stdio.h>
#include <stdlib.h>
#include <pico/stdlib.h>
#include "hardware/spi.h"
#include "ArducamCamera.h"

#define RESET_CAMERA                0XFF
#define SET_PICTURE_RESOLUTION      0X01
#define SET_VIDEO_RESOLUTION        0X02
#define SET_BRIGHTNESS              0X03
#define SET_CONTRAST                0X04
#define SET_SATURATION              0X05
#define SET_EV                      0X06
#define SET_WHITEBALANCE            0X07
#define SET_SPECIAL_EFFECTS         0X08
#define SET_FOCUS_CONTROL           0X09
#define SET_EXPOSUREANDGAIN_CONTROL 0X0A
// #define SET_GAIN_CONTROL         0X0B
#define SET_WHILEBALANCE_CONTROL    0X0C
#define SET_MANUAL_GAIN             0X0D
#define SET_MANUAL_EXPOSURE         0X0E
#define GET_CAMERA_INFO             0X0F
#define TAKE_PICTURE                0X10
#define SET_SHARPNESS               0X11
#define DEBUG_WRITE_REGISTER        0X12
#define STOP_STREAM                 0X21
#define GET_FRM_VER_INFO            0X30
#define GET_SDK_VER_INFO            0X40
#define SET_IMAGE_QUALITY           0X50

//#define READ_IMAGE_LENGTH           255
#define CAMERA_SPI_PORT spi1
#define CAMERA_MISO_PIN 12
#define CAMERA_MOSI_PIN 11
#define CAMERA_SCK_PIN  10

const int CS = 13;
#define READ_IMAGE_LENGTH 1024
uint8_t frame_buffer[READ_IMAGE_LENGTH] = {0};

char response;
ArducamCamera CAM;
int main()
{
  stdio_init_all();
  sleep_ms(3000);
  CAM = createArducamCamera(13);
  begin(&CAM);
  reset(&CAM);
  printf("Inicializando Camara\n");

  sleep_ms(200);
  printf("Prueba ArduCAM Mega 3MP\n");
  //sleep_ms(200);
  setAutoWhiteBalance(&CAM,0);
  setAutoWhiteBalanceMode(&CAM,CAM_WHITE_BALANCE_MODE_OFFICE);
  setImageQuality(&CAM, HIGH_QUALITY);
  setBrightness(&CAM,CAM_BRIGHTNESS_LEVEL_2);
  while (true) {
    //printf("Teclea C para capturar imagen y S para salir\n");
    scanf("%c",&response);
    if (response == 'C') {
      takePicture(&CAM, CAM_IMAGE_MODE_128X128, CAM_IMAGE_PIX_FMT_RGB565);
      while (CAM.receivedLength) { 
        uint32_t rlen = readBuff(&CAM, frame_buffer, READ_IMAGE_LENGTH);
        
        // CORRECCIÓN: Imprimir cada byte del buffer en hexadecimal
        for(uint32_t i = 0; i < rlen; i++) {
          printf("%02X ", frame_buffer[i]);
        }
        //printf("\n");
      }
      sleep_ms(2000);
    }else {
      printf("Reset camera\n");
      reset(&CAM);
    }
  }


  return 0;
}
