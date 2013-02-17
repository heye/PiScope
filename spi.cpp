#include "spi.h"

#include <stdint.h>
//#include <unistd.h>
//#include <getopt.h>
#include <fcntl.h>
#include <sys/ioctl.h>
//#include <linux/types.h>
//#include <linux/spi/spidev.h>
#include "spidev.h"
 

#include <iostream>
using namespace std;
#define ARRAY_SIZE(a) (sizeof(a) / sizeof((a)[0]))

spi::spi(uint32_t speed){
	mSpeed = speed;
	mDelay = 0;
	mBits = 8;
	mBuffLen = BUFFLEN;
	
	mRx = new uint8_t[BUFFLEN*3];
	mTx = new uint8_t[BUFFLEN*3];
	
	mCHA = new uint16_t[BUFFLEN];
	mCHB = new uint16_t[BUFFLEN];	
	
	for(int i=0; i<BUFFLEN; i++){
		mRx[i] = 42;
		mTx[i] = 42;
	}
	
	
	mDevice = open("/dev/spidev0.0", O_RDWR);
	if (mDevice < 0)
		error("can't open device");
		
	
	int ret = 0;
	static uint8_t mode;
		
	mode |= SPI_CPHA;
	//mode |= SPI_READY;
	
	/*
	 * spi mode
	 */
	ret = ioctl(mDevice, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		error("can't set spi mode");

	ret = ioctl(mDevice, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		error("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(mDevice, SPI_IOC_WR_BITS_PER_WORD, &mBits);
	if (ret == -1)
		error("can't set bits per word");

	ret = ioctl(mDevice, SPI_IOC_RD_BITS_PER_WORD, &mBits);
	if (ret == -1)
		error("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(mDevice, SPI_IOC_WR_MAX_SPEED_HZ, &mSpeed);
	if (ret == -1)
		error("can't set max speed hz");

	ret = ioctl(mDevice, SPI_IOC_RD_MAX_SPEED_HZ, &mSpeed);
	if (ret == -1)
		error("can't get max speed hz");
	
	/*
	uint8_t tx[] = {
		0x01
	};	
	uint8_t rx[ARRAY_SIZE(tx)] = {0, };
	struct spi_ioc_transfer tr;
		tr.tx_buf = (unsigned long)tx;
		tr.rx_buf = (unsigned long)rx;
		tr.len = ARRAY_SIZE(tx);
		tr.delay_usecs = 0;
		tr.speed_hz = (unsigned long)16000000;
		tr.bits_per_word = (uint8_t)8;

	//perform actual read/write
	ret = ioctl(mDevice, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		error("cant't send spi message");
	else
		error("message sent");
	*/
	
}
spi::~spi(){
	close(mDevice);
	delete mRx;
	delete mTx;
}

uint8_t* spi::getRX(){
	return mRx;
}

uint16_t* spi::getCHA(){
	return mCHA;
}
uint16_t* spi::getCHB(){
	return mCHB;
}

void spi::error(const char* s){
	std::cout << "SPI Error: "<< s << endl;
};

int spi::getBuffLen(){
	return mBuffLen;
}
void spi::print(){
	for(int i=0; i < 10; i++){
		cout << mRx[i] << endl;
	}
}

void spi::read(){	
	int ret;
	/*uint8_t tx[] = {
		0x01, 0x02, 0x03, 0x04, 0x05, 0x06, 0x07, 0x08, 0x09, 
		0x0A, 0x0B, 0x0C, 0x0D, 0x0E, 0x0F, 0x10, 0x11, 0x12, 
		0x13, 0x14, 0x15, 0x16, 0x17, 0x18, 0x19, 0x1A, 0x1B, 
		0x1C, 0x1D, 0x1E, 0x1F
	};	*/
	//uint8_t rx[ARRAY_SIZE(mTx)] = {0, };
	
	
	int spiNumTransmissions = (BUFFLEN*3)/4096;
	int spiTransmissionLen = 4096;
	int spiLastTransmissionLen = BUFFLEN*3 - spiTransmissionLen*spiNumTransmissions;
	
	//transmissions
	for(int i = 0; i < spiNumTransmissions; i++){
	
	struct spi_ioc_transfer tr;   
		tr.tx_buf = (unsigned long)mTx+i*spiTransmissionLen;
		tr.rx_buf = (unsigned long)mRx+i*spiTransmissionLen;
		tr.len = spiTransmissionLen ;
		tr.delay_usecs = mDelay;
		tr.speed_hz = mSpeed;
		tr.bits_per_word = mBits;
	
	ret = ioctl(mDevice, SPI_IOC_MESSAGE(1), &tr);
	
	if (ret < 1){
		error("cant't send spi message1");
	}
	}
	
	if(spiLastTransmissionLen > 0){
	//last transmission
	struct spi_ioc_transfer tr;   
		tr.tx_buf = (unsigned long)(mTx+spiNumTransmissions*spiTransmissionLen);
		tr.rx_buf = (unsigned long)(mRx+spiNumTransmissions*spiTransmissionLen);
		tr.len = spiLastTransmissionLen ;
		tr.delay_usecs = mDelay;
		tr.speed_hz = mSpeed;
		tr.bits_per_word = mBits;
	
	ret = ioctl(mDevice, SPI_IOC_MESSAGE(1), &tr);
	
	if (ret < 1){
		error("cant't send spi message2");
	}
	}
	
	for(int i=0, j=0; j < BUFFLEN; i+=3, j++){
		mCHB[j] = mCHA[j] = ((mRx[i+1] << 7) + mRx[i+2] >> 1) & 0b0000001111111111 ;
	}
	
	
	/*
	//10 mal 4096 bytes lesen
	for(int i=0; i<8; i++){
		//perform actual read/write
		ret = ioctl(mDevice, SPI_IOC_MESSAGE(1), &tr);
		//ret = ioctl(mDevice, SPI_IOC_MESSAGE(2), &tr[1]);
	
		if (ret < 1)
		error("cant't send spi message");
	}*/
		
	//pabort("can't send spi message");

/*	for (ret = 0; ret < ARRAY_SIZE(tx); ret++) {
		if (!(ret % 6))
			puts("");
		printf("%.2X ", rx[ret]);
	}*/
	
}