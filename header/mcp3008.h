#include <unistd.h>
#include <stdint.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/spi/spidev.h>
#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>

struct spi_setting {
	const char *device;
	unsigned int mode;
	unsigned int bitsPerWord;
	unsigned int speed;
	unsigned int delay;
};

int spiWriteRead(int fd, unsigned char *data, int length, struct spi_setting spi_set){
	
	struct spi_ioc_transfer spi[length];
	memset(&spi, 0, sizeof(spi));
	int i = 0;
	int retVal = -1;

	// one spi transfer for each byte

	for (i = 0 ; i < length ; i++){

		spi[i].tx_buf        = (unsigned long)(data + i); // transmit from "data"
		spi[i].rx_buf        = (unsigned long)(data + i) ; // receive into "data"
		spi[i].len           = sizeof(*(data + i)) ;
		spi[i].delay_usecs   = 0 ;
		spi[i].speed_hz      = spi_set.speed ;
		spi[i].bits_per_word = spi_set.bitsPerWord ;
		spi[i].cs_change = 0;
	}

	retVal = ioctl (fd, SPI_IOC_MESSAGE(length), &spi) ;

	if(retVal < 0){
		perror("Problem transmitting spi data..ioctl");
		exit(1);
	}

	return retVal;

}

int spiOpen(struct spi_setting spi_set){
    int statusVal = -1;
    int fd = open("/dev/spidev0.0", O_RDWR);
    if(fd < 0){
        perror("could not open SPI device");
        exit(1);
    }

    statusVal = ioctl (fd, SPI_IOC_WR_MODE, &(spi_set.mode));
    if(statusVal < 0){
        perror("Could not set SPIMode (WR)...ioctl fail");
        exit(1);
    }

    statusVal = ioctl (fd, SPI_IOC_RD_MODE, &(spi_set.mode));
    if(statusVal < 0) {
      perror("Could not set SPIMode (RD)...ioctl fail");
      exit(1);
    }

    statusVal = ioctl (fd, SPI_IOC_WR_BITS_PER_WORD, &(spi_set.bitsPerWord));
    if(statusVal < 0) {
      perror("Could not set SPI bitsPerWord (WR)...ioctl fail");
      exit(1);
    }

    statusVal = ioctl (fd, SPI_IOC_RD_BITS_PER_WORD, &(spi_set.bitsPerWord));
    if(statusVal < 0) {
      perror("Could not set SPI bitsPerWord(RD)...ioctl fail");
      exit(1);
    }

    statusVal = ioctl (fd, SPI_IOC_WR_MAX_SPEED_HZ, &(spi_set.speed));
    if(statusVal < 0) {
      perror("Could not set SPI speed (WR)...ioctl fail");
      exit(1);
    }

    statusVal = ioctl (fd, SPI_IOC_RD_MAX_SPEED_HZ, &(spi_set.speed));
    if(statusVal < 0) {
      perror("Could not set SPI speed (RD)...ioctl fail");
      exit(1);
    }
    return fd;
}
