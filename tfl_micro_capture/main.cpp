extern "C"{
#include <stdio.h>
#include "tft_display.h"
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/spi.h"
#include "ArducamCamera.h"
#include "pico/multicore.h"
#include <stdlib.h>
#include <stdint.h>
#include <math.h>
}
//#define READ_IMAGE_LENGTH           255
#define CAMERA_SPI_PORT spi1
#define CAMERA_MISO_PIN 12
#define CAMERA_MOSI_PIN 11
#define CAMERA_SCK_PIN  10

#define CAMERA_CS_PIN 13
#define READ_IMAGE_LENGTH 32768
#define SMALL_IMAGE_LENGTH 18432

uint8_t frame_buffer[READ_IMAGE_LENGTH] __attribute__((section(".sram0.buffer"))) __attribute__((aligned(4)));
uint8_t small_framebuffer[SMALL_IMAGE_LENGHT]__attribute__((section(".sram1.buffer"))) __attribute__((aligned(8)));

volatile bool image_ready = false;
volatile bool display_busy = false;


ArducamCamera CAM;
void init_arducam()

{
  CAM = createArducamCamera(CAMERA_CS_PIN);
  begin(&CAM);
  reset(&CAM);
  printf("Inicializando Arducam \n");
  sleep_ms(200);
}

void arducam_config()
{
  setAutoWhiteBalance(&CAM,0);
  setAutoWhiteBalanceMode(&CAM,CAM_WHITE_BALANCE_MODE_OFFICE);
  setImageQuality(&CAM,HIGH_QUALITY);
  setBrightness(&CAM,CAM_BRIGHTNESS_LEVEL_2);
}


void demo_colors()
{
  uint8_t r[3] = {0xFC,0x00,0x00};
  uint8_t g[3] = {0x00,0xFC,0x00};
  uint8_t b[3] = {0x00,0x00,0xFC};
uint8_t blk[3] = {0x00,0x00,0x00};
  uint8_t w[3] = {0xFC,0xFC,0xFC};

 /*Inicialicazión de display nota: crear una funcion que realice todo esto*/
  display_init(bitdepth_18);
  set_frame_area(0x00,0x80,0x00,0xA0);
  
  /*Inicio de escritura de framebuffers*/
  ram_write_init();
  DC_mode(1);

  for (int i = 0; i < (128*160); i++ ){
    
    write_data_array(r,3);
  }

  sleep_ms(2000);
  
  for (int j = 0; j < (128*160); j++ ){
    
    write_data_array(g,3);
  }

  sleep_ms(2000);

  for (int k = 0; k < (128*160); k++ ){
    
    write_data_array(b,3);
  }

  sleep_ms(2000);

  for (int l = 0; l < (128*160); l++ ){
    
    write_data_array(blk,3);
  }

  sleep_ms(2000);
  
  for (int m = 0; m < (128*160); m++ ){
    
    write_data_array(w,3);
  }

  sleep_ms(2000);
  DC_mode(0);
}




void core1_capture()
{
 while(1)
 {
   while (display_busy) 
   {
     tight_loop_contents();
   }

   takePicture(&CAM,CAM_IMAGE_MODE_128X128 , CAM_IMAGE_PIX_FMT_RGB565);
   while (CAM.receivedLength) 
   { 
     uint32_t rlen1 = readBuff(&CAM, frame_buffer, READ_IMAGE_LENGTH);
   }
   
   takePicture(&CAM, CAM_IMAGE_MODE_96X96, CAM_IMAGE_PIX_FMT_RGB565);
   while (CAM.receivedLength)
   { 
     uint32_t rlen = readBuff(&CAM, small_framebuffer, SMALL_IMAGE_LENGTH);
   }
   image_ready = true;
   multicore_fifo_push_blocking(0x01);
 }  
}

int main()
{
  stdio_init_all();
  tft_spi_begin();
  init_arducam();
  arducam_config();
  display_init(bitdepth_16);
  set_frame_area(0x00,0x80,0x00,0xA0);
  ram_write_init();
  DC_mode(1);
  uint8_t blk[3] = {0x00,0x00,0x00};
  for (int l = 0; l < (128*160); l++ ){
    
    write_data_array(blk,3);
  }
  printf("Camara y display incializados \n");


   multicore_launch_core1(core1_capture);
  while (1) 
  {
      uint32_t flag = multicore_fifo_pop_blocking();
      if(flag == 0x01){
        display_busy = true;
        set_frame_area(0x00,0x80,0x20,0xA0); //Imagen centrada en el display_busy
        ram_write_init();
        write_data_array(frame_buffer,16384);
        write_data_array(frame_buffer +16384, 16384);
        
        /*set_frame_area(0x10,0x70,0x10,0x70);
        ram_write_init();
        write_data_array(small_framebuffer,SMALL_IMAGE_LENGHT/2);
        write_data_array(small_framebuffer +(SMALL_IMAGE_LENGHT/2), SMALL_IMAGE_LENGHT/2);*/
        display_busy = false;
      }
  }
  return 0;
}
