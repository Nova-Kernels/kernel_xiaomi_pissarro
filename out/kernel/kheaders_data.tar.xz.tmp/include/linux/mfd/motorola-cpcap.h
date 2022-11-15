

#include <linux/device.h>
#include <linux/regmap.h>

#define CPCAP_VENDOR_ST		0
#define CPCAP_VENDOR_TI		1

#define CPCAP_REVISION_MAJOR(r)	(((r) >> 4) + 1)
#define CPCAP_REVISION_MINOR(r)	((r) & 0xf)

#define CPCAP_REVISION_1_0	0x08
#define CPCAP_REVISION_1_1	0x09
#define CPCAP_REVISION_2_0	0x10
#define CPCAP_REVISION_2_1	0x11


#define CPCAP_REG_INT1		0x0000	
#define CPCAP_REG_INT2		0x0004	
#define CPCAP_REG_INT3		0x0008	
#define CPCAP_REG_INT4		0x000c	
#define CPCAP_REG_INTM1		0x0010	
#define CPCAP_REG_INTM2		0x0014	
#define CPCAP_REG_INTM3		0x0018	
#define CPCAP_REG_INTM4		0x001c	
#define CPCAP_REG_INTS1		0x0020	
#define CPCAP_REG_INTS2		0x0024	
#define CPCAP_REG_INTS3		0x0028	
#define CPCAP_REG_INTS4		0x002c	
#define CPCAP_REG_ASSIGN1	0x0030	
#define CPCAP_REG_ASSIGN2	0x0034	
#define CPCAP_REG_ASSIGN3	0x0038	
#define CPCAP_REG_ASSIGN4	0x003c	
#define CPCAP_REG_ASSIGN5	0x0040	
#define CPCAP_REG_ASSIGN6	0x0044	
#define CPCAP_REG_VERSC1	0x0048	
#define CPCAP_REG_VERSC2	0x004c	

#define CPCAP_REG_MI1		0x0200	
#define CPCAP_REG_MIM1		0x0204	
#define CPCAP_REG_MI2		0x0208	
#define CPCAP_REG_MIM2		0x020c	
#define CPCAP_REG_UCC1		0x0210	
#define CPCAP_REG_UCC2		0x0214	

#define CPCAP_REG_PC1		0x021c	
#define CPCAP_REG_PC2		0x0220	
#define CPCAP_REG_BPEOL		0x0224	
#define CPCAP_REG_PGC		0x0228	
#define CPCAP_REG_MT1		0x022c	
#define CPCAP_REG_MT2		0x0230	
#define CPCAP_REG_MT3		0x0234	
#define CPCAP_REG_PF		0x0238	

#define CPCAP_REG_SCC		0x0400	
#define CPCAP_REG_SW1		0x0404	
#define CPCAP_REG_SW2		0x0408	
#define CPCAP_REG_UCTM		0x040c	
#define CPCAP_REG_TOD1		0x0410	
#define CPCAP_REG_TOD2		0x0414	
#define CPCAP_REG_TODA1		0x0418	
#define CPCAP_REG_TODA2		0x041c	
#define CPCAP_REG_DAY		0x0420	
#define CPCAP_REG_DAYA		0x0424	
#define CPCAP_REG_VAL1		0x0428	
#define CPCAP_REG_VAL2		0x042c	

#define CPCAP_REG_SDVSPLL	0x0600	
#define CPCAP_REG_SI2CC1	0x0604	
#define CPCAP_REG_Si2CC2	0x0608	
#define CPCAP_REG_S1C1		0x060c	
#define CPCAP_REG_S1C2		0x0610	
#define CPCAP_REG_S2C1		0x0614	
#define CPCAP_REG_S2C2		0x0618	
#define CPCAP_REG_S3C		0x061c	
#define CPCAP_REG_S4C1		0x0620	
#define CPCAP_REG_S4C2		0x0624	
#define CPCAP_REG_S5C		0x0628	
#define CPCAP_REG_S6C		0x062c	
#define CPCAP_REG_VCAMC		0x0630	
#define CPCAP_REG_VCSIC		0x0634	
#define CPCAP_REG_VDACC		0x0638	
#define CPCAP_REG_VDIGC		0x063c	
#define CPCAP_REG_VFUSEC	0x0640	
#define CPCAP_REG_VHVIOC	0x0644	
#define CPCAP_REG_VSDIOC	0x0648	
#define CPCAP_REG_VPLLC		0x064c	
#define CPCAP_REG_VRF1C		0x0650	
#define CPCAP_REG_VRF2C		0x0654	
#define CPCAP_REG_VRFREFC	0x0658	
#define CPCAP_REG_VWLAN1C	0x065c	
#define CPCAP_REG_VWLAN2C	0x0660	
#define CPCAP_REG_VSIMC		0x0664	
#define CPCAP_REG_VVIBC		0x0668	
#define CPCAP_REG_VUSBC		0x066c	
#define CPCAP_REG_VUSBINT1C	0x0670	
#define CPCAP_REG_VUSBINT2C	0x0674	
#define CPCAP_REG_URT		0x0678	
#define CPCAP_REG_URM1		0x067c	
#define CPCAP_REG_URM2		0x0680	

