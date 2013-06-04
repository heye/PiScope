#ifndef SPI_H
#define SPI_H

#include <stdint.h>

// bufflen ist länge an messwerten in cha, chb
#define BUFFLEN 4096 

class spi{
	private:
		uint32_t mSpeed;
		uint16_t mDelay;
		uint8_t mBits;
		//uint8_t mRx[BUFFLEN];
		//uint8_t mTx[BUFFLEN];
		
		uint8_t* mRx;
		uint8_t* mTx;
		
		uint16_t* mCHA;
		uint16_t* mCHB;
		
		int mBuffLen;
		
		int mDevice;
		void error(const char*);
	public:
		spi(uint32_t);
		~spi();
		void read();
		void setSampleDelay(uint16_t sampleDelay);
		
		uint8_t* getRX();
		
		uint16_t* getCHA();
		uint16_t* getCHB();
		
		int getBuffLen();
		
		void print();
};

#endif