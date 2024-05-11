#include "max6675.h"
#include "tools.h"

extern unsigned long microsRp(void);
extern unsigned long millisRp(void);

max6675::max6675() { m_prevTempMicros = 0; }


void max6675::begin(uint8_t SCK, uint8_t RX, uint8_t CS) {
  m_SCK = SCK;
  m_RX = RX;
  m_CS = CS;

  spi_init(spi0, 500 * 1000);  // 500 kHz
  gpio_set_function(8, GPIO_FUNC_SPI);
  gpio_set_function(10, GPIO_FUNC_SPI);

  gpio_init(9);
  gpio_set_dir(9, GPIO_OUT);

}  // end of begin

bool max6675::readC() {
  if ((microsRp() - m_prevTempMicros) < 5000000) return false;

  m_prevTempMicros = microsRp();

  gpio_put(m_CS, 0);
  uint16_t data;
  uint8_t MSB;
  uint8_t LSB;
  spi_read_blocking(spi0, 0, (uint8_t*)&MSB, sizeof(MSB));
  spi_read_blocking(spi0, 0, (uint8_t*)&LSB, sizeof(LSB));

  gpio_put(m_CS, 1);
  data = (MSB << 8) | LSB;

  float temperature = data & 0x4 ? 0 : (data >> 3) * 0.25;

  if (temperature != 0) {
    sent2Core0(TEMP2, temperature);
  }

  return true;
}
