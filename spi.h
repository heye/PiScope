#ifndef SPI_H
#define SPI_H

#include <stdint.h>
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
		
		int mBuffLen;
		
		int mDevice;
		void error(const char*);
	public:
		spi(uint32_t);
		~spi();
		void read();
		uint8_t* getRX();
		int getBuffLen();
		
		void print();
};

#endif