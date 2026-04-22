// Libreria para display tft con driver ST7735 de 128x160
// por: Daniel Michell Avila Garnica
/* Definición de puerto SPI */
#define DISPLAY_SPI_PORT spi0
#define DISPLAY_MISO_PIN 4
#define DISPLAY_MOSI_PIN 7
#define DISPLAY_SCK_PIN  6

#define DISPLAY_DATA_COMMAND 21
#define DISPLAY_CHIP_SELECT 5
#define DISPLAY_RESET 20 
//#ifndef TFT_DISPLAY_H
//#define TFT_DISPLAY_H
#include <stdint.h>
/* Definición de los registros de acceso al display */

typedef enum{

  NOP     = 0x00, //NO OPERATION
  SWRESET = 0x01, //SOFTWARE RESET APPLY DURING SLEEP IN MODE
  SLPIN   = 0x10, //SLEEP IN & BOOSTER OFF
  SLPOUT  = 0x11, //SLEEP OUT & BOOSTER ON
  PTLON   = 0x12, //PARTIAL MODE ON
  NORON   = 0x13, //PARTIAL MODE OFF (NORMAL)
  INVOFF  = 0x20, //DISPLAY INVERSION OFF
  INVON   = 0x21, //DISPLAY INVERSION ON.
  GAMSET  = 0x26, //GAMA CURVE SELECT
  DISPOFF = 0x28, //DISPLAY OFF
  DISPON  = 0x29, //DISPLAY ON 
  CASET   = 0x2A, //COLUMN ADDRESS SET
  RASET   = 0x2B, //ROW ADDRESS SET
  RAMWR   = 0x2C, //DISPLAY RAM MEMORY WRITE
  PTLAR   = 0x30, //PARTIAL START/END ADDRESS SET
  TEOFF   = 0x34, //TEARING EFFECT LINE OFF
  TEON    = 0x35, //TEARING EFFECT LINE ON
  MADCTL  = 0x36, //MEMORY DATA ACCESS CONTROL
  IDMOFF  = 0x38, //IDLE MODE OFF
  IDMON   = 0x39, //IDLE MODE ON
  COLMOD  = 0x3A, //INTERFACE PIXEL FORMAT
} TFT_COMMAND;

/* Definción de parámetros de pixel format */

typedef enum{
  bitdepth_12 = 0x03, //RGB444
  bitdepth_16 = 0x05, //RGB565
  bitdepth_18 = 0x06, //RGB666
}COLOR_MODE;


//void sleep_out();
//void display_on();
//void display_off();
//void hard_reset();
//void soft_reset();
void tft_spi_begin();
void set_frame_area(uint8_t xi, uint8_t xf, uint8_t yi, uint8_t yf);
void CS_mode(uint8_t mode);
void DC_mode(uint8_t mode);
//void set_color_mode(uint8_t mode);
void display_init(uint8_t color_mode);
void write_data_byte(uint8_t parameter);
void write_data_array(uint8_t *data, uint16_t lenght);
void display_reset();
void ram_write_init();
void write_data_array_16(uint16_t *data, uint16_t lenght);
//void demo_colors()
void mem_write_ctl();
