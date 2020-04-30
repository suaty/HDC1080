/*
 * i2cmaster.c
 *
 * Created: 29.04.2020 20:14:16
 *  Author: suat.yildirmaz

 Sample Pseudo Code

		char TxStr[50];
		HDC_Config();
		HDC_Read_Temperature();
		snprintf(TxStr, 13, "Temp=%f4.2\n\0", HDC_Read_Temperature());
		Uart1Write(TxStr);
		snprintf(TxStr, 13, "Humd=%f4.2\n\0", HDC_Read_Humidty());
		Uart1Write(TxStr);


 */ 

 #include <stdio.h>
 #include <asf.h>
 #include <avr/io.h>
 #include <util/delay.h>
 #include <i2cmaster.h>

 void TWI_Init(void)
 {
	 TWSR = 0x00;
	 TWBR = 0xC0;
	 TWCR |= (1 << TWEN);
 }

 uint8_t TWI_Start(void)
 {
	 TWCR = (1 << TWINT) | (1 << TWSTA) | (1 << TWEN);		// Send Start Condition
	 while( !(TWCR & (1 << TWINT)) );						// wait until transmission completed
	 return 0;
 }

 void TWI_Stop(void)
 {
	 TWCR = (1 << TWINT) | (1 << TWSTO) | (1 << TWEN);	
	 while(TWCR & (1<<TWSTO));			// wait until stop condition is executed and bus released
 }

 void TWI_Write(uint8_t u8data)
 {
	 TWDR = u8data;
	 TWCR = (1 << TWINT) | (1 << TWEN);
	 while ( (TWCR & (1 << TWINT)) == 0);
 }

 uint8_t TWIReadACK(void)
 {
	 TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);
	 while ((TWCR & (1<<TWINT)) == 0);
	 return TWDR;
 }

 //read byte with NACK
 uint8_t TWIReadNACK(void)
 {
	 TWCR = (1<<TWINT)|(1<<TWEN);
	 while ((TWCR & (1<<TWINT)) == 0);
	 return TWDR;
 }

 uint8_t TWIGetStatus(void)
 {
	 uint8_t status;	 
	 status = TWSR & 0xF8;			//mask status
	 return status;
 }

 uint8_t HDC_Config(void)
 {
	 TWI_Start();
	 TWI_Write(HDC1080_ADDRESS_W);
	 TWI_Write(HDC1080_CONFIG_PTR);		 //Config Register of HDC1080
	 TWI_Write(0x10);					//Temp and Humidty are 14bits
	 TWI_Write(0);
	 TWI_Stop();
	 
	 return 0;
 }

 // Read Temperature
 float HDC_Read_Temperature(void)
 {
	 uint16_t I2CData = 0;

	 TWI_Start();
	 TWI_Write(HDC1080_ADDRESS_W);				// Address to write
	 TWI_Write(HDC1080_TEMPERATURE_PTR);		// Address of the temperature
	 delay_ms(20);
	 TWI_Start();
	 TWI_Write(HDC1080_ADDRESS_R);				// Address to read
	 I2CData = 0;
	 I2CData = TWIReadACK()  << 8;
	 I2CData |= TWIReadNACK();	 
	 TWI_Stop();
	 
	 return (float)(( (float)I2CData * 165.0 / 65536.0) - 40.0);
 }

 //Read Humidty
  float HDC_Read_Humidty(void)
 {
	 uint16_t I2CData = 0;

	 TWI_Start();
	 TWI_Write(HDC1080_ADDRESS_W);				 // Address to write
	 TWI_Write(HDC1080_HUMIDITY_PTR);			// Address of the temperature
	 delay_ms(20);
	 TWI_Start();
	 TWI_Write(HDC1080_ADDRESS_R);		      // Address to read
	 I2CData = 0;
	 I2CData = TWIReadACK()  << 8;
	 I2CData |= TWIReadNACK();	 
	 TWI_Stop();	 
	 return (float)(( (float)I2CData * 100.0 / 65536.0));
 }