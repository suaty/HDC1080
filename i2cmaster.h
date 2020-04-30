/*
 * i2cmaster.h
 *
 * Created: 28.04.2020 21:14:51
 *  Author: suat.yildirmaz
 */ 


#ifndef i2cmaster_H_
#define i2cmaster_H_

#define HDC1080_ADDRESS_W          0b10000000
#define HDC1080_ADDRESS_R          0b10000001
#define HDC1080_TEMPERATURE_PTR    0
#define HDC1080_HUMIDITY_PTR       1
#define HDC1080_CONFIG_PTR         2
#define HDC1080_DEVICE_ID_PTR      0xff


uint8_t TWIGetStatus(void);
uint8_t TWIReadNACK(void);
uint8_t TWIReadACK(void);
void TWI_Write(uint8_t u8data);
void TWI_Stop(void);
uint8_t TWI_Start(void);
void TWI_Init(void);
uint8_t HDC_Config(void);
float HDC_Read_Temperature(void);
float HDC_Read_Humidty(void);

#endif /* i2cmaster */