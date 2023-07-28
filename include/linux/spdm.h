/* SPDX-License-Identifier: GPL-2.0 */
/*
 * DMTF Security Protocol and Data Model (SPDM)
 * https://www.dmtf.org/dsp/DSP0274
 *
 * Copyright (C) 2021-22 Huawei
 *     Jonathan Cameron <Jonathan.Cameron@huawei.com>
 *
 * Copyright (C) 2022-23 Intel Corporation
 */

#ifndef _SPDM_H_
#define _SPDM_H_

#include <linux/types.h>

struct key;
struct device;
struct spdm_state;

enum measurement_op {
	MEASUREMENT_OP_QUERY,
	MEASUREMENT_OP_ONE,
	MEASUREMENT_OP_ALL,
};

#define MEASUREMENT_ATTR_SIGN	BIT(0)
#define MEASUREMENT_ATTR_RAW	BIT(1)

struct spdm_measurement_block {
	u8 index;
	u8 specification;
	u16 size;
	/* Variable measurement size up to 64k */
};

struct spdm_measurement_request {
	enum measurement_op op;
	u8 attribute;
	u8 block_no;
	u8 slot_id;
	void *priv;
	void (*cb)(size_t count, u8 *measurement, void *priv);
};

typedef int (spdm_transport)(void *priv, struct device *dev,
			     const void *request, size_t request_sz,
			     void *response, size_t response_sz);

struct spdm_state *spdm_create(struct device *dev, spdm_transport *transport,
			       void *transport_priv, u32 transport_sz,
			       struct key *keyring);

int spdm_authenticate(struct spdm_state *spdm_state);

int spdm_get_measurements(struct spdm_state *spdm_state,
			  struct spdm_measurement_request *m);

void spdm_await(struct spdm_state *spdm_state);

void spdm_destroy(struct spdm_state *spdm_state);

#endif
