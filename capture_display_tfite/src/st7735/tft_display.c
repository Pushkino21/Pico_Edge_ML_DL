#include "pico/stdlib.h"
#include "string.h"
#include "pico/malloc.h"
#include "pico/binary_info.h"
#include "hardware/irq.h"
#include "hardware/spi.h"
#include "tft_display.h"

#define DISPLAY_SPI_PORT spi0
#define DISPLAY_MISO_PIN 4
#define DISPLAY_MOSI_PIN 7
#define DISPLAY_SCK_PIN  6

#define DISPLAY_DATA_COMMAND 21
#define DISPLAY_CHIP_SELECT 5
#define DISPLAY_RESET 20 

/* Funciones inicio spi*/

void tft_spi_begin()
{
  //Pines de SPI0
  spi_init(DISPLAY_SPI_PORT, 4000000);
  spi_set_format(DISPLAY_SPI_PORT, 
                   8,           // 8 bits por transferencia
                   SPI_CPOL_0,  // Polaridad del reloj (prueba 0 y 1)
                   SPI_CPHA_0,  // Fase del reloj (prueba 0 y 1)
                   SPI_MSB_FIRST);
  gpio_set_function(DISPLAY_MISO_PIN, GPIO_FUNC_SPI);
  gpio_set_function(DISPLAY_MOSI_PIN, GPIO_FUNC_SPI);
  gpio_set_function(DISPLAY_SCK_PIN, GPIO_FUNC_SPI);
  
  // Data/Command Chip select Reset GPIO definitions 
  gpio_init(DISPLAY_DATA_COMMAND);
  gpio_set_dir(DISPLAY_DATA_COMMAND,GPIO_OUT);
  gpio_init(DISPLAY_CHIP_SELECT);
  gpio_set_dir(DISPLAY_CHIP_SELECT,GPIO_OUT);
  gpio_init(DISPLAY_RESET);
  gpio_set_dir(DISPLAY_RESET,GPIO_OUT);
}





/* Funciones tft_display*/

void CS_mode(uint8_t mode)
{
 //Habilitar comunicacion cs = 0 deshabilitar cs = 1
 gpio_put(DISPLAY_CHIP_SELECT,mode);
}

void DC_mode(uint8_t mode)
{
  //Modo command = 0, modo parameter = 1
  gpio_put(DISPLAY_DATA_COMMAND,mode);
}


static void write_command_mode(uint8_t cmd)
{
  //Command mode
  CS_mode(0);
  DC_mode(0);
  //tft_write(&cmd,1);    
  spi_write_blocking(DISPLAY_SPI_PORT, &cmd, 1);
  CS_mode(1);
}

void write_data_byte(uint8_t parameter)
{
  //Parameter (data) mode
  CS_mode(0);
  DC_mode(1);
  spi_write_blocking(DISPLAY_SPI_PORT, &parameter, 1);
  CS_mode(0);
  //DC_mode(0);
}

void write_data_array(uint8_t *data, uint16_t lenght)
{
  //Parameter (data) mode
  CS_mode(0);
  DC_mode(1);
  spi_write_blocking(DISPLAY_SPI_PORT, data, lenght);
  CS_mode(0);
  //DC_mode(0);
}

void write_data_array_16(uint16_t *data, uint16_t lenght)
{
  //Parameter (data) mode
  CS_mode(0);
  DC_mode(1);
  spi_write16_blocking(DISPLAY_SPI_PORT, data, lenght);
  CS_mode(0);
  //DC_mode(0);
}




static void sleep_out()
{
  write_command_mode(SLPOUT);
}

static void display_on()
{
  write_command_mode(DISPON);
}

static void display_off()
{
  write_command_mode(DISPOFF);
}

static void hard_reset()
{
  gpio_put(DISPLAY_RESET,0);
  sleep_ms(100);
  gpio_put(DISPLAY_RESET,1);
}

static void soft_reset()
{
  write_command_mode(SWRESET);
}

void display_reset()
{
  hard_reset();
  soft_reset();
}

void ram_write_init()
{
  write_command_mode(RAMWR);
}

void set_frame_area(uint8_t xi, uint8_t xf, uint8_t yi, uint8_t yf)
{
  uint8_t dummy_byte = 0x00;
  write_command_mode(CASET);

  write_data_byte(dummy_byte);
  write_data_byte(xi);

  write_data_byte(dummy_byte);
  write_data_byte(xf);

  //sleep_ms(100);

  write_command_mode(RASET);

  write_data_byte(dummy_byte);
  write_data_byte(yi);

  write_data_byte(dummy_byte);
  write_data_byte(yf);

  //sleep_ms(100);
}
static void set_color_mode(uint8_t mode)
{
  write_command_mode(COLMOD);
  write_data_byte(mode);
}

static void tear_off()
{
  write_command_mode(TEOFF);
}

void display_init(uint8_t color_mode)
{
  hard_reset();
  sleep_ms(200);
  soft_reset();
  sleep_ms(200);
  sleep_out();
  sleep_ms(200);
  set_color_mode(color_mode);
  tear_off();
  sleep_ms(200);
  display_on();
  sleep_ms(200);
}
