#ifndef NETDEV_PCS_H
#define NETDEV_PCS_H

#include <linux/phy.h>
#include <linux/spinlock.h>
#include <linux/workqueue.h>

struct device_node;
struct ethtool_cmd;
struct net_device;

enum {
	MLO_PAUSE_NONE,
	MLO_PAUSE_ASYM = BIT(0),
	MLO_PAUSE_SYM = BIT(1),
	MLO_PAUSE_RX = BIT(2),
	MLO_PAUSE_TX = BIT(3),
	MLO_PAUSE_TXRX_MASK = MLO_PAUSE_TX | MLO_PAUSE_RX,
	MLO_PAUSE_AN = BIT(4),

	MLO_AN_PHY = 0,	
	MLO_AN_FIXED,	
	MLO_AN_SGMII,	
	MLO_AN_8023Z,	
};

static inline bool phylink_autoneg_inband(unsigned int mode)
{
	return mode == MLO_AN_SGMII || mode == MLO_AN_8023Z;
}

struct phylink_link_state {
	__ETHTOOL_DECLARE_LINK_MODE_MASK(advertising);
	__ETHTOOL_DECLARE_LINK_MODE_MASK(lp_advertising);
	phy_interface_t interface;	
	int speed;
	int duplex;
	int pause;
	unsigned int link:1;
	unsigned int an_enabled:1;
	unsigned int an_complete:1;
};

struct phylink_mac_ops {
	
	void (*validate)(struct net_device *ndev, unsigned long *supported,
			 struct phylink_link_state *state);

	
	int (*mac_link_state)(struct net_device *, struct phylink_link_state *);

	
	
	void (*mac_config)(struct net_device *ndev, unsigned int mode,
			   const struct phylink_link_state *state);

	
	void (*mac_an_restart)(struct net_device *ndev);

	void (*mac_link_down)(struct net_device *, unsigned int mode);
	void (*mac_link_up)(struct net_device *, unsigned int mode,
			    struct phy_device *);
};

struct phylink *phylink_create(struct net_device *, struct device_node *,
	phy_interface_t iface, const struct phylink_mac_ops *ops);
void phylink_destroy(struct phylink *);

int phylink_connect_phy(struct phylink *, struct phy_device *);
int phylink_of_phy_connect(struct phylink *, struct device_node *);
void phylink_disconnect_phy(struct phylink *);

void phylink_mac_change(struct phylink *, bool up);

void phylink_start(struct phylink *);
void phylink_stop(struct phylink *);

void phylink_ethtool_get_wol(struct phylink *, struct ethtool_wolinfo *);
int phylink_ethtool_set_wol(struct phylink *, struct ethtool_wolinfo *);

int phylink_ethtool_ksettings_get(struct phylink *,
				  struct ethtool_link_ksettings *);
int phylink_ethtool_ksettings_set(struct phylink *,
				  const struct ethtool_link_ksettings *);
int phylink_ethtool_nway_reset(struct phylink *);
void phylink_ethtool_get_pauseparam(struct phylink *,
				    struct ethtool_pauseparam *);
int phylink_ethtool_set_pauseparam(struct phylink *,
				   struct ethtool_pauseparam *);
int phylink_ethtool_get_module_info(struct phylink *, struct ethtool_modinfo *);
int phylink_ethtool_get_module_eeprom(struct phylink *,
				      struct ethtool_eeprom *, u8 *);
int phylink_init_eee(struct phylink *, bool);
int phylink_get_eee_err(struct phylink *);
int phylink_ethtool_get_eee(struct phylink *, struct ethtool_eee *);
int phylink_ethtool_set_eee(struct phylink *, struct ethtool_eee *);
int phylink_mii_ioctl(struct phylink *, struct ifreq *, int);

#define phylink_zero(bm) \
	bitmap_zero(bm, __ETHTOOL_LINK_MODE_MASK_NBITS)
#define __phylink_do_bit(op, bm, mode) \
	op(ETHTOOL_LINK_MODE_ ## mode ## _BIT, bm)

#define phylink_set(bm, mode)	__phylink_do_bit(__set_bit, bm, mode)
#define phylink_clear(bm, mode)	__phylink_do_bit(__clear_bit, bm, mode)
#define phylink_test(bm, mode)	__phylink_do_bit(test_bit, bm, mode)

void phylink_set_port_modes(unsigned long *bits);

#endif