#define CPCAP_REG_VAUDIOC	0x0800	
#define CPCAP_REG_CC		0x0804	
#define CPCAP_REG_CDI		0x0808	
#define CPCAP_REG_SDAC		0x080c	
#define CPCAP_REG_SDACDI	0x0810	
#define CPCAP_REG_TXI		0x0814	
#define CPCAP_REG_TXMP		0x0818	
#define CPCAP_REG_RXOA		0x081c	
#define CPCAP_REG_RXVC		0x0820	
#define CPCAP_REG_RXCOA		0x0824	
#define CPCAP_REG_RXSDOA	0x0828	
#define CPCAP_REG_RXEPOA	0x082c	
#define CPCAP_REG_RXLL		0x0830	
#define CPCAP_REG_A2LA		0x0834	
#define CPCAP_REG_MIPIS1	0x0838	
#define CPCAP_REG_MIPIS2	0x083c	
#define CPCAP_REG_MIPIS3	0x0840	
#define CPCAP_REG_LVAB		0x0844	

#define CPCAP_REG_CCC1		0x0a00	
#define CPCAP_REG_CRM		0x0a04	
#define CPCAP_REG_CCCC2		0x0a08	
#define CPCAP_REG_CCS1		0x0a0c	
#define CPCAP_REG_CCS2		0x0a10	
#define CPCAP_REG_CCA1		0x0a14	
#define CPCAP_REG_CCA2		0x0a18	
#define CPCAP_REG_CCM		0x0a1c	
#define CPCAP_REG_CCO		0x0a20	
#define CPCAP_REG_CCI		0x0a24	

#define CPCAP_REG_ADCC1		0x0c00	
#define CPCAP_REG_ADCC2		0x0c04	
#define CPCAP_REG_ADCD0		0x0c08	
#define CPCAP_REG_ADCD1		0x0c0c	
#define CPCAP_REG_ADCD2		0x0c10	
#define CPCAP_REG_ADCD3		0x0c14	
#define CPCAP_REG_ADCD4		0x0c18	
#define CPCAP_REG_ADCD5		0x0c1c	
#define CPCAP_REG_ADCD6		0x0c20	
#define CPCAP_REG_ADCD7		0x0c24	
#define CPCAP_REG_ADCAL1	0x0c28	
#define CPCAP_REG_ADCAL2	0x0c2c	

#define CPCAP_REG_USBC1		0x0e00	
#define CPCAP_REG_USBC2		0x0e04	
#define CPCAP_REG_USBC3		0x0e08	
#define CPCAP_REG_UVIDL		0x0e0c	
#define CPCAP_REG_UVIDH		0x0e10	
#define CPCAP_REG_UPIDL		0x0e14	
#define CPCAP_REG_UPIDH		0x0e18	
#define CPCAP_REG_UFC1		0x0e1c	
#define CPCAP_REG_UFC2		0x0e20	
#define CPCAP_REG_UFC3		0x0e24	
#define CPCAP_REG_UIC1		0x0e28	
#define CPCAP_REG_UIC2		0x0e2c	
#define CPCAP_REG_UIC3		0x0e30	
#define CPCAP_REG_USBOTG1	0x0e34	
#define CPCAP_REG_USBOTG2	0x0e38	
#define CPCAP_REG_USBOTG3	0x0e3c	
#define CPCAP_REG_UIER1		0x0e40	
#define CPCAP_REG_UIER2		0x0e44	
#define CPCAP_REG_UIER3		0x0e48	
#define CPCAP_REG_UIEF1		0x0e4c	
#define CPCAP_REG_UIEF2		0x0e50	
#define CPCAP_REG_UIEF3		0x0e54	
#define CPCAP_REG_UIS		0x0e58	
#define CPCAP_REG_UIL		0x0e5c	
#define CPCAP_REG_USBD		0x0e60	
#define CPCAP_REG_SCR1		0x0e64	
#define CPCAP_REG_SCR2		0x0e68	
#define CPCAP_REG_SCR3		0x0e6c	

