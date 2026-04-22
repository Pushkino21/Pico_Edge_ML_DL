#include "tensorflow/lite/micro/micro_mutable_op_resolver.h"
#include "tensorflow/lite/micro/tflite_bridge/micro_error_reporter.h"
#include "tensorflow/lite/micro/micro_interpreter.h"
#include "tensorflow/lite/schema/schema_generated.h"
//#include "tensorflow/lite/version.h"
#include "cifar10_model.h"


extern "C"{
#include <stdio.h>
#include "tft_display.h"
#include "pico/stdlib.h"
#include "hardware/irq.h"
#include "hardware/spi.h"
#include "ArducamCamera.h"
#include "pico/multicore.h"
}
//Struct to get the rgb888 values
typedef struct 
{
  uint8_t r;
  uint8_t g;
  uint8_t b;
}rgb888_t;

// Definiciones de puerto para la Camara
#define CAMERA_SPI_PORT spi1
#define CAMERA_MISO_PIN 12
#define CAMERA_MOSI_PIN 11
#define CAMERA_SCK_PIN  10
#define CAMERA_CS_PIN 13

//Definiciones de tamano de framebuffers
#define READ_IMAGE_LENGTH 32768
#define CUT_IMAGE_LENGTH 2048  

//Definicion de arrays para los framebuffers
uint8_t frame_buffer[READ_IMAGE_LENGTH] __attribute__((aligned(4))) = {0};
uint8_t small_Framebuffer[CUT_IMAGE_LENGTH] __attribute__((aligned(4))) ={0};
uint8_t image_tensor[1][32][32][3] __attribute__((aligned(4)));

constexpr int tensor_arena_size = 200 * 1024;
static uint8_t tensor_arena[tensor_arena_size] __attribute__((aligned(16)));

//Flags para el uso del segundo core
volatile bool image_ready = false;
volatile bool display_busy = false;

//creacion del "objeto" CAM
//char response;
ArducamCamera CAM;
uint32_t len;

//Foward Declaration
void init_arducam();
void arducam_config();
void demo_colors();
void core1_capture();
void extractFramebuffer();
rgb888_t extendRGB(uint8_t lowByte, uint8_t highByte);
void createTensor();


//=========================Comienzo de la funcion Main==========================
int main()
{
  //configuracion inicial Pico W
  stdio_init_all();
  tft_spi_begin();

  //Inicialicazión Arducam
  init_arducam();
  arducam_config();

  //Inicialicazión display ST7735
  display_init(bitdepth_16);
  set_frame_area(0x00,0x80,0x00,0xA0);
  ram_write_init();
  DC_mode(1);


  // ==================Inicialicazión tensorflow================================
  
  //Inicialicazión del log
  //tflite::MicroErrorReporter error_reporter;
  
  //Mapeando el modelo en una data structure usable
  const tflite::Model* model = tflite::GetModel(cifar10_tflite);

  //Creando el resolver con las operaciones que necesita el modelo
  tflite::MicroMutableOpResolver<5> resolver;
  resolver.AddFullyConnected();
  resolver.AddRelu();
  resolver.AddConv2D();
  resolver.AddMaxPool2D();
  resolver.AddReshape();


  //Creacion del interprete para correr el modelo
  tflite::MicroInterpreter interpreter(
    model, resolver, tensor_arena, tensor_arena_size);

  //Alojar memoria para el input y el output y tensores de entre medio
  interpreter.AllocateTensors();

  TfLiteTensor* input = interpreter.input(0);
  TfLiteTensor* output = interpreter.output(0);



  uint8_t blk[3] = {0x00,0x00,0x00};
  for (int l = 0; l < (128*160); l++ ){
    
    write_data_array(blk,3);
  }
  printf("Camara y display incializados \n");


   multicore_launch_core1(core1_capture);
  while (1) 
  {
    uint32_t flag = multicore_fifo_pop_blocking();
    if(flag == 0x01)
    {
      display_busy = true;
      set_frame_area(0x00,0x80,0x20,0xA0); //Imagen centrada en el display_busy
      ram_write_init();
      write_data_array(frame_buffer,16384);
      write_data_array(frame_buffer +16384, 16384);
      set_frame_area(0x00,0x1F,0x00,0x1F);
      ram_write_init();
      write_data_array(small_Framebuffer,2048);

      memcpy(input->data.f, image_tensor, 32 * 32 * 3 * sizeof(float));

      interpreter.Invoke();
      float value = output->data.f[0];
      printf("Valor de inferencia: %f\n",value);
      display_busy = false;
      } 
    
  }
  return 0;
}
//=============================Final de la funcion main===============================

//Funciones requeridas en el programa pero que no constituyen una libreria... de momento
void init_arducam()
{ // Funcion para manejar el inicio de la camara
  CAM = createArducamCamera(CAMERA_CS_PIN);
  begin(&CAM);
  reset(&CAM);
  printf("Inicializando Arducam \n");
  sleep_ms(200);
}

void arducam_config()
{ //Funcion para manejar la configuracion de la camara
  setAutoWhiteBalance(&CAM,0);
  setAutoWhiteBalanceMode(&CAM,CAM_WHITE_BALANCE_MODE_OFFICE);
  setImageQuality(&CAM,HIGH_QUALITY);
  setBrightness(&CAM,CAM_BRIGHTNESS_LEVEL_2);
}


void demo_colors()
{//Funcion para probar las capacidades de color del display
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
{//Funcion para manejar el otro core del RP2040
 while(1)
  {
    while (display_busy)
    {
        tight_loop_contents();
    }
    takePicture(&CAM, CAM_IMAGE_MODE_128X128, CAM_IMAGE_PIX_FMT_RGB565);
    while (CAM.receivedLength) 
    { 
      uint32_t rlen = readBuff(&CAM, frame_buffer, READ_IMAGE_LENGTH);
    }
    extractFramebuffer();
    //printf("Buffer extraido\n");
    createTensor();
    //printf("tensor creado\n");
    image_ready = true;
    multicore_fifo_push_blocking(0x01);
  } 

}

void extractFramebuffer()
{
  int j = 12450;

  for(int i = 0; i < (2048); i++)
  {
    small_Framebuffer[i] = frame_buffer[j];

    if(i%64 == 0)
    {
      j = j+193;
    }
    else 
    {
      j++;
    }
  }
}

void createTensor()
{
  for (int index = 0; index < CUT_IMAGE_LENGTH-1; index++)
  {
    rgb888_t color = extendRGB(small_Framebuffer[index],small_Framebuffer[index + 1]);
    for (int i = 0; i < 32; i++)
    {
      for (int j = 0; j < 32; j++) 
      {
        image_tensor[0][i][j][0] = color.r/255.0f;
        image_tensor[0][i][j][1] = color.g/255.0f;
        image_tensor[0][i][j][2] = color.b/255.0f;
      }
    }
  }
}

rgb888_t extendRGB(uint8_t lowByte, uint8_t highByte)
{
  uint16_t rgb565;
  
  rgb565 = ((highByte + 1) << 8) | lowByte;
  rgb888_t resultado;
  resultado.r = ((rgb565 >> 11) & 0x1F) << 3;
  resultado.g = ((rgb565 >> 5) & 0X3F) << 2;
  resultado.b = (rgb565 & 0x1F) << 3;
  return resultado;
}

