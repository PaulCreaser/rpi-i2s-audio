/*
 * =====================================================================================
 *
 *       Filename:  test.c
 *
 *    Description:  
 *
 *        Version:  1.0
 *        Created:  06/02/16 16:46:13
 *       Revision:  none
 *       Compiler:  gcc
 *
 *         Author:  YOUR NAME (), 
 *   Organization:  
 *
 * =====================================================================================
 */
#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/kmod.h>
#include <linux/platform_device.h>
#include <sound/simple_card.h>
#include <linux/delay.h>
/*
 * modified for linux 4.1.5
 * inspired by https://github.com/msperl/spi-config
 * with thanks for https://github.com/notro/rpi-source/wiki
 * as well as Florian Meier for the rpi i2s and dma drivers
 *
 * to use a differant (simple-card compatible) codec
 * change the codec name string in two places and the
 * codec_dai name string. (see codec's source file)
 *
 *
 * N.B. playback vs capture is determined by the codec choice
 * */

void device_release_callback(struct device *dev) { /*  do nothing */ };
static struct asoc_simple_card_info snd_rpi_simple_card_info = {
.card = "snd_rpi_simple_card", // -> snd_soc_card.name
.name = "simple-card_codec_link", // -> snd_soc_dai_link.name
.codec = "snd-soc-dummy", // "dmic-codec", // -> snd_soc_dai_link.codec_name
.platform = "3f203000.i2s",
.daifmt = SND_SOC_DAIFMT_I2S | SND_SOC_DAIFMT_NB_NF | SND_SOC_DAIFMT_CBS_CFS,
.cpu_dai = {
.name = "3f203000.i2s", // -> snd_soc_dai_link.cpu_dai_name
.sysclk = 0 },
.codec_dai = {
.name = "snd-soc-dummy-dai", //"dmic-codec", // -> snd_soc_dai_link.codec_dai_name
.sysclk = 0 },
};
static struct platform_device snd_rpi_simple_card_device = {
.name = "asoc-simple-card", //module alias
.id = 0,
.num_resources = 0,
.dev = { .release = &device_release_callback,
.platform_data = &snd_rpi_simple_card_info, // *HACK ALERT*
},
};


int hello_init(void)
{
const char *dmaengine = "bcm2708-dmaengine"; //module name
int ret;

ret = request_module(dmaengine);
pr_alert("request module load '%s': %d\n",dmaengine, ret);
ret = platform_device_register(&snd_rpi_simple_card_device);
pr_alert("register platform device '%s': %d\n",snd_rpi_simple_card_device.name, ret);

pr_alert("Hello World :)\n");
return 0;
}

void hello_exit(void)
{// you'll have to sudo modprobe -r the card & codec drivers manually (first?)
platform_device_unregister(&snd_rpi_simple_card_device);
pr_alert("Goodbye World!\n");
}
module_init(hello_init);
module_exit(hello_exit);
MODULE_DESCRIPTION("ASoC simple-card I2S setup");
MODULE_AUTHOR("Plugh Plover");
MODULE_LICENSE("GPL v2");