#define CPCAP_REG_VMC		0x0eac	
#define CPCAP_REG_OWDC		0x0eb0	
#define CPCAP_REG_GPIO0		0x0eb4	

#define CPCAP_REG_GPIO1		0x0ebc	

#define CPCAP_REG_GPIO2		0x0ec4	

#define CPCAP_REG_GPIO3		0x0ecc	

#define CPCAP_REG_GPIO4		0x0ed4	

#define CPCAP_REG_GPIO5		0x0edc	

#define CPCAP_REG_GPIO6		0x0ee4	

#define CPCAP_REG_MDLC		0x1000	
#define CPCAP_REG_KLC		0x1004	
#define CPCAP_REG_ADLC		0x1008	
#define CPCAP_REG_REDC		0x100c	
#define CPCAP_REG_GREENC	0x1010	
#define CPCAP_REG_BLUEC		0x1014	
#define CPCAP_REG_CFC		0x1018	
#define CPCAP_REG_ABC		0x101c	
#define CPCAP_REG_BLEDC		0x1020	
#define CPCAP_REG_CLEDC		0x1024	

#define CPCAP_REG_OW1C		0x1200	
#define CPCAP_REG_OW1D		0x1204	
#define CPCAP_REG_OW1I		0x1208	
#define CPCAP_REG_OW1IE		0x120c	

#define CPCAP_REG_OW1		0x1214	

#define CPCAP_REG_OW2C		0x1220	
#define CPCAP_REG_OW2D		0x1224	
#define CPCAP_REG_OW2I		0x1228	
#define CPCAP_REG_OW2IE		0x122c	

#define CPCAP_REG_OW2		0x1234	

#define CPCAP_REG_OW3C		0x1240	
#define CPCAP_REG_OW3D		0x1244	
#define CPCAP_REG_OW3I		0x1248	
#define CPCAP_REG_OW3IE		0x124c	

#define CPCAP_REG_OW3		0x1254	
#define CPCAP_REG_GCAIC		0x1258	
#define CPCAP_REG_GCAIM		0x125c	
#define CPCAP_REG_LGDIR		0x1260	
#define CPCAP_REG_LGPU		0x1264	
#define CPCAP_REG_LGPIN		0x1268	
#define CPCAP_REG_LGMASK	0x126c	
#define CPCAP_REG_LDEB		0x1270	
#define CPCAP_REG_LGDET		0x1274	
#define CPCAP_REG_LMISC		0x1278	
#define CPCAP_REG_LMACE		0x127c	

#define CPCAP_REG_TEST		0x7c00	

#define CPCAP_REG_ST_TEST1	0x7d08	

#define CPCAP_REG_ST_TEST2	0x7d18	



static inline int cpcap_get_revision(struct device *dev,
				     struct regmap *regmap,
				     u16 *revision)
{
	unsigned int val;
	int ret;

	ret = regmap_read(regmap, CPCAP_REG_VERSC1, &val);
	if (ret) {
		dev_err(dev, "Could not read revision\n");

		return ret;
	}

	*revision = ((val >> 3) & 0x7) | ((val << 3) & 0x38);

	return 0;
}

static inline int cpcap_get_vendor(struct device *dev,
				   struct regmap *regmap,
				   u16 *vendor)
{
	unsigned int val;
	int ret;

	ret = regmap_read(regmap, CPCAP_REG_VERSC1, &val);
	if (ret) {
		dev_err(dev, "Could not read vendor\n");

		return ret;
	}

	*vendor = (val >> 6) & 0x7;

	return 0;
}

extern int cpcap_sense_virq(struct regmap *regmap, int virq);
