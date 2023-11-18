#pragma once

#include "stdint.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"


class max6675 {
public:
  max6675();
  void begin(uint8_t SCK, uint8_t RX,uint8_t CS);
  bool readC();
  
private:
  uint8_t m_SCK;
  uint8_t m_RX;
  uint8_t m_CS;
  
  uint32_t m_prevTempMicros;

      
}; 





