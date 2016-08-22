#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <linux/hidraw.h>
#include <stdio.h>
#include <string.h>

static int
kl_hid_open (int vid, int pid, int usage_page, int usage)
{
	struct stat devstat;
	struct hidraw_devinfo info;
	struct hidraw_report_descriptor *desc;
	char buf[512];
	const unsigned char signature[] = {0x06, 0x31, 0xFF, 0x09, 0x74};
	int r, i, fd = -1, len, found = 0;

	for (i = 0; i < HIDRAW_MAX_DEVICES; i++) {
		if (fd > 0)
      close(fd);
		snprintf(buf, sizeof(buf), "/dev/hidraw%d", i);

		r = stat(buf, &devstat);
		if (r < 0)
      continue;

    fd = open(buf, O_RDWR);
		if (fd < 0)
      continue;

    r = ioctl(fd, HIDIOCGRAWINFO, &info);
		if (r < 0)
      continue;

		r = ioctl(fd, HIDIOCGRDESCSIZE, &len);
		if (r < 0 || len < 1)
      continue;

		desc = (struct hidraw_report_descriptor *)buf;
		if (len > sizeof(buf) - sizeof(int))
      len = sizeof(buf) - sizeof(int);
		desc->size = len;
		r = ioctl(fd, HIDIOCGRDESC, desc);
		if (r < 0)
      continue;

		if (len >= sizeof(signature) &&
		   memcmp(desc->value, signature, sizeof(signature)) == 0) {
			found = 1;
			break;
		}
	}

	if (!found) {
		if (fd > 0)
      close(fd);
		return -1;
	}
  return fd;
}

static int
kl_hid_read(int fd, void *buf, int bufsize)
{
	int num;

	if (fd < 0)
    return -1;

	while (1) {
		num = read(fd, buf, bufsize);
		if (num < 0) {
			if (errno == EINTR || errno == EAGAIN)
        continue;

			return -errno;
		}
		return num;
	}
}

static void
kl_hid_close(int fd)
{
	if (fd < 0)
    return;
	close(fd);
}

int
main(void)
{
	char buf[128], *in, *out;
	int num, count, fd;

	while (1) {
		fd = kl_hid_open(0, 0, 0xFF31, 0x0074);
		if (fd < 0) {
      sleep(1);
			continue;
		}

		while (1) {
			num = kl_hid_read(fd, buf, sizeof(buf));
			if (num < 0)
        break;
			if (num == 0)
        continue;

			in = out = buf;
			for (count=0; count<num; count++) {
				if (*in) {
					*out++ = *in;
				}
				in++;
			}
			count = out - buf;
			if (count) {
				num = write(1, buf, count);
				fflush(stdout);
        printf("\n");
			}
		}
		kl_hid_close(fd);
	}
	return 0;
}
