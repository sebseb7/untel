#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //usleep()

#include "dmx_output.h"


#include "libftdi1/ftdi.h"


static unsigned int initialized = 0;
static struct ftdi_context *ftdi;
	
void dmx_output_init(void)
{
	initialized=0;
	int ret;
	if ((ftdi = ftdi_new()) == 0)
	{
		fprintf(stderr, "ftdi_new failed\n");
		return;
	}
	if ((ret = ftdi_usb_open(ftdi, 0x0403, 0x6001)) < 0)
	{
		fprintf(stderr, "unable to open ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
		ftdi_free(ftdi);
		return;
	}
	// Read out FTDIChip-ID of R type chips
	if (ftdi->type == TYPE_R)
	{
		unsigned int chipid;
		printf("ftdi_read_chipid: %d\n", ftdi_read_chipid(ftdi, &chipid));
		printf("FTDI chipid: %X\n", chipid);
	}
	ret = ftdi_set_line_property(ftdi, 8, STOP_BIT_2, NONE);
	if (ret < 0)
	{
		fprintf(stderr, "unable to set line parameters: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
		ftdi_free(ftdi);
		return;
	}
	ret = ftdi_set_baudrate(ftdi, 250000);
	if (ret < 0)
	{
		fprintf(stderr, "unable to set baudrate: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
		ftdi_free(ftdi);
		return;
	}
	initialized=1;
}

void dmx_output_send(unsigned char* out)
{
	if(initialized==0)
	{
		dmx_output_init();
	}
	if(initialized==0)
	{
		return;
	}
	int ret;
	ret = ftdi_set_line_property2(ftdi, 8, STOP_BIT_2, NONE,BREAK_ON);
	if (ret < 0)
	{
		fprintf(stderr, "unable to set line parameters: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
		ftdi_free(ftdi);
		initialized=0;
		return;
	}
	usleep(100);
	ret = ftdi_set_line_property2(ftdi, 8, STOP_BIT_2, NONE,BREAK_OFF);
	if (ret < 0)
	{
		fprintf(stderr, "unable to set line parameters: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
		ftdi_free(ftdi);
		initialized=0;
		return;
	}
	unsigned char c=0;

	ret = ftdi_write_data(ftdi, &c, 0);
	if (ret < 0)
	{
		fprintf(stderr,"write failed , error %d (%s)\n",ret, ftdi_get_error_string(ftdi));
		ftdi_free(ftdi);
		initialized=0;
		return;
	}
	usleep(10);

	ret = ftdi_write_data(ftdi, out, 65);
	if (ret < 0)
	{
		fprintf(stderr,"write failed , error %d (%s)\n",ret, ftdi_get_error_string(ftdi));
		ftdi_free(ftdi);
		initialized=0;
		return;
	}
}

void dmx_output_deinit(void)
{
	int ret;

	if ((ret = ftdi_usb_close(ftdi)) < 0)
	{
		fprintf(stderr, "unable to close ftdi device: %d (%s)\n", ret, ftdi_get_error_string(ftdi));
		ftdi_free(ftdi);
		exit(-1);
	}
	ftdi_free(ftdi);

}
