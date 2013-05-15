/*
 * Copyright (c) 2012, Google Inc.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <fs.h>
#include <part.h>
#include <sandbox-blockdev.h>

static int do_sandbox_load(cmd_tbl_t *cmdtp, int flag, int argc,
			   char * const argv[])
{
	return do_load(cmdtp, flag, argc, argv, FS_TYPE_SANDBOX, 16);
}

static int do_sandbox_ls(cmd_tbl_t *cmdtp, int flag, int argc,
			   char * const argv[])
{
	return do_ls(cmdtp, flag, argc, argv, FS_TYPE_SANDBOX);
}

static int do_sandbox_save(cmd_tbl_t *cmdtp, int flag, int argc,
			   char * const argv[])
{
	return do_save(cmdtp, flag, argc, argv, FS_TYPE_SANDBOX, 16);
}

static int do_sandbox_bind(cmd_tbl_t *cmdtp, int flag, int argc,
			   char * const argv[])
{
	char *ep;
	char *dev_str = argv[1];
	char *file = NULL;
	int dev;

	if (argc < 2 || argc > 3)
		return CMD_RET_USAGE;

	if (argc >= 3)
		file = argv[2];
	dev = simple_strtoul(dev_str, &ep, 16);
	if (*ep) {
		printf("** Bad device specification %s **\n", dev_str);
		return CMD_RET_USAGE;
	}
	if (host_dev_bind(dev, file) != 0)
		return 0;
	else
		return 1;
}

static int do_sandbox_info(cmd_tbl_t *cmdtp, int flag, int argc,
			   char * const argv[])
{
	int min_dev = 0;
	int max_dev = CONFIG_HOST_MAX_DEVICES - 1;
	int dev;

	if (argc < 1 || argc > 2)
		return CMD_RET_USAGE;

	if (argc >= 2) {
		char *ep;
		char *dev_str = argv[1];
		int dev = simple_strtoul(dev_str, &ep, 16);

		if (*ep) {
			printf("** Bad device specification %s **\n", dev_str);
			return CMD_RET_USAGE;
		}
		min_dev = dev;
		max_dev = dev;
	}
	printf("%3s %12s %s\n", "dev", "blocks", "path");
	for (dev = min_dev; dev <= max_dev; dev++) {
		printf("%3d ", dev);
		block_dev_desc_t *blk_dev = host_get_dev(dev);
		if (!blk_dev)
			continue;
		struct host_block_dev *host_dev = blk_dev->priv;
		printf("%12lu %s\n", (unsigned long)blk_dev->lba,
		       host_dev->filename);
	}
	return 0;
}

static cmd_tbl_t cmd_sandbox_sub[] = {
	U_BOOT_CMD_MKENT(load, 7, 0, do_sandbox_load, "", ""),
	U_BOOT_CMD_MKENT(ls, 3, 0, do_sandbox_ls, "", ""),
	U_BOOT_CMD_MKENT(save, 6, 0, do_sandbox_save, "", ""),
	U_BOOT_CMD_MKENT(bind, 3, 0, do_sandbox_bind, "", ""),
	U_BOOT_CMD_MKENT(info, 3, 0, do_sandbox_info, "", ""),
};

static int do_sandbox(cmd_tbl_t *cmdtp, int flag, int argc,
		      char * const argv[])
{
	cmd_tbl_t *c;

	/* Skip past 'sandbox' */
	argc--;
	argv++;

	c = find_cmd_tbl(argv[0], cmd_sandbox_sub,
			 ARRAY_SIZE(cmd_sandbox_sub));

	if (c)
		return c->cmd(cmdtp, flag, argc, argv);
	else
		return CMD_RET_USAGE;
}

U_BOOT_CMD(
	sb,	8,	1,	do_sandbox,
	"Miscellaneous sandbox commands",
	"load hostfs 0 <addr> <filename> [<bytes> <offset>]  - "
		"load a file from host\n"
	"sb ls hostfs 0 <filename>                      - list files on host\n"
	"sb save hostfs 0 <filename> <addr> <bytes> [<offset>] - "
		"save a file to host\n"
	"sb bind <dev> [<filename>] - bind \"host\" device to file\n"
	"sb info [<dev>]            - show device binding & info"
);
