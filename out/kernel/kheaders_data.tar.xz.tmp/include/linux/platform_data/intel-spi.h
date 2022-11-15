

#ifndef INTEL_SPI_PDATA_H
#define INTEL_SPI_PDATA_H

enum intel_spi_type {
	INTEL_SPI_BYT = 1,
	INTEL_SPI_LPT,
	INTEL_SPI_BXT,
};


struct intel_spi_boardinfo {
	enum intel_spi_type type;
	bool writeable;
};

#endif 
