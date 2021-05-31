// SPDX-License-Identifier: GPL-2.0+
// static const char *__doc__ = " Trivial ktrace example";

#include <bpf/libbpf.h>
#include <stdlib.h>
#include <errno.h>

#include <stdio.h>

#define pr_err(fmt, ...) \
	fprintf(stderr, "%s:%d - " fmt, __FILE__, __LINE__, ##__VA_ARGS__)

int main(int argc, char **argv)
{
	struct bpf_object *obj = NULL;
	struct bpf_link *link = NULL;
	struct bpf_program *prog;
	char filename[256];
	//char pin_file[256];
	// char *pin_file;
	char buf[100];
	int err;
	int c;

	snprintf(filename, sizeof(filename), "%s_kern.o", argv[0]);

        obj = bpf_object__open_file(filename, NULL);
	err = libbpf_get_error(obj);
	if (err) {
		libbpf_strerror(err, buf, sizeof(buf));
		pr_err("Error(%d) opening file: %s\n", err, buf);
		goto out;
	}

	err = bpf_object__load(obj);
	if (err) {
		libbpf_strerror(err, buf, sizeof(buf));
		pr_err("Error(%d) loading: %s\n", err, buf);
		goto out;
	}

	prog = bpf_program__next(NULL, obj);
	if (!prog) {
		pr_err("No program!\n");
		err = -ENOENT;
		goto out;
	}

	link = bpf_program__attach(prog);
	err = libbpf_get_error(link);
	if (err) {
		libbpf_strerror(err, buf, sizeof(buf));
		pr_err("Error(%d) attaching: %s\n", err, buf);
		goto out;
	}

	printf("Loaded BPF file %s\n", filename);
	printf( "Press any key + enter to unload program again [Y/y]:");
	c = getchar();
	putchar(c);

	// snprintf(pin_file, sizeof(pin_file), "/sys/fs/bpf/%s", argv[0]);
	/*
	pin_file = "/sys/fs/bpf/ktrace01";
	err = bpf_link__pin(link, pin_file);
	printf("pin_file %s\n", pin_file);
	if (err) {
		libbpf_strerror(err, buf, sizeof(buf));
		pr_err("Error(%d) pinning: %s\n", err, buf);
		goto out;
	}
	printf("Loaded BPF file %s (pinned at %s)\n", filename, pin_file);
	*/
out:
	bpf_link__destroy(link);
	bpf_object__close(obj);
	if (err)
		return EXIT_FAILURE;
	return EXIT_SUCCESS;
}
