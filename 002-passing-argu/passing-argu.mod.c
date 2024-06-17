#include <linux/module.h>
#define INCLUDE_VERMAGIC
#include <linux/build-salt.h>
#include <linux/elfnote-lto.h>
#include <linux/export-internal.h>
#include <linux/vermagic.h>
#include <linux/compiler.h>

#ifdef CONFIG_UNWINDER_ORC
#include <asm/orc_header.h>
ORC_HEADER;
#endif

BUILD_SALT;
BUILD_LTO_INFO;

MODULE_INFO(vermagic, VERMAGIC_STRING);
MODULE_INFO(name, KBUILD_MODNAME);

__visible struct module __this_module
__section(".gnu.linkonce.this_module") = {
	.name = KBUILD_MODNAME,
	.init = init_module,
#ifdef CONFIG_MODULE_UNLOAD
	.exit = cleanup_module,
#endif
	.arch = MODULE_ARCH_INIT,
};

#ifdef CONFIG_RETPOLINE
MODULE_INFO(retpoline, "Y");
#endif



static const struct modversion_info ____versions[]
__used __section("__versions") = {
	{ 0xd6112f8d, "param_set_int" },
	{ 0x122c3a7e, "_printk" },
	{ 0x4f090d86, "param_get_int" },
	{ 0xf5bdfeca, "param_ops_int" },
	{ 0x654fa647, "param_array_ops" },
	{ 0x18bcd516, "param_ops_charp" },
	{ 0xe478ef45, "module_layout" },
};

MODULE_INFO(depends, "");


MODULE_INFO(srcversion, "319B1FCA250E141F4B015FB");
